//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Constraint:ConstraintManager
//	Gravitas space manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		20/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvConstraintFactory.h"
#include "GrvConstraintSolver.h"

Meson_Gravitas_Constraint_BEGIN

struct ConstraintManagerState;

//------------------------------------------------------------------------------------------
/// \class ConstraintManager
/// The physics engine's constraint manager module maintains a registry of constraint
/// factories and a registry for constraint solvers.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ConstraintManager
	: public Meson::Common::TSingleton<ConstraintManager>
{
	friend class Meson::Common::TSingleton<ConstraintManager>;

private: // private variables
	ConstraintManagerState *m_pConstraintManagerState;

private: // private methods
	ConstraintManager(void);
	~ConstraintManager(void);

public:
	//------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered constraint factories.
	/// \returns an enumerator of registered constraint factories.
	//------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<IConstraintFactory*>& GetConstraintFactories(void);

	//------------------------------------------------------------------------------------------
	/// Returns a constraint factory registered with the given name.
	/// \param p_strName Name of the constraint factory.
	/// \returns a constraint factory registered with the given name.
	//------------------------------------------------------------------------------------------
	IConstraintFactory* GetConstraintFactory(const Meson::Common::Text::String& p_strName);

	//------------------------------------------------------------------------------------------
	/// Registers a constraint factory with the constraint manager.
	/// \param p_pConstraintFactory Constraint factory to register.
	//------------------------------------------------------------------------------------------
	void RegisterConstraintFactory(IConstraintFactory* p_pConstraintFactory);

	//------------------------------------------------------------------------------------------
	/// Unregisters a constraint factory from the constraint manager.
	/// \param p_pConstraintFactory Constraint factory to unregister.
	//------------------------------------------------------------------------------------------
	void UnregisterConstraintFactory(IConstraintFactory* p_pConstraintFactory);

	//------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered constraint solvers.
	/// \returns an enumerator of registered constraint solvers.
	//------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<IConstraintSolver*>& GetConstraintSolvers(void);

	//------------------------------------------------------------------------------------------
	/// Returns a constraint solver registered with the given name.
	/// \param p_strName Name of the constraint solver.
	/// \returns a constraint solver registered with the given name.
	//------------------------------------------------------------------------------------------
	IConstraintSolver* GetConstraintSolver(const Meson::Common::Text::String& p_strName);

	//------------------------------------------------------------------------------------------
	/// Unregisters a constraint solver from the constraint manager.
	/// \param p_pConstraintSolver Constraint solver to unregister.
	//------------------------------------------------------------------------------------------
	void RegisterConstraintSolver(IConstraintSolver* p_pConstraintSolver);

	//------------------------------------------------------------------------------------------
	/// Unregisters a constraint solver from the constraint manager.
	/// \param p_pConstraintSolver Constraint solver to unregister.
	//------------------------------------------------------------------------------------------
	void UnregisterConstraintSolver(IConstraintSolver* p_pConstraintSolver);

	//------------------------------------------------------------------------------------------
	/// Unregisters all constraint solvers from the constraint manager.
	//------------------------------------------------------------------------------------------
	void UnregisterAllConstraintSolvers(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a constraint solver from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single constraint solver definition.
	//----------------------------------------------------------------------------------------------
	void LoadConstraintSolver(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a collection of constraint solvers from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        collection of constraint solver definitions.
	//----------------------------------------------------------------------------------------------
	void LoadConstraintSolvers(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a collection of constraint solvers from the given file.
	/// \param p_strFilename Name of a file containing a collection of constraint solver
	///        definitions.
	//----------------------------------------------------------------------------------------------
	void LoadConstraintSolvers(const Meson::Common::Text::String& p_strFilename);
};

Meson_Gravitas_Constraint_END
