///@formatter:off
#ifndef OPENGL1_SCREEN_H
#define OPENGL1_SCREEN_H

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

class Screen {
    GLFWwindow *window;
public:
    static unsigned SCREEN_WIDTH;
    static unsigned SCREEN_HEIGHT;

    static void framebufferSizeCallBack(GLFWwindow*window, int width, int height);

    Screen();
    bool init();

    void setParameteres();

    // Main Loop
    void update();
    void newFrame();

    // Window closing accessor and modifier
    bool shouldClose();
    void setShouldClose(bool shouldClose);
};


#endif //OPENGL1_SCREEN_H
