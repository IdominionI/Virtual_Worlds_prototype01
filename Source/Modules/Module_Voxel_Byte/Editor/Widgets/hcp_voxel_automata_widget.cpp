#include "hcp_voxel_automata_widget.h"


#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Interface/IconsFontAwesome.h>
#include <Source/Editor/Tools/dialogs.h>

void voxel_hcp_automata_widget_class::display() {
	if (voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR:: Cannot define/execute an automata voxel hcp rule. No voxel hcp object defined.\n");
		return;
	}

	float x_pos = 10.0f, y_pos = 70.0f;

	title("Voxel HCP Automata");


	y_pos += 20;

	//ImGui::SetCursorPosX(x_pos + 10);
	//ImGui::SetCursorPosY(y_pos);
	//ImGui::Checkbox("Use\nMultithreading###vaumt", &use_multithreading);

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

	if (ex_button(ICON_FA_STOP "###vausb", x_pos2, y_pos, 40, 23))
		stop_automata_execution();

	if (ex_button(ICON_FA_PAUSE "###vaupab", x_pos2 + 43, y_pos, 40, 23))
		pause_automata_execution();

	if (ex_button(ICON_FA_CIRCLE "###vaurb", x_pos2 + 86, y_pos, 40, 23))//ICON_FA_CIRCLE - ICON_FA_RECORD_VINAL
		record_automata_execution();

	if (ex_button(ICON_FA_PLAY "###vauplb", x_pos2 + 129, y_pos, 40, 23))
		play_automata_execution();

	if (ex_button(ICON_FA_STEP_FORWARD "###vausfb", x_pos2 + 171, y_pos, 40, 23))
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

	ImGui::SetCursorPosX(x_pos + 50);
	ImGui::SetCursorPosY(y_pos);
	ImGui::ProgressBar(progress, ImVec2(210.f, 0.f), buf);


	y_pos += 30;

	if (ex_button(ICON_FA_UPLOAD " Save###vausrb", x_pos + 20, y_pos, 80, 20))
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
					}
					else {
						current_automata_step++;
						progress = ((float)current_automata_step / (float)number_automata_steps);
					}
				}
			}
		}
		else { // Use multithreading

		}
	}

}

void voxel_hcp_automata_widget_class::clear_automata_rules() {
	voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules.clear();
}

void voxel_hcp_automata_widget_class::save_automata_rules() {
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


void voxel_hcp_automata_widget_class::load_automata_rules() {
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

void voxel_hcp_automata_widget_class::stop_automata_execution() {
	automata_stopped = true;
	current_automata_step = 0;
	progress = 0.0f;
	automata_paused = false;

	automata_play = false;
}

void voxel_hcp_automata_widget_class::pause_automata_execution() {
	//printf("paused button pressed\n");
	automata_paused = !automata_paused;
	//if(automata_paused == false)
	//printf("automata_paused == false\n");
	//else
	//printf("automata_paused == true\n");
}

void voxel_hcp_automata_widget_class::record_automata_execution() {
	// not yet implemented
}

void voxel_hcp_automata_widget_class::play_automata_execution() {
	if (!automata_play) {
		if (setup_play())
			automata_play = true;
	}
}

void voxel_hcp_automata_widget_class::next_step_automata_execution() {
	if (use_multithreading)
		thread_next_step_voxel_automata();
	else
		next_step_voxel_automata_no_thread();
}


// ****
bool voxel_hcp_automata_widget_class::setup_play() {
	//printf("setup_play 00");

	if (voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Play_voxel_automata_no_thread() :: No voxel hcp object specified to execute.\n");
		return false;
	}

	automata_stopped = false;
	automata_paused = false;
	automata_play = true;
	current_automata_step = 0;

	//Save Automata rules to execute for selected voxel object
	//voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules = get_voxel_automata_rules();
	//save_current_automata_data()

	voxel_automata_hcp_functions.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
	voxel_automata_hcp_functions.cloud = voxel_hcp_object_to_execute;
	voxel_automata_hcp_functions.voxel_hcp_automata_rules = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;

	number_automata_steps = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

	return true;
}
// ****



void voxel_hcp_automata_widget_class::next_step_voxel_automata_no_thread() {
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
	voxel_automata_hcp_functions.cloud = voxel_hcp_object_to_execute;
	voxel_automata_hcp_functions.voxel_hcp_automata_rules = voxel_hcp_object_to_execute->voxel_object_data.voxel_hcp_automata_byte_rules;
	//printf("next_step_voxel_automata 02 : ");

			// following is to update the progress bar
	int max_step_value = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

	//printf("next_step_voxel_automata 03 : %i ",max_step_value);

	number_automata_steps = max_step_value;

	if (current_automata_step <= max_step_value)
		if (perform_automata_step(current_automata_step)) {
			current_automata_step++;
			progress = ((float)current_automata_step / (float)max_step_value);
		}

	automata_stopped = true;
}

bool voxel_hcp_automata_widget_class::perform_automata_step(int step) {
	//printf("INFO", "perform_automata_step 00 : %i\n" , step);

			// Apparently the way that Imgui or the cpu works, it seems this log message is not displayed until after
			// the perform_voxel_automata_generation function is completed, so no advantage having it executed
			//if (log_panel != NULL) {
			//	log_panel->application_log.AddLog("PERFORMING CELLULAR AUTOMATA RULES !!! : PLease wait \n");
			//}

	if (!voxel_automata_hcp_functions.perform_voxel_automata_generation(step)) return false;
	//printf("INFO","perform_automata_step 01 : %i",step);

	if (log_panel != NULL) {
		log_panel->application_log.AddLog("Completed performing hcp voxel cellular automata rules !!!\n");
	}

	//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
	scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);
	//printf("voxel_hcp_automata_widget_class :: execute_voxel_function 11111\n");

	if (scene_voxel_object == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Automata : Could not find voxel in the scene to update geometry data.\n");
		//printf("voxel_hcp_automata_widget_class 22222 scene_voxel_object == NULL.\n");
		return false;
	}
	else {
		//printf("voxel_hcp_automata_widget_class 33333 scene_voxel_object != NULL.\n");

		if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Voxel Automata : Scene voxel object geometry could not be updated.\n");
			//printf("voxel_hcp_automata_widget_class 44444 not updated\n");
			return false;
		}

		//printf("voxel_hcp_automata_widget_class :: execute_voxel_function 55555\n");
	}
	return true;
}


void voxel_hcp_automata_widget_class::thread_next_step_voxel_automata() {
	printf("thread_next_step_voxel_automata 00");
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

bool voxel_hcp_automata_widget_class::thread_perform_automata_step(int step) {
	printf("INFO thread_perform_automata_step 00 : %i\n", step);
	/*
			if (!voxel_automata_hcp_thread_functions.perform_voxel_automata_generation(step)) return false;
	//printf("INFO","perform_automata_step 01 : %i",step);

			//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
			scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_enity_id);
	//printf("voxel_hcp_automata_widget_class :: execute_voxel_function 11111\n");

			if (scene_voxel_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Automata : Could not find voxel in the scene to update geometry data.\n");
	//printf("voxel_hcp_automata_widget_class 22222 scene_voxel_object == NULL.\n");
				return false;
			}
			else {
	//printf("voxel_hcp_automata_widget_class 33333 scene_voxel_object != NULL.\n");

				if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Voxel Automata : Scene voxel object geometry could not be updated.\n");
	//printf("voxel_hcp_automata_widget_class 44444 not updated\n");
					return false;
				}

	//printf("voxel_hcp_automata_widget_class :: execute_voxel_function 55555\n");
			}
	*/
	return true;
}

void voxel_hcp_automata_widget_class::automata_functions_logging(log_panel_class* log_panel) {
	voxel_automata_hcp_functions.log_panel = log_panel;
}
