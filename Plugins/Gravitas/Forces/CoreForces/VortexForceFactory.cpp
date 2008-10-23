#include "VortexForceFactory.h"

#include "GrvGravitasEngine.h"
#include "VortexForce.h"

using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Force;

VortexForceFactory::VortexForceFactory(void)
	: m_strName("Vortex")
{
}

VortexForceFactory::~VortexForceFactory(void)
{
}

const String& VortexForceFactory::GetName(void)
{
	return m_strName;
}

ForcePtr VortexForceFactory::CreateForce(void)
{
	VortexForce* pVortexForce = new VortexForce();
	GravitasEngine::GetInstance()->Logger().Out << "Vortex force created.\n";
	return ForcePtr(pVortexForce);
}

ForcePtr VortexForceFactory::CreateForce(const String& p_strId)
{
	VortexForce* pVortexForce = new VortexForce(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Vortex force created with ID '" + p_strId + "'.\n";
	return ForcePtr(pVortexForce);
}
