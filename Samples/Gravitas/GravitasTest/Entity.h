#pragma once

#include "GrvBody.h"

#include "Model3D.h"

struct Entity
{
	Meson::Gravitas::Body::BodyPtr Body;
	Model3DPtr Model3D;

	Entity(void);
	Entity(const Entity& p_entity);
	~Entity(void);

	Entity& operator=(const Entity& p_entity);
	bool operator==(const Entity& p_entity) const;
};

typedef Meson::Common::Collections::TList<Entity> EntityList;
typedef Meson::Common::TPointer<EntityList> EntityListPtr;
