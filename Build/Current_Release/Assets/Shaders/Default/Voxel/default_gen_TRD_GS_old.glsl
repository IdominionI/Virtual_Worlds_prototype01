// NULL Geometry Shader file

#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 48) out;
// for points need the following
//layout(points, max_vertices = 1) out;


flat in int gEnabledFaces[];// Must have as flat array otherwise will not work

flat in float voxSize[];
uniform mat4 mvpMatrix; // ************model-view-projection matrix 

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

uniform vec3 camera_loc;   // Camera location
uniform bool use_camera_lighting;
uniform vec3 lighting_camera_offset;
uniform int  camera_lighting_type;
uniform float lighting_intensity;
uniform vec3 camera_front_vector;
uniform vec3 camera_up_vector;
uniform vec3 camera_right_vector;

uniform vec3 voxel_origin;
uniform float voxel_hcp_z_increment;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following input/output block is to allow  varyingColor 
// to be passed to the fragment shader or without changing the name

in VertexData
{
  float value;
  vec4  varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

// ******* BEGIN TESTING PARAMETER TRANSFER

struct float_parameters_struct_type
{
	int   float_id;
	float float_value;
};

in parameters
{
	float_parameters_struct_type float_parameters[100];
} gs_in_parameters_in[];


// ******* END TESTING PARAMETER TRANSFER

out VertexData
{
  vec4 varyingColor;
} gs_out;

// Lighting and light shading
uniform bool  display_vertex_light_shading;
uniform vec3  lighting_direction;
uniform vec3  ambient_light;
uniform float specular_strength;
uniform vec3  light_color;

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

float sqrt_3 = sqrt(3.0);
float sqrt_6 = sqrt(6.0);

vec4 point_0 = vec4(0.0,0.0,(1.0/sqrt_6+sqrt_6/3.0),0.0)* voxSize[0];
vec4 point_1 = vec4(0.0,2.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize[0];
vec4 point_2 = vec4(1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize[0];
vec4 point_3 = vec4(1.0,-1.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize[0];
vec4 point_4 = vec4(0.0,-2.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize[0];
vec4 point_5 = vec4(-1.0,-1.0/sqrt_3,(sqrt_6/3.0),0.0)* voxSize[0];
vec4 point_6 = vec4(-1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize[0];


// vec3 plane_normal(vec3 a, vec3 b, vec3 c){
	// vec3 ab = b-a;
	// vec3 ac = c-a;

	// return normalize(cross(ab,ac));
// }

void add_top_bottom(vec4 center, int top_bottom, int sector){
	
	// glsl has rounding and accuracy problems with converting float to int and need to add 0.5 so any values
	// very close to an integer value but just below it are not floored
	int hcp_layer = int ((center.z-voxel_origin.z)/voxel_hcp_z_increment+0.5); 
	
	if(top_bottom == 1){
		if(sector == 0 ){
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(-0.5,-0.2886751346,-0.8164965809);
			
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
				EmitVertex();
			
			} else{
				vec3 vertex_normal = -vec3(0.1889822365,0.3273268354,0.9258200998);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
				EmitVertex();
			}
			
			EndPrimitive();
	
		}
	
		if(sector == 1){
			vec4 color = vec4(0.0,1.0,1.0,1.0);
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(0.0,0.5773502692,-0.8164965809);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
				EmitVertex();
			} else{
				vec3 vertex_normal = -vec3(-0.1889822365,0.3273268354,0.9258200998);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
				EmitVertex();
				
									
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
				EmitVertex();

			}

			EndPrimitive();
		}
		
		if(sector == 2){
			vec4 color = vec4(1.0,1.0,0.0,1.0);
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(0.5,-0.2886751346,-0.8164965809);

				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
				EmitVertex();
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
				EmitVertex();

			} else{
				vec3 vertex_normal = -vec3(0.0,-0.5773502692,-0.8164965809);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
				EmitVertex();				
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();

				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
				EmitVertex();
	
			}
			
			EndPrimitive();
		}
	}
	
		if(top_bottom == -1){
			if(sector == 0 ){
					if(mod(hcp_layer,2) == 0){
						vec3 vertex_normal = -vec3(0.5,0.2886751346,-0.8164965809);
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
						EmitVertex();

					} else {
						vec3 vertex_normal = vec3(-0.1889822365,-0.3273268354,0.9258200998);
					
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_5.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_4.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_3.z,1.0));
						EmitVertex();

					}
				EndPrimitive();
			}
			
			if(sector == 1){
				if(mod(hcp_layer,2) == 0){
					vec3 vertex_normal =-vec3(0.5,0.2886751346,-0.8164965809);
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
					EmitVertex();
	
				} else{
					vec3 vertex_normal = vec3(0.1889822365,-0.3273268354,0.9258200998);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_1.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_6.z,1.0));
					EmitVertex();

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_5.z,1.0));
					EmitVertex();
				}
				EndPrimitive();
			}
			
			if(sector == 2){
				if(mod(hcp_layer,2) == 0){
					vec3 vertex_normal = -vec3(-0.5,0.2886751346,-0.8164965809);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
					EmitVertex();
				} else {
					vec3 vertex_normal = -vec3(0.0,0.5773502692,-0.8164965809);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_3.z,1.0));
					EmitVertex();					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);

					gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_2.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_1.z,1.0));
					EmitVertex();
					
				}
				EndPrimitive();
			}
	}

}

void add_side(vec4 center,int side){

	int hcp_layer = int ((center.z-voxel_origin.z)/voxel_hcp_z_increment+0.5);

	if(side == 0){
		vec3 vertex_normal = -vec3(-0.5,-0.8660254038,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);

		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_1.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_6.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
		EmitVertex();

		EndPrimitive();
	}
	
	if(side == 1){
		vec3 vertex_normal =-vec3(-1,0,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_2.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_1.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}
	
	if(side == 2){
		vec3 vertex_normal = -vec3(0.8660254038,0.5,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_3.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_2.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 3){
		vec3 vertex_normal = -vec3(-0.8660254038,0.5,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_4.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_3.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 4){
		vec3 vertex_normal = -vec3(1,0,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_5.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_4.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 5){
		vec3 vertex_normal = -vec3(0.5,-0.8660254038,0.0);

		use_lighting(center, -vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_6.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_5.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

}

/*
void add_point(vec4 center){
	//gs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
	gs_out.varyingColor = gs_in[0].varyingColor;
	
	gl_Position = mvpMatrix*(center+vec4(0.0,0.0,0.0,1.0));
	EmitVertex();
	
	
	
	EndPrimitive();
}
*/

void main() {
    vec4 center = gl_in[0].gl_Position;
	
	if (gEnabledFaces[0] != 0){
		if(gs_in[0].value >= voxel_min_surface_display_value && gs_in[0].value <= voxel_max_surface_display_value){
		
			 for(int sector = 0; sector<3;sector++){
				 add_top_bottom(center,1,sector); // top surface elelment
			 }
			
			for(int sector = 0; sector<3;sector++){
				 add_top_bottom(center,-1,sector); // bottom surface elelment
			}
			
			for(int side = 0; side<6;side++){
				add_side(center,side); // side surface elelment
			}
		}
	} //else {
		//add_point(center);
		//gs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
		//gs_out.varyingColor = gs_in[0].varyingColor;
		//gl_Position = mvpMatrix*(center+vec4(0.0,0.0,0.0,1.0));
		//EmitVertex();
		//EndPrimitive();
	//}
	
}
