#pragma once

#include <vector>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Scene/scene_manager.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>
#include "../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"

#include "../Editor/Widgets/generator_widget.h"
#include "../Editor/Widgets/shaders_widget.h"

#include "../Compute/voxel_compute_generation.h"

#include "../Functions/vw_voxel_exports.h"


#include <Source/Editor/Main_Window/Panels/log_panel.h>
/*
	voxel_animation_functions_class

	C++ class that contains functions that are required for the hcp voxel data object model
	to be animated in the Virtual Worlds application.


*/

class voxel_animation_functions_class {
public:
	log_panel_class *log_panel = NULL;

	// List of pointers to the hcp voxel data objects in the Virtual Worlds scene data model that are to be rendered in the scene
	std::vector<voxel_hcp_object_class*>   voxel_hcp_objects_to_execute;

	// List of initial hcp voxel data object parameter data that are to be rendered in the scene
	std::vector<voxel_generator_parameters_struct_type>    vw_initial_voxel_generator_variables_array;
	std::vector<material_struct_type>					   vw_initial_voxel_shader_generator_variables_array;
	//std::vector<shader_parameters_struct_type>             vw_initial_voxel_shader_generator_variables_array;

	// List of hcp voxel data object compute generator classes that generate the hcp point cloud data the are to be rendered in the scene
	std::vector<voxel_compute_generator_class*> voxel_generators;

	// ------------- VOXEL HCP OBJECTS ----------------

	void set_display_shader_native_vertex_colors(bool display_voxel_object_data, bool display_object_renderer) {
		for(voxel_hcp_object_class * voxel_hcp_object: voxel_hcp_objects_to_execute) {
			voxel_hcp_object->voxel_object_data.shader_parameters.shader_display_parameters.display_default_vertex_shader = display_voxel_object_data;
		}
	}

	void define_initial_frame_paramters(bool use_voxel_texture_cb, bool use_voxel_automata_cb, int& max_automata_step_value, int frame_step_end) {
		vw_initial_voxel_generator_variables_array.clear();
		vw_initial_voxel_generator_variables_array.shrink_to_fit();

		vw_initial_voxel_shader_generator_variables_array.clear();
		vw_initial_voxel_shader_generator_variables_array.shrink_to_fit();

//printf("define_initial_frame_paramters 000 : %i\n", voxel_hcp_objects_to_execute.size());

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//***** Animations at this time can only have the first animation frame being frame zero to begin the animation from
		//***** In partiucular this is the case for cellula Automata as subsequent automata states can only be done from an initial starting conditions at a particular stating frame,
		//***** and then have the automata rules apply once that starting frame is defined.
		//***** A future enhancement to allow the function to be generated at a specified frame can be done by taking all of the initial variable parameters at frame 0 and adding a
		//***** multiple of the framestep value by the first animation frame value and then performing the function generation
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// Define and store initial values and first step of all objects to be animated
		int index = 0;
		for(voxel_hcp_object_class *voxel_hcp_object: voxel_hcp_objects_to_execute) {
			// voxel function generation initial paramters for animation
			vw_initial_voxel_generator_variables_array.push_back(voxel_hcp_object->voxel_object_data.voxel_generator_parameters);

//printf("define_initial_frame_paramters 111 : %f\n", voxel_hcp_object->voxel_object_data.voxel_generator_parameters.resolution_step);
//printf("define_initial_frame_paramters 222 : %f\n", vw_initial_voxel_generator_variables_array[index].resolution_step);
			// get voxel texture initial parameters for animation 
			if (use_voxel_texture_cb) {
//printf("define_initial_frame_paramters 333 :\n");
				vw_initial_voxel_shader_generator_variables_array.push_back(voxel_hcp_object->voxel_object_data.shader_parameters);
			}
//printf("define_initial_frame_paramters 444 :\n");
			// get voxel automata initial rule parameters for animation
			if (use_voxel_automata_cb) {
				// Get max animation frame in which to perform cellula automata calculations
				for(voxel_hcp_automata_byte_rule_struct_type voxel_hcp_automata_byte_rule : voxel_hcp_object->voxel_object_data.voxel_hcp_automata_byte_rules) {
					if (voxel_hcp_automata_byte_rule.end_step > max_automata_step_value)
						if (voxel_hcp_automata_byte_rule.end_step >= frame_step_end)
							max_automata_step_value = voxel_hcp_automata_byte_rule.end_step;
						else
							max_automata_step_value = voxel_hcp_automata_byte_rule.end_step;
				}
			}
			index++;
		}
	}

	bool define_voxel_hcp_objects_to_execute(int object_selection_cb, scene_manager_class *scene_manager,id_type current_selected_object_id) {
		voxel_hcp_objects_to_execute.clear();
		voxel_hcp_objects_to_execute.shrink_to_fit();

		int index = scene_manager->entities_manager.get_objects_of_category_index(ENTITY_CATEGORY_HCP_VOXEL); // ****
		if (index < 0) return false; // ****

		voxel_hcp_scene_objects_class *voxel_hcp_entities_to_export = dynamic_cast <voxel_hcp_scene_objects_class *>(scene_manager->entities_manager.scene_objects[index]); // ****

		switch (object_selection_cb) {
			//case 0: if (!define_selected_animation_voxel(current_selected_object_id, scene_manager->entities_manager.voxel_hcp_scene_objects)) return false;  break;
			//case 1: if (!define_active_animation_voxel(scene_manager->entities_manager.voxel_hcp_scene_objects)) return false; break;
			//case 2: if (!define_all_animation_voxel(scene_manager->entities_manager.voxel_hcp_scene_objects)) return false;    break;
			case 0: if (!define_selected_animation_voxel(current_selected_object_id, *voxel_hcp_entities_to_export)) return false;  break;
			case 1: if (!define_active_animation_voxel(*voxel_hcp_entities_to_export)) return false; break;
			case 2: if (!define_all_animation_voxel(*voxel_hcp_entities_to_export)) return false;    break;
			default: return false;               break;
		}
		
		voxel_generators.clear();
		voxel_generators.shrink_to_fit();
		for (index_type i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
			voxel_compute_generator_class* voxel_generator = new voxel_compute_generator_class; // If this is not a pointer, application crashes when push_backing voxel_generator
			voxel_generator->cloud = voxel_hcp_objects_to_execute[i];
			voxel_generators.push_back(voxel_generator);
		}

		return true;
	}

	bool define_selected_animation_voxel(id_type entity_id,voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export) {
		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_entities_to_export.get_voxel_hcp_object(entity_id);

		if (voxel_hcp_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
			return false;
		}

		voxel_hcp_objects_to_execute.push_back(voxel_hcp_object);

		return true;
	}

	bool define_active_animation_voxel(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class* voxel_hcp_object = voxel_hcp_entities_to_export[i];

			if (voxel_hcp_object->active_object) {
				voxel_hcp_objects_to_execute.push_back(voxel_hcp_object);
			}
		}

		if (voxel_hcp_objects_to_execute.size() == 0) return false;

		return true;
	}

	bool define_all_animation_voxel(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_entities_to_export[i];
			voxel_hcp_objects_to_execute.push_back(voxel_hcp_object);
		}

		if (voxel_hcp_objects_to_execute.size() == 0) return false;

		return true;
	}

/*
	bool define_voxel_hcp_entities_to_execute(QComboBox* object_selection_cb, vw_editor_viewer_class* fp_viewer) {
		voxel_hcp_entities_to_execute.clear();
		voxel_hcp_entities_to_execute.shrink_to_fit();

		int selection;
		//QMessageBox::information(NULL, "","in define_voxel_hcp_entities_to_execute : " + QString::number(object_selection_cb->currentIndex()), QMessageBox::Ok);
		switch (object_selection_cb->currentIndex()) {
			case 0: selection = SELECTED_INDEX;  break;
			case 1: selection = ACTIVATED_INDEX; break;
			case 2: selection = ALL_INDEX;       break;
			default: return false;               break;
		}

		voxel_hcp_entities_to_execute = fp_viewer->viewer_render.vw_scene->get_selection_scenes_entities_of_type_and_category(HCP_VOXEL_CATEGORY, selection);

		// for future consideration to be modified
		//for (index_type i; i < voxel_hcp_objects_to_execute.size(); i++) {
		//		voxel_compute_generator_class* voxel_generator = new voxel_compute_generator_class; // If this is not a pointer, application crashes when push_backing voxel_generator
		//		voxel_generator->cloud = voxel_hcp_objects_to_execute[i];
		//		voxel_generators.push_back(voxel_generator);
		//}

		return true;
	}
*/
	// ###############################################
	// !!!!!!!!!!!!!!!! VOXEL OBJECT TYPE !!!!!!!!!!!!!!!!!!
	void perform_animation_frame(animation_texture_model_parameters_struct_type& vw_animation_parameters,
								bool use_voxel_texture_cb, bool use_voxel_automata_cb, bool use_voxel_automata_multi_thread_cb,
								int frame, int max_automata_step_value, int ANIMATION_STEP,
								//scene_manager_class *scene_manager, int export_selection, id_type entity_id)
								scene_manager_class *scene_manager)
	{
//printf("animation funct :: perform_animation_frame 00 : %i \n",frame);

		// Define voxel function generation frame value for current animation frame
		// Keep a eye on this as it may need to be ignored when recording
		for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
			voxel_hcp_objects_to_execute[i]->voxel_object_data.voxel_generator_parameters.frame = frame;
		}

//printf("animation funct :: perform_animation_frame 11 : %i \n",frame);
		// Define voxel texture generation frame value for current animation frame
		if (use_voxel_texture_cb) {
//printf("animation funct :: perform_animation_fame 22 : %i \n",voxel_hcp_objects_to_execute.size());
			for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
				voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters.frame = frame;
				//voxel_hcp_objects_to_execute[i]->object_renderer->object_ogl_shader->vertex_shader.shader_parameters.frame = frame; // Need a way to directly reference object parameters without copying them ****
			}
		}

//printf("animation funct :: perform_animation_frame 33 : %i \n", voxel_hcp_objects_to_execute.size());
		// perform voxel function generation frame value for current animation frame
		// If automata animation is to be used and at the first frame to be animated or
		// If automata animation is not to be used
		if ((use_voxel_automata_cb && frame == vw_animation_parameters.frame_step_start) || !use_voxel_automata_cb) {
//printf("animation funct :: perform_animation_frame 44 : %i \n", voxel_hcp_objects_to_execute.size());
			for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
				generate_animation_frame(i, frame, vw_animation_parameters, ANIMATION_STEP, scene_manager, voxel_hcp_objects_to_execute[i]->object_id);
			}
		}

//printf("animation funct :: perform_animation_frame 55 : %i \n", use_voxel_automata_cb, frame, max_automata_step_value);

		// Perform voxel automata rules on current frame step if that frame is less than the maximum permited frame of the automata rules
		if (use_voxel_automata_cb) {
			for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
				if (voxel_hcp_objects_to_execute[i]->animate_automata && voxel_hcp_objects_to_execute[i]->automata_rule_within_frame(frame)) { // voxel object designated to animate automata rules
//printf("animation funct :: perform_animation_frame 66 voxel_hcp_objects_to_execute[i]->animate_automata\n");
					if (!perform_voxel_automata_rules(frame, voxel_hcp_objects_to_execute[i], use_voxel_automata_multi_thread_cb))
						break;
				}
				else {
//printf("animation funct :: perform_animation_frame 77 !voxel_hcp_objects_to_execute[i]->animate_automata\n");
					generate_animation_frame(i, frame, vw_animation_parameters, ANIMATION_STEP, scene_manager, voxel_hcp_objects_to_execute[i]->object_id);// if voxel object not designated to animate automata rules, generate normal animation frame
				}
			}
		}

//printf("animation funct :: perform_animation_fame 77 : %i \n",frame);
		// perform voxel texture generation frame value for current animation frame
		if (use_voxel_texture_cb) {
			for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
				//if (voxel_hcp_objects_to_execute[i]->animate_shaders) {// voxel object designated to animate shader ****
				if (voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters.animate_shaders) {// voxel object designated to animate shader ****
//printf("animation funct :: perform_animation_fame 88 : %i \n",frame);
					update_voxel_shader_parameter_step_values(voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters, ANIMATION_STEP, scene_manager, voxel_hcp_objects_to_execute[i]->object_id);
					//voxel_hcp_objects_to_execute[i]->object_renderer->object_ogl_shader->vertex_shader.shader_parameters = voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters; // Need a way to directly reference object parameters without copying them ****
				}
				//else {
				//	QMessageBox::information(NULL, "", "in play animation011 !voxel_hcp_objects_to_execute[i]->animate_shaders", QMessageBox::Ok);
				//}
			}
		}
	}

	// #################################################

	void generate_animation_frame(int index, int frame, animation_texture_model_parameters_struct_type &vw_animation_parameters, int ANIMATION_STEP, scene_manager_class* scene_manager, id_type entity_id) {
		update_voxel_generator_parameter_step_values(voxel_hcp_objects_to_execute[index]->voxel_object_data.voxel_generator_parameters, ANIMATION_STEP,scene_manager,entity_id);

		if (frame == vw_animation_parameters.frame_step_start) {
//printf("animation funct :: generate_animation_frame 00 : \n");
			voxel_generators[index]->generate_voxel_function();
			//voxel_hcp_objects_to_execute[index]->update_renderer_shader_variables();
		}
		else {
//printf("animation funct :: generate_animation_frame 11 : \n");
			voxel_generators[index]->update_voxel_generation();
		}
//printf("animation funct :: generate_animation_frame 22 : \n");
		voxel_hcp_objects_to_execute[index]->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
	}

	// ###############################################
	void restore_initial_frame(bool use_voxel_texture_cb, scene_manager_class *scene_manager) {
		if (voxel_hcp_objects_to_execute.size() > 0) {

			if (voxel_hcp_objects_to_execute.size() < 1 || vw_initial_voxel_generator_variables_array.size() != voxel_hcp_objects_to_execute.size()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("INFO : editor_animation_widget_class::No objects or initial animation objects have changed in number\n");
				return;
			}

			if (use_voxel_texture_cb) {
				if (vw_initial_voxel_shader_generator_variables_array.size() != voxel_hcp_objects_to_execute.size()) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : editor_animation_widget_class :: Could not restore initial frame conditions for object shader variables.\n  Current objects added or deleted or variables not used in initial frame \n");
				}
				else {
					for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
						voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters = vw_initial_voxel_shader_generator_variables_array[i];
						voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters.frame = 0;
						update_shader_variables(scene_manager, voxel_hcp_objects_to_execute[i]->voxel_object_data.shader_parameters, voxel_hcp_objects_to_execute[i]->object_id);
					}
				}
			}

			// restore original voxel genereation
			for (int i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
				voxel_hcp_objects_to_execute[i]->voxel_object_data.voxel_generator_parameters = vw_initial_voxel_generator_variables_array[i];
				voxel_hcp_objects_to_execute[i]->voxel_object_data.voxel_generator_parameters.frame = 0;

				voxel_generators[i]->generate_voxel_function();

				voxel_hcp_objects_to_execute[i]->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
			}
		}
	}

	// !!!!!!!!!!!!!!!!!!! VOXEL UPDATES !!!!!!!!!!!!!!!!!!

	void update_voxel_generator_parameter_step_values(voxel_generator_parameters_struct_type& voxel_generator_parameters, int step_action, scene_manager_class *scene_manager, id_type entity_id) {
//printf("update_voxel_generator_parameter_step_values::00L %i\n", voxel_generator_parameters.variables.size());

		// Get the scene render object that stores the object render properties
		//voxel_hcp_object_class *voxel_hcp_object = scene_manager->get_voxel_hcp_entity_object(entity_id);
		voxel_hcp_object_class *voxel_hcp_object = (voxel_hcp_object_class*)scene_manager->get_entity_object(entity_id, ENTITY_CATEGORY_HCP_VOXEL);

		if (voxel_hcp_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders for voxel object of id %i\n", entity_id);
//printf("update_voxel_generator_parameter_step_values::update_voxel_generator_parameter_step_values :entity_render_object == NULL\n");
			return;
		}
//printf("update_voxel_generator_parameter_step_values::11\n");

		int i = 0;
		for (voxel_generator_parameter_variable_struct_type variable : voxel_generator_parameters.variables) {
			if (variable.active_variable) {
				if (variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						variable.value = variable.value - variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							variable.value = variable.value + variable.variable_step;

					voxel_generator_parameters.variables[i].value = variable.value;
//printf("update_voxel_generator_parameter_step_values::AAA %f :: %f\n", voxel_generator_parameters.variables[i].value, voxel_hcp_object->voxel_object_data.voxel_generator_parameters.variables[i].value);
				}
			}
			i++;
		}

		i = 0;
		for (voxel_generator_parameter_int_variable_struct_type int_variable : voxel_generator_parameters.int_variables) {
			if (int_variable.active_variable) {
				if (int_variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						int_variable.value = int_variable.value - int_variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							int_variable.value = int_variable.value + int_variable.variable_step;

					voxel_generator_parameters.int_variables[i].value = int_variable.value;
				}
			}
			i++;
		}

//printf("update_voxel_generator_parameter_step_values::22\n");

//printf("update_voxel_generator_parameter_step_values::22 \n");
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.variables     = voxel_generator_parameters.variables;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.int_variables = voxel_generator_parameters.int_variables;
	}

	//void update_voxel_shader_parameter_step_values(shader_parameters_struct_type &voxel_texture_parameters, int step_action, scene_manager_class *scene_manager, id_type entity_id) {
	void update_voxel_shader_parameter_step_values(material_struct_type &voxel_texture_parameters, int step_action, scene_manager_class *scene_manager, id_type entity_id) {
		int i = 0;

		// Floating number varaibles
		for(shader_parameter_variable_struct_type variable: voxel_texture_parameters.variables) {
			if (variable.active_variable) {
				if (variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						variable.value = variable.value - variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							variable.value = variable.value + variable.variable_step;

					voxel_texture_parameters.variables[i].value = variable.value;
				}
			}
			i++;
		}

		i = 0;
		// integer number varaibles
		for(shader_parameter_int_variable_struct_type int_variable : voxel_texture_parameters.int_variables) {
			if (int_variable.active_variable) {
				if (int_variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						int_variable.value = int_variable.value - int_variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							int_variable.value = int_variable.value + int_variable.variable_step;

					voxel_texture_parameters.int_variables[i].value = int_variable.value;
				}
			}
			i++;
		}

		update_shader_variables(scene_manager, voxel_texture_parameters,entity_id);
	}

	//void update_shader_variables(scene_manager_class *scene_manager, shader_parameters_struct_type voxel_shader_parameters, id_type entity_id) { // not sure this is needed as render node does this task
	void update_shader_variables(scene_manager_class *scene_manager, material_struct_type voxel_shader_parameters, id_type entity_id) { // not sure this is needed as render node does this task
//printf("voxel_shaders_widget_class::update voxel shaders clicked\n");// replace with update variables
		// Get the scene render object that stores the object render properties
		scene_node_class <render_object_class> *entity_render_object = scene_manager->get_render_object(entity_id);                  
		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::update_shader_variables :entity_render_object == NULL\n");
			return;
		}
//printf("voxel_shaders_widget_class::update_shader_variables 111\n");

		material_struct_type* shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties for the render process

		if (shader_material == NULL) {
//printf("voxel_shaders_widget_class::update_shader_variables 222A shader_material == NULL\n");
			return;
		}
		//else
//printf("voxel_shaders_widget_class::update_shader_variables 222A shader_material != NULL\n");

//printf("voxel_shaders_widget_class::update_shader_variables 222\n");

		shader_material->variables      = voxel_shader_parameters.variables;
		shader_material->int_variables  = voxel_shader_parameters.int_variables;
		shader_material->bool_variables = voxel_shader_parameters.bool_variables;
//printf("voxel_shaders_widget_class::update_shader_variables 333\n");
	}

	bool perform_voxel_automata_step(u_long_int step, voxel_hcp_object_class* voxel_hcp_object) {
		voxel_automata_hcp_functions_class        voxel_automata_hcp_functions;
//if (log_widget != NULL) {
//	printf( "perform_voxel_automata_step 00 : %i \n",step);
//}

		if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() == 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to perform cellula automata step : Have undefined generation of voxels.\n  Current objects added or deleted or variables not used in initial frame \n");
			return false;
		}

		voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object;// ****

		voxel_automata_hcp_functions.cloud = voxel_hcp_object;
		voxel_automata_hcp_functions.log_panel = log_panel;
		voxel_automata_hcp_functions.voxel_hcp_automata_rules = voxel_hcp_object->voxel_object_data.voxel_hcp_automata_byte_rules;

		if (!voxel_automata_hcp_functions.perform_voxel_automata_generation(step)) return false;

//QMessageBox::information(NULL, "", "Time elapsed : "+ QString::number(time.elapsed())+ " ms", QMessageBox::Ok);// testing only
//QMessageBox::information(NULL, "", "perform_voxel_automata_step BBB00", QMessageBox::Ok);

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 01 : " + QString::number(voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size()));
//}

		voxel_hcp_object->voxel_object_data.delete_voxel_matrix_data();
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 02 : " + QString::number(voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size()));
//}

		voxel_hcp_object->voxel_object_data.voxel_matrix_data = voxel_automata_hcp_functions.voxel_automata_next_step_matrix->voxel_object_data.voxel_matrix_data;
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 03 : " + QString::number(voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size()));
//}

		voxel_hcp_object->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
		
		return true;
	}
/*
	bool perform_voxel_automata_step_multithread(u_long_int step, voxel_hcp_object_class* voxel_hcp_object) {
		voxel_automata_hcp_thread_functions_class voxel_automata_hcp_thread_functions;
		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step_multithread 00 : " + QString::number(step));
		//}

		if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() == 0) {
			//if (log_widget != NULL) {
			//	log_widget->log_message(log_display, log_message_type_enum_type::error, "Unable to perform cellula automata step : Have undefined generation of voxels.");
			//}
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to perform cellula automata step : Have undefined generation of voxels. \n");
			return false;
		}

		voxel_automata_hcp_thread_functions.cloud = voxel_hcp_object;
		voxel_automata_hcp_thread_functions.log_panel = log_panel;
		voxel_automata_hcp_thread_functions.voxel_hcp_automata_rules = voxel_hcp_object->voxel_object_data.voxel_hcp_automata_byte_rules;

		if (!voxel_automata_hcp_thread_functions.perform_voxel_automata_generation(step)) return false;

		//QMessageBox::information(NULL, "", "Time elapsed : "+ QString::number(time.elapsed())+ " ms", QMessageBox::Ok);// testing only
		//QMessageBox::information(NULL, "", "perform_voxel_automata_step BBB00", QMessageBox::Ok);

		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 01 : " + QString::number(voxel_hcp_object->voxel_object_data.voxel_matrix_data.size()));
		//}

		voxel_hcp_object->voxel_object_data.delete_voxel_matrix_data();
		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 02 : " + QString::number(voxel_hcp_object->voxel_object_data.voxel_matrix_data.size()));
		//}

		voxel_hcp_object->voxel_object_data.voxel_matrix_data = voxel_automata_hcp_thread_functions.voxel_automata_next_step_matrix->voxel_object_data.voxel_matrix_data;
		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step 03 : " + QString::number(voxel_hcp_object->voxel_object_data.voxel_matrix_data.size()));
		//}

		//voxel_hcp_object->update_renderer_shader_variables();
		voxel_hcp_object->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
		voxel_hcp_object->update_buffer = true;

		return true;
	}
*/
	bool perform_voxel_automata_rules(int frame, voxel_hcp_object_class* voxel_hcp_object, bool use_voxel_automata_multi_thread_cb) {
		//if (use_voxel_automata_multi_thread_cb)
		//	return perform_voxel_automata_step_multithread(frame, voxel_hcp_object);
		//else
			return perform_voxel_automata_step(frame, voxel_hcp_object);
	}


	// !!!!!!!!!!!!!!!!!!!!!!!! EXPORT MESH !!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	// The following save mesh data is a modified implementation from the same save voxel mesh menu options
	// that exits for the Main window. In future for centralisation of this task, merge the two into one
	// class for greater meintenance ability. 

	bool export_voxels_center_point_data(std::string saved_mesh_pathname, int frame, std::string file_format) {
//printf("export_voxels_center_point_data 000 export_mesh\n");
		export_voxel_geometry_class        export_voxel_geometry;
		export_voxel_geometry.log_panel = log_panel;

//printf("export_voxels_center_point_data AAA : %i\n", voxel_hcp_objects_to_execute.size());
		if (voxel_hcp_objects_to_execute.size() < 1) {
			//if (log_widget != NULL) {
			//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class : export_voxels_center_point_data :\n Failed to export center point voxels data. Could not define the HCP data to execute");
			//}
			if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_center_point_data :\n Failed to export center point voxels data. Could not define the HCP data to execute\n");
			//QMessageBox::information(NULL, "","export_mesh : !define_voxel_hcp_entities_to_execute(voxel_hcp_entities_to_execute)", QMessageBox::Ok);
			return false;
		}

//printf("export_voxels_center_point_data AAA : %i\n", voxel_hcp_objects_to_execute.size());

		for (index_type i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
			//entity_class* entity = voxel_hcp_entities_to_execute[i];

			//voxel_hcp_object_class* voxel_hcp_object_to_execute = fp_viewer->viewer_render.vw_scene->ecs_scene_manager.get_entity_component<voxel_hcp_object_class>(entity->entity_id);

			voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_objects_to_execute[i];

//printf("export_voxels_center_point_data BBB : %s\n ", saved_mesh_pathname.c_str());

			if (voxel_hcp_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_center_point_data :\n Failed to export center point voxels data. Could not find voxel HCP component data\n");
				return false;
			}

			saved_mesh_pathname = saved_mesh_pathname + "_cp_" + voxel_hcp_object->object_name + "_" + std::to_string(frame) + file_format;

			//export_voxel_geometry.export_voxel_center_points_to_file_ply(voxel_hcp_object_to_execute->voxel_object_data, saved_mesh_pathname);
//printf("export_voxels_center_point_data CCC : %s\n ", saved_mesh_pathname.c_str());
			export_voxel_geometry.export_voxel_center_points_to_file_ply(voxel_hcp_object->voxel_object_data, saved_mesh_pathname);
		}

		return true;

	}

	bool export_voxels_point_surface_data(std::string saved_mesh_pathname, int frame, std::string file_format) {
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "In export_voxels_point_surface_data", QMessageBox::Ok);

		voxel_surface_data_class voxel_surface_data;

		export_voxel_geometry_class export_voxel_geometry;
		export_voxel_geometry.log_panel = log_panel;

		if (voxel_hcp_objects_to_execute.size() < 1) {
		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class : export_voxels_point_surface_data :\n Failed to export surface point voxels data. Could not define the HCP data to execute");
		//}
			if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_point_surface_data :\n Failed to export surface point voxels data. Could not define the HCP data to execute\n");
//QMessageBox::information(NULL, "","export_mesh : !define_voxel_hcp_entities_to_execute(voxel_hcp_entities_to_execute)", QMessageBox::Ok);
			return false;
		}

		for (index_type i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
			voxel_hcp_object_class *voxel_hcp_object_to_execute = voxel_hcp_objects_to_execute[i];

			voxel_surface_data.cloud = voxel_hcp_object_to_execute;

			if (!voxel_surface_data.create_voxel_surface_point_data()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not generate voxel point surface data for \n %s \n No voxel surfacepoint mesh generated %s", voxel_hcp_object_to_execute->object_name, saved_mesh_pathname);
			}
			else {
				saved_mesh_pathname = saved_mesh_pathname + "_sp_" + voxel_hcp_object_to_execute->object_name + "_" + std::to_string(frame) + file_format;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class ::export_voxels_point_surface_data : " + saved_mesh_pathname);
//}

				export_voxel_geometry.export_voxel_point_surface_data_to_file_ply(voxel_hcp_object_to_execute->voxel_object_data, voxel_surface_data, saved_mesh_pathname);
			}
		}


		return true;
	}

	bool export_voxels_face_surface_data(std::string saved_mesh_pathname, int frame, std::string file_format) {
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "In export_voxels_surface_data", QMessageBox::Ok);
		voxel_surface_face_class voxel_surface_face_data;

		export_voxel_geometry_class export_voxel_geometry;
		export_voxel_geometry.log_panel = log_panel;

		if (voxel_hcp_objects_to_execute.size() < 1) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class : export_voxels_face_surface_data :\n Failed to export face surface voxels data. Could not define the HCP data to execute");
//}
			if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_face_surface_data :\n Failed to export face surface voxels data. Could not define the HCP data to execute\n");
//QMessageBox::information(NULL, "","export_mesh : !define_voxel_hcp_entities_to_execute(voxel_hcp_entities_to_execute)", QMessageBox::Ok);
			return false;
		}

		for (index_type i = 0; i < voxel_hcp_objects_to_execute.size(); i++) {
			voxel_hcp_object_class* voxel_hcp_object_to_execute = voxel_hcp_objects_to_execute[i];

			voxel_surface_face_data.cloud = voxel_hcp_object_to_execute;

			if (!voxel_surface_face_data.create_voxel_surface_data()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not generate voxel surface face data for \n %s \n No voxel surface face mesh generated%s", voxel_hcp_object_to_execute->object_name, saved_mesh_pathname);
			}
			else {
				saved_mesh_pathname = saved_mesh_pathname + "_sf_" + voxel_hcp_object_to_execute->object_name + "_" + std::to_string(frame) + file_format;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class ::export_voxels_point_surface_data : " + saved_mesh_pathname);
//}

				export_voxel_geometry.export_voxel_surface_faces_to_file_ply(voxel_hcp_object_to_execute->voxel_object_data, voxel_surface_face_data, saved_mesh_pathname);
			}
		}

		return true;
	}
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


private:
	
};