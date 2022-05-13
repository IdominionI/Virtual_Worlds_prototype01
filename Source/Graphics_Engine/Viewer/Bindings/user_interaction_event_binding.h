#pragma once

#include <vector>

#include "user_default_bindings.h"

/*
	Set of structures to define mouse and key bindings and
	user interaction with the application viewport camera
	and other viewport functionality.
*/

#define ACTION_TYPE_NONE   -1

#define NO_KEY_MODIFIER       NULL
#define SHIFT_KEY_MODIFIER    GLFW_MOD_SHIFT
#define CONTROL_KEY_MODIFIER  GLFW_MOD_CONTROL
#define ALT_KEY_MODIFIER      GLFW_MOD_ALT
#define KEYPAD_KEY_MODIFIER   GLFW_MOD_NUM_LOCK

enum class event_type_enum {key, mouse, wheel};
enum class event_state_enum {enabled, disabled};

enum class key_event_type_enum {press, release};
enum class mouse_event_type_enum {press, release, move, double_click};

enum class binding_type_enum {button_press,button_release,mouse_wheel,mouse_button_press,mouse_button_release,mouse_move,mouse_double_click};

// binding id is to allow actions of the same name to be able to be assigned to different actions for different event types or the same event type.
// Need to review to see if this is actually needed as application is developed

// Keyboard events
struct key_event_binding_struct_type{
	int					  id;
	event_state_enum      event_state;
	key_event_type_enum   event_type;
	int                   key;
	int					  modifiers;
	bool                  repeating_key;
	int					  action_type;
};

// mouse wheel events
struct mouse_wheel_event_binding_struct_type {
	int				  id;
	event_state_enum  event_state;
	int				  key;
	int				  modifiers;
	int				  source;
	int				  action_type;
};

// Mouse events
struct mouse_event_binding_struct_type {
	int				  id;
	event_state_enum  event_state;
	int				  button;
	int			      buttonsBefore; // only defined when doubleClick is true
	int				  key;
	int				  modifiers;
	bool              doubleClick;
	int				  action_type;
};

class user_interaction_event_binding_class {
public:
	std::vector <key_event_binding_struct_type> press_key_events;
	std::vector <key_event_binding_struct_type> release_key_events;

	std::vector <mouse_wheel_event_binding_struct_type> mouse_wheel_events;

	std::vector <mouse_event_binding_struct_type> mouse_button_press_events;
	std::vector <mouse_event_binding_struct_type> mouse_button_release_events;
	std::vector <mouse_event_binding_struct_type> mouse_movement_events;
	std::vector <mouse_event_binding_struct_type> mouse_double_click_events;

	void set_key_binding(int binding_id,key_event_type_enum event_type,event_state_enum event_state,
						 int key, int modifiers,
						 int action_type = ACTION_TYPE_NONE,
						 bool repeating_key = false) {
		
		key_event_binding_struct_type key_event_binding;

		key_event_binding.id            = binding_id;
		key_event_binding.event_state   = event_state;
		key_event_binding.event_type    = event_type;
		key_event_binding.key           = key;
		key_event_binding.modifiers     = modifiers;
		key_event_binding.repeating_key = repeating_key;
		key_event_binding.action_type   = action_type;
		
		if(event_type == key_event_type_enum::press)
			press_key_events.push_back(key_event_binding);
		else
			release_key_events.push_back(key_event_binding);
	}

	void set_mouse_wheel_binding(int binding_id,event_state_enum event_state,int key,int modifiers,int source,int action_type = ACTION_TYPE_NONE) {
		mouse_wheel_event_binding_struct_type mouse_wheel_event_binding;

		mouse_wheel_event_binding.id            = binding_id;
		mouse_wheel_event_binding.event_state   = event_state;
		mouse_wheel_event_binding.key           = key;
		mouse_wheel_event_binding.modifiers     = modifiers;
		mouse_wheel_event_binding.source        = source;
		mouse_wheel_event_binding.action_type   = action_type;

		mouse_wheel_events.push_back(mouse_wheel_event_binding);
	}

	void set_mouse_binding(int binding_id,mouse_event_type_enum event_type, event_state_enum event_state, int button, int buttonsBefore,
						   int key, int modifiers,  int action_type = ACTION_TYPE_NONE) {
		
		bool double_click = false;

		if(event_type == mouse_event_type_enum::double_click) double_click=true;

		mouse_event_binding_struct_type mouse_event_binding;

		mouse_event_binding.id            = binding_id;
		mouse_event_binding.event_state   = event_state;
		mouse_event_binding.button        = button;
		mouse_event_binding.buttonsBefore = buttonsBefore;
		mouse_event_binding.key           = key;
		mouse_event_binding.modifiers     = modifiers;
		mouse_event_binding.action_type   = action_type;

		switch (event_type) {
			case mouse_event_type_enum::press        : mouse_button_press_events.push_back(mouse_event_binding);   break;
			case mouse_event_type_enum::release      : mouse_button_release_events.push_back(mouse_event_binding); break;
			case mouse_event_type_enum::move         : mouse_movement_events.push_back(mouse_event_binding);       break;
			case mouse_event_type_enum::double_click : mouse_double_click_events.push_back(mouse_event_binding);   break;
		}
	}




	void clear_bindings(binding_type_enum binding_type) {
		switch (binding_type) {
			case binding_type_enum::button_press         : press_key_events.clear(); break;
			case binding_type_enum::button_release       : release_key_events.clear(); break;
			case binding_type_enum::mouse_button_press   : mouse_button_press_events.clear(); break;
			case binding_type_enum::mouse_button_release : mouse_button_release_events.clear(); break;
			case binding_type_enum::mouse_move           : mouse_movement_events.clear(); break;
			case binding_type_enum::mouse_double_click   : mouse_double_click_events.clear(); break;
			case binding_type_enum::mouse_wheel          : mouse_wheel_events.clear(); break;
		}
	}

	void clear_all_bindings() {
		press_key_events.clear();
		release_key_events.clear();
		mouse_button_press_events.clear();
		mouse_button_release_events.clear();
		mouse_movement_events.clear();
		mouse_double_click_events.clear();
		mouse_wheel_events.clear();
	}

	// Foolowing on the to do list
	void disable_binding(int binding_id) {

	}

	void enable_binding(int binding_id) {

	}

	void remove_binding(event_type_enum event_type, int binding_id) {

	}

private:
};
