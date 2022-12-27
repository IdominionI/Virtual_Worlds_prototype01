#pragma once

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Scene/scene_manager.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>

#include "../Hex_surface_object/DataTypes/dt_hex_surface_shader_parameters.h"
#include "../Hex_surface_object/DataTypes/dt_hex_surface_generator.h"
#include "../Hex_surface_object/hex_surface_object.h"

#include "../Editor/Widgets/generator_widget.h"
#include "../Editor/Widgets/shaders_widget.h"

#include "../Compute/hex_surface_compute_generation.h"

#include "../Functions/hex_surface_exports.h"

#include "../Automata/hex_surface_automata_functions.h"

#include <Source/Editor/Main_Window/Panels/log_panel.h>
/*
	hex_surface_animation_functions_class

	C++ class that contains functions that are required for the hex surface data object model
	to be animated in the Virtual Worlds application.
*/

class hex_surface_animation_functions_class {
public:
	log_panel_class* log_panel = NULL;// pointer to log widget to report to

	// List of pointers to the hex surface data objects in the Virtual Worlds scene data model that are to be rendered in the scene
	std::vector<hex_surface_object_class *> hex_surface_objects_to_execute;

	// List of initial hex surface data object parameter data that are to be rendered in the scene
	std::vector<hex_surface_generator_parameters_struct_type> vw_initial_hex_surface_generator_variables_array;
	std::vector<material_struct_type>                         vw_initial_hex_surface_shader_generator_variables_array;
	//std::vector<shader_parameters_struct_type>                vw_initial_hex_surface_shader_generator_variables_array;

	// List of hex surfacel data object compute generator classes that generate the hex suface point cloud data the are to be rendered in the scene
	std::vector<hex_surface_compute_generator_class *>    hex_surface_generators;


	void define_initial_frame_paramters(bool use_hex_texture_cb, bool use_hex_automata_cb, int& max_automata_step_value, int frame_step_end) {
		// -------------- HEX SURFACE OBJECT ---------------
		vw_initial_hex_surface_generator_variables_array.clear();
		vw_initial_hex_surface_generator_variables_array.shrink_to_fit();

		vw_initial_hex_surface_shader_generator_variables_array.clear();
		vw_initial_hex_surface_shader_generator_variables_array.shrink_to_fit();

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//***** Animations at this time can only have the first animation frame being frame zero to begin the animation from
		//***** In partiucular this is the case for cellula Automata as subsequent automata states can only be done from an initial starting conditions at a particular stating frame,
		//***** and then have the automata rules apply once that starting frame is defined.
		//***** A future enhancement to allow the function to be generated at a specified frame can be done by taking all of the initial variable parameters at frame 0 and adding a
		//***** multiple of the framestep value by the first animation frame value and then performing the function generation
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		int index = 0;
		for (hex_surface_object_class *hex_surface_object : hex_surface_objects_to_execute) {
			// voxel function generation initial paramters for animation
			vw_initial_hex_surface_generator_variables_array.push_back(hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters);

			//***** TO DO TO REPLACE ABOVE STATEMENT
			//***** hex_surface_generator_parameters_struct_type initial_hex_surface_generator_parameters = define_initial_hex_surface_generator_animation_parameters(hex_surface_object,frame);
			//***** vw_initial_hex_surface_generator_variables_array.push_back(initial_hex_surface_generator_parameters);

//QMessageBox::information(NULL, "","in play animation 05A : " + std::string::number(hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.resolution_step), QMessageBox::Ok);
//QMessageBox::information(NULL, "","in play animation 05AA : " + std::string::number(vw_initial_hex_surface_generator_variables_array[index].resolution_step), QMessageBox::Ok);
// 
			// get voxel texture initial parameters for animation 
			if (use_hex_texture_cb) {
				vw_initial_hex_surface_shader_generator_variables_array.push_back(hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters);

				//***** TO DO TO REPLACE ABOVE STATEMENT
				//*****shader_parameters_struct_type initial_hex_surface_shader_parameters = define_initial_hex_surface_shader_animation_parameters(hex_surface_object,frame);
				//*****vw_initial_hex_surface_shader_generator_variables_array.push_back(initial_hex_surface_shader_parameters);
			}
//QMessageBox::information(NULL, "","in play animation 05B : ", QMessageBox::Ok);
			// get voxel automata initial rule parameters for animation
			if (use_hex_automata_cb) {
				// Get max animation frame in which to perform cellula automata calculations
				for (hex_surface_automata_rule_struct_type hex_surface_automata_rule : hex_surface_object->hex_surface_object_data.hex_surface_automata_rules) {
					if (hex_surface_automata_rule.end_step > max_automata_step_value)
						if (hex_surface_automata_rule.end_step >= frame_step_end)
							max_automata_step_value = hex_surface_automata_rule.end_step;
						else
							max_automata_step_value = hex_surface_automata_rule.end_step;
				}
			}
			index++;
		}

		// other object types go below here
	}

	// ------------- HEX SURFACE OBJECTS ----------------
	bool define_hex_surface_objects_to_execute(int object_selection_cb, scene_manager_class* scene_manager, id_type current_selected_object_id) {
		hex_surface_objects_to_execute.clear();
		hex_surface_objects_to_execute.shrink_to_fit();

		int selection;

		int index = scene_manager->entities_manager.get_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF); // ****
		if (index < 0) return false; // ****

		hex_surface_scene_objects_class *hex_surface_entities_to_export = dynamic_cast <hex_surface_scene_objects_class*>(scene_manager->entities_manager.scene_objects[index]); // ****

		switch (object_selection_cb) {
			case 0: if (!define_selected_animation_hex_surface(current_selected_object_id, *hex_surface_entities_to_export)) return false;  break;
			case 1: if (!define_active_animation_hex_surface(*hex_surface_entities_to_export)) return false; break;
			case 2: if (!define_all_animation_hex_surface(*hex_surface_entities_to_export)) return false;    break;

			default: return false;               break;
		}

		hex_surface_generators.clear();
		hex_surface_generators.shrink_to_fit();
		for (index_type i = 0; i < hex_surface_objects_to_execute.size(); i++) {
			hex_surface_compute_generator_class *hex_surface_generator = new hex_surface_compute_generator_class; // If this is not a pointer, application crashes when push_backing voxel_generator
			hex_surface_generator->cloud = hex_surface_objects_to_execute[i];
			hex_surface_generators.push_back(hex_surface_generator);
		}

		return true;
	}


	bool define_selected_animation_hex_surface(id_type entity_id, hex_surface_scene_objects_class &hex_surface_entities) {
		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface surface geometry :: Cannot export hex surface point data to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export hex surface point surface to file::No entity selected to export");
			return false;
		}

		hex_surface_object_class *hex_surface_object = hex_surface_entities.get_hex_surface_object(entity_id);

		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface surface geometry :: Cannot export hex surface point data to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export hex surface point surface to file::Could not find entity in scene data to export");
			return false;
		}

		hex_surface_objects_to_execute.push_back(hex_surface_object);

		return true;
	}

	bool define_active_animation_hex_surface(hex_surface_scene_objects_class &hex_surface_entities) {
		for (index_type i = 0; i < hex_surface_entities.size(); i++) {
			hex_surface_object_class *hex_surface_object = hex_surface_entities[i];

			if (hex_surface_object->active_object) {
//printf("hex_animation_functions : define_active_animation_hex_surface 000\n");
				hex_surface_objects_to_execute.push_back(hex_surface_object);
			}
		}

		if (hex_surface_objects_to_execute.size() == 0) return false;

		return true;
	}

	bool define_all_animation_hex_surface(hex_surface_scene_objects_class &hex_surface_entities) {
		for (index_type i = 0; i < hex_surface_entities.size(); i++) {
			hex_surface_object_class* hex_surface_object = hex_surface_entities[i];
			hex_surface_objects_to_execute.push_back(hex_surface_object);
		}

		if (hex_surface_objects_to_execute.size() == 0) return false;

		return true;
	}

	void perform_animation_frame(animation_texture_model_parameters_struct_type &vw_animation_parameters,
								bool use_hex_texture_cb, bool use_hex_automata_cb, bool use_hex_automata_multi_thread_cb,
								int frame, int max_automata_step_value, int ANIMATION_STEP,
								scene_manager_class *scene_manager)
	{
	
//QMessageBox::information(NULL, "","in play animation 08 : " + std::string::number(frame), QMessageBox::Ok);
			// Define hex_surface function generation frame value for current animation frame
			for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
				hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_generator_parameters.frame = frame;
			}

//QMessageBox::information(NULL, "","in play animation09 " + std::string::number(frame), QMessageBox::Ok);
			// Define hex_surface texture generation frame value for current animation frame
			if (use_hex_texture_cb) {
//QMessageBox::information(NULL, "","in play animation09A use_hex_surface_texture_cb->isChecked() :" + std::string::number(hex_surface_objects_to_execute.size()), QMessageBox::Ok);
				for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
					hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters.frame = frame;
					//hex_surface_objects_to_execute[i]->object_renderer->object_ogl_shader->vertex_shader.shader_parameters.frame = frame; // Need a way to directly reference object parameters without copying them ****
				}
			}

//QMessageBox::information(NULL, "","in play animation10 " + std::string::number(frame), QMessageBox::Ok);
			// perform hex_surface function generation frame value for current animation frame
			// If automata animation is to be used and at the first frame to be animated or
			// If automata animation is not to be used
			if ((use_hex_automata_cb && frame == vw_animation_parameters.frame_step_start) || !use_hex_automata_cb) {
				for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
					//generate_animation_frame(i, frame, vw_animation_parameters, ANIMATION_STEP);
					generate_animation_frame(i, frame, vw_animation_parameters, ANIMATION_STEP, scene_manager, hex_surface_objects_to_execute[i]->object_id);
				}
			}

//printf("hex_surface_animation_functions_class :: perform_animation_frame:: 011 : %i\n",frame);
			// Perform hex_surface automata rules on current frame step if that frame is less than the maximum permited frame of the automata rules
			if (use_hex_automata_cb) {
				for (int i = 0; i < hex_surface_objects_to_execute.size();i++) {
					if (hex_surface_objects_to_execute[i]->animate_automata && hex_surface_objects_to_execute[i]->automata_rule_within_frame(frame)) { // ***
						if (!perform_hex_surface_automata_rules(frame, hex_surface_objects_to_execute[i], use_hex_automata_multi_thread_cb))
							break;
					} else {
						generate_animation_frame(i, frame, vw_animation_parameters, ANIMATION_STEP, scene_manager,hex_surface_objects_to_execute[i]->object_id);// if hex object not designated to animate automata rules, generate normal animation frame
					}
				}
			}

//printf("hex_surface_animation_functions_class :: perform_animation_frame:: 012 : %i\n",frame);
			// perform hex_surface texture generation frame value for current animation frame
			if (use_hex_texture_cb) {
				for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
					if (hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters.animate_shaders) {
						update_hex_surface_texture_parameter_step_values(hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters, ANIMATION_STEP, scene_manager, hex_surface_objects_to_execute[i]->object_id);
					}
				}
			}
//printf("hex_surface_animation_functions_class :: perform_animation_frame:: 013 : %i\n",frame);

	}

	// #######################################################
	//void generate_animation_frame(int index, int frame, animation_texture_model_parameters_struct_type &vw_animation_parameters, int ANIMATION_STEP) {
	void generate_animation_frame(int index, int frame, animation_texture_model_parameters_struct_type& vw_animation_parameters, int ANIMATION_STEP, scene_manager_class* scene_manager, id_type entity_id) {
//QMessageBox::information(NULL, "","in play animation10A " + std::string::number(hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_generator_parameters.variables[0].value), QMessageBox::Ok);
		update_hex_surface_generator_parameter_step_values(hex_surface_objects_to_execute[index]->hex_surface_object_data.hex_surface_generator_parameters, ANIMATION_STEP, scene_manager, entity_id);
//QMessageBox::information(NULL, "","in play animation10C " + std::string::number(frame), QMessageBox::Ok);
//printf("hex_surface_animation_functions_class :: generate_animation_frame::11\n");
		if (frame == vw_animation_parameters.frame_step_start) {
//printf("hex_surface_animation_functions_class :: generate_animation_frame::22\n");
			hex_surface_generators[index]->generate_hex_surface_function();
			//hex_surface_objects_to_execute[index]->update_renderer_shader_variables();
		} else {
//printf("hex_surface_animation_functions_class :: generate_animation_frame::33\n");
			hex_surface_generators[index]->update_hex_surface_generation();
		}

//printf("hex_surface_animation_functions_class :: generate_animation_frame::44\n");
		hex_surface_objects_to_execute[index]->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("hex_surface_animation_functions_class :: generate_animation_frame::55\n");
	}
	// #######################################################

	void restore_initial_frame(bool use_hex_texture_cb, scene_manager_class *scene_manager) {
		if (hex_surface_objects_to_execute.size() > 0 ) {
//QMessageBox::information(NULL, "","restore_initial_frame 00", QMessageBox::Ok);
			if (hex_surface_objects_to_execute.size() < 1 || vw_initial_hex_surface_generator_variables_array.size() != hex_surface_objects_to_execute.size()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("INFO : editor_animation_widget_class::Initial hex surface animation objects have changed in number\n");
				return;
			}
//QMessageBox::information(NULL, "","restore_initial_frame 01", QMessageBox::Ok);
			if (use_hex_texture_cb) {
				if (vw_initial_hex_surface_shader_generator_variables_array.size() != hex_surface_objects_to_execute.size()) {
					if (log_panel != NULL) log_panel->application_log.AddLog("INFO : editor_animation_widget_class:: Could not restore initial frame conditions for object shader variables.\n  Current objects added or deleted or variables not used in initial frame \n");
				}
				else {
					for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
						hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters = vw_initial_hex_surface_shader_generator_variables_array[i];
						hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters.frame = 0;
						update_shader_variables(scene_manager, hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters, hex_surface_objects_to_execute[i]->object_id);
						//hex_surface_objects_to_execute[i]->object_renderer->object_ogl_shader->vertex_shader.shader_parameters = hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_shader_parameters; // Need a way to directly reference object parameters without copying them ****
					}
				}
			}
//QMessageBox::information(NULL, "","restore_initial_frame 02", QMessageBox::Ok);
			// restore original voxel genereation
			for (int i = 0; i < hex_surface_objects_to_execute.size(); i++) {
				hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_generator_parameters = vw_initial_hex_surface_generator_variables_array[i];
				hex_surface_objects_to_execute[i]->hex_surface_object_data.hex_surface_generator_parameters.frame = 0;

				hex_surface_generators[i]->generate_hex_surface_function();

				hex_surface_objects_to_execute[i]->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
			}
		}
	}

		// !!!!!!!!!!!!!!!!!!! HEX SURFACE UPDATES !!!!!!!!!!!!!!!!!!

	void update_hex_surface_generator_parameter_step_values(hex_surface_generator_parameters_struct_type &hex_surface_generator_parameters, int step_action, scene_manager_class* scene_manager, id_type entity_id) {
		// Get the scene render object that stores the object render properties
		//hex_surface_object_class *hex_surface_object = scene_manager->get_hex_surface_entity_object(entity_id);
		hex_surface_object_class *hex_surface_object = (hex_surface_object_class*)scene_manager->get_entity_object(entity_id, ENTITY_CATEGORY_HEX_SURF);
		
		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update hex surface shaders for voxel object of id %i\n", entity_id);
//printf("update_hex_surface_generator_parameter_step_values::update_voxel_generator_parameter_step_values :entity_render_object == NULL\n");
			return;
		}
//printf("update_hex_surface_generator_parameter_step_values::11\n");

		int i = 0;
		for (hex_surface_generator_parameter_variable_struct_type variable : hex_surface_generator_parameters.variables) {
			if (variable.active_variable) {
				if (variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						variable.value = variable.value - variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							variable.value = variable.value + variable.variable_step;

					hex_surface_generator_parameters.variables[i].value = variable.value;
//printf("update_voxel_generator_parameter_step_values::AAA %f :: %f\n", voxel_generator_parameters.variables[i].value, voxel_hcp_object->voxel_object_data.voxel_generator_parameters.variables[i].value);
				}
			}
			i++;
		}

		i = 0;
		for (hex_surface_generator_parameter_int_variable_struct_type int_variable : hex_surface_generator_parameters.int_variables) {
			if (int_variable.active_variable) {
				if (int_variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						int_variable.value = int_variable.value - int_variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							int_variable.value = int_variable.value + int_variable.variable_step;

					hex_surface_generator_parameters.int_variables[i].value = int_variable.value;
				}
			}
			i++;
		}

//printf("update_voxel_generator_parameter_step_values::22\n");

//printf("update_voxel_generator_parameter_step_values::22 \n");
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables     = hex_surface_generator_parameters.variables;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables = hex_surface_generator_parameters.int_variables;
	}

	//void update_hex_surface_texture_parameter_step_values(shader_parameters_struct_type &hex_surface_texture_parameters, int step_action, scene_manager_class* scene_manager, id_type entity_id) {
	void update_hex_surface_texture_parameter_step_values(material_struct_type &hex_surface_texture_parameters, int step_action, scene_manager_class* scene_manager, id_type entity_id) {
		int i = 0;

		// Floating number varaibles
		for (shader_parameter_variable_struct_type variable : hex_surface_texture_parameters.variables) {
			if (variable.active_variable) {
				if (variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						variable.value = variable.value - variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							variable.value = variable.value + variable.variable_step;

					hex_surface_texture_parameters.variables[i].value = variable.value;
				}
			}
			i++;
		}

		i = 0;
		// integer number varaibles
		for (shader_parameter_int_variable_struct_type int_variable : hex_surface_texture_parameters.int_variables) {
			if (int_variable.active_variable) {
				if (int_variable.active_variable_step) {
					if (step_action == DECREMENT_STEP)
						int_variable.value = int_variable.value - int_variable.variable_step;
					else
						if (step_action == INCREMENT_STEP)
							int_variable.value = int_variable.value + int_variable.variable_step;

					hex_surface_texture_parameters.int_variables[i].value = int_variable.value;
				}
			}
			i++;
		}

		update_shader_variables(scene_manager, hex_surface_texture_parameters, entity_id);
	}

	//void update_shader_variables(scene_manager_class *scene_manager, shader_parameters_struct_type hex_surface_texture_parameters, id_type entity_id) { // not sure this is needed as render node does this task
	void update_shader_variables(scene_manager_class *scene_manager, material_struct_type hex_surface_texture_parameters, id_type entity_id) { // not sure this is needed as render node does this task
//printf("hex_surface_animation_functions_class::update_shader_variables\n");// replace with update variables
		// Get the scene render object that stores the object render properties
		scene_node_class <render_object_class> *entity_render_object = scene_manager->get_render_object(entity_id);                  
		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
//printf("hex_surface_animation_functions_class::update_shader_variables :entity_render_object == NULL\n");
			return;
		}

		material_struct_type* shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties for the render process

		if (shader_material == NULL) {
			//printf("hex_surface_animation_functions_class::update_shader_variables 222A shader_material == NULL\n");
			return;
		}
		//else
//printf("hex_surface_animation_functions_class::update_shader_variables 222A shader_material != NULL\n");

//printf("hex_surface_animation_functions_class::update_shader_variables 222\n");

		shader_material->variables      = hex_surface_texture_parameters.variables;
		shader_material->int_variables  = hex_surface_texture_parameters.int_variables;
		shader_material->bool_variables = hex_surface_texture_parameters.bool_variables;
//printf("hex_surface_animation_functions_class::update_shader_variables 333\n");
	}

	bool perform_hex_surface_automata_step(u_long_int step, hex_surface_object_class *hex_surface_object) {
		hex_surface_automata_functions_class  hex_surface_automata_functions;

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::00 %i\n",step);

		if (hex_surface_object->hex_surface_object_data.hex_surface_matrix_data.size() == 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to perform cellula automata step : Have undefined generation of hex surface.\n");
			printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::00 \nERROR : Unable to perform cellula automata step : Have undefined generation of hex surface.\n");
			return false;
		}

		hex_surface_automata_functions.hex_surface_object_to_execute = hex_surface_object;// ****

		hex_surface_automata_functions.cloud                      = hex_surface_object;
		hex_surface_automata_functions.log_panel                  = log_panel;
		hex_surface_automata_functions.hex_surface_automata_rules = hex_surface_object->hex_surface_object_data.hex_surface_automata_rules;
//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::01 %i\n",step);
		if (!hex_surface_automata_functions.perform_hex_surface_automata_generation(step)) return false;


//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::02 %i\n",step);

		hex_surface_object->hex_surface_object_data.delete_hex_surface_matrix_data();

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::03 \n");

		hex_surface_object->hex_surface_object_data.hex_surface_matrix_data = hex_surface_automata_functions.hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_surface_matrix_data;

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::04 \n");
		hex_surface_object->define_vbo_vertices(MIN_HEX_VALUE, MAX_HEX_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_step::05 \n");
		return true;
	}
/*
	bool perform_hex_surface_automata_step_multithread(u_long_int step, hex_surface_object_class *hex_surface_object) {
		hex_surface_automata_hcp_thread_functions_class hex_surface_automata_thread_functions;
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_hex_surface_automata_step_multithread 00 : " + std::string::number(step));
//}

		if (hex_surface_object->hex_surface_object_data.hex_surface_matrix_data.size() == 0) {
			if (log_widget != NULL) {
				log_widget->log_message(log_display, log_message_type_enum_type::error, "Unable to perform cellula automata step : Have undefined generation of voxels.");
			}
			return false;
		}

		hex_surface_automata_thread_functions.cloud = hex_surface_object;
		hex_surface_automata_thread_functions.log_widget = log_widget;
		hex_surface_automata_thread_functions.hex_surface_automata_rules = hex_surface_object->hex_surface_object_data.hex_surface_automata_rules;

		if (!hex_surface_automata_thread_functions.perform_hex_surface_automata_generation(step)) return false;

//QMessageBox::information(NULL, "", "Time elapsed : "+ std::string::number(time.elapsed())+ " ms", QMessageBox::Ok);// testing only
//QMessageBox::information(NULL, "", "perform_hex_surface_automata_step BBB00", QMessageBox::Ok);

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_hex_surface_automata_step 01 : " + std::string::number(hex_surface_object->voxel_object_data.voxel_matrix_data.size()));
//}

		hex_surface_object->hex_surface_object_data.delete_hex_surface_matrix_data();
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_hex_surface_automata_step 02 : " + std::string::number(hex_surface_object->voxel_object_data.voxel_matrix_data.size()));
//}

		hex_surface_object->hex_surface_object_data.hex_surface_matrix_data = hex_surface_automata_thread_functions.hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_surface_matrix_data;
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_hex_surface_automata_step 03 : " + std::string::number(hex_surface_object->voxel_object_data.voxel_matrix_data.size()));
//}

		hex_surface_object->update_renderer_shader_variables();
		//hex_surface_object->define_vbo_vertices(MIN_HEX_VALUE, MAX_HEX_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
		hex_surface_object->update_vbo_point_cloud_vertex_data();// need to define values for min/max voxel value range or have incorrect to misleading display
		
		//hex_surface_object->update_buffer = true;

		return true;
	}
*/
	bool perform_hex_surface_automata_rules(int frame, hex_surface_object_class *hex_surface_object, bool use_voxel_automata_multi_thread) {
		//if (use_voxel_automata_multi_thread)
			//return perform_hex_surface_automata_step_multithread(frame, hex_surface_object);
		//else
			return perform_hex_surface_automata_step(frame, hex_surface_object);
	}

	// !!!!!!!!!!!!!!!!!!!!!!!! EXPORT MESH !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	
	// The following save mesh data is a modified implementation from the same save voxel mesh menu options
	// that exits for the Main window. In future for centralisation of this task, merge the two into one
	// class for greater meintenance ability. 

	//bool export_voxels_center_point_data(std::vector<entity_class *> &voxel_hcp_entities_to_execute,std::string saved_mesh_pathname, int frame, std::string file_format, vw_editor_viewer_class *fp_viewer) {
	bool export_hex_surface_center_point_data(std::string saved_mesh_pathname, int frame, std::string file_format) {
//QMessageBox::information(NULL, "","export_mesh", QMessageBox::Ok);
		export_hex_surface_geometry_class        export_hex_surface_geometry;
		export_hex_surface_geometry.log_panel = log_panel;

		if (hex_surface_objects_to_execute.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_hex_surface_center_point_data :\n Failed to export center point hex surface data. Could not define the hex data to execute.\n");
//QMessageBox::information(NULL, "","export_mesh : !define_voxel_hcp_entities_to_execute(voxel_hcp_entities_to_execute)", QMessageBox::Ok);
			return false;
		}

//QMessageBox::information(NULL, "","export_mesh AAA : " + std::string::number(voxel_hcp_entities_to_execute.size()), QMessageBox::Ok);
		for (index_type i = 0; i < hex_surface_objects_to_execute.size(); i++) {
			hex_surface_object_class *hex_surface_object = hex_surface_objects_to_execute[i];

//printf("export_voxels_center_point_data BBB : %s\n ", saved_mesh_pathname.c_str());

			if (hex_surface_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_center_point_data :\n Failed to export center point voxels data. Could not find voxel HCP component data\n");
				return false;
			}

			saved_mesh_pathname = saved_mesh_pathname + "_hcp_" + hex_surface_object->object_name + "_" + std::to_string(frame) + file_format;

//printf("export_voxels_center_point_data CCC : %s\n ", saved_mesh_pathname.c_str());
			export_hex_surface_geometry.export_hex_surface_center_points_to_file_ply(hex_surface_object->hex_surface_object_data, saved_mesh_pathname);
		}

		return true;

	}

		bool export_hex_surface_face_surface_data(std::string saved_mesh_pathname, int frame, std::string file_format) {
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "In export_hex_surfaces_surface_data", QMessageBox::Ok);
		export_hex_surface_geometry_class export_hex_surface_geometry;
		export_hex_surface_geometry.log_panel = log_panel;

		if (hex_surface_objects_to_execute.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_hex_surfaces_face_surface_data :\n Failed to export face surface hex_surfaces data. Could not define the HCP data to execute.\n");
//QMessageBox::information(NULL, "","export_mesh : !define_hex_surface_hcp_entities_to_execute(hex_surface_hcp_entities_to_execute)", QMessageBox::Ok);
			return false;
		}

		for (index_type i = 0; i < hex_surface_objects_to_execute.size(); i++) {
			hex_surface_object_class *hex_surface_object= hex_surface_objects_to_execute[i];

			saved_mesh_pathname = saved_mesh_pathname + "_hssf_" + hex_surface_object->object_name + "_" + std::to_string(frame) + file_format;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class ::export_voxels_center_point_data : "+ saved_mesh_pathname);
//}
			if (hex_surface_object == NULL) {

				if (log_panel != NULL) log_panel->application_log.AddLog("DEBUG : editor_animation_widget_class : export_voxels_center_point_data :\n Failed to export center point voxels data. Could not find voxel HCP component data\n");

//QMessageBox::information(NULL, "","export_mesh : voxel_hcp_object_to_execute == NULL : " + entity->entity_name, QMessageBox::Ok);
				return false;
			}

			export_hex_surface_geometry.export_hex_surface_faces_ply(saved_mesh_pathname, hex_surface_object->hex_surface_object_data);
		}

		return true;
	}
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

private:
	//log_display_struct_type log_display;

};