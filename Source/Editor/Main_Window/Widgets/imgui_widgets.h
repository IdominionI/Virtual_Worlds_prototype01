#pragma once

//#include <Source/Graphics_Engine/Common/pch.h>

#include <string>

#include <Universal/ImGui/imgui.h>
#include <Universal/ImGui/imgui_internal.h>

#include "../../Common/imgui_custom.h"//*****
#include <Universal/3rdparty/include/glm/glm.hpp> //******

/*
	Derivations of various commonly used Imgui Widgets set as a
	function to simplify their use and reduce repetition of common
	code blocks 
*/

enum class imgui_text_location_enum {left,right, above, below, center};

	// ^^^^^^^^^^^ Text widgets ^^^^^^^^^^^^^^^^^^

	static void text(std::string text_string, float x, float y){
		ImGui::SetCursorPosX(x);
		ImGui::SetCursorPosY(y);
		ImGui::Text(text_string.c_str());
	}

	static void title(std::string text_string) {
		float w = ImGui::GetWindowWidth();
		ImGui::SetCursorPosX(w / 2.0f - 50.0f);
		ImGui::SetWindowFontScale(1.1f);
		ImGui::Text(text_string.c_str());
	}

	static void ascii_text_input(std::string text, std::string &value, float x, float y,int length) {
		ImGui::SetCursorPosX(x);
		ImGui::SetCursorPosY(y);

		ImGui::InputText(text.c_str(), &value[0], length, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);
	}

	// ^^^^^^^^^^^ Button widgets ^^^^^^^^^^^^^^^^^^

	static bool ex_button(std::string text_string,float x,float y, float width,float height) {
		ImGui::SetCursorPosX(x);
		ImGui::SetCursorPosY(y);
		return ImGui::Button(text_string.c_str(), { width,height });
	}

	// ^^^^^^^^^^^ Input widgets ^^^^^^^^^^^^^^^^^^

	static void float_input(std::string id_string, float &value,float x_pos, float y_pos, float width) {
		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(width);
		ImGui::InputFloat(id_string.c_str(),&value);
	}

	// Folowing needs to have the increment and decrement button names defined by te user as found
	// nothing works to create a const char * variable type that includes a unicode ICON_FA_CARET_UP
	// or ICON_FA_CARET_DOWN as a char * character.
	// Usage : float_input_delta("###ftest", fvalue, 0.4, 100.0f, 200.0f, 70.0f, ICON_FA_CARET_UP"###iftest", ICON_FA_CARET_DOWN"###dftest");
	static void float_input_delta(std::string id_string, float &value,float delta, float x_pos, float y_pos, float width, const char *bi, const char *bd) {
		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(width);
		ImGui::InputFloat(id_string.c_str(), &value);

		float b_height = ImGui::GetTextLineHeight() / 2.0f;

		ImGui::SetCursorPosX(x_pos + width + 3);
		ImGui::SetCursorPosY(y_pos - 3);

		//if (ImGui::ButtonEx(ICON_FA_CARET_UP"##i_increment", ImVec2(15, b_height + 3))){
		if (ImGui::ButtonEx(bi, ImVec2(15, b_height + 3))){
			printf("Button + clicked\n");
			value += delta;
		}

		ImGui::SetCursorPosX(x_pos + width + 3);
		ImGui::SetCursorPosY(y_pos + b_height + 3);
		//if (ImGui::ButtonEx(ICON_FA_CARET_DOWN"##i_decrement", ImVec2(15, b_height + 3))){
		if (ImGui::ButtonEx(bd, ImVec2(15, b_height + 3))){
			printf("Button - clicked\n");
			value -= delta;
		}
	}

	static void float_input_step(std::string id_prefix, float &value, float &step, bool &active, float x_pos, float y_pos, float value_width,float step_width ) {
		std::string w_id = "";

		y_pos += 30;
		w_id = id_prefix + "x";
		text("X :     ", x_pos + 75, y_pos);
		ImGui::SameLine(); w_id = id_prefix + "1"; ImGui::SetNextItemWidth(value_width); ImGui::InputFloat(w_id.c_str(), &value);
		ImGui::SameLine(); w_id = id_prefix + "2"; ImGui::SetNextItemWidth(step_width); ImGui::InputFloat(w_id.c_str(), &step);
		ImGui::SameLine(); w_id = id_prefix + "3"; ImGui::Checkbox(w_id.c_str(), &active);
	}

	static void integer_input(std::string id_string, int &value, float x_pos, float y_pos, float width) {
		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(width);
		ImGui::InputInt(id_string.c_str(), &value,0);
	}

	// Folowing needs to have the increment and decrement button names defined by te user as found
	// nothing works to create a const char * variable type that includes a unicode ICON_FA_CARET_UP
	// or ICON_FA_CARET_DOWN as a char * character.
	// usage :: integer_input_delta("###test", value, 2, 100.0f, 100.0f, 70.0f, ICON_FA_CARET_UP"###itest", ICON_FA_CARET_DOWN"###dtest");
	static void integer_input_delta(std::string id_string, int &value,int delta, float x_pos, float y_pos, float width, const char *bi, const char *bd) {
		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(width);
		ImGui::InputInt(id_string.c_str(), &value, 0);

		float b_height = ImGui::GetTextLineHeight() / 2.0f;

		ImGui::SetCursorPosX(x_pos + width + 3);
		ImGui::SetCursorPosY(y_pos - 3);

		//if (ImGui::ButtonEx(ICON_FA_CARET_UP"##i_increment", ImVec2(15, b_height + 3))){
		if (ImGui::ButtonEx(bi, ImVec2(15, b_height + 3))){
			//printf("Button + clicked\n");
			value += delta;
		}

		ImGui::SetCursorPosX(x_pos + width + 3);
		ImGui::SetCursorPosY(y_pos + b_height + 3);
		//if (ImGui::ButtonEx(ICON_FA_CARET_DOWN"##i_decrement", ImVec2(15, b_height + 3))){
		if (ImGui::ButtonEx(bd, ImVec2(15, b_height + 3))){
			//printf("Button - clicked\n");
			value -= delta;
		}
	}

	static void draw_vec3_widget(const std::string& label, glm::vec3& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();


	}

	
	// ^^^^^^^^^^^ Slider Widgets ^^^^^^^^^^^^^^^^^^
	static bool float_min_max_slider(std::string id_string, float x_pos, float y_pos, float slider_width,float &min_value, float &max_value, float &slider_value, float range_width) {
		std::string min_id    = "###g" + id_string + "min";
		std::string max_id    = "###g" + id_string + "max";
		std::string slider_id = "###g" + id_string + "slider";
	
		float_input(min_id.c_str(), min_value, x_pos, y_pos, range_width);
		float_input(max_id.c_str(), max_value, x_pos + range_width + slider_width + 10, y_pos, range_width);

		ImGui::SetCursorPosX(x_pos + range_width+5);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(slider_width);
		return ImGui::SliderFloat(slider_id.c_str(), &slider_value, min_value, max_value, "%.3f", ImGuiSliderFlags_None);
	}

	// xxxxxxxxxxxxx Checkbox Widget xxxxxxxxxxxxxxxxxxxxxxxx

	//static void checkbox(std::string id_string, std::string description, float x_pos, float y_pos, float x_offset, float y_offset, bool &value, imgui_text_location_enum imgui_text_location) {
	static bool checkbox(std::string id_string, std::string description, float x_pos, float y_pos, float x_offset, float y_offset, bool &value, imgui_text_location_enum imgui_text_location) {
		float box_x = 0.0, box_y = 0.0;

		switch (imgui_text_location) {
			case imgui_text_location_enum::left  : box_x = x_pos + x_offset; break;
			case imgui_text_location_enum::right : box_x = 0.0; break;
			case imgui_text_location_enum::above : box_y = y_pos + y_offset; break;
			case imgui_text_location_enum::below : box_y = y_pos - x_offset; break;
		}

		ImGui::SetCursorPosX(x_pos);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Text(description.c_str());
		ImGui::SetCursorPosX(x_pos+ box_x);
		ImGui::SetCursorPosY(y_pos+ box_y);
		//ImGui::Checkbox(id_string.c_str(), &value);
		return ImGui::Checkbox(id_string.c_str(), &value);
	}
