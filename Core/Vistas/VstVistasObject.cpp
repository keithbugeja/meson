#include "VstVistasObject.h"
#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VistasObject::VistasObject(void)
	: m_strId(VistasEngine::GetInstance()->GenerateGuid())
	, m_userDataMap()
{
}
//----------------------------------------------------------------------------------------------
VistasObject::VistasObject(bool p_bAutogenerateId)
	: m_strId()
	, m_userDataMap()
{
	if (p_bAutogenerateId) 
		m_strId = VistasEngine::GetInstance()->GenerateGuid();
}
//----------------------------------------------------------------------------------------------
VistasObject::VistasObject(const Meson::Common::Text::String& p_strId)
	: m_strId(p_strId)
	, m_userDataMap()
{ 
}
//----------------------------------------------------------------------------------------------
VistasObject::~VistasObject(void)
{
}
//----------------------------------------------------------------------------------------------
const String& VistasObject::GetId(void) const 
{ 
	return m_strId;
}
//----------------------------------------------------------------------------------------------
void VistasObject::SetId(const String& p_strId) 
{ 
	m_strId = p_strId; 
}
//----------------------------------------------------------------------------------------------
const VistasType& VistasObject::GetStaticType(void) 
{
	static VistasType ls_strType = "VistasObject";
	return ls_strType;
}
//----------------------------------------------------------------------------------------------
const VistasType& VistasObject::GetBaseType(void) const
{
	return VistasObject::GetStaticType();
}
//----------------------------------------------------------------------------------------------
const VistasType& VistasObject::GetInstanceType(void) const 
{ 
	return VistasObject::GetStaticType();
}
//----------------------------------------------------------------------------------------------
bool VistasObject::IsDerived(const VistasType& p_type) const
{
	return (VistasObject::GetStaticType().Compare(p_type) == 0);
}
//----------------------------------------------------------------------------------------------
bool VistasObject::IsExact(const VistasType& p_type) const
{
	return (VistasObject::GetStaticType().Compare(p_type) == 0);
}
//----------------------------------------------------------------------------------------------
bool VistasObject::IsDerivedTypeOf(const VistasObject* p_pObject) const
{
	return (VistasObject::GetStaticType().Compare(p_pObject->GetInstanceType()) == 0);
}
//----------------------------------------------------------------------------------------------
bool VistasObject::IsExactTypeOf(const VistasObject* p_pObject) const
{
	return (VistasObject::GetStaticType().Compare(p_pObject->GetInstanceType()) == 0);
}
//----------------------------------------------------------------------------------------------
TPointer<VistasObject> VistasObject::GetObjectPtr(void)
{
	return VistasObjectPtr((VistasObject*)this);
}
//----------------------------------------------------------------------------------------------
VistasObjectPtr VistasObject::Clone(bool p_bShallow)
{
	throw new MesonException("Object does not support Clone method.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
VistasObjectPtr VistasObject::CreateInstance(void)
{
	throw new MesonException("Object does not support CreateInstance method.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void VistasObject::SetUserData(void* p_pData) 
{ 
	SetUserData(0, p_pData);
}
//----------------------------------------------------------------------------------------------
void VistasObject::SetUserData(uint p_uiSlot, void* p_pData) 
{ 
	m_userDataMap.Insert(p_uiSlot, p_pData); 
}
//----------------------------------------------------------------------------------------------
void* VistasObject::GetUserData(void) const 
{ 
	return GetUserData(0);
}
//----------------------------------------------------------------------------------------------
void* VistasObject::GetUserData(uint p_uiSlot) const 
{ 
	void* pUserData = NULL;

	if (m_userDataMap.ContainsKey(p_uiSlot, pUserData))
		return pUserData;

	return NULL;
}
//----------------------------------------------------------------------------------------------
uint VistasObject::GetUserDataCount(void) const 
{ 
	return (uint)m_userDataMap.Size(); 
}
//----------------------------------------------------------------------------------------------
UserDataEnumerator VistasObject::GetUserDataEnumerator() 
{ 
	return m_userDataMap.GetValueEnumerator(); 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END

