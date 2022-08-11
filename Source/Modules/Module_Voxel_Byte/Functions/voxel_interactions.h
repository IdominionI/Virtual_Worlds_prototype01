#pragma once


#include "../Voxel_hcp_object/voxel_hcp_object.h"


class hcp_voxel_interactions_class {
public:

    scene_manager_class *scene_manager = NULL;
    log_panel_class     *log_panel     = NULL;

    static bool voxel_intersection(voxel_hcp_object_class *voxel_hcp_object_A, voxel_hcp_object_class *voxel_hcp_object_B) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = voxel_hcp_object_A->voxel_object_data.matrix_origin.x;
        float a_xmax = voxel_hcp_object_A->voxel_object_data.matrix_origin.x + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.x_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.x_start);
        float b_xmin = voxel_hcp_object_B->voxel_object_data.matrix_origin.x;
        float b_xmax = voxel_hcp_object_B->voxel_object_data.matrix_origin.x + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.x_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.x_start);

        float a_ymin = voxel_hcp_object_A->voxel_object_data.matrix_origin.y;
        float a_ymax = voxel_hcp_object_A->voxel_object_data.matrix_origin.y + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.y_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.y_start);
        float b_ymin = voxel_hcp_object_B->voxel_object_data.matrix_origin.y;
        float b_ymax = voxel_hcp_object_B->voxel_object_data.matrix_origin.y + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.y_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.y_start);

        float a_zmin = voxel_hcp_object_A->voxel_object_data.matrix_origin.z;
        float a_zmax = voxel_hcp_object_A->voxel_object_data.matrix_origin.z + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.z_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.z_start);
        float b_zmin = voxel_hcp_object_B->voxel_object_data.matrix_origin.z;
        float b_zmax = voxel_hcp_object_B->voxel_object_data.matrix_origin.z + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.z_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.z_start);


        if (((a_xmin >= b_xmin) && (a_xmin <= b_xmax)) || ((a_xmax <= b_xmax) && (a_xmax >= b_xmin))) x_intersection = true;
        if (((b_xmin >= a_xmin) && (b_xmin <= a_xmax)) || ((b_xmax <= a_xmax) && (b_xmax >= a_xmin))) x_intersection = true;
        if (((a_ymin >= b_ymin) && (a_ymin <= b_ymax)) || ((a_ymax <= b_ymax) && (a_ymax >= b_ymin))) y_intersection = true;
        if (((b_ymin >= a_ymin) && (b_ymin <= a_ymax)) || ((b_ymax <= a_ymax) && (b_ymax >= a_ymin))) y_intersection = true;
        if (((a_zmin >= b_zmin) && (a_zmin <= b_zmax)) || ((a_zmax <= b_zmax) && (a_zmax >= b_zmin))) z_intersection = true;
        if (((b_zmin >= a_zmin) && (b_zmin <= a_zmax)) || ((b_zmax <= a_zmax) && (b_zmax >= a_zmin))) z_intersection = true;

        if (x_intersection && x_intersection && x_intersection)
            return true;
        else
            return false;

    }

    static bool define_intersection_boundary(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B, glm::vec3 &boundary_min, glm::vec3 &boundary_max) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = voxel_hcp_object_A->voxel_object_data.matrix_origin.x;
        float a_xmax = voxel_hcp_object_A->voxel_object_data.matrix_origin.x + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.x_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.x_start);
        float b_xmin = voxel_hcp_object_B->voxel_object_data.matrix_origin.x;
        float b_xmax = voxel_hcp_object_B->voxel_object_data.matrix_origin.x + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.x_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.x_start);

        float a_ymin = voxel_hcp_object_A->voxel_object_data.matrix_origin.y;
        float a_ymax = voxel_hcp_object_A->voxel_object_data.matrix_origin.y + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.y_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.y_start);
        float b_ymin = voxel_hcp_object_B->voxel_object_data.matrix_origin.y;
        float b_ymax = voxel_hcp_object_B->voxel_object_data.matrix_origin.y + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.y_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.y_start);

        float a_zmin = voxel_hcp_object_A->voxel_object_data.matrix_origin.z;
        float a_zmax = voxel_hcp_object_A->voxel_object_data.matrix_origin.z + (voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.z_end - voxel_hcp_object_A->voxel_object_data.voxel_generator_parameters.z_start);
        float b_zmin = voxel_hcp_object_B->voxel_object_data.matrix_origin.z;
        float b_zmax = voxel_hcp_object_B->voxel_object_data.matrix_origin.z + (voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.z_end - voxel_hcp_object_B->voxel_object_data.voxel_generator_parameters.z_start);

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

        if ((a_zmin >= b_zmin) && (a_zmin <= b_zmax)){
            boundary_min.z = a_zmin;
            z_intersection = true;
        } else
            if ((b_zmin >= a_zmin) && (b_zmin <= a_zmax)) {
                boundary_min.z = b_zmin;
                z_intersection = true;
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

        if ((a_zmax <= b_zmax) && (a_zmax >= b_zmin)){
            boundary_max.z = a_xmax;
            z_intersection = true;
        } else
            if ((b_zmax <= a_zmax) && (b_zmax >= a_zmin)) {
                boundary_max.z = b_zmax;
                z_intersection = true;
            }

        if (x_intersection && y_intersection && z_intersection)
            return true;
        else
            return false;
    }


   bool align_voxel_matricies(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B, bool update_hcp_object_A_vertices = true) {
//printf("hcp_interaction_node_class:align_voxel_matricies 0000 %\n");
        if (voxel_hcp_object_A->voxel_object_data.voxel_size != voxel_hcp_object_B->voxel_object_data.voxel_size) return false;

        float voxel_size = voxel_hcp_object_A->voxel_object_data.voxel_size;

        glm::vec3 a_origin = voxel_hcp_object_A->voxel_object_data.matrix_origin;
        glm::vec3 b_origin = voxel_hcp_object_B->voxel_object_data.matrix_origin;

        glm::vec3 origin_diff = b_origin - a_origin;

        glm::vec3 translation;

        translation.x = fmod(origin_diff.x, voxel_size);
        translation.y = fmod(origin_diff.y, voxel_size);
        translation.z = fmod(origin_diff.z, voxel_size);

        voxel_hcp_object_A->voxel_object_data.matrix_origin += translation * voxel_size;

        if (update_hcp_object_A_vertices) update_voxel_verticies(voxel_hcp_object_A);

        return true;
    }

        // !!!!!!!! FOLLOWING UPDATE CODE NEEDS TO BE PUT INTO A GENERAL UPDATE VOXEL VERTICES FUNCTION !!!!!!!!!!!!!!
    void update_voxel_verticies(voxel_hcp_object_class *voxel_hcp_object) {
//printf("hcp_voxel_interactions_class:update_voxel_verticies 0000\n");        
        voxel_hcp_object->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);

        //####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
        scene_node_class <render_object_class>* scene_voxel_object = scene_manager->get_render_object(voxel_hcp_object->object_id);
//printf("hcp_voxel_interactions_class:update_voxel_verticies 11111: %i : %i \n", scene_voxel_object->scene_node_entity_id, voxel_hcp_object->object_id);

        if (scene_voxel_object != NULL) {
            if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&voxel_hcp_object->point_cloud, scene_voxel_object, log_panel)) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : scene voxel object geometry could not be updated.\n");
//printf("hcp_voxel_interactions_class action : scene_voxel_object not updated\n");
                return;
            }
//printf("hcp_voxel_interactions_class:update_voxel_verticies 22222 \n");
 //           application_default_shader_uniform_variables_struct_type uniform_variable;
 //           uniform_variable.type = application_default_shader_variable_type_enum::Floatv3; uniform_variable.name = "voxel_origin"; uniform_variable.value0 = &voxel_hcp_object->voxel_object_data.matrix_origin;
  //          scene_voxel_object->scene_graph_object.scene_object_class.shader_material.update_shader_variable(uniform_variable);
        }

//if (scene_voxel_object == NULL) {
//printf("hcp_interaction_node_class:update_voxel_verticies 33333 scene_voxel_object == NULL \n");       
//}
    }

    voxel_hcp_object_class *create_interaction_object(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B,int &entity_id) {
        glm::vec3 boundary_min, boundary_max;
       
        if (scene_manager == NULL) return NULL;

//printf("hcp_interaction_node_class:create_interaction_object 0000 %\n");
        if (!align_voxel_matricies(voxel_hcp_object_A, voxel_hcp_object_B), false) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 11111  %\n");
        if (!define_intersection_boundary(voxel_hcp_object_A, voxel_hcp_object_B, boundary_min, boundary_max)) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 2222 min : %f : %f : %f \n", boundary_min.x, boundary_min.y, boundary_min.z);
//printf("hcp_interaction_node_class:create_interaction_object 2222 max : %f : %f : %f \n", boundary_max.x, boundary_max.y, boundary_max.z);

        if (entity_id == INVALID_ID) {
            entity_id = globalc::get_available_entity_id();
            if (!scene_manager->add_entity(entity_id, ENTITY_CATEGORY_HCP_VOXEL)) {
                entity_id = INVALID_ID;
                return NULL;
            }
        }

        voxel_hcp_object_class* interaction_object = scene_manager->entities_manager.get_voxel_hcp_entity_object(entity_id);
        if (interaction_object == NULL) return NULL;

        interaction_object->voxel_object_data.matrix_origin = boundary_min;
        interaction_object->voxel_object_data.voxel_size = voxel_hcp_object_A->voxel_object_data.voxel_size;

        // ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
        float x_size = boundary_max.x - boundary_min.x;
        float y_size = boundary_max.y - boundary_min.y;
        float z_size = boundary_max.z - boundary_min.z;
        float z_mult = 2.0 * sqrt(6.0) / 3.0;

        float x_res_step = interaction_object->voxel_object_data.voxel_size * 2.0;
        float y_res_step = interaction_object->voxel_object_data.voxel_size * (3.0 / sqrt(3.0));
        float z_res_step = interaction_object->voxel_object_data.voxel_size * z_mult;

        int data_set_x_size, data_set_y_size, data_set_z_size;

        if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
            data_set_x_size = (int)(x_size / x_res_step) + 1;
        else
            data_set_x_size = (int)(x_size / x_res_step);

        if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
            data_set_y_size = (int)(y_size / y_res_step) + 1;
        else
            data_set_y_size = (int)(y_size / y_res_step);

        if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0)
            data_set_z_size = (int)(z_size / z_res_step) + 1;
        else
            data_set_z_size = (int)(z_size / z_res_step);

        interaction_object->voxel_object_data.matrix_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
//printf("hcp_interaction_node_class:create_interaction_object 3333 dim :  %i : %i : %i \n", interaction_object->voxel_object_data.matrix_dimension.x, interaction_object->voxel_object_data.matrix_dimension.y, interaction_object->voxel_object_data.matrix_dimension.z);
        interaction_object->voxel_object_data.create_empty_volume_cubic(interaction_object->voxel_object_data.matrix_dimension.x,
                                                                        interaction_object->voxel_object_data.matrix_dimension.y,
                                                                        interaction_object->voxel_object_data.matrix_dimension.z);

//printf("hcp_interaction_node_class:create_interaction_object 4444 size : %i \n", interaction_object->voxel_object_data.voxel_matrix_data.size());
        // ########### END CREATE EMPTY VOXEL CLOUD MATRIX #################

        return interaction_object;
    }

    void deterime_interaction_matrix_start_values(voxel_hcp_object_class *voxel_hcp_object_A,
                                                  voxel_hcp_object_class *voxel_hcp_object_B, 
                                                  voxel_hcp_object_class *interaction_object,
                                                  glm::ivec3 &a_start, glm::ivec3 &b_start)
    {
        float xs, ys, zs;// , xe, ye, ze;
        float voxel_size = interaction_object->voxel_object_data.voxel_size;

        //start indexes for voxel matrix A
        xs = ((interaction_object->voxel_object_data.matrix_origin.x - voxel_hcp_object_A->voxel_object_data.matrix_origin.x) / voxel_size) + 0.5;
        ys = ((interaction_object->voxel_object_data.matrix_origin.y - voxel_hcp_object_A->voxel_object_data.matrix_origin.y) / voxel_size) + 0.5;
        zs = ((interaction_object->voxel_object_data.matrix_origin.z - voxel_hcp_object_A->voxel_object_data.matrix_origin.z) / voxel_size) + 0.5;

        if (xs <= 1.0) a_start.x = 0; else a_start.x = (int)floor(xs)*voxel_size;
        if (ys <= 1.0) a_start.y = 0; else a_start.y = (int)floor(ys)*voxel_size;
        if (zs <= 1.0) a_start.z = 0; else a_start.z = (int)floor(zs)*voxel_size;

        //start indexes for voxel matrix B
        xs = ((interaction_object->voxel_object_data.matrix_origin.x - voxel_hcp_object_B->voxel_object_data.matrix_origin.x) / voxel_size) + 0.5;
        ys = ((interaction_object->voxel_object_data.matrix_origin.y - voxel_hcp_object_B->voxel_object_data.matrix_origin.y) / voxel_size) + 0.5;
        zs = ((interaction_object->voxel_object_data.matrix_origin.z - voxel_hcp_object_B->voxel_object_data.matrix_origin.z) / voxel_size) + 0.5;

        if (xs <= 1.0) b_start.x = 0; else b_start.x = (int)floor(xs)*voxel_size;
        if (ys <= 1.0) b_start.y = 0; else b_start.y = (int)floor(ys)*voxel_size;
        if (zs <= 1.0) b_start.z = 0; else b_start.z = (int)floor(zs)*voxel_size;

        // Calculate coordinates at end of entity matrices :: Is this needed ???????
        // Included here as comment to uncomment and put in seperate method if needed
/*        glm::vec3 a_origin = voxel_hcp_object_A->voxel_object_data.matrix_origin;
        glm::vec3 b_origin = voxel_hcp_object_B->voxel_object_data.matrix_origin;
        glm::vec3 i_origin = interaction_object->voxel_object_data.matrix_origin;
        glm::vec3 a_endf, b_endf,i_endf;

        a_endf.x = (a_origin.x + voxel_hcp_object_A->voxel_object_data.matrix_dimension.x*voxel_size);
        a_endf.y = (a_origin.y + voxel_hcp_object_A->voxel_object_data.matrix_dimension.y*voxel_size);
        a_endf.z = (a_origin.z + voxel_hcp_object_A->voxel_object_data.matrix_dimension.z*voxel_size);

        b_endf.x = (b_origin.x + voxel_hcp_object_B->voxel_object_data.matrix_dimension.x*voxel_size);
        b_endf.y = (b_origin.y + voxel_hcp_object_B->voxel_object_data.matrix_dimension.y*voxel_size);
        b_endf.z = (b_origin.z + voxel_hcp_object_B->voxel_object_data.matrix_dimension.z*voxel_size);

        i_endf.x = (i_origin.x + interaction_object->voxel_object_data.matrix_dimension.x * voxel_size);
        i_endf.y = (i_origin.y + interaction_object->voxel_object_data.matrix_dimension.y * voxel_size);
        i_endf.z = (i_origin.z + interaction_object->voxel_object_data.matrix_dimension.z * voxel_size);

        // End indexes for voxel matrix A
        xe = ((i_endf.x - a_endf.x) / voxel_size) + 0.5;
        ye = ((i_endf.y - a_endf.y) / voxel_size) + 0.5;
        ze = ((i_endf.z - a_endf.z) / voxel_size) + 0.5;

        if (xe <= 0.0) a_end.x = a_start.x + (int)(floor(xe)); else a_end.x = voxel_hcp_object_A->voxel_object_data.matrix_dimension.x;
        if (ye <= 0.0) a_end.y = a_start.y + (int)(floor(ye)); else a_end.y = voxel_hcp_object_A->voxel_object_data.matrix_dimension.y;
        if (ze <= 0.0) a_end.z = a_start.z + (int)(floor(ze)); else a_end.z = voxel_hcp_object_A->voxel_object_data.matrix_dimension.z;

        // End indexes for voxel matrix B
        xe = ((i_endf.x - b_endf.x) / voxel_size) + 0.5;
        ye = ((i_endf.y - b_endf.y) / voxel_size) + 0.5;
        ze = ((i_endf.z - b_endf.z) / voxel_size) + 0.5;

        if (xe <= 0.0) b_end.x = b_start.x + (int)(floor(xe)); else b_end.x = voxel_hcp_object_A->voxel_object_data.matrix_dimension.x;
        if (ye <= 0.0) b_end.y = b_start.y + (int)(floor(ye)); else b_end.y = voxel_hcp_object_A->voxel_object_data.matrix_dimension.y;
        if (ze <= 0.0) b_end.z = b_start.z + (int)(floor(ze)); else b_end.z = voxel_hcp_object_A->voxel_object_data.matrix_dimension.z;

        // END Calculate coordinates at end of entity matrices :: Is this needed ???????
*/
    }


private:
    voxel_hcp_render_class voxel_hcp_render;
};
