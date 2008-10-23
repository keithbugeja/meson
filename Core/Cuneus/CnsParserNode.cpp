#include "CnsParserNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"
#include "CnsTypeNode.h"

#include "iostream"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Cuneus_Compiler_BEGIN

ParserNode::ParserNode(void) : m_strName( "Node" )
{
	m_plstChildNode = NULL;
	m_pParent = NULL;
	m_pType = NULL;
}

ParserNode::ParserNode( Meson::Common::Text::String p_strName ) : m_strName( p_strName )
{
	m_plstChildNode = NULL;
	m_pParent = NULL;
	m_pType = NULL;
}

ParserNode::~ParserNode(void)
{
	if( m_plstChildNode )
	{
		FreePointers( *m_plstChildNode );
		delete m_plstChildNode;
	}
}

String ParserNode::GetName( void )
{
	return m_strName;
}

void ParserNode::SetName( String p_strName )
{
	m_strName = p_strName;
}

String ParserNode::GetQualifiedName( void )
{
	String strName = m_strName;

	ParserNode* pParent = m_pParent;
	while( pParent->GetParent() )
	//while( pParent )
	{
		strName.Insert( 0, '.' );
		strName.Insert( 0, pParent->GetName() );
		pParent = pParent->GetParent();
	}

	return strName;
}

dword ParserNode::GetTypeCode( void )
{
	if( !m_pType )
		return 0;

	return m_pType->GetTypeCode();
}

void ParserNode::SetTypeCode( dword p_dwTypeCode )
{
	if( !m_pType )
		m_pType = new ParserNodes::TypeNode();

	return m_pType->SetTypeCode( p_dwTypeCode );
}

ParserNode* ParserNode::GetParent( void )
{
	return m_pParent;
}

void ParserNode::SetParent( ParserNode* p_pParent )
{
	m_pParent = p_pParent;
}

void ParserNode::IncLocalVars( void )
{
	m_pParent->IncLocalVars();
}
	
dword ParserNode::GetLocalVars( void )
{
	return m_pParent->GetLocalVars();
}

bool ParserNode::HasChildren( void )
{
	return ( ( m_plstChildNode != NULL ) && ( m_plstChildNode->Size() > 0 ) );
}

bool ParserNode::HasNoChildren( void )
{
	return ( ( m_plstChildNode == NULL ) || ( m_plstChildNode->Size() == 0 ) );
}

size_t ParserNode::NumberOfChildren( void )
{
	if( !m_plstChildNode )
		return 0;
	else
		return m_plstChildNode->Size();
}

void ParserNode::AddChild( ParserNode* p_pParserNode )
{
	if( !m_plstChildNode )
		m_plstChildNode = new TBufferList<ParserNode*>();

	p_pParserNode->SetParent( this );
	m_plstChildNode->Add( p_pParserNode );
}

ParserNode* ParserNode::RemoveChild( int p_nIndex )
{
	ParserNode* pParserNode = NULL;
	if( m_plstChildNode )
	{
		pParserNode = (*m_plstChildNode)[ p_nIndex ];
		m_plstChildNode->RemoveAt( p_nIndex );
	}

	return pParserNode;
}

ParserNode* ParserNode::RemoveLastChild( void )
{
	ParserNode* pParserNode = NULL;
	if( m_plstChildNode )
	{
		int nIndex = ( (int)m_plstChildNode->Size() ) - 1;
		if( nIndex == -1 )
			return NULL;

		pParserNode = (*m_plstChildNode)[ nIndex ];
		m_plstChildNode->RemoveAt( nIndex );
	}

	return pParserNode;
}


ParserNode* ParserNode::GetChild( int p_nIndex )
{
	ParserNode* pParserNode = NULL;
	if( m_plstChildNode )
		pParserNode = (*m_plstChildNode)[ p_nIndex ];

	return pParserNode;
}

ParserNode* ParserNode::SetChild( int p_nIndex, ParserNode* p_pParserNode )
{
	ParserNode* pParserNode = NULL;
	if( m_plstChildNode )
	{
		pParserNode = (*m_plstChildNode)[ p_nIndex ];
		(*m_plstChildNode)[ p_nIndex ] = p_pParserNode;
	}

	return pParserNode;
}

void ParserNode::Merge( ParserNode* p_pParserNode )
{
	if( m_plstChildNode )
	{
		m_plstChildNode->AddList( *p_pParserNode->m_plstChildNode );
	}
	else
		m_plstChildNode = new TBufferList<ParserNode*>( *p_pParserNode->m_plstChildNode );

}

ParserNode* ParserNode::operator[]( int p_nIndex )
{
	return (m_plstChildNode) ? (*m_plstChildNode)[ p_nIndex ] : NULL;
}


Meson::Common::Text::String ParserNode::ToString( void )
{
	String str;

	if( m_strName.Size() > 0 )
		str += m_strName;
	
	if(m_pType)
		str += "<" + m_pType->ToString() + ">";

	if( m_plstChildNode )
	{
		str += '{';

		ParserNode* pNode;

		size_t unSize = m_plstChildNode->Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			//std::cout << "ParserNode:" << str << std::endl;
			pNode = (*m_plstChildNode)[ un ];
			if( pNode )
				str += (*m_plstChildNode)[ un ]->ToString();
			else
				str += "_NULLPTR_";

			if( un < (unSize - 1) )
				str += ", ";
		}

		str += '}';
	}

	return str;
}


bool ParserNode::TypeCheck( SemanticAnalyser* p_pAnalyser )
{
	return p_pAnalyser->Check( this );

	/*if( !m_plstChildNode )
		return true;
	else
	{
		size_t unSize = m_plstChildNode->Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			if( !p_pAnalyser->Check( (*m_plstChildNode)[un] ) )
				return false;
		}
		return true;
	}*/
}

bool ParserNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//bool ParserNode::Generate( CodeGenerator* p_pGenerator, int p_nIndex )
//{
//	//return p_pGenerator->Generate( this, p_nIndex );
//	return false;
//}

Meson_Cuneus_Compiler_END
