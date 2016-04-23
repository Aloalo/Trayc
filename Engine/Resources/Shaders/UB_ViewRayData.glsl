
layout(std140) uniform ViewRayData
{
    float tanHalfFovy; // tan(fovy / 2)
    float aspectTanHalfFovy; // aspectRatio * tan(fovy / 2)
};
