#pragma once

#include <string>


/*
                        OpenGL Window Class

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is a basis class that defines the required bais functions
    that an OpenGl window would need so as to function .
*/

class openGl_window_basis_class
{
public:

    virtual void* get_native_window() = 0;

    virtual void set_native_window(void* window) = 0;

    virtual void on_scroll(float delta) = 0;

    virtual void on_key(int key, int scancode, int action, int mods) = 0;

    virtual void on_resize(int width, int height) = 0;

    virtual void on_close() = 0;

    int Width;
    int Height;
    std::string Title;
};
