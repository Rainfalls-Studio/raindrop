#include "Raindrop/Modules/Scene/Components/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Raindrop::Components{

    Camera::Camera(){
        updateMatrix();
    }

    void Camera::updateMatrix(){
        switch (_projectionType){
            case ProjectionType::PERSPECTIVE: _projectionMatrix = glm::perspectiveFov(_fov, _width, _height, _near, _far); break;
            case ProjectionType::ORTHOGRAPHIC: _projectionMatrix = glm::ortho(-_width, _width, _height, -_height); break;
        }
    }
    
    void Camera::setClipPlans(float near, float far){
        _near = near;
        _far = far;
        updateMatrix();
    }

    void Camera::setSize(float width, float height){
        _width = width;
        _height = height;
        updateMatrix();
    }

    void Camera::setNear(float near){
        _near = near;
        updateMatrix();
    }

    void Camera::setFar(float far){
        _far = far;
        updateMatrix();
    }

    void Camera::setWidth(float width){
        _width = width;
        updateMatrix();
    }

    void Camera::setHeight(float height){
        _height = height;
        updateMatrix();
    }

    void Camera::setFov(float fov){
        _fov = fov;
        updateMatrix();
    }

    void Camera::setProjectionType(ProjectionType type){
        _projectionType = type;
        updateMatrix();
    }

    float Camera::getFar() const{
        return _far;
    }

    float Camera::getNear() const{
        return _near;
    }

    float Camera::getWidth() const{
        return _width;
    }

    float Camera::getHeight() const{
        return _height;
    }

    glm::vec2 Camera::getClipPlans() const{
        return {_near, _far};
    }

    glm::vec2 Camera::getSize() const{
        return {_width, _height};
    }

    float Camera::getFov() const{
        return _fov;
    }

    Camera::ProjectionType Camera::getProjectionType() const{
        return _projectionType;
    }

    const glm::mat4& Camera::getProjectionMatrix() const{
        return _projectionMatrix;
    }

    float Camera::getAspectRatio() const{
        return _width / _height;
    }

    Camera::RenderData Camera::makeRenderData(const glm::mat4& transform) const{
        return RenderData{
            _projectionMatrix * glm::inverse(transform)
        };
    }
}
