///@formatter:off
#ifndef OPENGL1_TEXTURE_H
#define OPENGL1_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
    static int currentId;

    const char *path;
    int width;
    int height;
    int nChannels;
public:
    int id;
    unsigned tex;
    const char *name;

    Texture();
    Texture(const char *path, const char *name, bool defaultParams = true);

    void generate();
    void load(bool flip = true);

    void setFilters(GLenum all);
    void setFilters(GLenum mag, GLenum min);

    void setWrap(GLenum all);
    void setWrap(GLenum s, GLenum t);

    void bind();
};


#endif //OPENGL1_TEXTURE_H
