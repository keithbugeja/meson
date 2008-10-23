//----------------------------------------------------------------------------------------------
//  ShaderProgramConstant
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//	OPTIMISATION: Replace type sizes by constants, avoiding recalculation for every copy.
//----------------------------------------------------------------------------------------------
#include "VstShaderProgramVariable.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
ShaderProgramConstant::ShaderProgramConstant(void)
	: m_dataType(ShaderProgramConstant::DT_NONE),
	  m_uiSize(0)
{
}
//----------------------------------------------------------------------------------------------
const String& ShaderProgramConstant::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetMatrixArray(Matrix4f* p_pMatrixData, uint p_uiCount)
{
	MESON_ASSERT(IsMatrix() && IsArray(), "Variable is not a matrix array");
	MESON_ASSERT(p_uiCount < m_uiSize, "Count out of range");

	memcpy(p_pMatrixData, m_fFloatData, sizeof(Matrix4f) * p_uiCount);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetMatrix(Matrix4f& p_matrix, uint p_uiIndex)
{
	MESON_ASSERT(IsVector() && IsArray(), "Variable is not a matrix array");
	MESON_ASSERT(p_uiIndex < m_uiSize, "Index out of range");

	memcpy(p_matrix.Element, ((byte*)m_fFloatData) + p_uiIndex * sizeof(Matrix4f), sizeof(Matrix4f));
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetMatrix(Matrix4f& p_matrix)
{
	MESON_ASSERT(IsMatrix(), "Variable is not a matrix");

	memcpy(p_matrix.Element, m_fFloatData, sizeof(Matrix4f));
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetVectorArray(Vector4f* p_pVectorData, uint p_uiCount)
{
	MESON_ASSERT(IsVector() && IsArray(), "Variable is not a vector array");
	MESON_ASSERT(p_uiCount < m_uiSize, "Count out of range");

	memcpy(p_pVectorData, m_fFloatData, sizeof(Vector4f) * p_uiCount);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetVector(Vector4f& p_vector, uint p_uiIndex)
{
	MESON_ASSERT(IsVector() && IsArray(), "Variable is not a vector array");
	MESON_ASSERT(p_uiIndex < m_uiSize, "Index out of range");

	memcpy(p_vector.Element, ((byte*)m_fFloatData) + p_uiIndex * sizeof(Vector4f), sizeof(Vector4f));
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetVector(Vector4f& p_vector)
{
	MESON_ASSERT(IsVector(), "Variable is not a vector");

	memcpy(p_vector.Element, m_fFloatData, sizeof(Vector4f));
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetFloatArray(float* p_fFloatData, uint p_uiCount)
{
	MESON_ASSERT(IsFloat() && IsArray(), "Variable is not a float array");
	MESON_ASSERT(p_uiCount < m_uiSize, "Count out of range");

	memcpy(p_fFloatData, m_fFloatData, sizeof(float) * p_uiCount);
}
//----------------------------------------------------------------------------------------------
float ShaderProgramConstant::GetFloat(uint p_uiIndex)
{
	MESON_ASSERT(IsFloat(), "Variable is not a float or float array");
	MESON_ASSERT(p_uiIndex < m_uiSize, "Index out of range");

	return m_fFloatData[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
float ShaderProgramConstant::GetFloat(void)
{
	MESON_ASSERT(IsFloat(), "Variable is not a float or float array");

	return m_fFloatData[0];
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetIntegerArray(int* p_nIntegerData, uint p_uiCount)
{
	MESON_ASSERT(IsInteger() && IsArray(), "Variable is not an integer array");
	MESON_ASSERT(p_uiCount < m_uiSize, "Count out of range");

	memcpy(p_nIntegerData, m_nIntData, sizeof(int) * p_uiCount);
}
//----------------------------------------------------------------------------------------------
int ShaderProgramConstant::GetInteger(uint p_uiIndex)
{
	MESON_ASSERT(IsInteger(), "Variable is not an integer or integer array");
	MESON_ASSERT(p_uiIndex < m_uiSize, "Index out of range");

	return m_nIntData[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
int ShaderProgramConstant::GetInteger(void)
{
	MESON_ASSERT(IsInteger(), "Variable is not an integer or integer array");

	return m_nIntData[0];
}
//----------------------------------------------------------------------------------------------
void ShaderProgramConstant::GetBooleanArray(bool* p_bBooleanData, uint p_uiCount)
{
	MESON_ASSERT(IsBoolean() && IsArray(), "Variable is not a boolean array");
	MESON_ASSERT(p_uiCount < m_uiSize, "Count out of range");

	memcpy(p_bBooleanData, m_bBoolData, sizeof(bool) * p_uiCount);
}
//----------------------------------------------------------------------------------------------
int ShaderProgramConstant::GetBoolean(uint p_uiIndex)
{
	MESON_ASSERT(IsBoolean(), "Variable is not a boolean or boolean array");
	MESON_ASSERT(p_uiIndex < m_uiSize, "Index out of range");

	return m_bBoolData[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
int ShaderProgramConstant::GetBoolean(void)
{
	MESON_ASSERT(IsBoolean(), "Variable is not a boolean or boolean array");

	return m_bBoolData[0];
}
//----------------------------------------------------------------------------------------------
const float* ShaderProgramConstant::GetFloatData(void) 
{
	return m_fFloatData;
}
//----------------------------------------------------------------------------------------------
const bool* ShaderProgramConstant::GetBooleanData(void)
{
	return m_bBoolData;
}
//----------------------------------------------------------------------------------------------
const int* ShaderProgramConstant::GetIntegerData(void)
{
	return m_nIntData;
}
//----------------------------------------------------------------------------------------------
ShaderProgramConstant::DataType ShaderProgramConstant::GetType(void) const
{
	return m_dataType;
}
//----------------------------------------------------------------------------------------------
uint ShaderProgramConstant::GetSize(void) const
{
	return m_uiSize;
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsArray(void) const
{
	return (m_uiSize > 1);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsScalar(void) const
{
	return !(IsArray() || IsVector() || IsMatrix());
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsInteger(void) const
{
	return (m_dataType == ShaderProgramConstant::DT_INTEGER);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsBoolean(void) const
{
	return (m_dataType == ShaderProgramConstant::DT_BOOLEAN);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsFloat(void) const
{
	return (m_dataType == ShaderProgramConstant::DT_FLOAT);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsVector(void) const
{
	return (m_dataType == ShaderProgramConstant::DT_VECTOR);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramConstant::IsMatrix(void) const
{
	return (m_dataType == ShaderProgramConstant::DT_MATRIX);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END