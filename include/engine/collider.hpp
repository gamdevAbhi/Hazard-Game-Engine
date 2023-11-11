#ifndef ENGINE_COLLIDER_HPP
#define ENGINE_COLLIDER_HPP

#include <glm/glm.hpp>
#include <engine/actor.hpp>
#include <engine/component.hpp>
#include <engine/transform.hpp>
#include <engine/quadtree.hpp>
#include <map>

namespace Engine
{
    class Collider : public Component
    {
    public:
        virtual std::vector<glm::vec3> getVertices() = 0;
        int getTag();
        std::tuple<bool, bool> getFixed();
        void setTag(int tag);
        void setFixed(bool x, bool y);
        void setTrigger(bool isTrigger);
    protected:
        inline static QuadTree** rootP = nullptr;
        Node* node;
        int tag;
        bool isTrigger;
        bool fixed_x;
        bool fixed_y;
        bool isMoved;
        Transform* transform;
        std::map<Collider*, bool> colliders;
        void onTransformChanged();
        void onDestroy();
        void call(Collider* collider, glm::vec3 axis);
        void stackUpdate();
        virtual void nodeUpdate();
        virtual std::vector<glm::vec3> getAxis(Collider* collider) = 0;
        virtual glm::vec2 getProjection(glm::vec3 axis) = 0;
        static bool isOverLap(glm::vec2 project1, glm::vec2 project2);
        static double getOverLap(glm::vec2 project1, glm::vec2 project2);
    friend class ColliderManager;
    };
}

#endif