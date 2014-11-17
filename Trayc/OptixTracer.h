/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_OPTIXTRACER_H
#define TRAYC_OPTIXTRACER_H

#include <assimp/scene.h>
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
		
		void AddScene(const std::string &path, const aiScene *scene);
		void AddMesh(const std::string &path, const aiMesh *mesh, const aiMaterial *mat);
		void AddScene(const optix::Material mat, const aiScene *scene);
		void AddMesh(const optix::Material mat, const aiMesh *mesh);
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

        //camera
        engine::Setting<float> apertureRadius;
        engine::Setting<float> focalLength;

        //real time
        engine::Setting<int> maxRayDepth;
        engine::Setting<int> shadowSamples;
        engine::Setting<int> dofSamples;
        engine::Setting<int> MSAA;

        //screenshot
        engine::Setting<int> SSmaxRayDepth;
        engine::Setting<int> SSrenderingDivisionLevel;
        engine::Setting<int> SSshadowSamples;
        engine::Setting<int> SSdofSamples;
        engine::Setting<int> SSMSAA;
        engine::Setting<int> SSbufferWidth;
        engine::Setting<int> SSbufferHeight;


	private:
		optix::Buffer SSbuffer;
		AccelHandler accelHandler;

        optix::Transform trans;
        optix::GeometryGroup geometrygroup;
		std::vector<optix::GeometryInstance> gis;
		std::vector<BasicLight> lights;
	};
};

#endif
