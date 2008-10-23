//----------------------------------------------------------------------------------------------
//	VstVistasApplication.h
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstWindowMessageHandler.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	//  \class VistasApplication
	//	The class provides a bare-bones framework for creating Vistas applications, which 
	//	initialises a rendering window, and provides a registry for scene nodes.
	//----------------------------------------------------------------------------------------------
	class VistasApplication : public WindowMessageHandler
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** \brief VistasApplication constructor.
			The constructor expects a render device name, width and height of the rendering canvas,
			and whether the application is expected to run in full screen mode. Moreover, an 
			instrumentation driver flag will, whenever possible and supported, force the application
			to be compliant with performance HUDs like NVIDIA's.
			\param p_strDeviceName	Name of the render device to use.
			\param p_strTitle Title for application window.
			\param p_uiWidth Width of rendering canvas.
			\param p_uiHeight Height of rendering canvas.
			\param p_bFullScreen Determines whether application is run in fullscreen or windowed modes.
			\param p_bInstrumentationDriver Determines whether application will launch instrumentation driver.
		 */
		//----------------------------------------------------------------------------------------------
		VistasApplication(const Meson::Common::Text::String& p_strDeviceName, const Meson::Common::Text::String& p_strTitle, 
			uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver);
		virtual ~VistasApplication();

		//----------------------------------------------------------------------------------------------
		/** \brief Run Vistas application.
			When executed, this function will load the plug-ins required by the application, perform 
			initialisation and call the Dispatch method, which executes until a termination signal 
			is received. When Dispatch returns, the application shutdown is called and the plug-ins 
			are unloaded.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Run(void);

		//----------------------------------------------------------------------------------------------
		/** \brief Templated function called to initialise plug-ins.
			This method must be implemented by any derived Application classes and used for loading
			plug-ins required by the Vistas engine to operate.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void OnLoadPlugIns(void);

		//----------------------------------------------------------------------------------------------
		/** \brief Templated function called to unload plug-ins when application terminates.
			This method must be implemented by any derived Application classes and used for unloading
			plug-ins formerly loaded by the application.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void OnUnloadPlugIns(void);
		
		//----------------------------------------------------------------------------------------------
		/** \brief Called when application is initialised.
			This method may be implemented by any derived Application classes and used to initialise
			the application environment. Note that the base VistasApplication object initialises a
			render device and therefore must be called, by derived classes if this functionality is to
			be retained.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void OnInitialise(void);

		//----------------------------------------------------------------------------------------------
		/** \brief Called when application is shut down.
			This method is counterpart to the OnInitialise function. Any initialisation carried out
			within OnInitialise must be undone within OnShutdown.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void OnShutdown(void);

		//----------------------------------------------------------------------------------------------
		/** \brief Called when application is idle.
			This method may be overloaded and implemented with the logic required by a specific 
			application.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void OnIdle(void);

	protected:
		//----------------------------------------------------------------------------------------------
		/** \brief Registers a scene node with the application registry.
			This method is used to register scene nodes with the application registry for easy
			retrieval within the application itself. Moreover, when the application terminates,
			the system automatically deletes the registered nodes.
			\param ISceneNode* Pointer to an ISceneNode-derived object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RegisterNode(ISceneNode* p_pNode);

		//----------------------------------------------------------------------------------------------
		/** \brief Returns a registered node by name.
			Nodes registered through the RegisterNode function may be queried for and returned 
			by their name.
			\param p_strId Id of registered node.
			\return A valid ISceneNode* if the node with the given name is found or NULL if the
					node has not been registered.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ISceneNode* GetNode(const Meson::Common::Text::String& p_strId);

		//----------------------------------------------------------------------------------------------
		/**	\brief Unregisters a node from the application registry.
			This method allows the unregistering of formerly registered scene nodes from the 
			application registry.
			\param p_pNode Pointer to the node object to unregister.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual void UnregisterNode(ISceneNode* p_pNode);
		
		//----------------------------------------------------------------------------------------------
		/**	\brief Unregisters a node from the application registry.
			This method allows the unregistering of formerly registered scene nodes from the 
			application registry.
			\param p_strId Id of the node object to unregister.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual void UnregisterNode(const Meson::Common::Text::String& p_strId);

		//----------------------------------------------------------------------------------------------
		/** \brief Frees all registered nodes.
			This method unregisters all nodes and frees their memory allocation.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyRegisteredNodes(void);

	protected:
		//----------------------------------------------------------------------------------------------
		/** \brief Returns the only instance of the VistasEngine.
			This method returns the only instance of the VistasEngine, since the VistasEngine is a
			singleton object. The singleton is aliased internally to help prevent problems with 
			static linking across libraries.
			\note This method provides an accessor to the \c m_pVistasEngine member.
		 */
		//----------------------------------------------------------------------------------------------
		VistasEngine* GetEngine(void) const { return m_pVistasEngine; }

		//----------------------------------------------------------------------------------------------
		/** \brief Returns the active Render device.
			\note This method provides an accessor to the \c m_pRenderDevice member.
		 */
		//----------------------------------------------------------------------------------------------
		IRenderDevice* GetRenderDevice(void) const { return m_pRenderDevice; }

		//----------------------------------------------------------------------------------------------
		/** \brief Returns the name of the active Render device.
			\note This method provides an accessor to the \c m_strDeviceName member.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetDeviceName(void) { return m_strDeviceName; }

		//----------------------------------------------------------------------------------------------
		/** \brief Returns the title of the application window.
			\note This method provides an accessor to the \c m_strTitle member.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetTitle(void) { return m_strTitle; }
		
		//----------------------------------------------------------------------------------------------
		/** \brief Returns the original width in pixels of the application window.
			\note This method provides an accessor to the \c m_uiWidth member.
		 */
		//----------------------------------------------------------------------------------------------
		uint GetWidth(void) const { return m_uiWidth; }

		//----------------------------------------------------------------------------------------------
		/** \brief Returns the original height in pixels of the application window.
			\note This method provides an accessor to the \c m_uiHeight member.
		 */
		//----------------------------------------------------------------------------------------------
		uint GetHeight(void) const { return m_uiHeight; }
		
		//----------------------------------------------------------------------------------------------
		/** \brief Returns whether the application is running an instrumentation driver or not.
			\note This method provides an accessor to the \c m_bIsInstrumentationDriver member.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsInstrumentationDriver(void) const { return m_bIsInstrumentationDriver; }

		//----------------------------------------------------------------------------------------------
		/** \brief Returns whether the application was originally started in full screen mode.
			\note This method provides an accessor to the \c m_bIsFullScreen member.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsFullScreen(void) const { return m_bIsFullScreen; }

	protected:
		VistasEngine*	m_pVistasEngine;
		IRenderDevice*	m_pRenderDevice;

		const Meson::Common::Text::String m_strDeviceName,
										  m_strTitle;

		uint m_uiWidth, 
			 m_uiHeight;

		bool m_bIsInstrumentationDriver,
			 m_bIsFullScreen;

		Meson::Common::Collections::TArrayList<ISceneNode*> m_nodeList;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ISceneNode*> m_nodeMap;
	};

Meson_Vistas_END