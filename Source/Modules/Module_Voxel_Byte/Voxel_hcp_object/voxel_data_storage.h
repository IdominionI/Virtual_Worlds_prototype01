#pragma once

#include <vector>

#include "parameters.h"
#include "Datatypes/dt_voxel_generator.h"

#include <Source/Graphics_Engine/Shader/shader_parameters.h>
#include "DataTypes/dt_voxel_automata.h"

// Because OpenGL GLSL does not have a prefered 8 bit unsigned char datatype,
// and the min datatype that can be expressed as a bitmap 
// is a 32bit unsigned integer, then that is what is used here
//typedef unsigned int voxel_data_type;
//#define VOXEL_BIT_NUMBER 32

// Ambition to change to type char with min value -127 max value 127
#define INVALID_VOXEL_VALUE 0
#define MIN_VOXEL_VALUE     1
#define MAX_VOXEL_VALUE     255

#define DEFAULT_ACTIVE_VALUE   255
#define DEFAULT_INACTIVE_VALUE 0 

typedef int index_data_type;

struct index_struct_type {
	index_data_type x = 0, y = 0, z = 0;
};

typedef index_struct_type index_vector;

//#define MAX_VOXEL_VERTICES   178956904 //INT_MAX/(3 *sizeof(GLfloat)); number of x,y,z vertices of data type float which is assumed to use four bytes
//#define MAX_VOXEL_VERTICES    64000000 // On a 16 gig rig have this value as a safe number as testin reveals this uses about 10 gig memory.

/*
	voxel_object_data_class

	C++ Class that defines and manages the hcp voxel data matrix that represents the point cloud
	data of the hcp voxel data object.
*/

class voxel_object_data_class {
public:

	float	     voxel_size           = 1.0f;
	glm::vec3    matrix_origin        = { 0.0,0.0,0.0 };
	glm::vec3    matrix_coordinate_scale_factors = { 1.0,1.0,1.0 }; // axis scale that each index node multiplied by to give real world x,y,z corrdinate value
	index_vector matrix_dimension     = { 0,0,0 };
	bool	     display_points       = true;
	bool	     perfom_rounding_up   = true;
	bool	     voxel_surface_volume = false;

	voxel_generator_parameters_struct_type			      voxel_generator_parameters;
	shader_parameters_struct_type					      shader_parameters;
	std::vector<voxel_hcp_automata_byte_rule_struct_type> voxel_hcp_automata_byte_rules;

	index_vector			   voxel_matrix_coord_index_vector;
	std::vector <voxel_data_type>  voxel_matrix_data;

	~voxel_object_data_class() {
		voxel_matrix_data.clear();
		voxel_matrix_data.shrink_to_fit();
	}


	index_data_type get_z_layer_index_value(index_data_type iX, index_data_type iY, index_data_type iZ) {
//QMessageBox::information(NULL, "", "get_z_layer_index_value00 x: "+ QString::number(iX) + " y :"+ QString::number(iY)+" z :"+ QString::number(iZ), QMessageBox::Ok);

		if (iZ % 2 == 0) // Even z level
			return (index_data_type(iY / 2) + iY % 2) * matrix_dimension.x + index_data_type(iY / 2) * (matrix_dimension.x - 1) + iX;
		else // Odd z level
			return (index_data_type(iY / 2) + iY % 2) * (matrix_dimension.x - 1) + index_data_type(iY / 2) * matrix_dimension.x + iX;
	}

	index_data_type get_index_value(index_data_type iX, index_data_type iY, index_data_type iZ) {
//QMessageBox::information(NULL, "", "get_index_value00 x: "+ QString::number(iX) + " y :"+ QString::number(iY)+" z :"+ QString::number(iZ), QMessageBox::Ok);

		return (index_data_type(iZ / 2) + iZ % 2) * get_z_layer_total(0) + index_data_type(iZ / 2) * get_z_layer_total(1) + get_z_layer_index_value(iX, iY, iZ);
	}

	index_data_type get_z_layer_total(index_data_type iZ, index_data_type xdim, index_data_type ydim) {
		if (iZ % 2 == 0) // Even z level
			return (index_data_type(ydim / 2) + ydim % 2) * xdim + index_data_type(ydim / 2) * (xdim - 1);
		else // Odd z level
			return (index_data_type((ydim - 1) / 2) + (ydim - 1) % 2) * (xdim - 1) + index_data_type((ydim - 1) / 2) * xdim;
	}

	index_data_type get_z_layer_total(index_data_type iZ) {
		return get_z_layer_total(iZ, matrix_dimension.x, matrix_dimension.y);
	}

	index_data_type calculate_voxel_matrix_data_size(index_data_type xdim, index_data_type ydim, index_data_type zdim) {
		return  (index_data_type(zdim / 2) + zdim % 2) * get_z_layer_total(0, xdim, ydim) + index_data_type(zdim / 2) * get_z_layer_total(1, xdim, ydim);
	}

	index_data_type calculate_voxel_matrix_data_size() {
		return calculate_voxel_matrix_data_size(matrix_dimension.x, matrix_dimension.y, matrix_dimension.z);
	}

	// END voxel matrix index 

	// ***************** VOXEL DATA ELEMENT FUNCTIONS TO COMPRESS INTO AND FROM DEFINED voxel_data_type storage data type of a unsigned integer *************************

	voxel_element_data_type extract_voxel_data_element_value(data_storage_type_enum voxel_data_element_value_type, index_data_type voxel_matrix_data_index) {
		if (voxel_matrix_data_index < 0 || voxel_matrix_data_index >= voxel_matrix_data.size())
			return INVALID_VOXEL_VALUE;

		switch (voxel_data_element_value_type) {
		case data_storage_type_enum::alpha:
		case data_storage_type_enum::value: return (voxel_matrix_data[voxel_matrix_data_index] & 0x000000ff);       break; // First 8 bits of 32 bit unsigned integer

		case data_storage_type_enum::red: return  voxel_matrix_data[voxel_matrix_data_index] >> 24;               break; // Last 8 bits of 32 bit unsigned integer
		case data_storage_type_enum::green: return (voxel_matrix_data[voxel_matrix_data_index] & 0x00ff0000) >> 16; break; // Thrid 8 bits of 32 bit unsigned integer
		case data_storage_type_enum::blue: return (voxel_matrix_data[voxel_matrix_data_index] & 0x0000ff00) >> 8;  break; // second 8 bits of 32 bit unsigned integer

		default: return INVALID_VOXEL_VALUE;
		}
	}

	std::vector<voxel_element_data_type> extract_voxel_data_element_values(index_data_type voxel_matrix_data_index) {
		std::vector<voxel_element_data_type> voxel_data_element_values = { INVALID_VOXEL_VALUE,INVALID_VOXEL_VALUE,INVALID_VOXEL_VALUE,INVALID_VOXEL_VALUE };

		if (voxel_matrix_data_index >= 0 && voxel_matrix_data_index < voxel_matrix_data.size()) {
			voxel_data_element_values[0] = (voxel_matrix_data[voxel_matrix_data_index] & 0x000000ff);       // alpha - voxel value;
			voxel_data_element_values[1] = (voxel_matrix_data[voxel_matrix_data_index] & 0x0000ff00) >> 8;  // blue
			voxel_data_element_values[2] = (voxel_matrix_data[voxel_matrix_data_index] & 0x00ff0000) >> 16; // green
			voxel_data_element_values[3] = voxel_matrix_data[voxel_matrix_data_index] >> 24;                // red
		}

		return voxel_data_element_values;
	}

	voxel_data_type insert_voxel_data_element_value(data_storage_type_enum voxel_data_element_value_type, index_data_type voxel_matrix_data_index, voxel_element_data_type value) {
		std::vector<voxel_element_data_type> voxel_data_element_values = extract_voxel_data_element_values(voxel_matrix_data_index);

		switch (voxel_data_element_value_type) {
		case data_storage_type_enum::alpha:
		case data_storage_type_enum::value: voxel_data_element_values[0] = value; break; // First 8 bits of 32 bit unsigned integer
		case data_storage_type_enum::blue: voxel_data_element_values[1] = value; break;  // second 8 bits of 32 bit unsigned integer
		case data_storage_type_enum::green: voxel_data_element_values[2] = value; break; // Thrid 8 bits of 32 bit unsigned integer
		case data_storage_type_enum::red: voxel_data_element_values[3] = value; break; // Last 8 bits of 32 bit unsigned integer
		}

		voxel_matrix_data[voxel_matrix_data_index] = voxel_data_element_values[3] << 24 | // red
			voxel_data_element_values[2] << 16 | // green
			voxel_data_element_values[1] << 8 |  // blue
			voxel_data_element_values[0];        // alpha - voxel value

		return voxel_matrix_data[voxel_matrix_data_index];
	}

	// ********************************************************************************************
	// ***************** VOXEL DATA ELEMENT FUNCTIONS TO COMPRESS INTO AND FROM DEFINED voxel_data_type storage data type of float *************************
	// *********** Obtained method from question and answer posted on stackoverflow but only is valid for first three RGB color values *************
/*	const std::vector4D bitEnc = std::vector4D(1.0f, 255.0f, 65025.0f, 16581375.0f);
	const std::vector4D bitDec = { 1.0f / bitEnc.x(),1.0f / bitEnc.y(),1.0f / bitEnc.z(),1.0f / bitEnc.w() };

	std::vector4D Encode_Float_to_RGBA(float v) {
		std::vector4D enc = bitEnc * v;
		//enc = fract(enc);
		enc = { enc.x() - floor(enc.x()),enc.y() - floor(enc.y()),enc.z() - floor(enc.z()),enc.w() - floor(enc.w())};
		//enc -= enc.yzww * vec2(1. / 255., 0.).xxxy;
		enc -= {enc.y()/255.0f, enc.z() / 255.0f, enc.w() / 255.0f, 0.0f};
		return enc;
	}

	float Encode_RGBA_to_float(std::vector4D v) {
		return std::vector4D::dotProduct(v, bitDec);
	}
*/
// ********************************************************************************************

	index_vector digitise_point_coordinate(glm::vec3 point_location) {
		index_vector digitised_point_coordinate = { 0,0,0 };
		glm::vec3  rounding_up_value = { 0.0, 0.0, 0.0 };

		if (matrix_coordinate_scale_factors.x <= 0.0 || matrix_coordinate_scale_factors.y <= 0.0 || matrix_coordinate_scale_factors.z <= 0.0)
			return{ 0,0,0 };

		//  digtised point cordinates are rounded up to nearest integer value if set
		if (perfom_rounding_up) {
			if (point_location.x > 0.0) rounding_up_value.x = matrix_coordinate_scale_factors.x / 2; else rounding_up_value.x= -(matrix_coordinate_scale_factors.x / 2);
			if (point_location.y > 0.0) rounding_up_value.y = matrix_coordinate_scale_factors.y / 2; else rounding_up_value.y = -(matrix_coordinate_scale_factors.y / 2);
			if (point_location.z > 0.0) rounding_up_value.z = matrix_coordinate_scale_factors.z / 2; else rounding_up_value.z = -(matrix_coordinate_scale_factors.z / 2);
		}

		digitised_point_coordinate.x = index_data_type((point_location.x + rounding_up_value.x) / matrix_coordinate_scale_factors.x);
		digitised_point_coordinate.y = index_data_type((point_location.y + rounding_up_value.y) / matrix_coordinate_scale_factors.y);
		digitised_point_coordinate.z = index_data_type((point_location.z + rounding_up_value.z) / matrix_coordinate_scale_factors.z);

		return digitised_point_coordinate;
	}

	void create_empty_volume_cubic(size_t xdim, size_t ydim, size_t zdim) {
		voxel_data_type volume_data_storage = voxel_data_type(DEFAULT_INACTIVE_VALUE);

		create_volume_cubic(volume_data_storage, xdim, ydim, zdim);
	}

	void create_solid_volume_cubic(size_t xdim, size_t ydim, size_t zdim) {
		voxel_data_type volume_data_storage = voxel_data_type(DEFAULT_ACTIVE_VALUE);

		create_volume_cubic(volume_data_storage, xdim, ydim, zdim);
	}

	void delete_voxel_matrix_data() {
		voxel_matrix_data.clear();
		voxel_matrix_data.shrink_to_fit();
	}

	void deactivate_voxel_matrix() {
		index_data_type i;
//QMessageBox::information(NULL, "voxelBS03", "deactivate_voxel_matrix01  :"+QString::number(voxel_matrix_data.size()), QMessageBox::Ok);

		for (i = 0; i < voxel_matrix_data.size(); i++)
			voxel_matrix_data[i] = voxel_data_type(DEFAULT_INACTIVE_VALUE);

	}

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	index_vector get_matrix_coordinate(index_data_type matrix_index) {
		index_vector matrix_coord;

		index_data_type t0 = get_z_layer_total(0);// total number for even z level
		index_data_type t1 = get_z_layer_total(1);// total number for odd  z level

		index_data_type zt = t0 + t1; // total number for even + odd z levels

		bool even_z_level;

		if (matrix_index < t0) {
			matrix_coord.z = 0;
			even_z_level = true;
		}
		else {
			index_data_type zt_index01 = index_data_type(matrix_index / zt);
			index_data_type zt_index02 = index_data_type((matrix_index - zt_index01 * zt) / t0);

			if (zt_index02 > 0) { // Have matrix_index in the odd  z level
				matrix_coord.z = zt_index01 * 2 + 1;
				even_z_level = false;
			}
			else {// Have matrix_index in the even z level
				matrix_coord.z = zt_index01 * 2;
				even_z_level = true;
			}
		}

		// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z voxel coordinate
		index_data_type zeroth_index = matrix_index - get_index_value(0, 0, matrix_coord.z);

		index_data_type yt = 2 * matrix_dimension.x - 1; // Even row number of voxels + odd row of voxels of any z level

		if (even_z_level) {
			index_data_type yt_index01, yt_index02;

			yt_index01 = index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < matrix_dimension.x)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01 * 2 + yt_index02;
			matrix_coord.x = zeroth_index - ((index_data_type(matrix_coord.y / 2) + matrix_coord.y % 2) * matrix_dimension.x + index_data_type(matrix_coord.y / 2) * (matrix_dimension.x - 1));
		}
		else {
			index_data_type yt_index01, yt_index02;

			yt_index01 = index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < matrix_dimension.x - 1)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01 * 2 + yt_index02;

//if(matrix_coord.z > 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 0BBB: " + QString::number(matrix_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(yt_index01) + " : " + QString::number(yt_index02) + ":" + QString::number((zeroth_index - (yt_index01 * yt + yt_index02 * (matrix_dimension.x - 1)))), QMessageBox::Ok);
//			}

			matrix_coord.x = zeroth_index - ((index_data_type(matrix_coord.y / 2) + matrix_coord.y % 2) * (matrix_dimension.x - 1) + index_data_type(matrix_coord.y / 2) * matrix_dimension.x);
		}

//if(matrix_coord.z == 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 01: " + QString::number(matrix_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(matrix_coord.x) + " : " + QString::number(matrix_coord.y) + ":" + QString::number(matrix_coord.z), QMessageBox::Ok);
//}

		return matrix_coord;
	}

	glm::vec3 get_voxel_cartesian_coordinate(index_vector voxel_coord, float voxel_size) {
		index_data_type i = voxel_coord.x, j = voxel_coord.y, k = voxel_coord.z;

		float sqrt3 = sqrt(3.0), third = 1.0 / 3.0, z_mult = 2.0 * sqrt(6.0) / 3.0, sqrt3_2 = sqrt(1.5);
		glm::vec3 voxel_cartesian_coordinate;

		if (j % 2 == 0) {
//QMessageBox::information(NULL, "voxelBS03", "here00 :"+QString::number(j * VOXEL_BIT_NUMBER + j_bit), QMessageBox::Ok);
			voxel_cartesian_coordinate.x = ((float(i) * 2 + float(k % 2)) * voxel_size);
			voxel_cartesian_coordinate.y =((sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * voxel_size);
		}
		else {
//QMessageBox::information(NULL, "voxelBS03", "here01 :"+QString::number(j * VOXEL_BIT_NUMBER + j_bit), QMessageBox::Ok);
			//voxel_cartesian_coordinate.setX((1.0 + float(i) * 2 + float(k % 2)) * voxel_size - 2.0 * (k % 2)); the old wrong one
			voxel_cartesian_coordinate.x = ((-1.0 + float(i) * 2 + float(k % 2) + 2 * float((k + 1) % 2)) * voxel_size);
			voxel_cartesian_coordinate.y = ((sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * voxel_size);
		}

	     voxel_cartesian_coordinate.z = (z_mult * float(k) *voxel_size);

		return voxel_cartesian_coordinate;
	}

	glm::vec3 get_voxel_cartesian_coordinate(index_vector voxel_coord) {
		return get_voxel_cartesian_coordinate(voxel_coord, voxel_size);
	}



	index_data_type get_voxel_matrix_data_index(index_vector matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_voxel_matrix_bit_location
		return get_index_value(matrix_coord.x, matrix_coord.y, matrix_coord.z);
	}

	voxel_element_data_type activate_voxel_matrix_coordinate(index_vector matrix_coord, voxel_element_data_type voxel_status_value = DEFAULT_ACTIVE_VALUE) {// testing only comment out or delete when not in use

//QMessageBox::information(NULL, "", "avmc00 :"+ QString::number(matrix_coord.matrix_index.x) + " x :"+ QString::number(matrix_coord.matrix_index.y)+"y :"+ QString::number(matrix_coord.matrix_index.z)+"z :"+ QString::number(matrix_coord.j_bit_index), QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "avmc00AAAA :"+ QString::number(matrix_dimension.x) + " :" + QString::number(matrix_dimension.y)+" :" + QString::number(matrix_dimension.z), QMessageBox::Ok);
//QMessageBox::information(NULL, "", "activate_voxel_matrix_coordinate00 x: "+ QString::number(matrix_coord.x) + " y :"+ QString::number(matrix_coord.y)+" z :"+ QString::number(matrix_coord.z), QMessageBox::Ok);

		int voxel_matrix_data_index = get_voxel_matrix_data_index(matrix_coord);
		//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "activate_voxel_matrix_coordinate01 :"+ QString::number(voxel_matrix_data_index) + " :" + QString::number(voxel_matrix_data[voxel_matrix_data_index]), QMessageBox::Ok);

		if (voxel_matrix_data_index >= voxel_matrix_data.size()) {
//QMessageBox::information(NULL, "", "avmc00BBBBBB :"+ QString::number(iX) + " x :"+ QString::number(iY)+"y :"+ QString::number(iZ)+"z :", QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "avmc01 :"+ QString::number(voxel_matrix_data_index), QMessageBox::Ok);
			//return -127;
			return DEFAULT_INACTIVE_VALUE;
		}

		insert_voxel_data_element_value(data_storage_type_enum::value, voxel_matrix_data_index, voxel_status_value);

		return extract_voxel_data_element_value(data_storage_type_enum::value, voxel_matrix_data_index);// testing only comment out or delete when not in use
//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "avmc03 :"+ QString::number(voxel_matrix_data[voxel_matrix_data_index] ), QMessageBox::Ok);
	}

	void deactivate_voxel_matrix_coordinate(index_vector matrix_coord) {
		int voxel_matrix_data_index = get_voxel_matrix_data_index(matrix_coord);

		if (voxel_matrix_data_index >= voxel_matrix_data.size()) {
//QMessageBox::information(NULL, "", "avmc00BBBBBB :"+ QString::number(iX) + " x :"+ QString::number(iY)+"y :"+ QString::number(iZ)+"z :", QMessageBox::Ok);
//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "avmc01 :"+ QString::number(voxel_matrix_data_index), QMessageBox::Ok);
			return;
		}

		insert_voxel_data_element_value(data_storage_type_enum::value, voxel_matrix_data_index, DEFAULT_INACTIVE_VALUE);
	}

	voxel_element_data_type voxel_matrix_coordinate_activation_status(index_data_type voxel_matrix_data_index) {
		if (voxel_matrix_data_index >= voxel_matrix_data.size()) {
//QMessageBox::information(NULL, "activate_voxel_matrix_coordinate", "voxel_matrix_data_index >= voxel_matrix_data.size() :"+ QString::number(voxel_matrix_data_index), QMessageBox::Ok);
			return INVALID_VOXEL_VALUE;
		}

		voxel_element_data_type voxel_value = extract_voxel_data_element_value(data_storage_type_enum::value, voxel_matrix_data_index);
		return voxel_value;
	}

	voxel_element_data_type voxel_matrix_coordinate_activation_status(index_vector matrix_coord) {
		index_data_type voxel_matrix_data_index = get_voxel_matrix_data_index(matrix_coord);

		if (voxel_matrix_data_index >= voxel_matrix_data.size()) return INVALID_VOXEL_VALUE;

		return voxel_matrix_coordinate_activation_status(voxel_matrix_data_index);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//bool create_voxel_matrix(voxel_hcp_object_class* cloud, voxel_generator_parameters_struct_type  voxel_generator_parameters) {
	bool create_voxel_matrix() {
		voxel_matrix_data.clear();   // ******
		voxel_matrix_data.shrink_to_fit(); // ******


		// ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
		float x_size = voxel_generator_parameters.x_end - voxel_generator_parameters.x_start;
		float y_size = voxel_generator_parameters.y_end - voxel_generator_parameters.y_start;
		float z_size = voxel_generator_parameters.z_end - voxel_generator_parameters.z_start;
		float z_mult = 2.0 * sqrt(6.0) / 3.0;

		float x_res_step = voxel_generator_parameters.resolution_step * 2.0;
		float y_res_step = voxel_generator_parameters.resolution_step * (3.0 / sqrt(3.0));
		float z_res_step = voxel_generator_parameters.resolution_step * z_mult;

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0)
			data_set_z_size = (int)(z_size / z_res_step) + 1;
		else
			data_set_z_size = (int)(z_size / z_res_step);
//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 00: "+QString::number(data_set_x_size)+":"+QString::number(data_set_y_size)+":"+QString::number(data_set_z_size)+":", QMessageBox::Ok);

		glm::vec3 origin = { voxel_generator_parameters.x_start,voxel_generator_parameters.y_start,voxel_generator_parameters.z_start };

		voxel_size = voxel_generator_parameters.resolution_step;

		matrix_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
		matrix_origin = origin;
		create_empty_volume_cubic(data_set_x_size, data_set_y_size, data_set_z_size);
//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 01: "+QString::number(cloud->voxel_object_data.voxel_matrix_data.size())+":", QMessageBox::Ok);

		if (voxel_matrix_data.size() > 0)
			return true;
		else
			return false;
	}
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// ++++++++++++++++++++ FUNCTIONS TO FIND VOXEL STORAGE INDEX LOCATIONS OF   +++++++++++++++++++++
	// ++++++++++++++++++++    A CARTESIAN COORDINATE POINT P IN 3D SPACE        +++++++++++++++++++++

	/*
		Following code is to find which voxel cell of a defined voxel size, origin and dimensions
		that a 3D point P of Cartesian coordinate (x,y,z) is to occupy.

		Usage

		//Get the index of the one dimensiona vector array that the point P of coordinate (x,y,z)
		//is designated to be assigned to
		index_data_type index = index_of_voxel_cell_with_cartesian_coord(x,y,z);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		voxel_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y,z) will be within the bounds of a 3D voxel cell.
	index_data_type index_of_voxel_cell_with_cartesian_coord(float x, float y,float z) {
//printf("index_of_voxel_cell_with_cartesian_coord 000 :x %f :y %f :z %f \n", x, y, z);
		index_vector voxel_coord = hcp_voxel_cell_coord_from_cartesian(x,y,z);
//printf("index_of_voxel_cell_with_cartesian_coord 111 :x %i :y %i :z %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
		if (cartesian_coord_within_matrix_bounds(voxel_coord))
			return get_voxel_matrix_data_index(voxel_coord);
		else
			return -1;
	}
	
	// Determine if a point P of cartesian coordinte (x,y,z) is within the limits of
	// the dimensions of the voxel matrix that is stored in the computer memory
	bool cartesian_coord_within_matrix_bounds(float x, float y,float z) {

		index_vector voxel_coord = hcp_voxel_cell_coord_from_cartesian(x, y, z);

		return cartesian_coord_within_matrix_bounds(voxel_coord);
	}

	bool cartesian_coord_within_matrix_bounds(index_vector voxel_coord) {
		if (voxel_coord.x < 0 || voxel_coord.y < 0 || voxel_coord.z < 0) return false;

		if (voxel_coord.z % 2 == 0) {// even level
			if (voxel_coord.y % 2 == 0){// even row
				if (voxel_coord.x >= matrix_dimension.x) return false;
			} else {// odd row
				if (voxel_coord.x >= matrix_dimension.x - 1) return false;
			}
		} else {// odd level
			if(voxel_coord.y % 2 == 0){// even row
				if (voxel_coord.x >= matrix_dimension.x-1) return false;
			}
			else {// odd row
				if (voxel_coord.x >= matrix_dimension.x) return false;
			}
		}

		if (voxel_coord.y >= matrix_dimension.y || voxel_coord.z >= matrix_dimension.z) return false;

		return true;
	}

	// Obtain the voxel matrix index coordinates of the voxel matrix that a point P of cartesian coordinte
	// (x,y,z) will be found to be within the bounds of a 3D voxel cell.
	index_vector hcp_voxel_cell_coord_from_cartesian(float x, float y, float z) {
		float voxel_radius = voxel_size;
		float voxel_height = voxel_radius * (sqrt(1.5f) + sqrt(6) / 3);
		float grid_x, grid_y;

		float grid_z = z - matrix_origin.z;

		int level;

		if(grid_z < -sqrt(6)/3)
			level = (int)((grid_z- voxel_height) / voxel_height);
		else
			level = (int)(grid_z / voxel_height);

		bool level_is_even = level % 2 == 0;

		if (level_is_even) {
			grid_x = x - matrix_origin.x;
			grid_y = y - matrix_origin.y;
			return get_hcp_voxel_cell_coord_even_level(grid_x, grid_y, grid_z, voxel_height, level);
		} else {
			grid_x = x - (matrix_origin.x + voxel_radius);
			grid_y = y - (matrix_origin.y + voxel_radius / sqrt(3.0f));
			return get_hcp_voxel_cell_coord_odd_level(grid_x, grid_y, grid_z, voxel_height, level);
		}
	}

	index_vector get_hcp_voxel_cell_coord_even_level(float grid_x, float grid_y, float grid_z, float voxel_height, int level) {
		float grid_radius = voxel_size;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));

//printf("get_hcp_voxel_cell_coord_even_level 000 hex_size : %f :grid_height %f : grid_radius %f : c %f \n", voxel_size, grid_height, grid_radius,c);
//printf("get_hcp_voxel_cell_coord_even_level 111 grid_origin :x %f :y %f :z %f \n", matrix_origin.x, matrix_origin.y, matrix_origin.z);
//printf("get_hcp_voxel_cell_coord_even_level 222 :x %f :y %f :z %f \n", grid_x, grid_y, grid_z);

		int row, column;

		if (grid_y < -1.0 / sqrt(3.0))
			row = (int)((grid_y - grid_height) / grid_height);
		else
			row = (int)(grid_y / grid_height);

		bool row_is_odd = abs(row % 2) == 1;

		if (row_is_odd) {
			column = (int)floor(grid_x / (voxel_size*2));
		} else {
			column = (int)floor((grid_x + grid_radius) / (voxel_size * 2));
		}

		
//printf("get_hcp_voxel_cell_coord_even_level 333 :row %i :col %i :level %i \n", row, column, level);
		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;
		float rel_z = grid_z - (level * voxel_height);

		if (row_is_odd){
			rel_x = grid_x - ((column * (voxel_size*2)) + grid_radius);
		} else {
			rel_x = grid_x - (column * (voxel_size * 2));
		}

		float m = 1.0f / sqrt(3.0f);

//printf("get_hcp_voxel_cell_coord_even_level 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0) { // LEFT edge
			row++;
			if (!row_is_odd) { // Have even row
				column--;
				row_is_odd = true;
				rel_x = grid_x - ((column * (voxel_size * 2)) + grid_radius);
			}else
				rel_x = rel_x + grid_radius;
			
			rel_y = grid_y - (row * grid_height);
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) {// RIGHT edge
				row++;
				if (row_is_odd) {
					column++;
					row_is_odd = false;
					rel_x = grid_x - (column * (voxel_size * 2));
				} else
					rel_x = rel_x - grid_radius;
				
				rel_y = grid_y - (row * grid_height);
			}
		}

		index_vector voxel_coord;
		voxel_coord.x = column;
		voxel_coord.y = row;

//printf("get_hcp_voxel_cell_coord_even_level 4AA :rel_x %f :rel_y %f : rel_z %f \n", rel_x,rel_y,rel_z);
//printf("get_hcp_voxel_cell_coord_even_level 555 :row %i :col %i \n",voxel_coord.y, column);
		// Get voxel z coord
		// Find distance to upper plane of the zone that the point exists in of the voxel
		// and if it is below the plane (ie distance negative) it is in the voxel, otherwise
		// it is in the neighbouring voxel of the upper level.

		float m0 = -1 / sqrt(3);//slope of line in xy plane from point point 6 to point 3
		float m1 =  1 / sqrt(3);//slope of line in xy plane from point point 5 to point 2

		glm::vec3 pq_vector = { rel_x,rel_y,rel_z - sqrt(1.5f) }; //distance vector from rel_z to top voxel vertex point.

		if (rel_x >= 0 && rel_y > m0 * rel_x) {// region 0 
			glm::vec3 normal_vector = { 1.0f,1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (row_is_odd)	voxel_coord.x++;
			}
//printf("get_hcp_voxel_cell_coord_even_level 666 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_y <= m0*rel_x && rel_y < m1 * rel_x) {// region 1 
			glm::vec3 normal_vector = { 0.0f,-1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.y--;
				voxel_coord.z = level + 1;
			}
//printf("get_hcp_voxel_cell_coord_even_level 777 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_x < 0 && rel_y >= m1 * rel_x) {// region 2 
			glm::vec3 normal_vector = {-1.0f,-1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (!row_is_odd) voxel_coord.x--; 
			}
//printf("get_hcp_voxel_cell_coord_even_level 888 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}
	}

	index_vector get_hcp_voxel_cell_coord_odd_level(float grid_x, float grid_y, float grid_z, float voxel_height, int level) {
		float grid_radius = voxel_size;
		//float grid_radius = voxel_size / 2.0;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));

//printf("get_hcp_voxel_cell_coord_odd_level 000 hex_size : %f :grid_height %f : grid_radius %f : c %f \n", voxel_size, grid_height, grid_radius,c);
//printf("get_hcp_voxel_cell_coord_odd_level 111 grid_origin :x %f :y %f :z %f \n", matrix_origin.x, matrix_origin.y, matrix_origin.z);
//printf("get_hcp_voxel_cell_coord_odd_level 222 :x %f :y %f :z %f \n", grid_x, grid_y, grid_z);

		int row;
		int column;

		if (grid_y < -1.0 / sqrt(3.0))
			row = (int)((grid_y - grid_height) / grid_height);
		else
			row = (int)(grid_y / grid_height);

		bool row_is_odd = abs(row % 2) == 1;

		column = (int)floor((grid_x + grid_radius) / (voxel_size * 2));

//printf("get_hcp_voxel_cell_coord_odd_level 333 :row %i :col %i :level %i \n", row, column, level);

		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;
		float rel_z = grid_z - (level * voxel_height);

		if (row_is_odd){
			rel_x = grid_x - (column * (voxel_size * 2)) + grid_radius;
		} else {
			rel_x = grid_x - (column * (voxel_size * 2));
		}

		float m = 1.0f / sqrt(3.0f);

//printf("get_hcp_voxel_cell_coord_odd_level 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0) { // LEFT edge
			row++;
			if (row_is_odd) { // Have even row
				column--;
				row_is_odd = true;
				rel_x = grid_x - ((column * (voxel_size * 2)) + grid_radius);
			}else
				rel_x = rel_x + grid_radius;
			
			rel_y = grid_y - (row * grid_height);
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) {// RIGHT edge
				row++;
				if (!row_is_odd) {
					column++;
					row_is_odd = false;
					rel_x = grid_x - (column * (voxel_size * 2));
				} else
					rel_x = rel_x - grid_radius;
				
				rel_y = grid_y - (row * grid_height);
			}
		}

		index_vector voxel_coord;
		voxel_coord.x = column;
		voxel_coord.y = row;

//printf("get_hcp_voxel_cell_coord_odd_level 4AA :rel_x %f :rel_y %f : rel_z %f \n", rel_x,rel_y,rel_z);
//printf("get_hcp_voxel_cell_coord_odd_level 555 :row %i :col %i \n",row, column);
		
		// Get voxel z coord
		// Find distance to upper plane of the zone that the point exists in of the voxel
		// and if it is below the plane (ie distance negative) it is in the voxel, otherwise
		// it is in the neighbouring voxel of the upper level.

		float m0 = -1 / sqrt(3);//slope of line in xy plane from point point 6 to point 3
		float m1 =  1 / sqrt(3);//slope of line in xy plane from point point 5 to point 2

		glm::vec3 pq_vector = { rel_x,rel_y,rel_z - sqrt(1.5f) }; //distance vector from rel_z to top voxel vertex point.

		if (rel_x <= 0 && rel_y < m0 * rel_x) { //region 0
			glm::vec3 normal_vector = { -1.0f,-1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (row_is_odd) voxel_coord.x--;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 666 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_y >= m0 * rel_x && rel_y > m * rel_x) { //region 1
			glm::vec3 normal_vector = { 0.0f,1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.y++;
				voxel_coord.z = level + 1;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 777 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_x > 0 && rel_y <= m1 * rel_x) {// region 2 
			glm::vec3 normal_vector = { 1.0f,1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };
			
			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (!row_is_odd) voxel_coord.x++;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 888 :col %i :row %i:level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}
	}

	float distance_to_plane(glm::vec3 normal_vector, glm::vec3 pq_vector) {
		float normal_vector_length = sqrt(normal_vector.x* normal_vector.x+normal_vector.y* normal_vector.y+ normal_vector.z* normal_vector.z);
		float normal_dot_pq        = dot(pq_vector, normal_vector);

//printf("distance_to_plane : %f : %f :%f \n", normal_vector_length, normal_dot_pq,normal_dot_pq / normal_vector_length);
		return normal_dot_pq / normal_vector_length;
	}

	// +++++++++++++++++++++++++++++++++++++++++



private:
	void create_volume_cubic(voxel_data_type volume_data_storage, size_t xdim, size_t ydim, size_t zdim) {
		if (!xdim || !ydim || !zdim) return;

		if (voxel_matrix_data.size() > 0) { voxel_matrix_data.clear(); voxel_matrix_data.shrink_to_fit(); }

		index_data_type voxel_matrix_data_size = calculate_voxel_matrix_data_size(xdim, ydim, zdim);

		voxel_matrix_data = std::vector<voxel_data_type>(voxel_matrix_data_size, volume_data_storage);

//QMessageBox::information(NULL, "voxelBS03", "create_solid_volume_cubic :"+QString::number(xdim * corrected_ydim * zdim *sizeof(voxel_data_type)), QMessageBox::Ok);
	}

};

