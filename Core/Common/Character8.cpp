//----------------------------------------------------------------------------------------------
//	Meson::Common::Character8
//	Character processing utility.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		14/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "Character8.h"

using namespace Meson::Common::Text;

Character8::Type Character8::GetType(char p_chValue)
{
	if (p_chValue == ' '
		|| p_chValue == '\t'
		|| p_chValue == '\n'
		|| p_chValue == '\r')
		return Character8::Symbol;

	if (p_chValue < 0x20 || p_chValue == 0x7F)
		return Character8::Control;

	if 	(p_chValue >= 'A' && p_chValue <= 'Z')
		return Character8::Uppercase;

	if 	(p_chValue >= '0' && p_chValue <= '9')
		return Character8::Digit;

	if 	(p_chValue >= 'a' && p_chValue <= 'z')
		return Character8::Lowercase;

	if (p_chValue >= 0x80)
		return Character8::Extended;

	return Character8::Symbol;
}

bool Character8::IsControl(char p_chValue)
{
	if (IsWhitespace(p_chValue)) return false;
	return p_chValue < 0x20 || p_chValue == 0x7F;
}

bool Character8::IsWhitespace(char p_chValue)
{
	return p_chValue == ' '
		|| p_chValue == '\t'
		|| p_chValue == '\n'
		|| p_chValue == '\r';
}

bool Character8::IsSymbol(char p_chValue)
{
	return (p_chValue >= 0x21 && p_chValue <= 0x2F)
		|| (p_chValue >= 0x3A && p_chValue <= 0x40)
		|| (p_chValue >= 0x5B && p_chValue <= 0x60)
		|| (p_chValue >= 0x7B && p_chValue <= 0x7E);
}

bool Character8::IsUppercaseLetter(char p_chValue)
{
	return p_chValue >= 'A' && p_chValue <= 'Z';
}

bool Character8::IsDigit(char p_chValue)
{
	return p_chValue >= '0' && p_chValue <= '9';
}

bool Character8::IsLowercaseLetter(char p_chValue)
{
	return p_chValue >= 'a' && p_chValue <= 'z';
}

bool Character8::IsExtended(char p_chValue)
{
	return p_chValue >= 0x80;
}

bool Character8::IsLetter(char p_chValue)
{
	return IsUppercaseLetter(p_chValue)
		|| IsLowercaseLetter(p_chValue);
}

bool Character8::IsLetterOrDigit(char p_chValue)
{
	return IsUppercaseLetter(p_chValue)
		|| IsLowercaseLetter(p_chValue)
		|| IsDigit(p_chValue);
}

bool Character8::IsPrintable(char p_chValue)
{
	Type type = GetType(p_chValue);
	return type != Character8::Control
		&& type != Character8::Extended;
}

Character8::Character8(void)
	: Value('\0')
{
}

Character8::Character8(const Character8& p_character)
	: Value(p_character.Value)
{
}

Character8::Character8(char p_chValue)
	: Value(p_chValue)
{
}

Character8::~Character8(void)
{
}

Character8& Character8::operator=(const Character8& p_character)
{
	Value = p_character.Value;
	return *this;
}

Character8& Character8::operator=(char p_chValue)
{
	Value = p_chValue;
	return *this;
}

bool Character8::operator==(const Character8& p_character)
{
	return Value == p_character.Value;
}

Character8::operator char(void)
{
	return Value;
}

Character8::operator String (void)
{
	return String(Value);
}

Character8::Type Character8::GetType(void)
{
	return Character8::GetType(Value);
}

bool Character8::IsControl(void)
{
	return Character8::IsControl(Value);
}

bool Character8::IsWhitespace(void)
{
	return Character8::IsWhitespace(Value);
}

bool Character8::IsSymbol(void)
{
	return Character8::IsSymbol(Value);
}

bool Character8::IsUppercaseLetter(void)
{
	return Character8::IsUppercaseLetter(Value);
}

bool Character8::IsDigit(void)
{
	return Character8::IsDigit(Value);
}

bool Character8::IsLowercaseLetter(void)
{
	return Character8::IsLowercaseLetter(Value);
}

bool Character8::IsExtended(void)
{
	return Character8::IsExtended(Value);
}

bool Character8::IsLetter(void)
{
	return Character8::IsLetter(Value);
}

bool Character8::IsLetterOrDigit(void)
{
	return Character8::IsLetterOrDigit(Value);
}

bool Character8::IsPrintable(void)
{
	return Character8::IsPrintable(Value);
}
