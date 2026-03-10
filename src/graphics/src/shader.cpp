#include "shader.hpp"

#include <iostream>
#include <string>
namespace
{
constexpr int DEFAULT_LOG_BUF_SIZE = 1024;
}

namespace graphics
{
Shader::Shader(const std::string &vertex_source, const std::string &fragment_source)
{
    // vertex shader
    const char *vertex_shader_code = vertex_source.c_str();
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    checkCompileErrors(vertex_shader, "VERTEX");

    // fragment shader
    const char *fragment_shader_code = fragment_source.c_str();
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    checkCompileErrors(fragment_shader, "FRAGMENT");

    // shader Program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex_shader);
    glAttachShader(id_, fragment_shader);
    glLinkProgram(id_);
    checkCompileErrors(id_, "PROGRAM");

    // delete the shadrers as they're linked into the program successfully and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use()
{
    glUseProgram(id_);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    std::string info_log;
    info_log.resize(DEFAULT_LOG_BUF_SIZE);

    int result;
    if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader, DEFAULT_LOG_BUF_SIZE, NULL, info_log.data());
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log.data());
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
} // namespace graphics