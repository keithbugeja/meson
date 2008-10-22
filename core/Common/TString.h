//----------------------------------------------------------------------------------------------
//	Meson::Common::Text::TString
//	String template that performs string operations in a convenient manner. Still to be revised
//  Do NOT Use
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	01/07/2007	1.0.0		First version
//	Gordon Mangion	10/10/2007	2.0.0		Re-Developed using reference counting mechanism
//----------------------------------------------------------------------------------------------


//TODO: Revise

#pragma once

#include "Namespaces.h"
#include "Types.h"
#include "Convertor.h"
#include "Pointer.h"
#include "BufferDual.h"
#include "RegularExpression.h"

#include <string.h>


Meson_Common_Text_BEGIN

template< typename TElement > 
class TString
{

private:
	typedef TString< TElement > String;
	typedef Meson::Common::Collections::TBufferDual< TElement > StringBuffer;

protected:
	Meson::Common::TPointer< StringBuffer > m_pStringBuffer;

#pragma region Construction

public:
	TString( void )
	{
		m_pStringBuffer = NULL;
	}

	explicit TString( int p_nSize )
	{
		m_pStringBuffer = new StringBuffer( p_nSize );
	}

	explicit TString( size_t p_unSize )
	{
		m_pStringBuffer = new StringBuffer( p_unSize );
	}

	TString( const TElement* p_pElement, int p_nSize )
	{
		m_pStringBuffer = new StringBuffer( p_pElement, p_unSize );
	}

	TString( const char* p_szElement )
	{
		m_pStringBuffer = new StringBuffer( p_szElement, strlen( p_szElement ) );
	}

	TString( const String& p_string )
	{
		m_pStringBuffer = p_string.m_pStringBuffer;
	}

	~TString( void )
	{
	}

#pragma endregion 



#pragma region Internal Methods

protected:
	inline void BranchBuffer( size_t p_unSize = 12 )
	{
		if( m_pStringBuffer == NULL )
			m_pStringBuffer = new StringBuffer( p_unSize );
		else if( m_pStringBuffer->GetReferenceCount() > 1 )
				m_pStringBuffer = new StringBuffer( *(this->m_pStringBuffer) );
	}

#pragma endregion



#pragma region Public Basic Operations

public:
	inline void Clear( void )
	{
		BranchBuffer();
		m_pStringBuffer->Clear();
	}

	inline size_t Size( void ) const
	{
		return m_pStringBuffer->Size();
	}

	inline size_t Length( void ) const
	{
		return m_pStringBuffer->Size();
	}

	inline TElement GetCharAt( size_t p_unIndex ) const
	{
		return m_pStringBuffer->GetElementAt( p_unIndex );
	}

	inline TElement SetCharAt( size_t p_unIndex, const TElement& p_element )
	{
		m_pStringBuffer->SetElementAt( p_unIndex, p_element );
	}

	inline void EnsureBufferSize( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->EnsureBufferSize( size_t p_unIndex );
	}

	inline void Delete( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveAt( p_unIndex );
	}

	inline void Delete( size_t p_unIndex, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveRange( p_unIndex, p_unLength );
	}

	inline void DeleteToEnd( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveRange( p_unIndex, Size() - 1 );
	}


	TElement Pop( void )
	{
		BranchBuffer();
		TElement element = (*this)[ Size() - 1 ];
		Delete( Size() - 1 );
		return element;
	}

	inline void Append( const TElement& p_Element )
	{
		BranchBuffer();
		m_pStringBuffer->Append( p_Element );
	}

	inline void Append( const TElement* p_pElement, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->Append( p_pElement, p_unLength );
	}

	inline void Append( const String& p_string )
	{
		BranchBuffer();
		m_pStringBuffer->Append( *p_string.m_pStringBuffer );
	}


	void Append( int p_nData )
	{
		BranchBuffer();
		Convertor::ToString( (long long)p_nData, *this, 10 );
	}

	void Append( dword p_unData )
	{
		BranchBuffer();
		Convertor::ToString( (qword)p_unData, *this, 10 );
	}

	void Append( long p_lData )
	{
		BranchBuffer();
		Convertor::ToString( (long long)p_lData, *this, 10 );
	}

	void Append( long long p_lData )
	{
		BranchBuffer();
		Convertor::ToString( p_lData, *this, 10 );
	}

	void Append( ulonglong p_ullData )
	{
		BranchBuffer();
		Convertor::ToString( p_ullData, *this, 10 );
	}

	void Append( float p_fData )
	{
		BranchBuffer();
		Convertor::ToString( p_fData, *this, 10 );
	}

	void Append( double p_dfData )
	{
		BranchBuffer();
		Convertor::ToString( p_dfData, *this, 10 );
	}

	void Append( long double p_ldfData )
	{
		BranchBuffer();
		Convertor::ToString( p_ldfData, *this, 10 );
	}

	void Append( bool p_bData )
	{
		BranchBuffer();
		Convertor::ToString( p_bData, *this );
	}

	void AppendFormat( const char *p_pszFormat, ... )
	{
		char pcTemp[ 1024 ];
		va_list argptr;
		va_start( argptr, p_pszFormat );

		char *args = va_arg( argptr, char* );
		sprintf_s( pcTemp, 1024, p_pszFormat, args );
		Append( pcTemp );
	}

	inline void Insert( size_t p_unIndex, const TElement& p_Element )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, p_Element );
	}

	inline void Insert( size_t p_unIndex, const TElement* p_pElement, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, p_pElement, p_unLength );
	}

	inline void Insert( size_t p_unIndex, const String& p_string )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, *p_string.m_pStringBuffer );
	}

	void Insert( int p_nIndex, long p_lData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_lData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, unsigned long p_ulData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_ulData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, long long p_llData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_llData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, ulonglong p_ullData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_ullData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, float p_fData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_fData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, double p_dfData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_dfData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void Insert( int p_nIndex, bool p_bData )
	{
		BranchBuffer();
		String str( 25 );
		Convertor::ToString( p_bData, str );
		m_pStringBuffer->Insert( str, p_nIndex );
	}

	void InsertFormat( int p_nIndex, const char *p_pszFormat, ... )
	{
		char pcTemp[ 1024 ];
		va_list argptr;
		va_start( argptr, p_pszFormat );

		char *args = va_arg( argptr, char* );
		sprintf_s( pcTemp, 1024, p_pszFormat, args );
		Insert( p_nIndex, pcTemp );
	}


#pragma endregion



#pragma region Public Operations

public:
	inline String SubstringIndex( size_t p_unStart, size_t p_unEnd ) const
	{
		String str( *this );
		str.BranchBuffer();
		str.m_pStringBuffer->SubBuffer( p_unStart, p_unEnd - p_unStart );

		return str;
	}

	inline String Substring( size_t p_unStart, size_t p_unSize ) const
	{
		String str( *this );
		str.BranchBuffer();
		str.m_pStringBuffer->SubBuffer( p_unStart, p_unSize );

		return str;
	}


	inline int Compare( const String& p_str ) const
	{
		size_t unSizeThis = Size();
		size_t unSizeOther = p_str.Size();
		size_t unCommonSize = unSizeThis < unSizeOther ? unSizeThis : unSizeOther;
		int nResult = memcmp( m_pStringBuffer->GetData(), p_str.m_pStringBuffer->GetData(), unCommonSize );
		if (nResult != 0) return nResult;

		if (unSizeThis < unSizeOther) return -1;
		if (unSizeThis > unSizeOther) return 1;
		return 0;
	}


	inline int CompareIgnoreCase( const String& p_str ) const
	{
		String strThis( *this );
		String strOther( p_str );
		
		size_t unSizeThis = Size();
		size_t unSizeOther = p_str.Size();

		strThis.ToUpperCase();
		strOther.ToUpperCase();

		size_t unCommonSize = unSizeThis < unSizeOther ? unSizeThis : unSizeOther;
		int nResult = memcmp( m_pStringBuffer->GetData(), p_str.m_pStringBuffer->GetData(), unCommonSize );
		if (nResult != 0) return nResult;

		if (unSizeThis < unSizeOther) return -1;
		if (unSizeThis > unSizeOther) return 1;
		return 0;
	}



	inline void ToLowerCase( void )
	{
		BranchBuffer();
		TElement* pDataBegin = m_pStringBuffer->GetData();
		TElement* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

		for( ; pDataBegin <= pDataEnd; pDataBegin++, pDataEnd-- )
		{
			if( *pDataBegin > 64 && *pDataBegin < 91 )
				*pDataBegin |= 32;
			if( *pDataEnd > 64 && *pDataEnd < 91 )
				*pDataEnd |= 32;
		}

	}

	inline void ToUpperCase( void )
	{
		BranchBuffer();
		TElement* pDataBegin = m_pStringBuffer->GetData();
		TElement* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

		for( ; pDataBegin <= pDataEnd; pDataBegin++, pDataEnd-- )
		{
			if( *pDataBegin > 96 && *pDataBegin < 123 )
				*pDataBegin ^= 32;
			if( *pDataEnd > 96 && *pDataEnd < 123 )
				*pDataEnd ^= 32;
		}

	}

	inline void ToInvertedCase( void )
	{
		BranchBuffer();
		TElement* pDataBegin = m_pStringBuffer->GetData();
		TElement* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

		for( ; pDataBegin <= pDataEnd; pDataBegin++, pDataEnd-- )
		{
			if( *pDataBegin > 64 && *pDataBegin < 91 )
				*pDataBegin |= 32;
			else if( *pDataBegin > 96 && *pDataBegin < 123 )
				*pDataBegin ^= 32;
			
			if( *pDataEnd > 64 && *pDataEnd < 91 )
				*pDataEnd |= 32;
			else if( *pDataEnd > 96 && *pDataEnd < 123 )
				*pDataEnd ^= 32;
		}

	}

	inline TElement* ToArray( void )
	{
		BranchBuffer();
		return m_pStringBuffer->GetData();
	}


	void Trim( const TElement& p_element = ' ' )
	{
		size_t	unLt = 0,
				unRt = Size() - 1;

		for( bool bFinished = false; ( unLt <= unRt ) && ( !bFinished ); )
		{
			bFinished = true;

			if( (*this)[ unLt ] == p_element )
			{
				unLt++;
				bFinished = false;
			}

			if( (*this)[ unRt ] == p_element )
			{
				unRt--;
				bFinished = false;
			}
		}

		m_pStringBuffer->SubBuffer( unLt, unRt - unLt );
	}

	void TrimLeft( const TElement& p_element = ' ' )
	{
		size_t	unLt = 0,
				unRt = Size() - 1;

		for( bool bFinished = false; ( unLt <= unRt ) && ( !bFinished ); )
		{
			bFinished = true;

			if( p_bLeft && ( (*this)[ unLt ] == p_element ) )
			{
				unLt++;
				bFinished = false;
			}
		}

		m_pStringBuffer->SubBuffer( unLt, unRt - unLt );
	}

	void TrimRight( const TElement& p_element = ' ' )
	{
		size_t	unLt = 0,
				unRt = Size() - 1;

		for( bool bFinished = false; ( unLt <= unRt ) && ( !bFinished ); )
		{
			bFinished = true;

			if( (*this)[ unRt ] == p_element )
			{
				unRt--;
				bFinished = false;
			}
		}

		m_pStringBuffer->SubBuffer( unLt, unRt - unLt );
	}

	inline void PadLeft( int p_unCount, TElement p_cData )
	{
		BranchBuffer();
		while( p_unCount > 0 )
		{
			Insert( 0, p_cData );
			p_unCount--;
		}
	}

	inline void PadRight( int p_unCount, TElement p_cData )
	{
		BranchBuffer();

		while( p_unCount > 0 )
		{
			Append( p_cData );
			p_unCount--;
		}
	}


	inline int IndexOf( TElement p_element, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		TElement* pDataBegin = m_pStringBuffer->GetData() + p_unOffset;
		TElement* pDataEnd = pDataBegin + m_pStringBuffer->Size();

		while( pDataBegin < pDataEnd )
		{
			if( *pDataBegin == p_element )
				return (int)( pDataEnd - m_pStringBuffer->GetData() );

			pDataBegin++;
		}

		return -1;
	}


	inline int IndexOf( const char* p_pszData, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		size_t unSize = strlen( p_pszData );
		char *pPos = m_pStringBuffer->GetData() + p_unOffset;
			
		while( true )
		{
			pPos = (char*)memchr( pPos, *p_pszData, Size() );
			if(!pPos)
				return -1;
			else
			{
				if( strncmp( pPos, p_pszData, unSize ) == 0 )
					return (int)( pPos - m_pStringBuffer->GetData() );
				else
					pPos++;
			}
		}
		return -1;
	}



	inline int IndexOf( const TElement* p_pElement, size_t p_unSize, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		int nIndex = (int)p_unOffset;

		while( nIndex < (int)p_unSize )
		{
			nIndex = IndexOf( *p_pElement, nIndex );
			if(nIndex == -1)
				return -1;
			else
			{
				if( strncmp( m_pStringBuffer->GetData() + nIndex, p_pElement, p_unSize ) == 0 )
					return nIndex;
				else
					nIndex++;
			}
		}
		return -1;
	}



	inline int IndexOf( const String& p_string, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		int nIndex = (int)p_unOffset;

		while( nIndex < (int)Size() )
		{
			nIndex = IndexOf( p_string[0], nIndex );
			if(nIndex == -1)
				return -1;
			else
			{
				if( strncmp( m_pStringBuffer->GetData() + nIndex, p_string.m_pStringBuffer->GetData(), p_string.Size() ) == 0 )
					return nIndex;
				else
					nIndex++;
			}
		}
		return -1;
	}


	inline int LastIndexOf( TElement p_element, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		TElement* pDataBegin = m_pStringBuffer->GetData() + p_unOffset;
		TElement* pDataEnd = pDataBegin + m_pStringBuffer->Size() - 1;

		while( pDataBegin < pDataEnd )
		{
			if( *pDataEnd == p_element )
				return (int)( pDataEnd - m_pStringBuffer->GetData() );

			pDataEnd--;
		}

		return -1;
	}


	inline int IndexOfAny( const char* p_pszData, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		int nResult = -1;
		size_t unLen = strlen( p_pszData );

		for( size_t unPos=0; unPos < unLen; unPos++ )
		{
			nResult = IndexOf( p_pszData[unPos], p_unOffset );
			if( nResult != -1 )
				return nResult;
		}

		return -1;
	}



	inline int IndexOfAny( const TElement* p_pElement, size_t p_unSize, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		int nResult = -1;
	
		for( size_t unPos=0; unPos < p_unSize; unPos++ )
		{
			nResult = IndexOf( p_pElement[unPos], p_unOffset );
			if( nResult != -1 )
				return nResult;
		}

		return -1;
	}



	inline int IndexOfAny( const String& p_string, size_t p_unOffset = 0 ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		int nResult = -1;
		size_t unLen = p_string.Size();

		for( size_t unPos=0; unPos < unLen; unPos++ )
		{
			nResult = IndexOf( p_string[unPos], p_unOffset );
			if( nResult != -1 )
				return nResult;
		}

		return -1;
	}



	inline void ReplaceFirst( TElement p_cOld, TElement p_cNew )
	{
		BranchBuffer();

		int nPos = IndexOf( p_cOld );
		if( nPos != -1 )
			(*this)[ nPos ] = p_cNew;
	}



	inline void Replace( char p_cOld, char p_cNew )
	{
		BranchBuffer();

		char *pcDataBegin = m_pStringBuffer->GetData();
		char *pcDataEnd = m_pStringBuffer->GetData() + Size() - 1;

		for( ; pcDataBegin <= pcDataEnd; pcDataBegin++, pcDataEnd-- )
		{
			if( *pcDataBegin == p_cOld )
				*pcDataBegin = p_cNew;
			if( *pcDataEnd == p_cOld )
				*pcDataEnd = p_cNew;
		}
	}

	inline void Replace( char p_cOld, char p_cNew, size_t p_unOffset, size_t p_unCount )
	{
		BranchBuffer();

		char *pcDataBegin = m_pStringBuffer->GetData() + p_unOffset;
		char *pcDataEnd = m_pStringBuffer->GetData() + Size() - 1;

		for( ; (pcDataBegin < pcDataEnd)  && (p_unCount > 0); pcDataBegin++ )
		{
			if( *pcDataBegin == p_cOld )
			{
				*pcDataBegin = p_cNew;
				p_unCount--;
			}
		}
	}


	inline void ReplaceFirst( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset = 0 )
	{
		size_t unOldSize = strlen( p_pszOld );
		size_t unNewSize = strlen( p_pszNew );

		int nOld = IndexOf( p_pszOld, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, unOldSize );
			Insert( nOld, p_pszNew );
		}

	}

	inline void ReplaceFirst( const TElement* p_pszOld, size_t p_unOldSize, const TElement *p_pszNew, size_t p_unNewSize, size_t p_unOffset = 0 )
	{
		int nOld = IndexOf( p_pszOld, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, p_unOldSize );
			Insert( nOld, p_pszNew, p_unNewSize );
		}

	}

	inline void ReplaceFirst( const String& p_strOld, const String& p_strNew, size_t p_unOffset = 0 )
	{
		int nOld = IndexOf( p_strOld, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, p_strOld.Size() );
			Insert( nOld, p_strNew, p_strNew.Size() );
		}
	}

	inline void Replace( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset = 0 )
	{
		size_t unOldSize = strlen( p_pszOld );
		size_t unNewSize = strlen( p_pszNew );

		int nOld = p_unOffset;
		while( true )
		{
			nOld = IndexOf( p_pszOld, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, unOldSize );
				Insert( nOld, p_pszNew );
				nOld += unNewSize;
			}
		}

	}

	inline void Replace( const TElement* p_pszOld, size_t p_unOldSize, const TElement *p_pszNew, size_t p_unNewSize, size_t p_unOffset = 0 )
	{

		int nOld = p_unOffset;
		while( true )
		{
			nOld = 	IndexOf( p_pszOld, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, p_unOldSize );
				Insert( nOld, p_pszNew, p_unNewSize );
				nOld += p_unNewSize;
			}
		}
	}

	inline void Replace( const String& p_strOld, const String& p_strNew, size_t p_unOffset = 0 )
	{
		int nOld = p_unOffset;
		while( true )
		{
			nOld = IndexOf( p_strOld, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, p_strOld.Size() );
				Insert( nOld, p_strNew, p_strNew.Size() );
				nOld += p_strNew.Size();
			}
		}
	}


	inline void Reverse( void )
	{
		BranchBuffer();

		TElement* pcDataBegin = m_pStringBuffer->GetData();
		TElement* pcDataEnd = m_pStringBuffer->GetData() + Size() - 1;

		TElement cTmp;
		for( ; pcDataBegin <= pcDataEnd; pcDataBegin++, pcDataEnd-- )
		{
			cTmp = *pcDataBegin;
			*pcDataBegin = *pcDataEnd;
			*pcDataEnd = cTmp;
		}
	}

	inline void Swap( size_t p_unOffset1, size_t p_unOffset2 )
	{
		BranchBuffer();

		TElement cTmp = *( m_pStringBuffer->GetData() + p_unOffset1 );
		*( m_pStringBuffer->GetData() + p_unOffset1 ) = *( m_pStringBuffer->GetData() + p_unOffset2 );
		*( m_pStringBuffer->GetData() + p_unOffset2 ) = cTmp;
	}


	inline void Split( TElement p_cData, Meson::Common::Collections::TList<String>& p_list ) const
	{
		int nIndex = -1,
			nLastIndex = 0;
		
		do
		{
			nIndex = IndexOf( p_cData, nLastIndex );

			if( nIndex == -1 )
			{
				p_list += SubstringIndex( nLastIndex, Size()-1 );
			}
			else
			{
				p_list += SubstringIndex( nLastIndex, nIndex - 1 );
			}

			nLastIndex = nIndex + 1;
		}
		while( nIndex != -1 );
	}

	inline bool Matches( const String& p_strRegExp )
	{
		Meson::Common::RegularExpressions::RegularExpression re( p_strRegExp );
		return re.Matches( *this );
	}

	inline bool Matches( const Meson::Common::RegularExpressions::RegularExpression& p_RegExp )
	{
		return p_RegExp.Matches( *this );
	}

	inline int Fetch( const String& p_strRegExp )
	{
		Meson::Common::RegularExpressions::RegularExpression re( p_strRegExp );
		return re.Fetch( *this );
	}

	inline int Fetch( const Meson::Common::RegularExpressions::RegularExpression& p_RegExp )
	{
		return p_RegExp.Fetch( *this );
	}


	String Escape( void )
	{
		int nLen = (int)Size();
		String str( nLen );
		TElement cTmp;

		for( int nPos = 0; nPos < nLen; nPos++ )
		{
			cTmp = (*this)[ nPos ];
			switch( cTmp )
			{
			case 0:
				str.Append( "\\0" );
				continue;

			case '\a':
				str.Append( "\\a" );
				continue;

			case '\b':
				str.Append( "\\b" );
				continue;

			case '\f':
				str.Append( "\\f" );
				continue;

			case '\n':
				str.Append( "\\n" );
				continue;

			case '\r':
				str.Append( "\\r" );
				continue;

			case '\t':
				str.Append( "\\t" );
				continue;

			case '\v':
				str.Append( "\\v" );
				continue;

			case '\'':
				str.Append( "\\\'" );
				continue;

			case '\"':
				str.Append( "\\\"" );
				continue;

			case '\\':
				str.Append( "\\\\" );
				continue;

			default:
				if( cTmp < 32 || cTmp > 126 )
				{
					str.Append("\\x");
					Meson::Common::Convertor::ToHex( cTmp, str );
				}
				else
					str.Append( cTmp );
			}
		}

		return str;
	}



	String Unescape( void )
	{
		int nLen = (int)Size();
		String str( nLen );

		TElement cTmp;

		for( int nPos = 0; nPos < nLen; nPos++ )
		{
			cTmp = (*this)[ nPos ];
			if( cTmp == '\\' )
			{
				nPos++;
				cTmp = (*this)[ nPos ];
				switch( cTmp )
				{
				case 'a': str.Append( '\a' ); continue;
				
				case 'b': str.Append( '\b' ); continue;
				
				case 'f': str.Append( '\f' ); continue;
				
				case 'n': str.Append( '\n' ); continue;
				
				case 'r': str.Append( '\r' ); continue;
				
				case 't': str.Append( '\t' ); continue;
				
				case 'v': str.Append( '\v' ); continue;
				
				case 'x': 
					{
						nPos++;
						cTmp = (*this)[ nPos ] | 32;
						nPos++;
						char cTmp2 = (*this)[ nPos ] | 32;

						Meson::Common::Convertor::FromHex( cTmp, cTmp2, str );
					}
					continue;
				
				case '\'': str.Append( '\'' ); continue;
				
				case '\"': str.Append( '\"' ); continue;
				
				case '\\': str.Append( '\\' ); continue;

				default:
					str.Append( cTmp );
				}
			}
			else
				str.Append( cTmp );
		}

		return str;
	}

#pragma endregion




#pragma region Operators

public:
	String& operator=( const String& p_string  )
	{
		m_pStringBuffer = p_string.m_pStringBuffer;
		return *this;
	}

	String& operator=( char p_cData )
	{
		Clear();
		Append( p_cData );
		return *this;
	}


	String& operator=( const char *p_sz )
	{
		Clear();
		Append( p_sz );
		return *this;
	}

	TElement operator[]( size_t p_unIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	TElement operator[]( int p_nIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	TElement& operator[]( size_t p_unIndex )
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	TElement& operator[]( int p_nIndex )
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	operator const char*()
	{
		BranchBuffer();
		(m_pStringBuffer->GetData())[ Size() ] = '\0';
		return m_pStringBuffer->GetData();
	}

	operator char*()
	{
		BranchBuffer();
		(m_pStringBuffer->GetData())[ Size() ] = '\0';
		return m_pStringBuffer->GetData();
	}

	String& operator+=( char p_cData )
	{
		Append( p_cData );
		return *this;
	}

	String& operator+=( const String& p_string )
	{
		Append( p_string );
		return *this;
	}

	String& operator+=( const int& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& operator+=( const char* p_pszData )
	{
		Append( p_pszData );
		return *this;
	}

	String& operator+=( const dword& p_data)
	{
		Append( p_data );
		return *this;
	}


	String& operator+=( const long long& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& operator+=( const qword& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& operator+=( const float& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& operator+=( const double& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& operator+=( const bool& p_data)
	{
		Append( p_data );
		return *this;
	}

	bool operator< ( const String& p_str ) const
	{
		return ( Compare( p_str ) < 0 );
	}

	bool operator> ( const String& p_str ) const
	{
		return ( Compare( p_str ) > 0 );
	}

	bool operator==( const String& p_str ) const
	{
		return ( Compare( p_str ) == 0 );
	}

	bool operator==( const char* p_pszValue ) const
	{
		return ( Compare( String(p_pszValue) ) == 0 );
	}

	bool operator/=( const String& p_str ) const
	{
		return ( CompareIgnoreCase( p_str ) == 0 );
	}

	bool operator!=( const String& p_str ) const
	{
		return ( Compare( p_str ) != 0 );
	}

	bool operator>=( const String& p_str ) const
	{
		return ( Compare( p_str ) >= 0 );
	}

	bool operator<=( const String& p_str ) const
	{
		return ( Compare( p_str ) <= 0 );
	}

	String operator+( char p_cData )
	{
		String str( *this );
		str.Append( p_cData );
		return str;
	}

	String operator+( const char* p_pszData )
	{
		String str( *this );
		str.Append( p_pszData );
		return str;
	}

	String operator+( const String& p_str )
	{
		String str( *this );
		str.Append( p_str );
		return str;
	}

	String operator+( const int p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const dword p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const long long p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const ulonglong p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const float p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const double p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String operator+( const bool p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

#pragma endregion
};

Meson_Common_Text_END



#pragma region Global Operators

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const bool p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, (bool) p_data );
	return str;
}


template< typename TElement > 
Meson::Common::Text::TString< TElement > operator+( char p_cData, const Meson::Common::Text::TString< TElement >& p_str )
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, p_cData );
	return str;
}


template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const char *p_pszData, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str(p_pszData);
	str.Append(p_str);
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const int p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, (long) p_data );
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const uint p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, (ulong) p_data );
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const long long p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, (long long) p_data );
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const ulonglong p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, (ulonglong) p_data );
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const float p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, p_data );
	return str;
}

template< typename TElement >
Meson::Common::Text::TString< TElement > operator+( const double p_data, const Meson::Common::Text::TString< TElement >& p_str)
{
	Meson::Common::Text::TString< TElement > str( p_str );
	str.Insert( 0, p_data );
	return str;
}

#pragma endregion
