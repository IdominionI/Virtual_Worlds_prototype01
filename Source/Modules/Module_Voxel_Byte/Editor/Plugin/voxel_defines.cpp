#include "voxel_defines.h"

#include "../Widgets/hcp_voxel_parameters_widget.h"
#include "../../Editor/Scene/voxel_hcp_scene_objects.h"
#include "../../Render/voxel_hcp_render_object.h"
#include "../../Voxel_hcp_object/voxel_hcp_import_export.h"
#include "../../Animation/scene_animation_functions.h"

    void hcp_voxel_defines_class::define_entity_db_categories(scene_entities_db_manager_class* scene_entities_db_manager,
                                                              log_panel_class* log_panel,
                                                              scene_manager_class* scene_manager) {
        // Define scene entity objects data base

        if (scene_entities_db_manager == NULL) return;

        // Define voxel entity db
        scene_entities_db_manager->define_new_entity_category(ENTITY_CATEGORY_HCP_VOXEL);

        int index = scene_entities_db_manager->get_objects_of_category_index(ENTITY_CATEGORY_HCP_VOXEL);

        if (index < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel entity category\n");
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP voxel entity category\n");

            voxel_hcp_scene_objects_class* voxel_hcp_scene_objects = new voxel_hcp_scene_objects_class;
            if (voxel_hcp_scene_objects == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel entity category object\n");
            }
            else {
                //printf("voxel_hcp_scene_objects != NULL\n");

                voxel_hcp_scene_objects->objects_category_id = scene_entities_db_manager->scene_objects[index]->objects_category_id;

                scene_entities_db_manager->scene_objects[index] = voxel_hcp_scene_objects;
            }

            // Following for testing only :: Delete or comment out when not needed

        }
    }

    void hcp_voxel_defines_class::define_render_object_categories(scene_render_objects_manager_class* render_objects_manager,
                                                                  log_panel_class* log_panel,
                                                                  scene_manager_class* scene_manager) {

        if (render_objects_manager == NULL) return;

        // Define voxel entity db
        render_objects_manager->define_new_entity_render_category(ENTITY_CATEGORY_HCP_VOXEL);

        int index = render_objects_manager->get_objects_of_category_index(ENTITY_CATEGORY_HCP_VOXEL);

        if (index < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel render category\n");
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP voxel render category\n");

            voxel_hcp_render_object_class* voxel_hcp_scene_render_object = new voxel_hcp_render_object_class;
            if (voxel_hcp_scene_render_object == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel render category object\n");
            }
            else {
                // printf("voxel_hcp_scene_render_object != NULL\n");

                voxel_hcp_scene_render_object->objects_category_id = render_objects_manager->scene_render_objects[index]->objects_category_id;
                voxel_hcp_scene_render_object->log_panel = log_panel;

                render_objects_manager->scene_render_objects[index] = voxel_hcp_scene_render_object;
            }

            // Following for testing only :: Delete or comment out when not needed

            //scene_entities_db_manager.add_new_entity(0, ENTITY_CATEGORY_HCP_VOXEL);

        }
    }

    void hcp_voxel_defines_class::define_outliner_import_export_managers(outliner_import_export_manager_class* outliner_import_export_manager,
                                                                         log_panel_class* log_panel,
                                                                         scene_manager_class* scene_manager) {

        if (outliner_import_export_manager == NULL) return;

        // Define voxel import/export functions
        outliner_import_export_manager->define_new_import_export_category(ENTITY_CATEGORY_HCP_VOXEL);

        int index = outliner_import_export_manager->get_import_export_category_index(ENTITY_CATEGORY_HCP_VOXEL);

        if (index < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define outliner HCP voxel import/export category\n");
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP voxel import/export category\n");

            hcp_voxel_import_export_class* hcp_voxel_import_export = new hcp_voxel_import_export_class;
            if (hcp_voxel_import_export == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel import/export category object\n");
            }
            else {
                //printf("hcp_voxel_import_export != NULL\n");

                hcp_voxel_import_export->import_export_object_category_id = outliner_import_export_manager->import_export_objects[index]->import_export_object_category_id;

                outliner_import_export_manager->import_export_objects[index] = hcp_voxel_import_export;
            }
        }
    }

    void hcp_voxel_defines_class::define_animation_manager_categories(animation_widget_class *animation_widget,
                                                                      log_panel_class* log_panel,
                                                                      scene_manager_class* scene_manager) {
        // ############# HCP Voxel  object ############
        animation_widget->define_new_animation_entity_category(ENTITY_CATEGORY_HCP_VOXEL);

        int index = animation_widget->get_animation_objects_of_category_index(ENTITY_CATEGORY_HCP_VOXEL);

        if (index < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define animation HCP voxel category\n");
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP Voxel animation category\n");

            scene_hcp_voxel_animation_class* scene_hcp_voxel_animation = new scene_hcp_voxel_animation_class;
            if (scene_hcp_voxel_animation == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel animation category object\n");
            }
            else {
                //printf("scene_hsp_voxel_animation != NULL\n");

                scene_hcp_voxel_animation->objects_category_id = animation_widget->animation_objects[index]->objects_category_id;

                animation_widget->animation_objects[index] = scene_hcp_voxel_animation;
            }
        }
    }

    void hcp_voxel_defines_class::define_parameter_panel_widget_categories(parameters_widget_manager_class *parameters_widget_manager,
                                                                           log_panel_class* log_panel,
                                                                           scene_manager_class* scene_manager) {
        // ############# HCP Voxel  object ############
        voxel_hcp_parameters_widget_class* voxel_hcp_parameters_widget = new voxel_hcp_parameters_widget_class;
        voxel_hcp_parameters_widget->log_panel = log_panel;
        voxel_hcp_parameters_widget->scene_manager = scene_manager;
        voxel_hcp_parameters_widget->parameter_category_id = ENTITY_CATEGORY_HCP_VOXEL;

        parameters_widget_manager->add_new_parameter_widget(voxel_hcp_parameters_widget);
    }