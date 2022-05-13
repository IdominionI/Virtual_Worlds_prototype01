#pragma once

#include "../../Common/pch.h"
#include "../../Shader/shader.h"

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


class light_object_class //: public object_basis_class
{
public:

    light_object_class()
    {
        mColor = glm::vec3(1.0f, 1.0f, 1.0f);
        mPosition = { 1.5f, 3.5f, 3.0f };
        mStrength = 100.0f;
    }

    ~light_object_class() {}

   // void update(shader_class* shader){
   //     shader->set_vec3(mPosition, "lightPosition");
   //     shader->set_vec3(mColor * mStrength, "lightColor");
   // }

    void update(int shader_program_id) {
        shader.set_vec3(shader_program_id,mPosition, "lightPosition");
        shader.set_vec3(shader_program_id,mColor * mStrength, "lightColor");



    }

    glm::vec3 mPosition;

    glm::vec3 mColor;

    float mStrength;
private:
    shader_class shader;
};
