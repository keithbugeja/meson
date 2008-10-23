#include "GravityForceFactory.h"

#include "GrvGravitasEngine.h"
#include "GravityForce.h"

using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Force;

GravityForceFactory::GravityForceFactory(void)
	: m_strName("Gravity")
{
}

GravityForceFactory::~GravityForceFactory(void)
{
}

const String& GravityForceFactory::GetName(void)
{
	return m_strName;
}

ForcePtr GravityForceFactory::CreateForce(void)
{
	GravityForce* pGravityForce = new GravityForce();
	GravitasEngine::GetInstance()->Logger().Out << "Gravity force created.\n";
	return ForcePtr(pGravityForce);
}

ForcePtr GravityForceFactory::CreateForce(const String& p_strId)
{
	GravityForce* pGravityForce = new GravityForce(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Gravity force created with ID '" + p_strId + "'.\n";
	return ForcePtr(pGravityForce);
}
