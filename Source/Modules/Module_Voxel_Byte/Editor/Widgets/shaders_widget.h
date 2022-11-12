#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>

#include "shader_variables_widget.h"

#include "../../Voxel_hcp_object/voxel_hcp_object.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "generation_selection_widget.h"
/*
			HCP voxel volume shader widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define the parameters and shader variables
	required to display a 3D HCP voxel volume surface.

	This class widget has controls to change the display of the
	3D HCP voxel volume on the computer screen in incremental
	steps by changing the shader variable values according to
	the settings that the user defines for each shader variable.
*/

class voxel_shaders_widget_class {
public:
	voxel_shaders_widget_class() {}
	~voxel_shaders_widget_class() {}

	material_struct_type              *voxel_shader_parameters = NULL;
	//shader_parameters_struct_type     *voxel_shader_parameters = NULL;
	shader_variables_widget_class      shader_variables_widget;
	//voxel_hcp_generation_widget_class *voxel_hcp_generation_widget;
	hcp_voxel_genertion_selection_widget_class *hcp_voxel_genertion_selection_widget;// ******
	log_panel_class               *log_panel = NULL;

	//bool animate_shaders             = false;

	id_type                  current_selected_entity_id = -1;   // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	scene_manager_class      *scene_manager = NULL;

	void display() {
//printf("voxel_shaders_widget_class::display() 0000 :current_selected_enity_id < 0 %i\n", current_selected_enity_id);

		initialise_parameters();

		float x_pos = 10.0f, y_pos = 70.0f;

		title("Voxel  Shaders ");

		y_pos += 30;
		text("Animate Shaders : ", x_pos+80, y_pos);

		ImGui::SetCursorPosX(x_pos + 220);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###sdisplas", &voxel_shader_parameters->animate_shaders);

		y_pos += 30;
		text("Display Bounding Volume : ", x_pos + 80, y_pos);

		ImGui::SetCursorPosX(x_pos + 273);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("###sdisplbv", &display_bounding_volume)) {
			if (scene_manager != NULL) {
				// Commented out code for possible dissable of function if hcp voxel is not displayed.
				//scene_node_class <render_object_class> *hcp_voxel_render_object = scene_manager->get_scene_entity_render_object(current_selected_entity_id);
				scene_node_class <render_object_class> *bv_render_object = scene_manager->get_scene_entity_render_object(current_selected_entity_id + BOUNDING_GEOMETRY_OFFSET);
				
				//if (hcp_voxel_render_object != NULL && hcp_voxel_render_object->scene_graph_object.scene_object_class.visible) {

					if (bv_render_object != NULL)
						bv_render_object->scene_graph_object.scene_object_class.visible = display_bounding_volume;
				//}
			}
		}

		y_pos += 20;
		text("Shader Files", x_pos + 100, y_pos);

		y_pos += 20;
		text("Vertex  :", x_pos, y_pos);

//printf("voxel_shaders_widget_class 0000 %s.\n", voxel_shader_parameters.vertex_shader_file_name.c_str());// replace with get file pathname tool;
		//if (ex_button(voxel_shader_parameters->vertex_shader_file_name.c_str(), x_pos + 80, y_pos, 190, 20))
		if (ex_button(voxel_shader_parameters->vertex_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_vertex_shader_file();

		ImGui::SetCursorPosX(x_pos + 120+ 150+3);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###sddvsf", &voxel_shader_parameters->use_default_vertex_shader);

		y_pos += 23;
		text("Point   :", x_pos, y_pos);

		//if (ex_button(voxel_shader_parameters->point_geometry_shader_file_name.c_str(), x_pos + 80, y_pos, 190, 20))
		if (ex_button(voxel_shader_parameters->point_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_point_geometry_shader_file();

		ImGui::SetCursorPosX(x_pos + 120 + 150 + 3);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###sddpsf", &voxel_shader_parameters->use_default_point_shader);

		y_pos += 23;
		text("Geometry:", x_pos, y_pos);

		if (ex_button(voxel_shader_parameters->geometry_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_geometry_shader_file();

		ImGui::SetCursorPosX(x_pos + 120 + 150 + 3);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###sddgsf", &voxel_shader_parameters->use_default_geometry_shader);

		y_pos += 23;
		text("Fragment:", x_pos, y_pos);

		if (ex_button(voxel_shader_parameters->fragment_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_fragment_shader_file();

		ImGui::SetCursorPosX(x_pos + 120 + 150 + 3);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("###sddfsf", &voxel_shader_parameters->use_default_fragment_shader);

		y_pos += 23;
		text("Shader Variables", x_pos + 80, y_pos);

		ImGui::BeginChild("shader_variables", ImVec2(320, 300.0f), true);

		if (shader_variables_widget.variables_manager(voxel_shader_parameters)) update_shader_variables(); // probably inefficient but simple

		ImGui::EndChild();


		y_pos += 330;

		if (ex_button(ICON_FA_STEP_BACKWARD "###vsd", x_pos + 50, y_pos, 50, 20))
			perform_decrement_variables();

		if (ex_button(ICON_FA_EJECT "###vgu", x_pos + 110, y_pos, 80, 20))
			update_voxel_shaders();
			

		if (ex_button(ICON_FA_STEP_FORWARD "###vsi", x_pos + 200, y_pos, 50, 20))
			perform_increment_variables();

		y_pos += 30;

		if (ex_button("Clear Variables###vgc", x_pos + 85, y_pos, 140, 20))
			clear_variables();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###vss", x_pos + 35, y_pos, 100, 20))
			save_shader_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###vsdl", x_pos + 165, y_pos, 100, 20))
			load_shader_parameters();

	}

	// NOTE :: to remove slider min max limits when performing increment and decrement funcions
	//         comment out the code between 
	//			if (<variable>.active_variable_step) {
	//		   and
	//         <variable>.value +-= <variable>.variable_step; 
	//          
	void perform_decrement_variables() {
//printf("perform_decrement_variables button clicked\n");// replace with decrement step
		for (shader_parameter_variable_struct_type &variable : voxel_shader_parameters->variables) {
			if (variable.active_variable_step) {
				if (variable.variable_step > 0.0 && variable.value - variable.variable_step < variable.slider_min)
					variable.value = variable.slider_min;
				else
					if (variable.variable_step < 0.0 && variable.value - variable.variable_step > variable.slider_max)
						variable.value = variable.slider_max;
					else
						variable.value -= variable.variable_step;
			}
		}

		for (shader_parameter_int_variable_struct_type &int_variable : voxel_shader_parameters->int_variables) {
			if (int_variable.active_variable_step) {
				if (int_variable.variable_step > 0 && int_variable.value - int_variable.variable_step < int_variable.slider_min)
					int_variable.value = int_variable.slider_min;
				else
					if (int_variable.variable_step < 0 && int_variable.value - int_variable.variable_step > int_variable.slider_max)
						int_variable.value = int_variable.slider_max;
					else
						int_variable.value -= int_variable.variable_step;
			}
		}

		update_shader_variables();
	}

	void perform_increment_variables() {
//printf("perform_increment_variables button clicked\n");// replace with decrement step
		for (shader_parameter_variable_struct_type &variable : voxel_shader_parameters->variables) {
			if (variable.active_variable_step) {
					if (variable.variable_step < 0.0 && variable.value + variable.variable_step < variable.slider_min)
						variable.value = variable.slider_min;
					else
						if (variable.variable_step > 0.0 && variable.value + variable.variable_step > variable.slider_max)
							variable.value = variable.slider_max;
						else
							variable.value += variable.variable_step;
			}
		}

		for (shader_parameter_int_variable_struct_type &int_variable : voxel_shader_parameters->int_variables) {
			if (int_variable.active_variable_step) {
				if (int_variable.variable_step < 0 && int_variable.value + int_variable.variable_step < int_variable.slider_min)
					int_variable.value = int_variable.slider_min;
				else
					if (int_variable.variable_step > 0 && int_variable.value + int_variable.variable_step > int_variable.slider_max)
						int_variable.value = int_variable.slider_max;
					else
						int_variable.value += int_variable.variable_step;
			}
		}

		update_shader_variables();
	}

	void update_shader_variables() { // not sure this is needed as render node does this task
//printf("voxel_shaders_widget_class::update voxel shaders clicked\n");// replace with update variables
		// Get the scene render object that stores the object render properties
		scene_node_class <render_object_class> *entity_render_object = scene_manager->get_render_object(current_selected_entity_id);
		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::update_shader_variables :entity_render_object == NULL\n");
			return;
		}
//printf("voxel_shaders_widget_class::update_shader_variables 111\n");

		material_struct_type *shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties fpr the render process

//printf("voxel_shaders_widget_class::update_shader_variables 222\n");

		shader_material->variables      = voxel_shader_parameters->variables;
		shader_material->int_variables  = voxel_shader_parameters->int_variables;
		shader_material->bool_variables = voxel_shader_parameters->bool_variables;


//printf("voxel_shaders_widget_class::update_shader_variables 333\n");
	}

	void update_voxel_shaders(bool notification = true) {
//printf("execute_shader_function button clicked\n");

		if (voxel_shader_parameters == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel shader parameter data is undefined. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class :: initialise_parameters : voxel_shader_parameters == NULL\n");
			return;
		}

		if (scene_manager == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Scene manager is undefined. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::initialise_parameters :scene_manager == NULL\n");
			return;
		}

		if (current_selected_entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : There is no defined current selected entity. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::initialise_parameters :current_selected_enity_id < 0 %i\n", current_selected_enity_id);
			return;
		}

		// Get the scene render object that stores the object render properties
		scene_node_class <render_object_class> *entity_render_object = scene_manager->get_render_object(current_selected_entity_id);
		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::initialise_parameters :entity_render_object == NULL\n");
			return;
		}

		material_struct_type *shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties fpr the render process
		
		// ######## ERROR : voxel_shader_parameters some how retains its values and is not cleard from memory when object is deleted and when an import is 
		// ########         performed the parameter data is somehow merged with the imported shader paramter data. Mystery as this does not occure for hex surface class
		// ########         which uses identical data class and code.

		//*shader_parameters = *voxel_shader_parameters; // copy the current shader code file data mnd variable data to the object render node storage that is used to render the object to screen

		shader_material->vertex_shader_file_pathname   = voxel_shader_parameters->vertex_shader_file_pathname;
		shader_material->point_shader_file_pathname    = voxel_shader_parameters->point_shader_file_pathname;
		shader_material->geometry_shader_file_pathname = voxel_shader_parameters->geometry_shader_file_pathname;
		shader_material->fragment_shader_file_pathname = voxel_shader_parameters->fragment_shader_file_pathname;

		shader_material->animate_shaders  = voxel_shader_parameters->animate_shaders;

		shader_material->use_default_vertex_shader   = voxel_shader_parameters->use_default_vertex_shader;
		shader_material->use_default_point_shader    = voxel_shader_parameters->use_default_point_shader;
		shader_material->use_default_geometry_shader = voxel_shader_parameters->use_default_geometry_shader;
		shader_material->use_default_fragment_shader = voxel_shader_parameters->use_default_fragment_shader;

		shader_material->variables      = voxel_shader_parameters->variables;
		shader_material->int_variables  = voxel_shader_parameters->int_variables;
		shader_material->bool_variables = voxel_shader_parameters->bool_variables;

		shader_material->define_shader_uniforms();

//printf("voxel_shaders_widget_class : update_shader_variables :Vertex glsl file  %i : %s\n" , shader_material->use_default_vertex_shader, shader_material->glsl_vertex_shader_file_pathname.c_str() );
//printf("voxel_shaders_widget_class : update_shader_variables :Geometry glsl file %i :: %i : %s\n" , shader_material->use_default_geometry_shader, shader_material->use_default_point_shader, shader_material->glsl_geometry_shader_file_pathname.c_str());
//printf("voxel_shaders_widget_class :update_shader_variables :Fragment glsl file %i : %s\n" , shader_material->use_default_fragment_shader, shader_material->glsl_fragment_shader_file_pathname.c_str());


//printf("voxel_shaders_widget_class::initialise_parameters : XXXXX %s:\n", voxel_shader_parameters->default_point_shader_file_pathname.c_str());
//printf("voxel_shaders_widget_class::initialise_parameters : YYYYY %i:\n", voxel_shader_parameters->variables.size());

		//voxel_hcp_render.define_shader_program(entity_render_object,log_panel);// ++++++++++++++++++++

		// Following required as generating a new shader program also needs to up date default shader variables
		// that are defined in the Hex Generation widget
		//voxel_hcp_generation_widget->change_voxels_display();
		hcp_voxel_genertion_selection_widget->change_voxels_display();
	}

	bool initialise_parameters() {
		if (voxel_hcp_object_to_execute == NULL) {
//printf("voxel_shaders_widget_class :: initialise_parameters :voxel_hcp_object_to_execute == NULL\n");
			return false;
		}

		//voxel_shader_parameters = &voxel_hcp_object_to_execute->voxel_object_data.shader_parameters;
		voxel_shader_parameters = &voxel_hcp_object_to_execute->voxel_object_data.shader_parameters;

//printf("voxel_shaders_widget_class :: initialise_parameters : AAAAA !!! %s :\n", voxel_shader_parameters->vertex_shader_file_name.c_str());

		if((voxel_shader_parameters->vertex_shader_file_pathname == ""))
			voxel_shader_parameters->vertex_shader_file_pathname = "...###vsf";
		//else
		//	voxel_shader_parameters->vertex_shader_file_pathname = vwDialogs::get_filename(voxel_shader_parameters->vertex_shader_file_pathname.string(), "/");

		if ((voxel_shader_parameters->point_shader_file_pathname == ""))
			voxel_shader_parameters->point_shader_file_pathname = "...###vsf";
		//else
		//	voxel_shader_parameters->point_shader_file_pathname = vwDialogs::get_filename(voxel_shader_parameters->point_shader_file_pathname, "/");

		if ((voxel_shader_parameters->geometry_shader_file_pathname == ""))
			voxel_shader_parameters->geometry_shader_file_pathname = "...###vsf";
		//else
		//	voxel_shader_parameters->geometry_shader_file_name = vwDialogs::get_filename(voxel_shader_parameters->geometry_shader_file_pathname, "/");

		if ((voxel_shader_parameters->fragment_shader_file_pathname == ""))
			voxel_shader_parameters->fragment_shader_file_pathname = "...###vsf";
		//else
		//	voxel_shader_parameters->fragment_shader_file_name = vwDialogs::get_filename(voxel_shader_parameters->fragment_shader_file_pathname, "/");

		return true;
	}

private:
	voxel_texture_import_export_class  voxel_texture_import_export_data;
	voxel_hcp_render_class			   voxel_hcp_render;

	bool display_bounding_volume = true;

	void save_shader_parameters() {

		if (!voxel_shader_parameters) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Cannot export shader parameters. No shader parameter defined to export from\n");
			return;
		}

//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.twm" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to save voxel generation parameter data to\n");
			return;
		}
		else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("save_generation_parameters != NULL  \n");

		voxel_texture_import_export_data.shader_parameters = *voxel_shader_parameters;
		voxel_texture_import_export_data.save_working_generated_function(file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data saved to file\n %s\n", file_pathname);

	}

	void load_shader_parameters() {

//printf("load button clicked\n");// replace with clear variables
		if (!voxel_shader_parameters) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Cannot import shader parameters. No shader parameter defined to import into\n");
			return;
		}

		clear_variables();

		char const* patterns[] = { "*.twm" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import voxel generation parameter data from.\n");
			return;
		}
		else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("load_generation_parameters != NULL  \n");

		voxel_texture_import_export_data.import_voxel_genereated_function(voxel_shader_parameters, file_pathname);

		//voxel_shader_parameters->vertex_shader_file_name         = vwDialogs::get_filename(voxel_shader_parameters->vertex_shader_file_pathname, "/");
		//voxel_shader_parameters->point_geometry_shader_file_name = vwDialogs::get_filename(voxel_shader_parameters->point_shader_file_pathname, "/");
		//voxel_shader_parameters->geometry_shader_file_name       = vwDialogs::get_filename(voxel_shader_parameters->geometry_shader_file_pathname, "/");
		//voxel_shader_parameters->fragment_shader_file_name       = vwDialogs::get_filename(voxel_shader_parameters->fragment_shader_file_pathname, "/");

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);

	}

	void select_vertex_shader_file() {
//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");
//printf("select_vertex_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = {"*_VS.glsl"};
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No vertex shader file selected.\n");
			return;
		}

//printf("vertex_shader_file_pathname != NULL %s:\n", file_pathname);

		//change directory deliminator from \ to / for consistency
		std::string s                                        = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->vertex_shader_file_pathname = s;
//printf("select_vertex_shader_file 111111111 : %s:\n",s.c_str());
		//voxel_shader_parameters->vertex_shader_file_name     = vwDialogs::get_filename(s, "/");

	}

	void select_point_geometry_shader_file() {
//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");
//printf("select_point_geometry_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_PGS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No point geometry shader file selected.\n");
			return;
		}
		//		else
//printf("point_geometry_shader_file_pathname != NULL %s:\n", file_pathname);

		std::string s                                       = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->point_shader_file_pathname = s;
		//voxel_shader_parameters->point_geometry_shader_file_name = vwDialogs::get_filename(file_pathname, "/");

	}

	void select_geometry_shader_file() {
//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");
//printf("select_geometry_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_GS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No geometry shader file selected.\n");
			return;
		}
		//		else
//printf("geometry_shader_file_pathname != NULL %s:\n", file_pathname);

		std::string s                                          = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->geometry_shader_file_pathname = s;
		//voxel_shader_parameters->geometry_shader_file_name     = vwDialogs::get_filename(file_pathname, "/");

	}

	void select_fragment_shader_file() {
		//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");

//printf("select_fragment_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_FS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No fragment shader file selected.\n");
			return;
		}
		//		else
//printf("vertex_fragment_file_pathname != NULL %s:\n", file_pathname);

		std::string s                                          = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->fragment_shader_file_pathname = s;
		//voxel_shader_parameters->fragment_shader_file_name     = vwDialogs::get_filename(file_pathname, "/");

	}

	void clear_variables() {
printf("Clear Variables clicked");// replace with clear variables
		voxel_shader_parameters->variables.clear();
		voxel_shader_parameters->int_variables.clear();
		voxel_shader_parameters->bool_variables.clear();
	}

};
