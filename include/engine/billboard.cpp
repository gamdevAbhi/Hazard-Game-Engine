#include <engine/billboard.hpp>

// set the color
void Engine::Billboard::setColor(glm::vec4 color)
{
    this->color = color;
}

// set sprite
void Engine::Billboard::setSprite(Sprite* sprite)
{
    if(sprite == nullptr) this->sprite = defaultSprite;
    else this->sprite = sprite;
}

// get the color
glm::vec4 Engine::Billboard::getColor()
{
    return color;
}

// get sprite
Engine::Sprite* Engine::Billboard::getSprite()
{
    if(sprite != defaultSprite) return sprite;
    else return nullptr;
}

void Engine::Billboard::start()
{
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sprite = defaultSprite;
    order = 0;
    
    glm::vec2 position = getActor()->getComponent<RectTransform>()->getRectPosition();
    glm::vec2 scale = getActor()->getComponent<RectTransform>()->getRectScale();

    float x = std::abs(scale.x) * 2;
    float y = std::abs(scale.y) * 2;
    float diagonal = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

    node = new Node(this, AABB(position.x, position.y, diagonal / 2.0f));

    while(true)
    {
        if(!root->getBoundary().contains(node->getBoundary())) root = root->expand(node->getBoundary());
        else break;
    }

    root->insert(node);
}

void Engine::Billboard::onTransformChanged()
{
    glm::vec2 position = getActor()->getComponent<RectTransform>()->getRectPosition();
    glm::vec2 scale = getActor()->getComponent<RectTransform>()->getRectScale();

    float x = std::abs(scale.x) * 2;
    float y = std::abs(scale.y) * 2;
    float diagonal = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

    AABB boundary = AABB(position.x, position.y, diagonal / 2.0f);
    
    while(true)
    {
        if(!root->getBoundary().contains(boundary)) root = root->expand(boundary);
        else break;
    }

    node->update(boundary, root);
}

void Engine::Billboard::onDestroy()
{
    node->destroy();
}

// initialize the members of billboard
void Engine::Billboard::initialize()
{
    root = new QuadTree(AABB(0, 0, 100));
}

// draw billboards
void Engine::Billboard::draw()
{
    renderShader->use();
    renderVao->bind();

    glm::mat4 ortho = UI::getOrtho();
    glm::mat4 screen_transform = getActor()->getComponent<RectTransform>()->getRectMatrix();

    glUniform4fv(renderShader->getLocation("color"), 1, &color[0]);
    glUniformMatrix4fv(renderShader->getLocation("viewProjMat"), 1, GL_FALSE, &ortho[0][0]);
    glUniformMatrix4fv(renderShader->getLocation("transformMat"), 1, GL_FALSE, &screen_transform[0][0]);
    glUniform1i(renderShader->getLocation("tex"), 0);
    
    sprite->bind();
    
    glDrawElements(GL_TRIANGLES, quadIndices.size(), GL_UNSIGNED_INT, 0);
    
    sprite->unbind();
    renderVao->unbind();
}