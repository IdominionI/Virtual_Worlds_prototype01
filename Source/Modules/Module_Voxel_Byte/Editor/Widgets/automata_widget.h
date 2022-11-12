#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include <Source/Editor/Scene/scene_manager.h>

#include "../../Voxel_hcp_object/voxel_hcp_object.h"

#include "automata_rules_widget.h"
#include "../../Automata/automata_byte_import_export.h"
#include "../../Automata/voxel_automata_functions.h"

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

class voxel_hcp_autmoata_widget_class {
public:
	voxel_hcp_autmoata_widget_class() {}
	~voxel_hcp_autmoata_widget_class() {}

	log_panel_class *log_panel = NULL;
	
	hcp_automata_rules_widget_class  hcp_automata_rules_widget;

	id_type                 current_selected_entity_id = -1;   // entity id of the selected entity to display/modify
	voxel_hcp_object_class *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	scene_manager_class* scene_manager = NULL;

	// Not sure just yet which of these need go private
	bool use_multithreading = false;
	bool animate_automata   = false;
	bool automata_stopped   = true;

	void display() {
		if (voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR:: Cannot define/execute an automata voxel hcp rule. No voxel hcp object defined.\n");
			return;
		}

		float x_pos = 10.0f, y_pos = 70.0f;

		title("Voxel HCP Automata");


		y_pos += 20;

		ImGui::SetCursorPosX(x_pos + 10);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Use\nMultithreading###vaumt", &use_multithreading);

		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Animate\nAutomata###vauanim", &voxel_hcp_object_to_execute->animate_automata);

		y_pos += 35;

		text("Automata Rules", x_pos + 100, y_pos);

		ImGui::BeginChild("shader_variables", ImVec2(320, 400.0f), true);

		hcp_automata_rules_widget.rules_manager(voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules);

		ImGui::EndChild();

		y_pos += 425;
		float x_pos2 = x_pos + 50;

		if (ex_button(ICON_FA_STOP "###vausb", x_pos2 , y_pos, 40, 23))
			stop_automata_execution();

		if (ex_button(ICON_FA_PAUSE "###vaupab", x_pos2+43, y_pos, 40, 23))
			pause_automata_execution();

		if (ex_button(ICON_FA_CIRCLE "###vaurb", x_pos2+86, y_pos, 40, 23))//ICON_FA_CIRCLE - ICON_FA_RECORD_VINAL
			record_automata_execution();

		if (ex_button(ICON_FA_PLAY "###vauplb", x_pos2+129, y_pos, 40, 23))
			play_automata_execution();

		if (ex_button(ICON_FA_STEP_FORWARD "###vausfb", x_pos2+171, y_pos, 40, 23))
			next_step_automata_execution();

		y_pos += 30;

		// Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
		// or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
		//ImGui::ProgressBar(progress, ImVec2(200.0f, 0.0f));
		//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		//ImGui::Text("Progress Bar");

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

		float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
		char buf[32];
		sprintf_s(buf, "%d/%d", (int)(progress_saturated * number_automata_steps), number_automata_steps);

		ImGui::SetCursorPosX(x_pos+50);
		ImGui::SetCursorPosY(y_pos);
		ImGui::ProgressBar(progress, ImVec2(210.f, 0.f), buf);


		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###vausrb", x_pos+20, y_pos, 80, 20))
			save_automata_rules();

		if (ex_button("Clear Rules###vaucrb", x_pos + 110, y_pos, 100, 20))
			clear_automata_rules();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###vauirb", x_pos + 220, y_pos, 80, 20))
			load_automata_rules();

		// This here to easily be able to pause stop automata process perform this statement
		if (automata_play) {
			if (!use_multithreading) {
				if (current_automata_step < number_automata_steps) {
					if (!automata_paused) {
						if (!perform_automata_step(current_automata_step)) {
							if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
							stop_automata_execution();
						} else{
							current_automata_step++;
							progress = ((float)current_automata_step / (float)number_automata_steps);
						}
					}
				}
			} else { // Use multithreading

			}
		}

	}

	void clear_automata_rules() {
		voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules.clear();
	}

	void save_automata_rules() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*ABYTER.txt" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to save voxel hcp automata rules data to\n");
			return;
		}
		else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("save_automata_rules != NULL  \n");

		import_export_byte_automata_rules.byte_automata_rules = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;
		import_export_byte_automata_rules.export_hcp_automata_byte_rules(file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel automata rules data saved to file\n %s\n", file_pathname);
	}


	void load_automata_rules() {
//printf("load button clicked\n");// replace with clear variables
		clear_automata_rules();

		char const* patterns[] = { "*ABYTER.txt" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to import voxel generation parameter data from.\n");
			return;
		}
		else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("load_automata_rules != NULL  \n");

		import_export_byte_automata_rules.import_hcp_automata_byte_rules(voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules, file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel automata rules data imported from file\n %s\n", file_pathname);
	}

	void stop_automata_execution() {
		automata_stopped      = true;
		current_automata_step = 0;
		progress			  = 0.0f;
		automata_paused       = false;

		automata_play         = false;
	}

	void pause_automata_execution() {
//printf("paused button pressed\n");
		automata_paused = !automata_paused;
//if(automata_paused == false)
//printf("automata_paused == false\n");
//else
//printf("automata_paused == true\n");
	}

	void record_automata_execution() {
		// not yet implemented
	}

	void play_automata_execution() {
		if (!automata_play) {
			if (setup_play())
				automata_play = true;
		}
	}

	void next_step_automata_execution() {
		if(use_multithreading)
			thread_next_step_voxel_automata();
		else
			next_step_voxel_automata_no_thread();
	}


	// ****
	bool setup_play() {
//printf("setup_play 00");

		if (voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Play_voxel_automata_no_thread() :: No voxel hcp object specified to execute.\n");
			return false;
		}

		automata_stopped      = false;
		automata_paused       = false;
		automata_play		  = true;
		current_automata_step = 0;

		//Save Automata rules to execute for selected voxel object
		//voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules = get_voxel_automata_rules();
		//save_current_automata_data()

		voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.cloud                       = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.voxel_hcp_automata_rules    = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;

		number_automata_steps = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

		return true;
	}
	// ****



	void next_step_voxel_automata_no_thread() {
//printf("INFO", "next_step_voxel_automata 00");

		if (current_selected_entity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR", "next_step_voxel_automata_no_thread():: No voxel hcp object specified to execute");
			return;
		}

//editor_logging_panel.add_log_message("INFO","next_step_voxel_automata 01 : ");

		automata_stopped = false;

		// Saving Voxel automata data not needed as the automata data is directly saved and stord by this editor
		//Save Automata rules to execute for selected voxel object
		//save_current_automata_data()

		// set automata object to execute automata function data on
		voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.cloud                       = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.voxel_hcp_automata_rules    = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;
//printf("next_step_voxel_automata 02 : ");

		// following is to update the progress bar
		int max_step_value = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

//printf("next_step_voxel_automata 03 : %i ",max_step_value);

		number_automata_steps = max_step_value;

		if (current_automata_step <= max_step_value)
			if (perform_automata_step(current_automata_step)) {
				current_automata_step++;
				progress =  ((float)current_automata_step/ (float) max_step_value);
			}

		automata_stopped = true;
	}

	bool perform_automata_step(int step) {
//printf("INFO", "perform_automata_step 00 : %i\n" , step);

		if (!voxel_automata_hcp_functions.perform_voxel_automata_generation(step)) return false;
//printf("INFO","perform_automata_step 01 : %i",step);

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);
//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 11111\n");

		if (scene_voxel_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Automata : Could not find voxel in the scene to update geometry data.\n");
//printf("voxel_hcp_autmoata_widget_class 22222 scene_voxel_object == NULL.\n");
			return false;
		}
		else {
//printf("voxel_hcp_autmoata_widget_class 33333 scene_voxel_object != NULL.\n");

			if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Voxel Automata : Scene voxel object geometry could not be updated.\n");
//printf("voxel_hcp_autmoata_widget_class 44444 not updated\n");
				return false;
			}

//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 55555\n");
		}
		return true;
	}


	void thread_next_step_voxel_automata() {
printf ("thread_next_step_voxel_automata 00");
/*
		if (current_selected_enity_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR", "next_step_voxel_automata_thread():: No voxel hcp object specified to execute");
			return;
		}

printf("thread_next_step_voxel_automata 01 : ");

		automata_stopped = false;

		// Saving Voxel automata data not needed as the automata data is directly saved and stord by this editor
		//Save Automata rules to execute for selected voxel object
		//save_current_automata_data()

		// set automata object to execute automata function data on
		voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.cloud                       = voxel_hcp_object_to_execute;
		voxel_automata_hcp_functions.voxel_hcp_automata_rules    = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;
//printf("thread_next_step_voxel_automata 02 : ");

		// following is to update the progress bar
		int max_step_value = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

//printf("thread_next_step_voxel_automata 03 : %i ",max_step_value);

		number_automata_steps = max_step_value;

		if (current_automata_step <= max_step_value)
			if (thread_perform_automata_step(current_automata_step)) {
				current_automata_step++;
				progress =  ((float)current_automata_step/ (float) max_step_value);
			}
*/
		automata_stopped = true;

printf("thread_next_step_voxel_automata 04 : ");
	}

	bool thread_perform_automata_step(int step) {
printf("INFO thread_perform_automata_step 00 : %i\n" , step);
/*
		if (!voxel_automata_hcp_thread_functions.perform_voxel_automata_generation(step)) return false;
//printf("INFO","perform_automata_step 01 : %i",step);

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_enity_id);
//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 11111\n");

		if (scene_voxel_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Automata : Could not find voxel in the scene to update geometry data.\n");
//printf("voxel_hcp_autmoata_widget_class 22222 scene_voxel_object == NULL.\n");
			return false;
		}
		else {
//printf("voxel_hcp_autmoata_widget_class 33333 scene_voxel_object != NULL.\n");

			if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Voxel Automata : Scene voxel object geometry could not be updated.\n");
//printf("voxel_hcp_autmoata_widget_class 44444 not updated\n");
				return false;
			}

//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 55555\n");
		}
*/
		return true;
	}

private:
	float progress = 0.0f;
	int   number_automata_steps = 0, current_automata_step = 0;
	bool  automata_paused = false;
	bool  automata_play   = false;

	import_export_byte_automata_rules_class   import_export_byte_automata_rules;
	voxel_automata_hcp_functions_class        voxel_automata_hcp_functions;
	//voxel_automata_hcp_thread_functions_class voxel_automata_hcp_thread_functions;

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