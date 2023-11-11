#ifndef ENGINE_FONT_HPP
#define ENGINE_FONT_HPP

#include <engine/handler.hpp>
#include <engine/resources.hpp>
#include <engine/sprite.hpp>
#include <engine/recttransform.hpp>
#include <map>

namespace Engine
{
    struct Character
    {
        Sprite* sprite;
        glm::ivec2 size;
        glm::ivec2 bearing;
        long int advance;
    };

    class Font
    {
    public:
        Font(std::string relative_font_path);
        std::map<char, Character> getCharacter(int fontSize);
        void destroy();
    protected:
        std::string font_path;
        std::map<int, std::map<char, Character>> character_map;
        bool addCharacter(int fontSize);
    };
}

#endif