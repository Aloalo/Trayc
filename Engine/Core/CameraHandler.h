#ifndef EN_CAMERAHANDLER_H
#define EN_CAMERAHANDLER_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/Updateable.h>

namespace engine
{
    class CameraHandler :
        public InputObserver, public Updateable
    {
    public:
        CameraHandler(const Camera &cam);
        virtual ~CameraHandler(void);

        virtual glm::mat4 GetProjectionMatrix() const = 0;
        virtual glm::mat4 GetViewMatrix() const = 0;
        const glm::vec3& GetPosition() const;
        const Camera& GetCamera() const;
        void SetAspectRatio(float aspect);

        Camera mCamera;
    };
}

#endif
