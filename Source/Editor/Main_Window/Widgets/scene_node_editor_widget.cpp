#include "scene_node_editor_widget.h"

#include "../../Common/global.h"

#include <Source/Modules/Module_Hex_Surface/Node_editor/hex_surface_node_definitions.h>
#include <Source/Modules/Module_Voxel_Byte/Node_editor/hcp_voxel_node_definitions.h>

void scene_node_editor_class::display() {
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
    if (no_node_element_is_hovered_over && selected_node == NULL && selected_link.link_id == INVALID_ID) { // This menu is for when no node or link is hovered over //*****

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
            menu_selection = true;

            if (ImGui::BeginMenu("HCP Voxel ...")) {
                {
                    if (ImGui::MenuItem("Create node for currently selected voxel")) 
                        create_node(ENTITY_CATEGORY_HCP_VOXEL,HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                    if (ImGui::MenuItem("Translation")) 
                        create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                    if (ImGui::MenuItem("Boolean"))     
                        create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);

                    ImGui::EndMenu();
                }
            };

            if (ImGui::BeginMenu("Hex Surface ...")) {
                {
                    if (ImGui::MenuItem("Create node for currently selected hex_surface"))
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                    if (ImGui::MenuItem("Translation")) 
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                    if (ImGui::MenuItem("Boolean"))     
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);
                    ImGui::EndMenu();
                }
            };

            ImGui::EndPopup();
            nodes_context->LeftMouseClicked = false;
            menu_selection = false;
        }
    }

    // Right click on a selected node to display a menu selection of actions to perform on that node
    if (selected_node != NULL) {

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            //ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
            menu_selection = true;

            if (ImGui::BeginMenu("Node ...")) {
                {
                    if (ImGui::MenuItem("Delete node")) {
                       if(current_selected_node_id > -1) graph.delete_node(current_selected_node_id);
                    }
                    if (ImGui::MenuItem("delete node links")) {
                      if(current_selected_node_id > -1) graph.delete_node_links(current_selected_node_id);
                    }

                    ImGui::EndMenu();
                }
            };

            // If selected node is an entity node, then these options are available to be performed
            if (selected_node->entity_id > -1) {
                if (ImGui::BeginMenu("Entity ...")) {
                    {
                        if (ImGui::MenuItem("save node tree")){
printf("scene_node_editor_class : display : save entity selected\n");
                            // Not yet implemented    
                        }

                        if (ImGui::MenuItem("select node tree")) {
printf("scene_node_editor_class : display : select node tree\n");
                          // Not yet implemented    
                        }
                            
                        if (ImGui::MenuItem("delete node tree")) {
                            node_tree.delete_node_tree(&graph, selected_node);
                        }

                        ImGui::EndMenu();
                    }
                };
            }

            ImGui::EndPopup();
            nodes_context->LeftMouseClicked = false;
            menu_selection = false;
        }
    }

    if (selected_link.link_id != INVALID_ID) {

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            menu_selection = true;

            if (ImGui::BeginMenu("Link ...")) {
                {
                    if (ImGui::MenuItem("Delete link")) {
                       if(selected_link.link_id > -1) graph.delete_link(selected_link.link_id);
                    }
                    ImGui::EndMenu();
                }
            };

            ImGui::EndPopup();
            nodes_context->LeftMouseClicked = false;
            menu_selection = false;
        }
    }
    
    //************************

//printf("scene_node_editor_class::display:: 00000\n");
    // Draw nodes
    for (node_basis_class* node : graph.nodes.nodes) {
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
 //printf("node hovered over id : %i\n", hover_node_id);
    }

    //****
    //if (!inside_editor_canvas(nodes_context->MousePos)) {
    //    globalc::set_current_selected_node_id(INVALID_ID);//*****
   // }
    //****

    if (nodes_context->LeftMouseClicked && inside_editor_canvas(nodes_context->MousePos) && !menu_selection) {
//printf("Left mouse clicked inside canvas 00 %f : %f \n", nodes_context->MousePos.x, nodes_context->MousePos.y);
//printf("Left mouse clicked 11 %f : %f \n", nodes_context->CanvasOriginScreenSpace.x, nodes_context->CanvasOriginScreenSpace.y);
//printf("Left mouse clicked 22 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.x, nodes_context->CanvasRectScreenSpace.Max.x);
//printf("Left mouse clicked 33 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.y, nodes_context->CanvasRectScreenSpace.Max.y);
        //if (globalc::get_current_selected_node_id() == -1) {
        if (current_selected_node_id == -1) {
            globalc::set_current_selected_entity_type_id(INVALID_ID);
            globalc::set_current_selected_entity_id(INVALID_ID);
            globalc::set_outliner_selection(false);
        }
    }

    num_selected_nodes = ImNodes::NumSelectedNodes();
    if (num_selected_nodes > 0 && !globalc::get_outliner_selection())
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
//if (globalc::get_current_selected_node_id() > -1) {
//printf("Found current node of id : %i  \n", current_selected_node_id);
        node_basis_class* node;
        node = graph.nodes.get_node(current_selected_node_id);

        if (node != NULL) {
//printf("Found current node of id : %i : %i : %i  \n", node->node_id, node->ui_node_type.category, node->entity_id);
            globalc::set_current_selected_entity_type_id(node->ui_node_type.category);
            globalc::set_current_selected_entity_id(node->entity_id);

            selected_node = node;
        }
    }

    //*****
    num_selected_links = ImNodes::NumSelectedLinks();
    if (num_selected_links > 0 )
    {
//printf("num_selected_links : %i \n", num_selected_links);
        static std::vector<int> selected_links;
        selected_links.resize(static_cast<size_t>(num_selected_links));
        ImNodes::GetSelectedLinks(selected_links.data());
        for (const int link_id : selected_links)
        {
//if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
            current_selected_link_id = selected_links[selected_links.size() - 1];
//printf("selected link id : %i : %i\n", link_id, current_selected_link_id);
//if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);

        }
    }
    else {
        current_selected_link_id = -1;
        selected_link.link_id = INVALID_ID;
//printf("current node id : %i \n", current_selected_node_id);
    }

    if (current_selected_link_id > -1) {
//if (globalc::get_current_selected_node_id() > -1) {
//printf("current_selected_link_id : %i  \n", current_selected_link_id);
        link_struct_type link;
        link = graph.links.find_link(current_selected_link_id);

        if (link.link_id != INVALID_ID) {
//printf("Found current link of id : %i  \n", link.link_id);
            selected_link = link;
        }
    }

    //*****
}


//----------------------------------------------

    void scene_node_editor_class::create_node(int category, int context,int function,int function_category,  ImVec2 click_pos) {

        //select existing voxel entity to reference and get entity id
//printf("scene_node_editor_class::create_entity_node 0000\n");
        node_type_struct editor_node_definition;
        editor_node_definition.category          = category;
        editor_node_definition.context           = context;
        editor_node_definition.function          = function;
        editor_node_definition.function_category = function_category;

//printf("scene_node_editor_class::create_entity_node 1111\n");
        node_editor_category_manager->perform_editor_node_creation(editor_node_definition, &graph, scene_manager, log_panel, click_pos);
//printf("scene_node_editor_class::create_entity_node 22222\n");
    }

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void scene_node_editor_class::handle_new_links() {
        int start_attr, end_attr;
        // This does not create a link, only returns the pin ids of the the two elected pin links
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            //printf("handle_new_links 0000 :: IsLinkCreated");


            pin_struct_type start_pin = graph.pins.find_pin(start_attr);
            pin_struct_type end_pin = graph.pins.find_pin(end_attr);

            //printf("handle_new_links 0000 %i : %i\n", start_pin.pin_id, end_pin.pin_id);

            if (start_pin.pin_value_type.category == end_pin.pin_value_type.category &&
                start_pin.pin_value_type.context == end_pin.pin_value_type.context &&
                start_pin.pin_value_type.function_category == end_pin.pin_value_type.function_category &&
                start_pin.pin_value_type.value_type == end_pin.pin_value_type.value_type)
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


    void scene_node_editor_class::handle_deleted_links() {
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
            const int num_selected_links = ImNodes::NumSelectedLinks();
            if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected_links));
                ImNodes::GetSelectedLinks(selected_links.data());

                for (int link_id : selected_links) {
                    graph.links.delete_link(link_id);
                }

                ImNodes::ClearLinkSelection();// or will still have selected links 
                //still defined that do not exist and will cause a crash
            }
        }

        // Delete node definitions
        {
            const int num_selected_links = ImNodes::NumSelectedNodes();
            if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected_links));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (int node_id : selected_nodes) {
//printf("scene_node_editor_class::handle_deleted_links:: 0000 %i \n", node_id);                   
                    graph.delete_node(node_id);
                }

                ImNodes::ClearNodeSelection();// or will still have selected nodes 
                // still defined that do not exist and will cause a crash 
            }

        }
    }