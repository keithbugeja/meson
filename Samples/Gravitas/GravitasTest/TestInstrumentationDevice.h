#pragma once

#include "GrvInstrumentationDevice.h"

#include "VstRenderDevice.h"

#include "Model3D.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Instrumentation;

struct DeviceVertex
{
	Point3f Position;
	ColourRGBA Colour;

	DeviceVertex(void);
	DeviceVertex(const DeviceVertex& p_deviceVertex);
	DeviceVertex(const Point3f& p_ptPosition);
	DeviceVertex(const Point3f& p_ptPosition, const ColourRGBA& p_colourRGBA);
	bool operator==(const DeviceVertex& p_deviceVertex) const;
};

struct DeviceState
{
	TMatrix4<Real> Transform;
	Meson::Gravitas::Instrumentation::Colour Colour;
	DrawMode::DrawMode DrawMode;
	bool operator==(const DeviceState& p_deviceState) const;
};

class TestInstrumentationDevice
	: public IInstrumentationDevice
{
private: // private const variables
	static const ushort SPHERE_SEGMENTS = 8;

private: // private variables
	TMatrix4<Real> m_matTransform;
	Meson::Gravitas::Instrumentation::Colour m_colour;
	Meson::Vistas::Colour m_colourVistas;
	DrawMode::DrawMode m_drawMode;
	TArrayList<DeviceState> m_listDeviceStates;

	Meson::Vistas::IRenderDevice* m_pRenderDevice;
	Meson::Vistas::VertexDeclarationPtr m_pVertexDeclaration;
	Meson::Vistas::VertexBufferPtr m_pVertexBufferLine;
	Meson::Vistas::VertexChannelPtr m_pVertexChannelLine;
	Meson::Vistas::IndexBufferPtr m_pIndexBufferLine;
	Meson::Vistas::VertexBufferPtr m_pVertexBufferSphere;
	Meson::Vistas::VertexChannelPtr m_pVertexChannelSphere;
	Meson::Vistas::IndexBufferPtr m_pIndexBufferSphere;
	Matrix4f m_matSphere;
	Meson::Vistas::VertexBufferPtr m_pVertexBufferBox;
	Meson::Vistas::VertexChannelPtr m_pVertexChannelBox;
	Meson::Vistas::IndexBufferPtr m_pIndexBufferBox;
	Matrix4f m_matAABB;

public:
	TestInstrumentationDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice);
	~TestInstrumentationDevice(void);

	const TMatrix4<Real>& GetTransform(void);
	void SetTransform(const TMatrix4<Real>& p_matTransform);

	const Meson::Gravitas::Instrumentation::Colour& GetColour(void);
	void SetColour(const Meson::Gravitas::Instrumentation::Colour& p_colour);

	void PushDeviceState(void);
	void PopDeviceState(void);

	DrawMode::DrawMode GetDrawMode(void);
	void SetDrawMode(DrawMode::DrawMode p_drawMode);

	void DrawText(const TPoint3<Real>& p_ptPosition,
		const String& p_strText);

	void DrawPoint(
		const TPoint3<Real>& p_ptPosition);

	void DrawLine(
		const TPoint3<Real>& p_ptStart,
		const TPoint3<Real>& p_ptEnd);

	void DrawArrow(
		const TPoint3<Real>& p_ptHead,
		const TPoint3<Real>& p_ptTail);

	void DrawSphere(
		const TPoint3<Real>& p_ptCentre,
		Real p_rRadius);

	void DrawBox(
		const TPoint3<Real>& p_ptCentre,
		const TVector3<Real>& p_vecExtents);

	void DrawBox(
		const TMatrix3<Real>& p_matOrientation,
		const TPoint3<Real>& p_ptCentre,
		const TVector3<Real>& p_vecExtents);
};
