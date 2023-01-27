
#include "Mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;
double Mouse::lastX = 0;
double Mouse::lastY = 0;
double Mouse::dx = 0;
double Mouse::dy = 0;
double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {false};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {false};

void Mouse::cursorPosCallback(GLFWwindow *window, double _x, double _y) {
    x = _x;
    y = _y;
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = x - lastX;
    dy = lastY - y; // Inverted
    lastX = x;
    lastY = y;
}

void Mouse::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!buttons[button]) {
            buttons[button] = true;
        }
    } else {
        buttons[button] = false;
    }
    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow *window, double dx, double dy) {
    scrollDx = dx;
    scrollDy = dy;
}

double Mouse::getMouseX() {
    return x;
}

double Mouse::getMouseY() {
    return y;
}

double Mouse::getDx() {
    double _dx = dx;
    dx = 0;
    return _dx;
}

double Mouse::getDy() {
    double _dy = dy;
    dy = 0;
    return _dy;
}

double Mouse::getScrollDx() {
    double _dy = scrollDy;
    scrollDy = 0;
    return _dy;
}

double Mouse::getScrollDy() {
    double _dx = scrollDx;
    scrollDx = 0;
    return _dx;
}

bool Mouse::button(int button) {
    return buttons[button];
}

bool Mouse::buttonChanged(int button) {
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}

bool Mouse::buttonWentUp(int button) {
    return !buttons[button] && buttonChanged(button);
}

bool Mouse::buttonWentDown(int button) {
    return buttons[button] && buttonChanged(button);
}
