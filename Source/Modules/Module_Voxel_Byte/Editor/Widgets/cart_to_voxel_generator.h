#pragma once

#include <Universal/3rdparty/include/glm/gtc/random.hpp>

#include <Universal/ImGui/imgui.h>
#include <Universal/3rdparty/include/assimp/Importer.hpp>
#include <Universal/3rdparty/include/assimp/postprocess.h>
#include <Universal/3rdparty/include/assimp/scene.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

#include "../../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "generator_variables_widget.h"
#include "../../Compute/voxel_compute_generation.h"


class cart_to_voxel_widget_class {
public:
	generator_variables_widget_class  generator_variables_widget_class;
	log_panel_class* log_panel = NULL;

	id_type                 current_selected_entity_id = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class    *scene_manager               = NULL;

	void display() {
		if (voxel_hcp_object_to_execute == NULL) {
			return;
		}

		const char *random_items[] = { "Ball", "Sphere" };// This cannot be defined outside this function	

		float x_pos = 10.0f, y_pos = 70.0f;

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
		float_input("###cvxstart", min_x, x_pos + 60, y_pos, 75.0f);
		float_input("###cvxend",   max_x, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###cvystart", min_y, x_pos + 60, y_pos, 75.0f);
		float_input("###cvyend",   max_y, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Z-axis", x_pos, y_pos);
		float_input("###cvzstart", min_z, x_pos + 60, y_pos, 75.0f);
		float_input("###cvzend",   max_z, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 30;
		text("Coordinate Generation", x_pos + 65, y_pos);
		y_pos += 25;
		text("ply file", x_pos + 60, y_pos);
		text("Function", x_pos + 180, y_pos);
		y_pos += 25;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###ampcrb", &generation_type, 0);
		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###amfsrb", &generation_type, 1);

		y_pos += 30;
		text("Number points", x_pos + 30, y_pos);
		integer_input("###ctvnpoints", number_of_points, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		text("Scale/radius", x_pos + 30, y_pos);
		float_input("###ctvtrad", ctov_scale, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Define Cartesian Data", x_pos + 60, y_pos, 180, 20))
			define_cartesian_data();

		y_pos += 60;
		text("Voxel Size", x_pos + 30, y_pos);
		float_input("###ctvvsize", voxel_size, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Generate Voxels", x_pos + 70, y_pos, 150, 20))
			cartesian_to_voxel();

		y_pos += 30;
		text("Voxel Scale", x_pos + 130, y_pos);

		y_pos += 20;
		if (float_min_max_slider("vs", x_pos, y_pos, 200.0f, min_vscale, max_vscale, voxel_scale_value, 50.0f)) {
			update_voxel_size();
		}

		y_pos += 30;
		text("Data Range", x_pos, y_pos);
		integer_input("###ctvmindr", min_data_value, x_pos + 100, y_pos, 45.0f);
		integer_input("###ctvmaxdr", max_data_value, x_pos + 190, y_pos, 45.0f);
	}

	void define_cartesian_data() {
		if(generation_type == 0)
			import_ply_file_to_cartesian_array();
		else 
			generate_cartesian_array();

		define_default_voxel_limits();
	}

	void import_ply_file_to_cartesian_array() {
		if(FW::filetools::file_exists(ply_file_pathname)){
			if (!import_file(ply_file_pathname)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not import file %s\n", ply_file_pathname);
				return;
			} else
				if (log_panel != NULL) log_panel->application_log.AddLog("INFO : File %s imported successfuly\n", ply_file_pathname);
		} else
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
		float z_range = max_z - min_z;

		if (x_range == 0 && y_range == 0 && z_range == 0) {
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
				case 0 :  r_coord = glm::ballRand(ctov_scale);      break;
				case 1 :  r_coord = glm::sphericalRand(ctov_scale); break;
				default : r_coord = glm::ballRand(ctov_scale);		 break;
			}
			cartesian_point_data.add_coordinate(r_coord);
		}
		
	}

	void define_default_voxel_limits() {
		min_x = cartesian_point_data.min_x, max_x = cartesian_point_data.max_x;
		min_y = cartesian_point_data.min_y, max_y = cartesian_point_data.max_y;
		min_z = cartesian_point_data.min_z, max_z = cartesian_point_data.max_z;
	}

	void define_generation_parameters() {
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_start = min_x;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_end   = max_x;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_start = min_y;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_end   = max_y;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_start = min_z;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_end   = max_z;

		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.resolution_step = voxel_size;
	}


	void cartesian_to_voxel(bool notification = true) {
printf("cart_to_voxel_widget_class :: in cartesian_to_voxel\n");

		if (current_selected_entity_id   == -1){
//printf("cart_to_voxel_widget_class :: current_selected_entity_id   == -1 #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}
		if (voxel_hcp_object_to_execute == NULL) { 
//printf("cart_to_voxel_widget_class :: voxel_hcp_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}

		define_generation_parameters();
		voxel_hcp_object_to_execute->voxel_object_data.create_voxel_matrix();
		int matrix_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size();

index_vector matrix_dimension = voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension; // tresting only
printf("cart_to_voxel_widget_class :: cartesian_to_voxel x_dim %i : y_dim  %i : z_dim %i\n", matrix_dimension.x, matrix_dimension.y, matrix_dimension.z);
		for (glm::vec3 coordinate : cartesian_point_data.coordinates) {
			int index = voxel_hcp_object_to_execute->voxel_object_data.index_of_voxel_cell_with_cartesian_coord(coordinate.x, coordinate.y, coordinate.z);

//printf("cart_to_voxel_widget_class :: INDEX !!!!  %i\n", index);
			if (index > -1 && index < matrix_size) {
				voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data[index]++;// add one to number of points in the voxel cell
			}
		}
//printf("cart_to_voxel_widget_class :: execute_voxel_function 3333\n");

		voxel_hcp_object_to_execute->define_vbo_vertices(min_data_value, max_data_value);// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("cart_to_voxel_widget_class :: execute_voxel_function 4444\n");

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);
//printf("cart_to_voxel_widget_class :: execute_voxel_function 5555\n");

		if (scene_voxel_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find voxel in the scene to update geometry data.\n");
//printf("cart_to_voxel_widget_class scene_voxel_object == NULL.\n");
		}
		else {
//printf("cart_to_voxel_widget_class scene_voxel_object != NULL.\n");

			if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated.\n");
//printf("cart_to_voxel_widget_class scene_voxel_object not updated\n");
				return;
			}

			// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default
			application_default_shader_uniform_variables_struct_type uniform_variable;
			float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxSize"; uniform_variable.value0 = &vox_size;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			uniform_variable.type = application_default_shader_variable_type_enum::Intv3; uniform_variable.name = "voxel_matrix_dimension"; uniform_variable.value0 = &voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			uniform_variable.type = application_default_shader_variable_type_enum::Floatv3; uniform_variable.name = "voxel_origin"; uniform_variable.value0 = &voxel_hcp_object_to_execute->voxel_object_data.matrix_origin;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

			float voxel_hcp_z_increment = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * 2.0 * sqrt(6.0) / 3.0;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxel_hcp_z_increment"; uniform_variable.value0 = &voxel_hcp_z_increment;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
			//uniform float voxel_hcp_z_increment; !!!!!!!!!! Need to find this one

			//voxel surface display data
			float v_min = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxel_min_surface_display_value"; uniform_variable.value0 = &v_min;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
			float v_max = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxel_max_surface_display_value"; uniform_variable.value0 = &v_max;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

//printf("cart_to_voxel_widget_class :: execute_voxel_function 6666\n");
		}

		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");


	}

	void execute_voxel_function(bool notification = true) {
printf("cart_to_voxel_widget_class :: in execute_voxel_function\n");

	}

private:
	float ctov_scale = 10.0f; // testing only

	int number_of_points = 1000;

	int random_type     = 0;
	int generation_type = 1;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

	string_type   ply_file_pathname  = "";
	string_type   ply_file_name      = "...";
	//string_type   expr_file_pathname = "";
	//string_type   expr_file_name     = "...";

	float min_x = -10.0f, max_x = 10.0f;
	float min_y = -10.0f, max_y = 10.0f;
	float min_z = -10.0f, max_z = 10.0f;

	float voxel_size        = 1.0f;
	float voxel_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	int min_data_value=1, max_data_value = 100;

	voxel_hcp_render_class   voxel_hcp_render;

	void update_voxel_size() {
		scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);

		if (scene_voxel_object != NULL) {
			application_default_shader_uniform_variables_struct_type uniform_variable;
			float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;
			uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "voxSize"; uniform_variable.value0 = &vox_size;
			scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
		}
	}

	void get_ply_file() {
		char const* patterns[] = { "*.ply","*.obj","*.stl"};
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No .ply file selected to perform voxel generation\n");
			return;
		}
		else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		ply_file_pathname = file_pathname;
		ply_file_name     = vwDialogs::get_filename(file_pathname, "\\");
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

		//voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname = file_pathname;
		//voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");
	}
*/
	// #########################################################

	struct cartesian_point_data_struct_type {
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
	
	cartesian_point_data_struct_type cartesian_point_data;


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
			cartesian_point_data.coordinates.clear();

            auto* mesh = pScene->mMeshes[0];

            for (uint32_t i = 0; i < mesh->mNumVertices; i++)
            {
				glm::vec3 mPos;
                mPos = { mesh->mVertices[i].x* ctov_scale, mesh->mVertices[i].y* ctov_scale ,mesh->mVertices[i].z * ctov_scale };

				cartesian_point_data.add_coordinate(mPos);
            }
            return true;
        }

        return false;
    }


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

};