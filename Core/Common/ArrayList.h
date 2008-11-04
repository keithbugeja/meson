//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "List.h"
#include "MesonException.h"
#include "Buffer.h"
#include "Pointer.h"
#include "DebugSupport.h"

// Disable DLL export warnings
#pragma warning (disable:4251)

Meson_Common_Collections_BEGIN

#define GROWTHFACTOR 1.25f

Meson::Common::Memory::MemoryManager *GetArrayListMemoryManager(void);

template< class TElement>
class TArrayList;

template< class TElement>
class TArrayListEnumerator
	: public TEnumerator<TElement>
{
private:
	TArrayList<TElement>* m_pArrayList;
	size_t m_unIndex;
	bool* m_pbEnumerating;

public:
	TArrayListEnumerator(TArrayList<TElement>* p_pArrayList, bool* p_pbEnumerating)
		: m_pArrayList(p_pArrayList)
		, m_unIndex(0)
		, m_pbEnumerating(p_pbEnumerating)
	{
	}

	bool HasMoreElements(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("List was modified during enumeration",
				__FILE__, __LINE__);

		return m_unIndex < m_pArrayList->Size();
	}

	void Reset(void)
	{
		m_unIndex = 0;
	}

	TElement& NextElement(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("List was modified during enumeration",
				__FILE__, __LINE__);

		if( m_unIndex >= m_pArrayList->Size() )
			throw new MesonException( "No more elements to enumerate.", __FILE__, __LINE__ );

		return (*m_pArrayList)[m_unIndex++];
	}

	size_t Size( void ) const
	{
		return m_pArrayList->Size();
	}

	size_t Available( void ) const
	{
		return m_pArrayList->Size() - m_unIndex;
	}
};

// use of 'this' in array list enumerator constructor
#pragma warning( disable : 4355 )

template< class TElement >
class TArrayList
	: public TList<TElement>
{
protected:
	TElement*	m_pBuffer;
	size_t		m_unBufferSize,
				m_unSize;
	float		m_fFactor;
	bool		m_bEnumerating;
	TArrayListEnumerator<TElement> m_arrayListEnumerator;



public: // Constructors and Destructor

	TArrayList( void )
		: m_arrayListEnumerator(this, &m_bEnumerating)
	{
		Init( 8 );
	}

	explicit TArrayList( int p_nSize )
		: m_arrayListEnumerator(this, &m_bEnumerating)
	{
		Init( p_nSize );
	}

	explicit TArrayList( size_t p_unSize )
		: m_arrayListEnumerator(this, &m_bEnumerating)
	{
		Init( (int)p_unSize );
	}

	TArrayList( const TImmutableList<TElement> &p_tList )
		: m_arrayListEnumerator(this, &m_bEnumerating)
	{
		m_fFactor = GROWTHFACTOR;
		Init( Growth( p_tList.Size() ) );
		AddList( p_tList );
	}

	TArrayList( const TArrayList<TElement> &p_tArrayList )
		: m_arrayListEnumerator(this, &m_bEnumerating)
	{
		m_fFactor = GROWTHFACTOR;
		Init( Growth( p_tArrayList.Size() ) );
		AddList( p_tArrayList );
	}

	~TArrayList( void )
	{
		delete[] m_pBuffer;
	}

	void *operator new( size_t p_unSize )
	{
		return GetArrayListMemoryManager()->AllocatePooled( p_unSize );
	}

	void operator delete(void *p_pObject, size_t p_unSize)
	{
		::operator delete( p_pObject );
	}

public: // public ImmutableList interface methods
	//------------------------------------------------------------------------------------------
	/// Accesses an element safely via the given index using the indexing operator. An exception
	/// is thrown if the index is out of range.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	inline TElement& operator[](size_t p_unIndex) const
	{
#ifdef MESON_DEBUG
		return GetElement( p_unIndex );
#else
		return m_pBuffer[ p_unIndex ];
#endif
	}

	//------------------------------------------------------------------------------------------
	/// Accesses an element via the given index using the functor operator. No range checking is
	/// performed on the index as the operator is intended for fast element access.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	inline TElement& operator()(size_t p_unIndex) const
	{
		return m_pBuffer[ p_unIndex ];
	}

	//------------------------------------------------------------------------------------------
	/// Returns the size of the list.
	/// \returns the size of the list.
	//------------------------------------------------------------------------------------------
	inline size_t Size(void) const
	{
		return m_unSize;
	}

	//------------------------------------------------------------------------------------------
	/// Accesses an element safely via a given index. An exception is thrown if the index is out
	/// of range.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	inline TElement& GetElement(size_t p_unIndex) const
	{
		if( p_unIndex >= m_unSize )
			throw new MesonException("ArrayList access out of range!", __FILE__, __LINE__ );

		return m_pBuffer[ p_unIndex ];
	}

	//------------------------------------------------------------------------------------------
	/// Returns the index of the given element if found in the list, or -1 otherwise.
	/// \returns the index of the given element if found in the list, or -1 otherwise.
	//------------------------------------------------------------------------------------------
	inline int IndexOf(const TElement& p_element) const
	{
		TElement* pLeft  = m_pBuffer;
		TElement* pRight = m_pBuffer + m_unSize - 1;

		while( pLeft <= pRight )
		{
			if( *pLeft == p_element )
				return (int)( pLeft - m_pBuffer );
			else if( *pRight == p_element )
				return (int)( pRight - m_pBuffer );
			else
			{
				pLeft++;
				pRight--;
			}
		}

		return -1;
	}

	//------------------------------------------------------------------------------------------
	/// Returns a reference to a TEnumerator for enumerating the elements of the list.
	/// \returns a reference to a TEnumerator for enumerating the elements of the list.
	//------------------------------------------------------------------------------------------
	inline TEnumerator<TElement>& GetEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_arrayListEnumerator.Reset();
		m_bEnumerating = true;
		return m_arrayListEnumerator;
	}

	inline typename TEnumerator<TElement>::Ptr CreateEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TElement>::Ptr(new TArrayListEnumerator<TElement>(this, &m_bEnumerating));
	}

public: // public List interface methods
	//------------------------------------------------------------------------------------------
	/// Assigns the given list into this list via the assignment operator.
	/// \param p_list Reference to a source list to assign.
	/// \returns a reference to this list.
	//------------------------------------------------------------------------------------------
	inline TArrayList<TElement>& operator=(const TList<TElement>& p_list)
	{
		if( &p_list == this ) return *this;

		//Clear();
		m_unSize = 0;
		m_arrayListEnumerator.Reset();
		AddList( p_list );
		return *this;
	}

	//------------------------------------------------------------------------------------------
	/// Assigns the given arraylist into this arraylist via the assignment operator.
	/// \param p_list Reference to a source list to assign.
	/// \returns a reference to this list.
	//------------------------------------------------------------------------------------------
	inline TArrayList<TElement>& operator=(const TArrayList<TElement>& p_list)
	{
		if( &p_list == this ) return *this;

		//Clear();
		m_unSize = 0;
		AddList( p_list );
		return *this;
	}


	//------------------------------------------------------------------------------------------
	/// Clears the contents of the list.
	//------------------------------------------------------------------------------------------
	inline void Clear(void)
	{
		m_unSize = 0;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Modifies an element safely via a given index. An exception is thrown if the index is out
	/// of range.
	/// \param p_unIndex Index of the element to access.
	/// \param p_element The new element value.
	//------------------------------------------------------------------------------------------
	inline void SetElement(size_t p_unIndex, const TElement& p_element)
	{
		if( p_unIndex >= m_unSize )
			throw new MesonException("ArrayList access out of range!");

		m_pBuffer[ p_unIndex ] = p_element;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Appends the given element to the list.
	/// \param p_element Reference to an element to append.
	//------------------------------------------------------------------------------------------
	inline void Add(const TElement& p_element)
	{
		if( m_unSize >= m_unBufferSize )
			EnsureCapacity( Growth( m_unSize ) );

		m_pBuffer[ m_unSize ] = p_element;
		m_unSize++;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Appends the given list to this list.
	/// \param p_list Reference to a list to append.
	//------------------------------------------------------------------------------------------
	inline void AddList(const TImmutableList<TElement>& p_list)
	{
		size_t unSize = p_list.Size();
		EnsureCapacity( Growth( (m_unSize + unSize) ) );

		for( size_t unIndex = 0; unIndex < unSize; unIndex++ )
		{
			m_pBuffer[ m_unSize ] = p_list[ unIndex ];
			m_unSize++;
		}
		m_bEnumerating = false;
	}


	//------------------------------------------------------------------------------------------
	/// Appends the given arraylist to this arraylist.
	/// \param p_list Reference to a list to append.
	//------------------------------------------------------------------------------------------
	inline void AddList(const TArrayList<TElement>& p_list)
	{
		size_t unSize = p_list.Size();
		EnsureCapacity( Growth( m_unSize + unSize ) );

		BufferCopyFwd( m_pBuffer + m_unSize, ((TArrayList<TElement>&)p_list).GetBuffer(), unSize );
		m_unSize += unSize;

		m_bEnumerating = false;
	}


	//------------------------------------------------------------------------------------------
	/// Inserts the given element to the list at the given index.
	/// \param p_unIndex List index for the element to be inserted.
	/// \param p_elemet Reference to a element to insert.
	//------------------------------------------------------------------------------------------
	inline void Insert(size_t p_unIndex,
		const TElement& p_element)
	{
		if( p_unIndex > m_unSize )
			throw new MesonException("Buffer access out of range!");

		if( m_unSize+1 >= m_unBufferSize )
			Resize( Growth( m_unSize ) );

		if( p_unIndex != m_unSize ) 
			CopyBck( m_pBuffer + m_unSize, m_pBuffer + m_unSize - 1, ( m_unSize - p_unIndex ) );

		m_pBuffer[ p_unIndex ] = p_element;
		m_unSize++;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Inserts the given list at the given index.
	/// \param p_unIndex List index where the given list is to be inserted.
	/// \param p_list Reference to a list to insert.
	//------------------------------------------------------------------------------------------
	inline void InsertList(size_t p_unIndex,
		const TImmutableList<TElement>& p_list)
	{
		size_t unLength = p_list.Size();
		if( !unLength )
			return;

		if( !m_unSize || p_unIndex == m_unSize )
		{
			AddList( p_list );
			return;
		}

		size_t unTmp = m_unSize + unLength;
		
		if( p_unIndex == 0 )
		{
			// if a resize is required at the begining leave a gap while resizing
			if( unTmp > m_unBufferSize )
				Resize( Growth( unTmp ), m_unSize );
		}
		else
		{
			// if a resize is required leave a gap at the insertion point
			if( unTmp > m_unBufferSize )
				Resize( Growth( unTmp ), p_unIndex, unLength );
			else
				CopyBck( m_pBuffer + unTmp - 1, m_pBuffer + m_unSize - 1, ( m_unSize - p_unIndex ) );
		}

		// Copy the list passed as parameter
		for( size_t unIndex = 0; unIndex < unLength; unIndex++ )
		{
			m_pBuffer[ p_unIndex ] = p_list[ unIndex ];
			p_unIndex++;
		}

		m_unSize += unLength;
		m_bEnumerating = false;
	}


	//------------------------------------------------------------------------------------------
	/// Inserts the given list at the given index.
	/// \param p_unIndex List index where the given list is to be inserted.
	/// \param p_list Reference to a list to insert.
	//------------------------------------------------------------------------------------------
	inline void InsertList(size_t p_unIndex,
		const TArrayList<TElement>& p_list)
	{
		size_t unLength = p_list.Size();
		if( !unLength )
			return;

		if( !m_unSize || p_unIndex == m_unSize )
		{
			AddList( p_list );
			return;
		}

		size_t unTmp = m_unSize + unLength;
		
		if( p_unIndex == 0 )
		{
			// if a resize is required at the begining leave a gap while resizing
			if( unTmp > m_unBufferSize )
				Resize( Growth( unTmp ), m_unSize );
		}
		else
		{
			// if a resize is required leave a gap at the insertion point
			if( unTmp > m_unBufferSize )
				Resize( Growth( unTmp ), p_unIndex, unLength );
			else
				CopyBck( m_pBuffer + unTmp - 1, m_pBuffer + m_unSize - 1, ( m_unSize - p_unIndex ) );
		}

		// Copy the list passed as parameter
		BufferCopyFwd( m_pBuffer + p_unIndex, ((TArrayList<TElement>&)p_list).GetBuffer(), unLength );

		m_unSize += unLength;
		m_bEnumerating = false;
	}



	//------------------------------------------------------------------------------------------
	/// Inserts the elements of the given enumerator at the given index.
	/// \param p_unIndex List index where the given list is to be inserted.
	/// \param p_enumerator Reference to the enumerator to be processed.
	//------------------------------------------------------------------------------------------
	inline void InsertEnumerator(size_t p_unIndex,
		TEnumerator<TElement>& p_enumerator)
	{
		if (p_unIndex > m_unSize)
			throw new MesonException("ArrayList index out of range.", __FILE__, __LINE__ );

		size_t unLength = p_enumerator.Size();

		if( p_unIndex == m_unSize )
		{
			EnsureCapacity( Growth( m_unSize + unLength ) );
		}
		else
		{
			if( Growth(  m_unSize + unLength ) <= (int)m_unBufferSize )
				InsertGap( p_unIndex, unLength );
			else
				Resize( Growth( m_unSize + unLength ), p_unIndex, unLength );
		}

		p_enumerator.Reset();
		while( p_enumerator.HasMoreElements() )
		{
			m_pBuffer[ p_unIndex++ ] = p_enumerator.NextElement();
		}
		
		m_unSize += unLength;
		m_bEnumerating = false;
	}


	//------------------------------------------------------------------------------------------
	/// Removes the element at the given index.
	/// \param p_unIndex Index of the element to be removed.
	//------------------------------------------------------------------------------------------
	inline void RemoveAt(size_t p_unIndex) 
	{
		if( p_unIndex >= m_unSize )
			throw new MesonException("ArrayList access out of range!", __FILE__, __LINE__ );

		TElement* pTmp = m_pBuffer + p_unIndex;
		CopyFwd( pTmp, pTmp + 1, ( m_unSize - p_unIndex - 1 ) );
		m_unSize--;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Removes all instances of the given element matched via the equality operator from the
	/// list.
	/// \param p_element Element matching the instances to be removed.
	//------------------------------------------------------------------------------------------
	inline void Remove(const TElement& p_element)
	{
		TElement* pTmp = m_pBuffer;
		for( size_t unIndex = 0; unIndex < m_unSize; )
		{
			if( m_pBuffer[ unIndex ] == p_element )
			{
				//RemoveAt( unIndex );
				m_unSize--;
				CopyFwd( pTmp, pTmp + 1, ( m_unSize - unIndex ) );
			}
			else
			{
				unIndex++;
				pTmp++;
			}
		}
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Removes a range of elements given by a starting index and an element count.
	/// \param p_unIndex Starting index for the element range to be removed.
	/// \param p_unLength Count of elements to be removed from the starting index onwards.
	//------------------------------------------------------------------------------------------
	inline void RemoveRange(size_t p_unIndex, size_t p_unLength)
	{
		size_t unTmp = p_unIndex + p_unLength;
		
		if( unTmp >= m_unBufferSize )
			throw new MesonException("Buffer access out of range!");

		CopyFwd( m_pBuffer + p_unIndex, m_pBuffer + unTmp, ( m_unSize - unTmp ) );
		m_unSize -= p_unLength;
		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Reverses the order of the elements within the list using the assignment operator to
	/// swap the elements across.
	//------------------------------------------------------------------------------------------
	inline void Reverse(void)
	{
		TElement *pLeft = m_pBuffer;
		TElement *pRight = m_pBuffer + m_unSize - 1;
		TElement element;

		while( pLeft < pRight )
		{
			element = *pLeft;
			*pLeft = *pRight;
			*pRight = element;
			pLeft++;
			pRight--;
		}

		m_bEnumerating = false;
	}

	//------------------------------------------------------------------------------------------
	/// Returns an immutable version of the list.
	/// \returns an immutable version of the list.
	//------------------------------------------------------------------------------------------
	inline TImmutableList<TElement>& AsImmutableList(void) const
	{
		return ( TImmutableList<TElement>& ) *this;
	}


public: // public methods
	inline void SetGrowthFactor( float p_fFactor )
	{
		if( p_fFactor < 1.1f )
			throw new MesonException("Invalid ArrayList growth factor!");

		m_fFactor = p_fFactor;
	}

	inline float GetGrowthFactor( void )
	{
		return m_fFactor;
	}


	inline void EnsureCapacity( size_t p_unSize )
	{
		if( p_unSize > m_unBufferSize )
		{
			TElement* pNewBuffer = new TElement[ p_unSize ];
			
			if( m_unSize > 0 )
				BufferCopyFwd( pNewBuffer, m_pBuffer, m_unSize );

			m_unBufferSize = p_unSize;
			delete[] m_pBuffer;
			m_pBuffer = pNewBuffer;
		}
	}	


	inline void Swap( size_t p_unIndex1, size_t p_unIndex2 )
	{
		if( p_unIndex1 == p_unIndex2 )
			return;

		TElement element = m_pBuffer[ p_unIndex1 ];
		m_pBuffer[ p_unIndex1 ] = m_pBuffer[ p_unIndex2 ];
		m_pBuffer[ p_unIndex2 ] = element;
		m_bEnumerating = false;
	}

	inline void *ElementPtr( size_t p_unIndex  ) const
	{
		return ( m_pBuffer + p_unIndex );
	}

	inline TElement Pop( size_t p_unIndex )
	{
		if( p_unIndex >= m_unBufferSize )
			throw new MesonException("ArrayList access out of range!", __FILE__, __LINE__ );

		TElement element = m_pBuffer[ p_unIndex ];

		TElement* pTmp = m_pBuffer + p_unIndex;
		CopyFwd( pTmp, pTmp + 1, ( m_unSize - p_unIndex - 1 ) );
		m_unSize--;
		m_bEnumerating = false;

		return element;
	}

	inline TElement* GetBuffer( void )
	{
		return m_pBuffer;
	}

	inline TArrayList<TElement> operator+(const TElement& p_list)
	{
		TArrayList<TElement> lstResult( *this );
		lstResult.Add( p_list );
		return lstResult;
	}

	inline TArrayList<TElement> operator+(const TList<TElement>& p_list)
	{
		TArrayList<TElement> lstResult( *this );
		lstResult.AddList( p_list );
		return lstResult;
	}

	inline bool operator==(const TList<TElement>& p_list) const
	{
		if (m_unSize != p_list.Size())
			return false;
		for (size_t unIndex = 0; unIndex < m_unSize; unIndex++)
			if (m_pBuffer[unIndex] != p_list[unIndex])
				return false;
		return true;
	}

private: // private internal methods

	inline int Growth( size_t p_unSize )
	{
		float fRes = m_fFactor;
		fRes *= (float) p_unSize;
		fRes++;
		return (int) fRes;
	}

	void Init( int p_nSize )
	{
		m_unBufferSize = ( p_nSize < 1 ) ? 1 : p_nSize;

		m_unSize = 0;
		m_pBuffer = new TElement[ m_unBufferSize ];

		m_bEnumerating = false;
		m_fFactor = GROWTHFACTOR;
	}

	inline void CopyBck( TElement* p_pDst, TElement* p_pSrc, size_t p_unItems )
	{
		for( ; p_unItems; p_unItems-- )
		{
			*p_pDst = *p_pSrc;
			p_pDst--;
			p_pSrc--;
		}
	}

	inline void CopyFwd( TElement* p_pDst, TElement* p_pSrc, size_t p_unItems )
	{
		for( ; p_unItems; p_unItems-- )
		{
			*p_pDst = *p_pSrc;
			p_pDst++;
			p_pSrc++;
		}
	}


	inline void BufferCopyFwd( TElement* p_pDst, TElement* p_pSrc, size_t p_unItems )
	{
		TElement *pDstRt = p_pDst + p_unItems - 1;
		TElement *pSrcRt = p_pSrc + p_unItems - 1;
		
		while( p_pDst < pDstRt )
		{
			*p_pDst = *p_pSrc;
			*pDstRt = *pSrcRt;

			p_pDst++;
			p_pSrc++;
			pSrcRt--;
			pDstRt--;
		}

		// If odd number of items copy last element
		if( p_unItems & 1 )
			*p_pDst = *p_pSrc;
	}


	inline void Resize( size_t p_unSize )
	{
		TElement* pNewBuffer = new TElement[ p_unSize ];
		if( m_unSize > 0 )
			BufferCopyFwd( pNewBuffer, m_pBuffer, m_unSize );

		m_unBufferSize = p_unSize;
		delete[] m_pBuffer;
		m_pBuffer = pNewBuffer;
	}

	inline void Resize( size_t p_unSize, size_t p_unOffset )
	{
		if( p_unSize + p_unOffset > m_unBufferSize )
		{
			TElement* pNewBuffer = new TElement[ p_unSize ];
			
			if( m_unSize > 0 )
				BufferCopyFwd( pNewBuffer + p_unOffset, m_pBuffer, m_unSize  );

			m_unBufferSize = p_unSize;
			delete[] m_pBuffer;
			m_pBuffer = pNewBuffer;
		}
		else
			throw new MesonException("Invalid size specified.", __FILE__, __LINE__ );
	}


	inline void Resize( size_t p_unSize, size_t p_unCut, size_t p_unGap )
	{
		TElement* pNewBuffer = new TElement[ p_unSize ];
		
		if( m_unSize > 0 )
		{
			BufferCopyFwd( pNewBuffer, m_pBuffer, p_unCut );
			BufferCopyFwd( pNewBuffer + p_unCut + p_unGap, m_pBuffer + p_unCut, m_unSize - p_unCut );
		}
		m_unBufferSize = p_unSize;
		delete[] m_pBuffer;
		m_pBuffer = pNewBuffer;
	}

	inline void InsertGap( size_t p_unCut, size_t p_unGap )
	{
		if( m_unSize > 0 )
		{
			CopyBck( m_pBuffer + m_unSize + p_unGap - 1, m_pBuffer + m_unSize - 1, m_unSize - p_unCut );
		}
	}

};

Meson_Common_Collections_END

template<class TElement>
Meson::Common::Collections::TArrayList<TElement>
	operator+(
		const TElement &p_element,
		const Meson::Common::Collections::TArrayList<TElement> &p_list)
{
	Meson::Common::Collections::TArrayList<TElement> listResult(p_list);
	listResult.Insert(0, p_element);
	return listResult;
};
