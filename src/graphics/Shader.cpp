
#include "Shader.h"

Shader::Shader() {}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char *vertexShaderPath, const char *fragmentShaderPath) {
    int success;
    char infoLog[512];

    GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error with program comp.:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void Shader::activate() {
    glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char *filepath) {
    std::ifstream file;
    std::stringstream buffer;
    std::string ret;

    file.open(filepath);
    if (file.is_open()) {
        buffer << file.rdbuf();
        ret = buffer.str();
    } else {
        std::cout << "Could not open " << filepath << std::endl;
    }
    file.close();
    return ret;
}

GLuint Shader::compileShader(const char *filepath, GLenum type) {
    int success;
    char infoLog[512];

    GLuint shaderId = glCreateShader(type);
    std::string shaderSrc = loadShaderSrc(filepath);
    const GLchar *shader = shaderSrc.c_str();
    glShaderSource(shaderId, 1, &shader, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "Error with graphics comp.:\n" << infoLog << std::endl;
    }

    return shaderId;
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set4Float(const std::string &name, float v1, float v2, float v3, float v4) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

