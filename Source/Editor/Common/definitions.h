#pragma once

#include <Source/Graphics_Engine/Common/definitions.h>

typedef id_type object_id_type;

//#include <QVector3D>
//#include <QColor>

//typedef QVector3D  vertex_data_type;
//typedef QVector3D  direction_vector_data_type;
//typedef QVector3D  rotation_data_type;

// QML components have only a text based variable objectName 
// that can be used as an id to a QML component that is displayed
// in the QML scenegraph

typedef id_type     category_id_type;
typedef string_type name_type;
typedef string_type description_type;
typedef string_type pathname_type;

#define SELECTED_INDEX  0
#define ACTIVATED_INDEX 1
#define ALL_INDEX       2

// id for outliner group node
#define GROUP_CATEGORY			 -200

// #### id of all entity or object types to be used ####
// #### throuout application to be defined here     ####
#define ENTITY_CATEGORY_GRID      190
#define ENTITY_CATEGORY_HCP_VOXEL 200
#define ENTITY_CATEGORY_HCP_SURF  210

//!!!!! THESE BELOW MAY NEED TO BE NOT USED !!!!!!!!!


#define HCP_VOXEL_CATEGORY       "hcp_Voxel"
#define HEX_SURFACE_CATEGORY     "hex_surface"
#define POINT_CLOUD_CATEGORY     "point_cloud"
#define CAMERA_CATEGORY          "camera"




// Animation 
struct animation_texture_model_parameters_struct_type {
	//int frame_step_start, frame_step_end, frame_step_interval;
	int frame_step_start, frame_step_end, frame_step_interval, current_frame = 0; // ***
};

#define ANIMATION_SELECTED_INDEX  0
#define ANIMATION_ACTIVATED_INDEX 1
#define ANIMATION_ALL_INDEX       2

#define DECREMENT_STEP -1
#define INCREMENT_STEP  1
