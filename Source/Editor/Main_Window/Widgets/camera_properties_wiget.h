#pragma once

#include "imgui_widgets.h"

#include <Source/Graphics_Engine/Scene/Scene_objects/camera_object.h>

/*
					CAMERA PROPOERTIES WIDGET
	
	A widget class that displays editable camera parameters or changes
	the camera behavior such as to define the user interaction with the
	scene camera or how the scene is depicted on the computer screen. 
*/


class camera_poperties_widget_class {
public:
	camera_poperties_widget_class() {}
	~camera_poperties_widget_class() {}

	bool display(universal_shader_variables_struct_type *universal_shader_variables) {
		if (universal_shader_variables == NULL) return false;
		float x_pos = 10.0f, y_pos = 50.0f;

		const char *camera_movement_items[] = {"None", "XY-Plane", "XZ-Plane", "YZ-Plane"};// This cannot be defined outside this function	

		title("Viewer Camera");

		y_pos += 25;
		text("Translation       Orientation", x_pos + 35, y_pos);

		x_pos = 20; y_pos += 20;

		text("X", x_pos, y_pos);
		float_input("###cxt", translation.x, x_pos + 30, y_pos, 75.0f);
		text("Pitch", x_pos+120, y_pos);
		float_input("###cop",pitch, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y", x_pos , y_pos);
		float_input("###cyt", translation.y, x_pos + 30, y_pos, 75.0f);
		text("Yaw", x_pos + 120, y_pos);
		float_input("###coy", yaw, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Z", x_pos, y_pos);
		float_input("###czt", translation.z, x_pos + 30, y_pos, 75.0f);
		text("Roll", x_pos + 120, y_pos);
		float_input("###cor", roll, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Translate###ctb", x_pos + 27, y_pos, 80, 20)) {
			universal_shader_variables->camera->set_location(translation);
		}

		if (ex_button("Orientate###cob", x_pos + 167, y_pos, 80, 20)) {
			universal_shader_variables->camera->set_orientation(pitch, yaw, roll);
		}

		y_pos += 25;
		if (ex_button("Relative###ctrb", x_pos + 27, y_pos, 80, 20)) {
			glm::vec3 position = universal_shader_variables->camera->mPosition;
			universal_shader_variables->camera->set_location(position+translation);
		}

		if (ex_button("Relativee###corb", x_pos + 167, y_pos, 80, 20)) {
			float c_pitch = universal_shader_variables->camera->mPitch;
			float c_yaw   = universal_shader_variables->camera->mYaw;
			float c_roll  = universal_shader_variables->camera->mRoll;
			universal_shader_variables->camera->set_orientation(pitch+ c_pitch, c_yaw+yaw,c_roll+roll);
		}

		y_pos += 35;
		x_pos = 10.0f;
		text("Set Camera Alignment", x_pos + 75, y_pos);
		y_pos += 20;
		x_pos = 5;
		if (ex_button("XY-Plane###caxy", x_pos + 27, y_pos, 80, 20)) {
			universal_shader_variables->camera->align_to_plane(camera_ailgnment_type_enum::xy_plane);
		}

		if (ex_button("XZ-Plane###caxz", x_pos + 110, y_pos, 80, 20)) {
			universal_shader_variables->camera->align_to_plane(camera_ailgnment_type_enum::xz_plane);
		}

		if (ex_button("YZ-Plane###cayz", x_pos + 193, y_pos, 80, 20)) {
			universal_shader_variables->camera->align_to_plane(camera_ailgnment_type_enum::xz_plane);
		}

		y_pos += 30;
		text("Camera Movement\n  Restriction", x_pos + 100, y_pos);
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(75);
		if (ImGui::Combo("###cmr", &camera_restrictions, &Funcs::ItemGetter, camera_movement_items, IM_ARRAYSIZE(camera_movement_items))) {
			switch (camera_restrictions) {
				case 0: universal_shader_variables->camera->movement = camera_movement_mode_enum::free;     break;
				case 1: universal_shader_variables->camera->movement = camera_movement_mode_enum::xy_plane; break;
				case 2: universal_shader_variables->camera->movement = camera_movement_mode_enum::xz_plane; break;
				case 3: universal_shader_variables->camera->movement = camera_movement_mode_enum::yz_plane; break;
			}
		}

		y_pos += 30;
		text("Camera Rotation", x_pos + 90, y_pos);
		y_pos += 20;

		ImGui::SetCursorPosX(x_pos + 50);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Pitch###crp", &universal_shader_variables->camera->pitch_rotation);
		ImGui::SetCursorPosX(x_pos + 130);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Yaw###cry", &universal_shader_variables->camera->yaw_rotation);
		ImGui::SetCursorPosX(x_pos + 210);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Roll###crr", &universal_shader_variables->camera->roll_rotation);

		x_pos = 40; y_pos += 30;
		text("Movement Factor", x_pos, y_pos+3);
		float_input("###cmf", universal_shader_variables->camera->movement_factor, x_pos + 150, y_pos, 50.0f);
		x_pos = 40; y_pos += 30;
		text("Mouse Sensitivity", x_pos, y_pos+3);
		float_input("###cms", universal_shader_variables->camera->cRotationSpeed, x_pos + 150, y_pos, 50.0f);

		y_pos += 25;
		text("Look At Location", x_pos + 45, y_pos);

		x_pos = 95; y_pos += 20;

		text("X", x_pos, y_pos);
		float_input("###cllx", universal_shader_variables->camera->look_at_location.x, x_pos + 30, y_pos, 75.0f);

		y_pos += 20;
		text("Y", x_pos, y_pos);
		float_input("###clly", universal_shader_variables->camera->look_at_location.y, x_pos + 30, y_pos, 75.0f);

		y_pos += 20;
		text("Z", x_pos, y_pos);
		float_input("###cllz", universal_shader_variables->camera->look_at_location.z, x_pos + 30, y_pos, 75.0f);

		y_pos += 25;
		x_pos = 5;
		if (ex_button("Look At###clab", x_pos + 120, y_pos, 75, 20)) {
			universal_shader_variables->camera->look_at(look_at_location);
		}

		x_pos = 5;
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Orbit\nRotation###cro", &universal_shader_variables->camera->orbital_rotation);

		return true;
	}


private:
	glm::vec3 translation      = { 0.0f,0.0f,0.0f };
	glm::vec3 look_at_location = { 0.0f,0.0f,0.0f };
	float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
	int   camera_restrictions = 0;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
	void define_camera_movement_restrictions() {

	}

};