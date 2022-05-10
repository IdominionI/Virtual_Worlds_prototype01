#pragma once

#include "geometry_basis.h"

class point_cloud_class : public geometry_basis_class { // This Class not yet tested
public:
    point_cloud_class() = default;

    virtual ~point_cloud_class() {
        delete_buffers();
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        point_data_class vertex;

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex.mPos = { i,j,k };
                    add_vertex(vertex);
                }
            }
        }
        update_geometry();
    }

    // Update geometry data in the buffers with the current 
    // vertex data content.
    bool update_geometry() {
//printf("point_cloud_class :: update_geometry : 0000 || %i\n", buffers_created);
        if (buffers_created) delete_buffers();
 //printf("point_cloud_class :: update_geometry : 1111 \n");
        if (!create_buffers()) return false;
// printf("point_cloud_class :: update_geometry : 2222 \n");
        return true;
    }

    // Need buffers_created to avoid app crashing if no VAO created by defining vertices
    // Seems OpneGl has no way to test if a VAO exists or not so need this
    // NOTE : Early stage of development so this may change 
    bool buffers_created = false;

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_data_class& vertex) { 
        vertices.push_back(vertex); 
        number_vertices++;
    }

    bool create_buffers() {
        vertex_buffer->create_point_vertex_buffer(vertices);
//printf("point_cloud_class :: create_buffers : CCCC %i\n", vertices.size());
        number_vertices = vertices.size();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
        vertex_buffer->delete_point_buffers();
        number_vertices = 0;
        buffers_created = false;
        return true;
    }

    std::vector<point_data_class> vertices;

private:
};

class point_cloud_data_class : public geometry_basis_class {
public:
    point_cloud_data_class() = default;

    virtual ~point_cloud_data_class() {
        delete_buffers();
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        point_data_value_class vertex;

        for (float i = -0.5f; i < 0.5f; i+=0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex.mPos   = {i,j,k};
                    vertex.mvalue = 1;

                    add_vertex(vertex);
                }
            }
        }

        update_geometry();
    }

    // Update geometry data in the buffers with the current 
    // vertex data content.
    bool update_geometry(){
//printf("point_cloud_data_class :: update_geometry : 0000 || %i\n", buffers_created);
        if(buffers_created) delete_buffers();
//printf("point_cloud_data_class :: update_geometry : 1111 \n");       
        if (!create_buffers()) return false;
//printf("point_cloud_data_class :: update_geometry : 2222 \n"); 
        return true;
    }
    
    // Need buffers_created to avoid app crashing if no VAO created by defining vertices
    // Seems OpneGl has no way to test if a VAO exists or not so need this
    // NOTE : Early stage of development so this may change 
    bool buffers_created = false;

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_data_value_class& vertex) {
        vertices.push_back(vertex);
        number_vertices++;
    }

    bool create_buffers() {
        if (vertices.size() < 1) return false;
//printf("point_cloud_data_class :: create_buffers : CCCC %i\n", vertices.size());
        vertex_buffer->create_point_data_vertex_buffer(vertices);
        number_vertices = vertices.size();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
//printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
        if (!buffers_created) return false;
//printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
        vertex_buffer->delete_point_data_buffers();
//printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 
        number_vertices = 0;
        buffers_created = false;
        return true;
    }

    std::vector<point_data_value_class> vertices;

private:
};


class point_cloud_vertex_class : public geometry_basis_class {
public:
    point_cloud_vertex_class() = default;

    virtual ~point_cloud_vertex_class() {
        delete_buffers();
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        point_vertex_class vertex;

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex.mPos = { i,j,k };
                    vertex.mNormal = { 0.0,0.0,0.0 };
 
                    add_vertex(vertex);
                }
            }
        }

        update_geometry();
    }

    // Update geometry data in the buffers with the current 
    // vertex data content.
    bool update_geometry() {
        //printf("point_cloud_data_class :: update_geometry : 0000 || %i\n", buffers_created);
        if (buffers_created) delete_buffers();
        //printf("point_cloud_data_class :: update_geometry : 1111 \n");       
        if (!create_buffers()) return false;
        //printf("point_cloud_data_class :: update_geometry : 2222 \n"); 
        return true;
    }

    // Need buffers_created to avoid app crashing if no VAO created by defining vertices
    // Seems OpneGl has no way to test if a VAO exists or not so need this
    // NOTE : Early stage of development so this may change 
    bool buffers_created = false;

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_vertex_class& vertex) {
        vertices.push_back(vertex);
        number_vertices++;
    }

    bool create_buffers() {
        if (vertices.size() < 1) return false;
        //printf("point_cloud_data_class :: create_buffers : CCCC %i\n", vertices.size());
        vertex_buffer->create_point_normal_vertex_buffer(vertices);
        number_vertices = vertices.size();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
        //printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
        if (!buffers_created) return false;
        //printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
        vertex_buffer->delete_point_normal_data_vertex_buffers();
        //printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 
        number_vertices = 0;
        buffers_created = false;
        return true;
    }

    std::vector<point_vertex_class> vertices;

private:
};
/*
class point_cloud_vertex_data_class : public geometry_basis_class {
public:
    point_cloud_vertex_data_class() = default;

    virtual ~point_cloud_vertex_data_class() {
        delete_buffers();
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        point_vertex_data_class vertex;

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex.mPos = { i,j,k };
                    vertex.mNormal = { 0.0,0.0,0.0 };
                    vertex.mvalue = 1;

                    add_vertex(vertex);
                }
            }
        }

        update_geometry();
    }

    // Update geometry data in the buffers with the current 
    // vertex data content.
    bool update_geometry() {
        //printf("point_cloud_data_class :: update_geometry : 0000 || %i\n", buffers_created);
        if (buffers_created) delete_buffers();
        //printf("point_cloud_data_class :: update_geometry : 1111 \n");       
        if (!create_buffers()) return false;
        //printf("point_cloud_data_class :: update_geometry : 2222 \n"); 
        return true;
    }

    // Need buffers_created to avoid app crashing if no VAO created by defining vertices
    // Seems OpneGl has no way to test if a VAO exists or not so need this
    // NOTE : Early stage of development so this may change 
    bool buffers_created = false;

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_vertex_data_class& vertex) {
        vertices.push_back(vertex);
        number_vertices++;
    }

    bool create_buffers() {
        if (vertices.size() < 1) return false;
//printf("point_cloud_data_class :: create_buffers : CCCC %i\n", vertices.size());
        vertex_buffer->create_point_normal_data_vertex_buffer(vertices);
        number_vertices = vertices.size();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
//printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
        if (!buffers_created) return false;
//printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
        vertex_buffer->delete_point_normal_data_vertex_buffers();
//printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 
        number_vertices = 0;
        buffers_created = false;
        return true;
    }

    std::vector<point_vertex_data_class> vertices;

private:
};
*/