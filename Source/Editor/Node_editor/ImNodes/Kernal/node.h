#pragma once

#include <vector>
#include <string>

#include <imgui.h>
#include <Universal/3rdparty/include/glm/glm.hpp>

#include "../../../Common/definitions.h"

#include "../imnodes.h"


//#define INVALID_ID  -1

// function category
#define GENERAL    -2
#define EVALUATOR  0
#define MODIFIER   1
#define GENERATOR  2
#define OPERATOR   3

struct node_type_struct {
    int category = -1;
    int context  = -1;
    int function_category = -1;
    int function = -1;
};



struct pin_value_type_struct {
	int category          = INVALID_ID;
    int context           = INVALID_ID;
	int function_category = INVALID_ID;
	int value_type        = INVALID_ID;
};

enum class pin_type_enum {single_input, single_output, multi_input, multi_output};

struct pin_struct_type {
    pin_type_enum         pin_type;
	pin_value_type_struct pin_value_type;

    std::string label = "Pin";

	int pin_id  = INVALID_ID;
	int node_id = INVALID_ID;
	//int link_id = INVALID_ID; not a good idea !!!! may have more than one link per pin

};


class pins_class {
public:
	std::vector<pin_struct_type> input_pins;
	std::vector<pin_struct_type> output_pins;

    int add_input_pin(pin_struct_type pin) {
        pin.pin_id  = pin_id_key.get_available_id_key();
        input_pins.push_back(pin);

        return pin.pin_id;
    }

    int add_output_pin(pin_struct_type pin) {
        pin.pin_id = pin_id_key.get_available_id_key();
        output_pins.push_back(pin);

        return pin.pin_id;
    }

    int get_input_pin_index(int input_pin_id) {
        for (int i = 0; i < input_pins.size();i++) {
            if (input_pins[i].pin_id == input_pin_id)
                return i;
        }
        return INVALID_ID;
    }

    int get_output_pin_index(int output_pin_id) {
        for (int i = 0; i < output_pins.size(); i++) {
            if (output_pins[i].pin_id == output_pin_id)
                return i;
        }
        return INVALID_ID;
    }

    pin_struct_type find_pin(int pin_id) {
        int i = get_input_pin_index(pin_id);
        if (i != INVALID_ID) return input_pins[i];

        i = get_output_pin_index(pin_id);
        if (i != INVALID_ID) return output_pins[i];
       
        pin_struct_type pin;
        return pin;
    }

    bool delete_input_pin(int input_pin_id) {
        int i = get_input_pin_index(input_pin_id);
        if (i == INVALID_ID) return false;

        pin_id_key.assign_free_id_key(input_pins[i].pin_id);
        input_pins.erase(input_pins.begin() + i);
        return true;
    }

    bool delete_output_pin(int output_pin_id) {
        int i = get_output_pin_index(output_pin_id);
        if (i == INVALID_ID) return false;

        pin_id_key.assign_free_id_key(output_pins[i].pin_id);
        output_pins.erase(output_pins.begin() + i);
        return true;
    }
protected:

private:
    id_key_manager_class<unsigned int> pin_id_key;
};

struct link_struct_type {
	int	link_id = INVALID_ID, from = -1, to = -1;

	//std::pair<int, int> link_pins;
    void swap(){
        int i = from;
        from = to;
        to = i;
    }
};

class links_class {
public:

	std::vector<link_struct_type > links;

    void insert(int start_pin_id,int end_pin_id) {
        link_struct_type link;
        link.link_id = link_id_key.get_available_id_key();
        link.from = start_pin_id;
        link.to   = end_pin_id;

        links.push_back(link);
    }

    bool delete_link(int link_id) {
        for (int i = 0; i < links.size(); i++) {
            if (links[i].link_id == link_id) {
                link_id_key.assign_free_id_key(links[i].link_id);
                links.erase(links.begin() + i);
                return true;
                break;
            }
        }
        return false;
    }

    link_struct_type find_link(int link_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.link_id == link_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    link_struct_type find_link_with_start_pin(int pin_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.from == pin_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    link_struct_type find_link_with_end_pin(int pin_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.to == pin_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    bool start_pin_has_link(int start_pin_id) {
        for (link_struct_type link : links) {
            if (link.from == start_pin_id) {
                return true;
                break;
            }
        }

        return false;
    }

    bool end_pin_has_link(int end_pin_id) {
        for (link_struct_type link : links) {
            if (link.to == end_pin_id) {
                return true;
                break;
            }
        }

        return false;
    }

protected:

private:
    id_key_manager_class<unsigned int> link_id_key;
};


class graph_class; // Forward declaration
class node_basis_class {
public:
    //explicit node_basis_class(graph_class *graph_) : graph(graph_) {} This creates a compile error
    ~node_basis_class() {
        //delete_node();
    }

    graph_class *graph = NULL;

    node_type_struct ui_node_type;
    // The identifying id of the ui node. For add, multiply, sine, and time
    // this is the "operation" node id. The additional input nodes are
    // stored in the structs.
    int     node_id   = INVALID_ID;
    id_type entity_id = INVALID_ID;

    std::vector<int> inputs;
    std::vector<int> outputs;

    std::string label = "node";

    float node_width = 100.f;
    float node_height = 0.0f;

    //UI_Node_struct_type ui_node; // This does not work as the way ImNodes is implemented
                                   // it seems that creating a vector of classes to store
                                   // the node ui_node data crashes the program.

    virtual bool define_node(ImVec2 click_pos, id_type  entity_id_) { return false; };
    virtual bool define_inputs(pins_class  &pins) { return false; };
    virtual bool define_outputs(pins_class &pins) { return false; };

    virtual void delete_node_entity() {};// since destructor creates compilation and other problems, this will need to be included

    virtual bool draw_node(pins_class pins) { return false; };
    // must have 
    // ImNodes::BeginNode(node.node_id);
    // < draw node code >
    // ImNodes::EndNode();

    virtual void display_ui(scene_manager_class *scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1) {}

    virtual void action() {}

    virtual void update() {}

    virtual void on_link_connection() {}

    virtual void on_link_disconnection() {}


    void delete_node_links(links_class &links) {
        for (int pin_id : inputs) {
            for (link_struct_type link : links.links) {
                if (link.to == pin_id) {
                    links.delete_link(link.link_id);
                }
            }
        }

        for (int pin_id : outputs) {
            for (link_struct_type link : links.links) {
                if (link.from == pin_id) {
                    links.delete_link(link.link_id);
                }
            }
        }
    }

    void  delete_node_pins(pins_class &pins) {
//printf("node_basis_class::delete_node_pins:: 000 %i \n", inputs.size());
        for (int pin_id : inputs) {
            pins.delete_input_pin(pin_id);
        }
//printf("node_basis_class::delete_node_pins:: 111 %i \n", outputs.size());
        for (int pin_id : outputs) {
            pins.delete_output_pin(pin_id);
        }
//printf("node_basis_class::delete_node_pins:: 222 \n");

        inputs.clear();
        outputs.clear();
    }

protected:
    // *********


// !!!!!!!!!!!!!!!!!!! DEFINE NODE INPUTS FUNCTIONS !!!!!!!!!!!!!!!!!!!!!!!!!
    bool define_node_input_value_of_type(std::string label,
                                         int category_type,
                                         int context_type,
                                         int function_category_type,
                                         int value_type,
                                         pin_type_enum pin_type,
                                         pins_class  &pins)
    {
        pin_struct_type pin;

        pin.label = label;
        pin.pin_type = pin_type;
        pin.pin_value_type.category           = category_type;
        pin.pin_value_type.context            = context_type;
        pin.pin_value_type.function_category  = function_category_type;
        pin.pin_value_type.value_type         = value_type;

        pin.node_id = node_id;

        pins.add_input_pin(pin);

        int i = pins.input_pins.size() - 1;
//printf("node_basis_class :: define_node_input_value_of_type 0000 %i : % i\n",i, pins.input_pins.size());
        if (i > INVALID_ID) {
            inputs.push_back(pins.input_pins[i].pin_id);
//printf("node_basis_class :: define_node_input_value_of_type iiiii %i : % i\n",i, pins.input_pins[i].pin_id);
            return true;
        }

        return false;
//printf("node_basis_class :: define_node_input_value_of_type 0000 %i : % i\n",i, pins.input_pins.size());
    }

    bool define_node_output_value_of_type(std::string label,
                                          int category_type,
                                          int context_type,
                                          int function_category_type,
                                          int value_type,
                                          pin_type_enum pin_type,
                                          pins_class  &pins)
    {
        pin_struct_type pin;

        pin.label = label;
        pin.pin_type = pin_type;
        pin.pin_value_type.category           = category_type;
        pin.pin_value_type.context            = context_type;
        pin.pin_value_type.function_category  = function_category_type;
        pin.pin_value_type.value_type         = value_type;

        pin.node_id = node_id;

        pins.add_output_pin(pin);

        int i = pins.output_pins.size() - 1;
//printf("node_basis_class :: define_node_output_value_of_type 0000 %i : % i\n",i, pins.input_pins.size());
        if (i > INVALID_ID) {
            outputs.push_back(pins.output_pins[i].pin_id);
//printf("node_basis_class :: define_node_output_value_of_type ooooo %i : % i\n",i, pins.output_pins[i].pin_id);
            return true;
        }

        return false;
//printf("node_basis_class :: define_node_output_value_of_type 0000 %i : % i\n",i, pins.input_pins.size());
    }



    bool define_node_of_type()
    {
        return true;
    }

    void define_input_edges() {
    }

    void append_node_to_editor(){ // add the ui node data for this node to the list of editor nodes

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
        int title_width = ImGui::CalcTextSize(label.c_str()).x;
        ImGui::TextUnformatted(label.c_str());
        if (title_width > node_width) node_width = title_width;
        ImNodes::EndNodeTitleBar();
    }

    void draw_node_input_link(int input_pin_id,
                              pins_class &pins,
                              ImNodesPinShape pin_shape = ImNodesPinShape_::ImNodesPinShape_CircleFilled)
    {
//printf("node_basis_class :: draw_node_input_link 0000\n");
        ImNodes::BeginInputAttribute(input_pin_id, pin_shape);
//printf("node_basis_class :: draw_node_input_link 1111  %i \n",pins.input_pins.size());
        int i = pins.get_input_pin_index(input_pin_id);
//printf("node_basis_class :: draw_node_input_link 2222 %i \n",i);
        if (i == INVALID_ID) return;
        std::string label = pins.input_pins[i].label;
//printf("node_basis_class :: draw_node_input_link 3333\n");
        float label_width = ImGui::CalcTextSize(label.c_str()).x;
        ImGui::TextUnformatted(label.c_str());

       // if (graph_.num_edges_from_node(input_link_id) == 0ull)
       // {
            ImGui::SameLine();
            ImGui::PushItemWidth(node_width - label_width);
            //ImGui::Text(label.c_str());

            //ImGui::DragFloat("##hidelabel", &graph_.node(node.ui.add.lhs).value, 0.01f);

            // need a function here to define what iput widget is to be defined to have the link
            // value assigned to and the display of that widget.
            // since for virtual worlds all inputs are to be displayed in a seperate widget
            // no widget display within the node itself.
            ImGui::PopItemWidth();
       // }
//printf("node_basis_class :: draw_node_input_link 44444\n");
        ImNodes::EndInputAttribute();

    }

    void draw_node_output_link(int output_pin_id,
                               pins_class &pins,
                               ImNodesPinShape pin_shape = ImNodesPinShape_::ImNodesPinShape_CircleFilled)
    {
//printf("node_basis_class :: draw_node_input_link 0000\n");
        ImNodes::BeginOutputAttribute(output_pin_id, pin_shape);
//printf("node_basis_class :: draw_node_input_link 1111  %i \n",pins.input_pins.size());
        int i = pins.get_output_pin_index(output_pin_id);
//printf("node_basis_class :: draw_node_input_link 2222 %i \n",i);
        if (i == INVALID_ID) return;
        std::string label = pins.output_pins[i].label;

        float label_width = ImGui::CalcTextSize(label.c_str()).x;
        ImGui::Indent(node_width - label_width);
        ImGui::TextUnformatted(label.c_str());

//printf("node_basis_class :: draw_node_input_link 44444\n");
        ImNodes::EndInputAttribute();

    }

    void draw_node_input_links() {
    }

    void draw_node_output_links() {

    }

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ END DRAW NODE FUNCTIONS ^^^^^^^^^^^^^^^^^^^^^^^^^^
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

};

//class node_basis_class;
class nodes_class {
public:
    std::vector<node_basis_class*> nodes;

    void insert(node_basis_class* node) {
        nodes.push_back(node);
    }

    int get_node_index(int node_id) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->node_id == node_id) {
                return i;
                break;
            }
        }

        return INVALID_ID;
    }

    node_basis_class* get_node(int node_id) {
        int index = get_node_index(node_id);
        if (index == INVALID_ID) return NULL;

        return nodes[index];
    }

    bool delete_node(int node_id) {
        int index = get_node_index(node_id);
        if (index == INVALID_ID) return false;
        nodes[index]->delete_node_entity();
        nodes.erase(nodes.begin() + index);

        return true;
    }


protected:

private:
};