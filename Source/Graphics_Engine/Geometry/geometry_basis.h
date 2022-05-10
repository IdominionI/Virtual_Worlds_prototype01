#pragma once

#include "../Common/definitions.h"

#include "vertex_data.h"
#include "../Render/Buffers/vertex_buffer.h"

class geometry_basis_class {
public :
	geometry_basis_class() {
		geometry_type = geometry_type_enum::undefined;
	}
	~geometry_basis_class() {}

	geometry_type_enum geometry_type;

	virtual bool init() {
		vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>(); // Critical to have this initialised in every derived class or app will crash

		create_buffers();
		return true;
	}

	//virtual bool load(const std::string& filepath) { return false; }
	virtual bool load(const std::string& filepath, float x_off ) { return false; }
	virtual bool save(const std::string& filepath) { return false; }

	virtual bool add_geometry_data() { return false; }
	virtual bool update_geometry()   { return false; }

	// add_vertex function required but cannot be defined as a virtual function as the function paramter
	// needs to be spcified and thus cannot define this function with a different paramater data type
	// Using a template just unneccessarily complicates things as well and causes problems
	//virtual void add_vertex() = 0; // !!!!!!!!!!!! If define this as a pure virtual function just causes too many problems

	virtual bool create_buffers() = 0;
	virtual bool delete_buffers() = 0;

	virtual void render(int shader_program_id) {
		//printf("MESH render %i\n", vertex_indices.size());

		switch (geometry_type) {
			case geometry_type_enum::points    : {if(vertex_buffer != NULL) vertex_buffer->draw_points(number_vertices, shader_program_id);
//printf("geometry_basis_class:: render %i \n", number_vertices);
												  }               break;
			case geometry_type_enum::mesh      : if(vertex_buffer != NULL) vertex_buffer->draw_triangles((int)vertex_indices.size()); break;
			
			case geometry_type_enum::triangles : if(vertex_buffer != NULL) vertex_buffer->draw_triangles_array(number_vertices, shader_program_id);  break;
			
			case geometry_type_enum::lines     : if(vertex_buffer != NULL) vertex_buffer->draw_lines((int)vertex_indices.size(), shader_program_id); break; // Not yet tested
			// Other expansion types here
			case geometry_type_enum::undefined : {
				// error message
				printf("Render type undefined to render entity of ID");
				//printf("Render type undefined to render entity of ID %i", entity_id_type);
				break; }
		}
	}// this to be defined in object basis class of to be called from object basis class????

	virtual void bind()   { vertex_buffer->bind(); }
	virtual void unbind() { vertex_buffer->unbind(); }

//protected:
	// Buffers manager
	std::unique_ptr<openGL_vertex_Index_buffer_class> vertex_buffer;

	// Vertices and indices
	//std::vector<vertex_data_class> vertices;
	std::vector<unsigned int>  vertex_indices;

	int number_vertices = 0;
};