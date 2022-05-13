#pragma once

#include <Source/Graphics_Engine/Common/definitions.h>

// Type definitions of application entity attributes
typedef id_type     object_id_type;
typedef id_type     category_id_type;
typedef string_type name_type;
typedef string_type description_type;
typedef string_type pathname_type;

// Defines of application entity selection
#define SELECTED_INDEX  0
#define ACTIVATED_INDEX 1
#define ALL_INDEX       2

// id for outliner group node
#define GROUP_CATEGORY			 -200

// #### id of all entity or object types to be used ####
// #### through out application to be defined here  ####
#define ENTITY_CATEGORY_GRID      190 // Mandetory

//      Module entity data type ID here
#define ENTITY_CATEGORY_HCP_VOXEL 200
#define ENTITY_CATEGORY_HCP_SURF  210
//  add Module entity data type ID here

//!!!!! THESE BELOW MAY NEED TO BE NOT USED !!!!!!!!
//      Module entity category data type descriptions and ID here
#define HCP_VOXEL_CATEGORY       "hcp_Voxel"
#define HEX_SURFACE_CATEGORY     "hex_surface"
//      Add Module entity category data type descriptions and IDs here

//#define POINT_CLOUD_CATEGORY     "point_cloud"
//#define CAMERA_CATEGORY          "camera"

// Animation 
struct animation_texture_model_parameters_struct_type {
	int frame_step_start, frame_step_end, frame_step_interval, current_frame = 0;
};

// Defines of application animation entity selection
#define ANIMATION_SELECTED_INDEX  0
#define ANIMATION_ACTIVATED_INDEX 1
#define ANIMATION_ALL_INDEX       2

// General application definitions
#define DECREMENT_STEP -1
#define INCREMENT_STEP  1
