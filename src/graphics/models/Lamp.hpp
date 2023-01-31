
#ifndef OPENGL1_LAMP_HPP
#define OPENGL1_LAMP_HPP

#include "Cube.hpp"

class Lamp : public Cube {
public:
    glm::vec3 lightColor;

    // Light Strength Values;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Lamp(glm::vec3 lightColor,
         glm::vec3 ambient,
         glm::vec3 diffuse,
         glm::vec3 specular,
         glm::vec3 pos,
         glm::vec3 size) :
            lightColor(lightColor),
            ambient(ambient),
            diffuse(diffuse),
            specular(specular),
            Cube(Material::white_plastic, pos, size) {}

    void render(Shader shader) {
        // Set Light Color
        shader.set3Float("lightColor", lightColor);

        Cube::render(shader);
    }
};

#endif //OPENGL1_LAMP_HPP
