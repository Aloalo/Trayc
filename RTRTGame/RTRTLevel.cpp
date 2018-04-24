#include "RTRTLevel.h"

RTRTLevel::~RTRTLevel(void)
{
    for (RTRTObject *o : mObjects) {
        delete o;
    }
}

void RTRTLevel::Update(float dt)
{
}
