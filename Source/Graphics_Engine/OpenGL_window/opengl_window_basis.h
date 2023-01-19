#pragma once

#include <string>

#include "GLFW_Window/glfw_keys.h"
#include "GLFW_Window/glfw_mouse.h"
#include "GLFW_Window/glfw_cursor.h"

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

    int Width;
    int Height;
    std::string Title;

    GLFWwindow* GLFW_window_ptr; // glfw

    virtual void* get_native_window() = 0;

    virtual void set_native_window(void* window) = 0;

    // UI related event functions
    virtual void on_scroll_event(float delta) = 0;
    virtual void on_mouse_button_event(int button, int state, int mods) = 0;
    virtual void on_key_event(int key, int scancode, int action, int mods) = 0;
    virtual void on_char_event(unsigned int key) {}

    virtual void on_cursor_position_event(double x, double y) {}
    virtual void on_cursor_enter_event(int value) {}
    virtual void on_drop_event(int count, const char** paths) {}


    // Window related event functions
    virtual void on_resize_event(int width, int height) = 0;
    virtual void on_close_event() = 0;

    virtual void on_window_position_event(int x, int y) {}
    virtual void on_window_refresh_event(GLFWwindow* glfw_window) {}
    virtual void on_window_focus_event(GLFWwindow* glfw_window, int value) {}
    virtual void on_window_iconify_event(GLFWwindow* glfw_window, int value) {}
    virtual void on_window_frame_buffer_size_event(GLFWwindow* glfw_window, int width, int height) {}

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    virtual void on_window_maximize_event(GLFWwindow* glfw_window, int value) {}
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    virtual void on_window_content_scale_event(GLFWwindow* glfw_window, float x_scale, float y_scale) {}
#endif


    // ################ glfw Window functions #################
    [[nodiscard]] bool shouldClose() const
    {
        if (!GLFW_window_ptr) return false;
        return glfwWindowShouldClose(GLFW_window_ptr);
    }

    bool setShouldClose(bool value_)
    {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowShouldClose(GLFW_window_ptr, value_);
        return true;
    }


    bool set_title(const char* title) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowTitle(GLFW_window_ptr, title);
        return true;
    }

    bool set_Icon(const std::vector<GLFWimage>& iconCandidates_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowIcon(GLFW_window_ptr, int(iconCandidates_.size()), iconCandidates_.data());
        return true;
    }

    [[nodiscard]] std::tuple<int, int> getPos() const {
        int xPos = 0, yPos = 0;
        if (GLFW_window_ptr) glfwGetWindowPos(GLFW_window_ptr, &xPos, &yPos);
        return { xPos, yPos };
    }

    bool set_Pos(int xPos_, int yPos_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowPos(GLFW_window_ptr, xPos_, yPos_);
        return true;
    }

    [[nodiscard]] std::tuple<int, int> getSize() const {
        int width = 0, height = 0;
        if (GLFW_window_ptr) glfwGetWindowSize(GLFW_window_ptr, &width, &height);
        return { width, height };
    }

    int width() {
        int width = 0, height = 0;
        if (GLFW_window_ptr) glfwGetWindowSize(GLFW_window_ptr, &width, &height);
        return width;
    }

    int height() {
        int width = 0, height = 0;
        if (GLFW_window_ptr) glfwGetWindowSize(GLFW_window_ptr, &width, &height);
        return height;
    }

    bool set_SizeLimits(int minWidth_, int minHeight_, int maxWidth_, int maxHeight_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowSizeLimits(GLFW_window_ptr, minWidth_, minHeight_, maxWidth_, maxHeight_);
        return true;
    }

    bool set_AspectRation(int numerator_, int denominator_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAspectRatio(GLFW_window_ptr, numerator_, denominator_);
        return true;
    }

    bool set_Size(int width_, int height_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowSize(GLFW_window_ptr, width_, height_);
        return true;
    }

    [[nodiscard]] std::tuple<int, int> getFramebufferSize() const {
        int width, height;
        glfwGetFramebufferSize(GLFW_window_ptr, &width, &height);
        return { width, height };
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] std::tuple<float, float> getContentScale() const {
        float xScale, yScale;
        glfwGetWindowContentScale(GLFW_window_ptr, &xScale, &yScale);
        return { xScale, yScale };
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] float getOpacity() const {
        if (!GLFW_window_ptr) return false;
        return glfwGetWindowOpacity(GLFW_window_ptr);
        return true;
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_Opacity(float opacity_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowOpacity(GLFW_window_ptr, opacity_);
        return true;
    }
#endif

    bool iconify() {
        if (!GLFW_window_ptr) return false;
        glfwIconifyWindow(GLFW_window_ptr);
        return true;
    }

    bool restore() {
        if (!GLFW_window_ptr) return false;
        glfwRestoreWindow(GLFW_window_ptr);
        return true;
    }

    bool maximize() {
        if (!GLFW_window_ptr) return false;
        glfwMaximizeWindow(GLFW_window_ptr);
        return true;
    }

    bool show() {
        if (!GLFW_window_ptr) return false;
        glfwShowWindow(GLFW_window_ptr);
        return true;
    }

    bool hide() {
        if (!GLFW_window_ptr) return false;
        glfwHideWindow(GLFW_window_ptr);
        return true;
    }

    bool focus() {
        if (!GLFW_window_ptr) return false;
        glfwFocusWindow(GLFW_window_ptr);
        return true;
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool requestAttention() {
        if (!GLFW_window_ptr) return false;
        glfwRequestWindowAttention(GLFW_window_ptr);
        return true;
    }
#endif

    [[nodiscard]] GLFWmonitor* getMonitor() const {
        return  glfwGetWindowMonitor(GLFW_window_ptr);
    }

    bool set_Monitor(GLFWmonitor* monitor_, int xPos_, int yPos_, int width_, int height_, int refreshRate_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowMonitor(GLFW_window_ptr, static_cast<GLFWmonitor*>(monitor_), xPos_, yPos_, width_, height_, refreshRate_);
    }

    [[nodiscard]] int getAttribFocused() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_FOCUSED);
    }

    [[nodiscard]] int getAttribIconified() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_ICONIFIED);
    }

    [[nodiscard]] int getAttribMaximized() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_MAXIMIZED);
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] int getAttribHovered() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_HOVERED);
    }
#endif

    [[nodiscard]] int getAttribVisible() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_VISIBLE);
    }

    [[nodiscard]] int getAttribResizable() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_RESIZABLE);
    }

    [[nodiscard]] int getAttribDecorated() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_DECORATED);
    }

    [[nodiscard]] int getAttribAutoIconify() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_AUTO_ICONIFY);
    }

    [[nodiscard]] int getAttribFloating() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_FLOATING);
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] int getAttribTransparentFramebuffer() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_TRANSPARENT_FRAMEBUFFER);
    }
#endif
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] int getAttribFocusOnShow() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_FOCUS_ON_SHOW);
    }
#endif

    [[nodiscard]] int  getAttribClientApi() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CLIENT_API);
    }

    [[nodiscard]] int getAttribContextCreationApi() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_CREATION_API);
    }

    [[nodiscard]] glm::ivec3 getAttribContextVersion() const {
        return {
                glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_VERSION_MAJOR),
                glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_VERSION_MINOR),
                glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_REVISION) };
    }

    [[nodiscard]] int getAttribOpenGlForwardCompat() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_OPENGL_FORWARD_COMPAT);
    }

    [[nodiscard]] int getAttribOpenGlDebugContext() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_OPENGL_DEBUG_CONTEXT);
    }

    [[nodiscard]] int getAttribOpenGlProfile() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_OPENGL_PROFILE);
    }

    [[nodiscard]] int getAttribContextReleaseBehavior() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_RELEASE_BEHAVIOR);
    }

    [[nodiscard]] int getAttribContextNoError() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_NO_ERROR);
    }

    [[nodiscard]] int getAttribContextRobustness() const {
        return glfwGetWindowAttrib(GLFW_window_ptr, GLFW_CONTEXT_ROBUSTNESS);
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_AttribDecorated(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAttrib(GLFW_window_ptr, GLFW_DECORATED, value_);
        return true;
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_AttribResizable(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAttrib(GLFW_window_ptr, GLFW_RESIZABLE, value_);
        return true;
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_AttribFloating(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAttrib(GLFW_window_ptr, GLFW_FLOATING, value_);
        return true;
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_AttribAutoIconify(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAttrib(GLFW_window_ptr, GLFW_AUTO_ICONIFY, value_);
        return true;
    }
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_AttribFocusOnShow(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetWindowAttrib(GLFW_window_ptr, GLFW_FOCUS_ON_SHOW, value_);
        return true;
    }
#endif

    template <class T>
    friend bool makeContextCurrent(const T& window_) {
        if (!window_.GLFW_window_ptr) return false;
        glfwMakeContextCurrent(window_.GLFW_window_ptr);
        return true;
    }

    template <class T>
    [[nodiscard]] friend T& getCurrentContext() {
        //return get_wrapper_pointer_to_glfw_window(glfwGetCurrentContext());
        return *static_cast<T*>(glfwGetWindowUserPointer(GLFW_window_ptr));
    }

    [[nodiscard]] int getInputModeCursor() const {
        return glfwGetInputMode(GLFW_window_ptr, GLFW_CURSOR);
    }

    [[nodiscard]] bool getInputModeStickyKeys() const {
        return glfwGetInputMode(GLFW_window_ptr, GLFW_STICKY_KEYS);
    }

    [[nodiscard]] bool getInputModeStickyMouseButtons() const {
        return glfwGetInputMode(GLFW_window_ptr, GLFW_STICKY_MOUSE_BUTTONS);
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] bool getInputModeLockKeyMods() const {
        return glfwGetInputMode(GLFW_window_ptr, GLFW_LOCK_KEY_MODS);
    }
#endif
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    [[nodiscard]] bool getInputModeRawMouseMotion() const {
        return glfwGetInputMode(GLFW_window_ptr, GLFW_RAW_MOUSE_MOTION);
    }
#endif

    bool set_InputModeCursor(int mode_) {
        if (!GLFW_window_ptr) return false;
        glfwSetInputMode(GLFW_window_ptr, GLFW_CURSOR, mode_);
        return true;
    }

    bool set_InputModeStickyKeys(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetInputMode(GLFW_window_ptr, GLFW_STICKY_KEYS, value_);
        return true;
    }

    bool set_InputModeStickyMouseButtons(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetInputMode(GLFW_window_ptr, GLFW_STICKY_MOUSE_BUTTONS, value_);
        return true;
    }

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_InputModeLockKeyMods(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetInputMode(GLFW_window_ptr, GLFW_LOCK_KEY_MODS, value_);
        return true;
    }
#endif
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool set_InputModeRawMouseMotion(bool value_) {
        if (!GLFW_window_ptr) return false;
        glfwSetInputMode(GLFW_window_ptr, GLFW_RAW_MOUSE_MOTION, value_);
        return true;
    }
#endif

    [[nodiscard]] int getKey(glfw::KeyCode key_) const {
        if (!GLFW_window_ptr) return -1;
        return glfwGetKey(GLFW_window_ptr, key_) != GLFW_RELEASE;
    }

    [[nodiscard]] int getMouseButton(glfw::MouseButton button_) const {
        if (!GLFW_window_ptr) return -1;
        return glfwGetMouseButton(GLFW_window_ptr, static_cast<int>(button_)) != GLFW_RELEASE;
    }

    [[nodiscard]] std::tuple<double, double> getCursorPos() const {
        double xPos = 0, yPos = 0;
        if (GLFW_window_ptr) glfwGetCursorPos(GLFW_window_ptr, &xPos, &yPos);
        return { xPos, yPos };
    }

    bool set_CursorPos(double xPos_, double yPos_) {
        if (!GLFW_window_ptr) return false;
        glfwSetCursorPos(GLFW_window_ptr, xPos_, yPos_);
        return true;
    }

    bool set_Cursor(GLFWcursor* cursor_) {
        //bool set_Cursor(const glfw::Cursor& cursor_) {
        if (!GLFW_window_ptr) return false;
        //glfwSetCursor(GLFW_window_ptr, static_cast<GLFWcursor*>(cursor_));
        glfwSetCursor(GLFW_window_ptr, cursor_);
        return true;
    }

    // +++++++++++++++++++
/*
#if defined(VK_VERSION_1_0)
    [[nodiscard]] VkResult createSurface(
        VkInstance instance,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface)
    {
        return glfwCreateWindowSurface(instance, GLFW_window_ptr, allocator, surface);
    }
#endif  // VK_VERSION_1_0
#ifdef VULKAN_HPP
    [[nodiscard]] vk::SurfaceKHR createSurface(
        const vk::Instance& instance,
        const std::optional<vk::AllocationCallbacks>& allocator = std::nullopt)
    {
        VkSurfaceKHR surface;
        VkResult result;
        if (allocator)
        {
            VkAllocationCallbacks allocator_tmp = *allocator;
            result = createSurface(static_cast<VkInstance>(instance), &allocator_tmp, &surface);
        }
        else
        {
            result = createSurface(static_cast<VkInstance>(instance), nullptr, &surface);
        }

        if (result < 0)
        {
            throw Error("Could not create window surface");
        }
        return static_cast<vk::SurfaceKHR>(surface);
    }
#endif  // VULKAN_HPP
*/

protected:
    // GLFWwindow *GLFW_window_ptr; // glfw

    bool mIsRunning;

};
