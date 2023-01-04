#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "Shader_basis_code/vs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------
float r;
float g;
float b;

float height;

vec4 density_color(float density){
   height = 2.0;

   if(density >= min_density && density <= max_density)
	   height = (density - min_density)/(max_density-min_density);

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
//	return vec4(1.0,0.0,0.0,1.0);
}

void main(){
	
	gl_Position = vec4(vertex.x,vertex.y,vertex.z,1.0f); // required as a min

	value = int(vertex.w);              // required as a min

	vs_out.varyingColor = density_color(vertex.w);
	//vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);

}