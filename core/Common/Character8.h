//----------------------------------------------------------------------------------------------
//	Meson::Common::Character8
//	Character processing utility.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		14/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "String8.h"

Meson_Common_Text_BEGIN

class Character8
{
public: // public enums
	enum Type
	{
		Control,
		Whitespace,
		Symbol,
		Digit,
		Uppercase,
		Lowercase,
		Extended
	};

public: // public variables
	char Value;

public: // public methods
	// basic classification functions
	static Type GetType(char p_chValue);
	static bool IsControl(char p_chValue);
	static bool IsWhitespace(char p_chValue);
	static bool IsSymbol(char p_chValue);
	static bool IsUppercaseLetter(char p_chValue);
	static bool IsDigit(char p_chValue);
	static bool IsLowercaseLetter(char p_chValue);
	static bool IsExtended(char p_chValue);

	// combined classification functions
	static bool IsLetter(char p_chValue);
	static bool IsLetterOrDigit(char p_chValue);
	static bool IsPrintable(char p_chValue);

	Character8(void);
	Character8(const Character8& p_character);
	Character8(char p_chValue);
	~Character8(void);

	Character8& operator=(const Character8& p_character);
	Character8& operator=(char p_chValue);
	bool operator==(const Character8& p_character);
	operator char(void);
	operator Meson::Common::Text::String (void);

	// classificaion methods
	Type GetType(void);
	bool IsControl(void);
	bool IsWhitespace(void);
	bool IsSymbol(void);
	bool IsUppercaseLetter(void);
	bool IsDigit(void);
	bool IsLowercaseLetter(void);
	bool IsExtended(void);
	bool IsLetter(void);
	bool IsLetterOrDigit(void);
	bool IsPrintable(void);
};

typedef Character8 Character;

Meson_Common_Text_END
