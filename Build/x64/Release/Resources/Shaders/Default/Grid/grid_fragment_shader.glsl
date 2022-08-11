#version 410

in vec4 varyingColor;// color variable to accept from the vertex buffer to renmder

layout(location = 0) out vec4 FragColor;// out put color
						 
void main(){
   //gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
   //gl_FragColor = varyingColor;
   FragColor = varyingColor;
}