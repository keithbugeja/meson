#include "WavefrontModelLoader.h"

#include "FileInputStream.h"
#include "Converter.h"

using namespace Meson::Common::IO;

WavefrontFace::WavefrontFace(void)
{
	VertexIndex[0] = VertexIndex[1] = VertexIndex[2] = 0;
	NormalIndex[0] = NormalIndex[1] = NormalIndex[2] = 0;
	TextureCoordinateIndex[0] = TextureCoordinateIndex[1] = TextureCoordinateIndex[2] = 0;
}

bool WavefrontFace::operator==(const WavefrontFace& p_wavefrontFace) const
{
	return false;
}

WavefrontModelLoader::WavefrontModelLoader(void)
{
}

WavefrontModelLoader::~WavefrontModelLoader(void)
{
}

void WavefrontModelLoader::LoadModel(
	const String& p_strFilename,
	VectorList& p_listVertices,
	VectorList& p_listNormals,
	TList< TVector2<Real> >& p_listTextureCoordinates,
	WavefrontFaceList& p_listFaces)
{
	p_listVertices.Clear();
	p_listNormals.Clear();
	p_listTextureCoordinates.Clear();
	p_listFaces.Clear();

	FileInputStream fileInputStream(p_strFilename);
	while (!fileInputStream.IsEndOfStream())
	{
		//read line
		String strLine;
		char ch;
		while (!fileInputStream.IsEndOfStream())
		{
			fileInputStream.Read(&ch, 1);
			if (ch == '\n' || ch == '\r')
				break;
			strLine += ch;
		}

		if (strLine.Length() < 2)
			continue;

		TArrayList<String> listTokens;

		strLine.Split(' ', listTokens);

		if (listTokens[0] == "v")
		{
			// vertex
			if (listTokens.Size() != 4)
				continue;
			TVector3<Real> vecVertex;
			vecVertex.X = Converter::ToDouble(listTokens[1]);
			vecVertex.Y = Converter::ToDouble(listTokens[2]);
			vecVertex.Z = Converter::ToDouble(listTokens[3]);
			p_listVertices.Add(vecVertex);
		}
		else if (listTokens[0] == "vn")
		{
			// normal
			if (listTokens.Size() != 4)
				continue;
			TVector3<Real> vecNormal;
			vecNormal.X = Converter::ToDouble(listTokens[1]);
			vecNormal.Y = Converter::ToDouble(listTokens[2]);
			vecNormal.Z = Converter::ToDouble(listTokens[3]);
			p_listNormals.Add(vecNormal);
		}
		else if (listTokens[0] == "vt")
		{
			// texture coord
			if (listTokens.Size() != 3)
				continue;
			TVector2<Real> vecTexture;
			vecTexture.X = Converter::ToDouble(listTokens[1]);
			vecTexture.Y = Converter::ToDouble(listTokens[2]);
			p_listTextureCoordinates.Add(vecTexture);
		}
		else if (listTokens[0] == "f")
		{
			// face
			if (listTokens.Size() < 4)
				continue;

			//extract faces
			TArrayList<int> listFaceVertexIndices;
			TArrayList<int> listFaceNormalIndices;
			TArrayList<int> listFaceTextureCoordIndices;
			for (size_t unIndex = 1; unIndex < listTokens.Size(); unIndex++)
			{
				String& strFaceVertex = listTokens[unIndex];
				TArrayList<String> listSubTokens;
				strFaceVertex.Split('/', listSubTokens);
				listFaceVertexIndices.Add(Converter::ToWord(listSubTokens[0]) - 1);
				if (listSubTokens.Size() >= 2)
					listFaceTextureCoordIndices.Add(Converter::ToWord(listSubTokens[1]) - 1);
				if (listSubTokens.Size() >= 3)
					listFaceNormalIndices.Add(Converter::ToWord(listSubTokens[2]) - 1);
			}

			// do triangle tessellation
			for (size_t unIndex = 2; unIndex < listFaceVertexIndices.Size(); unIndex++)
			{
				WavefrontFace wavefrontFace;
				wavefrontFace.VertexIndex[0] = listFaceVertexIndices[0];
				wavefrontFace.VertexIndex[1] = listFaceVertexIndices[unIndex];
				wavefrontFace.VertexIndex[2] = listFaceVertexIndices[unIndex - 1];

				if (listFaceNormalIndices.Size() > 0)
				{
					wavefrontFace.NormalIndex[0] = listFaceNormalIndices[0];
					wavefrontFace.NormalIndex[1] = listFaceNormalIndices[unIndex];
					wavefrontFace.NormalIndex[2] = listFaceNormalIndices[unIndex - 1];
				}

				if (listFaceTextureCoordIndices.Size() > 0)
				{
					wavefrontFace.TextureCoordinateIndex[0] = listFaceTextureCoordIndices[0];
					wavefrontFace.TextureCoordinateIndex[1] = listFaceTextureCoordIndices[unIndex];
					wavefrontFace.TextureCoordinateIndex[2] = listFaceTextureCoordIndices[unIndex - 1];
				}

				p_listFaces.Add(wavefrontFace);
			}
		}
	}
	fileInputStream.Close();

	// prepare normal list with same order as vertices
}
