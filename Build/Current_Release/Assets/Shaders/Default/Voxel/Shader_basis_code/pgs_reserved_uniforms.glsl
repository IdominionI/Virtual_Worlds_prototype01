layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // This required to display a voxel of 12 faces and 4 vertices per face
layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point
//layout(points) out;    // This required to display a voxel center of 1 point

in int value[]; // Voxel value :  Critical : Must have defined as an array 

out  vec4 fvaryingColor;

#include "universal_reserved_uniforms.glsl"

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



