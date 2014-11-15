/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TESTER_BOX_H
#define TESTER_BOX_H

#include <optix_math.h>

namespace trayc
{
	struct Box
	{
		Box(void);
		Box(const optix::float3 &boxmin, const optix::float3 &boxmax, int materialIdx);

		Box translated(const optix::float3 &t) const;
		Box translated(const optix::float3 &t, int matId) const;
		float getWidth() const;
		float getHeight() const;
		float getDepth() const;

		optix::float3 boxmin;
		optix::float3 boxmax;
		int matIdx;
	};
}

#endif
