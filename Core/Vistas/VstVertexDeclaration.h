//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "TreeSet.h"

#include "VstTypes.h"
#include "VstResource.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	/** Vertex data types
	 */
	//----------------------------------------------------------------------------------------------
	enum VertexDataType
	{
		VDT_FLOAT1 = 0,
		VDT_FLOAT2,
		VDT_FLOAT3,
		VDT_FLOAT4,

		VDT_FLOAT16_2,
		VDT_FLOAT16_4,

		VDT_UBYTE4,
		VDT_UBYTE4N,

		VDT_SHORT2,
		VDT_SHORT4,

		VDT_SHORT2N,
		VDT_SHORT4N,

		VDT_USHORT2N,
		VDT_USHORT4N,

		VDT_COLOUR,
		
		VDT_COUNT
	};

	//----------------------------------------------------------------------------------------------
	/** Vertex data functions
	 */
	//----------------------------------------------------------------------------------------------
	enum VertexDataFunction
	{
		VDF_POSITION = 0,
		VDF_BLEND_WEIGHT,
		VDF_BLEND_INDICES,
		VDF_NORMAL,
		VDF_TANGENT,
		VDF_BINORMAL,
		VDF_SPECULAR,
		VDF_DIFFUSE,
		VDF_COLOUR,
		VDF_TEXTURE_COORDINATES,
		
		VDF_COUNT
	};

	//----------------------------------------------------------------------------------------------
	/** Vertex element
	 */ 
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VertexElement
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Copy constructor for vertex element object.
			\param VertexElement Reference to a vertex element object
		 */
		//----------------------------------------------------------------------------------------------
		VertexElement(const VertexElement& p_vertexElement);

		//----------------------------------------------------------------------------------------------
		/** Constructs a vertex element object.
			\param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
			\param p_uiOffset Position of element within vertex declaration; byte offset.
			\param p_eVertexDataType The type of the specified element
			\param p_eVertexDataFunction The semantic function of the specified element
			\param p_eUsageIndex The index for multiple semantically-identical elements
		 */
		//----------------------------------------------------------------------------------------------
		VertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);
		
		//----------------------------------------------------------------------------------------------
		/** Sets the properties of the vertex element object.
			\param p_uiOffset Position of element within vertex declaration; byte offset.
			\param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
			\param p_eVertexDataType The type of the specified element
			\param p_eVertexDataFunction The semantic function of the specified element
			\param p_eUsageIndex The index for multiple semantically-identical elements
		 */
		//----------------------------------------------------------------------------------------------
		void SetVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);

		//----------------------------------------------------------------------------------------------
		/** Returns whether the element's data type is the same as the data type specified.
			\return True if the same, false otherwise
		 */
		//----------------------------------------------------------------------------------------------
		bool IsVertexDataType(VertexDataType p_eVertexDataType) const;
		
		//----------------------------------------------------------------------------------------------
		/** Returns whether the element's data function is the same as the data function specified.
			\return True if the same, false otherwise
		 */
		//----------------------------------------------------------------------------------------------
		bool IsVertexDataFunction(VertexDataFunction p_eVertexDataFunction) const;

		//----------------------------------------------------------------------------------------------
		/** Returns the data type of the specified element.
			\return Element data type
		 */
		//----------------------------------------------------------------------------------------------
		VertexDataType GetVertexDataType(void) const;

		//----------------------------------------------------------------------------------------------
		/** Returns the function of the specified element.
			\return Element function
		 */
		//----------------------------------------------------------------------------------------------
		VertexDataFunction GetVertexDataFunction(void)const;

		//----------------------------------------------------------------------------------------------
		/** Returns the usage index of the specified element; the usage index is the index of an
			element, given multiple semantically-identical elements in a declaration.
			\return Usage index
		 */
		//----------------------------------------------------------------------------------------------
		uint GetUsageIndex(void) const;

		//----------------------------------------------------------------------------------------------
		/** Returns the stream number the specified element is bound to.
			\return Stream
		 */
		//----------------------------------------------------------------------------------------------
		uint GetStream(void) const;

		//----------------------------------------------------------------------------------------------
		/** Returns the position offset of the specified element within a vertex declaration.
			\return Position offset
		 */
		//----------------------------------------------------------------------------------------------
		uint GetOffset(void) const;
		
		//----------------------------------------------------------------------------------------------
		/** Sets the vertex data type of the vertex element object.
			\param p_eVertexDataType The type of the specified element
		 */
		//----------------------------------------------------------------------------------------------
		void SetVertexDataType(VertexDataType p_eVertexDataType);

		//----------------------------------------------------------------------------------------------
		/** Sets the vertex data function of the vertex element object.
			\param p_eVertexDataFunction The semantic function of the specified element
		 */
		//----------------------------------------------------------------------------------------------
		void SetVertexDataFunction(VertexDataFunction p_eVertexDataFunction);
		
		//----------------------------------------------------------------------------------------------
		/** Sets the usage index of the vertex element object.
			\param p_eUsageIndex The index for multiple semantically-identical elements
		 */
		//----------------------------------------------------------------------------------------------
		void SetUsageIndex(uint p_uiUsageIndex);
		
		//----------------------------------------------------------------------------------------------
		/** Sets the stream binding of the vertex element object.
			\param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
		 */
		//----------------------------------------------------------------------------------------------
		void SetStream(uint p_uiStream);
		
		//----------------------------------------------------------------------------------------------
		/** Sets the starting offset of the vertex element object.
			\param p_uiOffset Position of element within vertex declaration; byte offset.
		 */
		//----------------------------------------------------------------------------------------------
		void SetOffset(uint p_uiOffset);

	protected:
		VertexDataType m_eVertexDataType;
		VertexDataFunction m_eVertexDataFunction;
		uint m_uiUsageIndex;
		uint m_uiStream;
		uint m_uiOffset;
	};

	//----------------------------------------------------------------------------------------------
	/** VertexDeclaration
	 */
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VertexDeclaration : public Resource
	{
	protected:
		void RefreshStreamSet(void);

	public:
		virtual ~VertexDeclaration(void);
		
		//----------------------------------------------------------------------------------------------
		/** Adds a vertex element to the vertex declaration.
			\param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
			\param p_uiOffset Position of element within vertex declaration; byte offset.
			\param p_eVertexDataType The type of the specified element
			\param p_eVertexDataFunction The semantic function of the specified element
			\param p_eUsageIndex The index for multiple semantically-identical elements
		 */
		//----------------------------------------------------------------------------------------------
		virtual void AddVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);
		
		//----------------------------------------------------------------------------------------------
		/** Appends a vertex element to the vertex declaration. Note that the element offset is set
			to follow the last element in the supplied stream.
			\param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
			\param p_eVertexDataType The type of the specified element
			\param p_eVertexDataFunction The semantic function of the specified element
			\param p_eUsageIndex The index for multiple semantically-identical elements
		 */
		//----------------------------------------------------------------------------------------------
		virtual void AppendVertexElement(uint p_uiStream, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);

		//----------------------------------------------------------------------------------------------
		/** Removes the vertex element with specific offset.
			\param p_uiOffset Offset in bytes of vertex element
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RemoveVertexElement(uint p_uiStream, uint p_uiOffset);
		
		//----------------------------------------------------------------------------------------------
		/** Removes all vertex elements from vertex declaration.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RemoveAllVertexElements(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the vertex element with specific offset.
			\param p_uiOffset Offset in bytes of vertex element
		 */
		//----------------------------------------------------------------------------------------------
		virtual const VertexElement* GetVertexElement(uint p_uiStream, uint p_uiOffset);

		//----------------------------------------------------------------------------------------------
		/** Returns the vertex size for the specified stream.
			\param p_uiStream Stream to query for size
			\return Vertex size in bytes
		 */
		//----------------------------------------------------------------------------------------------
		virtual uint GetStreamSize(uint p_uiStream);

		//----------------------------------------------------------------------------------------------
		/** Checks whether the vertex declaration contains a valid vertex element at the given offset
			with the given stream.
			\param p_uiStream Stream number
			\param p_uiOffset Offset within stream in bytes
			\return <b>true</b> if a vertex element exists at the specified location, <b>false</b> 
					otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool IsAllocated(uint p_uiStream, uint p_uiOffset);

		//----------------------------------------------------------------------------------------------
		/** Checks whether the vertex declaration contains a valid vertex element at the given offset
			with the given stream, returing the element in the process.
			\param p_uiStream Stream number
			\param p_uiOffset Offset within stream in bytes
			\param p_pVertexElement Out pointer for returning the vertex element
			\return <b>true</b> if a vertex element exists at the specified location, <b>false</b> 
					otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool IsAllocated(uint p_uiStream, uint p_uiOffset, VertexElement** p_pVertexElement);

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		typedef Meson::Common::Collections::TEnumerator<uint>& StreamEnumerator;
		virtual StreamEnumerator GetStreamEnumerator(void)
		{
			return m_streamSet.GetEnumerator();
		}

		//----------------------------------------------------------------------------------------------
		/** Returns the count of vertex elements within the specified stream which employ the given
			data function.

			\param p_uiStream Stream number within which to search vertex elements.
			\param p_eVertexDataFunction Data function to look for.
		  */
		//----------------------------------------------------------------------------------------------
		virtual uint GetCount(uint p_uiStream, VertexDataFunction p_eVertexDataFunction);

		//----------------------------------------------------------------------------------------------
		/** Returns the count of vertex elements within the specified stream which employ the given
			data function and data type.

			\param p_uiStream Stream number within which to search vertex elements.
			\param p_eVertexDataFunction Data function to look for.
			\param p_eVertexDataType Data type to look for.
		  */
		//----------------------------------------------------------------------------------------------
		virtual uint GetCount(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType);

		virtual bool Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);
		virtual bool Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex, VertexElement** p_pVertexElement);
		virtual bool Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType, uint p_uiUsageIndex);
		virtual bool Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType, uint p_uiUsageIndex, VertexElement** p_pVertexElement);

		//----------------------------------------------------------------------------------------------
		/** Returns the size of a specific vertex data type.
			\param p_eVertexDataType Vertex data type to query for size
			\return Size in bytes of vertex data type
		 */
		//----------------------------------------------------------------------------------------------
		static uint SizeOf(VertexDataType p_eVertexDataType);

	protected:
		Meson::Common::Collections::TTreeSet<uint> m_streamSet;
		Meson::Common::Collections::TArrayList<VertexElement*> m_vertexElementList;
	};

	typedef Meson::Common::TPointer<VertexDeclaration> VertexDeclarationPtr;

Meson_Vistas_END