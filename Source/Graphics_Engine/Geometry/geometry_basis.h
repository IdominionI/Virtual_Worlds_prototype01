#pragma once

#include "../Common/definitions.h"

#include "vertex_data.h"
#include "../Render/Buffers/vertex_buffer.h"

/*
				GEOMETRY BASIS CLASS

	This basis class is the class that is used to render any
	and all vertex graphics data types to the computer screen.

	All geometry object classes that are used to store and display
	an entity geometry on the computer screen must inherit this
	class, and then define for each scene geometry object the code 
	for the vertex data type that is defined in the	vertex_data.h
	file.
	
	Then the required and mandatory virtual functions in this
	class need to be indivually coded to accomadate the functionality
	required to render the graphics object. These are

	init()
	add_geometry_data()
	update_geometry()
	create_buffers()
	delete_buffers()
	bind()
	unbind()

*/


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

	virtual void render() = 0;

	virtual void bind()   { vertex_buffer->bind(); }
	virtual void unbind() { vertex_buffer->unbind(); }

//protected:
	// Buffers manager
	std::unique_ptr<openGL_vertex_Index_buffer_class> vertex_buffer;

	// Vertices and indices
	std::vector<unsigned int>  vertex_indices;

	size_t number_vertices = 0;
};
