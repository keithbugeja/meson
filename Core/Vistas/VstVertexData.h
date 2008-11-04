//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Pair.h"

#include "VstTypes.h"
#include "VstArrayBuffer.h"
#include "VstVertexDeclaration.h"

Meson_Vistas_BEGIN

	class VISTAS_API VertexDescriptor : 
		public Meson::Common::TReferenceCounter<VertexDescriptor>
	{
	public:
		enum Semantic
		{
			VDS_COLOUR,
			VDS_DIFFUSE,
			VDS_SPECULAR,
			VDS_POSITION,
			VDS_NORMAL,
			VDS_BINORMAL,
			VDS_TANGENT,
			VDS_TEXCOORD1D,
			VDS_TEXCOORD2D,
			VDS_TEXCOORD3D,
			VDS_TEXCOORD4D,
			
			VDS_COUNT
		};

	public:
		VertexDescriptor(void);
		VertexDescriptor(VertexDeclarationPtr p_pVertexDeclaration);
		VertexDescriptor(const VertexDescriptor& p_vertexDescriptor);
		
		void RebuildDescriptor(VertexDeclarationPtr p_pVertexDeclaration);
		void ExportDescriptor(VertexDeclarationPtr p_pVertexDeclaration);
		
		bool GetSemanticBufferMap(Semantic p_eSemantic, uint p_uiChannel, uint& p_pIndex, uint& p_pOffset);

		void AddSemantic(Semantic p_eSemantic, uint p_uiStream);

		uint GetSemanticCount(Semantic p_eSemantic);
		uint GetSemanticCount(Semantic p_eSemantic, uint p_uiStream);
		uint GetSemanticOffset(Semantic p_eSemantic, uint p_uiChannel);
		uint GetSemanticStream(Semantic p_eSemantic, uint p_uiChannel);
		uint GetSemanticUsage(Semantic p_eSemantic, uint p_uiChannel);

		uint GetStreamSize(uint p_uiStream);
		uint GetStreamCount(void) const;

		uint GetStreamAt(uint p_uiIndex);
		uint GetStreamSizeAt(uint p_uiIndex);

		uint GetVertexSize(void) const;
		
		static uint GetSemanticSize(Semantic p_eSemantic);

	protected:
		uint m_uiGlobalSemanticCount[VDS_COUNT],
			 m_uiVertexSize;

		// <Index> -> <Stream, Size>
		struct VISTAS_API StreamInformation
		{
			uint m_uiStream,
				 m_uiStreamSize;

			uint m_uiSemanticCount[VDS_COUNT];

			StreamInformation();
			StreamInformation(uint p_uiStream, uint p_uiStreamSize);
			StreamInformation(const StreamInformation& p_streamInformation);
			bool operator==(const StreamInformation& p_streamInformation) const;
			StreamInformation& operator=(const StreamInformation& p_streamInformation);
			void ResetSemanticCount(void);
		};

		Meson::Common::Collections::TArrayList<StreamInformation> m_streamInformationList;
		Meson::Common::Collections::THashMap<uint, uint> m_streamIndexMap;
		
		// <Semantic,GlobalIndex> -> <Stream, Usage, Offset>
		// 
		struct VISTAS_API SemanticDetails
		{
			uint m_uiStream,
				 m_uiUsageIndex,
				 m_uiOffset;

			SemanticDetails();
			SemanticDetails(uint p_uiStream, uint p_uiUsageIndex, uint p_uiOffset);
			SemanticDetails(const SemanticDetails& p_semanticDetails);
			bool operator==(const SemanticDetails& p_semanticDetails) const;
			SemanticDetails& operator=(const SemanticDetails& p_semanticDetails);
		};

		typedef Meson::Common::Collections::TPair<Semantic, uint> UniqueSemantic;
		Meson::Common::Collections::THashMap<UniqueSemantic, SemanticDetails> m_semanticMap;

		// <Semantic> -> <Function, Type>
		struct VISTAS_API SemanticDescriptor
		{
			VertexDataFunction m_eVertexDataFunction;
			VertexDataType m_eVertexDataType;
		};

		static SemanticDescriptor m_semanticDescriptorList[VDS_COUNT];
	
	protected:
		void Reset(void);
		bool ContainsStream(uint p_uiStream, uint& p_pIndexOut);
		StreamInformation& GetStreamInformation(uint p_uiStream);
	};
	
	typedef Meson::Common::TPointer<VertexDescriptor> VertexDescriptorPtr;

	//----------------------------------------------------------------------------------------------
	// Vertex section
	//----------------------------------------------------------------------------------------------
	class VISTAS_API Vertex : 
		public Meson::Common::TReferenceCounter<Vertex>
	{
		friend class VertexData;

	protected:
		void Copy(const Vertex& p_srcVertex, Vertex& p_dstVertex);
		void Move(Vertex& p_vertex, int p_nOffset);
		void Bind(Vertex& p_vertex, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList);
		void Bind(Vertex& p_vertex);
		void Release(Vertex& p_vertex);

	public:
		Vertex(VertexDescriptorPtr p_pVertexDescriptor);
		Vertex(VertexDescriptorPtr p_pVertexDescriptor, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList);
		~Vertex(void);
		
		Vertex& operator= (const Vertex& p_vertex);
		Vertex& operator++ (int p_nPostfix);
		Vertex& operator-- (int p_nPostfix);

		void Rebind(VertexDescriptorPtr p_pVertexDescriptor, Meson::Common::Collections::TArrayList<byte*> p_vertexDataList);
		void Rebind(VertexDescriptorPtr p_pVertexDescriptor);
		void TransferSemantic(VertexDescriptor::Semantic p_eSemantic, uint p_uiChannel, Vertex& p_vertex);

		bool HasColour(void) const;
		bool HasPosition(void) const;
		bool HasNormal(void) const;
		bool HasBinormal(void) const;
		bool HasTangent(void) const;
		bool HasTextureCoordinates1D(void) const;
		bool HasTextureCoordinates2D(void) const;
		bool HasTextureCoordinates3D(void) const;
		bool HasTextureCoordinates4D(void) const;

		uint GetColourCount(void) const;
		uint GetPositionCount(void) const;
		uint GetNormalCount(void) const;
		uint GetBinormalCount(void) const;
		uint GetTangentCount(void) const;
		uint GetTextureCoordinates1DCount(void) const;
		uint GetTextureCoordinates2DCount(void) const;
		uint GetTextureCoordinates3DCount(void) const;
		uint GetTextureCoordinates4DCount(void) const;

		Colour& GetColour(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector3f& GetPosition(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector3f& GetNormal(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector3f& GetBinormal(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector3f& GetTangent(uint p_uiChannel = 0);
		float& GetTextureCoordinates1D(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector2f& GetTextureCoordinates2D(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector3f& GetTextureCoordinates3D(uint p_uiChannel = 0);
		Meson::Common::Maths::Vector4f& GetTextureCoordinates4D(uint p_uiChannel = 0);

		void SetColour(const Colour& p_colour, uint p_uiChannel = 0);
		void SetPosition(const Meson::Common::Maths::Vector3f& p_vec3Position, uint p_uiChannel = 0);
		void SetNormal(const Meson::Common::Maths::Vector3f& p_vec3Normal, uint p_uiChannel = 0);
		void SetBinormal(const Meson::Common::Maths::Vector3f& p_vec3Binormal, uint p_uiChannel = 0);
		void SetTangent(const Meson::Common::Maths::Vector3f& p_vec3Tangent, uint p_uiChannel = 0);
		void SetTextureCoordinates1D(float p_fTextureCoordinates, uint p_uiChannel = 0);
		void SetTextureCoordinates2D(const Meson::Common::Maths::Vector2f& p_vec2TextureCoordinates, uint p_uiChannel = 0);
		void SetTextureCoordinates3D(const Meson::Common::Maths::Vector3f& p_vec3TextureCoordinates, uint p_uiChannel = 0);
		void SetTextureCoordinates4D(const Meson::Common::Maths::Vector4f& p_vec4TextureCoordinates, uint p_uiChannel = 0);

	protected:
		bool m_bOwner;

		Meson::Common::Collections::TArrayList<byte*> m_vertexDataList;
		VertexDescriptorPtr m_pVertexDescriptor;
	};

	typedef Meson::Common::TPointer<Vertex> VertexPtr;

	//----------------------------------------------------------------------------------------------
	// VertexData section
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VertexData : 
		public Meson::Common::TReferenceCounter<VertexData>
	{
	protected:
		void Build(VertexDescriptorPtr p_pVertexDescriptor, uint p_uiInitialVertexCount);
		void RefreshVertexCounts(void);

	public:
		VertexData(VertexDescriptorPtr p_pVertexDescriptor);
		VertexData(VertexDescriptorPtr p_pVertexDescriptor, uint p_uiInitialVertexCount);
		~VertexData(void);

		void Reorganise(VertexDescriptorPtr p_pVertexDescriptor);
		const VertexDescriptorPtr GetVertexDescriptor(void);

		VertexPtr GetVertex(uint p_uiVertexIndex);
		void SetVertex(uint p_uiVertexIndex, const Vertex& p_vertex);
		void Swap(uint p_uiVertexIndex1, uint p_uiVertexIndex2);

		void Add(const Vertex& p_vertex);
		void Insert(uint p_uiVertexIndex, const Vertex& p_vertex);
		void RemoveAt(uint p_uiVertexIndex);
		void RemoveAll(void);

		void* GetBuffer(uint p_uiBufferIndex = 0, uint p_uiVertexIndex = 0);
		Meson::Common::Collections::TArrayList<void*> GetBuffer(uint p_uiVertexIndex = 0);

		void SetBuffer(uint p_uiBufferIndex, uint p_uiVertexIndex, uint p_uiVertexCount, const void* p_pBuffer);
		void SetBuffer(uint p_uiVertexIndex, uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList);

		void AddBuffer(uint p_uiBufferIndex, uint p_uiVertexCount, const void* p_pBuffer);
		void AddBuffer(uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList);

		void InsertBuffer(uint p_uiBufferIndex, uint p_uiVertexIndex, uint p_uiVertexCount, const void* p_pBuffer);
		void InsertBuffer(uint p_uiVertexIndex, uint p_uiVertexCount, Meson::Common::Collections::TArrayList<void*> p_bufferList);

		uint GetMaximumVertexCount(void);
		uint GetMinimumVertexCount(void);
		uint GetVertexCount(uint p_uiBufferIndex);
		
		uint GetVertexSize(void) const;
		uint GetBufferCount(void) const;
		uint GetPartialVertexSize(uint p_uiBufferIndex) const;

		VertexPtr operator[] (uint p_uiIndex);

	protected:
		Meson::Common::Collections::TArrayList<ArrayBufferPtr> m_pVertexArrayList;
		VertexDescriptorPtr m_pVertexDescriptor;

		bool m_bIsStale;

		uint m_uiMinimumVertexCount, 
			 m_uiMaximumVertexCount;
	};
	
	typedef Meson::Common::TPointer<VertexData> VertexDataPtr;

Meson_Vistas_END
