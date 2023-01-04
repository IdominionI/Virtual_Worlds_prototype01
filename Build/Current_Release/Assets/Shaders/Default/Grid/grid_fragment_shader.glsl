#version 410

in vec4 varyingColor;// color variable to accept from the vertex buffer to renmder
						 
void main(){
   //gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
   gl_FragColor = varyingColor;
}