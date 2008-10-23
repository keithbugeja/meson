#include "BasicSimulatorFactory.h"

#include "BasicSimulator.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Simulation;

BasicSimulatorFactory::BasicSimulatorFactory(void)
	: m_strName("Basic")
{
}

BasicSimulatorFactory:: ~BasicSimulatorFactory(void)
{
}

const String &BasicSimulatorFactory::GetName(void)
{
	return m_strName;
}

SimulatorPtr BasicSimulatorFactory::CreateSimulator(void)
{
	BasicSimulator* pBasicSimulator = new BasicSimulator();
	GravitasEngine::GetInstance()->Logger().Out << "Basic simulator created.\n";
	return SimulatorPtr(pBasicSimulator);
}

SimulatorPtr BasicSimulatorFactory::CreateSimulator(const String& p_strId)
{
	BasicSimulator* pBasicSimulator = new BasicSimulator(p_strId);
	GravitasEngine::GetInstance()->Logger().Out << "Basic simulator created with ID '" + p_strId + "'.\n";
	return SimulatorPtr(pBasicSimulator);
}
