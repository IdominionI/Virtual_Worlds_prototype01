#pragma once

#include <Source/Editor/Animation/animation_basis.h>

#include "voxel_animation_functions.h"
#include "../Render/voxel_hcp_render.h"

class scene_hsp_voxel_animation_class : public animation_base_class {
public:
	void clear_objects_to_execute() {
		voxel_animation_functions.voxel_hcp_objects_to_execute.clear();
	}

	bool define_objects_to_execute(int current_selected_object_type_id,int animation_selection, scene_manager_class* scene_manager, id_type current_selected_object_id) {
		if (scene_manager == NULL) {
			vwDialogs::message_box("Voxel Animation", "Cannot perform  Animation :: Scene manager not defined.");
			return false;
		}

//printf("scene_hsp_voxel_animation_class::define_objects_to_execute  11 :: %i : %i : %i\n", current_selected_object_type_id, current_selected_object_id, animation_selection);

		// !!!!!!!!!!!!!!!! VOXEL OBJECT TYPE !!!!!!!!!!!!!!!!!!
		voxel_animation_functions.log_panel = log_panel;
		voxel_animation_functions.voxel_hcp_objects_to_execute.clear();
		voxel_animation_functions.voxel_hcp_objects_to_execute.shrink_to_fit();

		if (current_selected_object_type_id == ENTITY_CATEGORY_HCP_VOXEL && animation_selection == ANIMATION_SELECTED_INDEX || animation_selection != ANIMATION_SELECTED_INDEX) {
			if (!voxel_animation_functions.define_voxel_hcp_objects_to_execute(animation_selection, scene_manager, current_selected_object_id)) {
//printf("scene_hsp_voxel_animation_class::define_objects_to_execute  22 :: !voxel_animation_functions.define_voxel_hcp_objects_to_execute\n", current_selected_object_type_id, current_selected_object_id, animation_selection);
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : editor_animation_widget_class :: Could not define hcp objects to animate\n");
				number_objects_to_animate = 0;
				return false;
			} else{
				number_objects_to_animate = voxel_animation_functions.voxel_hcp_objects_to_execute.size();
//printf("scene_hsp_voxel_animation_class::define_objects_to_execute  22 :: voxel_animation_functions.define_voxel_hcp_objects_to_execute\n", current_selected_object_type_id, current_selected_object_id, animation_selection);
			}
		}

		return true; 
	}

	void define_initial_frame_paramters(bool use_textures, bool use_automata, int max_automata_step_value, int frame_step_end) {
		voxel_animation_functions.define_initial_frame_paramters(use_textures, use_automata, max_automata_step_value, frame_step_end);
	}

	bool perform_animation_frame(animation_texture_model_parameters_struct_type& vw_animation_parameters, bool use_textures, bool use_automata, bool  use_multi_thread_automata,
										 size_t frame, int max_automata_step_value, int animation_step,
										 scene_manager_class* scene_manager) {
	//printf("animation :: perform_animation_frame 00 : %i : %i  : %i : %i\n",frame, animation_step, vw_animation_parameters.current_frame, max_automata_step_value);

//if(scene_manager == NULL)
//printf("animation :: perform_animation_frame 00AA scene_manager == NULL\n");
//else
//printf("animation :: perform_animation_frame 00AA scene_manager != NULL\n");

		// Perform the objects animation frame to execute for each entity data type
		// !!!!!!!!!!!!!!!! VOXEL OBJECT TYPE !!!!!!!!!!!!!!!!!!
		voxel_animation_functions.perform_animation_frame(vw_animation_parameters, use_textures, use_automata, use_multi_thread_automata, frame, max_automata_step_value, animation_step, scene_manager);
//printf("animation :: perform_animation_frame 11 :\n");		
		// ############# UPDATE VOXEL GEOMETRY DATA ##################
		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE ######

		for (int i = 0; i < voxel_animation_functions.voxel_hcp_objects_to_execute.size(); i++) {
			voxel_hcp_object_class *voxel_hcp_object = voxel_animation_functions.voxel_hcp_objects_to_execute[i];

			scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(voxel_hcp_object->object_id);
//printf("animation :: perform_animation_frame 22\n");

			if (scene_voxel_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find voxel in the scene to update geometry data for voxel object. %s\n", voxel_hcp_object->object_name.c_str());
//printf("animation :: perform_animation_frame 33:: scene_voxel_object == NULL.\n");
			}
			else {
//printf("animation :: perform_animation_frame 44::scene_voxel_object != NULL.\n");
				if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object->point_cloud, scene_voxel_object,log_panel)) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated for voxel object %s %i\n", voxel_hcp_object->object_name.c_str());
//printf("animation :: perform_animation_frame 55:: scene_voxel_object not updated\n");
				}
			}
		}

		return true;
	}

	virtual void restore_initial_frame(bool use_textures, scene_manager_class* scene_manager) {
	
		voxel_animation_functions.restore_initial_frame(use_textures, scene_manager);

		// ############# UPDATE VOXEL GEOMETRY DATA ##################
		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		for (int i = 0; i < voxel_animation_functions.voxel_hcp_objects_to_execute.size(); i++) {
			voxel_hcp_object_class *voxel_hcp_object = voxel_animation_functions.voxel_hcp_objects_to_execute[i];

			scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(voxel_hcp_object->object_id);
//printf("restore_initial_frame 11\n");

			if (scene_voxel_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find voxel in the scene to update geometry data for voxel object. %s\n", voxel_hcp_object->object_name.c_str());
//printf("restore_initial_frame 22 scene_voxel_object == NULL.\n");
			}
			else {
//printf("restore_initial_frame 33 scene_voxel_object != NULL.\n");
				if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object->point_cloud, scene_voxel_object, log_panel)) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated for voxel object %s %i\n", voxel_hcp_object->object_name.c_str());
//printf("restore_initial_frame 44 \n");
				}

			}
		}
	};

	bool export_geometry(int animation_surface, std::string saved_mesh_pathname, int frame, std::string file_format) {
		bool export_voxel_mesh = false;

//printf("scene_hsp_voxel_animation_class:export_geometry : %s : %s : %i : %i \n", saved_mesh_pathname.c_str(), file_format.c_str(), animation_surface, frame);

		if (animation_surface == 0) {
//printf("scene_hsp_voxel_animation_class:export_geometry animation_surface == 0\n");
			export_voxel_mesh = voxel_animation_functions.export_voxels_center_point_data(saved_mesh_pathname, frame, file_format);
		}
		else
			if (animation_surface == 1) {
//printf("scene_hsp_voxel_animation_class:export_geometry animation_surface == 1\n");
				export_voxel_mesh = voxel_animation_functions.export_voxels_point_surface_data(saved_mesh_pathname, frame, file_format);
			}
			else {
				if (animation_surface == 2) {
//printf("scene_hsp_voxel_animation_class:export_geometry animation_surface == 2\n");
					export_voxel_mesh = voxel_animation_functions.export_voxels_face_surface_data(saved_mesh_pathname, frame, file_format);
				}
			}

		if (!export_voxel_mesh)
			return false;
		else
			return true;
	}


private:
	voxel_animation_functions_class	voxel_animation_functions;
	voxel_hcp_render_class			voxel_hcp_render;

};