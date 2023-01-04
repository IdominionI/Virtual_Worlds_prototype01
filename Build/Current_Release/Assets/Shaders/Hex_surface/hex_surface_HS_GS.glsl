begin_function

// A layout for the output must be defined for whatever geometry shader functions are performed
//layout(triangle_strip, max_vertices = 3) out; // The output needs to be defined by the user in the function definitions
layout(points, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions

void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){
     vec3 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec3 light_intensity;
	 vec3 camera_light_intensity;
	 vec3 light_direction_vector;
	 
     NdotL   = max(dot(normalize(vertex_normal), normalize(-lighting_direction)), 0.0);
	 //NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
     diffuse = NdotL * light_color;

     viewDir    = normalize(camera_loc - vec3(vertex.xyz));
     reflectDir = reflect(normalize(-lighting_direction), vertex_normal);
	 //reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

     //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
     vec3 specular = specular_strength * spec * light_color;

	 //light_intensity = light_intensity + (ambient_light + diffuse + specular);
	 light_intensity = (ambient_light + diffuse + specular)*lighting_intensity*0.5;//*.25

	 camera_light_intensity = vec3(0.0,0.0,0.0);
	 if(use_camera_lighting){
		 if(camera_lighting_type == 0){
			 vec3 light_camera_relative_x = camera_right_vector *lighting_camera_offset.x;
			 vec3 light_camera_relative_y = camera_up_vector    *lighting_camera_offset.y;
			 vec3 light_camera_relative_z = camera_front_vector *lighting_camera_offset.z;
			 
			 vec3 light_loc =  camera_loc  + light_camera_relative_x + light_camera_relative_y + light_camera_relative_z;
			 
			 light_direction_vector = -(light_loc - vertex.xyz);// spot light from camera pointing dir of camera
		 } else
			 light_direction_vector = (camera_front_vector);//  global light pointing in direction of camera : not good
		 
		 NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
		 diffuse = NdotL * light_color;

		 viewDir    = normalize(camera_loc - vec3(vertex.xyz));
		 reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

		 //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
		 vec3 specular = specular_strength * spec * light_color;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
	 }
	 
     gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
}


float xp = 0.866025;


vec4 point_0 = vec4(0.0,0.0,0.0,1.0) * hexSize[0];
vec4 point_1 = vec4(0.0,1.0,0.0,1.0) * hexSize[0];
vec4 point_2 = vec4(xp,0.5,0.0,1.0)  * hexSize[0];
vec4 point_3 = vec4(xp,-0.5,0.0,1.0) * hexSize[0];
vec4 point_4 = vec4(0.0,-1.0,0.0,1.0)* hexSize[0];
vec4 point_5 = vec4(-xp,-0.5,0.0,1.0)* hexSize[0];
vec4 point_6 = vec4(-xp,0.5,0.0,1.0) * hexSize[0];

void add_top(vec4 center){

	vec3 vertex_normal = vec3(0.0,0.0,1.0);
			
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_2);
	EmitVertex();
	
	EndPrimitive();
}

end_function

begin_expression

	vec4 center = gl_in[0].gl_Position;

	//add_top(center); // top surface element
	
	gs_out.varyingColor = gs_in[0].varyingColor;
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	EndPrimitive();
	
end_expression