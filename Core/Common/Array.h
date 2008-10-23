#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

Meson_Common_Collections_BEGIN

template< class TElement, int nMaxSize > class TArray
{
private:
	TElement m_pTElement[ nMaxSize ];
	int m_nSize;

public:
	TArray( void )
	{
		Clear();
	}

	~TArray( void )
	{
	}

	void Clear( void )
	{
		memset( m_pTElement, 0, ( sizeof( TElement ) * nMaxSize ) + sizeof(int) );
	}

	int Size( void )
	{
		return m_nSize;
	}

	int MaxSize( void )
	{
		return nMaxSize;
	}

	bool Add( TElement &p_element )
	{
		if( m_nSize > nMaxSize )
			return false;

		m_pTElement[ m_bySize ] = p_element;
		m_bySize++;

		return true;
	}

	TElement &operator[]( int p_nIndex )
	{
		if( p_nIndex<0 || p_nIndex>nMaxSize )
			throw new MesonException( "Array index out of range.", __FILE__, __LINE__ );
		return m_pTElement[ p_nIndex ];
	}

	TElement &operator()( int p_nIndex )
	{
		return m_pTElement[ p_nIndex ];
	}

};

Meson_Common_Collections_END

