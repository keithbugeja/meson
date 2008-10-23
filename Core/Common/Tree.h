//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	16/02/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#pragma once

#include "BufferList.h"


Meson_Common_Collections_BEGIN

template< class TElement >
class TTreeNode
{
private:
	typedef TBufferList< TTreeNode* > NodeList;

public:
	typedef void (*Visitor)( TTreeNode< TElement >* );


protected:
	TElement  m_Datum;
	NodeList* m_plstChildren;

public:
	TTreeNode( void )
	{
		m_plstChildren = NULL;
	}


	TTreeNode( const TElement& p_tElement )
	{
		m_plstChildren = NULL;
		m_Datum = p_tElement;
	}

	TTreeNode( const TTreeNode& p_TreeNode )
	{
		if( p_TreeNode.m_plstChildren )
			m_plstChildren = new NodeList( p_TreeNode.m_plstChildren );
		else
			m_plstChildren = NULL;

		m_Datum = p_TreeNode.m_Datum;
	}

	~TTreeNode( void )
	{
		if( m_plstChildren )
		{
			FreePointers( *m_plstChildren );
			delete m_plstChildren;
		}
	}


public:
	inline TTreeNode operator=( const TTreeNode& p_TreeNode )
	{
		Clear();
		TTreeNode node( p_TreeNode);
		return node;
	}

	inline TTreeNode operator=( const TElement& p_tElement )
	{
		Set( p_tElement );
		return node;
	}

	inline void Clear( void )
	{
		if( m_plstChildren )
		{
			FreePointers( *m_plstChildren );
			delete m_plstChildren;
			m_plstChildren = NULL;
		}
	}

	inline size_t Size( void )
	{
		size_t unSize = 1;

		if( m_plstChildren )
		{
			for( size_t unIndex=0; unIndex<m_plstChildren->Size(); unIndex++ )
			{
				unSize += m_plstChildren->GetElement( unIndex )->Size();
			}
		}

		return unSize;
	}


	inline TElement Get( void ) const
	{
		return m_Datum;
	}

	inline void Set( const TElement& p_tELement )
	{
		m_Datum = p_tELement;
	}


	inline TTreeNode* Add( const TElement& p_tElement )
	{
		TTreeNode* pNode = new TTreeNode( p_tElement );
		AddChild( pNode );
		return pNode;
	}

	inline void AddChild( TTreeNode* p_pTreeNode )
	{
		if( !m_plstChildren )
			m_plstChildren = new NodeList( 2 );

		m_plstChildren->Add( p_pTreeNode );
	}


	inline void ForEach( Visitor p_Visitor )
	{
		p_Visitor( this );
		
		if( m_plstChildren )
		{
			for( size_t unIndex=0; unIndex<m_plstChildren->Size(); unIndex++ )
			{
				m_plstChildren->GetElement( unIndex )->ForEach( p_Visitor );
			}
		}
	}





	String ToString( void )
	{
		String str;

		str += m_Datum;

		if( m_plstChildren )
		{
			str += '[';
			for( size_t unIndex=0; unIndex<m_plstChildren->Size(); unIndex++ )
			{
				str += m_plstChildren->GetElement( unIndex )->ToString();
				str += ',';
			}
			str += ']';
		}

		return str;
	}
};

Meson_Common_Collections_END
