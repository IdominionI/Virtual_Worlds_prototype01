layout(location = 0) in vec4 vertex_data;
vec3 vertex;
//layout(location = 0) in vec3 vertex;
//layout(location = 1) in int  voxel_value;

out int value;

#include "universal_reserved_uniforms.glsl"

uniform float voxSize;

// reserved Voxel parameter data
uniform ivec3 voxel_matrix_dimension;
uniform vec3  voxel_origin;
uniform float voxel_hcp_z_increment;

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

vec4 raw_color;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader, to then be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
 vec4 varyingColor;
} vs_out;