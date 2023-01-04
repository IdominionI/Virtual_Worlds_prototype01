
uniform mat4 mvpMatrix; //model-view-projection matrix

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;


// Camera data
uniform vec3  camera_loc;         // Camera location

uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;

// reserved light variables
uniform vec4  light_color;
uniform vec3  lighting_direction;
uniform float ambience;
uniform float specular_strength;
uniform float lighting_intensity;

// reserved camera light variables
uniform int   camera_lighting_type;
uniform int   use_camera_lighting;
uniform vec3  lighting_camera_offset;

uniform int   t_frame;

// **************************************
struct Material {
    float diffuse;
    float specular;
    float shininess;
};
// Light Data_structures

struct DirLight {
    vec3 direction;
	vec3 color;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
	vec3 color;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
	vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define MAX_NUMBER_DIRECTIONAL_LIGHTS 10
#define MAX_NUMBER_POINT_LIGHTS       10
#define MAX_NUMBER_SPOT_LIGHTS        10

uniform int number_directional_lights;
uniform int number_point_lights;
uniform int number_spot_lights;

uniform vec3 viewPos;

#define TRUE  0
#define FALSE 1



