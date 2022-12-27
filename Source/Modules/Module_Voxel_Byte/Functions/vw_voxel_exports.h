#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "voxel_basic_functions.h"

#include <Source/Editor/Import_Export/vw_import_export_parameters.h>
#include <Source/Editor/Scene/scene_entities_db_manager.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include <Source/Graphics_Engine/Viewer/scene_viewer.h>

/*
	export_voxel_geometry_class

	C++ Class that handles the export of hcp voxel point cloud data to a .ply formatted text
	file so as to be able to be imported into a third party 3D application like Blender.

	The hcp voxel point cloud data is in three forms.
	A volume point cloud where the point cloud data are the vertices of the centers of the
	hcp voxels

	A surface point cloud where the point cloud data are the vertices of the centers of the
	hcp voxels that are on the surface of the hcp point cloud object generated.

	A mesh of triangles that make up the surface of Trapezo-rhombic dodecahedral shapes of the
	generated hcp point cloud object.
*/

class export_voxel_geometry_class {
public:
	std::string  filename_to_write;
	std::fstream stream;

	log_panel_class	*log_panel = NULL;

	

	int open_file_stream(std::string filename) {
		filename_to_write = filename;
		
		stream.open(filename, std::ios::out | std::ios::out);

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
			
		return true;
	}

	void close_file_stream() {
		stream.close();
	}



	// ##########################################################################
	// # By default colors, of the generated voxel vertices are exported but an 
	// # option is left open to only export vertices by assigning the export_colors
	// # value to false in the calling of these functions

	// ###################### Request file name to export to #####################


	bool export_voxel_center_points_ply(voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export,int export_selection, id_type entity_id, bool export_colors = false) {

		if (voxel_hcp_entities_to_export.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
			return false;
		}

		std::string directory_pathname = vwDialogs::select_directory("./");
			
		switch (export_selection) {
			case SELECTED_EXPORT : return export_voxel_center_points_ply_selected(directory_pathname,voxel_hcp_entities_to_export, entity_id, export_colors);break;
			case ACTIVE_EXPORT	 : return export_voxel_center_points_ply_active(directory_pathname,voxel_hcp_entities_to_export,export_colors);			     break;
			case ALL_EXPORT      : return export_voxel_center_points_ply_all(directory_pathname,voxel_hcp_entities_to_export,export_colors);				 break;
		}

		return true;
	}

	bool export_voxel_center_points_ply_selected(std::string directory_pathname,voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export, id_type entity_id, bool export_colors) {

		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		voxel_hcp_object_class *voxel_hcp_object_to_export = voxel_hcp_entities_to_export.get_voxel_hcp_object(entity_id);

		if (voxel_hcp_object_to_export == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point data to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
			return false;
		}

		filename_to_write = directory_pathname + "/cp_" + voxel_hcp_object_to_export->object_name + ".ply";
		export_point_cloud(filename_to_write, voxel_hcp_object_to_export->voxel_object_data, export_colors);
		return true;
	}

	bool export_voxel_center_points_ply_active(std::string directory_pathname, voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export, bool export_colors) {
		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class* voxel_hcp_object_to_export = voxel_hcp_entities_to_export[i];

			if (voxel_hcp_object_to_export->active_object) {
				if (voxel_hcp_object_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
					filename_to_write = directory_pathname + "/cp_" + std::to_string(i) + "_" + voxel_hcp_object_to_export->object_name + ".ply";

					//if (log_widget != NULL) {
					//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: export_voxel_center_points_ply "+ filename_to_write);
					//}

					export_point_cloud(filename_to_write, voxel_hcp_object_to_export->voxel_object_data, export_colors);
				}
			}

		}
		return true;
	}

	bool export_voxel_center_points_ply_all(std::string directory_pathname, voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export, bool export_colors) {
		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class *voxel_hcp_object_to_export = voxel_hcp_entities_to_export[i];

			if (voxel_hcp_object_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
				filename_to_write = directory_pathname + "/cp_" + std::to_string(i) + "_" + voxel_hcp_object_to_export->object_name + ".ply";

				//if (log_widget != NULL) {
				//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_function_animation_widget_class :: export_voxel_center_points_ply "+ filename_to_write);
				//}

				export_point_cloud(filename_to_write, voxel_hcp_object_to_export->voxel_object_data, export_colors);
			}
		}
		return true;
	}

	bool export_voxel_surface2_data_obj(voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface2_data) {

		if (voxel_surface2_data.surface_vertices.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface to file :\n No voxel vertex data to export ");
			return false;
		}

		if (voxel_surface2_data.surface_vertices_normals.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface to file :\n No voxel vertex data to export ");
			return false;
		}

		char const* patterns[] = { "*.obj" };
		char const* filename_to_write = vwDialogs::save_file("./", patterns, 1);

		if (!export_voxel_surface_obj(filename_to_write, voxel_object_data, voxel_surface2_data)) return false;

		return true;
	}


	//88888888888888888888888888888888888888

	//bool export_voxel_point_surface_data_ply(std::vector<entity_class*> voxel_hcp_entities_to_export, vw_editor_viewer_class *vw_viewer, bool export_colors = true, bool export_faces = false) {
	bool export_voxel_point_surface_data_ply(voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export, int export_selection, id_type entity_id, bool export_colors = false, bool export_faces = false) {
		if (voxel_hcp_entities_to_export.size() < 1) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
			return false;
		}

		std::string directory_pathname = vwDialogs::select_directory("./");
			
		switch (export_selection) {
			case SELECTED_EXPORT : return export_voxel_point_surface_ply_selected(directory_pathname,voxel_hcp_entities_to_export, entity_id, export_colors, export_faces);break;
			case ACTIVE_EXPORT	 : return export_voxel_point_surface_ply_active(directory_pathname,voxel_hcp_entities_to_export,export_colors,export_faces);			   break;
			case ALL_EXPORT      : return export_voxel_point_surface_ply_all(directory_pathname,voxel_hcp_entities_to_export,export_colors, export_faces);				   break;

		}

		return true;

	}

	bool export_voxel_point_surface_ply_selected(std::string directory_pathname, voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export, id_type entity_id, bool export_colors, bool export_faces) {
		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		voxel_hcp_object_class *voxel_hcp_object_to_export = voxel_hcp_entities_to_export.get_voxel_hcp_object(entity_id);

		if (voxel_hcp_object_to_export == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
			return false;
		}

		if (voxel_hcp_object_to_export->voxel_object_data.voxel_matrix_data.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel vertex data for  voxel enitity\n", voxel_hcp_object_to_export->object_name.c_str());
			return false;
		}

		filename_to_write = directory_pathname + "/sp_"  + voxel_hcp_object_to_export->object_name + ".ply";

		return export_point_voxel_surface_ply_data(voxel_hcp_object_to_export, filename_to_write, export_colors,export_faces);
	}

	bool export_voxel_point_surface_ply_active(std::string directory_pathname, voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, bool export_colors, bool export_faces) {
		bool exported_all = true;

		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class *voxel_entity_to_export = voxel_hcp_entities_to_export[i];
		
			if (voxel_entity_to_export->active_object){
				if (voxel_entity_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
					filename_to_write = directory_pathname + "/sp_" + std::to_string(i) + "_" + voxel_entity_to_export->object_name + ".ply";

					if (!export_point_voxel_surface_ply_data(voxel_entity_to_export, filename_to_write, export_colors, export_faces))
						exported_all = false;

				}else{
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel vertex data for  voxel enitity\n", voxel_entity_to_export->object_name.c_str());
					exported_all = false;
				}
			}

		}

		return exported_all;
	}

	bool export_voxel_point_surface_ply_all(std::string directory_pathname, voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, bool export_colors, bool export_faces) {
		bool exported_all = true;

		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class* voxel_entity_to_export = voxel_hcp_entities_to_export[i];

			filename_to_write = directory_pathname + "/sp_" + std::to_string(i) + "_" + voxel_entity_to_export->object_name + ".ply";

			if(!export_point_voxel_surface_ply_data(voxel_entity_to_export, filename_to_write, export_colors, export_faces))
				exported_all = false;

		}

		return exported_all;

	}

	bool export_point_voxel_surface_ply_data(voxel_hcp_object_class *voxel_entity_to_export, std::string filename_to_write, bool export_colors = false, bool export_faces = false) {
		if (voxel_entity_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
			voxel_surface_data_class voxel_surface_point_data;
			voxel_surface_point_data.cloud = voxel_entity_to_export;


			if (!voxel_surface_point_data.create_voxel_surface_face_data()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel surface face mesh generated\n", voxel_entity_to_export->object_name.c_str());
				return false;
			}
			else {
				if (voxel_surface_point_data.surface_vertices.size() < 1) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel vertex data to export\n", voxel_entity_to_export->object_name.c_str());
					return false;
				}
				else {
					if (voxel_surface_point_data.surface_vertices_normals.size() < 1) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel normal data to export\n", voxel_entity_to_export->object_name.c_str());
						return false;
					}
					else {
						if (voxel_surface_point_data.surface_faces.size() < 1) {
							if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nno voxel surface face data to export\n", voxel_entity_to_export->object_name.c_str());
							return false;
						}
						else
							if (!export_voxel_point_surface_ply(filename_to_write, voxel_entity_to_export->voxel_object_data, voxel_surface_point_data,export_colors,export_faces )) {
								if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \n", voxel_entity_to_export->object_name.c_str());
								return false;
							}
					}
				}
			}
			return true;
		}

		return false;
	}
	
	//88888888888888888888888888888888888888

	bool export_voxel_surface_faces_data_ply(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, int export_selection, id_type entity_id) {
		if (voxel_hcp_entities_to_export.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
			return false;
		}

		std::string directory_pathname = vwDialogs::select_directory("./");

		switch (export_selection) {
			case SELECTED_EXPORT: return export_voxel_surface_faces_ply_selected(directory_pathname, voxel_hcp_entities_to_export, entity_id); break;
			case ACTIVE_EXPORT  : return export_voxel_surface_faces_ply_active(directory_pathname, voxel_hcp_entities_to_export);			   break;
			case ALL_EXPORT     : return export_voxel_surface_faces_ply_all(directory_pathname, voxel_hcp_entities_to_export);				   break;

		}

		return true;

	}

	bool export_voxel_surface_faces_ply_selected(std::string directory_pathname, voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, id_type entity_id) {
		if (entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity selected to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
			return false;
		}

		voxel_hcp_object_class* voxel_hcp_object_to_export = voxel_hcp_entities_to_export.get_voxel_hcp_object(entity_id);

		if (voxel_hcp_object_to_export == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Could not find entity in scene data to export\n");
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
			return false;
		}

		if (voxel_hcp_object_to_export->voxel_object_data.voxel_matrix_data.size() < 1) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel vertex data for  voxel enitity\n", voxel_hcp_object_to_export->object_name.c_str());
			return false;
		}

		filename_to_write = directory_pathname + "/sf_" + voxel_hcp_object_to_export->object_name + ".ply";

		return export_voxel_point_faces_ply(voxel_hcp_object_to_export, filename_to_write);

	}

	bool export_voxel_surface_faces_ply_active(std::string directory_pathname, voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
		bool exported_all = true;

		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class* voxel_entity_to_export = voxel_hcp_entities_to_export[i];

			if (voxel_entity_to_export->active_object) {
				if (voxel_entity_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
					filename_to_write = directory_pathname + "/sf_" + std::to_string(i) + "_" + voxel_entity_to_export->object_name + ".ply";

					if (!export_voxel_point_faces_ply(voxel_entity_to_export, filename_to_write))
						exported_all = false;

				}
				else {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel vertex data for  voxel enitity\n", voxel_entity_to_export->object_name.c_str());
					exported_all = false;
				}
			}

		}

		return exported_all;
	}

	bool export_voxel_surface_faces_ply_all(std::string directory_pathname, voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
		bool exported_all = true;

		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class* voxel_entity_to_export = voxel_hcp_entities_to_export[i];

			filename_to_write = directory_pathname + "/sf_" + std::to_string(i) + "_" + voxel_entity_to_export->object_name + ".ply";

			if (!export_voxel_point_faces_ply(voxel_entity_to_export, filename_to_write))
				exported_all = false;

		}

		return exported_all;

	}

	bool export_voxel_point_faces_ply(voxel_hcp_object_class* voxel_entity_to_export, std::string filename_to_write) {
		if (voxel_entity_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
			voxel_surface_face_class voxel_surface_face_data;
			voxel_surface_face_data.cloud = voxel_entity_to_export;

			if (!voxel_surface_face_data.create_voxel_surface_data()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel surface face mesh generated\n", voxel_entity_to_export->object_name.c_str());
				return false;
			}
			else {
				if (voxel_surface_face_data.vertices.size() < 1) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel vertex data to export\n", voxel_entity_to_export->object_name.c_str());
					return false;
				}
				else {
					if (voxel_surface_face_data.normals.size() < 1) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel normal data to export\n", voxel_entity_to_export->object_name.c_str());
						return false;
					}
					else {
						if (voxel_surface_face_data.faces.size() < 1) {
							if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nno voxel surface face data to export\n", voxel_entity_to_export->object_name.c_str());
							return false;
						}
						else
							if (!export_voxel_faces_ply(filename_to_write, voxel_entity_to_export->voxel_object_data, voxel_surface_face_data)) {
								if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \n", voxel_entity_to_export->object_name.c_str());
								return false;
							}
					}
				}
			}
			return true;
		}

		return false;
	}

	bool export_voxel_surface_faces_ply(voxel_hcp_scene_objects_class &voxel_hcp_entities_to_export) {
		std::string directory_pathname = vwDialogs::select_directory("./");
		bool exported_all = true;

		for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
			voxel_hcp_object_class *voxel_entity_to_export = voxel_hcp_entities_to_export[i];
			
			if (voxel_entity_to_export->voxel_object_data.voxel_matrix_data.size() > 0) {
		
				filename_to_write = directory_pathname + "/sf_" + std::to_string(i) + "_" + voxel_entity_to_export->object_name + ".ply";

				voxel_surface_face_class voxel_surface_face_data;
				voxel_surface_face_data.cloud = voxel_entity_to_export;

				if (!voxel_surface_face_data.create_voxel_surface_data()) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Volume to Voxel Surface :: Could not generate voxel surface face data for \n %s \nNo voxel surface face mesh generated\n", voxel_entity_to_export->object_name);
					exported_all = false;
				} else {
					if (voxel_surface_face_data.vertices.size() < 1) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel vertex data to export\n", voxel_entity_to_export->object_name);
						exported_all = false;
					} else {
						if (voxel_surface_face_data.normals.size() < 1) {
							if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nNo voxel normal data to export\n", voxel_entity_to_export->object_name);
							exported_all = false;
						} else{
							if (voxel_surface_face_data.faces.size() < 1) {
								if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \nno voxel surface face data to export\n", voxel_entity_to_export->object_name);
								exported_all = false;
							} else
								if (!export_voxel_faces_ply(filename_to_write, voxel_entity_to_export->voxel_object_data, voxel_surface_face_data)) {
									if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel surface faces to file  :\n %s \n", voxel_entity_to_export->object_name);
									exported_all = false;
								}
						}
					}
				}

			}
		}

		return exported_all;
	}

	// ###################### file name given to export to #####################
	bool export_voxel_center_points_to_file_ply(voxel_object_data_class& voxel_object_data, std::string file_pathname) {

		if (voxel_object_data.voxel_matrix_data.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point cloud :\n No voxel data to export ");
			return false;
		}

		if (!export_point_cloud(file_pathname, voxel_object_data, false)) return false;

		return true;
	}

	bool export_voxel_point_surface_data_to_file_ply(voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface2_data, std::string file_pathname, bool export_faces = false) {

		if (voxel_surface2_data.surface_vertices.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface to file :\n No voxel vertex data to export ");
			return false;
		}

		if (voxel_surface2_data.surface_vertices_normals.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface to file :\n No voxel vertex data to export ");
			return false;
		}

		if (!export_voxel_point_surface_ply(file_pathname, voxel_object_data, voxel_surface2_data, export_faces, false)) return false;

		return true;
	}

	bool export_voxel_surface_faces_to_file_ply(voxel_object_data_class& voxel_object_data, voxel_surface_face_class& voxel_face_data, std::string file_pathname) {

		if (voxel_face_data.vertices.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface faces to file :\n No voxel vertex data to export ");
			return false;
		}

		if (voxel_face_data.normals.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface faces to file :\n No voxel normal data to export ");
			return false;
		}

		if (voxel_face_data.faces.size() < 1) {
			vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel surface faces to file :\n No voxel surface face data to export ");
			return false;
		}

		if (!export_voxel_faces_ply(file_pathname, voxel_object_data, voxel_face_data,false)) return false;

		return true;
	}
private:
	//std::string endl = "\n";

	// --------------- Export voxel point cloud ------------------------

	bool export_point_cloud(std::string file_pathname, voxel_object_data_class& voxel_object_data, bool export_colors, bool notification = true) {
		int i = FW::stringtools::lastIndexOf(file_pathname,".ply");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".ply";
		else
			file_name = file_pathname;

		return export_ply_data(file_name, voxel_object_data, notification, export_colors);
	}

	bool export_ply_data(std::string object_file_name, voxel_object_data_class& voxel_object_data, bool notification, bool export_colors) {
		if (!open_file_stream(object_file_name)) {
			//vwDialogs::message_box("export_node", "export point cloud object ERROR:\n Failed to open file\n%s\nto write to", object_file_name.c_str());
			std::string str = "export point cloud object ERROR:\n Failed to open file \n" + object_file_name + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_object_data_ply_format(voxel_object_data, export_colors)) {
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

	bool export_object_data_ply_format(voxel_object_data_class &voxel_object_data, bool export_colors) {
		index_data_type first_voxel_export_index, last_voxel_export_index;

		size_t number_voxels_to_export = number_export_voxels(voxel_object_data, first_voxel_export_index, last_voxel_export_index);

		if (number_voxels_to_export == 0) {
			vwDialogs::message_box("EXPORT WARNING", "export  ERROR:\n No voxels to export to file: \n");
			return false;
		}

		create_file_ply_header(number_voxels_to_export, export_colors);

		index_data_type voxel_index;

		glm::ivec3 voxel_coordinate;
		glm::vec3    voxel_cartesian_coordinate;

		last_voxel_export_index++;
		voxel_element_data_type red_value = 255, green_value = 255, blue_value = 255, voxel_value = 255;
		for (voxel_index = first_voxel_export_index; voxel_index < last_voxel_export_index; voxel_index++) {
			if (export_colors){
				red_value   = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::red,   voxel_index);
				green_value = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::green, voxel_index);
				blue_value  = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::blue,  voxel_index);
			}

			voxel_value = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index);
			if (voxel_value != DEFAULT_INACTIVE_VALUE) {

				voxel_coordinate           = voxel_object_data.get_matrix_coordinate(voxel_index);
				voxel_cartesian_coordinate = voxel_object_data.get_voxel_cartesian_coordinate(voxel_coordinate);

				if (export_colors) {
					stream << voxel_cartesian_coordinate.x << " " << voxel_cartesian_coordinate.y << " " << voxel_cartesian_coordinate.z << " ";
					stream << red_value << " " << green_value << " " << blue_value << " " << voxel_value << endl;
				} else {
					stream << voxel_cartesian_coordinate.x << " " << voxel_cartesian_coordinate.y << " " << voxel_cartesian_coordinate.z << endl;
				}
			}
		}

		return true;
	}

	void create_file_ply_header(size_t number_voxels_to_export, bool export_colors) {
		stream << "ply" << endl;
		stream << "format ascii 1.0" << endl;
		stream << "comment exported from virtual worlds " << endl;
		stream << "comment Voxel surface data" << endl;
		stream << "element vertex " << number_voxels_to_export << endl;
		stream << "property float x" << endl;
		stream << "property float y" << endl;
		stream << "property float z" << endl;

		if (export_colors) {
			stream << "property uchar red"   << endl;
			stream << "property uchar green" << endl;
			stream << "property uchar blue"  << endl;
			stream << "property uchar alpha" << endl;
		}

		stream << "end_header" << endl;
	}

	size_t number_export_voxels(voxel_object_data_class &voxel_object_data, index_data_type& first_voxel_export_index, index_data_type& last_voxel_export_index) {
		size_t          number_voxels_to_export = 0, number_voxels = voxel_object_data.voxel_matrix_data.size();
		index_data_type voxel_index = 0;
		bool            first_voxel_index = true;

		first_voxel_export_index = 0;
		last_voxel_export_index = 0;

		for (voxel_index = 0; voxel_index < number_voxels; voxel_index++) {
			voxel_element_data_type voxel_value = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index);

			if (voxel_value != DEFAULT_INACTIVE_VALUE) {
				if (first_voxel_index) {
					first_voxel_export_index = voxel_index;
					first_voxel_index = false;
				}

				last_voxel_export_index = voxel_index;

				number_voxels_to_export += 1;
			}
		}

		return number_voxels_to_export;
	}



	//  --------------- Export Voxel surface2 obj ------------------------

	bool export_voxel_surface_obj(std::string file_pathname, voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data) {
		int i = FW::stringtools::lastIndexOf(file_pathname, ".obj");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".obj";
		else
			file_name = file_pathname;

		return export_voxel_surface2_obj_data(file_name, voxel_object_data, voxel_surface_data);
	}

	bool export_voxel_surface2_obj_data(std::string object_file_name, voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data) {
		if (!open_file_stream(object_file_name)) {
			std::string str = "export point cloud object ERROR:\n Failed to open file \n" + object_file_name + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_surface2_data_obj_format(voxel_object_data, voxel_surface_data)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		vwDialogs::message_box("export_node", "Finished exporting voxel surface data to file %s\n",object_file_name.c_str());


		return true;
	}

	bool export_surface2_data_obj_format(voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data) {
		index_data_type first_voxel_export_index, last_voxel_export_index;

		size_t number_vertices_to_export = voxel_surface_data.surface_vertices.size();
		size_t number_vertex_normals_to_export = voxel_surface_data.surface_vertices_normals.size();

		if (number_vertices_to_export != number_vertex_normals_to_export) {
			vwDialogs::message_box("export_node", "ERROR : Number of vertices do not match number vertex normals to export.\n Voxel surface expor aborted.");
			return false;
		}

		write_voxel_surface2_vertex_data_to_obj_file(voxel_surface_data.surface_vertices, number_vertices_to_export, voxel_object_data);
		write_voxel_surface2_vertex_normal_data_to_obj_file(voxel_surface_data.surface_vertices_normals, number_vertex_normals_to_export, voxel_object_data);

		return true;
	}

	void write_voxel_surface2_vertex_data_to_obj_file(std::vector<surface_vertices_struct_type>& surface_vertices, size_t number_vertices_to_export, voxel_object_data_class& voxel_object_data) {
		index_data_type index;
		glm::ivec3            voxel_coordinate;
		glm::vec3               voxel_cartesian_coordinate;

		for (index = 0; index < number_vertices_to_export; index++) {
			voxel_coordinate = voxel_object_data.get_matrix_coordinate(surface_vertices[index].voxel_index);
			voxel_cartesian_coordinate = voxel_object_data.get_voxel_cartesian_coordinate(voxel_coordinate);

			stream << "v " << voxel_cartesian_coordinate.x << " " << voxel_cartesian_coordinate.y << " " << voxel_cartesian_coordinate.z << endl;
		}
	}

	void write_voxel_surface2_vertex_normal_data_to_obj_file(std::vector<surface_vertices_normal_struct_type>& surface_vertices_normal, size_t number_vertex_normals_to_export, voxel_object_data_class& voxel_object_data) {
		index_data_type index;

		for (index = 0; index < number_vertex_normals_to_export; index++) {
			
			stream << "vn " << surface_vertices_normal[index].voxel_normal.x << " " << surface_vertices_normal[index].voxel_normal.y << " " << surface_vertices_normal[index].voxel_normal.z << endl;
		}
	}
	//  --------------- END Export Voxel surface2 obj------------------------

	//  --------------- Export Voxel surface2 ply ------------------------

	bool export_voxel_point_surface_ply(std::string file_pathname, voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data, bool export_colors, bool export_faces, bool notification = true) {
		int i = FW::stringtools::lastIndexOf(file_pathname, ".ply");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".ply";
		else
			file_name = file_pathname;
	
		return export_voxel_point_surface_ply_data(file_name, voxel_object_data, voxel_surface_data, export_colors, export_faces,notification);
	}

	bool export_voxel_point_surface_ply_data(std::string object_file_name, voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data, bool export_colors, bool export_faces,bool notification) {

		if (!open_file_stream(object_file_name)) {
			//vwDialogs::message_box("export_node", "export point cloud object ERROR:\n Failed to open file\n %s \nto write to",object_file_name.c_str());
			std::string str = "export point cloud object ERROR:\n Failed to open file \n" + object_file_name +"\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_surface_point_data_ply_format(voxel_object_data, voxel_surface_data, export_colors, export_faces)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		if (notification) {
			std::string str = "Finished exporting voxel surface data to file" + object_file_name;
			vwDialogs::message_box("export_node", str.c_str());
		}

		return true;
	}

	bool export_surface_point_data_ply_format(voxel_object_data_class& voxel_object_data, voxel_surface_data_class& voxel_surface_data, bool export_colors, bool export_faces) {
		index_data_type first_voxel_export_index, last_voxel_export_index;

		size_t number_vertices_to_export = voxel_surface_data.surface_vertices.size();
		size_t number_vertex_normals_to_export = voxel_surface_data.surface_vertices_normals.size();
		size_t number_faces_to_export = voxel_surface_data.surface_faces.size();

		if (number_vertices_to_export != number_vertex_normals_to_export) {
			vwDialogs::message_box("export_node", "ERROR : Number of vertices do not match number vertex normals to export.\n Voxel surface expor aborted.");
			return false;
		}

		create_voxel_point_surface_ply_header(number_vertices_to_export, number_faces_to_export, export_colors, export_faces);

		write_voxel_point_surface_ply_vertex_data_to_file(voxel_surface_data.surface_vertices, voxel_surface_data.surface_vertices_normals, number_vertices_to_export, voxel_object_data, export_colors);
		if (export_faces)
			write_voxel_point_surface_ply_face_data_to_file(voxel_surface_data, number_faces_to_export, voxel_object_data);

		return true;
	}

	void create_voxel_point_surface_ply_header(size_t number_vertices_to_export, size_t number_faces_to_export, bool export_colors, bool export_faces) {
		stream << "ply" << endl;
		stream << "format ascii 1.0" << endl;
		stream << "comment exported from virtual worlds " << endl;
		stream << "comment Voxel surface data" << endl;
		stream << "element vertex " << number_vertices_to_export << endl;
		stream << "property float x" << endl;
		stream << "property float y" << endl;
		stream << "property float z" << endl;
		stream << "property float nx" << endl;
		stream << "property float ny" << endl;
		stream << "property float nz" << endl;

		if (export_colors) {
			stream << "property uchar red" << endl;
			stream << "property uchar green" << endl;
			stream << "property uchar blue" << endl;
			stream << "property uchar alpha" << endl;
		}

		if (export_faces) {
			stream << "element face " << number_faces_to_export << endl;
			stream << "property list uchar int vertex_index" << endl;
		}
		stream << "end_header" << endl;
	}

	void write_voxel_point_surface_ply_vertex_data_to_file(std::vector<surface_vertices_struct_type>& surface_vertices, std::vector <surface_vertices_normal_struct_type> surface_vertices_normals, size_t number_vertices_to_export, voxel_object_data_class& voxel_object_data, bool export_colors) {
		index_data_type index;

		glm::ivec3  voxel_coordinate;
		glm::vec3     voxel_cartesian_coordinate;

		voxel_element_data_type red_value = 255, green_value = 255, blue_value = 255, voxel_value = 255;
		for (index = 0; index < number_vertices_to_export; index++) {
			voxel_coordinate           = voxel_object_data.get_matrix_coordinate(surface_vertices[index].voxel_index);
			voxel_cartesian_coordinate = voxel_object_data.get_voxel_cartesian_coordinate(voxel_coordinate);

			if (export_colors){
				red_value   = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::red,   index);
				green_value = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::green, index);
				blue_value  = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::blue,  index);
				voxel_value = voxel_object_data.extract_voxel_data_element_value(data_storage_type_enum::value, index);
			}

			if (export_colors) {
				stream << voxel_cartesian_coordinate.x << " " << voxel_cartesian_coordinate.y << " " << voxel_cartesian_coordinate.z << " ";
				stream << surface_vertices_normals[index].voxel_normal.x << " " << surface_vertices_normals[index].voxel_normal.y << " " << surface_vertices_normals[index].voxel_normal.z << " ";
				stream << red_value << " " << green_value << " " << blue_value << " " << voxel_value << endl;
			} else {
				stream << voxel_cartesian_coordinate.x << " " << voxel_cartesian_coordinate.y << " " << voxel_cartesian_coordinate.z << " ";
				stream << surface_vertices_normals[index].voxel_normal.x << " " << surface_vertices_normals[index].voxel_normal.y << " " << surface_vertices_normals[index].voxel_normal.z << endl;
			}

		}

	}

	void write_voxel_point_surface_ply_face_data_to_file(voxel_surface_data_class& surface_data, size_t number_faces_to_export, voxel_object_data_class& voxel_object_data) {
		index_data_type index;

		bool                    surface_error = false;
		glm::ivec3            voxel_coordinate;
		glm::vec3               voxel_cartesian_coordinate;

		std::vector<surface_faces_struct_type> surface_faces = surface_data.surface_faces;
		index_data_type vertex1_id, vertex2_id, vertex3_id;

		for (index = 0; index < number_faces_to_export; index++) {
			surface_faces_struct_type surface_face = surface_faces[index];

			vertex1_id = get_vertex_index(surface_data.surface_vertices, surface_face.voxel_index.x);
			vertex2_id = get_vertex_index(surface_data.surface_vertices, surface_face.voxel_index.y);
			vertex3_id = get_vertex_index(surface_data.surface_vertices, surface_face.voxel_index.z);

			if (vertex1_id < 0 || vertex1_id > surface_data.surface_vertices.size() ||
				vertex2_id < 0 || vertex2_id > surface_data.surface_vertices.size() ||
				vertex3_id < 0 || vertex3_id > surface_data.surface_vertices.size()) {
				surface_error = true;
//vwDialogs::message_box("Voxel Volume to Voxel Surface", "surface_error 00 :" + std::string::number(surface_data.surface_vertices.size()) +"\n"+ std::string::number(surface_face.voxel_index.x) + " : " + std::string::number(surface_face.voxel_index.x) + " : " + std::string::number(surface_face.voxel_index.x)
//																					 + "\n"+ std::string::number(vertex1_id) + " : " + std::string::number(vertex2_id) + " : " + std::string::number(vertex3_id));
			}
			else
				stream << 3 << " " << vertex1_id << " " << vertex2_id << " " << vertex3_id << endl;
		}

		if (surface_error) {
			vwDialogs::message_box("Voxel Volume to Voxel Surface", "WARNING : \n Face vericies have missing vertex indices"
				"\n Voxel surface generation has corrupted face data"
				"\n Probable cause is that suface voxels not correctly defined and some interior voxels\nincorrectly defined as surface voxels");
		}

	}

	//  --------------- END Export Voxel surface2 ply------------------------

	index_data_type get_vertex_index(std::vector<surface_vertices_struct_type>& surface_vertices, index_data_type voxel_index) {
		size_t i, number_vertices = surface_vertices.size();

		for (i = 0; i < number_vertices; i++) {
			if (surface_vertices[i].voxel_index == voxel_index) {
				return surface_vertices[i].vertex_id;
				break;
			}
		}

		return -1;
	}

	//  --------------- EXPORT VOXEL FACES PLY ------------------------

	bool export_voxel_faces_ply(std::string file_pathname, voxel_object_data_class &voxel_object_data, voxel_surface_face_class &voxel_face_data, bool notification = true) {
		int i = FW::stringtools::lastIndexOf(file_pathname, ".ply");
		std::string file_name;

		if (i < 1)
			file_name = file_pathname + ".ply";
		else
			file_name = file_pathname;

		return export_voxel_face_ply_data(file_name, voxel_object_data, voxel_face_data,notification);
	}

	bool export_voxel_face_ply_data(std::string object_file_name, voxel_object_data_class &voxel_object_data, voxel_surface_face_class& voxel_face_data, bool notification) {
		if (!open_file_stream(object_file_name)) {
			std::string str = "export point cloud object ERROR:\n Failed to open file\n" + object_file_name + "\nto write to";
			vwDialogs::message_box("export_node", str.c_str());
			//vwDialogs::message_box("export_node", "export point cloud object ERROR:\n Failed to open file\n %s\nto write to",object_file_name.c_str());
			return false;
		}

		if (!export_face_data_ply_format(voxel_object_data, voxel_face_data)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		if (notification) {
			std::string str = "Finished exporting voxel surface data to file" + object_file_name;
			vwDialogs::message_box("export_node", str.c_str());
		}

		return true;
	}

	bool export_face_data_ply_format(voxel_object_data_class& voxel_object_data, voxel_surface_face_class& voxel_face_data) {
		index_data_type first_voxel_export_index, last_voxel_export_index;

		size_t number_vertices_to_export = voxel_face_data.vertices.size();
		size_t number_vertex_normals_to_export = voxel_face_data.normals.size();
		size_t number_faces_to_export = voxel_face_data.faces.size();

		if (number_vertices_to_export != number_vertex_normals_to_export) {
			vwDialogs::message_box("export_node", "ERROR : Number of vertices do not match number vertex normals to export.\n Voxel surface expor aborted.");
			return false;
		}

		create_voxel_face_ply_header(number_vertices_to_export, number_faces_to_export);

		write_voxel_face_ply_vertex_data_to_file(voxel_face_data.vertices, voxel_face_data.normals, number_vertices_to_export);
		write_voxel_face_ply_face_data_to_file(voxel_face_data.faces, number_faces_to_export);

		return true;
	}

	void create_voxel_face_ply_header(size_t number_vertices_to_export, size_t number_faces_to_export) {
		stream << "ply" << endl;
		stream << "format ascii 1.0" << endl;
		stream << "comment exported from virtual worlds " << endl;
		stream << "comment Voxel surface data" << endl;
		stream << "element vertex " << number_vertices_to_export << endl;
		stream << "property float x" << endl;
		stream << "property float y" << endl;
		stream << "property float z" << endl;
		stream << "property float nx" << endl;
		stream << "property float ny" << endl;
		stream << "property float nz" << endl;
		stream << "element face " << number_faces_to_export << endl;
		stream << "property list uchar int vertex_index" << endl;
		stream << "end_header" << endl;
	}

	void write_voxel_face_ply_vertex_data_to_file(std::vector<glm::vec3>& vertices, std::vector <glm::vec3> normals, size_t number_vertices_to_export) {
		index_data_type index;

		for (index = 0; index < number_vertices_to_export; index++) {
			stream << vertices[index].x << " " << vertices[index].y << " " << vertices[index].z << " ";
			stream << normals[index].x << " " << normals[index].y << " " << normals[index].z << endl;
		}

	}

	void write_voxel_face_ply_face_data_to_file(std::vector<std::array<int, 4>> faces, size_t number_faces_to_export) {
		index_data_type index;

		for (index = 0; index < number_faces_to_export; index++) {
			stream << "4 " << faces[index][0] << " " << faces[index][1] << " " << faces[index][2] << " " << faces[index][3] << endl;
		}
	}

};

