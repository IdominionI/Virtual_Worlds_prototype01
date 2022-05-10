#include "../Common/pch.h"
#include "openGL_context.h"

#include "openGL_window_basis.h"

  static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    auto pWindow = static_cast<openGl_window_basis_class*>(glfwGetWindowUserPointer(window));
    pWindow->on_key(key, scancode, action, mods);
  }

  static void on_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
  {
    auto pWindow = static_cast<openGl_window_basis_class*>(glfwGetWindowUserPointer(window));
    pWindow->on_scroll(yoffset);
  }

  static void on_window_size_callback(GLFWwindow* window, int width, int height)
  {
    auto pWindow = static_cast<openGl_window_basis_class*>(glfwGetWindowUserPointer(window));
    pWindow->on_resize(width, height);
  }

  static void on_window_close_callback(GLFWwindow* window)
  {
      openGl_window_basis_class* pWindow = static_cast<openGl_window_basis_class*>(glfwGetWindowUserPointer(window));
    pWindow->on_close();
  }

  bool openGL_context_class::init(openGl_window_basis_class*window_base)
  {
    //__super::init(window);
    window = window_base;

    /* Initialize the library */
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
    glfwSetKeyCallback(glWindow, on_key_callback);
    glfwSetInputMode(glWindow, GLFW_STICKY_KEYS, 1); // ****
    glfwSetScrollCallback(glWindow, on_scroll_callback);
    glfwSetWindowSizeCallback(glWindow, on_window_size_callback);
    glfwSetWindowCloseCallback(glWindow, on_window_close_callback);
    glfwMakeContextCurrent(glWindow);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      return false;
    }

    glEnable(GL_DEPTH_TEST);

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
    glfwSwapBuffers((GLFWwindow*) window->get_native_window());
  }

  void openGL_context_class::end()
  {
    glfwDestroyWindow((GLFWwindow*)window->get_native_window());
    glfwTerminate();
  }
