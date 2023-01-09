#pragma once
#include "../../Geometry/point_cloud.h"

#include "../../Scene/Scene_objects/camera_object.h"
#include "../../Scene/scene_graph_manager.h"

/*
				Viewer Grid Class

	A set of definitions and class to define the
	vertices and shader variabels that are used by
	the grid shader program code in the resources 
	directory

				Resources/Shaders/Default/Grid/
	
	of this application

	The shader program uses some basic point data
	from a point data render object and with the
	defined uniform variable option and camera data
	renders an axis grid for one or more or no axis.
*/


static const char* grid_vertex_shader =
"uniform mat4 mvpMatrix;\n" //model-view-projection matrix
"in vec4 vertex;\n"         //vertex that the shader is processing
							//"in vec4 color;\n"			//color of vertex that shader is processing
							//"out vec4 grid_color;\n"  //output a varying color for the fragment shader to use for this vertex
	"void main(){\n"
	//"   varyingColor = color;\n"
	"	gl_Position = mvpMatrix * vertex;\n"
	"}\n";

static const char* grid_fragment_shader =
"uniform vec4 grid_color;\n" // a single color to color all surfaces
							 //"in vec4 varyingColor;\n" // color variable to accept from the vertex buffer to renmder
	"void main(){\n"
	"   gl_FragColor = grid_color;\n"
	//"   gl_FragColor = varyingColor;\n"
	"}\n";



#define XY_PLANE_GRID          10
#define XY_PLANE_RELATIVE      11
#define XY_PLANE_RELATIVE_DIST 12
#define XY_PLANE_ABSOLUTE_DIST 13
#define XY_PLANE_GRID_COLOR    14

#define XZ_PLANE_GRID		   30
#define XZ_PLANE_RELATIVE      31
#define XZ_PLANE_RELATIVE_DIST 32
#define XZ_PLANE_ABSOLUTE_DIST 33
#define XZ_PLANE_GRID_COLOR    34

#define YZ_PLANE_GRID		   50
#define YZ_PLANE_RELATIVE      51
#define YZ_PLANE_RELATIVE_DIST 52
#define YZ_PLANE_ABSOLUTE_DIST 53
#define YZ_PLANE_GRID_COLOR    54


#define GRID_ENTITY_ID 1000000000

class viewer_grid_class {
public:
	viewer_grid_class() {}

	~viewer_grid_class() { // Need to be tested and used instead of scene_graph_manager->delete_scene_entity_render_object(GRID_ENTITY_ID);
		delete grid_geometry_object;
		delete viewer_grid_render_object; 
	}

	// Default grid shader program directory path
	std::string shader_file_directory = "Resources/Shaders/Default/Grid/";

	// Default grid shader program source code files
	std::string grid_vertex_file   = "grid_vertex_shader.glsl";
	std::string grid_geometry_file = "grid_geometry_shader.glsl";
	std::string grid_fragment_file = "grid_fragment_shader.glsl";

	scene_graph_manager_class *scene_graph_manager;
	point_cloud3DD_class      *grid_geometry_object;

	size_t    number_grid_cells = 20;
	
	glm::vec4 xy_grid_color = { 0.7843,0.5882,0.5882,1 }, xz_grid_color = { 0.5882,0.7843,0.5882,1 }, yz_grid_color = { 0.1961,0.1961,0.7843,1 };

	// NOTE :: Very wierd phenomenon that the last assigned bool var display_yz_plane if set to true at any time will cause
	//         the value contents assigned within the address of the variable &display_xy_plane or &display_xz_plane as 256
	//		   or 257 and not 1 or 0. If display_yz_plane is assigned as false, then &display_xz_plane cans be assigned
	//         correctly as 0 or 1, but &display_xy_plane will remain being assigned as 256 and 257.
	//         Suspect this is a compilor issue and can have unpresictable behaviours for all variables of all types.
	//         Thus when assigning values to update shader variables, assign the value of a variable to a local variable to
	//         be passed to the update_shader_variable function as in the subroutines below. 

	bool display_xy_plane = true,  display_xz_plane = false, display_yz_plane = false;
	bool relative_xy_grid = false, relative_xz_grid = false, relative_yz_grid = false;
	
	float relative_xy_grid_dist = -3.0f, relative_xz_grid_dist = 3.0f, relative_yz_grid_dist = 3.0f;
	glm::vec3 xy_grid_origin = { 0.0f,0.0f,0.0f }, xz_grid_origin = { 0.0f,0.0f,0.0f }, yz_grid_origin = { 0.0f,0.0f,0.0f };
	
	bool initialise() {
//printf("viewer_grid_class :: initialise\n");
		if (!scene_graph_manager->add_scene_entity_render_object(GRID_ENTITY_ID)) {
			printf("ERROR: could not add viewer grid render geometry object\n");
			return false;
		}

		viewer_grid_render_object = scene_graph_manager->get_scene_entity_render_object(GRID_ENTITY_ID);

		if (viewer_grid_render_object == NULL) {
			printf("ERROR: could not find viewer grid render object\n");
			return false;
		}

		grid_geometry_object = new point_cloud3DD_class;
		if (grid_geometry_object == NULL) {
			printf("ERROR: could not create viewer grid render object geometry data\n");
			return false;
		}

		grid_geometry_object->init();
		
		grid_geometry_object->vertices.clear();
		glm::vec4 vertex;

		for (float i = -1.0f; i <1.1f; i += 2.0f/(float) number_grid_cells) {
			vertex = { i,0.0,0.0,0.0 };
			grid_geometry_object->add_vertex(vertex);
		}


		grid_geometry_object->update_geometry();

		viewer_grid_render_object->scene_graph_object.scene_object_class.geometry = grid_geometry_object;

		if (!create_viewer_grid_shader_program()) {
			printf("ERROR: could not create viewer grid shader_program.\n");
			scene_graph_manager->delete_scene_entity_render_object(GRID_ENTITY_ID);
			return false;
		}

		return true;
	}

	bool create_viewer_grid_shader_program() {
		std::string vertex_file   = shader_file_directory + grid_vertex_file;
		std::string geometry_file = shader_file_directory + grid_geometry_file;
		std::string fragment_file = shader_file_directory + grid_fragment_file;

//printf("create_viewer_grid_shader_program 00:  %s \n", vertex_file.c_str());
//printf("create_viewer_grid_shader_program 11:  %s \n", geometry_file.c_str());
//printf("create_viewer_grid_shader_program 22:  %s \n", fragment_file.c_str());

		grid_shader_program_id = grid_shader_program.create_shader_program(vertex_file, geometry_file, fragment_file);

		if (!grid_shader_program_id) {
			printf("ERROR: Create grid shader program failed\n ");
			return false;
		}
		material_basis_struct_type *grid_shader_material = new material_basis_struct_type;

		if(!grid_shader_material) {
			printf("ERROR: Create grid shader program failed : Could not allocate material to store grid material display parameter data\n ");
			return false;
		}

		viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material = grid_shader_material;

		viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material->shader_program_id  = grid_shader_program_id;
		viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material->entity_category_id = VIEWER_GRID_ID;
		viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material->shader_name        = VIEWER_GRID_CATEGORY;
		viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material->shader_description = VIEWER_GRID_CATEGORY;

		return true;
	}

	void update_viewer_grid() {
		// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default

//if(display_xy_plane == true)
//	printf("update_viewer_grid :: display_xy_plane == true  %i :: %i\n", grid_shader_program_id, display_xy_plane);
//else
//	printf("update_viewer_grid :: display_xy_plane != true  %i :: %i\n", grid_shader_program_id, display_xy_plane);

		grid_shader_program.set_b1(grid_shader_program_id, display_xy_plane, "display_xy_plane");

		grid_shader_program.set_b1(grid_shader_program_id, display_xz_plane, "display_xz_plane");

		grid_shader_program.set_b1(grid_shader_program_id, display_yz_plane, "display_yz_plane");

		grid_shader_program.set_b1(grid_shader_program_id, relative_xy_grid, "relative_xy_grid");

		grid_shader_program.set_b1(grid_shader_program_id, relative_yz_grid, "relative_yz_grid");

		grid_shader_program.set_b1(grid_shader_program_id, relative_xz_grid, "relative_xz_grid");

		grid_shader_program.set_f1(grid_shader_program_id, relative_xy_grid_dist, "relative_xy_grid_dist");

		grid_shader_program.set_f1(grid_shader_program_id, relative_xz_grid_dist, "relative_xz_grid_dist");

		grid_shader_program.set_f1(grid_shader_program_id, relative_yz_grid_dist, "relative_yz_grid_dist");

		grid_shader_program.set_vec4(grid_shader_program_id, xy_grid_color, "xy_grid_color");

		grid_shader_program.set_vec4(grid_shader_program_id, xz_grid_color, "xz_grid_color");

		grid_shader_program.set_vec4(grid_shader_program_id, yz_grid_color, "yz_grid_color");

	}

	void update_viewer_grid_colors(int uniform_to_update_id) {
		std::string grid_axis_name;
		glm::vec4   grid_color;

		viewer_grid_render_object = scene_graph_manager->get_scene_entity_render_object(GRID_ENTITY_ID);

		switch(uniform_to_update_id){

			case XY_PLANE_GRID_COLOR :{	grid_axis_name = "xy_grid_color";
										grid_color     = xy_grid_color;
										break;}

			case XZ_PLANE_GRID_COLOR :{ grid_axis_name = "xz_grid_color";
										grid_color     = xz_grid_color;
										break;}

			case YZ_PLANE_GRID_COLOR :{ grid_axis_name = "yz_grid_color";
										grid_color     = yz_grid_color;
										break;}

		}

		grid_shader_program.set_vec4(grid_shader_program_id, grid_color, grid_axis_name);
	}

	void update_viewer_grid_uniform(int uniform_to_update_id) {
		std::string grid_axis_name;
		glm::vec4   grid_color;

if(display_xy_plane == true)
	printf("display_xy_plane == true\n");
else
	printf("display_xy_plane != true\n");

		viewer_grid_render_object = scene_graph_manager->get_scene_entity_render_object(GRID_ENTITY_ID);

		switch (uniform_to_update_id) {
			case XY_PLANE_GRID          : {grid_shader_program.set_b1(grid_shader_program_id, display_xy_plane, "display_xy_plane");
										  break;}

			case XY_PLANE_RELATIVE		: {grid_shader_program.set_b1(grid_shader_program_id, relative_xy_grid, "relative_xy_grid");
										  break;}

			case XY_PLANE_RELATIVE_DIST	:{grid_shader_program.set_f1(grid_shader_program_id, relative_xy_grid_dist, "relative_xy_grid_dist");
										  break;}

			//case XY_PLANE_GRID_COLOR	:

			case XZ_PLANE_GRID          : {grid_shader_program.set_b1(grid_shader_program_id, display_xz_plane, "display_xz_plane");
										  break;}
							 
							 
			case XZ_PLANE_RELATIVE		: {grid_shader_program.set_b1(grid_shader_program_id, relative_xz_grid, "relative_xz_grid");
										  break;}
			//case XZ_PLANE_RELATIVE_DIST	:
			//case XZ_PLANE_GRID_COLOR	:

			case YZ_PLANE_GRID          : {grid_shader_program.set_b1(grid_shader_program_id, display_yz_plane, "display_yz_plane");
										  break;}
							 
			case YZ_PLANE_RELATIVE		: {grid_shader_program.set_b1(grid_shader_program_id, relative_yz_grid, "relative_yz_grid");
										  break;}
			//case YZ_PLANE_RELATIVE_DIST	:
			//case YZ_PLANE_GRID_COLOR	:
			default: return; break;
		}

		//viewer_grid_render_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
	}

private:
	scene_node_class <render_object_class> *viewer_grid_render_object;

	GLuint		 grid_shader_program_id = 0;
	shader_class grid_shader_program;
};

