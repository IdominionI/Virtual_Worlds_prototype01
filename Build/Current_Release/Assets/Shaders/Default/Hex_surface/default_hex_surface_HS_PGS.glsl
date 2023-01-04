#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/universal_reserved_uniforms.glsl"

#include "shader_basis_code/pgs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------

// A layout for the output must be defined for whatever geometry shader functions are performed
layout(points, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions

vec4 point_0 = vec4(0.0,0.0,0.0,1.0);


void main(){

	vec4 center = gl_in[0].gl_Position;
	
	gs_out.varyingColor = gs_in[0].varyingColor;
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	EndPrimitive();
	
}