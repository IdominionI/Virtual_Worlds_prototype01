begin_function


vec4 use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){
     vec3 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec3 light_intensity;
	 vec3 camera_light_intensity;
	 vec3 light_direction_vector;
	 
     NdotL   = max(dot(normalize(vertex_normal), normalize(-lighting_direction)), 0.0);
	 //NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
     diffuse = NdotL * light_color.xyz;

     viewDir    = normalize(camera_loc - vec3(vertex.xyz));
     reflectDir = reflect(normalize(-lighting_direction), vertex_normal);
	 //reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

     //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
     vec3 specular = specular_strength * spec * light_color.xyz;

	 //light_intensity = light_intensity + (ambient_light + diffuse + specular);
	 light_intensity = (ambience + diffuse + specular)*lighting_intensity*0.5;//*.25

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
		 diffuse = NdotL * light_color.xyz;

		 viewDir    = normalize(camera_loc - vec3(vertex.xyz));
		 reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

		 //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
		 vec3 specular = specular_strength * spec * light_color.xyz;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
	 }
	 
     //gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
     return vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
}


float x;
float y;
float z;

float r;
float g;
float b;

float height;

vec4  map_height_color(){
   height = 2.0;

   if(vertex.z >= min_height && vertex.z <= max_height)
	   height = (vertex.z - min_height)/(max_height-min_height);


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
}

end_function

begin_expression

	// following required as a min
	//if(vEnabledFaces != 0)
	if(display_as_points != 0)
		gl_Position = mvpMatrix*(vec4(vertex.x,vertex.y,vertex.z,1.0)); // required as a min
		//gl_Position = mvpMatrix*(vertex+vec4(0.0,0.0,0.0,1.0)); // required as a min
		//gl_Position = mvpMatrix*(vertex+vec4(a+b,0.0,0.0,1.0)); // required as a min
	else
		gl_Position = vec4(vertex.x,vertex.y,vertex.z,1.0) ; // required as a min
		//gl_Position = vertex ; // required as a min
		//gl_Position = vertex + vec4(a+b,0.0,0.0,0.0); // required as a min

	//raw_color = vec4(1.0,1.0,1.0,1.0); // required as a min
	
	raw_color = map_height_color();
	
	//vec4 h_color = map_height_color();
	
	//raw_color = use_lighting(gl_Position, normal, h_color);

end_expression