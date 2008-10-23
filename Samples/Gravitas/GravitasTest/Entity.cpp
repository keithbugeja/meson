#include "Entity.h"

Entity::Entity(void)
	: Body()
	, Model3D()
{
}

Entity::Entity(const Entity& p_entity)
	: Body(p_entity.Body)
	, Model3D(p_entity.Model3D)
{
}

Entity::~Entity(void)
{
}

Entity& Entity::operator=(const Entity& p_entity)
{
	Body = p_entity.Body;
	Model3D = p_entity.Model3D;
	return *this;
}

bool Entity::operator==(const Entity& p_entity) const
{
	return Body == p_entity.Body && Model3D == p_entity.Model3D;
}
