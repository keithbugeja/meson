#pragma once

#include "String8.h"
#include "LookaheadInputStream.h"
#include "ArrayList.h"
#include "Quaternion.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"

Meson_Gravitas_BEGIN

namespace LexerTokenType
{
	enum LexerTokenType
	{
		Undefined,
		Identifier,
		OpeningBrace,
		ClosingBrace,
		OpeningBracket,
		ClosingBracket,
		Assignment,
		Comma,
		Boolean,
		Integer,
		Real,
		String,
		EndOfStream
	};
}

Meson::Common::Text::String LexerTokenTypeToString(
	LexerTokenType::LexerTokenType p_lexerTokenType);

struct LexerToken
{
	LexerTokenType::LexerTokenType Type;
	bool ValueBoolean;
	int ValueInteger;
	Real ValueReal;
	Meson::Common::Text::String ValueString;

	LexerToken(void);
	LexerToken(const LexerToken& p_lexerToken);
	LexerToken(LexerTokenType::LexerTokenType p_lexerTokenType);
	LexerToken(bool p_bValue);
	LexerToken(int p_nValue);
	LexerToken(Real p_rValue);
	LexerToken(Meson::Common::Text::String p_strValue);

	bool operator==(const LexerToken& p_lexerToken) const;
	LexerToken& operator=(const LexerToken& p_lexerToken);
};

class Lexer
{
private:
	Meson::Common::IO::LookaheadInputStreamPtr m_pLookaheadInputStream;
	Meson::Common::Collections::TArrayList<LexerToken> m_listBuffer;

private:
	LexerToken ExtractToken();

public:
	Lexer(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);
	~Lexer(void);
	LexerToken PeekToken();
	LexerToken ReadToken();
	LexerToken ReadToken(LexerTokenType::LexerTokenType p_lexerTokenType);
	void ReadTokenList(
		LexerTokenType::LexerTokenType p_lexerTokenType,
		Meson::Common::Collections::TList<LexerToken>& p_listTokens);
	LexerToken ReadIdentifier(const Meson::Common::Text::String& p_strIdentifier);
	Meson::Common::Maths::TVector3<Real> ReadVector(void);
	Meson::Common::Maths::TPoint3<Real> ReadPoint(void);
	Meson::Common::Maths::TQuaternion<Real> ReadQuaternion(void);
	void ReadVectorList(Meson::Gravitas::VectorList& p_listVectors);
	void ReadPointList(Meson::Gravitas::PointList& p_listPoints);
	LexerToken ReadProperty(const Meson::Common::Text::String& p_strPropertyName,
		LexerTokenType::LexerTokenType p_lexerTokenType);
	bool ReadPropertyBoolean(const Meson::Common::Text::String& p_strPropertyName);
	int ReadPropertyInteger(const Meson::Common::Text::String& p_strPropertyName);
	Real ReadPropertyReal(const Meson::Common::Text::String& p_strPropertyName);
	Meson::Common::Text::String ReadPropertyString(
		const Meson::Common::Text::String& p_strPropertyName);
	Meson::Common::Maths::TVector3<Real> ReadPropertyVector(
		const Meson::Common::Text::String& p_strPropertyName);
	Meson::Common::Maths::TPoint3<Real> ReadPropertyPoint(
		const Meson::Common::Text::String& p_strPropertyName);
	void ReadCustomProperties(IGravitasEntity& p_gravitasEntity);
};

Meson_Gravitas_END
