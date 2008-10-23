#pragma once

#include "VstVistasEngine.h"

#include "GrvTransform.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

using namespace Meson::Vistas;

struct ModelVertex
{
	Point3f Position;
	ColourRGBA Colour;
	Vector3f Normal;
	Vector3f Binormal;
	Vector3f Tangent;
	Vector2f TexCoords;

	ModelVertex(void);
	ModelVertex(const ModelVertex& p_modelVertex);
	ModelVertex(const Point3f& p_ptPosition, ColourRGBA p_colourRGBA,
		const Vector3f& p_vecNormal, const Vector3f& p_vecTangent,
		const Vector2f& p_vecTexCoords);
	bool operator==(const ModelVertex& p_modelVertex);
};

struct ModelFace
{
	short VertexIndices[3];

	ModelFace(void);
	ModelFace(const ModelFace& p_modelFace);
	ModelFace::ModelFace(
		short p_nVertexIndex0,
		short p_nVertexIndex1,
		short p_nVertexIndex2);
	bool operator==(const ModelFace& p_modelFace);
};

class Model3D
	: public Meson::Common::TReferenceCounter<Model3D>
{
private:
	VertexBufferPtr m_pVertexBuffer;
	VertexChannelPtr m_pVertexChannel;
	IndexBufferPtr m_pIndexBuffer;
	Vector4f m_vecColour;
	bool m_bFilled;

public:
	Transform Transform;
	Vector3f Scale;
	TArrayList<ModelVertex> Vertices;
	TArrayList<ModelFace> Faces;

private:
	void ConvertMatrix(const TMatrix4<Real>& p_matSource, Matrix4f& p_matDestination);

public:
	Model3D(void);
	~Model3D(void);
	void Update(void);
	void MakeCube(const Vector3f& p_vecExtent, float p_fTextureScale);
	void MakeSphere(float p_fRadius, size_t p_unSubDivisions);
	void MakeCylinder(float p_fRadius, float p_fHeight, size_t p_unSubDivisions);
	void Render(IRenderDevice* p_pRenderDevice);
	void SetColour(const Vector4f& p_vecColour);
	void SetFilled(bool p_bFilled);
};

typedef Meson::Common::TPointer<Model3D> Model3DPtr;