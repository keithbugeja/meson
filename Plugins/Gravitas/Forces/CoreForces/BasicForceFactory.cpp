#include "BasicForceFactory.h"

#include "GrvGravitasEngine.h"
#include "BasicForce.h"

using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Force;

BasicForceFactory::BasicForceFactory(void)
	: m_strName("Basic")
{
}

BasicForceFactory::~BasicForceFactory(void)
{
}

const String& BasicForceFactory::GetName(void)
{
	return m_strName;
}

ForcePtr BasicForceFactory::CreateForce(void)
{
	BasicForce* pBasicForce = new BasicForce();
	GravitasEngine::GetInstance()->Logger().Out << "Basic force created.\n";
	return ForcePtr(pBasicForce);
}

ForcePtr BasicForceFactory::CreateForce(const String& p_strId)
{
	BasicForce* pBasicForce = new BasicForce(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Basic force created with ID '" + p_strId + "'.\n";
	return ForcePtr(pBasicForce);
}
