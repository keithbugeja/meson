
#include "Model3D.h"

VertexDeclarationPtr GetDefaultModelVertexDeclaration(void)
{
	static VertexDeclarationPtr s_pVertexDeclaration(NULL);

	if (s_pVertexDeclaration == NULL)
	{
		s_pVertexDeclaration
			= VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration();
		uint uiVertexOffset = 0;
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT3, VDF_POSITION, 0);

		uiVertexOffset += VertexDeclaration::SizeOf(VDT_FLOAT3);
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_COLOUR, VDF_COLOUR, 0);
		uiVertexOffset += VertexDeclaration::SizeOf(VDT_COLOUR);
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT3, VDF_NORMAL, 0);
		uiVertexOffset += VertexDeclaration::SizeOf(VDT_FLOAT3);
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT3, VDF_BINORMAL, 0);
		uiVertexOffset += VertexDeclaration::SizeOf(VDT_FLOAT3);
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT3, VDF_TANGENT, 0);
		uiVertexOffset += VertexDeclaration::SizeOf(VDT_FLOAT3);
		s_pVertexDeclaration->AddVertexElement(0, uiVertexOffset, VDT_FLOAT2, VDF_TEXTURE_COORDINATES, 0);
	}

	return s_pVertexDeclaration;
}

ModelVertex::ModelVertex(void)
	: Position()
	, Colour(0)
	, Normal(Vector3f::Up)
	, Tangent(Vector3f::Right)
	, Binormal(Vector3f::Up ^ Vector3f::Right)
	, TexCoords(Vector2f::Zero)
{
}

ModelVertex::ModelVertex(const ModelVertex& p_modelVertex)
	: Position(p_modelVertex.Position)
	, Colour(p_modelVertex.Colour)
	, Normal(p_modelVertex.Normal)
	, Tangent(p_modelVertex.Tangent)
	, Binormal(p_modelVertex.Binormal)
	, TexCoords(p_modelVertex.TexCoords)
{
}

ModelVertex::ModelVertex(const Vector3f& p_vecPosition, ColourRGBA p_colourRGBA,
	const Vector3f& p_vecNormal, const Vector3f& p_vecTangent,
	const Vector2f& p_vecTexCoords)
	: Position(p_vecPosition)
	, Colour(p_colourRGBA)
	, Normal(p_vecNormal)
	, Tangent(p_vecTangent)
	, Binormal(p_vecNormal ^ p_vecTangent)
	, TexCoords(p_vecTexCoords)
{
}

bool ModelVertex::operator==(const ModelVertex& p_modelVertex)
{
	return Position == p_modelVertex.Position
		&& Colour.ToRGBA() == p_modelVertex.Colour.ToRGBA();
}

ModelFace::ModelFace(void)
{
	VertexIndices[0] = VertexIndices[1] = VertexIndices[2] = 0;
}

ModelFace::ModelFace(const ModelFace& p_modelFace)
{
	VertexIndices[0] = p_modelFace.VertexIndices[0];
	VertexIndices[1] = p_modelFace.VertexIndices[1];
	VertexIndices[2] = p_modelFace.VertexIndices[2];
}

ModelFace::ModelFace(
	short p_nVertexIndex0,
	short p_nVertexIndex1,
	short p_nVertexIndex2)
{
	VertexIndices[0] = p_nVertexIndex0;
	VertexIndices[1] = p_nVertexIndex1;
	VertexIndices[2] = p_nVertexIndex2;
}

bool ModelFace::operator==(const ModelFace& p_modelFace)
{
	return VertexIndices[0] == p_modelFace.VertexIndices[0]
		&& VertexIndices[1] == p_modelFace.VertexIndices[1]
		&& VertexIndices[2] == p_modelFace.VertexIndices[2];
}

void Model3D::ConvertMatrix(const TMatrix4<Real>& p_matSource, Matrix4f& p_matDestination)
{
	for (size_t unIndex = 0; unIndex < 16; unIndex++)
		p_matDestination.Element[unIndex] = (float) p_matSource.Element[unIndex];
}

Model3D::Model3D(void)
	: m_pVertexBuffer(NULL)
	, m_pVertexChannel(NULL)
	, m_pIndexBuffer(NULL)
	, m_vecColour(0.0f, 0.0f, 0.0f, 0.0f)
	, Vertices()
	, Faces()
	, Transform()
	, Scale(1.0f, 1.0f, 1.0f)
	, m_bFilled(true)
{
}

Model3D::~Model3D(void)
{
	VistasEngine* pVistasEngine = VistasEngine::GetInstance();

	if (pVistasEngine == NULL)
		return;

	HardwareResourceManager* pHardwareResourceManager
		= pVistasEngine->GetHardwareResourceManager();

	if (m_pIndexBuffer != NULL)
		pHardwareResourceManager->ReleaseIndexBuffer(m_pIndexBuffer);

	if (m_pVertexChannel != NULL)
		pHardwareResourceManager->ReleaseVertexChannel(m_pVertexChannel);

	if (m_pVertexBuffer != NULL)
	{
		pHardwareResourceManager->ReleaseVertexBuffer(m_pVertexBuffer);
		m_pVertexBuffer = NULL;
	}
}

void Model3D::Update(void)
{
	HardwareResourceManager* pHardwareResourceManager
		= VistasEngine::GetInstance()->GetHardwareResourceManager();

	if (m_pVertexBuffer != NULL)
	{
		pHardwareResourceManager->ReleaseVertexBuffer(m_pVertexBuffer);
		m_pVertexBuffer = NULL;
	}

	m_pVertexBuffer = pHardwareResourceManager->CreateVertexBuffer(
		sizeof(ModelVertex), (uint) Vertices.Size(),
		HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	VertexDeclarationPtr pVertexDeclaration
		= GetDefaultModelVertexDeclaration();

	m_pVertexBuffer->Lock(HardwareResource::ML_NONE);
	m_pVertexBuffer->Write(0, 0, sizeof(ModelVertex) * (uint) Vertices.Size(), (const byte*)&Vertices[0]);
	m_pVertexBuffer->Unlock();

	if (m_pVertexChannel == NULL)
		m_pVertexChannel = pHardwareResourceManager->CreateVertexChannel();
	m_pVertexChannel->SetVertexDeclaration(pVertexDeclaration);
	m_pVertexChannel->SetStreamBinding(0, m_pVertexBuffer);

	if (m_pIndexBuffer != NULL)
	{
		pHardwareResourceManager->ReleaseIndexBuffer(m_pIndexBuffer);
		m_pIndexBuffer = NULL;
	}

	m_pIndexBuffer = pHardwareResourceManager->CreateIndexBuffer(
		IndexBuffer::IT_16BIT, (uint) Faces.Size() * 3,
		HardwareResource::MP_DEFAULT,
		HardwareResource::MU_DEFAULT);

	m_pIndexBuffer->Lock(Meson::Vistas::HardwareResource::ML_NONE);
	m_pIndexBuffer->Write(0, 0, IndexBuffer::IT_16BIT * (uint) Faces.Size() * 3, (const byte*)&Faces[0]);
	m_pIndexBuffer->Unlock();
}

void Model3D::MakeCube(const Vector3f& p_vecExtent, float p_fTextureScale)
{
	float fX = p_vecExtent.X;
	float fY = p_vecExtent.Y;
	float fZ = p_vecExtent.Z;

	Vertices.Clear();
	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, +fZ), Colour::Black, Vector3f::Out, Vector3f::Up, Vector2f(0.0f, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, +fZ), Colour::Black, Vector3f::Out, Vector3f::Up, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, +fZ), Colour::Black, Vector3f::Out, Vector3f::Up, Vector2f(0.0f, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, +fZ), Colour::Black, Vector3f::Out, Vector3f::Up, Vector2f(p_fTextureScale, p_fTextureScale)));

	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, -fZ), Colour::Black, Vector3f::In, Vector3f::Up, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, -fZ), Colour::Black, Vector3f::In, Vector3f::Up, Vector2f(0.0f, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, -fZ), Colour::Black, Vector3f::In, Vector3f::Up, Vector2f(p_fTextureScale, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, -fZ), Colour::Black, Vector3f::In, Vector3f::Up, Vector2f(0.0f, p_fTextureScale)));

	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, +fZ), Colour::Black, Vector3f::Left, Vector3f::Up, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, +fZ), Colour::Black, Vector3f::Left, Vector3f::Up, Vector2f(p_fTextureScale, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, -fZ), Colour::Black, Vector3f::Left, Vector3f::Up, Vector2f(0.0f, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, -fZ), Colour::Black, Vector3f::Left, Vector3f::Up, Vector2f(0.0f, p_fTextureScale)));

	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, +fZ), Colour::Black, Vector3f::Right, Vector3f::Up, Vector2f(0.0f, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, +fZ), Colour::Black, Vector3f::Right, Vector3f::Up, Vector2f(0.0f, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, -fZ), Colour::Black, Vector3f::Right, Vector3f::Up, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, -fZ), Colour::Black, Vector3f::Right, Vector3f::Up, Vector2f(p_fTextureScale, p_fTextureScale)));

	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, +fZ), Colour::Black, Vector3f::Up, Vector3f::In, Vector2f(p_fTextureScale, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, +fZ), Colour::Black, Vector3f::Up, Vector3f::In, Vector2f(0.0f, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(-fX, +fY, -fZ), Colour::Black, Vector3f::Up, Vector3f::In, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, +fY, -fZ), Colour::Black, Vector3f::Up, Vector3f::In, Vector2f(0.0f, 0.0f)));

	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, +fZ), Colour::Black, Vector3f::Down, Vector3f::Out, Vector2f(p_fTextureScale, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, +fZ), Colour::Black, Vector3f::Down, Vector3f::Out, Vector2f(0.0f, p_fTextureScale)));
	Vertices.Add(ModelVertex(Vector3f(-fX, -fY, -fZ), Colour::Black, Vector3f::Down, Vector3f::Out, Vector2f(p_fTextureScale, 0.0f)));
	Vertices.Add(ModelVertex(Vector3f(+fX, -fY, -fZ), Colour::Black, Vector3f::Down, Vector3f::Out, Vector2f(0.0f, 0.0f)));

	Faces.Clear();
	Faces.Add(ModelFace(0, 1, 2));
	Faces.Add(ModelFace(2, 1, 3));

	Faces.Add(ModelFace(4, 6, 5));
	Faces.Add(ModelFace(6, 7, 5));

	Faces.Add(ModelFace(8, 9, 10));
	Faces.Add(ModelFace(10, 9, 11));

	Faces.Add(ModelFace(12, 14, 13));
	Faces.Add(ModelFace(14, 15, 13));

	Faces.Add(ModelFace(16, 18, 17));
	Faces.Add(ModelFace(18, 19, 17));

	Faces.Add(ModelFace(20, 21, 22));
	Faces.Add(ModelFace(21, 23, 22));

	Update();
}

void Model3D::MakeSphere(float p_fRadius, size_t p_unSubDivisions)
{
	Vertices.Clear();
	float fDeltaTheta = TMaths<float>::Pi / p_unSubDivisions;
	float fDeltaPhi = fDeltaTheta;
	//float fPhiExtent = TMaths<float>::PiHalf - fDeltaPhi;

	short sHorizontalDivisions = (short) p_unSubDivisions * 2;

	// vertices - isomorphic to cylinder side
	for (short sIndexPhi = 0; sIndexPhi <= (short) p_unSubDivisions; sIndexPhi++)
	{
		float fPhi = sIndexPhi * fDeltaPhi;
		float fSinPhi = TMaths<float>::Sin(fPhi);
		for (short sIndexTheta = 0; sIndexTheta <= sHorizontalDivisions; sIndexTheta++)
		{
			float fTheta = sIndexTheta * fDeltaTheta;
			ModelVertex modelVertex;
			modelVertex.Position.X = p_fRadius * TMaths<float>::Cos(fTheta) * fSinPhi;
			modelVertex.Position.Z = p_fRadius * TMaths<float>::Sin(fTheta) * fSinPhi;
			modelVertex.Position.Y = p_fRadius * TMaths<float>::Cos(fPhi);
			modelVertex.Normal = modelVertex.Position.NormaliseCopy();

			modelVertex.Normal.ComputeOrthonormals(modelVertex.Tangent, modelVertex.Binormal);

			modelVertex.TexCoords.X = fTheta / Mathf::PiTwo;
			modelVertex.TexCoords.Y = fPhi / Mathf::Pi;
			//bool bColour = (sIndexTheta + sIndexPhi) % 2 == 0;
			//modelVertex.Colour = bColour ? Colour::Black : Colour::Yellow.ToARGB();
			modelVertex.Colour = Colour::Black;
			Vertices.Add(modelVertex);
		}
	}

	Faces.Clear();

	// non-pole faces
	for (short sIndexPhi = 0; sIndexPhi < (short) p_unSubDivisions; sIndexPhi++)
	{
		for (short sIndexTheta = 0; sIndexTheta < sHorizontalDivisions; sIndexTheta++)
		{
			short sIndex0 = sIndexPhi * (sHorizontalDivisions + 1) + sIndexTheta;
			short sIndex1 = sIndex0 + 1;
			//if (sIndex1 % sHorizontalDivisions == 0)
			//	sIndex1 -= sHorizontalDivisions;
			short sIndex2 = sIndex0 + sHorizontalDivisions + 1;
			short sIndex3 = sIndex1 + sHorizontalDivisions + 1;

			Faces.Add(ModelFace(sIndex0, sIndex2, sIndex1));
			Faces.Add(ModelFace(sIndex1, sIndex2, sIndex3));
		}
	}

	// pole faces
	/*short sNorthIndex = (short) Vertices.Size() - 2;
	short sSouthIndex = (short) Vertices.Size() - 1;
	short sOffset = sHorizontalDivisions * ((short) p_unSubDivisions - 2);
	for (short sIndexTheta = 0; sIndexTheta < sHorizontalDivisions; sIndexTheta++)
	{
		short sIndexTheta2 = (sIndexTheta + 1) % sHorizontalDivisions;
		Faces.Add(ModelFace(sNorthIndex, sIndexTheta2, sIndexTheta));
		Faces.Add(ModelFace(sSouthIndex, sIndexTheta + sOffset, sIndexTheta2 + sOffset));
	}*/

	Update();
}


void Model3D::MakeCylinder(float p_fRadius, float p_fHeight, size_t p_unSubDivisions)
{
	Vertices.Clear();
	float fDeltaTheta = TMaths<float>::PiTwo / p_unSubDivisions;

	// vertices
	for (float fY = -p_fHeight * 0.5f; fY < p_fHeight; fY += p_fHeight)
	{
		for (short sIndexTheta = 0; sIndexTheta <= (short) p_unSubDivisions; sIndexTheta++)
		{
			float fTheta = sIndexTheta * fDeltaTheta;
			ModelVertex modelVertex;

			modelVertex.Position.Y = 0.0f;
			modelVertex.Position.X = p_fRadius * TMaths<float>::Cos(fTheta);
			modelVertex.Position.Z = p_fRadius * TMaths<float>::Sin(fTheta);
			modelVertex.Normal = modelVertex.Position.NormaliseCopy();

			modelVertex.Position.Y = fY;

			modelVertex.Normal.ComputeOrthonormals(modelVertex.Tangent, modelVertex.Binormal);

			modelVertex.TexCoords.X = fTheta / Mathf::PiTwo;
			modelVertex.TexCoords.Y = (fY / p_fHeight) + 0.5f;

			modelVertex.Colour = Colour::Black;
			Vertices.Add(modelVertex);
		}
	}

	// cap vertices
	short sCapStart = (short) Vertices.Size();
	short sCapStart2 = (short) (Vertices.Size() * 3) / 2;
	for (float fY = -p_fHeight * 0.5f; fY < p_fHeight; fY += p_fHeight)
	{
		for (short sIndexTheta = 0; sIndexTheta <= (short) p_unSubDivisions; sIndexTheta++)
		{
			float fTheta = sIndexTheta * fDeltaTheta;
			ModelVertex modelVertex;

			modelVertex.Position.X = p_fRadius * TMaths<float>::Cos(fTheta);
			modelVertex.Position.Z = p_fRadius * TMaths<float>::Sin(fTheta);
			modelVertex.Position.Y = fY;

			modelVertex.Normal = (fY < 0.0f) ? Vector3f::Down : Vector3f::Up;

			modelVertex.Normal.ComputeOrthonormals(modelVertex.Tangent, modelVertex.Binormal);

			modelVertex.TexCoords.X = 0.5f + 0.5f * modelVertex.Position.X / p_fRadius;
			modelVertex.TexCoords.Y = 0.5f + 0.5f * modelVertex.Position.Z / p_fRadius;

			modelVertex.Colour = Colour::Black;
			Vertices.Add(modelVertex);
		}
	}

	Faces.Clear();

	// sides
	for (short sIndexTheta = 0; sIndexTheta < (short) p_unSubDivisions; sIndexTheta++)
	{
		short sIndex0 = sIndexTheta;
		short sIndex1 = (sIndex0 + 1) % ((short) p_unSubDivisions + 1);
		short sIndex2 = sIndex0 + (short) p_unSubDivisions + 1;
		short sIndex3 = sIndex1 + (short) p_unSubDivisions + 1;

		Faces.Add(ModelFace(sIndex0, sIndex1, sIndex2));
		Faces.Add(ModelFace(sIndex1, sIndex3, sIndex2));

		// caps
		if (sIndexTheta > 0)
		{
			Faces.Add(ModelFace(sCapStart, sCapStart + sIndex1, sCapStart + sIndex0));
			Faces.Add(ModelFace(sCapStart2, sCapStart2 + sIndex0, sCapStart2 + sIndex1));
		}
	}

	Update();
}


void Model3D::Render(IRenderDevice* p_pRenderDevice)
{
	TMatrix4<Real> matTransformPhysics;
	Transform.ExtractMatrix(matTransformPhysics);
	matTransformPhysics._11 *= Scale.X;
	matTransformPhysics._12 *= Scale.X;
	matTransformPhysics._13 *= Scale.X;
	matTransformPhysics._21 *= Scale.Y;
	matTransformPhysics._22 *= Scale.Y;
	matTransformPhysics._23 *= Scale.Y;
	matTransformPhysics._31 *= Scale.Z;
	matTransformPhysics._32 *= Scale.Z;
	matTransformPhysics._33 *= Scale.Z;

	Matrix4f matTransformGraphics;
	ConvertMatrix(matTransformPhysics, matTransformGraphics);

	p_pRenderDevice->SetWorldMatrix(matTransformGraphics);

	p_pRenderDevice->SetVertexChannel(m_pVertexChannel);
	p_pRenderDevice->SetIndexBuffer(m_pIndexBuffer);

	ShaderProgramVariable* pShaderProgramVariable
		= p_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL)->GetVariable("objectColour");
	pShaderProgramVariable->Set(m_vecColour);
	p_pRenderDevice->LoadShaderParameters(p_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_VERTEX));
	p_pRenderDevice->LoadShaderParameters(p_pRenderDevice->GetShaderProgram(IShaderProgram::SPT_PIXEL));

	IRenderDevice::PrimitiveType primitiveType = m_bFilled ? IRenderDevice::PT_TRIANGLE_LIST : IRenderDevice::PT_LINELIST_SEGMENTS;

	p_pRenderDevice->DrawIndexedPrimitive(primitiveType, 0, 0, 0,
		(uint) Vertices.Size(), (uint) Faces.Size());
}

void Model3D::SetColour(const Vector4f& p_vecColour)
{
	m_vecColour = p_vecColour;
}

void Model3D::SetFilled(bool p_bFilled)
{
	m_bFilled = p_bFilled;
}
