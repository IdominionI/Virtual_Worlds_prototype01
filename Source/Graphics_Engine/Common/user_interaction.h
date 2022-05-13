#pragma once

#include "../Common/pch.h"

/*
    Some Code retained from the original code that the 
    Virtual Wordlds application was adapted from.
*/

  enum class mouse_button_enum
  {
    Left   = 0,
    Right  = 1,
    Middle = 2,
    None   = 9
  };

  class user_interaction_class
  {
  public:
    static mouse_button_enum GetPressedButton(GLFWwindow* window)
    {
        mouse_button_enum result = mouse_button_enum::None;

      if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
        return mouse_button_enum::Left;
      else if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
        return mouse_button_enum::Right;
      else if (glfwGetMouseButton(window, 2) == GLFW_PRESS)
        return mouse_button_enum::Middle;
      
      return mouse_button_enum::None;

    }
  };
