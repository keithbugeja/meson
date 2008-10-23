//----------------------------------------------------------------------------------------------
//	Meson::Common::String8
//	String class that performs string operations in a convenient manner.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	01/07/2007	1.0.0		First version
//	Gordon Mangion	10/10/2007	2.0.0		Re-Developed using reference counting mechanism
//----------------------------------------------------------------------------------------------

#pragma once

#include "Namespaces.h"
#include "Types.h"
#include "Pointer.h"
//#include "BufferDual.h"


//----------------------------------------------------------------------------------------------

Meson_Common_BEGIN

	//template< typename TElement > class TPointer;

Meson_Common_END

Meson_Common_Collections_BEGIN

	template< typename TElement > class TList;
	template< typename TElement > class TBufferDual;

Meson_Common_Collections_END

Meson_Common_RegularExpressions_BEGIN

	class RegularExpression;

Meson_Common_RegularExpressions_END

//----------------------------------------------------------------------------------------------


Meson_Common_Text_BEGIN

class String8
	: public Meson::Common::TReferenceCounter<String8>
{

private:
	typedef Meson::Common::Collections::TBufferDual< char > StringBuffer;

protected:
	Meson::Common::TPointer< StringBuffer > m_pStringBuffer;

#pragma region Construction

public:
	String8( void );
	explicit String8( int p_nSize );
	explicit String8( size_t p_unSize );
	String8( char p_cChar );
	String8( const char* p_pElement, int p_nSize );
	String8( const char* p_szElement );
	String8( const String8& p_string );
	~String8( void );

#pragma endregion 



#pragma region Internal Methods

protected:
	inline void BranchBuffer( size_t p_unSize = 12 );

#pragma endregion



#pragma region Public Basic Operations

public:
	void SetGrowthFactors( float p_fLtFactor, float p_fRtFactor );
	void Clear( void );
	size_t Size( void ) const;
	size_t Length( void ) const;
	bool IsEmpty( void ) const;
	char GetCharAt( size_t p_unIndex ) const;
	void SetCharAt( size_t p_unIndex, char p_element );
	char* GetBuffer( void ) const;
	char* GetBuffer( void );
	void EnsureBufferSize( size_t p_unIndex );
	void Delete( size_t p_unIndex );
	void Delete( size_t p_unIndex, size_t p_unLength );
	void DeleteToEnd( size_t p_unIndex );
	char Pop( size_t p_unIndex );
	void Append( char p_Element );
	void Append( const char* p_pszElement );
	void Append( const char* p_pszElement, size_t p_unLength );
	void Append( const String8& p_string );
	void Append( int p_nData );
	void Append( dword p_unData );
	void Append( long p_lData );
	void Append( long long p_lData );
	void Append( ulonglong p_ullData );
	void Append( float p_fData );
	void Append( double p_dfData );
	void Append( long double p_ldfData );
	void Append( bool p_bData );
	void AppendFormat( const char *p_pszFormat, ... );
	void Insert( size_t p_unIndex, char p_Element );
	void Insert( size_t p_unIndex, const char* p_pElement );
	void Insert( size_t p_unIndex, const char* p_pElement, size_t p_unLength );
	void Insert( size_t p_unIndex, const String8& p_string );
	void Insert( size_t p_nIndex, long p_lData );
	void Insert( size_t p_nIndex, unsigned long p_ulData );
	void Insert( size_t p_nIndex, long long p_llData );
	void Insert( size_t p_nIndex, ulonglong p_ullData );
	void Insert( size_t p_nIndex, float p_fData );
	void Insert( size_t p_nIndex, double p_dfData );
	void Insert( size_t p_nIndex, bool p_bData );
	void InsertFormat( size_t p_nIndex, const char *p_pszFormat, ... );

#pragma endregion



#pragma region Public Operations

public:
	String8 SubstringIndex( size_t p_unStart, size_t p_unEnd ) const;
	String8 Substring( size_t p_unStart, size_t p_unSize ) const;
	int Compare( const char* p_szValue ) const;
	int Compare( const String8& p_str ) const;
	int CompareIgnoreCase( const String8& p_str ) const;
	void ToLowerCase( void );
	void ToUpperCase( void );
	void ToInvertedCase( void );
	char* ToArray( void );
	void Trim( char p_element = ' ' );
	void TrimLeft( char p_element = ' ' );
	void TrimRight( char p_element = ' ' );
	void PadLeft( int p_unCount, char p_cData );
	void PadRight( int p_unCount, char p_cData );
	int IndexOf( char p_element, size_t p_unOffset = 0 ) const;
	int IndexOf( const char* p_pszData, size_t p_unOffset = 0 ) const;
	int IndexOf( const char* p_pElement, size_t p_unSize, size_t p_unOffset = 0 ) const;
	int IndexOf( const String8& p_string, size_t p_unOffset = 0 ) const;
	int LastIndexOf( char p_element, size_t p_unOffset = 0 ) const;
	int IndexOfAny( const char* p_pszData, size_t p_unOffset = 0 ) const;
	int IndexOfAny( const char* p_pElement, size_t p_unSize, size_t p_unOffset = 0 ) const;
	int IndexOfAny( const String8& p_string, size_t p_unOffset = 0 ) const;
	void ReplaceFirst( char p_cOld, char p_cNew );
	void Replace( char p_cOld, char p_cNew );
	void Replace( char p_cOld, char p_cNew, size_t p_unOffset, size_t p_unCount );
	void ReplaceFirst( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset = 0 );
	void ReplaceFirst( const char* p_pszOld, size_t p_unOldSize, const char *p_pszNew, size_t p_unNewSize, size_t p_unOffset = 0 );
	void ReplaceFirst( const String8& p_strOld, const String8& p_strNew, size_t p_unOffset = 0 );
	void Replace( const char* p_pszOld, const char* p_pszNew, size_t p_unOffset = 0 );
	void Replace( const char* p_pszOld, size_t p_unOldSize, const char *p_pszNew, size_t p_unNewSize, size_t p_unOffset = 0 );
	void Replace( const String8& p_strOld, const String8& p_strNew, size_t p_unOffset = 0 );
	void Reverse( void );
	void Swap( size_t p_unOffset1, size_t p_unOffset2 );
	void Split( char p_cData, Meson::Common::Collections::TList<String8>& p_list ) const;
	bool Matches( String8& p_strRegExp );
	bool Matches( Meson::Common::RegularExpressions::RegularExpression p_RegExp );
	int Fetch( String8& p_strRegExp );
	int Fetch( Meson::Common::RegularExpressions::RegularExpression p_RegExp );
	String8 Escape( void );
	String8 Unescape( void );

#pragma endregion




#pragma region Operators

public:
	String8& operator=( const String8& p_string  );
	String8& operator=( char p_cData );
	String8& operator=( const char *p_sz );
	char operator[]( size_t p_unIndex ) const;
	char operator[]( int p_nIndex ) const;
	char& operator[]( size_t p_unIndex );
	char& operator[]( int p_nIndex );

	char& operator()( size_t p_unIndex ) const;
	char& operator()( int p_nIndex ) const;
	char& operator()( size_t p_unIndex );
	char& operator()( int p_nIndex );

	operator const char*() const;
	operator const char*();

	String8& operator+=( char p_cData );
	String8& operator+=( const String8& p_string );
	String8& operator+=( const int& p_data);
	String8& operator+=( const char* p_pszData );
	String8& operator+=( const dword& p_data);
	String8& operator+=( const long long& p_data);
	String8& operator+=( const qword& p_data);
	String8& operator+=( const float& p_data);
	String8& operator+=( const double& p_data);
	String8& operator+=( const bool& p_data);
	bool operator< ( const String8& p_str ) const;
	bool operator> ( const String8& p_str ) const;
	bool operator==( const String8& p_str ) const;
	bool operator==( const char* p_pszValue ) const;
	bool operator/=( const String8& p_str ) const;
	bool operator!=( const String8& p_str ) const;
	bool operator!=( const char* p_pszValue ) const;
	bool operator>=( const String8& p_str ) const;
	bool operator<=( const String8& p_str ) const;
	String8 operator+( char p_cData );
	String8 operator+( const char* p_pszData );
	String8 operator+( const String8& p_str );
	String8 operator+( const int p_data );
	String8 operator+( const dword p_data );
	String8 operator+( const long long p_data );
	String8 operator+( const ulonglong p_data );
	String8 operator+( const float p_data );
	String8 operator+( const double p_data );
	String8 operator+( const bool p_data );

#pragma endregion
};

typedef Meson::Common::Text::String8 String;
typedef Meson::Common::TPointer<String8> String8Ptr;
typedef Meson::Common::TPointer<String> StringPtr;

Meson_Common_Text_END



#pragma region Global Operators


Meson::Common::Text::String8 operator+( const bool p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( char p_cData, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const char* p_pszData, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const int p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const uint p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const long long p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const ulonglong p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const float p_data, const Meson::Common::Text::String8& p_str );
Meson::Common::Text::String8 operator+( const double p_data, const Meson::Common::Text::String8& p_str );


#pragma endregion
