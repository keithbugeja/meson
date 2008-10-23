//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstAbstractGeometry.h"
#include "VstAttribute.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::Collections::TArrayList<Attribute> AttributeData;
	typedef Meson::Common::TPointer<AttributeData> AttributeDataPtr;

	class AbstractMesh : public AbstractGeometry
	{	
		friend class MeshForge;

	public:
		struct Face 
		{ 
			VertexPtr	 Vertex[3]; 
			Attribute*	 Attribute;

			// IsDegenerate()
			// Edge(n)
			// UpdateNormal(n)
		};

	public:
		AbstractMesh(void);
		AbstractMesh(VertexDescriptorPtr p_pVertexDescriptor);
		AbstractMesh(VertexDataPtr p_pVertexData, IndexDataPtr p_pIndexData);

		//virtual void AddMaterialReference(const MaterialPtr& p_pMaterial);
		//virtual void RemoveMaterialReference(const MaterialPtr& p_pMaterial);

		virtual AttributeDataPtr GetAttributeData(void);
		virtual void SetAttributeData(AttributeDataPtr p_pAttributeData);

		virtual uint GetIndex(uint p_uiIndex);
		virtual void SetIndex(uint p_uiIndex, uint p_uiValue);

		virtual const VertexPtr GetVertex(uint p_uiIndex);
		virtual void SetVertex(uint p_uiIndex, const Vertex& p_vertex);

		virtual const Attribute& GetAttribute(uint p_uiIndex);
		virtual void SetAttribute(uint p_uiIndex, const Attribute& p_attribute);

		virtual Face GetFace(uint p_uiFaceIndex);
		virtual void SetFace(uint p_uiFaceIndex, const Face& p_face);
		virtual uint GetFaceCount(void) const;

		virtual void UpdateNormals(void);
		virtual void UpdateTangents(bool p_bIncludeBinormals=true);

		// Optimisation
		//void UpdateNormal(uint p_uiFaceIndex);
		//void WeldVertices(float p_fWeldingRange);

	private:
		AttributeDataPtr m_pAttributeData;
	};

	typedef Meson::Common::TPointer<AbstractMesh> AbstractMeshPtr;

Meson_Vistas_END
