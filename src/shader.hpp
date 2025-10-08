#pragma once

#include <glad/gl.h>
#include <string>

class Shader
{
public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);

private:
    unsigned int id_;
};