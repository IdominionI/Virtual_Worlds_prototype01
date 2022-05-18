#version 410

uniform mat4 mvpMatrix; //model-view-projection matrix
in vec4 vertex;        //vertex that the shader is processing

//in uint voxel_value; // Value assigned to Vertex
//in float voxel_value; // Value assigned to Vertex
//in float voxel_color; // Value assigned to Vertex

// Voxel parameter data
//uniform ivec3 voxel_matrix_dimension;
//uniform vec3  voxel_origin;
//uniform float voxel_hcp_z_increment;

//uniform ivec3 voxel_matrix_dimension;
uniform vec2  grid_origin;
//uniform float voxel_hcp_z_increment;

//in float voxel_size;
in float hex_size;
flat out float hexSize; 

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

uniform float hex_min_surface_display_value;
uniform float hex_max_surface_display_value;

// Camera data
uniform vec3  camera_loc;         // Camera location

uniform bool  use_camera_lighting;
uniform vec3  lighting_camera_offset;
uniform int   camera_lighting_type;
uniform float lighting_intensity;
uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;

//varying out vec4 varyingColor;  //output a varying color for the fragment shader to use for this vertex
//out vec4 varyingColor;  //output a varying color for the fragment shader to use for this vertex

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
  float  value;
  vec4 varyingColor;
} vs_out;

vec4 raw_color;

// ******* BEGIN TESTING PARAMETER TRANSFER
in int shader_parameters_frame;


// ******* define uniform variables to use in shader ********
// *******       DO NOT DELETE OR CHANGE NAMES       ********

#define TRUE  0
#define FALSE 1


uniform int   t_frame;
uniform float t_e_time;
uniform float t_c_time;

uniform vec2  origin;
uniform vec2  resolution;

uniform float float_va[10];
uniform int   int_va[10];
uniform int   bool_va[10];

// ************************************************************

// ******* END TESTING PARAMETER TRANSFER

in int vEnabledFaces; // ********** NEW attribute to be defined ***********
flat out int gEnabledFaces; // **********

// Lighting and light shading
uniform bool  display_vertex_light_shading;
uniform vec3  lighting_direction;
uniform vec3  ambient_light;
uniform float specular_strength;
uniform vec3  light_color;

//vec3 light_color2 = vec3(1.0,1.0,1.0);// to in

 //color of vertex that shader is processing
uniform vec4  color;			       
uniform int  display_native_vertex_colors;
//uniform float color_map_type;

// Distance color mapping
//uniform float min_distance;
//uniform float max_distance;


void main(){
   hexSize = hex_size;

   //gl_Position = vertex;
   gl_Position = mvpMatrix*(vertex+vec4(0.0,0.0,0.0,1.0));
   
   raw_color = color;

   vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
   //vs_out.varyingColor = raw_color;

   vs_out.value        = vertex.z;


   gEnabledFaces = vEnabledFaces;
 }
