#pragma once
#include <fstream>
#include <string>
#include <vector>

#include <Source/Editor/Import_Export/vw_import_export_parameters.h>
#include <Source/Editor/Scene/scene_entities_db_manager.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include <Source/Graphics_Engine/Viewer/scene_viewer.h>

#include "../Editor/Scene/hex_surface_scene_objects.h" // *****

/*
	export_hex_surface_geometry_class

	C++ Class that handles the export of hex suface data to a .ply formatted text
	file so as to be able to be imported into a third party 3D application like Blender.

	The hex suface data is in two forms.

	A surface point cloud where the point cloud data are the vertices of the centers of the
	hex cells that make up the surface of the hex suface object generated.

	A mesh of triangles that make up the surface of hexagonal shapes of the
	generated hex surface object.
*/

class export_hex_surface_geometry_class {
public:
	std::string  filename_to_write;
	std::fstream stream;

	log_panel_class	*log_panel = NULL;

	#define endl "\n"

	int open_file_stream(std::string filename) {
		filename_to_write = filename;

		stream.open(filename, std::ios::out | std::ios::out);

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool export_hex_surface_center_points_ply(hex_surface_scene_objects_class &hex_surface_entities_to_export,int export_selection, id_type entity_id, bool export_colors = false) {

		if (hex_surface_entities_to_export.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
			return false;
		}

		std::string directory_pathname = vwDialogs::select_directory("./");
			
		switch (export_selection) {
			case SELECTED_EXPORT : return export_hex_surface_center_points_ply_selected(directory_pathname, hex_surface_entities_to_export, entity_id, export_colors);break;
			case ACTIVE_EXPORT	 : return export_hex_surface_center_points_ply_active(directory_pathname, hex_surface_entities_to_export,export_colors);			     break;
			case ALL_EXPORT      : return export_hex_surface_center_points_ply_all(directory_pathname, hex_surface_entities_to_export,export_colors);				 break;
		}

		return true;
	}

	bool export_hex_surface_center_points_ply_selected(std::string directory_pathname, hex_surface_scene_objects_class& hex_surface_entities_to_export, id_type entity_id, bool export_colors) {

		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		hex_surface_object_class *hex_surface_object_to_export = hex_surface_entities_to_export.get_hex_surface_object(entity_id);

		if (hex_surface_object_to_export == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface geometry :: Cannot export hex point data to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export hex surface geometry", "Cannot export hex point surface to file::Could not find entity in scene data to export");
			return false;
		}

		filename_to_write = directory_pathname + "/hcp_" + hex_surface_object_to_export->object_name + ".ply";
		export_point_cloud(filename_to_write, hex_surface_object_to_export->hex_surface_object_data, export_colors);
		return true;
	}

	bool export_hex_surface_center_points_ply_active(std::string directory_pathname, hex_surface_scene_objects_class& hex_surface_entities_to_export, bool export_colors) {
		for (index_type i = 0; i < hex_surface_entities_to_export.size(); i++) {
			hex_surface_object_class* hex_surface_object_to_export = hex_surface_entities_to_export[i];

			if (hex_surface_object_to_export->active_object) {
				if (hex_surface_object_to_export->hex_surface_object_data.hex_surface_matrix_data.size() > 0) {
					filename_to_write = directory_pathname + "/hcp_" + std::to_string(i) + "_" + hex_surface_object_to_export->object_name + ".ply";

					//if (log_widget != NULL) {
					//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: exporthex_surfacecenter_points_ply "+ filename_to_write);
					//}

					export_point_cloud(filename_to_write, hex_surface_object_to_export->hex_surface_object_data, export_colors);
				}
			}

		}
		return true;
	}


	bool export_hex_surface_center_points_ply_all(std::string directory_pathname, hex_surface_scene_objects_class& hex_surface_entities_to_export, bool export_colors) {
		for (index_type i = 0; i < hex_surface_entities_to_export.size(); i++) {
			hex_surface_object_class* hex_surface_object_to_export = hex_surface_entities_to_export[i];

			if (hex_surface_object_to_export->hex_surface_object_data.hex_surface_matrix_data.size() > 0) {
				filename_to_write = directory_pathname + "/hcp_" + std::to_string(i) + "_" + hex_surface_object_to_export->object_name + ".ply";

				//if (log_widget != NULL) {
				//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: exporthex_surfacecenter_points_ply "+ filename_to_write);
				//}

				export_point_cloud(filename_to_write, hex_surface_object_to_export->hex_surface_object_data, export_colors);
			}
		}
		return true;
	}

		// ##########################################################################
	// # By default colors, of the generated voxel vertices are exported but an 
	// # option is left open to only export vertices by assigning the export_colors
	// # value to false in the calling of these functions

	// ###################### Request file name to export to #####################


	bool export_hex_surface_faces_ply(hex_surface_scene_objects_class &hex_surface_entities_to_export,int export_selection, id_type entity_id) {

		if (hex_surface_entities_to_export.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
			return false;
		}

		std::string directory_pathname = vwDialogs::select_directory("./");
			
		switch (export_selection) {
			case SELECTED_EXPORT : return export_hex_surface_faces_ply_selected(directory_pathname,hex_surface_entities_to_export, entity_id);break;
			case ACTIVE_EXPORT	 : return export_hex_surface_faces_ply_active(directory_pathname,hex_surface_entities_to_export);			     break;
			case ALL_EXPORT      : return export_hex_surface_faces_ply_all(directory_pathname,hex_surface_entities_to_export);				 break;
		}

		return true;
	}

	bool export_hex_surface_faces_ply_selected(std::string directory_pathname,hex_surface_scene_objects_class &hex_surface_entities_to_export, id_type entity_id) {

		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		hex_surface_object_class *hex_surface_object_to_export = hex_surface_entities_to_export.get_hex_surface_object(entity_id);

		if (hex_surface_object_to_export == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
			return false;
		}

		filename_to_write = directory_pathname + "/hs_" + hex_surface_object_to_export->object_name + ".ply";
		export_hex_surface(filename_to_write, hex_surface_object_to_export->hex_surface_object_data);
		return true;
	}

	bool export_hex_surface_faces_ply_active(std::string directory_pathname, hex_surface_scene_objects_class &hex_surface_entities_to_export) {
		for (index_type i = 0; i < hex_surface_entities_to_export.size(); i++) {
			hex_surface_object_class* hex_surface_object_to_export = hex_surface_entities_to_export[i];

			if (hex_surface_object_to_export->active_object) {
				if (hex_surface_object_to_export->hex_surface_object_data.hex_surface_matrix_data.size() > 0) {
					filename_to_write = directory_pathname + "/hs_" + std::to_string(i) + "_" + hex_surface_object_to_export->object_name + ".ply";

					//if (log_widget != NULL) {
					//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: exporthex_surfacecenter_points_ply "+ filename_to_write);
					//}

					export_hex_surface(filename_to_write, hex_surface_object_to_export->hex_surface_object_data);
				}
			}

		}
		return true;
	}

	bool export_hex_surface_faces_ply_all(std::string directory_pathname, hex_surface_scene_objects_class &hex_surface_entities_to_export) {
		for (index_type i = 0; i < hex_surface_entities_to_export.size(); i++) {
			hex_surface_object_class *hex_surface_object_to_export = hex_surface_entities_to_export[i];

			if (hex_surface_object_to_export->hex_surface_object_data.hex_surface_matrix_data.size() > 0) {
				filename_to_write = directory_pathname + "/hs_" + std::to_string(i) + "_" + hex_surface_object_to_export->object_name + ".ply";

				//if (log_widget != NULL) {
				//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: exporthex_surfacecenter_points_ply "+ filename_to_write);
				//}

				export_hex_surface(filename_to_write, hex_surface_object_to_export->hex_surface_object_data);
			}
		}
		return true;
	}

	// ###################### file name given to export to #####################
	bool export_hex_surface_center_points_to_file_ply(hex_surface_object_data_class &hex_surface_object_data, std::string file_pathname) {

		if (hex_surface_object_data.hex_surface_matrix_data.size() < 1) {
			vwDialogs::message_box("Export hex surface surface geometry", "Cannot export hex surface point cloud :\n No hex surface data to export ");
			return false;
		}

		if (!export_point_cloud(file_pathname, hex_surface_object_data, false)) return false;

		return true;
	}


	bool export_hex_surface_faces_ply(std::string object_file_name, hex_surface_object_data_class &hex_surface_object_data) {
		return export_hex_surface_face_ply_data(object_file_name, hex_surface_object_data, false);
	}

private:
	// --------------- Export hex surface point cloud ------------------------

	bool export_point_cloud(std::string file_pathname, hex_surface_object_data_class &hex_surface_object_data, bool export_colors, bool notification = true) {
		int i = FW::stringtools::lastIndexOf(file_pathname, ".ply");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".ply";
		else
			file_name = file_pathname;

		return export_ply_data(file_name, hex_surface_object_data, notification, export_colors);
	}

	bool export_ply_data(std::string object_file_name, hex_surface_object_data_class &hex_surface_object_data, bool notification, bool export_colors) {
		if (!open_file_stream(object_file_name)) {
			//vwDialogs::message_box("export_node", "export point cloud object ERROR:\n Failed to open file\n%s\nto write to", object_file_name.c_str());
			std::string str = "export point cloud object ERROR:\n Failed to open file \n" + object_file_name + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_object_data_ply_format(hex_surface_object_data, export_colors)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		if (notification) {
			std::string str = "Finished exporting voxel central point cloud data to file" + object_file_name;
			vwDialogs::message_box("export_node", str.c_str());
		}

		return true;
	}

	bool export_object_data_ply_format(hex_surface_object_data_class &hex_surface_object_data, bool export_colors) {
		index_data_type first_hex_export_index, last_hex_export_index;

		size_t number_hexes_to_export = number_export_hex_surface(hex_surface_object_data, first_hex_export_index, last_hex_export_index);

		if (number_hexes_to_export == 0) {
			vwDialogs::message_box(NULL, "EXPORT WARNING", "export  ERROR:\n No hexs to export to file: \n");
			return false;
		}

		create_file_ply_header(number_hexes_to_export, export_colors);

		index_data_type hex_index;

		glm::ivec3 hex_coordinate;
		glm::vec3  hex_cartesian_coordinate;

		last_hex_export_index++;
		hex_surface_data_type  hex_surface_value = 0.0;

		for (hex_index = 0; hex_index < number_hexes_to_export; hex_index++) {
			hex_surface_value = hex_surface_object_data.extract_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value, hex_index);

			hex_coordinate = hex_surface_object_data.get_matrix_coordinate(hex_index);
			hex_cartesian_coordinate = hex_surface_object_data.get_hex_surface_cartesian_coordinate(hex_coordinate);

			stream << hex_surface_object_data.grid_origin.x + hex_cartesian_coordinate.x << " " << hex_surface_object_data.grid_origin.y + hex_cartesian_coordinate.y << " " << hex_surface_value << endl;

		}

		return true;
	}

	void create_file_ply_header(size_t number_hexs_to_export, bool export_colors) {
		stream << "ply" << endl;
		stream << "format ascii 1.0" << endl;
		stream << "comment exported from virtual worlds " << endl;
		stream << "comment Voxel surface data" << endl;
		stream << "element vertex " << number_hexs_to_export << endl;
		stream << "property float x" << endl;
		stream << "property float y" << endl;
		stream << "property float z" << endl;

		stream << "end_header" << endl;
	}

	size_t number_export_hex_surface(hex_surface_object_data_class &hex_surface_object_data, index_data_type &first_hex_surface_export_index, index_data_type &last_hex_surface_export_index) {
		return hex_surface_object_data.hex_surface_matrix_data.size();
	}

	//  --------------- END Export Voxel surface2 ply------------------------

	//  --------------- EXPORT HEX SURFACE FACES PLY ------------------------

	bool export_hex_surface(std::string file_pathname, hex_surface_object_data_class &hex_surface_object_data) {
		int i = FW::stringtools::lastIndexOf(file_pathname, ".ply");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".ply";
		else
			file_name = file_pathname;

		return export_hex_surface_face_ply_data(file_name, hex_surface_object_data);
	}

	bool export_hex_surface_face_ply_data(std::string object_file_name, hex_surface_object_data_class &hex_surface_object_data, bool notification = true) {
		if (!open_file_stream(object_file_name)) {
			std::string str = "export point cloud hex surface object ERROR:\n Failed to open file \n" + object_file_name + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_hex_face_data_ply_format(hex_surface_object_data)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		if (notification) {
			std::string str = "Finished exporting hex surface surface data to file\n" + object_file_name;
			vwDialogs::message_box("export_node", str.c_str());
		}

		return true;
	}

	bool export_hex_face_data_ply_format(hex_surface_object_data_class &hex_surface_object_data) {
		index_data_type first_hex_surface_export_index, last_hex_surface_export_index;

		size_t number_vertices_to_export = hex_surface_object_data.hex_surface_matrix_data.size();
		size_t number_faces_to_export = get_number_hex_surface_faces_to_export(hex_surface_object_data);

//if (number_vertices_to_export != number_vertex_normals_to_export) {
//	QMessageBox::information(NULL, "export_node", "ERROR : Number of vertices do not match number vertex normals to export.\n Voxel surface expor aborted.", QMessageBox::Ok);
//	return false;
//}

		create_hex_face_ply_header(number_vertices_to_export, number_faces_to_export);

		write_hex_face_ply_vertex_data_to_file(hex_surface_object_data, number_vertices_to_export);
		write_hex_face_ply_face_data_to_file(hex_surface_object_data, number_faces_to_export);

		return true;
	}

	void create_hex_face_ply_header(size_t number_vertices_to_export, size_t number_faces_to_export) {
		stream << "ply" << endl;
		stream << "format ascii 1.0" << endl;
		stream << "comment exported from virtual worlds " << endl;
		stream << "comment Voxel surface data" << endl;
		stream << "element vertex " << number_vertices_to_export << endl;
		stream << "property float x" << endl;
		stream << "property float y" << endl;
		stream << "property float z" << endl;
		//stream << "property float nx" << endl;
		//stream << "property float ny" << endl;
		//stream << "property float nz" << endl;
		stream << "element face " << number_faces_to_export << endl;
		stream << "property list uchar int vertex_index" << endl;
		stream << "end_header" << endl;
	}

	size_t get_number_hex_surface_faces_to_export(hex_surface_object_data_class &hex_surface_object_data) {
		size_t x_dim = hex_surface_object_data.grid_dimension.x;
		size_t y_dim = hex_surface_object_data.grid_dimension.y;

		size_t number_even_rows = size_t(y_dim / 2) + y_dim % 2;
		size_t number_odd_rows = size_t(y_dim / 2);

		size_t number_faces = number_odd_rows + number_odd_rows * (x_dim - 2) * 2 + (number_even_rows - 1) * (x_dim - 2) * 2 + (number_even_rows - 1);

		return number_faces;
	}


	void write_hex_face_ply_vertex_data_to_file(hex_surface_object_data_class &hex_surface_object_data, size_t number_vertices_to_export) {
		hex_surface_data_type  hex_surface_value = 0.0;

		glm::ivec3 hex_coordinate;
		glm::vec3  hex_cartesian_coordinate;

		for (size_t hex_index = 0; hex_index < number_vertices_to_export; hex_index++) {
			hex_surface_value = hex_surface_object_data.extract_hex_surface_data_element_value(hex_surface_data_storage_type_enum::value, hex_index);

			hex_coordinate           = hex_surface_object_data.get_matrix_coordinate(hex_index);
			hex_cartesian_coordinate = hex_surface_object_data.get_hex_surface_cartesian_coordinate(hex_coordinate);

			stream << hex_surface_object_data.grid_origin.x + hex_cartesian_coordinate.x << " " << hex_surface_object_data.grid_origin.y + hex_cartesian_coordinate.y << " " << hex_surface_value << endl;
		}
	}

	void write_hex_face_ply_face_data_to_file(hex_surface_object_data_class &hex_surface_object_data, size_t number_faces_to_export) {
		size_t x_dim = hex_surface_object_data.grid_dimension.x;
		size_t y_dim = hex_surface_object_data.grid_dimension.y;

		size_t number_even_rows = size_t(y_dim / 2) + y_dim % 2;
		size_t number_odd_rows = size_t(y_dim / 2);

		size_t n = 0;

		for (size_t j = 0; j < y_dim; j++) {
			size_t x_d = x_dim - j % 2;
			for (size_t i = 0; i < x_d; i++) {

				if (j % 2 == 0 && j > 0) { // have even row
					if (i > 0) {
						stream << "3 " << n - x_dim << " " << n << " " << n - 1 << endl;
						if (i < x_dim - 1) {
							stream << "3 " << n - x_dim + 1 << " " << n << " " <<  n - x_dim << endl;
						}
					}
				}

				if (j % 2 != 0) {// have odd row
					if (i > 0) {
						stream << "3 " << n - x_dim << " " << n << " " <<  n - 1 << endl;
					}
					stream << "3 " << n - x_dim + 1 << " " << n << " " << n - x_dim << endl;
				}
				n++;
			}
		}
	}

};