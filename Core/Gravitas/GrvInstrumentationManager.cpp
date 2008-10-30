#include "GrvInstrumentationManager.h"

#include "GrvTransform.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Instrumentation;

Meson_Gravitas_Instrumentation_BEGIN

struct InstrumentationManagerState
{
	InstrumentationDevicePtr m_pInstrumentationDevice;
	uint m_unInstrumentationFlags;

	InstrumentationManagerState(void)
		: m_pInstrumentationDevice()
		, m_unInstrumentationFlags(InstrumentationFlags::None)
	{ }
};

InstrumentationFlags::InstrumentationFlags operator|(
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags1,
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags2)
{
	return (InstrumentationFlags::InstrumentationFlags)
		((uint) p_instrumentationFlags1 | (uint) p_instrumentationFlags2);
}

Meson_Gravitas_Instrumentation_END

InstrumentationManager::InstrumentationManager(void)
{
	m_pInstrumentationManagerState = new InstrumentationManagerState();
}

InstrumentationManager::~InstrumentationManager(void)
{
	delete m_pInstrumentationManagerState;
}

void InstrumentationManager::SetDevice(
	InstrumentationDevicePtr p_pInstrumentationDevice)
{
	m_pInstrumentationManagerState->m_pInstrumentationDevice
		= p_pInstrumentationDevice;
}

bool InstrumentationManager::IsEnabled(InstrumentationFlags::InstrumentationFlags p_instrumentationFlags) const
{
	return (m_pInstrumentationManagerState->m_unInstrumentationFlags & p_instrumentationFlags) != NULL;
}

void InstrumentationManager::Enable(
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags)
{
	m_pInstrumentationManagerState->m_unInstrumentationFlags |= p_instrumentationFlags;
}

void InstrumentationManager::Disable(
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags)
{
	m_pInstrumentationManagerState->m_unInstrumentationFlags &= !p_instrumentationFlags;
}

void InstrumentationManager::RenderInstrumentation(
	Environment* p_pEnvironment, Real p_rCurrentTime)
{
	IInstrumentationDevice* pInstrumentationDevice
		= m_pInstrumentationManagerState->m_pInstrumentationDevice;

	// do nothing if no device set
	if (pInstrumentationDevice == NULL)
		return;

	// reset transform
	pInstrumentationDevice->SetTransform(TMatrix4<Real>::Identity);

	// space, body and geometry instrumentation
	if (p_pEnvironment->Space != NULL)
	{
		// space instrumentation
		if (IsEnabled(InstrumentationFlags::Space))
		{
			pInstrumentationDevice->SetColour(Colour::White);
			p_pEnvironment->Space->RenderInstrumentation(
				pInstrumentationDevice);
		}

		// process bodies if bodies or geometry enabled
		if (IsEnabled(InstrumentationFlags::Bodies | InstrumentationFlags::Geometry))
		{
			TMatrix4<Real> matTransform;
			matTransform.MakeZero();

			// process all bodies
			TEnumerator<BodyPtr>& enumBodies = p_pEnvironment->Space->GetBodies();
			while(enumBodies.HasMoreElements())
			{
				BodyPtr pBody = enumBodies.NextElement();

				// body instrumentation
				if (IsEnabled(InstrumentationFlags::Bodies))
				{
					pInstrumentationDevice->SetColour(Colour::Cyan);
					pBody->RenderInstrumentation(pInstrumentationDevice);
				}

				// geometry instrumentation
				if (IsEnabled(InstrumentationFlags::Geometry))
				{
					// set device transform to allow rendering in body coords
					KineticProperties& kineticProperties = pBody->GetKineticProperties();
					Transform transform(kineticProperties.Orientation, kineticProperties.Position);
					transform.ExtractMatrix(matTransform);
					pInstrumentationDevice->SetTransform(matTransform);
					pInstrumentationDevice->SetColour(Colour::Yellow);
					pBody->GetGeometry()->RenderInstrumentation(pInstrumentationDevice);
				}

				// forces instrumentation
				if (IsEnabled(InstrumentationFlags::Forces))
				{
					pInstrumentationDevice->SetTransform(TMatrix4<Real>::Identity);
					pInstrumentationDevice->SetColour(Colour::Magenta);

					TImmutableList<ForcePtr>& listForces
						= pBody->GetForceAccumulator().GetForces();
					for (size_t unIndex = 0; unIndex < listForces.Size(); unIndex++)
					{
						ForcePtr pForce = listForces[unIndex];
						TVector3<Real> vecApplicationPointLocal = pForce->GetApplicationPoint(
							&pBody->GetMassProperties(), &pBody->GetKineticProperties(),
							pBody->GetGeometry(), p_rCurrentTime);

						TVector3<Real> vecApplicationPointWorld;
						pBody->GetKineticProperties().TransformPointToWorld(vecApplicationPointLocal, vecApplicationPointWorld);

						TVector3<Real> vecForceValue = pForce->GetForceValue(
							&pBody->GetMassProperties(), &pBody->GetKineticProperties(),
							pBody->GetGeometry(), p_rCurrentTime);

						pInstrumentationDevice->DrawArrow(
							vecApplicationPointWorld + vecForceValue, vecApplicationPointWorld);
					}
				}
			}

			pInstrumentationDevice->SetTransform(TMatrix4<Real>::Identity);
		}
	}

	// constraint instrumentation
	if (p_pEnvironment->Constraints != NULL
		&& IsEnabled(InstrumentationFlags::Constraints))
	{
		pInstrumentationDevice->SetColour(Colour::Red);

		for (size_t unIndex = 0;
			unIndex < p_pEnvironment->Constraints->Size();
			unIndex++)
		{
			ConstraintPtr pConstraint
				= (*p_pEnvironment->Constraints)[unIndex];
			pConstraint->RenderInstrumentation(pInstrumentationDevice);
		}
	}

	// kinetic integrator instrumentation
	if (p_pEnvironment->KineticIntegrator != NULL
		&& IsEnabled(InstrumentationFlags::KineticIntegrator))
		p_pEnvironment->KineticIntegrator->RenderInstrumentation(
			pInstrumentationDevice);

	// constraint solver instrumentation
	if (p_pEnvironment->ConstraintSolver != NULL
		&& IsEnabled(InstrumentationFlags::ConstraintSolver))
		p_pEnvironment->ConstraintSolver->RenderInstrumentation(
			pInstrumentationDevice);
}
