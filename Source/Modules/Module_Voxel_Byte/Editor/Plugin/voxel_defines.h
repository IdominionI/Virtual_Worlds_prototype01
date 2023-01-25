#pragma once

#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

#include <Source/Editor/Scene/scene_entities_db_manager.h>
#include <Source/Editor/Scene/scene_render_object_manager.h>
#include <Source/Editor/Main_Window/Outliner/outliner_import_export_manager.h>
#include <Source/Editor/Main_Window/Widgets/animation_widget.h>
#include <Source/Editor/Main_Window/Widgets/parameters_widget.h>

#include <Source/Editor/Node_editor/node_editor_category_manager.h>//*****
#include "../../Node_editor/Nodes/hcp_voxel_editor_nodes.h"//****


class hcp_voxel_defines_class {
public:

        void application_module_defines(scene_entities_db_manager_class      *scene_entities_db_manager,
                                        scene_render_objects_manager_class   *render_objects_manager,
                                        outliner_import_export_manager_class *outliner_import_export_manager,
                                        animation_widget_class               *animation_widget,
                                        parameters_widget_manager_class      *parameters_widget_manager,
                                        node_editor_category_manager_class   *node_editor_category_manager,//*****
                                        log_panel_class                      *log_panel,
                                        scene_manager_class                  *scene_manager) {

            define_entity_db_categories(scene_entities_db_manager,log_panel,scene_manager);
            define_render_object_categories(render_objects_manager,log_panel,scene_manager);
            define_outliner_import_export_managers(outliner_import_export_manager,log_panel,scene_manager);
            define_animation_manager_categories(animation_widget,log_panel,scene_manager);
            define_parameter_panel_widget_categories(parameters_widget_manager,log_panel,scene_manager);

            define_node_editor_nodes(node_editor_category_manager,log_panel,scene_manager);//*****
        }

private:
    
    void define_entity_db_categories(scene_entities_db_manager_class* scene_entities_db_manager,
                                     log_panel_class* log_panel,
                                     scene_manager_class* scene_manager);

    void define_render_object_categories(scene_render_objects_manager_class* render_objects_manager,
                                         log_panel_class* log_panel,
                                         scene_manager_class* scene_manager);

    void define_outliner_import_export_managers(outliner_import_export_manager_class* outliner_import_export_manager,
                                                log_panel_class* log_panel,
                                                scene_manager_class* scene_manager);

    void define_animation_manager_categories(animation_widget_class *animation_widget,
                                             log_panel_class* log_panel,
                                             scene_manager_class* scene_manager);

    void define_parameter_panel_widget_categories(parameters_widget_manager_class *parameters_widget_manager,
                                                  log_panel_class* log_panel,
                                                  scene_manager_class* scene_manager);

     //void define_node_editor_nodes(node_editor_category_manager_class* node_editor_category_manager,
     void define_node_editor_nodes(node_editor_category_manager_class* node_editor_category_manager,
                                     log_panel_class* log_panel,
                                     scene_manager_class* scene_manager)

    {
         if (node_editor_category_manager == NULL) {
             log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities.\n  Node editor category manager not defined\n");
             return;
         }

         if (scene_manager == NULL) {
             log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities.\n No Scene manager defined.\n");
             return;
         }

         // ********************************************************

         // Define voxel editor nodes db
         int index = node_editor_category_manager->add_editor_node_category(ENTITY_CATEGORY_HCP_VOXEL, log_panel);

         if (index < 0) {
             log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities\n");
             return;
         }

        if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP voxel editor nodes category\n");

        hcp_voxel_editor_node_class *voxel_hcp_voxel_editor_node = new hcp_voxel_editor_node_class;

        if (voxel_hcp_voxel_editor_node == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel entity node\n");
        }
        else {
//printf("voxel_hcp_scene_objects != NULL\n");
            voxel_hcp_voxel_editor_node->editor_node_definition.category          = ENTITY_CATEGORY_HCP_VOXEL;
            voxel_hcp_voxel_editor_node->editor_node_definition.context           = HCP_CONTEXT_VOXEL_MATRIX;
            voxel_hcp_voxel_editor_node->editor_node_definition.function          = HCP_VOXEL_FUNCTION_ENTITY;
            voxel_hcp_voxel_editor_node->editor_node_definition.function_category = FUNCTION_CATEGORY_GENERATOR;

            if (!node_editor_category_manager->add_editor_node(voxel_hcp_voxel_editor_node, scene_manager, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not add HCP voxel entity node\n");
           }
        }

        // ********************************************************
        hcp_voxel_editor_translation_node_class *translation_editor_node = new hcp_voxel_editor_translation_node_class;

        translation_editor_node->editor_node_definition.category          = ENTITY_CATEGORY_HCP_VOXEL;
        translation_editor_node->editor_node_definition.context           = HCP_CONTEXT_VOXEL_MATRIX;
        translation_editor_node->editor_node_definition.function          = HCP_VOXEL_FUNCTION_TRANSLATION;
        translation_editor_node->editor_node_definition.function_category = FUNCTION_CATEGORY_OPERATOR;

        if (translation_editor_node == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel translation node\n");
        }
        else {
//printf("voxel_hcp_scene_objects != NULL\n");
            if (!node_editor_category_manager->add_editor_node(translation_editor_node, scene_manager, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not add HCP voxel translation node\n");
            }
        }

        // ********************************************************
        hcp_voxel_editor_interaction_node_class *interaction_editor_node = new hcp_voxel_editor_interaction_node_class;

        interaction_editor_node->editor_node_definition.category          = ENTITY_CATEGORY_HCP_VOXEL;
        interaction_editor_node->editor_node_definition.context           = HCP_CONTEXT_VOXEL_MATRIX;
        interaction_editor_node->editor_node_definition.function          = HCP_VOXEL_FUNCTION_INTERACTION;
        interaction_editor_node->editor_node_definition.function_category = FUNCTION_CATEGORY_MODIFIER;

        if (interaction_editor_node == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel interaction node\n");
        }
        else {
//printf("voxel_hcp_scene_objects != NULL\n");
            if (!node_editor_category_manager->add_editor_node(interaction_editor_node, scene_manager, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not add HCP voxel interaction node\n");
            }
        }

    }

    
};