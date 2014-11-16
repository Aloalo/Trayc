/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/BufferDrawer.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>
#include <iostream>

using namespace std;
using namespace optix;
using namespace glm;
using namespace engine;

namespace trayc
{
    BufferDrawer::BufferDrawer(void)
        : SETTING(textureFilter), p(VertexShader(Utils::Shader("passthrough").c_str()), FragmentShader(Utils::Shader(postProcess ? "fxaa" : "passthrough").c_str())), SETTING(postProcess)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &verticesID);
        glGenTextures(1, &textureID);
        glGenBuffers(1, &outBufferID);

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
        AllocateBuffer(Environment::Get().bufferWidth, Environment::Get().bufferHeight);
        return outBufferID;
    }

    void BufferDrawer::Init(const Buffer &buffer)
    {
        const GLfloat quad[] = 
        { 
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,
            -1.0f, 1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f,
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
            RTsize elementSize = buffer->getElementSize();
            if(elementSize % 8 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
            else if(elementSize % 4 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            else if(elementSize % 2 == 0)
                glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            else
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        p.Use();
        p.SetUniform("renderedTexture", 0);

        glBindVertexArray(VAO);
        {
            glBindBuffer(GL_ARRAY_BUFFER, verticesID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        }
        glBindVertexArray(0);

        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            cerr << "OpenGL error: 0x" << hex << err << endl;
            system("pause");
        }
    }

    void BufferDrawer::Draw(optix::Buffer &buffer)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, outBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat, Environment::Get().bufferWidth.x, Environment::Get().bufferHeight.x, 0, glFormat, glDataType, nullptr);

        p.Use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    void BufferDrawer::AllocateBuffer(int width, int height)
    {
        glBindBuffer(GL_ARRAY_BUFFER, outBufferID);
        {
            glBufferData(GL_ARRAY_BUFFER, width * height * sizeof(uchar4), nullptr, GL_STREAM_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            cerr << "OpenGL error: 0x" << hex << err << endl;
            system("pause");
        }
    }
}
