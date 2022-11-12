#pragma once

#include <typeinfo>

#include "../../Common/pch.h"
#include "../../Geometry/vertex_data.h"

/*
                        OpenGL Vetex Buffer Objects Class

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is code that sets up and initialises and performs the
    various basic OpenGl vertex buffer object function calls 
    required to be performed to store data that is used in a glsl
    shader program, and delete the VBO when no longer needed.

    This class also manages the display of the VBO by the specified
    glsl shader program id number.
*/

class openGL_vertex_Index_buffer_class //: public vertex_index_buffer_class
{
public:
    openGL_vertex_Index_buffer_class() : mVBO{ 0 }, mVAO{ 0 }, mIBO{ 0 } {}

    /*
        Buffer Binding Target 	            Purpose
        
        GL_ARRAY_BUFFER 	            Vertex attributes
        GL_ATOMIC_COUNTER_BUFFER 	    Atomic counter storage
        GL_COPY_READ_BUFFER 	        Buffer copy source
        GL_COPY_WRITE_BUFFER 	        Buffer copy destination
        GL_DISPATCH_INDIRECT_BUFFER     Indirect compute dispatch commands
        GL_DRAW_INDIRECT_BUFFER 	    Indirect command arguments
        GL_ELEMENT_ARRAY_BUFFER 	    Vertex array indices
        GL_PIXEL_PACK_BUFFER 	        Pixel read target
        GL_PIXEL_UNPACK_BUFFER 	        Texture data source
        GL_QUERY_BUFFER 	            Query result buffer
        GL_SHADER_STORAGE_BUFFER 	    Read-write storage for shaders
        GL_TEXTURE_BUFFER 	            Texture data buffer
        GL_TRANSFORM_FEEDBACK_BUFFER 	Transform feedback buffer
        GL_UNIFORM_BUFFER 	            Uniform block storage

        void glBufferData( 	GLenum target, GLsizeiptr size,const void * data, GLenum usage);
        void glNamedBufferData( GLuint buffer, GLsizeiptr size,const void *data, GLenum usage);

        void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
        void glNamedBufferSubData( 	GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
    */


    // Amazing that this function works without having to specify data types from calling function !!! 
    // Seems defining data types in the derived class requires no definitions in calling this function
    template <typename VT1, typename VT2>
    //bool create_vertex3DD_buffer(vertex3DD_class &vertices ) {// need to change this data class
    bool create_vertex3DD_buffer(vertex_basis_class<VT1, VT2> &vertices ) {// need to change this data class
        const size_t verticesSizeBytes   = vertices.mVertices.size()   * sizeof(VT1);
        //const size_t verticesSizeBytes   = vertices.mVertices.size()   * sizeof(glm::vec4);
        const size_t colorsSizeBytes     = vertices.mColors.size()     * sizeof(glm::vec4);
		const size_t normalsSizeBytes    = vertices.mNormals.size()    * sizeof(VT2);
		//const size_t normalsSizeBytes    = vertices.mNormals.size()    * sizeof(glm::vec4);

		//const size_t texCoords0SizeBytes = mTexCoords0.size() * sizeof(glm::vec4);
		//const size_t texCoords1SizeBytes = mTexCoords1.size() * sizeof(glm::vec4);

        if (verticesSizeBytes < 1) return false;// Must have vertex location data

        size_t totalSizeBytes = verticesSizeBytes + normalsSizeBytes + colorsSizeBytes;// +texCoords0SizeBytes + texCoords1SizeBytes;

        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO: Not sure need this

        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO); // binds the vertex array object with name mVAO : Not sure need this

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id

//printf("openGL_vertex_Index_buffer_class : create_vertex_buffer %i : %i : %i \n", (int) verticesSizeBytes, (int)colorsSizeBytes, (int)normalsSizeBytes);

        // ################################################################################################
        // The folowing method of generating dynamic vertex data for opengl and glsl programs should be able to be applied to any
        // graphic or other data to be stored in a openGL vertex buffer object. What needs to be taken note of is what data is 
        // stored and the data type used to store that data in the buffer, and in what order is the data. 
        // Here it is point vertex location, color and normal data. 

        // vertex data is mandatory, but color and normal data is optional and if all three are used the glsl code needs to have the code
        //  layout(location = 0) in vec4 vertex;
        //  layout(location = 1) in vec4 color;
        //  layout(location = 2) in vec4 normal;
        // defined so as to have this data transfered and used in the glsl program
        // If normal data is defined, but not color, then this code becomes
        //  layout(location = 0) in vec4 vertex;
        //  layout(location = 1) in vec4 normal;

        //  All other vertex buffer data can follow this same method and code template to utilise any kind of data to be transfered to
        // a glsl program.


        glBufferData(GL_ARRAY_BUFFER, totalSizeBytes, NULL, GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        GLintptr offset = 0;
        GLuint   attribute_array_index = 0;
        GLint    number_of_VT1_components  = 3;
        GLint    number_of_VT2_components  = 3;
        bool     valid_VT1_data_type   = false;
        bool     valid_VT2_data_type   = false;

        if (typeid(VT1) == typeid(glm::vec2)) { number_of_VT1_components = 2; valid_VT1_data_type = true; }
        if (typeid(VT1) == typeid(glm::vec3)) { number_of_VT1_components = 3; valid_VT1_data_type = true; }
        if (typeid(VT1) == typeid(glm::vec4)) { number_of_VT1_components = 4; valid_VT1_data_type = true; }

        if (typeid(VT2) == typeid(glm::vec2)) { number_of_VT2_components = 2; valid_VT2_data_type = true; }
        if (typeid(VT2) == typeid(glm::vec3)) { number_of_VT2_components = 3; valid_VT2_data_type = true; }
        if (typeid(VT2) == typeid(glm::vec4)) { number_of_VT2_components = 4; valid_VT2_data_type = true; }

        if (!valid_VT1_data_type || !valid_VT2_data_type) {
            // Error message here !!!
            printf("openGL_vertex_Index_buffer_class :: create_vertex3DD_buffer : Error : invalid data type to create vertex buffer\n");
            return false;
        }

        // upload positions
        glBufferSubData(GL_ARRAY_BUFFER,offset, verticesSizeBytes, &vertices.mVertices[0]);
        glEnableVertexAttribArray(attribute_array_index); //use currently bound vertex array object for the operation of index 0
        //glVertexAttribPointer(attribute_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(VT1), (void*)0); //define an array of generic vertex location data :: see parameter list at bottom
        glVertexAttribPointer(attribute_array_index, number_of_VT1_components, GL_FLOAT, GL_FALSE, sizeof(VT1), (void*)0); //define an array of generic vertex location data :: see parameter list at bottom
        offset += verticesSizeBytes;
        attribute_array_index += 1;

        // upload colors
        if (!vertices.mColors.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, offset, colorsSizeBytes, &vertices.mColors[0]);
            glEnableVertexAttribArray(attribute_array_index); //use currently bound vertex array object for the operation of index 0
            glVertexAttribPointer(attribute_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)offset); //define an array of generic vertex location data :: see parameter list at bottom

            offset += colorsSizeBytes;
            attribute_array_index += 1;
        }

        // upload normals
        if (!vertices.mNormals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, offset, normalsSizeBytes, &vertices.mNormals[0]);
            glEnableVertexAttribArray(attribute_array_index); //use currently bound vertex array object for the operation of index 0
            //glVertexAttribPointer(attribute_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(VT2), (void*)offset); //define an array of generic vertex location data :: see parameter list at bottom
            glVertexAttribPointer(attribute_array_index, number_of_VT2_components, GL_FLOAT, GL_FALSE, sizeof(VT2), (void*)offset); //define an array of generic vertex location data :: see parameter list at bottom

            offset += normalsSizeBytes;
            attribute_array_index += 1;
        }

        // If ever to use texture coordinates define here

        // ################################################################################################


        glBindVertexArray(0); //: Not sure need this
    }

    void delete_vertex3DD_buffer() {
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }
    // ----------------------------------------------------------------------------------------


    void create_point_vertex_buffer(const std::vector<point_data_class>& vertices) {// need to change this data class
        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO: Not sure need this

        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO); // binds the vertex array object with name mVAO : Not sure need this

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_data_class), vertices.data(), GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        glEnableVertexAttribArray(0); //use currently bound vertex array object for the operation of index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point_data_class), (void*)0); //define an array of generic vertex location data :: see parameter list at bottom

        glBindVertexArray(0); //: Not sure need this
    }

    void delete_point_buffers() {
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

    // -------------------------------------------------------------------------------------------

    void create_point_data_vertex_buffer(const std::vector<point_data_value_class>& vertices) {
        // Generate buffers and store opengl index of the buffer to use later on in the variable &variable_name 
        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO

        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_data_value_class), vertices.data(), GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        glEnableVertexAttribArray(0); //use currently bound vertex array object for the operation of index 0
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point_data_value_class), (void*)0);//define an array of generic vertex attribute data :: see parameter list at bottom
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(point_data_value_class), (void*)0);//define an array of generic vertex attribute data :: see parameter list at bottom

        // Cannot get this integer data definition to pass integer data into a vertex shader to work :: Usual opengl-glsl BS where no clue
        // of what is going wrong, if anything, and how to fix it !!!!!!!
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(point_data_value_class), (void*)offsetof(point_data_value_class, mvalue));

        glBindVertexArray(0);

        // Should the following be here and not in delete_point_data_buffers ?
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    //virtual void delete_point_data_buffers() {
    // Having this as a virtual function crashes app with exception error
    void delete_point_data_buffers() {
        //glDisableVertexAttribArray(0);
        //glDisableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

    // -------------------------------------------------------------------------------------------
/*
    This funtion is not tested and going by inability to pass
    an integer value to a shader, the same may apply for passing
    a normal vector to a shader.

    This function may not be necessary and is thus commented out
*/
/*
    void create_point_normal_vertex_buffer(const std::vector<point_vertex_data_class>& vertices) {
        // Generate buffers and store opengl index of the buffer to use later on in the variable &variable_name 
        // Not tested

        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO

        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_vertex_data_class), vertices.data(), GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        glEnableVertexAttribArray(0); //use currently bound vertex array object for the operation of index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_data_class), (void*)0);//define an array of generic vertex attribute data :: see parameter list at bottom

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_data_class), (void*)offsetof(point_vertex_data_class, mNormal));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(point_vertex_data_class), (void*)offsetof(point_vertex_data_class, mvalue));

        glBindVertexArray(0);
    }

    void delete_point_normal_vertex_buffers() {
        //glDisableVertexAttribArray(0);
        //glDisableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }
*/
    // -------------------------------------------------------------------------------------------
    void create_point_normal_vertex_buffer(const std::vector<point_vertex_class>& vertices) {
        // Generate buffers and store opengl index of the buffer to use later on in the variable &variable_name
        // Not tested

        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO

        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_vertex_class), vertices.data(), GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        glEnableVertexAttribArray(0); //use currently bound vertex array object for the operation of index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_class), (void*)0);//define an array of generic vertex attribute data :: see parameter list at bottom

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_class), (void*)offsetof(point_vertex_class, mNormal));

        glBindVertexArray(0);
    }

    void delete_point_normal_data_vertex_buffers() {
        //glDisableVertexAttribArray(0);
        //glDisableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

    // -------------------------------------------------------------------------------------------

    //virtual void create_surface_buffers(const std::vector<vertex_data_class>& vertices, const std::vector<unsigned int>& indices) {
    void create_surface_buffers(const std::vector<point_vertex_class>& vertices, const std::vector<unsigned int>& indices) {
        // Generate buffers and store opengl index of the buffer to use later on in the variable &variable_name 
        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO

        glGenBuffers(1, &mIBO);
        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO); 

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_vertex_class), vertices.data(), GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); //use currently bound vertex array object for the operation of index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_class), (void*)0);//define an array of generic vertex attribute data :: see parameter list at bottom

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(point_vertex_class), (void*)offsetof(point_vertex_class, mNormal));

        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //virtual void delete_surface_buffers(){
    void delete_surface_buffers(){
        //glDisableVertexAttribArray(0);
        //glDisableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mIBO);
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

    // -------------------------------------------------------------------------------------------

    virtual void bind() {
        glBindVertexArray(mVAO);
    }

    virtual void unbind() {
        glBindVertexArray(0);
    }

    virtual void draw_triangles(int index_count) {
        bind();

        // the vertices as line loop
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
       
        unbind();
    }

    virtual void draw_triangles_array(int index_count, int shader_program_id) {
        bind();

        // the vertices as line loop
//printf("draw_triangles_array %i\n", index_count);
 
        glDrawArrays(GL_TRIANGLES, 0, index_count);

        unbind();
    }

    virtual void draw_points(int index_count) {
        bind();
//printf("draw_points %i\n", index_count);
       glDrawArrays(GL_POINTS,0, index_count);
       // glDrawElements(GL_POINTS, index_count, GL_UNSIGNED_INT, nullptr); // this causes a crash

        unbind();
    }

    virtual void draw_lines(int index_count) {
        bind();
//printf("draw_lines %i\n", index_count);
        glDrawArrays(GL_LINES, 0, index_count);

        unbind();
    }

protected:
    GLuint mVBO;// Storage id of vertex buffer object that stores geometry vertex data
    GLuint mVAO;// Storage id of vertex array object that stores geometry vertex data
    GLuint mIBO;// Storage id of vertex index buffer object that stores geometry vertex indices data for drawing triangles
    //GLuint mDBO;// Storage id of vertex data buffer object that stores data for each vertex point // *****
};


//void glVertexAttribPointer(GLuint index,              index of the generic vertex attribute to be modified
//                           GLint size,                number of components per generic vertex attribute. Must be 1, 2, 3, 4
//                           GLenum type,               data type of each component in the array
//                           GLboolean normalized,      specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
//                           GLsizei stride,            Specifies the byte offset between consecutive generic vertex attributes.
//                           const GLvoid * pointer);   Specifies a offset of the first component of the first generic vertex attribute