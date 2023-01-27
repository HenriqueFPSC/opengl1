
///@formatter:off
#ifndef OPENGL1_MOUSE_H
#define OPENGL1_MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse {
    static double x;
    static double y;

    static double lastX;
    static double lastY;

    static double dx;
    static double dy;

    static double scrollDx;
    static double scrollDy;

    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];

public:
    static void cursorPosCallback(GLFWwindow *window, double _x, double _y);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseWheelCallback(GLFWwindow *window, double dx, double dy);

    static double getMouseX();
    static double getMouseY();
    static double getDx();
    static double getDy();
    static double getScrollDx();
    static double getScrollDy();

    static bool button(int button);
    static bool buttonChanged(int button);
    static bool buttonWentUp(int button);
    static bool buttonWentDown(int button);
};


#endif //OPENGL1_MOUSE_H
