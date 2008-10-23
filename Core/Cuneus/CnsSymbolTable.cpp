#include "CnsSymbolTable.h"

#include "ArrayList.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Text;

using namespace Meson::Cuneus::RTS::Typing;


Meson_Cuneus_Compiler_BEGIN


SymbolTableEntry::SymbolTableEntry( Meson::Common::Text::String p_strName ) 
	: m_strName( p_strName )
	//, m_pDataType()
	, m_dwTypeCode( 0 )
	, m_dwOffset( 0 )
	, m_dwScope( 0 )
	, m_pNode( NULL )
	, m_eEntryType( SymbolTableEntryType::None )
{
}

SymbolTableEntry::~SymbolTableEntry( void )
{
}

String& SymbolTableEntry::GetName( void )
{
	return m_strName;
}

//TPointer< DataType > SymbolTableEntry::GetType( void )
//{
//	return m_pDataType;
//}
//
//void SymbolTableEntry::SetType( TPointer< DataType > p_pDataType )
//{
//	m_pDataType = p_pDataType;
//	m_dwSize = p_pDataType->GetSize();
//}

dword SymbolTableEntry::GetTypeCode( void )
{
	return m_dwTypeCode;
}

void SymbolTableEntry::SetTypeCode( dword p_dwTypeCode )
{
	m_dwTypeCode = p_dwTypeCode;
}


dword SymbolTableEntry::GetTargetOffset( void )
{
	return m_dwOffset;
}

void SymbolTableEntry::SetTargetOffset( dword p_dwOffset )
{
	m_dwOffset = p_dwOffset;
}

dword SymbolTableEntry::GetScope( void )
{
	return m_dwScope;
}

void SymbolTableEntry::SetScope( dword p_dwScope )
{
	m_dwScope = p_dwScope;
}


ParserNode* SymbolTableEntry::GetNode( void )
{
	return m_pNode;
}

void SymbolTableEntry::SetNode( ParserNode* p_pNode )
{
	m_pNode = p_pNode;
}

ParserNode* SymbolTableEntry::GetParentNode( void )
{
	if( m_pNode )
		return m_pNode->GetParent();

	return NULL;
}

SymbolTableEntryType::SymbolTableEntryType SymbolTableEntry::GetEntryType( void )
{
	return m_eEntryType;
}

void SymbolTableEntry::SetEntryType( SymbolTableEntryType::SymbolTableEntryType p_eValue )
{
	m_eEntryType = p_eValue;
}

void SymbolTableEntry::ToString( Meson::Common::Text::String& p_string )
{
	p_string.Append( "(" );
	p_string.Append( m_strName );
	p_string.Append( ", " );
	p_string.Append( m_dwScope );
	p_string.Append( ")->(" );

	switch( m_eEntryType )
	{
		case SymbolTableEntryType::Global:
			p_string.Append( "Global" );
			break;

		case SymbolTableEntryType::Local:
			p_string.Append( "Local" );
			break;

		case SymbolTableEntryType::Param:
			p_string.Append( "Param" );
			break;

		default:
			p_string.Append( "_" );
	}

	p_string.Append( ", " );
	p_string.Append( m_dwTypeCode );
	p_string.Append( ", " );
	p_string.Append( m_dwOffset );
	p_string.Append( ")" );
}


//---------------------------------------------------------------------------------------------------------



SymbolTable::SymbolTable( void ) : m_mapEntry()
{
}

SymbolTable::~SymbolTable( void )
{
	FreePointers( m_mapEntry );
}


//SymbolTableEntry* SymbolTable::Lookup( String p_strName )
//{
//	SymbolTableEntry* pEntry = NULL;
//
//	if( m_mapEntry.ContainsKey( p_strName, pEntry ) )
//	{
//		return pEntry;
//	}
//	else
//	{
//		/*int n = p_strName.LastIndexOf( '.' );
//		String	strKey,
//				strParent;
//
//		if( n == -1 )
//			strKey = p_strName;
//		else
//		{
//			strParent = p_strName.SubstringIndex( 0, n );
//			strKey = p_strName.SubstringIndex( n+1, p_strName.Size() - 1 );
//		}
//
//
//		n = strParent.LastIndexOf( '.' );
//		while( n != -1 )
//		{
//			strParent = strParent.SubstringIndex( 0, n );
//
//			if( m_mapEntry.ContainsKey( strParent + strKey, pEntry ) )
//				return pEntry;
//
//			n = strParent.LastIndexOf( '.' );
//		}
//
//		if( m_mapEntry.ContainsKey( strKey, pEntry ) )
//			return pEntry;*/
//
//		return pEntry;
//	}
//}


bool SymbolTable::Contains( Meson::Common::Text::String p_strName, SymbolTableEntry*& p_pEntry )
{
	return  m_mapEntry.ContainsKey( p_strName, p_pEntry );
}


SymbolTableEntry* SymbolTable::Lookup( String p_strName )
{
	SymbolTableEntry* pEntry = NULL;

	if( m_mapEntry.ContainsKey( p_strName, pEntry ) )
		return pEntry;

	TArrayList< String > lstName;
	p_strName.Split( '.', lstName );

	if( lstName.Size() == 1 )
		return NULL;

	return Lookup( lstName );
}

String BindKey( Meson::Common::Collections::TArrayList< Meson::Common::Text::String > p_lstName, int p_nIndex )
{
	String strKey;
	for( int n = 0; n < p_nIndex; n++ )
	{
		strKey += p_lstName[ n ];
		strKey += '.';
	}

	strKey += p_lstName[ p_lstName.Size() - 1 ];

	return strKey;
}


SymbolTableEntry* SymbolTable::Lookup( Meson::Common::Collections::TArrayList< Meson::Common::Text::String > p_lstName )
{
	SymbolTableEntry* pEntry = NULL;
	int n = (int)p_lstName.Size() - 2;
	if( n<0 )
		return Lookup( p_lstName[0] );

	while( n>=0 )
	{
		if( m_mapEntry.ContainsKey( BindKey( p_lstName, n ) , pEntry ) )
			return pEntry;

		n--;
	}

	return pEntry;
}



SymbolTableEntry* SymbolTable::Insert( Meson::Common::Text::String p_strName )
{
	SymbolTableEntry* pEntry = new SymbolTableEntry( p_strName );
	m_mapEntry.Insert( p_strName, pEntry );
	return pEntry;
}


SymbolTableEntry* SymbolTable::operator[]( String p_strName )
{
	return Lookup( p_strName );
}

SymbolTableEntry* SymbolTable::operator[]( Meson::Common::Collections::TArrayList< Meson::Common::Text::String > p_lstName )
{
	return Lookup( p_lstName );
}

void SymbolTable::ToString( Meson::Common::Text::String& p_string )
{
	TEnumerator< SymbolTableEntry* >& epEntry = m_mapEntry.GetValueEnumerator();
	
	p_string.Append( "{\r\n" );
	
	while( epEntry.HasMoreElements() )
	{
		epEntry.NextElement()->ToString(p_string);
		p_string.Append( ",\r\n" );
	}

	p_string.Append( "}" );
}

Meson_Cuneus_Compiler_END
