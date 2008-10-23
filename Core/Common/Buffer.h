#pragma once



//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "MesonException.h"



Meson_Common_Collections_BEGIN

#pragma region TBuffer

template< class TElement > class TBuffer
{
protected:
	TElement*	m_pBuffer;
	size_t		m_unBufferSize,
				m_unSize;

public:
	bool IsEmpty( void )
	{
		return ( ( !m_pBuffer ) || ( m_unSize==0 ) );
	}

	size_t Size( void ) const
	{
		return m_unSize;
	}

	size_t Capacity( void )
	{
		return m_unBufferSize;
	}

	TElement* GetBuffer( void )
	{
		return m_pBuffer;
	}

	virtual void Clear( void ) = 0;
	virtual TElement& ElementAt( size_t p_unPos ) = 0;
	virtual void SetElement( TElement& p_element, size_t p_unPos ) = 0;
	virtual void Append( const TElement& p_element ) = 0;
	virtual void Insert( const TElement& p_element, size_t p_unPos ) = 0;
	virtual void Insert( TBuffer<TElement>& p_buffer, size_t p_unStartPos, size_t p_unLength, size_t p_unPos ) = 0;
	virtual void Remove( const TElement& p_element ) = 0;
	virtual void RemoveAt( size_t p_unPos ) = 0;
	virtual void RemoveRange( size_t p_unStartPos, size_t p_unLength ) = 0;

protected:
	void CopyBck( TElement* p_pDst, TElement* p_pSrc, size_t p_unItems )
	{
		for( ; p_unItems; p_unItems-- )
		{
			*p_pDst = *p_pSrc;
			p_pDst--;
			p_pSrc--;
		}
	}

	void CopyFwd( TElement* p_pDst, TElement* p_pSrc, size_t p_unItems )
	{
		for( ; p_unItems; p_unItems-- )
		{
			*p_pDst = *p_pSrc;
			p_pDst++;
			p_pSrc++;
		}
	}
};

#pragma endregion


#pragma region TBufferFixed

template< class TElement > class TBufferFixed : public TBuffer< TElement >
{

public:
	TBufferFixed( void ) 
	{
		Init( 8 );
	}

	TBufferFixed( const TBufferFixed& p_fixed ) 
	{
		Init( p_fixed.Size() );
		m_unSize = p_fixed.Size();
		CopyFwd( m_pBuffer, p_fixed.GetBuffer(), m_unSize );
	}

	explicit TBufferFixed( int p_nSize ) 
	{
		Init( p_nSize );
	}

	~TBufferFixed( void ) 
	{
		if( m_pBuffer )
			delete[] m_pBuffer;
	}

public:
	void Clear( void )
	{
		m_unSize = 0;
	}

	TElement& ElementAt( size_t p_unPos )
	{
		if( p_unPos >= m_unSize )
			throw new MesonException("Buffer access out of range!");

		return m_pBuffer[ p_unPos ];
	}

	void SetElement( TElement& p_element, size_t p_unPos ) 
	{
		if( p_unPos >= m_unSize )
			throw new MesonException("Buffer access out of range!");

		m_pBuffer[ p_unPos ] = p_element;
	}


	void Append( const TElement& p_element )
	{
		if( m_unSize < m_unBufferSize )
		{
			m_pBuffer[ m_unSize ] = p_element;
			m_unSize++;
		}
		else
			throw new MesonException("Buffer full!");
	}

	void Insert( const TElement& p_element, size_t p_unPos )
	{
		if( p_unPos >= m_unBufferSize )
			throw new MesonException("Buffer access out of range!");

		if( m_unSize+1 > m_unBufferSize )
			throw new MesonException("Buffer full!");

		CopyBck( m_pBuffer + m_unBufferSize - 1, m_pBuffer + m_unBufferSize - 2, ( m_unSize - p_unPos ) );
		m_pBuffer[ p_unPos ] = p_element;
		m_unSize++;
	}

	void Insert( TBuffer<TElement>& p_buffer, size_t p_unStartPos, size_t p_unLength, size_t p_unPos )
	{
		if( ( m_unSize + p_unLength ) > m_unBufferSize )
			throw new MesonException("Buffer full!");

		CopyBck( m_pBuffer + m_unSize + p_unLength - 1, m_pBuffer + m_unSize - 1, p_unLength );
		CopyFwd( m_pBuffer + p_unPos,  p_buffer.GetBuffer() + p_unStartPos, p_unLength );
		m_unSize += p_unLength;
	}

	void RemoveAt( size_t p_unPos )
	{
		if( p_unPos >= m_unBufferSize )
			throw new MesonException("Buffer access out of range!");

		CopyFwd( m_pBuffer + p_unPos, m_pBuffer + p_unPos + 1, ( m_unSize - p_unPos ) );
		m_unSize--;
	}

	void Remove( const TElement& p_element )
	{
		for( size_t unIndex = 0; unIndex < m_unSize; )
		{
			if( m_pBuffer[ unIndex ] == p_element )
				RemoveAt( unIndex );
			else
				unIndex++;
		}
	}

	void RemoveRange( size_t p_unStartPos, size_t p_unLength )
	{
		
		CopyFwd( m_pBuffer + p_unStartPos, m_pBuffer + p_unStartPos + p_unLength, ( m_unSize - p_unStartPos - p_unLength ) );
		m_unSize -= p_unLength;
	}

protected:
	void Init( int p_nSize )
	{
		if( p_nSize < 1 )
			throw new MesonException("Invalid size specified.");

		m_unBufferSize = p_nSize;
		m_unSize = 0;
		m_pBuffer = new TElement[ p_nSize ];
	}


};

#pragma endregion


#pragma region TBufferDynamic

template< class TElement > class TBufferDynamic : public TBuffer< TElement >
{

public:
	TBufferDynamic( void ) 
	{
		Init( 8 );
	}

	TBufferDynamic( const TBufferDynamic& p_buffer ) 
	{
		Init( p_buffer.Size() );
		m_unSize = p_buffer.Size();
		CopyFwd( m_pBuffer, p_buffer.GetBuffer(), m_unSize );
	}

	explicit TBufferDynamic( int p_nSize ) 
	{
		Init( p_nSize );
	}

	~TBufferDynamic( void ) 
	{
		if( m_pBuffer )
			delete[] m_pBuffer;
	}

public:
	void Clear( void )
	{
		m_unSize = 0;
	}

	TElement& ElementAt( size_t p_unPos )
	{
		if( p_unPos >= m_unSize )
			throw new MesonException("Buffer access out of range!");

		return m_pBuffer[ p_unPos ];
	}

	void SetElement( TElement& p_element, size_t p_unPos ) 
	{
		if( p_unPos >= m_unSize )
			throw new MesonException("Buffer access out of range!");

		m_pBuffer[ p_unPos ] = p_element;
	}


	void Append( const TElement& p_element )
	{
		if( m_unSize >= m_unBufferSize )
			Resize( m_unSize << 1 );

		m_pBuffer[ m_unSize ] = p_element;
		m_unSize++;
	}

	void Insert( const TElement& p_element, size_t p_unPos )
	{
		if( p_unPos > m_unSize )
			throw new MesonException("Buffer access out of range!");

		if( m_unSize+1 >= m_unBufferSize )
			Resize( m_unSize << 1 );

		if( p_unPos != m_unSize ) 
			CopyBck( m_pBuffer + m_unSize, m_pBuffer + m_unSize - 1, ( m_unSize - p_unPos ) );

		m_pBuffer[ p_unPos ] = p_element;
		m_unSize++;
	}

	void Insert( TBuffer<TElement>& p_buffer, size_t p_unStartPos, size_t p_unLength, size_t p_unPos )
	{
		if( p_unLength == 0 )
			return;

		if( ( m_unSize + p_unLength ) > m_unBufferSize )
			Resize( ( m_unSize + p_unLength ) << 1 );

		CopyBck( m_pBuffer + m_unSize + p_unLength - 1, m_pBuffer + m_unSize - 1, ( m_unSize - p_unPos ) );

		CopyFwd( m_pBuffer + p_unPos,  p_buffer.GetBuffer() + p_unStartPos, p_unLength );
		m_unSize += p_unLength;
	}

	void RemoveAt( size_t p_unPos )
	{
		if( p_unPos >= m_unBufferSize )
			throw new MesonException("Buffer access out of range!");

		CopyFwd( m_pBuffer + p_unPos, m_pBuffer + p_unPos + 1, ( m_unSize - p_unPos ) );
		m_unSize--;
	}

	void Remove( const TElement& p_element )
	{
		for( size_t unIndex = 0; unIndex < m_unSize; )
		{
			if( m_pBuffer[ unIndex ] == p_element )
				RemoveAt( unIndex );
			else
				unIndex++;
		}
	}

	void RemoveRange( size_t p_unStartPos, size_t p_unLength )
	{
		if( ( p_unStartPos + p_unLength ) >= m_unBufferSize )
			throw new MesonException("Buffer access out of range!");

		CopyFwd( m_pBuffer + p_unStartPos, m_pBuffer + p_unStartPos + p_unLength, ( m_unSize - p_unStartPos - p_unLength ) );
		m_unSize -= p_unLength;
	}

protected:
	void Init( int p_nSize )
	{
		if( p_nSize < 1 )
			throw new MesonException("Invalid size specified.");

		m_unBufferSize = p_nSize;
		m_unSize = 0;
		m_pBuffer = new TElement[ p_nSize ];
	}

	void Resize( size_t p_unSize )
	{
		if( p_unSize > m_unBufferSize )
		{
			TElement* pNewBuffer = new TElement[ p_unSize ];
			CopyFwd( pNewBuffer, m_pBuffer, m_unSize );
			m_unBufferSize = p_unSize;
			delete m_pBuffer;
			m_pBuffer = pNewBuffer;
		}
		else
			throw new MesonException("Invalid size specified.");
	}

	void Resize( size_t p_unSize, size_t p_unOffset )
	{
		if( p_unSize + p_unOffset > m_unBufferSize )
		{
			TElement* pNewBuffer = new TElement[ p_unSize ];
			CopyFwd( pNewBuffer + p_unOffset, m_pBuffer, m_unSize  );
			m_unBufferSize = p_unSize;
			delete m_pBuffer;
			m_pBuffer = pNewBuffer;
		}
		else
			throw new MesonException("Invalid size specified.");
	}

};

#pragma endregion 

Meson_Common_Collections_END
