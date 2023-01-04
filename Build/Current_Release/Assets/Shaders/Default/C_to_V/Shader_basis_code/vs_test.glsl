#version 450 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in int  value;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	
	//gl_Position = projection * view * model * vec4(vertex, 1.0f);
	gl_Position = vec4(vertex, 1.0f);
}