#pragma once
#include <Source/Editor/Node_editor/ImNodes/Kernal/node.h>

#include "hex_surface_node_definitions.h"
#include "../Hex_surface_object/DataTypes/dt_hex_surface_generator.h"


class hex_surface_node_class : public node_basis_class {
public:

    hex_surface_node_class() {
        node_width = 60.0f;
    }

    bool define_node(ImVec2   click_pos,
                     id_type  entity_id_) {

        entity_id = entity_id_;

        ui_node_type.category          = ENTITY_CATEGORY_HEX_SURF;
        ui_node_type.context           = HEX_CONTEXT_SURFACE_GRID;
        ui_node_type.function_category = GENERATOR;
        ui_node_type.function          = HEX_SURFACE_GRID_VALUE;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }

    bool define_inputs(pins_class& pins) {
        return define_node_input_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, MODIFIER, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_input, pins);
    }

    bool define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, OPERATOR, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_output, pins);
    }

    bool draw_node(pins_class pins)
    {
//printf("hex_surface_node_class::draw_node:: 000\n");
        // Define titlebare colors
        ImNodes::PushColorStyle(ImNodesCol_TitleBar,         IM_COL32(240, 170, 20, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered,  IM_COL32(250, 200, 40, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 225, 60, 255));

        ImNodes::BeginNode(node_id);
 //printf("hex_surface_node_class::draw_node:: 000\n");
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));// Node header text color
        draw_node_header();
        ImGui::PopStyleColor();
 //printf("hex_surface_node_class::draw_node:: 111\n");
        ImGui::Dummy(ImVec2(node_width, node_height));

 //printf("hex_surface_node_class::draw_node:: 222\n");

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(250, 140, 10, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 225, 130, 255));
        draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
// printf("hex_surface_node_class::draw_node:: 333\n");

        ImGui::Spacing();

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(60, 10, 250, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(180, 100, 250, 255));
        draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

//private:
};
