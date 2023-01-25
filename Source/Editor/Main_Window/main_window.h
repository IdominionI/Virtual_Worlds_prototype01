#pragma once

#include "../Interface/theme.h"
#include "../Interface/IconsFontAwesome.h"
#include "../Common/global.h"

#include "../Scene/scene_manager.h"

#include <Graphics_Engine/OpenGL_window/ui_context.h>
#include <Graphics_Engine/OpenGL_window/openGL_context.h>
#include <Graphics_Engine/OpenGL_window/opengl_window_basis.h>
#include <Graphics_Engine/Viewer/scene_viewer.h>
#include <Graphics_Engine/Common/pch.h>
#include <Graphics_Engine/Common/user_interaction.h>

#include "Panels/property_panel.h"
#include "Panels/outliner_panel.h"
#include "Panels/parameter_panel.h"
#include "Panels/log_panel.h"

#include "Panels/node_editor_panel.h"

#include "../Node_editor/node_editor_category_manager.h"//*****

// ######## Include Module plugin defines  ################

#include <Source/Modules/Module_Hex_Surface/Editor/Plugin/hex_surface_defines.h>
#include <Source/Modules/Module_Voxel_Byte/Editor/Plugin/voxel_defines.h>

// ADD MORE Include Module plugin defines here ############

// ######## Include Module main menu fuctions  ################

#include <Source/Modules/Module_Voxel_Byte/Editor/Menus/main_window_menu_functions.h>
#include <Source/Modules/Module_Hex_Surface/Editor/Menus/hex_surface_main_menu_functions.h>

// ADD MORE Include Module plugin defines here ############

// ########################################################

// Testing only
//#include <Graphics_Engine/Shader/shader_format.h> 
//#include <Graphics_Engine/Shader/ShaderPreprocessor.h> 
//#include <Graphics_Engine/Shader/shader.h>
//#include <iostream>
//#include <filesystem>
//#include <cstring>
//#include <algorithm>
//#include <sstream>
// END Testing only

class main_window_class : public openGl_window_basis_class
{
public:

    main_window_class() : mIsRunning(true), GLFW_window_ptr(nullptr) {
        ui_context = std::make_unique<UI_context_class>();
        render_context = new openGL_context_class;
    }

    ~main_window_class() {
        ImNodes::DestroyContext();
        ui_context->end();
        render_context->end();

    }

    // Initialise application main window panels, variables, parameters etc.
    bool init(int width, int height, const std::string& title)
    {
        Width = width;
        Height = height;
        Title = title;

        render_context->init(this);

        ui_context->init(this);


        selected_node = NULL;

        globalc::set_current_selected_entity_id(-1);
        globalc::set_current_selected_entity_type_id(-1);

        ImNodes::CreateContext();

        log_panel = new log_panel_class();
        if (log_panel == NULL) {
            printf("CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n");
            return false;
        }

        scene_manager = new scene_manager_class;
        scene_manager->log_panel = log_panel;
        scene_graph_manager = &scene_manager->scene_graph_manager;

        // Define scene entity objects category data base
        scene_entities_db_manager = &scene_manager->entities_manager;

        // Define scene entity render objects category data base
        render_objects_manager = &scene_manager->render_objects_manager;

        outliner_panel.outliner_manager.scene_manager = scene_manager;

        // Define outliner import/export functions for each object category data base
        outliner_import_export_manager = &outliner_panel.outliner_manager.outliner_import_export_manager;

        parameter_panel.scene_manager = scene_manager;
        parameter_panel.log_panel = log_panel;

        scene_view = new scene_viewer_class(scene_graph_manager);

        //scene_manager = std::make_unique<scene_manager_class>(); // Just too problematic to work with



        property_panel.log_panel = log_panel;
        property_panel.openGL_context = render_context;
        property_panel.scene_viewer = scene_view;
        property_panel.scene_manager = scene_manager;
        property_panel.universal_shader_variables = scene_view->universal_shader_variables;

        node_editor_panel.log_panel = log_panel;
        node_editor_panel.scene_manager = scene_manager;
        node_editor_panel.nodes_context = ImNodes::GetCurrentContext();


        // ######## Begin Module plugin defines ################
        // ######## Must be placed here after all panels and managers 
        // ######## have been created and defined


        node_editor_category_manager = new node_editor_category_manager_class;//****

        hex_surface_defines.application_module_defines(scene_entities_db_manager,
                                                       render_objects_manager,
                                                       outliner_import_export_manager,
                                                       &property_panel.animation_widget,
                                                       &parameter_panel.parameters_widget_manager,
                                                       node_editor_category_manager, //****
                                                       log_panel,
                                                       scene_manager);

       hcp_voxel_defines.application_module_defines(scene_entities_db_manager,
                                                    render_objects_manager,
                                                    outliner_import_export_manager,
                                                    &property_panel.animation_widget,
                                                    &parameter_panel.parameters_widget_manager,
                                                    node_editor_category_manager, //****
                                                    log_panel,
                                                    scene_manager);

       node_editor_panel.scene_node_editor.node_editor_category_manager = node_editor_category_manager; //****

       // ADD OTHER MODULE PLUGIN DEFINES HERE

       // ######## End Module plugin defines ################
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // Set up the fonts file to use to display text 
        // and the awesome icon font file to display icons since ImGui is not designed
        // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        cfg.OversampleH = 3;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        cfg.GlyphExtraSpacing.x = 0.0f;
        cfg.GlyphExtraSpacing.y = 0.0f;
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);

        ImFont* iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        return mIsRunning;
    }

    // Display the application main window
    void render()
    {
        // Clear the view
        render_context->pre_render();

        // Initialize UI components
        ui_context->pre_render();

        // Define interface style parameters each render pass so as
        // to be able to change interface style or style components
        tron_style();

        // render scene to framebuffer and add it to scene view
        scene_view->render();

        property_panel.show(); // Display property panel with the selected entity property parameter values

        outliner_panel.show(log_panel); // Display outliner panel 

        //// Log panel to display application logging messages and shader code compile error messages
        log_panel->display_application_log();
        log_panel->display_code_log();

        parameter_panel.show(selected_node);

        node_editor_panel.show();
        selected_node = node_editor_panel.selected_node;

        // Create application main menu
        main_menu();

        // Following uncommented when investigating ImGui widget examples
        //ImGui::Begin;
        //ImGui::ShowDemoWindow();
        //ImGui::End;

        // Render the UI 
        ui_context->post_render();

        // Render end, swap buffers
        render_context->post_render();

        handle_input();
    }

    void main_menu() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File###mmfile")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Voxel###mmvoxel")) {
                if (ImGui::BeginMenu("Functions to surface###mmvfs")) {
                    if (ImGui::MenuItem("Selected###mmvfs")) { voxel_main_window_menu_functions.voxel_volume_to_voxel_surface(SELECTED_EXPORT,globalc::get_current_selected_entity_id(),scene_manager,log_panel); }
                    if (ImGui::MenuItem("Active###mmvfs")) { voxel_main_window_menu_functions.voxel_volume_to_voxel_surface(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                    if (ImGui::MenuItem("All###mmvfs")) { voxel_main_window_menu_functions.voxel_volume_to_voxel_surface(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Export###mmve")) {
                    if (ImGui::BeginMenu("As Point Cloud###mmvepc")) {
                        if (ImGui::MenuItem("Selected###mmvepc")) { voxel_main_window_menu_functions.export_voxels_center_point_data(SELECTED_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("Active###mmvepc")) { voxel_main_window_menu_functions.export_voxels_center_point_data(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("All###mmvepc")) { voxel_main_window_menu_functions.export_voxels_center_point_data(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Point Surface###mmveps")) {
                         if (ImGui::MenuItem("Selected###mmveps")) { voxel_main_window_menu_functions.export_voxels_point_surface_data(SELECTED_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("Active###mmveps")) { voxel_main_window_menu_functions.export_voxels_point_surface_data(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("All###mmveps")) { voxel_main_window_menu_functions.export_voxels_point_surface_data(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Face Surface###mmvefs")) {
                         if (ImGui::MenuItem("Selected###mmvefs")) { voxel_main_window_menu_functions.export_voxels_surface_face_data(SELECTED_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("Active###mmvefs")) { voxel_main_window_menu_functions.export_voxels_surface_face_data(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("All###mmvefs")) { voxel_main_window_menu_functions.export_voxels_surface_face_data(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Hex Surface###mmhex_surface")) {
                if (ImGui::BeginMenu("Export###mmve")) {
                    if (ImGui::BeginMenu("As Point Cloud###mmhepc")) {
                         if (ImGui::MenuItem("Selected###mmhepc")) { hex_surface_main_menu_functions.export_hex_surface_center_point_data(SELECTED_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("Active###mmhepc")) { hex_surface_main_menu_functions.export_hex_surface_center_point_data(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("All###mmhepc")) { hex_surface_main_menu_functions.export_hex_surface_center_point_data(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Face Surface###mmhefs")) {
                         if (ImGui::MenuItem("Selected###mmhefs")) { hex_surface_main_menu_functions.export_hex_surface_surface_face_data(SELECTED_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("Active###mmhefs")) { hex_surface_main_menu_functions.export_hex_surface_surface_face_data(ACTIVE_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        if (ImGui::MenuItem("All###mmhefs")) { hex_surface_main_menu_functions.export_hex_surface_surface_face_data(ALL_EXPORT, globalc::get_current_selected_entity_id(), scene_manager, log_panel); }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void handle_input()
    {
        double x, y;
        glfwGetCursorPos(GLFW_window_ptr, &x, &y);

        if (scene_view->focused) scene_view->on_mouse_move(float(x),float(y), user_interaction_class::GetPressedButton(GLFW_window_ptr));
    }

    void* get_native_window() override { return GLFW_window_ptr; }

    void set_native_window(void* window) {
        GLFW_window_ptr = (GLFWwindow*)window;
    }


    void on_scroll_event(float delta) override {
        if (scene_view->focused)  scene_view->on_mouse_wheel(delta);
    }

    void on_key_event(int key, int scancode, int action, int mods) override {
        if (scene_view->focused) scene_view->on_key_event(key, scancode, action, mods);
    }

    void on_mouse_button_event(int button, int state, int mods) {
        //printf("in on_mouse\n");
        if (scene_view->focused) scene_view->on_mouse_button_event(button, state, mods);
    }

    void on_resize_event(int width, int height) override
    {
        Width = width;
        Height = height;

        scene_view->resize(Width, Height);
        render();
    }

    void on_close_event() {
        mIsRunning = false;
    }

    bool is_running() { return mIsRunning; }

private:
    // Global variables

    node_basis_class* selected_node = NULL;

    GLFWwindow* GLFW_window_ptr; // glfw 

    // Render contexts
    std::unique_ptr<UI_context_class>     ui_context;
    openGL_context_class* render_context;

    // UI components
    property_panel_class  property_panel;
    outliner_panel_class  outliner_panel;
    parameter_panel_class parameter_panel;
    log_panel_class* log_panel;
    node_editor_panel_class node_editor_panel;

    scene_viewer_class* scene_view;

    bool mIsRunning;

    // *************
    scene_graph_manager_class* scene_graph_manager = NULL;
    scene_manager_class* scene_manager = NULL;

    scene_entities_db_manager_class* scene_entities_db_manager;
    scene_render_objects_manager_class* render_objects_manager;

    outliner_import_export_manager_class* outliner_import_export_manager;

    universal_shader_variables_struct_type* universal_shader_variables = NULL;

    // ####### Modules Defines #######
    hex_surface_defines_class hex_surface_defines;
    hcp_voxel_defines_class   hcp_voxel_defines;
    
    // add more modules defiens here
    // ###############################

    // %%%%%%% Module Main menu function classes %%%%%%%%%

    voxel_main_window_menu_functions_class voxel_main_window_menu_functions;
    hex_surface_main_menu_functions_class  hex_surface_main_menu_functions;

    // add more module main menu function classes here
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    //--------- Testing only -------------
    //mesh_class *mesh;
    node_editor_category_manager_class *node_editor_category_manager;//*****
};
