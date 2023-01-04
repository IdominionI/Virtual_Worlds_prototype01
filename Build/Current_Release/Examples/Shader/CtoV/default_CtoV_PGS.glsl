#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "Shader_basis_code/pgs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------


void main(){
	vec4 center = gl_in[0].gl_Position; 
	
	
	gs_out.varyingColor = gs_in[0].varyingColor;
    //gl_Position = projection * view * model * center;
    gl_Position = mvpMatrix * center;
	
	//varyingColor = vec4(1.0,1.0,1.0,1.0);

    EmitVertex();
    EndPrimitive();

}
