#pragma once

#include "openGL_window_basis.h"


/*
                        OpenGL Context Class

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is code that sets up and initialises and performs the
    various basic OpenGl function calls required for OpenGl to
    function on a basic level.
*/

class openGL_context_class
{

public:

    glm::vec4 background_color = { 0.2f,0.2f,0.2f,1.0f };

    openGL_context_class() : window(nullptr) {}

    bool init(openGl_window_basis_class *window_base);

    void pre_render();

    void post_render();

    void end();

protected:
    openGl_window_basis_class*window;
};
