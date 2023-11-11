#include <engine/circlecollider.hpp>

// set the radius of the circle
void Engine::CircleCollider::setRadius(float radius)
{
    this->radius = radius;

    glm::vec3 pos = transform->getWorldPosition();
    float area = getRadius();

    AABB boundary(pos.x, pos.y, area);

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, *rootP);
}

// get the radius of the circle
float Engine::CircleCollider::getRadius()
{
    glm::vec3 scale(transform->getWorldScale());
    
    return radius * std::max(scale.x, scale.y);
}

void Engine::CircleCollider::start()
{
    tag = 0;
    radius = 1.f;
    fixed_x = false;
    fixed_y = false;
    transform = getActor()->getComponent<Transform>();

    glm::vec3 pos = transform->getWorldPosition();
    float area = getRadius();

    node = new Node(this, AABB(pos.x, pos.y, area));

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }

    (*rootP)->insert(node);
}

// updating the node
void Engine::CircleCollider::nodeUpdate()
{
    if(!isMoved) return;

    glm::vec3 pos = transform->getWorldPosition();
    float area = getRadius();

    AABB boundary(pos.x, pos.y, area);

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, *rootP);
}

// get the vertices of the circle
std::vector<glm::vec3> Engine::CircleCollider::getVertices()
{
    float area = getRadius();
    std::vector<glm::vec3> vertices;

    vertices.push_back(transform->getWorldPosAt(glm::vec3(-area, 0.f, 0.f)));
    vertices.push_back(transform->getWorldPosAt(glm::vec3(area, 0.f, 0.f)));

    return vertices;
}

// get the axis of the circle
std::vector<glm::vec3> Engine::CircleCollider::getAxis(Collider* collider)
{
    std::vector<glm::vec3> vertices = collider->getVertices();
    std::vector<glm::vec3> axises;
    glm::vec3 pos = transform->getWorldPosition();

    glm::vec3 closestAxis;
    double distance = DBL_MAX;

    for(int i = 0; i < vertices.size(); i++)
    {
        glm::vec3 axis = vertices[0] - pos;
        double length = glm::length(axis);

        if(length < distance)
        {
            distance = length;
            closestAxis = axis;
        }
    }

    axises.push_back(glm::normalize(closestAxis));
    
    return axises;
}

// get the projection
glm::vec2 Engine::CircleCollider::getProjection(glm::vec3 axis)
{
    glm::vec3 pos = transform->getWorldPosition();
    
    float dot = glm::dot(axis, pos);
    float min = dot - getRadius();
    float max = dot + getRadius();
    
    return glm::vec2(min, max);
}