#include <engine/collidermanager.hpp>

// get the tag value
int Engine::ColliderManager::getTag(std::string tag)
{
    if(tags.find(tag) != tags.end()) return tags[tag];
    else return -1;
}

// add tag to the collision relation
int Engine::ColliderManager::addTag(std::string tag, bool selfRelation)
{
    if(tags.find(tag) != tags.end()) return -1;

    tags[tag] = tags.size();

    if(selfRelation) relations[tags[tag]].push_back(tags[tag]);

    relations[tags[tag]].push_back(tags["default"]);
    relations[tags["default"]].push_back(tags[tag]);

    return tags[tag];
}

// add relation between two tag
void Engine::ColliderManager::addRelation(int tag1, int tag2)
{
    if(tag1 == -1 || tag2 == -1) return;

    for(int i = 0; i < relations[tag1].size(); i++)
    {
        if(relations[tag1][i] == tag2) return;
    }
    
    relations[tag1].push_back(tag2);
    relations[tag2].push_back(tag1);
}

// call for initialization
void Engine::ColliderManager::initialize()
{
    tags["default"] = 0;
    relations[0].push_back(0);
    
    root = new QuadTree(AABB(0, 0, 100));
    Collider::rootP = &root;
}

// start detecting collision
void Engine::ColliderManager::startDetection()
{
    std::vector<Node*> nodes;

    root->find(root->getBoundary(), nodes);

    for(int i = 0; i < nodes.size(); i++)
    {
        narrowPhase(nodes[i]);
    }

    for(int i = 0; i < nodes.size(); i++)
    {
        Collider* collider = dynamic_cast<Collider*>(nodes[i]->getObject());
        collider->stackUpdate();
        collider->nodeUpdate();
    }
}

// narrow phase collision detection
void Engine::ColliderManager::narrowPhase(Node* node)
{
    std::vector<Node*> potentialCollisions;
    node->getParent()->find(node->getBoundary(), potentialCollisions);

    Collider* collider = dynamic_cast<Collider*>(node->getObject());

    for(int i = 0; i < potentialCollisions.size(); i++)
    {
        if(node == potentialCollisions[i]) continue;

        Collider* collider1 = dynamic_cast<Collider*>(potentialCollisions[i]->getObject());
        
        if(!hasRelation(collider->tag, collider1->tag)) continue;

        if(checkCollision(collider, collider1))
        {
            collisionDetected(collider, collider1);
        }
    }
}

// calls when collision detected between two colliders
void Engine::ColliderManager::collisionDetected(Collider* collider1, Collider* collider2)
{
    glm::vec3 offset = currentAxis;
    glm::vec3 pos1 = collider1->transform->getWorldPosition();
    glm::vec3 pos2 = collider2->transform->getWorldPosition();
    offset *= currentOverlap;

    float dot1 = glm::dot(currentAxis, pos2 - pos1);
    float dot2 = glm::dot(currentAxis, pos1 - pos2);

    if(dot1 < dot2)
    {
        collider1->call(collider2, offset);
        collider2->call(collider1, -offset);
    }
    else
    {
        collider1->call(collider2, -offset);
        collider2->call(collider1, offset);
    }

    collider1->nodeUpdate();
    collider2->nodeUpdate();
}

// checking collision between two box collider
bool Engine::ColliderManager::checkCollision(Collider* collider1, Collider* collider2)
{
    glm::vec3 axis;
    double overlap = DBL_MAX;

    std::vector<glm::vec3> axises1 = collider1->getAxis(collider2);
    std::vector<glm::vec3> axises2 = collider2->getAxis(collider1);

    for(int i = 0; i < axises1.size(); i++)
    {
        glm::vec2 projection1 = collider1->getProjection(axises1[i]);
        glm::vec2 projection2 = collider2->getProjection(axises1[i]);

        if(Collider::isOverLap(projection1, projection2) == false) return false;

        double current = Collider::getOverLap(projection1, projection2);
        
        if(current < overlap)
        {
            axis = axises1[i];
            overlap = current;
        }
    }

    for(int i = 0; i < axises2.size(); i++)
    {
        glm::vec2 projection1 = collider1->getProjection(axises2[i]);
        glm::vec2 projection2 = collider2->getProjection(axises2[i]);

        if(Collider::isOverLap(projection1, projection2) == false) return false;

        double current = Collider::getOverLap(projection1, projection2);
        
        if(current < overlap)
        {
            axis = axises2[i];
            overlap = current;
        }
    }

    currentAxis = axis;
    currentOverlap = overlap;

    return true;
}

// check if relation exist
bool Engine::ColliderManager::hasRelation(int tag1, int tag2)
{
    if(relations.find(tag1) == relations.end() || relations.find(tag2) == relations.end()) return false;

    for(int i = 0; i < relations[tag1].size(); i++) if(relations[tag1][i] == tag2) return true;
    
    return false;
}