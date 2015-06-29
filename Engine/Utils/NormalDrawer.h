/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_NORMALDRAWER_H
#define EN_NORMALDRAWER_H

#include <Engine/GL/Program.h>
#include <Engine/Utils/MathFunctions.h>
#include <vector>

namespace engine
{
	class NormalDrawer
	{
	public:
		NormalDrawer(const std::vector<glm::vec3> &positions, 
                     const std::vector<glm::vec3> &normals, 
                     const glm::mat4 &Model = I);

		void Draw(const glm::mat4 &View, const glm::mat4 &Projection);
        void CleanUp();

	private:
		Program mProgram;
		GLuint mVAO;
        GLuint mVBO;
        GLsizei mNumVertices;


		const glm::mat4 &mModel;
	};
}

#endif
