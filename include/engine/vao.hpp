#ifndef ENGINE_VAO_HPP
#define ENGINE_VAO_HPP

#include <glad/glad.h>
#include <engine/vbo.hpp>

namespace Engine
{
    class VAO
    {
    public:
        VAO();
        void bind();
        void unbind();
        void link(VBO& vbo, GLint index, GLuint size, GLsizeiptr stride, void* offset);
        void destroy();
    private:
        GLuint id;
    };
}

#endif