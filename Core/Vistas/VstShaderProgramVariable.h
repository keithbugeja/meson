//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstShaderProgramConstant.h"

Meson_Vistas_BEGIN
	
	/** This class extends the ShaderProgramConstant object, providing
		a read-write interface for representing shader constants.
	 */
	class VISTAS_API ShaderProgramVariable : public ShaderProgramConstant
	{
	protected:
		//----------------------------------------------------------------------------------------------
		/** Protected constructor, for use by derived classes.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramVariable(void);

	public:
		//----------------------------------------------------------------------------------------------
		/** Public constructors.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, int* p_nIntegerData, uint p_uiSize);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, int p_nIntegerValue);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, bool* p_bBooleanData, uint p_uiSize);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, bool p_bBooleanValue);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, float* p_fFloatData, uint p_uiSize);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, float p_fFloatValue);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, Meson::Common::Maths::Vector4f* p_pVectorData, uint p_uiSize);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, const Meson::Common::Maths::Vector4f& p_vectorValue);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, Meson::Common::Maths::Matrix4f* p_pMatrixData, uint p_uiSize);
		explicit ShaderProgramVariable(const Meson::Common::Text::String& p_strVariableName, const Meson::Common::Maths::Matrix4f& p_matrixValue);

		//----------------------------------------------------------------------------------------------
		/** Resets the variable type and size.
		 */
		//----------------------------------------------------------------------------------------------
		void Clear(void);

		//----------------------------------------------------------------------------------------------
		/** Setter methods.
		 */
		//----------------------------------------------------------------------------------------------
		void Set(const int* p_nIntegerData, uint p_uiSize);
		void Set(int p_nIntegerValue, uint p_uiIndex);
		void Set(int p_nIntegerValue);

		void Set(const bool* p_bBooleanData, uint p_uiSize);
		void Set(bool p_bBooleanValue, uint p_uiIndex);
		void Set(bool p_bBooleanValue);

		void Set(const float* p_fFloatData, uint p_uiSize);
		void Set(float p_nFloatValue, uint p_uiIndex);
		void Set(float p_nFloatValue);

		void Set(const Meson::Common::Maths::Vector4f* p_pVectorData, uint p_uiSize);
		void Set(const Meson::Common::Maths::Vector4f& p_vectorValue, uint p_uiIndex);
		void Set(const Meson::Common::Maths::Vector4f& p_vectorValue);

		void Set(const Meson::Common::Maths::Matrix4f* p_pMatrixData, uint p_uiSize);
		void Set(const Meson::Common::Maths::Matrix4f& p_matrixValue, uint p_uiIndex);
		void Set(const Meson::Common::Maths::Matrix4f& p_matrixValue);

		//----------------------------------------------------------------------------------------------
		/** Convenience setter methods which are provided to help avoid unnecessary casts and 
			conversions between Colour and Vector4f types. 
		 */
		//----------------------------------------------------------------------------------------------
		void Set(const Colour* p_pColourData, uint p_uiSize);
		void Set(const Colour& p_colourValue, uint p_uiIndex);
		void Set(const Colour& p_colourValue);
	};

Meson_Vistas_END
