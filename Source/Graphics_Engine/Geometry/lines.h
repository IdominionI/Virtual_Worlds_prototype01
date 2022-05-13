#pragma once

#include "geometry_basis.h"


/*
				LINE GEOMETRY CLASS

	The geometry class that can be used to store and display
	the geometry data that is defined to render lines.

	!!!!! NOTE :: This class has not been tested yet. !!!!!
*/

class line_class : public geometry_basis_class { // Not yet tested
public:
	line_class() {}
	~line_class() {
		delete_buffers();
	}

	bool init() {
		geometry_type = geometry_type_enum::lines;
		vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
		return true;
	}

	// Update geometry data in the buffers with the current 
    // vertex data content.
    bool update_geometry() {
printf("scene_grid_class :: update_geometry : 0000 || %i\n", buffers_created);
        if (buffers_created) delete_buffers();
 printf("scene_grid_class :: update_geometry : 1111 \n");
        if (!create_buffers()) return false;
 printf("scene_grid_class :: update_geometry : 2222 \n");
        return true;
    }

	// Need buffers_created to avoid app crashing if no VAO created by defining vertices
	// Seems OpneGl has no way to test if a VAO exists or not so need this
	// NOTE : Early stage of development so this may change 
	bool buffers_created = false;

	// add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_data_class& vertex) {
        vertices.push_back(vertex);
        number_vertices++;
    }

	bool create_buffers() {
		if (vertices.size() < 2) return false;
printf("scene_grid_class :: create_buffers : CCCC %i\n", vertices.size());
		vertex_buffer->create_point_vertex_buffer(vertices);
		number_vertices = vertices.size();
		buffers_created = true;
		return true;
	}

	bool delete_buffers() {
//printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
		if (!buffers_created) return false;
//printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
		vertex_buffer->delete_point_buffers();
//printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 
		number_vertices = 0;
		buffers_created = false;
		return true;
	}

	//void render(int shader_program_id) {
	
	
	//}

	// Vertices and indices
	std::vector<point_data_class>   vertices;
private:
};