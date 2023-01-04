
//in vec4 vertex;        //vertex that the shader is processing
//in vec3 normal;        //vertex that the shader is processing

layout(location = 0) in vec3 vertex; //******
layout(location = 1) in vec3 normal; //******

#include "universal_reserved_uniforms.glsl"

// hex surface parameter data
uniform vec2  grid_origin;

//in float hex_size;
//flat out float hexSize; 
uniform float hexSize; 

//hex surface display data
uniform float hex_min_surface_display_value;
uniform float hex_max_surface_display_value;


// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
//  float  value;
  vec4 varyingColor;
} vs_out;

vec4 raw_color;

// ******* BEGIN TESTING PARAMETER TRANSFER
in int shader_parameters_frame;

//uniform vec2  origin;
uniform vec2  resolution;

uniform int display_as_points;
 //color of vertex that shader is processing
uniform vec4  color;			       
uniform int  display_native_vertex_colors;

// ************************************************************


