/*
 Copyright (c) 2015, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "ShaderPreprocessor.h"

#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>

#include <Universal/Framework/FWfile_tools.h>

namespace {
// due to null or the rest of the line being a comment (//)
bool isTerminated( const char *c )
{
	if( *c == '\0' )
		return true;
	const char *next = c + 1;
	if( *c == '/' && *next && *next == '/' )
		return true;
	return false;
}

bool isWhiteSpace( const char *c )
{
	return *c == ' ' || *c == '\t';
}

bool isInlineCommentStart( const char *c )
{
	if( (*c == '\0') || (*c != '/') )
		return false;
	const char *next = c + 1;
	if( *next && *next == '*' )
		return true;
	return false;
}

bool isInlineCommentEnd( const char *c )
{
	if( (*c == '\0') || (*c != '*') )
		return false;
	const char *next = c + 1;
	if( *next && *next == '/' )
		return true;
	return false;
}

// consumes tabs, spaces, and inline comments; sets 'c' to first character that is none of these, or the terminus
void consumeWhiteSpace( const char **c )
{
	while( ( ! isTerminated( *c ) ) && isWhiteSpace( *c ) )
		++*c;
	if( isTerminated( *c ) )
		return;
	// did we encounter an inline comment?
	while( isInlineCommentStart( *c ) || isWhiteSpace( *c ) ) {
		while( isWhiteSpace( *c ) )
			++*c;
		const char *startBeforeComment = *c;
		while( ! isTerminated( *c ) && ! isInlineCommentEnd( *c ) )
			++*c;
		if( isTerminated( *c ) ) {
			*c = startBeforeComment;
			break;
		}
		else
			*c += 2;
	}
	return;
}

bool findIncludeStatement( const std::string &line, std::string *out )
{
	const int INCLUDE_KEYWORD_LEN = 7;
	const char *resultStart = nullptr;
	const char *c = line.c_str();
	consumeWhiteSpace( &c );
	// leading '#'
	if( ( isTerminated( c ) ) || ( *c != '#' ) )
		return false;
	++c;
	consumeWhiteSpace( &c );
	if( isTerminated( c ) )
		return false;
	// include keyword
	if( strncmp( c, "include", INCLUDE_KEYWORD_LEN ) )
		return false;
	c += INCLUDE_KEYWORD_LEN;
	consumeWhiteSpace( &c );
	// leading quote or angle bracket
	if( ( isTerminated( c ) ) || ( *c != '\"' && *c != '<' ) )
		return false;
	++c;
	resultStart = c;
	while( ( ! isTerminated( c ) )  && *c != '\"' && *c != '>' )
		++c;
	if( isTerminated( c ) ) // we hit the terminus before the closing symbol
		return false;
	if( out )
		*out = std::string( resultStart, c );
	return true;
}

bool findVersionStatement( const char *lineStart, int *versionNumberOut )
{
 printf("findVersionStatement 000 \n");
	const int VERSION_KEYWORD_LEN = 7;
	const char *c = lineStart;
	consumeWhiteSpace( &c );
	// leading '#'

	if( isTerminated( c ) || ( *c != '#' ) )
		return false;
	++c;
 printf("findVersionStatement 111 \n");	
	consumeWhiteSpace( &c );
	
	if( isTerminated( c ) )
		return false;
	
	// version keyword
	if( strncmp( c, "version", VERSION_KEYWORD_LEN ) ) // 7 == strlen( "version" )
		return false;

	c += VERSION_KEYWORD_LEN;
	consumeWhiteSpace( &c );
	// leading digit
	if( isTerminated( c ) || (! isdigit( *c )) )
	
		return false;
	if( versionNumberOut )
		*versionNumberOut = (int)std::strtol( c, nullptr, 0 );
	
	return true;
}
} // anonymous namespace

/*
ShaderPreprocessor::ShaderPreprocessor()
	: mUseFilenameInLineDirective( false )
{
	mSearchDirectories.push_back( app::Platform::get()->getAssetPath( "" ) );

	// set the default version
#if defined( CINDER_GL_ES_3 )
	mVersion = 300;
#elif defined( CINDER_GL_ES_2 )
	mVersion = 100;
#else // desktop
	mVersion = 150;
#endif
}
*/

//std::string ShaderPreprocessor::parse(const std::filesystem::path & sourcePath, std::set<std::filesystem::path> *includedFiles)
std::string ShaderPreprocessor::parse(shader_format_class shader_format, const std::filesystem::path & sourcePath, std::set<std::filesystem::path> *includedFiles)
{
	const std::filesystem::path fullPath = findFullPath( sourcePath, "" );
	if( fullPath.empty() ) {
//throw ShaderPreprocessorExc( "Could not find shader at source path: " + sourcePath.string() );
//CI_LOG_E("ERROR !!!!! : Could not find shader at source path: " + sourcePath.string());   // ***** Mod
		mparse_successfull_m = false;												// ***** Mod
		return "ERROR !!!!! :Could not find shader at source path: " + sourcePath.string();		// ***** Mod
	}

	mparse_successfull_m = true; // ***** Mod

	FW::filetools::read_all(fullPath.string());

	//std::string source = loadString( loadFile( fullPath ) );
	std::string source = FW::filetools::read_all(fullPath.string());
	//return parse( source, sourcePath, includedFiles );
	return parse(shader_format, source, sourcePath, includedFiles );
}

//std::string ShaderPreprocessor::parse( const std::string &source, const std::filesystem::path &sourcePath, std::set<std::filesystem::path> *includedFiles )
std::string ShaderPreprocessor::parse(shader_format_class shader_format, std::string &source, const std::filesystem::path &sourcePath, std::set<std::filesystem::path> *includedFiles)
{
 printf("parse 000 \n");
	std::set<std::filesystem::path> localIncludeTree; // even if user didn't ask for includedFiles, keep track of them to detect recursion
	if( ! includedFiles )
		includedFiles = &localIncludeTree;
	else
		includedFiles->clear();

	std::string directives;
	std::string sourceBody;
	int versionNumber = -1;
	int lineNumberStart;
 printf("parse 111 \n");

	// ########### Add here function to add application dynamic uniform variables ##################
		parse_application_uniforms(shader_format, source);
//printf("parse 22222 source after parse_application_uniforms : source_code  \n %s \n", source.c_str());
	// #############################

	parseDirectives( source, sourcePath, &directives, &sourceBody, &versionNumber, &lineNumberStart );
	if( directives.empty() ) {
		// There were no directives added, parse original source for includes
		return parseTopLevel( source, sourcePath, lineNumberStart, versionNumber, *includedFiles );
	}
	else {
		// Parse the remaining source and then append it to the directives string
		std::string result = parseTopLevel( sourceBody, sourcePath, lineNumberStart, versionNumber, *includedFiles );
		return directives + result;
	}
}

// ####################################
bool ShaderPreprocessor::parse_application_uniforms(shader_format_class shader_format, std::string& source_code) {
	std::string input_line = "";
	//int line_number = 0;
	size_t current_source_code_location = 0, lineEndPos =0;

//printf("parse_application_uniforms 0000 : %i \n %s \n", source_code.size(), source_code.c_str());

	// Move this define to a more appropriate source file and location
	#define DYNAMIC_UNIFORM_FLAG "#DD#"

	while (input_line.find(DYNAMIC_UNIFORM_FLAG) == (std::string::npos)){
		lineEndPos = source_code.find('\n', current_source_code_location);
		if (lineEndPos == std::string::npos) {
 //printf("parse_application_uniforms 11111 lineEndPos == std::string::npos %i : \n %i \n", current_source_code_location, lineEndPos);
			break;
		}
		
		input_line = source_code.substr(current_source_code_location, lineEndPos-current_source_code_location);

		current_source_code_location = lineEndPos+1;
//printf("parse_application_uniforms 2222 : %i \n %s \n", source_code.size(), input_line.c_str());
	}

	if ((input_line.find(DYNAMIC_UNIFORM_FLAG) == (std::string::npos))) return false; // Does not mean source code invalid as may not have any application uniforms to declare

//printf("parse_application_uniforms 2222AAA  %i : %i \n", current_source_code_location, source_code.size() - current_source_code_location);

	std::string source_code_prev  = source_code.substr(0, current_source_code_location);
	std::string source_code_after = source_code.substr(current_source_code_location, source_code.size()-current_source_code_location);

//printf("parse_application_uniforms 3333 : previous \n %s \n  after \n %s \n", source_code_prev.c_str(), source_code_after.c_str());


	std::string uniform_source_code = "";
	for (uniform_struct_type uniform : shader_format.user_uniforms) {
		uniform_source_code += "uniform ";
		
		switch (uniform.data_type) {
			case application_default_shader_variable_type_enum::Bool:     uniform_source_code += "int ";    break;
			case application_default_shader_variable_type_enum::Float1:   uniform_source_code += "float ";  break;
			case application_default_shader_variable_type_enum::Int1:     uniform_source_code += "int ";    break;
			case application_default_shader_variable_type_enum::UInt1:    uniform_source_code += "uint ";   break;
			//case application_default_shader_variable_type_enum::Float2:   uniform_source_code += "float2 "; break;
			case application_default_shader_variable_type_enum::Floatv2:  uniform_source_code += "vec2 ";   break;
			//case application_default_shader_variable_type_enum::Int2:     uniform_source_code += "int2 ";   break;
			case application_default_shader_variable_type_enum::Intv2:    uniform_source_code += "ivec2 ";  break;
			//case application_default_shader_variable_type_enum::UInt2:    uniform_source_code += "uint2 ";  break;
			case application_default_shader_variable_type_enum::UIntv2:   uniform_source_code += "uvec2 ";  break;
			//case application_default_shader_variable_type_enum::Float3:   uniform_source_code += "float3 "; break;
			case application_default_shader_variable_type_enum::Floatv3:  uniform_source_code += "vec3 ";   break;
			//case application_default_shader_variable_type_enum::Int3:     uniform_source_code += "int3 ";   break;
			case application_default_shader_variable_type_enum::Intv3:    uniform_source_code += "ivec3 ";  break;
			//case application_default_shader_variable_type_enum::UInt3:    uniform_source_code += "uint3 ";  break;
			case application_default_shader_variable_type_enum::UIntv3:   uniform_source_code += "uvec3 ";  break;
			//case application_default_shader_variable_type_enum::Float4:   uniform_source_code += "float4 "; break;
			case application_default_shader_variable_type_enum::Floatv4:  uniform_source_code += "vec4 ";   break;
			//case application_default_shader_variable_type_enum::Int4:     uniform_source_code += "int4 ";   break;
			case application_default_shader_variable_type_enum::Intv4:    uniform_source_code += "ivec4 ";  break;
			//case application_default_shader_variable_type_enum::UInt4:    uniform_source_code += "uint4 ";  break;
			case application_default_shader_variable_type_enum::UIntv4:   uniform_source_code += "uvec4 ";  break;
			case application_default_shader_variable_type_enum::Mat2fv:   uniform_source_code += "mat2 ";   break;
			case application_default_shader_variable_type_enum::Mat2x3fv: uniform_source_code += "mat2x3 "; break;
			case application_default_shader_variable_type_enum::Mat2x4fv: uniform_source_code += "mat2x4 "; break;
			case application_default_shader_variable_type_enum::Mat3fv:   uniform_source_code += "mat3 ";   break;
			case application_default_shader_variable_type_enum::Mat3x2fv: uniform_source_code += "mat3x2 "; break;
			case application_default_shader_variable_type_enum::Mat3x4fv: uniform_source_code += "mat3x4 "; break;
			case application_default_shader_variable_type_enum::Mat4fv:   uniform_source_code += "mat4 ";   break;
			case application_default_shader_variable_type_enum::Mat4x2fv: uniform_source_code += "mat4x2 "; break;
			case application_default_shader_variable_type_enum::Mat4x3fv: uniform_source_code += "mat4x3 "; break;
		}
		
		uniform_source_code += uniform.name + ";\n";

//printf("parse_application_uniforms 4444 : uniform_source_code \n  %s \n", uniform_source_code.c_str());
	}

/*
	for (int i = 0; i < shader_format.float_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform float "; uniform_source_code += shader_format.float_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.float2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform float2 "; uniform_source_code += shader_format.float2_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.float3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform float3 "; uniform_source_code += shader_format.float3_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.float4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform float4 "; uniform_source_code += shader_format.float4_uniforms.uniforms[i].name + ";\n"; }

	for (int i = 0; i < shader_format.int_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform int "; uniform_source_code += shader_format.int_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.int2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform int2 "; uniform_source_code += shader_format.int2_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.int3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform int3 "; uniform_source_code += shader_format.int3_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.int4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform int4 "; uniform_source_code += shader_format.int4_uniforms.uniforms[i].name + ";\n"; }

	for (int i = 0; i < shader_format.uint_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uint "; uniform_source_code += shader_format.uint_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.uint2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uint2 "; uniform_source_code += shader_format.uint2_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.uint3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uint3 "; uniform_source_code += shader_format.uint3_uniforms.uniforms[i].name + ";\n"; }
	//for (int i = 0; i < shader_format.uint4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uint4 "; uniform_source_code += shader_format.uint4_uniforms.uniforms[i].name + ";\n"; }

	for (int i = 0; i < shader_format.bool_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform bool "; uniform_source_code += shader_format.float_uniforms.uniforms[i].name + ";\n"; }
	
	for (int i = 0; i < shader_format.vec2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform vec2 "; uniform_source_code += shader_format.vec2_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.vec3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform vec3 "; uniform_source_code += shader_format.vec3_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.vec4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform vec4 "; uniform_source_code += shader_format.vec4_uniforms.uniforms[i].name + ";\n"; }

	for (int i = 0; i < shader_format.ivec2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform ivec2 "; uniform_source_code += shader_format.ivec2_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.ivec3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform ivec3 "; uniform_source_code += shader_format.ivec3_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.ivec4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform ivec4 "; uniform_source_code += shader_format.ivec4_uniforms.uniforms[i].name + ";\n"; }

	for (int i = 0; i < shader_format.uvec2_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uvec2 "; uniform_source_code += shader_format.uvec2_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.uvec3_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uvec3 "; uniform_source_code += shader_format.uvec3_uniforms.uniforms[i].name + ";\n"; }
	for (int i = 0; i < shader_format.uvec4_uniforms.uniforms.size(); i++) { uniform_source_code += "uniform uvec4 "; uniform_source_code += shader_format.uvec4_uniforms.uniforms[i].name + ";\n"; }
*/
	source_code = source_code_prev + uniform_source_code + source_code_after;

//printf("parse_application_uniforms 55555 : source_code  \n %s \n", source_code.c_str());

	return true;

}

// ####################################
// - returns directives string and remaining source separately, so that parseTopLevel can start after the directives we've added
// - lineNumberStart indicates what value a #line directive should have after parsing #include statements
void ShaderPreprocessor::parseDirectives( const std::string &source, const std::filesystem::path &sourcePath, std::string *directives, std::string *sourceBody, int *versionNumber, int *lineNumberStart )
{		
// printf("parseDirectives 000 %i \n %s\n", source.size(), source.c_str());
 printf("parseDirectives 000 \n");
	// go through each line and find the #version directive
	int lineNumber = 1;
	bool hasVersionLine = false;

	for( size_t lineStartPos = 0; lineStartPos < source.size(); /* */ ) {
		size_t lineEndPos = source.find( '\n', lineStartPos );
		if (lineEndPos == std::string::npos) {
 printf("parseDirectives lineEndPos == std::string::npos %i : \n %s \n.", source.size(), source.c_str());
			break;
		}
 printf("parseDirectives 111 \n");
		if( findVersionStatement( source.c_str() + lineStartPos, versionNumber ) ) {
			// if no defines, return leaving the directive and sourceBody strings empty,
			// thereby indicating to use the original source without modification;
			if( mDefineDirectives.empty() ) {
				*lineNumberStart = 1;
				return;
			}
 printf("parseDirectives 222 \n");
			// Copy #version line and everything before it to the directives string, the rest to sourceBody
			*directives = source.substr( 0, lineEndPos + 1 );
			*sourceBody = source.substr( lineEndPos + 1 );
			*lineNumberStart = lineNumber;
			hasVersionLine = true;

			break;
		}

		lineStartPos = lineEndPos + 1;
		lineNumber++;
	}
 printf("parseDirectives AAAA \n");
	// if we don't have a version yet, add a default one that will be at the top
	if( ! hasVersionLine ) {
#if defined( CINDER_GL_ES_3 )
		*directives += "#version " + std::to_string( mVersion ) + " es\n";
#else
		*directives += "#version " + std::to_string( mVersion ) + "\n";
#endif

		*sourceBody = source; // sourceBody is the entire source
		*lineNumberStart = 0;
		*versionNumber = mVersion;
	}
 printf("parseDirectives BBBB \n");
	// append any #defines to the directives string
	for( const auto &define : mDefineDirectives ) {
		*directives += "#define " + define.first;
		if( ! define.second.empty() )
			*directives += " " + define.second;

		*directives += "\n";
	}
 printf("parseDirectives CCCC \n");
	// if we've made any modifications, add a #line directive to ensure debug error statements are correct.
	if( ! mDefineDirectives.empty() || ! hasVersionLine ) {
		*directives += getLineDirective( sourcePath, *lineNumberStart, 0, *versionNumber );
		*lineNumberStart += 1;
	}
 printf("parseDirectives DDDD \n");
}

std::string ShaderPreprocessor::parseTopLevel( const std::string &source, const std::filesystem::path &sourcePath, int lineNumberStart, int versionNumber, std::set<std::filesystem::path> &includedFiles )
{
	std::istringstream input( source );
	return readStream( input, sourcePath, lineNumberStart, versionNumber, includedFiles );
}


std::string ShaderPreprocessor::parseRecursive( const std::filesystem::path &includePath, const std::filesystem::path &currentDirectory, int versionNumber, std::set<std::filesystem::path> &includeTree )
{	
	std::string output;
	std::string signalIncludeResult;
	const int lineNumberStart = 1;

	output = getLineDirective( includePath, 0, (int)includeTree.size() + 1, versionNumber );

	//if( mSignalInclude.emit( includePath, &signalIncludeResult ) ) {
	if(includePath.is_absolute()) { // need to check this is correct interpretation of what mSignalInclude.emit( includePath, &signalIncludeResult ) is doing

		if( includeTree.count( includePath ) ) {
			// circular include, skip it as it has already been appended.
			return "";
		}

		signalIncludeResult = includePath.string();// need to check this is correct interpretation of what mSignalInclude.emit( includePath, &signalIncludeResult ) is doing

		includeTree.insert( includePath );
		std::istringstream input( signalIncludeResult );
		output += readStream( input, includePath, lineNumberStart, versionNumber, includeTree );
	}
	else {
		const std::filesystem::path fullPath = findFullPath( includePath, currentDirectory );
		if( fullPath.empty() ) {
			//throw ShaderPreprocessorExc( "could not find shader with include path: " + includePath.string() );
			//CI_LOG_E("ERROR !!!!! : Could not find shader with include path: " + includePath.string());   // ***** Mod
			mparse_successfull_m = false;												// ***** Mod
			return "ERROR !!!!! :Could not find shader with include path: " + includePath.string();	// ***** Mod
		}

		if( includeTree.count( fullPath ) ) {
			// circular include, skip it as it has already been appended.
			return "";
		}

		includeTree.insert( fullPath );

		std::ifstream input( fullPath.string().c_str() );
		if (!input.is_open()) {
			//throw ShaderPreprocessorExc( "Failed to open file at include path: " + fullPath.string() );
			//CI_LOG_E("ERROR !!!!! :Failed to open file at include path: " + fullPath.string());   // ***** Mod
			mparse_successfull_m = false;											// ***** Mod
			return "ERROR !!!!! :Failed to open file at include path: " + fullPath.string();		// ***** Mod
		}

		//try {
			output += readStream( input, fullPath, lineNumberStart, versionNumber, includeTree );
		//}
		//catch( ShaderPreprocessorExc &exc ) {
		//	// append currently processed glsl file.
		//	//throw ShaderPreprocessorExc( string( exc.what() ) + ", while parsing file: " + fullPath.string() );
		//	//CI_LOG_E(string(exc.what()) + ",ERROR !!!!! : while parsing file: " + fullPath.string()); // ***** Mod
		//	mparse_successfull_m = false;												// ***** Mod
		//	return std::string(exc.what()) + ",ERROR !!!!! : while parsing file: " + fullPath.string();	// ***** Mod
		//}

		if (output.empty()) {
			return std::string(",ERROR !!!!! : while parsing file: " + fullPath.string());	// ***** Mod
		}

		input.close();
	}

	mparse_successfull_m = true; // ***** Mod

	return output;
}

std::string ShaderPreprocessor::readStream( std::istream &input, const std::filesystem::path &sourcePath, int lineNumberStart, int versionNumber, std::set<std::filesystem::path> &includeTree )
{
	// go through each line and process includes
	std::string line;
	int lineNumber = lineNumberStart;
	std::stringstream output;

	while( getline( input, line ) ) {
		std::string includeFilePath;
		if( findIncludeStatement( line, &includeFilePath ) ) {
			int numIncludesBefore = (int)includeTree.size();
			output << parseRecursive( includeFilePath, sourcePath.parent_path(), versionNumber, includeTree );
			output << getLineDirective( sourcePath, lineNumber, numIncludesBefore, versionNumber );
		}
		else
			output << line << "\n";

		lineNumber++;
	}

	return output.str();
}

std::string ShaderPreprocessor::getLineDirective( const std::filesystem::path &sourcePath, int lineNumber, int sourceStringNumber, int versionNumber ) const
{
	// in glsl 330 and up, the #line directive indicates what the next line should be. Before that, it is the current line.
	if( versionNumber >= 330 )
		lineNumber += 1;

	std::string fileId = mUseFilenameInLineDirective ? "\"" + sourcePath.filename().string() + "\"" : std::to_string( sourceStringNumber );
	return "#line " + std::to_string( lineNumber ) + " " + fileId + "\n";
}

void ShaderPreprocessor::addSearchDirectory( const std::filesystem::path &directory )
{
	if( ! std::filesystem::is_directory( directory ) ) {
		//CI_LOG_E( "Not a directory: " << directory );
		return;
	}

	std::filesystem::path dirCanonical = std::filesystem::canonical( directory );
	auto it = find( mSearchDirectories.begin(), mSearchDirectories.end(), dirCanonical );
	if( it == mSearchDirectories.end() )
		mSearchDirectories.push_back( dirCanonical );
}

void ShaderPreprocessor::removeSearchDirectory( const std::filesystem::path &directory )
{
	std::filesystem::path dirCanonical = std::filesystem::canonical( directory );
	mSearchDirectories.erase( remove( mSearchDirectories.begin(), mSearchDirectories.end(), dirCanonical ), mSearchDirectories.end() );
}


void ShaderPreprocessor::addDefine( const std::string &define )
{
	// Check if there are any existing defines with this symbol name, and if so replace it.
	std::string defineUntilSpace = define.substr( 0, define.find( ' ' ) );
	for( auto &dd : mDefineDirectives ) {

		if( dd.first == define ) {
			dd.first = define;
			dd.second = {};
			return;
		}
	}

	// define is unique, add it to list
	mDefineDirectives.push_back( { define, {} } );
}

void ShaderPreprocessor::addDefine( const std::string &define, const std::string &value )
{
//	string defineLine = define + " " + value;

	// Check if there are any existing defines with this symbol name, and if so replace it.
	for( auto &dd : mDefineDirectives ) {
		if( dd.first == define ) {
			dd.first = define;
			dd.second = value;
			return;
		}
	}

	// define is unique, add it to list
	mDefineDirectives.push_back( { define, value } );
}

void ShaderPreprocessor::setDefines( const std::vector<std::pair<std::string, std::string>> &defines )
{
	mDefineDirectives = defines;
}

void ShaderPreprocessor::removeDefine( const std::string &define )
{
	mDefineDirectives.erase( remove_if( mDefineDirectives.begin(), mDefineDirectives.end(),
		[&define]( const std::pair<std::string, std::string> &o ) {
			return o.first == define;
		} ), mDefineDirectives.end() );
}

void ShaderPreprocessor::clearDefines()
{
	mDefineDirectives.clear();
}

std::filesystem::path ShaderPreprocessor::findFullPath( const std::filesystem::path &includePath, const std::filesystem::path &currentDirectory )
{
	auto fullPath = currentDirectory / includePath;
	if( std::filesystem::exists( fullPath ) )
		return std::filesystem::canonical( fullPath );

	for( auto dirIt = mSearchDirectories.rbegin(); dirIt != mSearchDirectories.rend(); ++dirIt ) {
		fullPath = *dirIt / includePath;
		if( std::filesystem::exists( fullPath ) )
			return std::filesystem::canonical( fullPath );
	}

	return {};
}


