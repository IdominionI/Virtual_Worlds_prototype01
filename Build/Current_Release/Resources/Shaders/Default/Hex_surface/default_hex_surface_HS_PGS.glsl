begin_function

// A layout for the output must be defined for whatever geometry shader functions are performed
layout(points, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions

vec4 point_0 = vec4(0.0,0.0,0.0,1.0);

end_function



begin_expression

	vec4 center = gl_in[0].gl_Position;
	
	gs_out.varyingColor = gs_in[0].varyingColor;
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	EndPrimitive();
	
end_expression