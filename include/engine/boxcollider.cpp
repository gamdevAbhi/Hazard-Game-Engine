#include <engine/boxcollider.hpp>

// set boundary of collider
void Engine::BoxCollider::setBoundary(float left, float right, float up, float down)
{
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;

    glm::vec3 pos = transform->getWorldPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));
    
    AABB boundary(pos.x, pos.y, diagonal / 2.0f);

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, *rootP);
}

// get width of the collider
glm::vec2 Engine::BoxCollider::getWidth()
{
    float scale_x = transform->getWorldScale().x;
    return glm::vec2(left * scale_x, right * scale_x);
}

// get height of the collider
glm::vec2 Engine::BoxCollider::getHeight()
{
    float scale_y = transform->getWorldScale().y;
    return glm::vec2(up * scale_y, down * scale_y);
}

// start function
void Engine::BoxCollider::start()
{
    tag = 0;
    left = 1.f;
    right = 1.f;
    up = 1.f;
    down = 1.f;
    fixed_x = false;
    fixed_y = false;
    transform = getActor()->getComponent<Transform>();

    glm::vec3 pos = transform->getWorldPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));

    node = new Node(this, AABB(pos.x, pos.y, diagonal / 2.0f));

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }
    
    (*rootP)->insert(node);
}

// updating node
void Engine::BoxCollider::nodeUpdate()
{
    if(!isMoved) return;

    glm::vec3 pos = transform->getWorldPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));

    AABB boundary(pos.x, pos.y, diagonal / 2.0f);

    while(true)
    {
        if(!(*rootP)->getBoundary().contains(node->getBoundary())) *rootP = (*rootP)->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, *rootP);
}

// get the vertices
std::vector<glm::vec3> Engine::BoxCollider::getVertices()
{
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    glm::vec3 up_left_vertex = transform->getWorldPosAt(glm::vec3(-width.x, height.x, 0.0f));
    glm::vec3 up_right_vertex = transform->getWorldPosAt(glm::vec3(width.y, height.x, 0.0f));
    glm::vec3 down_left_vertex = transform->getWorldPosAt(glm::vec3(-width.x, -height.y, 0.0f));
    glm::vec3 down_right_vertex = transform->getWorldPosAt(glm::vec3(width.y, -height.y, 0.0f));

    std::vector<glm::vec3> vertices;
    
    vertices.push_back(up_left_vertex);
    vertices.push_back(up_right_vertex);
    vertices.push_back(down_left_vertex);
    vertices.push_back(down_right_vertex);

    return vertices;
}

// get edge axis
std::vector<glm::vec3> Engine::BoxCollider::getAxis(Collider* collider)
{
    std::vector<glm::vec3> vertices = getVertices();

    std::vector<glm::vec3> axis;
    
    glm::vec3 x_axis = glm::vec3(-(vertices[0].y - vertices[2].y), vertices[0].x - vertices[2].x, 0.0f);
    glm::vec3 y_axis = glm::vec3(-(vertices[1].y - vertices[0].y), vertices[1].x - vertices[0].x, 0.0f);

    x_axis = glm::normalize(x_axis);
    y_axis = glm::normalize(y_axis);

    axis.push_back(x_axis);
    axis.push_back(y_axis);

    return axis;
}

// get projection
glm::vec2 Engine::BoxCollider::getProjection(glm::vec3 axis)
{
    std::vector<glm::vec3> vertices = getVertices();

    double min = glm::dot(axis, vertices[0]);
    double max = min;

    for(int i = 1; i < vertices.size(); i++)
    {
        double p = glm::dot(axis, vertices[i]);

        if(p < min) min = p;
        else if(p > max) max = p;
    }

    return glm::vec2(min, max);
}