#pragma once

/*
	unversal defined paramter data that is to be used in multiple header files and/or classes
*/

#include <string>

typedef std::string  string_type;
typedef unsigned int idu_type;
typedef int          id_type;
typedef unsigned int index_type;


// !!!!!!!!!!!!!!!!!!! SETS !!!!!!!!!!!!!!!!!!!!!!!!!!
// Entity category definitions
#define NULL_CATEGORY_ID            UINT_MAX

#define VIEWER_GRID_CATEGORY     "viewer_grid"

#define VIEWER_GRID_ID     90

// ------------------------------------------------------

enum class scene_object_type_enum { Node, Light, Camera, Render_object };
enum class geometry_type_enum { undefined,null,points,mesh, triangles,lines };
