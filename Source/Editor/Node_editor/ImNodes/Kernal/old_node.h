#pragma once
//#include <map>

#include <string>
#include <memory> // Used for unique pointer : delete if not needed

#include <imgui.h>
#include <Universal/3rdparty/include/glm/glm.hpp>

#include "../../../Common/definitions.h"

#include "../imnodes.h"

#include "graph.h"

// function category
#define EVALUATOR  0
#define MODIFIER   1
#define GENERATOR  2


//function 
#define NO_FUNCTION -1
#define VALUE    0
#define STRUCT   100
#define ADD      1
#define MULTIPLY 2
#define OUTPUT   3
#define SINE     4
#define TIME     5

//typedef int node_category_ID_type;
//typedef int node_context_ID_type;
//typedef int node_function_category_ID_type;
//typedef int node_function_ID_type;

// Node input variable data structures

struct node_type_struct {
    int category          = -1;
    int context           = -1;
    int function_category = -1;
    int function          = -1;
};

enum class node_value_type_enum { None, Bool, Float, Integer, UInteger, Vec2, Vec3, Vec4, IVec2, IVec3, IVec4, Data_Ptr };

struct node_value_struct_type {
    int                  node_id;//******
    node_type_struct     node_type;
    node_value_type_enum node_value_type = node_value_type_enum::None;
    std::string          label = "";
    void                *value = NULL;

    explicit node_value_struct_type(const node_type_struct t) : node_type(t), node_value_type(node_value_type_enum::None), label(""), value(NULL) {}
    explicit node_value_struct_type(const node_type_struct t, const node_value_type_enum vt) : node_type(t), node_value_type(vt), label(""), value(NULL) {
        add_node_value(vt);
    }

    node_value_struct_type(const node_type_struct t, const node_value_type_enum vt, std::string s, void *v) : node_type(t), node_value_type(vt), label(s), value(v) {}

    ~node_value_struct_type() {
        delete value;
    }

    void add_node_value(node_value_type_enum value_type) {
        switch (value_type) {
            case node_value_type_enum::Bool    :{value = new bool;  *(bool *)value = false; break;}
            case node_value_type_enum::Float   :{value = new float; *(float*)value = 0.0f;  break;}
            case node_value_type_enum::Integer :{value = new int;   *(int *)value = 0;      break;}
            case node_value_type_enum::UInteger:{value = new unsigned int;*(unsigned int*)value = 0; break;}
            case node_value_type_enum::Vec2    :{value = new glm::vec2; *(glm::vec2*)value = { 0.0f,0.0f }; break; }
            case node_value_type_enum::Vec3    :{value = new glm::vec3; *(glm::vec3*)value = { 0.0f,0.0f,0.0f }; break; }
            case node_value_type_enum::Vec4    :{value = new glm::vec4; *(glm::vec4*)value = { 0.0f,0.0f,0.0f,0.0f }; break; }
            case node_value_type_enum::IVec2   :{value = new glm::ivec2; *(glm::ivec2*)value = { 0,0}; break; }
            case node_value_type_enum::IVec3   :{value = new glm::ivec3; *(glm::ivec3*)value = { 0,0,0 }; break; }
            case node_value_type_enum::IVec4   :{value = new glm::ivec4; *(glm::ivec4*)value = { 0,0,0,0 }; break; }
            case node_value_type_enum::Data_Ptr:{value = NULL; break; }
            
            //Add more data types here

            default: value = NULL;
        }


    }

};

//******************************************************************************************


class node_basis_class; // Forward declaration to be used in UI_Node_struct_type
struct UI_Node_struct_type{
    ~UI_Node_struct_type() {
        inputs.clear();
        inputs.shrink_to_fit();
        //delete node_ptr; // This creates an assertion error crash !!!!!
    }

    node_type_struct ui_node_type;
    // The identifying id of the ui node. For add, multiply, sine, and time
    // this is the "operation" node id. The additional input nodes are
    // stored in the structs.
    int     node_id = -1;
    id_type entity_id;

    // place variable data types below in derived structure types that
    // define the input values that are to be stored and used within the 
    // node to be used by internal node functions to give an output value(s)
    // eg
    //
    //  int lhs,rhs;
    // etc

    std::vector<int> inputs; // inputs are the id defined by graph of a stored value of type node_value_struct_type 
    int              output; // Apparently only one output permitted for ImNodes implementation and value is the node_id

    node_basis_class *node_ptr = NULL;//******

    std::vector<int> inputs_node_id; // Had to add to be able to store and retrieve node id on inputs
};

// A consideration of merging the above UI_Node_struct_type with node_basis_class needs to be considered
// since much of the operations of node functioning is envisaged as needing data from both to opperate
// Thus all references to UI_Node_struct_type becomes node_basis_class outside this class and all references
// to UI_Node_struct_type ui_node within this class are to be removed.
// An attempt to do this failed as for some reason as
// if (graph_.node(edge.from).node_type.function != VALUE) 
// caused a crash for an unknown reason even though graph_ had the edge.id, edge.from and edge.to
// correctly defined and stored. Thus have to do it the hard way. 
class node_basis_class {
public:
    node_basis_class() {}
    ~node_basis_class() {}

    std::string label   = "node";  

    float node_width  = 100.f;
    float node_height = 0.0f;

    ImNodes::Graph<node_value_struct_type>  *graph = NULL;//*****
    std::vector<UI_Node_struct_type>        *nodes = NULL;//*****
    int                                     root_node_id_;//*****
    int                                     node_id;     //*****

    //UI_Node_struct_type ui_node; // This does not work as the way ImNodes is implemented
                                   // it seems that creating a vector of classes to store
                                   // the node ui_node data crashes the program.

    virtual bool define_node(ImVec2 click_pos,
                             id_type  entity_id,
                             //int      function_type,
                             ImNodes::Graph<node_value_struct_type> &graph_, 
                             std::vector<UI_Node_struct_type>       &nodes_)  = 0;

    virtual bool draw_node(UI_Node_struct_type                    ui_node,
                           ImNodes::Graph<node_value_struct_type> graph_)     = 0;
    // must have 
    // ImNodes::BeginNode(node.node_id);
    // < draw node code >
    // ImNodes::EndNode();
 
    virtual void display_ui(scene_manager_class *scene_manager = NULL, log_panel_class *log_panel = NULL, id_type *current_selected_object_id = NULL){}
    
    virtual void action(){}

    virtual void update() {}

    virtual void on_link_connection() {}
    
    virtual void on_link_disconnection() {}

protected:
    // *********
 

// !!!!!!!!!!!!!!!!!!! DEFINE NODE INPUTS FUNCTIONS !!!!!!!!!!!!!!!!!!!!!!!!!
    void define_node_input_value_of_type(std::string label,
                                         int category_type,
                                         int context_type,
                                         int function_category_type,
                                         node_value_type_enum node_value_type, 
                                         ImNodes::Graph<node_value_struct_type>& graph_,
                                         UI_Node_struct_type &ui_node)
    {
        int value_id;
        node_type_struct node_type;

        node_type.category = category_type;
        node_type.context  = context_type;
        node_type.function_category  = function_category_type;
        node_type.function = VALUE;
        node_value_struct_type value(node_type, node_value_type);
        value.label = label;
        value_id = graph_.insert_node(value);
        ui_node.inputs.push_back(value_id);

        ui_node.inputs_node_id.push_back(-1);
    }

    bool define_node_of_type(std::string label, 
                             int category_type,
                             int context_type,
                             int function_category_type,
                             int funct_type,
                             id_type  entity_id,
                             ImNodes::Graph<node_value_struct_type>& graph_,
                             UI_Node_struct_type &ui_node)
    {
        // Aparently this node ImNodes implementation may only allow one output
        // of data. Need to investigate if this is the case and if more than one
        // output data stream possible or neccessary.

        node_type_struct function_type;
        function_type.category = category_type;
        function_type.context  = context_type;
        function_type.function_category = function_category_type;
        function_type.function = funct_type;
        node_value_struct_type op(function_type);
        op.label = label;
        ui_node.ui_node_type.category = category_type;
        ui_node.ui_node_type.context  = context_type;
        ui_node.ui_node_type.function_category = function_category_type;
        ui_node.ui_node_type.function = funct_type;
        ui_node.entity_id = entity_id;
        ui_node.node_id   = graph_.insert_node(op);
//printf("node_basis_class::define_node_of_type 00000::  %i \n", ui_node.node_id);
        ui_node.output = ui_node.node_id;

        node_id = ui_node.node_id;// remove if merge node_basis_class with UI_Node_struct_type
//printf("node_basis_class::define_node_of_type 11111:: %i \n", ui_node.node_id);       
        for (int input_id : ui_node.inputs) {
//printf("node_basis_class::define_node_of_type 22222::  %i : %i\n", ui_node.node_id, input_id);
            graph_.node(input_id).node_id = node_id;
        }
//printf("node_basis_class::define_node_of_type 33333::  %i : %i\n", ui_node.node_id); 
        graph_.node(ui_node.output).node_id = node_id;

        return true;
    }

    void define_input_edges(ImNodes::Graph<node_value_struct_type>& graph_,
                            UI_Node_struct_type ui_node) {
        for (int input_variable : ui_node.inputs) {
//printf("node_basis_class::define_input_edges:: %i | %i\n", ui_node.node_id, input_variable);
            graph_.insert_edge(ui_node.node_id, input_variable);
        }
    }

    void append_node_to_editor(ImVec2 click_pos,
                               std::vector<UI_Node_struct_type>& nodes_,
                               UI_Node_struct_type &ui_node) {
        nodes_.push_back(ui_node); // add the ui node data for this node to the list of editor nodes

        ImNodes::SetNodeScreenSpacePos(ui_node.node_id, click_pos);
    }
    // ^^^^^^^^^^^^^^^^^ END DEFINE NODE INPUTS FUNCTIONS ^^^^^^^^^^^^^^^^^

    // !!!!!!!!!!!!!!!!!!! DRAW NODE FUNCTIONS !!!!!!!!!!!!!!!!!!!

    void draw_node_header(std::string header_label) {
        ImNodes::BeginNodeTitleBar();
            int title_width = ImGui::CalcTextSize(label.c_str()).x;
            ImGui::TextUnformatted(header_label.c_str());
            if (title_width > node_width) node_width = title_width;
        ImNodes::EndNodeTitleBar();
    }

    void draw_node_header() {
        ImNodes::BeginNodeTitleBar();
            int title_width =  ImGui::CalcTextSize(label.c_str()).x;
            ImGui::TextUnformatted(label.c_str());
            if (title_width > node_width) node_width = title_width;
        ImNodes::EndNodeTitleBar();
    }

    void draw_node_input_link(int input_link_id,
                              ImNodes::Graph<node_value_struct_type>graph_,
                              ImNodesPinShape pin_shape = ImNodesPinShape_::ImNodesPinShape_CircleFilled) {

        ImNodes::BeginInputAttribute(input_link_id, pin_shape);
        std::string label = graph_.node(input_link_id).label;

        float label_width = ImGui::CalcTextSize(label.c_str()).x;
        ImGui::TextUnformatted(label.c_str());

        if (graph_.num_edges_from_node(input_link_id) == 0ull)
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(node_width - label_width);
            //ImGui::Text(label.c_str());

            //ImGui::DragFloat("##hidelabel", &graph_.node(node.ui.add.lhs).value, 0.01f);

            // need a function here to define what iput widget is to be defined to have the link
            // value assigned to and the display of that widget.
            // since for virtual worlds all inputs are to be displayed in a seperate widget
            // no widget display within the node itself.
            ImGui::PopItemWidth();
        }

        ImNodes::EndInputAttribute();
    }

    void draw_node_input_links(ImNodes::Graph<node_value_struct_type> graph_, UI_Node_struct_type ui_node) {
        for (int input_link_id : ui_node.inputs) {
//printf("node_basis_class::draw_node_input_links:: AAA %i\n", input_link_id);
            draw_node_input_link(input_link_id, graph_);
        }
    }

    void draw_node_output_link(int node_id,std::string output_label,
                               ImNodes::Graph<node_value_struct_type>graph_, UI_Node_struct_type ui_node,
                               ImNodesPinShape pin_shape = ImNodesPinShape_::ImNodesPinShape_CircleFilled) {
        //std::string label;
        //label_width;
        ImNodes::BeginOutputAttribute(node_id, pin_shape);
            //label = graph_.node(ui_node.output).label;
            //label_width = ImGui::CalcTextSize(label.c_str()).x;
            float label_width = ImGui::CalcTextSize(output_label.c_str()).x;
           
            ImGui::Indent(node_width - label_width);
            //ImGui::TextUnformatted(label.c_str());
            ImGui::TextUnformatted(output_label.c_str());
        ImNodes::EndOutputAttribute();
    }

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ END DRAW NODE FUNCTIONS ^^^^^^^^^^^^^^^^^^^^^^^^^^
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int find_node(int node_to_find_id,UI_Node_struct_type &ui_node) {
        int i = 0;
        for (UI_Node_struct_type node : *nodes) {
            if (node.node_id == node_to_find_id) {
                ui_node = node;
                return i;
                break;
            }
            i++;
        }

        return -1;
    }
};
