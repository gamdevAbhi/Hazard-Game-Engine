#include <engine/button.hpp>

// set boundary of button
void Engine::Button::setSize(float left, float right, float up, float down)
{
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;

    glm::vec3 pos = rect->getRectPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));
    
    AABB boundary(pos.x, pos.y, diagonal / 2.0f);

    while(true)
    {
        if(!root->getBoundary().contains(node->getBoundary())) root = root->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, root);
}

// set order
void Engine::Button::setOrder(int order)
{
    this->order = order;
}

// set the clickable status
void Engine::Button::setClickableStatus(bool status)
{
    isClickable = status;
}

// set the transparent status
void Engine::Button::setTransparentStatus(bool status)
{
    isTransparent = status;
}

// get width of the button
glm::vec2 Engine::Button::getWidth()
{
    float scale_x = rect->getRectScale().x;
    return glm::vec2(left * scale_x, right * scale_x);
}

// get height of the button
glm::vec2 Engine::Button::getHeight()
{
    float scale_y = rect->getRectScale().y;
    return glm::vec2(up * scale_y, down * scale_y);
}

// get order
int Engine::Button::getOrder()
{
    return order;
}

// get the clickable status
bool Engine::Button::getClickableStatus()
{
    return isClickable;
}

// get the transparent status
bool Engine::Button::getTransparentStatus()
{
    return isTransparent;
}

// get the hover status
Engine::Button::HoverStatus Engine::Button::getHoverStatus()
{
    return hoverStatus;
}

// get the click status
Engine::Button::ClickStatus Engine::Button::getClickStatus()
{
    return clickStatus;
}

// start function
void Engine::Button::start()
{
    left = 1.f;
    right = 1.f;
    up = 1.f;
    down = 1.f;
    order = 0;
    isClickable = true;
    isTransparent = false;
    isClicked = false;
    isHover = false;
    rect = getActor()->getComponent<RectTransform>();

    glm::vec3 pos = rect->getRectPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));

    node = new Node(this, AABB(pos.x, pos.y, diagonal / 2.0f));

    while(true)
    {
        if(!root->getBoundary().contains(node->getBoundary())) root = root->expand(node->getBoundary());
        else break;
    }
    
    root->insert(node);
}

// on transform changed
void Engine::Button::onTransformChanged()
{
    glm::vec3 pos = rect->getRectPosition();
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    float max_x = width.x + width.y;
    float max_y = height.x + height.y;
    float diagonal = std::sqrt(std::pow(max_x, 2) + std::pow(max_y, 2));
    
    AABB boundary(pos.x, pos.y, diagonal / 2.0f);

    while(true)
    {
        if(!root->getBoundary().contains(node->getBoundary())) root = root->expand(node->getBoundary());
        else break;
    }

    node->update(boundary, root);
}

// on destroy
void Engine::Button::onDestroy()
{
    node->destroy();
}

// clicked happend
void Engine::Button::clicked()
{
    isClicked = true;
}

// hover happend
void Engine::Button::hovered()
{
    isHover = true;
}

// update status
void Engine::Button::updateStatus()
{
    if(isHover)
    {
        if(hoverStatus == HOVER_ENTER || hoverStatus == HOVER_STAY) hoverStatus = HOVER_STAY;
        else hoverStatus = HOVER_ENTER;
    }
    else
    {
        if(hoverStatus == HOVER_EXIT) hoverStatus = HOVER_NONE;
        else hoverStatus = HOVER_EXIT;
    }

    if(isClicked)
    {
        if(clickStatus == BUTTON_CLICK || clickStatus == BUTTON_HOLD) clickStatus = BUTTON_HOLD;
        else clickStatus = BUTTON_CLICK;
    }
    else
    {
        if(clickStatus == BUTTON_RELEASE || hoverStatus == HOVER_EXIT) clickStatus = BUTTON_NONE;
        else clickStatus = BUTTON_RELEASE;
    }

    isClicked = false;
    isHover = false;
}

// get the vertices
std::vector<glm::vec3> Engine::Button::getVertices()
{
    glm::vec2 width = getWidth();
    glm::vec2 height = getHeight();

    glm::vec3 up_left_vertex = rect->getScreenPosAt(glm::vec3(-width.x, height.x, 0.0f));
    glm::vec3 up_right_vertex = rect->getScreenPosAt(glm::vec3(width.y, height.x, 0.0f));
    glm::vec3 down_left_vertex = rect->getScreenPosAt(glm::vec3(-width.x, -height.y, 0.0f));
    glm::vec3 down_right_vertex = rect->getScreenPosAt(glm::vec3(width.y, -height.y, 0.0f));

    std::vector<glm::vec3> vertices;
    
    vertices.push_back(up_left_vertex);
    vertices.push_back(up_right_vertex);
    vertices.push_back(down_left_vertex);
    vertices.push_back(down_right_vertex);

    return vertices;
}

// get edge axis
std::vector<glm::vec3> Engine::Button::getAxis()
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

// get the projection
glm::vec2 Engine::Button::getProjection(glm::vec3 axis)
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