//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Colour
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	typedef dword ARGB;
	typedef dword RGBA;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class VISTAS_API ColourARGB
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Colour object constructors
		 */
		//----------------------------------------------------------------------------------------------
		ColourARGB(void);
		ColourARGB(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue);
		ColourARGB(const ColourARGB& p_colour);
		ColourARGB(const ColourRGBA& p_colour);
		ColourARGB(ARGB p_argb);

		//----------------------------------------------------------------------------------------------
		/** Quick setter methods
		 */
		//----------------------------------------------------------------------------------------------
		void Set(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue);
		void Set(const ColourARGB& p_colourARGB);
		void Set(const ColourRGBA& p_colourRGBA);
		void Set(ARGB p_argb);

		//----------------------------------------------------------------------------------------------
		/** Conversion methods
		 */ 
		//----------------------------------------------------------------------------------------------
		ARGB ToARGB(void) const;
		RGBA ToRGBA(void) const;

		//----------------------------------------------------------------------------------------------
		/** Comparison operators
		 */
		//----------------------------------------------------------------------------------------------
		bool operator==(const ColourARGB& p_colour) const;
		ColourARGB& operator=(const ColourARGB& p_colour);

		//----------------------------------------------------------------------------------------------
		/** Dual-access to colour components
		 */
		//----------------------------------------------------------------------------------------------
		union 
		{
			float Component[4];
			struct
			{
				float A, R, G, B;
			};
		};

		//----------------------------------------------------------------------------------------------
		/** Pre-defined colour values
		 */
		//----------------------------------------------------------------------------------------------
		static const ColourARGB Black;
		static const ColourARGB White;
		static const ColourARGB Red;
		static const ColourARGB Green;
		static const ColourARGB Blue;
		static const ColourARGB Cyan;
		static const ColourARGB Magenta;
		static const ColourARGB Yellow;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class VISTAS_API ColourRGBA
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Colour object constructors
		 */
		//----------------------------------------------------------------------------------------------
		ColourRGBA(void);
		ColourRGBA(float p_fRed, float p_fGreen, float p_fBlue, float p_fAlpha);
		ColourRGBA(const ColourRGBA& p_colour);
		ColourRGBA(const ColourARGB& p_colour);
		ColourRGBA(RGBA p_rgba);

		//----------------------------------------------------------------------------------------------
		/** Quick setter methods
		 */
		//----------------------------------------------------------------------------------------------
		void Set(float p_fAlpha, float p_fRed, float p_fGreen, float p_fBlue);
		void Set(const ColourARGB& p_colourARGB);
		void Set(const ColourRGBA& p_colourRGBA);
		void Set(RGBA p_rgba);

		//----------------------------------------------------------------------------------------------
		/** Conversion methods
		 */ 
		//----------------------------------------------------------------------------------------------
		ARGB ToARGB(void) const;
		RGBA ToRGBA(void) const;
		
		//----------------------------------------------------------------------------------------------
		/** Comparison operators
		 */
		//----------------------------------------------------------------------------------------------
		bool operator==(const ColourRGBA& p_colour) const;
		ColourRGBA& operator=(const ColourRGBA& p_colour);

		//----------------------------------------------------------------------------------------------
		/** Dual-access to colour components
		 */
		//----------------------------------------------------------------------------------------------
		union 
		{
			float Component[4];
			struct
			{
				float R, G, B, A;
			};
		};

		//----------------------------------------------------------------------------------------------
		/** Pre-defined colour values
		 */
		//----------------------------------------------------------------------------------------------
		static const ColourRGBA Black;
		static const ColourRGBA White;
		static const ColourRGBA	Red;
		static const ColourRGBA	Green;
		static const ColourRGBA	Blue;
		static const ColourRGBA	Cyan;
		static const ColourRGBA	Magenta;
		static const ColourRGBA	Yellow;
	};

	typedef ColourRGBA Colour;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END