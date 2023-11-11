#include <engine/renderer.hpp>

// get the index
int Engine::Renderer::getOrder()
{
    return order;
}

// set draw order
void Engine::Renderer::setOrder(int index)
{
    order = index;
}

// initialize renderer
void Engine::Renderer::initRender()
{
    quadVertices = std::vector<vertex>
    {
        vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}
    };
    quadIndices = std::vector<GLuint>
    {
        0, 1, 3,
        0, 2, 3
    };

    renderVao = new VAO();
    renderVao->bind();
    renderVbo = new VBO(quadVertices.size() * sizeof(vertex), quadVertices.data(), GL_STATIC_DRAW);
    renderVbo->bind();
    renderEbo = new EBO(quadIndices.size() * sizeof(GLuint), quadIndices.data(), GL_STATIC_DRAW);
    renderEbo->bind();

    renderVao->link(*renderVbo, 0, 3, sizeof(vertex), (void*)0);
    renderVao->link(*renderVbo, 1, 2, sizeof(vertex), (void*)(3 * sizeof(float)));

    renderVbo->unbind();
    renderVao->unbind();
    renderEbo->unbind();

    renderShader = new Shader("\\resources\\shaders\\render_shader.vert", 
    "\\resources\\shaders\\render_shader.frag");
 
    if(renderShader->getStatus() != SHADER_NO_ERROR) Handler::error("can't make the default shader", 
    "renderer");

    defaultSprite = new Sprite("\\resources\\sprites\\default_sprite.png");
}

// compare two sprite renderer
bool Engine::Renderer::compare(Engine::Renderer *left, Engine::Renderer *right)
{
    return left->order > right->order;
}