#pragma once
#include "../../Node_editor/ImNodes/Kernal/node_editor.h"

#include <Source/Modules/Module_Voxel_Byte/Node_editor/hcp_voxel_node.h>
#include <Source/Modules/Module_Voxel_Byte/Node_editor/Modifiers/translation_node.h>
#include <Source/Modules/Module_Voxel_Byte/Node_editor/Operators/interaction_node.h>


#include <Source/Modules/Module_Hex_Surface/Node_editor/hex_surface_node.h>
#include <Source/Modules/Module_Hex_Surface/Node_editor/Modifiers/translation_node.h>
#include <Source/Modules/Module_Hex_Surface/Node_editor/Operators/interaction_node.h>

#include "../../Scene/scene_manager.h"

class scene_node_editor_class : public node_editor_basis_class {
public:
    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel = NULL;

    node_basis_class    *selected_node = NULL;

    void display() {
//printf("scene_node_editor_class:: display : TEST : %i : %i \n", global_sclass::get_current_selected_entity_id(), global_sclass::get_current_selected_entity_type_id());

        if (nodes_context == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Nodes context not defined\n");
            return;
        }

        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
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
                        if (ImGui::MenuItem("Translation")) create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_TRANSLATION, click_pos);
                        if (ImGui::MenuItem("Boolean"))     create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_INTERACTION, click_pos);
                        //if (ImGui::MenuItem("Hex Surface"))  add_object(*current_selected_node_id, ENTITY_CATEGORY_HEX_SURF);
                        ImGui::EndMenu();
                    }
                };

                if (ImGui::BeginMenu("Hex Surface ...")) {
                    {
                        if (ImGui::MenuItem("Translation")) create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_TRANSLATION, click_pos);
                        if (ImGui::MenuItem("Boolean"))     create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_INTERACTION, click_pos);
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
        for(node_basis_class *node : graph.nodes.nodes){
//printf("scene_node_editor_class::draw:: 000 %i \n", node->node_id);
            node->draw_node(graph.pins);
        }

//printf("scene_node_editor_class::display:: 11111\n");

        // Display existing link lines
        for (link_struct_type link : graph.links.links)
        {
            // If edge doesn't start at value, then it's an internal edge, i.e.
            // an edge which links a node's operation to its input. We don't
            // want to render node internals with visible links.
            //if (graph_.node(edge.from).node_type.function != VALUE)
            //    continue;

            ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(0, 139, 139, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(0, 175, 175, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(100, 255, 255, 255));
            ImNodes::Link(link.link_id, link.from, link.to);
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
//printf("Left mouse clicked inside canvas 00 %f : %f \n", nodes_context->MousePos.x, nodes_context->MousePos.y);
//printf("Left mouse clicked 11 %f : %f \n", nodes_context->CanvasOriginScreenSpace.x, nodes_context->CanvasOriginScreenSpace.y);
//printf("Left mouse clicked 22 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.x, nodes_context->CanvasRectScreenSpace.Max.x);
//printf("Left mouse clicked 33 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.y, nodes_context->CanvasRectScreenSpace.Max.y);
            if (current_selected_node_id = -1) {
                globalc::set_current_selected_entity_type_id(INVALID_ID);
                globalc::set_current_selected_entity_id(INVALID_ID);
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
//printf("Found current node of id : %i  \n", current_selected_node_id);
            node_basis_class *node;
            node = graph.nodes.get_node(current_selected_node_id);

            if (node != NULL) {
//printf("Found current node of id : %i : %i : %i  \n", node->node_id, node->ui_node_type.category, node->entity_id);
                globalc::set_current_selected_entity_type_id(node->ui_node_type.category);
                globalc::set_current_selected_entity_id(node->entity_id);

                selected_node = node;
            }
        }


    }


private:
    bool menu_selection = false;

    void create_entity_node(ImVec2 click_pos) {
        //select existing voxel entity to reference and get entity id
        //switch (*current_selected_entity_type_id) {
        switch (globalc::get_current_selected_entity_type_id()) {
            case ENTITY_CATEGORY_HCP_VOXEL: add_hcp_voxel_node(click_pos);   break;
            case ENTITY_CATEGORY_HEX_SURF : add_hex_surface_node(click_pos); break;

        }

    }

    void create_node(int entity_category, int entity_context, int entity_function, ImVec2 click_pos) {
//printf("scene_node_editor_class::create_node:: 00000\n");

        switch (entity_category) {
            case ENTITY_CATEGORY_HCP_VOXEL: add_hcp_voxel_node(entity_context, entity_function, click_pos); break;
            case ENTITY_CATEGORY_HEX_SURF : add_hex_surface_node(entity_context, entity_function, click_pos); break;
        }

    }

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hcp_voxel_node_class             *hcp_voxel_node;
    hcp_voxel_translation_node_class *hcp_voxel_translation_node;
    hcp_interaction_node_class       *hcp_voxel_interaction_node;

    void add_hcp_voxel_node(int entity_context, int entity_function, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_voxel_node:: 00000\n");
        switch (entity_context) {
            case HCP_CONTEXT_VOXEL_MATRIX: add_hcp_matrix_node(entity_function, click_pos); break;
                // case HCP_CONTEXT_POINT_CLOUD  :
        }

    }

    void add_hcp_matrix_node(int entity_function, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_matrix_node:: 00000\n");
        switch (entity_function) {
            case HCP_VOXEL_FUNCTION_TRANSLATION: { hcp_voxel_translation_node = new hcp_voxel_translation_node_class;
                                                   hcp_voxel_translation_node->label = "Translation";
                                                   hcp_voxel_translation_node->scene_manager = scene_manager;
                                                   hcp_voxel_translation_node->log_panel     = log_panel;
                                                   graph.add_node(hcp_voxel_translation_node, click_pos, NON_ENTITY_ID);
                                                   break; }

            case HCP_VOXEL_FUNCTION_INTERACTION: { hcp_voxel_interaction_node = new hcp_interaction_node_class;
                                                   hcp_voxel_interaction_node->label = "Interaction";
                                                   hcp_voxel_interaction_node->scene_manager = scene_manager;
                                                   hcp_voxel_interaction_node->log_panel = log_panel;
                                                   graph.add_node(hcp_voxel_interaction_node, click_pos, NON_ENTITY_ID);
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

        if (globalc::get_current_selected_entity_type_id() != ENTITY_CATEGORY_HCP_VOXEL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Selected entity is not a HCP Voxel\n");
            return;
        }

        if (globalc::get_current_selected_entity_id() < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity is selected or an entity has an invalid id\n");
            return;
        }

        if (graph.node_exists(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, globalc::get_current_selected_entity_id())) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Have entity node for selected entity defined.\n");
            return;
        }

        voxel_hcp_object_class* voxel_hcp_object = scene_manager->entities_manager.get_voxel_hcp_entity_object(globalc::get_current_selected_entity_id());

        if (voxel_hcp_object != NULL) {
//printf("scene_node_editor_class::add_hcp_voxel: 1111 %i \n", voxel_hcp_object->object_id);
            hcp_voxel_node            = new hcp_voxel_node_class;
            hcp_voxel_node->entity_id = voxel_hcp_object->object_id;
            hcp_voxel_node->label     = voxel_hcp_object->object_name;
            graph.add_node(hcp_voxel_node, click_pos, globalc::get_current_selected_entity_id());
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity of defined id exists within the scene.\n");
        }

    }

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hex_surface_node_class             *hex_surface_node;
    hex_surface_translation_node_class *hex_surface_translation_node;
    hex_surface_interaction_node_class *hex_surface_interaction_node;

    void add_hex_surface_node(int entity_context, int entity_function, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_voxel_node:: 00000\n");
        switch (entity_context) {
            case HEX_CONTEXT_SURFACE_GRID: add_hex_surface_node(entity_function, click_pos); break;
                // case HCP_CONTEXT_POINT_CLOUD  :
        }

    }

    void add_hex_surface_node(int entity_function, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_matrix_node:: 00000\n");
        switch (entity_function) {
           case HEX_SURFACE_FUNCTION_TRANSLATION: {hex_surface_translation_node = new hex_surface_translation_node_class;
                                                    hex_surface_translation_node->label         = "Translation";
                                                    hex_surface_translation_node->scene_manager = scene_manager;
                                                    hex_surface_translation_node->log_panel     = log_panel;
                                                    graph.add_node(hex_surface_translation_node, click_pos, NON_ENTITY_ID);
                                                    break; }



            case HEX_SURFACE_FUNCTION_INTERACTION: {hex_surface_interaction_node = new hex_surface_interaction_node_class;
                                                    hex_surface_interaction_node->label = "Interaction";
                                                    hex_surface_interaction_node->scene_manager = scene_manager;
                                                    hex_surface_interaction_node->log_panel = log_panel;
                                                    graph.add_node(hex_surface_interaction_node, click_pos, NON_ENTITY_ID);
                                                    break; }
        }

    }



    void add_hex_surface_node(ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hex_surface_node:: 00000\n");

        //select existing voxel entity to reference and get entity id
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
            return;
        }

        if (globalc::get_current_selected_entity_type_id() != ENTITY_CATEGORY_HEX_SURF) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Selected entity is not a hex surface entity\n");
            return;
        }

        if (globalc::get_current_selected_entity_id() < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity is selected or an entity has an invalid id\n");
            return;
        }

        if (graph.node_exists(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, globalc::get_current_selected_entity_id())) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Have entity node for selected entity defined.\n");
            return;
        }

        hex_surface_object_class* hex_surface_object = scene_manager->entities_manager.get_hex_surface_entity_object(globalc::get_current_selected_entity_id());

        if (hex_surface_object != NULL) {
//printf("scene_node_editor_class::add_hex_surface_node: 1111 %i \n", hex_surface_object->object_id);
            hex_surface_node            = new hex_surface_node_class;
            hex_surface_node->entity_id = hex_surface_object->object_id;
            hex_surface_node->label     = hex_surface_object->object_name;
            graph.add_node(hex_surface_node, click_pos, globalc::get_current_selected_entity_id());
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity of defined id exists within the scene.\n");
        }

    }

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links() {
        int start_attr, end_attr;
        // This does not create a link, only returns the pin ids of the the two elected pin links
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) 
        {
//printf("handle_new_links 0000 :: IsLinkCreated");


            pin_struct_type start_pin = graph.pins.find_pin(start_attr);
            pin_struct_type end_pin   = graph.pins.find_pin(end_attr);

//printf("handle_new_links 0000 %i : %i\n", start_pin.pin_id, end_pin.pin_id);

            if (start_pin.pin_value_type.category          == end_pin.pin_value_type.category &&
                start_pin.pin_value_type.context           == end_pin.pin_value_type.context &&
                start_pin.pin_value_type.function_category == end_pin.pin_value_type.function_category &&
                start_pin.pin_value_type.value_type        == end_pin.pin_value_type.value_type)
            {
                // Make sure do not create a link if have pin already assigned to a link if have a single link only permitted
                // Future enhancement do auto delete existing link ?????
                if ((start_pin.pin_type == pin_type_enum::single_output && !graph.links.start_pin_has_link(start_pin.pin_id)) ||
                     start_pin.pin_type == pin_type_enum::multi_output) 
                {
                    if ((end_pin.pin_type == pin_type_enum::single_input && !graph.links.end_pin_has_link(end_pin.pin_id)) ||
                        end_pin.pin_type == pin_type_enum::multi_input) 
                    {
                        graph.links.insert(start_pin.pin_id, end_pin.pin_id);
                    }
                }

                    

                // Ensure the edge is always directed from the value to
                // whatever produces the value
               // if (start_pin.pin_type != VALUE)
               // {
               //     std::swap(start_attr, end_attr);
               // }
               // graph_.insert_edge(start_attr, end_attr);

               // graph.links.insert(start_pin.pin_id,end_pin.pin_id);
            }

        }

    }


    void handle_deleted_links() {
        // Delete link edges that define existing link input-output pairs
        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                //graph_.erase_edge(link_id);
                graph.links.delete_link(link_id);
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

                for (int link_id : selected_links){
                    graph.links.delete_link(link_id);
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
                for (int node_id : selected_nodes){
 //printf("scene_node_editor_class::handle_deleted_links:: 0000 %i \n", node_id);                   
                    graph.delete_node(node_id);
                }

                ImNodes::ClearNodeSelection();// or will still have selected nodes 
                                              // still defined that do not exist and will cause a crash 
            }

        }
    }

};
