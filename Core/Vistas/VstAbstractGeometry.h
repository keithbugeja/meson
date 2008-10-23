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
#include "VstRenderDevice.h"
#include "VstVertexBufferGroup.h"
#include "VstVertexData.h"
#include "VstIndexData.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class AbstractGeometry : public Meson::Common::TReferenceCounter<AbstractGeometry>
	{	
	public:
		AbstractGeometry(void);
		AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType);
		AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDescriptorPtr p_pVertexDescriptor);
		AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDataPtr p_pVertexData, IndexDataPtr p_pIndexData);

		virtual void SetPrimitiveType(IRenderDevice::PrimitiveType p_ePrimitiveType);
		virtual void SetVertexData(VertexDataPtr p_pVertexData);
		virtual void SetIndexData(IndexDataPtr p_pIndexData);

		virtual IRenderDevice::PrimitiveType GetPrimitiveType(void) const;
		virtual VertexDataPtr GetVertexData(void);
		virtual IndexDataPtr  GetIndexData(void);

	protected:
		IRenderDevice::PrimitiveType m_ePrimitiveType;

		VertexDataPtr m_pVertexData;
		IndexDataPtr m_pIndexData;
	};

	typedef Meson::Common::TPointer<AbstractGeometry> AbstractGeometryPtr;

Meson_Vistas_END