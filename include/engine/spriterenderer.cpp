#include <engine/spriterenderer.hpp>

// get the color
glm::vec4 Engine::SpriteRenderer::getColor()
{
    return color;
}

// get sprite
Engine::Sprite* Engine::SpriteRenderer::getSprite()
{
    if(sprite != defaultSprite) return sprite;
    else return nullptr;
}

// set the color
void Engine::SpriteRenderer::setColor(glm::vec4 color)
{
    this->color = color;
}

// set sprite
void Engine::SpriteRenderer::setSprite(Sprite* sprite)
{
    if(sprite == nullptr) this->sprite = defaultSprite;
    else this->sprite = sprite;
}

// start function
void Engine::SpriteRenderer::start()
{
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sprite = defaultSprite;
    order = 0;
    
    glm::vec2 position = getActor()->getComponent<Transform>()->getWorldPosition();
    glm::vec2 scale = getActor()->getComponent<Transform>()->getWorldScale();

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

// on transform changed
void Engine::SpriteRenderer::onTransformChanged()
{
    glm::vec2 position = getActor()->getComponent<Transform>()->getWorldPosition();
    glm::vec2 scale = getActor()->getComponent<Transform>()->getWorldScale();

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

// on destroy
void Engine::SpriteRenderer::onDestroy()
{
    node->destroy();
}

// initialize the members of sprite renderer
void Engine::SpriteRenderer::initialize()
{
    root = new QuadTree(AABB(0, 0, 100));
}

// renders sprites
void Engine::SpriteRenderer::draw()
{
    renderShader->use();
    renderVao->bind();

    Engine::Camera* camera = Engine::Camera::getRenderCamera();
    glm::mat4 camera_matrix = camera->getOrtho();

    Transform* transform = getActor()->getComponent<Transform>();
    glm::mat4 world_transform = transform->getMatrix();

    glUniform4fv(renderShader->getLocation("color"), 1, &color[0]);
    glUniformMatrix4fv(renderShader->getLocation("viewProjMat"), 1, GL_FALSE, &camera_matrix[0][0]);
    glUniformMatrix4fv(renderShader->getLocation("transformMat"), 1, GL_FALSE, &world_transform[0][0]);
    glUniform1i(renderShader->getLocation("tex"), 0);
    
    sprite->bind();

    glDrawElements(GL_TRIANGLES, quadIndices.size(), GL_UNSIGNED_INT, 0);
    
    sprite->unbind();
    renderVao->unbind();
}