#pragma once

/*
	unversal defined paramter data that is to be used in multiple header files and/or classes
*/

#include <string>

#define NULL_INDEX   UINT_MAX

#define DATA_TYPE_ID_SHORT_INT           100
#define DATA_TYPE_ID_UNSIGNED_SHORT_INT  101
#define DATA_TYPE_ID_INT                 102
#define DATA_TYPE_ID_UNSIGNED_INT        103
#define DATA_TYPE_ID_CHAR                104
#define DATA_TYPE_ID_UNSIGNED_CHAR       105
#define DATA_TYPE_ID_FLOAT               106
#define DATA_TYPE_ID_BOOL                107

typedef std::string  string_type;
typedef unsigned int idu_type;
typedef int          id_type;
typedef unsigned int index_type;


#define NULL_ENTITY_ID            UINT_MAX 

struct entity_component_struct_type {
	id_type		component_id;               // component class id
	index_type  component_table_index;       // index of a list of component class or categories
	index_type  component_list_data_index;  // index within a component category for mow this is not viable as deletions create incorrect index references
};


// !!!!!!!!!!!!!!!!!!! SETS !!!!!!!!!!!!!!!!!!!!!!!!!!
// Entity category definitions
#define NULL_CATEGORY_ID            UINT_MAX

#define VIEWER_GRID_CATEGORY     "viewer_grid"

#define VIEWER_GRID_ID     90
#define HCP_VOXEL_ID       100
#define SURFACE_ID         101
#define POINT_CLOUD_ID     102
#define CAMERA_ID          103

// ------------------------------------------------------

enum class scene_object_type_enum { Node, Light, Camera, Render_object };
enum class geometry_type_enum { undefined,null,points,mesh, triangles,lines };
