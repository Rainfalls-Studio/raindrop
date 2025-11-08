#pragma once

#include <glm/glm.hpp>

namespace Raindrop::Components{
    class Camera{
        public:
            enum class ProjectionType{
                ORTHOGRAPHIC,
                PERSPECTIVE
            };

            Camera();
            ~Camera() = default;

            void setNear(float near);
            void setFar(float far);
            void setWidth(float width);
            void setHeight(float height);
            void setClipPlans(float near, float far);
            void setSize(float width, float height);
            void setFov(float fov);
            void setProjectionType(ProjectionType type);

            float getFar() const;
            float getNear() const;
            float getWidth() const;
            float getHeight() const;
            glm::vec2 getClipPlans() const;
            glm::vec2 getSize() const;
            float getFov() const;
            float getAspectRatio() const;
            ProjectionType getProjectionType() const;

            const glm::mat4& getProjectionMatrix() const;

        private:
            ProjectionType _projectionType = ProjectionType::PERSPECTIVE;
            float _near = 0.05f, _far = 4000.f;
            float _width = 1.f, _height = 1.f;
            float _fov = 75.f;

            glm::mat4 _projectionMatrix;
            void updateMatrix();

    };
}