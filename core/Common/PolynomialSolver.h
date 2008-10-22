#pragma once

#include "Namespaces.h"
#include "Maths.h"
#include "Complex.h"
#include "List.h"
#include "ArrayList.h"

Meson_Common_Maths_Solvers_BEGIN

template<typename TReal>
class TPolynomialSolver
{
private: // private methods
	TPolynomialSolver(void);
	~TPolynomialSolver(void);

private: // private static methods
	static inline Meson::Common::Maths::TComplex<TReal> EvaluateFunction(
		const Meson::Common::Collections::TList<TReal> &p_listCoefficients,
		Meson::Common::Maths::TComplex<TReal> p_cpxValue)
	{
		Meson::Common::Maths::TComplex<TReal> cpxX = (TReal)1.0;
		Meson::Common::Maths::TComplex<TReal> cpxResult = (TReal)0.0; 

		size_t unOrder =  p_listCoefficients.Size();
		for (size_t unIndex = 0; unIndex < unOrder; unIndex++)
		{
			cpxResult += p_listCoefficients[unIndex] * cpxX;
			cpxX *= p_cpxValue;
		}

		return cpxResult;
	}

public: // public static methods

	// solve Ax + B = 0
	static inline void SolveLinear(TReal p_rA, TReal p_rB, TReal &p_rSolution)
	{
		p_rSolution = -p_rB / p_rA;
	}

	// solve Ax^2 + Bx + C = 0 for real roots only
	static inline bool SolveQuadratic(TReal p_rA, TReal p_rB, TReal p_rC,
		TReal &p_rSolution1, TReal &p_rSolution2)
	{
		TReal rDiscriminant = p_rB * p_rB - (TReal)4.0 * p_rA * p_rC;
		if (rDiscriminant < 0.0) return false;

		TReal rDiscriminantSqrt = TMaths<TReal>::Sqrt(rDiscriminant);
		TReal rInverse2A = (TReal)0.5 / p_rA;

		p_rSolution1 = (-p_rB - rDiscriminantSqrt) * rInverse2A;
		p_rSolution2 = (-p_rB + rDiscriminantSqrt) * rInverse2A;

		return true;
	}

	// solve Ax^2 + Bx + C = 0 for any roots
	static inline void SolveQuadratic(TReal p_rA, TReal p_rB, TReal p_rC,
		Meson::Common::Maths::TComplex<TReal> &p_cpxSolution1,
		Meson::Common::Maths::TComplex<TReal> &p_cpxSolution2)
	{
		Meson::Common::Maths::TComplex<TReal> cpxDiscriminant
			= p_rB * p_rB - (TReal)4.0 * p_rA * p_rC;

		Meson::Common::Maths::TComplex<TReal> cpxDiscriminantSqrt
			= cpxDiscriminant ^ (TReal) 0.5;

		TReal rInverse2A = (TReal)0.5 / p_rA;

		p_cpxSolution1 = (-p_rB - cpxDiscriminantSqrt) * rInverse2A;
		p_cpxSolution2 = (-p_rB + cpxDiscriminantSqrt) * rInverse2A;
	}

	// solve Ax^3 + Bx^2 + Cx + D = 0 for any roots
	static inline void SolveCubic(TReal p_rA, TReal p_rB, TReal p_rC, TReal p_rD,
		Meson::Common::Maths::TComplex<TReal> &p_cpxSolution1,
		Meson::Common::Maths::TComplex<TReal> &p_cpxSolution2,
		Meson::Common::Maths::TComplex<TReal> &p_cpxSolution3)
	{
		TReal rA2 = p_rA * p_rA; 
		TReal rB2 = p_rB * p_rB;

		// q = (9abc - 27a^2d - 2b^3) / 54a^3
		TReal rQ = (TReal)9.0 * p_rA * p_rB * p_rC;
		rQ -= (TReal)27.0 * rA2 * p_rD;
		rQ -= (TReal)2.0 * rB2 * p_rB;
		rQ /= (TReal)54.0 * rA2 * p_rA;

		// r
		Meson::Common::Maths::TComplex<TReal> cpxR;
		cpxR.I = (TReal)0.0;
		cpxR.R = (TReal)3.0 * p_rA * p_rC - rB2;
		cpxR.R /= (TReal)9.0 * rA2;
		cpxR.R += rQ * rQ;
		cpxR.R = TMaths<TReal>::Sqrt(cpxR.R);

		TReal rCubeRoot = (TReal)(1.0/3.0);

		// s
		Meson::Common::Maths::TComplex<TReal> cpxS = (rQ + cpxR) ^ rCubeRoot;

		// t
		Meson::Common::Maths::TComplex<TReal> cpxT = (rQ - cpxR) ^ rCubeRoot;

		// solutions 1
		TReal rBover3A = p_rB / ((TReal)3.0 * p_rA);
		p_cpxSolution1 = cpxS + cpxT - rBover3A;

		// solutions 2 and 3
		TReal rRoot3on2 = (TReal)0.5 * TMaths<TReal>::Pow((TReal)3.0, (TReal)0.5);
		Meson::Common::Maths::TComplex<TReal> cpxPart1
			= (TReal)(-0.5) * (cpxS + cpxT) - rBover3A;
		Meson::Common::Maths::TComplex<TReal> cpxPart2
			= rRoot3on2 * (cpxS - cpxT) * Meson::Common::Maths::TComplex<TReal>::ImaginaryOne;

		p_cpxSolution2 = cpxPart1 + cpxPart2;
		p_cpxSolution3 = cpxPart1 - cpxPart2;
	}

	// generic solver using Durand-Kerner method
	static inline void SolveGeneric(
		const Meson::Common::Collections::TList<TReal> &p_listCoefficients,
		Meson::Common::Collections::TList<Meson::Common::Maths::TComplex<TReal>> &p_listSolutions,
		uint p_unMaxIterations = 10)
	{
		size_t unCount =  p_listCoefficients.Size();
		if (unCount == 0) return;

		// normalise polynomial
		Meson::Common::Collections::TArrayList<Real> listDepressedCoefficients;
		TReal rLeadingCoefficient = p_listCoefficients[unCount - 1];
		for (size_t unIndex = 0; unIndex < unCount; unIndex++)
			listDepressedCoefficients.Add(p_listCoefficients[unIndex] / rLeadingCoefficient);

		// initialise root values and prepare next values
		size_t unOrder = unCount - 1;
		TComplex<Real> cpxMultiplier;
		cpxMultiplier.R = (Real)4.0;
		cpxMultiplier.I = (Real)9.0;
		TComplex<Real> cpxSeed = Meson::Common::Maths::TComplex<Real>::RealOne;

		p_listSolutions.Clear();
		Meson::Common::Collections::TArrayList<Meson::Common::Maths::TComplex<TReal>> listNextValues;
		Meson::Common::Collections::TArrayList<bool> listStable;
		for (size_t unIndex = 0; unIndex < unOrder; unIndex++)
		{
			p_listSolutions.Add(cpxSeed);
			cpxSeed *= cpxMultiplier;
			listNextValues.Add(Meson::Common::Maths::TComplex<Real>::Zero);
			listStable.Add(false);
		}

		// iterate until all solutions are stable
		bool bStable = false;
		while (!bStable && p_unMaxIterations-- > 0)
		{
			// assume all solutions stable in this iteration
			bStable = true;
	
			// process all current solution approximations
			for (size_t unIndex = 0; unIndex < unOrder; unIndex++)
			{
				// skip this solution if already stable
				if (listStable[unIndex]) continue;

				// get current solution approximation
				Meson::Common::Maths::TComplex<Real> cpxCurrentValue
					= p_listSolutions[unIndex];

				// build divisor consisting of factors, each a difference of
				// the current approximation and the other solution approximations
				Meson::Common::Maths::TComplex<Real> cpxDenominator
					= Meson::Common::Maths::TComplex<Real>::RealOne;
				for (size_t unIndex2 = 0; unIndex2 < unOrder; unIndex2++)
				{
					if (unIndex2 == unIndex) continue;
					cpxDenominator *= (cpxCurrentValue - p_listSolutions[unIndex2]);
				}

				// if divisor is zero, current approximation is actually a good solution
				//if (cpxDenominator.IsZero()) continue;

				// compute numerator by evaluating polynomial for current approximation
				Meson::Common::Maths::TComplex<Real> cpxNumerator
					= EvaluateFunction(listDepressedCoefficients, cpxCurrentValue);

				// new approx is old approx - numberator / denominator
				listNextValues[unIndex] = p_listSolutions[unIndex] - cpxNumerator / cpxDenominator;

				if (listNextValues[unIndex] == p_listSolutions[unIndex])
					// mark this solution as stable
					listStable[unIndex] = true;
				else
					// otherwise, some solutions are still unstable
					bStable = false;
			}

			// replace old approx by new appox
			for (size_t unIndex = 0; unIndex < unOrder; unIndex++)
				p_listSolutions[unIndex] = listNextValues[unIndex];
		}
	}
};

typedef TPolynomialSolver<double> PolynomialSolver;
typedef TPolynomialSolver<float> PolynomialSolverf;

Meson_Common_Maths_Solvers_END
