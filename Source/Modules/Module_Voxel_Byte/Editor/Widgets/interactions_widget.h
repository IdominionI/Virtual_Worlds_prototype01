#pragma once
#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#define VOXEL_INTERACTIONS_NONE                 0
#define VOXEL_INTERACTIONS_SUPERPOSITION_ADD    1
#define VOXEL_INTERACTIONS_SUPERPOSITION_SUB    2
#define VOXEL_INTERACTIONS_BOOLEAN_UNION        3
#define VOXEL_INTERACTIONS_BOOLEAN_INTERSECTION 4
#define VOXEL_INTERACTIONS_BOOLEAN_EXOR         5
#define VOXEL_INTERACTIONS_BOOLEAN_COMPLEMENT   6

class voxel_hcp_interaction_widget_class {
public:
	voxel_hcp_interaction_widget_class() {
	}

	~voxel_hcp_interaction_widget_class() {}

	generator_variables_widget_class  generator_variables_widget_class;
	log_panel_class				     *log_panel = NULL;

	id_type                  current_selected_entity_id = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class     *scene_manager = NULL;

	int interaction_type;

	void display() {
		float x_pos = 10.0f, y_pos = 70.0f;

		text("HCP Voxel Interactions", x_pos+70, y_pos);
		
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
		text("Set Boolean", x_pos + 120, y_pos);

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		(ImGui::RadioButton("Union###hcpvitsu", &interaction_type, VOXEL_INTERACTIONS_BOOLEAN_UNION));

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		(ImGui::RadioButton("Intersection###hcpvitsi", &interaction_type, VOXEL_INTERACTIONS_BOOLEAN_INTERSECTION));

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		(ImGui::RadioButton("Exclusive Or###hcpvitse", &interaction_type, VOXEL_INTERACTIONS_BOOLEAN_EXOR));

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		(ImGui::RadioButton("Complement###hcpvitsc", &interaction_type, VOXEL_INTERACTIONS_BOOLEAN_COMPLEMENT));

	}



private:


};