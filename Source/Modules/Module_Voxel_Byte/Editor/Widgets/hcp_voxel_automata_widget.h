#pragma once

#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Scene/scene_manager.h>

#include "../../Voxel_hcp_object/voxel_hcp_object.h"

#include "hcp_voxel_automata_rules_widget.h"
#include "../../Automata/automata_byte_import_export.h"
#include "../../Automata/voxel_automata_functions.h"

#include "../../Render/voxel_hcp_render.h"

/*
	HCP voxel volume cellula automata rules widget class

	This class widget defines a ImGui widget and child widgets
	that are used to define one or more sets of cellula automata
	rules that the HCP voxel volume model can be subjected to 
	perform.

	Also this widget class has controls to perform the cellula
	automata rules one incremental step at a time according to the
	defined cellula automata rules that are defined here.
*/

class voxel_hcp_automata_widget_class {
public:
	log_panel_class *log_panel = NULL;
	
	hcp_automata_rules_widget_class  hcp_automata_rules_widget;

	id_type                 current_selected_entity_id = -1;   // entity id of the selected entity to display/modify
	voxel_hcp_object_class *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	scene_manager_class* scene_manager = NULL;

	// Not sure just yet which of these need go private
	bool use_multithreading = false;
	bool animate_automata   = false;
	bool automata_stopped   = true;

	void display();
	void clear_automata_rules();
	void save_automata_rules();
	void load_automata_rules();
	void stop_automata_execution();
	void pause_automata_execution();
	void record_automata_execution();
	void play_automata_execution();
	void next_step_automata_execution();
	bool perform_automata_step(int step);
	bool setup_play();
	void next_step_voxel_automata_no_thread();

	void thread_next_step_voxel_automata();
	bool thread_perform_automata_step(int step);
	void automata_functions_logging(log_panel_class* log_panel = NULL);

private:
	float progress = 0.0f;
	int   number_automata_steps = 0, current_automata_step = 0;
	bool  automata_paused = false;
	bool  automata_play   = false;

	import_export_byte_automata_rules_class   import_export_byte_automata_rules;
	voxel_automata_hcp_functions_class        voxel_automata_hcp_functions;

	voxel_hcp_render_class					voxel_hcp_render;
};

/*
	Following kept as reference to if a method can be found to be able to pause continue or stop a automation process
	void play_voxel_automata_no_thread() {
printf("play_voxel_automata_no_thread 00");

		if (voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Play_voxel_automata_no_thread() :: No voxel hcp object specified to execute.\n");
			return;
		}

		automata_stopped      = false;
		automata_paused       = false;
		current_automata_step = 0;

		//Save Automata rules to execute for selected voxel object
		//voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules = get_voxel_automata_rules();
		//save_current_automata_data()

		voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.cloud                       = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.voxel_hcp_automata_rules    = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;

		int max_step_value = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();
		int step;
		number_automata_steps = max_step_value;

		//editor_logging_panel.add_log_message("INFO","before play loop");
		for (step = 0; step <= max_step_value ; step++) {
			   // CRITICAL : It seems QML does not have a method as in Qt as next statement to pause or abort from a loop !!!!!
				//            Until find a method, may need to disable play_voxel_automata_no_thread .
				//QApplication::processEvents(); // This is required to eanble signal and slots events to be processed within a loop and allow other actions to take place.

				//if (automata_stopped) {editor_logging_panel.add_log_message("INFO","automata  loop stopped") break;}

				//if (automata_paused) {
				//editor_logging_panel.add_log_message("INFO","automata loop paused")
					//do {
						// CRITICAL : It seems QML does not have a method as in Qt as next statement to pause or abort from a loop !!!!!
						//            Until find a method, may need to disable play_voxel_automata_no_thread.
						//QApplication::processEvents();
					//} while (automata_paused && !automata_stopped);
				//}
				

printf("play loop : %i\n", step);
if (!perform_automata_step(step)) {
	if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
	break;
}

progress = ((float)step / (float)max_step_value);
		}

		current_automata_step = 0; // all automata steps completed reset current_automata_step to zero
	}
*/