
#ifndef OPENGL1_MESH_H
#define OPENGL1_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;

    static std::vector<Vertex> genList(float *vertices, int numberOfVertices);
};

class Mesh {
    unsigned VBO, EBO;

    void setup();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    unsigned VAO;

    std::vector<Texture> textures;

    Mesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures);

    void render(Shader shader);

    void cleanup();
};


#endif //OPENGL1_MESH_H
