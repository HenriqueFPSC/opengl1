///@formatter:off
#ifndef OPENGL1_SHADER_H
#define OPENGL1_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned id;

    Shader();
    Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
    void generate(const char *vertexShaderPath, const char *fragmentShaderPath);
    void activate();

    // Utility Functions
    std::string loadShaderSrc(const char *filepath);
    GLuint compileShader(const char *filepath, GLenum type);

    // Uniform Functions
    void setMat4(const std::string &name, glm::mat4 value);
    void setInt(const std::string &name, int value);
    void setBool(const std::string &name, bool value);
    void setFloat(const std::string &name, float value);
    void set3Float(const std::string &name, glm::vec3 v);
    void set3Float(const std::string &name, float v1, float v2, float v3);
    void set4Float(const std::string &name, float v1, float v2, float v3, float v4);
};


#endif //OPENGL1_SHADER_H
