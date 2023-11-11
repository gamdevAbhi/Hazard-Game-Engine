#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <engine/vao.hpp>
#include <engine/vbo.hpp>
#include <engine/ebo.hpp>
#include <engine/shader.hpp>
#include <engine/sprite.hpp>
#include <engine/actor.hpp>
#include <engine/handler.hpp>
#include <engine/component.hpp>
#include <algorithm>

namespace Engine
{
    // data structure for vertices
    struct vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

    class Renderer : public Component
    {
    public:
        int getOrder();
        void setOrder(int index);
    protected:
        inline static Shader* renderShader = nullptr;
        inline static Sprite* defaultSprite = nullptr;
        inline static std::vector<vertex> quadVertices;
        inline static std::vector<GLuint> quadIndices;
        inline static VAO* renderVao = nullptr;
        inline static VBO* renderVbo = nullptr;
        inline static EBO* renderEbo = nullptr;
        int order;
        virtual void draw() = 0;
        static void initRender();
        static bool compare(Renderer* left, Renderer* right);
    friend class RendererManager;
    };
}

#endif