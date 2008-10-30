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
	Vector3f Position;
	ColourRGBA Colour;

	DeviceVertex(void);
	DeviceVertex(const DeviceVertex& p_deviceVertex);
	DeviceVertex(const Vector3f& p_vecPosition);
	DeviceVertex(const Vector3f& p_vecPosition, const ColourRGBA& p_colourRGBA);
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

	void DrawText(const TVector3<Real>& p_vecPosition,
		const String& p_strText);

	void DrawPoint(
		const TVector3<Real>& p_vecPosition);

	void DrawLine(
		const TVector3<Real>& p_vecStart,
		const TVector3<Real>& p_vecEnd);

	void DrawArrow(
		const TVector3<Real>& p_vecHead,
		const TVector3<Real>& p_vecTail);

	void DrawSphere(
		const TVector3<Real>& p_vecCentre,
		Real p_rRadius);

	void DrawBox(
		const TVector3<Real>& p_vecCentre,
		const TVector3<Real>& p_vecExtents);

	void DrawBox(
		const TMatrix3<Real>& p_matOrientation,
		const TVector3<Real>& p_vecCentre,
		const TVector3<Real>& p_vecExtents);
};
