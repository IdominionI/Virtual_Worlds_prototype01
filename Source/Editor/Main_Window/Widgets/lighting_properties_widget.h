#pragma once

#include "imgui_widgets.h"

#include <Source/Graphics_Engine/Shader/shader_components.h>

/*
					SCENE LIGHTING PROPOERTIES WIDGET

	A widget class that displays editable scene lighting parameters that
	define the lighting of entity objects in the scene.
*/

class lighting_properties_widget_class {
public:
	lighting_properties_widget_class() {}
	~lighting_properties_widget_class() {}

	ImVec4 light_color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 255.0f / 255.0f);

	
	bool display(universal_shader_variables_struct_type *universal_shader_variables) {
		if (universal_shader_variables == NULL) return false;

		float x_pos = 10.0f, y_pos = 40.0f;

		title("Scene Lighting");

		y_pos += 40;

		ImGui::PushStyleColor(ImGuiCol_Button, light_color);// Chanage all the buttons to the light color until a ImGui::PopStyleColor
		if (ex_button("Light Color###plc", x_pos + 90, y_pos, 100, 30)) {
			ImGui::OpenPopup("Light Color###plcpu");
		}
		ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

		if(	ImGui::BeginPopup("Light Color###plcpu")) {
			ImGuiColorEditFlags flags = misc_flags;
			flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
			ImGui::ColorPicker4("Light Color###cp", (float*)&light_color, flags, ref_color ? &ref_color_v.x : NULL);
			universal_shader_variables->light_color.r = light_color.x;
			universal_shader_variables->light_color.g = light_color.y;
			universal_shader_variables->light_color.b = light_color.z;
			universal_shader_variables->light_color.a = light_color.w;
			ImGui::EndPopup();
		}

		y_pos += 40;
		text(" Global Lighting\nDirecional Vector", x_pos + 75,y_pos);
		y_pos += 35;
		text("x        y       z", x_pos + 75, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos+50);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::SliderFloat3("###lpdv", lighting_dir_vector, 0.0f, 1.0f, "%.2f")) {
			universal_shader_variables->lighting_direction.x = lighting_dir_vector[0];
			universal_shader_variables->lighting_direction.y = lighting_dir_vector[1];
			universal_shader_variables->lighting_direction.z = lighting_dir_vector[2];
		}

		y_pos += 30;
		text("Global Ambience Level", x_pos + 65, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos+100);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		ImGui::SliderFloat("###plal", &universal_shader_variables->ambience, 0.0f, 1.0f, "%.2f");

		y_pos += 30;
		text("Global Specular Level", x_pos + 65, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos + 100);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		ImGui::SliderFloat("###plsl", &universal_shader_variables->specular_strength, 0.0f, 1.0f, "%.2f");

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Use Camera to define lighting###plcl", &universal_shader_variables->camera_as_light);

		y_pos += 30;
		text("Type of Camera Lighting", x_pos + 65, y_pos);
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos+40);
		ImGui::SetCursorPosY(y_pos);
		ImGui::RadioButton("Point###plprb", &universal_shader_variables->type_camera_light, 0); ImGui::SameLine();
		ImGui::RadioButton("Directional###pldrb", &universal_shader_variables->type_camera_light, 1);
		
		y_pos += 30;
		text("Point Light Offset", x_pos +75, y_pos);
		y_pos += 20;
		text("x        y       z", x_pos + 75, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos + 50);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::SliderFloat3("###lpplo", camera_light_offset, -10.0f, 10.0f, "%.2f")) {
			universal_shader_variables->camera_light_offset.x = camera_light_offset[0];
			universal_shader_variables->camera_light_offset.y = camera_light_offset[1];
			universal_shader_variables->camera_light_offset.z = camera_light_offset[2];
		}

		y_pos += 30;
		text("Lighting Intensity", x_pos + 85, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos + 100);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		ImGui::SliderFloat("###plli", &universal_shader_variables->light_intensity, 0.0f, 1.0f, "%.2f");

		return true;
	}


private:

	float lighting_dir_vector[3] = {0.0f,0.0f,0.0f};
	float camera_light_offset[3] = {0.0f,0.0f,0.0f};


	// ********** Color Picker definitions *****************
	bool alpha_preview      = true;
	bool alpha_half_preview = false;
	bool drag_and_drop      = true;
	bool options_menu       = true;
	bool hdr                = false;

	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	//ImGui::Text("Color picker:");
	bool alpha         = true;
	bool alpha_bar     = true;
	bool side_preview  = true;
	bool ref_color     = false;
	ImVec4 ref_color_v = { 1.0f, 0.0f, 1.0f, 0.5f };
	int display_mode   = 0;
	int picker_mode    = 0;

	//if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
	//if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;
	//if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
	//if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
	//if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
	//if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
	//if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
	//if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
	//if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
	// **************************************************

};