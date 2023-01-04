begin_function

end_function

begin_expression
	vertex = vec3(vertex_data.x,vertex_data.y,vertex_data.z);
	
	gl_Position = vec4(vertex, 1.0f);        // required as a min
	//value = voxel_value;                   // required as a min
	value = int(vertex_data.w);              // required as a min

	vs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);

end_expression