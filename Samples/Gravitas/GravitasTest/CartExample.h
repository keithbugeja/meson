#pragma once

#include "GrvBody.h"
#include "GrvConstraint.h"

#include "Entity.h"

void CreateCart(
	Meson::Gravitas::Space::SpacePtr p_pSpace,
	EntityList* p_pEntityList, 
	Meson::Gravitas::Constraint::ConstraintList* p_pConstraintList);
