#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Enumerator.h"
#include "List.h"
#include "ArrayList.h"

#define DEFAULTSIZE 32
#define EXPANDFACTOR 1.5f

#include <iostream>

Meson_Common_Collections_BEGIN

template< class TElement > class TStack : public TEnumerator< TElement >
{
protected:
	TElement *m_pBegin,
			 *m_pTop,
			 *m_pEnd;

	float m_fFactor;

	bool   m_bEnumerating;
	size_t m_unEnumeratorPos;

public:
	TStack( void )
	{
		Init( DEFAULTSIZE );
	}

	explicit TStack( size_t p_unSize )
	{
		Init( p_unSize );
	}

	TStack( const TStack<TElement> &p_stack )
	{
		Init( p_stack.BufferSize() );
		Copy( m_pBegin, p_stack.m_pBegin, p_stack.Size() );
		m_pTop +=  p_stack.Size();
	}

	TStack( const TImmutableList<TElement> &p_list )
	{
		Init( (size_t)(p_list.Size() * EXPANDFACTOR)  );
		Push( p_list.GetEnumerator() );
	}

	TStack( const TEnumerator<TElement> &p_enumerator )
	{
		Init( DEFAULTSIZE );
		
		p_enumerator.Reset();
		Push( p_enumerator );
	}

	~TStack( void )
	{
		if( m_pBegin )
			delete[] m_pBegin;
	}

	inline void SetExpandFactor( float p_fFactor )
	{
		if( m_fFactor > 1 )
			m_fFactor = p_fFactor;
	}

	inline size_t BufferSize( void ) const
	{
		return m_pEnd - m_pBegin;
	}

	void EnsureBufferSize( size_t p_unSize )
	{
		if( p_unSize < BufferSize() )
			return;

		p_unSize = (size_t)(p_unSize * m_fFactor);
		TElement *pElement = new TElement[ p_unSize ];
		Copy( pElement, m_pBegin, Size() );
		m_pTop = pElement + Size();
		delete[] m_pBegin;
		m_pBegin = pElement;
		m_pEnd = m_pBegin + p_unSize;
	}


	inline size_t Size( void ) const
	{
		return m_pTop - m_pBegin;
	}

	inline size_t Available( void ) const
	{
		return Size() - m_unEnumeratorPos;
	}

	inline bool IsEmpty( void ) const
	{
		return ( m_pTop == m_pBegin );
	}

	inline bool IsFull( void ) const
	{
		return ( m_pTop >= m_pEnd );
	}

	void Clear( void )
	{
		m_pTop = m_pBegin;
		m_bEnumerating = false;
	}

	void Push_Unsafe( TElement &p_element )
	{
		*m_pTop = p_element;
		m_pTop++;

		m_bEnumerating = false;
	}

	void Push( const TElement &p_element )
	{
		if( IsFull() )
			Realloc();

		*m_pTop = p_element;
		m_pTop++;

		m_bEnumerating = false;
	}

	void Push( const TStack<TElement> &p_stack )
	{
		size_t unSize = p_stack.Size();
		EnsureBufferSize( Size() + unSize );
		if( &p_stack == this )
		{
			Copy( m_pTop, p_stack.m_pBegin, unSize );
		}
		else
		{
			Copy( m_pBegin, p_stack.m_pBegin, unSize );
		}
		m_pTop +=  unSize;
	}

	void Push( const TEnumerator<TElement> &p_enumerator )
	{
		( (TEnumerator<TElement>&) p_enumerator ).Reset();
		while( ( (TEnumerator<TElement>&) p_enumerator ).HasMoreElements() )
		{
			Push( ( (TEnumerator<TElement>&) p_enumerator ).NextElement() );
		}
	}

	TElement Pop_Unsafe( void )
	{
		m_pTop--;
		m_bEnumerating = false;
		return *m_pTop;
	}

	TElement Pop( void )
	{
		if( m_pTop == m_pBegin )
			throw new MesonException( "Stack index out of range.", __FILE__, __LINE__ );

		m_pTop--;
		m_bEnumerating = false;
		return *m_pTop;
	}


	void Pop_Unsafe( TList<TElement>& p_list, size_t p_unSize )
	{
		for( size_t un = 0; (un < p_unSize) && (m_pBegin < m_pTop); un++ )
		{
			m_pTop--;
			p_list.Add( *m_pTop );
		}

		m_bEnumerating = false;
	}


	void Pop( TList<TElement>& p_list, size_t p_unSize )
	{
		if( ( m_pTop - p_unSize ) < m_pBegin )
			throw new MesonException( "Stack index out of range.", __FILE__, __LINE__ );

		for( size_t un = 0; (un < p_unSize) && (m_pBegin < m_pTop); un++ )
		{
			m_pTop--;
			p_list.Add( *m_pTop );
		}

		m_bEnumerating = false;
	}


	void RemoveTop( size_t p_unSize )
	{
		if( ( m_pTop - p_unSize ) >= m_pBegin )
			m_pTop -= p_unSize;

		m_bEnumerating = false;
	}

	void RemoveTop_Safe( size_t p_unSize )
	{
		if( ( m_pTop - p_unSize ) >= m_pBegin )
			m_pTop -= p_unSize;
		else
			throw new MesonException( "Stack index out of range.", __FILE__, __LINE__ );

		m_bEnumerating = false;
	}

	TElement &Peek( void )
	{
		if( m_pTop > m_pBegin )
		{
			return *( m_pTop - 1 );
		}
		else
			throw new MesonException( "Stack index out of range.", __FILE__, __LINE__ );
	}

	
	void Peek( TList<TElement>& p_list, size_t p_unSize )
	{
		for( size_t un = 0; (un < p_unSize); un++ )
		{
			p_list.Add( *( m_pTop - un ) );
		}

		m_bEnumerating = false;
	}


	TElement &ElementAt( size_t p_unIndex ) const
	{
		if( p_unIndex >= Size() )
			throw new MesonException( "Stack index out of range.", __FILE__, __LINE__ );

		return m_pBegin[ p_unIndex ];
	}

	int IndexOf( const TElement &p_element ) const
	{
		size_t unSize = Size();
		for( size_t unIndex = 0; unIndex < unSize; unIndex++ )
		{
			if( m_pBegin[ unIndex ] == p_element )
				return unIndex;
		}
		return -1;
	}

	TEnumerator<TElement> &GetEnumerator( void )
	{
		Reset();
		return *this;
	}

	bool HasMoreElements( void )
	{
		return ( m_pBegin < m_pTop ) && ( m_unEnumeratorPos < Size() );
	}

	void Reset( void )
	{
		m_unEnumeratorPos = 0;
		m_bEnumerating = true;
	}

	TElement &NextElement(void)
	{
		if( !m_bEnumerating )
			throw new MesonException( "Stack was modified during enumeration", __FILE__, __LINE__ );

		if( !HasMoreElements() )
			throw new MesonException( "No more elements to enumerate.", __FILE__, __LINE__ );

		return m_pBegin[ m_unEnumeratorPos++ ];		
	}

	void Reverse( void )
	{
		size_t unSize = Size() >> 1;

		TElement *pElement1 = m_pBegin;
		TElement *pElement2 = m_pTop-1;
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

		m_bEnumerating = false;
	}



	void DuplicateTop( void )
	{
		if( !IsEmpty() )
		{
			TElement element = *( m_pTop - 1 );
			Push( element );
		}
	}
	void operator =( const TStack< TElement > &p_stack )
	{
		Clear();
		Push( p_stack );
	}

	void operator +=( const TElement &p_element )
	{
		Push( p_element );
	}

	void operator +=( const TStack< TElement > &p_stack )
	{
		Push( p_stack );
	}

	TElement operator-( void )
	{
		return Pop();
	}

	bool operator !( void ) const
	{
		return IsEmpty();
	}

	TElement &operator[]( size_t p_unIndex ) const
	{
		return ElementAt( p_unIndex );
	}

	TElement &operator()( size_t p_unIndex ) const
	{
		return m_pBegin[ p_unIndex ];
	}

protected:
	void Init( size_t p_unSize )
	{
		m_fFactor = EXPANDFACTOR;
		m_pBegin = new TElement[ p_unSize ];
		m_pTop = m_pBegin;
		m_pEnd = m_pBegin + p_unSize;
		m_bEnumerating = false;
		m_unEnumeratorPos = 0;
	}

	void Copy( TElement *p_pDst, TElement *p_pSrc, size_t unSize )
	{
		for( size_t un = 0; un < unSize; un++ )
		{
			*p_pDst = *p_pSrc;
			p_pDst++;
			p_pSrc++;
		}
	}

	void Realloc( void )
	{
		size_t unSize = (size_t)(BufferSize() * m_fFactor);
		TElement *pElement = new TElement[ unSize ];
		Copy( pElement, m_pBegin, Size() );
		m_pTop = pElement + Size();
		delete[] m_pBegin;
		m_pBegin = pElement;
		m_pEnd = m_pBegin + unSize;
	}
	
public:
	void Print( void )
	{
		std::cout << "Stack[";
		TEnumerator<TElement> &enumerator = GetEnumerator();
		enumerator.Reset();
		while( enumerator.HasMoreElements() )
		{
			std::cout << enumerator.NextElement() << ",";
		}
		std::cout << "]" << std::endl;
	}
};

Meson_Common_Collections_END
