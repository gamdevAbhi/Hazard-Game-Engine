#ifndef ENGINE_BOX_COLLIDER_HPP
#define ENGINE_BOX_COLLIDER_HPP

#include <engine/collider.hpp>

namespace Engine
{
    class BoxCollider : public Collider
    {
    public:
        void setBoundary(float left, float right, float up, float down);
        glm::vec2 getWidth();
        glm::vec2 getHeight();
    protected:
        float left;
        float right;
        float up;
        float down;
        void start();
        void nodeUpdate();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec3> getAxis(Collider* collider);
        glm::vec2 getProjection(glm::vec3 axis);
    };
}

#endif