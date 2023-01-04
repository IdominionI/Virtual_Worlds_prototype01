#version 410

layout (points) in;
layout (line_strip, max_vertices = 12) out;

uniform mat4 mvpMatrix; //model-view-projection matrix

// Camera data
uniform vec3  camera_loc;         // Camera location

uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;

uniform vec4 grid_color; 
uniform vec4 xy_grid_color;
uniform vec4 xz_grid_color;
uniform vec4 yz_grid_color;

uniform	int display_xy_plane;
uniform	int display_xz_plane;
uniform	int display_yz_plane;

uniform	int relative_xy_grid;
uniform	int relative_xz_grid;
uniform	int relative_yz_grid;

uniform	float relative_xy_grid_dist;
uniform	float relative_xz_grid_dist;
uniform	float relative_yz_grid_dist;

out vec4 varyingColor;

in vec3 nearPoint;
in vec3 farPoint;

out vec3 gnearPoint;
out vec3 gfarPoint;

void main (){
	vec4 x_center = gl_in[0].gl_Position;
	vec4 y_center = vec4(0.0,gl_in[0].gl_Position.x,0.0,1.0);
	vec4 z_center = vec4(0.0,0.0,gl_in[0].gl_Position.x,1.0);

	gnearPoint = nearPoint;
	gfarPoint  = farPoint;

	// ****************** X-Y PLANE *******************
	if(display_xy_plane != 0){
		float xy_dist = 0.0f;
		if(relative_xy_grid !=0)
			xy_dist = camera_loc.z + relative_xy_grid_dist;
			//xy_dist = camera_loc.z + -3.0f;
		
		varyingColor = xy_grid_color;
		gl_Position = mvpMatrix*(x_center+vec4(0.0, -1.0, 0.0+xy_dist, 0.0)); 
		EmitVertex();

		varyingColor = xy_grid_color;
		gl_Position =  mvpMatrix*(x_center+vec4( 0.0, 1.0, 0.0+xy_dist, 0.0));
		EmitVertex();
		
		EndPrimitive();

		varyingColor = xy_grid_color;
		gl_Position = mvpMatrix*(y_center+vec4(-1.0,0.0, 0.0+xy_dist, 0.0)); 
		EmitVertex();

		varyingColor = xy_grid_color;
		gl_Position =  mvpMatrix*(y_center+vec4(1.0,0.0, 0.0+xy_dist, 0.0));
		EmitVertex();
		
		EndPrimitive();
	}

	// ****************** X-Z PLANE *******************
	if(display_xz_plane !=0){
		float xz_dist = 0.0f;
		if(relative_xz_grid !=0)
			xz_dist = camera_loc.y + relative_xz_grid_dist;
		
		varyingColor = xz_grid_color;
		gl_Position = mvpMatrix*(x_center+vec4( 0.0, 0.0+xz_dist, -1.0,0.0)); 
		EmitVertex();

		varyingColor = xz_grid_color;
		gl_Position =  mvpMatrix*(x_center+vec4( 0.0, 0.0+xz_dist, 1.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
		
		varyingColor = xz_grid_color;
		gl_Position = mvpMatrix*(z_center+vec4(-1.0, 0.0+xz_dist, 0.0, 0.0)); 
		EmitVertex();

		varyingColor = xz_grid_color;
		gl_Position =  mvpMatrix*(z_center+vec4(1.0, 0.0+xz_dist, 0.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
	}

	// ****************** Y-Z PLANE *******************
	if(display_yz_plane !=0){
		float yz_dist = 0.0f;
		if(relative_yz_grid !=0)
			yz_dist = camera_loc.x + relative_yz_grid_dist;
		
		varyingColor = yz_grid_color;
		gl_Position = mvpMatrix*(z_center+vec4(0.0+yz_dist,-1.0, 0.0, 0.0)); 
		EmitVertex();

		varyingColor = yz_grid_color;
		gl_Position =  mvpMatrix*(z_center+vec4(0.0+yz_dist,1.0, 0.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
		
		varyingColor = yz_grid_color;
		gl_Position = mvpMatrix*(y_center+vec4(0.0+yz_dist, 0.0,-1.0, 0.0)); 
		EmitVertex();

		varyingColor = yz_grid_color;
		gl_Position =  mvpMatrix*(y_center+vec4(0.0+yz_dist, 0.0,1.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
	}
}