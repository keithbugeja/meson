//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Colour
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstColour.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
const ColourARGB ColourARGB::Black		= ColourARGB((ARGB)0x00000000);
const ColourARGB ColourARGB::White		= ColourARGB((ARGB)0x00FFFFFF);
const ColourARGB ColourARGB::Red		= ColourARGB((ARGB)0x00FF0000);
const ColourARGB ColourARGB::Green		= ColourARGB((ARGB)0x0000FF00);
const ColourARGB ColourARGB::Blue		= ColourARGB((ARGB)0x000000FF);
const ColourARGB ColourARGB::Cyan		= ColourARGB((ARGB)0x0000FFFF);
const ColourARGB ColourARGB::Magenta	= ColourARGB((ARGB)0x00FF00FF);
const ColourARGB ColourARGB::Yellow		= ColourARGB((ARGB)0x00FFFF00);

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
const ColourRGBA ColourRGBA::Black		= ColourRGBA((RGBA)0x00000000);
const ColourRGBA ColourRGBA::White		= ColourRGBA((RGBA)0xFFFFFF00);
const ColourRGBA ColourRGBA::Red		= ColourRGBA((RGBA)0xFF000000);
const ColourRGBA ColourRGBA::Green		= ColourRGBA((RGBA)0x00FF0000);
const ColourRGBA ColourRGBA::Blue		= ColourRGBA((RGBA)0x0000FF00);
const ColourRGBA ColourRGBA::Cyan		= ColourRGBA((RGBA)0x00FFFF00);
const ColourRGBA ColourRGBA::Magenta	= ColourRGBA((RGBA)0xFF00FF00);
const ColourRGBA ColourRGBA::Yellow		= ColourRGBA((RGBA)0xFFFF0000);
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
ColourARGB::ColourARGB(ARGB p_argb) 
{ 
	A = ((p_argb & 0xFF000000) >> 24) / 255.0f;
	R = ((p_argb & 0x00FF0000) >> 16) / 255.0f;
	G = ((p_argb & 0x0000FF00) >> 8) / 255.0f;
	B = ((p_argb & 0x000000FF)) / 255.0f;
}
//----------------------------------------------------------------------------------------------
ARGB ColourARGB::ToARGB(void) const
{
	return (ARGB)( (((byte)(A * 255)) << 24) + 
				   (((byte)(R * 255)) << 16) + 
				   (((byte)(G * 255)) << 8) + 
				   ((byte)(B * 255)) ); 
}
//----------------------------------------------------------------------------------------------
RGBA ColourARGB::ToRGBA(void) const
{
	return (RGBA)( (((byte)(R * 255)) << 24) + 
				   (((byte)(G * 255)) << 16) + 
				   (((byte)(B * 255)) << 8) + 
				   ((byte)(A * 255)) ); 
}
//----------------------------------------------------------------------------------------------
ColourARGB::ColourARGB(void)
	: A(1),
	  R(0), 
	  G(0), 
	  B(0)
{
}
//----------------------------------------------------------------------------------------------
ColourARGB::ColourARGB(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue) 
	: A(p_fAlpha), 
	  R(p_fRed), 
	  G(p_fGreen), 
	  B(p_fBlue)
{ 
}
//----------------------------------------------------------------------------------------------
ColourARGB::ColourARGB(const ColourARGB& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
}
//----------------------------------------------------------------------------------------------
ColourARGB::ColourARGB(const ColourRGBA& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
}
//----------------------------------------------------------------------------------------------
void ColourARGB::Set(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue)
{
	A = p_fAlpha; 
	R = p_fRed; 
	B = p_fBlue; 
	G = p_fGreen;
}
//----------------------------------------------------------------------------------------------
void ColourARGB::Set(const ColourARGB& p_colourARGB)
{
	A = p_colourARGB.A; 
	R = p_colourARGB.R; 
	G = p_colourARGB.G;
	B = p_colourARGB.B;
}
//----------------------------------------------------------------------------------------------
void ColourARGB::Set(const ColourRGBA& p_colourRGBA)
{
	A = p_colourRGBA.A; 
	R = p_colourRGBA.R; 
	G = p_colourRGBA.G;
	B = p_colourRGBA.B;
}
//----------------------------------------------------------------------------------------------
void ColourARGB::Set(ARGB p_argb)
{
	A = ((p_argb & 0xFF000000) >> 24) / 255.0f;
	R = ((p_argb & 0x00FF0000) >> 16) / 255.0f;
	G = ((p_argb & 0x0000FF00) >> 8) / 255.0f;
	B = ((p_argb & 0x000000FF)) / 255.0f;
}
//----------------------------------------------------------------------------------------------
bool ColourARGB::operator==(const ColourARGB& p_colour) const
{
	return (A == p_colour.A && R == p_colour.R && G == p_colour.G && B == p_colour.B);
}
//----------------------------------------------------------------------------------------------
ColourARGB& ColourARGB::operator=(const ColourARGB& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
	return *this;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
ColourRGBA::ColourRGBA(RGBA p_rgba) 
{ 
	R = ((p_rgba & 0xFF000000) >> 24) / 255.0f;
	G = ((p_rgba & 0x00FF0000) >> 16) / 255.0f;
	B = ((p_rgba & 0x0000FF00) >> 8) / 255.0f;
	A = ((p_rgba & 0x000000FF)) / 255.0f;
}
//----------------------------------------------------------------------------------------------
ARGB ColourRGBA::ToARGB(void) const
{
	return (ARGB)( (((byte)(A * 255)) << 24) + 
				   (((byte)(R * 255)) << 16) + 
				   (((byte)(G * 255)) << 8) + 
				   ((byte)(B * 255)) ); 
}
//----------------------------------------------------------------------------------------------
RGBA ColourRGBA::ToRGBA(void) const
{
	return (RGBA)( (((byte)(R * 255)) << 24) + 
				   (((byte)(G * 255)) << 16) + 
				   (((byte)(B * 255)) << 8) + 
				   ((byte)(A * 255)) ); 
}
//----------------------------------------------------------------------------------------------
ColourRGBA::ColourRGBA(void)
	: A(1),
	  R(0),
	  G(0),
	  B(0)
{
}
//----------------------------------------------------------------------------------------------
ColourRGBA::ColourRGBA(float p_fRed, float p_fGreen, float p_fBlue, float p_fAlpha) 
	: A(p_fAlpha), 
	  R(p_fRed), 
	  G(p_fGreen), 
	  B(p_fBlue)
{ 
}
//----------------------------------------------------------------------------------------------
ColourRGBA::ColourRGBA(const ColourRGBA& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
}
//----------------------------------------------------------------------------------------------
ColourRGBA::ColourRGBA(const ColourARGB& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
}
//----------------------------------------------------------------------------------------------
void ColourRGBA::Set(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue)
{
	A = p_fAlpha; 
	R = p_fRed; 
	B = p_fBlue; 
	G = p_fGreen;
}
//----------------------------------------------------------------------------------------------
void ColourRGBA::Set(const ColourARGB& p_colourARGB)
{
	A = p_colourARGB.A; 
	R = p_colourARGB.R; 
	G = p_colourARGB.G;
	B = p_colourARGB.B;
}
//----------------------------------------------------------------------------------------------
void ColourRGBA::Set(const ColourRGBA& p_colourRGBA)
{
	A = p_colourRGBA.A; 
	R = p_colourRGBA.R; 
	G = p_colourRGBA.G;
	B = p_colourRGBA.B;
}
//----------------------------------------------------------------------------------------------
void ColourRGBA::Set(RGBA p_rgba)
{
	R = ((p_rgba & 0xFF000000) >> 24) / 255.0f;
	G = ((p_rgba & 0x00FF0000) >> 16) / 255.0f;
	B = ((p_rgba & 0x0000FF00) >> 8) / 255.0f;
	A = ((p_rgba & 0x000000FF)) / 255.0f;
}
//----------------------------------------------------------------------------------------------
bool ColourRGBA::operator==(const ColourRGBA& p_colour) const
{
	return (A == p_colour.A && R == p_colour.R && G == p_colour.G && B == p_colour.B);
}
//----------------------------------------------------------------------------------------------
ColourRGBA& ColourRGBA::operator=(const ColourRGBA& p_colour)
{
	A = p_colour.A;
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
	return *this;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END