#pragma once

#include "GrvSpace.h"
#include "GrvConstraint.h"

#include "Entity.h"

void CreatePolyhedron(
	Meson::Gravitas::Space::SpacePtr p_pSpace,
	EntityList* p_pEntityList, 
	Meson::Gravitas::Constraint::ConstraintList* p_pConstraintList);
