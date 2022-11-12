/*
#include "shader_format.h"
#include "shader_manager.h"

bool shader_format_class::update_shader_variables() {
	shader_db_manager_class shader_manager;

	if (!shader_program_id) return false;

	// Floats
	for (int i = 0; i < float_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_f1(shader_program_id, float_uniforms.uniforms[i].value, float_uniforms.uniforms[i].name);
	}

	//for (int i = 0; i < float2_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f2(shader_program_id, float2_uniforms.uniforms[i].value[0], float2_uniforms.uniforms[i].value[1], float2_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < float3_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f3(shader_program_id, float3_uniforms.uniforms[i].value[0], float3_uniforms.uniforms[i].value[1], float3_uniforms.uniforms[i].value[2], float3_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < float4_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f4(shader_program_id, float4_uniforms.uniforms[i].value[0], float4_uniforms.uniforms[i].value[1], float4_uniforms.uniforms[i].value[2], float4_uniforms.uniforms[i].value[3], float4_uniforms.uniforms[i].name);
	//}

	// integers
	for (int i = 0; i < int_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_i1(shader_program_id, int_uniforms.uniforms[i].value, int_uniforms.uniforms[i].name);
	}

	//for (int i = 0; i < float2_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f2(shader_program_id, float2_uniforms.uniforms[i].value[0], float2_uniforms.uniforms[i].value[1], float2_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < float3_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f3(shader_program_id, float3_uniforms.uniforms[i].value[0], float3_uniforms.uniforms[i].value[1], float3_uniforms.uniforms[i].value[2], float3_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < float4_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_f4(shader_program_id, float4_uniforms.uniforms[i].value[0], float4_uniforms.uniforms[i].value[1], float4_uniforms.uniforms[i].value[2], float4_uniforms.uniforms[i].value[3], float4_uniforms.uniforms[i].name);
	//}

	// Booleans
	for (int i = 0; i < bool_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_b1(shader_program_id, bool_uniforms.uniforms[i].value, bool_uniforms.uniforms[i].name);
	}

	// Unsigned integers
	for (int i = 0; i < uint_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_ui1(shader_program_id, uint_uniforms.uniforms[i].value, uint_uniforms.uniforms[i].name);
	}

	//for (int i = 0; i < uint2_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_ui2(shader_program_id, uint2_uniforms.uniforms[i].value[0], uint2_uniforms.uniforms[i].value[1], uint2_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < uint3_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_ui3(shader_program_id, uint3_uniforms.uniforms[i].value[0], uint3_uniforms.uniforms[i].value[1], uint3_uniforms.uniforms[i].value[2], uint3_uniforms.uniforms[i].name);
	//}

	//for (int i = 0; i < uint4_uniforms.uniforms.size(); i++) {
	//	shader_manager.shader.set_ui4(shader_program_id, uint4_uniforms.uniforms[i].value[0], uint4_uniforms.uniforms[i].value[1], uint4_uniforms.uniforms[i].value[2], uint4_uniforms.uniforms[i].value[3], uint4_uniforms.uniforms[i].name);
	//}

	// vec
	for (int i = 0; i < vec2_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_vec2(shader_program_id, vec2_uniforms.uniforms[i].value, vec2_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < vec3_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_vec3(shader_program_id, vec3_uniforms.uniforms[i].value, vec3_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < vec4_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_vec4(shader_program_id, vec4_uniforms.uniforms[i].value, vec4_uniforms.uniforms[i].name);
	}

	// ivec
	for (int i = 0; i < ivec2_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_ivec2(shader_program_id, ivec2_uniforms.uniforms[i].value, ivec2_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < ivec3_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_ivec3(shader_program_id, ivec3_uniforms.uniforms[i].value, ivec3_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < ivec4_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_ivec4(shader_program_id, ivec4_uniforms.uniforms[i].value, ivec4_uniforms.uniforms[i].name);
	}

	// uvec
	for (int i = 0; i < uvec2_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_uivec2(shader_program_id, uvec2_uniforms.uniforms[i].value, uvec2_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < vec3_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_uivec3(shader_program_id, uvec3_uniforms.uniforms[i].value, uvec3_uniforms.uniforms[i].name);
	}

	for (int i = 0; i < vec4_uniforms.uniforms.size(); i++) {
		shader_manager.shader.set_uivec4(shader_program_id, uvec4_uniforms.uniforms[i].value, uvec4_uniforms.uniforms[i].name);
	}

	return true;
}
*/