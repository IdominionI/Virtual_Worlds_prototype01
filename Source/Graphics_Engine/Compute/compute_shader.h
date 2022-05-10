#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <Universal/Framework/FWfile_tools.h>
#include <Universal/Framework/FWstring_tools.h>

#include <Universal/3rdparty/include/gl/glew.h>

enum class compute_ogl_version_enum { v43, v45 };

class compute_shader_class {
public:
    compute_ogl_version_enum ogl_version = compute_ogl_version_enum::v45;

    std::string             source_code;

    GLuint progHandle = -1;
    GLuint cs         = -1;

    compute_shader_class() { }

    ~compute_shader_class() {}

    bool define_source_code_from_file(std::string shader_pathname) {

        std::fstream csFile(shader_pathname.c_str(), std::ios::in);

        if (!csFile.good()) {
            //if (log_widget != NULL) {
           //     log_widget->log_message(log_display, log_message_type_enum_type::error, "compute_service_class:: compute glsl file does not exist " + csFile.fileName());
           // }

            printf("Error :\n compute_service_class:: compute glsl file %s does not exist.\n", shader_pathname.c_str());
            return false;
        }

        source_code = FW::filetools::read_all(csFile);

        return true;
    }

    bool define_compute_program() {
        // Creating the compute shader, and the program object containing the shader
        progHandle = glCreateProgram();
        cs         = glCreateShader(GL_COMPUTE_SHADER);

        const GLchar *csSrc = source_code.c_str(); // Just so crazy stuff of OpenGL

        glShaderSource(cs, 1, &csSrc, NULL);
        glCompileShader(cs);

        
        glGetShaderiv(cs, GL_COMPILE_STATUS, &error_status);
        if (error_status == have_error) { // Need to change the code below
            fprintf(stderr, "Error in compiling the compute shader\n");
            GLchar log[10240];
            GLsizei length;
            glGetShaderInfoLog(cs, 10239, &length, log);
            printf("Compute Shader Souce Code:\n %s\nEnd Compute Shader Souce Code:\n", source_code.c_str()); // Need to print line numbers
            fprintf(stderr, "Compiler log:\n%s\n", log);
            return false;
        }

        glAttachShader(progHandle, cs);
        glLinkProgram(progHandle);
        glGetProgramiv(progHandle, GL_LINK_STATUS, &error_status);
        if (error_status == have_error) {
            fprintf(stderr, "Error in linking compute shader program\n");
            GLchar log[10240];
            GLsizei length;
            glGetProgramInfoLog(progHandle, 10239, &length, log);
            fprintf(stderr, "Linker log:\n%s\n", log);
            return false;
        }

        printf("Compute shader defined\n");

        return true;
    }

private:
    GLint  error_status;
    bool   have_error = GL_FALSE;


};
