begin_function

//layout(location=0) out vec4 frag_color;

end_function

begin_expression
	frag_color= fs_in.varyingColor;

	//out vec4 myOutputColor;
	//myOutputColor  = fs_in.varyingColor;
   
   //gl_FragColor = fs_in.varyingColor;
   //gl_FragColor = vec4(1.0,0.5,1.0,1.0);
   
end_expression