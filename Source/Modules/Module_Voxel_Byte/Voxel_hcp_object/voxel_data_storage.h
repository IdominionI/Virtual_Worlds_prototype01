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

	//void extend_matrix(int extension[3]) {
	//	// codes here
	//}

	//void retract_matrix(int extension[3]) {
	//	// codes here
	//}

	//bool matrix_coord_status(index_vector voxel_coord) {
	//	// code_here
	//	return false;
	//}



private:
	void create_volume_cubic(voxel_data_type volume_data_storage, size_t xdim, size_t ydim, size_t zdim) {
		if (!xdim || !ydim || !zdim) return;

		if (voxel_matrix_data.size() > 0) { voxel_matrix_data.clear(); voxel_matrix_data.shrink_to_fit(); }

		index_data_type voxel_matrix_data_size = calculate_voxel_matrix_data_size(xdim, ydim, zdim);

		voxel_matrix_data = std::vector<voxel_data_type>(voxel_matrix_data_size, volume_data_storage);

//QMessageBox::information(NULL, "voxelBS03", "create_solid_volume_cubic :"+QString::number(xdim * corrected_ydim * zdim *sizeof(voxel_data_type)), QMessageBox::Ok);
	}

};

