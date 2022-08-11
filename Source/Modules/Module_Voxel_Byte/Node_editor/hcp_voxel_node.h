#pragma once
#include <Source/Editor/Node_editor/ImNodes/Kernal/node.h>

#include "hcp_voxel_node_definitions.h"
#include "../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"


class hcp_voxel_node_class : public node_basis_class {
public:

    hcp_voxel_node_class() {
        node_width = 60.0f;
    }

    bool define_node(ImVec2   click_pos,
                     id_type  entity_id_) {

        entity_id = entity_id_;

        ui_node_type.category          = ENTITY_CATEGORY_HCP_VOXEL;
        ui_node_type.context           = HCP_CONTEXT_VOXEL_MATRIX;
        ui_node_type.function_category = GENERATOR;
        ui_node_type.function          = HCP_VOXEL_MATRIX_VALUE;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }

    bool define_inputs(pins_class& pins) {
        return define_node_input_value_of_type("Matrix", ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, MODIFIER, HCP_VOXEL_MATRIX_VALUE, pin_type_enum::single_input, pins);
    }

    bool define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Matrix", ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, OPERATOR, HCP_VOXEL_MATRIX_OPERATOR, pin_type_enum::single_output,  pins);
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

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(120, 30, 30, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(200, 85, 85, 255));
        draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
// printf("hcp_voxel_node_class::draw_node:: 333\n");

        ImGui::Spacing();

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(100, 30, 150, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(160, 40, 230, 255));
        draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

   //voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget;

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only

    void display_ui(scene_manager_class *scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1) {
 //printf("display_ui HCP VOxel 0000\n");
 /*       if (log_panel == NULL) {
            printf("ERROR: No log_panel defined to display messages in the log panbel from the node editor\n");
        }

        if (scene_manager == NULL) {
            if (log_panel != NULL) 
                log_panel->application_log.AddLog("ERROR: No scene manager defined to display hcp Voxel paramater data from the node editor\n");
            else
                printf("ERROR: No scene manager defined to display hcp Voxel paramater data from the node editor\n");
            return;
        }

        if (current_selected_object_id == NULL) {
            if (log_panel != NULL)
                log_panel->application_log.AddLog("ERROR: No current_selected HCP voxel entity defined to display hcp Voxel paramater data from the node editor\n");
            else
                printf("ERROR: No current_sel;ected HCP voxel entity defined to display hcp Voxel paramater data from the node editor\n");
            return;
        }

        //voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget;
 //printf("display_ui HCP VOxel 111111\n");
 		voxel_hcp_parameters_widget.scene_manager = scene_manager;
		voxel_hcp_parameters_widget.log_panel     = log_panel;

        voxel_hcp_parameters_widget.display_voxel_hcp_generation_widget(*current_selected_object_id);
*/
    }

private:
};
