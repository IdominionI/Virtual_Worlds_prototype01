#include "hex_generator_widget.h"

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Interface/IconsFontAwesome.h> //*****

#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>


void hex_surface_generation_widget_class::display() {
		if (hex_surface_object_to_execute == NULL) {
			return;
		}

		const char* invocation_items[] = { "1", "32", "64", "128", "256", "512", "1024" };// This cannot be defined outside this function	
		float x_pos = 10.0f, y_pos = 260.0f;

		text("Expression : ", x_pos, y_pos);
		if (ex_button(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name.c_str(), x_pos + 120, y_pos, 150, 20))
			get_expression_file();

		y_pos += 25;
		text("Start             End", x_pos + 80, y_pos);

		x_pos = 20; y_pos += 20;

		text("X-axis", x_pos, y_pos);
		float_input("###hgxstart", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_start, x_pos + 60, y_pos, 75.0f);
		float_input("###hgxend", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###hgystart", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_start, x_pos + 60, y_pos, 75.0f);
		float_input("###hgyend", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 30;
		text("Compute Invocation :", x_pos, y_pos);

		ImGui::SetCursorPosX(x_pos + 170);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(75);
		if (ImGui::Combo("###hinvocation", &invocation, &Funcs::ItemGetter, invocation_items, IM_ARRAYSIZE(invocation_items))) {
			define_voxel_generation_parameters();
		}

		y_pos += 30;
		if (ex_button("Execute Function", x_pos + 70, y_pos, 150, 20))
			execute_hex_surface_function();


		y_pos += 30;
		text("Min\nSurface\nValue", x_pos, y_pos);
		//integer_input("###gminsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);
		float_input("###gminsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);

		text("Max\nSurface\nValue", x_pos + 150, y_pos);
		//integer_input("###hgmaxsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);
		float_input("###hgmaxsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);

		y_pos += 60;
		text("Resolution Step", x_pos + 30, y_pos);
		float_input("###hgresstep", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.resolution_step, x_pos + 160, y_pos, 75.0f);

		y_pos += 20;
		text("Hex Generattion Variables", x_pos + 50, y_pos);

		ImGui::BeginChild("outer_child", ImVec2(320,300.0f), true);

			hex_surface_generator_variables_widget_class.variables_manager(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters);

		ImGui::EndChild();

		y_pos += 330;

		if (ex_button(ICON_FA_STEP_BACKWARD "###hgd", x_pos + 50, y_pos, 50, 20))
			perform_decrement_variables();

		//if (ex_button(ICON_FA_EJECT "###gu", x_pos + 110, y_pos, 80, 20)) // Not needed but just in case
		//	printf("update button clicked");// replace with update variables

		if (ex_button(ICON_FA_STEP_FORWARD "###hgi", x_pos + 200, y_pos, 50, 20))
			perform_increment_variables();

		y_pos += 30;

		if (ex_button("Clear Variables###gc", x_pos + 85, y_pos, 140, 20))
			clear_variables();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###hgs", x_pos + 35, y_pos, 100, 20))
			save_generation_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###hgdl", x_pos + 165, y_pos, 100, 20))
			load_generation_parameters();

	}

	void hex_surface_generation_widget_class::perform_decrement_variables() {
//printf("perform_decrement_variables button clicked\n");// replace with decrement step
		for (hex_surface_generator_parameter_variable_struct_type &variable : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.variables) {
			if (variable.active_variable_step) variable.value -= variable.variable_step;
		}

		for (hex_surface_generator_parameter_int_variable_struct_type &int_variable : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.int_variables) {
			if (int_variable.active_variable_step) int_variable.value -= int_variable.variable_step;
		}

		execute_hex_surface_function();
	}

	void hex_surface_generation_widget_class::perform_increment_variables() {
//printf("perform_increment_variables button clicked\n");// replace with decrement step
		for (hex_surface_generator_parameter_variable_struct_type &variable : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.variables) {
			if (variable.active_variable_step) variable.value += variable.variable_step; 
		}

		for (hex_surface_generator_parameter_int_variable_struct_type &int_variable : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.int_variables) {
			if (int_variable.active_variable_step) int_variable.value += int_variable.variable_step;
		}

		execute_hex_surface_function();
	}

	void hex_surface_generation_widget_class::execute_hex_surface_function(bool notification) {
//printf("Execute Function button clicked\n");

		//####### GET  OBJECT DATA THAT HAS PARAMETER DATA AND UPDATE #######

		//editor_logging_panel.add_log_message("INFO","Execute Buttton 000 : " + voxel_generation_item.current_selected_component_name);
		if (current_selected_entity_id   == -1){
//printf("hex_surface_generation_widget_class :: current_selected_entity_id   == -1 #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}
		if (hex_surface_object_to_execute == NULL) { 
//printf("hex_surface_generation_widget_class :: hex_surface_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}
//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 1111\n");
		define_voxel_generation_parameters(); // Only need to define invocation values
//editor_logging_panel.add_log_message("INFO","Execute Buttton 111 : " + voxel_generation_item.current_selected_component_name);
		// set voxel_generation object id to execute funtion data on
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters;
		hex_surface_compute_generator.cloud     = hex_surface_object_to_execute;
		hex_surface_compute_generator.log_panel = log_panel;

// *************** test code only :: Delete when finished ***************************
//test_hex_hexagon_cell_coord_from_cartesian(hex_surface_object_to_execute->hex_surface_object_data); // ***********

//printf("hex_surface_generation_widget_class :: execute_hex_surface_function %s 2222\n", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname.c_str());
		if (!hex_surface_compute_generator.generate_hex_surface_function()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Create hex surface function of %s failed.\n", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name.c_str());
			return;
		}
//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 3333\n");

// *************** test code only :: Delete when finished ***************************
//test_hex_hexagon_cell_coord_from_cartesian(hex_surface_object_to_execute->hex_surface_object_data); // ***********

		hex_surface_object_to_execute->define_vbo_vertices(MIN_HEX_SURF_VALUE, MAX_HEX_SURF_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 4444\n");

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		scene_node_class <render_object_class> *scene_hex_surface_object = scene_manager->get_render_object(current_selected_entity_id);
//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 5555\n");

		if (scene_hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find hex surface in the scene to update geometry data.\n");
//printf("scee_hex_surface_object == NULL.\n");
		}
		else {
//printf("scee_hex_surface_object != NULL.\n");

			if (!hex_surface_render.update_geometry_vertex_cloud_data(&hex_surface_object_to_execute->point_cloud, scene_hex_surface_object, log_panel)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene hex surface object geometry could not be updated.\n");
//printf("scee_hex_surface_object not updated\n");
				return;
			}

			shader_class shader;

			// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default
			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;

			shader.set_f1(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, vox_size, "hexSize");

			glm::vec3 grid_o = { hex_surface_object_to_execute->hex_surface_object_data.grid_origin.x,hex_surface_object_to_execute->hex_surface_object_data.grid_origin.y,0.0 };

			shader.set_vec3(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, grid_o, "grid_origin");

			//voxel surface display data
			float v_min = (float)hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value;

			shader.set_f1(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, v_min, "hex_min_surface_display_value");

			float v_max = (float)hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value;

			shader.set_f1(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, v_max, "hex_max_surface_display_value");

//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 6666\n");
		}

		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");

	}


	void hex_surface_generation_widget_class::initialise_parameters() {
		initialise_invocation();
	}

	void hex_surface_generation_widget_class::initialise_invocation() {
		switch (hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation) {
			case 1    : invocation = 0; break;
			case 32   : invocation = 1; break;
			case 64   : invocation = 2; break;
			case 128  : invocation = 3; break;
			case 256  : invocation = 4; break;
			case 512  : invocation = 5; break;
			case 1024 : invocation = 6; break;
			default   : invocation = 7; break;
		}
	}
/*
	// test_hex_hexagon_cell_coord_from_cartesian for testing only. Delete when complete
	void hex_surface_generation_widget_class::test_hex_hexagon_cell_coord_from_cartesian(hex_surface_object_data_class hex_surface_object_data) {
		printf("test_hex_hexagon_cell_coord_from_cartesian()000\n");
		glm::vec2 points[10];

		points[0] = { 6.95,-0.5 };
		points[1] = { 6.95,-0.57 };
		points[2] = { 6.95,-0.6 };
		points[3] = { 7.05,0.5 };
		points[4] = { 7.05,0.57 };
		points[5] = { 7.05,0.6 };
		points[6] = { 7.05,-.5 };
		points[7] = { 7.05,-0.57 };
		points[8] = { 7.05,-0.6 };
		points[9] = { 6.95,-2.0 };

		printf("test_hex_hexagon_cell_coord_from_cartesian()111\n");
		printf("Origin : x %f : y %f \n", hex_surface_object_data.grid_origin.x, hex_surface_object_data.grid_origin.y);
		printf("grid dim : x %i : y %i \n", hex_surface_object_data.grid_dimension.x, hex_surface_object_data.grid_dimension.y);
		printf("hex size : %f\n", hex_surface_object_data.hex_size);

		//index_vector3 iv;
		glm::ivec3 iv;
		for (int i = 0; i < 10; i++) {
			iv= hex_surface_object_data.hexagon_cell_coord_from_cartesian(points[i].x, points[i].y);
			printf("point %i x : %f y : %f :: hexagon_cell_coord_from_cartesian  x : %i y : %i \n",i, points[i].x, points[i].y, iv.x, iv.y);
		}
	}
*/
//------------------------------------------

	void hex_surface_generation_widget_class::define_voxel_generation_parameters() {
		switch (invocation) {
			case 0 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 1;    break;
			case 1 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 32;   break;
			case 2 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 64;   break;
			case 3 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 128;  break;
			case 4 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 256;  break;
			case 5 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 512;  break;
			case 6 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 1024; break;
			default :hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 256;  break;
		}
	}

	void hex_surface_generation_widget_class::get_expression_file() {

//printf("hex surface : Expression file button clicked");// replace with get file pathname tool
		char const* patterns[] = { "*_EXPR.txt" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file selected to perform hex surface generation\n");
			return;
		}else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = file_pathname;
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");

	}
	
	void hex_surface_generation_widget_class::save_generation_parameters() {

//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = {"*.hgp"};
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to save voxel generation parameter data to\n");
			return;
		} else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("save_generation_parameters != NULL  \n");

		switch (invocation) {
			case 0 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 1;    break;
			case 1 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 32;   break;
			case 2 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 64;   break;
			case 3 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 128;  break;
			case 4 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 256;  break;
			case 5 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 512;  break;
			case 6 : hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 1024; break;
			default :hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation = 256;  break;
		}

		hex_surface_function_import_export.hex_surface_generator_parameters = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters;

		hex_surface_function_import_export.save_hex_surface_generated_function(file_pathname);
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data saved to file\n %s\n", file_pathname);

	}

	void hex_surface_generation_widget_class::load_generation_parameters() {

//printf("load button clicked\n");// replace with clear variables
		clear_variables();

		char const* patterns[] = { "*.hgp" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import hex surface generation parameter data from.\n");
			return;
		}
		else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("load_generation_parameters != NULL  \n");

		hex_surface_function_import_export.import_hex_surface_genereated_function(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters, file_pathname);

		switch (hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation) {
			case 1    : invocation = 0; break;
			case 32   : invocation = 1; break;
			case 64   : invocation = 2; break;
			case 128  : invocation = 3; break;
			case 256  : invocation = 4; break;
			case 512  : invocation = 5; break;
			case 1024 : invocation = 6; break;
			default   : invocation = 7; break;
		}
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion hex surface generation parameter data imported from file\n %s\n", file_pathname);

	}

	void hex_surface_generation_widget_class::clear_variables() {
//printf("Clear Variables clicked");// replace with clear variables
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.variables.clear();
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.int_variables.clear();
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.clear();
	}
