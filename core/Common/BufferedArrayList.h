#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "List.h"
#include "MesonException.h"

#define FACTOR 1.2f

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetBufferedArrayListMemoryManager(void);

template<class TElement>
class TBufferedArrayList
	: public TList<TElement>
	, private TEnumerator<TElement>
{
protected:
	TElement *m_pElement;

	size_t	m_unSize,
			m_unBufferSize,
			m_unBufferStart;

	float  m_fFactor;

	bool   m_bEnumerating;
	size_t m_unEnumeratorPos;

public:
	TBufferedArrayList( void )
	{
		m_unSize = 0;
		m_unBufferSize = 32;
		m_unBufferStart = 8;
		m_fFactor = FACTOR;
		m_pElement = new TElement[ m_unBufferSize ];	
	}

	explicit TBufferedArrayList( size_t p_unSize )
	{
		m_unSize = 0;
		m_unBufferSize = p_unSize;
		m_unBufferStart = p_unSize >> 2;
		m_fFactor = FACTOR;
		m_pElement = new TElement[ m_unBufferSize ];
	}

	TBufferedArrayList( const TImmutableList<TElement> &p_tList )
	{
		this->TArrayList<TElement>::TBufferedArrayList( (size_t)( p_tList.Size() * FACTOR ) );
		AddList( p_tList );
	}

	TBufferedArrayList( const TBufferedArrayList<TElement> &p_tBufferedArrayList )
	{
		m_unSize = p_tBufferedArrayList.m_unSize;
		m_unBufferSize = p_tBufferedArrayList.m_unBufferSize;
		m_unBufferStart = p_tBufferedArrayList.m_unBufferStart;
		m_fFactor = p_tBufferedArrayList.m_fFactor;
		m_pElement = new TElement[ m_unBufferSize ];

		Copy( m_pElement, p_tBufferedArrayList.m_pElement, m_unBufferSize );
	}

	~TBufferedArrayList( void )
	{
		if( m_pElement )
			delete[] m_pElement;
	}

	void SetExpandFactor( float p_fFactor )
	{
		m_fFactor = p_fFactor;
	}

	void *operator new( size_t p_unSize )
	{
		return GetBufferedArrayListMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void *p_pObject, size_t p_unSize)
	{
		::operator delete( p_pObject );
	}

	
	size_t Size( void ) const
	{
		return m_unSize;
	}

	TElement &ElementAt( size_t p_unIndex ) const
	{
		if( p_unIndex >= m_unSize )
			throw new MesonException( "List index out of range.", __FILE__, __LINE__ );

		return m_pElement[ m_unBufferStart + p_unIndex ];
	}
	
	int IndexOf(const TElement &p_element) const
	{
		for( uint unIndex = 0; unIndex < m_unSize; unIndex++ )
		{
			if( m_pElement[ m_unBufferStart + unIndex ] == p_element )
				return unIndex;
		}
		return -1;
	}
	
	void Clear(void)
	{
		m_unSize = 0;
		m_unBufferStart = m_unBufferSize >> 2;
	}

	void Add( const TElement &p_element )
	{
		if( m_unSize == ( m_unBufferSize - m_unBufferStart ) )
			Realloc();

		m_pElement[ m_unBufferStart + m_unSize ] = p_element;
		m_unSize++;
		m_bEnumerating = false;
	}

	void AddList( const TImmutableList<TElement> &p_list )
	{
		TElement *pElement = m_pElement + m_unBufferStart + m_unSize;

		size_t unSize = p_list.Size();

		m_unSize += unSize;
		if( m_unSize >= ( m_unBufferSize - m_unBufferStart ) )
			Realloc( m_unSize );
	
		for( size_t unIndex = 0; unIndex < unSize; unIndex++ )
		{
			*(pElement) = p_list( unIndex );
			pElement++;
		}
		m_bEnumerating = false;
	}

	void Insert(size_t p_unIndex, const TElement &p_element)
	{
		if( p_unIndex == 0 )
		{
			if( m_unBufferStart == 0 )
				Realloc();

			m_unBufferStart--;
			m_pElement[ m_unBufferStart ] = p_element;
			m_unSize++;
		}
		else if( p_unIndex == ( m_unSize - 1 ) )
		{
			if( m_unSize == ( m_unBufferSize - m_unBufferStart ) )
				Realloc();

			m_pElement[ m_unBufferStart + m_unSize ] = p_element;
			m_unSize++;
		}
		else
		{
			size_t unRight = m_unSize - p_unIndex;

			if( p_unIndex < unRight )
			{
				if( m_unBufferStart == 0 )
					Realloc();

				m_unBufferStart--;
				
				Copy( m_pElement + m_unBufferStart, m_pElement + m_unBufferStart + 1, p_unIndex, false );
			}
			else
			{
				if( m_unSize >= m_unBufferSize )
					Realloc();
				Copy( m_pElement + m_unBufferStart + m_unSize, m_pElement + m_unBufferStart + m_unSize + 1, unRight, true );
			}
			m_pElement[ m_unBufferStart + p_unIndex ] = p_element;
			m_unSize++;
		}

		m_bEnumerating = false;
	}

	void InsertList( size_t p_unIndex, const TImmutableList<TElement> &p_list )
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.", __FILE__, __LINE__ );

		
		size_t unSize = p_list.Size();
		Realloc( unSize );

		TEnumerator<TElement> &enumerator = ((TList<TElement> &)p_list).GetEnumerator();
		
		while( enumerator.HasMoreElements() )
			Insert(p_unIndex++, enumerator.NextElement());
	}

	void InsertEnumerator( size_t p_unIndex, TEnumerator<TElement> &p_enumerator )
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.", __FILE__, __LINE__ );

		p_enumerator.Reset();
		while( p_enumerator.HasMoreElements() )
			Insert(p_unIndex++, p_enumerator.NextElement());
	}

	void RemoveAt( size_t p_unIndex )
	{
		if( p_unIndex == 0 )
		{
			m_unBufferStart++;
			m_unSize--;
		}
		else if( p_unIndex == ( m_unSize - 1 ) )
		{
			m_unSize--;
		}
		else
		{
			size_t unRight = m_unSize - p_unIndex;

			if( p_unIndex < unRight )
			{
				Copy( m_pElement + m_unBufferStart + p_unIndex, m_pElement + m_unBufferStart + p_unIndex - 1, p_unIndex, true );
				m_unBufferStart++;
			}
			else
			{
				Copy( m_pElement + m_unBufferStart + p_unIndex, m_pElement + m_unBufferStart + p_unIndex + 1, unRight, false );
			}
			m_unSize--;	
		}
		
		m_bEnumerating = false;
	}
	

	void Remove( const TElement &p_element )
	{
		uint unIndex = 0;
		for( ; unIndex < m_unSize; unIndex++ )
		{
			if( m_pElement[ m_unBufferStart + unIndex ] == p_element )
				break;
		}
		RemoveAt( unIndex );
	}
	


	void RemoveRange( size_t p_unIndex, size_t p_unLength )
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException( "List index out of range.", __FILE__, __LINE__ );

		size_t unRightIndex = p_unIndex + p_unLength;

		if ( unRightIndex > m_unSize)
			throw new MesonException( "List length out of range.", __FILE__, __LINE__ );

		size_t unRight = m_unSize - unRightIndex;
		if( p_unIndex < unRight )
		{
			Copy( m_pElement + m_unBufferStart + p_unIndex + p_unLength - 1, m_pElement + m_unBufferStart + p_unIndex + p_unLength - 2, p_unIndex + p_unLength, true );
			m_unBufferStart += p_unLength;
		}
		else
		{
			Copy( m_pElement + m_unBufferStart + p_unIndex, m_pElement + m_unBufferStart + p_unIndex + p_unLength, m_unSize - p_unLength - p_unIndex, false );
		}
		m_unSize -= p_unLength;
	}
	



	TBufferedArrayList<TElement> SubList( size_t p_unIndex, size_t p_unLength ) const
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.", __FILE__, __LINE__ );

		if (p_unIndex + p_unLength > m_unSize)
			throw new MesonException("List length out of range.", __FILE__, __LINE__ );

		TBufferedArrayList<TElement> lstSub( (size_t)( p_unLength * m_fFactor) );
		
		
		TBufferedArrayList<TElement> *_self_ = ((TBufferedArrayList<TElement>*)this);

		_self_->Copy( lstSub.m_pElement + lstSub.m_unBufferStart, 
			_self_->m_pElement + _self_->m_unBufferStart + p_unIndex, 
			p_unLength, false );

		lstSub.m_unSize = p_unLength;

		return lstSub;
	}

	void Reverse( void )
	{
		size_t unSize = m_unSize >> 1;

		TElement *pElement1 = m_pElement + m_unBufferStart;
		TElement *pElement2 = m_pElement + m_unBufferStart + m_unSize;
		TElement element;

		while( pElement1 < pElement2 )
		{
			element = *pElement1;
			*pElement1 = *pElement2;
			*pElement2 = element;
			pElement1++;
			pElement2--;
		}

		m_bEnumerating = false;
	}

	inline void Swap( size_t p_unIndex1, size_t p_unIndex2 )
	{
		if( p_unIndex1 == p_unIndex2 )
			return;

		TElement element = m_pElement[ p_unIndex1 ];
		m_pElement[ p_unIndex1 ] = m_pElement[ p_unIndex2 ];
		m_pElement[ p_unIndex2 ] = element;
	}

	TImmutableList<TElement> &AsImmutableList( void ) const
	{
		return (TImmutableList<TElement> &) *this;
	}

	inline TBufferedArrayList<TElement> operator+( const TElement &p_element )
	{
		TBufferedArrayList<TElement> listNew;
		listNew.AddList( *this );
		listNew.Add( p_element );
		return listNew;
	}

	inline TBufferedArrayList<TElement> operator-( const TElement &p_element )
	{
		TBufferedArrayList<TElement> listNew;
		listNew.AddList( *this );
		listNew.Remove( p_element );
		return listNew;
	}

	TList<TElement> &operator=(const TList<TElement> &p_list)
	{
		if( &p_list == this ) return *this;

		Clear();
		AddList( p_list );
		return *this;
	}

	inline TElement &operator[](size_t p_unIndex) const
	{
		return ElementAt(p_unIndex);
	}

	inline TElement &operator()( size_t p_unIndex ) const
	{
		return m_pElement[ m_unBufferStart + p_unIndex ];
	}

	TEnumerator<TElement> &GetEnumerator( bool p_bReset = true )
	{
		if (p_bReset || !m_bEnumerating)
			Reset();
		return *this;
	}


	inline bool HasMoreElements(void)
	{
		return (m_unEnumeratorPos < m_unSize);
	}

	inline void Reset(void)
	{
		m_unEnumeratorPos = 0;
		m_bEnumerating = true;
	}

	TElement &NextElement(void)
	{
		if( !m_bEnumerating )
			throw new MesonException( "List was modified during enumeration", __FILE__, __LINE__ );

		if( !HasMoreElements() )
			throw new MesonException( "No more elements to enumerate.", __FILE__, __LINE__ );

		size_t unPos = m_unEnumeratorPos++;
		
		return m_pElement[ m_unBufferStart + unPos ];
	}


	TBufferedArrayList<TElement> operator+(const TList<TElement> &p_list)
	{
		TBufferedArrayList<TElement> listResult(*this);
		listResult.AddList(p_list);
		return listResult;
	}

	void EnsureBufferSize( size_t unPreBufferSize, size_t unBufferSize )
	{
		if( unBufferSize < m_unSize )
				unBufferSize = m_unSize;

		size_t unNewSize = (size_t)( (unPreBufferSize + unBufferSize) * m_fFactor );

		TElement *pElement = new TElement[ unNewSize ];
		size_t unNewStart = unPreBufferSize;

		Copy( pElement + unPreBufferSize, m_pElement + m_unBufferStart, m_unSize );

		delete[] m_pElement;
		m_pElement = pElement;
		
		m_unBufferSize = unNewSize;
		m_unBufferStart = unPreBufferSize;

	}

protected:

	inline void Copy( TElement *p_pDst, const TElement *p_pSrc, size_t p_unSize, bool p_bReverse = false )
	{
		if( p_bReverse )
		{
			for( size_t unPos = 0; unPos < p_unSize; unPos++)
			{
				// p_pDst[unPos] = p_pSrc[unPos];
				*p_pDst = *p_pSrc;
				p_pDst--;
				p_pSrc--;
			}
		}
		else
		{
			for( size_t unPos = 0; unPos < p_unSize; unPos++)
			{
				//p_pDst[unPos] = p_pSrc[unPos];
				*p_pDst = *p_pSrc;
				p_pDst++;
				p_pSrc++;
			}
		}
	}


	inline void Realloc( size_t p_unSize )
	{
		size_t unNewSize = (size_t)( p_unSize * m_fFactor );

		if( unNewSize >= m_unBufferSize )
		{
			TElement *pElement = new TElement[ unNewSize ];
			size_t unNewStart = ( unNewSize - m_unBufferSize ) >> 1;

			Copy( pElement + unNewStart, m_pElement + m_unBufferStart, m_unSize );

			delete[] m_pElement;
			m_pElement = pElement;
			
			m_unBufferSize = unNewSize;
			m_unBufferStart = unNewStart;
		}
	}

	inline void Realloc( void )
	{
		size_t unNewSize = (size_t)( m_unBufferSize * m_fFactor );
		TElement *pElement = new TElement[ unNewSize ];
		size_t unNewStart = ( unNewSize - m_unBufferSize ) >> 1;

		Copy( pElement + unNewStart, m_pElement + m_unBufferStart, m_unSize );

		delete[] m_pElement;
		m_pElement = pElement;
		
		m_unBufferSize = unNewSize;
		m_unBufferStart = unNewStart;
	}

};

Meson_Common_Collections_END

template<class TElement>
Meson::Common::Collections::TBufferedArrayList<TElement>
	operator+(
		const TElement &p_element,
		const Meson::Common::Collections::TBufferedArrayList<TElement> &p_list)
{
	TBufferedArrayList<TElement> listResult(p_list);
	listResult.Insert(0, p_element);
	return listResult;
};
