#ifndef ENGINE_COLLIDER_MANAGER_HPP
#define ENGINE_COLLIDER_MANAGER_HPP

#include <engine/actor.hpp>
#include <engine/transform.hpp>
#include <engine/collider.hpp>

namespace Engine
{
    class ColliderManager
    {
    public:
        static int getTag(std::string tag);
        static int addTag(std::string tag, bool selfRelation);
        static void addRelation(int tag1, int tag2);
    protected:
        inline static glm::vec3 currentAxis;
        inline static double currentOverlap;
        inline static QuadTree* root;
        inline static std::map<std::string, int> tags;
        inline static std::map<int, std::vector<int>> relations;
        static void initialize();
        static void startDetection();
        static void narrowPhase(Node* node);
        static void collisionDetected(Collider* collider1, Collider* collider2);
        static bool checkCollision(Collider* collider1, Collider* collider2);
        static bool hasRelation(int tag1, int tag2);
    friend class GameLoop;
    };
}

#endif