#version 410

//in vec4 varyingColor;// color variable to accept from the vertex buffer to renmder
			
//layout(location = 1) in vec3 nearPoint; // nearPoint calculated in vertex shader
//layout(location = 2) in vec3 farPoint; // farPoint calculated in vertex shader

in vec3 nearPoint;
in vec3 farPoint;

out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}

void main(){
	//if(farPoint == 0.0)
    //    outColor = vec4(1.0f,1.0f,1.0f,1.0f);
	//else
	//	 outColor = vec4(0.0f,1.0f,1.0f,1.0f);

   //gl_FragColor = varyingColor;
   
    //float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    //outColor = vec4(1.0, 1.0, 1.0, 1.0 * float(t > 0)); // op
	
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
	//outColor = vec4(1.0, 1.0, 1.0, 1.0 * float(t > 0)); // op
    outColor = grid(fragPos3D, 10) * float(t > 0);
}