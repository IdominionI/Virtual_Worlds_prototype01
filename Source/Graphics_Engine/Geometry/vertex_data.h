#pragma once

#include "../Common/pch.h"

/*
    Vertex data classes that define the data storage of data to
    be stored by the application geometry classes and copied to
    a vertex buffer object for importation into an OpenGL shader
    program.

    Other vertex data classes can be added here or referenced from
    another C++ .h file as an include statement.
*/

// class to store vertex location, surface normal data and an integer 
// data value associated with that vertex location.

#include "primitives/color.h"

/*
    In definiing the vertex_basis_class found that could not call functions within the
    class that have overloaded functions of same name existing in derived class. Thus 
    each class that inherits this strcture needs all overloaded functions to add  data 
    to the mVertices, mNormals vectors need to be defined within the derived class. 
*/

template <typename VT1, typename VT2>
struct vertex_basis_class {
    std::vector<VT1>        mVertices;
    std::vector<glm::vec4>  mColors;
    std::vector<VT2>        mNormals;

    void add_color(const glm::vec4& color) { mColors.push_back(color); }
    void add_color(float r, float g, float b, float a = 1.0f) { mColors.push_back({ r,g,b,1.0 }); }
    void add_color(const Colorf& color) { mColors.push_back({ color.r,color.g,color.b,1.0 }); }
    void add_color(const ColorAf& color) { mColors.push_back(color); }

/*
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord(float s, float t, float r = 0, float q = 1) { add_texCoord0(glm::vec4(s, t, r, q)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord(const glm::vec2& t) { add_texCoord0(glm::vec4(t.x, t.y, 0, 1)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord(const glm::vec3& t) { add_texCoord0(glm::vec4(t.x, t.y, t.z, 1)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord(const glm::vec4& t) { add_texCoord0(t); }

	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord0(float s, float t, float r = 0, float q = 1) { add_texCoord0(glm::vec4(s, t, r, q)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord0(const glm::vec2& t) { add_texCoord0(glm::vec4(t.x, t.y, 0, 1)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord0(const glm::vec3& t) { add_texCoord0(glm::vec4(t.x, t.y, t.z, 1)); }
	//! Sets the texCoord0 for the current vertex (attribute geom::TEX_COORD_0)
	void	add_texCoord0(const glm::vec4& t) { mTexCoords0.push_back(t); }

	//! Sets the texCoord1 for the current vertex (attribute geom::TEX_COORD_1)
	void	add_texCoord1(float s, float t, float r = 0, float q = 1) { add_texCoord1(glm::vec4(s, t, r, q)); }
	//! Sets the texCoord1 for the current vertex (attribute geom::TEX_COORD_1)
	void	add_texCoord1(const glm::vec2& t) { add_texCoord1(glm::vec4(t.x, t.y, 0, 1)); }
	//! Sets the texCoord1 for the current vertex (attribute geom::TEX_COORD_1)
	void	add_texCoord1(const glm::vec3& t) { add_texCoord1(glm::vec4(t.x, t.y, t.z, 1)); }
	//! Sets the texCoord1 for the current vertex (attribute geom::TEX_COORD_1)
	void	add_texCoord1(const glm::vec4& t) { mTexCoords1.push_back(t); }
*/

    bool update_vertices(std::vector<VT1>& vertices) {
        if (vertices.size() != mVertices.size()) return false;
        mVertices = vertices;
        return true;
    }

    bool update_colors(std::vector<glm::vec4>& colors) {
        if (colors.size() != mVertices.size()) return false;
        mColors = colors;
        return true;
    }

    bool update_normals(std::vector<VT2>& normals) {
        if (normals.size() != mVertices.size()) return false;
        mNormals = normals;
        return true;
    }

    bool update_geometry_vertex(int index, VT1 vertex) {
        if (index >= mVertices.size()) return false;

        mVertices[index] = vertex;
        return true;
    }

    bool update_geometry_color(int index, ColorAf color) {
        if (index >= mVertices.size()) return false;

        mColors[index] = color;
        return true;
    }

    bool update_geometry_normal(int index, VT2 normal) {
        if (index >= mVertices.size()) return false;

        mNormals[index] = normal;
        return true;
    }


    std::vector<VT1>         get_geometry_data() { return mVertices; }
    std::vector<glm::vec4>   get_color_data()    { return mColors; }
    std::vector<VT2>         get_normal_data()   { return mNormals; }

    VT1 get_geometry_data(int i) {
        if (i < mVertices.size())
            return mVertices[i];
        else
            return VT1(0);
    }

    ColorAf get_color_data(int i) {
        if (i < mColors.size())
            return mColors[i];
        else
            return { 0.0,0.0,0.0,0.0 };
    }

    VT2 get_normal_data(int i) {
        if (i < mNormals.size())
            return mNormals[i];
        else
            return VT2(0);
    }

    int number_of_vertices() {
        return mVertices.size();
    }

    void clear() {
        mVertices.clear();
        mNormals.clear();
        mColors.clear();
    }
};


class vertex3DD_class : public vertex_basis_class<glm::vec4, glm::vec4> {
public:
	glm::vec4 mVertex,mColor,mNormal;

    void add_vertex(const glm::vec4& v) { mVertices.push_back(v); }
    void add_vertex(float x, float y, float z = 0, float w = 1) { mVertices.push_back(glm::vec4(x, y, z, w)); }
	void add_vertex(const glm::vec2& v) { mVertices.push_back(glm::vec4(v.x, v.y, 0, 1)); }
	void add_vertex(const glm::vec3& v) { mVertices.push_back(glm::vec4(v.x, v.y, v.z, 1)); }
	void add_vertex(const glm::vec4& v, const ColorAf& c) {
		mColors.push_back(c);
        mVertices.push_back(v);
	}

	//! Sets the normal for the current vertex (attribute geom::NORMAL)
    void add_normal(const glm::vec4 normal) { mNormals.push_back(normal);}
    void add_normal(float x, float y, float z) { mNormals.push_back({ x, y, z, 1.0 }); }


    bool update_data(std::vector<float>& data) {
        if (data.size() != mVertices.size()) return false;

        for (int i = 0; i < mVertices.size(); i++) {
            mVertices[i].w = data[i];
        }

        return true;
    }

    bool update_geometry_coordinate(int index, glm::vec3 coordinate) {
        if (index >= mVertices.size()) return false;

        mVertices[index] = glm::vec4(coordinate, mVertices[index].w);
        return true;
    }

    bool update_geometry_data(int index, float data) {
        if (index >= mVertices.size()) return false;

        mVertices[index].w = data;
        return true;
    }

    glm::vec3 get_geometry_location(int i) {
        if (i < mVertices.size())
            return { mVertices[i].x,mVertices[i].y,mVertices[i].z };
        else
            return { 0.0,0.0,0.0 };
    }

    float get_geometry_value(int i) {
        if (i < mVertices.size())
            return mVertices[i].w;
        else
            return 0.0;
    }

    glm::vec3 get_normal_vector(int i) {
        if (i < mNormals.size())
            return { mNormals[i].x,mNormals[i].y,mNormals[i].z };
        else
            return { 0.0,0.0,0.0 };
    }

    float get_normal_magnitude(int i) {
        if (i < mNormals.size())
            return mNormals[i].w;
        else
            return 0.0f;
    }

protected:

private:

};

class vertex2D_class : public vertex_basis_class<glm::vec2, glm::vec2> {
public:
    glm::vec2 mVertex;
    glm::vec4 mColor;
    glm::vec2 mNormal;

    std::vector<glm::vec2>  mVertices;
    std::vector<glm::vec4>  mColors;
    std::vector<glm::vec2>  mNormals;
    //std::vector<ColorAf>	    mColors;

    //std::vector<glm::vec4>		mTexCoords0, mTexCoords1;

    //! Locks the values for the current vertex and sets its position (attribute geom::POSITION). Call after any corresponding calls to color(), normal() and/or texCoord().
    void add_vertex(const glm::vec2& v) { mVertices.push_back(v);}
    void add_vertex(float x, float y) { add_vertex({ x, y }); }
    void add_vertex(const glm::vec4& v, const ColorAf& c) {
        mColors.push_back(c);
        add_vertex(v);
    }

    //! Sets the normal for the current vertex (attribute geom::NORMAL)
    void add_normal(const glm::vec2 normal) {mNormals.push_back(normal);}
    void add_normal(float x, float y) { add_normal({ x, y}); }

protected:

private:

};

class vertex3D_class : public vertex_basis_class<glm::vec3, glm::vec3> {
public:

    //!   Add vertex location / value data
    void add_vertex(const glm::vec3& v) { mVertices.push_back(v);}
    void add_vertex(float x, float y, float z) { mVertices.push_back({ x, y, z }); }
    void add_vertex(const glm::vec2& v) { mVertices.push_back({ v.x, v.y, 0 }); }
    void add_vertex(const glm::vec3& v, const ColorAf& c) {
        mColors.push_back(c);
        add_vertex(v);
    }

    //! Sets the normal for the current vertex
    void add_normal(const glm::vec3 normal) {mNormals.push_back(normal);}
    void add_normal(float x, float y) { mNormals.push_back({ x, y, 0.0 }); }
    void add_normal(float x, float y, float z) { mNormals.push_back({ x, y, z}); }
    void add_normal(const glm::vec2& n) { mNormals.push_back({ n.x, n.y, 0.0 }); }

    bool update_data(std::vector<float>& data) {
        if (data.size() != mVertices.size()) return false;

        for (int i = 0; i < mVertices.size(); i++) {
            mVertices[i].z = data[i];
        }

        return true;
    }

    bool update_geometry_coordinate(int index, glm::vec3 coordinate) {
        return update_geometry_vertex(index, coordinate);
    }

    bool update_geometry_coordinate(int index, glm::vec2 coordinate) {
        if (index >= mVertices.size()) return false;

        mVertices[index].x = coordinate.x;
        mVertices[index].y = coordinate.y;
        return true;
    }

    bool update_geometry2D_data(int index, float data) {
        if (index >= mVertices.size()) return false;

        mVertices[index].z = data;
        return true;
    }

    // ------------------------------------

    glm::vec2 get_geometry2D_location(int i) {
        if (i < mVertices.size())
            return { mVertices[i].x,mVertices[i].y };
        else
            return { 0.0,0.0 };
    }

    float get_geometry2D_value(int i) {
        if (i < mVertices.size())
            return mVertices[i].z;
        else
            return 0.0;
    }


    glm::vec2 get_normal2D_vector(int i) {
        if (i < mNormals.size())
            return { mNormals[i].x,mNormals[i].y };
        else
            return { 0.0,0.0 };
    }

    float get_normal2D_magnitude(int i) {
        if (i < mNormals.size())
            return mNormals[i].z;
        else
            return 0.0f;
    }

protected:

private:

};

typedef class vertex3D_class vertex2DD_class;


// class to store vertex location and surface normal data
class point_vertex_class {// was vertex_data_class

public:
    point_vertex_class() : mPos(), mNormal() {}

    // Initialize by string to be tokenized
    // TODO: Better handle outside of this class
    point_vertex_class(const std::vector<std::string> tokens)
    {
    }

    point_vertex_class(const glm::vec3& pos, const glm::vec3& normal)
        : mPos(pos), mNormal(normal)
    {
    }

    ~point_vertex_class() = default;


    glm::vec3 mPos;

    glm::vec3 mNormal;
};

// class to store vertex location data
class point_data_class {

public:
    point_data_class() : mPos() {}

    point_data_class(const glm::vec3& pos) : mPos(pos)
    {
    }

    ~point_data_class() = default;

    glm::vec3 mPos;
};

// class to store vertex location and an integer data value associated
// with that vertex location.
/*
    This class is designed to store a value in the vev4 pos.W variable
    At some future time when it is faund how to pass an integer value
    to a shader using this class, this may change.
*/
class point_data_value_class {

public:
    point_data_value_class() : mPos() {}

    point_data_value_class(const glm::vec4& pos)
        //: mPos(pos), mvalue(value)
        : mPos(pos)
    {
    }

    ~point_data_value_class() = default;

    glm::vec4 mPos;
    //glm::vec3 mPos;
    //int       mvalue;
};