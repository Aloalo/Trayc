/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Utils.h>

using namespace optix;
using namespace glm;
using namespace std;

namespace trayc
{
	float Utils::eps = 1e-3f;
	float Utils::pi = 3.1415926f;
	string Utils::ptxPath = "../Resources/ptxfiles/";

	string Utils::PathToPTX(const string &filename)
	{
		return ptxPath + filename + ".ptx";
	}

	string Utils::DefTexture(const string &name)
	{
		return "../Resources/DefaultTextures/" + name;
	}

	string Utils::Resource(const string &name)
	{
		return "../Resources/" + name;
	}

	string Utils::Shader(const string &name)
	{
		return "../Resources/Shaders/" + name;
	}

	float3 Utils::glmToOptix(const vec3 &vec)
	{
		return make_float3(vec.x, vec.y, vec.z);
	}

	bool Utils::Equals(const float3 &x, const float3 &y)
	{
		return abs(x.x - y.x) < eps && abs(x.y - y.y) < eps && abs(x.z - y.z) < eps;
	}

	bool Utils::Equals(float x, float y)
	{
		return abs(x - y) < eps;
	}
}
