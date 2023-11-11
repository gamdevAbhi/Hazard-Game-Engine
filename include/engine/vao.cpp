#include <engine/vao.hpp>

Engine::VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void Engine::VAO::bind()
{
    glBindVertexArray(id);
}

void Engine::VAO::unbind()
{
    glBindVertexArray(0);
}

void Engine::VAO::link(VBO& vbo, GLint index, GLuint size, GLsizeiptr stride, void* offset)
{
    bind();
    vbo.bind();
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
    vbo.unbind();
    unbind();
}

void Engine::VAO::destroy()
{
    glDeleteVertexArrays(1, &id);
}