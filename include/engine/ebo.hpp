#ifndef ENGINE_EBO_HPP
#define ENGINE_EBO_HPP

#include <glad/glad.h>
#include <vector>

namespace Engine
{
    class EBO
    {
    public:
        EBO(GLsizeiptr size, const void* data, GLenum usage);
        void bind();
        void unbind();
        void destroy();
    private:
        GLuint id;
    };
}

#endif