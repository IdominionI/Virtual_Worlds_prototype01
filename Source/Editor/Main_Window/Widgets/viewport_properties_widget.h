#pragma once

#include <Source/Graphics_Engine/OpenGL_window/openGL_context.h>

#include "imgui_widgets.h"

/*
					SCENE VIEWPORT PROPOERTIES WIDGET

	A widget class that displays scene viewport extensions and modifications
	that define what is to be displayed in the viewport and on the computer
	screen.
*/

class viewport_properties_widget_class {
public:
	viewport_properties_widget_class() {}
	~viewport_properties_widget_class() {}

	scene_viewer_class *scene_viewer = NULL;

	ImVec4 xy_grid_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 xz_grid_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 yz_grid_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

	ImVec4 xhair_color   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


	void display(openGL_context_class* openGL_context) {
		float x_pos = 10.0f, y_pos = 45.0f;

		title("Scene Viewport");

		//y_pos += 20;
		//checkbox("###vfps", "FPS", x_pos, y_pos, 0, 0, fps, imgui_text_location_enum::left);

		y_pos += 25;
		if (ImGui::CollapsingHeader("Grid###vgridch")) {
			y_pos += 25;
			text("Grid Plane   X-Y       X-Z       y-Z", x_pos, y_pos);
			y_pos += 25;
			text("Display:", x_pos, y_pos);
			//y_pos += 25;
			ImGui::SetCursorPosX(x_pos + 103); ImGui::SetCursorPosY(y_pos);
			if (ImGui::Checkbox("###vgridcbdxy", &scene_viewer->viewer_grid.display_xy_plane)) scene_viewer->viewer_grid.update_viewer_grid_uniform(XY_PLANE_GRID);
			ImGui::SetCursorPosX(x_pos + 180); ImGui::SetCursorPosY(y_pos);
			if(ImGui::Checkbox("###vgridcbdxz", &scene_viewer->viewer_grid.display_xz_plane)) scene_viewer->viewer_grid.update_viewer_grid_uniform(XZ_PLANE_GRID);
			ImGui::SetCursorPosX(x_pos + 257); ImGui::SetCursorPosY(y_pos);
			if(ImGui::Checkbox("###vgridcbdyz", &scene_viewer->viewer_grid.display_yz_plane))  scene_viewer->viewer_grid.update_viewer_grid_uniform(YZ_PLANE_GRID);
			
			y_pos += 25;
			text("Relative:", x_pos, y_pos);
			ImGui::SetCursorPosX(x_pos + 103); ImGui::SetCursorPosY(y_pos);
			if (ImGui::Checkbox("###vgridcbrxy", &scene_viewer->viewer_grid.relative_xy_grid)) scene_viewer->viewer_grid.update_viewer_grid_uniform(XY_PLANE_RELATIVE);
			ImGui::SetCursorPosX(x_pos + 180); ImGui::SetCursorPosY(y_pos);
			if (ImGui::Checkbox("###vgridcbrxz", &scene_viewer->viewer_grid.relative_xz_grid)) scene_viewer->viewer_grid.update_viewer_grid_uniform(XZ_PLANE_RELATIVE);
			ImGui::SetCursorPosX(x_pos + 257); ImGui::SetCursorPosY(y_pos);
			if (ImGui::Checkbox("###vgridcbryz", &scene_viewer->viewer_grid.relative_yz_grid)) scene_viewer->viewer_grid.update_viewer_grid_uniform(YZ_PLANE_RELATIVE);

			y_pos += 25;
			text("Relative\nlocation", x_pos, y_pos);
			y_pos += 5;
			ImGui::SetCursorPosX(x_pos+70);ImGui::SetCursorPosY(y_pos);ImGui::SetNextItemWidth(75);
			if(ImGui::DragFloat("###vgriddfxy", &scene_viewer->viewer_grid.relative_xy_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(XY_PLANE_RELATIVE_DIST);
			ImGui::SetCursorPosX(x_pos+150);ImGui::SetCursorPosY(y_pos);ImGui::SetNextItemWidth(75);
			if(ImGui::DragFloat("###vgriddfxz", &scene_viewer->viewer_grid.relative_xz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(XZ_PLANE_RELATIVE_DIST);
			ImGui::SetCursorPosX(x_pos+230);ImGui::SetCursorPosY(y_pos);ImGui::SetNextItemWidth(75);
			if(ImGui::DragFloat("###vgriddfyz", &scene_viewer->viewer_grid.relative_yz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(YZ_PLANE_RELATIVE_DIST);

			y_pos += 25;
			text("Relative to\nAbsolute", x_pos, y_pos);
			if (ex_button("X-Y###rtaxy", x_pos + 90, y_pos, 30, 20)) {
				scene_viewer->viewer_grid.xy_grid_origin = scene_viewer->camera->mPosition + glm::vec3{0.0f, 0.0f, scene_viewer->viewer_grid.relative_xy_grid_dist};
			}
			if (ex_button("X-Z###rtaxz", x_pos + 170, y_pos, 30, 20)) {
				scene_viewer->viewer_grid.xy_grid_origin = scene_viewer->camera->mPosition + glm::vec3{ 0.0f, scene_viewer->viewer_grid.relative_xz_grid_dist, 0.0f };
			}
			if (ex_button("Y-Z###rtayz", x_pos + 255, y_pos, 30, 20)) {
				scene_viewer->viewer_grid.xy_grid_origin = scene_viewer->camera->mPosition + glm::vec3{ scene_viewer->viewer_grid.relative_yz_grid_dist, 0.0f, 0.0f };
			}

			y_pos += 30;
			text("Absolute\nlocation", x_pos, y_pos);
			y_pos += 5;
			ImGui::SetCursorPosX(x_pos + 70); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
			if (ImGui::DragFloat("###vgriddfaxy", &scene_viewer->viewer_grid.relative_xy_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(XY_PLANE_ABSOLUTE_DIST);
			ImGui::SetCursorPosX(x_pos + 150); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
			if (ImGui::DragFloat("###vgriddfaxz", &scene_viewer->viewer_grid.relative_xz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(XZ_PLANE_ABSOLUTE_DIST);
			ImGui::SetCursorPosX(x_pos + 230); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
			if (ImGui::DragFloat("###vgriddfayz", &scene_viewer->viewer_grid.relative_yz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None))
				scene_viewer->viewer_grid.update_viewer_grid_uniform(YZ_PLANE_ABSOLUTE_DIST);

			y_pos += 35;
			text("Color", x_pos, y_pos);
			ImGui::PushStyleColor(ImGuiCol_Button, xy_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
			if (ex_button("X-Y###plcxy", x_pos +70, y_pos, 75, 20)) {
				ImGui::OpenPopup("X-Y###plcpuxy");
			}
			ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

			if (ImGui::BeginPopup("X-Y###plcpuxy")) {
				ImGuiColorEditFlags flags = misc_flags;
				flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorPicker4("X-Y###cpxy", (float*)&xy_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
				scene_viewer->viewer_grid.xy_grid_color.r = xy_grid_color.x;
				scene_viewer->viewer_grid.xy_grid_color.g = xy_grid_color.y;
				scene_viewer->viewer_grid.xy_grid_color.b = xy_grid_color.z;
				scene_viewer->viewer_grid.xy_grid_color.a = xy_grid_color.w;
				scene_viewer->viewer_grid.update_viewer_grid_colors(XY_PLANE_GRID_COLOR);
				ImGui::EndPopup();
			}

			ImGui::PushStyleColor(ImGuiCol_Button, xz_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
			if (ex_button("X-Z###plcxz", x_pos + 150, y_pos, 75, 20)) {
				ImGui::OpenPopup("X-Z###plcpuxz");
			}
			ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

			if (ImGui::BeginPopup("X-Z###plcpuxz")) {
				ImGuiColorEditFlags flags = misc_flags;
				flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorPicker4("X-Z###cpxz", (float*)&xz_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
				scene_viewer->viewer_grid.xz_grid_color.r = xz_grid_color.x;
				scene_viewer->viewer_grid.xz_grid_color.g = xz_grid_color.y;
				scene_viewer->viewer_grid.xz_grid_color.b = xz_grid_color.z;
				scene_viewer->viewer_grid.xz_grid_color.a = xz_grid_color.w;
				scene_viewer->viewer_grid.update_viewer_grid_colors(XZ_PLANE_GRID_COLOR);
				ImGui::EndPopup();
			}

			ImGui::PushStyleColor(ImGuiCol_Button, yz_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
			if (ex_button("Y-Z###plcyz", x_pos + 230, y_pos, 75, 20)) {
				ImGui::OpenPopup("Y-Z###plcpuyz");
			}
			ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

			if (ImGui::BeginPopup("Y-Z###plcpuyz")) {
				ImGuiColorEditFlags flags = misc_flags;
				flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorPicker4("Y-Z###cpyz", (float*)&yz_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
				scene_viewer->viewer_grid.yz_grid_color.r = yz_grid_color.x;
				scene_viewer->viewer_grid.yz_grid_color.g = yz_grid_color.y;
				scene_viewer->viewer_grid.yz_grid_color.b = yz_grid_color.z;
				scene_viewer->viewer_grid.yz_grid_color.a = yz_grid_color.w;
				scene_viewer->viewer_grid.update_viewer_grid_colors(YZ_PLANE_GRID_COLOR);
				ImGui::EndPopup();
			}

		}


		y_pos += 40;
		checkbox("###vxhair", "X Hair", x_pos, y_pos, 100, 0, scene_viewer->display_crosshairs, imgui_text_location_enum::left);
		ImGui::PushStyleColor(ImGuiCol_Button, xhair_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
		if (ex_button("###plcxh", x_pos + 170, y_pos, 75, 20)) {
			ImGui::OpenPopup("X-Hairs###plcpuxh");
		}
		ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

		if (ImGui::BeginPopup("X-Hairs###plcpuxh")) {
			ImGuiColorEditFlags flags = misc_flags;
			flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
			ImGui::ColorPicker4("X-Hairs###cpxh", (float*)&xhair_color, flags, ref_color ? &ref_color_v.x : NULL);
			scene_viewer->xhair_color.r = xhair_color.x;
			scene_viewer->xhair_color.g = xhair_color.y;
			scene_viewer->xhair_color.b = xhair_color.z;
			scene_viewer->xhair_color.a = xhair_color.w;
			ImGui::EndPopup();
		}
		
		x_pos += 100;
		y_pos += 25;
		checkbox("###vcaminfo", "Camera Info", x_pos, y_pos, 0, 0, scene_viewer->display_camera_info, imgui_text_location_enum::left);

		y_pos += 25;
		checkbox("###gimbal", "Gimbal", x_pos, y_pos, 0, 0, scene_viewer->display_gimbal, imgui_text_location_enum::left);

		y_pos += 50;
		x_pos -= 20;
		text("Viewer Background Color",x_pos,y_pos);

		y_pos += 25;
		x_pos -= 20;

		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGuiColorEditFlags flags = misc_flags;
		flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
		if(openGL_context != NULL) 
			ImGui::ColorPicker4("###vbgc", (float*)&openGL_context->background_color, flags, ref_color ? &ref_color_v.x : NULL);

	}


private:

	//bool fps      = true;
	bool grid     = true;
	bool axis     = true;
	bool cam_info = true;
	//bool x_hair = true;

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