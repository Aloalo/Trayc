/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/OptixTracer.h>
#include <Trayc/Utils.h>
#include <Trayc/Environment.h>
#include <Trayc/Handlers/ProgramHandler.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Engine/Core/EventHandler.h>
#include <ctime>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

using namespace optix;
using namespace engine;
using namespace std;

namespace trayc
{
    //HELPER FUNCTIONS
    template<class T>
    static inline Buffer GetBufferFromVector(const vector<T> &vec, RTformat type)
    {
        Buffer ret = ctx->createBuffer(RT_BUFFER_INPUT);
        ret->setFormat(type);
        ret->setSize(vec.size());
        memcpy(static_cast<void*>(ret->map()), (const void*)vec.data(), vec.size() * sizeof(T));
        ret->unmap();
        return ret;
    }

    static inline Geometry GetGeometry(const TriangleMesh &mesh, const engine::Material &mat = engine::Material())
    {
        Geometry gMesh = ctx->createGeometry();
        gMesh->setPrimitiveCount(mesh.indices.size());
        gMesh->setBoundingBoxProgram(ProgramHandler::Get().Get("triangle_mesh.cu", "bounds"));
        gMesh->setIntersectionProgram(ProgramHandler::Get().Get("triangle_mesh.cu", "intersect"));

        gMesh["vertex_buffer"]->setBuffer(GetBufferFromVector(mesh.positions, RT_FORMAT_FLOAT3));
        gMesh["normal_buffer"]->setBuffer(GetBufferFromVector(mesh.normals, RT_FORMAT_FLOAT3));
        gMesh["tangent_buffer"]->setBuffer(GetBufferFromVector(mesh.tangents, RT_FORMAT_FLOAT3));
        gMesh["bitangent_buffer"]->setBuffer(GetBufferFromVector(mesh.bitangents, RT_FORMAT_FLOAT3));
        gMesh["normal_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.normal_map, 0.0f, RT_WRAP_REPEAT, Utils::DefTexture("bumpDefault.png")));

        gMesh["texcoord_buffer"]->setBuffer(GetBufferFromVector(mesh.uvs, RT_FORMAT_FLOAT2));
        gMesh["index_buffer"]->setBuffer(GetBufferFromVector(mesh.indices, RT_FORMAT_INT3));

        return gMesh;
    }
    //HELPER FUNCTIONS

    OptixTracer::OptixTracer(void) :
        SETTING(apertureRadius),
        SETTING(focalLength),
        SETTING(AOsamplingRadius),

        SETTING(maxRayDepth),
        SETTING(shadowSamples),
        SETTING(dofSamples),
        SETTING(ambientOcclusionSamples),
        SETTING(MSAA),

        SETTING(SSmaxRayDepth),
        SETTING(SSshadowSamples),
        SETTING(SSdofSamples),
        SETTING(SSambientOcclusionSamples),
        SETTING(SSMSAA),
        SETTING(SSrenderingDivisionLevel),
        SETTING(SSbufferWidth),
        SETTING(SSbufferHeight)
    {
    }

    OptixTracer::~OptixTracer(void)
    {
    }

    void OptixTracer::SetBufferSize(int w, int h)
    {
        w = max(1, w);
        h = max(1, h);
        outBuffer->setSize(w, h);
    }

    void OptixTracer::Initialize(unsigned int GLBO)
    {
        ctx = Context::create();

        ctx->setRayTypeCount(2);
        ctx->setEntryPointCount(1);
        ctx->setCPUNumThreads(4);

        ctx["radiance_ray_type"]->setUint(0);
        ctx["shadow_ray_type"]->setUint(1);
        ctx["scene_epsilon"]->setFloat(0.01f);
        ctx["importance_cutoff"]->setFloat(0.01f);
        ctx["ambient_light_color"]->setFloat(0.3f, 0.3f, 0.3f);

        outBuffer = ctx->createBufferFromGLBO(RT_BUFFER_OUTPUT, GLBO);
        outBuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
        ctx["output_buffer"]->setBuffer(outBuffer);

        SSbuffer = ctx->createBuffer(RT_BUFFER_OUTPUT);
        SSbuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);

        ctx->setRayGenerationProgram(0, ProgramHandler::Get().Get("context_shaders.cu", "dof_camera"));

        ctx->setMissProgram(0, ProgramHandler::Get().Get("context_shaders.cu", "envmap_miss"));
        ctx["envmap"]->setTextureSampler(OptixTextureHandler::Get().Get(Utils::DefTexture("environment.jpg")));

        ctx->setExceptionProgram(0, ProgramHandler::Get().Get("context_shaders.cu", "exception"));
        ctx["bad_color"]->setFloat(1.0f, 0.0f, 0.0f);

        lightMaterial = ctx->createMaterial();
        lightMaterial->setClosestHitProgram(0, ProgramHandler::Get().Get("material_shaders.cu", "closest_hit_light"));
        lightMaterial->setAnyHitProgram(1, ProgramHandler::Get().Get("material_shaders.cu", "any_hit_light"));

        staticGG = ctx->createGeometryGroup();
        lightsGG = ctx->createGeometryGroup();
        lightsGG->setAcceleration(ctx->createAcceleration("NoAccel", "NoAccel"));

        topNode = ctx->createGroup();
        topNode->setAcceleration(ctx->createAcceleration("NoAccel", "NoAccel"));
        topNode->addChild(staticGG);
        topNode->addChild(lightsGG);

        ctx["top_object"]->set(topNode);
        ctx["top_shadower"]->set(staticGG);

        ApplySettings();
    }


    void OptixTracer::ApplySettings()
    {
        ctx->setStackSize(768 + 256 * maxRayDepth);
        ctx["renderingDivisionLevel"]->setInt(1);
        ctx["max_depth"]->setInt(maxRayDepth);
        ctx["shadow_samples"]->setInt(shadowSamples);
        ctx["ambient_occlusion_samples"]->setInt(ambientOcclusionSamples);
        outBuffer->setSize(Environment::Get().bufferWidth, Environment::Get().bufferHeight);
        ctx["AAlevel"]->setInt(MSAA);
        ctx["focal_length"]->setFloat(focalLength);
        ctx["aperture_radius"]->setFloat(apertureRadius);
        ctx["dof_samples"]->setInt(dofSamples);
        ctx["ao_sampling_radius"]->setFloat(AOsamplingRadius);
    }

    void OptixTracer::AddScene(const Scene &scene)
    {
        const int ctMeshes = scene.meshes.size();
        for(int i = 0; i < ctMeshes; ++i)
            AddMesh(scene.meshes[i], scene.materials[scene.meshes[i].materialIndex]);
    }

    void OptixTracer::AddMesh(const TriangleMesh &mesh, const engine::Material &mat)
    {
        const Geometry gMesh = GetGeometry(mesh, mat);
        const optix::Material material = matHandler.CreateMaterial(mat);

        GeometryInstance inst = ctx->createGeometryInstance();
        inst->setMaterialCount(1);
        inst->setGeometry(gMesh);
        inst->setMaterial(0, material);

        staticGG->addChild(inst);
    }

    void OptixTracer::AddScene(const Scene &scene, const optix::Material mat)
    {   
        const int ctMeshes = scene.meshes.size();
        for(int i = 0; i < ctMeshes; ++i)
            AddMesh(scene.meshes[i], mat);
    }

    void OptixTracer::AddMesh(const TriangleMesh &mesh, const optix::Material mat)
    {
        const Geometry gMesh = GetGeometry(mesh);

        GeometryInstance inst = ctx->createGeometryInstance();
        inst->setMaterialCount(1);
        inst->setGeometry(gMesh);
        inst->setMaterial(0, mat);

        staticGG->addChild(inst);
    }

    void OptixTracer::AddLight(const BasicLight &light)
    {
        lights.push_back(light);
    }

    void OptixTracer::AddGeometryInstance(const GeometryInstance gi)
    {
        staticGG->addChild(gi);
    }

    void OptixTracer::CompileSceneGraph(const std::string accelLocation, bool cacheAccel)
    {
        //white spheres for lights
        for(int i = 0; i < lights.size(); ++i)
        {
            const BasicLight &light = lights[i];
            if(!light.is_directional)
            {
                Geometry sphere = ctx->createGeometry();
                sphere->setPrimitiveCount(1);
                sphere->setBoundingBoxProgram(ProgramHandler::Get().Get("light.cu", "bounds"));
                sphere->setIntersectionProgram(ProgramHandler::Get().Get("light.cu", "intersect"));
                sphere["light_idx"]->setInt(i);
                GeometryInstance inst = ctx->createGeometryInstance();
                inst->setMaterialCount(1);
                inst->setGeometry(sphere);
                inst->setMaterial(0, lightMaterial);
                lightsGG->addChild(inst);
            }
        }

        Buffer lightBuffer = ctx->createBuffer(RT_BUFFER_INPUT);
        lightBuffer->setFormat(RT_FORMAT_USER);
        lightBuffer->setElementSize(sizeof(BasicLight));
        ctx["lights"]->setBuffer(lightBuffer);

        lightBuffer->setSize(lights.size());
        memcpy(lightBuffer->map(), (const void*)lights.data(), lights.size() * sizeof(BasicLight));
        lightBuffer->unmap();

        Acceleration accel = ctx->createAcceleration("Sbvh", "Bvh");
        accel->setProperty("index_buffer_name", "index_buffer");
        accel->setProperty("vertex_buffer_name", "vertex_buffer");
        accel->markDirty();
        staticGG->setAcceleration(accel);

        if(staticGG->getChildCount() > 0)
        {
            bool success = false;
            if(cacheAccel)
                success = accelHandler.LoadAccelCache(accelLocation, staticGG);

            try
            {
                ctx->launch(0, 0, 0);
            }
            catch (exception &e)
            {
                cerr << e.what() << endl;
                EventHandler::SetQuit();
                return;
            }

            if(!success && cacheAccel)
                accelHandler.SaveAccelCache(accelLocation, staticGG);
        }
        ctx->validate();
        ctx->compile();
    }


    void OptixTracer::ClearSceneGraph()
    {
        ctx["lights"]->getBuffer()->destroy();
        lights.clear();
        const int ctStatic = staticGG->getChildCount();
        for(int i = ctStatic - 1; i >= 0; --i)
        {
            staticGG->getChild(i)->getGeometry()->destroy();
            staticGG->getChild(i)->destroy();
            staticGG->removeChild(i);
        }

        const int ctLights = lightsGG->getChildCount();
        for(int i = ctLights - 1; i >= 0; --i)
        {
            lightsGG->getChild(i)->getGeometry()->destroy();
            lightsGG->getChild(i)->destroy();
            lightsGG->removeChild(i);
        }

        staticGG->getAcceleration()->destroy();
        lightsGG->getAcceleration()->markDirty();
        matHandler.Clear();
    }

    void OptixTracer::Trace(unsigned int entryPoint, RTsize width, RTsize height, int renderingDivisionLevel, unsigned int rndSeed)
    {
        ctx["rnd_seed"]->setUint(rndSeed);
        for(int i = 0; i < renderingDivisionLevel; ++i)
        {
            ctx["myStripe"]->setInt(i);
            ctx->launch(entryPoint, width, height / renderingDivisionLevel);
            if(renderingDivisionLevel > 1)
                cout << "Rendered Stripe: " << i + 1 << "/" << renderingDivisionLevel << endl;
        }
    }

    int OptixTracer::GetNumLights() const
    {
        return lights.size();
    }

    BasicLight& OptixTracer::GetLight(int i)
    {
        return lights[i];
    }

    void OptixTracer::UpdateLight(int idx)
    {
        memcpy(static_cast<void*>((static_cast<BasicLight*>(ctx["lights"]->getBuffer()->map())+idx)), static_cast<const BasicLight*>(lights.data()+idx), sizeof(BasicLight));
        ctx["lights"]->getBuffer()->unmap();
    }

    void OptixTracer::SetCamera(const Camera &cam)
    {
        const float tanfov = tanf(cam.FoV * Utils::pi / 360.0f) * 0.5f;

        ctx["eye"]->setFloat(*(float3*)(&cam.position));
        ctx["U"]->setFloat(*(float3*)&(cam.GetRight() * tanfov * cam.aspectRatio));
        ctx["V"]->setFloat(*(float3*)&(cam.GetUp() * tanfov));
        ctx["W"]->setFloat(*(float3*)&cam.GetDirection());
    }

    void OptixTracer::SetCameraPos(const glm::vec3 &pos)
    {
        ctx["eye"]->setFloat(*(float3*)(&pos));
    }

    void OptixTracer::SetCameraDir(const Camera &cam)
    {
        const float tanfov = tanf(cam.FoV * Utils::pi / 360.0f) * 0.5f;
        ctx["U"]->setFloat(*(float3*)&(cam.GetRight() * tanfov * cam.aspectRatio));
        ctx["V"]->setFloat(*(float3*)&(cam.GetUp() * tanfov));
        ctx["W"]->setFloat(*(float3*)&cam.GetDirection());
    }

    void OptixTracer::RenderToPPM(const std::string &name)
    {
        const RTsize w = SSbufferWidth;
        const RTsize h = SSbufferHeight;
        SSbuffer->setSize(w, h);

        ctx->setStackSize(768 + 256 * SSmaxRayDepth);
        ctx["max_depth"]->setInt(SSmaxRayDepth);
        ctx["AAlevel"]->setInt(SSMSAA);
        ctx["renderingDivisionLevel"]->setInt(SSrenderingDivisionLevel);
        ctx["dof_samples"]->setInt(SSdofSamples);
        ctx["shadow_samples"]->setInt(SSshadowSamples);
        ctx["ambient_occlusion_samples"]->setInt(SSambientOcclusionSamples);
        ctx["output_buffer"]->setBuffer(SSbuffer);

        Trace(0, w, h, SSrenderingDivisionLevel, 1337);

        ctx["output_buffer"]->setBuffer(outBuffer);
        ctx["shadow_samples"]->setInt(shadowSamples);
        ctx["dof_samples"]->setInt(dofSamples);
        ctx["ambient_occlusion_samples"]->setInt(ambientOcclusionSamples);
        ctx["renderingDivisionLevel"]->setInt(1);
        ctx["AAlevel"]->setInt(MSAA);
        ctx["max_depth"]->setInt(maxRayDepth);
        ctx->setStackSize(768 + 256 * maxRayDepth);
        
        const int k = 4;

        unsigned char *out = (unsigned char*)SSbuffer->map();
        {
            std::ofstream ofs(name, std::ios::out | std::ios::binary);
            ofs << "P6\n" << w << " " << h << "\n255\n";
            for(int i = h-1; i >= 0; --i)
                for(int j = 0; j < w; ++j)
                    ofs << out[(i*w+j)*k + 2] << out[(i*w+j)*k + 1] << out[(i*w+j)*k + 0];
                ofs.close();
        }
        SSbuffer->unmap();
    }
}
