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

int   x_level=0, y_level=0, z_level=0;
float x_grid_interval=1.0f,y_grid_interval=1.0f,z_grid_interval=1.0f;

float cost_log10 = 1/log(10);

float log_10(float x){ // OpenGl does not have this math function !!!!
	return cost_log10*log(x);
}

void define_grid_intervals() {
	int cx_level = int(log_10(abs(camera_loc.x- relative_yz_grid_dist)));
	int cy_level = int(log_10(abs(camera_loc.y- relative_xz_grid_dist)));
	int cz_level = int(log_10(abs(camera_loc.z- relative_xy_grid_dist)));
	
	int max_level= max(cx_level,max(cy_level,cz_level));
	
	x_grid_interval = pow(10,max_level);
	y_grid_interval = pow(10,max_level);
	z_grid_interval = pow(10,max_level);
	
	//x_grid_interval = pow(10,cx_level);
	//y_grid_interval = pow(10,cy_level);
	//z_grid_interval = pow(10,cz_level);
}

void main (){
	//vec4 x_center = gl_in[0].gl_Position*10;
	vec4 x_center = vec4(gl_in[0].gl_Position.x*10,0.0,0.0,1.0);
	vec4 y_center = vec4(0.0,gl_in[0].gl_Position.x*10,0.0,1.0);
	vec4 z_center = vec4(0.0,0.0,gl_in[0].gl_Position.x*10,1.0);
	
	define_grid_intervals();
	
	// ****************** X-Y PLANE *******************
	if(display_xy_plane != 0){
		float xy_dist = 0.0f;
			
		if(relative_xy_grid !=0)
			xy_dist = camera_loc.z - relative_xy_grid_dist;
			
		vec4 xy_xcenter = x_center;
		vec4 xy_ycenter = y_center;
		
		xy_xcenter.x = x_center.x * z_grid_interval;
		xy_ycenter.y = y_center.y * z_grid_interval;
	
		varyingColor = xy_grid_color;
		gl_Position = mvpMatrix*(xy_xcenter+vec4(0.0, -10.0*z_grid_interval, 0.0+xy_dist, 0.0)); 
		EmitVertex();

		varyingColor = xy_grid_color;
		gl_Position =  mvpMatrix*(xy_xcenter+vec4( 0.0, 10.0*z_grid_interval, 0.0+xy_dist, 0.0));
		EmitVertex();
		
		EndPrimitive();

		varyingColor = xy_grid_color;
		gl_Position = mvpMatrix*(xy_ycenter+vec4(-10.0*z_grid_interval,0.0, 0.0+xy_dist, 0.0)); 
		EmitVertex();

		varyingColor = xy_grid_color;
		gl_Position =  mvpMatrix*(xy_ycenter+vec4(10.0*z_grid_interval,0.0, 0.0+xy_dist, 0.0));
		EmitVertex();
			
		EndPrimitive();
	}

	// ****************** X-Z PLANE *******************
	if(display_xz_plane !=0){
		float xz_dist = 0.0f;
		if(relative_xz_grid !=0)
			xz_dist = camera_loc.y - relative_xz_grid_dist;
		
		vec4 xz_xcenter = x_center;
		vec4 xz_zcenter = z_center;
		
		xz_xcenter.x = x_center.x * y_grid_interval;
		xz_zcenter.z = z_center.z * y_grid_interval;
		
		varyingColor = xz_grid_color;
		gl_Position = mvpMatrix*(xz_xcenter+vec4( 0.0, 0.0+xz_dist, -10.0* y_grid_interval,0.0)); 
		EmitVertex();

		varyingColor = xz_grid_color;
		gl_Position =  mvpMatrix*(xz_xcenter+vec4( 0.0, 0.0+xz_dist, 10.0* y_grid_interval, 0.0));
		EmitVertex();
		
		EndPrimitive();
		
		varyingColor = xz_grid_color;
		gl_Position = mvpMatrix*(xz_zcenter+vec4(-10.0* y_grid_interval, 0.0+xz_dist, 0.0, 0.0)); 
		EmitVertex();

		varyingColor = xz_grid_color;
		gl_Position =  mvpMatrix*(xz_zcenter+vec4(10.0* y_grid_interval, 0.0+xz_dist, 0.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
	}

	// ****************** Y-Z PLANE *******************
	if(display_yz_plane !=0){
		float yz_dist = 0.0f;
		if(relative_yz_grid !=0)
			yz_dist = camera_loc.x - relative_yz_grid_dist;
		
		vec4 yz_ycenter = y_center;
		vec4 yz_zcenter = z_center;
		
		yz_ycenter.y = y_center.y * x_grid_interval;
		yz_zcenter.z = z_center.z * x_grid_interval;
		
		varyingColor = yz_grid_color;
		gl_Position = mvpMatrix*(yz_zcenter+vec4(0.0+yz_dist,-10.0* x_grid_interval, 0.0, 0.0)); 
		EmitVertex();

		varyingColor = yz_grid_color;
		gl_Position =  mvpMatrix*(yz_zcenter+vec4(0.0+yz_dist,10.0* x_grid_interval, 0.0, 0.0));
		EmitVertex();
		
		EndPrimitive();
		
		varyingColor = yz_grid_color;
		gl_Position = mvpMatrix*(yz_ycenter+vec4(0.0+yz_dist, 0.0,-10.0* x_grid_interval, 0.0)); 
		EmitVertex();

		varyingColor = yz_grid_color;
		gl_Position =  mvpMatrix*(yz_ycenter+vec4(0.0+yz_dist, 0.0,10.0* x_grid_interval, 0.0));
		EmitVertex();
		
		EndPrimitive();
	}

	

}