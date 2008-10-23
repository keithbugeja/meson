#include "TestInstrumentationDevice.h"

bool DeviceState::operator==(const DeviceState& p_deviceState) const
{
	return Transform == p_deviceState.Transform
		&& Colour == p_deviceState.Colour
		&& DrawMode == p_deviceState.DrawMode;
}

DeviceVertex::DeviceVertex(void)
	: Position()
	, Colour(ColourRGBA::White)
{
}

DeviceVertex::DeviceVertex(const DeviceVertex& p_deviceVertex)
	: Position(p_deviceVertex.Position)
	, Colour(p_deviceVertex.Colour)
{
}

DeviceVertex::DeviceVertex(const Point3f& p_ptPosition)
	: Position(p_ptPosition)
	, Colour(ColourRGBA::White)
{
}

DeviceVertex::DeviceVertex(const Point3f& p_ptPosition, const ColourRGBA& p_colourRGBA)
	: Position(p_ptPosition)
	, Colour(p_colourRGBA)
{
}

bool DeviceVertex::operator==(const DeviceVertex& p_deviceVertex) const
{
	return Position == p_deviceVertex.Position
		&& Colour.ToRGBA() == p_deviceVertex.Colour.ToRGBA();
}

TestInstrumentationDevice::TestInstrumentationDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice)
	: m_matTransform(TMatrix4<Real>::Identity)
	, m_pRenderDevice(p_pRenderDevice)
	, m_drawMode(DrawMode::Outline)
	, m_colour(Meson::Gravitas::Instrumentation::Colour::White)
	, m_colourVistas(Meson::Vistas::Colour::White)
	, m_listDeviceStates()
	, m_pVertexDeclaration()
	, m_pVertexBufferLine()
	, m_pVertexChannelLine()
	, m_pIndexBufferLine()
	, m_pVertexBufferSphere()
	, m_pVertexChannelSphere()
	, m_pIndexBufferSphere()
	, m_matSphere()
	, m_pVertexBufferBox()
	, m_pVertexChannelBox()
	, m_pIndexBufferBox()
	, m_matAABB()
{
	HardwareResourceManager* pHardwareResourceManager
		= VistasEngine::GetInstance()->GetHardwareResourceManager();

	// prepare vertex declaration for drawing primitives
	m_pVertexDeclaration
		= pHardwareResourceManager->CreateVertexDeclaration();

	uint uiVertexOffset = 0;
	m_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT3, VDF_POSITION, 0);
	uiVertexOffset += VertexDeclaration::SizeOf(VDT_FLOAT3);
	m_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_COLOUR, VDF_COLOUR, 0);
	uiVertexOffset += VertexDeclaration::SizeOf(VDT_COLOUR);

	// line visual elements

	// prepare vertex buffer for line
	TArrayList<DeviceVertex> listVerticesLine;
	listVerticesLine.Add(DeviceVertex(Point3f(-1.0f,  0.0f,  0.0f)));
	listVerticesLine.Add(DeviceVertex(Point3f( 1.0f,  0.0f,  0.0f)));

	m_pVertexBufferLine = pHardwareResourceManager->CreateVertexBuffer(
		sizeof(DeviceVertex), (uint) listVerticesLine.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);
	m_pVertexBufferLine->Lock(HardwareResource::ML_NONE);
	m_pVertexBufferLine->Write(0, 0, sizeof(DeviceVertex) * (uint) listVerticesLine.Size(), (const byte*)&listVerticesLine[0]);
	m_pVertexBufferLine->Unlock();

	// prepare vertex channel for line
	m_pVertexChannelLine = pHardwareResourceManager->CreateVertexChannel();
	m_pVertexChannelLine->SetVertexDeclaration(m_pVertexDeclaration);
	m_pVertexChannelLine->SetStreamBinding(0, m_pVertexBufferLine);

	// prepare index buffer for line
	m_pIndexBufferLine = pHardwareResourceManager->CreateIndexBuffer(
		IndexBuffer::IT_16BIT, 2,
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	TArrayList<short> listBoxSegmentIndices;
	listBoxSegmentIndices.Add(0);
	listBoxSegmentIndices.Add(1);

	m_pIndexBufferLine->Lock(Meson::Vistas::HardwareResource::ML_NONE);
	m_pIndexBufferLine->Write(0, 0, IndexBuffer::IT_16BIT * 24, (const byte*)&listBoxSegmentIndices[0]);
	m_pIndexBufferLine->Unlock();

	// sphere visual elements

	// prepare vertex buffer for sphere
	TArrayList<DeviceVertex> listVerticesSphere;
	DeviceVertex deviceVertex;
	for (size_t unIndex1 = 0; unIndex1 <= SPHERE_SEGMENTS; unIndex1++)
	{
		float fAngleVertical
			= (float) unIndex1 * TMaths<float>::Pi / (float) SPHERE_SEGMENTS;
		deviceVertex.Position.Y = TMaths<float>::Cos(fAngleVertical);
		float fRadiusHorizontal = TMaths<float>::Sin(fAngleVertical);
		for (size_t unIndex2 = 0; unIndex2 < SPHERE_SEGMENTS * 2; unIndex2++)
		{
			float fAngleHorizontal = (float) unIndex2 * TMaths<float>::Pi
				/ (float) SPHERE_SEGMENTS;
			deviceVertex.Position.X
				= TMaths<float>::Cos(fAngleHorizontal) * fRadiusHorizontal;
			deviceVertex.Position.Z
				= TMaths<float>::Sin(fAngleHorizontal) * fRadiusHorizontal;
			listVerticesSphere.Add(deviceVertex);
		}
	}

	m_pVertexBufferSphere = pHardwareResourceManager->CreateVertexBuffer(
		sizeof(DeviceVertex), (uint) listVerticesSphere.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);
	m_pVertexBufferSphere->Lock(HardwareResource::ML_NONE);
	m_pVertexBufferSphere->Write(0, 0, sizeof(DeviceVertex) * (uint) listVerticesSphere.Size(), (const byte*)&listVerticesSphere[0]);
	m_pVertexBufferSphere->Unlock();

	// prepare vertex channel for sphere
	m_pVertexChannelSphere = pHardwareResourceManager->CreateVertexChannel();
	m_pVertexChannelSphere->SetVertexDeclaration(m_pVertexDeclaration);
	m_pVertexChannelSphere->SetStreamBinding(0, m_pVertexBufferSphere);

	// prepare index buffer for sphere
	TArrayList<short> listEdgesSphere;
	for (ushort unIndex1 = 0; unIndex1 < SPHERE_SEGMENTS; unIndex1++)
	{
		for (ushort unIndex2 = 0; unIndex2 < SPHERE_SEGMENTS * 2; unIndex2++)
		{
			ushort unIndex2Next = (unIndex2 + 1) % (SPHERE_SEGMENTS * 2);
			listEdgesSphere.Add(unIndex1 * (SPHERE_SEGMENTS * 2) + unIndex2);
			listEdgesSphere.Add(unIndex1 * (SPHERE_SEGMENTS * 2) + unIndex2Next);
			listEdgesSphere.Add(unIndex1 * (SPHERE_SEGMENTS * 2) + unIndex2);
			listEdgesSphere.Add((unIndex1 + 1) * (SPHERE_SEGMENTS * 2) + unIndex2);
		}
	}

	m_pIndexBufferSphere = pHardwareResourceManager->CreateIndexBuffer(
		IndexBuffer::IT_16BIT, (uint) listEdgesSphere.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	m_pIndexBufferSphere->Lock(Meson::Vistas::HardwareResource::ML_NONE);
	m_pIndexBufferSphere->Write(0, 0,
		IndexBuffer::IT_16BIT * (uint) listEdgesSphere.Size(),
		(const byte*)&listEdgesSphere[0]);
	m_pIndexBufferSphere->Unlock();

	// matrix for AABB box
	m_matSphere.MakeZero();
	m_matSphere._44 = 1.0f;

	// box visual elements

	// prepare vertex buffer for box
	TArrayList<DeviceVertex> listVerticesBox;
	listVerticesBox.Add(DeviceVertex(Point3f(-1.0f, -1.0f, -1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f(-1.0f, -1.0f,  1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f(-1.0f,  1.0f, -1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f(-1.0f,  1.0f,  1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f( 1.0f, -1.0f, -1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f( 1.0f, -1.0f,  1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f( 1.0f,  1.0f, -1.0f)));
	listVerticesBox.Add(DeviceVertex(Point3f( 1.0f,  1.0f,  1.0f)));

	m_pVertexBufferBox = pHardwareResourceManager->CreateVertexBuffer(
		sizeof(DeviceVertex), (uint) listVerticesBox.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	m_pVertexBufferBox->Lock(HardwareResource::ML_NONE);
	m_pVertexBufferBox->Write(0, 0, sizeof(DeviceVertex) * (uint) listVerticesBox.Size(), (const byte*)&listVerticesBox[0]);
	m_pVertexBufferBox->Unlock();

	// prepare vertex channel for box
	m_pVertexChannelBox = pHardwareResourceManager->CreateVertexChannel();
	m_pVertexChannelBox->SetVertexDeclaration(m_pVertexDeclaration);
	m_pVertexChannelBox->SetStreamBinding(0, m_pVertexBufferBox);

	// prepare index buffer for box
	TArrayList<short> listEdgesBox;
	listEdgesBox.Add(0);
	listEdgesBox.Add(1);
	listEdgesBox.Add(0);
	listEdgesBox.Add(2);
	listEdgesBox.Add(1);
	listEdgesBox.Add(3);
	listEdgesBox.Add(2);
	listEdgesBox.Add(3);
	listEdgesBox.Add(4);
	listEdgesBox.Add(5);
	listEdgesBox.Add(4);
	listEdgesBox.Add(6);
	listEdgesBox.Add(5);
	listEdgesBox.Add(7);
	listEdgesBox.Add(6);
	listEdgesBox.Add(7);
	listEdgesBox.Add(0);
	listEdgesBox.Add(4);
	listEdgesBox.Add(1);
	listEdgesBox.Add(5);
	listEdgesBox.Add(2);
	listEdgesBox.Add(6);
	listEdgesBox.Add(3);
	listEdgesBox.Add(7);

	m_pIndexBufferBox = pHardwareResourceManager->CreateIndexBuffer(
		IndexBuffer::IT_16BIT, (uint) listEdgesBox.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	m_pIndexBufferBox->Lock(Meson::Vistas::HardwareResource::ML_NONE);
	m_pIndexBufferBox->Write(0, 0,
		IndexBuffer::IT_16BIT * (uint) listEdgesBox.Size(),
		(const byte*)&listEdgesBox[0]);
	m_pIndexBufferBox->Unlock();

	// matrix for AABB box
	m_matAABB.MakeZero();
	m_matAABB._44 = 1.0f;
}

TestInstrumentationDevice::~TestInstrumentationDevice(void)
{
	HardwareResourceManager* pHardwareResourceManager
		= VistasEngine::GetInstance()->GetHardwareResourceManager();

	// box

	// release index buffer for box
	pHardwareResourceManager->ReleaseIndexBuffer(m_pIndexBufferBox);
	m_pIndexBufferBox = NULL;

	// release vertex channel for box
	pHardwareResourceManager->ReleaseVertexChannel(m_pVertexChannelBox);
	m_pVertexChannelBox = NULL;

	// release vertex buffer for box
	pHardwareResourceManager->ReleaseVertexBuffer(m_pVertexBufferBox);
	m_pVertexBufferBox = NULL;

	// sphere

	// release index buffer for sphere
	pHardwareResourceManager->ReleaseIndexBuffer(m_pIndexBufferSphere);
	m_pIndexBufferSphere = NULL;

	// release vertex channel for sphere
	pHardwareResourceManager->ReleaseVertexChannel(m_pVertexChannelSphere);
	m_pVertexChannelSphere = NULL;

	// release vertex buffer for sphere
	pHardwareResourceManager->ReleaseVertexBuffer(m_pVertexBufferSphere);
	m_pVertexBufferSphere = NULL;

	// line

	// release index buffer for line
	pHardwareResourceManager->ReleaseIndexBuffer(m_pIndexBufferLine);
	m_pIndexBufferBox = NULL;

	// release vertex channel for line
	pHardwareResourceManager->ReleaseVertexChannel(m_pVertexChannelLine);
	m_pVertexChannelLine = NULL;

	// release vertex buffer for line
	pHardwareResourceManager->ReleaseVertexBuffer(m_pVertexBufferLine);
	m_pVertexBufferLine = NULL;

	// common vertex declaration

	// release vertex declaration for primitives
	pHardwareResourceManager->ReleaseVertexDeclaration(m_pVertexDeclaration);
	m_pVertexDeclaration = NULL;
}

const TMatrix4<Real>& TestInstrumentationDevice::GetTransform(void)
{
	return m_matTransform;
}

void TestInstrumentationDevice::SetTransform(const TMatrix4<Real>& p_matTransform)
{
	m_matTransform = p_matTransform;

	m_pRenderDevice->SetWorldMatrix(p_matTransform);
}

const Meson::Gravitas::Instrumentation::Colour& TestInstrumentationDevice::GetColour(void)
{
	return m_colour;
}

void TestInstrumentationDevice::SetColour(const Meson::Gravitas::Instrumentation::Colour& p_colour)
{
	m_colour = p_colour;
	m_colourVistas.R = (float) m_colour.R;
	m_colourVistas.G = (float) m_colour.G;
	m_colourVistas.B = (float) m_colour.B;
}

DrawMode::DrawMode TestInstrumentationDevice::GetDrawMode(void)
{
	return m_drawMode;
}

void TestInstrumentationDevice::SetDrawMode(DrawMode::DrawMode p_drawMode)
{
	m_drawMode = p_drawMode;
}

void TestInstrumentationDevice::PushDeviceState(void)
{
	DeviceState deviceState;
	deviceState.Colour = m_colour;
	deviceState.DrawMode = m_drawMode;
	deviceState.Transform = m_matTransform;
	m_listDeviceStates.Add(deviceState);
}

void TestInstrumentationDevice::PopDeviceState(void)
{
	if (m_listDeviceStates.Size() == 0)
		throw new MesonException("Instrumentation device stack empty.", __FILE__, __LINE__);
	size_t unLast = m_listDeviceStates.Size() - 1;
	DeviceState deviceState = m_listDeviceStates[unLast];
	m_listDeviceStates.RemoveAt(unLast);
	SetColour(deviceState.Colour);
	SetDrawMode(deviceState.DrawMode);
	SetTransform(deviceState.Transform);
}

void TestInstrumentationDevice::DrawText(const TPoint3<Real>& p_ptPosition,
	const String& p_strText)
{
}

void TestInstrumentationDevice::DrawPoint(const TPoint3<Real>& p_ptPosition)
{
}

void TestInstrumentationDevice::DrawLine(
	const TPoint3<Real>& p_ptStart,
	const TPoint3<Real>& p_ptEnd)
{
	TMatrix4<Real> matTransformOld = m_matTransform;

	TPoint3<Real> ptCentre = p_ptStart + (p_ptEnd - p_ptStart) * (Real) 0.5;
	TVector3<Real> vecExtent = p_ptEnd - ptCentre;

	Matrix4f matScale = Matrix4f::Identity;
	matScale._11 = (float) vecExtent.Length();

	TVector3<Real> vecDirection = vecExtent.NormaliseCopy();
	TVector3<Real> vecAxis = vecDirection ^ TVector3<Real>::Right;

	TMatrix4<Real> matRotate;

	if (vecAxis.IsZero())
		matRotate.MakeIdentity();
	else
	{
		Real rRotation = TMaths<Real>::ACos(vecDirection * TVector3<Real>::Right);
		vecAxis.Normalise();
		matRotate.MakeRotation(vecAxis, -rRotation);
	}

	Matrix4f matLine = matRotate * matScale;
	matLine._14 = (float) ptCentre.X;
	matLine._24 = (float) ptCentre.Y;
	matLine._34 = (float) ptCentre.Z;

	m_pRenderDevice->SetWorldMatrix(m_matTransform * matLine);

	m_pRenderDevice->SetVertexChannel(m_pVertexChannelLine);
	m_pRenderDevice->SetIndexBuffer(m_pIndexBufferLine);

	ShaderProgramVariable* pShaderProgramVariable
		= m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX)->GetVariable("instrumentationColour");
	pShaderProgramVariable->Set(m_colourVistas);
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX));
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL));

	m_pRenderDevice->DrawIndexedPrimitive(IRenderDevice::PT_LINELIST_SEGMENTS, 0, 0, 0, 2, 1);

	SetTransform(matTransformOld);
}

void TestInstrumentationDevice::DrawArrow(
	const TPoint3<Real>& p_ptHead,
	const TPoint3<Real>& p_ptTail)
{
	DrawLine(p_ptTail, p_ptHead);
	TVector3<Real> vecOffset(p_ptHead - p_ptTail);
	TVector3<Real> vecNormal1, vecNormal2;
	vecOffset.ComputeOrthonormals(vecNormal1, vecNormal2);
	TVector3<Real> vecOffsetTenth = vecOffset * (Real) 0.1;
	Real rTenth = vecOffset.Length() * (Real) 0.1;
	DrawLine(p_ptHead, p_ptHead - vecOffsetTenth + vecNormal1 * rTenth);
	DrawLine(p_ptHead, p_ptHead - vecOffsetTenth - vecNormal1 * rTenth);
	DrawLine(p_ptHead, p_ptHead - vecOffsetTenth + vecNormal2 * rTenth);
	DrawLine(p_ptHead, p_ptHead - vecOffsetTenth - vecNormal2 * rTenth);
}

void TestInstrumentationDevice::DrawSphere(
	const TPoint3<Real>& p_ptCentre,
	Real p_rRadius)
{
	TMatrix4<Real> matTransformOld = m_matTransform;

	m_matSphere._11 = m_matSphere._22 = m_matSphere._33 = (float) p_rRadius;
	m_matSphere._14 = (float) (p_ptCentre.X);
	m_matSphere._24 = (float) (p_ptCentre.Y);
	m_matSphere._34 = (float) (p_ptCentre.Z);

	m_pRenderDevice->SetWorldMatrix(m_matTransform * m_matSphere);

	m_pRenderDevice->SetVertexChannel(m_pVertexChannelSphere);
	m_pRenderDevice->SetIndexBuffer(m_pIndexBufferSphere);

	ShaderProgramVariable* pShaderProgramVariable
		= m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX)->GetVariable("instrumentationColour");
	pShaderProgramVariable->Set(m_colourVistas);
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX));
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL));

	m_pRenderDevice->DrawIndexedPrimitive(IRenderDevice::PT_LINELIST_SEGMENTS, 0, 0, 0,
		SPHERE_SEGMENTS * SPHERE_SEGMENTS * 2,
		SPHERE_SEGMENTS * SPHERE_SEGMENTS * 4);

	SetTransform(m_matTransform);
}

void TestInstrumentationDevice::DrawBox(
	const TPoint3<Real>& p_ptCentre,
	const TVector3<Real>& p_vecExtents)
{
	TMatrix4<Real> matTransformOld = m_matTransform;

	m_matAABB._11 = (float) p_vecExtents.X;
	m_matAABB._22 = (float) p_vecExtents.Y;
	m_matAABB._33 = (float) p_vecExtents.Z;
	m_matAABB._14 = (float) (p_ptCentre.X);
	m_matAABB._24 = (float) (p_ptCentre.Y);
	m_matAABB._34 = (float) (p_ptCentre.Z);

	m_pRenderDevice->SetWorldMatrix(m_matTransform * m_matAABB);

	m_pRenderDevice->SetVertexChannel(m_pVertexChannelBox);
	m_pRenderDevice->SetIndexBuffer(m_pIndexBufferBox);

	ShaderProgramVariable* pShaderProgramVariable
		= m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX)->GetVariable("instrumentationColour");
	pShaderProgramVariable->Set(m_colourVistas);
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX));
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL));

	m_pRenderDevice->DrawIndexedPrimitive(IRenderDevice::PT_LINELIST_SEGMENTS, 0, 0, 0, 8, 12);

	SetTransform(matTransformOld);
}

void TestInstrumentationDevice::DrawBox(
	const TMatrix3<Real>& p_matOrientation,
	const TPoint3<Real>& p_ptCentre,
	const TVector3<Real>& p_vecExtents)
{
	TMatrix4<Real> matTransformOld = m_matTransform;

	TMatrix4<Real> matOBB;
	matOBB.MakeZero();
	matOBB._11 = (float) p_vecExtents.X;
	matOBB._22 = (float) p_vecExtents.Y;
	matOBB._33 = (float) p_vecExtents.Z;
	matOBB._44 = (Real) 1.0;

	matOBB = TMatrix4<Real>(p_matOrientation) * matOBB;

	TMatrix4<Real> matTranslation;
	matTranslation.MakeTranslation(p_ptCentre.ToVector());
	matOBB = matTranslation * matOBB;

	m_pRenderDevice->SetWorldMatrix(m_matTransform * matOBB);

	m_pRenderDevice->SetVertexChannel(m_pVertexChannelBox);
	m_pRenderDevice->SetIndexBuffer(m_pIndexBufferBox);

	ShaderProgramVariable* pShaderProgramVariable
		= m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX)->GetVariable("instrumentationColour");
	pShaderProgramVariable->Set(m_colourVistas);
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX));
	m_pRenderDevice->LoadShaderParameters(m_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL));

	m_pRenderDevice->DrawIndexedPrimitive(IRenderDevice::PT_LINELIST_SEGMENTS, 0, 0, 0, 8, 12);

	SetTransform(matTransformOld);
}
