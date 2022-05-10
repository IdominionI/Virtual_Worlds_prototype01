#pragma once

#include "../Widgets/log_widget.h"
#include <Universal/Framework/FWstring_tools.h>

class log_panel_class {
public:
    log_widget_class application_log;
    log_widget_class code_log;

    // Demonstrate creating a simple log window with basic filtering.
    void display_application_log() {
        ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
        application_log.Draw("Application Log");
    }

    // Demonstrate creating a simple log window with basic filtering.
    void display_code_log(){
        ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
        code_log.Draw("Code Log");
    }

    void  display_code_log_code(std::string compile_code){
        std::vector<std::string> lines = FW::stringtools::split(compile_code,"\n");
        int line_number = 1;

        std::string code_lines = "";

        for (std::string line : lines) {
            code_lines = code_lines + std::to_string(line_number) + " : " + line + "\n";
            line_number ++;
        }

        if(code_lines.length() > 0) 
            code_log.AddLog(code_lines.c_str());
    }

    void display_code_errors(std::string code_errors) {
        code_log.AddLog(" ------------- Shader compile Errors -----------------\n\n");
        if (code_errors.length() > 0)
            code_log.AddLog(code_errors.c_str());
    }

private:
};