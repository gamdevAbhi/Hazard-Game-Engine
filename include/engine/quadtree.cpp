#include <engine/quadtree.hpp>

// constructor
Engine::AABB::AABB(float _x, float _y, float _expand) :
x(_x),
y(_y),
expand(_expand)
{
}

// check if the box is contained by the aabb
bool Engine::AABB::contains(AABB box)
{
    if(x - expand <= box.x - box.expand && x + expand >= box.x + box.expand
    && y - expand <= box.y - box.expand && y + expand >= box.y + box.expand) return true;
    else return false;
}

// check if the point is contained by the aabb
bool Engine::AABB::contains(glm::vec2 point)
{
    if((x - expand <= point.x || x + expand >= point.x)
    && (y - expand <= point.y || y + expand >= point.y)) return true;
    else return false;
}

// check if the two box intersects each other
bool Engine::AABB::intersects(AABB box)
{
    return !(x - expand >= box.x + box.expand || x + expand <= box.x - box.expand
    || y + expand <= box.y - box.expand || y - expand >= box.y + box.expand);
}

// constructor
Engine::Node::Node(Engine::Component* _object, Engine::AABB _boundary) :
boundary(_boundary)
{
    object = _object;
    parent = nullptr;
}

// get object
Engine::Component* Engine::Node::getObject()
{
    return object;
}

// get parent
Engine::QuadTree* Engine::Node::getParent()
{
    return parent;
}


// get boundary
Engine::AABB Engine::Node::getBoundary()
{
    return boundary;
}

// updating the node
void Engine::Node::update(Engine::AABB _boundary, Engine::QuadTree* root)
{
    if(parent == nullptr) return;

    boundary = _boundary;

    parent->remove(this);
    root->insert(this);
}

// destroy the node
void Engine::Node::destroy()
{
    if(parent == nullptr) return;

    parent->remove(this);
    parent = nullptr;
}

// constructor
Engine::QuadTree::QuadTree(Engine::AABB _boundary) :
devided(false),
boundary(_boundary)
{
}

// insert the node to the quad tree
bool Engine::QuadTree::insert(Node* node)
{
    if(node == nullptr || node->getObject() == nullptr) return false;

    if(!boundary.contains(node->getBoundary())) return false;

    if(nodes.size() < capacity && devided == false) 
    {
        nodes.push_back(node);
        node->parent = this;
        return true;
    }
    
    if(devided == false) subdevide();

    if(northEast->insert(node)) return true;
    else if(northWest->insert(node)) return true;
    else if(southEast->insert(node)) return true;
    else if(southWest->insert(node)) return true;
    else 
    {
        nodes.push_back(node);
        node->parent = this;
    }

    return true;
}

// remove the node from the quad tree
bool Engine::QuadTree::remove(Node* node)
{
    if(node == nullptr) return false;

    for(int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i] != node) continue; 
        nodes.erase(nodes.begin() + i);
        return true;
    }
    
    if(devided == false) return false; 

    if(northEast->remove(node)) return true;
    else if(northWest->remove(node)) return true;
    else if(southEast->remove(node)) return true;
    else if(southWest->remove(node)) return true;
    else return false;
}

// get boundary
Engine::AABB Engine::QuadTree::getBoundary()
{
    return boundary;
}

// expand the quad tree
Engine::QuadTree* Engine::QuadTree::expand(AABB _boundary)
{
    QuadTree* nTree = nullptr;

    float x = _boundary.x - boundary.x;
    float y = _boundary.y - boundary.y;

    if(x > 0.f && y <= 0.f)
    {
        AABB nBoundary(boundary.x + boundary.expand, boundary.y - boundary.expand, boundary.expand * 2.0f);
        nTree = new QuadTree(nBoundary);
        nTree->subdevide();
        delete nTree->northWest;
        nTree->northWest = this;
    }
    else if(x < 0.f && y <= 0.f)
    {
        AABB nBoundary(boundary.x - boundary.expand, boundary.y - boundary.expand, boundary.expand * 2.0f);
        nTree = new QuadTree(nBoundary);
        nTree->subdevide();
        delete nTree->northEast;
        nTree->northEast = this;
    }
    else if(x >= 0.f && y > 0.f)
    {
        AABB nBoundary(boundary.x + boundary.expand, boundary.y + boundary.expand, boundary.expand * 2.0f);
        nTree = new QuadTree(nBoundary);
        nTree->subdevide();
        delete nTree->southWest;
        nTree->southWest = this;
    }
    else
    {
        AABB nBoundary(boundary.x - boundary.expand, boundary.y + boundary.expand, boundary.expand * 2.0f);
        nTree = new QuadTree(nBoundary);
        nTree->subdevide();
        delete nTree->southEast;
        nTree->southEast = this;
    }

    return nTree;
}

// find the nodes in quadtree that in the boundary
void Engine::QuadTree::find(AABB _boundary, std::vector<Engine::Node*>& query)
{
    if(boundary.intersects(_boundary) == false) return;
    
    for(int i = 0; i < nodes.size(); i++)
    {
        if(_boundary.intersects(nodes[i]->boundary)) query.push_back(nodes[i]);
    }

    if(devided == false) return;
    
    northEast->find(_boundary, query);
    northWest->find(_boundary, query);
    southEast->find(_boundary, query);
    southWest->find(_boundary, query);
}

// find the nodes in quadtree that contains the point
void Engine::QuadTree::find(glm::vec2 point, std::vector<Engine::Node*>& query)
{
    if(boundary.contains(point) == false) return;
    
    for(int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i]->boundary.contains(point)) query.push_back(nodes[i]);
    }

    if(devided == false) return;
    
    northEast->find(point, query);
    northWest->find(point, query);
    southEast->find(point, query);
    southWest->find(point, query);
}

// subdevide the region into 4
void Engine::QuadTree::subdevide()
{
    devided = true;

    AABB ne(boundary.x + boundary.expand / 2.0, boundary.y + boundary.expand / 2.0, 
    boundary.expand / 2.0);
    AABB nw(boundary.x - boundary.expand / 2.0, boundary.y + boundary.expand / 2.0, 
    boundary.expand / 2.0);
    AABB se(boundary.x + boundary.expand / 2.0, boundary.y - boundary.expand / 2.0, 
    boundary.expand / 2.0);
    AABB sw(boundary.x - boundary.expand / 2.0, boundary.y - boundary.expand / 2.0, 
    boundary.expand / 2.0);
    
    northEast = new QuadTree(ne);
    northWest = new QuadTree(nw);
    southEast = new QuadTree(se);
    southWest = new QuadTree(sw);

    std::vector<Node*> notIncluded;

    for(int i = 0; i < nodes.size(); i++)
    {
        if(northEast->insert(nodes[i])) continue;
        else if(northWest->insert(nodes[i])) continue;
        else if(southEast->insert(nodes[i])) continue;
        else if(southWest->insert(nodes[i])) continue;
        notIncluded.push_back(nodes[i]);
    }

    nodes = notIncluded;
}