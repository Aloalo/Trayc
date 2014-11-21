
#include <Trayc/SceneGraph.h>
#include <Trayc/Environment.h>

using namespace optix;

namespace trayc
{
    void SceneGraph::Init()
    {
        topGroup = ctx->createGroup();

        lightGG = ctx->createGeometryGroup();
        topGroup->addChild(lightGG);

        staticGG = ctx->createGeometryGroup();
        topGroup->addChild(staticGG);
    }

    void SceneGraph::Add(GeometryInstance inst)
    {
        staticGG->addChild(inst);
    }

    void SceneGraph::Compile()
    {
        Acceleration accel = ctx->createAcceleration("Sbvh", "Bvh");
        accel->setProperty("index_buffer_name", "index_buffer");
        accel->setProperty("vertex_buffer_name", "vertex_buffer");
        accel->markDirty();
        staticGG->setAcceleration(accel);
        topGroup->setAcceleration(ctx->createAcceleration("NoAccel", "NoAccel"));
        lightGG->setAcceleration(ctx->createAcceleration("NoAccel", "NoAccel"));

        ctx->validate();
        ctx->compile();
    }

    void SceneGraph::Reset()
    {
        const int ctLights = lightGG->getChildCount();
        for(int i = ctLights - 1; i >= 0; --i)
        {
            lightGG->getChild(i)->getGeometry()->destroy();
            lightGG->getChild(i)->destroy();
            lightGG->removeChild(i);
        }

        const int ctStatic = staticGG->getChildCount();
        for(int i = ctStatic - 1; i >= 0; --i)
        {
            staticGG->getChild(i)->getGeometry()->destroy();
            staticGG->getChild(i)->destroy();
            staticGG->removeChild(i);
        }
        staticGG->getAcceleration()->destroy();
        Acceleration accel = ctx->createAcceleration("Sbvh", "Bvh");
        accel->setProperty("index_buffer_name", "index_buffer");
        accel->setProperty("vertex_buffer_name", "vertex_buffer");
        accel->markDirty();
        staticGG->setAcceleration(accel);
    }

    void SceneGraph::AddLight(GeometryInstance inst)
    {
        lightGG->addChild(inst);
    }
}

