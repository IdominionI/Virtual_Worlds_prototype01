#version 450 core
layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // This required to display a voxel of 12 faces and 4 vertices per face
layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point
//layout(points) out;    // This required to display a voxel center of 1 point

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main() {    
    //gl_Position = gl_in[0].gl_Position; 
	
	vec4 center = gl_in[0].gl_Position; 
	
    gl_Position = projection * view * model * center; 
    //gl_Position = projection * view * model * gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}