#pragma once

#include <Source/Editor/Scene/scene_entities_db_manager.h>
#include <Source/Editor/Scene/scene_render_object_manager.h>
#include <Source/Editor/Main_Window/Outliner/outliner_import_export_manager.h>
#include <Source/Editor/Main_Window/Widgets/animation_widget.h>
#include <Source/Editor/Main_Window/Widgets/parameters_widget.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

class hcp_voxel_defines_class {
public:

        void application_module_defines(scene_entities_db_manager_class      *scene_entities_db_manager,
                                        scene_render_objects_manager_class   *render_objects_manager,
                                        outliner_import_export_manager_class *outliner_import_export_manager,
                                        animation_widget_class               *animation_widget,
                                        parameters_widget_manager_class      *parameters_widget_manager,
                                        log_panel_class                      *log_panel,
                                        scene_manager_class                  *scene_manager) {

            define_entity_db_categories(scene_entities_db_manager,log_panel,scene_manager);
            define_render_object_categories(render_objects_manager,log_panel,scene_manager);
            define_outliner_import_export_managers(outliner_import_export_manager,log_panel,scene_manager);
            define_animation_manager_categories(animation_widget,log_panel,scene_manager);
            define_parameter_panel_widget_categories(parameters_widget_manager,log_panel,scene_manager);
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
};