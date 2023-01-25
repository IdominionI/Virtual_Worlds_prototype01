#pragma once
#include <Source/Editor/Node_editor/ImNodes/Kernal/graph.h>

#include "../hex_surface_node_definitions.h"
#include "../../Render/hex_surface_render.h"

class hex_surface_translation_node_class : public node_basis_class {
public:
    //hcp_voxel_translation_node_class(graph_class *graph_) : graph(graph_) { This creates a copile error
    hex_surface_translation_node_class() {
        node_width = 60.0f;
    }

    // Not sure just yet, but scene_manager and log_panel may need to be defined in the node basis class if
    // found that most or all node classes require it
    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel     = NULL;

    bool define_node(ImVec2   click_pos,
                     id_type  entity_id_) {

        entity_id = entity_id_;

        ui_node_type.category          = ENTITY_CATEGORY_HEX_SURF;
        ui_node_type.context           = HEX_CONTEXT_SURFACE_GRID;
        ui_node_type.function_category = MODIFIER;
        ui_node_type.function          = HEX_SURFACE_GRID_VALUE;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }

    bool define_inputs(pins_class& pins) {
        return define_node_input_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, MODIFIER, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_input, pins);
    }

    bool define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, MODIFIER, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_output,  pins);
    }

    bool draw_node(pins_class pins)
    {
//printf("hcp_voxel_node_class::draw_node:: 000\n");
        // Define titlebare colors
        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 139, 139, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 175, 175, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 255, 255));

        ImNodes::BeginNode(node_id);
 //printf("hcp_voxel_node_class::draw_node:: 000\n");
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));// Node header text color
        draw_node_header();
        ImGui::PopStyleColor();
 //printf("hcp_voxel_node_class::draw_node:: 111\n");
        ImGui::Dummy(ImVec2(node_width, node_height));

 //printf("hcp_voxel_node_class::draw_node:: 222\n");

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(250, 140, 10, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 225, 130, 255));
        draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
// printf("hcp_voxel_node_class::draw_node:: 333\n");

        ImGui::Spacing();

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(250, 140, 10, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 225, 130, 255));
        draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

    void display_ui(scene_manager_class *scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = INVALID_ID) {
        float x_pos = 10.0f, y_pos = 60.0f;
        translation = { 0.0f,0.0f };
//printf("hcp_voxel_translation_node_class::display_ui 00000 \n");
        text("Hex surface Translation", x_pos+100, y_pos);
        y_pos += 40;
        text("Coordinate     value       step active", x_pos+50, y_pos);

        //float_input_delta("X location", x_loc, 1.0f, x_pos+100, y_pos + 20, 50, "###tbu", "###tbu");
        std::string id_prefix = "###nhsui"+ FW::stringtools::toString(node_id); std::string w_id_prefix = ""; std::string w_id = "";

        y_pos += 30;
        w_id_prefix = id_prefix+"x";
        text("X :     ", x_pos + 75, y_pos);
        ImGui::SameLine(); w_id = w_id_prefix + "2"; ImGui::SetNextItemWidth(75); ImGui::InputFloat(w_id.c_str(), &translation_step[0]);
        // ************************** Added increment/decrement buttons ********************************
        float t_height = ImGui::GetTextLineHeight();
        float b_height = ImGui::GetTextLineHeight() / 2.0f;
        float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

        ImGui::SameLine(); w_id = w_id_prefix + "5"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
        ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                           // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation_step[0] += step[0];
        }

        ImGui::SameLine(); w_id = w_id_prefix + "6"; ImGui::SetNextItemWidth(15);
        ImGui::SetCursorPosY(ypos - b_height - 8);
        ImGui::SetCursorPosX(xpos + 226);

        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation_step[0] -= step[0];
        }

        ImGui::SameLine(); ImGui::SetCursorPosY(ypos - b_height - 18);
        w_id = w_id_prefix + "3"; ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &step[0]);
        ImGui::SameLine(); w_id = w_id_prefix + "4"; ImGui::Checkbox(w_id.c_str(), &active[0]);

        // ************************** Added increment/decrement buttons ********************************
        t_height = ImGui::GetTextLineHeight();
        b_height = ImGui::GetTextLineHeight() / 2.0f;
        xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

        ImGui::SameLine(); w_id = w_id_prefix + "7"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
        ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                           // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation[0] = step[0];
            action();
        }

        ImGui::SameLine(); w_id = w_id_prefix + "8"; ImGui::SetNextItemWidth(15);
        ImGui::SetCursorPosY(ypos - b_height - 8);
        ImGui::SetCursorPosX(xpos + 327);

        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation[0] = -step[0];
            action();
        }


        //----------------------------------------------------------------------------------------

        y_pos += 20;
        w_id_prefix = id_prefix + "y";
        text("Y :     ", x_pos + 75, y_pos);
        ImGui::SameLine(); w_id = w_id_prefix + "2"; ImGui::SetNextItemWidth(75); ImGui::InputFloat(w_id.c_str(), &translation_step[1]);
        // ************************** Added increment/decrement buttons ********************************
        t_height = ImGui::GetTextLineHeight();
        b_height = ImGui::GetTextLineHeight() / 2.0f;
        xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

        ImGui::SameLine(); w_id = w_id_prefix + "5"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
        ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                           // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation_step[1] += step[1];
        }

        ImGui::SameLine(); w_id = w_id_prefix + "6"; ImGui::SetNextItemWidth(15);
        ImGui::SetCursorPosY(ypos - b_height - 8);
        ImGui::SetCursorPosX(xpos + 226);

        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation_step[1] -= step[1];
        }

        ImGui::SameLine(); ImGui::SetCursorPosY(ypos - b_height - 18);
        w_id = w_id_prefix + "3"; ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &step[1]);
        ImGui::SameLine(); w_id = w_id_prefix + "4"; ImGui::Checkbox(w_id.c_str(), &active[1]);

        // ************************** Added increment/decrement buttons ********************************
        t_height = ImGui::GetTextLineHeight();
        b_height = ImGui::GetTextLineHeight() / 2.0f;
        xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

        ImGui::SameLine(); w_id = w_id_prefix + "7"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
        ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                           // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation[1] = step[1];
            action();
        }

        ImGui::SameLine(); w_id = w_id_prefix + "8"; ImGui::SetNextItemWidth(15);
        ImGui::SetCursorPosY(ypos - b_height - 8);
        ImGui::SetCursorPosX(xpos + 327);

        if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
            translation[1] = -step[1];
            action();
        }
        //---------------------------------------------------------------------------------
 
        y_pos += 40;

        if (ex_button(ICON_FA_STEP_BACKWARD "###nhsuidt", x_pos + 145, y_pos, 50, 20))
            decrement_tanslation();


        if (ex_button(ICON_FA_STEP_FORWARD "###nhsuiit", x_pos + 245, y_pos, 50, 20))
            increment_translation();

        y_pos += 40;

        if (ex_button("Relative###nhsuirt", x_pos + 180, y_pos, 80, 20))
            coordinate_translation();
        y_pos += 30;
        if (ex_button("World###nhsuiat", x_pos + 180, y_pos, 80, 20))
            coordinate_translation(false);

    }



    void action() {
//printf("translation action 0000 %i \n", node_id);
        node_basis_class *modify_node = find_node_to_modify(outputs[0]);
        if (modify_node == NULL) return;
//printf("translation action 11111 %i \n", modify_node->node_id);
        if(modify_node->node_id == NON_ENTITY_ID) return;
//printf("translation action 22222 %s \n", modify_node->label.c_str());
        
        if(scene_manager == NULL) return;
//printf("translation action 33333 %i \n",modify_node->entity_id);
       // hex_surface_object_class * hex_surface_object = scene_manager->entities_manager.get_hex_surface_entity_object(modify_node->entity_id);
        hex_surface_object_class *hex_surface_object = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(modify_node->entity_id, ENTITY_CATEGORY_HEX_SURF);
//printf("translation action 4444  \n");
        if (hex_surface_object == NULL) return;
//printf("translation action 55555  \n");
        if(translate)
            hex_surface_object->hex_surface_object_data.grid_origin += translation;
        else
            hex_surface_object->hex_surface_object_data.grid_origin = translation;
//printf("translation action 66666 %f : %f : %f \n", voxel_hcp_object->voxel_object_data.matrix_origin.x, voxel_hcp_object->voxel_object_data.matrix_origin.y, voxel_hcp_object->voxel_object_data.matrix_origin.z);
//printf("translation action 66666AAA %f : %f: %f \n", location.x, location.y, location.z);
        
        // !!!!!!!! FOLLOWING UPDATE CODE NEEDS TO BE PUT INTO A GENERAL UPDATE VOXEL VERTICES FUNCTION !!!!!!!!!!!!!!
        hex_surface_object->define_vbo_vertices(MIN_HEX_SURF_VALUE, MAX_HEX_SURF_VALUE);

        //####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		scene_node_class <render_object_class> *scene_hex_surface_object = scene_manager->get_render_object(modify_node->entity_id);
//printf("voxel_hcp_generation_widget_class :: execute_voxel_function 5555\n");

        if (scene_hex_surface_object != NULL) {
            if (!hex_surface_render.update_geometry_vertex_cloud_data(&hex_surface_object->point_cloud, scene_hex_surface_object, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene hex surface object geometry could not be updated.\n");
//printf("translation action : scene_voxel_object not updated\n");
                return;
            }

            shader_class shader;

            //application_default_shader_uniform_variables_struct_type uniform_variable;
//printf("translation action 77777 %s \n");
            //uniform_variable.type = application_default_shader_variable_type_enum::Floatv2; uniform_variable.name = "grid_origin"; uniform_variable.value0 = &hex_surface_object->hex_surface_object_data.grid_origin;
            //scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);

            glm::vec3 grid_origin = { hex_surface_object->hex_surface_object_data.grid_origin.x,hex_surface_object->hex_surface_object_data.grid_origin.y,0.0f };

            shader.set_vec3(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, grid_origin, "voxel_origin");
        }
    }

    void decrement_tanslation() {
        for (int i = 0; i < 2; i++) {
            if (active[i]) {
                translation[i] = -step[i];
            }
        }
        action();
    }

    void increment_translation() {
        for (int i = 0; i < 2; i++) {
            if (active[i]) {
                translation[i] = step[i];
            }
        }
        action();
    }

    void coordinate_translation(bool relative_translation = true) {
        for (int i = 0; i < 2; i++) {
           translation[i] = translation_step[i];
        }

        translate = relative_translation;
        action();
        translate = true;
    }

    node_basis_class *find_node_to_modify(int output_pin_id) {
//printf("find_node_to_modify 0000 %i \n", output_pin_id);
        link_struct_type link = graph->find_link_with_start_pin(output_pin_id);
//printf("find_node_to_modify 11111 %i : %i\n", link.link_id, link.to);
        while (link.link_id != INVALID_ID) {
            int to_node_id = graph->pins.find_pin(link.to).node_id;
//printf("find_node_to_modify 2222 %i \n", to_node_id);
            node_basis_class *node = graph->nodes.get_node(to_node_id);
            if (node == NULL) { return NULL; break; }
//printf("find_node_to_modify 33333 %i \n", to_node_id);
            if (node->node_id != NON_ENTITY_ID) {
                if(node->ui_node_type.category          == ENTITY_CATEGORY_HEX_SURF &&
                   node->ui_node_type.context           == HEX_CONTEXT_SURFACE_GRID &&
                   node->ui_node_type.function_category == GENERATOR &&
                   node->ui_node_type.function          == HEX_SURFACE_GRID_VALUE)
                {
                    return node;
                    break;
                }
            }
//printf("find_node_to_modify 44444 %i \n", node->outputs[0]);
            link = graph->find_link_with_start_pin(node->outputs[0]);
//printf("find_node_to_modify 55555 %i : %i\n", link.link_id, link.to);
        }

        return NULL;
    }

 
protected:

private:
    hex_surface_render_class hex_surface_render;
    bool translate = true;
    bool translate_increment = true;
    glm::vec2  location, step, translation, translation_step;
    glm::bvec3 active; // ImGui Checkbox does not like std::vector<bool> types !!!!



};