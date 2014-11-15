/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Entity.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace trayc
{
	Entity::Entity(void)
	{
	}

	Entity::~Entity(void)
	{
	}

	const vec3& Entity::GetPos() const
	{
		return pos;
	}

	void Entity::SetPos(const vec3 &p)
	{
		pos = p;
		RecalcMatrix();
	}

	const vec3& Entity::GetScale() const
	{
		return scale;
	}
	
	void Entity::SetScale(const vec3 &s)
	{
		scale = s;
		RecalcMatrix();
	}
	
	void Entity::RecalcMatrix()
	{
		const mat4 mat(translate(glm::scale(mat4(1.0f), scale), pos));
		trans->setMatrix(false, (float*)&mat, (float*)&inverse(mat));
	}
}
