#pragma once

#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

#include "../../Functions/vw_voxel_exports.h"

class voxel_main_window_menu_functions_class {
public:

    void voxel_volume_to_voxel_surface(int selection, id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);
  
    void export_voxels_center_point_data(int export_selection,  id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);
    void export_voxels_point_surface_data(int export_selection, id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);
    void export_voxels_surface_face_data(int export_selection,  id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);

private:
    export_voxel_geometry_class  export_voxel_geometry;

    void voxel_center_points_selected(voxel_hcp_scene_objects_class& voxel_hcp_entities, id_type entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);
    void voxel_center_points_active(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, scene_manager_class* scene_manager, log_panel_class* log_panel);
    void voxel_center_points_all(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, scene_manager_class* scene_manager, log_panel_class* log_panel);
    void create_voxel_center_point_data(voxel_hcp_object_class* voxel_hcp_object, id_type entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel);

    voxel_hcp_scene_objects_class* get_voxel_hcp_scene_objects(scene_manager_class* scene_manager);
};