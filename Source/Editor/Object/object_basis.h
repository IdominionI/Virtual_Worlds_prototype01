#pragma once

#include "../Common/definitions.h"

/*
					OBJECT BASIS

	A basis class of common attributes, parameters etd
	that all entity objects must have and thus inherit
*/


class object_basis_class {
public:

	object_id_type  	object_id          = 0;
	category_id_type	object_category_id = 0;
	name_type			object_name        = "object";
	description_type	object_description = "";

	object_basis_class() {
	}

	~object_basis_class() {
	}

private:

};