#ifndef ENGINE_BILLBOARD_HPP
#define ENGINE_BILLBOARD_HPP

#include <engine/ui.hpp>
#include <engine/renderer.hpp>
#include <engine/actor.hpp>
#include <engine/recttransform.hpp>
#include <engine/shader.hpp>
#include <engine/sprite.hpp>
#include <engine/quadtree.hpp>

namespace Engine
{
    class Billboard : public Renderer
    {
    public:
        void setColor(glm::vec4 color);
        void setSprite(Sprite* sprite);
        glm::vec4 getColor();
        Sprite* getSprite();
    protected:
        inline static QuadTree* root = nullptr;
        Sprite* sprite;
        Node* node;
        glm::vec4 color;
        void start();
        void onTransformChanged();
        virtual void onDestroy();
    private:
        static void initialize();
        void draw();
    friend class RendererManager;
    };
}

#endif