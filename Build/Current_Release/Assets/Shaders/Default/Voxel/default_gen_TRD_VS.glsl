#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"

// Comment uncomment following as needed
//layout(location = 1) in vec4 color;
//layout(location = 2) in vec3 nomrmal;



// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------

void main(){
	vertex = vec3(vertex_data.x,vertex_data.y,vertex_data.z);
	
	gl_Position = vec4(vertex, 1.0f);        // required as a min
	//gl_Position = mvpMatrix* vec4(vertex, 1.0f);        // required as a min if not using geometry shader program
	value = int(vertex_data.w);              // required as a min

	vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);

}