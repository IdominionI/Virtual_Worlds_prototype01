#pragma once

#include "../Common/pch.h"

#include "shader_format.h"
#include "ShaderPreprocessor.h"

/*
						Shader Class
 
	Class to load, compile and attach a vertex, geometry and fragment shader to a program
	and to define and assign glsl unifor shader variables to a shader program of a given
	shader program id.

	If a compilation of a shader program fails, the compilation error log is stored in the
	string variable compile_log that can be referenced by the calling function.
	
	NOTE: Absolutly critical that all datatypes to openGL shader compilaions are of type
		  GLuint or there are problems that can arise such as not being able to change
          the shader program in a hot reload. Crazy openGL that uses numbers insead of
          pointers and makes things all that more diffucult.
*/


class shader_class
{
public:
	shader_class() = default;

	// #############################################################################################

	bool use_preprocessor = true;
	std::string error_log = "";
	//shader_format_class shader_program_format;

	// Load a vertex and a fragment shader from file
	GLuint create_glsl_shader_program(std::string vertex_shader_file,
									  std::string geometry_shader_file,
									  std::string fragment_shader_file) {

		bool have_geometry_shader = false;
		compile_log = "";
		program_id = 0;
		shader_compile_successful = 0;

//printf("create_shader_program :Vertex glsl file %s\n", vertex_shader_file.c_str());
//printf("create_shader_program :Geometry glsl file %s\n", geometry_shader_file.c_str());
//printf("create_shader_program :Fragment glsl file %s\n", fragment_shader_file.c_str());


		if (vertex_shader_file == "") {
			compile_log = "Compile error in Vertex shader : No vertex file defined to compile \n";
			return program_id;
		}

		if (fragment_shader_file == "") {
			compile_log = "Compile error in Fragment shader : No fragment file defined to compile \n";
			return program_id;
		}
//printf("\n create_shader_program vshader id 00000 :  \n");
		if (geometry_shader_file != "") have_geometry_shader = true;

		//	mGlsl = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("Shaders/Default/Voxel/default_gen_TRD_VS.glsl"))
				//.fragment(loadAsset("shaders/basic.frag"))
		//		.geometry(loadAsset("Shaders/Default/Voxel/default_gen_TRD_PGS.glsl"))
		//		.fragment(loadAsset("Shaders/Default/Voxel/default_gen_TRD_FS.glsl")));

		//DataSourceRef		loadAsset( const fs::path &relativePath )					{ return Platform::get()->loadAsset( relativePath ); }

		shader_format_class  shader_program_format;
//printf("create_shader_program ==>> vshader id 11111 :  \n");
		if (!shader_program_format.define_shader_to_compile(shader_type_enum::vertex, vertex_shader_file)) {
			return shader_compile_successful;
		}

		if (have_geometry_shader) {
			if (!shader_program_format.define_shader_to_compile(shader_type_enum::geometry, geometry_shader_file)) {
				return shader_compile_successful;
			}
		}

		if (!shader_program_format.define_shader_to_compile(shader_type_enum::fragment, fragment_shader_file)) {
			return shader_compile_successful;
		}

//printf("create_shader_program vshader id 222222 :  \n");
		//shader_compile_successful = create_glsl_program(shader_program_format);
		shader_compile_successful = create_glsl_program(shader_program_format);

//printf("create_shader_program vshader id 222A : \n");
//printf("create_shader_program vshader id 222B : %i \n", shader_compile_successful);

		if (shader_compile_successful <= 0) {
			compile_log = error_log;
			//shader_program.reset();
			program_id = 0;
//printf("create_shader_program vshader id 3333 : %s \n ", error_log.c_str());
		}
		else {
//printf("create_shader_program vshader id 4444 : %s \n ", error_log.c_str());
			program_id = shader_compile_successful;
		}

//printf("Definitely USED create_glsl_shader_program !!!!!\n ");
		return program_id;
	}


	GLuint create_glsl_shader_program(shader_format_class shader_program_format, bool _use_preprocessor = true) {
				bool have_geometry_shader = false;
		compile_log = "";
		program_id = 0;
		shader_compile_successful = 0;

//printf("create_shader_program :Vertex glsl file %s\n", shader_program_format.glsl_vertex_shader_file_pathname.c_str());
//printf("create_shader_program :Geometry glsl file %s\n", shader_program_format.glsl_geometry_shader_file_pathname.c_str());
//printf("create_shader_program :Fragment glsl file %s\n", shader_program_format.glsl_fragment_shader_file_pathname.c_str());


		if (shader_program_format.glsl_vertex_shader_file_pathname == "") {
			compile_log = "Compile error in Vertex shader : No vertex file defined to compile \n";
			return program_id;
		}

		if (shader_program_format.glsl_fragment_shader_file_pathname == "") {
			compile_log = "Compile error in Fragment shader : No fragment file defined to compile \n";
			return program_id;
		}
//printf("\n create_shader_program vshader id 00000 :  \n");
		if (shader_program_format.glsl_geometry_shader_file_pathname != "") have_geometry_shader = true;

		//	mGlsl = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("Shaders/Default/Voxel/default_gen_TRD_VS.glsl"))
				//.fragment(loadAsset("shaders/basic.frag"))
		//		.geometry(loadAsset("Shaders/Default/Voxel/default_gen_TRD_PGS.glsl"))
		//		.fragment(loadAsset("Shaders/Default/Voxel/default_gen_TRD_FS.glsl")));

		//DataSourceRef		loadAsset( const fs::path &relativePath )					{ return Platform::get()->loadAsset( relativePath ); }

//printf("create_shader_program ==>> vshader id 11111 :  \n");
		if (!shader_program_format.define_shader_to_compile(shader_type_enum::vertex, shader_program_format.glsl_vertex_shader_file_pathname.string())) {
			return shader_compile_successful;
		}

		if (have_geometry_shader) {
			if (!shader_program_format.define_shader_to_compile(shader_type_enum::geometry, shader_program_format.glsl_geometry_shader_file_pathname.string())) {
				return shader_compile_successful;
			}
		}

		if (!shader_program_format.define_shader_to_compile(shader_type_enum::fragment, shader_program_format.glsl_fragment_shader_file_pathname.string())) {
			return shader_compile_successful;
		}

//printf("create_shader_program vshader id 222222 :  \n");
		//shader_compile_successful = create_glsl_program(shader_program_format);
		shader_compile_successful = create_glsl_program(shader_program_format);

//printf("create_shader_program vshader id 222A : \n");
//printf("create_shader_program vshader id 222B : %i \n", shader_compile_successful);

		if (shader_compile_successful <= 0) {
			compile_log = error_log;
			//shader_program.reset();
			program_id = 0;
//printf("create_shader_program vshader id 3333 : %s \n ", error_log.c_str());
		}
		else {
//printf("create_shader_program vshader id 4444 : %s \n ", error_log.c_str());
			program_id = shader_compile_successful;
		}

//printf("Definitely USED create_glsl_shader_program !!!!!\n ");
		return program_id;
	}
	


	GLuint create_glsl_program(shader_format_class shader_program_format, bool _use_preprocessor = true) {
		use_preprocessor = _use_preprocessor;
		//shader_program_format = _shader_program_format;
		error_log = "";

		if (!shader_program_format.vertex_source_code_defined || !shader_program_format.fragment_source_code_defined) {
			error_log = "FATAL ERROR : Do not have a vertex or fragmant glsl source code defined for files \n";
			error_log += shader_program_format.vertex_shader_file_pathname.string() + "\n";
			error_log += shader_program_format.fragment_shader_file_pathname.string() + "\n";
			error_log += "Must have source code defined for both these shader types defined as a minimum.\n";

			return NULL;
		}

		program_id = glCreateProgram();
		mHandle = program_id; // Delete mHandle when not needed !!!!

		GLuint vertex_handle = loadShader(shader_program_format, shader_program_format.vertex_shader_source_code, shader_program_format.glsl_vertex_shader_file_pathname, GL_VERTEX_SHADER);
		if (vertex_handle <= 0) return 0;

		GLuint geometry_handle;
		if (shader_program_format.geometry_source_code_defined) {
			geometry_handle = loadShader(shader_program_format, shader_program_format.geometry_shader_source_code, shader_program_format.glsl_geometry_shader_file_pathname, GL_GEOMETRY_SHADER);
			if (geometry_handle <= 0) return 0;
		}

		GLuint fragment_handle = loadShader(shader_program_format, shader_program_format.fragment_shader_source_code, shader_program_format.glsl_fragment_shader_file_pathname, GL_FRAGMENT_SHADER);
		if (fragment_handle <= 0) return 0;

		// Other shader types tesselation and compute to go here ?????

		glLinkProgram(program_id);
		glValidateProgram(program_id);

		glDeleteShader(vertex_handle);
		if (shader_program_format.geometry_source_code_defined) glDeleteShader(geometry_handle);
		glDeleteShader(fragment_handle);

		return program_id;
	}
	

	GLuint loadShader(shader_format_class shader_program_format, std::string& shaderSource, const std::filesystem::path& shaderPath, GLint shaderType)
	{
		GLuint handle = 0;

		if (!shaderSource.empty()) {
			handle = glCreateShader(shaderType);
//printf("\n ==>> GlslProg loadShader 00000 : %s \n", shaderPath.string().c_str());
			if (use_preprocessor) {
				std::set<std::filesystem::path> includedFiles;
//printf("\n ==>> GlslProg loadShader 11111AAAAA : \n");
				//std::string preprocessedSource = preprocessor.parse(shaderSource, shaderPath, &includedFiles);
				std::string preprocessedSource = preprocessor.parse(shader_program_format,shaderSource, shaderPath, &includedFiles); // !!!!!!!!!!!!!!!!!!!!!!
				if (!preprocessor.mparse_successfull_m) {
					shader_compile_successful = 0;    // ****** mod flag set failed to compile value
					Create_Compilation_log(preprocessedSource, shaderType, shaderPath.string(), shaderSource); // ****** mod
					return(shader_compile_successful);// ****** mod
				}
//printf("\n ==>> GlslProg loadShader 11111BBBBB : \n");
				mShaderPreprocessorIncludedFiles.insert(mShaderPreprocessorIncludedFiles.end(), includedFiles.begin(), includedFiles.end());
//printf("\n ==>> GlslProg loadShader 11111 : \n");
				const char* cStr = preprocessedSource.c_str();
				glShaderSource(handle, 1, reinterpret_cast<const GLchar**>(&cStr), NULL);
			}
			else {
//printf("\n ==>> GlslProg loadShader 22222 : \n");
				const char* cStr = shaderSource.c_str();
				glShaderSource(handle, 1, reinterpret_cast<const GLchar**>(&cStr), NULL);
			}
//printf("\n ==>> GlslProg loadShader 33333 : \n");
			glCompileShader(handle);
//printf("\n ==>> GlslProg loadShader 44444 : \n");
			GLint status;
			glGetShaderiv((GLuint)handle, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE) {
				std::string log = getShaderLog((GLuint)handle);

//printf("\n ==>> GlslProg loadShader 55555 : \n");
				// Since the GlslProg destructor will not be called after we throw, we must delete all
				// owned GL objects here to avoid leaking. Any other attached shaders will be cleaned up
				// when the program is deleted
				glDeleteShader(handle);
				glDeleteProgram(program_id);
//printf("\n ==>> GlslProg loadShader 666666 : \n");
				// disabled throw command and enabled a failed flag to be returned to be acted upon
				// so as not to have the application terminated.
				shader_compile_successful = 0;    // ****** mod flag set failed to compile value
				Create_Compilation_log(log, shaderType, shaderPath.string(), shaderSource); // ****** mod
				return(shader_compile_successful);// ****** mod

				//throw GlslProgCompileExc( log, shaderType );// ****** mod


			}

			glAttachShader(program_id, handle);

			// Scope the shader's lifetime to the program's. It will be cleaned up when later detached.
			glDeleteShader(handle);
		}

		return handle;
	}

	std::string getShaderLog(GLuint handle) const
	{
		std::string log;

		GLchar* debugLog;
		GLint charsWritten = 0;
		GLint debugLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &debugLength);

		if (debugLength > 0) {
			debugLog = new GLchar[debugLength];
			glGetShaderInfoLog(handle, debugLength, &charsWritten, debugLog);
			log.append(debugLog, 0, debugLength);
			delete[] debugLog;
		}

		return log;
	}

	// Below code to be put into private:

	std::vector<std::filesystem::path> mShaderPreprocessorIncludedFiles;
	ShaderPreprocessor preprocessor;
	GLuint mHandle = 0;
	// #################### MOD FUNTION Create_Compilation_log ADDED #############################
	void Create_Compilation_log(const std::string& log, GLint shaderType, const std::string& pathname, const std::string& shader_source)
	{
		std::string typeString;
		error_log = "";

		switch (shaderType) {
		case GL_VERTEX_SHADER:			typeString = "Compile error in Vertex shader: \n"; break;
		case GL_FRAGMENT_SHADER:		typeString = "Compile error in Fragment shader: \n"; break;
		case GL_GEOMETRY_SHADER:		typeString = "Compile error in Geometry shader: \n"; break;
		case GL_TESS_CONTROL_SHADER:	typeString = "Compile error in Tessellation control: \n"; break;
		case GL_TESS_EVALUATION_SHADER:	typeString = "Compile error in Tessellation evaluation: \n"; break;
		case GL_COMPUTE_SHADER:			typeString = "Compile error in Compute shader: \n"; break;
		default:						typeString = "Compile error UNKNOWN: \n";
		}

		error_log = (typeString + pathname + "\n" + shader_source + "\n" + log + "\n");
	}
	// #################### END MOD Create_Compilation_log FUNTION ADDED #############################

	// -----------------

	// #######################################################################

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
//printf("shader b1 : %i : %i  : %s \n", shader_program_id, v, name.c_str());
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform1i(shader_program_id, myLoc, v);
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
	void set_i1(GLuint shader_program_id, GLint v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform1i(myLoc, v);
		glProgramUniform1i(shader_program_id, myLoc, v);
	}

// glUniform2i
	void set_i2(GLuint shader_program_id, GLint a, GLint b, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2i(shader_program_id, myLoc, a, b); //Not tested
	}
// glUniform3i
	void set_i3(GLuint shader_program_id, GLint a, GLint b, GLint c, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3i(shader_program_id, myLoc, a, b,c); //Not tested
	}

// glUniform4i 
	void set_i4(GLuint shader_program_id, GLint a, GLint b, GLint c, GLint d, const std::string& name) {
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

	void set_ui1(GLuint shader_program_id, GLuint v, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		//glUniform1i(myLoc, v);
		glProgramUniform1ui(shader_program_id, myLoc, v);
	}

	void set_ui2(GLuint shader_program_id, GLuint a, GLuint b, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform2ui(shader_program_id, myLoc, a, b); //Not tested
	}

	void set_ui3(GLuint shader_program_id, GLuint a, GLuint b, GLuint c, const std::string& name) {
		GLint myLoc = glGetUniformLocation(shader_program_id, name.c_str());
		glProgramUniform3ui(shader_program_id, myLoc, a, b, c); //Not tested
	}

	void set_ui4(GLuint shader_program_id, GLuint a, GLuint b, GLuint c, GLuint d, const std::string& name) {
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