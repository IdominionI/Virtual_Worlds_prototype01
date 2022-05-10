#pragma once

#include "../Common/definitions.h"

//template <class T>
class entity_class {
public:
	entity_class() {}

	~entity_class() {}

	idu_type    entity_id           = NULL_ENTITY_ID; // Unique ID number to distiguish this entity from all others
	idu_type    entity_category_id  = NULL_ENTITY_ID; // Unique ID number of category data type of this entity
	string_type entity_name         = "";
	string_type entity_description  = "";
/*
	T* entity_object_class = NULL;	// entity_object_class is defined as being of type void* pointer so as to be 
									// able to store entity object data of any type without having to define a different
									// object class type table in the code for every component data type.
*/
	void* entity_object_class = NULL;	// entity_object_class is defined as being of type void* pointer so as to be 
										// able to store entity object data of any type without having to define a different
										// object class type table in the code for every component data type.
										// Templates just cannot support this functionality

private:
};