
#ifndef OPENGL1_MODEL_H
#define OPENGL1_MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Mesh.h"

class Model {
public:
    std::vector<Mesh> meshes;

    Model();

    void init();

    void render(Shader shader);

    void cleanup();
};


#endif //OPENGL1_MODEL_H
