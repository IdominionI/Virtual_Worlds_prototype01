// NULL Geometry Shader file

#version 410

layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // This required to display a voxel of 12 faces and 4 vertices per face
layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point
//layout(points) out;    // This required to display a voxel center of 1 point

flat in int gEnabledFaces[];// Must have as flat array otherwise will not work

flat in float voxSize[];
uniform mat4 mvpMatrix; // ************model-view-projection matrix 

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

void main() {
	vec4 center = gl_in[0].gl_Position;

	gs_out.varyingColor = gs_in[0].varyingColor;
	gl_Position = mvpMatrix*(center+vec4(0.0,0.0,0.0,1.0));
	EmitVertex();
	EndPrimitive();

/*
	int i;
    for(i = 0; i < gl_in.length(); i++)
    {
		gs_out.varyingColor = gs_in[0].varyingColor;
        gl_Position = mvpMatrix*gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
	*/
	
}
