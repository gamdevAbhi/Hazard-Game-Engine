#ifndef ENGINE_BUTTON_MANAGER_HPP
#define ENGINE_BUTTON_MANAGER_HPP

#include <engine/input.hpp>
#include <engine/button.hpp>
#include <algorithm>

namespace Engine
{
    class ButtonManager
    {
    protected:
        static void initialize();
        static void checkInteraction();
        static bool isHover(Button* button);
        static bool compare(Node* left, Node* right);
    friend class GameLoop;
    };
}

#endif