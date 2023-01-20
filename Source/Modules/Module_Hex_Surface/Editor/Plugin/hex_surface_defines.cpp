#include "hex_surface_defines.h"

#include "../Widgets/hex_parameters_widget.h"
#include "../../Editor/Scene/hex_surface_scene_objects.h"
#include "../../Render/hex_surface_render_object.h"
#include "../../Hex_surface_object/hex_surface_import_export.h"
#include "../../Animation/scene_animation_functions.h"


void hex_surface_defines_class::define_entity_db_categories(scene_entities_db_manager_class* scene_entities_db_manager,
                                                            log_panel_class* log_panel,
                                                            scene_manager_class* scene_manager) {
    // Define hex surface entity db
    scene_entities_db_manager->define_new_entity_category(ENTITY_CATEGORY_HEX_SURF);
    int index = scene_entities_db_manager->get_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF);

    if (index < 0) {
        if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface entity category\n");
    }
    else {
        if (log_panel != NULL) log_panel->application_log.AddLog("Defined Hex surface entity category\n");

        hex_surface_scene_objects_class* hex_surface_scene_objects = new hex_surface_scene_objects_class;
        if (hex_surface_scene_objects == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface entity category object\n");
        }
        else {
            //printf("hex_surface_scene_objects != NULL\n");

            hex_surface_scene_objects->objects_category_id = scene_entities_db_manager->scene_objects[index]->objects_category_id;

            scene_entities_db_manager->scene_objects[index] = hex_surface_scene_objects;
        }

        // Following for testing only :: Delete or comment out when not needed

    }

    // ADD OTHER DATA CATEGORY TYPES BELOW

}

void hex_surface_defines_class::define_render_object_categories(scene_render_objects_manager_class* render_objects_manager,
                                                                log_panel_class* log_panel,
                                                                scene_manager_class* scene_manager) {

    if (render_objects_manager == NULL) return;

    // Define voxel entity db
    render_objects_manager->define_new_entity_render_category(ENTITY_CATEGORY_HEX_SURF);

    int index = render_objects_manager->get_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF);

    if (index < 0) {
        if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface render category\n");
    }
    else {
        if (log_panel != NULL) log_panel->application_log.AddLog("Defined Hex surface render category\n");

        hex_surface_render_object_class* hex_surface_scene_render_object = new hex_surface_render_object_class;
        if (hex_surface_scene_render_object == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface render category object\n");
        }
        else {
            //printf("voxel_hcp_scene_render_object != NULL\n");

            hex_surface_scene_render_object->objects_category_id = render_objects_manager->scene_render_objects[index]->objects_category_id;
            hex_surface_scene_render_object->log_panel = log_panel;

            render_objects_manager->scene_render_objects[index] = hex_surface_scene_render_object;
        }

        // Following for testing only :: Delete or comment out when not needed

        //scene_entities_db_manager.add_new_entity(0, ENTITY_CATEGORY_HCP_VOXEL);

    }

    // ADD OTHER DATA CATEGORY TYPES BELOW
}

void hex_surface_defines_class::define_outliner_import_export_managers(outliner_import_export_manager_class* outliner_import_export_manager,
                                                                       log_panel_class* log_panel,
                                                                       scene_manager_class* scene_manager) {
    // Define hex surface import/export functions
    outliner_import_export_manager->define_new_import_export_category(ENTITY_CATEGORY_HEX_SURF);

    int index = outliner_import_export_manager->get_import_export_category_index(ENTITY_CATEGORY_HEX_SURF);

    if (index < 0) {
        if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define outliner Hex surface import/export category\n");
    }
    else {
        if (log_panel != NULL) log_panel->application_log.AddLog("Defined Hex surface import/export category\n");

        hex_surface_import_export_class* hex_surface_import_export = new hex_surface_import_export_class;
        if (hex_surface_import_export == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface import/export category object\n");
        }
        else {
            //printf("hex surface_import_export != NULL\n");

            hex_surface_import_export->import_export_object_category_id = outliner_import_export_manager->import_export_objects[index]->import_export_object_category_id;

            outliner_import_export_manager->import_export_objects[index] = hex_surface_import_export;
        }
    }

    // ADD OTHER DATA CATEGORY TYPES BELOW
}

void hex_surface_defines_class::define_animation_manager_categories(animation_widget_class* animation_widget,
                                                                    log_panel_class* log_panel,
                                                                    scene_manager_class* scene_manager) {
    // ############# Hex surface Object ############
    animation_widget->define_new_animation_entity_category(ENTITY_CATEGORY_HEX_SURF);

    int index = animation_widget->get_animation_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF);

    if (index < 0) {
        if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define animation Hex surface category\n");
    }
    else {
        if (log_panel != NULL) log_panel->application_log.AddLog("Defined Hex surface animation category\n");

        scene_hex_surface_animation_class* scene_hex_surface_animation = new scene_hex_surface_animation_class;
        if (scene_hex_surface_animation == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define Hex surface animation category object\n");
        }
        else {
            //printf("scene_hex_surface_animation != NULL\n");

            scene_hex_surface_animation->objects_category_id = animation_widget->animation_objects[index]->objects_category_id;

            animation_widget->animation_objects[index] = scene_hex_surface_animation;
        }
    }

    // ADD OTHER DATA CATEGORY TYPES BELOW
}

//*******
void hex_surface_defines_class::define_parameter_panel_widget_categories(parameters_widget_manager_class* parameters_widget_manager,
                                                                         log_panel_class* log_panel,
                                                                         scene_manager_class* scene_manager) {
    // ############# Hex surface Object ############
    hex_surface_parameters_widget_class* hex_surface_parameters_widget = new hex_surface_parameters_widget_class;
    hex_surface_parameters_widget->log_panel             = log_panel;
    hex_surface_parameters_widget->scene_manager         = scene_manager;
    hex_surface_parameters_widget->parameter_category_id = ENTITY_CATEGORY_HEX_SURF;

    parameters_widget_manager->add_new_parameter_widget(hex_surface_parameters_widget);

}