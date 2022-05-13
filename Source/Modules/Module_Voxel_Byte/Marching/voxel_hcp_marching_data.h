#pragma once

/*
	Sets of static unchanging data structures that define the uniform unit vertex
	values of a hcp voxel Trapezo-rhombic dodecahedral to be used in displaying
	the hcp voxel on the computer screen and in creating the point cloud and surface
	vertex data locations for .ply files of vertex and surface data to be imported 
	into third party 3D applincations.

*/


struct voxel_neighbours_face_connections_struct_type {
	int neighbour_0, neighbour_1;
};

// Offsets from a central cartesian point for each vertex of a Trapezo-rhombic dodecahedral
// shape that is in an even layer of the hcp voxel data matrix
//static const QVector3D voxel_hcp_even_neighbour_coord_offset[12] = {
static const glm::vec3 voxel_hcp_even_neighbour_coord_offset[12] = {
	{1.0,   0.5773502692,  1.2247448714},
	{0.0,  -0.5773502692,  1.2247448714},
	{-1.0,  0.5773502692,  1.2247448714},

	{ 1.0,  1.732050808, 0.0},
	{ 2.0,  0.0,         0.0},
	{ 1.0, -1.732050808, 0.0},
	{-1.0, -1.732050808, 0.0},
	{-2.0,  0.0,         0.0},
	{-1.0,  1.732050808, 0.0},

	{1.0,   0.5773502692, -1.2247448714},
	{0.0,  -0.5773502692, -1.2247448714},
	{-1.0,  0.5773502692, -1.2247448714}
};

// Offsets from a central cartesian point for each vertex of a Trapezo-rhombic dodecahedral
// shape that is in an odd layer of the hcp voxel data matrix
//static const QVector3D voxel_hcp_odd_neighbour_coord_offset[12] = {
static const glm::vec3 voxel_hcp_odd_neighbour_coord_offset[12] = {
	{-1.0, -0.5773502692,  1.2247448714},
	{0.0,   0.5773502692,  1.2247448714},
	{1.0,  -0.5773502692,  1.2247448714},

	{ 1.0,  1.732050808, 0.0},
	{ 2.0,  0.0,         0.0},
	{ 1.0, -1.732050808, 0.0},
	{-1.0, -1.732050808, 0.0},
	{-2.0,  0.0,         0.0},
	{-1.0,  1.732050808, 0.0},

	{-1.0, -0.5773502692, -1.2247448714},
	{0.0,   0.5773502692, -1.2247448714},
	{1.0,  -0.5773502692, -1.2247448714},
};

// Vetex normals for each vertex of a Trapezo-rhombic dodecahedral
// shape that is in an even layer of the hcp voxel data matrix
//static const QVector3D voxel_hcp_even_neighbour_normals[12] = {
static const glm::vec3 voxel_hcp_even_neighbour_normals[12] = {
	{-1.0, -0.5773502692, -1.2247448714},
	{ 0.0,  0.5773502692, -1.2247448714},
	{ 1.0, -0.5773502692, -1.2247448714},

	{-1.0, -1.732050808, 0.0},
	{-2.0,  0.0,         0.0},
	{-1.0,  1.732050808, 0.0},
	{ 1.0,  1.732050808, 0.0},
	{ 2.0,  0.0,         0.0},
	{ 1.0, -1.732050808, 0.0},

	{-1.0, -0.5773502692, 1.2247448714},
	{ 0.0,  0.5773502692, 1.2247448714},
	{ 1.0, -0.5773502692, 1.2247448714}
};

// Vetex normals for each vertex of a Trapezo-rhombic dodecahedral
// shape that is in an odd layer of the hcp voxel data matrix
//static const QVector3D voxel_hcp_odd_neighbour_normals[12] = {
static const glm::vec3 voxel_hcp_odd_neighbour_normals[12] = {
	{ 1.0,  0.5773502692, -1.2247448714},
	{ 0.0, -0.5773502692, -1.2247448714},
	{-1.0,  0.5773502692, -1.2247448714},

	{-1.0, -1.732050808, 0.0},
	{-2.0,  0.0,         0.0},
	{-1.0,  1.732050808, 0.0},
	{ 1.0,  1.732050808, 0.0},
	{ 2.0,  0.0,         0.0},
	{ 1.0, -1.732050808, 0.0},

	{ 1.0, 0.5773502692, 1.2247448714},
	{ 0.0,-0.5773502692, 1.2247448714},
	{-1.0, 0.5773502692, 1.2247448714},
};

// Vertex locations of a Trapezo-rhombic dodecahedral
// shape that is in an even layer of the hcp voxel data matrix
static const glm::vec3 point_0{  0.0f,  0.0f,          1.2247448714};

static const glm::vec3 point_1_e{  0.0f,  1.1547005384f, 0.8164965809f};
static const glm::vec3 point_2_e{  1.0f,  0.5773502692f, 0.4082482905f};
static const glm::vec3 point_3_e{  1.0f, -0.5773502692f, 0.8164965809f};
static const glm::vec3 point_4_e{  0.0f, -1.1547005384f, 0.4082482905f};
static const glm::vec3 point_5_e{ -1.0f, -0.5773502692f, 0.8164965809f};
static const glm::vec3 point_6_e{ -1.0f,  0.5773502692f, 0.4082482905f};

static const glm::vec3 point_7_e{  0.0f,  1.1547005384f, -0.8164965809f};
static const glm::vec3 point_8_e{  1.0f,  0.5773502692f, -0.4082482905f};
static const glm::vec3 point_9_e{  1.0f, -0.5773502692f, -0.8164965809f};
static const glm::vec3 point_10_e{  0.0f, -1.1547005384f, -0.4082482905f};
static const glm::vec3 point_11_e{ -1.0f, -0.5773502692f, -0.8164965809f};
static const glm::vec3 point_12_e{ -1.0f,  0.5773502692f, -0.4082482905f};

// Vertex locations of a Trapezo-rhombic dodecahedral
// shape that is in an odd layer of the hcp voxel data matrix
static const glm::vec3 point_1_o{   0.0f,  1.1547005384f,  0.4082482905f };
static const glm::vec3 point_2_o{   1.0f,  0.5773502692f,  0.8164965809f };
static const glm::vec3 point_3_o{   1.0f, -0.5773502692f,  0.4082482905f };
static const glm::vec3 point_4_o{   0.0f, -1.1547005384f,  0.8164965809f };
static const glm::vec3 point_5_o { -1.0f, -0.5773502692f,  0.4082482905f };
static const glm::vec3 point_6_o{  -1.0f,  0.5773502692f,  0.8164965809f };

static const glm::vec3 point_7_o{   0.0f,  1.1547005384f, -0.4082482905f };
static const glm::vec3 point_8_o{   1.0f,  0.5773502692f, -0.8164965809f };
static const glm::vec3 point_9_o{   1.0f, -0.5773502692f, -0.4082482905f };
static const glm::vec3 point_10_o{  0.0f, -1.1547005384f, -0.8164965809f };
static const glm::vec3 point_11_o{ -1.0f, -0.5773502692f, -0.4082482905f };
static const glm::vec3 point_12_o{ -1.0f,  0.5773502692f, -0.8164965809f };

static const glm::vec3 point_13{ 0.0f,  0.0f,          -1.2247448714 };

// Voxel quad surface elements of even z levels
static const glm::vec3 voxel_hcp_even_surface_element[12][4] = {
	//Top voxel surface elements
	{point_3_e,point_2_e,point_1_e,point_0},
	{point_5_e,point_4_e,point_3_e,point_0},
	{point_1_e,point_6_e,point_5_e,point_0},

	// Side voxel surface elements
	{point_2_e,point_8_e,point_7_e,point_1_e},
	{point_3_e,point_9_e,point_8_e,point_2_e},
	{point_4_e,point_10_e,point_9_e,point_3_e},
	{point_5_e,point_11_e,point_10_e,point_4_e},
	{point_6_e,point_12_e,point_11_e,point_5_e},
	{point_1_e,point_7_e,point_12_e,point_6_e},

	//bottom voxel surface elements
	{point_7_e,point_8_e,point_9_e,point_13},
	{point_9_e,point_10_e,point_11_e,point_13},
	{point_11_e,point_12_e,point_7_e,point_13}
};

// Voxel quad surface elements of odd z levels
static const glm::vec3 voxel_hcp_odd_surface_element[12][4] = {
	//Top voxel surface elements
	{point_6_o,point_5_o,point_4_o,point_0},
	{point_2_o,point_1_o,point_6_o,point_0},
	{point_4_o,point_3_o,point_2_o,point_0},

	// Side voxel surface elements
	{point_2_o,point_8_o,point_7_o,point_1_o},
	{point_3_o,point_9_o,point_8_o,point_2_o},
	{point_4_o,point_10_o,point_9_o,point_3_o},
	{point_5_o,point_11_o,point_10_o,point_4_o},
	{point_6_o,point_12_o,point_11_o,point_5_o},
	{point_1_o,point_7_o,point_12_o,point_6_o},

	//bottom voxel surface elements
	{point_12_o,point_11_o,point_10_o,point_13},
	{point_8_o,point_7_o,point_12_o,point_13},
	{point_10_o,point_9_o,point_8_o,point_13}
};

static const glm::vec3 voxel_hcp_even_surface_GLSL_element[12][4] = {
	//Top voxel surface elements
	{point_3_e,point_2_e,point_0,point_1_e},
	{point_5_e,point_4_e,point_0,point_3_e},
	{point_1_e,point_6_e,point_0,point_5_e},

	// Side voxel surface elements
	{point_2_e,point_8_e,point_1_e,point_7_e},
	{point_3_e,point_9_e,point_2_e,point_8_e},
	{point_4_e,point_10_e,point_3_e,point_9_e},
	{point_5_e,point_11_e,point_4_e,point_10_e},
	{point_6_e,point_12_e,point_5_e,point_11_e},
	{point_1_e,point_7_e,point_6_e,point_12_e},

	//bottom voxel surface elements
	{point_7_e,point_8_e,point_13,point_9_e},
	{point_9_e,point_10_e,point_13,point_11_e},
	{point_11_e,point_12_e,point_13,point_7_e}
};

// Voxel quad surface elements of odd z levels
static const glm::vec3 voxel_hcp_odd_surface_GLSL_element[12][4] = {
	//Top voxel surface elements
	{point_2_o,point_3_o,point_0,point_4_o},
	{point_4_o,point_5_o,point_0,point_6_o},
	{point_6_o,point_1_o,point_0,point_2_o},

	// Side voxel surface elements
	{point_2_o,point_8_o,point_1_o,point_7_o},
	{point_3_o,point_9_o,point_2_o,point_8_o},
	{point_4_o,point_10_o,point_3_o,point_9_o},
	{point_5_o,point_11_o,point_4_o,point_10_o},
	{point_6_o,point_12_o,point_5_o,point_11_o},
	{point_1_o,point_7_o,point_6_o,point_12_o},

	//bottom voxel surface elements
	{point_8_o,point_9_o,point_0,point_10_o},
	{point_10_o,point_11_o,point_0,point_12_o},
	{point_12_o,point_11_o,point_0,point_8_o}
};

#define NUMBER_HCP_FACE_CONNECTIONS 10
#define NUMBER_HCP_EVEN_FACE_CONNECTIONS 10

static const voxel_neighbours_face_connections_struct_type voxel_hcp_even_neighbours_face_connections[10] = {
	// forward and upward only needed

	//top faces
	{0,3},// neighbours [0,3]
	{0,4},// neighbours [0,4]
	{1,5},// neighbours [1,5]
	
	{1,4},
	//{1,7},

	//x-y plane faces
	{3,4},// neighbours [3,4]
	{4,5},// ne
	
	//bottom faces
	{9,3},// neighbours [9,3]
	{9,4},// neighbours [9,4]
	{10,5},// neighbours [10,5]
	{10,4}
	//{10,7}
};

#define NUMBER_HCP_ODD_FACE_CONNECTIONS 10

static const voxel_neighbours_face_connections_struct_type voxel_hcp_odd_neighbours_face_connections[10] = {
	// forward and upward only needed

	//top faces
	{1,3},// neighbours [0,3]
	{2,4},// neighbours [0,4]
	{2,5},// neighbours [1,5]

	{1,7},

	//x-y plane faces
	{3,4},// neighbours [3,4]
	{4,5},// neighbours [4,5]

	//botom faces
	{10,3},// neighbours [10,3]
	{11,4},// neighbours [11,4]
	{11,5},// neighbours [11,5]

	{10,7}
};
