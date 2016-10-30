
#ifndef EN_RENDERABLE_H
#define EN_RENDERABLE_H

#include <glm/glm.hpp>
#include <Engine/Engine/RenderingContext.h>

namespace engine
{
    class Renderable
    {
    public:
        Renderable(void);
        virtual ~Renderable(void);

        virtual void Draw(const RenderingContext &rContext) const = 0;

        bool mIsActive;
    };
}

#endif
