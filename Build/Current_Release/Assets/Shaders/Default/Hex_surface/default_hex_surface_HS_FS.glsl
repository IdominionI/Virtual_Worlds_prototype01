#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/universal_reserved_uniforms.glsl"

#include "shader_basis_code/fs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------
void main(){

   //gl_FragColor = fs_in.varyingColor;
   frag_color = fs_in.varyingColor;
   
   
}