//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstAbstractMesh.h"
#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
AbstractMesh::AbstractMesh(void)
	: AbstractGeometry(IRenderDevice::PT_TRIANGLE_LIST)
{
}
//----------------------------------------------------------------------------------------------
AbstractMesh::AbstractMesh(VertexDescriptorPtr p_pVertexDescriptor)
	: AbstractGeometry(IRenderDevice::PT_TRIANGLE_LIST, p_pVertexDescriptor)
{
}
//----------------------------------------------------------------------------------------------
AbstractMesh::AbstractMesh(VertexDataPtr p_pVertexData, IndexDataPtr p_pIndexData)
	: AbstractGeometry(IRenderDevice::PT_TRIANGLE_LIST, p_pVertexData, p_pIndexData)
{
}
//----------------------------------------------------------------------------------------------
AttributeDataPtr AbstractMesh::GetAttributeData(void)
{
	return m_pAttributeData;
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::SetAttributeData(AttributeDataPtr p_pAttributeData)
{
	m_pAttributeData = p_pAttributeData;
}
//----------------------------------------------------------------------------------------------
uint AbstractMesh::GetIndex(uint p_uiIndex)
{
	return m_pIndexData->GetIndex(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::SetIndex(uint p_uiIndex, uint p_uiValue)
{
	m_pIndexData->Set(p_uiIndex, p_uiValue);
}
//----------------------------------------------------------------------------------------------
const VertexPtr AbstractMesh::GetVertex(uint p_uiIndex)
{
	return m_pVertexData->GetVertex(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::SetVertex(uint p_uiIndex, const Vertex& p_vertex)
{
	m_pVertexData->SetVertex(p_uiIndex, p_vertex);
}
//----------------------------------------------------------------------------------------------
const Attribute& AbstractMesh::GetAttribute(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_pAttributeData->Size(), "Cannot return attribute : index out of range.");
	return (*m_pAttributeData)(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::SetAttribute(uint p_uiIndex, const Attribute& p_attribute)
{
	MESON_ASSERT(p_uiIndex < m_pAttributeData->Size(), "Cannot set attribute : index out of range.");
	(*m_pAttributeData)(p_uiIndex) = p_attribute;
}
//----------------------------------------------------------------------------------------------
AbstractMesh::Face AbstractMesh::GetFace(uint p_uiFaceIndex)
{
	uint uiVertexIndex = p_uiFaceIndex * 3;

	Face face;

	face.Vertex[0] = m_pVertexData->GetVertex(m_pIndexData->GetIndex(uiVertexIndex));
	face.Vertex[1] = m_pVertexData->GetVertex(m_pIndexData->GetIndex(uiVertexIndex + 1));
	face.Vertex[2] = m_pVertexData->GetVertex(m_pIndexData->GetIndex(uiVertexIndex + 2));

	//(*face.Attribute) = (*m_pAttributeData)(p_uiFaceIndex);

	return face;
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::SetFace(uint p_uiFaceIndex, const AbstractMesh::Face& p_face)
{
	uint uiVertexIndex = p_uiFaceIndex * 3;

	m_pVertexData->SetVertex(m_pIndexData->GetIndex(uiVertexIndex), *(p_face.Vertex[0]));
	m_pVertexData->SetVertex(m_pIndexData->GetIndex(uiVertexIndex + 1), *(p_face.Vertex[1]));
	m_pVertexData->SetVertex(m_pIndexData->GetIndex(uiVertexIndex + 2), *(p_face.Vertex[2]));

	//(*m_pAttributeData)(p_uiFaceIndex) = (*p_face.Attribute);
}
//----------------------------------------------------------------------------------------------
uint AbstractMesh::GetFaceCount(void) const
{
	return m_pIndexData->GetIndexCount() / 3;
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::UpdateNormals(void)
{
	// Check for normal semantic before applying normal update
	if (m_pVertexData->GetVertexDescriptor()->GetSemanticCount(VertexDescriptor::VDS_NORMAL) > 0)
	{
		Vector3f vec3Edge1, vec3Edge2, vec3Normal;
		Face face;

		VertexPtr pVertex(NULL);

		uint uiVertexCount = m_pVertexData->GetMinimumVertexCount();

		// Clear all vertex normals
		for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
		{
			pVertex = m_pVertexData->GetVertex(uiVertexIndex);
			pVertex->SetNormal(Vector3f::Zero);
		}

		// Find normals
		for (uint uiFaceIndex = 0, uiFaceCount = GetFaceCount(); uiFaceIndex < uiFaceCount; uiFaceIndex++)
		{
			face = GetFace(uiFaceIndex);
			
			vec3Edge1 = face.Vertex[1]->GetPosition() - face.Vertex[0]->GetPosition();
			vec3Edge2 = face.Vertex[2]->GetPosition() - face.Vertex[0]->GetPosition();
			vec3Normal = vec3Edge1.CrossProduct(vec3Edge2);

			face.Vertex[0]->SetNormal(vec3Normal + face.Vertex[0]->GetNormal());
			face.Vertex[1]->SetNormal(vec3Normal + face.Vertex[1]->GetNormal());
			face.Vertex[2]->SetNormal(vec3Normal + face.Vertex[2]->GetNormal());
		}

		// Normalize all vertex normals
		for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
		{
			pVertex = m_pVertexData->GetVertex(uiVertexIndex);
			pVertex->GetNormal().Normalise();
		}
	}
}
//----------------------------------------------------------------------------------------------
void AbstractMesh::UpdateTangents(bool p_bIncludeBinormals)
{
	// Check for normal semantic before applying normal update
	if (m_pVertexData->GetVertexDescriptor()->GetSemanticCount(VertexDescriptor::VDS_TANGENT) > 0 &&
		m_pVertexData->GetVertexDescriptor()->GetSemanticCount(VertexDescriptor::VDS_NORMAL) > 0)
	{
		Vector3f vec3Edge1, vec3Edge2, vec3Tangent, vec3Binormal, vec3Normal;
		Vector2f vec2Edge1, vec2Edge2;
		Face face;

		VertexPtr pVertex(NULL);

		uint uiVertexCount = m_pVertexData->GetMinimumVertexCount();
		
		// Clear all vertex binormals/tangents
		for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
		{
			pVertex = m_pVertexData->GetVertex(uiVertexIndex);
			pVertex->SetBinormal(Vector3f::Zero);
			pVertex->SetTangent(Vector3f::Zero);
		}

		// Find normals
		for (uint uiFaceIndex = 0, uiFaceCount = GetFaceCount(); uiFaceIndex < uiFaceCount; uiFaceIndex++)
		{
			face = GetFace(uiFaceIndex);

			vec3Edge1 = face.Vertex[1]->GetPosition() - face.Vertex[0]->GetPosition();
			vec3Edge2 = face.Vertex[2]->GetPosition() - face.Vertex[0]->GetPosition();
			
			vec2Edge1 = face.Vertex[1]->GetTextureCoordinates2D() - face.Vertex[0]->GetTextureCoordinates2D();
			vec2Edge2 = face.Vertex[2]->GetTextureCoordinates2D() - face.Vertex[0]->GetTextureCoordinates2D();

			float r = 1.0f / (vec2Edge1.X * vec2Edge2.Y - vec2Edge2.X * vec2Edge1.Y);
			vec3Tangent.X = r * (vec2Edge2.Y * vec3Edge1.X - vec2Edge1.Y * vec3Edge2.X);
			vec3Tangent.Y = r * (vec2Edge2.Y * vec3Edge1.Y - vec2Edge1.Y * vec3Edge2.Y);
			vec3Tangent.Z = r * (vec2Edge2.Y * vec3Edge1.Z - vec2Edge1.Y * vec3Edge2.Z);

			for (uint uiVertexIndex = 0; uiVertexIndex < 3; uiVertexIndex++)
				face.Vertex[uiVertexIndex]->SetTangent(face.Vertex[uiVertexIndex]->GetTangent() + vec3Tangent);
		}

		if (p_bIncludeBinormals && 
			m_pVertexData->GetVertexDescriptor()->GetSemanticCount(VertexDescriptor::VDS_BINORMAL) > 0)
		{
			// Normalize all vertex tangent/binormals
			for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
			{
				pVertex = m_pVertexData->GetVertex(uiVertexIndex);
				
				vec3Normal = pVertex->GetNormal();
				vec3Tangent = pVertex->GetTangent();
				vec3Tangent = (vec3Tangent - vec3Normal * vec3Normal.DotProduct(vec3Tangent)).NormaliseCopy();
				vec3Binormal = vec3Normal.CrossProduct(vec3Tangent);
				
				pVertex->SetBinormal(vec3Binormal);
				pVertex->SetTangent(vec3Tangent);
			}
		}
		else
		{
			// Normalize all vertex tangent/binormals
			for (uint uiVertexIndex = 0; uiVertexIndex < uiVertexCount; uiVertexIndex++)
			{
				pVertex = m_pVertexData->GetVertex(uiVertexIndex);
				
				vec3Normal = pVertex->GetNormal();
				vec3Tangent = pVertex->GetTangent();
				vec3Tangent = (vec3Tangent - vec3Normal * vec3Normal.DotProduct(vec3Tangent)).NormaliseCopy();
				
				pVertex->SetTangent(vec3Tangent);
			}
		}
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
