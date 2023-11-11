#include <engine/shader.hpp>

Engine::Shader::Shader(std::string vertex_relative_path, std::string fragment_relative_path)
{
    // creating program for the shader
    program = glCreateProgram();

    // attaching shader to the program and checking if any error occur
    status = attach
    (
        Engine::Resources::get_file_content(Engine::Resources::get_current_dir() + vertex_relative_path).data(),
        GL_VERTEX_SHADER
    )? SHADER_NO_ERROR : VERTEX_ERROR;

    if(status == SHADER_NO_ERROR) status = attach
    (
        Engine::Resources::get_file_content(Engine::Resources::get_current_dir() + fragment_relative_path).data(),
        GL_FRAGMENT_SHADER
    )? SHADER_NO_ERROR : FRAGMENT_ERROR;

    // linking program
    if(status == SHADER_NO_ERROR)
    {
        glLinkProgram(program);
        status = checkLinkStatus()? SHADER_NO_ERROR : LINK_ERROR;
    }
}

// use the shader program
void Engine::Shader::use()
{
    glUseProgram(program);
}

GLint Engine::Shader::getLocation(const char* uniformName)
{
    return glGetUniformLocation(program, uniformName);
}

// return the shader status
std::string Engine::Shader::getStatus()
{
    return status;
}

// delete the program
void Engine::Shader::destroy()
{
    glDeleteProgram(program);
}

// return the status of create and compile shader and attach to program
bool Engine::Shader::attach(const char* source_code, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source_code, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
    bool status = checkShaderStatus(shader);
    glDeleteShader(shader);
    return status;
}

// get if the shader compile successfully
bool Engine::Shader::checkShaderStatus(GLuint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    return status == GL_TRUE;
}

// get if the program linked successfully
bool Engine::Shader::checkLinkStatus()
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    return status == GL_TRUE;
}