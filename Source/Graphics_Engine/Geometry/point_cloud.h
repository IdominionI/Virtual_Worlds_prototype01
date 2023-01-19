#pragma once

#include "geometry_basis.h"

/*
                POINT CLOUD GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point cloud object data type with
    point vertex location data only.
*/

class point_cloud3DD_class : public geometry_basis_class { // This Class not yet tested
public:
    glm::vec4 default_vertex_color = glm::vec4(0.7f, 0.3f, 0.7f, 1.0f);

    point_cloud3DD_class() = default;

    virtual ~point_cloud3DD_class() {
        if(buffers_created) delete_buffers(); // without this test, will have a crash 
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        glm::vec4 vertex;

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex = { i,j,k,1.0 };
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

    bool update_geometry(std::vector<glm::vec4>& coordinates) {
//printf("point_cloud_class :: update_geometry : 0000 || %i\n", buffers_created);
        if (buffers_created) delete_buffers();;
        if (coordinates.size() != vertices.mVertices.size()) {
            vertices.mColors = std::vector<glm::vec4>(coordinates.size(), default_vertex_color);
        }

        vertices.mVertices = coordinates;
        return create_buffers();

//printf("point_cloud_class :: update_geometry : 1111 \n");
        //if (!create_buffers()) return false;
// printf("point_cloud_class :: update_geometry : 2222 \n");

        //return true;
    }

    // Need buffers_created to avoid app crashing if no VAO created by defining vertices
    // Seems OpneGl has no way to test if a VAO exists or not so need this
    // NOTE : Early stage of development so this may change 
    bool buffers_created = false;

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const glm::vec4 &vertex) {
       // vertices.push_back(vertex);
        vertices.add_vertex(vertex);
 //       glm::vec4 testv = vertices.get_geometry_data(number_vertices - 1);// testing only : delete when testing done
//printf("TEST vertex basis : %f : %f :%f : %f \n", testv.x, testv.y,testv.z,testv.w);
        //vertices.add_color(1.0, 0.0, 0.0, 1.0);// testing only : delete when testing done
        //vertices.add_normal(1.0, 2.0, 0.0);// testing only : delete when testing done
        //number_vertices++;
    }

    // **********************

    bool update_colors(std::vector<glm::vec4>& colors) {
        if (!buffers_created) return false;
        return vertices.update_colors(colors);
    }

    bool update_normals(std::vector<glm::vec4>& normals) {
        if (!buffers_created) return false;
        return vertices.update_normals(normals);
    }

    bool update_data(std::vector<float>& data) {
        if (!buffers_created) return false;
        return vertices.update_data(data);
    }

    bool update_geometry_coordinate(int index, glm::vec3 coordinate) {
        if (!buffers_created) return false;
        return vertices.update_geometry_coordinate(index, coordinate);
    }

    bool update_geometry_color(int index, ColorAf color) {
        if (!buffers_created) return false;
        return vertices.update_geometry_color(index, color);
    }

    bool update_geometry_normal(int index, glm::vec4 normal) {
        if (!buffers_created) return false;
        return vertices.update_geometry_normal(index, normal);
    }

    bool update_geometry_data(int index, float data) {
        return vertices.update_geometry_data(index, data);
    }

    // **********************

    bool create_buffers() {
        vertex_buffer->create_vertex3DD_buffer(vertices);
        //printf("point_cloud_class :: create_buffers : CCCC %i\n", vertices.size());
        //number_vertices = vertices.mVertices.size();
        //number_vertices = vertices.number_of_vertices();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
        vertex_buffer->delete_vertex3DD_buffer();
        //number_vertices = 0;
        buffers_created = false;
        return true;
    }

    void render() {
        vertex_buffer->draw_points(vertices.number_of_vertices());
        //vertex_buffer->draw_points(number_vertices);
    }

    //std::vector<point_data_class> vertices;
    vertex3DD_class vertices;

private:
};


/*
                POINT CLOUD VERTEX GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point data cloud object data type
    with point vertex location and a normal vector value 
    corresponding to that vertex location.
*/

class point_cloud3D_class : public geometry_basis_class {
public:
    point_cloud3D_class() = default;

    virtual ~point_cloud3D_class() {
        if (buffers_created) delete_buffers(); // without this test, will have a crash 
    }

    bool init() {
        geometry_type = geometry_type_enum::points;
        vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
        generate_test_data();
        return true;
    }

    void generate_test_data() {
        vertices.clear();
        glm::vec3 vertex;
        glm::vec3 normal;

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex = { i,j,k };
                    normal = { 0.0,0.0,0.0 };
 
                    add_vertex(vertex);
                    add_normal(normal);
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
    //void add_vertex(const point_vertex_class& vertex) {
    void add_vertex(const glm::vec3& vertex) {
        vertices.add_vertex(vertex);
        number_vertices++;
    }

    void add_normal(const glm::vec3& normal) {
        vertices.add_normal(normal);
        //number_vertices++;
    }

    bool create_buffers() {
        if (vertices.number_of_vertices() < 1) return false;
//printf("point_cloud_data_class :: create_buffers : CCCC %i\n", vertices.size());
        vertex_buffer->create_vertex3DD_buffer(vertices);
        //number_vertices = vertices.number_of_vertices();
        buffers_created = true;
        return true;
    }

    bool delete_buffers() {
//printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
        if (!buffers_created) return false;
//printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
        vertex_buffer->delete_point_normal_data_vertex_buffers();
//printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 
        //number_vertices = 0;
        buffers_created = false;
        return true;
    }

    void render() {
        vertex_buffer->draw_points(vertices.number_of_vertices());
        //vertex_buffer->draw_points(number_vertices);
    }

    vertex3D_class vertices;

private:
};


/*
                POINT CLOUD DATA GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point data cloud object data type
    with point vertex location and a data value corresponding
    to that vertex location.
*/

class point_cloud_data_class : public geometry_basis_class {
public:
    point_cloud_data_class() = default;

    virtual ~point_cloud_data_class() {
        if (buffers_created) delete_buffers(); // without this test, will have a crash 
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

        for (float i = -0.5f; i < 0.5f; i += 0.1f) {
            for (float j = -0.5f; j < 0.5f; j += 0.1f) {
                for (float k = -0.5f; k < 0.5f; k += 0.1f) {
                    vertex.mPos = { i,j,k,1.0f };
                    //vertex.mPos   = {i,j,k};
                    //vertex.mvalue = 1;

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

    void render() {
        vertex_buffer->draw_points(int(number_vertices));
    }

    std::vector<point_data_value_class> vertices;

private:
};


/*
class point_cloud_class : public geometry_basis_class { // This Class not yet tested
public:
    point_cloud_class() = default;

    virtual ~point_cloud_class() {
        if (buffers_created) delete_buffers(); // without this test, will have a crash
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

    void render() {
        vertex_buffer->draw_points(number_vertices);
    }

    std::vector<point_data_class> vertices;

private:
};
*/

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