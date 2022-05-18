begin_function

end_function

begin_expression
	vec4 center = gl_in[0].gl_Position; 
	
	
	gs_out.varyingColor = gs_in[0].varyingColor;
    //gl_Position = projection * view * model * center;
    gl_Position = mvpMatrix * center;
	
	//varyingColor = vec4(1.0,1.0,1.0,1.0);

    EmitVertex();
    EndPrimitive();

end_expression
