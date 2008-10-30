#include "GrvLexer.h"

#include "MesonException.h"
#include "HashMap.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;

Meson_Gravitas_BEGIN

enum LexerState
{
	Start,
	Comment,
	LineComment,
	LineCommentEnding,
	BlockComment,
	BlockCommentEnding,
	Identifier,
	Integer,
	FloatingPoint,
	Fraction,
	StringLiteral,
	StringEscape,
	Error
};

String LexerTokenTypeToString(LexerTokenType::LexerTokenType p_lexerTokenType)
{
	switch(p_lexerTokenType)
	{
	case LexerTokenType::Identifier: return "Identifier";
	case LexerTokenType::OpeningBrace: return "Opening brace '{'";
	case LexerTokenType::ClosingBrace: return "Closing brace '}'";
	case LexerTokenType::Assignment: return "Assignment '='";
	case LexerTokenType::Boolean: return "Boolean value";
	case LexerTokenType::Integer: return "Integer value";
	case LexerTokenType::Real: return "Real value";
	case LexerTokenType::String: return "String literal";
	case LexerTokenType::EndOfStream: return "End of stream";
	default: return "Undefined";
	}
}

Meson_Gravitas_END

LexerToken::LexerToken(void)
	: Type(LexerTokenType::Undefined)
	, ValueBoolean(false)
	, ValueInteger(0)
	, ValueReal((Real) 0.0)
	, ValueString()
{
}

LexerToken::LexerToken(const LexerToken& p_lexerToken)
	: Type(p_lexerToken.Type)
	, ValueBoolean(p_lexerToken.ValueBoolean)
	, ValueInteger(p_lexerToken.ValueInteger)
	, ValueReal(p_lexerToken.ValueReal)
	, ValueString(p_lexerToken.ValueString)
{
}

LexerToken::LexerToken(LexerTokenType::LexerTokenType p_lexerTokenType)
	: Type(p_lexerTokenType)
	, ValueBoolean(false)
	, ValueInteger(0)
	, ValueReal((Real) 0.0)
	, ValueString()
{
}

LexerToken::LexerToken(bool p_bValue)
	: Type(LexerTokenType::Boolean)
	, ValueBoolean(p_bValue)
	, ValueInteger(0)
	, ValueReal((Real) 0.0)
	, ValueString()
{
}

LexerToken::LexerToken(int p_nValue)
	: Type(LexerTokenType::Integer)
	, ValueBoolean(false)
	, ValueInteger(p_nValue)
	, ValueReal((Real) 0.0)
	, ValueString()
{
}

LexerToken::LexerToken(Real p_rValue)
	: Type(LexerTokenType::Real)
	, ValueBoolean(false)
	, ValueInteger(0)
	, ValueReal(p_rValue)
	, ValueString()
{
}

LexerToken::LexerToken(String p_strValue)
	: Type(LexerTokenType::String)
	, ValueBoolean(false)
	, ValueInteger(0)
	, ValueReal((Real) 0.0)
	, ValueString(p_strValue)
{
}

bool LexerToken::operator==(const LexerToken& p_lexerToken) const
{
	return false;
}

LexerToken& LexerToken::operator=(const LexerToken& p_lexerToken)
{
	Type = p_lexerToken.Type;
	ValueBoolean = p_lexerToken.ValueBoolean;
	ValueInteger = p_lexerToken.ValueInteger;
	ValueReal = p_lexerToken.ValueReal;
	ValueString = p_lexerToken.ValueString;
	return *this;
}

LexerToken Lexer::ExtractToken()
{
	LexerState state = Start;

	LexerToken lexerToken;
	char ch;
	String strIdentifier;
	bool bNegative = false;
	int nInteger = 0;
	Real rReal = (Real) 0.0;
	Real rDivisor = (Real) 1.0;
	String strValue;

	while (!m_pLookaheadInputStream->IsEndOfStream())
	{
		m_pLookaheadInputStream->Read(&ch, 1);

		switch(state)
		{
		case Start:
			if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
				; // ingore whitespace
			else if (ch == '{')
			{
				lexerToken.Type = LexerTokenType::OpeningBrace;
				lexerToken.ValueString = "{";
				return lexerToken;
			}
			else if (ch == '}')
			{
				lexerToken.Type = LexerTokenType::ClosingBrace;
				lexerToken.ValueString = "}";
				return lexerToken;
			}
			else if (ch == '[')
			{
				lexerToken.Type = LexerTokenType::OpeningBracket;
				lexerToken.ValueString = "[";
				return lexerToken;
			}
			else if (ch == ']')
			{
				lexerToken.Type = LexerTokenType::ClosingBracket;
				lexerToken.ValueString = "]";
				return lexerToken;
			}
			else if (ch == '=')
			{
				lexerToken.Type = LexerTokenType::Assignment;
				lexerToken.ValueString = "=";
				return lexerToken;
			}
			else if (ch == ',')
			{
				lexerToken.Type = LexerTokenType::Comma;
				lexerToken.ValueString = ",";
				return lexerToken;
			}
			else if (ch == '/')
				state = Comment;
			else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
			{
				strIdentifier = ch;
				state = Identifier;
			}
			else if (ch == '-')
			{
				bNegative = true;
				strValue = ch;
				state = Integer;
			}
			else if (ch >= '0' && ch <= '9')
			{
				nInteger = ch - '0';
				state = Integer;
			}
			else if (ch == '\"')
				state = StringLiteral;
			else
				throw new MesonException(
					"Parsing Error: Unrecognised token.", __FILE__, __LINE__);
			break;
		case Comment:
			switch (ch)
			{
			case '/':
				state = LineComment;
				break;
			case '*':
				state = BlockComment;
				break;
			default:
				throw new MesonException(
					"Parsing error: '/' or '*' expected.", __FILE__, __LINE__);
			}
			break;
		case LineComment:
			if (ch == '\r')
				state = LineCommentEnding;
			break;
		case LineCommentEnding:
			if (ch == '\n')
				state = Start;
			else
			{
				m_pLookaheadInputStream->Pushback(&ch, 1);
				state = Start;
			}
			break;
		case BlockComment:
			if (ch == '*')
				state = BlockCommentEnding;
			break;
		case BlockCommentEnding:
			if (ch == '/')
				state = Start;
			else
				state = BlockComment;
			break;
		case Identifier:
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'
				|| (ch >= '0' && ch <= '9'))
				strIdentifier += ch;
			else
			{
				m_pLookaheadInputStream->Pushback(&ch, 1);

				if (strIdentifier == "true" || strIdentifier == "false")
				{
					lexerToken.Type = LexerTokenType::Boolean;
					lexerToken.ValueString = strIdentifier;
					lexerToken.ValueBoolean = (strIdentifier == "true");
					return lexerToken;				
				}
				else
				{
					lexerToken.Type = LexerTokenType::Identifier;
					lexerToken.ValueString = strIdentifier;
					return lexerToken;				
				}
			}
			break;
		case Integer:
			if (ch >= '0' && ch <= '9')
			{
				nInteger = nInteger * 10 + (ch - '0');
			}
			else if (ch == '.')
			{
				rReal = (Real) nInteger;
				state = FloatingPoint;
			}
			else
			{
				m_pLookaheadInputStream->Pushback(&ch, 1);
				if (bNegative)
					nInteger = -nInteger;
				return LexerToken(nInteger);
			}
			break;
		case FloatingPoint:
			if (ch >= '0' && ch <= '9')
			{
				rDivisor = (Real) 0.1;
				rReal += (Real) (ch - '0') * rDivisor;
				state = Fraction;
			}
			else
				throw new MesonException(
					"Parsing Error: Digit expected after floating point.", __FILE__, __LINE__);
			break;
		case Fraction:
			if (ch >= '0' && ch <= '9')
			{
				rDivisor *= (Real) 0.1;
				rReal += (Real) (ch - '0') * rDivisor;
			}
			else
			{
				m_pLookaheadInputStream->Pushback(&ch, 1);
				if (bNegative)
					rReal = -rReal;
				return LexerToken(rReal);
			}
			break;
		case StringLiteral:
			if (ch == '\"')
				return LexerToken(strValue);
			else if (ch == '\\')
				state = StringEscape;
			else if (ch == '\r' || ch == '\n')
				throw new MesonException(
					"Parsing Error: String cannot span across lines.", __FILE__, __LINE__);
			else
				strValue += ch;
			break;
		case StringEscape:
			if (ch == 'r')
				strValue += '\r';
			else if (ch == 'n')
				strValue += '\n';
			else if (ch == 't')
				strValue += '\t';
			else if (ch == 'a')
				strValue += '\a';
			else if (ch == 'b')
				strValue += '\b';
			else if (ch == 'f')
				strValue += '\f';
			else if (ch == 'v')
				strValue += '\v';
			else if (ch == '\\')
				strValue += '\\';
			else if (ch == '\"')
				strValue += '\"';
			else
				throw new MesonException(
					"Parsing Error: Invalid string escape code..", __FILE__, __LINE__);
			state = StringLiteral;
			break;
		}
	}

	if (state != Start)
		throw new MesonException(
			"Parsing error: Unexpected end of stream.", __FILE__, __LINE__);

	lexerToken.Type = LexerTokenType::EndOfStream;
	return lexerToken;
}


Lexer::Lexer(LookaheadInputStreamPtr p_pLookaheadInputStream)
	: m_pLookaheadInputStream(p_pLookaheadInputStream)
	, m_listBuffer()
{
}

Lexer::~Lexer(void)
{
}

LexerToken Lexer::PeekToken()
{
	if (m_listBuffer.Size() == 0)
		m_listBuffer.Add(ExtractToken());
	return m_listBuffer[0];
}

LexerToken Lexer::ReadToken()
{
	if (m_listBuffer.Size() > 0)
	{
		LexerToken lexerToken = m_listBuffer[0];
		m_listBuffer.RemoveAt(0);
		return lexerToken;
	}

	return ExtractToken();
}

LexerToken Lexer::ReadToken(LexerTokenType::LexerTokenType p_lexerTokenType)
{
	LexerToken lexerToken = ReadToken();
	if (lexerToken.Type!= p_lexerTokenType)
		throw new MesonException(
			"Parsing Error: Expected token of type: "
				+ LexerTokenTypeToString(p_lexerTokenType),
			__FILE__, __LINE__);
	return lexerToken;
}

TVector3<Real> Lexer::ReadVector(void)
{
	TVector3<Real> vecValue;
	ReadToken(LexerTokenType::OpeningBrace);

	LexerToken lexerToken = ReadToken(LexerTokenType::Real);
	vecValue.X = lexerToken.ValueReal;

	ReadToken(LexerTokenType::Comma);

	lexerToken = ReadToken(LexerTokenType::Real);
	vecValue.Y = lexerToken.ValueReal;

	ReadToken(LexerTokenType::Comma);

	lexerToken = ReadToken(LexerTokenType::Real);
	vecValue.Z = lexerToken.ValueReal;

	ReadToken(LexerTokenType::ClosingBrace);

	return vecValue;
}

TQuaternion<Real> Lexer::ReadQuaternion(void)
{
	TQuaternion<Real> qtnValue;
	ReadToken(LexerTokenType::OpeningBrace);

	LexerToken lexerToken = ReadToken(LexerTokenType::Real);
	qtnValue.W = lexerToken.ValueReal;

	ReadToken(LexerTokenType::Comma);

	lexerToken = ReadToken(LexerTokenType::Real);
	qtnValue.X = lexerToken.ValueReal;

	ReadToken(LexerTokenType::Comma);

	lexerToken = ReadToken(LexerTokenType::Real);
	qtnValue.Y = lexerToken.ValueReal;

	ReadToken(LexerTokenType::Comma);

	lexerToken = ReadToken(LexerTokenType::Real);
	qtnValue.Z = lexerToken.ValueReal;

	ReadToken(LexerTokenType::ClosingBrace);

	return qtnValue;
}

void Lexer::ReadTokenList(
	LexerTokenType::LexerTokenType p_lexerTokenType,
	Meson::Common::Collections::TList<LexerToken>& p_listTokens)
{
	p_listTokens.Clear();

	ReadToken(LexerTokenType::OpeningBrace);
	while (true)
	{
		LexerToken lexerToken = PeekToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			break;

		if (p_listTokens.Size() > 0)
			ReadToken(LexerTokenType::Comma);

		lexerToken = ReadToken(p_lexerTokenType);

		p_listTokens.Add(lexerToken);
	}

	ReadToken(LexerTokenType::ClosingBrace);
}

LexerToken Lexer::ReadIdentifier(const String& p_strIdentifier)
{
	LexerToken lexerToken = ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != p_strIdentifier)
		throw new MesonException(
			"Parsing error: Expected identifier '" + p_strIdentifier + "'",
			__FILE__, __LINE__);

	return lexerToken;
}

void Lexer::ReadVectorList(VectorList& p_listVectors)
{
	p_listVectors.Clear();

	ReadToken(LexerTokenType::OpeningBrace);
	while(true)
	{
		LexerToken lexerToken = PeekToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			break;

		if (p_listVectors.Size() > 0)
			ReadToken(LexerTokenType::Comma);

		p_listVectors.Add(ReadVector());
	}

	ReadToken(LexerTokenType::ClosingBrace);
}

LexerToken Lexer::ReadProperty(const String& p_strPropertyName,
	LexerTokenType::LexerTokenType p_lexerTokenType)
{
	ReadIdentifier(p_strPropertyName);

	ReadToken(LexerTokenType::Assignment);

	LexerToken lexerToken = ReadToken(p_lexerTokenType);

	return lexerToken;
}

bool Lexer::ReadPropertyBoolean(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerTokenType::Boolean);
	return lexerToken.ValueBoolean;
}

int Lexer::ReadPropertyInteger(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerTokenType::Integer);
	return lexerToken.ValueInteger;
}

Real Lexer::ReadPropertyReal(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerTokenType::Real);
	return lexerToken.ValueReal;
}

String Lexer::ReadPropertyString(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerTokenType::String);
	return lexerToken.ValueString;
}

TVector3<Real> Lexer::ReadPropertyVector(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerTokenType::Assignment);

	return ReadVector();
}

void Lexer::ReadCustomProperties(IGravitasEntity& p_gravitasEntity)
{
	LexerToken lexerToken = ReadToken(LexerTokenType::Identifier);

	if (lexerToken.ValueString != "CustomProperties")
		throw new MesonException(
			"Parsing Error: Expected identifier 'CustomProperties'",
			__FILE__, __LINE__);

	ReadToken(LexerTokenType::OpeningBrace);

	// get properties accepted by entity
	THashMap<String, PropertyDescriptor> mapProperties;
	p_gravitasEntity.EnumerateProperties(mapProperties);

	// lists for list-type values
	TArrayList<LexerToken> listTokens;
	TArrayList<bool> listBooleans;
	TArrayList<int> listIntegers;
	TArrayList<Real> listReals;
	TArrayList<String> listStrings;
	VectorArrayList listVectors;

	// properties
	while (true)
	{
		lexerToken = ReadToken();

		// closing brace for 'Properties' container
		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier)
			throw new MesonException(
				"Parsing Error: Opening bracket or property name expected.",
				__FILE__, __LINE__);

		String strPropertyName = lexerToken.ValueString;

		// check if property is valid
		if (!mapProperties.ContainsKey(strPropertyName))
			throw new MesonException(
				"Unsupported property '" + strPropertyName + "'.",
				__FILE__, __LINE__);

		PropertyDescriptor& propertyDescriptor = mapProperties[strPropertyName];

		// if property is index, parse indexing
		int nPropertyIndex = -1;
		if (propertyDescriptor.IsIndexedProperty())
		{
			ReadToken(LexerTokenType::OpeningBracket);
			lexerToken = ReadToken(LexerTokenType::Integer);
			nPropertyIndex = lexerToken.ValueInteger;

			if (nPropertyIndex < 0)
				throw new MesonException(
					"A property index must be non-negative.",
					__FILE__, __LINE__);

			ReadToken(LexerTokenType::ClosingBracket);
		}


		// assignment
		ReadToken(LexerTokenType::Assignment);


		LexerToken lexerTokenValue;
		switch (propertyDescriptor.Type)
		{
		case PropertyType::Boolean:
			lexerTokenValue = ReadToken(LexerTokenType::Boolean);
			p_gravitasEntity.SetProperty(strPropertyName, lexerTokenValue.ValueBoolean);
			break;
		case PropertyType::Integer:
			lexerTokenValue = ReadToken(LexerTokenType::Integer);
			p_gravitasEntity.SetProperty(strPropertyName, lexerTokenValue.ValueInteger);
			break;
		case PropertyType::Real:
			lexerTokenValue = ReadToken(LexerTokenType::Real);
			p_gravitasEntity.SetProperty(strPropertyName, lexerTokenValue.ValueReal);
			break;
		case PropertyType::String:
			lexerTokenValue = ReadToken(LexerTokenType::String);
			p_gravitasEntity.SetProperty(strPropertyName, lexerTokenValue.ValueString);
			break;
		case PropertyType::Vector:
			p_gravitasEntity.SetProperty(strPropertyName, ReadVector());
			break;

		case PropertyType::IndexedBoolean:
			lexerTokenValue = ReadToken(LexerTokenType::Boolean);
			p_gravitasEntity.SetProperty(strPropertyName, (size_t) nPropertyIndex,
				lexerTokenValue.ValueBoolean);
			break;
		case PropertyType::IndexedInteger:
			lexerTokenValue = ReadToken(LexerTokenType::Integer);
			p_gravitasEntity.SetProperty(strPropertyName, (size_t) nPropertyIndex,
				lexerTokenValue.ValueInteger);
			break;
		case PropertyType::IndexedReal:
			lexerTokenValue = ReadToken(LexerTokenType::Real);
			p_gravitasEntity.SetProperty(strPropertyName, (size_t) nPropertyIndex,
				lexerTokenValue.ValueReal);
			break;
		case PropertyType::IndexedString:
			lexerTokenValue = ReadToken(LexerTokenType::String);
			p_gravitasEntity.SetProperty(strPropertyName, (size_t) nPropertyIndex,
				lexerTokenValue.ValueString);
			break;
		case PropertyType::IndexedVector:
			p_gravitasEntity.SetProperty(strPropertyName, (size_t) nPropertyIndex,
				ReadVector());
			break;

		case PropertyType::BooleanList:
			ReadTokenList(LexerTokenType::Boolean, listTokens);
			listBooleans.Clear();
			for (size_t unIndex = 0; unIndex < listTokens.Size(); unIndex++)
				listBooleans.Add(listTokens[unIndex].ValueBoolean);
			p_gravitasEntity.SetProperty(strPropertyName, listBooleans);
			break;
		case PropertyType::IntegerList:
			ReadTokenList(LexerTokenType::Integer, listTokens);
			listIntegers.Clear();
			for (size_t unIndex = 0; unIndex < listTokens.Size(); unIndex++)
				listIntegers.Add(listTokens[unIndex].ValueInteger);
			p_gravitasEntity.SetProperty(strPropertyName, listIntegers);
			break;
		case PropertyType::RealList:
			ReadTokenList(LexerTokenType::Real, listTokens);
			listReals.Clear();
			for (size_t unIndex = 0; unIndex < listTokens.Size(); unIndex++)
				listReals.Add(listTokens[unIndex].ValueReal);
			p_gravitasEntity.SetProperty(strPropertyName, listReals);
			break;
		case PropertyType::StringList:
			ReadTokenList(LexerTokenType::String, listTokens);
			listStrings.Clear();
			for (size_t unIndex = 0; unIndex < listTokens.Size(); unIndex++)
				listStrings.Add(listTokens[unIndex].ValueString);
			p_gravitasEntity.SetProperty(strPropertyName, listStrings);
			break;
		case PropertyType::VectorList:
			ReadVectorList(listVectors);
			p_gravitasEntity.SetProperty(strPropertyName, listVectors);
			break;
		}
	}
}