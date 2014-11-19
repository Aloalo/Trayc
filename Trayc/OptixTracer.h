/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_OPTIXTRACER_H
#define TRAYC_OPTIXTRACER_H

#include <eNGINE/Geometry/Scene.h>
#include <Trayc/Handlers/MaterialHandler.h>
#include <Trayc/Handlers/AccelHandler.h>
#include <Engine/Core/Camera.h>
#include <Engine/Utils/Setting.h>
#include <Trayc/CUDAfiles/lights.h>

namespace trayc
{
    //handles optix traceing
    class OptixTracer
    {
    public:
        OptixTracer(void);
        ~OptixTracer(void);

        void Initialize(unsigned int GLBO);
        void ApplySettings();
        
        void AddScene(const engine::Scene &scene);
        void AddMesh(const engine::TriangleMesh &mesh, const engine::Material &mat);

        //Add scene so that all meshes have mat as their material
        void AddScene(const engine::Scene &scene, const optix::Material mat);
        void AddMesh(const engine::TriangleMesh &mesh, const optix::Material mat);

        void AddGeometryInstance(const optix::GeometryInstance gi);
        void AddLight(const BasicLight &light);

        void CompileSceneGraph();
        void ClearSceneGraph();
        void Trace(unsigned int entryPoint, RTsize width, RTsize height, int renderingDivisionLevel = 1);

        BasicLight& GetLight(int i);
        void UpdateLight(int idx);
        
        void SetBufferSize(int w, int h);
        void SetCamera(const engine::Camera &cam);
        void RenderToPPM(const std::string &name);

        optix::Buffer outBuffer;

        //general
        engine::Setting<float> apertureRadius;
        engine::Setting<float> focalLength;
        engine::Setting<float> AOsamplingRadius;

        //real time
        engine::Setting<int> maxRayDepth;
        engine::Setting<int> shadowSamples;
        engine::Setting<int> dofSamples;
        engine::Setting<int> ambientOcclusionSamples;
        engine::Setting<int> MSAA;

        //screenshot
        engine::Setting<int> SSmaxRayDepth;
        engine::Setting<int> SSshadowSamples;
        engine::Setting<int> SSdofSamples;
        engine::Setting<int> SSambientOcclusionSamples;
        engine::Setting<int> SSMSAA;
        engine::Setting<int> SSrenderingDivisionLevel;
        engine::Setting<int> SSbufferWidth;
        engine::Setting<int> SSbufferHeight;


    private:
        optix::Buffer SSbuffer;
        AccelHandler accelHandler;
        MaterialHandler matHandler;

        optix::GeometryGroup geometrygroup;
        std::vector<optix::GeometryInstance> gis;
        std::vector<BasicLight> lights;
    };
};

#endif
