//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstVisibleEntitySet.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	// TODO:
	// 1. Provide a means of enabling and disabling culling planes.
	// 2. Allow for additional user defined culling planes.
	// 3. Add a filter, to remove unwanted node categories. (IMPORTANT)

	class ICuller : public Meson::Common::TReferenceCounter<ICuller>
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Returns the culler type.
		 */
		//----------------------------------------------------------------------------------------------
		virtual const Meson::Common::Text::String& GetType(void) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates a copy of the culler.
		 */
		//----------------------------------------------------------------------------------------------
		virtual Meson::Common::TPointer<ICuller> Clone(bool p_bShallow = true) = 0;
		virtual Meson::Common::TPointer<ICuller> CreateInstance(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Manage clipping plane list
		 */
		//----------------------------------------------------------------------------------------------
		virtual uint GetPlaneCount(void) const = 0;
		virtual void SetPlane(uint p_uiPlaneIndex, bool p_bEnabled) = 0;
		virtual bool IsPlaneEnabled(uint p_uiPlaneIndex) const = 0;
		virtual void PushPlane(Meson::Common::Maths::Planef& p_plane) = 0;
		virtual void PopPlane(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the node used as root in the culler's operations.
			\param p_pLocalRootNode Node at which to start culling.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetRootNode(ISceneNode* p_pLocalRootNode) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the camera for the given culler.
			\param p_pCamera Pointer to a camera object
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetCamera(Camera* p_pCamera) = 0;

		//----------------------------------------------------------------------------------------------
		/** Insert a scene node into the potential visibility set.
			\param p_pVisibleEntity Node which has passed the culling test.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Push(ISceneNode* p_pVisibleEntity) = 0;

		//----------------------------------------------------------------------------------------------
		/** Push a composite node into the potential visibility set. Pushed nodes are expected to be
			popped after their respective children have been processed. For instance, a state node
			should be pushed before its children are processed and popped immediately after; failure
			to do so will inevitably cause other nodes to be affected by the state change too.
			\param p_pVisibleEntity Composite node to push on visibility queue.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual void Enter(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect) = 0;

		//----------------------------------------------------------------------------------------------
		/** Pop a previously pushed composite node from the visibility set.
			\param p_pVisibleEntity Composite node to pop from queue.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Leave(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect) = 0;

		//----------------------------------------------------------------------------------------------
		/** Determines the visibility of a scene node entity.
			\param p_pEntity Pointer to a node object.
			\return true if the object passes visibility tests.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual bool IsVisible(ISceneNode* p_pEntity) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Determines the visibility of a bounding volume.
			\param p_pBoundingVolume Pointer to a bounding volume object.
			\return true if the object passes visibility tests.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual bool IsVisible(IBoundingVolume* p_pBoundingVolume) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Builds the internal culling structures for complex cullers. 
			Complex partitioning schemes such as octrees may need precomputation to generate
			and subdivide space, which may be achived through implementing this method.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual void Rebuild(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Update internal culling structures for complex cullers.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual void Update(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the visibility set last computed.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual VisibleEntitySetPtr GetVisibleSet(void) = 0;
		virtual void SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet) = 0;

		//----------------------------------------------------------------------------------------------
		/** Forces computation of the visibility set.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual VisibleEntitySetPtr ComputeVisibleSet(bool p_bAccumulate=false) = 0;
	};

	typedef Meson::Common::TPointer<ICuller> CullerPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END