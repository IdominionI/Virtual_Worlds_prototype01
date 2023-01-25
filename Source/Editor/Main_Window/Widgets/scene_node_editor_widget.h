#pragma once
#include "../../Node_editor/ImNodes/Kernal/node_editor.h"
#include "../../Scene/scene_manager.h"
#include "../../Node_editor/node_editor_category_manager.h"

class scene_node_editor_class : public node_editor_basis_class {
public:
    node_editor_category_manager_class *node_editor_category_manager;

    node_tree_class_basis node_tree;

    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel = NULL;

    node_basis_class    *selected_node = NULL;
    link_struct_type    selected_link ;

    void display();

private:
    bool menu_selection = false;

    void create_node(int category, int context, int function, int function_category, ImVec2 click_pos);

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links();
    void handle_deleted_links();

};
