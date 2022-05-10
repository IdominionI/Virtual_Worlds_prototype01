#pragma once

#include "../Common/imgui_custom.h"

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

//#include <Source/Graphics_Engine/Geometry/mesh.h> // Testing only

#include <Source/Modules/Module_Voxel_Byte/Functions/vw_voxel_exports.h>
#include <Source/Modules/Module_hex_surface/Functions/hex_surface_exports.h>


class main_window_class : public openGl_window_basis_class
{
public:

    main_window_class() : mIsRunning(true), GLFW_window_ptr(nullptr) {
        ui_context     = std::make_unique<UI_context_class>();
        render_context = new openGL_context_class;
    }

    ~main_window_class() {
        ui_context->end();
        render_context->end();
    }

    bool init(int width, int height, const std::string& title)
    {
        Width = width;
        Height = height;
        Title = title;

        render_context->init(this);

        ui_context->init(this);

        log_panel = new log_panel_class();
        if (log_panel == NULL) {
            printf("CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n");
            return false;
        }

        scene_manager            = new scene_manager_class;
        scene_manager->log_panel = log_panel;
        scene_graph_manager = &scene_manager->scene_graph_manager;

        outliner_panel.outliner_manager.scene_manager = scene_manager;

        parameter_panel.scene_manager = scene_manager;
        parameter_panel.log_panel     = log_panel;

        scene_view          = new scene_viewer_class(scene_graph_manager);

        //scene_manager = std::make_unique<scene_manager_class>(); // Just too problematic to work with
/*
        // ***
        // Test render object(s)
            if (!scene_graph_manager->add_scene_entity_render_object(0)) {
                printf("main window : not created\n");
               
            }else {
                printf("main window : created\n");

                scene_node_class <render_object_class> *test_mesh = scene_graph_manager->get_scene_entity_render_object(0);
                if (test_mesh == NULL) {
                    printf("main window : test_mesh == NULL\n");
                }
                else {
                    printf("main window : test_mesh != NULL\n");
                    mesh = new mesh_class;
                    test_mesh->scene_graph_object.scene_object_class.geometry = mesh;
                   // test_mesh->scene_graph_object.scene_object_class.universal_shader_variables = scene_view->universal_shader_variables;
                    // define shaders and dynamic shader variables here
                    shader_parameter_variable_struct_type v1, v2, v3;
                    v1.variable_name = "roughness"; v1.value = 0.2f;
                    v2.variable_name = "metallic"; v2.value = 0.1f;
                    v3.variable_name = "ao"; v3.value = 1.0f;
                    test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v1);
                    test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v2);
                    test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v3);

                    if (!test_mesh->scene_graph_object.scene_object_class.geometry->load("cube.fbx",0.0))
                        printf("main window : mesh not loaded\n");
                    else {

                        printf("main window : mesh loaded %i\n", test_mesh->scene_graph_object.scene_object_class.geometry->number_vertices);
                        printf("main window : mesh loadedAAA %i\n", test_mesh->scene_graph_object.scene_object_class.geometry->vertex_indices.size());

                        test_mesh->scene_graph_object.scene_object_class.geometry->geometry_type = geometry_type_enum::triangles;

                        int shader_id = shader_db_manager.shader.create_shader_program("shaders/vs.shader", "", "shaders/fs_pbr.shader");
                        if (shader_id < 0)
                            printf("main window : shader program not created\n");
                        else {
                            printf("main window : shader program created : %i\n", shader_id);

                            test_mesh->scene_graph_object.scene_object_class.shader_material.shader_program_id = shader_id;
                        }//shader program created
                    }//mesh loaded
                }//test_mesh != NULL
            }
*/
        // ***
/*
            if (!scene_graph_manager->add_scene_entity_render_object(1)) {
                printf("main window2 : not created\n");

            }
            else {
                printf("main window2 : created\n");

                scene_node_class <render_object_class> *test_mesh2 = scene_graph_manager->get_scene_entity_render_object(1);
                if (test_mesh2 == NULL) {
                    printf("main window2 : test_mesh == NULL\n");
                }
                else {
                    printf("main window 2: test_mesh != NULL\n");
                    mesh = new mesh_class;
                    test_mesh2->scene_graph_object.scene_object_class.geometry = mesh;
                   // test_mesh2->scene_graph_object.scene_object_class.universal_shader_variables = scene_view->universal_shader_variables;
                    // define shaders and dynamic shader variables here
                    shader_parameter_variable_struct_type v1, v2, v3;
                    v1.variable_name = "roughness"; v1.value = 0.2f;
                    v2.variable_name = "metallic"; v2.value = 1.0f;
                    v3.variable_name = "ao"; v3.value = 1.0f;
                    test_mesh2->scene_graph_object.scene_object_class.shader_material.variables.push_back(v1);
                    test_mesh2->scene_graph_object.scene_object_class.shader_material.variables.push_back(v2);
                    test_mesh2->scene_graph_object.scene_object_class.shader_material.variables.push_back(v3);

                    if (!test_mesh2->scene_graph_object.scene_object_class.geometry->load("cube.fbx",2.0f))
                        printf("main window2 : mesh not loaded\n");
                    else {

                        printf("main window2 : mesh loaded %i\n", test_mesh2->scene_graph_object.scene_object_class.geometry->number_vertices);
                        printf("main window2 : mesh loadedAAA %i\n", test_mesh2->scene_graph_object.scene_object_class.geometry->vertex_indices.size());

                        test_mesh2->scene_graph_object.scene_object_class.geometry->geometry_type = geometry_type_enum::triangles;

                        int shader_id = shader_db_manager.shader.create_shader_program("shaders/vs.shader", "", "shaders/fs_pbr.shader");
                        if (shader_id < 0)
                            printf("main window2 : shader program not created\n");
                        else {
                            printf("main window2 : shader program created : %i\n", shader_id);

                            test_mesh2->scene_graph_object.scene_object_class.shader_material.shader_program_id = shader_id;
                        }//shader program created
                    }//mesh loaded
                }//test_mesh != NULL
            }
*/

        property_panel.log_panel                  = log_panel;
        property_panel.openGL_context             = render_context;
        property_panel.scene_viewer               = scene_view;
        property_panel.scene_manager              = scene_manager;
        property_panel.universal_shader_variables = scene_view->universal_shader_variables;// ****

        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // Set up the fonts file to use to display text 
        // and the awesome icon font file to display icons since ImGui is not designed
        // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        //memset(&cfg, 0, sizeof(ImFontConfig));
        cfg.OversampleH = 3.;
        cfg.OversampleV = 1.;
        cfg.PixelSnapH = true;
        cfg.GlyphExtraSpacing.x = 0.0f;
        cfg.GlyphExtraSpacing.y = 0.0f;
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont *mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);

        ImFont *iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        return mIsRunning;
    }

    void render()
    {
        // Clear the view
        render_context->pre_render();

        // Initialize UI components
        ui_context->pre_render();

        tron_style();

        // render scene to framebuffer and add it to scene view
        scene_view->render();// to be replaced with scene manager render

        property_panel.current_selected_object_type_id = outliner_panel.outliner_manager.current_selected_entity_type_id;// to be as a function parameter ??
        property_panel.current_selected_object_id      = outliner_panel.outliner_manager.current_selected_node_id;// to be as a function parameter ??
        property_panel.show();

        outliner_panel.show(log_panel);

        log_panel->display_application_log();
        log_panel->display_code_log();

        parameter_panel.current_selected_object_type_id = outliner_panel.outliner_manager.current_selected_entity_type_id;// to be as a function parameter ??
        parameter_panel.current_selected_object_id      = outliner_panel.outliner_manager.current_selected_node_id;// to be as a function parameter ??
        parameter_panel.show();

        main_menu();

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
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File###mmfile")){
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Voxel###mmvoxel")){
               if (ImGui::BeginMenu("Functions to surface###mmvfs")) {
                        if (ImGui::MenuItem("Selected###mmvfs")) { voxel_volume_to_voxel_surface(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmvfs"))   { voxel_volume_to_voxel_surface(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmvfs"))      { voxel_volume_to_voxel_surface(ALL_EXPORT); }
                    ImGui::EndMenu();
                }
                 if (ImGui::BeginMenu("Export###mmve")) {
                    if (ImGui::BeginMenu("As Point Cloud###mmvepc")) {
                        if (ImGui::MenuItem("Selected###mmvepc")) { export_voxels_center_point_data(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmvepc"))   { export_voxels_center_point_data(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmvepc"))      { export_voxels_center_point_data(ALL_EXPORT); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Point Surface###mmveps")) {
                        if (ImGui::MenuItem("Selected###mmveps")) { export_voxels_point_surface_data(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmveps"))   { export_voxels_point_surface_data(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmveps"))      { export_voxels_point_surface_data(ALL_EXPORT); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Face Surface###mmvefs")) {
                        if (ImGui::MenuItem("Selected###mmvefs")) { export_voxels_surface_face_data(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmvefs"))   { export_voxels_surface_face_data(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmvefs"))      { export_voxels_surface_face_data(ALL_EXPORT); }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Hex Surface###mmhex_surface")) {
                if (ImGui::BeginMenu("Export###mmve")) {
                    if (ImGui::BeginMenu("As Point Cloud###mmhepc")) {
                        if (ImGui::MenuItem("Selected###mmhepc")) { export_hex_surface_center_point_data(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmhepc"))   { export_hex_surface_center_point_data(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmhepc"))      { export_hex_surface_center_point_data(ALL_EXPORT); }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("As Face Surface###mmhefs")) {
                        if (ImGui::MenuItem("Selected###mmhefs")) { export_hex_surface_surface_face_data(SELECTED_EXPORT); }
                        if (ImGui::MenuItem("Active###mmhefs"))   { export_hex_surface_surface_face_data(ACTIVE_EXPORT); }
                        if (ImGui::MenuItem("All###mmhefs"))      { export_hex_surface_surface_face_data(ALL_EXPORT); }
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

        if (scene_view->focused) scene_view->on_mouse_move(x, y, user_interaction_class::GetPressedButton(GLFW_window_ptr));
    }

    void* get_native_window() override { return GLFW_window_ptr; }

    void set_native_window(void* window) {
        GLFW_window_ptr = (GLFWwindow*)window;
    }


    void on_scroll(float delta) override{
        if(scene_view->focused)  scene_view->on_mouse_wheel(delta);
    }

    void on_key(int key, int scancode, int action, int mods) override {
        if (scene_view->focused) scene_view->on_key_event(key,scancode,action,mods);
    }

    void on_resize(int width, int height) override
    {
        Width = width;
        Height = height;

        scene_view->resize(Width, Height);
        render();
    }

    void on_close() {
        mIsRunning = false;
    }

    bool is_running() { return mIsRunning; }

private:
    GLFWwindow* GLFW_window_ptr; // glfw 

    // Render contexts
    std::unique_ptr<UI_context_class>     ui_context;
    openGL_context_class  *render_context;

    // UI components
    property_panel_class  property_panel;
    outliner_panel_class  outliner_panel;
    parameter_panel_class parameter_panel;
    log_panel_class       *log_panel;

    scene_viewer_class   *scene_view;

    bool mIsRunning;

    // *************
    scene_graph_manager_class *scene_graph_manager = NULL;
    scene_manager_class       *scene_manager       = NULL;

    universal_shader_variables_struct_type *universal_shader_variables = NULL;

    export_voxel_geometry_class       export_voxel_geometry;
    export_hex_surface_geometry_class export_hex_surface_geometry;

    //--------- Testing only -------------
    mesh_class *mesh;

    shader_db_manager_class shader_db_manager;

    // ################### Voxel specifics ####################

    voxel_hcp_render_class  voxel_hcp_render;

    void voxel_volume_to_voxel_surface(int selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel surface geometry :: Cannot perform voxel volume to surface function:: Scene Manager is undefined\n");
            return;
        }

        voxel_hcp_scene_objects_class & voxel_hcp_objects = scene_manager->entities_manager.voxel_hcp_scene_objects;

        if (voxel_hcp_objects.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel surface geometry :: Cannot perform voxel volume to surface function:: No entity data in scene to export\n");
            vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
            return ;
        }

        voxel_object_data_class    voxel_object_surface_data;

        switch (selection) {
			case SELECTED_EXPORT : voxel_center_points_selected(voxel_hcp_objects, outliner_panel.outliner_manager.current_selected_node_id);break;
			case ACTIVE_EXPORT	 : voxel_center_points_active(voxel_hcp_objects);  break;
			case ALL_EXPORT      : voxel_center_points_all(voxel_hcp_objects);	   break;

		}
    }

    void voxel_center_points_selected(voxel_hcp_scene_objects_class &voxel_hcp_entities, id_type entity_id) {

        if (entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity selected to export\n");
            vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
            return ;
        }

        voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_entities.get_voxel_hcp_object(entity_id);

        if (voxel_hcp_object == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Could not find entity in scene data to export\n");
            vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            return ;
        }

        create_voxel_center_point_data(voxel_hcp_object,entity_id);

//printf("voxel_center_points_ply_selected 44 %i\n", entity_id);
    }

    void voxel_center_points_active(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
        for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
            voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_entities_to_export[i];

            if (voxel_hcp_object == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No voxel entities defined\n");
                vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            }

            if (voxel_hcp_object->active_object) {
                if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() > 0) {
                    create_voxel_center_point_data(voxel_hcp_object, voxel_hcp_object->object_id);
                }
                else {
                    if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->object_name);
                }
            }
        }
    }

    void voxel_center_points_all(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export) {
        for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
            voxel_hcp_object_class *voxel_hcp_object = voxel_hcp_entities_to_export[i];

            if (voxel_hcp_object == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No voxel entities defined\n");
                vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            }

            if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() > 0) {
                create_voxel_center_point_data(voxel_hcp_object, voxel_hcp_object->object_id);
            } else {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->object_name);
            }
            
        }
    }

    void create_voxel_center_point_data(voxel_hcp_object_class *voxel_hcp_object, id_type entity_id) {
        voxel_surface_data_class voxel_surface_data;
        voxel_surface_data.cloud = voxel_hcp_object;
//printf("create_voxel_center_point_data 00\n");


        if (!voxel_surface_data.create_voxel_surface_point_data()) {
//printf("create_voxel_center_point_data 11\n");
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->object_name);
        }

        if (voxel_surface_data.surface_vertices.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel vertex data\n", voxel_hcp_object->object_name);
            return;
        }

        if (voxel_surface_data.surface_vertices_normals.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel normal data\n", voxel_hcp_object->object_name);
            return;
        }
//printf("create_voxel_center_point_data 22\n");
        voxel_object_data_class    voxel_object_surface_data;
        voxel_object_surface_data.create_empty_volume_cubic(voxel_hcp_object->voxel_object_data.matrix_dimension.x, voxel_hcp_object->voxel_object_data.matrix_dimension.y, voxel_hcp_object->voxel_object_data.matrix_dimension.z);

        for (int i = 0; i < voxel_surface_data.surface_vertices.size(); i++) {
            voxel_object_surface_data.voxel_matrix_data[voxel_surface_data.surface_vertices[i].voxel_index] = voxel_hcp_object->voxel_object_data.voxel_matrix_data[voxel_surface_data.surface_vertices[i].voxel_index];
        }

        voxel_hcp_object->voxel_object_data.voxel_matrix_data.clear();
        voxel_hcp_object->voxel_object_data.voxel_matrix_data.shrink_to_fit();

        voxel_hcp_object->voxel_object_data.voxel_matrix_data = voxel_object_surface_data.voxel_matrix_data;

        voxel_hcp_object->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display

        //####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
        scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(entity_id);
//printf("create_voxel_center_point_data 33\n");

        if (scene_voxel_object == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find voxel in the scene to update geometry data.\n");
//printf("scene_voxel_object == NULL.\n");
        }
        else {
//printf("scene_voxel_object != NULL.\n");

            if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object->point_cloud, scene_voxel_object, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated.\n");
//printf("scene_voxel_object not updated\n");
                return;
            }

        }
        //########################################################################
//printf("create_voxel_center_point_data 44\n");
    }

    void export_voxels_center_point_data(int export_selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point dat to file:: Scene Manager is undefined\n");
        }

        export_voxel_geometry.export_voxel_center_points_ply(scene_manager->entities_manager.voxel_hcp_scene_objects,export_selection,outliner_panel.outliner_manager.current_selected_node_id);
    }

    void  export_voxels_point_surface_data(int export_selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Scene Manager is undefined\n");
        }

        export_voxel_geometry.export_voxel_point_surface_data_ply(scene_manager->entities_manager.voxel_hcp_scene_objects, export_selection, outliner_panel.outliner_manager.current_selected_node_id);
    }

    void  export_voxels_surface_face_data(int export_selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Scene Manager is undefined\n");
        }

        export_voxel_geometry.export_voxel_surface_faces_data_ply(scene_manager->entities_manager.voxel_hcp_scene_objects, export_selection, outliner_panel.outliner_manager.current_selected_node_id);
    }


    // ################### Hex Surface specifics ####################

    void export_hex_surface_center_point_data(int export_selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface point geometry :: Cannot export hex surface point dat to file:: Scene Manager is undefined\n");
        }

        export_hex_surface_geometry.export_hex_surface_center_points_ply(scene_manager->entities_manager.hex_surface_scene_objects, export_selection, outliner_panel.outliner_manager.current_selected_node_id);
    }

    void export_hex_surface_surface_face_data(int export_selection) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface surface geometry :: Cannot export hex surface surface dat to file:: Scene Manager is undefined\n");
        }

        export_hex_surface_geometry.export_hex_surface_faces_ply(scene_manager->entities_manager.hex_surface_scene_objects, export_selection, outliner_panel.outliner_manager.current_selected_node_id);
    }

};
