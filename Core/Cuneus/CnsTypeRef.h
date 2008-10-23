#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN

class TypeRef : public DataType
{
private:
	void* m_pValue;

public:
	TypeRef( void );
	TypeRef( void* p_pValue );

	void* GetValue( void );
	void SetValue( void* p_pValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeRef& operator=( void* p_pValue );
	operator void*( void );

	Meson::Common::Text::String ToString( void );
};




template< class TType >
class TTypeRef : public DataType
{
private:
	TType* m_pData;
public:
	TTypeRef( void )
		: DataType( "Ref", 0 )
		, m_pData( NULL )
	{
	}

	TTypeRef( void* p_pData )
		: DataType( "Ref", 0 )
		, m_pData( p_pData )
	{
	}

	TType* GetValue( void )
	{
		return m_pData;
	}

	void SetValue( TType* p_pValue )
	{
		m_pData = p_pValue;
	}

	void GetData( byte* p_pbyData )
	{
		*((TType**)p_pbyData) = m_pData;
	}

	void SetData( byte* p_pbyData )
	{
		m_pData = *((TType**)p_pbyData);
	}

	TTypeRef& operator=( TType* p_pValue )
	{
		m_pData = p_pValue;
		return *this;
	}

	operator TType*( void )
	{
		return m_pData;
	}

	TType& operator*( void )
	{
		return *m_pData;
	}
	
	Meson::Common::Text::String ToString( void )
	{
		String str;
		str += ( (long long)m_pData );
		return str;
	}



	DataType OpNeg( DataType& p_dataType )
	{
	}

	DataType OpAdd( DataType& p_dataType )
	{
		return (*m_pData).OpAdd( p_dataType );
	}

	DataType OpSub( DataType& p_dataType )
	{
	}

	DataType OpMul( DataType& p_dataType )
	{
	}

	DataType OpDiv( DataType& p_dataType )
	{
	}

	DataType OpMod( DataType& p_dataType )
	{
	}

	DataType OpInc( DataType& p_dataType )
	{
	}

	DataType OpDec( DataType& p_dataType )
	{
	}

	DataType OpNot( DataType& p_dataType )
	{
	}

	DataType OpAnd( DataType& p_dataType )
	{
	}

	DataType OpOr( DataType& p_dataType )
	{
	}

	DataType OpXor( DataType& p_dataType )
	{
	}

	DataType OpShl( DataType& p_dataType )
	{
	}

	DataType OpShr( DataType& p_dataType )
	{
	}

	DataType OpSar( DataType& p_dataType )
	{
	}

	DataType OpRol( DataType& p_dataType )
	{
	}

	DataType OpRor( DataType& p_dataType )
	{
	}

	DataType OpBitSet( DataType& p_dataType )
	{
	}

	DataType OpBitRes( DataType& p_dataType )
	{
	}

	DataType OpBitTgl( DataType& p_dataType )
	{
	}

	DataType OpBitTst( DataType& p_dataType )
	{
	}


};

typedef TTypeRef< bool >      TypeRefBool;
typedef TTypeRef< byte >      TypeRefByte;
typedef TTypeRef< char >      TypeRefChar;
typedef TTypeRef< word >      TypeRefWord;
typedef TTypeRef< short >     TypeRefShort;
typedef TTypeRef< dword >     TypeRefDWord;
typedef TTypeRef< int >       TypeRefInt;
typedef TTypeRef< qword >     TypeRefQWord;
typedef TTypeRef< long long > TypeRefLong;
typedef TTypeRef< float >     TypeRefFloat;
typedef TTypeRef< double >    TypeRefDouble;

Meson_Cuneus_RTS_Typing_END
