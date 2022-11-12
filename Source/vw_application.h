#pragma once

#include <Source/Editor/Main_Window/main_window.h>

#include <Source/Graphics_Engine/Common/pch.h>

class vw_Application {

public:

	// Define application
	vw_Application(const std::string& app_name) {
		/* Initialize the glfw library */
		if (!glfwInit()) {
			fprintf(stderr, "Error: GLFW Window couldn't be initialized\n");
			exit(0);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // !!!!!!!!!! comment this line in a release build! !!!!!!!!

		main_window = std::make_unique<main_window_class>();
		main_window->init(1700, 1000, app_name);
	}

	static vw_Application &Instance() { return *sInstance; }

	// Application main loop
	void loop() {
		while (main_window->is_running()) {
			main_window->render();
		}
	}

private:
	static vw_Application *sInstance;

	std::unique_ptr<main_window_class> main_window;
};