begin_function

float color_map_type;

float x;
float y;
float z;

float r;
float g;
float b;

// Function definitions
//vec4 map_distance_color();
vec4 map_height_color();
void use_lighting();
/*
// Distance color mapping
//uniform float min_distance;
//uniform float max_distance;
float distance;

vec4  map_distance_color(){
   x = camera_loc.x - vertex.x;
   y = camera_loc.y - vertex.y;
   z = camera_loc.z - vertex.z;

   distance = (sqrt(x*x + y*y + z*z)-min_distance)/(max_distance-min_distance);

  if(distance <= 0.2){
      g = distance/0.2;
      r = 1.0;
      b = 0.0;
  } else {
      if(distance > 0.2 && distance <= 0.4){
          b = 0.0;
          r = (distance-0.4)/0.2;
          g = 1.0;
       } else {
          if(distance > 0.4 && distance <= 0.6){
              b = (distance-0.4)/0.2;
              g = 1.0;
              r = 0.0;
          } else {
              if(distance > 0.6 && distance <= .80){
                  g = (0.8 - distance)/0.2;
                  b = 1.0;
                  r = 0.0;
          } else {
              if(distance <= 1.0){
                  r = (distance-0.8)/0.2;
                  b = 1.0;
                  g = 0.0;
              } else {
                   r = 0.5;
                   g = 0.5;
                   b = 0.5;
                }
         }
         }
         }
     }
     return  vec4(r,g,b,1.0);
//     varyingColor = vec4(r,g,b,1.0);
}
*/
//height color mapping variables 
//uniform float height_map_axis;
//uniform float min_height;
//uniform float max_height;
//float height_map_axis = int_va[1];
//float height_map_axis = t_c_time;
float height_map_axis = 0.0;
//float min_height      = float_va[0];
//float max_height      = float_va[1];
float height;

vec4  map_height_color(){
   height = 2.0;
   if(height_map_axis <= 0.0){ // x axis
      if(vertex.x >= min_height && vertex.x <= max_height)
          height = (vertex.x - min_height)/(max_height-min_height);
   } else {
      if(height_map_axis <= 1.0){ // y axis
        if(vertex.y >= min_height && vertex.y <= max_height)
            height = (vertex.y - min_height)/(max_height-min_height);
        } else {
            if(height_map_axis <= 2.0){ // z axis
               if(vertex.z >= min_height && vertex.z <= max_height)
                   height = (vertex.z - min_height)/(max_height-min_height);
            }
        }
   }

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

vec4 get_vertex_raw_color(){
	color_map_type = 2;

   //if(display_native_vertex_colors != 0){
   //if(n_colors != 0){ // user boolean defined uniform variable
   //      return color;
   //  } else {
         if(color_map_type == 1){
            //return map_distance_color();
            return vec4(0.0,.5,.5,1.0);
	      } else {
            if(color_map_type == 2){
             return map_height_color();
	        } else {
				return light_color;
			}
         }
    //}
}


end_function

begin_expression
	
	vertex = vec3(vertex_data.x,vertex_data.y,vertex_data.z);// required as a min
	
	gl_Position = vec4(vertex, 1.0f);             // required as a min
	value = int(vertex_data.w);                   // required as a min
	//vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
	
	vs_out.varyingColor = get_vertex_raw_color();

end_expression