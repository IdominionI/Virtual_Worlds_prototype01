#pragma once

#include "outliner_node.h"
#include "../../Kernal/id_key_manager.h"

#include "../Panels/log_panel.h"

#include "../../Common/imgui_custom.h"

#include "../../Scene/scene_manager.h"

#include <Source/Modules/Module_Voxel_Byte/Voxel_hcp_object/voxel_hcp_import_export.h>
#include <Source/Modules/Module_Hex_Surface/Hex_surface_object/hex_surface_import_export.h>


/*
							OUTLINER MANAGER

	Manager Class that performs all the functionality required to display
	and manage the application scene entities that are to have functions
	performed on them and displayed onto the computer screen.

	The ouliner manager displays stores a list of outliner nodes that
	is a representation of all of the entities that currently exists
	within the scene data base and scene display management systems.

	As each outliner node is created and deleted, so is the entity (or
	entities in the case of a group node) that the node represents within
	the scene data base and scene display management systems.

	The outliner also performs a selection finction to define which entity
	is to have its parameter data displayed in the application and for
	generation or modification of data or parameters to be performed.

	The outliner is designed so that all entities must have a parent group node.
	Thus no entity can exist within the current outliner without a parent group
	node. A parent group node needs to be created before any entity node is.
	This may change in the future as within the scene data base manager and scene
	diaplay manager, only entities are defined and no the outliner groupings.

	Importing and exporting of data is also managed here to save and load scene
	entity data. Most is performed by each of the entity import/export classes, 
	however an embedded import/export class object had to be included with the
	outliner class to avoid problems with circular referencing that no other 
	solution could befound.

	Without this embedded import/export class, the outliner nodes could not be
	generated upon importing data
*/

class outliner_manager_class {
public:

	outliner_manager_class() {
		root_node = new outliner_node_class;
		root_node->outliner_node_type = outliner_node_type_enum::root;

		group_id_key.invalid_key_value     = -1;
		group_id_key.max_key_value         = -2;
		group_id_key.next_available_id_key = -2;


/*
		outliner_node_class *group_node = add_new_group(root_node);
		if (group_node != NULL) {
			group_node->name = "group00";
			//group_node->entity_id = -101;
			// ---- test code here -----

			//outliner_node_class *new_node = add_new_node(group_node, outliner_node_type_enum::entity, ENTITY_CATEGORY_HCP_VOXEL);
			//if (new_node != NULL) {
			//	new_node->name = "tesst";
			//	//new_node->entity_id = 2020;
			//}

			//new_node = add_new_node(group_node, outliner_node_type_enum::entity, ENTITY_CATEGORY_HCP_VOXEL);
			//if (new_node != NULL) {
			//	new_node->name = "thteh";
			//	//new_node->entity_id = 2040;
			//}

		}
*/
		// -------------------------
	}

	~outliner_manager_class() {}


	scene_manager_class *scene_manager;

	outliner_node_class *root_node = NULL;

	id_type current_selected_node_id        = -1;
	id_type current_selected_entity_type_id = -1;
	id_type current_selected_entity_id      = -1;

	//	Display outliner nodes :
	//	To display the outliner nodes, the list of outiner tree nodes is queried and
	//  examined. The Outliner has a current design that all entities must be defined
	//  within a parent group node. Thus each child tree node of the ImGui root node is 
	//  a group node to make coding the outliner easier, and less coding to deal with.
	//  This means the code below to display the outliner tree nodes first looks at the
	//  ImGui tree root child nodes, displays that group node, then examines the children
	//  of that group node and displays them according to what entity data type that child
	//  is, and the ID of that entity. With the data type and entity ID, a call to the
	//  application scene manager is called to retireve the name and description data of
	//  that entity is made, which can be edited and changed by the ImGui widget displaying
	//  those parameters.
	//
	//  For a selection made on any ImGui tree node, an entity data type ID and entity ID value
	//  is assigned for the application to display the entity data within a serperate parameter
	//  panel and/or to create a menu selection to be performed within the outliner widget.

	void display_nodes(log_panel_class *_log_panel = NULL) {
		log_panel = _log_panel;

		static int selection_mask = (1 << 2);
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; //| ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::SetCursorPosX(115.0f); ImGui::Text("Name    vis sel   Description");

		outliner_node_class *group  = NULL;
		outliner_node_class *entity = NULL;

		bool root_node_open = ImGui::TreeNodeEx((void*)(intptr_t)0, base_flags, "root", 0);
		if (root_node_open) { // Outliner ImGui root node
			for (int i = 0; i < root_node->children.size(); i++) { // Get Outliner group node
				group  = root_node->children[i];
				entity = NULL;

				ImGuiTreeNodeFlags node_flags = base_flags; // Note : Critical This needs to be reinitalised for each node or app will crash

				const bool is_selected = (selection_mask & (1 << i)) != 0;
				if (is_selected) 
					node_flags |= ImGuiTreeNodeFlags_Selected;

				bool group_node_open = ImGui::TreeNodeEx((void*)(intptr_t)group, node_flags, "");

				if (ImGui::IsItemClicked() || ImGui::IsItemHovered()){
					current_selected_node_id = group->entity_id;
					if (ImGui::IsItemClicked()) {
						current_selected_entity_id      = -1;
						current_selected_entity_type_id = -1;
					}
//printf("display_nodes :: group selected: %i  \n", current_selected_node_id); //this is the group
				}

				// Each ImGui Widget needs to have a unique id identifier which is strangely a string to function and
				// avoid conflicts of signals to and from other widgets or application bindings. If have widgets have
				// the same ID, the widgets will not function or crash the app. Funny enoough, this ID is displayed 
				// unless a ### preceeds the ID text as given by id_prefix. A process of adding a unique number to this
				// prefix ensures a unique widget id that is not duplicated.

				std::string id_prefix = "###g"; std::string w_id = "";
				ImGui::SameLine(); ImGui::Text(ICON_FA_LAYER_GROUP); // Define an icon to display to indicate this node is a group node
				ImGui::SameLine();
				ImGui::SetNextItemWidth(75.0f);
				w_id = id_prefix + std::to_string(i * 100);
				char *group_name; group_name = group->name.data(); // Define a variable that can be modified by ImGui Widget
				if (ImGui::InputText(w_id.c_str(), group_name, 20, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) {
					group->name = group_name;
				}
				ImGui::SameLine(); w_id = id_prefix + std::to_string(i * 100 + 1); ImGui::Checkbox(w_id.c_str(), &group->visible);
				ImGui::SameLine(); w_id = id_prefix + std::to_string(i * 100 + 2); ImGui::Checkbox(w_id.c_str(), &group->selected);

				w_id = id_prefix + std::to_string(i * 100 + 3);
				char *group_description; group_description = group->description.data();// Define a variable that can be modified by ImGui Widget
				ImGui::SameLine(); ImGui::SetNextItemWidth(100.0f);
				if (ImGui::InputText(w_id.c_str(), group_description, 30, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) {
					group->description = group_description;
				}

				// Create a menu to perform actions if a group node is selected and right mouse pressed
				if (current_selected_node_id < -1) {
					if (ImGui::BeginPopupContextWindow("1", 1, true)) {
						if (ImGui::BeginMenu("Add Object")) {
							{
								if(ImGui::MenuItem("HCP Voxel"))    add_object(current_selected_node_id,ENTITY_CATEGORY_HCP_VOXEL);
								if(ImGui::MenuItem("Hex Surface"))  add_object(current_selected_node_id,ENTITY_CATEGORY_HCP_SURF);
								ImGui::EndMenu();
							} };

						if (ImGui::BeginMenu("Import Object")) {
							{
								if (ImGui::MenuItem("HCP Voxel"))    import_object(current_selected_node_id,ENTITY_CATEGORY_HCP_VOXEL);
								if (ImGui::MenuItem("Hex Surface"))  import_object(current_selected_node_id,ENTITY_CATEGORY_HCP_SURF);
								ImGui::EndMenu();
							}
						};

						ImGui::Separator();
						if (ImGui::MenuItem("Delete Group")) delete_group(current_selected_node_id);
						ImGui::Separator();
						if (ImGui::MenuItem("Save Group"))   save_group(current_selected_node_id);

						ImGui::EndPopup();// Root
					}
				} else {// Create a menu to perform actions if an entity node is selected and right mouse pressed
					if (current_selected_node_id > -1) {
						if (ImGui::BeginPopupContextWindow("2", 1, true)) {
							if (ImGui::MenuItem("Delete Object")) delete_object(current_selected_node_id);
							ImGui::Separator();
							if (ImGui::MenuItem("Save Object"))   save_object(current_selected_node_id);

							ImGui::EndPopup();// Root
						}
					}
				}

				if (group_node_open) {
					for(int j = 0; j< group->children.size();j++){ // Get Outliner entity node of current group node 
						entity = group->children[j];

						node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						bool entity_selected = ImGui::TreeNodeEx((void*)(intptr_t)entity, node_flags, "");
						std::string id_prefix = "###e"; std::string w_id = "";

						if (ImGui::IsItemClicked() || ImGui::IsItemHovered()){
							current_selected_node_id = entity->entity_id;
							if (ImGui::IsItemClicked()) {
								current_selected_entity_id      = entity->entity_id;
								current_selected_entity_type_id = entity->category_id;
							}
//printf("display_nodes :: entity selected: %i  \n", current_selected_node_id); //this is the group
						}

						// Define an icon to display to indicate what type of entity data type item this ImGui tree widget represents
						ImGui::SameLine();
						switch (entity->category_id) {
							//case  GROUP_CATEGORY			: ImGui::Text(ICON_FA_LAYER_GROUP); break; // No sub groups yet implemented
							case  ENTITY_CATEGORY_HCP_VOXEL : ImGui::Text(ICON_FA_SNOWFLAKE_O); break;
							case  ENTITY_CATEGORY_HCP_SURF  : ImGui::Text(ICON_FA_MOUNTAIN); break;
								// Other entity category types to be added here
							default : ImGui::Text("!"); break;
						}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(75.0f);
						w_id = id_prefix + std::to_string(i*100+j*10);char *entity_name; entity_name = entity->name.data();// Define a variable that can be modified by ImGui Widget
						if (ImGui::InputText(w_id.c_str(), entity_name, 20, ImGuiInputTextFlags_CallbackCharFilter , Input_Filters::name)) {
							entity->name = entity_name;
							change_entity_name(entity->entity_id, entity->category_id, entity->name);
						}

						ImGui::SameLine(); w_id = id_prefix + std::to_string(i * 100 + j * 10 + 1); 
						if (ImGui::Checkbox(w_id.c_str(), &entity->visible)) {
							set_entity_visibility(entity);
						}
						ImGui::SameLine(); w_id = id_prefix + std::to_string(i * 100 + j * 10 + 2 ); 
						if (ImGui::Checkbox(w_id.c_str(), &entity->selected)) {
							set_entity_activity(entity);
						}

						w_id = id_prefix + std::to_string(i * 100 + j * 10 + 3);
						ImGui::SameLine(); ImGui::SetNextItemWidth(100.0f); char *entity_description; entity_description = entity->description.data();// Define a variable that can be modified by ImGui Widget
						if(ImGui::InputText(w_id.c_str(), entity_description,30, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)){
							entity->description = entity_description;
							change_entity_description(entity->entity_id, entity->category_id, entity->description);
						}
					}
					ImGui::TreePop();// group child node : must be within a node open test or app will crash upon clicking on toggle open close icon
				}
			}
			ImGui::TreePop();// group node : must be within a node open test or app will crash upon clicking on toggle open close icon
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			current_selected_node_id        = -1;
			current_selected_entity_id      = -1;
			current_selected_entity_type_id = -1;
		}

		// Right-click on blank space
		// Create a menu to perform actions if nothing is selected and right mouse pressed
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Add Object Group"))				 add_group();
			if (ImGui::MenuItem("Add Imported Scene"))				 add_scene();
			ImGui::Separator();
			if (ImGui::MenuItem("Save Scene"))						 save_scene();
			if (ImGui::MenuItem("Clear Scene"))						 clear_scene();
			ImGui::Separator();
			if (ImGui::MenuItem("Import Object Group"))              import_group();
			ImGui::Separator();
			if (ImGui::MenuItem("Delete Selected Items"))            delete_selected_nodes();
			if (ImGui::MenuItem("Deactivate Selected Items"))        deactivate_selected_nodes();
			if (ImGui::MenuItem("Toggle Selected Items visibility")) toggle_selected_nodes_visibility();
			if (ImGui::MenuItem("Toggle Selected Items activity"))   toggle_selected_nodes_activity();

			ImGui::EndPopup();
		}

	}

	outliner_node_class *add_new_node(outliner_node_class* parent_node, outliner_node_type_enum outliner_node_type, category_id_type category_id) {
		switch (outliner_node_type) {
			case outliner_node_type_enum::group:  {return add_new_group(parent_node); break; }
			case outliner_node_type_enum::entity: {return add_new_entity(parent_node, category_id); break; }
		}

		return NULL;
	}

	//bool delete_node() {
	//	return true;
	//}

	//bool insert_node() {
	//	return true;
	//}

	void delete_selected_nodes() {
//printf("display_nodes :: Delete Selected Items MenuItem selected  \n");
		current_selected_node_id = -1;
	}

	void deactivate_selected_nodes() {
//printf("display_nodes :: Deactivate Selected Items MenuItem selected  \n");
		current_selected_node_id = -1;
	}

	void toggle_selected_nodes_visibility() {
//printf("display_nodes :: toggle Selected Items visibility MenuItem selected  \n");
		current_selected_node_id = -1;
	}

	void toggle_selected_nodes_activity() {
//printf("display_nodes :: toggle Selected Itemsa activity MenuItem selected  \n");
		current_selected_node_id = -1;
	}

	// --------------- Scene functions ---------------------
	void add_scene() {
//printf("display_nodes :: Add Iported Scene MenuItem selected  \n");
		current_selected_node_id = -1;

		vw_import_export_class vw_import_export;
		vw_import_export.log_panel = log_panel;

		vw_import_export.import_virtual_world_scene(this);
	}

	void save_scene() {
//printf("display_nodes :: Save Scene MenuItem selected  \n");
		current_selected_node_id = -1;

		vw_import_export_class vw_import_export;
		vw_import_export.log_panel = log_panel;

		vw_import_export.export_scene(root_node, scene_manager);
	}

	void clear_scene() {
//printf("outliner_manager_class :: clear_scene 00  \n");

		int number_groups = root_node->children.size();

		for (int i = number_groups - 1; i > -1; i--) {
			delete_group(root_node->children[i]);
		}

		entity_id_key.reinitialise();
		group_id_key.reinitialise();

		current_selected_node_id = -1;
	}
	// --------------- Group functions ---------------------

	void add_group() {
//printf("display_nodes :: Add Group MenuItem selected  \n");
		add_new_group(root_node);

		current_selected_node_id = -1;
	}

	outliner_node_class *get_outliner_group_node(id_type group_id) {
		outliner_node_class *group_node = NULL;

		for (int i = 0; i < root_node->children.size(); i++) {
			group_node = root_node->children[i];
//printf("get_outliner_group_node 000 %i %i \n", group_node->entity_id, group_id);

			if (group_node->entity_id == group_id)
				return group_node;
		}

		return NULL;
	}

	void import_group() {
//printf("display_nodes :: Import group MenuItem selected  \n");
		current_selected_node_id = -1;

		vw_import_export_class vw_import_export;
		vw_import_export.log_panel = log_panel;

		vw_import_export.import_object_group(root_node,this);
	}

	void delete_group(id_type group_id) {
//printf("outliner_manager_class :: delete_group 00 \n");
		current_selected_node_id = -1;

		outliner_node_class *group_to_delete = root_node->find_child(group_id);
//printf("outliner_manager_class :: delete_group 11 \n");
		if (group_to_delete == NULL) {
			//error message to log
			return;
		}
//printf("outliner_manager_class :: delete_group 22 \n");
		delete_group(group_to_delete);
	}

	void delete_group(outliner_node_class *group_to_delete) {
//printf("outliner_manager_class :: delete_group 33 \n");
		current_selected_node_id = -1;
		if (group_to_delete == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : No group defined to delete.\n");
			return;
		}

		int number_children = group_to_delete->children.size();

//printf("outliner_manager_class 44 :: Delete Group MenuItem selected#### %i %i \n", group_to_delete->entity_id,group_to_delete->children.size());

		for (int i = number_children - 1; i > -1; i--) {
			scene_manager->delete_entity(group_to_delete->children[i]->entity_id, group_to_delete->children[i]->category_id);

			entity_id_key.assign_free_id_key(group_to_delete->children[i]->entity_id);// Free entity ID number to be reused when a new entity is created
			delete group_to_delete->children[i]; // destructor should delete group childern entries
//printf("outliner_manager_class 55 :: delete_group children !!!! %i\n", group_to_delete->children.size());
		}

//printf("outliner_manager_class :: delete_group 66 \n");
		group_id_key.assign_free_id_key(group_to_delete->entity_id);// Free entity ID number to be reused when a new entity is created
		delete group_to_delete;
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Group deleted.\n");
	}

	void save_group(id_type group_id) {
//printf("display_nodes :: Save Group MenuItem selected  \n");
		current_selected_node_id = -1;

		vw_import_export_class vw_import_export;
		vw_import_export.log_panel = log_panel;

		outliner_node_class *group_node = get_outliner_group_node(group_id);

		if (group_node == NULL) {
			vwDialogs::message_box("Save group", "Could not save selected entity group object. No entry in outliner coud be found !!!", "ok", "error");
			return;
		}

		vw_import_export.export_scene_object_group(group_node, scene_manager);
	}

	// --------------- Object functions ---------------------

	void add_object(id_type parent_group_id, category_id_type entity_category) {
//printf("display_nodes :: Add Object MenuItem selected  \n");
		current_selected_node_id = -1;
		outliner_node_class *outliner_group_node = get_outliner_group_node(parent_group_id);

//printf("display_nodes :: Add Object MenuItem selected 11111 %i \n", outliner_group_node->entity_id);
		if (outliner_group_node == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : No group found to add entity to.\n");
			return;
		}

		outliner_node_class *new_entity = add_new_entity(outliner_group_node, entity_category);

		if (new_entity == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Failed add a new entity to selected group.\n");
			return;
		}
		
//printf("display_nodes :: Add Object MenuItem selected 222222 %i %i %i \n", new_entity->entity_id, new_entity->parent->entity_id, new_entity->parent->children.size());
		
	}

	outliner_node_class *get_outliner_object(id_type object_id){
		for (outliner_node_class *group_node : root_node->children) {
			for (outliner_node_class *entity_node : group_node->children) {
				if (entity_node->entity_id == object_id)
					return entity_node;
			}
		}

		return NULL;
	}

	void import_object(id_type parent_group_id, id_type entity_category) {
//printf("display_nodes :: Import Object MenuItem selected  \n");
		switch (entity_category) {
			case ENTITY_CATEGORY_HCP_VOXEL : import_hcp_voxel_object(parent_group_id); break;
			case ENTITY_CATEGORY_HCP_SURF  : import_hex_surface_object(parent_group_id); break;
			// Other category types below this line
		}
	}

	void save_object(id_type object_id) {
//printf("display_nodes :: save Object MenuItem selected  \n");
		outliner_node_class *entity_node = get_outliner_object(object_id);
		if (entity_node == NULL) {
			vwDialogs::message_box("Save entity", "Could not save selected entity object. No entry in outliner coud be found !!!", "ok", "error");
			return;
		}

		switch (entity_node->category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL :{
				// export voxel hcp entity data 
				// Get pointer to hcp voxel object in the scene to export from the scene manager
				voxel_hcp_object_class *voxel_hcp_object_to_export = scene_manager->get_voxel_hcp_entity_object(entity_node->entity_id);
				hcp_voxel_import_export.export_hcp_object(voxel_hcp_object_to_export, ENTITY_CATEGORY_HCP_VOXEL);}
				break;

			case ENTITY_CATEGORY_HCP_SURF: {
				hex_surface_object_class* hex_surface_object_to_export = scene_manager->get_hex_surface_entity_object(entity_node->entity_id);
				hex_surface_import_export.export_hex_surface_object(hex_surface_object_to_export, ENTITY_CATEGORY_HCP_SURF); }
				break;

			// Other category types below this line
		}

	}

	bool delete_object(id_type object_id) {
//printf("display_nodes :: delete Object MenuItem selected %i \n", object_id);
		outliner_node_class *group_node = NULL;
		current_selected_node_id = -1;

		for (int i = 0; i < root_node->children.size(); i++) {
			group_node = root_node->children[i];

			outliner_node_class *object_to_delete = group_node->find_child(object_id);

			if (object_to_delete != NULL) {
				entity_id_key.assign_free_id_key(object_to_delete->entity_id);// Free entity ID number to be reused when a new entity is created
				
				scene_manager->delete_entity(object_to_delete->entity_id, object_to_delete->category_id);
				
				delete object_to_delete;
				if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Entity deleted.\n");
//printf("display_nodes :: delete Object MenuItem selected 000 object_to_delete != NULL\n");
				return true;
			}
		}

		//printf("display_nodes :: delete Object MenuItem selected 1111 object_to_delete == NULL \n");
		return false;
		// Message object not deleted cannot find to logger
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	outliner_node_class *add_new_group(outliner_node_class* parent_node) {
		outliner_node_class *new_outliner_node = new outliner_node_class(parent_node);
		if (new_outliner_node != NULL) {
			new_outliner_node->entity_id           = group_id_key.get_available_id_key();

			new_outliner_node->name               = "group_" + std::to_string(-new_outliner_node->entity_id);
			new_outliner_node->outliner_node_type = outliner_node_type_enum::group;
			new_outliner_node->category_id        = GROUP_CATEGORY;
			new_outliner_node->description        = "group_" + std::to_string(-new_outliner_node->entity_id);

			new_outliner_node->parent = parent_node;
			parent_node->children.push_back(new_outliner_node);
			if(log_panel!= NULL) log_panel->application_log.AddLog("INFO : New group added.\n"); 
		}
		return new_outliner_node;
	}

	outliner_node_class *add_new_entity(outliner_node_class *parent_node, category_id_type category_id) {
		outliner_node_class *new_outliner_node = new outliner_node_class(parent_node);
		if (new_outliner_node != NULL) {
			new_outliner_node->entity_id           = entity_id_key.get_available_id_key();
			new_outliner_node->name               = "object_" + std::to_string(new_outliner_node->entity_id);
			new_outliner_node->outliner_node_type = outliner_node_type_enum::entity;
			new_outliner_node->category_id		  = category_id;
			new_outliner_node->description        = "object_" + std::to_string(new_outliner_node->entity_id);
			


//printf("display_nodes :: add_new_entity 0000 %i %i\n", parent_node->entity_id, new_outliner_node->entity_id);

			if(scene_manager != NULL){
//printf("display_nodes :: scene_manager != NULL 1111 %i %i\n", parent_node->entity_id, new_outliner_node->entity_id);
				if (!scene_manager->add_entity(new_outliner_node->entity_id, category_id)) {
//printf("display_nodes :: add_new_entity failed 2222 %i %i\n", new_outliner_node->entity_id, category_id);
					
					entity_id_key.assign_free_id_key(new_outliner_node->entity_id);
					delete new_outliner_node;
					return NULL;
				}

				change_entity_name(new_outliner_node->entity_id, category_id, new_outliner_node->name);
			}
//printf("display_nodes :: add_new_entity succeeded 3333 %i %i\n", new_outliner_node->entity_id, category_id);

			new_outliner_node->parent = parent_node; 
			parent_node->children.push_back(new_outliner_node);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : New entity added.\n");
		}
		return new_outliner_node;
	}


private:
	id_key_manager_class<idu_type> entity_id_key;
	neg_id_key_manager_class	   group_id_key;

	log_panel_class* log_panel;
	
	void set_entity_visibility(outliner_node_class *entity) {
		if (scene_manager != NULL) {
			scene_node_class <render_object_class>* render_object = scene_manager->get_render_object(entity->entity_id);
			if (render_object != NULL)
				render_object->scene_graph_object.scene_object_class.visible = entity->visible;
		}
	}

	void set_entity_activity(outliner_node_class *entity) {
		if (scene_manager != NULL) {

			switch (entity->category_id) {
				case ENTITY_CATEGORY_HCP_VOXEL :{
					voxel_hcp_object_class *voxel_hcp_object = scene_manager->get_voxel_hcp_entity_object(entity->entity_id);
					if(voxel_hcp_object != NULL) voxel_hcp_object->active_object = entity->selected;}
					break;

				case ENTITY_CATEGORY_HCP_SURF :{
printf("OUTLINER : set_entity_activity 000:: %i\n", entity->selected);
					hex_surface_object_class *hex_surface_object = scene_manager->get_hex_surface_entity_object(entity->entity_id);
					if(hex_surface_object != NULL) {hex_surface_object->active_object = entity->selected;
printf("OUTLINER : set_entity_activity 111:: %i\n", hex_surface_object->active_object);
					}}
					break;
				// Other category types below this line
			}

		}
	}

	void change_entity_name(id_type entity_id, id_type category_id, name_type entity_name) {
		switch (category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL :{
				voxel_hcp_object_class *voxel_hcp_object = scene_manager->get_voxel_hcp_entity_object(entity_id);
				voxel_hcp_object->object_name = entity_name;}
				break;

			case ENTITY_CATEGORY_HCP_SURF :{
				hex_surface_object_class *hex_surface_object = scene_manager->get_hex_surface_entity_object(entity_id);
				hex_surface_object->object_name = entity_name;}
				break;
			// Other category types below this line
		}
	}

	void change_entity_description(id_type entity_id, id_type category_id, name_type entity_description) {
		switch (category_id) {
		case ENTITY_CATEGORY_HCP_VOXEL: {
			voxel_hcp_object_class* voxel_hcp_object = scene_manager->get_voxel_hcp_entity_object(entity_id);
			voxel_hcp_object->object_description = entity_description; }
			break;

		case ENTITY_CATEGORY_HCP_SURF: {
			hex_surface_object_class* hex_surface_object = scene_manager->get_hex_surface_entity_object(entity_id);
			hex_surface_object->object_description = entity_description; }
			break;
			// Other category types below this line
		}
	}


	void initialise_entity(id_type entity_id, category_id_type category_id) {
		switch (category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL : break;
		}
	}

	// ###################### Voxel export/import functions ############################

	hcp_voxel_import_export_class hcp_voxel_import_export;

	bool import_hcp_voxel_object(id_type object_group_id) {
		char const* patterns[] = { "*.vobj" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save voxel object aborted\n");
			return false;
		}
//else
//	printf("export_hcp_object 00 %s \n", file_pathname);

		std::fstream import_file(file_pathname, std::ios::in);

		if (!import_file) {
			std::string str = " Could not read file \n" + (std::string)file_pathname;
			vwDialogs::message_box("Import voxel object :", str.c_str());
			return false;
		}

		std::string voxel_object_string = FW::filetools::read_all(import_file);

		hcp_voxel_import_export.log_panel = log_panel;
		hcp_voxel_import_export.lines.clear(); hcp_voxel_import_export.lines.shrink_to_fit();
		hcp_voxel_import_export.lines = FW::stringtools::split(voxel_object_string, '\n');// Create a list of strings for each line in the expression code

		hcp_voxel_import_export.line_number = 0;

		return read_voxel_object_file(object_group_id);

		//return false;
	}

	bool read_voxel_object_file(id_type object_group_id) {
		outliner_node_class *group_to_add_to = get_outliner_group_node(object_group_id);
		outliner_node_class *voxel_entity    = add_new_entity(group_to_add_to, ENTITY_CATEGORY_HCP_VOXEL);

//printf( "outliner_manager_class::read_voxel_object_file AAA : %i \n", voxel_entity->category_id);

		if (voxel_entity == NULL) {
//printf( "hcp_voxel_import_export_class::read_voxel_object_file  AAA : new_entity == NULL\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : outliner_manager_class::read_voxel_object_file \n Could not add new entity to object group.\n");
			return false;
		}

		voxel_hcp_object_class *voxel_hcp_object = scene_manager->get_voxel_hcp_entity_object(voxel_entity->entity_id);

		if (voxel_hcp_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : outliner_manager_class::read_voxel_object_file \n Could not add new entity to object group. Could not create/find voxel object in scene database\n");
			delete_object(voxel_entity->entity_id);
			return false;
		}

		hcp_voxel_import_export.log_panel = log_panel;

		if (!hcp_voxel_import_export.read_voxel_object_file(voxel_hcp_object)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to import voxel object data. Error in reading voxel data\n");
			vwDialogs::message_box("Import voxel Data", "Failed to import voxel object data. Error in reading voxel data","ok","error");
			return false;
		}

		voxel_entity->name        = voxel_hcp_object->object_name;
		voxel_entity->description = voxel_hcp_object->object_description;

//printf( "outliner_manager_class::read_voxel_object_file BBBS : %i : %i\n", voxel_entity->category_id, voxel_hcp_object->object_category_id);
		return true;
	}

	// ###################### Hex Surface export/import functions ############################

	hex_surface_import_export_class hex_surface_import_export;

	bool import_hex_surface_object(id_type object_group_id) {
		char const* patterns[] = { "*.hobj" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save hex surface object aborted\n");
			return false;
		}
		//else
		//	printf("export_hcp_object 00 %s \n", file_pathname);

		std::fstream import_file(file_pathname, std::ios::in);

		if (!import_file) {
			std::string str = " Could not read file \n" + (std::string)file_pathname;
			vwDialogs::message_box("Import hex surface object :", str.c_str());
			return false;
		}

		std::string voxel_object_string = FW::filetools::read_all(import_file);

		hex_surface_import_export.log_panel = log_panel;
		hex_surface_import_export.lines.clear(); hcp_voxel_import_export.lines.shrink_to_fit();
		hex_surface_import_export.lines = FW::stringtools::split(voxel_object_string, '\n');// Create a list of strings for each line in the expression code

		hex_surface_import_export.line_number = 0;

		return read_hex_surface_object_file(object_group_id);

		//return false;
	}

	bool read_hex_surface_object_file(id_type object_group_id) {
		outliner_node_class *group_to_add_to = get_outliner_group_node(object_group_id);
		outliner_node_class *hex_entity    = add_new_entity(group_to_add_to, ENTITY_CATEGORY_HCP_SURF);

//printf( "outliner_manager_class::read_voxel_object_file AAA : %i \n", voxel_entity->category_id);

		if (hex_entity == NULL) {
//printf( "hcp_voxel_import_export_class::read_voxel_object_file  AAA : new_entity == NULL\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : outliner_manager_class::read_hex_surface_object_file \n Could not add new entity to object group.\n");
			return false;
		}

		hex_surface_object_class *hex_surface_object = scene_manager->get_hex_surface_entity_object(hex_entity->entity_id);

		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : outliner_manager_class::read_hex_surface_object_file \n Could not add new entity to object group. Could not create/find voxel object in scene database\n");
			delete_object(hex_entity->entity_id);
			return false;
		}

		hex_surface_import_export.log_panel = log_panel;

		if (!hex_surface_import_export.read_hex_surface_object_file(hex_surface_object)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to import hex surface object data. Error in reading hex surface data\n");
			vwDialogs::message_box("Import voxel Data", "Failed to import hex surface object data. Error in reading voxel data", "ok", "error");
			return false;
		}

		hex_entity->name        = hex_surface_object->object_name;
		hex_entity->description = hex_surface_object->object_description;

//printf( "outliner_manager_class::read_voxel_object_file BBBS : %i : %i\n", voxel_entity->category_id, voxel_hcp_object->object_category_id);
		return true;
	}


	// ****
	class vw_import_export_class {
	public:
		std::string     filename_to_write;

		log_panel_class* log_panel = NULL;

		std::fstream stream;

		// ******* Place here the object import/export classes that import/export the object data **************

		hcp_voxel_import_export_class   hcp_voxel_import_export;
		hex_surface_import_export_class hex_surface_import_export;

		// *****************************************************************************************************

		int open_file_stream(std::string filename) {
			filename_to_write = filename;

			stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!

			return true;
		}

		void close_file_stream() {
			stream.close();
		}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!    EXPORT FUNCTIONS  !!!!!!!!!!!!!!!!!!!!!!!!
		bool export_scene_object_group(outliner_node_class *object_group, scene_manager_class *scene_manager) {
			if (object_group == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to export object group: Object group is undefined\n");
				return false;
			}

			bool  group_saved_success = true;

			char const* patterns[] = { "*.vwg" };
			char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

			if (file_pathname == nullptr) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save virtual world group aborted\n");
				return false;
			}

			if (!open_file_stream(file_pathname)) {
				std::string str = "export voxel object ERROR:\n Failed to open file \n" + (std::string)file_pathname + "\nto write to.";
				vwDialogs::message_box("export_node", str.c_str());
				return false;
			}

			group_saved_success = export_object_group_data_to_file(object_group, scene_manager);

			close_file_stream();

			return group_saved_success;
		}

		bool export_scene(outliner_node_class *scene, scene_manager_class *scene_manager) {
			if (scene == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to export scene: Scene is undefined\n");
				return false;
			}

			bool  scene_saved_success = true;
			char const* patterns[] = { "*.vws" };
			char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

			if (file_pathname == nullptr) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save virtual world scene aborted\n");
				return false;
			}

			if (!open_file_stream(file_pathname)) {
				std::string str = "export voxel object ERROR:\n Failed to open file \n" + (std::string)file_pathname + "\nto write to.";
				vwDialogs::message_box("export_node", str.c_str());
				return false;
			}

			scene_saved_success = export_scene_data_to_file(scene, scene_manager);

			close_file_stream();

			return scene_saved_success;
		}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!    IMPORT FUNCTIONS  !!!!!!!!!!!!!!!!!!!!!!!!
		bool import_object_group(outliner_node_class *object_group, outliner_manager_class *outliner_manager) {
			if (object_group == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to import voxel object group: Group to import voxel object into is undefined\n");
				return false;
			}

			char const* patterns[] = { "*.vwg" };
			char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

			if (file_pathname == nullptr) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save voxel object aborted\n");
				return false;
			}
//else
//	printf("export_hcp_object 00 %s \n", file_pathname);

			std::fstream import_file(file_pathname, std::ios::in);

			if (!import_file) {
				std::string str = "Could not read file \n" + (std::string)file_pathname;
				vwDialogs::message_box("Import voxel object", str.c_str());
				return false;
			}

			std::string voxel_object_group_string = FW::filetools::read_all(import_file);
//printf("vw_import_export_class :import_object_group 00 \n");
			lines.clear(); lines.shrink_to_fit();
			lines = FW::stringtools::split(voxel_object_group_string, '\n');// Create a list of strings for each line in the expression code

			line_number = 0;
//printf("vw_import_export_class :import_object_group 11\n");
			return read_object_group_file(object_group, outliner_manager);
		}


		bool import_virtual_world_scene(outliner_manager_class* outliner_manager) {//, scene_manager_class *scene_manager) {
			if (outliner_manager == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not import scene data. Undefined outliner manager to import into\n");
				return false;
			}

			char const* patterns[] = { "*.vws" };
			char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

			if (file_pathname == nullptr) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save voxel object aborted\n");
				return false;
			}
			else
				printf("export_hcp_object 00 %s \n", file_pathname);

			std::fstream import_file(file_pathname, std::ios::in);

			if (!import_file) {
				std::string str = "Could not read file \n" + (std::string)file_pathname;
				vwDialogs::message_box("Import voxel object", str.c_str());
				return false;
			}

			std::string virtual_world_scene_string = FW::filetools::read_all(import_file);

			lines.clear(); lines.shrink_to_fit();
			lines = FW::stringtools::split(virtual_world_scene_string, '\n');// Create a list of strings for each line in the expression code

			line_number = 0;
			//return read_scene_data_file(scene, scene_manager);
			return read_scene_data_file(outliner_manager);
		}

	private:
		int line_number = 0;
		std::vector<std::string>  lines;
		std::string line;

		bool export_scene_data_to_file(outliner_node_class* scene, scene_manager_class* scene_manager) {

			stream << SCENE_BLOCK_START << endl;
			index_type number_object_groups = scene->children.size();

			for (index_type i = 0; i < number_object_groups; i++) {
				outliner_node_class *object_group = scene->children[i];

				if (!export_object_group_data_to_file(object_group, scene_manager)) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to export scene data to file.\n");
					return false;
				}
			}

			stream << SCENE_BLOCK_END << endl;

			return true;
		}

		bool export_object_group_data_to_file(outliner_node_class *object_group, scene_manager_class* vw_scene) {
//printf("in export_voxel_object_group_data_to_file ()");
			stream << OBJECT_GROUP_BLOCK_START << endl;

			stream << object_group->name << endl;
			stream << object_group->description << endl;

//printf("Outliner Manager :: export_object_group_data_to_file 111 : %i : \n", object_group->children.size());
			for (index_type i = 0; i < object_group->children.size(); i++) {
				outliner_node_class *entity = object_group->children[i];

//printf("Outliner Manager :: export_object_group_data_to_file 333 : %i : \n", entity->category_id);
				// seems swicth does not like using  entity_categories->categories_map[HCP_VOXEL_CATEGORY].category_id or any other map variable as a case value
				if (entity->category_id == ENTITY_CATEGORY_HCP_VOXEL) {
					// this has only one component type : need to modify if/when additional components added to hcp entity
					voxel_hcp_object_class *voxel_hcp_object = vw_scene->get_voxel_hcp_entity_object(entity->entity_id);

					if (voxel_hcp_object == NULL) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not export entity %s to group save file :: Unable to locate component \n", entity->name);
					}
					else {
//printf("Outliner Manager :: export_object_group_data_to_file 444 : %i : \n", entity->category_id);
						stream << HCP_VOXEL_CATEGORY << endl;// ****
						hcp_voxel_import_export.initialise_hcp_voxel_export(filename_to_write, log_panel);
						hcp_voxel_import_export.stream.set_rdbuf(stream.rdbuf());// Not sure this will work !!!!!! It works
						hcp_voxel_import_export.export_voxel_object_data_to_file(voxel_hcp_object, ENTITY_CATEGORY_HCP_VOXEL);
					}
				}

				if (entity->category_id == ENTITY_CATEGORY_HCP_SURF) {
					// this has only one component type : need to modify if/when additional components added to hcp entity
					hex_surface_object_class *hex_surface_object = vw_scene->get_hex_surface_entity_object(entity->entity_id);

					if (hex_surface_object == NULL) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not export entity %s to group save file :: Unable to locate component \n", entity->name);
					}
					else {
//printf("Outliner Manager :: export_object_group_data_to_file 444 : %i : \n", entity->category_id);
						stream << HEX_SURFACE_CATEGORY << endl;// ****
						hex_surface_import_export.initialise_hex_surface_export(filename_to_write, log_panel);
						hex_surface_import_export.stream.set_rdbuf(stream.rdbuf());// Not sure this will work !!!!!! It works
						hex_surface_import_export.export_hex_surface_object_data_to_file(hex_surface_object, ENTITY_CATEGORY_HCP_SURF);
					}
				}

				// Other data types to be added below
			}

			stream << OBJECT_GROUP_BLOCK_END << endl;

			return true;
		}

		bool read_object_group_file(outliner_node_class *object_group, outliner_manager_class *outliner_manager) {
			bool have_category = false;
			// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
			while (!FW::stringtools::contains(lines[line_number], OBJECT_GROUP_BLOCK_START))
				line_number++;

//printf("Outliner Manager :: read_object_group_file 00 \n");
			// If have not found a defined expression return an error
			if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel object", "Import voxel generation model ERROR : \n Could not find start block to import parameter data.", QMessageBox::Ok);
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object start block to import parameter data. \n");
				return false;
			}

			line_number++;
			if (lines.size() < line_number + 5) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.", QMessageBox::Ok);
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : voxel general data\n");
				return false;
			}

//printf("Outliner Manager :: read_object_group_file 11 \n");
			outliner_node_class *new_group = outliner_manager->add_new_group(object_group);
			if (new_group == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create a new group in computer memeory to import group into \n");
				return false;
			}
//printf("Outliner Manager :: read_object_group_file 11AAAA \n");
			std::string line = lines[line_number];  FW::stringtools::truncate(line, line.size()); // get rid of end of line char
			new_group->name = line;         line_number++;
			line = lines[line_number];  FW::stringtools::truncate(line, line.size()); // get rid of end of line char
			new_group->description = line;         line_number++;

//printf("Outliner Manager :: read_object_group_file 22  : %i : %s \n",line_number, line.c_str());
			while (!FW::stringtools::contains(lines[line_number], OBJECT_GROUP_BLOCK_END)) {
//printf("Outliner Manager :: read_object_group_file 22AAAA \n");
				// ********** READ OBJECTS OF CATEGORIES FROM FILE **********
				// next line must contain the object group entity category type
				have_category = false;
				line = lines[line_number];  FW::stringtools::truncate(line, line.size()); // get rid of end of line char
				name_type category = line; line_number++;
//printf("Outliner Manager :: read_object_group_file 22BBBS : %i : %s \n", line_number,line.c_str());
				//id_type category = stoi(line); line_number++;
//printf("Outliner Manager :: read_object_group_file 33 : %i : %s : %s : \n",line_number, category.c_str(), line.c_str());
				//if (category == ENTITY_CATEGORY_HCP_VOXEL) {
				if (category == HCP_VOXEL_CATEGORY) {
					hcp_voxel_import_export.log_panel   = log_panel;
					hcp_voxel_import_export.line_number = line_number;
					hcp_voxel_import_export.lines       = lines;

//printf("Outliner Manager :: read_object_group_file 44 \n");
					outliner_node_class    *entity           = outliner_manager->add_new_entity(new_group, ENTITY_CATEGORY_HCP_VOXEL);
					if (entity == NULL) {// Not tested
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Import voxel object data failed : Could not define entity node\n");
						delete new_group;
						return false;
					}
					
					voxel_hcp_object_class *voxel_hcp_object = outliner_manager->scene_manager->get_voxel_hcp_entity_object(entity->entity_id);

					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// For some god foresaken reason, after carefull checking and finding that somehow by generating the shader program, and then deleting the voxel_hcp_object 
					// and then importing it, the hex shader parameters for variables can still be retained and added to the imported data. 
					// Can only conclude this is some how a VS2022 compilation error or that memory
					// was not deallocated corectly by the CPU. It seems on recreating a voxel_hcp_object in the scene manager, it can reference the memory deallocated that still
					// retains the shader parameter data. So need to clear it out before proceeding to import voxel_hcp_object data. Utterly wierd as am finding
					// some other code functioning with wierd unexplained behavior.

//printf("Outliner Manager :: read_object_group_file 55 %i : %i\n", entity->entity_id, voxel_hcp_object->voxel_object_data.shader_parameters.variables.size());
					if (voxel_hcp_object != NULL && voxel_hcp_object->voxel_object_data.shader_parameters.variables.size() > 0) {
						voxel_hcp_object->voxel_object_data.shader_parameters.variables.clear(); voxel_hcp_object->voxel_object_data.shader_parameters.variables.shrink_to_fit();
					}

					if (voxel_hcp_object != NULL && voxel_hcp_object->voxel_object_data.shader_parameters.int_variables.size() > 0) {
						voxel_hcp_object->voxel_object_data.shader_parameters.int_variables.clear(); voxel_hcp_object->voxel_object_data.shader_parameters.int_variables.shrink_to_fit();
					}

					if (voxel_hcp_object != NULL && voxel_hcp_object->voxel_object_data.shader_parameters.bool_variables.size() > 0) {
						voxel_hcp_object->voxel_object_data.shader_parameters.bool_variables.clear(); voxel_hcp_object->voxel_object_data.shader_parameters.bool_variables.shrink_to_fit();
					}
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//printf("Outliner Manager :: read_object_group_file 55AA %i : %i\n", entity->entity_id, voxel_hcp_object->voxel_object_data.shader_parameters.variables.size());
					if (!hcp_voxel_import_export.read_voxel_object_file(voxel_hcp_object)) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Import voxel object data failed\n");
						for (outliner_node_class *group_entity: new_group->children) {
							outliner_manager->scene_manager->delete_entity(group_entity->entity_id, ENTITY_CATEGORY_HCP_VOXEL);
							delete group_entity;
						}
						delete new_group;
						return false;
					}
//printf("Outliner Manager :: read_object_group_file 66 \n");
					entity->name        = voxel_hcp_object->object_name;
					entity->description = voxel_hcp_object->object_description;

					line_number = hcp_voxel_import_export.line_number;
					have_category = true;
				}

				if (category == HEX_SURFACE_CATEGORY) {
					hex_surface_import_export.log_panel   = log_panel;
					hex_surface_import_export.line_number = line_number;
					hex_surface_import_export.lines       = lines;

//printf("Outliner Manager :: read_object_group_file 44 \n");
					outliner_node_class      *entity             = outliner_manager->add_new_entity(new_group, ENTITY_CATEGORY_HCP_SURF);
					if (entity == NULL) {// Not tested
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Import hex surface object data failed : Could not define entity node\n");
						delete new_group;
						return false;
					}
					hex_surface_object_class *hex_surface_object = outliner_manager->scene_manager->get_hex_surface_entity_object(entity->entity_id);
					
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// For some god foresaken reason, after carefull checking and finding that somehow by generating the shader program, and then deleting the hex_surface_object 
					// and then importing it, the hex shader parameters for variables can still be retained and added to the imported data. 
					// Can only conclude this is some how a VS2022 compilation error or that memory
					// was not deallocated corectly by the CPU. It seems on recreating a hex_surface_object in the scene manager, it can reference the memory deallocated that still
					// retains the shader parameter data. So need to clear it out before proceeding to import voxel_hcp_object data. Utterly wierd as am finding
					// some other code functioning with wierd unexplained behavior. 
//printf("Outliner Manager :: read_object_group_file 55 %i : %i\n", entity->entity_id, voxel_hcp_object->voxel_object_data.shader_parameters.variables.size());
					if (hex_surface_object != NULL && hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.variables.size() > 0) {
						hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.variables.clear(); hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.variables.shrink_to_fit();
					}

					if (hex_surface_object != NULL && hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.int_variables.size() > 0) {
						hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.int_variables.clear(); hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.int_variables.shrink_to_fit();
					}

					if (hex_surface_object != NULL && hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.bool_variables.size() > 0) {
						hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.bool_variables.clear(); hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.bool_variables.shrink_to_fit();
					}
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//printf("Outliner Manager :: read_object_group_file 55 \n");
//if(hex_surface_object == NULL )printf("hex_surface_object == NULL\n");
					if (!hex_surface_import_export.read_hex_surface_object_file(hex_surface_object)) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Import hex surface object data failed\n");
						for (outliner_node_class *group_entity: new_group->children) {
							outliner_manager->scene_manager->delete_entity(group_entity->entity_id, ENTITY_CATEGORY_HCP_SURF);
							delete group_entity;
						}
						delete new_group;
						return false;
					}
//printf("Outliner Manager :: read_object_group_file 66 \n");
					entity->name        = hex_surface_object->object_name;
					entity->description = hex_surface_object->object_description;

					line_number = hex_surface_import_export.line_number;
					have_category = true;
				}

				// Other data types to be added below

				// ********************************************************
				if (!have_category) {
//QMessageBox::information(NULL, "", "vw_import_export_class:: read_object_group_file 2222 !have_category " + line + " " + category, QMessageBox::Ok);
					// error message
					//delete entities
					//delete group
					return false;
				}

//QMessageBox::information(NULL, "Import voxel object", "hcp_voxel_import_export_class:: read_voxel_object_group_file 01  " + std::string::number(entity.entity_id), QMessageBox::Ok);

				line_number++;
//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_group_file 01  " + std::string::number(line_number) + " : "  + lines[line_number], QMessageBox::Ok);
//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_group_file 03 " + std::string::number(line_number) + " : " + lines[line_number]);
//}else
//log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_group_file 03 " + std::string::number(line_number));

				if (line_number >= lines.size()) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : importing group object data has ill defined data. Reached unexpeced end of data file while importing object group.\n");
					return false;
				}
			}

			return true;
		}

		//bool read_scene_data_file(outliner_node_class *scene, scene_manager_class *scene_manager) {
		bool read_scene_data_file(outliner_manager_class *outliner_manager) {
			//while (!lines[line_number].contains(SCENE_BLOCK_START))
			while (!FW::stringtools::contains(lines[line_number], SCENE_BLOCK_START))
				line_number++;

			// If have not found a defined expression return an error
			if (line_number >= lines.size()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find scene start block to import parameter data.\n");
				return false;
			}

			// If have scene data to be imported it would go here. Presently none reqiured as multiple scenes not implemented
			line_number++;
			//while (!lines[line_number].contains(SCENE_BLOCK_END)) {
			while (!FW::stringtools::contains(lines[line_number], SCENE_BLOCK_END)) {
//QMessageBox::information(NULL, "Import voxel object", "read_virtual_world_scene_file 00  " + std::string::number(line_number), QMessageBox::Ok);

				if (!read_object_group_file(outliner_manager->root_node, outliner_manager)) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Import voxel object group failed\n");

//if (log_widget != NULL) {
//	log_display.dialog = true;
//	log_widget->log_message(log_display, log_message_type_enum_type::error, "Could not find scene start block to import parameter data.");
//}
					return false;
				}

				line_number++;
//QMessageBox::information(NULL, "Import voxel object", "read_virtual_world_scene_file 01  " + std::string::number(line_number) + " : "  + lines[line_number], QMessageBox::Ok);
//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_group_file 03 " + std::string::number(line_number) + " : " + lines[line_number]);
//}
//else
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_group_file 03 " + std::string::number(line_number));

				if (line_number >= lines.size()) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : importing scene data has ill defined data. Reached unexpeced end of data file while importing virtual world scene.\n");
					return false;
				}
			}

			return true;
		}
	};
	// ****
};