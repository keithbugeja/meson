//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstPropertySet.h"
#include "VstVistasObject.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API Camera : public VistasObject, IPropertySet
	{
		VISTAS_IMPLEMENT_RTTI(Camera, VistasObject);

		friend class Viewport;
		
	public:
		enum CameraProjection
		{
			CP_PERSPECTIVE,
			CP_ORTHOGRAPHIC,
			CP_COUNT
		};

		enum FrustumPlane
		{
			CF_XMIN,
			CF_XMAX,
			CF_YMIN,
			CF_YMAX,
			CF_ZMIN,
			CF_ZMAX,
			CF_COUNT
		};

	protected:
		void UpdateFrustumPlanes(void);

		void UpdateView(IRenderDevice* p_pRenderDevice);
		void UpdateProjection(IRenderDevice* p_pRenderDevice);

		//----------------------------------------------------------------------------------------------
		// IPropertySet methods
		//----------------------------------------------------------------------------------------------
		void EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, 
			PropertyDescriptor> p_propertyDescriptorMap);
		void GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName,
			PropertyDescriptor& p_propertyDescriptor);
		bool ContainsProperty(const Meson::Common::Text::String& p_strPropertyName);

		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			const Meson::Common::Maths::Vector3f& p_vector3);
		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			const Meson::Common::Maths::Vector4f& p_vector4);
		
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			Meson::Common::Maths::Vector3f& p_vector3) const;
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			Meson::Common::Maths::Vector4f& p_vector4) const;
		//----------------------------------------------------------------------------------------------

	public:
		Camera(void);
		Camera(const Meson::Common::Text::String& p_strName);
		~Camera(void);

		void Reset(void);

		void Update(void);

		void InvalidateView(void);
		void InvalidateFrustum(void);

		bool HasViewChanged(void) const;
		bool HasFrustumChanged(void) const;

		void LookAt(const Meson::Common::Maths::Vector3f& p_vec3Position, 
			const Meson::Common::Maths::Vector3f& p_vec3Target, 
			const Meson::Common::Maths::Vector3f& p_vec3UpVector);

		CameraProjection GetProjection(void) const;
		void SetProjection(const CameraProjection p_eCameraProjection);

		void SetAxes(const Meson::Common::Maths::Vector3f& p_vec3Direction, 
			const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, 
			const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis,
			bool p_bNormaliseAxes = false);
		
		void SetFrame(const Meson::Common::Maths::Vector3f& p_vec3Position, 
			const Meson::Common::Maths::Vector3f& p_vec3Direction, 
			const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, 
			const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis,
			bool p_bNormaliseAxes = false);
	
		const Meson::Common::Maths::Vector3f& GetPosition(void) const;
		const Meson::Common::Maths::Vector3f& GetDirection(void) const;
		const Meson::Common::Maths::Vector3f& GetHorizontalAxis(void) const;
		const Meson::Common::Maths::Vector3f& GetVerticalAxis(void) const;

		void SetPosition(const Meson::Common::Maths::Vector3f& p_vec3Position);
		void SetDirection(const Meson::Common::Maths::Vector3f& p_vec3Direction, bool p_bNormalise = false);
		void SetHorizontalAxis(const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis, bool p_bNormalise = false);
		void SetVerticalAxis(const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, bool p_bNormalise = false);
		
		void SetFrustum(float p_fFieldOfView, float p_fAspectRatio, float p_fZMin, float p_fZMax);
		void SetFrustum(float p_fXMin, float p_fXMax, float p_fYMin, float p_fYMax, float p_fZMin, float p_fZMax);

		void GetFrustum(float& p_fFieldOfView, float& p_fAspectRatio, float& p_fZMin, float& p_fZMax) const;
		void GetFrustum(float& p_fXMin, float& p_fXMax, float& p_fYMin, float& p_fYMax, float& p_fZMin, float& p_fZMax) const;

		const Meson::Common::Maths::Matrix4f& GetViewMatrix(void);
		const Meson::Common::Maths::Planef& GetFrustumPlane(FrustumPlane p_eFrustumPlane) const;
		Meson::Common::Maths::Vector3f GetPickingDirection(float p_fXPosition, float p_fYPosition);

		Camera& operator=(const Camera& p_camera);
		bool operator==(const Camera& p_camera) const;
		
	protected:
		CameraProjection m_cameraProjection;

		bool m_bHasViewChanged,
			 m_bHasFrustumChanged;

		float m_fFieldOfView,
			  m_fAspectRatio;

		float m_fFrustum[CF_COUNT];

		Meson::Common::Maths::Vector3f m_vec3Up,
									   m_vec3Right,
									   m_vec3Direction,
									   m_vec3Position;

		Meson::Common::Maths::Matrix4f m_mat4View,
									   m_mat4Projection;

		Meson::Common::Maths::Planef m_frustumPlanes[CF_COUNT];
	};
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Camera> CameraPtr;
	typedef Meson::Common::Collections::TArrayList<CameraPtr> CameraList;
	typedef Meson::Common::Collections::TEnumerator<CameraPtr>& CameraEnumerator;
	typedef Meson::Common::TPointer<CameraList> CameraListPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
