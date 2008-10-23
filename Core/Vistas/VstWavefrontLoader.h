//----------------------------------------------------------------------------------------------
#pragma once

#include "VstGeometryLoader.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class WavefrontFace
	{
	public:
		WavefrontFace();
		WavefrontFace& operator=(const WavefrontFace& p_face);
		bool operator==(const WavefrontFace& p_face);

		qword GetHash(uint p_uiIndex);

	public:
		uint VertexIndex[3],
			 NormalIndex[3],
			 TexcrdIndex[3];
	};

	//----------------------------------------------------------------------------------------------
	class WavefrontObject
	{
	public:
		WavefrontObject();
		~WavefrontObject();

		bool IsEmpty();

		void ResetObject(void);
		void ResetBaseIndices(void);

		void SetBaseIndices(void);
		void SetBaseIndices(uint p_uiBaseVertexIndex, uint p_uiBaseTexcrdIndex, uint p_uiBaseNormalIndex);

		uint GetBaseVertexIndex(void) const;
		uint GetBaseNormalIndex(void) const;
		uint GetBaseTexcrdIndex(void) const;

		void SetName(const Meson::Common::Text::String& p_strName);
		void SetMaterialName(const Meson::Common::Text::String p_strMaterialName);

		void AddNormal(const Meson::Common::Maths::Vector3f& p_vec3Normal);
		void AddVertex(const Meson::Common::Maths::Vector3f& p_vec3Vertex);
		void AddTexcrd(const Meson::Common::Maths::Vector2f& p_vec2Texcrd);
		void AddFace(const WavefrontFace& p_face);

		GeometryPtr ConvertToGeometry(void);

	protected:
		uint m_uiBaseVertexIndex,
			 m_uiBaseNormalIndex,
			 m_uiBaseTexcrdIndex;

		Meson::Common::Text::String m_strObjectName;
		Meson::Common::Text::String m_strMaterialName;

		Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector3f> m_vertexList;
		Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector3f> m_normalList;
		Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector2f> m_texcrdList;

		Meson::Common::Collections::TArrayList<WavefrontFace> m_faceList;
	};

	//----------------------------------------------------------------------------------------------
	class WavefrontLoader : public IGeometryLoader
	{
	public:
		WavefrontLoader();
		~WavefrontLoader();

		const Meson::Common::Text::String& GetType(void) const;

		GeometryListPtr Load(const Meson::Common::Text::String& p_strFilename);
		GeometryPtr LoadSingle(const Meson::Common::Text::String& p_strFilename);

	protected:
		void CmdObject(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);
		void CmdMaterial(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);
		void CmdVertex(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);
		void CmdNormal(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);
		void CmdTextureCoordinates(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);
		void CmdFace(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenList, WavefrontObject& p_wfObject);

		uint Checksum(Meson::Common::Text::String& p_strString);
		void SplitTokens(char p_cChar, Meson::Common::Text::String& p_strString, Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenListOut);
		
		bool IsResourceLoaded(void) const;
		bool LoadResource(const Meson::Common::Text::String& p_strResourcePath);
		void UnloadResource(void);
		
		bool IsEndOfBuffer() const;
		bool TokeniseNextLine(Meson::Common::Collections::TArrayList<Meson::Common::Text::String>& p_tokenListOut);

	protected:
		Meson::Common::Text::String m_strName,
									m_strCmdVertex,
									m_strCmdNormal,
									m_strCmdTexCoords,
									m_strCmdFace,
									m_strCmdObject,
									m_strCmdMaterial,
									m_strLine;

		char* m_pBuffer;
		uint  m_uiBufferLength,
			  m_uiBufferPosition;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END