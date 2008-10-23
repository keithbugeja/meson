#pragma once
//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	26/02/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "MesonException.h"
#include "Pointer.h"


Meson_Common_Collections_BEGIN

#define BUFFER_DUAL_LEFT_GROWTH_FACTOR 1.25f
#define BUFFER_DUAL_RIGHT_GROWTH_FACTOR 1.25f


template< typename TElement > 
class TBufferDual 
{

public:
	typedef void (*Visitor)( TElement& );


#pragma region Attributes
protected:

	TElement* m_pBuffer;
	TElement* m_pLeft;
	TElement* m_pRight;

	size_t  m_unBufferSize;

	float	m_fLtFactor,
			m_fRtFactor;

	int m_nReferenceCount;


#pragma endregion 


#pragma region Construction


public:
	TBufferDual( void )
	{
		SetGrowthFactors( BUFFER_DUAL_LEFT_GROWTH_FACTOR, BUFFER_DUAL_RIGHT_GROWTH_FACTOR );
		Init( 12 );
	}


	explicit TBufferDual( size_t p_unSize )
	{
		if( p_unSize == 0 )
			throw new MesonException( "Buffer size 0.", __FILE__, __LINE__ );

		SetGrowthFactors( BUFFER_DUAL_LEFT_GROWTH_FACTOR, BUFFER_DUAL_RIGHT_GROWTH_FACTOR );
		Init( Growth( p_unSize ) );
	}

	explicit TBufferDual( int p_nSize )
	{
		if( p_nSize <= 0 )
			throw new MesonException( "Buffer size 0.", __FILE__, __LINE__ );

		SetGrowthFactors( BUFFER_DUAL_LEFT_GROWTH_FACTOR, BUFFER_DUAL_RIGHT_GROWTH_FACTOR );
		Init( Growth( (size_t)p_nSize ) );
	}

	TBufferDual( const TBufferDual& p_buffer )
	{
		SetGrowthFactors( BUFFER_DUAL_LEFT_GROWTH_FACTOR, BUFFER_DUAL_RIGHT_GROWTH_FACTOR );
		Init( Growth( p_buffer.Size() ) );
		CopyFwd( m_pLeft, p_buffer.m_pLeft, p_buffer.Size() );
		m_pRight += p_buffer.Size();
	}

	TBufferDual( const TElement* p_pBuffer, size_t p_unSize )
	{
		if( p_unSize == 0 )
			throw new MesonException( "Buffer size 0.", __FILE__, __LINE__ );
		
		SetGrowthFactors( BUFFER_DUAL_LEFT_GROWTH_FACTOR, BUFFER_DUAL_RIGHT_GROWTH_FACTOR );
		Init( Growth( p_unSize ) );
		CopyFwd( m_pLeft, (TElement*)p_pBuffer, p_unSize );
		m_pRight += p_unSize;
	}

	~TBufferDual( void )
	{
		if( m_pBuffer )
		{
			delete[] m_pBuffer;
		}
	}

#pragma endregion

#pragma region Reference Counting Methods

public:
	inline void IncrementReferenceCount( void )
	{ 
		m_nReferenceCount++; 
	}

	inline int GetReferenceCount( void ) 
	{
		return m_nReferenceCount; 
	}
	
	inline void DecrementReferenceCount( void )
	{
		m_nReferenceCount--;
		if(!m_nReferenceCount)
			delete this; 
	}

#pragma endregion


#pragma region Internal methods

protected:
	inline size_t Growth( size_t p_unSize ) 
	{
		float fRes = ( 1.0f + m_fLtFactor + m_fRtFactor );
		fRes *= (float)p_unSize;
		fRes++;
		return (size_t) fRes;
	}


	inline void Init( size_t p_unSize )
	{
		size_t unLeft = (size_t)( m_fLtFactor * p_unSize );

		m_pBuffer = new TElement[ p_unSize ];
		m_pLeft = m_pBuffer + unLeft;
		m_pRight = m_pLeft;

		m_unBufferSize = p_unSize;

		m_nReferenceCount = 0;
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
		memcpy( p_pDst, p_pSrc, p_unItems * sizeof( TElement ) );
	}


	inline void Resize( size_t p_unNewSize, size_t p_unCut, size_t p_unLength  )
	{
		size_t unLeft = (size_t)( m_fLtFactor * p_unNewSize );

		TElement* pNewBuffer = new TElement[ p_unNewSize ];
		
		if( Size() > 0 )
		{
			CopyFwd( pNewBuffer + unLeft, m_pLeft, p_unCut );
			CopyFwd( pNewBuffer + unLeft + p_unCut + p_unLength, m_pLeft + p_unCut, Size() - p_unCut );
		}

		m_unBufferSize = p_unNewSize;
		delete[] m_pBuffer;
		m_pBuffer = pNewBuffer;

		m_pRight  = m_pBuffer + unLeft + Size() + p_unLength;
		m_pLeft   = m_pBuffer + unLeft;

	}


	inline void InsertGap( size_t p_unIndex, size_t p_unLength )
	{
		if( (size_t)( m_pLeft - m_pBuffer ) > p_unLength )
		{
			//Available Left
			CopyFwd( m_pLeft - p_unLength, m_pLeft, p_unIndex );
			m_pLeft -= p_unLength;
		}
		else if( (size_t)( m_pBuffer + m_unBufferSize - m_pRight ) > p_unLength )
		{
			//Available Right
			CopyBck( m_pRight + p_unLength, m_pRight, p_unLength );
			m_pRight += p_unLength;
		}
		else
			Resize( Growth( m_unBufferSize + p_unLength ), p_unIndex, p_unLength );

	}



#pragma endregion


#pragma region Public Operations
public:

	inline size_t Size( void ) const
	{
		return m_pRight - m_pLeft;
	}

	inline void SetGrowthFactors( float p_fLtFactor, float p_fRtFactor )
	{
		if( p_fLtFactor < 1.1f )
			p_fLtFactor = 1.1f;

		if( p_fRtFactor < 1.1f )
			p_fRtFactor = 1.1f;

		m_fLtFactor = p_fLtFactor - 1;
		m_fRtFactor = p_fRtFactor - 1;
	}

	inline TElement GetElementAt( size_t p_unIndex ) const
	{
		if( p_unIndex > Size() )
			throw new MesonException( "Buffer index out of range.", __FILE__, __LINE__ );

		return m_pLeft[ p_unIndex ];
	}
		
	inline void SetElementAt( size_t p_unIndex, const TElement& p_element )
	{
		if( p_unIndex > Size() )
			throw new MesonException( "Buffer index out of range.", __FILE__, __LINE__ );

		m_pLeft[ p_unIndex ] = p_element;
	}


	inline void Clear( void )
	{
		size_t unLeft = (size_t)( m_fLtFactor * m_unBufferSize );
		m_pLeft = m_pBuffer + unLeft;
		m_pRight = m_pLeft;
	}

	TElement* GetData( void ) const
	{
		return m_pLeft;
	}

	TElement* GetDataEnd( void ) const
	{
		return m_pRight - 1;
	}


	inline void EnsureBufferSize( size_t p_unSize )
	{
		EnsureCapacity( Growth( p_unSize ) );
	}

	inline void EnsureCapacity( size_t p_unSize )
	{
		size_t unLeft = (size_t)( m_fLtFactor * p_unSize );
		TElement* pNewBuffer = new TElement[ p_unSize ];
		CopyFwd( pNewBuffer + unLeft, m_pLeft, Size() );
		delete[] m_pBuffer;
		
		m_pBuffer = pNewBuffer;
		m_pRight  = m_pBuffer + unLeft + Size();
		m_pLeft   = m_pBuffer + unLeft;

		m_unBufferSize = p_unSize;
	}


	inline void EnsureCapacity( size_t p_unLeftSize, size_t p_unRightSize )
	{
		m_unBufferSize = p_unLeftSize + Size() + p_unRightSize;
		TElement* pNewBuffer = new TElement[ m_unBufferSize ];
		CopyFwd( pNewBuffer + p_unLeftSize, m_pLeft, Size() );
		delete[] m_pBuffer;
		
		m_pBuffer = pNewBuffer;
		m_pRight  = m_pBuffer + p_unLeftSize + Size();
		m_pLeft   = m_pBuffer + p_unLeftSize;
	}


	inline void Append( const TElement& p_element )
	{
		if( ( m_pRight + 1 ) >= ( m_pBuffer + m_unBufferSize ) )
			EnsureCapacity( Growth( Size() ) );

		*m_pRight = p_element;
		m_pRight++;
	}

	inline void Append( const TElement* p_pElement, size_t p_unSize )
	{
		if( ( m_pRight + p_unSize ) >= ( m_pBuffer + m_unBufferSize ) )
			EnsureCapacity( Growth( Size() + p_unSize ) );

		CopyFwd( m_pRight, (TElement*)p_pElement, p_unSize );
		m_pRight += p_unSize;
	}

	inline void Append( const TBufferDual<TElement>& p_buffer )
	{
		if( ( m_pRight + p_buffer.Size() ) >= ( m_pBuffer + m_unBufferSize ) )
			EnsureCapacity( Growth( Size() + p_buffer.Size() ) );

		CopyFwd( m_pRight, p_buffer.GetData(), p_buffer.Size() );
		m_pRight += p_buffer.Size();
	}


	inline void Prepend( const TElement& p_element )
	{
		if( m_pLeft <= m_pBuffer )
			EnsureCapacity( Growth( Size() ) );

		m_pLeft--;
		*m_pLeft = p_element;
	}

	inline void Prepend( const TElement* p_pElement, size_t p_unSize )
	{
		if( ( m_pLeft - p_unSize ) <= m_pBuffer )
			EnsureCapacity( p_unSize, m_unBufferSize - Size() );

		CopyFwd( m_pLeft - p_unSize, (TElement*)p_pElement, p_unSize );
		m_pLeft -= p_unSize;
	}

	
	inline void Prepend( const TBufferDual<TElement>& p_buffer )
	{
		if( ( m_pLeft - p_buffer.Size() ) <= m_pBuffer )
			EnsureCapacity( p_buffer.Size(), m_unBufferSize - Size() );

		CopyFwd( m_pLeft - p_buffer.Size(), p_buffer.GetData(), p_buffer.Size() );
		m_pLeft -= p_buffer.Size();
	}



	inline void Insert( size_t p_unIndex, const TElement& p_element )
	{
		if( p_unIndex == Size() )
		{
			Append( p_element );
			return;
		}
		else if( p_unIndex == 0 )
		{
			Prepend( p_element );
			return;
		}
		else
		{
			InsertGap( p_unIndex, 1 );
			m_pLeft[ p_unIndex ] = p_element;
		}
	}



	inline void Insert( size_t p_unIndex, const TElement* p_pElement, size_t p_unSize )
	{
		if( p_unIndex == Size() )
		{
			Append( p_pElement, p_unSize );
			return;
		}
		else if( p_unIndex == 0 )
		{
			Prepend( p_pElement, p_unSize );
			return;
		}
		else
		{
			InsertGap( p_unIndex, p_unSize );
			CopyFwd( m_pLeft + p_unIndex, (TElement*)p_pElement, p_unSize );
		}
	}



	inline void Insert( size_t p_unIndex, const TBufferDual<TElement>& p_buffer )
	{
		if( p_unIndex == Size() )
		{
			Append( p_buffer );
			return;
		}
		else if( p_unIndex == 0 )
		{
			Prepend( p_buffer );
			return;
		}
		else
		{
			InsertGap( p_unIndex, p_buffer.Size() );
			CopyFwd( m_pLeft + p_unIndex, p_buffer.GetData(), p_buffer.Size() );
		}
	}



	inline void RemoveAt( size_t p_unIndex )
	{
		size_t unSize = Size();
		if( p_unIndex >= unSize )
			throw new MesonException( "Buffer index out of range.", __FILE__, __LINE__ );
		
		if( p_unIndex == 0 )
		{
			m_pLeft++;
			return;
		}
		else if( p_unIndex == ( unSize - 1 ) )
		{
			m_pRight--;
			return;
		}
		else
		{
			TElement* pCut = m_pLeft + p_unIndex;
			if( p_unIndex < ( unSize >> 1 ) )
			{
				//Right Biased
				CopyBck( pCut, pCut - 1, p_unIndex );
				m_pLeft++;
				return;
			}
			else
			{
				//Left Biased
				CopyFwd( pCut, pCut + 1, unSize - p_unIndex );
				m_pRight--;
				return;
			}
		}
	}


	inline void Remove( const TElement& p_element )
	{
		TElement* pLeft  = m_pLeft;
		TElement* pRight = m_pRight - 1;

		while( pLeft <= pRight )
		{
			if( *pLeft == p_element )
				RemoveAt( pLeft - m_pLeft );

			if( *pRight == p_element )
				RemoveAt( m_pRight - pRight );

			pLeft++;
			pRight--;
		}

	}

	inline void RemoveRange( size_t p_unIndex, size_t p_unLength )
	{ 
		size_t unTmp = p_unIndex + p_unLength;

		if( ( m_pLeft + unTmp ) > m_pRight )
			throw new MesonException( "Buffer index out of range.", __FILE__, __LINE__ );

		if( p_unIndex > ( Size() - unTmp ) )
		{
			//Right Biased
			CopyFwd( m_pLeft + p_unIndex, m_pLeft + unTmp, Size() - unTmp );
			m_pRight -= p_unLength;
		}
		else
		{
			//Left Biased
			CopyBck( m_pLeft + unTmp - 1, m_pLeft + p_unIndex - 1, p_unIndex );
			m_pLeft += p_unLength;
		}
	}

	inline void SubBuffer( size_t p_unIndex, size_t p_unLength )
	{
		m_pLeft += p_unIndex;
		m_pRight = m_pLeft + p_unLength;
	}


	inline void Apply( Visitor p_visitor )
	{
		TElement* pLeft  = m_pLeft;
		TElement* pRight = m_pRight - 1;

		while( pLeft < pRight )
		{
			p_visitor( *pLeft );
			p_visitor( *pRight );

			pLeft++;
			pRight--;
		}

		if( pLeft == pRight )
			p_visitor( *pLeft );
	}


	inline void ApplyInOrder( Visitor p_visitor )
	{
		TElement* pLeft  = m_pLeft;

		while( pLeft < m_pRight )
		{
			p_visitor( *pLeft );
			pLeft++;
		}
	}

	inline TBufferDual<TElement>& operator=( const TBufferDual<TElement>& p_buffer )
	{
		Clear();
		Append( p_buffer );
		return *this;
	}


#pragma endregion

};





Meson_Common_Collections_END
