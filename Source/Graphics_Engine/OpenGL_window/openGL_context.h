#pragma once

#include "openGL_window_basis.h"

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
