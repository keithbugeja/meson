#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"
#include "String8.h"
#include "Sorting.h"
#include "ArrayList.h"

Meson_Common_Maths_Solvers_BEGIN

template<typename TReal>
class TLPTerm
{
public: // public variables
	Meson::Common::Text::String m_strVariable;
	TReal m_rCoefficient;

public: // public methods
	TLPTerm(void)
		: m_strVariable("")
		, m_rCoefficient((TReal)0.0)
	{
	}

	TLPTerm(const Meson::Common::Text::String &p_strVariable,
		TReal p_rCoefficient)
		: m_strVariable(p_strVariable)
		, m_rCoefficient(p_rCoefficient)
	{
	}

	TLPTerm &operator=(const TLPTerm &lpTerm)
	{
		m_strVariable = lpTerm.m_strVariable;
		m_rCoefficient = lpTerm.m_rCoefficient;
		return *this;
	}

	bool operator==(const TLPTerm &lpTerm) const
	{
		return m_strVariable == lpTerm.m_strVariable;
	}

	bool operator<(const TLPTerm &lpTerm) const
	{
		return m_strVariable < lpTerm.m_strVariable;
	};

	bool IsSlack(void)
	{
		return m_strVariable.Substring(0, 1) == "s";
	}

	bool IsSolution(void)
	{
		return m_strVariable.Substring(0, 1) == "x";
	}

	bool IsArtificial(void)
	{
		return m_strVariable.Substring(0, 1) == "w";
	}

	Meson::Common::Text::String ToString(void) const
	{
		Meson::Common::Text::String strValue = "";

		if (Meson::Common::Maths::TMaths<TReal>::Equals(m_rCoefficient, (TReal) 1.0))
			; // do nothing
		else if (Meson::Common::Maths::TMaths<TReal>::Equals(m_rCoefficient, (TReal) -1.0))
		{
			strValue += "-";
		}
		else
		{
			strValue += m_rCoefficient;
			strValue += " ";
		}

		strValue += m_strVariable;
		return strValue;
	}
};

template<typename TReal>
class TLPEquation
{
public: // public variables
	Meson::Common::Text::String m_strLeftTerm;
	TReal m_rRightConstant;
	Meson::Common::Collections::TArrayList<TLPTerm<TReal>> m_listRightTerms;

private: // private methods

public:
	TLPEquation(void)
		: m_strLeftTerm("")
		, m_rRightConstant((TReal) 0.0)
		, m_listRightTerms()
	{
	}

	TLPEquation &operator=(const TLPEquation &p_lpEquation)
	{
		m_strLeftTerm = p_lpEquation.m_strLeftTerm;
		m_rRightConstant = p_lpEquation.m_rRightConstant;

		m_listRightTerms.Clear();
		for (size_t unIndex = 0; unIndex < p_lpEquation.m_listRightTerms.Size(); unIndex++)
			m_listRightTerms.Add(p_lpEquation.m_listRightTerms(unIndex));
			
		return *this;
	}

	bool operator==(const TLPEquation &p_lpEquation) const
	{
		if (m_strLeftTerm != p_lpEquation.m_strLeftTerm) return false;
		
		return true;
	}

	int IndexOfTerm(const Meson::Common::Text::String& p_strRightTerm)
	{
		TLPTerm<TReal> lpTermDummy = TLPTerm<TReal>(p_strRightTerm, (TReal) 0.0);
		return m_listRightTerms.IndexOf(lpTermDummy);
	}

	void AddRightTerm(TReal p_rCoefficient)
	{
		TLPTerm<TReal> lpTerm;
		lpTerm.m_strVariable = "x";
		lpTerm.m_strVariable += ((int) m_listRightTerms.Size() + 1);
		lpTerm.m_rCoefficient = p_rCoefficient;
		m_listRightTerms.Add(lpTerm);
	}

	void CollectTerms(void)
	{
		// nothing to collect if less than 2 RHS terms
		if (m_listRightTerms.Size() < 2) return;

		// sort RHS terms
		Meson::Common::Collections::TSorting<TLPTerm<TReal>>::QuickSort(m_listRightTerms);

		for (int nIndex = (int) m_listRightTerms.Size() - 2; nIndex >= 0; nIndex--)
		{
			TLPTerm<TReal> &lpTermThis = m_listRightTerms((size_t) nIndex);
			TLPTerm<TReal> &lpTermNext = m_listRightTerms((size_t) nIndex + 1);

			// do nothing if current term different from next
			if (lpTermThis.m_strVariable != lpTermNext.m_strVariable)
				continue;

			// otherwise collapse terms
			lpTermThis.m_rCoefficient += lpTermNext.m_rCoefficient;
			m_listRightTerms.RemoveAt((size_t) nIndex + 1);
		}
	}

	void EliminateZeroTerms(void)
	{
		if (m_listRightTerms.Size() == 0) return;

		// eliminate zero terms
		for (int nIndex = (int) m_listRightTerms.Size() - 1; nIndex >= 0; nIndex--)
			if (Meson::Common::Maths::TMaths<TReal>::Equals(
				m_listRightTerms((size_t) nIndex).m_rCoefficient, (TReal) 0.0))
				m_listRightTerms.RemoveAt((size_t) nIndex);
	}

	void EliminateArtificialTerms(void)
	{
		if (m_listRightTerms.Size() == 0) return;

		// eliminate artificial terms
		for (int nIndex = (int) m_listRightTerms.Size() - 1; nIndex >= 0; nIndex--)
			if (m_listRightTerms((size_t) nIndex).IsArtificial())
				m_listRightTerms.RemoveAt((size_t) nIndex);
	}


	void Normalise(void)
	{
		// collect terms
		CollectTerms();

		// eliminate zero terms
		EliminateZeroTerms();

		// see if LHS term also occurs on RHS
		int nIndexCommon = IndexOfTerm(m_strLeftTerm);

		// do nothing else if only on LHS
		if (nIndexCommon < 0) return;

		// otherwise, get matching RHS term 
		TLPTerm<TReal> lpTermCommon = m_listRightTerms((size_t) nIndexCommon);

		// calculate inverse of LHS coefficient after collecting LHS counterpart
		TReal rFactor = (TReal) 1.0 / ((TReal) 1.0 - lpTermCommon.m_rCoefficient);

		// remove RHS term
		m_listRightTerms.RemoveAt(nIndexCommon);

		// multiply all RHS terms and constant by inverse coefficient to keep LHS coefficient as 1
		m_rRightConstant *= rFactor;
		for (size_t unIndex = 0; unIndex < m_listRightTerms.Size(); unIndex++)
			m_listRightTerms(unIndex).m_rCoefficient *= rFactor;
	}

	void SolveForVariable(const Meson::Common::Text::String &p_strVariable)
	{
		if (m_strLeftTerm == p_strVariable)
		{
			Normalise();
			return;
		}

		m_listRightTerms.Add(TLPTerm<TReal>(p_strVariable, (TReal) 1.0));
		m_listRightTerms.Add(TLPTerm<TReal>(m_strLeftTerm, (TReal) -1.0));
		m_strLeftTerm = p_strVariable;
		Normalise();
	}

	void Substitute(const TLPEquation<TReal> &p_lpEquation)
	{
		Normalise();

		// do nothing if term to substitute is LHS
		if (m_strLeftTerm == p_lpEquation.m_strLeftTerm)
			return;

		// otherwise, substitute matching term in RHS

		// try to match term
		int nIndex = IndexOfTerm(p_lpEquation.m_strLeftTerm);

		// do nothing if no match
		if (nIndex < 0) return;

		// get coefficient of term to substitute
		TReal rCoefficient = m_listRightTerms((size_t) nIndex).m_rCoefficient;

		// remove term
		m_listRightTerms.RemoveAt((size_t) nIndex);

		// insert RHS of substitute multiplied by old term's coefficient
		m_rRightConstant += p_lpEquation.m_rRightConstant * rCoefficient;

		for (size_t unIndex = 0; unIndex < p_lpEquation.m_listRightTerms.Size(); unIndex++)
		{
			TLPTerm<TReal> lpTerm = p_lpEquation.m_listRightTerms(unIndex);
			lpTerm.m_rCoefficient *= rCoefficient;
			m_listRightTerms.Add(lpTerm);
		}

		Normalise();
	}

	Meson::Common::Text::String ToString(void) const
	{
		Meson::Common::Text::String strValue = m_strLeftTerm + " = " + m_rRightConstant;
		for (size_t nIndex = 0; nIndex < m_listRightTerms.Size(); nIndex++)
		{
			TLPTerm<TReal> lpTerm = m_listRightTerms(nIndex);
			if (lpTerm.m_rCoefficient > (TReal) 0.0)
				strValue += " + " + lpTerm.ToString();
			else
			{
				lpTerm.m_rCoefficient = -lpTerm.m_rCoefficient;
				strValue += " - " + lpTerm.ToString();
			}
		}
		return strValue;
	}
};

template<typename TReal>
class TLPTableau
{
public:
	TLPEquation<TReal> m_lpEquationObjective;
	Meson::Common::Collections::TArrayList<TLPEquation<TReal>> m_listConstraints;

private:
	int FindOptimisableTerm(void)
	{
		for (size_t unIndex = 0; unIndex < m_lpEquationObjective.m_listRightTerms.Size(); unIndex++)
		{
			TLPTerm<TReal> &lpTerm = m_lpEquationObjective.m_listRightTerms[unIndex];
			if (lpTerm.m_rCoefficient > Meson::Common::Maths::TMaths<TReal>::Epsilon)
				return (int) unIndex;
		}

		return -1;
	}

	int FindLimitingConstraint(const Meson::Common::Text::String& p_strVariable)
	{
		int nConstraintIndex = -1;
		TReal rBestRatio = Meson::Common::Maths::TMaths<TReal>::Maximum;

		// process all constraint equations
		for (size_t unIndex = 0; unIndex < m_listConstraints.Size(); unIndex++)
		{
			TLPEquation<TReal> &lpEquationConstraint = m_listConstraints(unIndex);

			// find corresponding term in equation
			int nTermIndex = lpEquationConstraint.IndexOfTerm(p_strVariable);

			// if none found, can treat as zero coeff i.e. not a limiting constraint 
			if (nTermIndex < 0) continue;

			// get matching term
			TLPTerm<TReal> &lpTerm = lpEquationConstraint.m_listRightTerms((size_t) nTermIndex);

			// if coeff > 0, not a limiting constraint
			if (lpTerm.m_rCoefficient > (TReal) 0.0) continue;

			// compute limiting ratio
			TReal rRatio = -lpEquationConstraint.m_rRightConstant / lpTerm.m_rCoefficient;

			// keep if best
			if (rRatio < rBestRatio)
			{
				nConstraintIndex = (int) unIndex;
				rBestRatio = rRatio;
			}
		}

		return nConstraintIndex;
	}

	void TransposeVariables(int nObjectiveTermIndex, size_t unConstraintIndex)
	{
		// get constraint equation to use as substitute
		TLPEquation<TReal> lpEquationSubstitute = m_listConstraints(unConstraintIndex);

		// solve substitute equation for limiting term
		lpEquationSubstitute.SolveForVariable(
			m_lpEquationObjective.m_listRightTerms(nObjectiveTermIndex).m_strVariable);

		// substitute into objective function
		m_lpEquationObjective.Substitute(lpEquationSubstitute);

		// substitute into other constraints
		for (size_t unIndex = 0; unIndex < m_listConstraints.Size(); unIndex++)
		{
			// skip self
			if (unIndex == unConstraintIndex) continue;

			m_listConstraints(unIndex).Substitute(lpEquationSubstitute);
		}

		// remove substitute constraint
		m_listConstraints.RemoveAt(unConstraintIndex);

		// insert new constraint
		m_listConstraints.Add(lpEquationSubstitute);
	}

public:
	TLPTableau(void)
		: m_lpEquationObjective()
		, m_listConstraints()
	{
	}

	bool Optimise(void)
	{
		while(true)
		{
			int nOptimisableTerm = FindOptimisableTerm();

			// stop if no more terms to optimise
			if (nOptimisableTerm < 0) return true;

			int nLimitingConstraint = FindLimitingConstraint(
				m_lpEquationObjective.m_listRightTerms((size_t) nOptimisableTerm).m_strVariable);

			// if there are no limiting constraints, LP is unbounded
			if (nLimitingConstraint < 0) return false;

			// transpose optimisable term with limiting constraint
			TransposeVariables(nOptimisableTerm, nLimitingConstraint);
		}
	}

	Meson::Common::Text::String ToString(void)
	{
		String strValue = m_lpEquationObjective.ToString();
		for (size_t unIndex; unIndex < m_listConstraints.Size(); unIndex++)
		{
			strValue += "\n";
			strValue += m_listConstraints(unIndex).ToString();
		}
	}
};

template<typename TReal>
class TLPConstraint
{
public: // public variables
	Meson::Common::Collections::TArrayList<TLPTerm<TReal>> m_listTerms;
	TReal m_rValue;

private: // private methods

public:
	TLPConstraint(void)
		: m_listTerms()
		, m_rValue((TReal) 0.0)
	{
	}

	TLPConstraint &operator=(const TLPConstraint &p_lpConstraint)
	{
		m_listTerms.Clear();
		for (size_t unIndex = 0; unIndex < p_lpConstraint.m_listTerms.Size(); unIndex++)
			m_listTerms.Add(p_lpConstraint.m_listTerms(unIndex));

		m_rValue = p_lpConstraint.m_rValue;
			
		return *this;
	}

	bool operator==(const TLPConstraint &p_lpConstraint) const
	{
		if (!Meson::Common::Maths::TMaths<TReal>::Equals(
			m_rValue, p_lpConstraint.m_rValue)) return false;

		if (m_listTerms.Size() != p_lpConstraint.m_listTerms.Size())
			return false;

		for (size_t unIndex = 0; unIndex < m_listTerms.Size(); unIndex++)
			if (p_lpConstraint.m_listTerms.IndexOf(m_listTerms(unIndex)) < 0)
				return false;

		return true;
	}

	void AddVariableTerm(TReal p_rCoefficient)
	{
		TLPTerm<TReal> lpTerm;
		lpTerm.m_strVariable = "x";
		lpTerm.m_strVariable += ((int) m_listTerms.Size() + 1);
		lpTerm.m_rCoefficient = p_rCoefficient;
		m_listTerms.Add(lpTerm);
	}

	Meson::Common::Text::String ToString(void) const
	{
		Meson::Common::Text::String strValue = "";

		for (size_t nIndex = 0; nIndex < m_listTerms.Size(); nIndex++)
		{
			TLPTerm<TReal> lpTerm = m_listTerms(nIndex);

			if (nIndex == 0)
			{
				strValue += lpTerm.ToString();
				continue;
			}

			if (lpTerm.m_rCoefficient > (TReal) 0.0)
				strValue += " + " + lpTerm.ToString();
			else
			{
				lpTerm.m_rCoefficient = -lpTerm.m_rCoefficient;
				strValue += " - " + lpTerm.ToString();
			}
		}

		strValue += " <= ";
		strValue +=	m_rValue;

		return strValue;
	}
};


template<typename TReal>
class TLPSolver
{
private:
	void ConstructNormalFormTableau(
		const TLPEquation<TReal> &p_lpEquationObjectiveFunction,
		const Meson::Common::Collections::TList<TLPConstraint<TReal>> &p_listConstraints,
		TLPTableau<TReal> &p_lpTableauNormalForm)
	{
		p_lpTableauNormalForm.m_lpEquationObjective.m_listRightTerms.Clear();
		p_lpTableauNormalForm.m_listConstraints.Clear();

		// construct NF equalities
		for (size_t unConstraintIndex = 0; unConstraintIndex < p_listConstraints.Size(); unConstraintIndex++)
		{
			// get current constraint
			TLPConstraint<TReal> &lpConstraint = p_listConstraints(unConstraintIndex);

			// construct NF equation
			TLPEquation<TReal> lpEquation;

			// set artificial term
			lpEquation.m_strLeftTerm = "w";
			lpEquation.m_strLeftTerm += ((int) unConstraintIndex + 1);
			lpEquation.m_rRightConstant = Meson::Common::Maths::TMaths<TReal>::Abs(lpConstraint.m_rValue);

			// insert variable terms
			TReal rFactor = lpConstraint.m_rValue > Meson::Common::Maths::TMaths<TReal>::Epsilon
				? (TReal) -1.0 : (TReal) 1.0;
			for (size_t unTermIndex = 0; unTermIndex < lpConstraint.m_listTerms.Size(); unTermIndex++)
			{
				TLPTerm<TReal> lpTerm = lpConstraint.m_listTerms(unTermIndex);
				lpTerm.m_rCoefficient *= rFactor;
				lpEquation.m_listRightTerms.Add(lpTerm);
			}

			// insert slack term
			TLPTerm<TReal> lpTermSlack;
			lpTermSlack.m_strVariable = "s";
			lpTermSlack.m_strVariable += ((int) unConstraintIndex + 1);
			lpTermSlack.m_rCoefficient = rFactor;
			lpEquation.m_listRightTerms.Add(lpTermSlack);

			p_lpTableauNormalForm.m_listConstraints.Add(lpEquation);
		}

		// construct auxiliary objective function from negative sum of artificial variables
		p_lpTableauNormalForm.m_lpEquationObjective.m_strLeftTerm = "g";
		p_lpTableauNormalForm.m_lpEquationObjective.m_rRightConstant = (TReal) 0.0;
		
		for (size_t unConstraintIndex = 0; unConstraintIndex < p_listConstraints.Size(); unConstraintIndex++)
		{
			// insert ith artificial term (-wi)
			Meson::Common::Text::String strVariable = "w";
			strVariable += ((int) unConstraintIndex + 1);
			p_lpTableauNormalForm.m_lpEquationObjective.m_listRightTerms.Add(
				TLPTerm<TReal>(strVariable, (TReal) -1.0));

			// substitute in terms of normal and slack variables
			p_lpTableauNormalForm.m_lpEquationObjective.Substitute(
				p_lpTableauNormalForm.m_listConstraints(unConstraintIndex));
		}
	}


public:
	TLPSolver(void)
	{
	}

	bool Solve(
		const TLPEquation<TReal> &p_lpEquationObjectiveFunction,
		const Meson::Common::Collections::TList<TLPConstraint<TReal>> &p_listConstraints,
		Meson::Common::Collections::TList<TLPTerm<TReal>> &p_listSolutions)
	{
		// construct tableau in normal form
		TLPTableau<TReal> lpTableauNormalForm;
		ConstructNormalFormTableau(p_lpEquationObjectiveFunction, p_listConstraints, lpTableauNormalForm);

		// optimise for initial feasible vector
		if (!lpTableauNormalForm.Optimise()) return false;

		// construct main tableau
		TLPTableau<TReal> lpTableau;
		lpTableau.m_lpEquationObjective = p_lpEquationObjectiveFunction;

		// set up constraints
		for (size_t unIndex = 0; unIndex < lpTableauNormalForm.m_listConstraints.Size(); unIndex++)
		{
			// set up constraint from auxiliary tableau without artificial terms
			TLPEquation<TReal> lpEquationConstraint
				= lpTableauNormalForm.m_listConstraints(unIndex);
			lpEquationConstraint.EliminateArtificialTerms();
			lpTableau.m_listConstraints.Add(lpEquationConstraint);

			// substitute into objective function
			lpTableau.m_lpEquationObjective.Substitute(lpEquationConstraint);
		}

		// optimise main tableau
		if (!lpTableau.Optimise()) false;

		// extract solution
		p_listSolutions.Clear();
		for (size_t unIndex = 0; unIndex < lpTableau.m_listConstraints.Size(); unIndex++)
		{
			Meson::Common::Text::String strVariable = lpTableau.m_listConstraints(unIndex).m_strLeftTerm;
			if (!(strVariable.Substring(0, 1) == "x")) continue;
			p_listSolutions.Add(TLPTerm<TReal>(strVariable, lpTableau.m_listConstraints(unIndex).m_rRightConstant));
		}

		// sort solutions
		Meson::Common::Collections::TSorting<TLPTerm<TReal>>::QuickSort(p_listSolutions);

		return true;
	}
};

/*
template<typename TReal>
class TLPSolver
{
private:
	void BuildTableau(
		size_t p_unVariables, size_t p_unConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TReal> &p_listConstraintCoefficients,
		Meson::Common::Collections::TList<TReal> &p_listTableau)
	{
		size_t unWidth = p_unVariables + p_unConstraints + 2;

		// constraints
		size_t unTermIndex = 0;
		for (size_t unConstraintIndex = 0; unConstraintIndex < p_unConstraints; unConstraintIndex++)
		{
			// constraint variable coefficients
			for (size_t unVariableIndex = 0; unVariableIndex < p_unVariables; unVariableIndex++)
				p_listTableau.Add(p_listConstraintCoefficients(unTermIndex++));

			// slack variable coefficients
			for (size_t unSlackIndex = 0; unSlackIndex < p_unConstraints; unSlackIndex++)
				p_listTableau.Add(unSlackIndex == unConstraintIndex ? (TReal) 1.0 : (TReal) 0.0);

			// objective function coefficient
			p_listTableau.Add((TReal) 0.0);

			// constant term
			TReal rConstantTerm
				= p_listConstraintCoefficients(unTermIndex++);
			//MESON_ASSERT(rConstantTerm > Meson::Common::Maths::TMaths<TReal>::Epsilon, 
			//	"Constraint constant terms must be positive.")
			p_listTableau.Add(rConstantTerm);
		}

		// objective function

		// negated variable coefficients
		for (size_t unIndex = 0; unIndex < p_unVariables; unIndex++)
			p_listTableau.Add(-p_listObjectiveFunctionCoefficients(unIndex));

		// slack terms
		for (size_t unSlackIndex = 0; unSlackIndex < p_unConstraints; unSlackIndex++)
			p_listTableau.Add((TReal) 0.0);

		// objective function term
		p_listTableau.Add((TReal) 1.0);

		// objective function constant term
		p_listTableau.Add((TReal) 0.0);
	}

	void AddArtificialVariable(
		size_t p_unConstraints, size_t p_unRowIndex,
		size_t &p_unVariables, size_t &p_unWidth,
		Meson::Common::Collections::TList<TReal> &p_listTableau)
	{
		// negate given row
		size_t unInvertOffset = p_unRowIndex * p_unWidth;
		for (size_t unInvertIndex = 0; unInvertIndex < p_unWidth; unInvertIndex++)
			p_listTableau(unInvertOffset++) *= (TReal) -1.0;

		size_t unOffset = p_unVariables;
		for (size_t unRowIndex = 0; unRowIndex < p_unConstraints + 1; unRowIndex++)
		{
			p_listTableau.Insert(unOffset, (p_unRowIndex == unRowIndex) ? (TReal) -1.0 : (TReal) -0.0);
			unOffset += (p_unWidth + 1);
		}

		++p_unVariables;
		++p_unWidth;
	}

	void AddArtificialVariables(
		size_t p_unConstraints, size_t &p_unVariables, size_t &p_unWidth,
		Meson::Common::Collections::TList<TReal> &p_listTableau)
	{
		for (size_t unRowIndex = 0; unRowIndex < p_unConstraints; unRowIndex++)
		{
			if (p_listTableau(unRowIndex * p_unWidth + p_unWidth - 1) < (TReal) 0.0)
				AddArtificialVariable(p_unConstraints,
					unRowIndex, p_unVariables, p_unWidth, p_listTableau); 
		}
	}

	size_t SelectColumn(
		const Meson::Common::Collections::TList<TReal> &p_listTableau,
		size_t p_unOffset, size_t p_unAllVariables)
	{
		size_t unMinIndex = 0;
		TReal rMinValue = Meson::Common::Maths::TMaths<TReal>::Maximum;
		for (size_t unIndex = 0; unIndex < p_unAllVariables; unIndex++)
		{
			TReal rValue = p_listTableau(p_unOffset++);
			if (rValue < rMinValue)
			{
				unMinIndex = unIndex;
				rMinValue = rValue;
			}
		}

		return unMinIndex;
	}

	size_t SelectRow(
		const Meson::Common::Collections::TArrayList<TReal> &p_listTableau,
		size_t p_unWidth, size_t p_unConstraints, size_t p_unColumn)
	{
		size_t unMinIndex = 0;
		TReal rMinRatio = Meson::Common::Maths::TMaths<TReal>::Maximum;

		size_t unOffset = 0;
		for (size_t unConstraintIndex = 0; unConstraintIndex < p_unConstraints;
			unConstraintIndex++)
		{
			TReal rCoefficient = p_listTableau(unOffset + p_unColumn);

			if (Meson::Common::Maths::TMaths<TReal>::Equals(rCoefficient, (TReal) 0.0))
			{
				unOffset += p_unWidth;
				continue;
			}

			TReal rConstantValue = p_listTableau(unOffset + p_unWidth - 1);
			TReal rRatio = rConstantValue / rCoefficient;

			if ((rRatio > (TReal) 0.0) && (rRatio < rMinRatio))
			{
				rMinRatio = rRatio;
				unMinIndex = unConstraintIndex;
			}

			unOffset += p_unWidth;
		}

		return unMinIndex;
	}

	void DoPivot(
		Meson::Common::Collections::TList<TReal> &p_listTableau,
		size_t p_unWidth, size_t p_unHeight, size_t p_unRow, size_t p_unColumn)
	{
		// get pivot value
		size_t unPivotRowOffset = p_unRow * p_unWidth;
		TReal rPivotValue = p_listTableau(unPivotRowOffset + p_unColumn);
		TReal rPivotInverse = (TReal) ((TReal) 1.0 / rPivotValue);

		// divide selected row by pivot value
		size_t unOffset = unPivotRowOffset;
		for (size_t unIndex = 0; unIndex < p_unWidth; unIndex++)
			 p_listTableau(unOffset++) *= rPivotInverse;

		// subtract pivot row from other rows multiplied by a factor
		size_t unRowOffset = 0;
		for (size_t unRowIndex = 0; unRowIndex < p_unHeight; unRowIndex++)
		{
			if (unRowIndex == p_unRow)
			{
				unRowOffset += p_unWidth;
				continue;
			}

			TReal rValue = p_listTableau(unRowOffset + p_unColumn);

			unOffset = unPivotRowOffset;
			size_t unOffsetCurrent = unRowOffset;
			for (size_t unColumnIndex = 0; unColumnIndex < p_unWidth; unColumnIndex++)
				p_listTableau(unOffsetCurrent++) -= p_listTableau(unOffset++) * rValue;

			unRowOffset += p_unWidth;
		}
	}

	bool HasNegativeCoefficients(
		const Meson::Common::Collections::TArrayList<TReal> &p_listTableau,
		size_t p_unFunctionOffset, size_t p_unAllVariables)
	{
		for (size_t unIndex = 0; unIndex < p_unAllVariables; unIndex++)
			if (p_listTableau(p_unFunctionOffset) < -Meson::Common::Maths::TMaths<TReal>::Epsilon)
				return true;

		return false;
	}

	void PrepareResults(
		const Meson::Common::Collections::TArrayList<TReal> &p_listTableau,
		size_t p_unWidth, size_t p_unConstraints, size_t p_unVariables,
		Meson::Common::Collections::TList<TReal> &p_listVariables)
	{
		p_listVariables.Clear();
		for (size_t unIndex = 0; unIndex < p_unVariables; unIndex++)
			p_listVariables.Add((TReal) 0.0);

		for (size_t unColumnIndex = 0; unColumnIndex < p_unConstraints; unColumnIndex++)
		{
			size_t unZeros = 0, unOnes = 0;
			size_t unOffset = unColumnIndex;
			size_t unOneIndex = 0;
			for (size_t unRowIndex = 0; unRowIndex < p_unConstraints; unRowIndex++)
			{
				if (Meson::Common::Maths::TMaths<TReal>::Equals(p_listTableau(unOffset), (TReal) 0.0))
					++unZeros;
				if (Meson::Common::Maths::TMaths<TReal>::Equals(p_listTableau(unOffset), (TReal) 1.0))
				{
					++unOnes;
					unOneIndex = unRowIndex;
				}

				unOffset += p_unWidth;
			}

			if ((unZeros == p_unConstraints - 1)
				&& (unOnes == 1))
			{
				p_listVariables(unColumnIndex) = p_listTableau(unOneIndex * p_unWidth + p_unWidth - 1);
			}
		}
	}
public:
	TLPSolver(void)
	{
	}

	void Maximise(size_t p_unVariables, size_t p_unConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TReal> &p_listConstraintCoefficients,
		Meson::Common::Collections::TList<TReal> &p_listVariables, TReal &p_rMaximum)
	{
		// perform some checks
		MESON_ASSERT(p_listObjectiveFunctionCoefficients.Size() == p_unVariables, 
			"The size of the object function coefficients list must equal the given variable count.")
		MESON_ASSERT(p_unConstraints * (p_unVariables + 1) == p_listConstraintCoefficients.Size(), 
			"The exact number of variable and constant coefficients for the given number of constraints must be given.")

		size_t unWidth = p_unVariables + p_unConstraints + 2;
		size_t unHeight = p_unConstraints + 1;

		// set up tableau
		Meson::Common::Collections::TArrayList<TReal> listTableau(unWidth * unHeight);
		BuildTableau(
			p_unVariables, p_unConstraints,
			p_listObjectiveFunctionCoefficients, p_listConstraintCoefficients,
			listTableau);

		// add artificial variables if needed (for negative constants)
		AddArtificialVariables(p_unConstraints, p_unVariables, unWidth, listTableau);

		// do simplex algorithm
		size_t unAllVariables = p_unVariables + p_unConstraints;
		size_t unObjectiveFunctionOffset = p_unConstraints * unWidth;
		while(HasNegativeCoefficients(listTableau,
			unObjectiveFunctionOffset, unAllVariables))
		{
			// select variable with most negative coefficient
			size_t unPivotColumn = SelectColumn(listTableau,
				unObjectiveFunctionOffset, unAllVariables);

			// select constraint with minimum ratio
			size_t unPivotRow = SelectRow(
				listTableau, unWidth, p_unConstraints, unPivotColumn);

			// do pivot step
			DoPivot(listTableau, unWidth, unHeight, unPivotRow, unPivotColumn); 
		}

		// prepare results

		// get variables
		PrepareResults(listTableau, unWidth,
			p_unConstraints, p_unVariables, p_listVariables);

		// maximum value
		p_rMaximum = listTableau(unObjectiveFunctionOffset + p_unVariables + p_unConstraints + 1);
	}


	void Minimise(size_t p_unVariables, size_t p_unConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TReal> &p_listConstraintCoefficients,
		Meson::Common::Collections::TList<TReal> &p_listVariables, TReal &p_rMaximum)
	{
		// prepare negation of objective function
		Meson::Common::Collections::TArrayList<TReal>
			p_listObjectiveFunctionAuxiliary(p_listObjectiveFunctionCoefficients.Size());
		for (size_t unIndex = 0; unIndex < p_unVariables; unIndex++)
			p_listObjectiveFunctionAuxiliary.Add(
				-p_listObjectiveFunctionCoefficients(unIndex));

		// maximise auxiliary function
		Maximise(p_unVariables, p_unConstraints,
			p_listObjectiveFunctionAuxiliary, p_listConstraintCoefficients,
			p_listVariables, p_rMaximum);

		// negate auxiliary maximum
		p_rMaximum = -p_rMaximum;
	}
};

*/

/*
template<typename TReal>
class TLPConstraint
{
public: // public  variables
	Meson::Common::Collections::TArrayList<TReal> m_listCoefficients;
	TReal m_rConstant;

public: // public methods
	TLPConstraint(void)
		: m_listCoefficients()
		, m_rConstant((TReal) 0.0)
	{
	}

	TLPConstraint(size_t p_unVariables)
		: m_listCoefficients(p_unVariables)
		, m_rConstant((TReal) 0.0)
	{
		for (size_t unIndex = 0; unIndex < p_unVariables; unIndex++)
			m_listCoefficients.Add((TReal) 0.0);
	}

	TLPConstraint(const TLPConstraint &p_lpConstraint)
		: m_listCoefficients(p_lpConstraint.m_listCoefficients)
		, m_rConstant(p_lpConstraint.m_rConstant)
	{
	}

	TLPConstraint &operator=(const TLPConstraint &p_lpConstraint)
	{
		m_listCoefficients.Clear();
		m_listCoefficients.AddList(p_lpConstraint.m_listCoefficients);
		m_rConstant = p_lpConstraint.m_rConstant;
		return *this;
	}

	bool operator==(const TLPConstraint &p_lpConstraint)
	{
		if (!Meson::Common::Maths::TMaths<TReal>::Equals(
			m_rConstant, p_lpConstraint.m_rConstant))
			return false;

		size_t unCoefficients = m_listCoefficients.Size();
		if (unCoefficients != p_lpConstraint.m_listCoefficients.Size())
			return false;

		for (size_t unIndex = 0; unIndex < unCoefficients; unIndex++)
			if (!Meson::Common::Maths::TMaths<TReal>::Equals(
				m_listCoefficients(unIndex),
				p_lpConstraint.m_listCoefficients(unIndex)))
				return false;

		return true;
	}

	bool SatisfiedBy(const Meson::Common::Collections::TList<TReal> &p_listValues)
	{
		size_t unCount = Meson::Common::Maths::TMaths<TReal>::Min(
			m_listCoefficients.Size(), p_listValues.Size());
		TReal rLHS = (TReal) 0.0;
		for (size_t unIndex = 0; unIndex < unCount; unIndex++)
			rLHS += m_listCoefficients(unIndex) * p_listValues(unIndex);
		return rLHS <= m_rConstant;
	}

	void Add(const TLPConstraint &p_lpConstraint, TReal p_rMultiplier)
	{
		size_t unCount = Meson::Common::Maths::TMaths<TReal>::Min(
			m_listCoefficients.Size(), p_lpConstraint.m_listCoefficients.Size());

		for (size_t unIndex = 0; unIndex < unCount; unIndex++)
			m_listCoefficients(unIndex)
				+= p_lpConstraint.m_listCoefficients(unIndex) * p_rMultiplier;

		m_rConstant += p_lpConstraint.m_rConstant * p_rMultiplier;
	}
};

template<typename TReal>
class TLPSolver
{
private: // private methods
	inline bool MaximiseSingleVariable(
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TLPConstraint<TReal>> &p_listConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listLowerBounds,
		const Meson::Common::Collections::TList<TReal> &p_listUpperBounds,
		Meson::Common::Collections::TList<TReal> &p_listVariables)
	{
		// handle single-variable case

		// compute initial values from bounds
		TReal rMaximum = p_listUpperBounds(0);
		TReal rMinimum = p_listLowerBounds(0);

		// process all constraints
		for (size_t unIndex = 0; unIndex < p_listConstraints.Size(); unIndex++)
		{
			// get current constraint
			TLPConstraint<TReal> &lpConstraint = p_listConstraints(unIndex);
			TReal rCoefficient = lpConstraint.m_listCoefficients(0);

			if (Meson::Common::Maths::TMaths<TReal>::Equals(rCoefficient, (TReal) 0.0))
			{
				// check constraint when var coefficient is zero
				// 0 X1 <= C and C < 0 i.e. C >= 0 and C < 0 i.e. no solution 
				if (lpConstraint.m_rConstant < -Meson::Common::Maths::TMaths<TReal>::Epsilon)
					return false;
			}
			else
			{
				TReal rSolution = lpConstraint.m_rConstant / rCoefficient;
				if (lpConstraint.m_rConstant >= Meson::Common::Maths::TMaths<TReal>::Epsilon)
				{
					// evaluate upper bound as min of all solutions for constant > 0
					if (rMaximum > rSolution)
						rMaximum = rSolution;
				}
				else
				{
					// evaluate lower bound as max of all solutions for constant < 0
					if (rMinimum < rSolution)
						rMinimum = rSolution;
				}
			}
		}

		// empty interval - no solution
		if (rMaximum < rMinimum) return false;

		// otherwise maximal value is maximum
		p_listVariables.Clear();
		p_listVariables.Add(rMaximum);

		return true;
	}

	bool MaximiseRecursive(
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TLPConstraint<TReal>> &p_listConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listLowerBounds,
		const Meson::Common::Collections::TList<TReal> &p_listUpperBounds,
		Meson::Common::Collections::TList<TReal> &p_listVariables)
	{
		// determine variable count
		size_t unVariables = p_listObjectiveFunctionCoefficients.Size();

		// solve single-variable case
		if (unVariables == 1)
			return MaximiseSingleVariable(p_listObjectiveFunctionCoefficients,
				p_listConstraints, p_listLowerBounds, p_listUpperBounds,
				p_listVariables);

		// otherwise, handle multi-variable case

		// compute initial optimum solution (assuming largest possible bounds)
		p_listVariables.Clear();
		for (size_t unIndex = 0; unIndex < p_listObjectiveFunctionCoefficients.Size(); unIndex++)
		{
			p_listVariables.Add(
				p_listObjectiveFunctionCoefficients(unIndex) >= -Meson::Common::Maths::TMaths<TReal>::Epsilon
				? p_listUpperBounds(unIndex)
				: p_listLowerBounds(unIndex));
		}

		Meson::Common::Collections::TArrayList<TLPConstraint<TReal>> listConstraintsProcessed;

		for (size_t unConstraintIndex = 0;
			unConstraintIndex < p_listConstraints.Size(); unConstraintIndex++)
		{
			TLPConstraint<TReal> &lpConstraint = p_listConstraints(unConstraintIndex);

			// if current solution satisfies current constraint, keep current solution
			if (lpConstraint.SatisfiedBy(p_listVariables))
			{
				// insert current constraint in processed list
				listConstraintsProcessed.Add(lpConstraint);
				
				continue;
			}

			// otherwise, compute new solution from current

			// select non-zero constraint coefficient with highest index
			int nIndexToRemove = -1;
			Meson::Common::Collections::TArrayList<TReal> &listCoefficients
				= lpConstraint.m_listCoefficients;
			for (size_t unIndex = 0; unIndex < listCoefficients.Size(); unIndex++)
			{
				if (!Meson::Common::Maths::TMaths<TReal>::Equals(listCoefficients(unIndex), (TReal) 0.0))
					nIndexToRemove = unIndex;
			}

			// if no such term found, no solution
			if (nIndexToRemove == -1) return false;

			// otherwise prepare parameters for recursive call

			// prepare constraints
			Meson::Common::Collections::TArrayList<TLPConstraint<TReal>> listConstraintsRec;
			TReal rTermInverse = (TReal) 1.0 / listCoefficients(nIndexToRemove);
			for (size_t unIndex = 0; unIndex < listConstraintsProcessed.Size(); unIndex++)
			{
				// constraint
				TLPConstraint<TReal> &lpConstraintProcessed = listConstraintsProcessed(unIndex);
				TReal rTermProcessed = lpConstraintProcessed.m_listCoefficients(nIndexToRemove);

				TReal rMultiplier = -rTermProcessed * rTermInverse;
				TLPConstraint<TReal> &lpConstraintRec = lpConstraintProcessed;
				lpConstraintRec.Add(lpConstraint, rMultiplier);
				lpConstraintRec.m_listCoefficients.RemoveAt(nIndexToRemove);
			}

			// prepare projected bounds as constraints in recursive call
			TLPConstraint<TReal> lpConstraintUpperBound(unVariables);
			lpConstraintUpperBound.m_listCoefficients(nIndexToRemove) = (TReal) 1.0;
			lpConstraintUpperBound.m_rConstant = p_listUpperBounds(nIndexToRemove);
			lpConstraintUpperBound.Add(lpConstraint, -rTermInverse);
			lpConstraintUpperBound.m_listCoefficients.RemoveAt(nIndexToRemove);
			listConstraintsRec.Add(lpConstraintUpperBound);

			TLPConstraint<TReal> lpConstraintLowerBound(unVariables);
			lpConstraintLowerBound.m_listCoefficients(nIndexToRemove) = (TReal) -1.0;
			lpConstraintLowerBound.m_rConstant = -p_listLowerBounds(nIndexToRemove);
			lpConstraintLowerBound.Add(lpConstraint, -rTermInverse);
			lpConstraintLowerBound.m_listCoefficients.RemoveAt(nIndexToRemove);
			listConstraintsRec.Add(lpConstraintLowerBound);

			// prepare objective function
			Meson::Common::Collections::TArrayList<TReal> listObjectiveFunctionCoefficientsRec;
			TReal rObjectiveTermToRemove
				= p_listObjectiveFunctionCoefficients(nIndexToRemove);
			for (size_t unIndex = 0; unIndex < unVariables; unIndex++)
			{
				listObjectiveFunctionCoefficientsRec.Add(
					p_listObjectiveFunctionCoefficients(unIndex)
					- rObjectiveTermToRemove * rTermInverse * lpConstraint.m_listCoefficients(unIndex));
			}
			listObjectiveFunctionCoefficientsRec.RemoveAt(nIndexToRemove);	

			// prepare new variable bounds
			Meson::Common::Collections::TArrayList<TReal> listLowerBoundsRec;
			listLowerBoundsRec.AddList(p_listLowerBounds);
			listLowerBoundsRec.RemoveAt(nIndexToRemove);

			Meson::Common::Collections::TArrayList<TReal> listUpperBoundsRec;
			listUpperBoundsRec.AddList(p_listUpperBounds);
			listUpperBoundsRec.RemoveAt(nIndexToRemove);

			// do recursive call
			Meson::Common::Collections::TArrayList<TReal> listVariablesRec;
			if (!MaximiseRecursive(listObjectiveFunctionCoefficientsRec, listConstraintsRec,
				listLowerBoundsRec, listUpperBoundsRec, listVariablesRec))
				return false;


			// lift result from recusrive call to present dimension
			p_listVariables.Clear();
			p_listVariables.AddList(listVariablesRec);
			p_listVariables.Insert(nIndexToRemove, (TReal) 0.0);

			// prepare and insert lifted term
			TReal rLiftedTerm = lpConstraint.m_rConstant;
			for (size_t unIndex = 0; unIndex < unVariables; unIndex++)
				rLiftedTerm -= lpConstraint.m_listCoefficients(unIndex) * p_listVariables(unIndex);
			rLiftedTerm *= rTermInverse;
			p_listVariables(nIndexToRemove) = rLiftedTerm;

			// insert current contraint in processed list
			listConstraintsProcessed.Add(lpConstraint);
		}

		return true;
	}

public: // public methods
	TLPSolver(void)
	{
	}

	inline bool Maximise(
		const Meson::Common::Collections::TList<TReal> &p_listObjectiveFunctionCoefficients,
		const Meson::Common::Collections::TList<TLPConstraint<TReal>> &p_listConstraints,
		const Meson::Common::Collections::TList<TReal> &p_listLowerBounds,
		const Meson::Common::Collections::TList<TReal> &p_listUpperBounds,
		Meson::Common::Collections::TList<TReal> &p_listVariables)
	{
		// determine variable count
		size_t unVariables = p_listObjectiveFunctionCoefficients.Size();

		MESON_ASSERT(
			unVariables > 0,
			"At least one variable must be specified.")

#ifdef MESON_DEBUG

		for (size_t unIndex = 0; unIndex < p_listConstraints.Size(); unIndex++)
		{
			TLPConstraint<TReal> &lpConstraint = p_listConstraints(unIndex);
			MESON_ASSERT(
				unVariables == lpConstraint.m_listCoefficients.Size(),
				"Constraints must contain the same number of variable terms.")
		}
#endif

		return MaximiseRecursive(
			p_listObjectiveFunctionCoefficients, p_listConstraints,
			p_listLowerBounds, p_listUpperBounds,  p_listVariables);
	}

};

*/

Meson_Common_Maths_Solvers_END