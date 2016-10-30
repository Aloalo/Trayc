
#include <Engine/Geometry/Frustum.h>

using namespace glm;

namespace engine
{
    Frustum::Frustum(void)
    {
    }

    Frustum::Frustum(const vec3 &p, const vec3 &l, const vec3 &u, float fov, float ar, float near, float far)
    {
        Set(p, l, u, fov, ar, near, far);
    }

    void Frustum::Set(const vec3 &p, const vec3 &l, const vec3 &u, float fov, float ar, float near, float far)
    {
        const vec3 Z = normalize(p - l);
        const vec3 X = normalize(cross(u, Z));
        const vec3 Y = cross(Z, X);

        const vec3 nc = p - Z * near;
        const vec3 fc = p - Z * far;

        const float tang = tanf(radians(fov));
        const float nh = near * tang;
        const float nw = nh * ar;
        const float fh = far * tang;
        const float fw = fh * ar;

        const vec3 ntl = nc + Y * nh - X * nw;
        const vec3 ntr = nc + Y * nh + X * nw;
        const vec3 nbl = nc - Y * nh - X * nw;
        const vec3 nbr = nc - Y * nh + X * nw;

        const vec3 ftl = fc + Y * fh - X * fw;
        const vec3 ftr = fc + Y * fh + X * fw;
        const vec3 fbl = fc - Y * fh - X * fw;
        const vec3 fbr = fc - Y * fh + X * fw;

        mPlanes[TOP].Set(ntr, ntl, ftl);
        mPlanes[BOTTOM].Set(nbl, nbr, fbr);
        mPlanes[LEFT].Set(ntl, nbl, fbl);
        mPlanes[RIGHT].Set(nbr, ntr, fbr);
        mPlanes[NEARP].Set(ntl, ntr, nbr);
        mPlanes[FARP].Set(ftr, ftl, fbl);
    }


    bool Frustum::Intersect(const vec3 &p) const
    {
        for(const Plane &pl : mPlanes) {
            if(pl.Distance(p) < 0.0f)
                return false;
        }
        return true;
    }


    bool Frustum::Intersect(const Sphere &s) const
    {
        for(const Plane &pl : mPlanes) {
            if(pl.Distance(s.mCenter) < -s.mRadius)
                return false;
        }
        return true;
    }


    bool Frustum::Intersect(const AABB &b) const
    {
        for(const Plane &pl : mPlanes) {
            if(pl.Distance(b.GetPositiveVertex(pl.mNormal)) < 0.0f)
                return false;
        }
        return true;
    }
}
