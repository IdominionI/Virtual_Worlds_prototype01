#version 410

layout(location = 0) in vec3 vertex;
//layout(location = 1) in int  voxel_value;

uniform mat4 mvpMatrix; //model-view-projection matrix

// Camera data
uniform vec3  camera_loc;         // Camera location

uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;

uniform vec4 grid_color; 
uniform vec4 xy_grid_color;
uniform vec4 xz_grid_color;
uniform vec4 yz_grid_color;

uniform	int display_xy_plane;
uniform	int display_xz_plane;
uniform	int display_yz_plane;

uniform	int relative_xy_grid;
uniform	int relative_xz_grid;
uniform	int relative_yz_grid;

uniform	float relative_xy_grid_dist;
uniform	float relative_xz_grid_dist;
uniform	float relative_yz_grid_dist;

void main () {
	
	gl_Position = vec4(vertex, 1.0f); // required as a min

}


