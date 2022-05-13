#pragma once

/*
	Import-Export ASCII data block ID to identify the 
	start and end of various data blocks or other data
	import - export requirements
*/

//#define endl "\n"
//#define ENDL "\n"

#define SCENE_BLOCK_START "|-"
#define SCENE_BLOCK_END   "-|"

#define OBJECT_GROUP_BLOCK_START "<<"
#define OBJECT_GROUP_BLOCK_END   ">>"

#define OBJECT_BLOCK_START "[["
#define OBJECT_BLOCK_END   "]]"

#define DATA_BLOCK_START "{{"
#define DATA_BLOCK_END   "}}"

#define FLOAT_VARIABLE_BLOCK_START "<"
#define FLOAT_VARIABLE_BLOCK_END   ">"

#define INT_VARIABLE_BLOCK_START "{"
#define INT_VARIABLE_BLOCK_END   "}"

#define BOOL_VARIABLE_BLOCK_START "("
#define BOOL_VARIABLE_BLOCK_END   ")"

#define TEXTURE_INT_BLOCK_START "{"
#define TEXTURE_INT_BLOCK_END   "}"

#define TEXTURE_BOOL_BLOCK_START "("
#define TEXTURE_BOOL_BLOCK_END   ")"

#define AUTOMATA_RULE_START "("
#define AUTOMATA_RULE_END   ")"

#define AUTOMATA_RULE_DELIMINATOR ":"

#define SELECTED_EXPORT 0
#define ACTIVE_EXPORT   1
#define ALL_EXPORT      2