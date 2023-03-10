
#include "Mesh.h"

std::vector<Vertex> Vertex::genList(float *vertices, int numberOfVertices) {
    std::vector<Vertex> res(numberOfVertices);
    const int stride = sizeof(Vertex) / sizeof(float);

    for (int i = 0; i < numberOfVertices; i++) {
        res[i].pos = glm::vec3(
                vertices[i * stride + 0],
                vertices[i * stride + 1],
                vertices[i * stride + 2]
        );
        res[i].normal = glm::vec3(
                vertices[i * stride + 3],
                vertices[i * stride + 4],
                vertices[i * stride + 5]
        );
        res[i].texCoord = glm::vec2(
                vertices[i * stride + 6],
                vertices[i * stride + 7]
        );
    }
    return res;
}

// ---- Public Methods ---- //

Mesh::Mesh() = default;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
        vertices(vertices), indices(indices), textures(textures) {
    setup();
}

void Mesh::render(Shader shader) {
    // Textures
    for (unsigned i = 0; i < textures.size(); i++) {
        shader.setInt(textures[i].name, textures[i].id);
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].bind();
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// ---- Private Methods ---- //

void Mesh::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    // Set Vertex Attribute Pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr); // Position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}
