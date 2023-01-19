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

    void display();

private:
    bool menu_selection = false;

    void create_entity_node(ImVec2 click_pos);
    void create_node(int entity_category, int entity_context, int entity_function, ImVec2 click_pos);

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hcp_voxel_node_class* hcp_voxel_node;
    hcp_voxel_translation_node_class* hcp_voxel_translation_node;
    hcp_interaction_node_class* hcp_voxel_interaction_node;

    void add_hcp_voxel_node(int entity_context, int entity_function, ImVec2 click_pos);
    void add_hcp_matrix_node(int entity_function, ImVec2 click_pos);
    void add_hcp_voxel_node(ImVec2 click_pos);

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // !!!!!!!!!!!!!!!!!!! hcp Voxel node definitions, data and functions !!!!!!!!!!!!!!!!!!!!!!!!!!
    hex_surface_node_class* hex_surface_node;
    hex_surface_translation_node_class* hex_surface_translation_node;
    hex_surface_interaction_node_class* hex_surface_interaction_node;

    void add_hex_surface_node(int entity_context, int entity_function, ImVec2 click_pos);
    void add_hex_surface_node(int entity_function, ImVec2 click_pos);
    void add_hex_surface_node(ImVec2 click_pos);

    // ^^^^^^^^^^^^^^^^^^^^ End hcp Voxel node definitions, data and functions ^^^^^^^^^^^^^^^^^^^^

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links();
    void handle_deleted_links();

};
