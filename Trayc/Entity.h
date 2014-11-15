/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_ENTITY_H
#define TRAYC_ENTITY_H

#include <glm/glm.hpp>
#include <Trayc/SafeOptix.h>

namespace trayc
{
	class Entity
	{
	public:
		Entity(void);
		~Entity(void);

		const glm::vec3& GetPos() const;
		void SetPos(const glm::vec3 &p);

		const glm::vec3& GetScale() const;
		void SetScale(const glm::vec3 &s);

		optix::Transform trans;
		optix::Group node;
		
	protected:
		void RecalcMatrix();

		glm::vec3 scale;
		glm::vec3 pos;
	};
}

#endif
