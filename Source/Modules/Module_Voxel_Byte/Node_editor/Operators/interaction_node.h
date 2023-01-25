#pragma once

#include <Source/Editor/Node_editor/ImNodes/Kernal/graph.h>

#include "../hcp_voxel_node_definitions.h"

#include "../../Functions/voxel_interactions.h"

#define VOXEL_INTERACTIONS_NONE               0
#define VOXEL_INTERACTIONS_SUPERPOSITION_ADD  1
#define VOXEL_INTERACTIONS_SUPERPOSITION_SUB  2
#define VOXEL_INTERACTIONS_UNION              3
#define VOXEL_INTERACTIONS_AND                4
#define VOXEL_INTERACTIONS_OR                 5
#define VOXEL_INTERACTIONS_EXOR               6
#define VOXEL_INTERACTIONS_COMPLEMENT         7

class hcp_interaction_node_class : public node_basis_class {
public:
    //hcp_voxel_translation_node_class(graph_class *graph_) : graph(graph_) { This creates a copile error
        hcp_interaction_node_class() {
        node_width = 60.0f;
    }

    // Not sure just yet, but scene_manager and log_panel may need to be defined in the node basis class if
    // found that most or all node classes require it
    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel     = NULL;

    int interaction_type;

    bool define_node(ImVec2   click_pos,
                     id_type  entity_id) {

        entity_id = entity_id;

        ui_node_type.category          = ENTITY_CATEGORY_HCP_VOXEL;
        ui_node_type.context           = HCP_CONTEXT_VOXEL_MATRIX;
        ui_node_type.function_category = OPERATOR;
        ui_node_type.function          = HCP_VOXEL_MATRIX_OPERATOR;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }

    bool define_inputs(pins_class& pins) {
        if(!define_node_input_value_of_type("Matrix A", ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, OPERATOR, HCP_VOXEL_MATRIX_OPERATOR, pin_type_enum::single_input, pins))
            return false;

        return define_node_input_value_of_type("Matrix B", ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, OPERATOR, HCP_VOXEL_MATRIX_OPERATOR, pin_type_enum::single_input, pins);
    }

    bool define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Matrix", ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, GENERAL, HCP_VOXEL_MATRIX_VALUE, pin_type_enum::single_output,  pins);
    }

    void delete_node_entity() {
//printf("hcp_interaction_node_class :: delete_node_entity 000\n");
        if (entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity has invalid id : not deleted.\n");
            return;
        }

        globalc::assign_free_entity_id(entity_id);// Free entity ID number to be reused when a new entity is created

        if (!scene_manager->delete_entity(entity_id, ENTITY_CATEGORY_HCP_VOXEL)) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity not deleted.\n");
        }else{
            if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Entity deleted.\n");
        }
//printf("hcp_interaction_node_class :: delete_node_entity 1111\n");
    }

    bool draw_node(pins_class pins)
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

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(100, 30, 150, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(160, 40, 230, 255));
        draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
// printf("hcp_voxel_node_class::draw_node:: 333\n");
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(100, 30, 150, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(160, 40, 230, 255));
        draw_node_input_link(inputs[1], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        ImGui::Spacing();

        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 170, 115, 255));
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(0, 230, 160, 255));
        draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

    void display_ui(scene_manager_class* scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1) {
        float x_pos = 10.0f, y_pos = 70.0f;

        text("HCP Voxel Interactions", x_pos + 70, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("None###hcpvitsn", &interaction_type, VOXEL_INTERACTIONS_NONE));

        y_pos += 30;
        text("Superposition", x_pos + 90, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Addition###hcpvitsa", &interaction_type, VOXEL_INTERACTIONS_SUPERPOSITION_ADD));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Subtraction###hcpvitss", &interaction_type, VOXEL_INTERACTIONS_SUPERPOSITION_SUB));

        y_pos += 30;
        text("Logical", x_pos + 120, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("And###hcpvitsi", &interaction_type, VOXEL_INTERACTIONS_AND));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Or###hcpvitso", &interaction_type, VOXEL_INTERACTIONS_OR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Exclusive Or###hcpvitse", &interaction_type, VOXEL_INTERACTIONS_EXOR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Complement###hcpvitsc", &interaction_type, VOXEL_INTERACTIONS_COMPLEMENT));

        y_pos += 30;
        if (ex_button("Perform interaction###cpvitsp", x_pos + 100, y_pos,150, 20))
            action();

    }

    void action() {
        if (interaction_type < 1 || interaction_type >6) return;
        if (scene_manager == NULL) return;

        voxel_interaction.scene_manager = scene_manager; // ****** future enhancement to have this function as a static
        voxel_interaction.log_panel     = log_panel;     // ****** future enhancement to have this function as a static

        node_basis_class *node_A = find_node_to_modify(inputs[0]);
        node_basis_class *node_B = find_node_to_modify(inputs[1]);

        if (node_A == NULL || node_B == NULL) return;
        if (node_A->node_id == NON_ENTITY_ID || node_A->node_id == NON_ENTITY_ID) return;

        //voxel_hcp_object_class *voxel_hcp_object_A = scene_manager->entities_manager.get_voxel_hcp_entity_object(node_A->entity_id);
        //voxel_hcp_object_class *voxel_hcp_object_B = scene_manager->entities_manager.get_voxel_hcp_entity_object(node_B->entity_id);

        voxel_hcp_object_class* voxel_hcp_object_A = (voxel_hcp_object_class*)scene_manager->entities_manager.get_entity_of_category(node_A->entity_id, ENTITY_CATEGORY_HCP_VOXEL);
        voxel_hcp_object_class* voxel_hcp_object_B = (voxel_hcp_object_class*)scene_manager->entities_manager.get_entity_of_category(node_B->entity_id, ENTITY_CATEGORY_HCP_VOXEL);

        if(voxel_hcp_object_A == NULL || voxel_hcp_object_B == NULL) return;

        // Voxel matrices must be of the same size
        if (voxel_hcp_object_A->voxel_object_data.voxel_size != voxel_hcp_object_B->voxel_object_data.voxel_size) return;

printf("hcp_interaction_node_class:action 0000 %i\n",entity_id);
        interaction_object = voxel_interaction.create_interaction_object(voxel_hcp_object_A, voxel_hcp_object_B, entity_id); // ****** future enhancement to have this function as a static
        if (interaction_object == NULL || entity_id == INVALID_ID) return ;
printf("hcp_interaction_node_class:action 1111 %i\n",entity_id);

        if(!peform_interaction(voxel_hcp_object_A, voxel_hcp_object_B, interaction_type)) return;

//printf("hcp_interaction_node_class action 3333\n");
        voxel_interaction.update_voxel_verticies(interaction_object); // ****** future enhancement to have this function as a static
//printf("hcp_interaction_node_class action 4444\n");

    }

       node_basis_class *find_node_to_modify(int input_pin_id) {
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
                if(node->ui_node_type.category          == ENTITY_CATEGORY_HCP_VOXEL &&
                   node->ui_node_type.context           == HCP_CONTEXT_VOXEL_MATRIX &&
                   node->ui_node_type.function_category == GENERATOR &&
                   node->ui_node_type.function          == HCP_VOXEL_MATRIX_VALUE)
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

    bool peform_interaction(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B, int interaction_type) {
//printf("hcp_interaction_node_class:interaction_and 0000 %i\n",entity_id);
        glm::ivec3 a_start, b_start;

        voxel_interaction.deterime_interaction_matrix_start_values(voxel_hcp_object_A, voxel_hcp_object_B, interaction_object, a_start, b_start);
//printf("hcp_interaction_node_class:interaction_and 11111 %i : %i : %i\n", a_start.x, a_start.y, a_start.z);
//printf("hcp_interaction_node_class:interaction_and 22222 %i : %i : %i\n", b_start.x, b_start.y, b_start.z);
//printf("hcp_interaction_node_class:interaction_and 33333 %f : %f : % f\n", interaction_object->voxel_object_data.matrix_origin.x, interaction_object->voxel_object_data.matrix_origin.y, interaction_object->voxel_object_data.matrix_origin.z);


        // Assign interaction values to interaction matrix
        int i_x_dim = interaction_object->voxel_object_data.matrix_dimension.x;
        int i_y_dim = interaction_object->voxel_object_data.matrix_dimension.y;
        int i_z_dim = interaction_object->voxel_object_data.matrix_dimension.z;

//printf("hcp_interaction_node_class:interaction_and 44444 %i : %i : % i\n",i_x_dim, i_y_dim, i_z_dim);
        for (int xi = 0; xi < i_x_dim; xi++) {
            for (int yi = 0; yi < i_y_dim; yi++) {
                for (int zi = 0; zi < i_z_dim; zi++) {

//printf("hcp_interaction_node_class:interaction_and 4444 %\n");
                    // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                    voxel_data_type a_value = DEFAULT_INACTIVE_VALUE, b_value = DEFAULT_INACTIVE_VALUE;
                    glm::ivec3 matrix_coord_A = { a_start.x + xi,a_start.y + yi ,a_start.z + zi };
                    index_data_type a_index = voxel_hcp_object_A->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_A);
                    if (a_index >= 0 && a_index < voxel_hcp_object_A->voxel_object_data.voxel_matrix_data.size()) {
                        a_value = voxel_hcp_object_A->voxel_object_data.voxel_matrix_data[a_index];
                    }

                    // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                    glm::ivec3 matrix_coord_B = { b_start.x + xi,b_start.y + yi ,b_start.z + zi };
                    index_data_type b_index = voxel_hcp_object_B->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_B);
                    if (b_index >= 0 && b_index < voxel_hcp_object_B->voxel_object_data.voxel_matrix_data.size()) {
                        b_value = voxel_hcp_object_B->voxel_object_data.voxel_matrix_data[b_index];
                    }

                    switch (interaction_type) {
                        case VOXEL_INTERACTIONS_SUPERPOSITION_ADD: assign_voxel(xi, yi, zi, a_value + b_value);break;
                        case VOXEL_INTERACTIONS_SUPERPOSITION_SUB: assign_voxel(xi, yi, zi, abs(a_value - b_value)); break;
                        case VOXEL_INTERACTIONS_AND              : {if (a_value != DEFAULT_INACTIVE_VALUE && b_value != DEFAULT_INACTIVE_VALUE) 
                                                                      assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                       break;}
                        case VOXEL_INTERACTIONS_OR               : {if (a_value != DEFAULT_INACTIVE_VALUE || b_value != DEFAULT_INACTIVE_VALUE) 
                                                                      assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                       break;}
                        case VOXEL_INTERACTIONS_EXOR             : {if ((a_value != DEFAULT_INACTIVE_VALUE) != (b_value != DEFAULT_INACTIVE_VALUE)) 
                                                                      assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                       break;}
                        case VOXEL_INTERACTIONS_COMPLEMENT       : {if (!((a_value != DEFAULT_INACTIVE_VALUE) == (b_value != DEFAULT_INACTIVE_VALUE))) 
                                                                      assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                      break; }
                    }
                }
            }
        }

        return true;
    }

    void assign_voxel(int xi, int yi, int zi,int value) {
        glm::ivec3 matrix_coord_C = { xi,yi ,zi };
        index_data_type c_index = interaction_object->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_C);
        // Future consideration is that this value is assigned to some other value
        if (c_index >= 0 && c_index < interaction_object->voxel_object_data.voxel_matrix_data.size()) {
            interaction_object->voxel_object_data.voxel_matrix_data[c_index] = value;
        }
    }

protected:
private:
    voxel_hcp_object_class *interaction_object = NULL;

    hcp_voxel_interactions_class voxel_interaction;
};
