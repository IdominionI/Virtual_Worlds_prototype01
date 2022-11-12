#pragma once
#include "../../Common/pch.h"
#include "../../Shader/shader.h"

#include "../../Geometry/lines.h"

class bounding_volume_class  {
public :
    line_class           *geometry = NULL;
    material_struct_type *material = NULL; // *****

    glm::vec3 min_corner = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 max_corner = glm::vec3(1.0, 1.0, 1.0);

    GLuint program_id = -1;

    shader_class shader;


    bool initialise() {
printf("bounding_volume_class 000\n");
       
        if ((program_id = shader.create_shader_program(v_shader_filename,"", f_shader_filename)) == 0) 
            return false;
        else
printf("bounding_volume_class 111\n");
          return true;
    }

    bool update_limits(glm::vec3 min_corner, glm::vec3 max_corner) {
//printf("bounding_volume_class:: draw_bbox 000\n");
        if (min_corner == max_corner) {
            //error message
            return false;
        }
//printf("bounding_volume_class:: draw_bbox 111\n");
        if (max_corner.x < min_corner.x) {
            float min = max_corner.x;
            max_corner.x = min_corner.x;
            min_corner.x = min;
        }

        if (max_corner.y < min_corner.y) {
            float min = max_corner.y;
            max_corner.y = min_corner.y;
            min_corner.y = min;
        }

        if (max_corner.z < min_corner.z) {
            float min = max_corner.z;
            max_corner.z = min_corner.z;
            min_corner.z = min;
        }

        geometry->vertices.clear();

        // Bottom plane
        geometry->add_vertex(min_corner);
        geometry->add_vertex(glm::vec3(max_corner.x,min_corner.y,min_corner.z));

        geometry->add_vertex(glm::vec3(max_corner.x, min_corner.y, min_corner.z));
        geometry->add_vertex(glm::vec3(max_corner.x, max_corner.y, min_corner.z));
        
        geometry->add_vertex(glm::vec3(max_corner.x, max_corner.y, min_corner.z));
        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, min_corner.z));
       
        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, min_corner.z));
        geometry->add_vertex(min_corner);
        // Top plane
        geometry->add_vertex(glm::vec3(min_corner.x, min_corner.y, max_corner.z));
        geometry->add_vertex(glm::vec3(max_corner.x, min_corner.y, max_corner.z));

        geometry->add_vertex(glm::vec3(max_corner.x, min_corner.y, max_corner.z));
        geometry->add_vertex(max_corner);

        geometry->add_vertex(max_corner);
        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, max_corner.z));

        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, max_corner.z));
        geometry->add_vertex(glm::vec3(min_corner.x, min_corner.y, max_corner.z));
        // Sides
        geometry->add_vertex(min_corner);
        geometry->add_vertex(glm::vec3(min_corner.x, min_corner.y, max_corner.z));

        geometry->add_vertex(glm::vec3(max_corner.x, min_corner.y, min_corner.z));
        geometry->add_vertex(glm::vec3(max_corner.x, min_corner.y, max_corner.z));

        geometry->add_vertex(glm::vec3(max_corner.x, max_corner.y, min_corner.z));
        geometry->add_vertex(glm::vec3(max_corner.x, max_corner.y, max_corner.z));

        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, min_corner.z));
        geometry->add_vertex(glm::vec3(min_corner.x, max_corner.y, max_corner.z));

        geometry->update_geometry();

        return true;
    }

private:
    // these need to change to more appropriate default names and location !!!!!!
    //std::string v_shader_filename = "phong-shading.v.glsl";
    //std::string f_shader_filename = "phong-shading.f.glsl";

    std::string v_shader_filename = "Assets/Shaders/Default/VW_application/bounding_box_VS.glsl";
    std::string f_shader_filename = "Assets/Shaders/Default/VW_application/bounding_box_FS.glsl";
};