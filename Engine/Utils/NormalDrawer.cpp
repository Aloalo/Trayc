/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/NormalDrawer.h>

using namespace glm;
using namespace std;

namespace engine
{
    NormalDrawer::NormalDrawer(const vector<vec3> &positions,
                               const vector<vec3> &normals,
                               const mat4 &Model /*= I*/)
        : mProgram("../Resources/Shaders/NormalShader"),
          mModel(Model)
    {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO);
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3) * 2, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(vec3), positions.data());
            glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), normals.size() * sizeof(vec3), normals.data());

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(positions.size() * sizeof(vec3)));
        }
        glBindVertexArray(0);
    }

	void NormalDrawer::Draw(const mat4 &View, const mat4 &Projection)
	{
		const mat4 MVP = Projection * View * mModel;

		mProgram.Use();
		mProgram.SetUniform("MVP", MVP);
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
	}

    void NormalDrawer::CleanUp()
    {
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
        mProgram.Delete();
    }

}
