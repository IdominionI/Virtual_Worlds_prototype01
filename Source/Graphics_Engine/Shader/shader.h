#pragma once

#include "../Common/pch.h"

// Utitlity class to load, compile and attach a vertex- and fragment shader to a program
// NOTE: Absolutly critical that all datatypes to openGL shader compilaions are of type
//       GLuint or there are problems that can arise such as not being able to change
//       the shader program in a hot reload. Crazy openGL that uses numbers insead of
//       pointers and makes things all that more diffucult.
class shader_class
{
public:
	shader_class() = default;
	
	// Load a vertex and a fragment shader from file
	GLuint create_shader_program(const std::string& vertex_shader_file,
							     const std::string& geometry_shader_file, 
							     const std::string& fragment_shader_file) {

		bool have_geometry_shader = false;

		if (geometry_shader_file != "") have_geometry_shader = true;

		std::ifstream is_vs(vertex_shader_file);
		const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

		std::ifstream is_fs(fragment_shader_file);
		const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

		program_id = glCreateProgram();
		shader_compile_successful = program_id; //***

		GLuint vs = get_compiled_shader(GL_VERTEX_SHADER, f_vs);
		GLuint fs = get_compiled_shader(GL_FRAGMENT_SHADER, f_fs);

		GLuint gs;
		if (have_geometry_shader) {
			std::ifstream is_gs(geometry_shader_file);
			const std::string f_gs((std::istreambuf_iterator<char>(is_gs)), std::istreambuf_iterator<char>());

			gs = get_compiled_shader(GL_GEOMETRY_SHADER, f_gs);
		}


		glAttachShader(program_id, vs);
		if(have_geometry_shader) glAttachShader(program_id, gs);
		glAttachShader(program_id, fs);

		glLinkProgram(program_id);
		glValidateProgram(program_id);

		glDeleteShader(vs);
		if (have_geometry_shader) glDeleteShader(gs);
		glDeleteShader(fs);

		return shader_compile_successful;
	}

	// ****
	GLuint compile_shader_program(const std::string& vertex_shader_code,
							      const std::string& geometry_shader_code, 
							      const std::string& fragment_shader_code) {
		
		bool have_geometry_shader = false;

		if (geometry_shader_code != "") have_geometry_shader = true;

		program_id = glCreateProgram();
		shader_compile_successful = program_id;

		GLuint vs = get_compiled_shader(GL_VERTEX_SHADER, vertex_shader_code);
		GLuint fs = get_compiled_shader(GL_FRAGMENT_SHADER, fragment_shader_code);

		GLuint gs;
		if (have_geometry_shader) {
			gs = get_compiled_shader(GL_GEOMETRY_SHADER, geometry_shader_code);
		}


		glAttachShader(program_id, vs);
		if(have_geometry_shader) glAttachShader(program_id, gs);
		glAttachShader(program_id, fs);

		glLinkProgram(program_id);
		glValidateProgram(program_id);

		glDeleteShader(vs);
		if (have_geometry_shader) glDeleteShader(gs);
		glDeleteShader(fs);

		return shader_compile_successful;

	}
	// ****
	// Use the program
	void bind() {
		glUseProgram(program_id);
	}

	void release() {
		glUseProgram(0);
	}

	void use(GLuint shader_program_id) {
		glUseProgram(shader_program_id);
	}

	//Delete the program
	void unload() {
		glDeleteProgram(program_id);
	}

	void unload(GLuint shader_program_id) {
		glDeleteProgram(shader_program_id);
	}

	//unsigned int get_program_id() { return program_id; }

	// ################# DEFINE SHADER UNIFORM VARIABLES #####################

	void set_b1(GLuint shader_program_id, bool v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		if (v == true) v = 1;
		glUniform1i(myLoc, v);
	}

	// ------------ Floats

	void set_f1(GLuint shader_program_id, float v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform1f(myLoc, v);
		glProgramUniform1f(shader_program_id,myLoc, v);
	}

// glUniform2f
	void set_f2(GLuint shader_program_id, float a, float b, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2f(shader_program_id, myLoc, a, b); //Not tested
	}

	void set_f3(GLuint shader_program_id, float a, float b, float c, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform3f(myLoc, a, b, c);
		glProgramUniform3f(shader_program_id, myLoc, a, b, c);
	}

// glUniform4f 
	void set_f4(GLuint shader_program_id, float a, float b, float c, float d, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform3f(myLoc, a, b, c);
		glProgramUniform4f(shader_program_id, myLoc, a, b, c, d);
	}
	// ------------ Float vectors
// glUniform1fv
	// Cannot use this function as have compile linking problems
/*	void set_vec1(GLuint shader_program_id, std::vector<float>& vec1, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform1fv(shader_program_id, myLoc, 1, glm::value_ptr(vec1)); //Not tested
	}
*/
// glUniform2fv
	void set_vec2(GLuint shader_program_id, glm::vec2& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2fv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

 //glUniform3fv
	void set_vec3(GLuint shader_program_id, const glm::vec3& vec3, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3fv(shader_program_id, myLoc, 1, glm::value_ptr(vec3));
	}

// glUniform4fv
	void set_vec4(GLuint shader_program_id, const glm::vec4& vec4, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform4fv(shader_program_id, myLoc, 1, glm::value_ptr(vec4));
	}
	// ------------ Integers
	void set_i1(GLuint shader_program_id, int v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform1i(myLoc, v);
		glProgramUniform1i(shader_program_id, myLoc, v);
	}

// glUniform2i
	void set_i2(GLuint shader_program_id, int a, int b, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2i(shader_program_id, myLoc, a, b); //Not tested
	}
// glUniform3i
	void set_i3(GLuint shader_program_id, int a, int b,int c, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3i(shader_program_id, myLoc, a, b,c); //Not tested
	}

// glUniform4i 
	void set_i4(GLuint shader_program_id, int a, int b, int c,int d, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform4i(shader_program_id, myLoc, a, b, c,d); //Not tested
	}
	// ------------ Integer Vectors
// glUniform1iv
   	// Cannot use this function as have compile linking problems
	//void set_ivec1(GLuint shader_program_id, glm::ivec1& vec1, const std::string& name) {
/*	void set_ivec1(GLuint shader_program_id, std::vector<int>& vec1, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform1iv(shader_program_id, myLoc, 1, glm::value_ptr(vec1)); //Not tested
	}
*/
// glUniform2iv
	void set_ivec2(GLuint shader_program_id, glm::ivec2& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2iv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

// glUniform3iv 
	void set_ivec3(GLuint shader_program_id, glm::ivec3& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3iv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

// glUniform4iv
	void set_ivec4(GLuint shader_program_id, glm::ivec4& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform4iv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

	// ------------ Unsigned integers

	void set_ui1(GLuint shader_program_id, unsigned int v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform1i(myLoc, v);
		glProgramUniform1ui(shader_program_id, myLoc, v);
	}

	void set_ui2(GLuint shader_program_id, unsigned int a, unsigned int b, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2ui(shader_program_id, myLoc, a, b); //Not tested
	}

	void set_ui3(GLuint shader_program_id, unsigned int a, unsigned int b, unsigned int c, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3ui(shader_program_id, myLoc, a, b, c); //Not tested
	}

	void set_ui4(GLuint shader_program_id, unsigned int a, unsigned int b, unsigned int c, unsigned int d, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform4ui(shader_program_id, myLoc, a, b, c, d); //Not tested
	}

	// ------------ Unsigned integer vectors
	// Cannot use this function as have compile linking problems
	/*void set_uivec1(GLuint shader_program_id, std::vector<unsigned int> vec1, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform1uiv(shader_program_id, myLoc, 1, glm::value_ptr(vec1)); //Not tested
	}
	*/

	// glUniform2iv
	void set_uivec2(GLuint shader_program_id, glm::uvec2& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2uiv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

	// glUniform3iv 
	void set_uivec3(GLuint shader_program_id, glm::uvec3& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3uiv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}

	// glUniform4iv
	void set_uivec4(GLuint shader_program_id, glm::uvec4& v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform4uiv(shader_program_id, myLoc, 1, glm::value_ptr(v)); //Not tested
	}
	// ------------ Matrices

// glUniformMatrix2fv 
	void set_mat2(GLuint shader_program_id, const glm::mat2& mat2, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix2fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat2));//Not tested
	}
// glUniformMatrix3fv
	void set_mat3(GLuint shader_program_id, const glm::mat3& mat3, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix3fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat3));//Not tested
	}

	void set_mat4(GLuint shader_program_id, const glm::mat4 & mat4, const std::string & name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
		glProgramUniformMatrix4fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void set_mat2x3(GLuint shader_program_id, const glm::mat2x3& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix2x3fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}

	void set_mat3x2(GLuint shader_program_id, const glm::mat3x2& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix3x2fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}

	void set_mat2x4(GLuint shader_program_id, const glm::mat2x4& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix2x4fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}

	void set_mat4x2(GLuint shader_program_id, const glm::mat4x2& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix4x2fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}

	void set_mat3x4(GLuint shader_program_id, const glm::mat3x4& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix3x4fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}

	void set_mat4x3(GLuint shader_program_id, const glm::mat4x3& mat, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniformMatrix4x3fv(shader_program_id, myLoc, 1, GL_FALSE, glm::value_ptr(mat));//Not tested
	}


// not sure about these
// glVertexAttrib1d 
// glVertexAttrib1dv 
// glVertexAttrib1f 
// glVertexAttrib1fv 
// glVertexAttrib1s 
// glVertexAttrib1sv 
// glVertexAttrib2d 
// glVertexAttrib2dv 
// glVertexAttrib2f 
// glVertexAttrib2fv 
// glVertexAttrib2s 
// glVertexAttrib2sv 
// glVertexAttrib3d 
// glVertexAttrib3dv 
// glVertexAttrib3f 
// glVertexAttrib3fv 
// glVertexAttrib3s 
// glVertexAttrib3sv 
// glVertexAttrib4Nbv 
// glVertexAttrib4Niv 
// glVertexAttrib4Nsv 
// glVertexAttrib4Nub 
// glVertexAttrib4Nubv 
// glVertexAttrib4Nuiv 
// glVertexAttrib4Nusv 
// glVertexAttrib4bv 
// glVertexAttrib4d 
// glVertexAttrib4dv 
// glVertexAttrib4f 
// glVertexAttrib4fv 
// glVertexAttrib4iv 
// glVertexAttrib4s 
// glVertexAttrib4sv 
// glVertexAttrib4ubv 
// glVertexAttrib4uiv 
// glVertexAttrib4usv 
// glVertexAttribPointer



	GLuint shader_compile_successful = 0;
	//bool shader_compile_successful = true;
	std::string compile_log = "";
	GLuint compile_shader_type;

private:
	GLuint program_id;

	GLuint get_compiled_shader(GLuint shader_type, const std::string& shader_source) {
		GLuint shader_id = glCreateShader(shader_type);

		const char* c_source = shader_source.c_str();
		glShaderSource(shader_id, 1, &c_source, nullptr);
		glCompileShader(shader_id);

		GLint result;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

			GLchar* strInfoLog = new GLchar[length + 1];
			glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

			switch (shader_type) {
				case GL_VERTEX_SHADER   : fprintf(stderr, "Compile error in Vertex Shader: %s\n", strInfoLog);   break;
				case GL_GEOMETRY_SHADER : fprintf(stderr, "Compile error in Geometry Shader: %s\n", strInfoLog); break;
				case GL_FRAGMENT_SHADER : fprintf(stderr, "Compile error in Fragmant Shader: %s\n", strInfoLog); break;
			}

			//fprintf(stderr, "Compile error in  shader: %s\n", strInfoLog);

			compile_log = strInfoLog;
			compile_shader_type = shader_type;
			shader_compile_successful = 0;

			delete[] strInfoLog;

			//glDeleteShader(shader_id);
			//return -1;
		}

		return shader_id;
	}

};