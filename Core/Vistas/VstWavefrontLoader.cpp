#include "VstWavefrontLoader.h"

#include "VstMeshForge.h"
#include "VstBoundingSphere.h"
#include "VstBoundingAxisAlignedBox.h"
#include "VstVistasEngine.h"

#include "FileInputStream.h"
#include "Converter.h"
#include "File.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
WavefrontFace::WavefrontFace() { }
//----------------------------------------------------------------------------------------------
WavefrontFace& WavefrontFace::operator=(const WavefrontFace& p_face)
{
	VertexIndex[0] = p_face.VertexIndex[0];
	VertexIndex[1] = p_face.VertexIndex[1];
	VertexIndex[2] = p_face.VertexIndex[2];
	NormalIndex[0] = p_face.NormalIndex[0];
	NormalIndex[1] = p_face.NormalIndex[1];
	NormalIndex[2] = p_face.NormalIndex[2];
	TexcrdIndex[0] = p_face.TexcrdIndex[0];
	TexcrdIndex[1] = p_face.TexcrdIndex[1];
	TexcrdIndex[2] = p_face.TexcrdIndex[2];

	return *this;
}
//----------------------------------------------------------------------------------------------
bool WavefrontFace::operator==(const WavefrontFace& p_face) 
{
	return VertexIndex[0] == p_face.VertexIndex[0] &&
		   VertexIndex[1] == p_face.VertexIndex[1] &&
		   VertexIndex[2] == p_face.VertexIndex[2] &&
		   NormalIndex[0] == p_face.NormalIndex[0] &&
		   NormalIndex[1] == p_face.NormalIndex[1] &&
		   NormalIndex[2] == p_face.NormalIndex[2] &&
		   TexcrdIndex[0] == p_face.TexcrdIndex[0] &&
		   TexcrdIndex[1] == p_face.TexcrdIndex[1] &&
		   TexcrdIndex[2] == p_face.TexcrdIndex[2];
}
//----------------------------------------------------------------------------------------------
qword WavefrontFace::GetHash(uint p_uiIndex)
{
	qword qwHash = 0;
	
	word *pVertexHash = (word*)&qwHash + 1,
		 *pNormalHash = (word*)&qwHash + 2,
		 *pTexcrdHash = (word*)&qwHash + 3;

	*pVertexHash = (word)(VertexIndex[p_uiIndex] & 0xFFFF);
	*pNormalHash = (word)(NormalIndex[p_uiIndex] & 0xFFFF);
	*pTexcrdHash = (word)(TexcrdIndex[p_uiIndex] & 0xFFFF);

	return qwHash;
}

//----------------------------------------------------------------------------------------------
WavefrontObject::WavefrontObject()
{
	ResetBaseIndices();
	ResetObject();
}
//----------------------------------------------------------------------------------------------
WavefrontObject::~WavefrontObject() { }
//----------------------------------------------------------------------------------------------
bool WavefrontObject::IsEmpty()
{
	return (m_faceList.Size() == 0 &&
			m_vertexList.Size() == 0 &&
			m_normalList.Size() == 0 &&
			m_texcrdList.Size() == 0);
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::ResetObject(void)
{
	m_strObjectName.Clear();
	m_strMaterialName.Clear();

	m_faceList.Clear();
	m_vertexList.Clear();
	m_normalList.Clear();
	m_texcrdList.Clear();
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::ResetBaseIndices(void)
{
	m_uiBaseVertexIndex = 1;
	m_uiBaseNormalIndex = 1;
	m_uiBaseTexcrdIndex = 1;
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::SetBaseIndices(void)
{
	m_uiBaseVertexIndex += (uint)m_vertexList.Size();
	m_uiBaseNormalIndex += (uint)m_normalList.Size();
	m_uiBaseTexcrdIndex += (uint)m_texcrdList.Size();
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::SetBaseIndices(uint p_uiBaseVertexIndex, uint p_uiBaseTexcrdIndex, uint p_uiBaseNormalIndex)
{
	m_uiBaseVertexIndex = p_uiBaseVertexIndex;
	m_uiBaseNormalIndex = p_uiBaseNormalIndex;
	m_uiBaseTexcrdIndex = p_uiBaseTexcrdIndex;
}
//----------------------------------------------------------------------------------------------
uint WavefrontObject::GetBaseVertexIndex(void) const {
	return m_uiBaseVertexIndex;
}
//----------------------------------------------------------------------------------------------
uint WavefrontObject::GetBaseNormalIndex(void) const {
	return m_uiBaseNormalIndex;
}
//----------------------------------------------------------------------------------------------
uint WavefrontObject::GetBaseTexcrdIndex(void) const {
	return m_uiBaseTexcrdIndex;
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::SetName(const Meson::Common::Text::String& p_strName)
{
	m_strObjectName = p_strName;
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::SetMaterialName(const Meson::Common::Text::String p_strMaterialName)
{
	m_strMaterialName = p_strMaterialName;
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::AddNormal(const Meson::Common::Maths::Vector3f& p_vec3Normal)
{
	m_normalList.Add(p_vec3Normal);
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::AddVertex(const Meson::Common::Maths::Vector3f& p_vec3Vertex)
{
	m_vertexList.Add(p_vec3Vertex);
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::AddTexcrd(const Meson::Common::Maths::Vector2f& p_vec2Texcrd)
{
	m_texcrdList.Add(p_vec2Texcrd);
}
//----------------------------------------------------------------------------------------------
void WavefrontObject::AddFace(const WavefrontFace& p_face)
{
	m_faceList.Add(p_face);
}
//----------------------------------------------------------------------------------------------
GeometryPtr WavefrontObject::ConvertToGeometry(void)
{
	AbstractMeshPtr pMesh = MeshForge::CreateAbstractMesh(MeshForge::MF_ENABLE_ALL);
	VertexDataPtr pVertexData = pMesh->GetVertexData();
	IndexDataPtr pIndexData = pMesh->GetIndexData();
	VertexDescriptorPtr pVertexDescriptor = pVertexData->GetVertexDescriptor();
	Vertex vertex(pVertexDescriptor);

	// Vertex colour to black
	vertex.SetColour(Colour::Black);

	THashMap<qword, uint> vertexIndexMap;

	// First create vertex data, factoring out similar vertices
	for (uint uiFaceIndex = 0; uiFaceIndex < m_faceList.Size(); uiFaceIndex++)
	{
		WavefrontFace& face = m_faceList.GetElement(uiFaceIndex);

		for (uint uiIndex = 0; uiIndex < 3; uiIndex++)
		{
			uint uiVertexIndex = 0;
			qword qwHash = face.GetHash(uiIndex);
			
			if (!vertexIndexMap.ContainsKey(qwHash, uiVertexIndex))
			{
				vertex.SetPosition(m_vertexList[face.VertexIndex[uiIndex] - m_uiBaseVertexIndex]);
				vertex.SetNormal(m_normalList[face.NormalIndex[uiIndex] - m_uiBaseNormalIndex]);
				vertex.SetTextureCoordinates2D(m_texcrdList[face.TexcrdIndex[uiIndex] - m_uiBaseTexcrdIndex]);
				
				uiVertexIndex = pVertexData->GetVertexCount(0);
				vertexIndexMap.Insert(qwHash, uiVertexIndex);

				pVertexData->Add(vertex);
			}

			pIndexData->Add(uiVertexIndex);
		}
	}

	// Update tangents and binormals
	pMesh->UpdateTangents(true);

	// Allocate hardware buffers for storing indices, vertices and descriptors
	IndexBufferPtr pIndexBuffer = MeshForge::CreateIndexBuffer(pIndexData);
	VertexBufferGroupPtr pVertexBufferGroup = MeshForge::CreateVertexBufferGroup(pVertexData);
	VertexDeclarationPtr pVertexDeclaration = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration(false);
	pVertexDescriptor->ExportDescriptor(pVertexDeclaration);

	// Create geometry object
	GeometryPtr pGeometry(new Geometry(m_strObjectName));
	pGeometry->SetGeometryParameters(IRenderDevice::PT_TRIANGLE_LIST, pVertexDeclaration, pVertexBufferGroup, pIndexBuffer);

	// Create an axis-aligned bounding box for geometry
	BoundingVolumePtr pBoundingVolume(new BoundingAxisAlignedBox());
	//BoundingVolumePtr pBoundingVolume(new BoundingSphere());
	pBoundingVolume->ComputeFromVertices(m_vertexList);
	pGeometry->SetBoundingVolume(pBoundingVolume);

	// Set material if valid
	MaterialPtr pMaterial(NULL);
	if (VistasEngine::GetInstance()->GetMaterialManager()->ContainsMaterial(m_strMaterialName, pMaterial))
		pGeometry->SetMaterial(pMaterial);

	VistasEngine::GetInstance()->Logger().Out << "Geometry object created...\n";
	VistasEngine::GetInstance()->Logger().Out << "  Name : [" << m_strObjectName << "]\n";
	VistasEngine::GetInstance()->Logger().Out << "  Material : [" << m_strMaterialName << "]\n";
	VistasEngine::GetInstance()->Logger().Out << "  Vertices (P,N,T) : [" << (uint)m_vertexList.Size();
	VistasEngine::GetInstance()->Logger().Out << ", " << (uint)m_normalList.Size();
	VistasEngine::GetInstance()->Logger().Out << ", " << (uint)m_texcrdList.Size() << "]\n";
	VistasEngine::GetInstance()->Logger().Out << "  Faces : [" << (uint)m_faceList.Size() << "]\n";

	return pGeometry;
}

//----------------------------------------------------------------------------------------------
WavefrontLoader::WavefrontLoader()
	: m_strName("Wavefront")
	, m_pBuffer(NULL)
	, m_uiBufferLength(0)
	, m_uiBufferPosition(0)
	, m_strCmdVertex("v")
	, m_strCmdNormal("vn")
	, m_strCmdTexCoords("vt")
	, m_strCmdFace("f")
	, m_strCmdObject("o")
	, m_strCmdMaterial("usemtl")
{
}
//----------------------------------------------------------------------------------------------
WavefrontLoader::~WavefrontLoader()
{
	SAFE_DELETE_ARRAY(m_pBuffer);
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& WavefrontLoader::GetType(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
bool WavefrontLoader::IsResourceLoaded(void) const
{
	return (m_pBuffer != NULL);
}
//----------------------------------------------------------------------------------------------
bool WavefrontLoader::LoadResource(const Meson::Common::Text::String& p_strResourcePath)
{
	if (IsResourceLoaded())
		UnloadResource();

	// Open resource file
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourcePath, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	File file(strResourcePath);
	m_uiBufferLength = (uint)file.GetSize();
	m_pBuffer = new char[m_uiBufferLength + 1];
	m_uiBufferPosition = 0;

	FileInputStream fileInputStream(strResourcePath);
	fileInputStream.Read(m_pBuffer, m_uiBufferLength);
	fileInputStream.Close();

	return true;
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::UnloadResource(void)
{
	SAFE_DELETE_ARRAY(m_pBuffer);

	m_uiBufferLength = 0;
	m_uiBufferPosition = 0;
}
//----------------------------------------------------------------------------------------------
GeometryPtr WavefrontLoader::LoadSingle(const Meson::Common::Text::String& p_strFilename)
{
	WavefrontObject wavefrontObject;
	LoadResource(p_strFilename);

	// Process file
	TArrayList<String> listTokens;

	while(!IsEndOfBuffer())
	{
		if (!TokeniseNextLine(listTokens))
			continue;

		if (listTokens[0] == m_strCmdObject)
		{
			//if (!wavefrontObject.IsEmpty())
			//{
			//	GeometryPtr pGeometry = wavefrontObject.ConvertToGeometry();
			//	
			//	wavefrontObject.SetBaseIndices();
			//	wavefrontObject.ResetObject();
			//}
			
			CmdObject(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdMaterial)
		{
			CmdMaterial(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdVertex)
		{
			CmdVertex(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdNormal)
		{
			CmdNormal(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdTexCoords)
		{
			CmdTextureCoordinates(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdFace)
		{
			CmdFace(listTokens, wavefrontObject);
		}
	}

	if (wavefrontObject.IsEmpty())
		return GeometryPtr(NULL);

	return wavefrontObject.ConvertToGeometry();
}
//----------------------------------------------------------------------------------------------
GeometryListPtr WavefrontLoader::Load(const Meson::Common::Text::String& p_strFilename)
{
	WavefrontObject wavefrontObject;

	GeometryListPtr pGeometryList(new GeometryList());
	TArrayList<String> listTokens;

	LoadResource(p_strFilename);

	while(!IsEndOfBuffer())
	{
		if (!TokeniseNextLine(listTokens))
			continue;

		if (listTokens[0] == m_strCmdObject)
		{
			if (!wavefrontObject.IsEmpty())
			{
				pGeometryList->Add(wavefrontObject.ConvertToGeometry());
				
				wavefrontObject.SetBaseIndices();
				wavefrontObject.ResetObject();
			}
			
			CmdObject(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdMaterial)
		{
			CmdMaterial(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdVertex)
		{
			CmdVertex(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdNormal)
		{
			CmdNormal(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdTexCoords)
		{
			CmdTextureCoordinates(listTokens, wavefrontObject);
		}
		else if (listTokens[0] == m_strCmdFace)
		{
			CmdFace(listTokens, wavefrontObject);
		}
	}

	if (!wavefrontObject.IsEmpty())
		pGeometryList->Add(wavefrontObject.ConvertToGeometry());

	return pGeometryList;
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdObject(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 2)
	{
		String strName(p_tokenList[1]);
		strName.Trim();
		p_wfObject.SetName(strName);
	}
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdMaterial(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 2)
	{
		String strMaterial(p_tokenList[1]);
		strMaterial.Trim();
		p_wfObject.SetMaterialName(strMaterial);
	}
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdVertex(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 4)
	{
		Vector3f vec3Position(Converter::ToFloat(p_tokenList[1]), Converter::ToFloat(p_tokenList[2]), Converter::ToFloat(p_tokenList[3]));
		p_wfObject.AddVertex(vec3Position);
	}
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdNormal(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 4)
	{
		Vector3f vec3Normal(Converter::ToFloat(p_tokenList[1]), Converter::ToFloat(p_tokenList[2]), Converter::ToFloat(p_tokenList[3]));
		p_wfObject.AddNormal(vec3Normal);
	}
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdTextureCoordinates(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 3)
	{
		Vector2f vec2Texcrds(Converter::ToFloat(p_tokenList[1]), 1.0f - Converter::ToFloat(p_tokenList[2]));
		p_wfObject.AddTexcrd(vec2Texcrds);
	}
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::CmdFace(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject)
{
	if (p_tokenList.Size() == 4)
	{
		WavefrontFace face;
		TArrayList<String> subTokenList;

		for (uint uiIndex = 0; uiIndex < 3; uiIndex++)
		{
			subTokenList.Clear();

			// Uncomment the following method if SplitTokens is to be removed.
			// Note that the SplitTokens method is a tad faster than the string counterpart
			//p_tokenList[uiIndex + 1].Split('/', subTokenList);
			SplitTokens('/', p_tokenList[uiIndex + 1], subTokenList);
				
			if (subTokenList.Size() != 3)
				continue;

			face.VertexIndex[uiIndex] = Converter::ToLong(subTokenList[0]);
			face.TexcrdIndex[uiIndex] = Converter::ToLong(subTokenList[1]);
			face.NormalIndex[uiIndex] = Converter::ToLong(subTokenList[2]);
		}

		p_wfObject.AddFace(face);
	}
}
//----------------------------------------------------------------------------------------------
bool WavefrontLoader::IsEndOfBuffer() const
{
	return (m_uiBufferPosition >= m_uiBufferLength);
}
//----------------------------------------------------------------------------------------------
bool WavefrontLoader::TokeniseNextLine(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenListOut)
{
	char ch;

	uint uiLineStart = m_uiBufferPosition,
		 uiLineLength = 0;

	while(m_uiBufferPosition < m_uiBufferLength)
	{
		ch = m_pBuffer[m_uiBufferPosition++];
		if (ch == '\n' || ch == '\r')
			break;
	}

	// Comment line or invalid line
	if ((uiLineLength = (m_uiBufferPosition - uiLineStart)) < 2 || m_pBuffer[uiLineStart] == '#')
		return false;

	// Clear token list and line string
	p_tokenListOut.Clear();
	m_strLine.Clear();

	// Set line string and tokenise it
	m_strLine.Append(m_pBuffer + uiLineStart, uiLineLength);

	// Uncomment the following method if SplitTokens is to be removed.
	// Note that the SplitTokens method is a tad faster than the string counterpart
	//m_strLine.Split(' ', p_tokenListOut);
	SplitTokens(' ', m_strLine, p_tokenListOut);

	return (p_tokenListOut.Size() != 0);
}
//----------------------------------------------------------------------------------------------
uint WavefrontLoader::Checksum(Meson::Common::Text::String& p_strString)
{
	uint uiChecksum = 0;

	char *pBuffer = p_strString.GetBuffer();
	size_t uiLength = p_strString.Size();

	for (uint uiIndex = 0; uiIndex < uiLength; uiIndex++, pBuffer++)
	{
		if (*pBuffer >= 48 && *pBuffer <= 57)
			uiChecksum  = uiChecksum * 10 + (*pBuffer - 48);
	}

	return uiChecksum;
}
//----------------------------------------------------------------------------------------------
void WavefrontLoader::SplitTokens(char p_cChar, String& p_strString, TArrayList<String>& p_tokenList)
{
	char *pStringBuffer = p_strString.GetBuffer(),
		 *pStringMarker = pStringBuffer;

	uint uiTokenStart = 0,
		 uiTokenLength = 0,
		 uiLength = (uint)p_strString.Size();
	
	for (uint uiIndex = 0;
		 uiIndex < uiLength; 
		 uiIndex++, pStringBuffer++)
	{
		if (*pStringBuffer == p_cChar)
		{
			uiTokenLength = uiIndex - uiTokenStart;

			if (uiTokenLength > 0)
			{
				p_tokenList.Add(String(pStringMarker, uiTokenLength));
				uiTokenStart = uiIndex + 1;
				pStringMarker = pStringBuffer + 1;
			}
		}
	}

	uiTokenLength = uiLength - uiTokenStart;

	if (uiTokenLength > 0)
	{
		p_tokenList.Add(String(pStringMarker, uiTokenLength));
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END