#pragma once
#include <Source/Editor/Common/global.h>

#include "../hex_surface_object/hex_surface_object.h"
#include "../Render/hex_surface_render.h"
#include "../Hex_surface_object/hex_surface_parameters.h"

class hex_surface_interactions_class {
public:

    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel     = NULL;

    static bool voxel_intersection(hex_surface_object_class *hex_surface_object_A, hex_surface_object_class *hex_surface_object_B) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = hex_surface_object_A->hex_surface_object_data.grid_origin.x;
        float a_xmax = hex_surface_object_A->hex_surface_object_data.grid_origin.x + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_start);
        float b_xmin = hex_surface_object_B->hex_surface_object_data.grid_origin.x;
        float b_xmax = hex_surface_object_B->hex_surface_object_data.grid_origin.x + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_start);

        float a_ymin = hex_surface_object_A->hex_surface_object_data.grid_origin.y;
        float a_ymax = hex_surface_object_A->hex_surface_object_data.grid_origin.y + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_start);
        float b_ymin = hex_surface_object_B->hex_surface_object_data.grid_origin.y;
        float b_ymax = hex_surface_object_B->hex_surface_object_data.grid_origin.y + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_start);


        if (((a_xmin >= b_xmin) && (a_xmin <= b_xmax)) || ((a_xmax <= b_xmax) && (a_xmax >= b_xmin))) x_intersection = true;
        if (((b_xmin >= a_xmin) && (b_xmin <= a_xmax)) || ((b_xmax <= a_xmax) && (b_xmax >= a_xmin))) x_intersection = true;
        if (((a_ymin >= b_ymin) && (a_ymin <= b_ymax)) || ((a_ymax <= b_ymax) && (a_ymax >= b_ymin))) y_intersection = true;
        if (((b_ymin >= a_ymin) && (b_ymin <= a_ymax)) || ((b_ymax <= a_ymax) && (b_ymax >= a_ymin))) y_intersection = true;

        if (x_intersection && y_intersection)
            return true;
        else
            return false;

    }

    static bool define_intersection_boundary(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, glm::vec3 &boundary_min, glm::vec3 &boundary_max) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = hex_surface_object_A->hex_surface_object_data.grid_origin.x;
        float a_xmax = hex_surface_object_A->hex_surface_object_data.grid_origin.x + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_start);
        float b_xmin = hex_surface_object_B->hex_surface_object_data.grid_origin.x;
        float b_xmax = hex_surface_object_B->hex_surface_object_data.grid_origin.x + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_start);

        float a_ymin = hex_surface_object_A->hex_surface_object_data.grid_origin.y;
        float a_ymax = hex_surface_object_A->hex_surface_object_data.grid_origin.y + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_start);
        float b_ymin = hex_surface_object_B->hex_surface_object_data.grid_origin.y;
        float b_ymax = hex_surface_object_B->hex_surface_object_data.grid_origin.y + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_start);

        if ((a_xmin >= b_xmin) && (a_xmin <= b_xmax)) {
            boundary_min.x = a_xmin;
            x_intersection = true;
        } else
            if ((b_xmin >= a_xmin) && (b_xmin <= a_xmax)) {
                boundary_min.x = b_xmin;
                x_intersection = true;
            }

        if ((a_ymin >= b_ymin) && (a_ymin <= b_ymax)){
            boundary_min.y = a_ymin;
            y_intersection = true;
        } else
            if ((b_ymin >= a_ymin) && (b_ymin <= a_ymax)) {
                boundary_min.y = b_ymin;
                y_intersection = true;
            }

        if ((a_xmax <= b_xmax) && (a_xmax >= b_xmin)){
            boundary_max.x = a_xmax;
            x_intersection = true;
        } else
            if ((b_xmax <= a_xmax) && (b_xmax >= a_xmin)) {
                boundary_max.x = b_xmax;
                x_intersection = true;
            }

        if ((a_ymax <= b_ymax) && (a_ymax >= b_ymin)){
            boundary_max.y = a_ymax;
            y_intersection = true;
        } else
            if ((b_ymax <= a_ymax) && (b_ymax >= a_ymin)) {
                boundary_max.y = b_ymax;
                z_intersection = true;
            }

        if (x_intersection && y_intersection)
            return true;
        else
            return false;
    }


   bool align_voxel_matricies(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, bool update_hcp_object_A_vertices = true) {
//printf("hcp_interaction_node_class:align_voxel_matricies 0000 %\n");
        if (hex_surface_object_A->hex_surface_object_data.hex_size != hex_surface_object_B->hex_surface_object_data.hex_size) return false;

        float hex_size = hex_surface_object_A->hex_surface_object_data.hex_size;

        glm::vec2 a_origin = hex_surface_object_A->hex_surface_object_data.grid_origin;
        glm::vec2 b_origin = hex_surface_object_B->hex_surface_object_data.grid_origin;

        glm::vec2 origin_diff = b_origin - a_origin;

        glm::vec2 translation;

        translation.x = fmod(origin_diff.x, hex_size);
        translation.y = fmod(origin_diff.y, hex_size);

        hex_surface_object_A->hex_surface_object_data.grid_origin += translation * hex_size;

        if (update_hcp_object_A_vertices) update_voxel_verticies(hex_surface_object_A);

        return true;
    }

        // !!!!!!!! FOLLOWING UPDATE CODE NEEDS TO BE PUT INTO A GENERAL UPDATE VOXEL VERTICES FUNCTION !!!!!!!!!!!!!!
    void update_voxel_verticies(hex_surface_object_class *hex_surface_object) {
//printf("hcp_voxel_interactions_class:update_voxel_verticies 0000\n");        
        hex_surface_object->define_vbo_vertices(MIN_HEX_VALUE, MAX_HEX_VALUE);

        //####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
        scene_node_class <render_object_class>* scene_hex_surface_object = scene_manager->get_render_object(hex_surface_object->object_id);
//printf("hcp_voxel_interactions_class:update_voxel_verticies 11111: %i : %i \n", scene_voxel_object->scene_node_entity_id, voxel_hcp_object->object_id);

        if (scene_hex_surface_object != NULL) {
            if (!hex_surface_render.update_geometry_vertex_cloud_data(&hex_surface_object->point_cloud, scene_hex_surface_object, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated.\n");
//printf("hcp_voxel_interactions_class action : scene_voxel_object not updated\n");
                return;
            }
//printf("hcp_voxel_interactions_class:update_voxel_verticies 22222 \n");
 //           application_default_shader_uniform_variables_struct_type uniform_variable;
 //           uniform_variable.type = application_default_shader_variable_type_enum::Floatv3; uniform_variable.name = "voxel_origin"; uniform_variable.value0 = &voxel_hcp_object->hex_surface_object_data.grid_origin;
  //          scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
        }

//if (scene_voxel_object == NULL) {
//printf("hcp_interaction_node_class:update_voxel_verticies 33333 scene_voxel_object == NULL \n");       
//}
    }

    hex_surface_object_class *create_interaction_object(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B,int &entity_id) {
        glm::vec3 boundary_min, boundary_max;
       
        if (scene_manager == NULL) return NULL;

//printf("hcp_interaction_node_class:create_interaction_object 0000 %\n");
        if (!align_voxel_matricies(hex_surface_object_A, hex_surface_object_B), false) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 11111  %\n");
        if (!define_intersection_boundary(hex_surface_object_A, hex_surface_object_B, boundary_min, boundary_max)) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 2222 min : %f : %f \n", boundary_min.x, boundary_min.y);
//printf("hcp_interaction_node_class:create_interaction_object 3333 max : %f : %f\n", boundary_max.x, boundary_max.y);

        if (entity_id == INVALID_ID) {
            entity_id = globalc::get_available_entity_id();
            if (!scene_manager->add_entity(entity_id, ENTITY_CATEGORY_HEX_SURF)) {
                entity_id = INVALID_ID;
                return NULL;
            }
        }

        //hex_surface_object_class *interaction_object = scene_manager->entities_manager.get_hex_surface_entity_object(entity_id);
        hex_surface_object_class *interaction_object = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(entity_id, ENTITY_CATEGORY_HEX_SURF);
        if (interaction_object == NULL) return NULL;

        interaction_object->hex_surface_object_data.grid_origin = boundary_min;
        interaction_object->hex_surface_object_data.hex_size = hex_surface_object_A->hex_surface_object_data.hex_size;

        // ########### CREATE EMPTY HEX SURFACE CLOUD MATRIX #################
        float x_size = boundary_max.x - boundary_min.x;
        float y_size = boundary_max.y - boundary_min.y;

        float x_res_step = interaction_object->hex_surface_object_data.hex_size * 2.0f;
        float y_res_step = interaction_object->hex_surface_object_data.hex_size * (3.0f / sqrt(3.0f));

        int data_set_x_size, data_set_y_size;

        if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0f)
            data_set_x_size = (int)(x_size / x_res_step) + 1;
        else
            data_set_x_size = (int)(x_size / x_res_step);

        if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0f)
            data_set_y_size = (int)(y_size / y_res_step) + 1;
        else
            data_set_y_size = (int)(y_size / y_res_step);

        interaction_object->hex_surface_object_data.grid_dimension = { data_set_x_size,data_set_y_size,0};
//printf("hcp_interaction_node_class:create_interaction_object 4444 dim :  %i : %i : %i \n", interaction_object->hex_surface_object_data.grid_dimension.x, interaction_object->hex_surface_object_data.grid_dimension.y);
        interaction_object->hex_surface_object_data.create_empty_surface_cubic(interaction_object->hex_surface_object_data.grid_dimension.x,
                                                                               interaction_object->hex_surface_object_data.grid_dimension.y);

//printf("hcp_interaction_node_class:create_interaction_object 55555 size : %i \n", interaction_object->hex_surface_object_data.hex_surface_matrix_data.size());
        // ########### END CREATE EMPTY VOXEL CLOUD MATRIX #################

        return interaction_object;
    }

    void deterime_interaction_matrix_start_values(hex_surface_object_class *hex_surface_object_A,
                                                  hex_surface_object_class *hex_surface_object_B, 
                                                  hex_surface_object_class *interaction_object,
                                                  glm::ivec2 &a_start, glm::ivec2 &b_start)
    {
        float xs, ys;// , xe, ye, ze;
        float hex_size = interaction_object->hex_surface_object_data.hex_size;

        //start indexes for voxel matrix A
        xs = ((interaction_object->hex_surface_object_data.grid_origin.x - hex_surface_object_A->hex_surface_object_data.grid_origin.x) / hex_size) + 0.5f;
        ys = ((interaction_object->hex_surface_object_data.grid_origin.y - hex_surface_object_A->hex_surface_object_data.grid_origin.y) / hex_size) + 0.5f;

        if (xs <= 1.0f) a_start.x = 0; else a_start.x = int(floor(xs)*hex_size);
        if (ys <= 1.0f) a_start.y = 0; else a_start.y = int(floor(ys)*hex_size);

        //start indexes for voxel matrix B
        xs = ((interaction_object->hex_surface_object_data.grid_origin.x - hex_surface_object_B->hex_surface_object_data.grid_origin.x) / hex_size) + 0.5f;
        ys = ((interaction_object->hex_surface_object_data.grid_origin.y - hex_surface_object_B->hex_surface_object_data.grid_origin.y) / hex_size) + 0.5f;

        if (xs <= 1.0f) b_start.x = 0; else b_start.x = int(floor(xs)*hex_size);
        if (ys <= 1.0f) b_start.y = 0; else b_start.y = int(floor(ys)*hex_size);

        // Calculate coordinates at end of entity matrices :: Is this needed ???????
        // Included here as comment to uncomment and put in seperate method if needed
/*        glm::vec3 a_origin = hex_surface_object_A->hex_surface_object_data.grid_origin;
        glm::vec3 b_origin = hex_surface_object_B->hex_surface_object_data.grid_origin;
        glm::vec3 i_origin = interaction_object->hex_surface_object_data.grid_origin;
        glm::vec3 a_endf, b_endf,i_endf;

        a_endf.x = (a_origin.x + hex_surface_object_A->hex_surface_object_data.matrix_dimension.x*hex_size);
        a_endf.y = (a_origin.y + hex_surface_object_A->hex_surface_object_data.matrix_dimension.y*hex_size);
        a_endf.z = (a_origin.z + hex_surface_object_A->hex_surface_object_data.matrix_dimension.z*hex_size);

        b_endf.x = (b_origin.x + hex_surface_object_B->hex_surface_object_data.matrix_dimension.x*hex_size);
        b_endf.y = (b_origin.y + hex_surface_object_B->hex_surface_object_data.matrix_dimension.y*hex_size);
        b_endf.z = (b_origin.z + hex_surface_object_B->hex_surface_object_data.matrix_dimension.z*hex_size);

        i_endf.x = (i_origin.x + interaction_object->hex_surface_object_data.matrix_dimension.x * hex_size);
        i_endf.y = (i_origin.y + interaction_object->hex_surface_object_data.matrix_dimension.y * hex_size);
        i_endf.z = (i_origin.z + interaction_object->hex_surface_object_data.matrix_dimension.z * hex_size);

        // End indexes for voxel matrix A
        xe = ((i_endf.x - a_endf.x) / hex_size) + 0.5;
        ye = ((i_endf.y - a_endf.y) / hex_size) + 0.5;
        ze = ((i_endf.z - a_endf.z) / hex_size) + 0.5;

        if (xe <= 0.0) a_end.x = a_start.x + (int)(floor(xe)); else a_end.x = hex_surface_object_A->hex_surface_object_data.matrix_dimension.x;
        if (ye <= 0.0) a_end.y = a_start.y + (int)(floor(ye)); else a_end.y = hex_surface_object_A->hex_surface_object_data.matrix_dimension.y;
        if (ze <= 0.0) a_end.z = a_start.z + (int)(floor(ze)); else a_end.z = hex_surface_object_A->hex_surface_object_data.matrix_dimension.z;

        // End indexes for voxel matrix B
        xe = ((i_endf.x - b_endf.x) / hex_size) + 0.5;
        ye = ((i_endf.y - b_endf.y) / hex_size) + 0.5;
        ze = ((i_endf.z - b_endf.z) / hex_size) + 0.5;

        if (xe <= 0.0) b_end.x = b_start.x + (int)(floor(xe)); else b_end.x = hex_surface_object_A->hex_surface_object_data.matrix_dimension.x;
        if (ye <= 0.0) b_end.y = b_start.y + (int)(floor(ye)); else b_end.y = hex_surface_object_A->hex_surface_object_data.matrix_dimension.y;
        if (ze <= 0.0) b_end.z = b_start.z + (int)(floor(ze)); else b_end.z = hex_surface_object_A->hex_surface_object_data.matrix_dimension.z;

        // END Calculate coordinates at end of entity matrices :: Is this needed ???????
*/
    }


private:
    hex_surface_render_class hex_surface_render;
};
