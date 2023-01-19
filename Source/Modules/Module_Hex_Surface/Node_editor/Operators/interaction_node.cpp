#include "interaction_node.h"

#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>

    bool hex_surface_interaction_node_class::define_node(ImVec2 click_pos,id_type  entity_id) {

        entity_id = entity_id;

        ui_node_type.category          = ENTITY_CATEGORY_HEX_SURF;
        ui_node_type.context           = HEX_CONTEXT_SURFACE_GRID;
        ui_node_type.function_category = OPERATOR;
        ui_node_type.function          = HEX_SURFACE_GRID_OPERATOR;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }

    bool hex_surface_interaction_node_class::define_inputs(pins_class& pins) {
        if(!define_node_input_value_of_type("Grid A", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, OPERATOR, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_input, pins))
            return false;

        return define_node_input_value_of_type("Grid B", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, OPERATOR, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_input, pins);
    }

    bool hex_surface_interaction_node_class::define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, GENERAL, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_output,  pins);
    }

    void hex_surface_interaction_node_class::delete_node_entity() {
//printf("hcp_interaction_node_class :: delete_node_entity 000\n");
        if (entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity has invalid id : not deleted.\n");
            return;
        }

        globalc::assign_free_entity_id(entity_id);// Free entity ID number to be reused when a new entity is created

        if (!scene_manager->delete_entity(entity_id, ENTITY_CATEGORY_HEX_SURF)) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity not deleted.\n");
        }else{
            if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Entity deleted.\n");
        }
//printf("hcp_interaction_node_class :: delete_node_entity 1111\n");
    }

    bool hex_surface_interaction_node_class::draw_node(pins_class pins)
    {
//printf("hcp_voxel_node_class::draw_node:: 000\n");
        // Define titlebare colors
        ImNodes::PushColorStyle(ImNodesCol_TitleBar,        IM_COL32(0, 139, 139, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 175, 175, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected,IM_COL32(0, 255, 255, 255));

        ImNodes::BeginNode(node_id);
 //printf("hcp_voxel_node_class::draw_node:: 000\n");
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));// Node header text color
        draw_node_header();
        ImGui::PopStyleColor();
 //printf("hcp_voxel_node_class::draw_node:: 111\n");
        ImGui::Dummy(ImVec2(node_width, node_height));

 //printf("hcp_voxel_node_class::draw_node:: 222\n");

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(60, 10, 250, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(180, 100, 250, 255));
        draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
// printf("hcp_voxel_node_class::draw_node:: 333\n");
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(60, 10, 250, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(180, 100, 250, 255));
        draw_node_input_link(inputs[1], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImGui::Spacing();

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(110, 185, 0, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(150, 255, 0, 255));
        draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

    void hex_surface_interaction_node_class::display_ui(scene_manager_class* scene_manager, log_panel_class* log_panel, id_type current_selected_object_id) {
        float x_pos = 10.0f, y_pos = 70.0f;

        text("HCP Voxel Interactions", x_pos + 70, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("None###hcpvitsn", &interaction_type, HEX_SURFACE_INTERACTIONS_NONE));

        y_pos += 30;
        text("Superposition", x_pos + 90, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Addition###hcpvitsa", &interaction_type, HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Subtraction###hcpvitss", &interaction_type, HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB));

        y_pos += 30;
        text("Logical", x_pos + 120, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("And###hcpvitsi", &interaction_type, HEX_SURFACE_INTERACTIONS_AND));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Or###hcpvitso", &interaction_type, HEX_SURFACE_INTERACTIONS_OR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Exclusive Or###hcpvitse", &interaction_type, HEX_SURFACE_INTERACTIONS_EXOR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Complement###hcpvitsc", &interaction_type, HEX_SURFACE_INTERACTIONS_COMPLEMENT));

        y_pos += 30;
        if (ex_button("Perform interaction###cpvitsp", x_pos + 100, y_pos,150, 20))
            action();

    }

    void hex_surface_interaction_node_class::action() {
        if (interaction_type < 1 || interaction_type >6) return;
        if (scene_manager == NULL) return;

        hex_surface_interaction.scene_manager = scene_manager; // ****** future enhancement to have this function as a static
        hex_surface_interaction.log_panel     = log_panel;     // ****** future enhancement to have this function as a static

        node_basis_class *node_A = find_node_to_modify(inputs[0]);
        node_basis_class *node_B = find_node_to_modify(inputs[1]);

        if (node_A == NULL || node_B == NULL) return;
        if (node_A->node_id == NON_ENTITY_ID || node_A->node_id == NON_ENTITY_ID) return;

        //hex_surface_object_class *hex_surface_object_A = scene_manager->entities_manager.get_hex_surface_entity_object(node_A->entity_id);
        //hex_surface_object_class *hex_surface_object_B = scene_manager->entities_manager.get_hex_surface_entity_object(node_B->entity_id);

        hex_surface_object_class *hex_surface_object_A = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(node_A->entity_id, ENTITY_CATEGORY_HEX_SURF);
        hex_surface_object_class *hex_surface_object_B = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(node_B->entity_id, ENTITY_CATEGORY_HEX_SURF);

        if(hex_surface_object_A == NULL || hex_surface_object_B == NULL) return;

        // Voxel matrices must be of the same size
        if (hex_surface_object_A->hex_surface_object_data.hex_size != hex_surface_object_B->hex_surface_object_data.hex_size) return;

//printf("hcp_interaction_node_class:action 0000 %i\n",entity_id);
        interaction_object = hex_surface_interaction.create_interaction_object(hex_surface_object_A, hex_surface_object_B, entity_id); // ****** future enhancement to have this function as a static
        if (interaction_object == NULL || entity_id == INVALID_ID) return ;
//printf("hcp_interaction_node_class:action 1111 %i\n",entity_id);

        if(!peform_interaction(hex_surface_object_A, hex_surface_object_B, interaction_type)) return;

//printf("hcp_interaction_node_class action 3333\n");
        hex_surface_interaction.update_voxel_verticies(interaction_object); // ****** future enhancement to have this function as a static
//printf("hcp_interaction_node_class action 4444\n");

    }

    node_basis_class * hex_surface_interaction_node_class::find_node_to_modify(int input_pin_id) {
//printf("find_node_to_modify 0000 %i \n", output_pin_id);
        link_struct_type link = graph->find_link_with_end_pin(input_pin_id);
//printf("find_node_to_modify 11111 %i : %i\n", link.link_id, link.to);
        while (link.link_id != INVALID_ID) {
            int to_node_id = graph->pins.find_pin(link.from).node_id;
//printf("find_node_to_modify 2222 %i \n", to_node_id);
            node_basis_class *node = graph->nodes.get_node(to_node_id);
            if (node == NULL) { return NULL; break; }
//printf("find_node_to_modify 33333 %i \n", to_node_id);
            if (node->node_id != NON_ENTITY_ID) {
                // This only works for a voxel generator :  future consideration to cater for other node types !!!!!!
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
            link = graph->find_link_with_start_pin(node->inputs[0]); // This assumes only one input of single type :  future consideration to cater for !!!!!!!
//printf("find_node_to_modify 55555 %i : %i\n", link.link_id, link.to);
        }

        return NULL;
    }

    bool hex_surface_interaction_node_class::peform_interaction(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, int interaction_type) {
//printf("hcp_interaction_node_class:interaction_and 0000 %i\n",entity_id);
        glm::ivec2 a_start, b_start;

        hex_surface_interaction.deterime_interaction_matrix_start_values(hex_surface_object_A, hex_surface_object_B, interaction_object, a_start, b_start);
//printf("hcp_interaction_node_class:interaction_and 11111 %i : %i : %i\n", a_start.x, a_start.y, a_start.z);
//printf("hcp_interaction_node_class:interaction_and 22222 %i : %i : %i\n", b_start.x, b_start.y, b_start.z);
//printf("hcp_interaction_node_class:interaction_and 33333 %f : %f : % f\n", interaction_object->hex_surface_object_data.matrix_origin.x, interaction_object->hex_surface_object_data.matrix_origin.y, interaction_object->hex_surface_object_data.matrix_origin.z);


        // Assign interaction values to interaction matrix
        int i_x_dim = interaction_object->hex_surface_object_data.grid_dimension.x;
        int i_y_dim = interaction_object->hex_surface_object_data.grid_dimension.y;

//printf("hcp_interaction_node_class:interaction_and 44444 %i : %i : % i\n",i_x_dim, i_y_dim, i_z_dim);
        for (int xi = 0; xi < i_x_dim; xi++) {
            for (int yi = 0; yi < i_y_dim; yi++) {         
//printf("hcp_interaction_node_class:interaction_and 4444 %\n");
                // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                hex_surface_data_type a_value = DEFAULT_INACTIVE_VALUE, b_value = DEFAULT_INACTIVE_VALUE;
                hex_index_vector matrix_coord_A = { a_start.x + xi,a_start.y + yi };
                index_data_type a_index = hex_surface_object_A->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_A);
                if (a_index >= 0 && a_index < hex_surface_object_A->hex_surface_object_data.hex_surface_matrix_data.size()) {
                    a_value = hex_surface_object_A->hex_surface_object_data.hex_surface_matrix_data[a_index];
                }

                // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                hex_index_vector matrix_coord_B = { b_start.x + xi,b_start.y + yi  };
                index_data_type b_index = hex_surface_object_B->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_B);
                if (b_index >= 0 && b_index < hex_surface_object_B->hex_surface_object_data.hex_surface_matrix_data.size()) {
                    b_value = hex_surface_object_B->hex_surface_object_data.hex_surface_matrix_data[b_index];
                }

                switch (interaction_type) {
                    case HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD: assign_voxel(xi, yi, a_value + b_value);break;
                    case HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB: assign_voxel(xi, yi, abs(a_value - b_value)); break;
                    case HEX_SURFACE_INTERACTIONS_AND              : {if (a_value != DEFAULT_INACTIVE_VALUE && b_value != DEFAULT_INACTIVE_VALUE) 
                                                                    assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case HEX_SURFACE_INTERACTIONS_OR               : {if (a_value != DEFAULT_INACTIVE_VALUE || b_value != DEFAULT_INACTIVE_VALUE) 
                                                                    assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case HEX_SURFACE_INTERACTIONS_EXOR             : {if ((a_value != DEFAULT_INACTIVE_VALUE) != (b_value != DEFAULT_INACTIVE_VALUE)) 
                                                                    assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case HEX_SURFACE_INTERACTIONS_COMPLEMENT       : {if (!((a_value != DEFAULT_INACTIVE_VALUE) == (b_value != DEFAULT_INACTIVE_VALUE))) 
                                                                    assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                    break; }
                }
                }
            }


        return true;
    }

    void hex_surface_interaction_node_class::assign_voxel(int xi, int yi, hex_surface_data_type value) {
        hex_index_vector matrix_coord_C = { xi,yi };
        index_data_type c_index = interaction_object->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_C);
        // Future consideration is that this value is assigned to some other value
        if (c_index >= 0 && c_index < interaction_object->hex_surface_object_data.hex_surface_matrix_data.size()) {
            interaction_object->hex_surface_object_data.hex_surface_matrix_data[c_index] = value;
        }
    }