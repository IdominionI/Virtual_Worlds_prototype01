#include "../Common/pch.h"
#include "openGL_context.h"

#include "openGL_window_basis.h"

#include "GLFW_Window/callback_functions.h"
/*
                        OpenGL Context

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is code that sets up and initialises and performs the
    various basic OpenGl function calls required for OpenGl to
    function on a basic level.
*/
// Callback function for printing debug statements
//void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
void APIENTRY gl_debug_output(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* msg, const void* data)
{
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    printf("%d: %s of %s severity, raised from %s: %s\n",
        id, _type, _severity, _source, msg);
    //printf("Line : %s \n", sp);

}



bool openGL_context_class::init(openGl_window_basis_class* window_base)
{
    //__super::init(window);
    window = window_base;

    /* Initialize the glfw library */
    if (!glfwInit())
    {
        fprintf(stderr, "Error: GLFW Window couldn't be initialized\n");
        return false;
    }

    // Create the window and store this window as window pointer
    // so that we can use it in callback functions
    auto glWindow = glfwCreateWindow(window->Width, window->Height, window->Title.c_str(), nullptr, nullptr);
    window->set_native_window(glWindow);

    if (!glWindow)
    {
        fprintf(stderr, "Error: GLFW Window couldn't be created\n");
        return false;
    }

    glfwSetWindowUserPointer(glWindow, window);


    glfwSetInputMode(glWindow, GLFW_STICKY_KEYS, 1);

    // ************** User Interaction realted call backs ***********
    glfwSetKeyCallback(glWindow, on_key_callback<openGl_window_basis_class*>);
    glfwSetMouseButtonCallback(glWindow, on_mouse_button_callback<openGl_window_basis_class*>);
    glfwSetScrollCallback(glWindow, on_scroll_callback<openGl_window_basis_class*>);

    // Extra UI related call backs
    glfwSetCharCallback(glWindow, on_char_callback<openGl_window_basis_class*>);
    glfwSetCursorPosCallback(glWindow, on_cursor_pos_callback<openGl_window_basis_class*>);
    glfwSetCursorEnterCallback(glWindow, on_cursor_enter_callback<openGl_window_basis_class*>);
    glfwSetDropCallback(glWindow, on_drop_event_callback<openGl_window_basis_class*>);

    // ########### Window related call backs #############

    glfwSetWindowSizeCallback(glWindow, on_window_size_callback<openGl_window_basis_class*>);
    glfwSetWindowCloseCallback(glWindow, on_window_close_callback<openGl_window_basis_class*>);

    // Extra window related call backs 
    glfwSetWindowPosCallback(glWindow, on_window_position_callback<openGl_window_basis_class*>);
    glfwSetWindowRefreshCallback(glWindow, on_window_refresh_callback<openGl_window_basis_class*>);
    glfwSetWindowFocusCallback(glWindow, on_window_focus_callback<openGl_window_basis_class*>);
    glfwSetWindowIconifyCallback(glWindow, on_window_iconify_callback<openGl_window_basis_class*>);
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    glfwSetWindowMaximizeCallback(glWindow, on_window_maximize_callback<openGl_window_basis_class*>);
#endif
    glfwSetFramebufferSizeCallback(glWindow, on_window_frame_buffer_size_callback<openGl_window_basis_class*>);
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    glfwSetWindowContentScaleCallback(glWindow, on_window_content_scale_callback<openGl_window_basis_class*>);
#endif

    glfwMakeContextCurrent(glWindow);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    int debug_context_flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &debug_context_flags);
    if (debug_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        printf("debug context flag set\n");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, NULL);
        //glDebugMessageCallback(gl_debug_output, std::string("AAA\n").c_str());
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else {
        printf("debug context flag NOT set\n");
    }


    return true;
}

void openGL_context_class::pre_render()
{
    glViewport(0, 0, window->Width, window->Height);
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void openGL_context_class::post_render()
{
    glfwPollEvents();
    glfwSwapBuffers((GLFWwindow*)window->get_native_window());
}

void openGL_context_class::end()
{
    glfwDestroyWindow((GLFWwindow*)window->get_native_window());
    glfwTerminate();
}
