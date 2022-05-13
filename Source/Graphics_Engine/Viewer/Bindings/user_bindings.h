#pragma once

#include "user_default_bindings.h"
#include "user_interaction_event_binding.h"

/*
						User Bindings class

	Class that sets the default mouse and key bindings that the application
	uses to allow the user to interact with the scene camera and rendering
	of the viewer overlays

*/

class user_binding_class : public user_interaction_event_binding_class {
public:

	void set_default_key_press_bindings() {

		set_key_binding(ACTION_TYPE_EXIT_VIEWER,    key_event_type_enum::press,event_state_enum::enabled,KEY_EXIT_VIEWER,    NO_KEY_MODIFIER,ACTION_TYPE_EXIT_VIEWER);
		set_key_binding(ACTION_TYPE_DRAW_AXIS,      key_event_type_enum::press,event_state_enum::enabled,KEY_DRAW_AXIS,      NO_KEY_MODIFIER,ACTION_TYPE_DRAW_AXIS);
		set_key_binding(ACTION_TYPE_DRAW_GRID,      key_event_type_enum::press,event_state_enum::enabled,KEY_DRAW_GRID,      NO_KEY_MODIFIER,ACTION_TYPE_DRAW_GRID);
		set_key_binding(ACTION_TYPE_DRAW_XY_GRID,   key_event_type_enum::press,event_state_enum::enabled,KEY_DRAW_GRID,      SHIFT_KEY_MODIFIER,ACTION_TYPE_DRAW_GRID);
		set_key_binding(ACTION_TYPE_DRAW_XZ_GRID,   key_event_type_enum::press,event_state_enum::enabled,KEY_DRAW_GRID,      CONTROL_KEY_MODIFIER,ACTION_TYPE_DRAW_GRID);
		set_key_binding(ACTION_TYPE_DRAW_YZ_GRID,   key_event_type_enum::press,event_state_enum::enabled,KEY_DRAW_GRID,      ALT_KEY_MODIFIER,ACTION_TYPE_DRAW_GRID);

		set_key_binding(ACTION_TYPE_DISPLAY_FPS,    key_event_type_enum::press,event_state_enum::enabled,KEY_DISPLAY_FPS,    NO_KEY_MODIFIER,ACTION_TYPE_DISPLAY_FPS);
		set_key_binding(ACTION_TYPE_DISPLAY_CAMERA_INFO, key_event_type_enum::press,event_state_enum::enabled,KEY_DISPLAY_CAMERA_INFO, NO_KEY_MODIFIER,ACTION_TYPE_DISPLAY_CAMERA_INFO);
		set_key_binding(ACTION_TYPE_DISPLAY_CROSS_HAIRS, key_event_type_enum::press,event_state_enum::enabled,KEY_DISPLAY_CROSS_HAIRS, NO_KEY_MODIFIER,ACTION_TYPE_DISPLAY_CROSS_HAIRS);
		set_key_binding(ACTION_TYPE_ENABLE_TEXT,    key_event_type_enum::press,event_state_enum::enabled,KEY_ENABLE_TEXT,    SHIFT_KEY_MODIFIER,ACTION_TYPE_ENABLE_TEXT);
		set_key_binding(ACTION_TYPE_SAVE_SCREENSHOT,key_event_type_enum::press,event_state_enum::enabled,KEY_SAVE_SCREENSHOT,CONTROL_KEY_MODIFIER,ACTION_TYPE_SAVE_SCREENSHOT);

		set_key_binding(ACTION_TYPE_TOGGLE_FIRST_PERSON,    key_event_type_enum::press,event_state_enum::enabled,KEY_TOGGLE_FIRST_PERSON, NO_KEY_MODIFIER,ACTION_TYPE_TOGGLE_FIRST_PERSON);

		// default movement

		set_key_binding(ACTION_TYPE_MOVEMENT_FORWARD, key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_FORWARD,  NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_FORWARD);
		set_key_binding(ACTION_TYPE_MOVEMENT_BACKWARD,key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_BACKWARD, NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_BACKWARD);
		set_key_binding(ACTION_TYPE_MOVEMENT_LEFT,    key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_LEFT,     NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_LEFT);
		set_key_binding(ACTION_TYPE_MOVEMENT_RIGHT,   key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_RIGHT,	  NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_RIGHT);
		set_key_binding(ACTION_TYPE_MOVEMENT_UP,      key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_UP,       NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_UP);
		set_key_binding(ACTION_TYPE_MOVEMENT_DOWN,    key_event_type_enum::press,event_state_enum::enabled,KEY_MOVEMENT_DOWN,     NO_KEY_MODIFIER,ACTION_TYPE_MOVEMENT_DOWN);
		set_key_binding(ACTION_TYPE_ROLL_LEFT,        key_event_type_enum::press,event_state_enum::enabled,KEY_ROLL_LEFT,         NO_KEY_MODIFIER,ACTION_TYPE_ROLL_LEFT);
		set_key_binding(ACTION_TYPE_ROLL_RIGHT,       key_event_type_enum::press,event_state_enum::enabled,KEY_ROLL_RIGHT,        NO_KEY_MODIFIER,ACTION_TYPE_ROLL_RIGHT);

		// camera
		set_key_binding(ACTION_TYPE_CAMERA_ROLL,key_event_type_enum::press,event_state_enum::enabled,KEY_CAMERA_ROLL, NO_KEY_MODIFIER,ACTION_TYPE_CAMERA_ROLL);
		set_key_binding(ACTION_TYPE_CAMERA_ALIGN_XY_PLANE,key_event_type_enum::press,event_state_enum::enabled,KEY_CAMERA_ALIGN_PLANE, NO_KEY_MODIFIER,ACTION_TYPE_CAMERA_ALIGN_XY_PLANE);
		set_key_binding(ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE,key_event_type_enum::press,event_state_enum::enabled,KEY_CAMERA_ALIGN_PLANE, ALT_KEY_MODIFIER,ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE);
		set_key_binding(ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE,key_event_type_enum::press,event_state_enum::enabled,KEY_CAMERA_ALIGN_PLANE, CONTROL_KEY_MODIFIER,ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE);
	}

	void set_default_mouse_bindings() {
		set_mouse_wheel_binding(ACTION_TYPE_FRAME_REGION_ZOOM, event_state_enum::enabled, NO_KEY, NO_MODIFIER, NO_EVENT_SOURCE, ACTION_TYPE_FRAME_REGION_ZOOM);

		set_mouse_binding(ACTION_TYPE_ALIGN_CAMERA,      mouse_event_type_enum::double_click, event_state_enum::enabled, GLFW_MOUSE_BUTTON_LEFT,   NO_BUTTON, NO_KEY, NO_MODIFIER,      ACTION_TYPE_ALIGN_CAMERA);
		set_mouse_binding(ACTION_TYPE_CENTER_SCENE,      mouse_event_type_enum::double_click, event_state_enum::enabled, GLFW_MOUSE_BUTTON_RIGHT,  NO_BUTTON, NO_KEY, NO_MODIFIER,      ACTION_TYPE_CENTER_SCENE);
		set_mouse_binding(ACTION_TYPE_SHOW_ENTIRE_SCENE, mouse_event_type_enum::double_click, event_state_enum::enabled, GLFW_MOUSE_BUTTON_MIDDLE, NO_BUTTON, NO_KEY, NO_MODIFIER,      ACTION_TYPE_SHOW_ENTIRE_SCENE);
		set_mouse_binding(ACTION_TYPE_ZOOM_ON_PIXEL,     mouse_event_type_enum::double_click, event_state_enum::enabled, GLFW_MOUSE_BUTTON_LEFT,   NO_BUTTON, NO_KEY, ALT_KEY_MODIFIER, ACTION_TYPE_ZOOM_ON_PIXEL);
		set_mouse_binding(ACTION_TYPE_ZOOM_TO_FIT,       mouse_event_type_enum::double_click, event_state_enum::enabled, GLFW_MOUSE_BUTTON_RIGHT,  NO_BUTTON, NO_KEY, ALT_KEY_MODIFIER, ACTION_TYPE_ZOOM_TO_FIT);
	}

private:
};
