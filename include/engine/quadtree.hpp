#ifndef ENGINE_QUADTREE_HPP
#define ENGINE_QUADTREE_HPP

#include <glm/glm.hpp>
#include <engine/component.hpp>
#include <iostream>
#include <vector>

namespace Engine
{
    class AABB;
    class Node;
    class QuadTree;
    
    // AABB detection class
    class AABB
    {
    public:
        AABB(float _x, float _y, float _expand);
        bool contains(AABB box);
        bool contains(glm::vec2 point);
        bool intersects(AABB box);
    protected:
        float x;
        float y;
        float expand;
    friend class QuadTree;
    };

    // node class for QuadTree
    class Node
    {
    public:
        Node(Component* _object, AABB _boundary);
        Component* getObject();
        QuadTree* getParent();
        AABB getBoundary();
        void update(AABB _boundary, QuadTree* root);
        void destroy();
    protected:
        AABB boundary;
        QuadTree* parent;
        Component* object;
    friend class QuadTree;
    };

    // QuadTree class
    class QuadTree
    {
    public:
        QuadTree(AABB _boundary);
        bool insert(Node* node);
        bool remove(Node* node);
        AABB getBoundary();
        QuadTree* expand(AABB _boundary);
        void find(AABB boundary, std::vector<Node*>& query);
        void find(glm::vec2 point, std::vector<Node*>& query);
    protected:
        inline static int capacity = 4;
        bool devided;
        AABB boundary;
        std::vector<Node*> nodes;
        QuadTree* northEast;
        QuadTree* northWest;
        QuadTree* southEast;
        QuadTree* southWest;
        void subdevide();
    };
}

#endif