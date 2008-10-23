//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstLexer.h"
#include "VstColour.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
LexerToken::LexerToken() 
	: Type(TT_UNDEFINED) 
	, BooleanValue(false)
	, IntegerValue(0)
	, FloatValue(0.0f)
	, StringValue()
{ }
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(bool p_bValue)
	: Type(LexerToken::TT_BOOLEAN)
	, BooleanValue(p_bValue)
	, IntegerValue(0)
	, FloatValue(0.0f)
	, StringValue()
{
}
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(int p_nValue)
	: Type(LexerToken::TT_INTEGER)
	, BooleanValue(false)
	, IntegerValue(p_nValue) 
	, FloatValue(0.0f)
	, StringValue()
{ }
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(float p_fValue)
	: Type(LexerToken::TT_FLOAT)
	, BooleanValue(false)
	, IntegerValue(0) 
	, FloatValue(p_fValue)
	, StringValue()
{ }
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(const Meson::Common::Text::String& p_strValue, bool p_bIsLiteral)
	: Type(p_bIsLiteral ? LexerToken::TT_LITERAL : LexerToken::TT_KEYWORD) 	
	, BooleanValue(false)
	, IntegerValue(0) 
	, FloatValue(0.0f)
	, StringValue(p_strValue)
{ }
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(TokenType p_eTokenType)
	: Type(p_eTokenType) 
	, BooleanValue(false)
	, IntegerValue(0) 
	, FloatValue(0.0f)
	, StringValue()
{ }
//----------------------------------------------------------------------------------------------
LexerToken::LexerToken(const LexerToken& p_lexerToken)
	: Type(p_lexerToken.Type)
	, BooleanValue(p_lexerToken.BooleanValue)
	, IntegerValue(p_lexerToken.IntegerValue)
	, FloatValue(p_lexerToken.FloatValue)
	, StringValue(p_lexerToken.StringValue) 
{ }
//----------------------------------------------------------------------------------------------
LexerToken& LexerToken::operator=(const LexerToken& p_lexerToken)
{
	Type = p_lexerToken.Type;
	BooleanValue = p_lexerToken.BooleanValue;
	IntegerValue = p_lexerToken.IntegerValue;
	FloatValue = p_lexerToken.FloatValue;
	StringValue = p_lexerToken.StringValue;
	return *this;
}
//----------------------------------------------------------------------------------------------
bool LexerToken::operator==(const LexerToken& p_lexerToken)
{
	if (Type != p_lexerToken.Type)
		return false;

	switch(Type)
	{
		case TT_BOOLEAN:
			return p_lexerToken.BooleanValue == BooleanValue;
		case TT_INTEGER:
			return p_lexerToken.IntegerValue == IntegerValue;
		case TT_FLOAT:
			return p_lexerToken.FloatValue == FloatValue;
		case TT_LITERAL:
		case TT_KEYWORD:
			return p_lexerToken.StringValue == StringValue;
		case TT_EOS:
		case TT_COMMA:
		case TT_EQUALS:
		case TT_LEFT_BRACE:
		case TT_LEFT_BRACKET:
		case TT_RIGHT_BRACE:
		case TT_RIGHT_BRACKET:
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
Meson::Common::Text::String LexerToken::LexerTokenToString(LexerToken::TokenType p_eTokenType)
{
	switch (p_eTokenType)
	{
		case TT_BOOLEAN:
			return "Boolean value";
		case TT_INTEGER:
			return "Integer value";
		case TT_FLOAT:
			return "Single-precision floating point value";
		case TT_LITERAL:
			return "String literal";
		case TT_KEYWORD:
			return "Keyword";
		case TT_EOS:
			return "End of stream";
		case TT_COMMA:
			return "Separator ','";
		case TT_EQUALS:
			return "Equals '='";
		case TT_LEFT_BRACE:
			return "Left brace '{'";
		case TT_LEFT_BRACKET:
			return "Left bracket ']'";
		case TT_RIGHT_BRACE:
			return "Right brace '}'";
		case TT_RIGHT_BRACKET:
			return "Right bracket ']'";
		default:
			return "Undefined";
	}
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
LexerToken Lexer::ExtractToken(void)
{
	LexerState state = LS_START;
	LexerToken lexerToken;

	char ch;
	String strKeyword;
	bool bNegative = false;
	int nInteger = 0;
	float fReal = 0.0f;
	float fDivisor = 1.0f;
	String strValue;

	while (!m_pInputStream->IsEndOfStream())
	{
		m_pInputStream->Read(&ch, 1);

		switch(state)
		{
		case LS_START:
			if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
				; // ingore whitespace
			else if (ch == '{')
			{
				lexerToken.Type = LexerToken::TT_LEFT_BRACE;
				lexerToken.StringValue = "{";
				return lexerToken;
			}
			else if (ch == '}')
			{
				lexerToken.Type = LexerToken::TT_RIGHT_BRACE;
				lexerToken.StringValue = "}";
				return lexerToken;
			}
			else if (ch == '[')
			{
				lexerToken.Type = LexerToken::TT_LEFT_BRACKET;
				lexerToken.StringValue = "[";
				return lexerToken;
			}
			else if (ch == ']')
			{
				lexerToken.Type = LexerToken::TT_RIGHT_BRACKET;
				lexerToken.StringValue = "]";
				return lexerToken;
			}
			else if (ch == '=')
			{
				lexerToken.Type = LexerToken::TT_EQUALS;
				lexerToken.StringValue = "=";
				return lexerToken;
			}
			else if (ch == ',')
			{
				lexerToken.Type = LexerToken::TT_COMMA;
				lexerToken.StringValue = ",";
				return lexerToken;
			}
			else if (ch == '/')
				state = LS_COMMENT;
			else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
			{
				strKeyword = ch;
				state = LS_KEYWORD;
			}
			else if (ch == '-')
			{
				bNegative = true;
				strValue = ch;
				state = LS_INTEGER;
			}
			else if (ch >= '0' && ch <= '9')
			{
				nInteger = ch - '0';
				state = LS_INTEGER;
			}
			else if (ch == '\"')
				state = LS_LITERAL;
			else
				throw new MesonException(
					"Parsing Error: Unrecognised token.", __FILE__, __LINE__);
			break;
		case LS_COMMENT:
			switch (ch)
			{
			case '/':
				state = LS_LINECOMMENT;
				break;
			case '*':
				state = LS_BLOCKCOMMENT;
				break;
			default:
				throw new MesonException(
					"Parsing error: '/' or '*' expected.", __FILE__, __LINE__);
			}
			break;
		case LS_LINECOMMENT:
			if (ch == '\r')
				state = LS_LINECOMMENT_ENDING;
			break;
		case LS_LINECOMMENT_ENDING:
			if (ch == '\n')
				state = LS_START;
			else
			{
				m_pInputStream->Pushback(&ch, 1);
				state = LS_START;
			}
			break;
		case LS_BLOCKCOMMENT:
			if (ch == '*')
				state = LS_BLOCKCOMMENT_ENDING;
			break;
		case LS_BLOCKCOMMENT_ENDING:
			if (ch == '/')
				state = LS_START;
			else
				state = LS_BLOCKCOMMENT;
			break;
		case LS_KEYWORD:
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'
				|| (ch >= '0' && ch <= '9'))
				strKeyword += ch;
			else
			{
				m_pInputStream->Pushback(&ch, 1);

				if (strKeyword == "true" || strKeyword == "false")
				{
					lexerToken.Type = LexerToken::TT_BOOLEAN;
					lexerToken.StringValue = strKeyword;
					lexerToken.BooleanValue = (strKeyword == "true");
					return lexerToken;				
				}
				else
				{
					lexerToken.Type = LexerToken::TT_KEYWORD;
					lexerToken.StringValue = strKeyword;
					return lexerToken;				
				}
			}
			break;
		case LS_INTEGER:
			if (ch >= '0' && ch <= '9')
			{
				nInteger = nInteger * 10 + (ch - '0');
			}
			else if (ch == '.')
			{
				fReal = (float) nInteger;
				state = LS_FLOAT;
			}
			else
			{
				m_pInputStream->Pushback(&ch, 1);
				if (bNegative)
					nInteger = -nInteger;
				return LexerToken(nInteger);
			}
			break;
		case LS_FLOAT:
			if (ch >= '0' && ch <= '9')
			{
				fDivisor = 0.1f;
				fReal += (float) (ch - '0') * fDivisor;
				state = LS_FRACTION;
			}
			else
				throw new MesonException(
					"Parsing Error: Digit expected after floating point.", __FILE__, __LINE__);
			break;
		case LS_FRACTION:
			if (ch >= '0' && ch <= '9')
			{
				fDivisor *= 0.1f;
				fReal += (float) (ch - '0') * fDivisor;
			}
			else
			{
				m_pInputStream->Pushback(&ch, 1);
				if (bNegative)
					fReal = -fReal;
				return LexerToken(fReal);
			}
			break;
		case LS_LITERAL:
			if (ch == '\"')
				return LexerToken(strValue, true);
			else if (ch == '\\')
				state = LS_ESCAPE;
			else if (ch == '\r' || ch == '\n')
				throw new MesonException(
					"Parsing Error: String cannot span across lines.", __FILE__, __LINE__);
			else
				strValue += ch;
			break;
		case LS_ESCAPE:
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
			state = LS_LITERAL;
			break;
		}
	}

	if (state != LS_START)
		throw new MesonException(
			"Parsing error: Unexpected end of stream.", __FILE__, __LINE__);

	lexerToken.Type = LexerToken::TT_EOS;
	return lexerToken;
}
//----------------------------------------------------------------------------------------------
Lexer::~Lexer()
{
	m_pInputStream = NULL;
	m_listBuffer.Clear();
}
//----------------------------------------------------------------------------------------------
Lexer::Lexer(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
	: m_pInputStream(p_pInputStream)
	, m_listBuffer()
{
}
//----------------------------------------------------------------------------------------------
LexerToken Lexer::PeekToken()
{
	if (m_listBuffer.Size() == 0)
		m_listBuffer.Add(ExtractToken());
	return m_listBuffer[0];
}
//----------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------
LexerToken Lexer::ReadToken(LexerToken::TokenType p_eLexerTokenType)
{
	LexerToken lexerToken = ReadToken();
	if (lexerToken.Type!= p_eLexerTokenType)
		throw new MesonException(
			"Parsing Error: Expected token of type: "
				+ LexerToken::LexerTokenToString(p_eLexerTokenType),
			__FILE__, __LINE__);
	return lexerToken;
}
//----------------------------------------------------------------------------------------------
void Lexer::ReadTokenList(LexerToken::TokenType p_eLexerTokenType, Meson::Common::Collections::TList<LexerToken>& p_listTokens)
{
	p_listTokens.Clear();

	ReadToken(LexerToken::TT_LEFT_BRACE);
	while (true)
	{
		LexerToken lexerToken = PeekToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			break;

		if (p_listTokens.Size() > 0)
			ReadToken(LexerToken::TT_COMMA);

		lexerToken = ReadToken(p_eLexerTokenType);

		p_listTokens.Add(lexerToken);
	}

	ReadToken(LexerToken::TT_RIGHT_BRACE);
}
//----------------------------------------------------------------------------------------------
LexerToken Lexer::ReadKeyword(const String& p_strKeyword)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strKeyword)
		throw new MesonException(
			"Parsing error: Expected identifier '" + p_strKeyword + "'",
			__FILE__, __LINE__);

	return lexerToken;
}
//----------------------------------------------------------------------------------------------
Vector2f Lexer::ReadVector2()
{
	Vector2f vecValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.X = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.Y = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return vecValue;
}
//----------------------------------------------------------------------------------------------
Vector3f Lexer::ReadVector3()
{
	Vector3f vecValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.X = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.Y = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.Z = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return vecValue;
}
//----------------------------------------------------------------------------------------------
Vector4f Lexer::ReadVector4()
{
	Vector4f vecValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.X = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.Y = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.Z = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	vecValue.W = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return vecValue;
}
//----------------------------------------------------------------------------------------------
Matrix2f Lexer::ReadMatrix2()
{
	Matrix2f matValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken;

	for (uint index = 0; index < matValue.Order - 1; index++)
	{
		lexerToken = ReadToken(LexerToken::TT_FLOAT);
		matValue.Element[index] = lexerToken.FloatValue;

		ReadToken(LexerToken::TT_COMMA);
	}

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	matValue.Element[matValue.Order-1] = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return matValue;
}
//----------------------------------------------------------------------------------------------
Matrix3f Lexer::ReadMatrix3()
{
	Matrix3f matValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken;

	for (uint index = 0; index < matValue.Order - 1; index++)
	{
		lexerToken = ReadToken(LexerToken::TT_FLOAT);
		matValue.Element[index] = lexerToken.FloatValue;

		ReadToken(LexerToken::TT_COMMA);
	}

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	matValue.Element[matValue.Order-1] = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return matValue;
}
//----------------------------------------------------------------------------------------------
Matrix4f Lexer::ReadMatrix4()
{
	Matrix4f matValue;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken;

	for (uint index = 0; index < matValue.Order - 1; index++)
	{
		lexerToken = ReadToken(LexerToken::TT_FLOAT);
		matValue.Element[index] = lexerToken.FloatValue;

		ReadToken(LexerToken::TT_COMMA);
	}

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	matValue.Element[matValue.Order-1] = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return matValue;
}
//----------------------------------------------------------------------------------------------
Colour Lexer::ReadColour()
{
	Colour colour;
	ReadToken(LexerToken::TT_LEFT_BRACE);

	LexerToken lexerToken = ReadToken(LexerToken::TT_FLOAT);
	colour.R = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	colour.G = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	colour.B = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_COMMA);

	lexerToken = ReadToken(LexerToken::TT_FLOAT);
	colour.A = lexerToken.FloatValue;

	ReadToken(LexerToken::TT_RIGHT_BRACE);

	return colour;
}
//----------------------------------------------------------------------------------------------
LexerToken Lexer::ReadProperty(const String& p_strPropertyName, LexerToken::TokenType p_eTokenType)
{
	ReadKeyword(p_strPropertyName);
	ReadToken(LexerToken::TT_EQUALS);
	LexerToken lexerToken = ReadToken(p_eTokenType);

	return lexerToken;
}
//----------------------------------------------------------------------------------------------
bool Lexer::ReadPropertyBoolean(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerToken::TT_BOOLEAN);
	return lexerToken.BooleanValue;
}
//----------------------------------------------------------------------------------------------
int Lexer::ReadPropertyInteger(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerToken::TT_INTEGER);
	return lexerToken.IntegerValue;
}
//----------------------------------------------------------------------------------------------
float Lexer::ReadPropertyFloat(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerToken::TT_FLOAT);
	return lexerToken.FloatValue;
}
//----------------------------------------------------------------------------------------------
String Lexer::ReadPropertyString(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadProperty(p_strPropertyName, LexerToken::TT_LITERAL);
	return lexerToken.StringValue;
}
//----------------------------------------------------------------------------------------------
Vector2f Lexer::ReadPropertyVector2(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadVector2();
}
//----------------------------------------------------------------------------------------------
Vector3f Lexer::ReadPropertyVector3(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadVector3();
}
//----------------------------------------------------------------------------------------------
Vector4f Lexer::ReadPropertyVector4(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadVector4();
}
//----------------------------------------------------------------------------------------------
Matrix2f Lexer::ReadPropertyMatrix2(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadMatrix2();
}
//----------------------------------------------------------------------------------------------
Matrix3f Lexer::ReadPropertyMatrix3(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadMatrix3();
}
//----------------------------------------------------------------------------------------------
Matrix4f Lexer::ReadPropertyMatrix4(const String& p_strPropertyName)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);
	if (lexerToken.StringValue != p_strPropertyName)
		throw new MesonException(
			"Parsing Error: Expected property: '" + p_strPropertyName + "'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_EQUALS);

	return ReadMatrix4();
}
//----------------------------------------------------------------------------------------------
void Lexer::ReadCustomProperties(IPropertySet& p_propertySet)
{
	LexerToken lexerToken = ReadToken(LexerToken::TT_KEYWORD);

	if (lexerToken.StringValue != "CustomProperties")
		throw new MesonException(
			"Parsing Error: Expected identifier 'CustomProperties'",
			__FILE__, __LINE__);

	ReadToken(LexerToken::TT_LEFT_BRACE);

	// get properties accepted by entity
	//THashMap<String, PropertyDescriptor> mapProperties;
	//p_propertySet.EnumeraterProperties(mapProperties);
	PropertyDescriptor propertyDescriptor; 

	// properties
	while (true)
	{
		lexerToken = ReadToken();

		// closing brace for 'Properties' container
		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD)
			throw new MesonException(
				"Parsing Error: Opening bracket or property name expected.",
				__FILE__, __LINE__);

		String strPropertyName = lexerToken.StringValue;

		// check if property is valid
		if (!p_propertySet.ContainsProperty(strPropertyName))
			throw new MesonException(
				"Unsupported property '" + strPropertyName + "'.",
				__FILE__, __LINE__);

		p_propertySet.GetPropertyDescriptor(strPropertyName, propertyDescriptor);

		// assignment
		ReadToken(LexerToken::TT_EQUALS);

		LexerToken lexerTokenValue;
		switch (propertyDescriptor.GetType())
		{
		case PropertyDescriptor::PT_BOOLEAN:
			lexerTokenValue = ReadToken(LexerToken::TT_BOOLEAN);
			p_propertySet.SetProperty(strPropertyName, lexerTokenValue.BooleanValue);
			break;
		case PropertyDescriptor::PT_INTEGER:
			lexerTokenValue = ReadToken(LexerToken::TT_INTEGER);
			p_propertySet.SetProperty(strPropertyName, lexerTokenValue.IntegerValue);
			break;
		case PropertyDescriptor::PT_REAL:
			lexerTokenValue = ReadToken(LexerToken::TT_FLOAT);
			p_propertySet.SetProperty(strPropertyName, lexerTokenValue.FloatValue);
			break;
		case PropertyDescriptor::PT_STRING:
			lexerTokenValue = ReadToken(LexerToken::TT_LITERAL);
			p_propertySet.SetProperty(strPropertyName, lexerTokenValue.StringValue);
			break;
		case PropertyDescriptor::PT_VECTOR2:
			p_propertySet.SetProperty(strPropertyName, ReadVector2());
			break;
		case PropertyDescriptor::PT_VECTOR3:
			p_propertySet.SetProperty(strPropertyName, ReadVector3());
			break;
		case PropertyDescriptor::PT_VECTOR4:
			p_propertySet.SetProperty(strPropertyName, ReadVector4());
			break;
		case PropertyDescriptor::PT_MATRIX2:
			p_propertySet.SetProperty(strPropertyName, ReadMatrix2());
			break;
		case PropertyDescriptor::PT_MATRIX3:
			p_propertySet.SetProperty(strPropertyName, ReadMatrix3());
			break;
		case PropertyDescriptor::PT_MATRIX4:
			p_propertySet.SetProperty(strPropertyName, ReadMatrix4());
			break;
		case PropertyDescriptor::PT_COLOUR:
			p_propertySet.SetProperty(strPropertyName, ReadColour());
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END