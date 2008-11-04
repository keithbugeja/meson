//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "InputStream.h"
#include "LookaheadInputStream.h"
#include "TreeSet.h"

#include "VstTypes.h"
#include "VstToken.h"
#include "VstPropertySet.h"

Meson_Vistas_BEGIN

	class VISTAS_API LexerToken
	{
	public:
		enum TokenType
		{
			TT_LEFT_BRACKET,
			TT_RIGHT_BRACKET,
			TT_LEFT_BRACE,
			TT_RIGHT_BRACE,
			TT_COMMA,
			TT_EQUALS,
			TT_BOOLEAN,
			TT_INTEGER,
			TT_FLOAT,
			TT_LITERAL,
			TT_KEYWORD,
			TT_EOS,
			TT_UNDEFINED,

			TT_COUNT
		};

	public:
		LexerToken();
		LexerToken(bool p_bValue);
		LexerToken(int p_nValue);
		LexerToken(float p_fValue);
		LexerToken(const Meson::Common::Text::String& p_strValue, bool p_bIsLiteral=false);
		LexerToken(TokenType p_eTokenType);
		LexerToken(const LexerToken& p_lexerToken);
		LexerToken& operator=(const LexerToken& p_lexerToken);
		bool operator==(const LexerToken& p_lexerToken);

		static Meson::Common::Text::String LexerTokenToString(TokenType p_eTokenType);

	public:
		TokenType Type;
		Meson::Common::Text::String StringValue;
		bool BooleanValue;
		float FloatValue;
		int IntegerValue;
	};

	class VISTAS_API Lexer
	{
	public:
		enum LexerState
		{
			LS_START,
			LS_COMMENT,
			LS_LINECOMMENT,
			LS_LINECOMMENT_ENDING,
			LS_BLOCKCOMMENT,
			LS_BLOCKCOMMENT_ENDING,
			LS_KEYWORD,
			LS_INTEGER,
			LS_FLOAT,
			LS_FRACTION,
			LS_LITERAL,
			LS_ESCAPE,
			LS_ERROR,

			LS_COUNT
		};

	protected:
		LexerToken ExtractToken(void);

	public:
		Lexer(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		~Lexer(void);
		LexerToken PeekToken();
		LexerToken ReadToken();
		LexerToken ReadToken(LexerToken::TokenType p_eLexerTokenType);
		void ReadTokenList(LexerToken::TokenType p_eLexerTokenType, Meson::Common::Collections::TList<LexerToken>& p_listTokens);
		LexerToken ReadKeyword(const Meson::Common::Text::String& p_strKeyword);
		
		Meson::Common::Maths::Vector2f ReadVector2();
		Meson::Common::Maths::Vector3f ReadVector3();
		Meson::Common::Maths::Vector4f ReadVector4();

		Meson::Common::Maths::Matrix2f ReadMatrix2();
		Meson::Common::Maths::Matrix3f ReadMatrix3();
		Meson::Common::Maths::Matrix4f ReadMatrix4();

		Colour ReadColour();

		LexerToken ReadProperty(const Meson::Common::Text::String& p_strPropertyName, LexerToken::TokenType p_eLexerTokenType);
		
		bool ReadPropertyBoolean(const Meson::Common::Text::String& p_strPropertyName);
		int ReadPropertyInteger(const Meson::Common::Text::String& p_strPropertyName);
		float ReadPropertyFloat(const Meson::Common::Text::String& p_strPropertyName);
		
		Meson::Common::Text::String ReadPropertyString(const Meson::Common::Text::String& p_strPropertyName);

		Meson::Common::Maths::Vector2f ReadPropertyVector2(const Meson::Common::Text::String& p_strPropertyName);
		Meson::Common::Maths::Vector3f ReadPropertyVector3(const Meson::Common::Text::String& p_strPropertyName);
		Meson::Common::Maths::Vector4f ReadPropertyVector4(const Meson::Common::Text::String& p_strPropertyName);

		Meson::Common::Maths::Matrix2f ReadPropertyMatrix2(const Meson::Common::Text::String& p_strPropertyName);
		Meson::Common::Maths::Matrix3f ReadPropertyMatrix3(const Meson::Common::Text::String& p_strPropertyName);
		Meson::Common::Maths::Matrix4f ReadPropertyMatrix4(const Meson::Common::Text::String& p_strPropertyName);

		void ReadCustomProperties(IPropertySet& p_propertySet);

	protected:
		Meson::Common::IO::LookaheadInputStreamPtr m_pInputStream;
		Meson::Common::Collections::TArrayList<LexerToken> m_listBuffer;
	};

	//----------------------------------------------------------------------------------------------
	class VISTAS_API ILexer 
	{
	public:
		ILexer(Meson::Common::IO::IInputStream* p_pInputStream)
			: m_pInputStream(p_pInputStream)
		{
		}

		virtual ~ILexer(void) { }

		int NextChar(void)
		{
			char cInput; 

			while(m_pInputStream->IsDataAvailable())
			{
				(*m_pInputStream)>>cInput;
				
				if (IsValidTokenChar(cInput))
					return cInput;
			}

			return -1;
		}

		int ReadChar(void)
		{
			char cInput = -1;

			if (m_pInputStream->IsDataAvailable())
				(*m_pInputStream)>>cInput;
			
			return cInput;
		}

		void AddKeyword(Meson::Common::Text::String p_strKeyword, int p_nKeywordId = 0)
		{
			m_keywordMap.Insert(p_strKeyword, p_nKeywordId);
		}

		void AddSymbol(char p_cSymbol)
		{
			m_symbolSet.Insert(p_cSymbol);
		}
		
		bool IsValidTokenChar(char cValue)
		{
			return (cValue >= 33 && cValue <=126);
		}

		bool IsDigit(char cValue)
		{
			return (cValue >= '0' && cValue <= '9');
		}

		bool IsSymbol(char p_cSymbol)
		{
			return (m_symbolSet.ContainsElement(p_cSymbol));
		}

		bool IsKeyword(Meson::Common::Text::String& p_strKeyword)
		{
			return (m_keywordMap.ContainsKey(p_strKeyword));
		}

		virtual Token ReadToken(void) = 0;

	protected:
		Meson::Common::IO::IInputStream* m_pInputStream;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, int> m_keywordMap;
		Meson::Common::Collections::TTreeSet<char> m_symbolSet;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END