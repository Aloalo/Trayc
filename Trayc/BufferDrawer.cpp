/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/BufferDrawer.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>
#include <Engine/Common/ErrorCheck.h>
#include <iostream>

using namespace std;
using namespace optix;
using namespace glm;
using namespace engine;

namespace trayc
{
    BufferDrawer::BufferDrawer(void)
        : SETTING(textureFilter), SETTING(useFxaa)
    {
        glDataType = GL_UNSIGNED_BYTE;
        glFormat = GL_BGRA;
        glTextureFormat = GL_RGBA8;
    }


    void BufferDrawer::CleanUP()
    {
        cout << "Cleaning up BufferDrawer ... ";
        p.Delete();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &verticesID);
        glDeleteBuffers(1, &outBufferID);
        glDeleteTextures(1, &textureID);
        cout << "DONE" << endl;
    }

    GLuint BufferDrawer::CreateGLBuffer()
    {
        glGenBuffers(1, &outBufferID);
        AllocateBuffer(Environment::Get().bufferWidth, Environment::Get().bufferHeight);
        return outBufferID;
    }

    void BufferDrawer::Init(int bufferElementSize)
    {
        p.Init(&VertexShader(Utils::Shader("fxaa").c_str()), nullptr, &FragmentShader(Utils::Shader("fxaa").c_str()));

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &verticesID);
        glGenTextures(1, &textureID);

        const GLfloat quad[] = 
        { 
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f,
            -1.0f, 1.0f
        };

        glBindTexture(GL_TEXTURE_2D, textureID);
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, outBufferID);
        {
            if(bufferElementSize % 8 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
            else if(bufferElementSize % 4 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            else if(bufferElementSize % 2 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            else
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        p.Use();
        p.SetUniform("renderedTexture", 0);
        p.SetUniform("useFxaa", useFxaa);

        glBindVertexArray(VAO);
        {
            glBindBuffer(GL_ARRAY_BUFFER, verticesID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        }
        glBindVertexArray(0);

        GLErrCheck(true);
    }

    void BufferDrawer::Draw(optix::Buffer &buffer)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, outBufferID);
        glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat, Environment::Get().bufferWidth.x, Environment::Get().bufferHeight.x, 0, glFormat, glDataType, nullptr);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        p.Use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void BufferDrawer::AllocateBuffer(int width, int height)
    {
        glBindBuffer(GL_ARRAY_BUFFER, outBufferID);
        {
            glBufferData(GL_ARRAY_BUFFER, width * height * sizeof(uchar4), nullptr, GL_STREAM_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BufferDrawer::SetOutBufferTextureFilter(GLenum textureFilter)
    {
        this->textureFilter = textureFilter;
        glBindTexture(GL_TEXTURE_2D, textureID);
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void BufferDrawer::SetUseFxaa(bool useFxaa)
    {
        this->useFxaa = useFxaa;
        p.SetUniform("useFxaa", useFxaa);
    }


}
