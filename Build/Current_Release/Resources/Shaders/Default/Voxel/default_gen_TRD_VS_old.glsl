#version 410

uniform mat4 mvpMatrix; //model-view-projection matrix
in vec4 vertex;         //vertex that the shader is processing

//in uint voxel_value; // Value assigned to Vertex
in float voxel_value; // Value assigned to Vertex
in float voxel_color; // Value assigned to Vertex

// Voxel parameter data
uniform ivec3 voxel_matrix_dimension;
uniform vec3  voxel_origin;
uniform float voxel_hcp_z_increment;

in float voxel_size;
flat out float voxSize; 

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

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

//struct float_parameters_struct_type
//{
//	int   float_id;
//	float float_value;
//};

//in float_parameters_struct_type shader_parameters[20];

//in parameters
//{
//	float_parameters_struct_type shader_parameters[20];
//} vs_in_parameters[];

//out parameters
//{
//	float_parameters_struct_type shader_parameters[20];
//} vs_out_parameters;


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

float color_map_type;

float x;
float y;
float z;

float r;
float g;
float b;

// Function definitions
vec4 map_distance_color();
vec4 map_height_color();
void use_lighting();

// Distance color mapping
uniform float min_distance;
uniform float max_distance;
float distance;

vec4  map_distance_color(){
   x = camera_loc.x - vertex.x;
   y = camera_loc.y - vertex.y;
   z = camera_loc.z - vertex.z;

   distance = (sqrt(x*x + y*y + z*z)-min_distance)/(max_distance-min_distance);

  if(distance <= 0.2){
      g = distance/0.2;
      r = 1.0;
      b = 0.0;
  } else {
      if(distance > 0.2 && distance <= 0.4){
          b = 0.0;
          r = (distance-0.4)/0.2;
          g = 1.0;
       } else {
          if(distance > 0.4 && distance <= 0.6){
              b = (distance-0.4)/0.2;
              g = 1.0;
              r = 0.0;
          } else {
              if(distance > 0.6 && distance <= .80){
                  g = (0.8 - distance)/0.2;
                  b = 1.0;
                  r = 0.0;
          } else {
              if(distance <= 1.0){
                  r = (distance-0.8)/0.2;
                  b = 1.0;
                  g = 0.0;
              } else {
                   r = 0.5;
                   g = 0.5;
                   b = 0.5;
                }
         }
         }
         }
     }
     return  vec4(r,g,b,1.0);
//     varyingColor = vec4(r,g,b,1.0);
}

//height color mapping variables 
//uniform float height_map_axis;
//uniform float min_height;
//uniform float max_height;
//float height_map_axis = int_va[1];
float height_map_axis = t_c_time;
//float min_height      = float_va[0];
//float max_height      = float_va[1];
float height;

vec4  map_height_color(){
   height = 2.0;
   if(height_map_axis <= 0.0){ // x axis
      if(vertex.x >= min_height && vertex.x <= max_height)
          height = (vertex.x - min_height)/(max_height-min_height);
   } else {
      if(height_map_axis <= 1.0){ // y axis
        if(vertex.y >= min_height && vertex.y <= max_height)
            height = (vertex.y - min_height)/(max_height-min_height);
        } else {
            if(height_map_axis <= 2.0){ // z axis
               if(vertex.z >= min_height && vertex.z <= max_height)
                   height = (vertex.z - min_height)/(max_height-min_height);
            }
        }
   }

// RGB color cube color space along cube edges 
  if(height <= 0.1666667){
      r = (0.1666667-height)/0.1666667;
      g = 0;
      b = 1.0;
  } else {
      if(height > 0.1666667 && height <= 0.3333333){
          r = 0.0;
          g = (height-0.1666667)/0.1666667;
          b = 1.0;
       } else {
          if(height > 0.3333333 && height <= 0.5){
              b = (0.5-height)/0.1666667;
              r = 0.0;
              g = 1.0;
           } else {
               if(height > 0.5 && height <= 0.6666667){
                   b = 0;
                   r = (height-0.5)/0.1666667;
                   g = 1.0;
               } else {
                    if(height > 0.6666667 && height <= 0.8333333){
                        b = 0;
                        r = 1.0;
                        g = (0.8333333-height)/0.1666667;
                    } else {
                         if(height <= 1.0){
                             r = 1.0;
                             b = (height-0.8333333)/0.1666667;
                             g = (height-0.8333333)/0.1666667;
                } else {
                  r = 0.5;
                  g = 0.5;
                  b = 0.5;
              }
              }
              }
         }
         }
     }
     return vec4(r,g,b,1.0);
//     varyingColor = vec4(r,g,b,1.0);
}

vec4 get_vertex_raw_color(){
	color_map_type = int_va[0];

   if(display_native_vertex_colors != 0){
         return color;
     } else {
         if(color_map_type == 1){
            return map_distance_color();
	      } else {
            if(color_map_type == 2){
             return map_height_color();
	        } else {
				return color;
			}
         }
    }
}

// ******************* following only works for RGB values *********************
// *********** Obtained method from question and answer posted on stackoverflow but only is valid for first three RGB color values *************
const vec4 bitEnc = vec4(1.,255.,65025.,16581375.);
const vec4 bitDec = 1./bitEnc;
vec4 encode_float_to_RGB (float v) {
    vec4 enc = bitEnc * v;
    enc = fract(enc);
    enc -= enc.yzww * vec2(1./255., 0.).xxxy;
    return enc;
}

//float DecodeFloatRGBA (vec4 v) {
//    return dot(v, bitDec);
//}
// ******************************************


void main(){
   voxSize = voxel_size;

   vec4 rgb = encode_float_to_RGB (voxel_color);
   
   //if(rgb.b*255.0 >= 179.0 && rgb.b*255.0 <= 181.0){  // value failed
   if( voxel_value >= 1.0 ) { // && rgba.w*256.0 <= 91.0){  // value failed

   gl_Position = vertex;
   
   raw_color = get_vertex_raw_color();
   //raw_color = vec4(1.0,1.0,1.0,1.0);
   
   //float fvalue;
   //if(value == 1.0)
	//	fvalue = 1.0;//float(value)/255.0;
	//else
	//	fvalue = 0.0;

	//raw_color = vec4(0.5,0.5,0.5,1.0);// testing only
    //raw_color = vec4(value/255.0,value/255.0,value/255.0,1.0);// testing only
   
   //vs_out_parameters.shader_parameters[1].float_id = 1;
   //vs_out_parameters.shader_parameters[1].float_value =  shader_parameters[1].float_value;

   vs_out.varyingColor = raw_color;
   //vs_out.varyingColor = rgb;
   vs_out.value        = voxel_value;
   }

   gEnabledFaces = vEnabledFaces;
 }
