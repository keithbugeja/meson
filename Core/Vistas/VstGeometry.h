//----------------------------------------------------------------------------------------------
//  Base geometry class for renderable objects.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstMaterial.h"
#include "VstRenderable.h"
#include "VstRenderDevice.h"
#include "VstBoundingVolume.h"
#include "VstVertexBufferGroup.h"
#include "VstVertexData.h"
#include "VstIndexData.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	/** The geometry subset describes how a subset of the vertices and indices in the geometry
		object is interpreted; the subset defines a primitive type for rendering the given vertices 
		through their respective indices.
	 */
	//----------------------------------------------------------------------------------------------
	class GeometrySubset
	{
	public:
		enum Flags
		{
			GSF_NONE	 = 0x0000
		};

	protected:
		IRenderDevice::PrimitiveType m_ePrimitiveType;

		uint m_uiIndexStart,
			 m_uiVertexStart,
			 m_uiVertexCount,
			 m_uiPrimitiveCount;

		dword m_dwFlags;

	public:
		GeometrySubset(void);
		GeometrySubset(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiVertexStart, uint p_uiVertexCount, uint p_uiIndexStart, uint p_uiPrimitiveCount);
		GeometrySubset(const GeometrySubset& p_geometrySubset);

		IRenderDevice::PrimitiveType GetPrimitiveType(void) const;
		void SetPrimitiveType(IRenderDevice::PrimitiveType p_ePrimitiveType);
		
		bool IsFlagSet(Flags p_eFlag) const;
		void SetFlag(Flags p_eFlag, bool p_bEnabled);

		uint GetIndexStart(void) const;
		uint GetVertexStart(void) const;
		uint GetVertexCount(void) const;
		uint GetPrimitiveCount(void) const;
		dword GetFlags(void) const;

		void SetIndexStart(uint p_uiPrimitiveStart);
		void SetVertexStart(uint p_uiVertexStart);
		void SetVertexCount(uint p_uiVertexCount);
		void SetPrimitiveCount(uint p_uiPrimitiveCount);
		void SetFlags(dword p_uiFlags);

		GeometrySubset& operator=(const GeometrySubset& p_geometrySubset);
		bool operator==(const GeometrySubset& p_geometrySubset);

		static uint GetPrimitiveCount(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiIndexCount);
	};

	typedef Meson::Common::Collections::TArrayList<GeometrySubset> GeometrySubsetList;
	typedef Meson::Common::TPointer<GeometrySubsetList> GeometrySubsetListPtr;

	typedef Meson::Common::Collections::TEnumerator<GeometrySubset>& GeometrySubsetEnumerator;

	//----------------------------------------------------------------------------------------------
	class Geometry : public Renderable
	{
		VISTAS_IMPLEMENT_RTTI(Geometry, Renderable);

	public:
		Geometry(void);
		Geometry(const Geometry& p_geometry);
		Geometry(const Meson::Common::Text::String& p_strId);

		virtual ~Geometry(void);

		void SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexChannelPtr p_pVertexChannel, IndexBufferPtr p_pIndexBuffer);
		void SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDeclarationPtr p_pVertexDeclaration, VertexBufferGroupPtr p_pVertexBufferGroup, IndexBufferPtr p_pIndexBuffer);
		void SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDeclarationPtr p_pVertexDeclaration, VertexBufferPtr p_pVertexBuffer, IndexBufferPtr p_pIndexBuffer);

		virtual void Draw(IRenderDevice* p_pRenderDevice);
		virtual void DrawSubset(IRenderDevice* p_pRenderDevice, uint p_uiSubset);

		void SetPrimitiveType(IRenderDevice::PrimitiveType p_ePrimitiveType);

		GeometrySubsetListPtr GetGeometrySubsetList(void);
		void SetGeometrySubsetList(GeometrySubsetListPtr p_pGeometrySubsetList);

		MaterialPtr GetMaterial(void);
		void SetMaterial(MaterialPtr p_pMaterial);

		virtual BoundingVolumePtr GetBoundingVolume(void);
		virtual void SetBoundingVolume(BoundingVolumePtr p_pBoundingVolume);

		virtual uint GetSubsetCount(void) const;

		Geometry& operator=(const Geometry& p_geometry);

	protected:
		GeometrySubsetListPtr	m_pGeometrySubsetList;
		BoundingVolumePtr		m_pBoundingVolume;
		MaterialPtr				m_pMaterial;
	};

	typedef Meson::Common::TPointer<Geometry> GeometryPtr;
	typedef Meson::Common::Collections::TArrayList<GeometryPtr> GeometryList;
	typedef Meson::Common::TPointer<GeometryList> GeometryListPtr;
	typedef Meson::Common::Collections::TEnumerator<GeometryPtr>& GeometryEnumerator;

Meson_Vistas_END