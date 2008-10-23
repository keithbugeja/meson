//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class Token
	{
	public:
		enum TokenType
		{
			TT_KEYWORD,
			TT_SYMBOL,
			TT_LITERAL,
			TT_INTEGER,
			TT_REAL,
			TT_EOS,
			TT_UNDEFINED,

			TT_COUNT
		};

	public:
		Token()
			: m_eTokenType(TT_UNDEFINED)
			, m_nIntegerValue(0)
		{ }

		Token(const Token& p_token)
			: m_eTokenType(p_token.m_eTokenType)
			, m_dfRealValue(p_token.m_dfRealValue)
			, m_strStringValue(p_token.m_strStringValue)
		{
		}

		~Token() { }

		Token& operator=(const Token& p_token)
		{
			m_eTokenType = p_token.m_eTokenType;
			m_dfRealValue = p_token.m_dfRealValue;
			m_strStringValue = p_token.m_strStringValue;
			
			return *this;
		}

		bool operator==(const Token& p_token)
		{
			return (this == &p_token);
		}

		void SetType(TokenType p_eTokenType) { m_eTokenType = p_eTokenType; }
		TokenType GetType(void) const { return m_eTokenType; }

		void SetString(const Meson::Common::Text::String& p_strValue) { m_strStringValue = p_strValue; }
		const Meson::Common::Text::String& GetString(void) { return m_strStringValue; }

		void SetInteger(int p_nValue) { m_nIntegerValue = p_nValue; }
		int GetInteger(void) const { return m_nIntegerValue; }

		void SetReal(double p_dfValue) { m_dfRealValue = p_dfValue; }
		double GetReal(void) const { return m_dfRealValue; }
	
		bool IsDefined(void) const { return m_eTokenType != TT_UNDEFINED; }
		bool IsEOS(void) const { return m_eTokenType == TT_EOS; }
		bool IsNumeric(void) const { return m_eTokenType == TT_INTEGER || m_eTokenType == TT_REAL; }
		bool IsLiteral(void) const { return m_eTokenType == TT_LITERAL; }
		bool IsKeyword(void) const { return m_eTokenType == TT_KEYWORD; }
		bool IsSymbol(void) const { return m_eTokenType == TT_SYMBOL; }

	protected:
		TokenType m_eTokenType;
		Meson::Common::Text::String m_strStringValue;

		union
		{
			long double m_dfRealValue;
			int m_nIntegerValue;
		};
	};
	//----------------------------------------------------------------------------------------------
	class TokenStream : public Meson::Common::Collections::TArrayList<Token>
	{
	public:
		void Reset(void)
		{
			m_uiPosition = 0;
		}

		bool HasMoreTokens(void) const
		{
			return (m_uiPosition < this->Size());
		}

		void Forward(uint p_uiOffset = 1)
		{
			m_uiPosition += p_uiOffset;
		}

		Token& NextToken(void)
		{
			return this->GetElement(m_uiPosition++);
		}

		Token& GetToken(uint p_uiOffset) const
		{
			uint uiPosition = m_uiPosition + p_uiOffset;
			return this->GetElement(uiPosition);
		}

	protected:
		uint m_uiPosition;
	};

	//----------------------------------------------------------------------------------------------
Meson_Vistas_END