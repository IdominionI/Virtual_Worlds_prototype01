#pragma once

/*
	Definitions that define the parameters values and
	data types that define the generation and display
	of the 2D hex surface.
*/

//#define INVALID_HEX_VALUE 0
//#define MIN_HEX_VALUE     1
//#define MAX_HEX_VALUE     255

//#define DEFAULT_ACTIVE_VALUE   255
//#define DEFAULT_INACTIVE_VALUE 0 

#define INVALID_HEX_VALUE INT_MIN
#define MIN_HEX_VALUE     INT_MIN+1
#define MAX_HEX_VALUE     INT_MAX

#define DEFAULT_ACTIVE_HEX_VALUE   0
#define DEFAULT_INACTIVE_HEX_VALUE INT_MIN 

typedef int hex_surface_index_data_type;

struct hex_surface_index_data_type_index_struct_type {
	hex_surface_index_data_type x = 0, y = 0;
};

typedef hex_surface_index_data_type_index_struct_type hex_index_vector;

enum class hex_surface_data_storage_type_enum { red, green, blue, alpha, value };

typedef float surface_element_data_type;

// hex_surface_data_type and voxel_GLdata_type must be of the same equivalent data type 
typedef float   hex_surface_data_type;

//#define MAX_VOXEL_VERTICES    64000000 // On a 16 gig rig have this value as a safe number as testin reveals this uses about 10 gig memory.
#define MAX_HEX_SURFACE_VERTICES    64000000 // On a 16 gig rig have this value as a safe number as testin reveals this uses about 10 gig memory.