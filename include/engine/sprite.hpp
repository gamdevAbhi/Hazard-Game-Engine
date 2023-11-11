#ifndef ENGINE_SPRITE_HPP
#define ENGINE_SPRITE_HPP

#include <stb/stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <engine/handler.hpp>
#include <engine/resources.hpp>
#include <iostream>

namespace Engine
{
    // create sprite texture
    class Sprite
    {
    public:
        Sprite(std::string relative_file_path);
        Sprite(FT_Face& face);
        void destroy();
    private:
        GLuint id;
        void bind();
        void unbind();
    friend class SpriteRenderer;
    friend class Billboard;
    friend class Text;
    };
}

#endif