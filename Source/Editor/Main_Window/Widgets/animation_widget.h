#pragma once

#include <algorithm>

#include "imgui_widgets.h"

#include "../../Scene/scene_manager.h"

#include "../../Tools/dialogs.h"

// Module entity data type animation functions to be animated goes here
//#include <Source/Modules/Module_Voxel_Byte/Animation/voxel_animation_functions.h>
//#include <Source/Modules/Module_Hex_Surface/Animation/hex_animation_functions.h>

/*
					SCENE ANIMATION WIDGET

	A widget class that displays the controls the animation of the entities within
	the scene so that the user can interact with the animation of the objects that
	are visible within the scene.

	Each entity data type has an animation class that defines the animation options
	and operations. The animation widget class operates on a frame step basis that is
	anble to perform frame step operations of entities according to those entity animation
	class functions.

	Animation steps can be recorded if the entity animation functions allow it as vertex 
	point or vertex surfaces in .ply format.

	The way that the animation actions are performed and coded is governed by the way that
	ImGiu works. Because of this, animations are not performed in a seperate loop which
	the control of cannot be interupted or stopped but each animation step is performed within
	each ImGui display call that actually simplifies things.

	Animation control is utilised by the status of control parameters that are set to true or
	flase according to the user interactions. By what is set to be true, that animation function
	that performs that task will bw performed if true.

	A feature that is utilised is the ability to restore the animations that do not involve a
	cellula automata procedure to its initial animation status.
*/

#include "../../Animation/animation_basis.h"

class animation_widget_class : public animation_manager_class {
public:
	animation_widget_class() {}
	~animation_widget_class() {}

	void display() {
		float x_pos = 10.0f, y_pos = 40.0f;

		title("Animation");

		y_pos += 30;
		text("Objects to animate", x_pos + 85, y_pos);
		y_pos += 25;
		text("Selected    Activated    All", x_pos + 45, y_pos);
		y_pos += 25;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###aselrb", &animation_selection, 0);
		ImGui::SetCursorPosX(x_pos + 165);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###aactrb", &animation_selection, 1);
		ImGui::SetCursorPosX(x_pos + 240);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###aallrb", &animation_selection, 2);

		y_pos += 25;
		ImGui::SetCursorPosX(x_pos + 80);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Use Shader textures###astcb", &use_textures);

		y_pos += 25;
		ImGui::SetCursorPosX(x_pos + 80);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Use Automata###aautocb", &use_automata);

		y_pos += 25;
		ImGui::SetCursorPosX(x_pos + 80);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Multithread Automata###amyacb", &use_multi_thread_automata);

		y_pos += 25;
		text("Animated Object\nFrames Directory", x_pos + 120, y_pos);
		y_pos += 30;
		if (ex_button(animation_frames_directory, x_pos + 80, y_pos, 190, 20))
			select_animation_frames_directory();

		y_pos += 25;
		text("Animation File Name \n     Template", x_pos + 100, y_pos);
		y_pos += 35;
		ascii_text_input("###afnt", filename_template, x_pos + 60, y_pos, 30);

		y_pos += 30;
		text("Animation Mesh", x_pos + 115, y_pos);
		y_pos += 25;
		text("Point\nCloud", x_pos + 60, y_pos);
		text("Point\nSurface", x_pos + 140, y_pos);
		text("Face\nSurface", x_pos + 230, y_pos);
		y_pos += 35;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###ampcrb", &animation_surface, 0);
		ImGui::SetCursorPosX(x_pos + 165);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###apsrb", &animation_surface, 1);
		ImGui::SetCursorPosX(x_pos + 240);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("###amfsrb", &animation_surface, 2);

		y_pos += 30;
		text("Frame Steps to Render", x_pos + 85, y_pos);
		y_pos += 20;
		text("Start", x_pos + 50, y_pos);
		integer_input_delta("###afss", start_frame, 1, x_pos +100, y_pos, 40, "###afssi", "###afssd");
		
		text("End", x_pos + 195, y_pos);
		integer_input_delta("###afse", end_frame, 1, x_pos + 230, y_pos, 40, "###afsei", "###afsed");
		y_pos += 35;
		text("Delta F :", x_pos + 100, y_pos);
		integer_input_delta("###afdf", frame_delta, 1, x_pos + 180, y_pos, 40, "###afdfi", "###afdfd");

		y_pos += 30;
		float x_pos2 = x_pos + 90;
		text("Animation Controls", x_pos + 85, y_pos);
		y_pos += 20;

		if (ex_button(ICON_FA_CARET_LEFT "###vaupbb", x_pos2, y_pos, 40, 23))
			reverse_animation();

		if (ex_button(ICON_FA_STOP "###vausb", x_pos2 + 43, y_pos, 40, 23))
			stop_animation();

		if (ex_button(ICON_FA_CIRCLE "###vaurb", x_pos2 + 86, y_pos, 40, 23))//ICON_FA_CIRCLE - ICON_FA_RECORD_VINAL
			record_animation();

		if (ex_button(ICON_FA_PLAY "###vaupfb", x_pos2 + 129, y_pos, 40, 23))
			play_animation();

		y_pos += 30;

		if (ex_button(ICON_FA_STEP_BACKWARD "###vausbb", x_pos2, y_pos, 40, 23))
			step_back_animation();

		if (ex_button(ICON_FA_PAUSE "###vauppb", x_pos2 + 43, y_pos, 40, 23))
			pause_animation();

		if (ex_button(ICON_FA_RETWEET "###vaurpb", x_pos2 + 86, y_pos, 40, 23))
			repeat_animation();

		if (ex_button(ICON_FA_STEP_FORWARD "###vausfb", x_pos2 + 129, y_pos, 40, 23))
			step_forward_animation();

		y_pos += 35;
		text("Frames Completed", x_pos + 115, y_pos);

		#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
		y_pos += 25;
		float progress_saturated = IM_CLAMP(animation_progress, 0.0f, 1.0f);
		char buf[32];
		sprintf_s(buf, "%d/%d", (int)(progress_saturated * number_animation_frames), number_animation_frames);

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		ImGui::ProgressBar(animation_progress, ImVec2(210.f, 0.f), buf);

		y_pos += 35;
		if (ex_button("Restore Initial Frame###arif", x_pos + 80, y_pos, 190, 20)) {
			restore_initial_frame_conditions();
		}

		y_pos += 25;
		ImGui::SetCursorPosX(x_pos + 80);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Restore First Frame When\nanimation completed###arifcb", &restore_animation_on_completion);


		// This here to easily be able to pause stop automata process perform this statement
		if (animation_play) {
			if (current_animation_frame <= end_frame) {
				if (!animation_paused) {
					if (!perform_animation_frame(current_animation_frame,INCREMENT_STEP)) {
						if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
						stop_animation();
					}
					else {
						vw_animation_parameters.current_frame += vw_animation_parameters.frame_step_interval;
						current_animation_frame = vw_animation_parameters.current_frame;
						animation_progress = ((float)current_animation_frame / (float)number_animation_frames);
					}
				}

				if (current_animation_frame >= end_frame) {
//printf("animation_play 000 :: current_animation_frame >= end_frame\n");
					//if (restore_animation_on_completion && current_animation_frame >= end_frame) { // Restore initial conditions of animation
					if (restore_animation_on_completion && !animation_repeat) { // Restore initial conditions of animation
						restore_initial_frame_conditions();
					} else {
//printf("animation_play 111 \n");
						if (animation_repeat) {
//printf("animation_play 222 :: animation_repeat\n");
							restore_initial_frame_conditions();
							animation_repeat = true;// because restore_initial_frame(); sets animation_repeat to false
							play_animation();
						}
					}
				}
			}
		} else {
//printf("animation_reverse_play AAA ::current_animation_frame > start_frame %i : %i\n", current_animation_frame, start_frame);
			if (animation_reverse_play) {
				if (current_animation_frame > start_frame) {
//printf("animation_reverse_play 000 ::current_animation_frame > start_frame %i : %i\n", current_animation_frame, start_frame);
					if (!animation_paused) {
//printf("animation_reverse_play 1111 ::\n");
						if (!perform_animation_frame(current_animation_frame, DECREMENT_STEP)) {
//printf("animation_reverse_play 2222 ::!perform_animation_frame(current_animation_frame, DECREMENT_STEP)\n");
							if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
							stop_animation();
						}
						else {
//printf("animation_reverse_play 3333::\n");
							vw_animation_parameters.current_frame -= vw_animation_parameters.frame_step_interval;
							current_animation_frame = vw_animation_parameters.current_frame;
							animation_progress = ((float)current_animation_frame / (float)number_animation_frames);
						}
					}
				} else {
						stop_animation();
						vw_animation_parameters.current_frame = start_frame;
					}
			}
			else {
				if (animation_record) {
//printf("animation_record 000 :: \n");
					if (current_animation_frame <= end_frame) {
						if (!animation_paused) {
//printf("animation_record 111 :: %i \n", current_animation_frame);
							if (!perform_animation_frame(current_animation_frame, INCREMENT_STEP)) {
								if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
								stop_animation();
							} else {
//printf("animation_record 222 :: %i \n", current_animation_frame);
								save_animation_frame_mesh(saved_filename_prefix, current_animation_frame, saved_filename_format); // Save the Current frame mesh
								vw_animation_parameters.current_frame += vw_animation_parameters.frame_step_interval;
								current_animation_frame = vw_animation_parameters.current_frame;
								animation_progress = ((float)current_animation_frame / (float)number_animation_frames);
							}
						}

						if (current_animation_frame >= end_frame) {
//printf("animation_record 000 :: current_animation_frame >= end_frame\n");
							if (restore_animation_on_completion) { // Restore initial conditions of animation
								restore_initial_frame_conditions();
							}
						}
					} else {
						animation_record = false;
					}
				}
			} 
		}
	}


	void select_animation_frames_directory() {
		std::string directory_pathname = vwDialogs::select_directory("./");
				
		//animation_frames_directory_pathname = vwDialogs::select_directory("./"); // Crashes app if no directory selected

		if (!directory_pathname.empty()) {
			animation_frames_directory_pathname = directory_pathname;
			animation_frames_directory = vwDialogs::get_filename(animation_frames_directory_pathname, "\\");
		}
	}

	bool reverse_animation() {
//printf("reverse_animation 00\n");
		animation_reverse_play  = true;
		animation_stopped       = false;
		animation_paused        = false;
		animation_play          = false;
		animation_record        = false;

		// This seems to work
		if (vw_animation_parameters.current_frame <= end_frame && vw_animation_parameters.current_frame > start_frame)
			reverse_end_frame = vw_animation_parameters.current_frame;

		number_animation_frames = reverse_end_frame - start_frame;

		return true;
	}

	void stop_animation() {
		animation_stopped       = true;
		current_animation_frame = 0;
		animation_progress      = 0.0f;
		animation_paused        = false;
		animation_play          = false;
		animation_repeat        = false;
		animation_reverse_play  = false;
		animation_record        = false;
	}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Perform the application play and record animation pipeline
	// NOTE : This function will need to be modified for each entity data object type that is used
	//        by the application.
	// Record or saving of mesh frames
	// Note: Even though no facility to export color data is implemented, the voxel tecture color functionality is
	//       left within the recording function for display purposes and future implementation to save
	// 
	// 1 : Define objects to execute for each entity data type
	// 2 : Get directory to save animation frame files to
	// 3 : define_animation_parameters();
	// 4 : define_initial_frame_paramters();
	// 5 : save initial animation frame mesh for each object that is to be animated in the scene
	// 6 : For each animation frame 
	//	   generate point cloud data for each object data type that is to be animated in the scene
	//	   save the animation frame mesh for each object that is to be animated in the scene to disk
	// 7 : restore the scene to its initial state.
	//
	void record_animation() {
		vw_animation_parameters.current_frame = 0;

		if (!initialise_animation()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : editor_animation_widget_class :: Could not initialise animation.\n");
			return; // Initialise the entity data type objects for animation
		}

		number_animation_frames = end_frame - start_frame;

		if (!define_objects_to_execute()) return;

		if (!FW::filetools::directory_exists(animation_frames_directory_pathname) || animation_frames_directory_pathname == "") {
			vwDialogs::message_box("","Have no valid directory to save voxel animation frame meshes defined.\nPlease define the directory to save voxel animation frame meshes to.");
			return;
		}

		std::replace(animation_frames_directory_pathname.begin(), animation_frames_directory_pathname.end(), '\\', '/'); // replace all '\' to '/'

//printf("record_animation0 1: %s \n", animation_frames_directory_pathname.c_str());

		if (filename_template.size() == 0) {
			vwDialogs::message_box("", "Have no voxel mesh frame file name template defined.\nPlease enter animation mesh frame file name template");
			return;
		}

		saved_filename_prefix = animation_frames_directory_pathname + "/" + filename_template;

		// Only ply format is defined as it is the best to use for this application thus far
		// This code retained to use when other formats found to be required.
//QString saved_filename_format = "." + mesh_format_cb->currentText();
		saved_filename_format = ".ply";

		animation_stopped      = false;
		animation_stopped      = false;
		animation_play         = false;
		animation_reverse_play = false;
		animation_record       = true;
	}

	void play_animation() {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "editor_animation_widget_class : play_animation 00");
//}
		animation_reverse_play = false;
		animation_stopped      = false;
		animation_paused       = false;

		if (!animation_play) {
			if (setup_play())
				animation_play = true;
		}
	}

	bool setup_play() {
//printf("setup_play 00");

		animation_stopped       = false;
		animation_paused        = false;
		animation_play          = true;
		current_animation_frame = 0;

		// This seems to work
		if (vw_animation_parameters.current_frame <= start_frame) {
			if (!initialise_animation()) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : editor_animation_widget_class :: Could not initialise animation.\n");
				return false; // Initialise the entity data type objects for animation
			}
		}

		number_animation_frames = end_frame - start_frame;

		return true;
	}

	//void step_back_animation() {
	//}

	void pause_animation() {
//printf("paused button pressed\n");
		animation_paused = !animation_paused;
//if(animation_paused == false)
//printf("animation_paused == false\n");
//else
//printf("animation_paused == true\n");
	}

	void repeat_animation() {
		animation_repeat = !animation_repeat;
		play_animation();
	}

	//void next_step_animation() {
	//}

	void restore_initial_frame_conditions() {
		stop_animation();
		vw_animation_parameters.current_frame = 0;
		restore_initial_frame();
	}

private:
	bool restore_animation_on_completion = false;

	bool animation_play                  = false;
	bool animation_paused                = false;
	bool animation_reverse_play          = false;
	bool animation_stopped               = false;
	bool animation_repeat				 = false;
	bool animation_record				 = false;

	float animation_progress = 0.0f;


	// ################### VOXEL ANIMATION ###################
	//animation_texture_model_parameters_struct_type vw_animation_parameters;
	// vp Generator
	int max_automata_step_value = 0; // Not sure this is needed

	// Perform the animation one step in reverse
	void step_back_animation() {
//QMessageBox::information(NULL, "","Not yet implemented " , QMessageBox::Ok);

		if (vw_animation_parameters.current_frame <= 0) {
			return;
		}
		// for some unknown reason define_objets_to_execute here does not work properly for the referenceing of generation of point
		// data as it seems even though the generation variables change, they are not used to generate data where as shader and automata data are
		// Suspect a compilation anomoly or error
		// else {
		//	if (!define_objets_to_execute()) return;
		//	define_animation_parameters();
		//}

		vw_animation_parameters.current_frame = current_animation_frame;

		if (vw_animation_parameters.current_frame - vw_animation_parameters.frame_step_interval >= vw_animation_parameters.frame_step_start) {
			vw_animation_parameters.current_frame -= vw_animation_parameters.frame_step_interval;
			perform_animation_frame(vw_animation_parameters.current_frame, DECREMENT_STEP);
		}
		else {
			if (vw_animation_parameters.current_frame - vw_animation_parameters.frame_step_interval < vw_animation_parameters.frame_step_start) {
				vw_animation_parameters.current_frame = vw_animation_parameters.frame_step_start;
				perform_animation_frame(vw_animation_parameters.current_frame, DECREMENT_STEP);
			}
		}

		current_animation_frame = vw_animation_parameters.current_frame;
		animation_progress = ((float)vw_animation_parameters.current_frame / (float)vw_animation_parameters.frame_step_end);
	}

	// Perform the animation one step forward
	void step_forward_animation() {
//printf("step_forward_animation 00 %i : %i \n", vw_animation_parameters.current_frame, vw_animation_parameters.frame_step_end);
//std::string s = "step_forward_animation 00 " + std::to_string(vw_animation_parameters.current_frame);
//vwDialogs::message_box("",s.c_str());

		if (vw_animation_parameters.current_frame >= vw_animation_parameters.frame_step_end && vw_animation_parameters.current_frame > 0) return;
//printf("step_forward_animation 11 %i\n" ,vw_animation_parameters.current_frame);
		if (vw_animation_parameters.current_frame <= 0) {
//printf("step_forward_animation 22 %i\n", vw_animation_parameters.current_frame);
			initialise_animation();
		} 
		// for some unknown reason define_objets_to_execute here does not work properly for the referenceing of generation of point
		// data as it seems even though the generation variables change, they are not used to generate data where as shader and automata data are
		// Suspect a compilation anomoly or error
		//else {
//printf("step_forward_animation 22AA %i\n", vw_animation_parameters.current_frame);
		//	if (!define_objets_to_execute()) return;
		//	define_animation_parameters();
		//}

		//vw_animation_parameters.current_frame = current_animation_frame; // *****

		if (vw_animation_parameters.current_frame + vw_animation_parameters.frame_step_interval <= vw_animation_parameters.frame_step_end) {
//printf("step_forward_animation 33 %i\n" ,vw_animation_parameters.current_frame);
			perform_animation_frame(vw_animation_parameters.current_frame, INCREMENT_STEP);
			vw_animation_parameters.current_frame += vw_animation_parameters.frame_step_interval;
		}
		else {
			if (vw_animation_parameters.current_frame + vw_animation_parameters.frame_step_interval > vw_animation_parameters.frame_step_end) {
//printf("step_forward_animation 44 %i\n" ,vw_animation_parameters.current_frame);
				vw_animation_parameters.current_frame = vw_animation_parameters.frame_step_end;
				perform_animation_frame(vw_animation_parameters.current_frame, INCREMENT_STEP);
			}
		}

		current_animation_frame = vw_animation_parameters.current_frame;
		animation_progress = ((float)vw_animation_parameters.current_frame / (float)vw_animation_parameters.frame_step_end);
	}

	// Initialise the entity data objects to be animated
	// NOTE : This function will need to be modified for each entity data object type that is used
	//        by the application.
	bool initialise_animation() {
		if (!define_objects_to_execute()) return false;

		animation_stopped = false;

//QMessageBox::information(NULL, "","in play animation 04 : ", QMessageBox::Ok);
		define_animation_parameters();
//QMessageBox::information(NULL, "","in play animation 05 : ", QMessageBox::Ok);
		define_initial_frame_paramters(); // must be done afer define_animation_parameters()
//QMessageBox::information(NULL, "","in play animation 06 : ", QMessageBox::Ok);
		return true;
	}


	void define_animation_parameters() {
		vw_animation_parameters.frame_step_start    = start_frame;
		vw_animation_parameters.frame_step_end      = end_frame;
		vw_animation_parameters.frame_step_interval = frame_delta;
		vw_animation_parameters.current_frame       = current_animation_frame;
	}

};

