#ifndef ENGINE_CIRCLE_COLLIDER_HPP
#define ENGINE_CIRCLE_COLLIDER_HPP

#include <engine/collider.hpp>

namespace Engine
{
    class CircleCollider : public Collider
    {
    public:
        void setRadius(float radius);
        float getRadius();
    protected:
        float radius;
        void start();
        void nodeUpdate();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec3> getAxis(Collider* collider);
        glm::vec2 getProjection(glm::vec3 axis);
    };
}

#endif