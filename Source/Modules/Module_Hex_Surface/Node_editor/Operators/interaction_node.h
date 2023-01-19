#pragma once

#include <Source/Editor/Scene/scene_manager.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Node_editor/ImNodes/Kernal/graph.h>
#include <Source/Editor/Common/global.h>

#include "../hex_surface_node_definitions.h"
#include "../../Functions/hex_surface_interactions.h"

#define HEX_SURFACE_INTERACTIONS_NONE               0
#define HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD  1
#define HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB  2
#define HEX_SURFACE_INTERACTIONS_UNION              3
#define HEX_SURFACE_INTERACTIONS_AND                4
#define HEX_SURFACE_INTERACTIONS_OR                 5
#define HEX_SURFACE_INTERACTIONS_EXOR               6
#define HEX_SURFACE_INTERACTIONS_COMPLEMENT         7

class hex_surface_interaction_node_class : public node_basis_class {
	public:
    //hcp_voxel_translation_node_class(graph_class *graph_) : graph(graph_) { This creates a copile error
    hex_surface_interaction_node_class() {
        node_width = 60.0f;
    }

    // Not sure just yet, but scene_manager and log_panel may need to be defined in the node basis class if
    // found that most or all node classes require it
    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel     = NULL;

    int interaction_type;

    bool define_node(ImVec2 click_pos,id_type  entity_id);
    bool define_inputs(pins_class& pins);
    bool define_outputs(pins_class& pins);
    void delete_node_entity();
    bool draw_node(pins_class pins);
    void display_ui(scene_manager_class* scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1);
    void action();
    node_basis_class* find_node_to_modify(int input_pin_id);
    bool peform_interaction(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, int interaction_type);
    void assign_voxel(int xi, int yi, hex_surface_data_type value);

protected:
private:
    hex_surface_object_class *interaction_object = NULL;

    hex_surface_interactions_class hex_surface_interaction;
};
