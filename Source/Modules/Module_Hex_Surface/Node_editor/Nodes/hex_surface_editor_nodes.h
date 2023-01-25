#pragma once

#include <Source/Editor/Node_editor/node_editor_basis.h>
//#include <Source/Editor/Node_editor/ImNodes/Kernal/graph.h>

#include "../../Hex_surface_object/hex_surface_object.h"
#include "../hex_surface_node.h"

#include <Source/Editor/Common/global.h>

class hex_surface_editor_node_class : public editor_node_basis {
public:

    void create_editor_node(node_type_struct ui_node_type, graph_class *graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {
//printf("hex_surface_editor_node_class::create_editor_node:: 00000\n");

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

        if (graph->node_exists(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, globalc::get_current_selected_entity_id())) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Have entity node for selected entity defined.\n");
            return;
        }

        hex_surface_object_class *hex_object = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(globalc::get_current_selected_entity_id(), ENTITY_CATEGORY_HEX_SURF);

        if (hex_object != NULL) {
//printf("hex_surface_editor_node_class::add_hcp_voxel: 1111 %i \n", voxel_hcp_object->object_id);
            hex_node            = new hex_surface_node_class;
            hex_node->entity_id = hex_object->object_id;
            hex_node->label     = hex_object->object_name;
            graph->add_node(hex_node, click_pos, globalc::get_current_selected_entity_id());
        }
        else {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No entity of defined id exists within the scene.\n");
        }
//printf("hex_surface_editor_node_class::create_editor_node:: EEEEE\n");
    }

private:
    hex_surface_node_class *hex_node;

};

#include "../Modifiers/translation_node.h"

class hex_surface_editor_translation_node_class : public editor_node_basis {
public:

    void create_editor_node(node_type_struct ui_node_type, graph_class *graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_voxel:: 00000\n");

        hex_translation_node                = new hex_surface_translation_node_class;
        hex_translation_node->label         = "Translation";
        hex_translation_node->scene_manager = scene_manager;
        hex_translation_node->log_panel     = log_panel;
        graph->add_node(hex_translation_node, click_pos, NON_ENTITY_ID);
    }

private:
    hex_surface_translation_node_class *hex_translation_node;
};

#include "../Operators/interaction_node.h"

class hex_surface_editor_interaction_node_class : public editor_node_basis {
public:

    void create_editor_node(node_type_struct ui_node_type, graph_class *graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {
//printf("scene_node_editor_class::add_hcp_voxel:: 00000\n");
        
        hex_interaction_node                = new hex_surface_interaction_node_class;
        hex_interaction_node->label         = "Interaction";
        hex_interaction_node->scene_manager = scene_manager;
        hex_interaction_node->log_panel     = log_panel;
        graph->add_node(hex_interaction_node, click_pos, NON_ENTITY_ID);

    }

private:
    hex_surface_interaction_node_class *hex_interaction_node;
};
