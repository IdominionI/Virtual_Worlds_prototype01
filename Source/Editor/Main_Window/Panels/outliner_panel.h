#pragma once

#include "../../Common/definitions.h"

#include "log_panel.h" 

#include <Universal/ImGui/imgui.h>
#include <Universal/3rdParty/Plugins/ImGui/ImFileBrowser.h>

#include "../Widgets/imgui_widgets.h"

#include "../Outliner/outliner_manager.h"

/*
							OUTLINER PANEL

	Parent class to define the application outliner panel that is to
	have the application outliner and any other desired widget defined
	within it. This panel is a detachable window of the docking feature
	available in he ImGui frame work.
*/

class outliner_panel_class{
public:
	outliner_panel_class() {}

	~outliner_panel_class() {}

	outliner_manager_class outliner_manager;

	void show(log_panel_class *log_panel = NULL) {
//if (log_panel == NULL) {
//	printf("CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n");
//	return;
//}

		ImGui::Begin("Scene Outliner");

		outliner_manager.display_nodes(log_panel);

		ImGui::End();
	}

	//void hide() {
		// Not implemented
	//}

private:
	log_panel_class *log_panel;
};