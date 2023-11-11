#ifndef ENGINE_RENDERER_MANAGER_HPP
#define ENGINE_RENDERER_MANAGER_HPP

#include <engine/spriterenderer.hpp>
#include <engine/billboard.hpp>
#include <engine/text.hpp>

namespace Engine
{
    class RendererManager
    {
    public:
        static int getLastRenderCount();
    protected:
        inline static int count = 0;
        static void initialize();
        static void drawFrame();
        static void findRenderers(std::vector<Renderer*>& renderers);
    friend class GameLoop;
    };
}

#endif