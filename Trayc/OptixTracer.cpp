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

    static inline Geometry GetGeometry(const aiMesh *mesh, const aiMaterial *mat = nullptr, const std::string &path = Utils::DefTexture(""))
    {
        vector<int3> indices;
        indices.reserve(mesh->mNumFaces);
        for(int i = 0; i < mesh->mNumFaces; ++i)
            indices.push_back(make_int3(mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]));

        vector<float3> vertexData;
        vertexData.reserve(mesh->mNumVertices);
        vector<float3> normalData;
        normalData.reserve(mesh->mNumVertices);
        vector<float3> tangentData;
        tangentData.reserve(mesh->mNumVertices);
        vector<float3> bitangentData;
        bitangentData.reserve(mesh->mNumVertices);
        vector<float2> uvData;
        uvData.reserve(mesh->mNumVertices);

        const bool hasNormalMap = mat == NULL ? false : mat->GetTextureCount(aiTextureType_NORMALS) || mat->GetTextureCount(aiTextureType_HEIGHT);

        for(int i = 0; i < mesh->mNumVertices; ++i)
        {
            vertexData.push_back(Utils::aiToOptix(mesh->mVertices[i]));
            normalData.push_back(Utils::aiToOptix(mesh->mNormals[i]));
            if(hasNormalMap)
            {
                tangentData.push_back(Utils::aiToOptix(mesh->mTangents[i]));
                bitangentData.push_back(Utils::aiToOptix(mesh->mBitangents[i]));
            }

            if(mesh->HasTextureCoords(0))
                uvData.push_back(make_float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }

        Geometry gMesh = ctx->createGeometry();
        gMesh->setPrimitiveCount(indices.size());
        gMesh->setBoundingBoxProgram(Programs::meshBoundingBox);
        gMesh->setIntersectionProgram(Programs::meshIntersect);

        gMesh["vertex_buffer"]->setBuffer(GetBufferFromVector(vertexData, RT_FORMAT_FLOAT3));
        gMesh["normal_buffer"]->setBuffer(GetBufferFromVector(normalData, RT_FORMAT_FLOAT3));
        gMesh["tangent_buffer"]->setBuffer(GetBufferFromVector(tangentData, RT_FORMAT_FLOAT3));
        gMesh["bitangent_buffer"]->setBuffer(GetBufferFromVector(bitangentData, RT_FORMAT_FLOAT3));
        gMesh["normal_map"]->setTextureSampler(OptixTextureHandler::Get().Get(
            MaterialHandler::Get().GetTextureName(mat, aiTextureType_HEIGHT, path, "bumpDefault.png")));

        gMesh["texcoord_buffer"]->setBuffer(GetBufferFromVector(uvData, RT_FORMAT_FLOAT2));
        gMesh["index_buffer"]->setBuffer(GetBufferFromVector(indices, RT_FORMAT_INT3));

        return gMesh;
    }
    //HELPER FUNCTIONS

	OptixTracer::OptixTracer(void) :
		SETTING(shadowSamples),
		SETTING(maxRayDepth),
		SETTING(MSAA),
		SETTING(renderingDivisionLevel),
		SETTING(SSbufferWidth),
		SETTING(SSbufferHeight),
		SETTING(dofSamples)
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
		printf("Available device memory: %d MB\n", ctx->getAvailableDeviceMemory(0) >> 20);

		Programs::Init(ctx);

		ctx->setRayTypeCount(2);
		ctx->setEntryPointCount(1);
		ctx->setCPUNumThreads(4);
		ctx->setStackSize(768 + 256 * maxRayDepth);

		ctx["radiance_ray_type"]->setUint(0);
		ctx["shadow_ray_type"]->setUint(1);
		ctx["scene_epsilon"]->setFloat(1.e-2f);
		ctx["importance_cutoff"]->setFloat(0.01f);
		ctx["renderingDivisionLevel"]->setInt(renderingDivisionLevel);
		ctx["ambient_light_color"]->setFloat(0.3f, 0.3f, 0.3f);

		ctx["max_depth"]->setInt(maxRayDepth);
		ctx["shadow_samples"]->setInt(shadowSamples);

		outBuffer = ctx->createBufferFromGLBO(RT_BUFFER_OUTPUT, GLBO);
		outBuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
		outBuffer->setSize(Environment::Get().bufferWidth, Environment::Get().bufferHeight);
		ctx["output_buffer"]->setBuffer(outBuffer);

		SSbuffer = ctx->createBuffer(RT_BUFFER_OUTPUT);
		SSbuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
		SSbuffer->setSize(SSbufferWidth, SSbufferHeight);

		ctx->setRayGenerationProgram(0, Programs::rayGeneration);
		ctx["AAlevel"]->setInt(MSAA);
		ctx["focal_length"]->setFloat(10.0f);
		ctx["aperture_radius"]->setFloat(0.025f);
		ctx["dof_samples"]->setInt(dofSamples);
		ctx["AAlevel"]->setInt(MSAA);

		ctx->setMissProgram(0, Programs::envmapMiss);
		ctx["envmap"]->setTextureSampler(OptixTextureHandler::Get().Get(Utils::DefTexture("environment.jpg")));

		ctx->setExceptionProgram(0, Programs::exception);
		ctx["bad_color"]->setFloat(1.0f, 0.0f, 0.0f);
	}

	void OptixTracer::AddMesh(const string &path, const aiMesh *mesh, const aiMaterial *mat)
	{
		const Geometry gMesh = GetGeometry(mesh, mat, path);
		const Material material = MaterialHandler::Get().CreateMaterial(path, mat);

		GeometryInstance inst = ctx->createGeometryInstance();
		inst->setMaterialCount(1);
		inst->setGeometry(gMesh);
		inst->setMaterial(0, material);

		gis.push_back(inst);
	}

	void OptixTracer::AddMesh(const optix::Material mat, const aiMesh *mesh)
	{
		const Geometry gMesh = GetGeometry(mesh);

		GeometryInstance inst = ctx->createGeometryInstance();
		inst->setMaterialCount(1);
		inst->setGeometry(gMesh);
		inst->setMaterial(0, mat);

		gis.push_back(inst);
	}

	void OptixTracer::AddScene(const std::string &path, const aiScene *scene)
	{
		for(int i = 0; i < scene->mNumMeshes; ++i)
			AddMesh(path, scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
	}

	void OptixTracer::AddScene(const optix::Material mat, const aiScene * scene)
	{
		for(int i = 0; i < scene->mNumMeshes; ++i)
			AddMesh(mat, scene->mMeshes[i]);
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

		trans = ctx->createTransform();
		trans->setChild(geometrygroup);
		const float s = 0.05f;
		trans->setMatrix(false, (float*)&glm::scale(glm::mat4(1.0f), glm::vec3(s)), (float*)&glm::inverse(glm::scale(glm::mat4(1.0f), glm::vec3(s))));
		ctx["top_object"]->set(trans);

		geometrygroup->setAcceleration(ctx->createAcceleration("Sbvh", "Bvh"));

		accelHandler.SetMesh(Utils::Resource("accelCaches/accel.accelcache"));
		accelHandler.LoadAccelCache(geometrygroup);

		if(!accelHandler.accel_cache_loaded)
		{
			Acceleration accel = ctx->createAcceleration("Sbvh", "Bvh");

			accel->setProperty("index_buffer_name", "index_buffer");
			accel->setProperty("vertex_buffer_name", "vertex_buffer");
			accel->markDirty();
			geometrygroup->setAcceleration(accel);
			ctx->launch(0, 0, 0);

			accelHandler.SaveAccelCache(geometrygroup);
		}
		ctx->validate();
		ctx->compile();

		printf("Available device memory after compile: %d MB\n", ctx->getAvailableDeviceMemory(0) >> 20);
	}


	void OptixTracer::ClearSceneGraph()
	{
        ctx["lights"]->getBuffer()->destroy();
        lights.clear();
        for(auto instance : gis)
            instance->destroy();
		gis.clear();
        geometrygroup->destroy();
        trans->destroy();
	}

	void OptixTracer::Trace(unsigned int entryPoint, RTsize width, RTsize height)
	{
		static int frame = 1;
		frame++;
		ctx["frame"]->setInt(frame);
		for(int i = 0; i < renderingDivisionLevel; ++i)
		{
			ctx["myStripe"]->setInt(i);
			ctx->launch(entryPoint, width, height / renderingDivisionLevel);
            if(renderingDivisionLevel > 1)
                printf("DONE %d / %d\n", i + 1, renderingDivisionLevel);
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
		RTsize w, h;
		SSbuffer->getSize(w, h);

		const int rdl = 270;
		const int tmp = renderingDivisionLevel;
		renderingDivisionLevel = rdl;
		ctx["AAlevel"]->setInt(2);
		ctx["renderingDivisionLevel"]->setInt(rdl);
		ctx["dof_samples"]->setInt(1);
		ctx["shadow_samples"]->setInt(128);
		ctx["output_buffer"]->setBuffer(SSbuffer);
		Trace(0, w, h);
		ctx["output_buffer"]->setBuffer(outBuffer);
		ctx["shadow_samples"]->setInt(shadowSamples);
		renderingDivisionLevel = tmp;
		ctx["dof_samples"]->setInt(dofSamples);
		ctx["renderingDivisionLevel"]->setInt(renderingDivisionLevel);
		ctx["AAlevel"]->setInt(MSAA);
		
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
