begin_function

// reserved light variables
//uniform vec4  light_color; // ****
//uniform vec3  lighting_direction; // ****
//uniform float ambience; //****
//uniform float specular_strength; //****
//uniform float light_intensity; //****


// reserved camera light variables
//uniform int   type_camera_light; //****
//uniform int   camera_as_light; //****
//uniform vec3  camera_light_offset; //****

//in int value[];// Critical : Must have defined as an array

end_function

begin_expression
	vec4 center = gl_in[0].gl_Position; 
	
    //gl_Position = projection * view * model * center;
    gl_Position = mvpMatrix * center;
	
	int v = value[0];

	//varyingColor = vec4(0.0,1.0,0.0,1.0);
	//varyingColor = vec4(a,1.0,0.0,1.0);
	fSSvaryingColor = vec4(lighting_camera_offset.x/10.0,lighting_camera_offset.y/10.0,lighting_camera_offset.z/10.0,1.0);
	//varyingColor = vec4(light_color.r,light_color.g,light_color.b,light_color.a);

    EmitVertex();
    EndPrimitive();
	
end_expression
