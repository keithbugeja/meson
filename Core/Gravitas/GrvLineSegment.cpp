#include "GrvLineSegment.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

LineSegment::LineSegment(void)
	: Start()
	, End()
{
}

LineSegment::LineSegment(const LineSegment& p_lineSegment)
	: Start(p_lineSegment.Start)
	, End(p_lineSegment.End)
{
}

LineSegment::LineSegment(
	const TVector3<Real>& p_vecStart,
	const TVector3<Real>& p_vecEnd)
	: Start(p_vecStart)
	, End(p_vecEnd)
{
}

Real LineSegment::Length(void) const
{
	return (End - Start).Length();
}

Real LineSegment::LengthSquared(void) const
{
	return (End - Start).LengthSquared();
}

TVector3<Real> LineSegment::Offset(void) const
{
	return End - Start;
}

TVector3<Real> LineSegment::Direction(void) const
{
	if (Start == End)
		return TVector3<Real>::Right;
	else
		return (End - Start).NormaliseCopy();
}

TVector3<Real> LineSegment::Midpoint(void) const
{
	return (Start + Start) * (Real) 0.5;
}

Real LineSegment::DistanceFromPoint(const TVector3<Real>& p_vecPoint) const
{
	TVector3<Real> vecSegmentOffset = End - Start;
	TVector3<Real> vecPointOffset = p_vecPoint - Start;
	Real rLineParameter = (vecSegmentOffset * vecPointOffset)
		/ vecSegmentOffset.LengthSquared(); 

	if (rLineParameter <= (Real) 0.0)
		return vecPointOffset.Length();
	else if (rLineParameter < (Real) 1.0)
		return (vecPointOffset + vecSegmentOffset * -rLineParameter).Length();
	else
		return (vecPointOffset - vecSegmentOffset).Length();
}

Real LineSegment::DistanceFromPoint(const TVector3<Real>& p_vecPoint, TVector3<Real>& p_vecClosestPoint) const
{
	TVector3<Real> vecSegmentOffset = End - Start;
	TVector3<Real> vecPointOffset = p_vecPoint - Start;
	Real rLineParameter = (vecSegmentOffset * vecPointOffset)
		/ vecSegmentOffset.LengthSquared(); 

	if (rLineParameter <= (Real) 0.0)
	{
		p_vecClosestPoint = Start;
		return vecPointOffset.Length();
	}
	else if (rLineParameter < (Real) 1.0)
	{
		p_vecClosestPoint = Start + vecSegmentOffset * rLineParameter;
		return (vecPointOffset - vecSegmentOffset * rLineParameter).Length();
	}
	else
	{
		return (p_vecPoint - End).Length();
	}
}

TVector3<Real> LineSegment::ClosestPoint(const TVector3<Real>& p_vecPoint) const
{
	TVector3<Real> vecSegmentOffset(End - Start);

	if (vecSegmentOffset.IsZero())
		return Start;

	TVector3<Real> vecPointOffset(p_vecPoint - Start);
	Real rLineParameter = (vecSegmentOffset * vecPointOffset) / vecSegmentOffset.LengthSquared(); 

	if (rLineParameter <= (Real) 0.0)
	{
		return Start;
	}
	else if (rLineParameter < (Real) 1.0)
	{
		return Start + vecSegmentOffset * rLineParameter;
	}
	else
	{
		return End;
	}
}

void LineSegment::ClosestPointsToSegment(
	const LineSegment& p_lineSegment,
	TVector3<Real>& p_vecPoint1, 
	TVector3<Real>& p_vecPoint2)
{
	// compute segment offsets and sequare lengths
	TVector3<Real> vecOffset1 = Offset();
	TVector3<Real> vecOffset2 = p_lineSegment.Offset();
	TVector3<Real> vecStartOffset = Start - p_lineSegment.Start;
	Real rLengthSquared1 = vecOffset1.LengthSquared();
	Real rLengthSquared2 = vecOffset2.LengthSquared();
	Real rDot2 = vecOffset2 * vecStartOffset;

	if (TMaths<Real>::Equals(rLengthSquared1, (Real) 0.0)
		&& TMaths<Real>::Equals(rLengthSquared2, (Real) 0.0))
	{
		// both segments are degenerate i.e. points
		p_vecPoint1 = Start;
		p_vecPoint2 = p_lineSegment.Start; 
		return;
	}

	Real rParam1, rParam2;
	if (TMaths<Real>::Equals(rLengthSquared1, (Real) 0.0))
	{
		// only first segment degenerate
		rParam1 = (Real) 0.0;
		rParam2 = TMaths<Real>::Clamp(rDot2 / rLengthSquared2);
	}
	else
	{
		Real rDot1 = vecOffset1 * vecStartOffset;
		if (TMaths<Real>::Equals(rLengthSquared2, (Real) 0.0))
		{
			// only second segment degenerate
			rParam2 = (Real) 0.0;
			rParam1 = TMaths<Real>::Clamp(-rDot1 / rLengthSquared1);
		}
		else
		{
			// both segments non-degenerate (general case)
			Real rDotOffsets = vecOffset1 * vecOffset2;
			Real rDenominator = rLengthSquared1 * rLengthSquared2 - rDotOffsets * rDotOffsets; // (always >= 0)

			if (!TMaths<Real>::Equals(rDenominator, (Real) 0.0))
				// segments non-parallel - compute parameter for closest point
				// on first line
				rParam1 = TMaths<Real>::Clamp((rDotOffsets * rDot2 - rDot1 * rLengthSquared2) / rDenominator);
			else
				// segments parallel - assume parameter for start pt
				rParam1 = (Real) 0.0;

			// compute parameter for second segment
			rParam2 = (rDotOffsets * rParam1 + rDot2) / rLengthSquared2;

			// clamp first parameter to 0 and compute
			// second parameter accordingly, clamped to segment
			if (rParam2 < (Real) 0.0)
			{
				rParam2 = (Real) 0.0;
				rParam1 = TMaths<Real>::Clamp(-rDot1 / rLengthSquared1);
			}

			// clamp first parameter to 1 and compute
			// second parameter accordingly, clamped to segment
			if (rParam2 > (Real) 1.0)
			{
				rParam2 = (Real) 1.0;
				rParam1 = TMaths<Real>::Clamp((rDotOffsets - rDot1) / rLengthSquared1);
			}
		}
	}

	// compute closest points by applying parameters to line eqns
	p_vecPoint1 = Start + vecOffset1 * rParam1;
	p_vecPoint2 = p_lineSegment.Start + vecOffset2 * rParam2;
}

void LineSegment::Invert(void)
{
	TVector3<Real> vecTemp = Start;
	Start = End;
	End = vecTemp;
}

void LineSegment::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	p_transform.Apply(Start);
	p_transform.Apply(End);
}

LineSegment LineSegment::TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	return LineSegment(p_transform.ApplyCopy(Start), p_transform.ApplyCopy(End));
}

LineSegment& LineSegment::operator=(const LineSegment& p_lineSegment)
{
	Start = p_lineSegment.Start;
	End = p_lineSegment.End;
	return *this;
}

bool LineSegment::operator==(const LineSegment& p_lineSegment) const
{
	if (Start != p_lineSegment.Start) return false;
	if (End != p_lineSegment.End) return false;
	return true;
}

