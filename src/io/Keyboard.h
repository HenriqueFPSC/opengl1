
#ifndef OPENGL1_KEYBOARD_H
#define OPENGL1_KEYBOARD_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

///@formatter:off
class Keyboard {
    static bool keys[];
    static bool keysChanged[];

public:
    // Key state callback
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    // Accessors
    static bool key(int key);
    static bool keyChanged(int key);
    static bool keyWentUp(int key);
    static bool keyWentDown(int key);
};


#endif //OPENGL1_KEYBOARD_H
