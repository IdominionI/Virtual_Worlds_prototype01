#include "hex_surface_main_menu_functions.h"

#include "../../Editor/Scene/hex_surface_scene_objects.h"

void hex_surface_main_menu_functions_class::export_hex_surface_center_point_data(int export_selection, id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel) {
    if (scene_manager == NULL) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface point geometry :: Cannot export hex surface point dat to file:: Scene Manager is undefined\n");
    }

    hex_surface_scene_objects_class* hex_surface_scene_objects = get_hex_surface_scene_objects(scene_manager);
    if (hex_surface_scene_objects == NULL) return;
    export_hex_surface_geometry.export_hex_surface_center_points_ply(*hex_surface_scene_objects, export_selection, current_selected_entity_id);
}

void hex_surface_main_menu_functions_class::export_hex_surface_surface_face_data(int export_selection, id_type current_selected_entity_id, scene_manager_class* scene_manager, log_panel_class* log_panel) {
    if (scene_manager == NULL) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface surface geometry :: Cannot export hex surface surface dat to file:: Scene Manager is undefined\n");
    }

    hex_surface_scene_objects_class* hex_surface_scene_objects = get_hex_surface_scene_objects(scene_manager);
    if (hex_surface_scene_objects == NULL) return;
    export_hex_surface_geometry.export_hex_surface_faces_ply(*hex_surface_scene_objects, export_selection, current_selected_entity_id);
}

// -------------------------------------

hex_surface_scene_objects_class* hex_surface_main_menu_functions_class::get_hex_surface_scene_objects(scene_manager_class* scene_manager) {
    int i = scene_manager->entities_manager.get_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF);
    if (i < 0) return NULL;

    hex_surface_scene_objects_class* hex_surface_scene_objects = dynamic_cast<hex_surface_scene_objects_class*>(scene_manager->entities_manager.scene_objects[i]);
    return hex_surface_scene_objects;
}