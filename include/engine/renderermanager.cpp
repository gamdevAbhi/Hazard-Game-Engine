#include <engine/renderermanager.hpp>

// get the last render count
int Engine::RendererManager::getLastRenderCount()
{
    return count;
}

// initialize all the renderers
void Engine::RendererManager::initialize()
{
    Renderer::initRender();
    SpriteRenderer::initialize();
    Billboard::initialize();
    Text::initialize();
}

// draw the current frame
void Engine::RendererManager::drawFrame()
{
    std::vector<Renderer*> renderers;
    
    findRenderers(renderers);
    
    for(int i = 0; i < renderers.size(); i++)
    {
        renderers[i]->draw();
    }

    count = renderers.size();
}

// get all the renderers which needed to drawn
void Engine::RendererManager::findRenderers(std::vector<Renderer*>& renderers)
{
    // finding the sprite renderers which needed be drawn
    std::vector<Renderer*> spriteRenderers;
    Engine::Camera* camera = Engine::Camera::getRenderCamera();
    glm::mat4 camera_matrix = camera->getOrtho();

    glm::vec2 position = camera->getActor()->getComponent<Transform>()->getWorldPosition();
    std::vector<Node*> nodes;

    SpriteRenderer::root->find(AABB(position.x, position.y, camera->getDiagonal()), nodes);

    for(int i = 0; i < nodes.size(); i++) 
    {
        if(nodes[i]->getObject()->getActor()->getActive() == false) continue;
        spriteRenderers.push_back(dynamic_cast<Renderer*>(nodes[i]->getObject()));
    }

    // finding all UI renderers
    std::vector<Renderer*> uIRenderers;
    glm::mat4 ortho = UI::getOrtho();
    glm::vec2 resolution = UI::getResolution();
    
    // finding the billboard renderers which needed be drawn
    nodes.clear();

    float area = std::sqrt(std::pow(resolution.x, 2) + std::pow(resolution.y, 2));

    Billboard::root->find(AABB(resolution.x / 2.f, resolution.y / 2.f, area / 2.f), nodes);

    for(int i = 0; i < nodes.size(); i++) 
    {
        if(nodes[i]->getObject()->getActor()->getActive() == false) continue;
        uIRenderers.push_back(dynamic_cast<Renderer*>(nodes[i]->getObject()));
    }

    // finding all the text renderers
    nodes.clear();

    Text::root->find(AABB(resolution.x / 2.f, resolution.y / 2.f, area / 2.f), nodes);

    for(int i = 0; i < nodes.size(); i++) 
    {
        if(nodes[i]->getObject()->getActor()->getActive() == false) continue;
        uIRenderers.push_back(dynamic_cast<Renderer*>(nodes[i]->getObject()));
    }

    // adding all the renderers
    renderers.reserve(spriteRenderers.size() + uIRenderers.size());

    if(spriteRenderers.size() > 0)
    {
        std::sort(spriteRenderers.begin(), spriteRenderers.end(), Renderer::compare);
        renderers.insert(renderers.end(), spriteRenderers.begin(), spriteRenderers.end());
    }
    if(uIRenderers.size() > 0) 
    {
        std::sort(uIRenderers.begin(), uIRenderers.end(), Renderer::compare);
        renderers.insert(renderers.end(), uIRenderers.begin(), uIRenderers.end());
    }
}