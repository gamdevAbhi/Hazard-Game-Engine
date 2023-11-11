#ifndef ENGINE_GAMELOOP_HPP
#define ENGINE_GAMELOOP_HPP

#include <engine/actor.hpp>
#include <engine/button.hpp>
#include <engine/buttonmanager.hpp>
#include <engine/billboard.hpp>
#include <engine/boxcollider.hpp>
#include <engine/camera.hpp>
#include <engine/circlecollider.hpp>
#include <engine/collider.hpp>
#include <engine/collidermanager.hpp>
#include <engine/component.hpp>
#include <engine/ebo.hpp>
#include <engine/handler.hpp>
#include <engine/input.hpp>
#include <engine/resources.hpp>
#include <engine/shader.hpp>
#include <engine/sprite.hpp>
#include <engine/spriterenderer.hpp>
#include <engine/time.hpp>
#include <engine/text.hpp>
#include <engine/transform.hpp>
#include <engine/renderer.hpp>
#include <engine/renderermanager.hpp>
#include <engine/recttransform.hpp>
#include <engine/vao.hpp>
#include <engine/vbo.hpp>
#include <engine/ui.hpp>
#include <engine/window.hpp>

namespace Engine
{
    class GameLoop final
    {
    public:
        static void initialize(std::string title);
        static void begin();
        static void endGameLoop();
        static glm::ivec2 getCurrentWindowSize();
    private:
        inline static std::string title = "";
        inline static int width = 0;
        inline static int height = 0;
        inline static Window* window = nullptr;
    };
}

#endif