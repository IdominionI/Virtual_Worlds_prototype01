#pragma once

#include <Source/Graphics_Engine/Common/definitions.h>

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

	// The get and set outliner selection global variables are so as to set the above gloabal entity and entity type
	// variables correctly between the outliner and node editor selection of scene entities. Without this the user
	// would need to deselect the node in the node editor before selecting an entity in the outliner widget, otherwise
	// no entity other than the selected node entity will be designated as the current selected entity. 
	static bool get_outliner_selection() { return get().outliner_selection; }
	static void set_outliner_selection(bool n) { get().outliner_selection = n; }


	static idu_type get_available_entity_id() { return get().entity_id_key.get_available_id_key(); }
	static void     assign_free_entity_id(idu_type entity_id) { get().entity_id_key.assign_free_id_key(entity_id); };
	static void     reinitialise() { get().entity_id_key.reinitialise(); };

private:
	globalc() {};

	id_type current_selected_entity_id      = -1;
	id_type current_selected_entity_type_id = -1;

	bool outliner_selection = false;

	// Not sure the following pointers is best here
	// to be reviewed and decided :: 
	// log panel to also be singleton class ?????
	//scene_manager_class *scene_manager = NULL;
	//log_panel_class     *log_panel     = NULL;
	//node_basis_class    *selected_node = NULL;

	id_key_manager_class<idu_type> entity_id_key;

};