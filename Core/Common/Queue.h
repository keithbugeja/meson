//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "MesonException.h"
#include "List.h"

// Disable DLL export warnings
#pragma warning (disable:4251)

Meson_Common_Collections_BEGIN

template< class TElement>
class TQueue;

template< class TElement>
class TQueueEnumerator
	: public TEnumerator<TElement>
{
private:
	TQueue<TElement>* m_pQueue;
	size_t m_unIndex;
	bool* m_pbEnumerating;

public:
	TQueueEnumerator(TQueue<TElement>* p_pQueue, bool* p_pbEnumerating)
		: m_pQueue(p_pQueue)
		, m_unIndex(0)
		, m_pbEnumerating(p_pbEnumerating)
	{
	}

	bool HasMoreElements(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("Queue was modified during enumeration",
				__FILE__, __LINE__);

		return m_unIndex < m_pQueue->Size();
	}

	void Reset(void)
	{
		m_unIndex = 0;
	}

	TElement& NextElement(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("Queue was modified during enumeration",
				__FILE__, __LINE__);

		if( m_unIndex >= m_pQueue->Size() )
			throw new MesonException( "No more elements to enumerate.", __FILE__, __LINE__ );

		return (*m_pQueue)[m_unIndex++];
	}

	size_t Size( void ) const
	{
		return m_pQueue->Size();
	}

	size_t Available( void ) const
	{
		return m_pQueue->Size() - m_unIndex;
	}
};

// use of 'this' in queue enumerator constructor
#pragma warning( disable : 4355 )

template< class TElement > 
class TQueue : public TImmutableList< TElement >
{
protected:
	TElement	*m_pBegin,
				*m_pEnd,
				*m_pFirst,
				*m_pLast;
	bool   m_bEnumerating;
	TQueueEnumerator<TElement> m_queueEnumerator;

public:
	TQueue( void )
		: m_queueEnumerator(this, &m_bEnumerating)
	{
		Init( 8 );
	}

	explicit TQueue( size_t p_unSize )
		: m_queueEnumerator(this, &m_bEnumerating)
	{
		Init( p_unSize );
	}

	TQueue( const TQueue< TElement > &p_queue )
		: m_queueEnumerator(this, &m_bEnumerating)
	{
		size_t unSize = p_queue.Size();
		Init( unSize << 1 );
		for( size_t un = 0; un < unSize; un++ )
		{
			Push( p_queue( un ) );
		}
	}

	~TQueue( void )
	{
		if( m_pBegin )
			delete[] m_pBegin;
	}

	void Clear( void )
	{
		m_pFirst = m_pLast = m_pBegin;
	}

	size_t Size( void ) const
	{
		return ( m_pLast - m_pFirst );
	}


	void Push( TElement p_element )
	{
		if( m_pLast >= m_pEnd )
			Realloc();

		*m_pLast = p_element;
		m_pLast++;

		m_bEnumerating = false;
	}


	void Push( TImmutableList< TElement > &p_list )
	{
		size_t unSize = p_list.Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			Push( p_list( un ) );
		}

		m_bEnumerating = false;
	}
	
	TElement Pop( void )
	{
		TElement element = *m_pFirst;
		m_pFirst++;
		
		if( m_pFirst == m_pLast )
			m_pFirst = m_pLast = m_pBegin;
		
		m_bEnumerating = false;

		return element;
	}


	void Pop( TList< TElement > &p_list, size_t p_unSize )
	{
		for( size_t un = 0; un < p_unSize; un++ )
		{
			p_list.Add( Pop() );
		}

		m_bEnumerating = false;
	}

	void PushRef( TElement &p_element )
	{
		if( m_pLast >= m_pEnd )
			Realloc();

		*m_pLast = p_element;
		m_pLast++;

		m_bEnumerating = false;
	}

	
	TElement &PopRef( void )
	{
		TElement &element = *m_pFirst;
		m_pFirst++;
		
		if( m_pFirst == m_pLast )
			m_pFirst = m_pLast = m_pBegin;
		
		m_bEnumerating = false;

		return element;
	}

	
	TElement &GetElement( size_t p_unIndex ) const
	{
		if( p_unIndex >= (unsigned int)( m_pLast - m_pFirst ) )
			throw new MesonException( "Queue index out of range.", __FILE__, __LINE__ );

		return m_pBegin[ p_unIndex ];
	}

	int IndexOf(const TElement &p_element) const
	{
		
		size_t unSize = Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			if( (*this)( un ) == p_element )
				return (int)un;
		}
		return -1;
	}

	TElement &operator[]( size_t p_unIndex ) const
	{
		return GetElement( p_unIndex );
	}

	TElement &operator()( size_t p_unIndex ) const
	{
		return m_pBegin[ p_unIndex ];
	}


	void operator=( TQueue< TElement > &p_queue )
	{
		Clear();

		size_t unSize = p_queue.Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			Push( p_queue( un ) );
		}

		m_bEnumerating = false;
	}


	inline TEnumerator<TElement> &GetEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_queueEnumerator.Reset();
		m_bEnumerating = true;
		return m_queueEnumerator;
	}

	inline typename TEnumerator<TElement>::Ptr CreateEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TElement>::Ptr(new TQueueEnumerator<TElement>(this, &m_bEnumerating));
	}

protected:
	inline void Init( size_t p_unSize )
	{
		m_pBegin = new TElement[p_unSize];
		m_pFirst = m_pLast = m_pBegin;
		m_pEnd   = m_pBegin + p_unSize;
		
		m_bEnumerating = false;
	}

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

	void Realloc( void )
	{
		size_t unSize = Size() << 1;
		TElement *pTemp = new TElement[ unSize ];
		Copy( pTemp, m_pBegin, Size() );
		m_pFirst = pTemp + ( m_pFirst - m_pBegin );
		m_pLast  = pTemp + ( m_pLast - m_pBegin );
		delete[] m_pBegin;
		m_pBegin = pTemp;
		m_pEnd   = m_pBegin + unSize;
	}

};

Meson_Common_Collections_END
