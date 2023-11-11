#include <engine/vbo.hpp>

Engine::VBO::VBO(GLsizeiptr size, const void* data, GLenum usage)
{
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    unbind();
}

void Engine::VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Engine::VBO::subData(GLsizeiptr size, const void* data)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    unbind();
}

void Engine::VBO::updateData(float* data, int offset, int size)
{
    bind();

    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for(int j = offset; j < size; j++) *(ptr + j) = *(data + j);
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    unbind();
}

void Engine::VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::VBO::destroy()
{
    glDeleteBuffers(1, &id);
}