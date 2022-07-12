#pragma once

#include "../../Common/pch.h"
#include "../../Shader/shader.h"

#include <Universal/Framework/FWstring_tools.h>

//#include <Universal/Framework/FWid_key_manager.h>
/*
                 Light Object Class

    A Scene object class to define a light to be used to render
    an OpenGL scene

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    As Yet this scene object class is not used, but is retained
    for expectation of future utilisation.
*/

// NOTE : CRITCAL The max number of lights defined below nust be matched
//                by the same max number of lights defined in the shader
#define MAX_NUMBER_DIRECTIONAL_LIGHTS 10
#define MAX_NUMBER_POINT_LIGHTS       10
#define MAX_NUMBER_SPOT_LIGHTS        10

#define SCENE_LIGHT_ERROR_NUMBER_PERMITTED_LIGHTS_EXCEEDED -1
#define SCENE_LIGHT_ERROR_NUMBER_LIGHT_CREATED             0

enum class light_type_enum{none,directional, point, spotlight};

class light_basis_class {
public:
   // light_type_enum light_type;
    int       light_id = -1;

    glm::vec3 mPosition;
    glm::vec3 mColor;

    float mStrength;

    float ambience;
    float diffuse;
    float specular;

    light_basis_class() {}

    virtual void update(int shader_program_id) {
    }

protected:
    shader_class shader;

};


class directional_light_class : public light_basis_class {
public:
    glm::vec3 direction;

    directional_light_class() {
       // light_type = light_type_enum::directional;
        mColor = glm::vec3(1.0f, 1.0f, 1.0f);
        mStrength = 100.0f;
    }

    void update(int shader_program_id, int index) {
        shader.set_vec3(shader_program_id, direction,          "dirLight["+FW::stringtools::toString(index)+"].direction");
        shader.set_vec3(shader_program_id, mColor * mStrength, "dirLight[" + FW::stringtools::toString(index) + "].color");

        shader.set_vec3(shader_program_id, glm::vec3(ambience, ambience, ambience), "dirLight[" + FW::stringtools::toString(index) + "].ambient");
        shader.set_vec3(shader_program_id, glm::vec3(diffuse, diffuse, diffuse),    "dirLight[" + FW::stringtools::toString(index) + "].diffuse");
        shader.set_vec3(shader_program_id, glm::vec3(specular, specular, specular), "dirLight[" + FW::stringtools::toString(index) + "].specular");
    }

};

class point_light_class : public light_basis_class {
public:
    float constant;
    float linear;
    float quadratic;
    float attenuation_factor;

    point_light_class() {
       // light_type = light_type_enum::point;
        mColor = glm::vec3(1.0f, 1.0f, 1.0f);
        mStrength = 100.0f;
    }

    void update(int shader_program_id, int index) {
        if (light_id >= MAX_NUMBER_POINT_LIGHTS) return;

        shader.set_vec3(shader_program_id, mPosition,          "pointLights["+FW::stringtools::toString(index)+"].position");
        shader.set_vec3(shader_program_id, mColor * mStrength, "pointLights["+FW::stringtools::toString(index)+"].color");
        shader.set_vec3(shader_program_id, glm::vec3(ambience, ambience, ambience), "pointLights["+FW::stringtools::toString(index)+"].ambient");
        shader.set_vec3(shader_program_id, glm::vec3(diffuse, diffuse, diffuse),    "pointLights["+FW::stringtools::toString(index)+"].diffuse");
        shader.set_vec3(shader_program_id, glm::vec3(specular, specular, specular), "pointLights["+FW::stringtools::toString(index)+"].specular");

        shader.set_f1(shader_program_id, constant, "pointLights["+FW::stringtools::toString(index)+"].constant");
        shader.set_f1(shader_program_id, linear,   "pointLights["+FW::stringtools::toString(index)+"].linear");
        shader.set_f1(shader_program_id, quadratic,"pointLights["+FW::stringtools::toString(index)+"].quadratic");
        shader.set_f1(shader_program_id, attenuation_factor,"pointLights["+FW::stringtools::toString(index)+"].attenuation_factor");
    }

};

class spot_light_class : public light_basis_class {
public:
    glm::vec3 direction;

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
    float attenuation_factor;

    spot_light_class() {
        // light_type = light_type_enum::point;
        mColor = glm::vec3(1.0f, 1.0f, 1.0f);
        mStrength = 100.0f;
    }

    void update(int shader_program_id, int index) {
        if (light_id >= MAX_NUMBER_POINT_LIGHTS) return;

        shader.set_vec3(shader_program_id, mPosition, "spotLight["+FW::stringtools::toString(index)+"].position");
        shader.set_vec3(shader_program_id, direction, "spotLight["+FW::stringtools::toString(index)+"].direction");
        shader.set_vec3(shader_program_id, mColor * mStrength, "spotLight["+FW::stringtools::toString(index)+"].color");
        shader.set_vec3(shader_program_id, glm::vec3(ambience, ambience, ambience), "spotLight["+FW::stringtools::toString(index)+"].ambient");
        shader.set_vec3(shader_program_id, glm::vec3(diffuse, diffuse, diffuse),    "spotLight["+FW::stringtools::toString(index)+"].diffuse");
        shader.set_vec3(shader_program_id, glm::vec3(specular, specular, specular), "spotLight["+FW::stringtools::toString(index)+"].specular");

        shader.set_f1(shader_program_id, constant,   "spotLight["+FW::stringtools::toString(index)+"].constant");
        shader.set_f1(shader_program_id, linear,     "spotLight["+FW::stringtools::toString(index)+"].specular");
        shader.set_f1(shader_program_id, quadratic,  "spotLight["+FW::stringtools::toString(index)+"].quadratic");
        shader.set_f1(shader_program_id, cutOff,     "spotLight["+FW::stringtools::toString(index)+"].cutOff");
        shader.set_f1(shader_program_id, outerCutOff,"spotLight["+FW::stringtools::toString(index)+"].outerCutOff");
        shader.set_f1(shader_program_id, attenuation_factor, "spotLight[" + FW::stringtools::toString(index) + "].attenuation_factor");
    }


};

class scene_lights_objects_class
{
public:
    std::vector<directional_light_class> directional_lights;
    std::vector <point_light_class>      point_lights;
    std::vector <spot_light_class>       spot_lights;

    //*****
    glm::vec3 mPosition;
    glm::vec3 mColor;

    float mStrength;
    //****

    scene_lights_objects_class(){
        add_new_light(light_type_enum::directional, glm::vec3(0.0f,0.0f,10000.0f));
        add_new_light(light_type_enum::directional, glm::vec3(0.0f,0.0f,-10000.0f));
        // Testing ownly
        //*****

        mColor = glm::vec3(1.0f, 1.0f, 1.0f);
        mPosition = { 1.5f, 3.5f, 3.0f };
        mStrength = 100.0f;

        directional_lights[0].direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        directional_lights[0].ambience  = 0.05f;
        directional_lights[0].diffuse   = 0.4f;
        directional_lights[0].specular  = 0.5f;

        directional_lights[1].direction = glm::vec3(0.2f, 1.0f, 0.3f);
        directional_lights[1].ambience = 0.05f;
        directional_lights[1].diffuse  = 0.4f;
        directional_lights[1].specular = 0.5f;
/*
        // point light 1
        add_new_light(light_type_enum::point, glm::vec3(0.7f, 0.2f, 12.0f));
        point_lights[0].ambience = 0.05f;
        point_lights[0].diffuse  = 0.8f;
        point_lights[0].specular = 1.0f;
        point_lights[0].constant = 1.0f;
        point_lights[0].linear   = 0.09f;
        point_lights[0].quadratic=0.032f;
        point_lights[0].attenuation_factor =100.0f;
        // point light 2
        add_new_light(light_type_enum::point, glm::vec3(2.3f, -3.3f, -14.0f));
        point_lights[1].ambience  = 0.05f;
        point_lights[1].diffuse   = 0.8f;
        point_lights[1].specular  = 1.0f;
        point_lights[1].constant  = 1.0f;
        point_lights[1].linear    = 0.09f;
        point_lights[1].quadratic = 0.032f;
        point_lights[1].attenuation_factor =100.0f;
        // point light 3
        add_new_light(light_type_enum::point, glm::vec3(-4.0f, 2.0f, -12.0f));
        point_lights[2].ambience = 0.05f;
        point_lights[2].diffuse  = 0.8f;
        point_lights[2].specular = 1.0f;
        point_lights[2].constant = 1.0f;
        point_lights[2].linear   = 0.09f;
        point_lights[2].quadratic=0.032f;
        point_lights[2].attenuation_factor = 100.0f;
        // point light 4
        add_new_light(light_type_enum::point, glm::vec3(0.0f, 0.0f, -13.0f));
        point_lights[3].ambience  = 0.05f;
        point_lights[3].diffuse   = 0.8f;
        point_lights[3].specular  = 1.0f;
        point_lights[3].constant  = 1.0f;
        point_lights[3].linear    = 0.09f;
        point_lights[3].quadratic = 0.032f;
        point_lights[3].attenuation_factor = 100.0f;
        // spotLight
        add_new_light(light_type_enum::spotlight, glm::vec3(0.0f, 0.0f, 13.0f));
        spot_lights[0].direction =  glm::vec3(0.0f, 0.0f,0-1.0f);

        spot_lights[0].ambience   = 0.00f;
        spot_lights[0].diffuse   = 1.0f;
        spot_lights[0].specular  = 1.0f;
        spot_lights[0].constant  = 1.0f;
        spot_lights[0].linear    = 0.09f;
        spot_lights[0].quadratic = 0.032f;
        spot_lights[0].attenuation_factor = 100.0f;

        spot_lights[0].cutOff      = glm::cos(glm::radians(12.5f));
        spot_lights[0].outerCutOff = glm::cos(glm::radians(15.0f));
        //*****
*/
    }

    ~scene_lights_objects_class() {
        clear_directional_lights();
        clear_point_lights();
        clear_spot_lights();
    }

    void update(int shader_program_id) {
        //Testing only
        // Material of object to be defined for object
        shader.set_f1(shader_program_id, 0.5f,  "material.diffuse");
        shader.set_f1(shader_program_id, 0.75f, "material.specular");
        shader.set_f1(shader_program_id, 0.5f,  "material.shininess");

        shader.set_vec3(shader_program_id, mPosition, "lightPosition");
        shader.set_vec3(shader_program_id, mColor * mStrength, "lightColor");

        //end Testing only

        shader.set_i1(shader_program_id, directional_lights.size(), "number_directional_lights");
        shader.set_i1(shader_program_id, point_lights.size(),       "number_point_lights");
        shader.set_i1(shader_program_id, spot_lights.size(),        "number_spot_lights");

        for (int i = 0; i < directional_lights.size();i++) {
            directional_lights[i].update(shader_program_id,i);
        }

        for (int i = 0; i < point_lights.size(); i++) {
            point_lights[i].update(shader_program_id,i);
        }

        for (int i = 0; i < spot_lights.size(); i++) {
            spot_lights[i].update(shader_program_id,i);
        }

    }

    int add_new_light(light_type_enum light_type,glm::vec3 location = glm::vec3(0.0f,0.0f,0.0f)) {
        switch (light_type) {
            case light_type_enum::directional : return add_new_directional_light(location);break;
            case light_type_enum::point       : return add_new_point_light(location);break;
            case light_type_enum::spotlight   : return add_new_spot_light(location);break;
        }
        return 1;
    }

    bool delete_light(light_type_enum light_type, int light_id) {
         switch (light_type) {
            case light_type_enum::directional : return delete_directional_light(light_id);break;
            case light_type_enum::point       : return delete_point_light(light_id);break;
            case light_type_enum::spotlight   : return delete_spot_light(light_id);break;
        }
        return 1;
    }

    void clear_directional_lights() {
        if (directional_lights.size() > 1) {
            directional_lights.erase(directional_lights.begin() + 1, directional_lights.end());
            directional_lights.shrink_to_fit();
        }
    }

    void clear_point_lights() { 
        point_lights.clear();
        point_lights.shrink_to_fit();
    }

    void clear_spot_lights() {
        spot_lights.clear();
        spot_lights.shrink_to_fit();
    }


private:
    shader_class shader;

    int add_new_directional_light(glm::vec3 location) {
        if (directional_lights.size() >= MAX_NUMBER_DIRECTIONAL_LIGHTS)
            return SCENE_LIGHT_ERROR_NUMBER_PERMITTED_LIGHTS_EXCEEDED;

        directional_light_class directional_light;
        directional_light.mPosition = location;

        directional_lights.push_back(directional_light);

        return SCENE_LIGHT_ERROR_NUMBER_LIGHT_CREATED;
    }

    bool add_new_point_light(glm::vec3 location) {
        if (point_lights.size() >= MAX_NUMBER_POINT_LIGHTS)
            return SCENE_LIGHT_ERROR_NUMBER_PERMITTED_LIGHTS_EXCEEDED;

        point_light_class point_light;
        point_light.mPosition = location;

        point_lights.push_back(point_light);

        return SCENE_LIGHT_ERROR_NUMBER_LIGHT_CREATED;
    }

    bool add_new_spot_light(glm::vec3 location) {
        if (spot_lights.size() >= MAX_NUMBER_SPOT_LIGHTS)
            return SCENE_LIGHT_ERROR_NUMBER_PERMITTED_LIGHTS_EXCEEDED;

        spot_light_class spotlight;
        spotlight.mPosition = location;

        spot_lights.push_back(spotlight);

        return SCENE_LIGHT_ERROR_NUMBER_LIGHT_CREATED;
    }

    int delete_directional_light(int light_id) {
        for (int i = 0; i = directional_lights.size(); i++) {
            if (directional_lights[i].light_id == light_id) {
                directional_lights.erase(directional_lights.begin() + i);
                return true;
            }
        }
        return false;
    }

    int delete_point_light(int light_id) {
        for (int i = 0; i = point_lights.size(); i++) {
            if (point_lights[i].light_id == light_id) {
                point_lights.erase(point_lights.begin() + i);
                return true;
            }
        }
        return false;
    }

    int delete_spot_light(int light_id) {
        for (int i = 0; i = spot_lights.size(); i++) {
            if (spot_lights[i].light_id == light_id) {
                spot_lights.erase(spot_lights.begin() + i);
                return true;
            }
        }
        return false;
    }

};

