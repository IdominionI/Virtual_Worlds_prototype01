#pragma once

#include <Source/Editor/Main_Window/main_window.h>

#include <Source/Graphics_Engine/Common/pch.h>

class vw_Application {

public:

	// Define application
	vw_Application(const std::string& app_name) {
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