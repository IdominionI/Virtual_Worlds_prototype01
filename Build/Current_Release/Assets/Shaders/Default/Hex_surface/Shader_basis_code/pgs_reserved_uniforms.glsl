layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // This required to display a voxel of 12 faces and 4 vertices per face
//layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point
//layout(points) out;    // This required to display a voxel center of 1 point

//flat in int gEnabledFaces[];// Must have as flat array otherwise will not work

#include "universal_reserved_uniforms.glsl"

//flat in float hexSize[];
uniform float hexSize;

//hex surface display data
uniform float hex_min_surface_display_value;
uniform float hex_max_surface_display_value;


uniform vec2  grid_origin;
uniform float voxel_hcp_z_increment;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following input/output block is to allow  varyingColor 
// to be passed to the fragment shader or without changing the name

in VertexData
{
 // float value;
  vec4 varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

out VertexData
{
//  float value;
  vec4 varyingColor;
} gs_out;

uniform vec2  resolution;