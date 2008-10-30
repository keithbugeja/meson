#include "GeometryLoader.h"

#include "FileInputStream.h"
#include "Converter.h"

using namespace Meson::Common::IO;

using namespace Meson::Gravitas::Geometry;

GeometryFace::GeometryFace(void)
	: VertexIndices()
{
}

GeometryFace& GeometryFace::operator=(const GeometryFace& p_geometryFace)
{
	VertexIndices = p_geometryFace.VertexIndices;
	return *this;
}

bool GeometryFace::operator==(const GeometryFace& p_geometryFace) const
{
	return false;
}

GeometryLoader::GeometryLoader(void)
{
}

GeometryLoader::~GeometryLoader(void)
{
}

void GeometryLoader::LoadWavefrontGeometry(
	const String& p_strFilename,
	bool p_bTriangulateFaces,
	VectorList& p_listVertices,
	GeometryFaceList& p_listGeometryFaces)
{
	p_listVertices.Clear();
	p_listGeometryFaces.Clear();

	TArrayList<int> listFaceVertexIndices;

	FileInputStream fileInputStream(p_strFilename);
	TArrayList<String> listTokens;
	String strLine;
	GeometryFace geometryFace;
	while (!fileInputStream.IsEndOfStream())
	{
		//read line
		strLine.Clear();
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

		listTokens.Clear();
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
		else if (listTokens[0] == "f")
		{
			// face
			if (listTokens.Size() < 4)
				continue;

			//extract faces
			listFaceVertexIndices.Clear();
			for (size_t unIndex = 1; unIndex < listTokens.Size(); unIndex++)
			{
				String& strFaceVertex = listTokens[unIndex];
				TArrayList<String> listSubTokens;
				strFaceVertex.Split('/', listSubTokens);
				listFaceVertexIndices.Add(Converter::ToWord(listSubTokens[0]) - 1);
			}

			if (p_bTriangulateFaces)
			{
				// do triangle tessellation
				for (size_t unIndex = 2; unIndex < listFaceVertexIndices.Size(); unIndex++)
				{
					GeometryFace geometryFace;
					geometryFace.VertexIndices.Add(listFaceVertexIndices[0]);
					geometryFace.VertexIndices.Add(listFaceVertexIndices[unIndex]);
					geometryFace.VertexIndices.Add(listFaceVertexIndices[unIndex - 1]);

					p_listGeometryFaces.Add(geometryFace);
				}
			}
			else
			{
				// allow polygon faces with more than 3 vertices 
				geometryFace.VertexIndices = listFaceVertexIndices;
				p_listGeometryFaces.Add(geometryFace);
			}
		}
	}
	fileInputStream.Close();
}
