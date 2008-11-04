//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

Meson_Vistas_BEGIN
	
	/** This class provides a basic read-only interface for representing 
		constants within shader programs.
	 */
	class VISTAS_API ShaderProgramConstant
	{
	public:
		/** The following list enumerates the data types that can be passed 
			to shader programs. Scalar quantities support arrays of up to 16 
			elements, vectors support arrays of up to 4 elements and matrices, 
			as is, do not support arrays.
		 */
		enum DataType 
		{
			DT_NONE,
			DT_BOOLEAN,
			DT_INTEGER,
			DT_FLOAT,
			DT_VECTOR,
			DT_MATRIX,
			DT_COUNT
		};
	
	protected:
		//----------------------------------------------------------------------------------------------
		/** Protected constructor for use by derived classes.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramConstant(void);

	public:
		//----------------------------------------------------------------------------------------------
		/** Returns the name of the shader constant.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetName(void) const;

		//----------------------------------------------------------------------------------------------
		/** Accessor methods for matrix, vector, float, integer and boolean elements. Arrays exist for 
			each fo the specified types. 
		 */
		//----------------------------------------------------------------------------------------------
		void GetMatrixArray(Meson::Common::Maths::Matrix4f* p_pMatrixData, uint p_uiCount);
		void GetMatrix(Meson::Common::Maths::Matrix4f& p_matrix, uint p_uiIndex);
		void GetMatrix(Meson::Common::Maths::Matrix4f& p_matrix);
		
		void GetVectorArray(Meson::Common::Maths::Vector4f* p_pVectorData, uint p_uiCount);
		void GetVector(Meson::Common::Maths::Vector4f& p_vector, uint p_uiIndex);
		void GetVector(Meson::Common::Maths::Vector4f& p_vector);
		
		void GetFloatArray(float* p_fFloatData, uint p_uiCount);
		float GetFloat(uint p_uiIndex);
		float GetFloat(void);
		
		void GetIntegerArray(int* p_nIntegerData, uint p_uiCount);
		int GetInteger(uint p_uiIndex);
		int GetInteger(void);
		
		void GetBooleanArray(bool* p_bBooleanData, uint p_uiCount);
		int GetBoolean(uint p_uiIndex);
		int GetBoolean(void);
		
		//----------------------------------------------------------------------------------------------
		/** Return the atomic data elements of the constant. Matrix and vector elements are given
			by calls to <b>GetFloatData</b>.
		 */
		//----------------------------------------------------------------------------------------------
		const float* GetFloatData(void);
		const bool* GetBooleanData(void);
		const int* GetIntegerData(void);

		//----------------------------------------------------------------------------------------------
		/** Return size and type information about the shader constant.
		 */
		//----------------------------------------------------------------------------------------------
		DataType GetType(void) const;
		uint GetSize(void) const;

		bool IsArray(void) const;
		bool IsScalar(void) const;
		bool IsInteger(void) const;
		bool IsBoolean(void) const;
		bool IsFloat(void) const;
		bool IsVector(void) const;
		bool IsMatrix(void) const;
	
		//----------------------------------------------------------------------------------------------
		/** The following constants determine the internal storage requirements of a single shader 
			constant. The internal buffer is shared by all the elements. The actual size of the buffer 
			is: MAX(MaxBoolean * sizeof(bool), MaxInteger * sizeof(int), MaxFloat * sizeof(float))
		 */
		//----------------------------------------------------------------------------------------------
		static const uint MaxBoolean = 128;
		static const uint MaxInteger = 128;
		static const uint MaxFloat = 128;
		static const uint MaxVector = MaxFloat / 4;
		static const uint MaxMatrix = MaxFloat / 16;

	protected:
		Meson::Common::Text::String m_strName;
		DataType m_dataType;

		uint m_uiSize;

		union
		{
			float m_fFloatData[MaxFloat];
			bool  m_bBoolData[MaxBoolean];
			int	  m_nIntData[MaxInteger];
		};
	};

Meson_Vistas_END