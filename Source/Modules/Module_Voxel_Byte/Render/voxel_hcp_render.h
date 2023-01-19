#pragma once

#include <string>

#include <Source/Graphics_Engine/Shader/shader_manager.h>
#include <Source/Graphics_Engine/Scene/Scene_objects/render_object.h>
#include <Source/Graphics_Engine/Scene/scene_node.h>

#include <Source/Graphics_Engine/Geometry/point_cloud.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../Voxel_hcp_object/voxel_hcp_object.h"

/*
			HCP voxel volume Render Class

	This class mamages the generation and deletion of the shader program
	that is to be used to display the 3D HCP voxel volume that an instance
	of this class is asociated with.

	The user selects through the application the files that contain the
	glsl code to be compiled and when an update to the shader program is
	called, a new shader program is compiled, and if successful, the older
	shader progrmam is destroyed, and the new shader program assigned to
	the hex surface object render variable.
*/
class voxel_hcp_render_class {
public:
	//voxel_hcp_render_class() {}
	//~voxel_hcp_render_class() {}

	point_cloud3DD_class *voxel_hcp_cloud = NULL;
	material_struct_type *voxel_material  = NULL;

	std::string shader_basis_code_directory_pathname = "assets/Shaders/Default/Voxel/Shader_basis_code/";

	std::string default_shader_file_directory   = "assets/Shaders/Default/Voxel/";
	std::string shader_file_directory           = "assets/Shaders/Default/Voxel/";

	std::string voxel_hcp_default_vertex_file   = "default_gen_TRD_VS.glsl";
	std::string voxel_hcp_default_point_file    = "default_gen_TRD_PGS.glsl";
	std::string voxel_hcp_default_geometry_file = "default_gen_TRD_GS.glsl";
	std::string voxel_hcp_default_fragment_file = "default_gen_TRD_FS.glsl";

	std::string voxel_hcp_vertex_file           = "default_gen_TRD_VS.glsl";
	std::string voxel_hcp_point_file            = "default_gen_TRD_PGS.glsl";
	std::string voxel_hcp_geometry_file         = "default_gen_TRD_GS.glsl";
	std::string voxel_hcp_fragment_file         = "default_gen_TRD_FS.glsl";


	bool define_voxel_hcp_render_object(scene_node_class <render_object_class>* entity_render_object, voxel_hcp_object_class* voxel_hcp_object, log_panel_class* log_panel);
	bool update_geometry_vertex_cloud_data(point_cloud3DD_class* voxel_hcp_cloud, scene_node_class <render_object_class>* entity_render_object, log_panel_class* log_panel);
	bool define_shader_program(scene_node_class <render_object_class>* entity_render_object, log_panel_class* log_panel);
	void define_shader_variables(scene_node_class <render_object_class>* entity_render_object);

private:
	shader_db_manager_class shader_db_manager;

	shader_parameters_struct_type shader_parameters;

	std::string vertex_shader_file   = "";
	std::string geometry_shader_file = "";
	std::string fragment_shader_file = "";

	bool define_initial_shader_program(scene_node_class <render_object_class>* entity_render_object, voxel_hcp_object_class* voxel_hcp_object, log_panel_class* log_panel);

};
/*
#include <Source/Editor/Scene/object_render_basis.h>

class voxel_hcp_render_object_class : public object_render_basis_class {
public:

	//voxel_hcp_render_object_class();
	bool define_render_object(object_basis_class* entity_object, scene_node_class <render_object_class>* entity_render_object);
	bool define_bounding_box(scene_graph_manager_class* scene_graph_manager, id_type entity_id, object_basis_class* entity_object);
	bool update_shader(scene_node_class <render_object_class>* entity_render_object);


private:
	voxel_hcp_render_class voxel_hcp_render;
	bounding_volume_class  bounding_volume;
};
*/