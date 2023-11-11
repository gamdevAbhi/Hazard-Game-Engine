#include <engine/collider.hpp>

// get tag
int Engine::Collider::getTag()
{
    return tag;
}

// get fixed position
std::tuple<bool, bool> Engine::Collider::getFixed()
{
    return std::tuple<bool, bool>(fixed_x, fixed_y);
}

// set tag
void Engine::Collider::setTag(int tag)
{
    this->tag = tag;
}

// set fixed position
void Engine::Collider::setFixed(bool x, bool y)
{
    fixed_x = x;
    fixed_y = y;
}

// set the trigger
void Engine::Collider::setTrigger(bool isTrigger)
{
    this->isTrigger = isTrigger;
}

// on transform changed
void Engine::Collider::onTransformChanged()
{
    isMoved = true;
}

// call when component is removed
void Engine::Collider::onDestroy()
{
    node->destroy();
}

// call collider related functions
void Engine::Collider::call(Collider* collider, glm::vec3 axis)
{
    bool exist = false;

    if(colliders.find(collider) != colliders.end()) 
    {
        colliders[collider] = true;
        exist = true;
    }
    else colliders[collider] = true;

    if(collider->isTrigger == false && isTrigger == false)
    {
        for(int i = 0; i < getActor()->components.size(); i++)
        {
            if(exist == false) getActor()->components[i]->onCollisionEnter(collider);
            else getActor()->components[i]->onCollisionStay(collider);
        }

        if(fixed_x == true) axis.x = 0.f;
        if(fixed_y == true) axis.y = 0.f;

        transform->setWorldPosition(transform->getWorldPosition() + axis);
    }
    else if(isTrigger == true && collider->isTrigger == false)
    {
        for(int i = 0; i < getActor()->components.size(); i++)
        {
            if(exist == false) getActor()->components[i]->onTriggerEnter(collider);
            else getActor()->components[i]->onTriggerStay(collider);
        }
    }
}

// updating colliders stack
void Engine::Collider::stackUpdate()
{
    std::vector<Collider*> notFounds;

    for(auto i = colliders.begin(); i != colliders.end(); i++)
    {
        if(i->second == false) notFounds.push_back(i->first);
        i->second = false;
    }

    for(int i = 0; i < notFounds.size(); i++)
    {
        colliders.erase(notFounds[i]);

        if(notFounds[i]->isTrigger == false && isTrigger == false)
        {
            for(int j = 0; j < getActor()->components.size(); j++)
            {
                getActor()->components[j]->onCollisionExit(notFounds[i]);
            }
        }
        else if(isTrigger == true && notFounds[i]->isTrigger == false)
        {
            for(int j = 0; j < getActor()->components.size(); j++)
            {
                getActor()->components[j]->onTriggerExit(notFounds[i]);
            }
        }
    }
}

void Engine::Collider::nodeUpdate() {}

// check if two projection is overlap or not
bool Engine::Collider::isOverLap(glm::vec2 project1, glm::vec2 project2)
{
    return !(project1.y < project2.x || project2.y < project1.x);
}

// get the overlap between to projection
double Engine::Collider::getOverLap(glm::vec2 project1, glm::vec2 project2)
{
    float x = project1.y - project2.x;
    float y = project2.y - project1.x;

    return std::min(x, y);
}