//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstCamera.h"
#include "VstRenderDevice.h"

#include "Maths.h"
#include "Vector4.h"
#include "Matrix4.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
void Camera::EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, PropertyDescriptor> p_propertyDescriptorMap)
{
	p_propertyDescriptorMap.Insert("Position", PropertyDescriptor("Position", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Target", PropertyDescriptor("Target", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Direction", PropertyDescriptor("Direction", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("VerticalAxis", PropertyDescriptor("VerticalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("HorizontalAxis", PropertyDescriptor("HorizontalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Frustum", PropertyDescriptor("Frustum", PropertyDescriptor::PT_VECTOR4, PropertyDescriptor::PC_SCALAR, false));
}
//----------------------------------------------------------------------------------------------
void Camera::GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor)
{
	if (!p_strPropertyName.Compare("Position"))
		p_propertyDescriptor =  PropertyDescriptor("Position", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Target"))
		p_propertyDescriptor =  PropertyDescriptor("Target", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Direction"))
		p_propertyDescriptor =  PropertyDescriptor("Direction", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		p_propertyDescriptor =  PropertyDescriptor("VerticalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		p_propertyDescriptor =  PropertyDescriptor("HorizontalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Frustum"))
		p_propertyDescriptor =  PropertyDescriptor("Frustum", PropertyDescriptor::PT_VECTOR4, PropertyDescriptor::PC_SCALAR, false);
	else
		throw new MesonException("Property set does not contain specified property!", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
bool Camera::ContainsProperty(const Meson::Common::Text::String& p_strPropertyName)
{
	return (!p_strPropertyName.Compare("Position") ||
		!p_strPropertyName.Compare("Target") ||
		!p_strPropertyName.Compare("Direction") ||
		!p_strPropertyName.Compare("VerticalAxis") ||
		!p_strPropertyName.Compare("HorizontalAxis") ||
		!p_strPropertyName.Compare("Frustum"));
}
//----------------------------------------------------------------------------------------------
void Camera::SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector3f& p_vector3)
{
	if (!p_strPropertyName.Compare("Position"))
		SetPosition(p_vector3);
	else if (!p_strPropertyName.Compare("Target"))
		LookAt(GetPosition(), p_vector3, GetVerticalAxis());
	else if (!p_strPropertyName.Compare("Direction"))
		SetDirection(p_vector3, true);
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		SetVerticalAxis(p_vector3, true);
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		SetHorizontalAxis(p_vector3, true);
}
//----------------------------------------------------------------------------------------------
void Camera::SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector4f& p_vector4)
{
	if (!p_strPropertyName.Compare("Frustum"))
		SetFrustum(p_vector4.X, p_vector4.Y, p_vector4.Z, p_vector4.W);
}
//----------------------------------------------------------------------------------------------
void Camera::GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector3f& p_vector3) const
{
	if (!p_strPropertyName.Compare("Position"))
		p_vector3 = GetPosition();
	else if (!p_strPropertyName.Compare("Direction") || !p_strPropertyName.Compare("Target"))
		p_vector3 = GetDirection();
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		p_vector3 = GetVerticalAxis();
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		p_vector3 = GetHorizontalAxis();
}
//----------------------------------------------------------------------------------------------
void Camera::GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector4f& p_vector4) const
{
	if (!p_strPropertyName.Compare("Frustum"))
		Camera::GetFrustum(p_vector4.Element[0], p_vector4.Element[1], p_vector4.Element[2], p_vector4.Element[3]);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
Camera::Camera(void)
	: VistasObject()
	, m_cameraProjection(CP_PERSPECTIVE)
{
}
//----------------------------------------------------------------------------------------------
Camera::Camera(const String& p_strName)
	: VistasObject(p_strName)
	, m_cameraProjection(CP_PERSPECTIVE)
{
}
//----------------------------------------------------------------------------------------------
Camera::~Camera(void)
{
}
//----------------------------------------------------------------------------------------------
void Camera::Reset(void)
{
	m_cameraProjection = CP_PERSPECTIVE;

	Camera::SetFrustum(Mathf::DegreeToRadian(45.0f), 1.25f, 1.0f, 100.0f);
	Camera::LookAt(Vector3f::Zero, Vector3f::Out, Vector3f::Up);
}
//----------------------------------------------------------------------------------------------
Camera::CameraProjection Camera::GetProjection(void) const
{
	return m_cameraProjection;
}
//----------------------------------------------------------------------------------------------
void Camera::SetProjection(const Camera::CameraProjection p_eCameraProjection)
{
	m_cameraProjection = p_eCameraProjection;
}
//----------------------------------------------------------------------------------------------
void Camera::SetPosition(const Vector3f& p_vec3Position)
{
	m_vec3Position = p_vec3Position;

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
void Camera::SetDirection(const Vector3f& p_vec3Direction, bool p_bNormalise)
{
	m_vec3Direction = p_vec3Direction;
	
	if (p_bNormalise)
		m_vec3Direction.Normalise();

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
void Camera::SetHorizontalAxis(const Vector3f& p_vec3HorizontalAxis, bool p_bNormalise)
{
	m_vec3Right = p_vec3HorizontalAxis;

	if (p_bNormalise)
		m_vec3Right.Normalise();

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
void Camera::SetVerticalAxis(const Vector3f& p_vec3VerticalAxis, bool p_bNormalise)
{
	m_vec3Up = p_vec3VerticalAxis;

	if (p_bNormalise)
		m_vec3Up.Normalise();

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
void Camera::SetAxes(const Vector3f& p_vec3Direction, const Vector3f& p_vec3VerticalAxis, 
					 const Vector3f& p_vec3HorizontalAxis, bool p_bNormaliseAxes)
{
	m_vec3Direction = p_vec3Direction;
	m_vec3Up = p_vec3VerticalAxis;
	m_vec3Right = p_vec3HorizontalAxis;

	if (p_bNormaliseAxes)
	{
		m_vec3Up.Normalise();
		m_vec3Right.Normalise();
		m_vec3Direction.Normalise();
	}

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
void Camera::SetFrame(const Vector3f &p_vec3Position, const Vector3f &p_vec3Direction, 
					  const Vector3f &p_vec3VerticalAxis, const Vector3f &p_vec3HorizontalAxis,
					  bool p_bNormaliseAxes)
{
	m_vec3Position = p_vec3Position;
	m_vec3Direction = p_vec3Direction;
	m_vec3Up = p_vec3VerticalAxis;
	m_vec3Right = p_vec3HorizontalAxis;

	if (p_bNormaliseAxes)
	{
		m_vec3Up.Normalise();
		m_vec3Right.Normalise();
		m_vec3Direction.Normalise();
	}

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
const Vector3f& Camera::GetPosition(void) const
{
	return m_vec3Position;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Camera::GetDirection(void) const
{
	return m_vec3Direction;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Camera::GetHorizontalAxis(void) const
{
	return m_vec3Right;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Camera::GetVerticalAxis(void) const
{
	return m_vec3Up;
}
//----------------------------------------------------------------------------------------------
Vector3f Camera::GetPickingDirection(float p_fXPosition, float p_fYPosition)
{
	Vector3f vec3PickingDirection = m_fFrustum[CF_ZMIN] * m_vec3Direction;

	vec3PickingDirection  -= m_vec3Right * p_fXPosition * (m_fFrustum[CF_XMAX] - m_fFrustum[CF_XMIN]) * 0.5f;
	vec3PickingDirection  -= m_vec3Up * p_fYPosition * (m_fFrustum[CF_YMAX] - m_fFrustum[CF_YMIN]) * 0.5f;

	vec3PickingDirection.Normalise();

	return vec3PickingDirection;
}
//----------------------------------------------------------------------------------------------
void Camera::SetFrustum(float p_fFieldOfView, float p_fAspectRatio, float p_fZMin, float p_fZMax)
{
	m_fFieldOfView = p_fFieldOfView;
	m_fAspectRatio = p_fAspectRatio;

	m_fFrustum[CF_ZMAX] = p_fZMax;
	m_fFrustum[CF_ZMIN] = p_fZMin;
	m_fFrustum[CF_YMAX] = m_fFrustum[CF_ZMIN] * Mathf::Tan(p_fFieldOfView * 0.5f);
	m_fFrustum[CF_YMIN] = -m_fFrustum[CF_YMAX];
	m_fFrustum[CF_XMAX] = m_fAspectRatio * m_fFrustum[CF_YMAX];
	m_fFrustum[CF_XMIN] = -m_fFrustum[CF_XMAX];

	InvalidateFrustum();
}
//----------------------------------------------------------------------------------------------
void Camera::SetFrustum(float p_fXMin, float p_fXMax, float p_fYMin, float p_fYMax, float p_fZMin, float p_fZMax)
{
	m_fFrustum[CF_ZMAX] = p_fZMax;
	m_fFrustum[CF_ZMIN] = p_fZMin;
	m_fFrustum[CF_YMAX] = p_fYMax;
	m_fFrustum[CF_YMIN] = p_fYMin;
	m_fFrustum[CF_XMAX] = p_fXMax;
	m_fFrustum[CF_XMIN] = p_fXMin;

	m_fFieldOfView = Mathf::ATan((p_fZMax * 2.0f) / (p_fYMax - p_fYMin));
	m_fAspectRatio = (p_fXMax - p_fXMin) / (p_fYMax - p_fYMin);

	InvalidateFrustum();
}
//----------------------------------------------------------------------------------------------
void Camera::GetFrustum(float& p_fFieldOfView, float& p_fAspectRatio, float& p_fZMin, float& p_fZMax) const
{
	p_fFieldOfView = m_fFieldOfView;
	p_fAspectRatio = m_fAspectRatio;
	p_fZMin = m_fFrustum[CF_ZMIN];
	p_fZMax = m_fFrustum[CF_ZMAX];
}
//----------------------------------------------------------------------------------------------
void Camera::GetFrustum(float& p_fXMin, float& p_fXMax, float& p_fYMin, float& p_fYMax, float& p_fZMin, float& p_fZMax) const
{
	p_fZMin = m_fFrustum[CF_ZMIN];
	p_fZMax = m_fFrustum[CF_ZMAX];
	p_fYMin = m_fFrustum[CF_YMIN];
	p_fYMax = m_fFrustum[CF_YMAX];
	p_fXMin = m_fFrustum[CF_XMIN];
	p_fXMax = m_fFrustum[CF_XMAX];
}
//----------------------------------------------------------------------------------------------
const Matrix4f& Camera::GetViewMatrix(void)
{
	Update();

	return m_mat4View; 
}
//----------------------------------------------------------------------------------------------
void Camera::LookAt(const Vector3f& p_vec3Position, const Vector3f& p_vec3Target, const Vector3f& p_vec3UpVector)
{
	m_vec3Position = p_vec3Position;
	
	m_vec3Direction = p_vec3Target - p_vec3Position;
	m_vec3Direction.Normalise();

	m_vec3Right = p_vec3UpVector.CrossProduct(m_vec3Direction);
	m_vec3Right.Normalise();

	m_vec3Up = m_vec3Direction.CrossProduct(m_vec3Right);
	m_vec3Up.Normalise();

	InvalidateView();
}
//----------------------------------------------------------------------------------------------
bool Camera::HasViewChanged(void) const
{
	return m_bHasViewChanged;
}
//----------------------------------------------------------------------------------------------
bool Camera::HasFrustumChanged(void) const
{
	return m_bHasFrustumChanged;
}
//----------------------------------------------------------------------------------------------
void Camera::InvalidateView(void)
{
	m_bHasViewChanged = true;
}
//----------------------------------------------------------------------------------------------
void Camera::InvalidateFrustum(void)
{
	m_bHasFrustumChanged = true;
}
//----------------------------------------------------------------------------------------------
void Camera::UpdateView(IRenderDevice* p_pRenderDevice)
{
	Update();

	p_pRenderDevice->SetViewMatrix(m_mat4View);
}
//----------------------------------------------------------------------------------------------
void Camera::UpdateProjection(Meson::Vistas::IRenderDevice *p_pRenderDevice)
{
	if (HasFrustumChanged())
	{
		m_cameraProjection == CP_PERSPECTIVE 
			? p_pRenderDevice->MakePerspectiveProjection(m_fFrustum[CF_XMIN], m_fFrustum[CF_XMAX], m_fFrustum[CF_YMAX], m_fFrustum[CF_YMIN], m_fFrustum[CF_ZMIN], m_fFrustum[CF_ZMAX], &m_mat4Projection)
			: p_pRenderDevice->MakeOrthographicProjection(m_fFrustum[CF_XMIN], m_fFrustum[CF_XMAX], m_fFrustum[CF_YMAX], m_fFrustum[CF_YMIN], m_fFrustum[CF_ZMIN], m_fFrustum[CF_ZMAX], &m_mat4Projection);

		m_bHasFrustumChanged = false;
	}
	
	p_pRenderDevice->SetProjectionMatrix(m_mat4Projection);
}
//----------------------------------------------------------------------------------------------
void Camera::Update(void)
{
	if (m_bHasViewChanged)
	{
		m_mat4View._11 = m_vec3Right.X;
		m_mat4View._12 = m_vec3Right.Y;
		m_mat4View._13 = m_vec3Right.Z;
		m_mat4View._14 = -m_vec3Right.DotProduct(m_vec3Position);

		m_mat4View._21 = m_vec3Up.X;
		m_mat4View._22 = m_vec3Up.Y;
		m_mat4View._23 = m_vec3Up.Z;
		m_mat4View._24 = -m_vec3Up.DotProduct(m_vec3Position);

		m_mat4View._31 = m_vec3Direction.X;
		m_mat4View._32 = m_vec3Direction.Y;
		m_mat4View._33 = m_vec3Direction.Z;
		m_mat4View._34 = -m_vec3Direction.DotProduct(m_vec3Position);

		m_mat4View._41 = m_mat4View._42 = m_mat4View._43 = 0.0f;
		m_mat4View._44 = 1.0f;

		UpdateFrustumPlanes();

		m_bHasViewChanged = false;
	}
}
//----------------------------------------------------------------------------------------------
const Planef& Camera::GetFrustumPlane(FrustumPlane p_eFrustumPlane) const
{
	return m_frustumPlanes[p_eFrustumPlane];
}
//----------------------------------------------------------------------------------------------
void Camera::UpdateFrustumPlanes(void)
{
	float fDepth = m_fFrustum[CF_ZMAX] - m_fFrustum[CF_ZMIN],
	      fHalfWidth = 0.5f * (m_fFrustum[CF_XMAX] - m_fFrustum[CF_XMIN]),
		  fHalfHeight = 0.5f * (m_fFrustum[CF_YMAX] - m_fFrustum[CF_YMIN]);

	// Near and far clipping planes
	m_frustumPlanes[CF_ZMAX].Normal = m_vec3Direction;
	m_frustumPlanes[CF_ZMAX].Distance = m_frustumPlanes[CF_ZMAX].Normal.DotProduct(m_vec3Position) + m_fFrustum[CF_ZMAX];

	m_frustumPlanes[CF_ZMIN].Normal = -m_vec3Direction;
	m_frustumPlanes[CF_ZMIN].Distance = m_frustumPlanes[CF_ZMIN].Normal.DotProduct(m_vec3Position) - m_fFrustum[CF_ZMIN];


	/**	Right clip plane:
		S = norm |F * near + R * (right - left) / 2|
		N = U x S
		D = N . P
	 */

	Vector3f vec3Sum = m_vec3Direction * m_fFrustum[CF_ZMIN] + m_vec3Right * fHalfWidth; vec3Sum.Normalise();
	m_frustumPlanes[CF_XMAX].Normal = m_vec3Up.CrossProduct(vec3Sum);
	m_frustumPlanes[CF_XMAX].Distance = m_frustumPlanes[CF_XMAX].Normal.DotProduct(m_vec3Position);

	/**	Left clip plane:
		S = norm |F * near - R * (right - left) / 2|
		N = S x U
		D = N . P
	 */

	vec3Sum = m_vec3Direction * m_fFrustum[CF_ZMIN] - m_vec3Right * fHalfWidth; vec3Sum.Normalise();
	m_frustumPlanes[CF_XMIN].Normal = vec3Sum.CrossProduct(m_vec3Up);
	m_frustumPlanes[CF_XMIN].Distance = m_frustumPlanes[CF_XMIN].Normal.DotProduct(m_vec3Position);

	/**	Top clip plane:
		S = norm |F * near - U * (top - bottom) / 2|
		N = R x S
		D = N . P
	 */

	vec3Sum = m_vec3Direction * m_fFrustum[CF_ZMIN] - m_vec3Up * fHalfHeight; vec3Sum.Normalise();
	m_frustumPlanes[CF_YMAX].Normal = m_vec3Right.CrossProduct(vec3Sum);
	m_frustumPlanes[CF_YMAX].Distance = m_frustumPlanes[CF_YMAX].Normal.DotProduct(m_vec3Position);

	/**	Bottom clip plane:
		S = norm |F * near + U * (top - bottom) / 2|
		N = S x R
		D = N . P
	 */

	vec3Sum = m_vec3Direction * m_fFrustum[CF_ZMIN] + m_vec3Up * fHalfHeight; vec3Sum.Normalise();
	m_frustumPlanes[CF_YMIN].Normal = vec3Sum.CrossProduct(m_vec3Right);
	m_frustumPlanes[CF_YMIN].Distance = m_frustumPlanes[CF_YMIN].Normal.DotProduct(m_vec3Position);
}
//----------------------------------------------------------------------------------------------
bool Camera::operator==(const Camera& p_camera) const
{
	return 
		m_strId.Compare(p_camera.m_strId) == 0 &&
		m_cameraProjection == p_camera.m_cameraProjection &&
		
		m_vec3Position == p_camera.m_vec3Position &&
		m_vec3Direction == p_camera.m_vec3Direction &&
		m_vec3Right == p_camera.m_vec3Right &&
		m_vec3Up == p_camera.m_vec3Up &&

		Mathf::Equals(m_fFrustum[CF_ZMIN], p_camera.m_fFrustum[CF_ZMIN]) &&
		Mathf::Equals(m_fFrustum[CF_ZMAX], p_camera.m_fFrustum[CF_ZMAX]) &&
		Mathf::Equals(m_fFrustum[CF_YMIN], p_camera.m_fFrustum[CF_YMIN]) &&
		Mathf::Equals(m_fFrustum[CF_YMAX], p_camera.m_fFrustum[CF_YMAX]) &&
		Mathf::Equals(m_fFrustum[CF_XMIN], p_camera.m_fFrustum[CF_XMIN]) &&
		Mathf::Equals(m_fFrustum[CF_XMAX], p_camera.m_fFrustum[CF_XMAX]);
}
//----------------------------------------------------------------------------------------------
Camera& Camera::operator=(const Camera& p_camera)
{
	m_strId = p_camera.m_strId;

	m_cameraProjection = p_camera.m_cameraProjection;

	m_fFieldOfView = p_camera.m_fFieldOfView;
	m_fAspectRatio = p_camera.m_fAspectRatio;

	for (uint uiFrustumPlane = 0; uiFrustumPlane < CF_COUNT; uiFrustumPlane++)
	{
		m_fFrustum[uiFrustumPlane] = p_camera.m_fFrustum[uiFrustumPlane];
		m_frustumPlanes[uiFrustumPlane] = p_camera.m_frustumPlanes[uiFrustumPlane];
	}

	m_vec3Up = p_camera.m_vec3Up;
	m_vec3Right = p_camera.m_vec3Right;
	m_vec3Direction = p_camera.m_vec3Direction;
	m_vec3Position = p_camera.m_vec3Position;

	m_mat4View = p_camera.m_mat4View;

	m_bHasViewChanged = p_camera.m_bHasViewChanged;
	m_bHasFrustumChanged = p_camera.m_bHasFrustumChanged;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
