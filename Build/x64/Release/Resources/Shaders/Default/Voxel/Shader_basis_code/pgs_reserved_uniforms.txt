layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // This required to display a voxel of 12 faces and 4 vertices per face
layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point
//layout(points) out;    // This required to display a voxel center of 1 point

in int value[]; // Voxel value :  Critical : Must have defined as an array 

uniform mat4 mvpMatrix; //model-view-projection matrix

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//in int value;// this causes shader problemsS

out  vec4 fvaryingColor;

// reserved light variables
uniform vec4  light_color; // ****
uniform vec3  lighting_direction; // ****
uniform float ambience; //****
uniform float specular_strength; //****
uniform float lighting_intensity; //****


// reserved camera light variables
uniform int   camera_lighting_type; //****
uniform int   use_camera_lighting; //****
uniform vec3  lighting_camera_offset; //****

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Camera data
uniform vec3  camera_loc;         // Camera location

uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;


uniform int   t_frame;
 
uniform float voxSize;

uniform vec3  voxel_origin;
uniform float voxel_hcp_z_increment;

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

// -----------------------------------

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following input/output block is to allow  varyingColor 
// to be passed to the fragment shader or without changing the name

in VertexData
{
  vec4 varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

out VertexData
{
  vec4 varyingColor;
} gs_out;

