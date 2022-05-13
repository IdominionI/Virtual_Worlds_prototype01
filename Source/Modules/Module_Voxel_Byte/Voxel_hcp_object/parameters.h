#pragma once

/*
	Definitions that define the parameters values and
	data types that define the generation and display
	of a 3D HCP voxel volume.
*/

enum class data_storage_type_enum { red, green, blue, alpha, value };

//typedef uint8_t voxel_data_type;
typedef uint8_t voxel_element_data_type; // For color r,g,b, and alpha or voxel value data type

// voxel_data_type and voxel_GLdata_type must be of the same equivalent data type 
typedef int   voxel_data_type;   // CPU  
//typedef unsigned int   voxel_data_type;   // CPU  
//typedef GLuint voxel_GLdata_type; // GPU 

#define MAX_VOXEL_VERTICES    64000000 // On a 16 gig rig have this value as a safe number as testin reveals this uses about 10 gig memory.