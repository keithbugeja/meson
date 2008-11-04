//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Viewport
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstCamera.h"
#include "VstRenderTarget.h"

Meson_Vistas_BEGIN

/** NOTE: 1. Should viewport be made aware that it is currently active?  
		  2. If viewport is made aware that it is currently active, should
			 render device binding also be included to show activity scope?
 */
class VISTAS_API Viewport
{
public:
	//----------------------------------------------------------------------------------------------
	/** The viewport's only constructor
	 */
	//----------------------------------------------------------------------------------------------
	Viewport(RenderTarget* p_pRenderTarget, Camera* p_pCamera, uint p_uiPriorityIndex, float p_fLeft, float p_fTop, float p_fWidth, float p_fHeight, float p_fNear, float p_fFar);
	
	//----------------------------------------------------------------------------------------------
	/** Sets or gets the camera attached to the viewport
	 */
	//----------------------------------------------------------------------------------------------
	void SetCamera(Camera* p_pCamera);
	Camera* GetCamera(void);

	//----------------------------------------------------------------------------------------------
	/** Updates the viewport; if the force flag is set, the camera is invalidated before 
	 *	the update takes place.
	 */
	//----------------------------------------------------------------------------------------------
	void Update(bool p_bForce=false);

	//----------------------------------------------------------------------------------------------
	/** The viewport's priority index
	 */
	//----------------------------------------------------------------------------------------------
	uint GetPriorityIndex(void);

	//----------------------------------------------------------------------------------------------
	/** Returns the owner of the viewport
	 */ 
	//----------------------------------------------------------------------------------------------
	RenderTarget* GetRenderTarget(void);

	//----------------------------------------------------------------------------------------------
	/** Sets or gets the viewport's frame
	 */
	//----------------------------------------------------------------------------------------------
	void SetFrame(float p_fLeft, float p_fTop, float p_fWidth, float p_fHeight);
	void GetFrame(float& p_fLeft, float& p_fTop, float& p_fWidth, float& p_fHeight);
	
	//----------------------------------------------------------------------------------------------
	/** Sets or gets the viewport's depth range
	 */
	//----------------------------------------------------------------------------------------------
	void SetDepthRange(float p_fNear, float p_fFar);
	void GetDepthRange(float& p_fNear, float& p_fFar);

	//----------------------------------------------------------------------------------------------
	/** Return the viewport's size
	 */
	//----------------------------------------------------------------------------------------------
	float GetFrameLeft(void);
	float GetFrameTop(void);
	float GetFrameWidth(void);
	float GetFrameHeight(void);

	float GetNearDepth(void);
	float GetFarDepth(void);

	//----------------------------------------------------------------------------------------------
	/** Return the viewport's size mapped to its parent render target
	 */
	//----------------------------------------------------------------------------------------------
	float GetMappedFrameLeft(void);
	float GetMappedFrameTop(void);
	float GetMappedFrameWidth(void);
	float GetMappedFrameHeight(void);

	//----------------------------------------------------------------------------------------------
	/** Return the viewport's size mapped to its parent render target
	 */
	//----------------------------------------------------------------------------------------------
	uint GetDeviceWidth(void);
	uint GetDeviceHeight(void);


protected:
	RenderTarget* m_pRenderTarget;
	Camera* m_pCamera;

	uint m_uiPriorityIndex;

	float m_fLeft,
		  m_fTop,
		  m_fWidth,
		  m_fHeight,
		  m_fNear,
		  m_fFar;
};

Meson_Vistas_END