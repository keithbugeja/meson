//----------------------------------------------------------------------------------------------
//	Meson::Common::String8
//	String class that performs string operations in a convenient manner.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	01/07/2007	1.0.0		First version
//	Gordon Mangion	10/10/2007	2.0.0
//----------------------------------------------------------------------------------------------

#pragma once

#include "Namespaces.h"
#include "Types.h"

Meson_Common_Text_BEGIN

//namespace BufferType
//{
//	enum BufferType
//	{ 
//		NONE = 0,
//		STATIC,
//		DYNAMIC
//	};
//};


class StringBuffer
{
private:
	//BufferType::BufferType m_eType;
	char *m_pcBufferStart,
		 *m_pcDataStart;
	size_t	m_unRefCount,
			m_unBufferSize,
			m_unDataSize;


public:

#pragma region Construction

	StringBuffer(void);
	StringBuffer( const StringBuffer &p_stringBuffer );
	explicit StringBuffer( int p_nSize );
	virtual ~StringBuffer(void);

	void Alloc( size_t p_unSize );
	void Realloc( size_t p_unSize );
	void Realloc( size_t p_unOffset, size_t p_unSize );
	void Copy( char *p_pcDst, const char *p_pcSrc, size_t p_unSize, bool p_bFwd );

	char *GetBufferStart( void );
	char *GetData( void );
	size_t GetBufferSize( void ) const;

	void SetDataStart( char *p_pcData );
	void SetDataSize( size_t p_unSize );
	

#pragma endregion

#pragma region Reference Counting

	void IncRef( void );
	void DecRef( void );
	size_t GetRef( void );

#pragma endregion


#pragma region Typing Methods

	size_t GetDataSize( void ) const;
	//BufferType::BufferType GetType( void ) const;
	//void SetType( BufferType::BufferType p_eType );

#pragma endregion


	size_t Size( void );

	//void SetStatic( const char *p_psz );

	StringBuffer *Clone( void );

	void Clear( void );

	void Append( char p_cData );
	void Append( const char *p_szData );
	void Append( const char *p_szData, size_t p_unSize );

	void Insert( char p_cData, int p_nPos );
	void Insert( const char *p_szData, int p_nPos );

	void Delete( size_t p_unPos );
	void Delete( size_t p_unStart, size_t p_unLength );
	void DeleteToEnd( size_t p_unStart );
	char Pop( size_t p_unPos );

	void Trim( bool p_bLeft, bool p_bRight );

	const char *ToSZ( void );

#pragma region Operators

public:
	char &operator[]( int p_nPos );
	char &operator()( int p_nPos );

#pragma endregion




#pragma region Private Operations

private:

#pragma endregion




};

Meson_Common_Text_END
