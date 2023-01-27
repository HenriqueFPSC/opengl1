
///@formatter:off
#ifndef OPENGL1_SHADER_H
#define OPENGL1_SHADER_H

#include "include/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "include/glm/glm/glm.hpp"
#include "include/glm/glm/gtc/type_ptr.hpp"

class Shader {
public:
    unsigned id;

    Shader(const char *vertexShaderPath, const char *framentShaderPath);
    void activate();

    // Utility Functions
    std::string loadShaderSrc(const char *filepath);
    GLuint compileShader(const char *filepath, GLenum type);

    // Uniform Functions
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;
    void setFloat(const std::string &name, float value) const;
    void set4Float(const std::string &name, float v1, float v2, float v3, float v4) const;
};


#endif //OPENGL1_SHADER_H
