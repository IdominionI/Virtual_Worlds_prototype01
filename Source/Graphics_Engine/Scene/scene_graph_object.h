#pragma once

// Followig enum to be used if have more than one scene_graph_object_class data type to store
//enum class scene_object_type_enum { Node, Light, Camera, Render_object };
//enum class scene_render_object_type_enum { Node, mesh, points };

template <class T>
class scene_graph_object_class {
public:
	scene_graph_object_class() {}
	~scene_graph_object_class() {}

	T scene_object_class;

	// Followig to be used if have more than one scene_graph_object_class data type to store
/*
	scene_object_type_enum scene_object_type;

	void *scene_object_class; // scene_object_class is defined as being of type void* pointer so as to be 
							  // able to store scene object data of any type without having to define a different
							  // scene class type table in the code for every node data type.
							  // Templates just cannot support this functionality
*/

private:
};