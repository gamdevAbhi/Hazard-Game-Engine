#ifndef ENGINE_VBO_HPP
#define ENGINE_VBO_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

namespace Engine
{
    class VBO
    {
    public:
        VBO(GLsizeiptr size, const void* data, GLenum usage);
        void bind();
        void subData(GLsizeiptr size, const void* data);
        void updateData(float* data, int offset, int size);
        void unbind();
        void destroy();
    private:
        GLuint id;
    };
}

#endif