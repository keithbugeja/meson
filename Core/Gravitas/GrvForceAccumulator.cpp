#include "GrvForceAccumulator.h"

#include "LinkedList.h"

Meson_Gravitas_Force_BEGIN

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Geometry;

struct ForceAccumulatorState
{
	MassProperties* m_pMassProperties;
	KineticProperties* m_pKineticProperties;
	GeometryPtr m_pGeometry;
	TLinkedList<ForcePtr> m_listForces;
	IVectorTimeFunction* m_pForce;
	IVectorTimeFunction* m_pTorque;
	IVectorTimeFunction* m_pLinearAcceleration;
	IVectorTimeFunction* m_pAngularAcceleration;

	ForceAccumulatorState(
		MassProperties* p_pMassProperties,
		KineticProperties* p_pKineticProperties,
		GeometryPtr p_pGeometry)
		: m_pMassProperties(p_pMassProperties)
		, m_pKineticProperties(p_pKineticProperties)
		, m_pGeometry(p_pGeometry)
		, m_listForces()
		, m_pForce(NULL)
		, m_pTorque(NULL)
		, m_pLinearAcceleration(NULL)
		, m_pAngularAcceleration(NULL)
	{}

	void ComputeTotalForce(Real p_rTime, TVector3<Real> &p_vecTotalForce)
	{
		p_vecTotalForce.MakeZero();

		TEnumerator<ForcePtr> &enumForces
			= m_listForces.GetEnumerator();
		while (enumForces.HasMoreElements())
		{
			ForcePtr pForce = enumForces.NextElement();
			p_vecTotalForce += pForce->GetForceValue(
				m_pMassProperties, m_pKineticProperties, m_pGeometry, p_rTime);
		}
	}

	void ComputeTotalTorque(Real p_rTime, TVector3<Real> &p_vecTotalTorque)
	{
		p_vecTotalTorque.MakeZero();

		TQuaternion<Real>& qtnOrientation = m_pKineticProperties->Orientation;
		TQuaternion<Real>& qtnOrientationConjugate
			= m_pKineticProperties->OrientationConjugate;

		TEnumerator<ForcePtr>& enumForces
			= m_listForces.GetEnumerator();
		while (enumForces.HasMoreElements())
		{
			ForcePtr pForce = enumForces.NextElement();

			// transform offset to world coordinates
			TVector3<Real> vecOffsetBody
				= pForce->GetApplicationPoint(
					m_pMassProperties, m_pKineticProperties, m_pGeometry, p_rTime);
			TVector3<Real> vecOffset
				= (qtnOrientation * vecOffsetBody * qtnOrientationConjugate).Vector;

			p_vecTotalTorque += (vecOffset
				^ pForce->GetForceValue(
					m_pMassProperties, m_pKineticProperties, m_pGeometry, p_rTime));
		}
	}
};

class ForceFunction
	: public IVectorTimeFunction
{
public: // public variables
	ForceAccumulatorState *m_pForceAccumulatorState;
	TVector3<Real> m_vecValue;
public: // public methods
	ForceFunction(ForceAccumulatorState *p_pForceAccumulatorState)
		: m_pForceAccumulatorState(p_pForceAccumulatorState)
		, m_vecValue() {}

	const TVector3<Real> &operator()(Real p_rTime)
	{
		m_pForceAccumulatorState->ComputeTotalForce(p_rTime, m_vecValue);
		return m_vecValue;
	}
};

class TorqueFunction
	: public IVectorTimeFunction
{
public: // public variables
	ForceAccumulatorState *m_pForceAccumulatorState;
	TVector3<Real> m_vecValue;
public: // public methods
	TorqueFunction(ForceAccumulatorState *p_pForceAccumulatorState)
		: m_pForceAccumulatorState(p_pForceAccumulatorState)
		, m_vecValue() {}

	TVector3<Real> &operator()(Real p_rTime)
	{
		m_pForceAccumulatorState->ComputeTotalTorque(p_rTime, m_vecValue);
		return m_vecValue;
	}
};

class LinearAccelerationFunction
	: public IVectorTimeFunction
{
public: // public variables
	ForceAccumulatorState *m_pForceAccumulatorState;
	TVector3<Real> m_vecValue;
public: // public methods
	LinearAccelerationFunction(ForceAccumulatorState *p_pForceAccumulatorState)
		: m_pForceAccumulatorState(p_pForceAccumulatorState)
		, m_vecValue() {}

	const TVector3<Real> &operator()(Real p_rTime)
	{
		m_pForceAccumulatorState->ComputeTotalForce(p_rTime, m_vecValue);
		m_vecValue *= m_pForceAccumulatorState->m_pMassProperties->MassInverse;
		return m_vecValue;
	}
};

class AngularAccelerationFunction
	: public IVectorTimeFunction
{
public: // public variables
	ForceAccumulatorState *m_pForceAccumulatorState;
	TVector3<Real> m_vecValue;
public: // public methods
	AngularAccelerationFunction(ForceAccumulatorState *p_pForceAccumulatorState)
		: m_pForceAccumulatorState(p_pForceAccumulatorState)
		, m_vecValue() {}

	TVector3<Real> &operator()(Real p_rTime)
	{
		m_pForceAccumulatorState->ComputeTotalTorque(p_rTime, m_vecValue);
		m_vecValue = m_pForceAccumulatorState->m_pMassProperties->InertiaTensorWorldInverse * m_vecValue;
		return m_vecValue;
	}
};

ForceAccumulator::ForceAccumulator(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry)
	: m_pForceAccumulatorState(NULL)
{
	m_pForceAccumulatorState = new ForceAccumulatorState(p_pMassProperties, p_pKineticProperties, p_pGeometry);

	m_pForceAccumulatorState->m_pForce = new ForceFunction(m_pForceAccumulatorState);
	m_pForceAccumulatorState->m_pTorque = new TorqueFunction(m_pForceAccumulatorState);

	m_pForceAccumulatorState->m_pLinearAcceleration = new LinearAccelerationFunction(m_pForceAccumulatorState);
	m_pForceAccumulatorState->m_pAngularAcceleration = new AngularAccelerationFunction(m_pForceAccumulatorState);
}

ForceAccumulator::~ForceAccumulator(void)
{
	m_pForceAccumulatorState->m_listForces.Clear();
	delete m_pForceAccumulatorState->m_pAngularAcceleration;
	delete m_pForceAccumulatorState->m_pLinearAcceleration;

	delete m_pForceAccumulatorState;
}

TImmutableList<ForcePtr> &ForceAccumulator::GetForces(void) const
{
	return m_pForceAccumulatorState->m_listForces.AsImmutableList();
}

void ForceAccumulator::ClearForces(void)
{
	m_pForceAccumulatorState->m_pKineticProperties->Active = true;
	m_pForceAccumulatorState->m_listForces.Clear();
}

void ForceAccumulator::ClearInstantaneousForces(void)
{
	for (size_t unIndex = 0; unIndex < m_pForceAccumulatorState->m_listForces.Size();)
	{
		IForce* pForce = m_pForceAccumulatorState->m_listForces(unIndex);
		if (pForce->IsInstantaneous())
			m_pForceAccumulatorState->m_listForces.RemoveAt(unIndex);
		else
			unIndex++;
	}
}

void ForceAccumulator::AddForce(ForcePtr p_pForce)
{
	m_pForceAccumulatorState->m_pKineticProperties->Active = true;
	m_pForceAccumulatorState->m_listForces.Add(p_pForce);
}

void ForceAccumulator::RemoveForce(ForcePtr p_pForce)
{
	m_pForceAccumulatorState->m_pKineticProperties->Active = true;
	m_pForceAccumulatorState->m_listForces.Remove(p_pForce);
}

Meson::Gravitas::Kinetics::IVectorTimeFunction& ForceAccumulator::GetForce(void)
{
	return *m_pForceAccumulatorState->m_pForce;
}

Meson::Gravitas::Kinetics::IVectorTimeFunction& ForceAccumulator::GetTorque(void)
{
	return *m_pForceAccumulatorState->m_pTorque;
}

IVectorTimeFunction& ForceAccumulator::GetLinearAcceleration(void)
{
	return *m_pForceAccumulatorState->m_pLinearAcceleration;
}

IVectorTimeFunction& ForceAccumulator::GetAngularAcceleration(void)
{
	return *m_pForceAccumulatorState->m_pAngularAcceleration;
}

Meson_Gravitas_Force_END
