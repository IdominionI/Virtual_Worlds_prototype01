#pragma once

//#include <QVector>
//#include <QString>

#include "../Common/definitions.h"

class object_basis_class {
public:

	object_id_type  	object_id = 0;
	category_id_type	object_category_id = 0;
	name_type			object_name = "object";
	description_type	object_description = "";

	//vertex_data_type            local_location;
	//vertex_data_type            world_location;
	//direction_vector_data_type  forward_normal;
	//rotation_data_type          local_rotation;

	//bool						hide_object     = false;
	//bool						object_editable = false;

	object_basis_class() {
	}

	~object_basis_class() {
	}

private:

};