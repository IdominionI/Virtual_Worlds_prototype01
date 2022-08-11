#pragma once

#include <Source/Graphics_Engine/Common/definitions.h>

//#include "../Scene/scene_manager.h"
//#include "../Main_Window/Panels/log_panel.h"

#include "../Kernal/id_key_manager.h"

// Singleton class to store global variables and functions
// to be used through out the application

//class scene_manager_class;
//class log_panel_class;
//class node_basis_class;

class globalc {
public:
	globalc(const globalc&) = delete;

	static globalc &get(){
		static globalc instance;
		return instance;
	}

	static id_type get_current_selected_entity_id() { return get().current_selected_entity_id; }
	static void    set_current_selected_entity_id(id_type n) { get().current_selected_entity_id = n; }

	static id_type get_current_selected_entity_type_id() { return get().current_selected_entity_type_id; }
	static void    set_current_selected_entity_type_id(id_type n) { get().current_selected_entity_type_id = n; }

	//static scene_manager_class *get_scene_manager() { return get().scene_manager; }
	//static void set_scene_manager(scene_manager_class *sceen_manager_) { get().scene_manager = sceen_manager_; }

	//static log_panel_class *get_log_panel() { return get().log_panel; }
	//static void set_scene_manager(log_panel_class *log_panel_) { get().log_panel = log_panel_; }

	//static node_basis_class *get_selected_node() { return get().selected_node; }
	//static void set_selected_node(node_basis_class *selected_node_) { get().selected_node = selected_node_; }

	static idu_type get_available_entity_id() { return get().entity_id_key.get_available_id_key(); }
	static void     assign_free_entity_id(idu_type entity_id) { get().entity_id_key.assign_free_id_key(entity_id); };
	static void     reinitialise() { get().entity_id_key.reinitialise(); };

private:
	globalc() {};

	id_type current_selected_entity_id      = -1;
	id_type current_selected_entity_type_id = -1;

	// Not sure the following pointers is best here
	// to be reviewed and decided :: 
	// log panel to also be singleton class ?????
	//scene_manager_class *scene_manager = NULL;
	//log_panel_class     *log_panel     = NULL;
	//node_basis_class    *selected_node = NULL;

	id_key_manager_class<idu_type> entity_id_key;

};