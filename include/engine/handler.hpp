#ifndef ENGINE_HANDLER_HPP
#define ENGINE_HANDLER_HPP

#include <engine/window.hpp>
#include <iostream>

namespace Engine
{
    class Handler
    {
    public:
        static void debug(std::string message, std::string senderName);
        static void error(std::string message, std::string senderName);
    private:
        inline static Window* gameWindow = nullptr;
    friend class GameLoop;
    };
}

#endif