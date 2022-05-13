#pragma once

#include "../Common/definitions.h"

#include "shader.h"

/*
			Shader Manager class

	Class that stores and manages all the
	shader programs information and shader
	programs.



*/

class shader_db_manager_class {
public:
	shader_db_manager_class() {}
	~shader_db_manager_class() {}

	shader_class          shader;

	std::vector<idu_type> shaders;// List of all created shader program ids

	bool use_shader(int shader_program_id) {
		if (!shader_program_exists(shader_program_id)) {
			// log message to go here
			return false;
		}

		shader.use(shader_program_id);
		
		return true;
	}

	void release_shader(int shader_program_id) {
		shader.unload(shader_program_id);
	}

	bool shader_program_exists(int shader_program_id) {
		if (shader_program_id < 0) return false;

		for (int i = 0; i < shaders.size(); i++) {
			if (shaders[i] == shader_program_id)
				return true;
		}

		return false;
	}

	int get_shader_id_index(idu_type shader_id) {
		for (int i = 0; i < shaders.size(); i++) {
			if (shaders[i] = shader_id) return i;
		}

		return -1;
	}

	int create_shader_program(const std::string& vertex_shader_file,
							  const std::string& geometry_shader_file,
							  const std::string& fragment_shader_file) {

		int shader_id = shader.create_shader_program(vertex_shader_file, geometry_shader_file, fragment_shader_file);

		if (shader_id < 1) {
			// error message + shader.compile_log
			return 0;
		}

		shaders.push_back(shader_id);

		return shader_id;
	}

	void remove_shader_program(idu_type shader_id) {
		shader.unload(shader_id);
		delete_shader_id(shader_id);
	}

	void delete_shader_id(idu_type shader_id) {
		int index = get_shader_id_index(shader_id);

		if (index > -1 )
			shaders.erase(shaders.begin() + index);
	}

private:
};
