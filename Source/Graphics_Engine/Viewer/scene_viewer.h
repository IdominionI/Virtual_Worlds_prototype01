#pragma once

#include <Universal/ImGui/imgui_internal.h> // **** required to draw crosshairs
#include <Universal/ImGui/imgui.h>

#include "../Render/Buffers/frame_buffer.h"
#include "../Common/user_interaction.h"

#include "../Scene/scene_graph_manager.h"

#include "Bindings/user_bindings.h"

#include "Extensions/viewer_grid.h"

#include <Universal/imGuIZMO_quat/imGuIZMOquat.h>
#include <Universal/ImGuizmo/ImGuizmo.h>

#include "../Scene/Scene_objects/bounding_volume_object.h"

// !!!! THIS IS THE SAME CLASS AS THE vw_viewer class in Virtual Worlds QT !!!!
// !!!! TO ACHEIVE THE SAME FUCNTIONALITY TO DRAW AND DISPLAY ALL OPENGL   !!!!
// !!!! RENDERING FUNCTIONS AND CALLS.

/*
                   Scene Viewer Class

    The Scene viewport Class that displays and renders the scene as
    defined by the camera class object.

    Within this scene viewer class the user intereacts with the scene
    through the mouse and keyboard controls. Additional gpahic overlays
    of cross hairs, camera information text and a navigational gimbal
    is displayed through this class.

    The navigational Gimbal is a small ImGui tool obtained from github
    called imGuIZMO by Cedric Guillemet.

    Note : class not complete and will have future updates applied.

    This is a heavily modified and altered version of the viewport
    rendering code that was in the JGL Demo project from which formed
    the basis of this application.
*/


class scene_viewer_class
{
public:
    bool focused = false;
    bool display_crosshairs  = true;
    bool display_camera_info = true;
    bool display_gimbal      = true;
    bool display_grid        = true;

    camera_object_class *camera = NULL;
    viewer_grid_class     viewer_grid;

    glm::vec4 xhair_color = glm::vec4{ 1.0f,1.0f,1.0f,1.0f };

    bounding_volume_class bounding_volume ;// = new bounding_volume_class;// Testing only
   
    scene_viewer_class(scene_graph_manager_class *_scene_manager) : frame_buffer(nullptr){
       frame_buffer = std::make_unique<openGL_frame_buffer_class>();
       frame_buffer->create_buffers(1700, 1000);
       camera        = new camera_object_class;
       //default_light = new light_object_class;
       scene_lights = new scene_lights_objects_class;

       universal_shader_variables = new universal_shader_variables_struct_type;
       universal_shader_variables->camera = camera;
       universal_shader_variables->scene_lights = scene_lights;

       scene_graph_manager = _scene_manager;

       // Initialise key bindings
       user_bindings.set_default_key_press_bindings();
       user_bindings.set_default_mouse_bindings();

       // Define a grid axis to be displayed
       viewer_grid.scene_graph_manager = scene_graph_manager;
       viewer_grid.initialise();

       // Testing only **********
  /*     if (!bounding_volume.initialise())
           printf("!bounding_volume.initialise()");
       else {
           scene_graph_manager->add_scene_entity_render_object(1000);
           scene_node_class <render_object_class> *box_object = scene_graph_manager->get_scene_entity_render_object(1000);

           if (box_object == NULL) {
               //if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find voxel in the scene to update geometry data.\n");
               printf("box_object == NULL.\n");
           }
           else {
               printf("box_object != NULL.\n");
               line_class *bv = new line_class;// Testing only

               bounding_volume.geometry = bv;
               
               box_object->scene_graph_object.scene_object_class.geometry = bounding_volume.geometry;
               box_object->scene_graph_object.scene_object_class.geometry->init();
               box_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = bounding_volume.program_id;

               bounding_volume.update_limits(glm::vec3(-4.0, -2.5, -5.0), glm::vec3(1.0, 3, 2.0));
           }
       }
       // end Testing only **********
*/
    }

    ~scene_viewer_class()
    {
    }

    scene_graph_manager_class              *scene_graph_manager  = NULL;
    universal_shader_variables_struct_type *universal_shader_variables = NULL;

    quat qRot = quat(1.f, 0.f, 0.f, 0.f);

    void resize(int32_t width, int32_t height) {
        size.x = width;
        size.y = height;

       frame_buffer->create_buffers((int32_t)size.x, (int32_t)size.y);
    }

    void render() {
       pre_draw();
       draw();
       post_draw();
    }

    virtual void pre_draw() {
    }

    void draw() {
        frame_buffer->bind(); // tHIS MAY BE SET IN EACH RENDER OBJECT

        // *********** THIS IS WHERE THE SCENE GRAPH IS CALLED TO RENDER ALL RENDABLE OBJECTS *************

        scene_graph_manager->render_scene(universal_shader_variables);
       // if(bounding_volume.program_id > -1)    bounding_volume.draw_bbox();
       // bounding_volume.draw_bbox();
        

        frame_buffer->unbind(); // tHIS MAY BE SET IN EACH RENDER OBJECT
        //bounding_volume->draw_bbox();

        ImGui::Begin("Scene");

        if (ImGui::IsWindowHovered()) focused = true; else focused = false;

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        size = { viewportPanelSize.x, viewportPanelSize.y };

        camera->set_aspect(size.x / size.y);

        ImGuiContext* g = ImGui::GetCurrentContext();
        ImGuiWindow* window = g->CurrentWindow;

        if (display_crosshairs) {// Draw crosshairs at center of viewport 
            auto draw = ImGui::GetForegroundDrawList(window->Viewport);

            ImVec2 p1 = { window->DC.CursorPos.x + size.x / 2 - 7, window->DC.CursorPos.y + size.y / 2 };
            ImVec2 p2 = { window->DC.CursorPos.x + size.x / 2 + 7, window->DC.CursorPos.y + size.y / 2 };
            ImVec2 p3 = { window->DC.CursorPos.x + size.x / 2,     window->DC.CursorPos.y + size.y / 2 - 7 };
            ImVec2 p4 = { window->DC.CursorPos.x + size.x / 2,     window->DC.CursorPos.y + size.y / 2 + 7 };

            draw->AddLine(p1, p2, IM_COL32((int)(xhair_color.r*255), (int)(xhair_color.g * 255), (int)(xhair_color.b * 255), (int)(xhair_color.a * 255)), 1.0f);
            draw->AddLine(p3, p4, IM_COL32((int)(xhair_color.r*255), (int)(xhair_color.g * 255), (int)(xhair_color.b * 255), (int)(xhair_color.a * 255)), 1.0f);
        }

        //draw->AddCircle(ImVec2(1920 / 2, 1080 / 2), 6, IM_COL32(255, 0, 0, 255), 100, 0.0f);
        
        if (display_camera_info) {// Display an overlay on viewport of camera informantion
            ImVec2 p1 = { window->DC.CursorPos.x + 5, window->DC.CursorPos.y + size.y - 110 };
            ImGui::SetNextWindowPos(p1, ImGuiCond_Always, { 0.0f,0.0f });



            ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
            if (ImGui::Begin("cam_info###vci", NULL, overlay_window_flags))
            {
                float y_pos = 5.0;
                
                //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32((int)(info_color.x * 255), (int)(info_color.y * 255), (int)(info_color.z * 255), (int)(info_color.w * 255)));
                
                ImGui::SetCursorPosY(y_pos);
                ImGui::Text("X_coord : %f", camera->mPosition.x);
                y_pos += 15.0; ImGui::SetCursorPosY(y_pos);
                ImGui::Text("Y_coord : %f", camera->mPosition.y);
                y_pos += 15.0; ImGui::SetCursorPosY(y_pos);
                ImGui::Text("Z_coord : %f", camera->mPosition.z);
                y_pos += 15.0; ImGui::SetCursorPosY(y_pos);
                ImGui::Text("Pitch   : %f", glm::degrees(camera->mPitch));
                y_pos += 15.0; ImGui::SetCursorPosY(y_pos);
                ImGui::Text("Yaw     : %f", glm::degrees(camera->mYaw));
                y_pos += 15.0; ImGui::SetCursorPosY(y_pos);
                ImGui::Text("Roll    : %f", glm::degrees(camera->mRoll));

                //ImGui::PopStyleColor();
            }
            ImGui::End();
        }

        if (display_gimbal) {// Display an overlay on viewport of navigation gimbal
             ImVec2 p2 = { window->DC.CursorPos.x + size.x / 2.0f - 50.0f , window->DC.CursorPos.y + size.y - 120.0f };
            ImGui::SetNextWindowPos(p2, ImGuiCond_Always, { 0.0f,0.0f });
            ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
            if (ImGui::Begin("gizmo###vgiz", NULL, overlay_window_flags))
            {
                if (ImGui::gizmo3D("##gizmo1", qRot, 100.0, imguiGizmo::mode3Axes)) {
                    camera->set_orientation(qRot);
                }else
                    qRot = camera->get_orientation();
            }
            ImGui::End();
        }

        // add rendered texture to ImGUI scene window
        uint64_t textureID = frame_buffer->get_texture();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
    }

    virtual void post_draw() {

    }


    void on_mouse_move(float x, float y, mouse_button_enum button) {
       camera->on_mouse_move(x, y, button);
    }

    void on_mouse_wheel(float delta) {
       camera->on_mouse_wheel(delta);
    }

    void on_key_event(int key, int scancode, int action, int mods) {
//printf("on_key_event\n");
        switch (action) {
            case GLFW_PRESS   : keyPressEvent  (key, scancode,mods);break;
            case GLFW_REPEAT  : keyPressEvent  (key, scancode,mods);break;
            case GLFW_RELEASE : keyReleaseEvent(key, scancode,mods);break;

        }
    }

    	// ####################### EVENTS SECTION ##############################
	// The default actions and processes of user initiated events of interaction through keyboard and mouse inputs

	virtual void keyPressEvent(int key, int scancode, int mods) {
//printf("keyPressEvent\n");

		for (key_event_binding_struct_type key_binding : user_bindings.press_key_events) {
//if(key_binding.id == ACTION_TYPE_ROLL_LEFT){
 //   printf("key bindings : %i : % i : %i : %i \n", key_binding.key, key, key_binding.modifiers, mods);
//}

			if (key_binding.event_state == event_state_enum::enabled && key_binding.event_type == key_event_type_enum::press &&
				key_binding.key == key && key_binding.modifiers == mods) {
				perform_key_action(key_binding);
				//update();
				break;
			}
		}
	}

	virtual void keyReleaseEvent(int key, int scancode, int mods) {
//printf("keyReleaseEvent\n");
		for (key_event_binding_struct_type key_binding : user_bindings.press_key_events) {
			if (key_binding.event_state == event_state_enum::enabled && key_binding.event_type == key_event_type_enum::release &&
				key_binding.key == key && key_binding.modifiers == mods) {
				perform_key_action(key_binding);
				//update();
				break;
			}
		}
	}

	virtual void perform_key_action(key_event_binding_struct_type key_binding) {
//printf("perform_key_action : %i :: %i\n", key_binding.id, ACTION_TYPE_ROLL_LEFT);

		switch (key_binding.id) {
		    //case ACTION_TYPE_EXIT_VIEWER: save_viewer_state(); close(); exit(0); break;
		    //case ACTION_TYPE_TOGGLE_FIRST_PERSON: toggle_first_person_mode();            break;

		    // movement this needs to be decided what are the plane space
		    case ACTION_TYPE_MOVEMENT_FORWARD:  if (!(camera->movement == camera_movement_mode_enum::xz_plane)) camera->move_forward();  break;
		    case ACTION_TYPE_MOVEMENT_BACKWARD: if (!(camera->movement == camera_movement_mode_enum::xz_plane)) camera->move_backward(); break;
		    case ACTION_TYPE_MOVEMENT_RIGHT:    if (!(camera->movement == camera_movement_mode_enum::yz_plane)) camera->move_right();    break;
		    case ACTION_TYPE_MOVEMENT_LEFT:     if (!(camera->movement == camera_movement_mode_enum::yz_plane)) camera->move_left();     break;
		    case ACTION_TYPE_MOVEMENT_UP:       if (!(camera->movement == camera_movement_mode_enum::xy_plane)) camera->move_up();       break;
		    case ACTION_TYPE_MOVEMENT_DOWN:     if (!(camera->movement == camera_movement_mode_enum::xy_plane)) camera->move_down();     break;

		    case ACTION_TYPE_ROLL_LEFT:         if (camera->roll_rotation) camera->roll_camera(0.5); break;
		    case ACTION_TYPE_ROLL_RIGHT:		if (camera->roll_rotation) camera->roll_camera(-0.5); break;

			// interface tools ::: TO DO ???? 
		    //case ACTION_TYPE_DRAW_AXIS:           toggle_display_axis();        break;
		    //case ACTION_TYPE_DRAW_GRID:           toggle_display_grid();	    break;
		    //case ACTION_TYPE_DRAW_XY_GRID:        toggle_display_xy_grid();     break;
		    //case ACTION_TYPE_DRAW_XZ_GRID:        toggle_display_xz_grid();     break;
		    //case ACTION_TYPE_DRAW_YZ_GRID:        toggle_display_yz_grid();     break;
		    //case ACTION_TYPE_DISPLAY_FPS:         toggle_FPS_display();	        break;
		    //case ACTION_TYPE_DISPLAY_CAMERA_INFO: toggle_camera_info_display(); break;
		    //case ACTION_TYPE_DISPLAY_CROSS_HAIRS: toggle_cross_hairs_display(); break;
		    //case ACTION_TYPE_ENABLE_TEXT:         toggle_enable_all_text();     break;
		    //case ACTION_TYPE_SAVE_SCREENSHOT:     save_snapShot();              break;

			    // Camera alignment
		    case ACTION_TYPE_CAMERA_ALIGN_XY_PLANE: camera->align_to_plane(camera_ailgnment_type_enum::xy_plane); break;
		    case ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE: camera->align_to_plane(camera_ailgnment_type_enum::xz_plane); break;
		    case ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE: camera->align_to_plane(camera_ailgnment_type_enum::yz_plane); break;
		}
	}

    void reset_view(){
       camera->reset();
    }
 
private:
    std::unique_ptr<openGL_frame_buffer_class> frame_buffer;
    glm::vec2 size;

    ImGuiWindowFlags overlay_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    //light_object_class *default_light = NULL;
    scene_lights_objects_class *scene_lights = NULL;

    user_binding_class  user_bindings;


 

};



