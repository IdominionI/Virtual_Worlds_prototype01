#pragma once

#include <Source/Graphics_Engine/Common/pch.h>
#include <Source/Graphics_Engine/Viewer/scene_viewer.h>

#include "log_panel.h"

#include "../Widgets/animation_widget.h"
#include "../Widgets/camera_properties_wiget.h"
#include "../Widgets/lighting_properties_widget.h"
#include "../Widgets/viewport_properties_widget.h"

#include <Universal/ImGui/imgui.h>
#include "../Widgets/imgui_widgets.h"

/*
                            PARAMETER PANEL

    Parent class to define the application property panel that is the parent
    "window widget" for the display of the application scene lighting, camera
    and viewer display paramters that are used to display the graphics onto
    the computer screen. This panel can be detached from the main window according
    to the docking features of the ImGui framework.

    It is also here that an animation widget to perform basic interactive animations
    is added.
*/


  class property_panel_class
  {
  public:

  property_panel_class() {}
  ~property_panel_class() {}

    universal_shader_variables_struct_type *universal_shader_variables = NULL;

    log_panel_class      *log_panel      = NULL;
    openGL_context_class *openGL_context = NULL;
    scene_viewer_class   *scene_viewer   = NULL;

    scene_manager_class *scene_manager = NULL;

    void show() {
//printf("property_panel_class :: display 000\n");

//if (current_selected_object_type_id == NULL || current_selected_object_id == NULL) {
//    if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Property panel :No Entity variable defined \n");
//    return;
//}

        if (scene_viewer == NULL) {
            if (log_panel) log_panel->application_log.AddLog("ERROR: Cannot display property panel of allplication. No scene viewer defined.\n");
            return;
        }

        if (openGL_context == NULL) {
            if (log_panel) log_panel->application_log.AddLog("ERROR: Cannot display property panel of allplication. No openGL context defined.\n");
            return;
        }

        if (universal_shader_variables == NULL) {
            if (log_panel) log_panel->application_log.AddLog("ERROR: Cannot display property panel of allplication. No universal shader variable defined.\n");
            return;
        }

        ImGui::Begin("Scene Properties"); // Must be present for any class definition to have a dockable window

        if (ImGui::BeginTabBar("##hcp_voxel_tabs", ImGuiTabBarFlags_None)){
            if (ImGui::BeginTabItem("Lighting##lighting_tab")){
                display_lighting_widget();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Camera##camera_tab")) {
                display_camera_widget();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Viewer##viewer_tab")){
                display_viewer_widget();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Animation##animation_tab")) {
                display_animation_widget();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    animation_widget_class           animation_widget;


  private:
      camera_poperties_widget_class    camera_poperties_widget;
      lighting_properties_widget_class lighting_properties_widget;
      viewport_properties_widget_class viewport_properties_widget;

      void display_lighting_widget() {
          lighting_properties_widget.display(universal_shader_variables);
      }

      void display_camera_widget() {
          camera_poperties_widget.display(universal_shader_variables);
      }

      void display_viewer_widget() {
          viewport_properties_widget.scene_viewer = scene_viewer;
          viewport_properties_widget.display(openGL_context);
      }

      void display_animation_widget() {
          animation_widget.scene_manager                   = scene_manager;
          animation_widget.current_selected_object_id      = globalc::get_current_selected_entity_id();
          animation_widget.current_selected_object_type_id = globalc::get_current_selected_entity_type_id();

          animation_widget.display();
      }
  };


