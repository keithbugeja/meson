//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <string.h>

#include "VstVertexData.h"
#include "VstColour.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// VertexDescriptor Section
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
VertexDescriptor::SemanticDescriptor VertexDescriptor::m_semanticDescriptorList[VDS_COUNT] =
{
	VDF_COLOUR,					VDT_COLOUR,		//	VDS_COLOUR
	VDF_DIFFUSE,				VDT_COLOUR,		//	VDS_DIFFUSE
	VDF_SPECULAR,				VDT_COLOUR,		//	VDS_SPECULAR
	VDF_POSITION,				VDT_FLOAT3,		//	VDS_POSITION
	VDF_NORMAL,					VDT_FLOAT3,		//	VDS_NORMAL
	VDF_BINORMAL,				VDT_FLOAT3,		//	VDS_BINORMAL
	VDF_TANGENT,				VDT_FLOAT3,		//	VDS_TANGENT
	VDF_TEXTURE_COORDINATES,	VDT_FLOAT1,		//	VDS_TEXCOORD1D
	VDF_TEXTURE_COORDINATES,	VDT_FLOAT2,		//	VDS_TEXCOORD2D
	VDF_TEXTURE_COORDINATES,	VDT_FLOAT3,		//	VDS_TEXCOORD3D
	VDF_TEXTURE_COORDINATES,	VDT_FLOAT4		//	VDS_TEXCOORD4D
};

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// VertexDescriptor::SemanticDetails
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
VertexDescriptor::SemanticDetails::SemanticDetails() 
{ 
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::SemanticDetails::SemanticDetails(uint p_uiStream, uint p_uiUsageIndex, uint p_uiOffset) 
	: m_uiStream(p_uiStream), 
	  m_uiUsageIndex(p_uiUsageIndex), 
	  m_uiOffset(p_uiOffset)
{ 
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::SemanticDetails::SemanticDetails(const SemanticDetails& p_semanticDetails)
	: m_uiStream(p_semanticDetails.m_uiStream),
	  m_uiUsageIndex(p_semanticDetails.m_uiUsageIndex),
	  m_uiOffset(p_semanticDetails.m_uiOffset)
{
}
//----------------------------------------------------------------------------------------------
bool VertexDescriptor::SemanticDetails::operator==(const SemanticDetails& p_semanticDetails) const
{
	return (m_uiStream == p_semanticDetails.m_uiStream && 
		    m_uiUsageIndex == p_semanticDetails.m_uiUsageIndex &&
		    m_uiOffset == p_semanticDetails.m_uiOffset);
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::SemanticDetails& VertexDescriptor::SemanticDetails::operator=(const SemanticDetails& p_semanticDetails)
{
	m_uiStream = p_semanticDetails.m_uiStream;
	m_uiUsageIndex = p_semanticDetails.m_uiUsageIndex;
	m_uiOffset = p_semanticDetails.m_uiOffset;
	
	return *this;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// VertexDescriptor::StreamInformation
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
VertexDescriptor::StreamInformation::StreamInformation()
	: m_uiStream(0), 
	  m_uiStreamSize(0)
 { 
	ResetSemanticCount(); 
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::StreamInformation::StreamInformation(uint p_uiStream, uint p_uiStreamSize)
	: m_uiStream(p_uiStream), 
	  m_uiStreamSize(p_uiStreamSize)
{ 
	ResetSemanticCount(); 
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::StreamInformation::StreamInformation(const StreamInformation& p_streamInformation)
	: m_uiStream(p_streamInformation.m_uiStream),
	  m_uiStreamSize(p_streamInformation.m_uiStreamSize)
{
	memcpy(m_uiSemanticCount, p_streamInformation.m_uiSemanticCount, VertexDescriptor::VDS_COUNT * sizeof(uint));
}
//----------------------------------------------------------------------------------------------
bool VertexDescriptor::StreamInformation::operator==(const StreamInformation& p_streamInformation) const
{
	if (p_streamInformation.m_uiStream != m_uiStream || p_streamInformation.m_uiStreamSize != m_uiStreamSize)
		return false;

	for (uint uiIndex = 0; uiIndex < VertexDescriptor::VDS_COUNT; uiIndex++)
	{
		if (p_streamInformation.m_uiSemanticCount[uiIndex] != m_uiSemanticCount[uiIndex])
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::StreamInformation& VertexDescriptor::StreamInformation::operator=(const StreamInformation& p_streamInformation)
{
	m_uiStream = p_streamInformation.m_uiStream;
	m_uiStreamSize = p_streamInformation.m_uiStreamSize;

	memcpy(m_uiSemanticCount, p_streamInformation.m_uiSemanticCount, VertexDescriptor::VDS_COUNT * sizeof(uint));

	return *this;
}
//----------------------------------------------------------------------------------------------
void VertexDescriptor::StreamInformation::ResetSemanticCount(void) 
{ 
	memset(m_uiSemanticCount, 0, VDS_COUNT * sizeof(uint)); 
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
VertexDescriptor::VertexDescriptor(void) 
{
	Reset();
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::VertexDescriptor(VertexDeclarationPtr p_pVertexDeclaration)
{
	RebuildDescriptor(p_pVertexDeclaration);
}
//----------------------------------------------------------------------------------------------
void VertexDescriptor::Reset(void)
{
	m_semanticMap.Clear();
	m_streamIndexMap.Clear();
	m_streamInformationList.Clear();

	m_uiVertexSize = 0;
	memset(m_uiGlobalSemanticCount, 0, VDS_COUNT * sizeof(uint));
}
//----------------------------------------------------------------------------------------------
VertexDescriptor::StreamInformation& VertexDescriptor::GetStreamInformation(uint p_uiStream)
{
	uint uiIndex;

	if (ContainsStream(p_uiStream, uiIndex))
		return m_streamInformationList(uiIndex);

	throw new MesonException("Cannot return stream information : Specified stream does not exist");
}
//----------------------------------------------------------------------------------------------
bool VertexDescriptor::ContainsStream(uint p_uiStream, uint& p_pIndexOut)
{
	return m_streamIndexMap.ContainsKey(p_uiStream, p_pIndexOut);
}
//----------------------------------------------------------------------------------------------
void VertexDescriptor::RebuildDescriptor(VertexDeclarationPtr p_pVertexDeclaration)
{
	SemanticDescriptor* pSemanticDescriptor = NULL;
	VertexElement*		pVertexElement		= NULL;

	StreamInformation	streamInformation;
	SemanticDetails		semanticDetails;
	UniqueSemantic		semantic;

	uint uiStream, 
		 uiStreamSemanticUsageCount;

	Reset();

	// Retrieve iterator for streams
	VertexDeclaration::StreamEnumerator streamEnumerator = p_pVertexDeclaration->GetStreamEnumerator();
	
	// Iterate through individual stream, updating each StreamInformation structure
	while(streamEnumerator.HasMoreElements())
	{
		uiStream = streamEnumerator.NextElement();
		
		streamInformation.m_uiStream = uiStream;
		streamInformation.m_uiStreamSize = p_pVertexDeclaration->GetStreamSize(uiStream);
		streamInformation.ResetSemanticCount();

		m_uiVertexSize += streamInformation.m_uiStreamSize;
		
		pSemanticDescriptor = m_semanticDescriptorList;

		for (uint uiIndex = 0; uiIndex < VDS_COUNT; uiIndex++, pSemanticDescriptor++)
		{
			uiStreamSemanticUsageCount = p_pVertexDeclaration->GetCount(uiStream, pSemanticDescriptor->m_eVertexDataFunction, pSemanticDescriptor->m_eVertexDataType);
			streamInformation.m_uiSemanticCount[uiIndex] = uiStreamSemanticUsageCount;

			for (uint uiUsage = 0; uiUsage < uiStreamSemanticUsageCount; uiUsage++)
			{
				if (p_pVertexDeclaration->Contains(uiStream, pSemanticDescriptor->m_eVertexDataFunction, pSemanticDescriptor->m_eVertexDataType, uiUsage, &pVertexElement))
				{
					semantic.Element1 = (Semantic)uiIndex;
					semantic.Element2 = m_uiGlobalSemanticCount[uiIndex];
					
					semanticDetails.m_uiStream = uiStream;
					semanticDetails.m_uiUsageIndex = uiUsage;
					semanticDetails.m_uiOffset = pVertexElement->GetOffset();

					m_semanticMap.Insert(semantic, semanticDetails);

					m_uiGlobalSemanticCount[uiIndex]++;
				}
			}
		}

		// Add stream information to list
		m_streamIndexMap.Insert(uiStream, (uint)m_streamInformationList.Size());
		m_streamInformationList.Add(streamInformation);
	}
}
//----------------------------------------------------------------------------------------------
void VertexDescriptor::ExportDescriptor(VertexDeclarationPtr p_pVertexDeclaration)
{
	TEnumerator<TKeyValuePair<UniqueSemantic, SemanticDetails>>& semanticMapEnumerator = m_semanticMap.GetEnumerator();
	TKeyValuePair<UniqueSemantic, SemanticDetails> semanticMapEntry;
	SemanticDescriptor* pSemanticDescriptor = NULL;

	UniqueSemantic semantic;
	SemanticDetails semanticDetails;

	while (semanticMapEnumerator.HasMoreElements())
	{
		semanticMapEntry = semanticMapEnumerator.NextElement();
		
		semantic = semanticMapEntry.Key;
		semanticDetails = semanticMapEntry.Value;

		pSemanticDescriptor = &m_semanticDescriptorList[semantic.Element1];
		p_pVertexDeclaration->AddVertexElement(semanticDetails.m_uiStream, semanticDetails.m_uiOffset, pSemanticDescriptor->m_eVertexDataType, pSemanticDescriptor->m_eVertexDataFunction, semanticDetails.m_uiUsageIndex);
	}
}
//----------------------------------------------------------------------------------------------
bool VertexDescriptor::GetSemanticBufferMap(Semantic p_eSemantic, uint p_uiChannel, uint& p_pIndexOut, uint& p_pOffsetOut)
{
	uint uiStreamIndex;
	SemanticDetails semanticDetails;

	UniqueSemantic semantic(p_eSemantic, p_uiChannel);
	
	if (m_semanticMap.ContainsKey(semantic, semanticDetails))
	{
		if (ContainsStream(semanticDetails.m_uiStream, uiStreamIndex))
		{
			p_pOffsetOut = semanticDetails.m_uiOffset;
			p_pIndexOut = uiStreamIndex;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
void VertexDescriptor::AddSemantic(Semantic p_eSemantic, uint p_uiStream)
{
	uint uiStreamIndex;
	StreamInformation streamInformation;
	
	if (!ContainsStream(p_uiStream, uiStreamIndex))
	{
		streamInformation.m_uiStreamSize = 0;
		streamInformation.m_uiStream = p_uiStream;
		streamInformation.ResetSemanticCount();

		uiStreamIndex = (uint)m_streamInformationList.Size();
		m_streamIndexMap.Insert(p_uiStream, uiStreamIndex);
		m_streamInformationList.Add(streamInformation);
	}

	UniqueSemantic semantic(p_eSemantic, m_uiGlobalSemanticCount[p_eSemantic]);
	SemanticDetails semanticDetails(p_uiStream, streamInformation.m_uiSemanticCount[p_eSemantic], m_streamInformationList(uiStreamIndex).m_uiStreamSize);

	m_semanticMap.Insert(semantic, semanticDetails);

	uint uiTypeSize = VertexDeclaration::SizeOf(m_semanticDescriptorList[p_eSemantic].m_eVertexDataType);

	m_streamInformationList(uiStreamIndex).m_uiStreamSize += uiTypeSize;
	m_streamInformationList(uiStreamIndex).m_uiSemanticCount[p_eSemantic]++;

	m_uiVertexSize += uiTypeSize;
	m_uiGlobalSemanticCount[p_eSemantic]++;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticCount(Semantic p_eSemantic, uint p_uiStream)
{
	uint uiIndex;

	if (ContainsStream(p_uiStream, uiIndex))
		return m_streamInformationList(uiIndex).m_uiSemanticCount[p_eSemantic];

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticCount(VertexDescriptor::Semantic p_eSemantic)
{
	return m_uiGlobalSemanticCount[p_eSemantic];
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticOffset(VertexDescriptor::Semantic p_eSemantic, uint p_uiChannel)
{
	UniqueSemantic semantic(p_eSemantic, p_uiChannel);
	
	if (m_semanticMap.ContainsKey(semantic))
		return m_semanticMap(semantic).m_uiOffset;

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticStream(VertexDescriptor::Semantic p_eSemantic, uint p_uiChannel)
{
	UniqueSemantic semantic(p_eSemantic, p_uiChannel);
	
	if (m_semanticMap.ContainsKey(semantic))
		return m_semanticMap(semantic).m_uiStream;

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticUsage(VertexDescriptor::Semantic p_eSemantic, uint p_uiChannel)
{
	UniqueSemantic semantic(p_eSemantic, p_uiChannel);
	
	if (m_semanticMap.ContainsKey(semantic))
		return m_semanticMap(semantic).m_uiUsageIndex;

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetStreamAt(uint p_uiIndex)
{
	if (p_uiIndex < m_streamInformationList.Size())
		return m_streamInformationList(p_uiIndex).m_uiStream;

	throw new MesonException("Cannot return stream at index : Index out of range", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetStreamSizeAt(uint p_uiIndex)
{
	if (p_uiIndex < m_streamInformationList.Size())
		return m_streamInformationList(p_uiIndex).m_uiStreamSize;

	throw new MesonException("Cannot return stream size at index : Index out of range", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetStreamSize(uint p_uiStream)
{
	uint uiIndex;

	if (ContainsStream(p_uiStream, uiIndex))
		return m_streamInformationList(uiIndex).m_uiStreamSize;

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetStreamCount(void) const
{
	return (uint)m_streamInformationList.Size();
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetVertexSize(void) const 
{
	return m_uiVertexSize;
}
//----------------------------------------------------------------------------------------------
uint VertexDescriptor::GetSemanticSize(Semantic p_eSemantic)
{
	return VertexDeclaration::SizeOf(m_semanticDescriptorList[p_eSemantic].m_eVertexDataType);
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Vertex Section
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
Vertex::Vertex(VertexDescriptorPtr p_pVertexDescriptor, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList)
	: m_pVertexDescriptor(p_pVertexDescriptor),
	  m_vertexDataList(p_vertexDataList),
	  m_bOwner(false)
{
}
//----------------------------------------------------------------------------------------------
Vertex::Vertex(VertexDescriptorPtr p_pVertexDescriptor)
	: m_pVertexDescriptor(p_pVertexDescriptor),
	  m_bOwner(true)
{
	Bind(*this);
}
//----------------------------------------------------------------------------------------------
Vertex::~Vertex(void)
{
	Release(*this);
}
//----------------------------------------------------------------------------------------------
Vertex& Vertex::operator= (const Vertex& p_vertex)
{
	// TODO: Decide whether to throw exception or leave as is with assertion.
	MESON_ASSERT(m_pVertexDescriptor == p_vertex.m_pVertexDescriptor, "Vertex descriptors differ - cannot perform assignment");

	Copy(p_vertex, *this);
	return *this;
}
//----------------------------------------------------------------------------------------------
Vertex& Vertex::operator-- (int p_nPostfix)
{
	Move(*this, -1);
	return *this;
}
//----------------------------------------------------------------------------------------------
Vertex& Vertex::operator++ (int p_nPostfix)
{
	Move(*this, 1);
	return *this;
}
//----------------------------------------------------------------------------------------------
void Vertex::Bind(Vertex& p_vertex, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList)
{
	p_vertex.m_vertexDataList = p_vertexDataList;
	m_bOwner = false;
}
//----------------------------------------------------------------------------------------------
void Vertex::Bind(Vertex& p_vertex)
{
	uint uiStreamCount = p_vertex.m_pVertexDescriptor->GetStreamCount();

	for (uint uiIndex = 0; uiIndex < uiStreamCount; uiIndex++)
		p_vertex.m_vertexDataList.Add(new byte[p_vertex.m_pVertexDescriptor->GetStreamSizeAt(uiIndex)]);

	m_bOwner = true;
}
//----------------------------------------------------------------------------------------------
void Vertex::Release(Vertex& p_vertex)
{
	if (m_bOwner) 
	{
		for (uint uiIndex = 0; uiIndex < m_vertexDataList.Size(); uiIndex++)
			SAFE_DELETE(m_vertexDataList(uiIndex));

		m_vertexDataList.Clear();
	}
}
//----------------------------------------------------------------------------------------------
void Vertex::Copy(const Vertex& p_srcVertex, Vertex& p_dstVertex)
{
	if (p_srcVertex.m_pVertexDescriptor == p_dstVertex.m_pVertexDescriptor)
	{
		uint uiSize = (uint)p_srcVertex.m_vertexDataList.Size();

		for (uint uiIndex = 0; uiIndex < uiSize; uiIndex++)
			memcpy(p_dstVertex.m_vertexDataList(uiIndex), 
				   p_srcVertex.m_vertexDataList(uiIndex), 
				   p_srcVertex.m_pVertexDescriptor->GetStreamSizeAt(uiIndex)); 
	}
}
//----------------------------------------------------------------------------------------------
void Vertex::Move(Vertex& p_vertex, int p_nOffset)
{
	for (uint uiIndex = 0; uiIndex < p_vertex.m_vertexDataList.Size(); uiIndex++)
		p_vertex.m_vertexDataList[uiIndex] = p_vertex.m_vertexDataList(uiIndex) + p_vertex.m_pVertexDescriptor->GetStreamSizeAt(uiIndex) * p_nOffset;
}
//----------------------------------------------------------------------------------------------
void Vertex::TransferSemantic(VertexDescriptor::Semantic p_eSemantic, uint p_uiChannel, Vertex& p_vertex)
{
	uint uiSrcOffset, uiSrcIndex,
		 uiDstOffset, uiDstIndex;

	m_pVertexDescriptor->GetSemanticBufferMap(p_eSemantic, p_uiChannel, uiSrcIndex, uiSrcOffset);
	p_vertex.m_pVertexDescriptor->GetSemanticBufferMap(p_eSemantic, p_uiChannel, uiDstIndex, uiDstOffset);

	memcpy(p_vertex.m_vertexDataList(uiDstIndex) + uiDstOffset, m_vertexDataList(uiSrcIndex) + uiSrcOffset, m_pVertexDescriptor->GetSemanticSize(p_eSemantic));
}
//----------------------------------------------------------------------------------------------
void Vertex::Rebind(VertexDescriptorPtr p_pVertexDescriptor)
{
	m_pVertexDescriptor = p_pVertexDescriptor;

	Release(*this);
	Bind(*this);
}
//----------------------------------------------------------------------------------------------
void Vertex::Rebind(VertexDescriptorPtr p_pVertexDescriptor, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList)
{
	m_pVertexDescriptor = p_pVertexDescriptor;

	Release(*this);
	Bind(*this, p_vertexDataList);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasColour(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_COLOUR) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasPosition(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_POSITION) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasNormal(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_NORMAL) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasBinormal(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_BINORMAL) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasTangent(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TANGENT) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasTextureCoordinates1D(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD1D) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasTextureCoordinates2D(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD2D) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasTextureCoordinates3D(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD3D) > 0);
}
//----------------------------------------------------------------------------------------------
bool Vertex::HasTextureCoordinates4D(void) const {
	return (m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD4D) > 0);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetColourCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_COLOUR);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetPositionCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_POSITION);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetNormalCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_NORMAL);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetBinormalCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_BINORMAL);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetTangentCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TANGENT);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetTextureCoordinates1DCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD1D);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetTextureCoordinates2DCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD2D);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetTextureCoordinates3DCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD3D);
}
//----------------------------------------------------------------------------------------------
uint Vertex::GetTextureCoordinates4DCount(void) const {
	return m_pVertexDescriptor->GetSemanticCount(VertexDescriptor::VDS_TEXCOORD4D);
}
//----------------------------------------------------------------------------------------------
Colour& Vertex::GetColour(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetColourCount(), "Cannot return Colour component : Colour channel does not exist!");

	uint uiOffset, uiBuffer;
	
	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_COLOUR, p_uiChannel, uiBuffer, uiOffset);
	return *(Colour*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector3f& Vertex::GetPosition(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetPositionCount(), "Cannot return Position component : Position channel does not exist!");

	uint uiOffset, uiBuffer;
	
	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_POSITION, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector3f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector3f& Vertex::GetNormal(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetNormalCount(), "Cannot return Normal component : Normal channel does not exist!");

	uint uiOffset, uiBuffer;
	
	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_NORMAL, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector3f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector3f& Vertex::GetBinormal(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetBinormalCount(), "Cannot return Binormal component : Binormal channel does not exist!");

	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_BINORMAL, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector3f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector3f& Vertex::GetTangent(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetTangentCount(), "Cannot return Tangent component : Tangent channel does not exist!");

	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_TANGENT, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector3f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
float& Vertex::GetTextureCoordinates1D(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates1DCount(), "Cannot return 1D Texture Coordinates component : Channel does not exist!");

	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_TEXCOORD1D, p_uiChannel, uiBuffer, uiOffset);
	return *(float*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector2f& Vertex::GetTextureCoordinates2D(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates2DCount(), "Cannot return 2D Texture Coordinates component : Channel does not exist!");

	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_TEXCOORD2D, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector2f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector3f& Vertex::GetTextureCoordinates3D(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates3DCount(), "Cannot return 3D Texture Coordinates component : Channel does not exist!");

	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_TEXCOORD3D, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector3f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
Vector4f& Vertex::GetTextureCoordinates4D(uint p_uiChannel) 
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates4DCount(), "Cannot return 4D Texture Coordinates component : Channel does not exist!");
	
	uint uiOffset, uiBuffer;

	m_pVertexDescriptor->GetSemanticBufferMap(VertexDescriptor::VDS_TEXCOORD4D, p_uiChannel, uiBuffer, uiOffset);
	return *(Vector4f*)(m_vertexDataList(uiBuffer) + uiOffset);
}
//----------------------------------------------------------------------------------------------
void Vertex::SetColour(const Colour& p_colour, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetColourCount(), "Cannot set Colour component : Colour channel does not exist!");
	GetColour() = p_colour;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetPosition(const Vector3f& p_vec3Position, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetPositionCount(), "Cannot set Position component : Position channel does not exist!");
	GetPosition() = p_vec3Position;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetNormal(const Vector3f& p_vec3Normal, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetNormalCount(), "Cannot set Normal component : Normal channel does not exist!");
	GetNormal() = p_vec3Normal;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetBinormal(const Vector3f& p_vec3Binormal, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetBinormalCount(), "Cannot set Binormal component : Binormal channel does not exist!");
	GetBinormal() = p_vec3Binormal;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetTangent(const Vector3f& p_vec3Tangent, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetTangentCount(), "Cannot set Tangent component : Tangent channel does not exist!");
	GetTangent() = p_vec3Tangent;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetTextureCoordinates1D(float p_fTextureCoordinates, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates1DCount(), "Cannot set 1D Texture Coordinates component : Channel does not exist!");
	GetTextureCoordinates1D() = p_fTextureCoordinates;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetTextureCoordinates2D(const Vector2f& p_vec2TextureCoordinates, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates2DCount(), "Cannot set 2D Texture Coordinates component : Channel does not exist!");
	GetTextureCoordinates2D() = p_vec2TextureCoordinates;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetTextureCoordinates3D(const Vector3f& p_vec3TextureCoordinates, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates3DCount(), "Cannot set 3D Texture Coordinates component : Channel does not exist!");
	GetTextureCoordinates3D() = p_vec3TextureCoordinates;
}
//----------------------------------------------------------------------------------------------
void Vertex::SetTextureCoordinates4D(const Vector4f& p_vec4TextureCoordinates, uint p_uiChannel)
{
	MESON_ASSERT(p_uiChannel < GetTextureCoordinates4DCount(), "Cannot set 4D Texture Coordinates component : Channel does not exist!");
	GetTextureCoordinates4D() = p_vec4TextureCoordinates;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// VertexData
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
VertexData::VertexData(VertexDescriptorPtr p_pVertexDescriptor)
	: m_pVertexDescriptor(p_pVertexDescriptor)
	, m_bIsStale(true)
{
	Build(p_pVertexDescriptor, 0);
}
//----------------------------------------------------------------------------------------------
VertexData::VertexData(VertexDescriptorPtr p_pVertexDescriptor, uint p_uiInitialVertexCount)
	: m_pVertexDescriptor(p_pVertexDescriptor)
	, m_bIsStale(true)
{
	Build(p_pVertexDescriptor, p_uiInitialVertexCount);
}
//----------------------------------------------------------------------------------------------
VertexData::~VertexData(void)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex) = NULL;

	m_pVertexDescriptor = NULL;
}
//----------------------------------------------------------------------------------------------
void VertexData::Build(VertexDescriptorPtr p_pVertexDescriptor, uint p_uiVertexCount)
{
	uint uiStreamCount = p_pVertexDescriptor->GetStreamCount(), 
		 uiStreamSize;

	ArrayBufferPtr pArrayBuffer(NULL);

	for (uint uiStream = 0; uiStream < uiStreamCount; uiStream++)
	{
		uiStreamSize = p_pVertexDescriptor->GetStreamSizeAt(uiStream);
		pArrayBuffer = new ArrayBuffer(uiStreamSize, p_uiVertexCount, p_uiVertexCount + 3);
		m_pVertexArrayList.Add(pArrayBuffer);
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::RefreshVertexCounts(void)
{
	m_uiMaximumVertexCount = 0;
	m_uiMinimumVertexCount = MAX_INT32;

	uint uiVertexCount = 0;

	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
	{
		uiVertexCount = m_pVertexArrayList(uiIndex)->Size();

		m_uiMaximumVertexCount = MAX(m_uiMaximumVertexCount, uiVertexCount);
		m_uiMinimumVertexCount = MIN(m_uiMinimumVertexCount, uiVertexCount);
	}

	m_bIsStale = false;
}
//----------------------------------------------------------------------------------------------
void VertexData::Reorganise(VertexDescriptorPtr p_pVertexDescriptor)
{
	// Build new array
	uint uiVertexCount = GetMinimumVertexCount(),
		 uiStreamCount = p_pVertexDescriptor->GetStreamCount(), 
		 uiStreamSize;

	ArrayBufferPtr pArrayBuffer(NULL);
	TArrayList<ArrayBufferPtr> pVertexArrayList;

	for (uint uiStream = 0; uiStream < uiStreamCount; uiStream++)
	{
		uiStreamSize = p_pVertexDescriptor->GetStreamSizeAt(uiStream);
		pArrayBuffer = new ArrayBuffer(uiStreamSize, uiVertexCount, uiVertexCount);
		pVertexArrayList.Add(pArrayBuffer);
	}

	// Copy vertices
	Vertex vertexSrc(m_pVertexDescriptor),
		   vertexDst(p_pVertexDescriptor);

	uint uiDstSemanticCount,
		 uiSrcSemanticCount,
		 uiSemanticCount;

	TArrayList<byte*> srcArray, 
		              dstArray;

	for (uint uiBufferIndex = 0; uiBufferIndex < m_pVertexArrayList.Size(); uiBufferIndex++)
	{
		srcArray.Add((byte*)m_pVertexArrayList(uiBufferIndex)->ElementAt(0));
		dstArray.Add((byte*)pVertexArrayList(uiBufferIndex)->ElementAt(0));
	}

	vertexSrc.Rebind(m_pVertexDescriptor, srcArray);
	vertexDst.Rebind(p_pVertexDescriptor, dstArray);
		
	for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
	{
		for (uint uiIdx = 0; uiIdx < VertexDescriptor::VDS_COUNT; uiIdx++)
		{
			uiSrcSemanticCount = m_pVertexDescriptor->GetSemanticCount((VertexDescriptor::Semantic)uiIdx);
			uiDstSemanticCount = p_pVertexDescriptor->GetSemanticCount((VertexDescriptor::Semantic)uiIdx);

			uiSemanticCount = MIN(uiDstSemanticCount, uiSrcSemanticCount);
		
			for (uint uiChannel = 0; uiChannel < uiSemanticCount; uiChannel++)
				vertexSrc.TransferSemantic((VertexDescriptor::Semantic)uiIdx, uiChannel, vertexDst);
		}

		vertexSrc++;
		vertexDst++;
	}

	m_pVertexDescriptor = p_pVertexDescriptor;
	
}
//----------------------------------------------------------------------------------------------
const VertexDescriptorPtr VertexData::GetVertexDescriptor(void)
{
	return m_pVertexDescriptor;
}
//----------------------------------------------------------------------------------------------
VertexPtr VertexData::GetVertex(uint p_uiVertexIndex)
{
	TArrayList<byte*> bufferList;
	
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		bufferList.Add((byte*)m_pVertexArrayList(uiIndex)->ElementAt(p_uiVertexIndex));

	VertexPtr pVertex(new Vertex(m_pVertexDescriptor, bufferList));
	return pVertex;
}
//----------------------------------------------------------------------------------------------
void VertexData::SetVertex(uint p_uiVertexIndex, const Vertex& p_vertex)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->Set(p_uiVertexIndex, p_vertex.m_vertexDataList(uiIndex));
}
//----------------------------------------------------------------------------------------------
void VertexData::Swap(uint p_uiVertexIndex1, uint p_uiVertexIndex2)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->Swap(p_uiVertexIndex1, p_uiVertexIndex2);
}
//----------------------------------------------------------------------------------------------
void VertexData::Add(const Vertex& p_vertex)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->Add(p_vertex.m_vertexDataList(uiIndex));
}
//----------------------------------------------------------------------------------------------
void VertexData::Insert(uint p_uiVertexIndex, const Vertex& p_vertex)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->Insert(p_uiVertexIndex, p_vertex.m_vertexDataList(uiIndex));
}
//----------------------------------------------------------------------------------------------
void VertexData::RemoveAt(uint p_uiVertexIndex)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->RemoveAt(p_uiVertexIndex);
}
//----------------------------------------------------------------------------------------------
void VertexData::RemoveAll(void)
{
	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		m_pVertexArrayList(uiIndex)->Clear();
}
//----------------------------------------------------------------------------------------------
// TODO: Clean up following functions
void* VertexData::GetBuffer(uint p_uiBufferIndex, uint p_uiVertexIndex)
{
	MESON_ASSERT(p_uiBufferIndex < m_pVertexArrayList.Size(), "Cannot return buffer : invalid buffer index.");
	MESON_ASSERT(p_uiVertexIndex < m_pVertexArrayList[p_uiBufferIndex]->Size(), "Cannot return buffer : invalid vertex index.");
	
	return m_pVertexArrayList(p_uiBufferIndex)->ElementAt(p_uiVertexIndex);
}
//----------------------------------------------------------------------------------------------
TArrayList<void*> VertexData::GetBuffer(uint p_uiVertexIndex)
{
	TArrayList<void*> arrayBufferList;

	for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
	{
		MESON_ASSERT(p_uiVertexIndex < m_pVertexArrayList[uiIndex]->Size(), "Cannot return buffer list : invalid vertex index.");
		arrayBufferList.Add(m_pVertexArrayList(uiIndex)->ElementAt(p_uiVertexIndex));
	}

	return arrayBufferList;
}
//----------------------------------------------------------------------------------------------
void VertexData::SetBuffer(uint p_uiBufferIndex, uint p_uiVertexIndex, uint p_uiVertexCount, const void* p_pBuffer)
{
	if (p_uiVertexCount > 0)
	{
		MESON_ASSERT(p_uiBufferIndex < m_pVertexArrayList.Size(), "Cannot set buffer : invalid buffer index.");
		MESON_ASSERT(p_uiVertexIndex + p_uiVertexCount > m_pVertexArrayList[p_uiBufferIndex]->Size(), "Cannot set buffer : vertex index and count exceed buffer size.");
		
		m_pVertexArrayList(p_uiBufferIndex)->Set(p_uiVertexIndex, (byte*)p_pBuffer, p_uiVertexCount);
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::SetBuffer(uint p_uiVertexIndex, uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList)
{
	if (p_uiVertexCount > 0)
	{
		for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		{
			MESON_ASSERT(p_uiVertexIndex + p_uiVertexCount < m_pVertexArrayList[uiIndex]->Size(), "Cannot set buffer : vertex index and count exceed buffer size.");
			m_pVertexArrayList(uiIndex)->Set(p_uiVertexIndex, p_bufferList(uiIndex), p_uiVertexCount);
		}
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::AddBuffer(uint p_uiBufferIndex, uint p_uiVertexCount, const void* p_pBuffer)
{
	if (p_uiVertexCount > 0)
	{
		MESON_ASSERT(p_uiBufferIndex < m_pVertexArrayList.Size(), "Cannot add buffer : invalid buffer index.");
		m_pVertexArrayList(p_uiBufferIndex)->Add((byte*)p_pBuffer, p_uiVertexCount);
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::AddBuffer(uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList)
{
	if (p_uiVertexCount > 0)
	{
		for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		{
			m_pVertexArrayList(uiIndex)->Add((byte*)p_bufferList(uiIndex), p_uiVertexCount);
		}
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::InsertBuffer(uint p_uiBufferIndex, uint p_uiVertexIndex, uint p_uiVertexCount, const void* p_pBuffer)
{
	if (p_uiVertexCount > 0)
	{
		MESON_ASSERT(p_uiBufferIndex < m_pVertexArrayList.Size(), "Cannot insert buffer : invalid buffer index.");
		MESON_ASSERT(p_uiVertexIndex < m_pVertexArrayList(p_uiBufferIndex)->Size(), "Cannot insert buffer : invalid vertex index.");
		m_pVertexArrayList(p_uiBufferIndex)->Insert(p_uiVertexIndex, (byte*)p_pBuffer, p_uiVertexCount);
	}
}
//----------------------------------------------------------------------------------------------
void VertexData::InsertBuffer(uint p_uiVertexIndex, uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList)
{
	if (p_uiVertexCount > 0)
	{
		for (uint uiIndex = 0; uiIndex < m_pVertexArrayList.Size(); uiIndex++)
		{
			MESON_ASSERT(p_uiVertexIndex < m_pVertexArrayList(uiIndex)->Size(), "Cannot insert buffer : index exceeds size");
			m_pVertexArrayList(uiIndex)->Insert(p_uiVertexIndex, (byte*)p_bufferList(uiIndex), p_uiVertexCount);
		}
	}
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetMaximumVertexCount(void)
{
	if (m_bIsStale)
		RefreshVertexCounts();

	return m_uiMaximumVertexCount;
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetMinimumVertexCount(void)
{
	if (m_bIsStale)
		RefreshVertexCounts();

	return m_uiMinimumVertexCount;
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetVertexCount(uint p_uiBufferIndex)
{
	MESON_ASSERT(p_uiBufferIndex < m_pVertexArrayList.Size(), "Cannot return vertex count : buffer index exceeds buffer count.");
	return m_pVertexArrayList(p_uiBufferIndex)->Size();
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetVertexSize(void) const
{
	return m_pVertexDescriptor->GetVertexSize();
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetPartialVertexSize(uint p_uiBufferIndex) const
{
	return m_pVertexDescriptor->GetStreamSizeAt(p_uiBufferIndex);
}
//----------------------------------------------------------------------------------------------
uint VertexData::GetBufferCount(void) const
{
	return (uint)m_pVertexArrayList.Size();
}
//----------------------------------------------------------------------------------------------
VertexPtr VertexData::operator[] (uint p_uiIndex)
{
	return GetVertex(p_uiIndex);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
