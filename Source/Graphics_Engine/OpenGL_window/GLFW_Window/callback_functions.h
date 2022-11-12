#pragma once

#include "../../Common/pch.h"

// User interaction realted callback function linkage
template <class T>
static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_key_event(key, scancode, action, mods);
}

template <class T>
static void on_char_callback(GLFWwindow* window, unsigned int key)
{
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_char_event(key);
}

template <class T>
static void on_mouse_button_callback(GLFWwindow* window, int button, int state, int mods)
{
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_mouse_button_event(button, state, mods);
}

template <class T>
static void on_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_scroll_event(yoffset);
}

template <class T>
static void on_cursor_pos_callback(GLFWwindow* window, double x, double y) {
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_cursor_position_event(x, y);
}

template <class T>
static void on_cursor_enter_callback(GLFWwindow* window, int value) {
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_cursor_enter_event(value);
}

template <class T>
static void on_drop_event_callback(GLFWwindow* window, int count, const char** paths) {
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_drop_event(count, paths);
}


// Window actions realted callback function linkage
template <class T>
static void on_window_size_callback(GLFWwindow* window, int width, int height)
{
    auto pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_resize_event(width, height);
}

template <class T>
static void on_window_close_callback(GLFWwindow* window)
{
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_close_event();
}

template <class T>
static void on_window_position_callback(GLFWwindow* window, int x, int y) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_position_event(x, y);
}

template <class T>
static void on_window_refresh_callback(GLFWwindow* window) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_refresh_event(window);
}

template <class T>
static void on_window_focus_callback(GLFWwindow* window, int value) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_focus_event(window, value);
}

template <class T>
static void on_window_iconify_callback(GLFWwindow* window, int value) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_iconify_event(window, value);
}

template <class T>
static void on_window_frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_frame_buffer_size_event(window, width, height);
}

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
template <class T>
static void on_window_maximize_callback(GLFWwindow* window, int value) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_maximize_event(window, value);
}
#endif

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
template <class T>
static void on_window_content_scale_callback(GLFWwindow* window, float x_scale, float y_scale) {
    T pWindow = static_cast<T>(glfwGetWindowUserPointer(window));
    pWindow->on_window_content_scale_event(window, x_scale, y_scale);
}
#endif