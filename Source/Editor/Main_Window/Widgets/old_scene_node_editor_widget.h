#pragma once
#include "../../Node_editor/ImNodes/Kernal/node_editor.h"

#include <Source/Modules/Module_Voxel_Byte/Node_editor/hcp_voxel_node.h>
#include <Source/Modules/Module_Voxel_Byte/Node_editor/Modifiers/translation.h>


#include <Source/Modules/Module_Hex_Surface/Node_editor/hex_surface_node.h>

#include "../../Scene/scene_manager.h"

class scene_node_editor_class : public node_editor_basis_class {
public:
    scene_manager_class* scene_manager = NULL;

    id_type* current_selected_entity_type_id = NULL;
    id_type* current_selected_entity_id = NULL;

    log_panel_class* log_panel = NULL;

    node_basis_class* selected_node = NULL;//*****

    void display() {
        if (nodes_context == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Nodes context not defined\n");
            return;
        }

        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
            return;
        }

        if (current_selected_entity_type_id == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No Entity variable defined \n");
            return;
        }

        ImNodes::BeginNodeEditor();

        // Right-click on blank space
        // Create a menu to perform actions if nothing is selected and right mouse pressed
        if (no_node_element_is_hovered_over) { // This menu is for when no node or link is hovered over

            if (ImGui::BeginPopupContextWindow(0, 1, false)) {
                ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
                menu_selection = true;
                //bool menu_select = ImGui::MenuItem("Create node for currently selected entity");
                if (ImGui::MenuItem("Create node for currently selected entity")) create_entity_node(click_pos);
                if (ImGui::BeginMenu("HCP Voxel ...")) {
                    {
                        if (ImGui::MenuItem("Translation"))    create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_TRANSLATION, click_pos);
                        //if (ImGui::MenuItem("Hex Surface"))  add_object(*current_selected_node_id, ENTITY_CATEGORY_HEX_SURF);
                        ImGui::EndMenu();
                    }
                };

                ImGui::EndPopup();
                nodes_context->LeftMouseClicked = false;
                menu_selection = false;
            }
        }

        //printf("scene_node_editor_class::display:: 00000\n");
                // Draw nodes
                // Beware that if a node that has been defined in nodes_ is not drawn, it seems
                // that displaying links will cause a crash. Add a counter to count that all nodes
                // are drawn and testing for it can prevent this
        for (UI_Node_struct_type node : nodes_) {
            if (node.node_ptr)
                node.node_ptr->draw_node(node, graph_);

            /*            switch (node.ui_node_type.function) {
                            case ENTITY_CATEGORY_HCP_VOXEL: hcp_voxel_node.draw_node(node, graph_); break;
                            case ENTITY_CATEGORY_HEX_SURF : hex_surface_node.draw_node(node, graph_); break;

                            case HCP_VOXEL_FUNCTION_TRANSLATION: {hcp_voxel_translation_node.draw_node(node, graph_); break; }
                            // Add other entity types here
                            //default : return;
                        }
            */
        }

        //printf("scene_node_editor_class::display:: 11111\n");

                // Display existing link lines
                //display_links();
        for (const auto& edge : graph_.edges())
        {
            // If edge doesn't start at value, then it's an internal edge, i.e.
            // an edge which links a node's operation to its input. We don't
            // want to render node internals with visible links.
            if (graph_.node(edge.from).node_type.function != VALUE)
                continue;

            ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(0, 139, 139, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(0, 175, 175, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(100, 255, 255, 255));
            ImNodes::Link(edge.id, edge.from, edge.to);
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }



        ImNodes::EndNodeEditor();

        // ---------------------------------------------------------------------------------
        // These are driven by Imnodes, so we place the code after EndNodeEditor().

        // Handle links
        handle_new_links();
        handle_deleted_links();


        no_node_element_is_hovered_over = no_element_hovered_over();

        if (ImNodes::IsNodeHovered(&hover_node_id))
        {
            //node_hovered = node_id;
           // printf("node hovered over id : %i\n", hover_node_id);
        }

        if (nodes_context->LeftMouseClicked && inside_editor_canvas(nodes_context->MousePos) && !menu_selection) {
            printf("Left mouse clicked inside canvas 00 %f : %f \n", nodes_context->MousePos.x, nodes_context->MousePos.y);
            //printf("Left mouse clicked 11 %f : %f \n", nodes_context->CanvasOriginScreenSpace.x, nodes_context->CanvasOriginScreenSpace.y);
            //printf("Left mouse clicked 22 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.x, nodes_context->CanvasRectScreenSpace.Max.x);
            //printf("Left mouse clicked 33 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.y, nodes_context->CanvasRectScreenSpace.Max.y);
            if (current_selected_node_id = -1) {
                *current_selected_entity_type_id = -1;
                *current_selected_entity_id = -1;
            }
        }

        num_selected_nodes = ImNodes::NumSelectedNodes();
        if (num_selected_nodes > 0)
        {
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected_nodes));
            ImNodes::GetSelectedNodes(selected_nodes.data());
            for (const int node_id : selected_nodes)
            {
                //if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
                current_selected_node_id = selected_nodes[selected_nodes.size() - 1];
                //printf("selected node id : %i : %i\n", node_id, current_selected_node_id);
                                //if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);

            }
        }
        else {
            current_selected_node_id = -1;
            selected_node = NULL;
            //printf("current node id : %i \n", current_selected_node_id);
        }

        if (current_selected_node_id > -1) {
            int node_index = find_node_index(current_selected_node_id);
            UI_Node_struct_type node;
            if (node_index > -1) {
                node = nodes_[node_index];
                //printf("Found current node of id : %i : %i : %i  \n", node.node_id, node.ui_node_type.category, node.entity_id);
                *current_selected_entity_type_id = node.ui_node_type.category;
                *current_selected_entity_id = node.entity_id;

                selected_node = node.node_ptr;
            }
        }

        //num_selected_nodes = 0;
/*
        if (ImNodes::IsNodeSelected(selected_node_id))
        {
            //node_hovered = node_id;
            printf("node hovered over id : %i\n", selected_node_id);
            if (log_panel != NULL) log_panel->application_log.AddLog("node hovered over id : %i\n", selected_node_id);
        }
*/
    }


private:
    bool menu_selection = false;

    void create_entity_node(ImVec2 click_pos) {
        //select existing voxel entity to reference and get entity id
        switch (*current_selected_entity_type_id) {
        case ENTITY_CATEGORY_HCP_VOXEL: add_hcp_voxel_node(click_pos);   break;
        case ENTITY_CATEGORY_HEX_SURF:  add_hex_surface_node(click_pos); break;

        }
    }

    void create_node(int entity_category, int entity_context, int entity_function, ImVec2 click_pos) {
        printf("scene_node_editor_class::create_node:: 00000\n");

        switch (entity_category) {
        case ENTITY_CATEGORY_HCP_VOXEL: add_hcp_voxel_node(entity_context, entity_function, click_pos); break;
            //case ENTITY_CATEGORY_HEX_SURF : add_hex_surface_node(entity_context, int entity_function, click_pos);
        }

    }

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hcp_voxel_node_class* hcp_voxel_node;
    hcp_voxel_translation_node_class* hcp_voxel_translation_node;

    void add_hcp_voxel_node(int entity_context, int entity_function, ImVec2 click_pos) {
        printf("scene_node_editor_class::add_hcp_voxel_node:: 00000\n");
        switch (entity_context) {
        case HCP_CONTEXT_VOXEL_MATRIX: add_hcp_matrix_node(entity_function, click_pos); break;
            // case HCP_CONTEXT_POINT_CLOUD  :
        }
    }

    void add_hcp_matrix_node(int entity_function, ImVec2 click_pos) {
        printf("scene_node_editor_class::add_hcp_matrix_node:: 00000\n");
        switch (entity_function) {
        case HCP_VOXEL_FUNCTION_TRANSLATION: { hcp_voxel_translation_node = new hcp_voxel_translation_node_class;
            hcp_voxel_translation_node->label = "Translation";
            hcp_voxel_translation_node->define_node(click_pos, -1, graph_, nodes_);
            break; }
        }
    }

    void add_hcp_voxel_node(ImVec2 click_pos) {
        //printf("scene_node_editor_class::add_hcp_voxel:: 00000\n");

                //select existing voxel entity to reference and get entity id
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
            return;
        }

        if (*current_selected_entity_type_id != ENTITY_CATEGORY_HCP_VOXEL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Selected entity is not a HCP Voxel\n");
            return;
        }

        if (*current_selected_entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity is selected or an entity has an invalid id\n");
            return;
        }

        if (node_exists(ENTITY_CATEGORY_HCP_VOXEL, ENTITY_CATEGORY_HCP_VOXEL, *current_selected_entity_id)) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Have entity node for selected entity defined.\n");
            return;
        }

        voxel_hcp_object_class* voxel_hcp_object = scene_manager->entities_manager.get_voxel_hcp_entity_object(*current_selected_entity_id);

        if (voxel_hcp_object != NULL) {
            //printf("scene_node_editor_class::add_hcp_voxel: 1111 %i \n", voxel_hcp_object->object_id);
            hcp_voxel_node = new hcp_voxel_node_class;
            hcp_voxel_node->label = voxel_hcp_object->object_name;
            hcp_voxel_node->define_node(click_pos, *current_selected_entity_id, graph_, nodes_);
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity of defined id exists within the scene.\n");
        }

        // menu_selection = false;
    }

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hex_surface_node_class* hex_surface_node;
    void add_hex_surface_node(ImVec2 click_pos) {
        //printf("scene_node_editor_class::add_hex_surface_node:: 00000\n");

                //select existing voxel entity to reference and get entity id
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
            return;
        }


        if (*current_selected_entity_type_id != ENTITY_CATEGORY_HEX_SURF) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Selected entity is not a hex surface entity\n");
            return;
        }

        if (*current_selected_entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity is selected or an entity has an invalid id\n");
            return;
        }

        if (node_exists(ENTITY_CATEGORY_HEX_SURF, ENTITY_CATEGORY_HEX_SURF, *current_selected_entity_id)) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Have entity node for selected entity defined.\n");
            return;
        }

        hex_surface_object_class* hex_surface_object = scene_manager->entities_manager.get_hex_surface_entity_object(*current_selected_entity_id);

        if (hex_surface_object != NULL) {
            //printf("scene_node_editor_class::add_hex_surface_node: 1111 %i \n", hex_surface_object->object_id);
            hex_surface_node = new hex_surface_node_class;
            hex_surface_node->label = hex_surface_object->object_name;
            hex_surface_node->define_node(click_pos, *current_selected_entity_id, graph_, nodes_);
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity of defined id exists within the scene.\n");
        }

        // menu_selection = false;
    }

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links() {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            const node_type_struct start_type = graph_.node(start_attr).node_type;
            const node_type_struct end_type = graph_.node(end_attr).node_type;

            printf("handle_new_links 0000 %i : %i", graph_.node(start_attr).node_id, graph_.node(end_attr).node_id);

            //const bool valid_function_link = start_type.function != end_type.function;
            //if (valid_function_link)
            if (start_type.category == end_type.category &&
                start_type.context == end_type.context &&
                start_type.function_category == end_type.function_category &&
                start_type.function != end_type.function)// && valid_function_link)
            {
                // Ensure the edge is always directed from the value to
                // whatever produces the value
                if (start_type.function != VALUE)
                {
                    std::swap(start_attr, end_attr);
                }
                graph_.insert_edge(start_attr, end_attr);
            }
        }
    }

    void handle_deleted_links() {
        // Delete link edges that define existing link input-output pairs
        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                graph_.erase_edge(link_id);
            }
        }

        // Delete link definitions
        {
            const int num_selected = ImNodes::NumSelectedLinks();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedLinks(selected_links.data());
                for (const int edge_id : selected_links)
                {
                    graph_.erase_edge(edge_id);
                }
                ImNodes::ClearLinkSelection();// or will still have selected links 
                                              //still defined that do not exist and will cause a crash
            }
        }

        // Delete node definitions
        {
            const int num_selected = ImNodes::NumSelectedNodes();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (const int node_id : selected_nodes)
                {
                    graph_.erase_node(node_id);
                    auto iter = std::find_if(
                        nodes_.begin(), nodes_.end(), [node_id](const UI_Node_struct_type& node) -> bool {
                            return node.node_id == node_id;
                        });

                    // Erase any additional internal nodes
                    switch (iter->ui_node_type.function)
                    {
                    case ENTITY_CATEGORY_HCP_VOXEL: graph_.erase_node(iter->inputs[0]);
                        graph_.erase_node(iter->inputs[1]);
                        break;

                        //case UiNodeType::add:
                        //        graph_.erase_node(iter->ui.add.lhs);
                        //        graph_.erase_node(iter->ui.add.rhs);
                        //        break;
                        //    case UiNodeType::multiply:
                        //        graph_.erase_node(iter->ui.multiply.lhs);
                        //        graph_.erase_node(iter->ui.multiply.rhs);
                        //        break;
                        //    case UiNodeType::output:
                        //        graph_.erase_node(iter->ui.output.r);
                        //        graph_.erase_node(iter->ui.output.g);
                        //        graph_.erase_node(iter->ui.output.b);
                        //        root_node_id_ = -1;
                        //        break;
                        //    case UiNodeType::sine:
                        //        graph_.erase_node(iter->ui.sine.input);
                        //        break;

                    default:
                        break;
                    }
                    nodes_.erase(iter);
                }
                ImNodes::ClearNodeSelection();// or will still have selected nodes 
                                              // still defined that do not exist and will cause a crash 
            }

        }
    }

};
#pragma once
