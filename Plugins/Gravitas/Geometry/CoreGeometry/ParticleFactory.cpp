#include "ParticleFactory.h"
#include "Particle.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

ParticleFactory::ParticleFactory(void)
	: m_strName("Particle")
{
}

ParticleFactory:: ~ParticleFactory(void)
{
}

const String& ParticleFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr ParticleFactory::CreateGeometry(void)
{
	Particle* pParticle = new Particle();
	GravitasEngine::GetInstance()->Logger().Out << "Particle geometry created.\n";
	return GeometryPtr(pParticle);
}

GeometryPtr ParticleFactory::CreateGeometry(const String& p_strId)
{
	Particle* pParticle = new Particle(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Particle geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pParticle);
}
