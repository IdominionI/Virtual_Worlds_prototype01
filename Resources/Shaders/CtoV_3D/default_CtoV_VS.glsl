begin_function

float color_map_type;

float x;
float y;
float z;

float r;
float g;
float b;

// Function definitions
vec4 map_density_color();
void use_lighting();
float density;

vec4  map_density_color(){
	float voxel_value = vertex_data.w;

	if(max_density <= min_density){
		return vec4(0.0,0.0,0.0,0.0);
	}
   
   float range = max_density-min_density;

	if(voxel_value >= min_density && voxel_value <= max_density){
		//density = 0.5f;
		density = (voxel_value-min_density)/range;
	} else{
		density= 0.0f;
	}

	r= density; g=density;b=density;

// RGB color cube color space along cube edges 
  if(density <= 0.1666667){
      r = (0.1666667-density)/0.1666667;
      g = 0;
      b = 1.0;
  } else {
      if(density > 0.1666667 && density <= 0.3333333){
          r = 0.0;
          g = (density-0.1666667)/0.1666667;
          b = 1.0;
       } else {
          if(density > 0.3333333 && density <= 0.5){
              b = (0.5-density)/0.1666667;
              r = 0.0;
              g = 1.0;
           } else {
               if(density > 0.5 && density <= 0.6666667){
                   b = 0;
                   r = (density-0.5)/0.1666667;
                   g = 1.0;
               } else {
                    if(density > 0.6666667 && density <= 0.8333333){
                        b = 0;
                        r = 1.0;
                        g = (0.8333333-density)/0.1666667;
                    } else {
                         if(density <= 1.0){
                             r = 1.0;
                             b = (density-0.8333333)/0.1666667;
                             g = (density-0.8333333)/0.1666667;
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

vec4 get_vertex_raw_color(){

    return map_density_color();
}


end_function

begin_expression

	//gl_Position = vertex; // required as a min
	//raw_color = get_vertex_raw_color();
	
	vertex = vec3(vertex_data.x,vertex_data.y,vertex_data.z);
	
	gl_Position = vec4(vertex, 1.0f); // required as a min
	//value = voxel_value;            // required as a min
	value = int(vertex_data.w);       // required as a min
	//vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
	
	vs_out.varyingColor = get_vertex_raw_color();

end_expression