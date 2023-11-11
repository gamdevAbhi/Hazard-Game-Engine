#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include <engine/window.hpp>
#include <engine/handler.hpp>
#include <engine/component.hpp>
#include <engine/transform.hpp>

namespace Engine
{
    // Camera class
    class Camera : public Component
    {
    public:
        glm::mat4 getOrtho();
        glm::vec2 getBoundary();
        glm::vec2 getPerWorldToScreen();
        glm::vec2 getWorldToScreenPos(glm::vec3 worldPosition);
        glm::vec3 getScreenToWorldPos(glm::vec2 screenPos);
        glm::vec3 getBackgroundColor();
        float getOrthographicSize();
        float getDiagonal();
        void setBackgroundColor(glm::vec3 color);
        void setOrthographicSize(float orthographicSize);
        static Camera* getRenderCamera();
    protected:
        float nearClip = 0.1f;
        float farClip = 100.0f;
        float orthographicSize = 10.0f;
        glm::vec3 backgroundColor;
        Transform* transform;
        void start();
        void onTransformChanged();
        void setDestroy();
        void updateOrtho();
    private:
        inline static Camera* renderCamera = nullptr;
        inline static Window* gameWindow = nullptr;
        glm::mat4 ortho;
    friend class Actor;
    friend class GameLoop;
    };
}

#endif