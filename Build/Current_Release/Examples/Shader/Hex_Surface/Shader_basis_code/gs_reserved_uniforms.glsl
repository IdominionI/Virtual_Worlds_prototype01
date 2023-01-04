layout(points) in;
//layout(triangle_strip, max_vertices = 48) out; // The output needs to be defined by the user in the function definitions

flat in int gEnabledFaces[];// Must have as flat array otherwise will not work

flat in float hexSize[];

#include "universal_reserved_uniforms.glsl"

// hex surface parameter data
uniform vec2  grid_origin;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following input/output block is to allow  varyingColor 
// to be passed to the fragment shader or without changing the name

in VertexData
{
 float value;
 vec4  varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

out VertexData
{
  float value;
  vec4 varyingColor;
} gs_out;

