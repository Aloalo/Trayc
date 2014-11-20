/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/OptixTracer.h>
#include <Trayc/Utils.h>
#include <Trayc/Environment.h>
#include <Trayc/Programs.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
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
        ctx["frame"]->setUint(1);

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

        gis.push_back(inst);
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

        gis.push_back(inst);
    }

    void OptixTracer::AddLight(const BasicLight &light)
    {
        lights.push_back(light);
    }

    void OptixTracer::AddGeometryInstance(const GeometryInstance gi)
    {
        gis.push_back(gi);
    }

    void OptixTracer::CompileSceneGraph()
    {
        //white spheres for lights
        for(const auto &light : lights)
            if(!light.is_directional)
            {
                Geometry sphere = ctx->createGeometry();
                sphere->setPrimitiveCount(1);
                sphere->setBoundingBoxProgram(ProgramHandler::Get().Get("sphere.cu", "bounds"));
                sphere->setIntersectionProgram(ProgramHandler::Get().Get("sphere.cu", "intersect"));
                sphere["sphere"]->setFloat(light.pos.x, light.pos.y, light.pos.z, light.radius);
                GeometryInstance inst = ctx->createGeometryInstance();
                inst->setMaterialCount(1);
                inst->setGeometry(sphere);
                inst->setMaterial(0, lightMaterial);
                gis.push_back(inst);
            }

        Buffer lightBuffer = ctx->createBuffer(RT_BUFFER_INPUT);
        lightBuffer->setFormat(RT_FORMAT_USER);
        lightBuffer->setElementSize(sizeof(BasicLight));
        ctx["lights"]->setBuffer(lightBuffer);

        lightBuffer->setSize(lights.size());
        memcpy(lightBuffer->map(), (const void*)lights.data(), lights.size() * sizeof(BasicLight));
        lightBuffer->unmap();

        geometrygroup = ctx->createGeometryGroup();
        geometrygroup->setChildCount(gis.size());
        for(int i = 0; i < gis.size(); ++i)
            geometrygroup->setChild(i, gis[i]);

        ctx["top_object"]->set(geometrygroup);

        geometrygroup->setAcceleration(ctx->createAcceleration("Sbvh", "Bvh"));

        if(!accelHandler.accel_cache_loaded && gis.size() > 0)
        {
            const string filename = Utils::Resource("accelCaches/accel.accelcache");
            accelHandler.LoadAccelCache(filename, geometrygroup);

            Acceleration accel = ctx->createAcceleration("Sbvh", "Bvh");

            accel->setProperty("index_buffer_name", "index_buffer");
            accel->setProperty("vertex_buffer_name", "vertex_buffer");
            accel->markDirty();
            geometrygroup->setAcceleration(accel);
            try
            {
                ctx->launch(0, 0, 0);
            }
            catch (exception &e)
            {
                cerr << e.what() << endl;
                exit(-1);
            }
            

            accelHandler.SaveAccelCache(filename, geometrygroup);
        }
        ctx->validate();
        ctx->compile();
    }


    void OptixTracer::ClearSceneGraph()
    {
        ctx["lights"]->getBuffer()->destroy();
        lights.clear();
        for(auto instance : gis)
        {
            instance->getGeometry()->destroy();
            instance->destroy();
        }
        gis.clear();
        geometrygroup->destroy();

        matHandler.Clear();
    }

    void OptixTracer::Trace(unsigned int entryPoint, RTsize width, RTsize height, int renderingDivisionLevel)
    {
        //static unsigned int frame = 1;
        //ctx["frame"]->setUint(frame);
        //frame++;
        for(int i = 0; i < renderingDivisionLevel; ++i)
        {
            ctx["myStripe"]->setInt(i);
            ctx->launch(entryPoint, width, height / renderingDivisionLevel);
            if(renderingDivisionLevel > 1)
                cout << "DONE " << i + 1 << "/" << renderingDivisionLevel << endl;
        }
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

        ctx["eye"]->setFloat(Utils::glmToOptix(cam.position));
        ctx["U"]->setFloat(Utils::glmToOptix(cam.GetRight() * tanfov * cam.aspectRatio));
        ctx["V"]->setFloat(Utils::glmToOptix(cam.GetUp() * tanfov));
        ctx["W"]->setFloat(Utils::glmToOptix(cam.GetDirection()));
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

        Trace(0, w, h, SSrenderingDivisionLevel);

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
