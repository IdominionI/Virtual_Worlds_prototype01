#pragma once
#include <vector>

#include <Universal/3rdparty/include/glm/glm.hpp>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>

#include "hex_surface_parameters.h"

#include "Datatypes/dt_hex_surface_generator.h"
#include "Datatypes/dt_hex_surface_shader_parameters.h"
//**** proposed to be included in hex surface object class
#include "DataTypes/dt_hex_surface_automata.h"

/*
	hex_surface_object_data_class

	C++ Class that defines and manages the hex surface data matrix that represents the point cloud
	data of the hex suface data object.
*/

// ???????????????????????????????????????????????????????????????????????????????????????
// Need to check if this will be dined twice or not as also defined in voxel hcp data storage

#define DEFAULT_ACTIVE_VALUE   255
#define DEFAULT_INACTIVE_VALUE 0 

typedef int index_data_type;

// ???????????????????????????????????????????????????????????????????????????????????????

struct index_struct_type3 {
	index_data_type x = 0, y = 0, z = 0;
};

typedef index_struct_type3 index_vector3;


class hex_surface_object_data_class {
public:

	float	      hex_size                      = 1.0f;
	glm::vec2     grid_origin                   = { 0.0,0.0 };
	glm::vec2     grid_coordinate_scale_factors = { 1.0,1.0 }; // axis scale that each index node multiplied by to give real world x,y,z corrdinate value
	index_vector3 grid_dimension                = { 0,0,0 };  // NEED TO CHANGE THIS
	bool	      perfom_rounding_up            = true;
	bool	      hex_surface_surface_volume    = false;

	// Strange penomenom occurs that if have a class global variable within of exactly the same as below
	// the code will compile without notice of a variable of the same name already defined, but there is an
	// upredictable behaviour that will cause unintended results to occur and the application to malfunction.
	// eg had also a 
	// std::vector<hex_surface_automata_rule_struct_type> hex_surface_automata_rules;
	// defined in editor_hex_surface_automata_editor.h thinking this will cause no problems but it did.

	hex_surface_generator_parameters_struct_type   hex_surface_generator_parameters;
	shader_parameters_struct_type	               hex_surface_shader_parameters;
	std::vector<hex_surface_automata_rule_struct_type> hex_surface_automata_rules;

	index_vector3			         hex_surface_matrix_coord_index_vector;
	std::vector <hex_surface_data_type>  hex_surface_matrix_data;

	~hex_surface_object_data_class() {
		hex_surface_matrix_data.clear();
		hex_surface_matrix_data.shrink_to_fit();
	}


	hex_surface_index_data_type get_index_value(hex_surface_index_data_type iX, hex_surface_index_data_type iY, hex_surface_index_data_type iZ) {
//QMessageBox::information(NULL, "", "get_index_value00 x: "+ QString::number(iX) + " y :"+ QString::number(iY)+" z :"+ QString::number(iZ), QMessageBox::Ok);
		// Even z level
		return (hex_surface_index_data_type(iY / 2) + iY % 2) * grid_dimension.x + hex_surface_index_data_type(iY / 2) * (grid_dimension.x - 1) + iX;
	}

	hex_surface_index_data_type get_z_layer_total(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim) {
		// Even z level
		return (hex_surface_index_data_type(ydim/2) + ydim%2)*xdim + hex_surface_index_data_type(ydim/2)*(xdim-1);
	}

	hex_surface_index_data_type get_z_layer_total(hex_surface_index_data_type iZ) {
		return get_z_layer_total( grid_dimension.x, grid_dimension.y);
	}

	hex_surface_index_data_type calculate_hex_surface_matrix_data_size(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim) {
		return   get_z_layer_total( xdim, ydim);
	}

	hex_surface_index_data_type calculate_hex_surface_matrix_data_size() {
		return calculate_hex_surface_matrix_data_size(grid_dimension.x, grid_dimension.y);
	}

	// END hex surface matrix index 
		// ***************** HEX SURFACE DATA ELEMENT FUNCTIONS TO COMPRESS INTO AND FROM DEFINED hex_surface_data_type storage data type of a unsigned integer *************************

	surface_element_data_type extract_hex_surface_data_element_value(hex_surface_data_storage_type_enum hex_surface_data_element_value_type, hex_surface_index_data_type hex_surface_matrix_data_index) {
		return hex_surface_matrix_data[hex_surface_matrix_data_index];
	}

	hex_surface_data_type insert_hex_surface_data_element_value(hex_surface_data_storage_type_enum hex_surface_data_element_value_type, hex_surface_index_data_type hex_surface_matrix_data_index, surface_element_data_type value) {
		hex_surface_matrix_data[hex_surface_matrix_data_index] = value;
		return hex_surface_matrix_data[hex_surface_matrix_data_index];
	}



	void create_empty_surface_cubic(size_t xdim, size_t ydim) {
		hex_surface_data_type volume_data_storage = hex_surface_data_type(DEFAULT_INACTIVE_VALUE);

		create_hex_surface_grid(volume_data_storage,xdim,ydim);
	}

	void create_solid_surface_cubic(size_t xdim, size_t ydim) {
		hex_surface_data_type volume_data_storage = hex_surface_data_type(DEFAULT_ACTIVE_HEX_VALUE);

		create_hex_surface_grid(volume_data_storage,xdim,ydim);
	}

	void delete_hex_surface_matrix_data() {
		hex_surface_matrix_data.clear();
		hex_surface_matrix_data.shrink_to_fit();
	}

	void deactivate_hex_surface_matrix() {
		hex_surface_index_data_type i;
//QMessageBox::information(NULL, "hexBS03", "deactivate_hex_surface_matrix01  :"+QString::number(hex_surface_matrix_data.size()), QMessageBox::Ok);

		for (i = 0; i < hex_surface_matrix_data.size(); i++)
			hex_surface_matrix_data[i] = hex_surface_data_type(DEFAULT_INACTIVE_VALUE);

	}

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	index_vector3 get_matrix_coordinate(hex_surface_index_data_type matrix_index) {
		index_vector3 matrix_coord;

		hex_surface_index_data_type t0 = get_z_layer_total(0);// total number for even z level
		hex_surface_index_data_type t1 = get_z_layer_total(1);// total number for odd  z level

		hex_surface_index_data_type zt = t0 + t1; // total number for even + odd z levels

		bool even_z_level;

		if (matrix_index < t0) {
			matrix_coord.z = 0;
			even_z_level = true;
		}
		else {
			hex_surface_index_data_type zt_index01 = hex_surface_index_data_type(matrix_index / zt);
			hex_surface_index_data_type zt_index02 = hex_surface_index_data_type((matrix_index - zt_index01 * zt)/t0);

			if (zt_index02 > 0) { // Have matrix_index in the odd  z level
				matrix_coord.z = zt_index01*2 + 1;
				even_z_level = false;
			}
			else {// Have matrix_index in the even z level
				matrix_coord.z = zt_index01 *2;
				even_z_level = true;
			}
		}

		// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z hex surface coordinate
		hex_surface_index_data_type zeroth_index = matrix_index - get_index_value(0, 0, matrix_coord.z);

/*
if(matrix_coord.z > 0){
QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 00: " + QString::number(matrix_index) + " : " + QString::number(t0) + ":" + QString::number(t1) + ":" + QString::number(zt) +
							"\n " + QString::number(matrix_coord.z) + " : " + QString::number(even_z_level) + ":" + QString::number(zeroth_index) + ":" + QString::number(get_index_value(0, 0, matrix_coord.z)), QMessageBox::Ok);
}
*/
		hex_surface_index_data_type yt = 2*grid_dimension.x-1; // Even row number of hexs + odd row of hexs of any z level

		if(even_z_level){
			hex_surface_index_data_type yt_index01, yt_index02;

			yt_index01 = hex_surface_index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < grid_dimension.x)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01 * 2 + yt_index02;

//if(matrix_coord.z > 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 01AAA: " + QString::number(grid_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(yt_index01) + " : " + QString::number(yt_index02) + ":" + QString::number((zeroth_index - (yt_index01 * yt + yt_index02 * (grid_dimension.x - 1)))), QMessageBox::Ok);
//}

			matrix_coord.x = zeroth_index - ( (hex_surface_index_data_type(matrix_coord.y/2) +matrix_coord.y%2) *grid_dimension.x + hex_surface_index_data_type(matrix_coord.y/2)*(grid_dimension.x-1) );
		} else{
			hex_surface_index_data_type yt_index01, yt_index02;

			yt_index01 = hex_surface_index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < grid_dimension.x - 1)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01*2 + yt_index02;

//if(matrix_coord.z > 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 0BBB: " + QString::number(grid_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(yt_index01) + " : " + QString::number(yt_index02) + ":" + QString::number((zeroth_index - (yt_index01 * yt + yt_index02 * (grid_dimension.x - 1)))), QMessageBox::Ok);
//			}

			matrix_coord.x = zeroth_index - ( (hex_surface_index_data_type(matrix_coord.y/2) +matrix_coord.y%2) *(grid_dimension.x-1) + hex_surface_index_data_type(matrix_coord.y/2)*grid_dimension.x );
		}

//if(matrix_coord.z == 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 01: " + QString::number(grid_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(matrix_coord.x) + " : " + QString::number(matrix_coord.y) + ":" + QString::number(matrix_coord.z), QMessageBox::Ok);
//}

		return matrix_coord;
	}

	glm::vec3 get_hex_surface_cartesian_coordinate(index_vector3 hex_surface_coord, float hex_size) {
		hex_surface_index_data_type i = hex_surface_coord.x, j = hex_surface_coord.y, k = 0;

		float sqrt3 = sqrt(3.0), third = 1.0 / 3.0, z_mult = 2.0 * sqrt(6.0) / 3.0, sqrt3_2 = sqrt(1.5);
		glm::vec3 hex_cartesian_coordinate;

		if (j % 2 == 0) {
//QMessageBox::information(NULL, "hexBS03", "here00 :"+QString::number(j * HEX_SURFACE_BIT_NUMBER + j_bit), QMessageBox::Ok);
			//hex_cartesian_coordinate.setX((float(i) * 2 + float(k % 2)) * hex_size);
			//hex_cartesian_coordinate.setY((sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * hex_size);

			hex_cartesian_coordinate.x = (float(i) * 2 + float(k % 2)) * hex_size;
			hex_cartesian_coordinate.y = (sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * hex_size;
		}
		else {
//QMessageBox::information(NULL, "hexBS03", "here01 :"+QString::number(j * HEX_SURFACE_BIT_NUMBER + j_bit), QMessageBox::Ok);
		    //hex_surface_cartesian_coordinate.setX((1.0 + float(i) * 2 + float(k % 2)) * hex_surface_size - 2.0 * (k % 2)); the old wrong one
			
			//hex_cartesian_coordinate.setX((-1.0 + float(i) * 2 + float(k % 2) + 2 * float((k + 1) % 2)) * hex_size);
			//hex_cartesian_coordinate.setY((sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * hex_size);

			hex_cartesian_coordinate.x = (-1.0 + float(i) * 2 + float(k % 2) + 2 * float((k + 1) % 2)) * hex_size;
			hex_cartesian_coordinate.y = (sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * hex_size;
		}

		//hex_cartesian_coordinate.setZ(0.0);
		hex_cartesian_coordinate.z = 0.0;

		return hex_cartesian_coordinate;
	}

	glm::vec3 get_hex_surface_cartesian_coordinate(index_vector3 hex_surface_coord) {
		return get_hex_surface_cartesian_coordinate(hex_surface_coord, hex_size);
	}

	hex_surface_index_data_type get_hex_surface_matrix_data_index(hex_index_vector matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_hex_surface_matrix_bit_location
		return get_index_value(matrix_coord.x,matrix_coord.y, 0);
	}

	hex_surface_data_type activate_hex_surface_matrix_coordinate(hex_index_vector matrix_coord, hex_surface_data_type hex_surface_status_value = DEFAULT_ACTIVE_HEX_VALUE) {// testing only comment out or delete when not in use
	
//QMessageBox::information(NULL, "", "avmc00 :"+ QString::number(matrix_coord.matrix_index.x) + " x :"+ QString::number(matrix_coord.matrix_index.y)+"y :"+ QString::number(matrix_coord.matrix_index.z)+"z :"+ QString::number(matrix_coord.j_bit_index), QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "avmc00AAAA :"+ QString::number(grid_dimension.x) + " :" + QString::number(grid_dimension.y)+" :" + QString::number(grid_dimension.z), QMessageBox::Ok);
//QMessageBox::information(NULL, "", "activate_hex_surface_matrix_coordinate00 x: "+ QString::number(matrix_coord.x) + " y :"+ QString::number(matrix_coord.y)+" z :"+ QString::number(matrix_coord.z), QMessageBox::Ok);

		int hex_surface_matrix_data_index  = get_hex_surface_matrix_data_index(matrix_coord);
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "activate_hex_surface_matrix_coordinate01 :"+ QString::number(hex_surface_matrix_data_index) + " :" + QString::number(hex_surface_matrix_data[hex_surface_matrix_data_index]), QMessageBox::Ok);

		if (hex_surface_matrix_data_index >= hex_surface_matrix_data.size()) {
//QMessageBox::information(NULL, "", "avmc00BBBBBB :"+ QString::number(iX) + " x :"+ QString::number(iY)+"y :"+ QString::number(iZ)+"z :", QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "avmc01 :"+ QString::number(hex_surface_matrix_data_index), QMessageBox::Ok);
			//return -127;
			return DEFAULT_INACTIVE_VALUE;
		}

		insert_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value, hex_surface_matrix_data_index, hex_surface_status_value);

		return extract_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value, hex_surface_matrix_data_index);// testing only comment out or delete when not in use
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "avmc03 :"+ QString::number(hex_surface_matrix_data[hex_surface_matrix_data_index] ), QMessageBox::Ok);
	}

	void deactivate_hex_surface_matrix_coordinate(hex_index_vector matrix_coord) {
		int hex_surface_matrix_data_index  = get_hex_surface_matrix_data_index(matrix_coord);

		if (hex_surface_matrix_data_index >= hex_surface_matrix_data.size()) {
//QMessageBox::information(NULL, "", "avmc00BBBBBB :"+ QString::number(iX) + " x :"+ QString::number(iY)+"y :"+ QString::number(iZ)+"z :", QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "avmc01 :"+ QString::number(hex_surface_matrix_data_index), QMessageBox::Ok);
			return;
		}

		insert_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value, hex_surface_matrix_data_index, DEFAULT_INACTIVE_VALUE);
	}

	surface_element_data_type hex_surface_matrix_coordinate_activation_status(hex_surface_index_data_type hex_surface_matrix_data_index) {
		if (hex_surface_matrix_data_index >= hex_surface_matrix_data.size()) {
//QMessageBox::information(NULL, "activate_hex_surface_matrix_coordinate", "hex_surface_matrix_data_index >= hex_surface_matrix_data.size() :"+ QString::number(hex_surface_matrix_data_index), QMessageBox::Ok);
			return INVALID_HEX_VALUE;
		}

		surface_element_data_type hex_surface_value = extract_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value,hex_surface_matrix_data_index);
		return hex_surface_value;
	}

	surface_element_data_type hex_surface_matrix_coordinate_activation_status(hex_index_vector matrix_coord) {
		hex_surface_index_data_type hex_surface_matrix_data_index = get_hex_surface_matrix_data_index(matrix_coord);

		if (hex_surface_matrix_data_index >= hex_surface_matrix_data.size()) return INVALID_HEX_VALUE;

		return hex_surface_matrix_coordinate_activation_status(hex_surface_matrix_data_index);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool create_hex_grid() {
		hex_surface_matrix_data.clear();
		hex_surface_matrix_data.shrink_to_fit();


		// ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
		float x_size = hex_surface_generator_parameters.x_end - hex_surface_generator_parameters.x_start;
		float y_size = hex_surface_generator_parameters.y_end - hex_surface_generator_parameters.y_start;

		float x_res_step = hex_surface_generator_parameters.resolution_step * 2.0;
		float y_res_step = hex_surface_generator_parameters.resolution_step * (3.0 / sqrt(3.0));

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 00: "+QString::number(data_set_x_size)+":"+QString::number(data_set_y_size)+":"+QString::number(data_set_z_size)+":", QMessageBox::Ok);

		glm::vec3 origin = { hex_surface_generator_parameters.x_start,hex_surface_generator_parameters.y_start,0.0 };

		hex_size = hex_surface_generator_parameters.resolution_step;

		grid_dimension = { data_set_x_size,data_set_y_size};
		grid_origin = origin;
		create_empty_surface_cubic(data_set_x_size, data_set_y_size);
//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 01: "+QString::number(cloud->voxel_object_data.voxel_matrix_data.size())+":", QMessageBox::Ok);

		if (hex_surface_matrix_data.size() > 0)
			return true;
		else
			return false;
	}
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// ++++++++++++++++++++ FUNCTIONS TO FIND HEX STORAGE INDEX LOCATIONS OF   +++++++++++++++++++++
	// ++++++++++++++++++++    A CARTESIAN COORDINATE POINT P IN 2D SPACE      +++++++++++++++++++++

	/*
		Following code is to find which hex cell of a defined hex size, origin and dimensions
		that a point P of Cartesian coordinate (x,y) is to occupy.

		!!!!!!!!!!!!!! NOT YET FULLY TESTED !!!!!!!!!!!!!!!

		Usage

		//Get the index of the one dimensiona vector array that the point P of coordinate (x,y)
		//is designated to be assigned to
		hex_surface_index_data_type index = index_of_hex_cell_with_cartesian_coord(x,y);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		hex_surface_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y) will be within the bounds of a 2D hegagon voxel cell.
	hex_surface_index_data_type index_of_hex_cell_with_cartesian_coord(float x, float y) {
		index_vector3 hex_coord = hexagon_cell_coord_from_cartesian(x,y);

		if (cartesian_coord_within_grid_bounds(hex_coord))
			return get_index_value(hex_coord.x, hex_coord.y, hex_coord.z);
		else
			return - 1;
	}

	// Determine if a point P of cartesian coordinte (x,y) is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(float x, float y) {
		index_vector3 hex_coord = hexagon_cell_coord_from_cartesian(x,y);

		return cartesian_coord_within_grid_bounds(hex_coord);
	}

	bool cartesian_coord_within_grid_bounds(index_vector3 hex_coord) {
		if (hex_coord.x < 0 || hex_coord.y < 0) return false;

		if (hex_coord.y % 2 == 0){// even row
			if (hex_coord.x > grid_dimension.x) return false;
		}
		else {// odd row
			if (hex_coord.x > grid_dimension.x - 1) return false;
		}

		if (hex_coord.y > grid_dimension.y) return false;

		return true;
	}

	// Obtain the hex grid index coordinates of the hex grid that a point P of cartesian coordinte
	// (x,y) will be found to be within the bounds of a 2D hexgagon voxel cell.
	index_vector3 hexagon_cell_coord_from_cartesian(float x, float y) {
		//float grid_radius = hex_size / 2.0;
		float grid_radius = hex_size ;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));
		
//printf("hexagon_cell_coord_from_cartesian 000 voxel_size : %f :grid_height %f : grid_radius %f : c %f \n", hex_size, grid_height, grid_radius,c);
//printf("hexagon_cell_coord_from_cartesian 111 grid_origin :x %f :y %f \n", grid_origin.x, grid_origin.y);

		float grid_x = x - grid_origin.x;
		float grid_y = y - grid_origin.y;

//printf("hexagon_cell_coord_from_cartesian 222 :x %f :y %f \n", grid_x, grid_y);

		int row, column;

		if (grid_y < -1.0 / sqrt(3.0))
			row = (int)((grid_y - grid_height) / grid_height);
		else
			row = (int)(grid_y / grid_height);

		bool row_is_odd = abs(row % 2) == 1;

		if (row_is_odd)
			column = (int)floor(grid_x / (hex_size*2.0f));
		else
			column = (int)floor((grid_x + grid_radius) / (hex_size * 2.0f));

//printf("hexagon_cell_coord_from_cartesian 333 :row %i :col %i \n",row, column);
		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;

		if (row_is_odd)
			rel_x = (grid_x - ((column * (hex_size * 2.0f)) + grid_radius));
		else
			rel_x = grid_x - (column * (hex_size * 2.0f));

		float m = 1.0f / sqrt(3.0f);

//printf("hexagon_cell_coord_from_cartesian 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0){ // LEFT edge
//printf("hexagon_cell_coord_from_cartesian 555 :row %i :col %i \n",row, column);
			row++;
			if (!row_is_odd)
				column--;
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) { // RIGHT edge
//printf("hexagon_cell_coord_from_cartesian 666 :row %i :col %i \n", row, column);
				row++;
				if (row_is_odd)
					column++;
			}
		}

		index_vector3 hex_coord;
		hex_coord.x = column;
		hex_coord.y = row;
		hex_coord.z = 0;

		return hex_coord;
	}

	// +++++++++++++++++++++++++++++++++++++++++

private:
	void create_hex_surface_grid(hex_surface_data_type volume_data_storage, size_t xdim, size_t ydim) {
		if (!xdim || !ydim) return;

		if (hex_surface_matrix_data.size() > 0) { hex_surface_matrix_data.clear(); hex_surface_matrix_data.shrink_to_fit(); }

		hex_surface_index_data_type surface_grid_data_size = calculate_hex_surface_matrix_data_size(xdim, ydim);

		hex_surface_matrix_data = std::vector<hex_surface_data_type>(surface_grid_data_size, volume_data_storage);

//QMessageBox::information(NULL, "hexBS03", "create_solid_volume_cubic :"+QString::number(xdim * corrected_ydim * zdim *sizeof(hex_surface_data_type)), QMessageBox::Ok);
	}
};