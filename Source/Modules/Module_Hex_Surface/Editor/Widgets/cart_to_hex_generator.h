#pragma once

#include <random>

#include <Universal/ImGui/imgui.h>
#include <Universal/3rdparty/include/assimp/Importer.hpp>
#include <Universal/3rdparty/include/assimp/postprocess.h>
#include <Universal/3rdparty/include/assimp/scene.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

#include "../../hex_surface_object/DataTypes/dt_hex_surface_generator.h"
#include "../../Kernal/hex_surface_function_import_export.h"

//#include "generator_variables_widget.h"
//#include "../../Compute/hex_surface_compute_generation.h"


class cart_to_hex_widget_class {
public:
	//generator_variables_widget_class  generator_variables_widget_class;
	log_panel_class* log_panel = NULL;

	id_type                   current_selected_entity_id = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class      *scene_manager = NULL;

	void display() {
		if (hex_surface_object_to_execute == NULL) {
			return;
		}

		const char* random_items[] = { "function","square", "Circle"};// This cannot be defined outside this function	

		float x_pos = 10.0f, y_pos = 260.0f;

		text("PLY File : ", x_pos, y_pos);
		if (ex_button(ply_file_name + "###ctovb01", x_pos + 120, y_pos, 150, 20))
			get_ply_file();

		y_pos += 25;

		//text("Expression : ", x_pos, y_pos);
		//if (ex_button(expr_file_name+"###ctovb02", x_pos + 120, y_pos, 150, 20))
		//	get_expression_file();

		text("Random Generaton :", x_pos, y_pos);

		ImGui::SetCursorPosX(x_pos + 170);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(75);
		ImGui::Combo("###ctovrgen", &random_type, &Funcs::ItemGetter, random_items, IM_ARRAYSIZE(random_items));

		y_pos += 25;
		text("Start             End", x_pos + 80, y_pos);

		x_pos = 20; y_pos += 20;

		text("X-axis", x_pos, y_pos);
		float_input("###chxstart", min_x, x_pos + 60, y_pos, 75.0f);
		float_input("###chxend", max_x, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###chystart", min_y, x_pos + 60, y_pos, 75.0f);
		float_input("###chyend", max_y, x_pos + 190.0f, y_pos, 75.0f);


		y_pos += 30;
		text("Coordinate Generation", x_pos + 65, y_pos);
		y_pos += 25;
		text("ply file", x_pos + 60, y_pos);
		text("Function", x_pos + 180, y_pos);
		y_pos += 25;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###hpcrb", &generation_type, 0);
		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###hfsrb", &generation_type, 1);

		y_pos += 30;
		text("Ply Average z :", x_pos + 30, y_pos);

		ImGui::SetCursorPosX(x_pos + 220);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###havez", &display_average_z_value);

		y_pos += 30;
		text("Number points", x_pos + 30, y_pos);
		integer_input("###cthnpoints", number_of_points, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		text("Scale/radius", x_pos + 30, y_pos);
		float_input("###cthtrad", ctov_scale, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Define Cartesian Data", x_pos + 60, y_pos, 180, 20))
			define_cartesian_data();

		y_pos += 60;
		text("Hex Size", x_pos + 30, y_pos);
		float_input("###cthhsize", hex_size, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Generate Grid", x_pos + 70, y_pos, 150, 20))
			cartesian_to_voxel();
/*
		y_pos += 30;
		text("Hex Scale", x_pos + 130, y_pos);

		y_pos += 20;
		if (float_min_max_slider("hs", x_pos, y_pos, 200.0f, min_vscale, max_vscale, hex_scale_value, 50.0f)) {
			update_voxel_size();
		}
*/
		y_pos += 30;
		text("Data Range", x_pos, y_pos);
		integer_input("###cthmindr", min_data_value, x_pos + 100, y_pos, 45.0f);
		integer_input("###cthmaxdr", max_data_value, x_pos + 190, y_pos, 45.0f);

	}

	void define_cartesian_data() {
		if (generation_type == 0)
			import_ply_file_to_cartesian_array();
		else
			generate_cartesian_array();

		define_default_hex_limits();
	}

	void import_ply_file_to_cartesian_array() {
		if (FW::filetools::file_exists(ply_file_pathname)) {
			if (!import_file(ply_file_pathname)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not import file %s\n", ply_file_pathname);
				return;
			}
			else
				if (log_panel != NULL) log_panel->application_log.AddLog("INFO : File %s imported successfuly\n", ply_file_pathname);
		}
		else
			printf("File of path %s does not exist\n", ply_file_pathname.c_str());
	}

	void generate_cartesian_array() {
		if (number_of_points <= 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have specified zero points to generate cartesian data\n");
			return;
		}

		if (ctov_scale <= 0.0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have test radius invalid value\n");
			return;
		}

		float x_range = max_x - min_x;
		float y_range = max_y - min_y;

		if (x_range == 0 && y_range == 0 ) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have one or more of the data ranges to generate cartesian data array equal to zero\n");
			return;
		}

		glm::vec3 r_coord;

		cartesian_point_data.reset();
		/*
				// folowing for testing only
				for (int i = -10; i < 10; i++) {
					//for (int j = -10; j < 10; j++) {
						int inc = 2;
						for (int w = 0; w < inc; w++) {
							r_coord.x = float(i) + float(w)/float(inc);
							//r_coord.y = float(j) + float(w) / float(inc);
							r_coord.y = 0.0;
							r_coord.z = 0.0;

							cartesian_point_data.add_coordinate(r_coord);
						}
					//}
				}
		*/

		// use srand() for different outputs
		srand(time(0));

		for (int i = 0; i < number_of_points; i++) {
			switch (random_type) {
				case 0:  r_coord = xy_function();						  break;
				case 1:  r_coord = random_xy(min_x, max_x, min_y, max_y); break;
				case 2:  r_coord = rnd_circle();						  break;
			default: r_coord = random_xy(min_x, max_x, min_y, max_y);	  break;
			}
			cartesian_point_data.add_coordinate(r_coord);
		}

	}

	glm::vec3 xy_function() {
		glm::vec3 coord;

		coord = random_xy(min_x,max_x,min_y,max_y);
		float r = sqrt(coord.x * coord.x + coord.y * coord.y);
		coord.z = (sin(r) / r) * ctov_scale;

		return coord;
	}

	glm::vec3 rnd_circle() {
		glm::vec3 coord;

		coord = random_xy(min_x, max_x, min_y, max_y);
		float r = sqrt(coord.x * coord.x + coord.y * coord.y);

		if (r <= ctov_scale) 
			return coord;
		else
			return { 0.0f,0.0f,0.0f };
	}


	glm::vec3 random_xy(float min_x,float max_x, float min_y, float max_y,float scale = 1.0f) {
		glm::vec3 xy_coord;

		float x_range = max_x - min_x;
		float y_range = max_y - min_y;

		xy_coord.x = (min_x + ((float)(std::rand()) / (float)RAND_MAX) * x_range) * scale;
		xy_coord.y = (min_y + ((float)(std::rand()) / (float)RAND_MAX) * y_range) * scale;
		xy_coord.z = 0.0f;

		return xy_coord;
	}

	void define_default_hex_limits() {
		min_x = cartesian_point_data.min_x, max_x = cartesian_point_data.max_x;
		min_y = cartesian_point_data.min_y, max_y = cartesian_point_data.max_y;
		//min_z = cartesian_point_data.min_z, max_z = cartesian_point_data.max_z;
	}

	void define_generation_parameters() {
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_start = min_x;
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_end   = max_x;
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_start = min_y;
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_end   = max_y;

		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.resolution_step = hex_size;
	}


	void cartesian_to_voxel(bool notification = true) {
		printf("cart_to_voxel_widget_class :: in cartesian_to_voxel\n");

		if (current_selected_entity_id == -1) {
			//printf("cart_to_voxel_widget_class :: current_selected_entity_id   == -1 #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}
		if (hex_surface_object_to_execute == NULL) {
			//printf("cart_to_voxel_widget_class :: hex_surface_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}

		define_generation_parameters();
		hex_surface_object_to_execute->hex_surface_object_data.create_hex_grid();
		int matrix_size = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data.size();

//		index_vector3 matrix_dimension = hex_surface_object_to_execute->hex_surface_object_data.grid_dimension; // testing only
//printf("cart_to_voxel_widget_class :: cartesian_to_voxel x_dim %i : y_dim  %i : z_dim %i\n", matrix_dimension.x, matrix_dimension.y, matrix_dimension.z);
		for (glm::vec3 coordinate : cartesian_point_data.coordinates) {
			int index = hex_surface_object_to_execute->hex_surface_object_data.index_of_hex_cell_with_cartesian_coord(coordinate.x, coordinate.y);

//printf("cart_to_voxel_widget_class :: INDEX !!!!  %i\n", index);
			if (index > -1 && index < matrix_size) {
				if (display_average_z_value && generation_type ==0 || display_average_z_value && (generation_type == 1 && random_type == 0)) {
					float voxel_av_z_value = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data[index];
					voxel_av_z_value = (voxel_av_z_value + coordinate.z) / 2.0f;
					hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data[index] = voxel_av_z_value;
				} else
					hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data[index]++;// add one to number of points in the voxel cell
			}
		}
//printf("cart_to_voxel_widget_class :: execute_voxel_function 3333\n");

		hex_surface_object_to_execute->define_vbo_vertices(min_data_value, max_data_value);// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("cart_to_voxel_widget_class :: execute_voxel_function 4444\n");

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

			// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default
			application_default_shader_uniform_variables_struct_type uniform_variable;
			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "hexSize"; uniform_variable.value0 = &vox_size;
			scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			uniform_variable.type = application_default_shader_variable_type_enum::Floatv3; uniform_variable.name = "grid_origin"; uniform_variable.value0 = &hex_surface_object_to_execute->hex_surface_object_data.grid_origin;
			scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			//voxel surface display data
			float v_min = (float)hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "hex_min_surface_display_value"; uniform_variable.value0 = &v_min;
			scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			float v_max = (float)hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "hex_min_surface_display_value"; uniform_variable.value0 = &v_max;
			scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

//printf("hex_surface_generation_widget_class :: execute_hex_surface_function 6666\n");
		}


		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");


	}

private:
	float ctov_scale = 1.0f; // testing only

	int  number_of_points = 1000;
	bool display_average_z_value  = false;

	int random_type     = 0;
	int generation_type = 1;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

	string_type   ply_file_pathname = "";
	string_type   ply_file_name = "...";
	//string_type   expr_file_pathname = "";
	//string_type   expr_file_name     = "...";

	float min_x = -10.0f, max_x = 10.0f;
	float min_y = -10.0f, max_y = 10.0f;

	float hex_size        = 1.0f;
	float hex_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	int min_data_value = 1, max_data_value = 100;

	hex_surface_render_class   hex_surface_render;
/*
	void update_voxel_size() {
		scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);

		if (scene_voxel_object != NULL) {
			application_default_shader_uniform_variables_struct_type uniform_variable;
			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxSize"; uniform_variable.value0 = &vox_size;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
		}
	}
*/
	void get_ply_file() {
		char const* patterns[] = { "*.ply","*.obj","*.stl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No .ply file selected to perform voxel generation\n");
			return;
		}
		else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		ply_file_pathname = file_pathname;
		ply_file_name = vwDialogs::get_filename(file_pathname, "\\");
	}
	/*
		void get_expression_file() {
	printf("Expression file button clicked");// replace with get file pathname tool
			char const* patterns[] = { "*_C3D_EXPR.txt" };
			char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

			if (file_pathname == nullptr) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file selected to perform voxel generation\n");
				return;
			}else
				printf("expression_file_pathname != NULL %s \n", file_pathname);

			//hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = file_pathname;
			//hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");
		}
	*/
	// #########################################################

	struct hex_cartesian_point_data_struct_type {
		float min_x = 0.0f, max_x = 0.0f;
		float min_y = 0.0f, max_y = 0.0f;
		float min_z = 0.0f, max_z = 0.0f;

		std::vector<glm::vec3> coordinates;

		void add_coordinate(glm::vec3 coordinate) {
			if (coordinate.x < min_x) min_x = coordinate.x;
			if (coordinate.x > max_x) max_x = coordinate.x;
			if (coordinate.y < min_y) min_y = coordinate.y;
			if (coordinate.y > max_y) max_y = coordinate.y;
			if (coordinate.z < min_z) min_z = coordinate.z;
			if (coordinate.z > max_z) max_z = coordinate.z;

			coordinates.push_back(coordinate);
		}

		void reset() {
			coordinates.clear();
			coordinates.shrink_to_fit();
			min_x = 0.0f; max_x = 0.0f;
			min_y = 0.0f; max_y = 0.0f;
			min_z = 0.0f; max_z = 0.0f;
		}
	};

	hex_cartesian_point_data_struct_type cartesian_point_data;


	// #########################################################

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bool import_file(const std::string& filepath) {
		const uint32_t cMeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_ValidateDataStructure;

		Assimp::Importer Importer;

		const aiScene* pScene = Importer.ReadFile(filepath.c_str(),
			cMeshImportFlags);

		if (pScene && pScene->HasMeshes())
		{
			cartesian_point_data.reset();

			auto* mesh = pScene->mMeshes[0];

			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec3 mPos;
				if(display_average_z_value)
					mPos = { mesh->mVertices[i].x * ctov_scale, mesh->mVertices[i].y * ctov_scale ,mesh->mVertices[i].z * ctov_scale };
				else
					mPos = { mesh->mVertices[i].x * ctov_scale, mesh->mVertices[i].y * ctov_scale ,0.0f};

				cartesian_point_data.add_coordinate(mPos);
			}
			return true;
		}

		return false;
	}


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

};