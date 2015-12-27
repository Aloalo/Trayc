/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Utils/BSpline.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace engine;
using namespace std;
using namespace glm;

class AnimatedMesh : public Renderable, public Updateable
{
public:
    AnimatedMesh(const vector<vec3> &r)
        : mBSpline(r), M(1.0f)
    {
    }

    ~AnimatedMesh(void)
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mVBO);
        mProgram.Delete();
    }

    virtual void Draw(const RenderingContext &rContext) const override
    {
        mProgram.Use();

        const mat4 V = rContext.mCamera->GetViewMatrix();
        mProgram.SetUniform("MVP", rContext.mCamera->GetProjectionMatrix() * V * M);

        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLES, 0, mCtTriangles);
        glBindVertexArray(0);

        Program::Unbind();
    }

    virtual void Update(float dt) override
    {
        static float mdt = 0.0f;

        mdt += dt;
        if(int(mdt) >= mBSpline.NumControlPoints() - 3)
            mdt -= float(mBSpline.NumControlPoints() - 3);

        static const vec3 startOrientation(0.0f, 0.0f, 1.0f);

        const vec3 tg = mBSpline.Tangent(mdt);
        const float angle = dot(startOrientation, tg) / (length(startOrientation) * length(tg));
        const vec3 pos = mBSpline[mdt];

        M = rotate(mat4(1.0f), angle, cross(startOrientation, tg));
        M = glm::translate(M, pos);

    }

    void LoadMesh(const char *fname)
    {
        vector<vec3> vertices;
        vector<vec3> mesh;

        ifstream in(fname);
        string line;
        while(getline(in, line))
        {
            istringstream iss(line);
            char c;
            if(!(iss >> c))
                break;

            if(c == 'v')
            {
                vec3 v;
                iss >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            }
            else
            {
                int a, b, c;
                iss >> a >> b >> c;
                mesh.push_back(vertices[a - 1]);
                mesh.push_back(vertices[b - 1]);
                mesh.push_back(vertices[c - 1]);
            }
        }

        mCtTriangles = mesh.size();

        mProgram.Init("Simple");

        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glBindVertexArray(mVAO);
        {

            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }
        glBindVertexArray(0);
    }
private:
    GLsizei mCtTriangles;
    GLuint mVAO;
    GLuint mVBO;
    Program mProgram;
    CubicBSpline mBSpline;
    mat4 M;
};

RotationalCameraHandler ConstructCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 0.1f;
    const vec3 cameraPos(40.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;
    const float zoomSpeed = 6.0f;
    const float springiness = 15.0f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed, zoomSpeed, springiness);
}

int main(int argc, char *argv[])
{
    const ivec2 SSize(1280, 720);
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructCameraHandler(SSize, 60.0f, 1000.0f));
    //Init Scene
    Scene scene(timeStep);
    scene.Init(&camHandler, argv[0], "Test", SSize.x, SSize.y);
    scene.mSDLHandler.VsyncMode(1);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    vector<vec3> controlPoints;
    controlPoints.push_back(vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(vec3(0.0f, 10.0f, 5.0f));
    controlPoints.push_back(vec3(10.0f, 10.0f, 10.0f));
    controlPoints.push_back(vec3(10.0f, 0.0f, 15.0f));
    controlPoints.push_back(vec3(0.0f, 0.0f, 20.0f));
    controlPoints.push_back(vec3(0.0f, 10.0f, 25.0f));
    controlPoints.push_back(vec3(10.0f, 10.0f, 30.0f));
    controlPoints.push_back(vec3(10.0f, 0.0f, 35.0f));
    controlPoints.push_back(vec3(0.0f, 0.0f, 40.0f));
    controlPoints.push_back(vec3(0.0f, 10.0f, 45.0f));
    controlPoints.push_back(vec3(10.0f, 10.0f, 50.0f));
    controlPoints.push_back(vec3(10.0f, 0.0f, 55.0f));

    AnimatedMesh aMesh(controlPoints);
    aMesh.LoadMesh("medo.obj");

    scene.mRenderer.AddRenderable(&aMesh);
    scene.mRenderer.AddLight(Light());
    scene.mUpdateableMenager.AddUpdateable(&aMesh);

    scene.GameLoop();
    return 0;
}
