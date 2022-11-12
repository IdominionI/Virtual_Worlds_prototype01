#pragma once

#include <GLFW/glfw3.h>

#include "glfw_helper.h"

namespace glfw
{
    using Image = GLFWimage;

    class Cursor : public detail::OwningPtr<GLFWcursor>
    {
    public:
        using detail::OwningPtr<GLFWcursor>::OwningPtr;

        Cursor() noexcept = default;
        Cursor(Cursor&&) noexcept = default;

        Cursor& operator=(Cursor&& other) noexcept
        {
            glfwDestroyCursor(static_cast<GLFWcursor*>(*this));
            static_cast<detail::OwningPtr<GLFWcursor>&>(*this) = std::move(other);
            return *this;
        }

        ~Cursor() noexcept
        {
            glfwDestroyCursor(static_cast<GLFWcursor*>(*this));
        }

    public:
        [[nodiscard]] static Cursor createCursor(const Image& image_, int xHot_, int yHot_)
        {
            return Cursor{ glfwCreateCursor(&image_, xHot_, yHot_) };
        }
        [[nodiscard]] static Cursor createStandardCursorArrow()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_ARROW_CURSOR) };
        }
        [[nodiscard]] static Cursor createStandardCursorIBeam()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_IBEAM_CURSOR) };
        }
        [[nodiscard]] static Cursor createStandardCursorCrosshair()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR) };
        }
        [[nodiscard]] static Cursor createStandardCursorHand()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_HAND_CURSOR) };
        }
        [[nodiscard]] static Cursor createStandardCursorHorizontalResize()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR) };
        }
        [[nodiscard]] static Cursor createStandardCursorVerticalResize()
        {
            return Cursor{ glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR) };
        }
    };

    enum class CursorMode
    {
        Normal = GLFW_CURSOR_NORMAL,
        Disabled = GLFW_CURSOR_DISABLED,
        Hidden = GLFW_CURSOR_HIDDEN
    };

} //namespace glfw