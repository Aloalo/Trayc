/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_BUFFERDRAWER_H
#define TRAYC_BUFFERDRAWER_H

#include <Engine/GL/Program.h>
#include <optix_world.h>
#include <Engine/Utils/Setting.h>

namespace trayc
{
	/*
	draws the optix::Buffer on the whole screen
	void init(const optix::Buffer &buffer) must be called each time buffer type changes
	*/
	class BufferDrawer
	{
	public:
		BufferDrawer(void);

		void CleanUP();

		GLuint CreateGLBuffer();
		void Init(int bufferElementSize);
		void Draw(optix::Buffer &buffer);
		void AllocateBuffer(int width, int height);

        void SetOutBufferTextureFilter(GLenum textureFilter);
        void SetUseFxaa(bool useFxaa);

        engine::Setting<int> textureFilter;
        engine::Setting<int> useFxaa;

	private:
		GLenum glDataType;
		GLenum glFormat;
		GLenum glTextureFormat;

        GLuint textureID;
        GLuint verticesID;
        GLuint outBufferID;
        GLuint VAO;

        engine::Program p;
	};
}

#endif
