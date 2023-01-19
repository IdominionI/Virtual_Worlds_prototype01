#pragma once

#include "../Scene/scene_manager.h"
//#include "../Tools/dialogs.h"

class animation_base_class {
public:
	id_type objects_category_id = -1;

	log_panel_class *log_panel = NULL;

	int number_objects_to_animate = 0;

	bool geometry_exported = false;

	virtual void clear_objects_to_execute() { };

	virtual bool define_objects_to_execute(int current_selected_object_type_id,int animation_selection,scene_manager_class *scene_manager, id_type current_selected_object_id) { return false; }

	virtual void define_initial_frame_paramters(bool use_textures, bool use_automata, int max_automata_step_value,  int frame_step_end) {}

	virtual bool perform_animation_frame(animation_texture_model_parameters_struct_type &vw_animation_parameters,bool use_textures,bool use_automata,bool  use_multi_thread_automata,
										 int frame,int max_automata_step_value,int animation_step, 
										 scene_manager_class *scene_manager) { 
//printf("animation_base_class : perform_animation_frame !!!!!\n");
		return false;}

	virtual void restore_initial_frame(bool use_textures,scene_manager_class *scene_manager) {};

	virtual bool export_geometry(int animation_surface,std::string saved_mesh_pathname, int frame, std::string file_format) { return false; }

protected:

private:
};

class animation_manager_class {
public:
	scene_manager_class *scene_manager = NULL;
	log_panel_class		*log_panel     = NULL;

	id_type          current_selected_object_id      = -1;
	id_type          current_selected_object_type_id = -1;

	std::vector<animation_base_class*> animation_objects;

	animation_texture_model_parameters_struct_type vw_animation_parameters;

	// -----------------------------------------------------------------
	bool define_new_animation_entity_category(id_type entity_category_id) {
		if (entity_category_id > -1) {

			int index = get_animation_objects_of_category_index(entity_category_id);
			if (index >= 0) {
				printf("ERROR : Cannot define new Object category as it exists\n");
				return false;
			}

			animation_base_class *animation_objects_of_category = new animation_base_class;
			if (animation_objects_of_category == NULL) {
				printf("ERROR : Cannot define new Animaion Object category : memory allocation error\n");
				return false;
			}

			animation_objects_of_category->objects_category_id = entity_category_id;
			animation_objects.push_back(animation_objects_of_category);
//printf("scene_entities_db_manager2_class:define_new_entity_category : %i\n", animation_objects.size());
			return true;
		}
		return false;
	}


	int get_animation_objects_of_category_index(id_type entity_category_id) {

//printf("scene_entities_db_manager2_class : get_objects_of_category_index 0000 : %i\n", scene_objects.size());
		for (int i = 0; i < animation_objects.size(); i++) {
//printf("scene_entities_db_manager2_class : get_objects_of_category_index 1111: %i :: %i \n", scene_objects[i]->objects_category_id, entity_category_id);
			if (animation_objects[i]->objects_category_id == entity_category_id)
				return i;
		}

		return -1;
	}

	void delete_animation_entities_of_category(id_type entity_category_id) {
		if (entity_category_id > -1) {
			int index = get_animation_objects_of_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return;
			}

			animation_objects[index]->clear_objects_to_execute();
		}
	}

	void delete_animation_object() {
		for (animation_base_class *animation_object : animation_objects) {
			animation_object->clear_objects_to_execute();
		}

		animation_objects.clear();
	}

	// -----------------------------------------------------------------



	bool use_textures =false;
	bool use_automata = false;
	bool use_multi_thread_automata = false; 
	int  max_automata_step_value;
	int  frame_step_end;

	int animation_selection = 0;
	int animation_surface = 1;
	int start_frame = 0, end_frame = 10, frame_delta = 1;
	int animation_frame = 0, number_animation_frames = 0, current_animation_frame = 0;
	int reverse_end_frame = 0;

	int total_number_objects_to_animate = 0;

	std::string animation_frames_directory_pathname = "";
	std::string animation_frames_directory          = "...";
	std::string filename_template                   = "a_animate";

	std::string saved_filename_prefix = "";
	std::string saved_filename_format = ".ply";

	bool define_objects_to_execute() {
		for (animation_base_class* animation_object : animation_objects) {
			animation_object->define_objects_to_execute(current_selected_object_type_id, animation_selection, scene_manager, current_selected_object_id);
			total_number_objects_to_animate += animation_object->number_objects_to_animate;
		}

		if (total_number_objects_to_animate < 1)
		{
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : editor_animation_widget_class :: No objects selected to animate\n");
			vwDialogs::message_box("Object Animation :: ", "Cannot perform Object Animation\n No objects selected data to animate.");
			return false;
		}

		return true;
	}

	void define_initial_frame_paramters() {
		for (animation_base_class* animation_object : animation_objects) {
			animation_object->define_initial_frame_paramters(use_textures, use_automata,max_automata_step_value, frame_step_end);
		}
	}

	bool perform_animation_frame(int frame, int animation_step) {
//printf("animation_manager_class :: perform_animation_frame 0000 %i \n", animation_objects.size());
		for (animation_base_class* animation_object : animation_objects) {
//if(animation_object != NULL)
//printf("animation_manager_class :: perform_animation_frame 1111 animation_object != NULL  %i : %i\n", animation_object->objects_category_id, animation_object->number_objects_to_animate);
//else
//printf("animation_manager_class :: perform_animation_frame 1111 animation_object == NULL\n");
			animation_object->perform_animation_frame(vw_animation_parameters,use_textures,use_automata,use_multi_thread_automata,
													  frame,max_automata_step_value,animation_step, 
													  scene_manager);
		}
		return true;
	}

	void restore_initial_frame() {
		vw_animation_parameters.current_frame = 0;
		for (animation_base_class* animation_object : animation_objects) {
			animation_object->restore_initial_frame(use_textures,scene_manager);
		}


	}

	void define_animation_parameters() {
		vw_animation_parameters.frame_step_start    = start_frame;
		vw_animation_parameters.frame_step_end      = end_frame;
		vw_animation_parameters.frame_step_interval = frame_delta;
		vw_animation_parameters.current_frame       = current_animation_frame; // ****
	}

	// Save the animation entity objects defined to be animated point cloud data to disk
	bool save_animation_frame_mesh(std::string saved_mesh_pathname, int frame, std::string file_format) {
//printf("save_animation_frame_mesh 000 :: %s : %i : %s : %i\n", saved_mesh_pathname.c_str(), frame, file_format.c_str(), animation_surface);
//if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG :: editor_animation_widget_class :: in save_animation_frame_mesh : %s \n", saved_mesh_pathname);

		if(animation_objects.size() < 1) {
//printf("save_animation_frame_mesh : animation_objects.size() < 1 \n", saved_mesh_pathname.c_str(), frame, file_format.c_str());
			return false;
		}

		for (animation_base_class *animation_object : animation_objects) {
//printf("save_animation_frame_mesh 111 \n");
			animation_object->geometry_exported = animation_object->export_geometry(animation_surface,saved_mesh_pathname, frame, file_format);
		}

		for (animation_base_class *animation_object : animation_objects) {
//printf("save_animation_frame_mesh 222 \n");
			if (animation_object->geometry_exported == false) return false; // add Message to indicate wich object(s) not exported ????
		}

		return true;
	}



//protected:

//private:
};