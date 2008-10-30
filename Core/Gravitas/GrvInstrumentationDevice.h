#pragma once

#include "Pointer.h"
#include "String8.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"

Meson_Gravitas_Instrumentation_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Colour
/// Colour representation for the instrumentation device interface.
//----------------------------------------------------------------------------------------------
struct Colour
{
	//----------------------------------------------------------------------------------------------
	/// Black predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Black;

	//----------------------------------------------------------------------------------------------
	/// Blue predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Blue;

	//----------------------------------------------------------------------------------------------
	/// Green predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Green;

	//----------------------------------------------------------------------------------------------
	/// Cysn predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Cyan;

	//----------------------------------------------------------------------------------------------
	/// Red predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Red;

	//----------------------------------------------------------------------------------------------
	/// Magenta predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Magenta;

	//----------------------------------------------------------------------------------------------
	/// Yellow predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour Yellow;

	//----------------------------------------------------------------------------------------------
	/// White predefined static colour.
	//----------------------------------------------------------------------------------------------
	static const Colour White;

	//----------------------------------------------------------------------------------------------
	/// Red colour component. Ranges between 0.0 and 1.0.
	//----------------------------------------------------------------------------------------------
	Real R;

	//----------------------------------------------------------------------------------------------
	/// Green colour component. Ranges between 0.0 and 1.0.
	//----------------------------------------------------------------------------------------------
	Real G;

	//----------------------------------------------------------------------------------------------
	/// Blue colour component. Ranges between 0.0 and 1.0.
	//----------------------------------------------------------------------------------------------
	Real B;

	//----------------------------------------------------------------------------------------------
	/// Constructs an instrumentation colour, initialised to white.
	//----------------------------------------------------------------------------------------------
	Colour(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs an instrumentation colour using the given source.
	/// \param p_colour Source colour to copy.
	//----------------------------------------------------------------------------------------------
	Colour(const Colour& p_colour);

	//----------------------------------------------------------------------------------------------
	/// Constructs an instrumentation colour using the given red, green and blue components.
	/// \param p_rRed Red colour component.
	/// \param p_rGreen Green colour component.
	/// \param p_rBlue Blue colour component.
	//----------------------------------------------------------------------------------------------
	Colour(Real p_rRed, Real p_rGreen, Real p_rBlue);

	//----------------------------------------------------------------------------------------------
	/// Assigns the given colour to this colour.
	/// \param p_colour Source colour to copy.
	/// \returns a reference to this colour.
	//----------------------------------------------------------------------------------------------
	Colour& operator=(const Colour& p_colour);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the givne colour is equal to this colour.
	/// \returns true if the givne colour is equal to this colour.
	//----------------------------------------------------------------------------------------------
	bool operator==(const Colour& p_colour) const;
};

namespace DrawMode
{
	//----------------------------------------------------------------------------------------------
	/// Instrumentation draw mode enumeration.
	//----------------------------------------------------------------------------------------------
	enum DrawMode
	{
		Outline,
		Filled
	};
}

//----------------------------------------------------------------------------------------------
/// \class IInstrumentationDevice
/// Abstract instrumentation device interface for rendering profiling and debugging information
/// related to a simulation. An implementation for the interface is usually developed using the
/// same 3D rendering technology used for generating the visuals for the simulation itself.
/// The interface represents a stateful graphics device that specifies a current world
/// transform, rendering colour and a drawing mode. In addition, the device supports a state
/// stack to allow hierarchical rendering. The device provides functions for rendering text,
/// points, lines, arrows, spheres and boxes. The instrumentation device is used by the
/// InstrumentationManager depending on the rendering flags set and provided that an explicit
/// call to RenderInstrumentation is made from the simulation loop.
//----------------------------------------------------------------------------------------------
class IInstrumentationDevice
	: public Meson::Common::TReferenceCounter<IInstrumentationDevice>
{
public:
	virtual ~IInstrumentationDevice(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the current world transform as a 4x4 matrix.
	/// \returns the current world transform as a 4x4 matrix.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Maths::TMatrix4<Real>& GetTransform(void)= 0;

	//----------------------------------------------------------------------------------------------
	/// Sets the current world transform.
	/// \param p_matTransform Current world transform as a 4x4 matrix.
	//----------------------------------------------------------------------------------------------
	virtual void SetTransform(
		const Meson::Common::Maths::TMatrix4<Real>& p_matTransform) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns the current rendering colour.
	/// \returns the current rendering colour.
	//----------------------------------------------------------------------------------------------
	virtual const Colour& GetColour(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Sets the current rendering colour.
	/// \param p_colour Instrumentation device colour to apply.
	//----------------------------------------------------------------------------------------------
	virtual void SetColour(const Colour& p_colour) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns the current draw mode of the device.
	/// \returns the current draw mode of the device.
	//----------------------------------------------------------------------------------------------
	virtual DrawMode::DrawMode GetDrawMode(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Sets the current draw mode of the device.
	/// \param p_drawMode Draw mode to set for the device.
	//----------------------------------------------------------------------------------------------
	virtual void SetDrawMode(DrawMode::DrawMode p_drawMode) = 0;

	//----------------------------------------------------------------------------------------------
	/// Pushes the current device state onto the device's state stack. The state consists of the
	/// current world transform, rendering colour and draw mode.
	//----------------------------------------------------------------------------------------------
	virtual void PushDeviceState(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Pops the current device state from the device's state stack. The state consists of the
	/// current world transform, rendering colour and draw mode.
	//----------------------------------------------------------------------------------------------
	virtual void PopDeviceState(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders the given text at the given world position. This function is typically used to
	/// label objects or display object properties in the world. The text should be rendered with
	/// the current device colour and the position should be transformed using the current world
	/// transform.
	/// \param p_vecPosition World coordinates to render the text.
	/// \param p_strText Text to render at the givne position.
	//----------------------------------------------------------------------------------------------
	virtual void DrawText(
		const Meson::Common::Maths::TVector3<Real>& p_vecPosition,
		const Meson::Common::Text::String& p_strText) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a point with the current device colour at the given world position. The position
	/// should be transformed using the current world transform.
	/// \param p_vecPosition World coordinate where to render the point.
	//----------------------------------------------------------------------------------------------
	virtual void DrawPoint(
		const Meson::Common::Maths::TVector3<Real>& p_vecPosition) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a line with the given device colour using the given points transformed using the
	/// current world transform.
	/// \param p_vecStart Starting point for the line.
	/// \param p_vecEnd Ending point for the line.
	//----------------------------------------------------------------------------------------------
	virtual void DrawLine(
		const Meson::Common::Maths::TVector3<Real>& p_vecStart,
		const Meson::Common::Maths::TVector3<Real>& p_vecEnd) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a representation of an arrow with the given device colour using the given points
	/// transformed using the current world transform.
	/// \param p_vecHead Position representing the head of the arrow.
	/// \param p_vecTail Position representing the tail of the arrow.
	//----------------------------------------------------------------------------------------------
	virtual void DrawArrow(
		const Meson::Common::Maths::TVector3<Real>& p_vecHead,
		const Meson::Common::Maths::TVector3<Real>& p_vecTail) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a sphere using the current device colour with the given centre transformed using
	/// the current world transform and the given radius.
	/// \param p_vecCentre Sphere centre.
	/// \param p_rRadius Sphere radius.
	//----------------------------------------------------------------------------------------------
	virtual void DrawSphere(
		const Meson::Common::Maths::TVector3<Real>& p_vecCentre,
		Real p_rRadius) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a box using the current device colour with the given box centre transformed using
	/// the current world transform and the given extents also transformed. Prior to transformation,
	/// the box is axis-aligned.
	/// \param p_vecCentre Box centroid.
	/// \param p_vecExtents A vector of box extents.
	//----------------------------------------------------------------------------------------------
	virtual void DrawBox(
		const Meson::Common::Maths::TVector3<Real>& p_vecCentre,
		const Meson::Common::Maths::TVector3<Real>& p_vecExtents) = 0;

	//----------------------------------------------------------------------------------------------
	/// Renders a box using the current device colour with the given box centre and extents. The
	/// box is transformed by the given orientation and also the current world transform.
	/// \param p_matOrientation Box orientation prior to world transform.
	/// \param p_vecCentre Box centroid.
	/// \param p_vecExtents A vector of box extents.
	//----------------------------------------------------------------------------------------------
	virtual void DrawBox(
		const Meson::Common::Maths::TMatrix3<Real>& p_matOrientation,
		const Meson::Common::Maths::TVector3<Real>& p_vecCentre,
		const Meson::Common::Maths::TVector3<Real>& p_vecExtents) = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to an instrumentation device.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IInstrumentationDevice> InstrumentationDevicePtr;

Meson_Gravitas_Instrumentation_END
