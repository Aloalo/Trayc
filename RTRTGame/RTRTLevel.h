#ifndef RTRT_RTRT_LEVEL_H
#define RTRT_RTRT_LEVEL_H

#include <vector>

#include "RTRTObjects.h"

class RTRTLevel
{
public:
    ~RTRTLevel(void);

    void Update(float dt);

    std::string mName = "default";
    std::vector<RTRTObject*> mObjects;
};

#endif