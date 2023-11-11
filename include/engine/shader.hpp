#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#define SHADER_NO_ERROR "SHADER_NO_ERROR"
#define VERTEX_ERROR    "VERTEX_ERROR"
#define FRAGMENT_ERROR  "FRAGMENT_ERROR"
#define LINK_ERROR      "LINK_ERROR"

#include <glad/glad.h>
#include <engine/resources.hpp>
#include <iostream>

namespace Engine
{
    // shader class for render actor
    class Shader final
    {
    protected:
        Shader(std::string vertex_relative_path, std::string fragment_relative_path);
        void use();
        GLint getLocation(const char* uniformName);
        std::string getStatus();
        void destroy();
    private:
        GLuint program;
        std::string status;
        bool attach(const char* source_code, GLenum shader_type);
        bool checkShaderStatus(GLuint shader);
        bool checkLinkStatus();
    friend class Renderer;
    friend class SpriteRenderer;
    friend class Billboard;
    friend class Text;
    };
}

#endif