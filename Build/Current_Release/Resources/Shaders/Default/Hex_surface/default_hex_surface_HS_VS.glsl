begin_function


end_function

begin_expression

	// following required as a min
	//if(vEnabledFaces != 0)
	//	gl_Position = mvpMatrix*(vertex+vec4(0.0,0.0,0.0,1.0)); // required as a min
	//else
	gl_Position = vertex; // required as a min

	raw_color = vec4(1.0,1.0,1.0,1.0); // required as a min

end_expression