#pragma once

#include "VstVistasEngine.h"
//#include "VstMesh.h"

#include "GrvBody.h"
#include "GrvConstraint.h"
#include "GrvSpace.h"

#include "Entity.h"

using namespace Meson::Vistas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Space;

class MainWindowMessageHandler
	: public IWindowMessageHandler
{
private:
	WindowRenderTarget* m_pWindowRenderTarget;
	bool m_bRunning;
	SpacePtr m_pSpace;
	ConstraintListPtr m_pConstraintList;
	EntityListPtr m_pEntityList;
	bool m_bInstrumentationActive;

private: // private methods

public:
	// IWindowMessageHandler interface
	MainWindowMessageHandler(WindowRenderTarget* p_pWindowRenderTarget);
	void SetSpace(SpacePtr p_pSpace);
	void SetConstraintList(ConstraintListPtr p_pConstraintList);
	void SetEntityList(EntityListPtr p_pEntityList);
	void Terminate(void);
	void OnMouseDown(MouseKey p_eMouseKey);
	void OnClose(void);
	void OnKeyDown(KeyCode p_eKey);
	void OnKeyUp(KeyCode p_eKey);
	void OnResize(int p_nHorizontal, int p_nVertical, WindowState p_windowState);
	void OnMove(int p_nHorizontal, int p_nVertical);
	bool Step(void);
	void Run(void);

	// custom interface
	bool IsInstrumentationActive(void);
};
