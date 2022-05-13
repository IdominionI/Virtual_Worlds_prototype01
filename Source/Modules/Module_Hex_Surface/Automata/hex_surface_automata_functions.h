#pragma once
#include <vector>

#include <Source/Modules/Module_Voxel_Byte/Editor/Scene/voxel_hcp_scene_objects.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../Hex_surface_object/DataTypes/dt_hex_surface_automata.h"
#include "../Hex_surface_object/hex_surface_data_storage.h"
#include "../Hex_surface_object/hex_surface_object.h"
/*
	hex_surface_automata_functions_class

	C++ class that generates the data values of a hex surface data object based on the
	current state of the hex surface data object data values and the cellula Automata
	rules defined.

	NOTE : This is based on the hcp voxel cellula automata code as a hex suface can be considered as
	       a zero even layer hcp voxel with no neighbor voxels above or below, and only on the same
		   even zero layer or plane.
*/

class hex_surface_automata_functions_class {
public:
	object_id_type     object_id;

	log_panel_class* log_panel = NULL;

	std::vector <hex_surface_automata_rule_struct_type>   hex_surface_automata_rules;
	hex_surface_automata_generator_parameters_struct_type hex_surface_automata_generator_parameters;

	hex_surface_object_class *cloud = NULL;                          // Pointer to the hex surface data object that is to have cellula Automata rules applied to
	hex_surface_object_class *hex_surface_automata_next_step_matrix; // Pointer to a copy of the hex surface data object that is to have result of cellula Automata rules applied save to
	u_long_int                max_number_steps;

	hex_surface_object_class *hex_surface_object_to_execute = NULL;	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	u_long_int get_max_hex_surface_automata_step_value() {
		u_long_int max_number_steps = 0;
		for (hex_surface_automata_rule_struct_type hcp_automata_rule : hex_surface_automata_rules) {
			if(hcp_automata_rule.end_step > max_number_steps) max_number_steps = hcp_automata_rule.end_step;
		}

		return max_number_steps;
	}

	// Create an empty hex suface data object matrix in which to store the resultant cellula autoamata values to
	bool create_hex_surface_automata_next_step_matrix() {
		hex_surface_automata_next_step_matrix = new hex_surface_object_class;

		if (hex_surface_automata_next_step_matrix == NULL) return false;

		hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_size       = cloud->hex_surface_object_data.hex_size;

		hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_dimension = cloud->hex_surface_object_data.grid_dimension;
		hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_origin    = cloud->hex_surface_object_data.grid_origin;
		hex_surface_automata_next_step_matrix->hex_surface_object_data.create_empty_surface_cubic(cloud->hex_surface_object_data.grid_dimension.x, cloud->hex_surface_object_data.grid_dimension.y);

//QMessageBox::information(0, "Function Expression Success", "next_step_matrix 00: "+std::string::number(hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_dimension.x)+":"+std::string::number(hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_dimension.y)+":"+std::string::number(hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_dimension.z)+":", QMessageBox::Ok);


		if (hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_surface_matrix_data.size() > 0)
			return true;
		else {
			delete hex_surface_automata_next_step_matrix;
			return false;
		}
	}

	bool perform_hex_surface_automata_generation(u_long_int automata_step) {
		if(cloud == NULL){
			printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation::cloud == NULL %i\n", automata_step);
			return false;
		}

		if (!create_hex_surface_automata_next_step_matrix()) { 
			printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation:: ERROR: Failure to create automata step matrix \n");
			return false; 
		}

		if (!perform_hex_surface_automata_step_generation(automata_step)) {
			//QMessageBox::information(0, "Function Automata", "ERROR: Failure to create perform automata data at step "+std::string::number(automata_step), QMessageBox::Ok);
			printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation:: ERROR: Failure to create perform automata data at step %i \n", automata_step);
			return false;
		}

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation::00 \n");
		// FOLOWING FUNCTIONS WAS PREVIOUSLY PERFORMED EXTERNALLY AFTER perform_hex_surface_automata_generation WAS CALLED
		if(hex_surface_object_to_execute == NULL){
			printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation:: ERROR: Undefined hex_surface_object_to_execute \n");
			return false;
		}

		hex_surface_object_to_execute->hex_surface_object_data.delete_hex_surface_matrix_data();
//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation::00AA \n");
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data = hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_surface_matrix_data;

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation::01 \n");

		hex_surface_object_to_execute->define_vbo_vertices(MIN_HEX_VALUE, MAX_HEX_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display

//printf("hex_surface_animation_functions_class :: perform_hex_surface_automata_generation::02 \n");

		return true;
	}

	// generate and save cellula automata data for cellula automata generation step 
	// This procedure requires an initial voxel matrix that has initialised conditions set.
	bool perform_hex_surface_automata_step_generation(int step) {
		if (cloud == NULL) {
//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "cloud == NULL", QMessageBox::Ok);
			return false;
		}

//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "here00", QMessageBox::Ok);
		
		hex_surface_automata_generator_parameters.x_start = cloud->hex_surface_object_data.hex_surface_generator_parameters.x_start;
		hex_surface_automata_generator_parameters.x_end   = cloud->hex_surface_object_data.hex_surface_generator_parameters.x_end;
		hex_surface_automata_generator_parameters.y_start = cloud->hex_surface_object_data.hex_surface_generator_parameters.y_start;
		hex_surface_automata_generator_parameters.y_end   = cloud->hex_surface_object_data.hex_surface_generator_parameters.y_end;

		hex_surface_automata_generator_parameters.resolution_step = cloud->hex_surface_object_data.hex_surface_generator_parameters.resolution_step;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_hex_surface_automata_step_generation 00 : " + std::string::number(hex_surface_automata_generator_parameters.x_start) + " : "+ std::string::number(hex_surface_automata_generator_parameters.resolution_step));
//}

		//define the hex suface iteration parameters that define the hex surface matrix that apply to the cellula automata rules
		// Same as for the hex suface generation and thus use hex suface generation parameter data
		hex_surface_index_data_type iX = 0,iY,iZ;
		float fX=0.0f,fY=0.0f,fZ=0.0f,fT=0.0f;

	  	float x_res_step = hex_surface_automata_generator_parameters.resolution_step*2.0;
		float y_res_step = hex_surface_automata_generator_parameters.resolution_step*(3.0/sqrt(3.0));
		
		float x_size = hex_surface_automata_generator_parameters.x_end - hex_surface_automata_generator_parameters.x_start;
		float y_size = hex_surface_automata_generator_parameters.y_end - hex_surface_automata_generator_parameters.y_start;

		int data_set_x_size, data_set_y_size;

		if(x_size/x_res_step-float((int)(x_size / x_res_step)) > 0.0 )
			data_set_x_size = (int)(x_size / x_res_step)+1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if(y_size/y_res_step-float((int)(y_size / y_res_step)) > 0.0 )
			data_set_y_size = (int)(y_size / y_res_step)+1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		// Allow for not all x,y,z to be used
		if (hex_surface_automata_generator_parameters.x_start == hex_surface_automata_generator_parameters.x_end) 
			hex_surface_automata_generator_parameters.x_end = hex_surface_automata_generator_parameters.x_start + hex_surface_automata_generator_parameters.resolution_step/2.0f;

		if (hex_surface_automata_generator_parameters.y_start == hex_surface_automata_generator_parameters.y_end) 
			hex_surface_automata_generator_parameters.y_end = hex_surface_automata_generator_parameters.y_start + hex_surface_automata_generator_parameters.resolution_step/2.0f;

		hex_surface_index_data_type  dim_x, dim_y;

		iZ = 0;

			iY = 0;
			dim_y = ((iZ+1)%2)*data_set_y_size + (iZ%2)*(data_set_y_size-1);
			for (fY = hex_surface_automata_generator_parameters.y_start; fY < hex_surface_automata_generator_parameters.y_end && iY < dim_y; fY = fY + y_res_step) {
				iX = 0;
				if (iZ % 2 == 0) { // Even z level
					dim_x = (iY+1)%2*data_set_x_size + (iY%2)*(data_set_x_size-1);
				} else { // Odd z level
					dim_x = (iY+1)%2*(data_set_x_size-1) + (iY%2)*data_set_x_size;
				}
				iX = 0 ;
//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "here00A :"+std::string::number(iX)+":"+std::string::number(iY)+":"+std::string::number(iZ)+":", QMessageBox::Ok);
//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "here00AAA"+std::string::number(fX)+":"+std::string::number(fY)+":"+std::string::number(hex_surface_automata_generator_parameters.z_start)+":", QMessageBox::Ok);
				for (fX = hex_surface_automata_generator_parameters.x_start; fX < hex_surface_automata_generator_parameters.x_end && iX < dim_x; fX = fX + x_res_step) {
					perform_hex_surface_automata_rules(step,iX,iY,iZ);
					iX++;
				}
				iY++;
			}
//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "here00B", QMessageBox::Ok);

		return true;
	}


	// Perform the cellula automata rules for step at hex surface matrix location x_index, y_index, z_index = 0
	// and if all rules are met assign a defined value for this case, otherwise set a rules not met value
	bool perform_hex_surface_automata_rules(u_long_int step,hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_index_data_type z_index ) {
		if(cloud == NULL) return false;
		hex_index_vector hex_surface_data_index_coord = { x_index, y_index };// z_index };

		hex_surface_data_type rule_met_status = INVALID_HEX_VALUE;
		bool automata_rule_met = false;

		// The first automata activation rule in the order of rules given that is valid or true is the rule that is applied.
		// A future implementation considered is to put all rules that are met into an array and through a selection process the rule that is applied is chosen
		// via an option of first rule or by a randome process that can include biased outcomes
//QMessageBox::information(0, "perform_hex_surface_automata_rules", "here01", QMessageBox::Ok);
		for (hex_surface_automata_rule_struct_type hex_surface_automata_rule : hex_surface_automata_rules) {
			if (hex_surface_automata_rule.active_rule && !automata_rule_met) {
				if (hex_surface_automata_rule_met(hex_surface_automata_rule, step, x_index, y_index, z_index)) {
					// this method of determining the value of the hex surface status when more than one rule is met can be chnaged
					rule_met_status   = hex_surface_automata_rule.hex_state;
					automata_rule_met = true; // Set flag to indicate that an automata rule has been met
					break;
				}
			}
		}
//QMessageBox::information(0, "perform_hex_surface_automata_rules", "here01A: "+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index)+":", QMessageBox::Ok);

		// This method of determining the value of the hex surface status when more than one rule is met can be chnaged
		// If a rule is met then a chnage of the hex surface status value is made to the accumulation of all the rule values that has been met
		// otherwise leave the hex surface status value unchanged
		if (automata_rule_met) {
			if (rule_met_status > MAX_HEX_VALUE)
				rule_met_status = MAX_HEX_VALUE;
			else
				if (rule_met_status < INVALID_HEX_VALUE)
					rule_met_status = INVALID_HEX_VALUE;

 //stream << "perform_hex_surface_automata_rules 00 : rule met : " << std::string::number(rule_met_status)<< ":" + std::string::number(x_index) + ":" << std::string::number(y_index) << ":" + std::string::number(z_index) << '\n';// testing only
			hex_surface_data_type value = hex_surface_automata_next_step_matrix->hex_surface_object_data.activate_hex_surface_matrix_coordinate(hex_surface_data_index_coord, rule_met_status); // majority rule to activate hex surface state
// stream << "perform_hex_surface_automata_rules 00 : rule met A: " << std::string::number(value) << '\n';// testing only
		} else{
 //stream <<"perform_hex_surface_automata_rules 01 : rule not met : "  <<  std::string::number(x_index) << ":" << std::string::number(y_index) << ":" + std::string::number(z_index) << '\n';// testing only

			assign_no_hex_state_change(hex_surface_data_index_coord); // no majority rule or rule to apply so have no change in hex surface state 
		}
//QMessageBox::information(0, "perform_hex_surface_automata_rules", "here01B", QMessageBox::Ok);
		return true;
	}

	// Return if a cellular automata rule is met for the hex suface matrix at location x_index, y_index, z_index = 0
	// A cellular automata rule is met if all nieghbor hex surface cells rules apply
	bool hex_surface_automata_rule_met(hex_surface_automata_rule_struct_type hex_surface_automata_byte_rule, u_long_int step, hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_index_data_type z_index) {
		if (step >= hex_surface_automata_byte_rule.end_step || step < hex_surface_automata_byte_rule.start_step) return false;
//QMessageBox::information(0, "hex_surface_autoamta_rule_met", "hex_surface_automata_rule_met 000 "+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index), QMessageBox::Ok);
		std::vector<hex_surface_data_type> hex_surface_neighbours_state = get_hex_surface_neighbours_state(x_index, y_index, z_index);
//QMessageBox::information(0, "hex_surface_autoamta_rule_met", "hex_surface_automata_rule_met "+std::string::number(hex_surface_neighbours_state), QMessageBox::Ok);

		short neighbours_activity_rule = 0;

		for (size_t i = 0; i < NUMBER_HEX_NEIGHBOURS; i++) {
			bool neighbours_lower_activity_rule = false ,neighbours_upper_activity_rule = false;

			switch (hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule) {
				case hex_lower_rule_condition_enum_type::ignore : neighbours_lower_activity_rule = true;	break;
				case hex_lower_rule_condition_enum_type::LT     : if( hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value < hex_surface_neighbours_state[i]  )
																	neighbours_lower_activity_rule = true; break;
				case hex_lower_rule_condition_enum_type::LTEQ   : if( hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value <= hex_surface_neighbours_state[i]  )
																	neighbours_lower_activity_rule = true; break;;
			}

			switch (hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule) {
				case hex_upper_rule_condition_enum_type::ignore : neighbours_upper_activity_rule = true;	break;
				case hex_upper_rule_condition_enum_type::NE     : if( hex_surface_neighbours_state[i] != hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value   )
																	neighbours_upper_activity_rule = true; break;
				case hex_upper_rule_condition_enum_type::EQ     : if(  hex_surface_neighbours_state[i]  == hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value )
																	neighbours_upper_activity_rule = true; break;;
				case hex_upper_rule_condition_enum_type::LT     : if( hex_surface_neighbours_state[i] < hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value   )
																	neighbours_upper_activity_rule = true; break;
				case hex_upper_rule_condition_enum_type::LTEQ   : if(  hex_surface_neighbours_state[i]  <= hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value )
																	neighbours_upper_activity_rule = true; break;;
			}

			if(!neighbours_lower_activity_rule || !neighbours_upper_activity_rule) return false;
		}

		return true;
	}

	// Get all the hex suface neighbor cell values of hex cell at matrix location x_index, y_index, z_index = 0  stored in a std::vector list
	std::vector<hex_surface_data_type> get_hex_surface_neighbours_state(hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_index_data_type z_index) {
		std::vector<hex_surface_data_type> neighbours_activity;
//QMessageBox::information(0, "get_hex_surface_neighbours_state", "here03", QMessageBox::Ok);
		for (size_t neighbour = 0; neighbour < NUMBER_HEX_NEIGHBOURS; neighbour++) {

			hex_surface_data_type hex_state = get_hex_surface_neighbour_state(neighbour, x_index, y_index, z_index);

			neighbours_activity.push_back(hex_state);
//if( z_index == INVETSIGATE_Z)// testing only
//	stream << "| hex surface activcation status 1"<< '\n';// testing only

		}

		//stream << "| neighbours_activity :"<< neighbours_activity << '\n'; // testing only
		return neighbours_activity;
	}

	// Get the neighbor cell status value of a hex cell that is at hex suface matrix location x_index, y_index, z_index = 0
	hex_surface_data_type get_hex_surface_neighbour_state(size_t neighbour,hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_index_data_type z_index) {
		hex_index_vector hex_surface_neighbour_index_coord = get_hex_surface_neighbour_index_coordinates(neighbour,x_index,y_index,z_index);

//stream << "| neighbour : " << neighbour << " inex x: " << hex_surface_neighbour_index_coord.x <<" y: " << hex_surface_neighbour_index_coord.y << " z: " << hex_surface_neighbour_index_coord.z ;// testing only

		if (!valid_index_coordinate(hex_surface_neighbour_index_coord)) {

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_hex_surface_neighbour_state", "get_hex_surface_neighbour_state00 invalid  Cloud : " + std::string::number(neighbour) + ":" + std::string::number(x_index) + ":" + std::string::number(y_index) + ":" + std::string::number(z_index) +
//							" \nneighbour :" + std::string::number(hex_surface_neighbour_index_coord.x) + ":" + std::string::number(hex_surface_neighbour_index_coord.y) + ":" + std::string::number(hex_surface_neighbour_index_coord.z) +
//							"\n activation status:" + std::string::number(INVALID_HEX_VALUE), QMessageBox::Ok);

			return INVALID_HEX_VALUE; // *** Need to change this for a non valid value
		}		
		
//if( z_index == INVETSIGATE_Z)// testing only
//stream << " | Cloud : " << " inex x: " << hex_surface_data_index_coord.x <<" y: " << hex_surface_data_index_coord.y << " z: " << hex_surface_data_index_coord.z ;// testing only

//if( z_index == INVETSIGATE_Z)// testing only
//	stream << " | Index : " << cloud->hex_surface_object_data.get_hex_surface_matrix_data_index(hex_surface_data_index_coord); ;// testing only

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_hex_surface_neighbour_state", "get_hex_surface_neighbour_state01 valid Cloud : " + std::string::number(neighbour) + ":" + std::string::number(x_index) + ":" + std::string::number(y_index) + ":" + std::string::number(z_index) + 
//							" \nneighbour :" + std::string::number(hex_surface_neighbour_index_coord.x) + ":" + std::string::number(hex_surface_neighbour_index_coord.y) + ":" + std::string::number(hex_surface_neighbour_index_coord.z) + 
//							"\n activation status:" + std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_neighbour_index_coord)), QMessageBox::Ok);

		return cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_neighbour_index_coord);
	}

	hex_index_vector get_hex_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index, int z_index) {
		hex_index_vector neighbor_index_coordinate;
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here05 "+std::string::number( neighbour)+":"+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index), QMessageBox::Ok);
		//if (z_index % 2 == 0) {  // Even Z 
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here05A", QMessageBox::Ok);
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
					case 0: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
					case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;      break;
					case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1;  break;
					case 3: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1;  break;
					case 4: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;      break;
					case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1;  break;
				}
			}else{// Odd Y 
				switch (neighbour) {
					case 0: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
					case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
					case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
					case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
					case 4: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
					case 5: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
				}
			}
		return neighbor_index_coordinate;
	}

	// Determine if hex suface cell index coordinate hex_surface_index_coordinate is valid or not
	// Ie if it is outside the plane and limits of the hex suface matrix 
	bool valid_index_coordinate(hex_index_vector hex_surface_index_coordinate) {
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here06", QMessageBox::Ok);

			if(hex_surface_index_coordinate.y > cloud->hex_surface_object_data.grid_dimension.y-1) return false;

			if (hex_surface_index_coordinate.y % 2 == 0) { // even y coord
				if (hex_surface_index_coordinate.x > cloud->hex_surface_object_data.grid_dimension.x-1) return false;
			}else{
				if (hex_surface_index_coordinate.x > cloud->hex_surface_object_data.grid_dimension.x-2) return false;
			}

//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here06A", QMessageBox::Ok);
		return true;
	}

	void assign_no_hex_state_change(hex_index_vector hex_surface_data_index_coord) {
		hex_surface_data_type hex_surface_activation_status = cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_data_index_coord);

		hex_surface_automata_next_step_matrix->hex_surface_object_data.activate_hex_surface_matrix_coordinate(hex_surface_data_index_coord, hex_surface_activation_status);
	}

	// Copy into the cellular automata data structure variable hex_surface_automata_initial_data the cellula automata parameter values
	// that are stored in the virtual worlds scene data model that hex surface hex_surface_automata_initial_data points to
	bool load_hex_surface_automata_initial_data(hex_surface_object_class *hex_surface_automata_initial_data) {
		if(hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_data.size() == 0) return false;
		if(hex_surface_automata_initial_data->hex_surface_object_data.hex_size != hex_surface_automata_generator_parameters.resolution_step)
			hex_surface_automata_generator_parameters.resolution_step = hex_surface_automata_initial_data->hex_surface_object_data.hex_size;

//QMessageBox::information(NULL, "","load_hex_surface_automata_initial_data 001", QMessageBox::Ok);
		if(cloud == NULL){
			cloud = new hex_surface_object_class;
			if (cloud == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Memory Allocation Error:\nCould not allocate memory to import initial hex surface automata data.\n");
				return false;
			}
		}

		cloud->hex_surface_object_data.hex_surface_matrix_data.clear();
		cloud->hex_surface_object_data.hex_surface_matrix_data.shrink_to_fit();
		if(!create_hex_surface_matrix()) return false;
//QMessageBox::information(NULL, "","load_hex_surface_automata_initial_data 002", QMessageBox::Ok);
		hex_surface_index_data_type iX = 0,iY,iZ;

//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002 : " + std::string::number(origin_offset.x())+":"+std::string::number(coordinate_offset.x())+":"+std::string::number(coordinate_offset_xi), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002AA : " + std::string::number(cloud->hex_surface_object_data.grid_dimension.x)+":"+std::string::number(cloud->hex_surface_object_data.grid_dimension.y)+":"+std::string::number(cloud->hex_surface_object_data.grid_dimension.z), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002AAB : " + std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_data.size()), QMessageBox::Ok);

		hex_surface_index_data_type data_set_x_size,data_set_y_size,data_set_z_size;
		hex_surface_index_data_type  dim_x, dim_y;
		bool even_z_level;

		data_set_x_size = cloud->hex_surface_object_data.grid_dimension.x;
		data_set_y_size = cloud->hex_surface_object_data.grid_dimension.y;
		data_set_z_size = cloud->hex_surface_object_data.grid_dimension.z;

		iZ = 0;
		for (iZ = 0;  iZ < data_set_z_size; iZ++) {
			if (iZ % 2 == 0)
				even_z_level = true;
			else 
				even_z_level = false;

			dim_y = ((iZ+1)%2)*data_set_y_size + (iZ%2)*(data_set_y_size-1);

			for (iY = 0;  iY < dim_y; iY++) {
				if (even_z_level) {
					dim_x = (iY+1)%2*data_set_x_size + (iY%2)*(data_set_x_size-1);
				} else {
					dim_x = (iY+1)%2*(data_set_x_size-1) + (iY%2)*data_set_x_size;
				}
				for (iX = 0;  iX < dim_x; iX++) {
					if (hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status({ iX, iY }) > hex_surface_data_type(0)) {  // *** Need to change this for a valid activation value
					//if (hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status({ iX, iY, iZ }) > hex_surface_data_type(0)) {  // *** Need to change this for a valid activation value
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002B : " + std::string::number(initial_iX)+":"+std::string::number(initial_iY)+":"+std::string::number(initial_iZ), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002C : " + std::string::number(automata_data_index_coord.matrix_index.x)+":"+std::string::number(automata_data_index_coord.matrix_index.y)+":"+ std::string::number(automata_data_index_coord.matrix_index.z)+":"+std::string::number(automata_data_index_coord.j_bit_index), QMessageBox::Ok);
						cloud->hex_surface_object_data.activate_hex_surface_matrix_coordinate({ iX, iY },1);
						//cloud->hex_surface_object_data.activate_hex_surface_matrix_coordinate({ iX, iY, iZ },1);
					}
				}
			}
		}
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 003", QMessageBox::Ok);
		return true;

	}

	// Create an empty hex surface matrix
	bool create_hex_surface_matrix() {

		// ########### CREATE EMPTY HEX CLOUD MATRIX #################
		float x_size = hex_surface_automata_generator_parameters.x_end - hex_surface_automata_generator_parameters.x_start;
		float y_size = hex_surface_automata_generator_parameters.y_end - hex_surface_automata_generator_parameters.y_start;

		float z_mult = 2.0*sqrt(6.0)/3.0;

		float x_res_step = hex_surface_automata_generator_parameters.resolution_step*2.0;
		float y_res_step = hex_surface_automata_generator_parameters.resolution_step*(3.0/sqrt(3.0));

		int data_set_x_size,data_set_y_size,data_set_z_size;

		if(x_size/x_res_step-float((int)(x_size / x_res_step)) > 0.0 )
			data_set_x_size = (int)(x_size / x_res_step)+1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if(y_size/y_res_step-float((int)(y_size / y_res_step)) > 0.0 )
			data_set_y_size = (int)(y_size / y_res_step)+1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

//QMessageBox::information(0, "Automata", "Automata create_hex_surface_matrix 00: "+std::string::number(data_set_x_size)+":"+std::string::number(data_set_y_size)+":"+std::string::number(data_set_z_size)+":", QMessageBox::Ok);

		glm::vec3 origin = { hex_surface_automata_generator_parameters.x_start,hex_surface_automata_generator_parameters.y_start,0.0f };

		cloud->hex_surface_object_data.hex_size = hex_surface_automata_generator_parameters.resolution_step;

		cloud->hex_surface_object_data.grid_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
		cloud->hex_surface_object_data.grid_origin = { origin.x,origin.y};
		cloud->hex_surface_object_data.create_empty_surface_cubic(data_set_x_size, data_set_y_size);// data_set_z_size);
//QMessageBox::information(0, "Automata", "Automata create_hex_surface_matrix 01: "+std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_data.size())+":", QMessageBox::Ok);

		if(cloud->hex_surface_object_data.hex_surface_matrix_data.size() > 0) 
			return true;
		else
			return false;
	}
};

/*
	hex_surface_automata_hcp_thread_functions_class

	C++ class that generates as a multithreaded exucution, the data values of a hex suface data
	object  based on the current state of the hex surface data object data	values and the 
	cellula Automata rules defined.

*/
/*
class hex_surface_automata_hcp_thread_functions_class {
public:
	object_id_type     object_id;

	log_panel_class* log_panel = NULL;

	std::vector <hex_surface_automata_rule_struct_type>   hex_surface_automata_rules;
	hex_surface_automata_generator_parameters_struct_type hex_surface_automata_generator_parameters;

	hex_surface_object_class* cloud = NULL;                          // Pointer to the hex surface data object that is to have cellula Automata rules applied to
	hex_surface_object_class* hex_surface_automata_next_step_matrix; // Pointer to a copy of the hex surface data object that is to have result of cellula Automata rules applied save to
	u_long_int                max_number_steps;

	hex_surface_object_class* hex_surface_object_to_execute = NULL;	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	u_long_int get_max_hex_surface_automata_step_value() {
		u_long_int max_number_steps = 0;
		for (hex_surface_automata_rule_struct_type hcp_automata_rule: hex_surface_automata_rules) {
			if(hcp_automata_rule.end_step > max_number_steps) max_number_steps = hcp_automata_rule.end_step;
		}

		return max_number_steps;
	}

	// Create an empty hex suface data object matrix in which to store the resultant cellula autoamata values to
	bool create_hex_surface_automata_next_step_matrix() {
		hex_surface_automata_next_step_matrix = new hex_surface_object_class;

		if (hex_surface_automata_next_step_matrix == NULL) return false;

		hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_size       = cloud->hex_surface_object_data.hex_size;
		//hex_surface_automata_next_step_matrix->world_location                     = cloud->world_location;

		hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_dimension = cloud->hex_surface_object_data.grid_dimension;
		hex_surface_automata_next_step_matrix->hex_surface_object_data.grid_origin    = cloud->hex_surface_object_data.grid_origin;
		hex_surface_automata_next_step_matrix->hex_surface_object_data.create_empty_surface_cubic(cloud->hex_surface_object_data.grid_dimension.x, cloud->hex_surface_object_data.grid_dimension.y);
		//hex_surface_automata_next_step_matrix->hex_surface_object_data.create_empty(cloud->hex_surface_object_data.grid_dimension.x, cloud->hex_surface_object_data.grid_dimension.y, cloud->hex_surface_object_data.grid_dimension.z);


		if (hex_surface_automata_next_step_matrix->hex_surface_object_data.hex_surface_matrix_data.size() > 0)
			return true;
		else {
			delete hex_surface_automata_next_step_matrix;
			return false;
		}

	}

	// generate and save cellula automata data for cellula automata generation step 
	// This procedure requires an initial voxel matrix that has initialised conditions set.
	bool perform_hex_surface_automata_generation(u_long_int automata_step) {

		if(cloud == NULL){
			QMessageBox::information(0, "Function Automata", "ERROR: Have not defined an initial hex surface matrix", QMessageBox::Ok);
			return false;
		}

		if (!create_hex_surface_automata_next_step_matrix()) { 
			QMessageBox::information(0, "Function Automata", "ERROR: Failure to create automata step matrix", QMessageBox::Ok);
			return false; 
		}

		if (!perform_hex_surface_automata_step_generation_thread(automata_step)) {
			QMessageBox::information(0, "Function Automata", "ERROR: Failure to create perform automata data at step "+std::string::number(automata_step), QMessageBox::Ok);
			return false;
		}

		return true;
	}

	// Define the variables and parameters to perform a multithreaded execution of cellula automata data
#define TILESIZE 16

	typedef struct
	{
		long    tileID;
		HANDLE  barrier;
		hex_surface_object_class *cloud;
		hex_surface_object_class *hex_surface_automata_next_step_matrix;
		hex_index_vector  resolution;
		int         step;

		std::vector <hex_surface_automata_rule_struct_type>   hex_surface_automata_rules;

	}TData;

	// generate and save cellula automata data for cellula automata generation step as a multithreaded function
	// This procedure requires an initial voxel matrix that has initialised conditions set.
	// NOTE : All functions called by multithreaded functions must be static functions or application will not compile
	bool perform_hex_surface_automata_step_generation_thread(int step) {
		if (cloud == NULL) {
//QMessageBox::information(0, "perform_hex_surface_automata_step_generation", "cloud == NULL", QMessageBox::Ok);
			return false;
		}

		hex_surface_automata_generator_parameters.x_start = cloud->hex_surface_object_data.hex_surface_generator_parameters.x_start;
		hex_surface_automata_generator_parameters.x_end   = cloud->hex_surface_object_data.hex_surface_generator_parameters.x_end;
		hex_surface_automata_generator_parameters.y_start = cloud->hex_surface_object_data.hex_surface_generator_parameters.y_start;
		hex_surface_automata_generator_parameters.y_end   = cloud->hex_surface_object_data.hex_surface_generator_parameters.y_end;
		//hex_surface_automata_generator_parameters.z_start = cloud->hex_surface_object_data.hex_surface_generator_parameters.z_start;
		//hex_surface_automata_generator_parameters.z_end   = cloud->hex_surface_object_data.hex_surface_generator_parameters.z_end;
		hex_surface_automata_generator_parameters.resolution_step = cloud->hex_surface_object_data.hex_surface_generator_parameters.resolution_step;

		hex_index_vector resolution;

		hex_surface_index_data_type iX = 0, iY, iZ;
		float fX = 0.0f, fY = 0.0f, fZ = 0.0f, fT = 0.0f;

		float z_mult = 2.0*sqrt(6.0)/3.0;

		float x_res_step = hex_surface_automata_generator_parameters.resolution_step*2.0;
		float y_res_step = hex_surface_automata_generator_parameters.resolution_step*(3.0/sqrt(3.0));
		float z_res_step = hex_surface_automata_generator_parameters.resolution_step*z_mult;

		float x_size = hex_surface_automata_generator_parameters.x_end - hex_surface_automata_generator_parameters.x_start;
		float y_size = hex_surface_automata_generator_parameters.y_end - hex_surface_automata_generator_parameters.y_start;
		//float z_size = hex_surface_automata_generator_parameters.z_end - hex_surface_automata_generator_parameters.z_start;

		int data_set_x_size,data_set_y_size,data_set_z_size;

		if(x_size/x_res_step-float((int)(x_size / x_res_step)) > 0.0 )
			data_set_x_size = (int)(x_size / x_res_step)+1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if(y_size/y_res_step-float((int)(y_size / y_res_step)) > 0.0 )
			data_set_y_size = (int)(y_size / y_res_step)+1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		//if(z_size/z_res_step-float((int)(z_size / z_res_step)) > 0.0 )
		//	data_set_z_size = (int)(z_size / z_res_step)+1;
		//else
		//	data_set_z_size = (int)(z_size / z_res_step);

		// Allow for not all x,y,z to be used
		if (hex_surface_automata_generator_parameters.x_start == hex_surface_automata_generator_parameters.x_end)
			hex_surface_automata_generator_parameters.x_end = hex_surface_automata_generator_parameters.x_start + hex_surface_automata_generator_parameters.resolution_step/2.0f;

		if (hex_surface_automata_generator_parameters.y_start == hex_surface_automata_generator_parameters.y_end)
			hex_surface_automata_generator_parameters.y_end = hex_surface_automata_generator_parameters.y_start + hex_surface_automata_generator_parameters.resolution_step/2.0f;

		//if (hex_surface_automata_generator_parameters.z_start == hex_surface_automata_generator_parameters.z_end)
		//	hex_surface_automata_generator_parameters.z_end = hex_surface_automata_generator_parameters.z_start + hex_surface_automata_generator_parameters.resolution_step/2.0f;


		resolution.x = data_set_x_size;
		resolution.y = data_set_y_size;
		//resolution.z = data_set_z_size;

//QMessageBox::information(0, "thread001", "Thread001 : "+std::string::number(resolution.x)+":"+std::string::number(resolution.y)+":"+std::string::number(resolution.z)+":"+std::string::number(step), QMessageBox::Ok);

		// get number of CPUs available
		SYSTEM_INFO si;
		GetSystemInfo( &si );
		int numCpus;
		// min and std::min just not recognised
		if (si.dwNumberOfProcessors < 256)
			numCpus = si.dwNumberOfProcessors;
		else
			numCpus = 256;

		// data for worker threads
		__declspec(align(32)) TData data = {
			0,                                         // tile id/counter
			CreateSemaphore( 0, numCpus, numCpus, 0 ), // create barrier
			cloud, // image
			hex_surface_automata_next_step_matrix,
			resolution,
			step,
			hex_surface_automata_rules
		};

		// launch worker threads
		HANDLE th[256];
		for( int i=0; i<numCpus; i++ )
			th[i] = CreateThread( 0, 0, doRender, &data, 0, 0 );

		// wait for worker threads to finish
		WaitForMultipleObjects( numCpus, th, true, INFINITE);

		// destroy threads
		for( int i=0; i<numCpus; i++ )
			CloseHandle( th[i] );

		// destroy barrier
		CloseHandle( data.barrier );

		return true;
	}

	static unsigned long __stdcall doRender( void *vdata ){
		// prep data
		TData *data = (TData*)vdata;
		const hex_index_vector resolution = data->resolution;
		const int   numxtiles  = resolution.x/TILESIZE+1;
		const int   numytiles  = resolution.y/TILESIZE+1;
		//const int   numztiles  = resolution.z/TILESIZE+1;
		//const int   numtiles   = numxtiles*numytiles*numztiles;
		const int   numtiles   = numxtiles*numytiles*numytiles;

		// Multithreading crashes if there is only one thread tile to work on
		// So need to perform task as if there is no threading implemented
		if (numtiles == 1) {
			//for (int k = 0; k< resolution.z; k++) {
				for (int j = 0; j <  resolution.y; j++) {
					for (int i = 0; i < resolution.x; i++) {
						//perform_hex_surface_automata_rules(data->step, data->hex_surface_automata_rules, data->cloud, data->hex_surface_automata_next_step_matrix, i,  j, k);
						perform_hex_surface_automata_rules(data->step, data->hex_surface_automata_rules, data->cloud, data->hex_surface_automata_next_step_matrix, i,  j);
					}
				}
			//}

			ReleaseSemaphore( data->barrier, 1, NULL );
			return 1;
		}


		// synch point
		WaitForSingleObject( data->barrier, 0 );

		// render tiles
		for(;;)
		{
			// get next tile to consume
			const int tile = InterlockedIncrement( &data->tileID )-1;
			if( tile >= numtiles ) break;

			// tile offset
			const int ia = TILESIZE*(tile % numxtiles);
			const int ja = TILESIZE*((tile - numxtiles*numytiles*(tile / (numxtiles*numytiles)))/numxtiles);
			const int ka = TILESIZE*(tile / (numxtiles*numytiles));

			// for every pixel in this tile, compute color
			//for (int k = 0; k < TILESIZE; k++) {
				//if (ka + k< resolution.z) {
					for (int j = 0; j < TILESIZE; j++) {
						if (ja + j < resolution.y) {
							for (int i = 0; i < TILESIZE; i++) {
								if (ia + i < resolution.x)
									//perform_hex_surface_automata_rules(data->step, data->hex_surface_automata_rules, data->cloud, data->hex_surface_automata_next_step_matrix, ia + i, ja + j, ka + k);
									perform_hex_surface_automata_rules(data->step, data->hex_surface_automata_rules, data->cloud, data->hex_surface_automata_next_step_matrix, ia + i, ja + j);
							}
						}
					}
				//}
			//}
		}

		// synch point
		ReleaseSemaphore( data->barrier, 1, NULL );
		return 1;
	}

	// Perform the cellula automata rules for step at hex surface matrix location x_index, y_index, z_index = 0
	// and if all rules are met assign a defined value for this case, otherwise set a rules not met value
	static bool perform_hex_surface_automata_rules(u_long_int step,std::vector <hex_surface_automata_rule_struct_type>   hex_surface_automata_rules,
											 hex_surface_object_class *cloud,hex_surface_object_class *hex_surface_automata_next_step_matrix,
											 int x_index, int y_index) {
											// int x_index, int y_index, int z_index) {
		if(cloud == NULL) return false;
		//hex_index_vector hex_surface_data_index_coord = { x_index, y_index, z_index };// *****
		hex_index_vector hex_surface_data_index_coord = { x_index, y_index};// *****


		hex_surface_data_type rule_met_status = 0;
		bool automata_rule_met = false;// ******

//QMessageBox::information(0, "thread001", "perform_hex surface001 : "+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index)+":", QMessageBox::Ok);

		// The first automata activation rule in the order of rules given that is valid or true is the rule that is applied.
		// A future implementation considered is to put all rules that are met into an array and through a selection process the rule that is applied is chosen
		// via an option of first rule or by a randome process that can include biased outcomes
//QMessageBox::information(0, "perform_hex_surface_automata_rules", "here01", QMessageBox::Ok);
		for (hex_surface_automata_rule_struct_type hex_surface_automata_rule : hex_surface_automata_rules) {
			if (hex_surface_automata_rule.active_rule) {
				//if (hex_surface_automata_rule_met(hex_surface_automata_rule, step, x_index, y_index, z_index,cloud)) { //*******
				if (hex_surface_automata_rule_met(hex_surface_automata_rule, step, x_index, y_index,cloud)) { //*******
					rule_met_status   = hex_surface_automata_rule.hex_state;
					automata_rule_met = true; // Set flag to indicate that an automata rule has been met
					break;
				}
			}
		}


		// This method of determining the value of the hex surface status when more than one rule is met can be chnaged
		// If a rule is met then a chnage of the hex surface status value is made to the accumulation of all the rule values that has been met
		// otherwise leave the hex surface status value unchanged
		if (automata_rule_met) {
			if (rule_met_status > MAX_HEX_VALUE)
				rule_met_status = MAX_HEX_VALUE;
			else
				if (rule_met_status < INVALID_HEX_VALUE)
					rule_met_status = INVALID_HEX_VALUE;

			hex_surface_automata_next_step_matrix->hex_surface_object_data.activate_hex_surface_matrix_coordinate(hex_surface_data_index_coord, rule_met_status); // majority rule to activate hex surface state
		}
		else
			assign_no_hex_state_change(hex_surface_data_index_coord,cloud,hex_surface_automata_next_step_matrix); // no majority rule or rule to apply so have no change in hex surface state 

//QMessageBox::information(0, "perform_hex_surface_automata_rules", "here01B", QMessageBox::Ok);
		return true;
	}

	// Return if a cellular automata rule is met for the hex suface matrix at location x_index, y_index, z_index = 0
	// A cellular automata rule is met if all nieghbor hex surface cells rules apply
	static bool hex_surface_automata_rule_met(hex_surface_automata_rule_struct_type hex_surface_automata_byte_rule, u_long_int step,
										hex_surface_index_data_type x_index, hex_surface_index_data_type y_index,
										//hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_index_data_type z_index,
										hex_surface_object_class *cloud) {
		if (step >= hex_surface_automata_byte_rule.end_step || step < hex_surface_automata_byte_rule.start_step) return false;
//QMessageBox::information(0, "hex_surface_autoamta_rule_met", "hex_surface_automata_rule_met 000 "+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index), QMessageBox::Ok);
		//std::vector<hex_surface_data_type> hex_surface_neighbours_state = get_hex_surface_neighbours_state(x_index, y_index, z_index,cloud);
		std::vector<hex_surface_data_type> hex_surface_neighbours_state = get_hex_surface_neighbours_state(x_index, y_index, cloud);
//QMessageBox::information(0, "hex_surface_autoamta_rule_met", "hex_surface_automata_rule_met "+std::string::number(hex_surface_neighbours_state), QMessageBox::Ok);

		short neighbours_activity_rule = 0;

		for (size_t i = 0; i < NUMBER_HEX_NEIGHBOURS; i++) {
			bool neighbours_lower_activity_rule = false ,neighbours_upper_activity_rule = false;

			switch (hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule) {
				case hex_lower_rule_condition_enum_type::ignore : neighbours_lower_activity_rule = true;	break;
				case hex_lower_rule_condition_enum_type::LT     : if( hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value < hex_surface_neighbours_state[i]  )
																	neighbours_lower_activity_rule = true; break;
				case hex_lower_rule_condition_enum_type::LTEQ   : if( hex_surface_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value <= hex_surface_neighbours_state[i]  )
																	neighbours_lower_activity_rule = true; break;;
			}

			switch (hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule) {
				case hex_upper_rule_condition_enum_type::ignore : neighbours_upper_activity_rule = true;	break;
				case hex_upper_rule_condition_enum_type::NE     : if( hex_surface_neighbours_state[i] != hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value   )
																	neighbours_upper_activity_rule = true; break;
				case hex_upper_rule_condition_enum_type::EQ     : if(  hex_surface_neighbours_state[i]  == hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value )
																	neighbours_upper_activity_rule = true; break;;
				case hex_upper_rule_condition_enum_type::LT     : if( hex_surface_neighbours_state[i] < hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value   )
																	neighbours_upper_activity_rule = true; break;
				case hex_upper_rule_condition_enum_type::LTEQ   : if(  hex_surface_neighbours_state[i]  <= hex_surface_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value )
																	neighbours_upper_activity_rule = true; break;;
			}

			if(!neighbours_lower_activity_rule || !neighbours_upper_activity_rule) return false;
		}

		return true;
	}

	// Get all the hex suface neighbor cell values of hex cell at matrix location x_index, y_index, z_index = 0  stored in a std::vector list
	static std::vector<hex_surface_data_type> get_hex_surface_neighbours_state(hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_object_class *cloud) {
		std::vector<hex_surface_data_type> neighbours_activity;
//QMessageBox::information(0, "get_hex_surface_neighbours_state", "here03", QMessageBox::Ok);
		for (size_t neighbour = 0; neighbour < NUMBER_HEX_NEIGHBOURS; neighbour++) {

			//hex_surface_data_type hex_state = get_hex_surface_neighbour_state(neighbour, x_index, y_index, z_index,cloud);
			hex_surface_data_type hex_state = get_hex_surface_neighbour_state(neighbour, x_index, y_index, cloud);

			neighbours_activity.push_back(hex_state);
//if( z_index == INVETSIGATE_Z)// testing only
//	stream << "| hex surface activcation status 1"<< '\n';// testing only

		}

		//stream << "| neighbours_activity :"<< neighbours_activity << '\n'; // testing only
		return neighbours_activity;
	}

	// Get the neighbor cell status value of a hex cell that is at hex suface matrix location x_index, y_index, z_index = 0
	static hex_surface_data_type get_hex_surface_neighbour_state(size_t neighbour, hex_surface_index_data_type x_index, hex_surface_index_data_type y_index, hex_surface_object_class *cloud) {
		hex_index_vector hex_surface_neighbour_index_coord = get_hex_surface_neighbour_index_coordinates(neighbour, x_index, y_index);
		//hex_index_vector hex_surface_neighbour_index_coord = get_hex_surface_neighbour_index_coordinates(neighbour, x_index, y_index, z_index);

//if( z_index == INVETSIGATE_Z)// testing only
//stream << "| neighbour : " << neighbour << " inex x: " << hex_surface_neighbour_index_coord.x <<" y: " << hex_surface_neighbour_index_coord.y << " z: " << hex_surface_neighbour_index_coord.z ;// testing only

		if (!valid_index_coordinate(hex_surface_neighbour_index_coord,cloud)) {

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_hex_surface_neighbour_state", "get_hex_surface_neighbour_state00 invalid  Cloud : " + std::string::number(neighbour) + ":" + std::string::number(x_index) + ":" + std::string::number(y_index) + ":" + std::string::number(z_index) +
//							" \nneighbour :" + std::string::number(hex_surface_neighbour_index_coord.x) + ":" + std::string::number(hex_surface_neighbour_index_coord.y) + ":" + std::string::number(hex_surface_neighbour_index_coord.z) +
//							"\n activation status:" + std::string::number(INVALID_HEX_VALUE), QMessageBox::Ok);

			return INVALID_HEX_VALUE; // *** Need to change this for a non valid value
		}

//if( z_index == INVETSIGATE_Z)// testing only
//stream << " | Cloud : " << " inex x: " << hex_surface_data_index_coord.x <<" y: " << hex_surface_data_index_coord.y << " z: " << hex_surface_data_index_coord.z ;// testing only

//if( z_index == INVETSIGATE_Z)// testing only
//	stream << " | Index : " << cloud->hex_surface_object_data.get_hex_surface_matrix_data_index(hex_surface_data_index_coord); ;// testing only

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_hex_surface_neighbour_state", "get_hex_surface_neighbour_state01 valid Cloud : " + std::string::number(neighbour) + ":" + std::string::number(x_index) + ":" + std::string::number(y_index) + ":" + std::string::number(z_index) + 
//							" \nneighbour :" + std::string::number(hex_surface_neighbour_index_coord.x) + ":" + std::string::number(hex_surface_neighbour_index_coord.y) + ":" + std::string::number(hex_surface_neighbour_index_coord.z) + 
//							"\n activation status:" + std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_neighbour_index_coord)), QMessageBox::Ok);

		return cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_neighbour_index_coord);
	}

	static 	hex_index_vector get_hex_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index) {
		hex_index_vector neighbor_index_coordinate;
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here05 "+std::string::number( neighbour)+":"+std::string::number(x_index)+":"+std::string::number(y_index)+":"+std::string::number(z_index), QMessageBox::Ok);
		//if (z_index % 2 == 0) {  // Even Z 
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here05A", QMessageBox::Ok);
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
				//case 0: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				//case 1: neighbor_index_coordinate.x = x_index; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				//case 2: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;


				case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
				case 5: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
				case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; break;

				//case 9:  neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				//case 10: neighbor_index_coordinate.x = x_index; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				//case 11: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
			else {// Odd Y 
				switch (neighbour) {
				//case 0: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				//case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				//case 2: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
				case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
				case 8: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index + 1;  break;

				//case 9:  neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				//case 10: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				//case 11: neighbor_index_coordinate.x = x_index;   neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}

		return neighbor_index_coordinate;
	}

	// Determine if hex suface cell index coordinate hex_surface_index_coordinate is valid or not
	// Ie if it is outside the plane and limits of the hex suface matrix 
	static bool valid_index_coordinate(hex_index_vector hex_surface_index_coordinate, hex_surface_object_class* cloud) {
//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here06", QMessageBox::Ok);

		//if (hex_surface_index_coordinate.z < 0 || hex_surface_index_coordinate.z > cloud->hex_surface_object_data.grid_dimension.z - 1) return false;
		//if (hex_surface_index_coordinate.y < 0 || hex_surface_index_coordinate.x < 0) return false;

//		if (hex_surface_index_coordinate.z % 2 == 0) {// even z level
			if (hex_surface_index_coordinate.y > cloud->hex_surface_object_data.grid_dimension.y - 1) return false;

			if (hex_surface_index_coordinate.y % 2 == 0) { // even y coord
				if (hex_surface_index_coordinate.x > cloud->hex_surface_object_data.grid_dimension.x - 1) return false;
			}
			else {
				if (hex_surface_index_coordinate.x > cloud->hex_surface_object_data.grid_dimension.x - 2) return false;
			}

//QMessageBox::information(0, "get_hex_surface_neighbour_index_coordinates", "here06A", QMessageBox::Ok);
		return true;
	}

	static void assign_no_hex_state_change(hex_index_vector hex_surface_data_index_coord, hex_surface_object_class* cloud, hex_surface_object_class *hex_surface_automata_next_step_matrix) {
		hex_surface_data_type hex_surface_activation_status = cloud->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status(hex_surface_data_index_coord);

		hex_surface_automata_next_step_matrix->hex_surface_object_data.activate_hex_surface_matrix_coordinate(hex_surface_data_index_coord, hex_surface_activation_status);
	}

	// Copy into the cellular automata data structure variable hex_surface_automata_initial_data the cellula automata parameter values
	// that are stored in the virtual worlds scene data model that hex surface hex_surface_automata_initial_data points to
	bool load_hex_surface_automata_initial_data(hex_surface_object_class* hex_surface_automata_initial_data) {
		if (hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_data.size() == 0) return false;
		if (hex_surface_automata_initial_data->hex_surface_object_data.hex_size != hex_surface_automata_generator_parameters.resolution_step)
			hex_surface_automata_generator_parameters.resolution_step = hex_surface_automata_initial_data->hex_surface_object_data.hex_size;

//QMessageBox::information(NULL, "","load_hex_surface_automata_initial_data 001", QMessageBox::Ok);
		if (cloud == NULL) {
			cloud = new hex_surface_object_class;
			if (cloud == NULL) {
				QMessageBox::information(NULL, "Hex Suface Automata", "Memory Allocation Error:\nCould not allocate memory to import initial hex surface automata data.\n", QMessageBox::Ok);
				return false;
			}
		}

		cloud->hex_surface_object_data.hex_surface_matrix_data.clear();
		cloud->hex_surface_object_data.hex_surface_matrix_data.shrink_to_fit();
		if (!create_hex_surface_matrix()) return false;
//QMessageBox::information(NULL, "","load_hex_surface_automata_initial_data 002", QMessageBox::Ok);
		hex_surface_index_data_type iX = 0, iY, iZ;

//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002 : " + std::string::number(origin_offset.x())+":"+std::string::number(coordinate_offset.x())+":"+std::string::number(coordinate_offset_xi), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002AA : " + std::string::number(cloud->hex_surface_object_data.grid_dimension.x)+":"+std::string::number(cloud->hex_surface_object_data.grid_dimension.y)+":"+std::string::number(cloud->hex_surface_object_data.grid_dimension.z), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002AAB : " + std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_data.size()), QMessageBox::Ok);

		hex_surface_index_data_type data_set_x_size, data_set_y_size, data_set_z_size;
		hex_surface_index_data_type  dim_x, dim_y;
		bool even_z_level;

		data_set_x_size = cloud->hex_surface_object_data.grid_dimension.x;
		data_set_y_size = cloud->hex_surface_object_data.grid_dimension.y;
		data_set_z_size = cloud->hex_surface_object_data.grid_dimension.z;

		iZ = 0;
		for (iZ = 0; iZ < data_set_z_size; iZ++) {
			if (iZ % 2 == 0)
				even_z_level = true;
			else
				even_z_level = false;

			dim_y = ((iZ + 1) % 2) * data_set_y_size + (iZ % 2) * (data_set_y_size - 1);

			for (iY = 0; iY < dim_y; iY++) {
				if (even_z_level) {
					dim_x = (iY + 1) % 2 * data_set_x_size + (iY % 2) * (data_set_x_size - 1);
				}
				else {
					dim_x = (iY + 1) % 2 * (data_set_x_size - 1) + (iY % 2) * data_set_x_size;
				}
				for (iX = 0; iX < dim_x; iX++) {
					if (hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status({ iX, iY }) > hex_surface_data_type(0)) {  // *** Need to change this for a valid activation value
					//if (hex_surface_automata_initial_data->hex_surface_object_data.hex_surface_matrix_coordinate_activation_status({ iX, iY, iZ }) > hex_surface_data_type(0)) {  // *** Need to change this for a valid activation value
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002B : " + std::string::number(initial_iX)+":"+std::string::number(initial_iY)+":"+std::string::number(initial_iZ), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 002C : " + std::string::number(automata_data_index_coord.matrix_index.x)+":"+std::string::number(automata_data_index_coord.matrix_index.y)+":"+ std::string::number(automata_data_index_coord.matrix_index.z)+":"+std::string::number(automata_data_index_coord.j_bit_index), QMessageBox::Ok);
						//cloud->hex_surface_object_data.activate_hex_surface_matrix_coordinate({ iX, iY, iZ }, 1);
						cloud->hex_surface_object_data.activate_hex_surface_matrix_coordinate({ iX, iY}, 1);
					}
				}
			}
		}
//QMessageBox::information(NULL, "","assign_no_hex_state_chnage 003", QMessageBox::Ok);
		return true;

	}

	// Create an empty hex surface matrix
	bool create_hex_surface_matrix() {
//QMessageBox::information(NULL, "","create_hex_surface_matrix 001", QMessageBox::Ok);
		// ########### CREATE EMPTY HEX CLOUD MATRIX #################
		float x_size = hex_surface_automata_generator_parameters.x_end - hex_surface_automata_generator_parameters.x_start;
		float y_size = hex_surface_automata_generator_parameters.y_end - hex_surface_automata_generator_parameters.y_start;
		//float z_size = hex_surface_automata_generator_parameters.z_end - hex_surface_automata_generator_parameters.z_start;

		float z_mult = 2.0 * sqrt(6.0) / 3.0;

		float x_res_step = hex_surface_automata_generator_parameters.resolution_step * 2.0;
		float y_res_step = hex_surface_automata_generator_parameters.resolution_step * (3.0 / sqrt(3.0));
		float z_res_step = hex_surface_automata_generator_parameters.resolution_step * z_mult;

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		//if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0)
		//	data_set_z_size = (int)(z_size / z_res_step) + 1;
		//else
		//	data_set_z_size = (int)(z_size / z_res_step);
		QMessageBox::information(0, "Automata", "Automata create_hex_surface_matrix 00: " + std::string::number(data_set_x_size) + ":" + std::string::number(data_set_y_size) + ":" + std::string::number(data_set_z_size) + ":", QMessageBox::Ok);

		glm::vec3 origin = { hex_surface_automata_generator_parameters.x_start,hex_surface_automata_generator_parameters.y_start,0 };

		cloud->hex_surface_object_data.hex_size = hex_surface_automata_generator_parameters.resolution_step;
		//cloud->world_location = origin;

		cloud->hex_surface_object_data.grid_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
		cloud->hex_surface_object_data.grid_origin = { origin.x,origin.y };
		cloud->hex_surface_object_data.create_empty_surface_cubic(data_set_x_size, data_set_y_size);
		QMessageBox::information(0, "Automata", "Automata create_hex_surface_matrix 01: " + std::string::number(cloud->hex_surface_object_data.hex_surface_matrix_data.size()) + ":", QMessageBox::Ok);


		if (cloud->hex_surface_object_data.hex_surface_matrix_data.size() > 0)
			return true;
		else
			return false;
	}
};
*/