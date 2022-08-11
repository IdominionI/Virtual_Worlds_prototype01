#pragma once
#include "graph.h"
#include "../imnodes.h"
#include "../imnodes_internal.h"

#include <imgui.h>

class node_editor_basis_class {
public:
	ImNodesContext* nodes_context = NULL;

protected:
    graph_class graph;
    int         root_node_id_;

    bool no_node_element_is_hovered_over;

    int hover_node_id, hover_link_id, hover_pin_id;
    int current_selected_node_id = -1;
    int num_selected_nodes;

    bool inside_editor_canvas(ImVec2 location) {
        if (location.x >= nodes_context->CanvasRectScreenSpace.Min.x &&
            location.x <= nodes_context->CanvasRectScreenSpace.Max.x &&
            location.y >= nodes_context->CanvasRectScreenSpace.Min.y &&
            location.y <= nodes_context->CanvasRectScreenSpace.Max.y)
            return true;
        else
            return false;
    }

    bool no_element_hovered_over() {
        if (ImNodes::IsNodeHovered(&hover_node_id) ||
            ImNodes::IsLinkHovered(&hover_link_id) ||
            ImNodes::IsPinHovered(&hover_pin_id))
            return false;
        else
            return true;
    }

private:
};