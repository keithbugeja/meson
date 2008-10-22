//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TInterval<T>
//	Templated class for continuous intervals.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		13/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

Meson_Common_Maths_BEGIN

template <class TReal> class TInterval
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised interval.
	//----------------------------------------------------------------------------------------------
	inline TInterval( void ) {}	

	//----------------------------------------------------------------------------------------------
	/// Copy constructor.
	/// \param p_interval An interval object used as a source for copy
	//----------------------------------------------------------------------------------------------
	inline TInterval( const TInterval &p_interval ) : Min(p_interval.Min), Max(p_interval.Max) { }

	//----------------------------------------------------------------------------------------------
	/// Copy constructor.
	/// \param p_real A real number used as a source for copy
	//----------------------------------------------------------------------------------------------
	inline TInterval( const TReal &p_real) : Min(p_real), Max(p_real) { }

	//----------------------------------------------------------------------------------------------
	/// Construct an initialised interval.
	/// \param p_tR	Real component
	/// \param p_tI	Imaginary component
	//----------------------------------------------------------------------------------------------
	inline TInterval( const TReal p_tMin, const TReal p_tMax ) : Min(p_tMin), Max(p_tMax) { }

	//----------------------------------------------------------------------------------------------
	/// Sets the endpoints of the interval to zero.
	//----------------------------------------------------------------------------------------------
	inline void MakeZero( void ) 
	{
		Min = Max = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Makes the interval an empty set (Min > Max).
	//----------------------------------------------------------------------------------------------
	inline void MakeEmpty( void ) 
	{
		Min = TMaths<TReal>::Maximum;
		Max = -TMaths<TReal>::Maximum;
	}

	//----------------------------------------------------------------------------------------------
	/// Makes the interval a universal set.
	//----------------------------------------------------------------------------------------------
	inline void MakeUniversal( void ) 
	{
		Min = -TMaths<TReal>::Maximum;
		Max = TMaths<TReal>::Maximum;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the interval is a point.
	/// \returns True if the interval consists of only one point, false otherwise
	//----------------------------------------------------------------------------------------------
	inline bool IsPoint( void ) const
	{
		return TMaths<TReal>::Equals(Min, Max);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the interval is an empty set.	/// \returns True if the interval is an empty set, false otherwise
	//----------------------------------------------------------------------------------------------
	inline bool IsEmpty( void ) const
	{
		return Min > Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the span of the interval.
	/// \returns Span of the interval
	//----------------------------------------------------------------------------------------------
	inline TReal Span(void) const
	{
		return Max - Min;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the median of the interval.
	/// \returns Median of the interval
	//----------------------------------------------------------------------------------------------
	inline TReal Median(void) const
	{
		return (Min + Max) * (TReal) 0.5;
	}

	//----------------------------------------------------------------------------------------------
	/// Extends the interval to include the given real value.
	/// \param p_tValue Real value to be included in the interval.
	//----------------------------------------------------------------------------------------------
	inline void Extend(TReal p_tValue)
	{
		if (Min > p_tValue)
			Min = p_tValue;
		if (Max < p_tValue)
			Max = p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	/// Extends the interval to include the given interval.
	/// \param p_tValue Interval to be included in this interval.
	//----------------------------------------------------------------------------------------------
	inline void Extend(const TInterval &interval)
	{
		if (interval.IsEmpty()) return;
		if (Min > interval.Min)
			Min = interval.Min;
		if (Max < interval.Max)
			Max = interval.Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Extends the interval to include the given real value.
	/// \param p_tValue Real value to be included in the interval.
	//----------------------------------------------------------------------------------------------
	inline void ExtendCopy(TReal p_tValue) const
	{
		return TInterval(
			TMaths<TReal>::Min(Min, p_tValue),
			TMaths<TReal>::Max(Max, p_tValue)); 
	}

	//----------------------------------------------------------------------------------------------
	/// Extends the interval to include the given real value.
	/// \param p_tValue Real value to be included in the interval.
	//----------------------------------------------------------------------------------------------
	inline void ExtendCopy(const TInterval &interval) const
	{
		return TInterval(
			TMaths<TReal>::Min(Min, interval.Min),
			TMaths<TReal>::Max(Max, interval.Max)); 
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the given value to this interval.
	/// \param p_tValue Second operand in interval addition
	//----------------------------------------------------------------------------------------------
	inline void Add( TReal p_tValue ) const
	{
		Min += p_tValue;
		Max += p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the given interval to this interval.
	/// \param p_interval Second operand in interval addition
	//----------------------------------------------------------------------------------------------
	inline void Add( const TInterval &p_interval ) const
	{
		Min += p_interval.Min;
		Max += p_interval.Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the interval with the given real value.
	/// \param p_tValue Second operand in interval addition
	/// \returns Result of interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval AddCopy( TReal p_tValue ) const
	{
		return TInterval(Min + p_tValue, Max + p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the interval with the given interval.
	/// \param p_interval Second operand in interval addition
	/// \returns Result of interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval AddCopy( const TInterval &p_interval ) const
	{
		return TInterval(Min + p_interval.Min, Max + p_interval.Max);
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the given real value from this interval.
	/// \param p_interval Second operand in interval subtraction
	//----------------------------------------------------------------------------------------------
	inline void Subtract( TReal p_tValue ) const
	{
		Min -= p_tValue;
		Max -= p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the given interval from this interval.
	/// \param p_interval Second operand in interval subtraction
	//----------------------------------------------------------------------------------------------
	inline void Subtract( const TInterval &p_interval ) const
	{
		Min -= p_interval.Min;
		Max -= p_interval.Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the interval by the given real value.
	/// \param p_interval Second operand in interval subtraction
	/// \returns Result of interval subtraction
	//----------------------------------------------------------------------------------------------
	TInterval SubtractCopy( TReal p_tValue ) const
	{
		return TInterval(Min - p_tValue, Max - p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the interval by the given interval.
	/// \param p_interval Second operand in interval subtraction
	/// \returns Result of interval subtraction
	//----------------------------------------------------------------------------------------------
	TInterval SubtractCopy( const TInterval &p_interval ) const
	{
		return TInterval(Min - p_interval.Max, Max - p_interval.Min);
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given real value.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	void Multiply( TReal p_tValue ) const
	{
		if (p_tValue >= (TReal) 0.0)
		{
			Min *= p_tValue;
			Max *= p_tValue;
		}
		else
		{
			TReal tNewMax = Min * p_tValue;
			Min = Max * p_tValue;
			Max = tNewMax;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given interval.
	/// \param p_interval Second operand in interval multiplication
	//----------------------------------------------------------------------------------------------
	void Multiply( const TInterval &p_interval ) const
	{
		TReal tMinMin = Min * p_interval.Min;
		TReal tMinMax = Min * p_interval.Max;
		TReal tMaxMin = Max * p_interval.Min;
		TReal tMaxMax = Max * p_interval.Max;

		Min = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		Max = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given real value.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	TInterval MultiplyCopy( TReal p_tValue ) const
	{
		if (p_tValue >= (TReal) 0.0)
			return TInterval(Min * p_tValue, Max * p_tValue);
		else
			return TInterval(Max * p_tValue, Min * p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies the interval by the given interval.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	TInterval MultiplyCopy( const TInterval &p_interval ) const
	{
		TReal tMinMin = Min * p_interval.Min;
		TReal tMinMax = Min * p_interval.Max;
		TReal tMaxMin = Max * p_interval.Min;
		TReal tMaxMax = Max * p_interval.Max;

		TReal tResultMin = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		TReal tResultMax = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));

		return TInterval(tResultMin, tResultMax);
	}

	//----------------------------------------------------------------------------------------------
	/// Divides this interval by the given real value.
	/// \param p_interval Second operand in interval division
	//----------------------------------------------------------------------------------------------
	void Divide( TReal p_tValue ) const
	{
		TReal tResult1 = p_tValue / p_interval.Min;
		TReal tResult2 = p_tValue / p_interval.Max;

		if (tResult1 < tResult2)
		{
			Min = tResult1;
			Max = tResult2;
		}
		else
		{
			Min = tResult2;
			Max = tResult1;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Divides this interval by the given interval.
	/// \param p_interval Second operand in interval division
	//----------------------------------------------------------------------------------------------
	void Divide( const TInterval &p_interval ) const
	{
		TReal tMinMin = Min / p_interval.Min;
		TReal tMinMax = Min / p_interval.Max;
		TReal tMaxMin = Max / p_interval.Min;
		TReal tMaxMax = Max / p_interval.Max;

		Min = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		Max = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the interval by the given real value.
	/// \param p_interval Second operand in interval division
	/// \returns Result of interval division
	//----------------------------------------------------------------------------------------------
	TInterval DivideCopy( TReal p_tValue ) const
	{
		TReal tResult1 = p_tValue / p_interval.Min;
		TReal tResult2 = p_tValue / p_interval.Max;

		if (tResult1 < tResult2)
			return TInterval(tResult1, tResult2);
		else
			return TInterval(tResult2, tResult1);
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the interval by the given interval.
	/// \param p_interval Second operand in interval division
	/// \returns Result of interval division
	//----------------------------------------------------------------------------------------------
	TInterval DivideCopy( const TInterval &p_interval ) const
	{
		TReal tMinMin = Min / p_interval.Min;
		TReal tMinMax = Min / p_interval.Max;
		TReal tMaxMin = Max / p_interval.Min;
		TReal tMaxMax = Max / p_interval.Min;

		TReal tResultMin = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		TReal tResultMax = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));

		return TInterval(tResultMin, tResultMax);
	}

	//----------------------------------------------------------------------------------------------
	/// Raises this interval to the given real power.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	void Power( TReal p_tValue ) const
	{
		TReal tResult1 = Maths<TReal>::Pow(Min, p_tValue);
		TReal tResult2 = Maths<TReal>::Pow(Max, p_tValue);

		if (tResult1 < tResult2)
		{
			Min = tResult1;
			Max = tResult2;
		}
		else
		{
			Min = tResult2;
			Max = tResult1;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Raises this interval by the given interval power.
	/// \param p_interval Second operand in interval multiplication
	//----------------------------------------------------------------------------------------------
	void Power( const TInterval &p_interval ) const
	{
		TReal tMinMin = TMaths<TReal>::Pow(Min, p_interval.Min);
		TReal tMinMax = TMaths<TReal>::Pow(Min, p_interval.Max);
		TReal tMaxMin = TMaths<TReal>::Pow(Max, p_interval.Min);
		TReal tMaxMax = TMaths<TReal>::Pow(Max, p_interval.Max);

		Min = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		Max = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));
	}

	//----------------------------------------------------------------------------------------------
	/// Raises the interval to the given real power.
	/// \param p_interval Second operand in interval power
	/// \returns Result of interval power
	//----------------------------------------------------------------------------------------------
	TInterval PowerCopy( TReal p_tValue ) const
	{
		TReal tResult1 = Maths<TReal>::Pow(Min, p_tValue);
		TReal tResult2 = Maths<TReal>::Pow(Max, p_tValue);

		if (tResult1 < tResult2)
			return TInterval(tResult1, tResult2);
		else
			return TInterval(tResult2, tResult1);
	}

	//----------------------------------------------------------------------------------------------
	/// Raises the interval to the given interval power.
	/// \param p_interval Second operand in interval power
	/// \returns Result of interval power
	//----------------------------------------------------------------------------------------------
	TInterval PowerCopy( const TInterval &p_interval ) const
	{
		TReal tMinMin = Maths<TReal>::Pow(Min, p_interval.Min);
		TReal tMinMax = Maths<TReal>::Pow(Min, p_interval.Max);
		TReal tMaxMin = Maths<TReal>::Pow(Max, p_interval.Min);
		TReal tMaxMax = Maths<TReal>::Pow(Max, p_interval.Max);

		TReal tResultMin = TMaths<TReal>::Min(
			TMaths<TReal>::Min(tMinMin, tMinMax),
			TMaths<TReal>::Min(tMaxMin, tMaxMax));

		TReal tResultMax = TMaths<TReal>::Max(
			TMaths<TReal>::Max(tMinMin, tMinMax),
			TMaths<TReal>::Max(tMaxMin, tMaxMax));

		return TInterval(tResultMin, tResultMax);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the interal.
	//----------------------------------------------------------------------------------------------
	inline void Negate(void) 
	{
		TReal tNewMax = -Min;
		Min = -Max;
		Max = tNewMax;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the interval.
	/// \returns Negative interval counterpart
	//----------------------------------------------------------------------------------------------
	inline TInterval NegateCopy(void) const
	{
		return TInterval(-Max, -Min);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the intersection of the interval with the given interval.
	/// \param p_interval Second operand in interval intersection
	/// \returns Result of interval intersection
	//----------------------------------------------------------------------------------------------
	TInterval IntersectCopy( const TInterval &p_interval ) const
	{
		return TInterval(
			TMaths<TReal>::Max(Min, p_interval.Min),
			TMaths<TReal>::Min(Max, p_interval.Max));
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the intersection of this interval with the given interval.
	/// \param p_interval Second operand in interval intersection
	//----------------------------------------------------------------------------------------------
	void Intersect( const TInterval &p_interval ) const
	{
		TReal tNewMin = TMaths<TReal>::Max(Min, p_interval.Min);
		Max = TMaths<TReal>::Min(Max, p_interval.Max);
		Min = tNewMin;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns true if this interval intersects the given interval.
	/// \param p_interval Second operand in interval intersection test
	//----------------------------------------------------------------------------------------------
	bool Intersects( const TInterval &p_interval ) const
	{
		return Max >= p_interval.Min && Min <= p_interval.Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns true if this interval contains the given real value.
	/// \param p_tValue Value used in containment test
	//----------------------------------------------------------------------------------------------
	bool Contains( const TReal &p_tValue ) const
	{
		return p_tValue >= Min && p_tValue <= Max;
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the given value to this interval.
	/// \param p_tValue Second operand in interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator+=( TReal p_tValue ) const
	{
		Add(p_tValue);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the given interval to this interval.
	/// \param p_interval Second operand in interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator+=( const TInterval &p_interval ) const
	{
		Add(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the interval with the given real value.
	/// \param p_tValue Second operand in interval addition
	/// \returns Result of interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval operator+( TReal p_tValue ) const
	{
		return AddCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Adds the interval with the given interval.
	/// \param p_interval Second operand in interval addition
	/// \returns Result of interval addition
	//----------------------------------------------------------------------------------------------
	inline TInterval operator+( const TInterval &p_interval ) const
	{
		return AddCopy(p_interval);
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the given real value from this interval.
	/// \param p_interval Second operand in interval subtraction
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator-=( TReal p_tValue ) const
	{
		Subtract(p_tValue);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the given interval from this interval.
	/// \param p_interval Second operand in interval subtraction
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator-=( const TInterval &p_interval ) const
	{
		Subtract(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the interval by the given real value.
	/// \param p_interval Second operand in interval subtraction
	/// \returns Result of interval subtraction
	//----------------------------------------------------------------------------------------------
	inline TInterval operator-( TReal p_tValue ) const
	{
		return SubtractCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Subtracts the interval by the given interval.
	/// \param p_interval Second operand in interval subtraction
	/// \returns Result of interval subtraction
	//----------------------------------------------------------------------------------------------
	inline TInterval operator-( const TInterval &p_interval ) const
	{
		return SubtractCopy(p_interval);
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given real value.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator*=( TReal p_tValue ) const
	{
		Multiply(p_tValue);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given interval.
	/// \param p_interval Second operand in interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator*=( const TInterval &p_interval ) const
	{
		Multiply(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies this interval by the given real value.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval operator*( TReal p_tValue ) const
	{
		return MultiplyCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies the interval by the given interval.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval operator*( const TInterval &p_interval ) const
	{
		return MultiplyCopy(p_interval);
	}

	//----------------------------------------------------------------------------------------------
	/// Divides this interval by the given real value.
	/// \param p_interval Second operand in interval division
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator/=( TReal p_tValue ) const
	{
		Divide(p_tValue);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Divides this interval by the given interval.
	/// \param p_interval Second operand in interval division
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator/=( const TInterval &p_interval ) const
	{
		Divide(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the interval by the given real value.
	/// \param p_interval Second operand in interval division
	/// \returns Result of interval division
	//----------------------------------------------------------------------------------------------
	inline TInterval operator/( TReal p_tValue ) const
	{
		return DivideCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the interval by the given interval.
	/// \param p_interval Second operand in interval division
	/// \returns Result of interval division
	//----------------------------------------------------------------------------------------------
	inline TInterval operator/( const TInterval &p_interval ) const
	{
		return DivideCopy(p_interval);
	}

	//----------------------------------------------------------------------------------------------
	/// Raises this interval to the given real power.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator^=( TReal p_tValue ) const
	{
		Power(p_tValue);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Raises this interval by the given interval power.
	/// \param p_interval Second operand in interval multiplication
	/// \returns Result of interval multiplication
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator^=( const TInterval &p_interval ) const
	{
		Power(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Raises the interval to the given real power.
	/// \param p_interval Second operand in interval power
	/// \returns Result of interval power
	//----------------------------------------------------------------------------------------------
	inline TInterval operator^( TReal p_tValue ) const
	{
		return PowerCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Raises the interval to the given interval power.
	/// \param p_interval Second operand in interval power
	/// \returns Result of interval power
	//----------------------------------------------------------------------------------------------
	inline TInterval operator^( const TInterval &p_interval ) const
	{
		return PowerCopy(p_interval);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the interval.
	/// \returns Negative interval counterpart
	//----------------------------------------------------------------------------------------------
	inline TInterval operator-(void) const
	{
		return NegateCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Cumputes the intersection of this interval with the given interval.
	/// \param p_interval Second operand in interval intersection
	//----------------------------------------------------------------------------------------------
	inline TInterval &operator&=( const TInterval &p_interval ) const
	{
		Intersect(p_interval);
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Cumputes the intersection of the interval with the given interval.
	/// \param p_interval Second operand in interval intersection
	/// \returns Result of interval intersection
	//----------------------------------------------------------------------------------------------
	inline TInterval operator&( const TInterval &p_interval ) const
	{
		return IntersectCopy(p_interval);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	inline bool operator== (const TInterval &p_interval) const
	{
		return ( TMaths<TReal>::Equals(p_interval.Min, Min) && 
				 TMaths<TReal>::Equals(p_interval.Max, Max) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	inline bool operator!= (const TInterval &p_interval) const
	{
		return ( !TMaths<TReal>::Equals(p_interval.Min, Min) || 
				 !TMaths<TReal>::Equals(p_interval.Max, Max) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the double array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	inline operator TReal*() const
	{
		return (TReal*)&Element;
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TInterval Empty;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[2];
		struct
		{
			TReal Min, Max;
		};
	};
	//----------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <class TReal> TInterval<TReal> const TInterval<TReal>::Empty = TInterval<TReal>(TMaths<TReal>::Maximum, TMaths<TReal>::Minimum);


//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TInterval<double>	Interval;
typedef TInterval<float>	Intervalf;

Meson_Common_Maths_END

//----------------------------------------------------------------------------------------------
// Global operators
//----------------------------------------------------------------------------------------------
template <class TReal>
inline Meson::Common::Maths::TInterval<TReal> operator+(TReal p_rValue,
	const Meson::Common::Maths::TInterval<TReal> p_interval)
{
	return p_interval + p_rValue;
};

template <class TReal>
inline Meson::Common::Maths::TInterval<TReal> operator-(TReal p_rValue,
	const Meson::Common::Maths::TInterval<TReal> p_interval)
{
	return -p_interval + p_rValue;
};

template <class TReal>
inline Meson::Common::Maths::TInterval<TReal> operator*(TReal p_rValue,
	const Meson::Common::Maths::TInterval<TReal> p_interval)
{
	return p_interval * p_rValue;
};

template <class TReal>
inline Meson::Common::Maths::TInterval<TReal> operator/(TReal p_rValue,
	const Meson::Common::Maths::TInterval<TReal> p_interval)
{
	return Meson::Common::Maths::TComplex<TInterval>(p_rValue, p_rValue)
		/ p_interval;
};

template <class TReal>
inline Meson::Common::Maths::TInterval<TReal> operator^(TReal p_rValue,
	const Meson::Common::Maths::TInterval<TReal> p_interval)
{
	return Meson::Common::Maths::TComplex<TInterval>(p_rValue, p_rValue)
		^ p_interval;
};
