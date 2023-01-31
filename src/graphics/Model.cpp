
#include "Model.h"

Model::Model() = default;

void Model::init() {}

void Model::render(Shader shader) {
    for (Mesh mesh: meshes)
        mesh.render(shader);
}

void Model::cleanup() {
    for (Mesh mesh: meshes)
        mesh.cleanup();
}
