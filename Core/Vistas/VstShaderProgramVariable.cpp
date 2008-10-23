#include "VstShaderProgramVariable.h"
#include "VstColour.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(void)
{
	Clear();
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName)
{ 
	m_strName = p_strVariableName;
	Clear();
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Clear(void)
{
	m_dataType = ShaderProgramConstant::DT_NONE;
	m_uiSize = 0;
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, int* p_nIntegerData, uint p_uiSize)
{
	m_strName = p_strVariableName;
	Set(p_nIntegerData, p_uiSize);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, int p_nIntegerValue)
{
	m_strName = p_strVariableName;
	Set(p_nIntegerValue);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, bool* p_bBooleanData, uint p_uiSize)
{
	m_strName = p_strVariableName;
	Set(p_bBooleanData, p_uiSize);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, bool p_bBooleanValue)
{
	m_strName = p_strVariableName;
	Set(p_bBooleanValue);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, float* p_fFloatData, uint p_uiSize)
{
	m_strName = p_strVariableName;
	Set(p_fFloatData, p_uiSize);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, float p_fFloatValue)
{
	m_strName = p_strVariableName;
	Set(p_fFloatValue);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, Vector4f* p_pVectorData, uint p_uiSize)
{
	m_strName = p_strVariableName;
	Set(p_pVectorData, p_uiSize);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, const Vector4f& p_vectorValue)
{
	m_strName = p_strVariableName;
	Set(p_vectorValue);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, Matrix4f* p_pMatrixData, uint p_uiSize)
{
	m_strName = p_strVariableName;
	Set(p_pMatrixData, p_uiSize);
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable::ShaderProgramVariable(const String& p_strVariableName, const Matrix4f& p_matrixValue)
{
	m_strName = p_strVariableName;
	Set(p_matrixValue);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const int* p_nIntegerData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxInteger, "Array size out of bounds");

	memcpy(m_nIntData, p_nIntegerData, sizeof(int) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_INTEGER;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(int p_nIntegerValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxInteger, "Array index out of bounds");
	m_nIntData[p_uiIndex] = p_nIntegerValue;
	
	if (m_dataType != ShaderProgramConstant::DT_INTEGER)
	{
		m_dataType = ShaderProgramConstant::DT_INTEGER;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex + 1, m_uiSize);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(int p_nIntegerValue)
{
	m_nIntData[0] = p_nIntegerValue;
	m_dataType = ShaderProgramConstant::DT_INTEGER;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const bool* p_bBooleanData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxBoolean, "Array size out of bounds");

	memcpy(m_bBoolData, p_bBooleanData, sizeof(bool) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_BOOLEAN;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(bool p_bBooleanValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxBoolean, "Array index out of bounds");
	m_bBoolData[p_uiIndex] = p_bBooleanValue;

	if (m_dataType != ShaderProgramConstant::DT_BOOLEAN)
	{
		m_dataType = ShaderProgramConstant::DT_BOOLEAN;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex + 1, m_uiSize);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(bool p_bBooleanValue)
{
	m_bBoolData[0] = p_bBooleanValue;
	m_dataType = ShaderProgramConstant::DT_BOOLEAN;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const float* p_fFloatData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxFloat, "Array size out of bounds");

	memcpy(m_fFloatData, p_fFloatData, sizeof(float) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_FLOAT;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(float p_nFloatValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxFloat, "Array size out of bounds");
	m_fFloatData[p_uiIndex] = p_nFloatValue;

	if (m_dataType != ShaderProgramConstant::DT_FLOAT)
	{
		m_dataType = ShaderProgramConstant::DT_FLOAT;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex + 1, m_uiSize);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(float p_nFloatValue)
{
	m_fFloatData[0] = p_nFloatValue;
	m_dataType = ShaderProgramConstant::DT_FLOAT;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Vector4f* p_pVectorData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxVector, "Array size out of bounds");

	memcpy(m_fFloatData, p_pVectorData, sizeof(Vector4f) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_VECTOR;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Vector4f& p_vectorValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxVector, "Array index out of bounds");

	uint uiBaseIndex = p_uiIndex * 4;
	m_fFloatData[uiBaseIndex++] = p_vectorValue.X;
	m_fFloatData[uiBaseIndex++] = p_vectorValue.Y;
	m_fFloatData[uiBaseIndex++] = p_vectorValue.Z;
	m_fFloatData[uiBaseIndex++] = p_vectorValue.W;

	if (m_dataType != ShaderProgramConstant::DT_VECTOR)
	{
		m_dataType = ShaderProgramConstant::DT_VECTOR;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex + 1, m_uiSize);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Vector4f& p_vectorValue)
{
	m_fFloatData[0] = p_vectorValue.X;
	m_fFloatData[1] = p_vectorValue.Y;
	m_fFloatData[2] = p_vectorValue.Z;
	m_fFloatData[3] = p_vectorValue.W;

	m_dataType = ShaderProgramConstant::DT_VECTOR;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Matrix4f* p_pMatrixData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxMatrix, "Array size out of bounds");

	memcpy(m_fFloatData, p_pMatrixData, sizeof(Matrix4f) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_MATRIX;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Matrix4f& p_matrixValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxMatrix, "Array index out of bounds");
	memcpy(((byte*)m_fFloatData) + p_uiIndex * sizeof(Matrix4f), p_matrixValue.Element, sizeof(Matrix4f));

	if (m_dataType != ShaderProgramConstant::DT_MATRIX)
	{
		m_dataType = ShaderProgramConstant::DT_MATRIX;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex, m_uiSize);
		
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Matrix4f& p_matrixValue)
{
	memcpy(m_fFloatData, p_matrixValue.Element, sizeof(Matrix4f));

	m_dataType = ShaderProgramConstant::DT_MATRIX;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Colour* p_pColourData, uint p_uiSize)
{
	MESON_ASSERT(p_uiSize > 0 && p_uiSize <= ShaderProgramConstant::MaxVector, "Array size out of bounds");

	memcpy(m_fFloatData, p_pColourData, sizeof(Colour) * p_uiSize);
	m_dataType = ShaderProgramConstant::DT_VECTOR;
	m_uiSize = p_uiSize;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Colour& p_colourValue, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < ShaderProgramConstant::MaxVector, "Array index out of bounds");

	uint uiBaseIndex = p_uiIndex * 4;
	m_fFloatData[uiBaseIndex++] = p_colourValue.R;
	m_fFloatData[uiBaseIndex++] = p_colourValue.G;
	m_fFloatData[uiBaseIndex++] = p_colourValue.B;
	m_fFloatData[uiBaseIndex++] = p_colourValue.A;

	if (m_dataType != ShaderProgramConstant::DT_VECTOR)
	{
		m_dataType = ShaderProgramConstant::DT_VECTOR;
		m_uiSize = p_uiIndex + 1;
	}
	else
		m_uiSize = MAX(p_uiIndex + 1, m_uiSize);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramVariable::Set(const Colour& p_colourValue)
{
	m_fFloatData[0] = p_colourValue.R;
	m_fFloatData[1] = p_colourValue.G;
	m_fFloatData[2] = p_colourValue.B;
	m_fFloatData[3] = p_colourValue.A;

	m_dataType = ShaderProgramConstant::DT_VECTOR;
	m_uiSize = 1;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
