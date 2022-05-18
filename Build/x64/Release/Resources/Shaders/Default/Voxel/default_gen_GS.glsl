// NULL Geometry Shader file

#version 410

#define TRUE  0
#define FALSE 1

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

flat in int gEnabledFaces[];// Must have as flat array otherwise will not work

flat in float voxSize[];
uniform mat4 mvpMatrix; // ************model-view-projection matrix 

uniform vec3 camera_loc;         // Camera location

uniform bool use_camera_lighting;
uniform vec3 lighting_camera_offset;
uniform int  camera_lighting_type;
uniform float lighting_intensity;
uniform vec3 camera_front_vector;
uniform vec3 camera_up_vector;
uniform vec3 camera_right_vector;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following input/output block is to allow  varyingColor 
// to be passed to the fragment shader or without changing the name

in VertexData
{
  vec4 varyingColor;
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

     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
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

		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		 vec3 specular = specular_strength * spec * light_color;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
	 }

     gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
}

// The order in which the vertices are assigned is critical to whether
//  a quad or a triangleis drawn and on which side it can be viewed from.

void AddQuad(vec4 center, vec4 dy, vec4 dx,vec3 vertex_normal) {
	
	//gs_out.varyingColor = gs_in[0].varyingColor;
	//use_lighting(center, vertex_normal,gs_in[0].varyingColor);
    gl_Position = center + (dx - dy);
	use_lighting(gl_Position, vertex_normal,gs_in[0].varyingColor);
    EmitVertex();

	//use_lighting(center,vertex_normal,gs_in[0].varyingColor);
    gl_Position = center + (-dx - dy);
	use_lighting(gl_Position, vertex_normal,gs_in[0].varyingColor);
    EmitVertex();

	//use_lighting(center,vertex_normal,gs_in[0].varyingColor);
    gl_Position = center + (dx + dy);
	use_lighting(gl_Position, vertex_normal,gs_in[0].varyingColor);
    EmitVertex();

	//use_lighting(center,vertex_normal,gs_in[0].varyingColor);
    gl_Position = center + (-dx + dy);
	use_lighting(gl_Position, vertex_normal,gs_in[0].varyingColor);
    EmitVertex();

    EndPrimitive();
}

bool IsCulled(vec4 normal) {
    //return normal.z > 0;
	return false; // For now display all voxel sides
}

void main() {
    vec4 center = gl_in[0].gl_Position;

	vec4 dx = mvpMatrix[0] / 2.0f * voxSize[0];
    vec4 dy = mvpMatrix[1] / 2.0f * voxSize[0];
    vec4 dz = mvpMatrix[2] / 2.0f * voxSize[0];

	
	//if (gEnabledFaces[0] != 0 && !IsCulled(dx))
		//AddQuad(center + dx, dy, dz,normalize(dx.xyz));
		AddQuad(center + dx, dy, dz,vec3(1.0,0.0,0.0));
		
	//if (gEnabledFaces[0] != 0 && !IsCulled(-dx))
		 //AddQuad(center - dx, dz, dy,normalize(dx.xyz));
		 AddQuad(center - dx, dz, dy,vec3(-1.0,0.0,0.0));

	//if (gEnabledFaces[0] != 0 && !IsCulled(dy))
		 //AddQuad(center + dy, dz, dx,normalize(dy.xyz));
		 AddQuad(center + dy, dz, dx,vec3(0.0,1.0,0.0));

	// if (gEnabledFaces[0] != 0 && !IsCulled(-dy))
		//AddQuad(center - dy, dx, dz,normalize(-dy.xyz));
		AddQuad(center - dy, dx, dz,vec3(0.0,-1.0,0.0));

	// if (gEnabledFaces[0] != 0 && !IsCulled(dz))
		 //AddQuad(center + dz, dx, dy,normalize(dz.xyz));
		 AddQuad(center + dz, dx, dy,vec3(0.0,0.0,1.0));

	// if (gEnabledFaces[0] != 0 && !IsCulled(-dz))
		//AddQuad(center - dz, dy, dx,normalize(-dz.xyz));
		AddQuad(center - dz, dy, dx,vec3(0.0,0.0,-1.0));

}
