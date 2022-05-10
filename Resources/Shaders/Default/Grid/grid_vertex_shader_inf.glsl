#version 410

layout(location = 0) in vec3 vertex;
//layout(location = 1) in int  voxel_value;

uniform mat4 mvpMatrix; //cameral-view-projection matrix
uniform mat4 view;      //camera-view-projection matrix
uniform mat4 projection;//camera-view-projection matrix

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


//void main () {
	
//    gl_Position = mvpMatrix*vec4(vertex, 1.0f); // required as a min

//}

// Grid position are in clipped space
//vec3 gridPlane[6] = vec3[] (
//    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
//    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
//);


vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

out vec3 nearPoint;
out vec3 farPoint;

void main() {

    //vec3 p = gridPlane[gl_VertexID].xyz;
    vec3 p = vertex;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint  = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane
    //gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
	
	//gl_Position = projection * view * vec4(gridPlane[gl_VertexID].xyz, 1.0);
	//gl_Position = mvpMatrix*vec4(p, 1.0f);
	gl_Position = projection*view*vec4(p, 1.0f);
	
	//gl_Position = vec4(vertex, 1.0f); // required as a min
}

