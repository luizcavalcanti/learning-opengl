#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader {
 public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    // TODO: outros sets de uniforms
    // TODO: destrutor
 private:
    unsigned int compileShader(const char* shaderCode, unsigned int shaderType);
};

#endif
