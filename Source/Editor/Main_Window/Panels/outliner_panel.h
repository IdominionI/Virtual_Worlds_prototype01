#pragma once

#include "../../Common/definitions.h"
//#include "../../Scene/vw_scene_db.h"

#include "log_panel.h" // ****

#include <Universal/ImGui/imgui.h>
#include "../Widgets/imgui_widgets.h"
#include <Universal/3rdParty/Plugins/ImGui/ImFileBrowser.h>

#include "../../Kernal/outliner_manager.h"

class outliner_panel_class{
public:
	outliner_panel_class() {

	}

	~outliner_panel_class() {}

	outliner_manager_class outliner_manager;

	//id_type current_selected_object = -1;
	//scene_db_class *scene_db = NULL;

	//void show(scene_db_class *scene_db) {
	void show(log_panel_class *log_panel = NULL) {
		//if (log_panel == NULL) {
		//	printf("CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n");
		//	return;
		//}

		ImGui::Begin("Scene Outliner");

		outliner_manager.display_nodes(log_panel);

		ImGui::End();
	}

	void hide() {

	}

private:
	log_panel_class* log_panel;

};