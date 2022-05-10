#pragma once
#include <array>

#include "../Voxel_hcp_object/voxel_data_storage.h"
#include "../Marching/voxel_hcp_marching_data.h"
#include "../Automata/voxel_automata_functions.h"

/*
	voxel_basic_functions_class

	C++ class that contain a set of universal basic functions to be used in the hcp voxel module
	and Virtual Worlds application.
*/

class voxel_basic_functions_class {

public:

	//voxel_object_data_class *surface_voxels_copy(voxel_object_data_class *voxel_object_data) {
	voxel_object_data_class* surface_voxels_copy(voxel_hcp_object_class* cloud, voxel_data_type surface_value = 0) {
		voxel_object_data_class* voxel_object_data_copy = new voxel_object_data_class;
		voxel_object_data_copy->voxel_size = cloud->voxel_object_data.voxel_size;
		voxel_object_data_copy->matrix_origin = cloud->voxel_object_data.matrix_origin;
		//voxel_object_data_copy->matrix_coordinate_scale_factors = { 1.0,1.0,1.0 }; // axis scale that each index node multiplied by to give real world x,y,z corrdinate value
		voxel_object_data_copy->matrix_dimension = cloud->voxel_object_data.matrix_dimension;

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "Here 01", QMessageBox::Ok);

				//if (!get_surface_voxels(cloud->voxel_object_data, true, voxel_object_data_copy)) {
		if (!get_surface_voxels(cloud, surface_value, true, voxel_object_data_copy)) {
			delete voxel_object_data_copy;
			voxel_object_data_copy = NULL;
		}

		//int n = 0;
		//for (int voxel_index = 0; voxel_index < voxel_object_data_copy->voxel_matrix_data.size(); voxel_index++) {
		//	if (voxel_object_data_copy->voxel_matrix_data[voxel_index] < 0) n++;
		//}
		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "surface_voxels_copy AAAA : " + QString::number(n), QMessageBox::Ok);

		return voxel_object_data_copy;
	}

	//bool get_surface_voxels(voxel_object_data_class* voxel_object_data, bool make_copy = false, voxel_object_data_class *voxel_object_surface_data_copy = NULL) {
	bool get_surface_voxels(voxel_hcp_object_class *cloud, voxel_data_type surface_value, bool make_copy = false, voxel_object_data_class* voxel_object_surface_data_copy = NULL) {
		//std::vector <voxel_data_type>  voxel_matrix_surface_data;
		std::vector <voxel_data_type>  voxel_matrix_surface_data;
		index_data_type            voxel_index = 0;
		//size_t                     number_voxels = voxel_object_data->voxel_matrix_data.size();
		size_t                     number_voxels = cloud->voxel_object_data.voxel_matrix_data.size();
		voxel_data_type non_surface_value = DEFAULT_INACTIVE_VALUE;

		voxel_matrix_surface_data = std::vector<voxel_data_type>(number_voxels, non_surface_value); // allocate Vector of size number_voxels and set values to not_surface_value

		if (voxel_matrix_surface_data.size() == 0) return false;
		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "get_surface_voxels 00 "+ QString::number(voxel_matrix_surface_data.size()) +":"+ QString::number(number_voxels), QMessageBox::Ok);

		int n = 0;
		for (voxel_index = 0; voxel_index < number_voxels; voxel_index++) {
			//if (!inside_voxel_volume(voxel_object_data, voxel_index))
			if (!inside_voxel_volume(cloud, voxel_index, surface_value))
				//voxel_matrix_surface_data[voxel_index] = cloud->voxel_object_data.voxel_matrix_data[voxel_index];
				voxel_matrix_surface_data[voxel_index] = cloud->voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index); // ****
			else {
				voxel_matrix_surface_data[voxel_index] = non_surface_value;
				//cloud->voxel_object_data.deactivate_voxel_matrix_coordinate(voxel_index);
				n++;
			}

		}
		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "get_surface_voxels 01 : "+ QString::number(n), QMessageBox::Ok);
		if (make_copy) {
			voxel_object_surface_data_copy->voxel_matrix_data.clear();
			voxel_object_surface_data_copy->voxel_matrix_data.shrink_to_fit();

			voxel_object_surface_data_copy->voxel_matrix_data = voxel_matrix_surface_data;
		}
		else {
			//voxel_object_data->voxel_matrix_data.clear();
			cloud->voxel_object_data.voxel_matrix_data.clear();
			//voxel_object_data->voxel_matrix_data.shrink_to_fit();
			cloud->voxel_object_data.voxel_matrix_data.shrink_to_fit();

			//voxel_object_data->voxel_matrix_data = voxel_matrix_surface_data;
			cloud->voxel_object_data.voxel_matrix_data = voxel_matrix_surface_data;
		}
		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "get_surface_voxels 02", QMessageBox::Ok);
		return true;
	}

	// Determine if the hcp voxel that has hcp voxel matrix index value voxel_index is inside the generated point cloud data matrix 
	// that has a surface value of surface_value or not
	bool inside_voxel_volume(voxel_hcp_object_class *cloud, index_data_type voxel_index, voxel_data_type surface_value) {
		index_vector voxel_matrix_coord;
		bool         inside_volume_voxel = true;

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "inside_voxel_volume 01", QMessageBox::Ok);
		voxel_matrix_coord = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "inside_voxel_volume 02 x:"+ QString::number(voxel_matrix_coord.x) +" y:"+ QString::number(voxel_matrix_coord.y) + " z:"+ QString::number(voxel_matrix_coord.z), QMessageBox::Ok);

		voxel_automata_hcp_functions.cloud = cloud;

		// Get the state value of each of the hcp voxel neighbours
		std::vector<voxel_data_type> voxel_neighbours_state = voxel_automata_hcp_functions.get_voxel_neighbours_state(voxel_matrix_coord.x, voxel_matrix_coord.y, voxel_matrix_coord.z);

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "inside_voxel_volume 03 ", QMessageBox::Ok);

		for (int i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "inside_voxel_volume 00 :" + QString::number(voxel_neighbours_state[i]), QMessageBox::Ok);
			if (voxel_neighbours_state[i] <= surface_value) {
//if(voxel_neighbours_state[i] == INVALID_VOXEL_VALUE) 
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "edge voxel ", QMessageBox::Ok);
				inside_volume_voxel = false;
				break;
			}
		}

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "inside_voxel_volume 04" + QString::number(inside_volume_voxel), QMessageBox::Ok);

		return inside_volume_voxel;
	}

private:
	voxel_automata_hcp_functions_class voxel_automata_hcp_functions;


};

// VOXEL SURFACE FUNCTIONS


/*
	voxel_surface_data_class

	C++ class and structures that create a list of verticies as a std::vector class that represents 
	the surface of a generated hcp voxel point cloud.
*/

struct surface_vertices_struct_type {
	index_data_type voxel_index;
	index_data_type vertex_id;
};
// ***** /*
struct surface_faces_struct_type {
	index_data_type face_id;
	index_vector    voxel_index;
};
// **** */
struct surface_vertices_normal_struct_type {
	index_data_type voxel_index;
	glm::vec3       voxel_normal;
};

struct surface_faces_normal_struct_type {
	index_data_type voxel_index;
	glm::vec3       voxel_normal;
};

class voxel_surface_data_class {
public:
	voxel_hcp_object_class *cloud = NULL;

	std::vector<surface_vertices_struct_type>         surface_vertices;
	std::vector<surface_faces_struct_type>            surface_faces;
	std::vector <surface_vertices_normal_struct_type> surface_vertices_normals;

	~voxel_surface_data_class() {
		clear_data();
	}

	bool create_voxel_surface_point_data() {
//printf("create_voxel_surface_data 00\n");
		if (cloud == NULL) return false;
//printf("create_voxel_surface_data 11\n");
		if (cloud->voxel_object_data.voxel_matrix_data.size() == 0) return false;
//printf("create_voxel_surface_data 22\n");
		voxel_automata_hcp_functions.cloud = cloud;

		clear_data();

//printf("create_voxel_surface_data 33\n");
		return create_voxel_surface_face_point_data();

		//return false;
	}

	bool create_voxel_surface_face_data() {
		if (cloud == NULL) return false;
		if (cloud->voxel_object_data.voxel_matrix_data.size() == 0) return false;
		voxel_automata_hcp_functions.cloud = cloud;

		clear_data();

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_data 00", QMessageBox::Ok);
		return create_voxel_surface_faces_data();

		//return false;
	}

	// Determine if the hcp voxel of the voxel data matrix index coordinate voxel_coordinate is a surface voxel or not
	// A surface voxel is a voxel that has at least one neighbour that has an invalid voxel value.
	bool is_voxel_surface_vertex(index_vector voxel_coordinate) {
		//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] == INVALID_VOXEL_VALUE) return false;

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_vertex_data 00 :" + QString::number(number_voxels), QMessageBox::Ok);
		int neighbour;
		//index_vector voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);
		int valid_neighbours = 0;

		// Need to also cater for a valid voxel threshold value 
		for (neighbour = 0; neighbour < NUMBER_HCP_NEIGHBOURS; neighbour++) {
			if (voxel_automata_hcp_functions.get_voxel_neighbour_state(neighbour, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z) != INVALID_VOXEL_VALUE)
				valid_neighbours++;
		}

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_vertex_data 01 :" + QString::number(surface_vertices.size()), QMessageBox::Ok);
		if (valid_neighbours == NUMBER_HCP_NEIGHBOURS) // voxel is an interior voxel
			return false;
		else
			return true;
	}

	// Create a std::vector that contains the centers of hcp voxels that are on the surface of the generated
	// hcp voxel point cloud data.
	bool create_voxel_surface_face_point_data() {
		index_data_type voxel_index, vertex_id = 0;
		size_t          number_voxels = cloud->voxel_object_data.voxel_matrix_data.size(), face_id = 0;
		size_t          voxel_neighbour_0, voxel_neighbour_1;
		index_vector    voxel_coordinate;

		surface_vertices_struct_type        surface_vertex;
		surface_vertices_normal_struct_type surface_vertex_normal;

		glm::vec3 voxel_normal = { 0.0f,0.0f,0.0f };

		index_vector voxel_hcp_neighbours_face_connection_coord[2];

//printf("create_voxel_surface_face_point_data 00 : %i\n", number_voxels);
		for (voxel_index = 0; voxel_index < number_voxels; voxel_index++) {
			//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] != INVALID_VOXEL_VALUE) {
			if (cloud->voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index) != INVALID_VOXEL_VALUE) {
//printf("create_voxel_surface_face_point_data 11 : %i\n", number_voxels);
				voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);
//printf("create_voxel_surface_face_point_data 22 : %i\n", number_voxels);
				if (is_voxel_surface_vertex(voxel_coordinate)) {
//printf("create_voxel_surface_face_point_data 33 : %i\n", number_voxels);
					surface_vertex.vertex_id = vertex_id;
					surface_vertex.voxel_index = voxel_index;

					surface_vertices.push_back(surface_vertex);
					vertex_id++;

					// Step 1 : calculate voxel of voxel_index normal vector
					voxel_normal = calc_voxel_normal_vector(voxel_coordinate);
//printf("create_voxel_surface_face_point_data 44 : %i\n", number_voxels);
					// Store surface voxel normal data
					surface_vertex_normal.voxel_index = voxel_index;
					surface_vertex_normal.voxel_normal = voxel_normal;
					surface_vertices_normals.push_back(surface_vertex_normal);
				}

			}
		}

//printf("create_voxel_surface2_vertex_data 55 : %i : %i \n", surface_vertices.size(),surface_vertices_normals.size());
		if (surface_vertices.size() < 3)
			return false;
		else
			return true;
	}


	// THE FOLLOWING FACE CREATION ALGORITIM WORKS FOR A CUBE VOXEL SHAPE, BUT HAS ISSUES WITH SURFACE VOXELS OR ARBRITARY SHAPES AND DOES
	// NOT CREATE COMPLETE SURFACES AS THE SURFACE VOXELS IS INCOMPETE. SOMETHING TO COME BACK TO AT A LATTER TIME
	bool create_voxel_surface_faces_data() {
		index_data_type voxel_index, vertex_id = 0;
		size_t          number_voxels = cloud->voxel_object_data.voxel_matrix_data.size(), face_id = 0;
		size_t          voxel_neighbour_0, voxel_neighbour_1;
		index_vector    voxel_coordinate;

		surface_vertices_struct_type        surface_vertex;
		surface_vertices_normal_struct_type surface_vertex_normal;
		surface_faces_struct_type           surface_face;

		glm::vec3 voxel_normal = { 0.0f,0.0f,0.0f }, surface_neighbor_0_normal = { 0.0f,0.0f,0.0f }, surface_neighbor_1_normal = { 0.0f,0.0f,0.0f };

		index_vector voxel_hcp_neighbours_face_connection_coord[2];

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface2_vertex_data 00 :" + QString::number(number_voxels), QMessageBox::Ok);
		for (voxel_index = 0; voxel_index < number_voxels; voxel_index++) {
			//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] != INVALID_VOXEL_VALUE) {
			if (cloud->voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index) != INVALID_VOXEL_VALUE) { // *****

				voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);

				if (is_voxel_surface_vertex(voxel_coordinate)) {

					surface_vertex.vertex_id = vertex_id;
					surface_vertex.voxel_index = voxel_index;

					surface_vertices.push_back(surface_vertex);
					vertex_id++;

					// Step 1 : calculate voxel of voxel_index normal vector
					voxel_normal = calc_voxel_normal_vector(voxel_coordinate);
					// Store surface voxel normal data
					surface_vertex_normal.voxel_index = voxel_index;
					surface_vertex_normal.voxel_normal = voxel_normal;
					surface_vertices_normals.push_back(surface_vertex_normal);

					// THE FOLLOWING FACE CREATION ALGORITIM WORKS FOR A CUBE VOXEL SHAPE, BUT HAS ISSUES WITH SURFACE VOXELS OR ARBRITARY SHAPES AND DOES
					// NOT CREATE COMPLETE SURFACES AS THE SURFACE VOXELS IS INCOMPETE. SOMETHING TO COME BACK TO AT A LATTER TIME

					// step 2 : for each neighbour pair of voxel_hcp_even_neighbours_face_connections or voxel_hcp_odd_neighbours_face_connections
					//			determine if have a traingle face to construct and store and if so calculate veterx normals of these neighbours
					for (int i = 0; i < NUMBER_HCP_FACE_CONNECTIONS; i++) {

						// Find neighbours to construct surface triangle from
						if (voxel_coordinate.z % 2 == 0) {//even z level voxel
							voxel_neighbour_0 = voxel_hcp_even_neighbours_face_connections[i].neighbour_0;
							voxel_neighbour_1 = voxel_hcp_even_neighbours_face_connections[i].neighbour_1;
						}
						else {//odd z level voxel
							voxel_neighbour_0 = voxel_hcp_odd_neighbours_face_connections[i].neighbour_0;
							voxel_neighbour_1 = voxel_hcp_odd_neighbours_face_connections[i].neighbour_1;
						}

						//find surface neigbours coordinates
						voxel_hcp_neighbours_face_connection_coord[0] = voxel_automata_hcp_functions.get_voxel_neighbour_index_coordinates(voxel_neighbour_0, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z);
						voxel_hcp_neighbours_face_connection_coord[1] = voxel_automata_hcp_functions.get_voxel_neighbour_index_coordinates(voxel_neighbour_1, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z);

						// If have a valid surface triangle candidate, calculate neighbor surface normal vector
						if (voxel_automata_hcp_functions.valid_index_coordinate(voxel_hcp_neighbours_face_connection_coord[0]) &&
							voxel_automata_hcp_functions.valid_index_coordinate(voxel_hcp_neighbours_face_connection_coord[1])) { // Have a valid triangle surface face coorinate

							if (is_voxel_surface_vertex(voxel_hcp_neighbours_face_connection_coord[0]) && is_voxel_surface_vertex(voxel_hcp_neighbours_face_connection_coord[1])) {// Have a valid triangle surface face

								surface_neighbor_0_normal = calc_voxel_normal_vector(voxel_hcp_neighbours_face_connection_coord[0]);
								surface_neighbor_1_normal = calc_voxel_normal_vector(voxel_hcp_neighbours_face_connection_coord[1]);

								// step 3 : perform a dot product of vertex normal to its neighbours of 2 and if the dot product > 0 for both voxels neighbours then construct a face and designate
								//          vertex data , face data to store

								//float surface_neighbor_0_angle = voxel_normal.dotProduct(voxel_normal, surface_neighbor_0_normal);// (voxel_normal.length() * surface_neighbor_0_normal.length());
								//float surface_neighbor_1_angle = voxel_normal.dotProduct(voxel_normal, surface_neighbor_1_normal);// / (voxel_normal.length() * surface_neighbor_1_normal.length());

								float surface_neighbor_0_angle = glm::dot(voxel_normal, surface_neighbor_0_normal);// (voxel_normal.length() * surface_neighbor_0_normal.length());
								float surface_neighbor_1_angle = glm::dot(voxel_normal, surface_neighbor_1_normal);// / (voxel_normal.length() * surface_neighbor_1_normal.length());


								if (surface_neighbor_0_angle > 0.0f && surface_neighbor_1_angle > 0.0f) {// Have a valid surface triangle to construct

									index_data_type neighbor_0_index = cloud->voxel_object_data.get_voxel_matrix_data_index(voxel_hcp_neighbours_face_connection_coord[0]);
									index_data_type neighbor_1_index = cloud->voxel_object_data.get_voxel_matrix_data_index(voxel_hcp_neighbours_face_connection_coord[1]);

//if(neighbor_0_index < 0 || neighbor_1_index < 0 || neighbor_0_index > number_voxels-1 || neighbor_1_index > number_voxels-1){
//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface2_vertex_data 01 :" + QString::number(voxel_index) + " : " + QString::number(neighbor_0_index) + " : " + QString::number(neighbor_1_index)
// + "\n" + QString::number(voxel_coordinate.x) + " : " + QString::number(voxel_coordinate.y) + " : " + QString::number(voxel_coordinate.z)
// + "\n" + QString::number(voxel_hcp_neighbours_face_connection_coord[0].x) + " : " + QString::number(voxel_hcp_neighbours_face_connection_coord[0].y) + " : " + QString::number(voxel_hcp_neighbours_face_connection_coord[0].z)
// + "\n" + QString::number(voxel_hcp_neighbours_face_connection_coord[1].x) + " : " + QString::number(voxel_hcp_neighbours_face_connection_coord[1].y) + " : " + QString::number(voxel_hcp_neighbours_face_connection_coord[1].z), QMessageBox::Ok);
//}

									//if ((neighbor_0_index > -1 && neighbor_0_index < number_voxels) && (neighbor_1_index > -1 && neighbor_1_index < number_voxels)) {

										surface_face.face_id = face_id;
										surface_face.voxel_index = { voxel_index,neighbor_1_index,neighbor_0_index };
										surface_faces.push_back(surface_face);

										face_id++;
									//}
								}

							}
						}

					}
				}
			}
		}

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface2_vertex_data 01 :" + QString::number(surface_vertices.size()) + " : " + QString::number(surface_vertices_normals.size()) + " : " + QString::number(surface_faces.size()), QMessageBox::Ok);
		if (surface_vertices.size() < 3)
			return false;
		else
			return true;
	}
	//*****


	glm::vec3 calc_voxel_normal_vector(index_vector voxel_coordinate) {
		glm::vec3 voxel_normal = { 0.0f,0.0f,0.0f };

		//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] != INVALID_VOXEL_VALUE) {

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_vertex_data 00 :" + QString::number(number_voxels), QMessageBox::Ok);
		int neighbour;
		//index_vector voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);

		// Need to also cater for a valid voxel threshold value 
		for (neighbour = 0; neighbour < NUMBER_HCP_NEIGHBOURS; neighbour++) {
			if (voxel_automata_hcp_functions.get_voxel_neighbour_state(neighbour, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z) != INVALID_VOXEL_VALUE) {
				if (voxel_coordinate.z % 2 == 0) {//even z level voxel
					voxel_normal += voxel_hcp_even_neighbour_normals[neighbour];
				}
				else { //odd z level voxel
					voxel_normal += voxel_hcp_odd_neighbour_normals[neighbour];
				}

			}
		}
		//}

		//return voxel_normal.normalized();
		return glm::normalize(voxel_normal);
	}

	// Not sure this is correct;
	glm::vec3 calc_Face_normals_from_vertex_normals(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2) {
		glm::vec3 face_normal = (vertex0 + vertex1 + vertex2);

		//return face_normal.normalized();
		return glm::normalize(face_normal);
	}

	void clear_data() {
		surface_vertices.clear();
		surface_vertices.shrink_to_fit();
		surface_faces.clear();
		surface_faces.shrink_to_fit();
		surface_vertices_normals.clear();
		surface_vertices_normals.shrink_to_fit();
	}


private:
	voxel_automata_hcp_functions_class voxel_automata_hcp_functions;
	voxel_object_data_class            voxel_object;
};

/*
	voxel_surface_face_class

	C++ class and structures that create a list of verticies as a std::vector class that represents
	the quad faces of the surface of a generated hcp voxel point cloud.
 
*/

class voxel_surface_face_class {
public:
	voxel_hcp_object_class *cloud = NULL; // hcp voxel class that is to have a quad surface created from

	std::vector<glm::vec3>          vertices; // std::vector to store traingular surface data
	std::vector<glm::vec3>          normals;
	std::vector<std::array<int, 4>> faces;    // List of vertex ids that give order of vertices to create surface from

	~voxel_surface_face_class() {
		clear_data();
	}

	bool create_voxel_surface_data() {
		if (cloud == NULL) return false;
		if (cloud->voxel_object_data.voxel_matrix_data.size() == 0) return false;
		voxel_automata_hcp_functions.cloud = cloud;

		clear_data();

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_data 00", QMessageBox::Ok);
		return create_voxel_surface_faces();

	}

	// Even though this works to a certain extent of creating a 3D surface has issues having holes or gaps in the surface
	// May Need to investiugate further and suspect that problems not being able to finding all voxels that lie on a surface. 

	bool create_voxel_surface_faces() {
		index_data_type voxel_index, vertex_id = 0;
		size_t          number_voxels = cloud->voxel_object_data.voxel_matrix_data.size(), face_id = 0;
		size_t          voxel_neighbour_0, voxel_neighbour_1;
		index_vector    voxel_coordinate;

		vertices.clear();
		vertices.shrink_to_fit();
		normals.clear();
		normals.shrink_to_fit();
		faces.clear();
		faces.shrink_to_fit();

		//surface_vertices_struct_type        surface_vertex;
		//surface_vertices_normal_struct_type surface_vertex_normal;

		bool surface_defined = false;

		//glm::vec3 voxel_normal = { 0.0f,0.0f,0.0f };// , surface_neighbor_0_normal = { 0.0f,0.0f,0.0f }, surface_neighbor_1_normal = { 0.0f,0.0f,0.0f };

		//index_vector voxel_hcp_neighbours_face_connection_coord[2];

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface2_vertex_data 00 :" + QString::number(number_voxels), QMessageBox::Ok);
		for (voxel_index = 0; voxel_index < number_voxels; voxel_index++) {

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_faces() 00 :" + QString::number(number_voxels), QMessageBox::Ok);
			//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] != INVALID_VOXEL_VALUE) {// Inside voxel volume or is a surface voxel
			if (cloud->voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value,voxel_index) != INVALID_VOXEL_VALUE) {// Inside voxel volume or is a surface voxel // *****

				voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);

				if (is_voxel_surface_vertex(voxel_coordinate)) {
					for (int neighbour = 0; neighbour < NUMBER_HCP_NEIGHBOURS; neighbour++) {
						if (neighbour_is_voxel_surface(voxel_coordinate, neighbour)) {
							define_voxel_surface(voxel_coordinate, neighbour);
							surface_defined = true;
						}
					}
				}
			}

		}

		//remove_duplicate_vertices(); //loading .ply generated to crash mesh lab and Blender. Something to do with values ~ x.xxxe-8 generated ?????

		return surface_defined;
	}

	void define_voxel_surface(index_vector voxel_coordinate, int neighbour) {
		glm::vec3 voxel_cartesian_coordinate = cloud->voxel_object_data.get_voxel_cartesian_coordinate(voxel_coordinate);

		int vertex_index = vertices.size();
		std::array<int, 4> vertex_face;

		//if (neighbour < 10) {
		//if (neighbour == 3 || neighbour == 4 || neighbour == 5 || neighbour == 6 || neighbour == 7 || neighbour == 8) {
		if (voxel_coordinate.z % 2 == 0) {// even z level
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_even_surface_element[neighbour][0] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_even_surface_element[neighbour][1] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_even_surface_element[neighbour][2] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_even_surface_element[neighbour][3] * cloud->voxel_object_data.voxel_size);

			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_even_surface_element[neighbour][0]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_even_surface_element[neighbour][1]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_even_surface_element[neighbour][2]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_even_surface_element[neighbour][3]);
		}
		else { // odd z level
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_odd_surface_element[neighbour][0] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_odd_surface_element[neighbour][1] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_odd_surface_element[neighbour][2] * cloud->voxel_object_data.voxel_size);
			vertices.push_back(voxel_cartesian_coordinate + voxel_hcp_odd_surface_element[neighbour][3] * cloud->voxel_object_data.voxel_size);

			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_odd_surface_element[neighbour][0]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_odd_surface_element[neighbour][1]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_odd_surface_element[neighbour][2]);
			normals.push_back(voxel_cartesian_coordinate - voxel_hcp_odd_surface_element[neighbour][3]);
		}

		vertex_face[0] = vertex_index;
		vertex_face[1] = vertex_index + 1;
		vertex_face[2] = vertex_index + 2;
		vertex_face[3] = vertex_index + 3;

		faces.push_back(vertex_face);
		//}
	//}
	}

	// A simple algorithim to remove duplicate vertex points for a suface of non duplicate faces
	void  remove_duplicate_vertices() {
		int vertices_vertex_index, face_index, face_vertex_index;
		int number_faces = faces.size();

		std::vector<glm::vec3> new_vertices; // The vector to store single unique surface coordinates that faces reference
		std::vector<glm::vec3> new_normals;  // The vector to store single unique surface normals at the surface vertex coordinate

//QMessageBox::information(NULL, "", "in remove_duplicate_vertices 01 : "+ QString::number(vertices.size()), QMessageBox::Ok);
		for (face_index = 0; face_index < number_faces; face_index++) { // iterate each face
			for (face_vertex_index = 0; face_vertex_index < 3; face_vertex_index++) { // iterate each vertax of each face
				vertices_vertex_index = faces[face_index][face_vertex_index];         // get the index of the vertices vector that the face vertex corresponds to

				// Find the vertex index in the new vertices array that the coordinate of the face of face_index
				//  and vertex of face_vertex_index that that face is equivalent to.
				int new_vertices_vertex_index = find_new_vertices_vertex_id(vertices_vertex_index, new_vertices);

				// Have not found the vertex coordinate in the new_vertices list : create one and assign the index of this
				// created vector entry to vertices_vertex_index
				if (new_vertices_vertex_index < 0) {
					new_vertices_vertex_index = new_vertices.size();
					new_vertices.push_back(vertices[vertices_vertex_index]);
					new_normals.push_back(normals[vertices_vertex_index]);
				}
				else {
					new_normals[new_vertices_vertex_index] += normals[vertices_vertex_index]; // to get an average normal of the vertex
				}

				// Assign the index value of the new_vertices vector that contains the coordinate of
				// the vertex face_vertex_indexof the face of face_index in the faces vector
				faces[face_index][face_vertex_index] = new_vertices_vertex_index;

			}
		}

		// delete redundant vertices and normal vectors with duplicate vertices and normals
		vertices.clear();
		vertices.shrink_to_fit();
		normals.clear();
		normals.shrink_to_fit();

		vertices = new_vertices;
		normals = new_normals;

		//normalize_vertex_normals();

		//QMessageBox::information(NULL, "", "in remove_duplicate_vertices 02 : "+ QString::number(vertices.size()), QMessageBox::Ok);
	}

	// Return the vertex index in the new vertices array that the coordinate of the face of face_index
	// and vertex of face_vertex_index that that face is equivalent to.
	// If not found return -1.
	int find_new_vertices_vertex_id(int vertices_vertex_index, std::vector<glm::vec3>& new_vertices) {
		int number_new_vertices = new_vertices.size(), new_vertices_vertex_index;

		glm::vec3 vertex_coord_to_find = vertices[vertices_vertex_index];

		for (new_vertices_vertex_index = 0; new_vertices_vertex_index < number_new_vertices; new_vertices_vertex_index++) {
			if (new_vertices[new_vertices_vertex_index] == vertex_coord_to_find) return new_vertices_vertex_index;
		}

		return -1;
	}

	void normalize_vertex_normals() {
		int number_vertices = normals.size();
		for (int i = 0; i < number_vertices; i++)
			glm::normalize(normals[i]);
			//normals[i].normalize();
	}

	bool neighbour_is_voxel_surface(index_vector voxel_coordinate, int neighbour) {

		if (voxel_automata_hcp_functions.get_voxel_neighbour_state(neighbour, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z) == INVALID_VOXEL_VALUE)
			return true;
		else
			return false;
	}

	bool is_voxel_surface_vertex(index_vector voxel_coordinate) {
		//if (cloud->voxel_object_data.voxel_matrix_data[voxel_index] == INVALID_VOXEL_VALUE) return false;

//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_vertex_data 00 :" + QString::number(number_voxels), QMessageBox::Ok);
		int neighbour;
		//index_vector voxel_coordinate = cloud->voxel_object_data.get_matrix_coordinate(voxel_index);
		int valid_neighbours = 0;

		// Need to also cater for a valid voxel threshold value 
		for (neighbour = 0; neighbour < NUMBER_HCP_NEIGHBOURS; neighbour++) {
			if (voxel_automata_hcp_functions.get_voxel_neighbour_state(neighbour, voxel_coordinate.x, voxel_coordinate.y, voxel_coordinate.z) != INVALID_VOXEL_VALUE)
				valid_neighbours++;
		}

		//QMessageBox::information(NULL, "Voxel Volume to Voxel Surface", "create_voxel_surface_vertex_data 01 :" + QString::number(surface_vertices.size()), QMessageBox::Ok);
		//if (valid_neighbours == NUMBER_HCP_NEIGHBOURS || valid_neighbours == 0) // voxel is an interior voxel or is exterior voxel not on surface
		if (valid_neighbours == NUMBER_HCP_NEIGHBOURS) // voxel is an interior voxel
			return false;
		else
			return true;
	}

	void clear_data() {
		vertices.clear();
		vertices.shrink_to_fit();
		faces.clear();
		faces.shrink_to_fit();
		normals.clear();
		normals.shrink_to_fit();
	}

private:
	voxel_automata_hcp_functions_class voxel_automata_hcp_functions;
	voxel_object_data_class            voxel_object;
};