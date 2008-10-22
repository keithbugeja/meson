#include "String8.h"

#include "Types.h"
#include "Converter.h"
#include "Pointer.h"
#include "BufferDual.h"
#include "RegularExpression.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

Meson_Common_Text_BEGIN

#pragma region Construction

	String8::String8( void )
		: m_pStringBuffer(NULL)
	{
	}

	String8::String8( int p_nSize )
		: m_pStringBuffer( new StringBuffer( p_nSize ) )
	{
	}

	String8::String8( size_t p_unSize )
		: m_pStringBuffer( new StringBuffer( p_unSize ) )
	{
	}

	String8::String8( char p_cChar )
		: m_pStringBuffer( new StringBuffer() )
	{
		m_pStringBuffer->Append( p_cChar );
	}

	String8::String8( const char* p_pszElement, int p_nSize )
		: m_pStringBuffer()
	{
		if (p_nSize > 0)
			m_pStringBuffer = new StringBuffer( p_pszElement, p_nSize );
	}

	String8::String8( const char* p_pszElement )
		: m_pStringBuffer()
	{
		size_t unSize = strlen(p_pszElement);
		if (p_pszElement != NULL && unSize > 0)
			m_pStringBuffer = new StringBuffer(p_pszElement, unSize);
	}

	String8::String8( const String8& p_string )
		: m_pStringBuffer( p_string.m_pStringBuffer )
	{
	}

	String8::~String8( void )
	{
		m_pStringBuffer = NULL;
	}

#pragma endregion 



#pragma region Internal Methods

	inline void String8::BranchBuffer( size_t p_unSize )
	{
		if( m_pStringBuffer == NULL )
			m_pStringBuffer = new StringBuffer( p_unSize );
		else if( m_pStringBuffer->GetReferenceCount() > 1 )
				m_pStringBuffer = new StringBuffer( *(this->m_pStringBuffer) );
	}

#pragma endregion



#pragma region Public Basic Operations

	void String8::SetGrowthFactors( float p_fLtFactor, float p_fRtFactor )
	{
		if( m_pStringBuffer == NULL )
			m_pStringBuffer = new StringBuffer( 12 );

		m_pStringBuffer->SetGrowthFactors( p_fLtFactor, p_fRtFactor );
	}

	void String8::Clear( void )
	{
		BranchBuffer();
		m_pStringBuffer->Clear();
	}

	size_t String8::Size( void ) const
	{
		if( m_pStringBuffer == NULL)
			return 0;

		return m_pStringBuffer->Size();
	}

	size_t String8::Length( void ) const
	{
		if( m_pStringBuffer == NULL)
			return 0;

		return m_pStringBuffer->Size();
	}

	bool String8::IsEmpty( void ) const
	{
		if( m_pStringBuffer == NULL)
			return true;
		else
			return (m_pStringBuffer->Size() == 0);
	}

	char String8::GetCharAt( size_t p_unIndex ) const
	{
		return m_pStringBuffer->GetElementAt( p_unIndex );
	}

	void String8::SetCharAt( size_t p_unIndex, char p_element )
	{
		m_pStringBuffer->SetElementAt( p_unIndex, p_element );
	}

	char* String8::GetBuffer( void ) const
	{
		return m_pStringBuffer->GetData();
	}

	char* String8::GetBuffer( void )
	{
		return m_pStringBuffer->GetData();
	}

	void String8::EnsureBufferSize( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->EnsureBufferSize( p_unIndex );
	}

	void String8::Delete( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveAt( p_unIndex );
	}

	void String8::Delete( size_t p_unIndex, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveRange( p_unIndex, p_unLength );
	}

	void String8::DeleteToEnd( size_t p_unIndex )
	{
		BranchBuffer();
		m_pStringBuffer->RemoveRange( p_unIndex, Size() - 1 );
	}


	char String8::Pop( size_t p_unIndex )
	{
		BranchBuffer();
		char element = (*this)[ p_unIndex ];
		Delete( p_unIndex );
		return element;
	}

	void String8::Append( char p_Element )
	{
		BranchBuffer();
		m_pStringBuffer->Append( p_Element );
	}

	void String8::Append( const char* p_pElement )
	{
		BranchBuffer();
		m_pStringBuffer->Append( p_pElement, strlen( p_pElement ) );
	}

	void String8::Append( const char* p_pElement, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->Append( p_pElement, p_unLength );
	}

	void String8::Append( const String& p_string )
	{
		if( p_string.IsEmpty() )
			return;

		BranchBuffer();
		m_pStringBuffer->Append( *p_string.m_pStringBuffer );
	}


	void String8::Append( int p_nData )
	{
		BranchBuffer();
		Converter::ToString( (long long)p_nData, *this, 10 );
	}

	void String8::Append( dword p_unData )
	{
		BranchBuffer();
		Converter::ToString( (qword)p_unData, *this, 10 );
	}

	void String8::Append( long p_lData )
	{
		BranchBuffer();
		Converter::ToString( (long long)p_lData, *this, 10 );
	}

	void String8::Append( long long p_lData )
	{
		BranchBuffer();
		Converter::ToString( p_lData, *this, 10 );
	}

	void String8::Append( ulonglong p_ullData )
	{
		BranchBuffer();
		Converter::ToString( p_ullData, *this, 10 );
	}

	void String8::Append( float p_fData )
	{
		BranchBuffer();
		Converter::ToString( p_fData, *this, 10 );
	}

	void String8::Append( double p_dfData )
	{
		BranchBuffer();
		Converter::ToString( p_dfData, *this, 10 );
	}

	void String8::Append( long double p_ldfData )
	{
		BranchBuffer();
		Converter::ToString( p_ldfData, *this, 10 );
	}

	void String8::Append( bool p_bData )
	{
		BranchBuffer();
		Converter::ToString( p_bData, *this );
	}

	void String8::AppendFormat( const char *p_pszFormat, ... )
	{
		char pcTemp[ 1024 ];
		va_list argptr;
		va_start( argptr, p_pszFormat );

		char *args = va_arg( argptr, char* );
		sprintf_s( pcTemp, 1024, p_pszFormat, args );
		Append( pcTemp, strlen( pcTemp ) );
	}

	void String8::Insert( size_t p_unIndex, char p_Element )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, p_Element );
	}

	void String8::Insert( size_t p_unIndex, const char* p_pElement )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, p_pElement, strlen( p_pElement ) );
	}

	void String8::Insert( size_t p_unIndex, const char* p_pElement, size_t p_unLength )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, p_pElement, p_unLength );
	}

	void String8::Insert( size_t p_unIndex, const String& p_string )
	{
		BranchBuffer();
		m_pStringBuffer->Insert( p_unIndex, *p_string.m_pStringBuffer );
	}

	void String8::Insert( size_t p_nIndex, long p_lData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_lData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, unsigned long p_ulData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_ulData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, long long p_llData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_llData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, ulonglong p_ullData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_ullData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, float p_fData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_fData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, double p_dfData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_dfData, str );
		Insert( p_nIndex, str );
	}

	void String8::Insert( size_t p_nIndex, bool p_bData )
	{
		BranchBuffer();
		String str( 25 );
		Converter::ToString( p_bData, str );
		Insert( p_nIndex, str );
	}

	void String8::InsertFormat( size_t p_nIndex, const char *p_pszFormat, ... )
	{
		char pcTemp[ 1024 ];
		va_list argptr;
		va_start( argptr, p_pszFormat );

		char *args = va_arg( argptr, char* );
		sprintf_s( pcTemp, 1024, p_pszFormat, args );
		Insert( p_nIndex, pcTemp, strlen( pcTemp ) );
	}


#pragma endregion



#pragma region Public Operations

	String String8::SubstringIndex( size_t p_unStart, size_t p_unEnd ) const
	{
		String str( *this );
		str.BranchBuffer();
		str.m_pStringBuffer->SubBuffer( p_unStart, p_unEnd - p_unStart + 1 );

		return str;
	}

	String String8::Substring( size_t p_unStart, size_t p_unSize ) const
	{
		String str( *this );
		str.BranchBuffer();
		str.m_pStringBuffer->SubBuffer( p_unStart, p_unSize );

		return str;
	}

	int String8::Compare( const char* p_szValue ) const
	{
		size_t unSizeThis = Size();
		size_t unSizeOther = strlen(p_szValue);
		size_t unCommonSize = unSizeThis < unSizeOther ? unSizeThis : unSizeOther;

		char* szThisBuffer = m_pStringBuffer == NULL ? "" : m_pStringBuffer->GetData();
		int nResult = memcmp( szThisBuffer, p_szValue, unCommonSize );
		if (nResult != 0) return nResult;

		if (unSizeThis < unSizeOther) return -1;
		if (unSizeThis > unSizeOther) return 1;
		return 0;
	}

	int String8::Compare( const String& p_str ) const
	{
		size_t unSizeThis = Size();
		size_t unSizeOther = p_str.Size();
		size_t unCommonSize = unSizeThis < unSizeOther ? unSizeThis : unSizeOther;

		char* szThisBuffer = m_pStringBuffer == NULL ? "" : m_pStringBuffer->GetData();
		char* szOtherBuffer = p_str.m_pStringBuffer == NULL ? "" : p_str.m_pStringBuffer->GetData();
		int nResult = memcmp( szThisBuffer, szOtherBuffer, unCommonSize );
		if (nResult != 0) return nResult;

		if (unSizeThis < unSizeOther) return -1;
		if (unSizeThis > unSizeOther) return 1;
		return 0;
	}

	int String8::CompareIgnoreCase( const String& p_str ) const
	{
		String strThis( *this );
		String strOther( p_str );
		
		size_t unSizeThis = Size();
		size_t unSizeOther = p_str.Size();

		strThis.ToUpperCase();
		strOther.ToUpperCase();

		size_t unCommonSize = unSizeThis < unSizeOther ? unSizeThis : unSizeOther;
		int nResult = memcmp( strThis.m_pStringBuffer->GetData(), strOther.m_pStringBuffer->GetData(), unCommonSize );
		if (nResult != 0) return nResult;

		if (unSizeThis < unSizeOther) return -1;
		if (unSizeThis > unSizeOther) return 1;
		return 0;
	}



	void String8::ToLowerCase( void )
	{
		BranchBuffer();
		char* pDataBegin = m_pStringBuffer->GetData();
		char* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

		for( ; pDataBegin <= pDataEnd; pDataBegin++, pDataEnd-- )
		{
			if( *pDataBegin > 64 && *pDataBegin < 91 )
				*pDataBegin |= 32;
			if( *pDataEnd > 64 && *pDataEnd < 91 )
				*pDataEnd |= 32;
		}

	}

	void String8::ToUpperCase( void )
	{
		BranchBuffer();
		char* pDataBegin = m_pStringBuffer->GetData();
		char* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

		for( ; pDataBegin <= pDataEnd; pDataBegin++, pDataEnd-- )
		{
			if( *pDataBegin > 96 && *pDataBegin < 123 )
				*pDataBegin ^= 32;
			if( *pDataEnd > 96 && *pDataEnd < 123 )
				*pDataEnd ^= 32;
		}

	}

	void String8::ToInvertedCase( void )
	{
		BranchBuffer();
		char* pDataBegin = m_pStringBuffer->GetData();
		char* pDataEnd = m_pStringBuffer->GetData() + m_pStringBuffer->Size() - 1;

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

	char* String8::ToArray( void )
	{
		BranchBuffer();
		return m_pStringBuffer->GetData();
	}


	void String8::Trim( char p_element )
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

	void String8::TrimLeft( char p_element )
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
		}

		m_pStringBuffer->SubBuffer( unLt, unRt - unLt );
	}

	void String8::TrimRight( char p_element )
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

	void String8::PadLeft( int p_unCount, char p_cData )
	{
		BranchBuffer();
		while( p_unCount > 0 )
		{
			Insert( 0U, p_cData );
			p_unCount--;
		}
	}

	void String8::PadRight( int p_unCount, char p_cData )
	{
		BranchBuffer();

		while( p_unCount > 0 )
		{
			Append( p_cData );
			p_unCount--;
		}
	}


	int String8::IndexOf( char p_element, size_t p_unOffset ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		char* pDataBegin = m_pStringBuffer->GetData() + p_unOffset;
		char* pDataPtr = (char*)memchr( pDataBegin, p_element, m_pStringBuffer->Size() - p_unOffset );

		if( !pDataPtr )
			return -1;
		else
			return (int)( pDataPtr - m_pStringBuffer->GetData() );
	}


	int String8::IndexOf( const char* p_pszData, size_t p_unOffset ) const
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



	int String8::IndexOf( const char* p_pElement, size_t p_unSize, size_t p_unOffset ) const
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



	int String8::IndexOf( const String& p_string, size_t p_unOffset ) const
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


	int String8::LastIndexOf( char p_element, size_t p_unOffset ) const
	{
		if (m_pStringBuffer == NULL)
			return -1;

		char* pDataBegin = m_pStringBuffer->GetData() + p_unOffset;
		char* pDataEnd = pDataBegin + m_pStringBuffer->Size() - 1;

		while( pDataBegin < pDataEnd )
		{
			if( *pDataEnd == p_element )
				return (int)( pDataEnd - m_pStringBuffer->GetData() );

			pDataEnd--;
		}

		return -1;
	}


	int String8::IndexOfAny( const char* p_pszData, size_t p_unOffset ) const
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



	int String8::IndexOfAny( const char* p_pElement, size_t p_unSize, size_t p_unOffset ) const
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



	int String8::IndexOfAny( const String& p_string, size_t p_unOffset ) const
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



	void String8::ReplaceFirst( char p_cOld, char p_cNew )
	{
		BranchBuffer();

		int nPos = IndexOf( p_cOld );
		if( nPos != -1 )
			(*this)[ nPos ] = p_cNew;
	}



	void String8::Replace( char p_cOld, char p_cNew )
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

	void String8::Replace( char p_cOld, char p_cNew, size_t p_unOffset, size_t p_unCount )
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


	void String8::ReplaceFirst( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset )
	{
		size_t unOldSize = strlen( p_pszOld );
		size_t unNewSize = strlen( p_pszNew );

		int nOld = IndexOf( p_pszOld, unOldSize, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, unOldSize );
			Insert( nOld, p_pszNew, unNewSize );
		}

	}

	void String8::ReplaceFirst( const char* p_pszOld, size_t p_unOldSize, const char *p_pszNew, size_t p_unNewSize, size_t p_unOffset )
	{
		int nOld = IndexOf( p_pszOld, p_unOldSize, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, p_unOldSize );
			Insert( nOld, p_pszNew, p_unNewSize );
		}

	}

	void String8::ReplaceFirst( const String& p_strOld, const String& p_strNew, size_t p_unOffset )
	{
		int nOld = IndexOf( p_strOld, p_unOffset );
		if(nOld == -1)
			return;
		else
		{
			Delete( nOld, p_strOld.Size() );
			Insert( nOld, p_strNew );
		}
	}

	void String8::Replace( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset )
	{
		size_t unOldSize = strlen( p_pszOld );
		size_t unNewSize = strlen( p_pszNew );

		int nOld = (int)p_unOffset;
		while( true )
		{
			nOld = IndexOf( p_pszOld, unOldSize, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, unOldSize );
				Insert( nOld, p_pszNew, unNewSize );
				nOld += (int)unNewSize;
			}
		}

	}

	void String8::Replace( const char* p_pszOld, size_t p_unOldSize, const char *p_pszNew, size_t p_unNewSize, size_t p_unOffset )
	{

		int nOld = (int)p_unOffset;
		while( true )
		{
			nOld = 	IndexOf( p_pszOld, p_unOldSize, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, p_unOldSize );
				Insert( nOld, p_pszNew, p_unNewSize );
				nOld += (int)p_unNewSize;
			}
		}
	}

	void String8::Replace( const String& p_strOld, const String& p_strNew, size_t p_unOffset )
	{
		int nOld = (int)p_unOffset;
		while( true )
		{
			nOld = IndexOf( p_strOld, nOld );
			if(nOld == -1)
				return;
			else
			{
				Delete( nOld, p_strOld.Size() );
				Insert( nOld, p_strNew );
				nOld += (int)p_strNew.Size();
			}
		}
	}


	void String8::Reverse( void )
	{
		BranchBuffer();

		char* pcDataBegin = m_pStringBuffer->GetData();
		char* pcDataEnd = m_pStringBuffer->GetData() + Size() - 1;

		char cTmp;
		for( ; pcDataBegin <= pcDataEnd; pcDataBegin++, pcDataEnd-- )
		{
			cTmp = *pcDataBegin;
			*pcDataBegin = *pcDataEnd;
			*pcDataEnd = cTmp;
		}
	}

	void String8::Swap( size_t p_unOffset1, size_t p_unOffset2 )
	{
		BranchBuffer();

		char cTmp = *( m_pStringBuffer->GetData() + p_unOffset1 );
		*( m_pStringBuffer->GetData() + p_unOffset1 ) = *( m_pStringBuffer->GetData() + p_unOffset2 );
		*( m_pStringBuffer->GetData() + p_unOffset2 ) = cTmp;
	}


	void String8::Split( char p_cData, Meson::Common::Collections::TList<String>& p_list ) const
	{
		int nIndex = -1,
			nLastIndex = 0;
		
		do
		{
			nIndex = IndexOf( p_cData, nLastIndex );

			if( nIndex == -1 )
			{
				p_list += SubstringIndex( nLastIndex, Size() - 1 );
			}
			else
			{
				p_list += SubstringIndex( nLastIndex, nIndex - 1 );
			}

			nLastIndex = nIndex + 1;
		}
		while( nIndex != -1 );
	}

	bool String8::Matches( String8& p_strRegExp )
	{
		Meson::Common::RegularExpressions::RegularExpression re( p_strRegExp );
		return re.Matches( *this );
	}

	bool String8::Matches( Meson::Common::RegularExpressions::RegularExpression p_RegExp )
	{
		return p_RegExp.Matches( *this );
	}

	int String8::Fetch( String8& p_strRegExp )
	{
		Meson::Common::RegularExpressions::RegularExpression re( p_strRegExp );
		int n = re.Fetch( *this );
		return n;
	}

	int String8::Fetch( Meson::Common::RegularExpressions::RegularExpression p_RegExp )
	{
		int n = p_RegExp.Fetch( *this );
		return n;
	}


	String String8::Escape( void )
	{
		int nLen = (int)Size();
		String str( nLen );
		char cTmp;

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
					Meson::Common::Converter::ToHex( (byte)cTmp, str );
				}
				else
					str.Append( cTmp );
			}
		}

		return str;
	}



	String String8::Unescape( void )
	{
		int nLen = (int)Size();
		String str( nLen );

		char cTmp;

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

						Meson::Common::Converter::FromHex( cTmp, cTmp2, str );
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


	String& String8::operator=( const String& p_string  )
	{
		m_pStringBuffer = p_string.m_pStringBuffer;
		return *this;
	}

	String& String8::operator=( char p_cData )
	{
		Clear();
		Append( p_cData );
		return *this;
	}


	String& String8::operator=( const char* p_sz )
	{
		Clear();
		Append( p_sz, strlen( p_sz ) );
		return *this;
	}

	char String8::operator[]( size_t p_unIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	char String8::operator[]( int p_nIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	char& String8::operator[]( size_t p_unIndex )
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	char& String8::operator[]( int p_nIndex )
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	char& String8::operator()( size_t p_unIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	char& String8::operator()( int p_nIndex ) const
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	char& String8::operator()( size_t p_unIndex )
	{
		return (m_pStringBuffer->GetData())[ p_unIndex ];
	}

	char& String8::operator()( int p_nIndex )
	{
		return (m_pStringBuffer->GetData())[ p_nIndex ];
	}

	String8::operator const char*() const
	{
		((String8*)this)->BranchBuffer();
		(m_pStringBuffer->GetData())[ Size() ] = '\0';
		return m_pStringBuffer->GetData();
	}

	String8::operator const char*()
	{
		BranchBuffer();
		(m_pStringBuffer->GetData())[ Size() ] = '\0';
		return m_pStringBuffer->GetData();
	}

	String& String8::operator+=( char p_cData )
	{
		Append( p_cData );
		return *this;
	}

	String& String8::operator+=( const String& p_string )
	{
		Append( p_string );
		return *this;
	}

	String& String8::operator+=( const int& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& String8::operator+=( const char* p_pszData )
	{
		Append( p_pszData, strlen( p_pszData ) );
		return *this;
	}

	String& String8::operator+=( const dword& p_data)
	{
		Append( p_data );
		return *this;
	}


	String& String8::operator+=( const long long& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& String8::operator+=( const qword& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& String8::operator+=( const float& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& String8::operator+=( const double& p_data)
	{
		Append( p_data );
		return *this;
	}

	String& String8::operator+=( const bool& p_data)
	{
		Append( p_data );
		return *this;
	}

	bool String8::operator< ( const String& p_str ) const
	{
		return ( Compare( p_str ) < 0 );
	}

	bool String8::operator> ( const String& p_str ) const
	{
		return ( Compare( p_str ) > 0 );
	}

	bool String8::operator==( const String& p_str ) const
	{
		return ( Compare( p_str ) == 0 );
	}

	bool String8::operator==( const char* p_pszValue ) const
	{
		return ( Compare( p_pszValue ) == 0 );
	}

	bool String8::operator/=( const String& p_str ) const
	{
		return ( CompareIgnoreCase( p_str ) == 0 );
	}

	bool String8::operator!=( const String& p_str ) const
	{
		return ( Compare( p_str ) != 0 );
	}

	bool String8::operator!=( const char* p_pszValue ) const
	{
		return ( Compare( String(p_pszValue) ) != 0 );
	}

	bool String8::operator>=( const String& p_str ) const
	{
		return ( Compare( p_str ) >= 0 );
	}

	bool String8::operator<=( const String& p_str ) const
	{
		return ( Compare( p_str ) <= 0 );
	}

	String String8::operator+( char p_cData )
	{
		String str( *this );
		str.Append( p_cData );
		return str;
	}

	String String8::operator+( const char* p_pszData )
	{
		String str( *this );
		str.Append( p_pszData, strlen( p_pszData ) );
		return str;
	}

	String String8::operator+( const String& p_str )
	{
		String str( *this );
		str.Append( p_str );
		return str;
	}

	String String8::operator+( const int p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const dword p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const long long p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const ulonglong p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const float p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const double p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

	String String8::operator+( const bool p_data )
	{
		String str( *this );
		str.Append( p_data );
		return str;
	}

#pragma endregion

Meson_Common_Text_END



#pragma region Global Operators


Meson::Common::Text::String8 operator+( const bool p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, (bool) p_data );
	return str;
}


 
Meson::Common::Text::String8 operator+( char p_cData, const Meson::Common::Text::String8& p_str )
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0U, p_cData );
	return str;
}



Meson::Common::Text::String8 operator+( const char *p_pszData, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str(p_pszData);
	str.Append(p_str);
	return str;
}


Meson::Common::Text::String8 operator+( const int p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, (long) p_data );
	return str;
}


Meson::Common::Text::String8 operator+( const uint p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, (ulong) p_data );
	return str;
}


Meson::Common::Text::String8 operator+( const long long p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, (long long) p_data );
	return str;
}


Meson::Common::Text::String8 operator+( const ulonglong p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, (ulonglong) p_data );
	return str;
}


Meson::Common::Text::String8 operator+( const float p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, p_data );
	return str;
}


Meson::Common::Text::String8 operator+( const double p_data, const Meson::Common::Text::String8& p_str)
{
	Meson::Common::Text::String8 str( p_str );
	str.Insert( 0, p_data );
	return str;
}

#pragma endregion
