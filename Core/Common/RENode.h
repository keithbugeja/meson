#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"
#include "Stack.h"
#include "String8.h"
#include "BitSet.h"

Meson_Common_RegularExpressions_BEGIN

namespace RENodeType 
{ 
	enum RENodeType 
	{ 
		FAULT, NONE, DOT, CHAR, BEGIN, END, PARENTHESIS, BRACES, BRACKETS, STAR, PLUS, QMARK, PIPE 
	}; 
};

class RENode
{
protected:
	RENodeType::RENodeType m_eType;
	Meson::Common::Collections::TStack< RENode* > *m_plstNode;

	union
	{
		char m_cChar;

		struct
		{
			int	m_nMin,
				m_nMax;
		};

		Meson::Common::Collections::BitSet *m_pBitSet;
	};

public:
	RENode( void );
	RENode( RENodeType::RENodeType p_eType );
	~RENode( void );


	RENodeType::RENodeType GetType( void );
	void SetType( RENodeType::RENodeType p_eType );

	char GetChar( void );
	void SetChar( char p_cChar );
	int  GetMin( void );
	void SetMin( int p_nMin );
	int  GetMax( void );
	void SetMax( int p_nMax );
	Meson::Common::Collections::BitSet *GetBitSet( void );

	void AddNode( RENode* p_pNode );
	RENode *PopNode( void );

	bool Matches( Meson::Common::Text::String &p_str, size_t &p_unIndex );

	virtual Meson::Common::Text::String ToString( void );

	operator const char*( void );
};

Meson_Common_RegularExpressions_END

