#version 450 core
layout(location = 0) in vec3 vertex;

uniform mat4 mvpMatrix; //model-view-projection matrix

void main() {
   gl_Position = mvpMatrix*vec4(vertex, 1.0f);        // required as a min

 
}
