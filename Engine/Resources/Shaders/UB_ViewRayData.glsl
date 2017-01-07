
layout(std140) uniform ViewRayData
{
    vec4 fovData; // tan(fovy / 2) :: aspectRatio * tan(fovy / 2) :: - :: -
    vec4 cameraDist; // near :: far :: far-near :: 1/(far-near)
};
