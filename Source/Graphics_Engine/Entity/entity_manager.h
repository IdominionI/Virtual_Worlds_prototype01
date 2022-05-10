#pragma once

#include <vector>

#include "../Common/id_key_manager.h"
#include "entity.h"

//template <class T>
typedef std::vector<entity_class>  entities_type; // Universal type definition for the list of entities in the ECS data base model

//template <class T>
class entities_manager_class {
public:

	idu_type entity_category_id = NULL_ENTITY_ID; // Unique ID number of category data type of this entity
	//set_category_class<index_type>* entity_sets = NULL; // Set Class to store entities and entity status
	entities_type  entities;           // Storage of list of ECS entities
	//vector<entity_class<T>> entities;           // Storage of list of ECS entities

	//category_map_class              entity_categories;  // Class to store, manage and reference entity data types

	// Get an unused unique entity id number to assign to an entity
	idu_type next_entity_id() {
		return entity_id_key.get_next_available_id_key();
	}

	// asign an entity a unique id number and add to the list of entities in the ECS
	void add_new_entity(entity_class entity ) {
		entity.entity_id = entity_id_key.get_available_id_key();
		entities.push_back(entity);
	}

	// Create and add a new entity of entity data type entity_category_name, with name entity_name and description entity_description
	//entity_class* add_new_entity(string_type entity_name, entity_category_name_type entity_category_name, string_type entity_description) {
	//	return add_new_entity(entity_name, category_data.category_id, entity_description);
		
		//category_struct_type category_data;


		//if (entity_categories.get_category(entity_category_name, category_data)) {
			//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 0000 " + entity_category_name + " : " + QString::number(category_data.category_id) + " : " + category_data.category_name, QMessageBox::Ok);
			//return add_new_entity(entity_name, category_data.category_id, entity_description);
		//}
		//else {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Cannot add new entity " + entity_name + "\nEntity category " + entity_category_name + " not defined.", QMessageBox::Ok);
		//	return NULL;
		//}
	//}

	// Create and add a new entity of entity data category id entity_category_id, with name entity_name and description entity_description
	entity_class* add_new_entity(string_type entity_name, idu_type entity_category_id, string_type entity_description) {
		entity_class entity;

		if (have_duplicate_entity(entity_name, entity_category_id)) {
			//QMessageBox::information(NULL, "entities_manager_class", "Error : Cannot add a new entity as have an entity defined with the same name.", QMessageBox::Ok);
			return NULL;
		}
		//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 22222", QMessageBox::Ok);
		//if (entity_sets == NULL) return NULL; // Entity sets not defined 
//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 3333", QMessageBox::Ok);
		//set_class<set_index_type>* live_set = entity_sets->get_set_of_name(DEFAULT_SET_LIVE_NAME); // Get set of all existing entities
		//if (live_set == NULL) {
		//	QMessageBox::information(NULL, "entities_manager_class", "ERROR : Cannot add new entity " + entity_name + "\nSet to store entities in the scne could not be found.", QMessageBox::Ok);
		//	return NULL;
		//}

		//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 44444", QMessageBox::Ok);
				// define attributes of entity to define and add to list of all entities
		entity.entity_name = entity_name;
		entity.entity_category_id = entity_category_id;
		entity.entity_description = entity_description;
		add_new_entity(entity);
		//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 5555", QMessageBox::Ok);

		//if (!add_to_entity_category_set(entities.size() - 1, entity_category_id)) {
		//	entities.erase(entities.begin() + entities.size() - 1);
			return NULL;
		//}
		//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 66666", QMessageBox::Ok);

				// !!! MAY BE BETTER TO HAVE ENTITY ID STORED RATHER THAN VECTOR ARRAY INDEX
				// !!!  TO AVOID CONFUSION AND SIMPLIFY CODE AND MAINTENANCE 
		//live_set->add_to_set(entities.size() - 1);
		//QMessageBox::information(NULL, "", "entities_manager_class :: add_new_entity 7777 " + QString::number(live_set->set_elements.size()) + " : " + QString::number(entities.size() - 1), QMessageBox::Ok);
		return &entities[entities.size() - 1];
	}

	// ###################################################

	void clear_entities() {
		entities.clear();
		entities.shrink_to_fit();
		entity_id_key.reinitialise();
	}

	// Delete entity of ID number entity_id from the list of entities
	// Entity components must be deleted prior to and outside this function
	bool delete_entity(idu_type entity_id) {
		index_type index, last = entities.size() - 1;

		index = find_entity(entity_id); // find index of entity in the entities QVector list
//QMessageBox::information(NULL, "", "Here 000 : " + QString::number(index) + " : " + QString::number(entity_id) + " : " + QString::number(last), QMessageBox::Ok);
		if (index == NULL_ENTITY_ID) {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Entity of id " + QString::number(entity_id) + " could not be found to be deleted.", QMessageBox::Ok);
			return false;
		}

		// Delete entity index reference from all the entity sets
		//delete_entity_set_members(index, DEFAULT_SET_LIVE_NAME);// QMessageBox::information(NULL, "", "Here 005A", QMessageBox::Ok);
		//delete_entity_set_members(index, DEFAULT_SET_SELECTED_NAME);// QMessageBox::information(NULL, "", "Here 005A", QMessageBox::Ok);
		//delete_entity_set_members(index, DEFAULT_SET_HIDDEN_NAME);// QMessageBox::information(NULL, "", "Here 005A", QMessageBox::Ok);
		//delete_entity_set_members(index, DEFAULT_SET_ACTIVE_NAME);// QMessageBox::information(NULL, "", "Here 005A", QMessageBox::Ok);

//QMessageBox::information(NULL, "", "Here 002 : " + QString::number(index) + " : " + QString::number(last), QMessageBox::Ok);

		// Reconfigure the QVector entities list to cater for deletion of entity at QVector index locaction
		// If group not at end of vector array copy last group array index to current vector array index
		// and if last group index number is in any of the group sets, replace that last array index value
		// in these sets with the group vector current index value. 
		if (index < last) {
			//QMessageBox::information(NULL, "", "Here 003 :" + QString::number(entities[index].entity_id) + " : " + QString::number(entities[last].entity_id), QMessageBox::Ok);
			entities[index].entity_id          = entities[last].entity_id; //QMessageBox::information(NULL, "", "Here 007B", QMessageBox::Ok);
			entities[index].entity_category_id = entities[last].entity_category_id; //QMessageBox::information(NULL, "", "Here 007C", QMessageBox::Ok);
			entities[index].entity_name        = entities[last].entity_name; //QMessageBox::information(NULL, "", "Here 007D", QMessageBox::Ok);
			entities[index].entity_description = entities[last].entity_description;

			entities[index].entity_object_class = entities[last].entity_object_class;
			//QMessageBox::information(NULL, "", "Here 003AA :" + QString::number(entities[index].entity_id) + " : " + QString::number(entities[last].entity_id), QMessageBox::Ok);
/*			set_index_type set_index = 0;

			set_class<index_type>* live_set = get_entity_set(DEFAULT_SET_LIVE_NAME);
			if (live_set->find_set_element(last, set_index))
				live_set->set_elements[set_index] = index;

			set_class<index_type>* selected_set = get_entity_set(DEFAULT_SET_SELECTED_NAME);
			if (selected_set->find_set_element(last, set_index))
				selected_set->set_elements[set_index] = index;

			set_class<index_type>* hidden_set = get_entity_set(DEFAULT_SET_HIDDEN_NAME);
			if (hidden_set->find_set_element(last, set_index))
				hidden_set->set_elements[set_index] = index;

			set_class<index_type>* active_set = get_entity_set(DEFAULT_SET_ACTIVE_NAME);
			if (active_set->find_set_element(last, set_index))
				active_set->set_elements[set_index] = index;
*/

			//QString s = "";
			//for (int i = 0; i < live_set->set_elements.size(); i++) {
			//	s = s + QString::number(live_set->set_elements[i]) + " : ";
			//}
			//QMessageBox::information(NULL, "", "live_set : " + s, QMessageBox::Ok);


			//s = "";
			//for (int i = 0; i < hidden_set->set_elements.size(); i++) {
			//	s = s + QString::number(hidden_set->set_elements[i]) + " : ";
			//}
			//QMessageBox::information(NULL, "", "hidden_set : " + s, QMessageBox::Ok);
		}


		//QMessageBox::information(NULL, "", "Here 004 : " + QString::number(last) + " : " + QString::number(entities.size()), QMessageBox::Ok);
		//
		//QString s = "";
		//for (int i = 0; i < entities.size(); i++) {
		//	s = s + QString::number(entities[i].entity_id) + " : ";
		//}
		//QMessageBox::information(NULL, "", "entities : " + s + "Last : " + QString::number(entities[last].entity_id), QMessageBox::Ok);

		entities.erase(entities.begin() + last);

		//QMessageBox::information(NULL, "", "Here 005", QMessageBox::Ok);
		//s = "";
		//for (int i = 0; i < entities.size(); i++) {
		//	s = s + QString::number(entities[i].entity_id) + " : ";
		//}
		//if (entities.size() > 0)QMessageBox::information(NULL, "", "entities after del : " + s + "Last : " + QString::number(entities[entities.size() - 1].entity_id), QMessageBox::Ok);

		entity_id_key.assign_free_id_key(entity_id);// Free entity ID number to be reused when a new entity is created

		return true;
	}

	// Delete entity of entity category data type id entity_category_id and name entity_name from list of entities 
	bool delete_entity(string_type entity_name, idu_type entity_category_id) {
		idu_type entity_id = find_entity(entity_name, entity_category_id);

		if (entity_id == NULL_ENTITY_ID) {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Entity of id " + QString::number(entity_id) + " could not be found to be deleted.", QMessageBox::Ok);
			return false;
		}

		//category_struct_type category_data;

		//if (entity_categories.get_category(entity_category_id, category_data)) {
		//	set_class<idu_type>* set = entity_sets->get_set_of_name(category_data.category_name);
		//	set->remove_from_set(entity_id);
		//}

		entities.erase(entities.begin() + entity_id);

		return true;
	}

	// Delete entity of entity category data type name entity_category_name and name entity_name from list of entities 
	//bool delete_entity(string_type entity_name, string_type entity_category_name) {
		//category_struct_type category_data;
		//if (!entity_categories.get_category(entity_category_name, category_data)) {
		//	QMessageBox::information(NULL, "entities_manager_class", "ERROR : Unable to find entity " + entity_name + " category " + entity_category_name + " to delete entity", QMessageBox::Ok);
		//	return false;
		//}

	//	delete_entity(entity_name, category_data.category_id);

	//	return true;
	//}

	// ###################################################

	entity_class* get_entity_of_id(idu_type entity_id) {
		index_type entity_index = find_entity(entity_id);
		if (entity_index == NULL_ENTITY_ID)
			return NULL;
		else
			return &entities[entity_index];
	}

	// Get pointer to entity of entity category data type id entity_category_id and name entity_name from list of entities 
	entity_class* get_entity_of_name_category(string_type entity_name, idu_type entity_category_id) {
		index_type entity_index = find_entity(entity_name, entity_category_id);
		if (entity_index == NULL_ENTITY_ID)
			return NULL;
		else
			return &entities[entity_index];
	}

	// ############################################################################
	// rename entity of entity category data type id entity_category_id and entity id entity_id to new_name
	bool rename_entity(idu_type entity_id, string_type new_name, idu_type entity_category_id) {
		if (have_duplicate_entity(new_name, entity_category_id)) {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Unable to to rename entity to " + new_name + " as have entity of same name ", QMessageBox::Ok);
			return false;
		}

		entity_class* entity = get_entity_of_id(entity_id);
		if (entity == NULL) {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Unable to find entity to rename", QMessageBox::Ok);
			return false;
		}
		entity->entity_name = new_name;

		return true;
	}

	// Assign description entity of entity id to entity_description
	bool set_entity_description(idu_type entity_id, string_type entity_description) {
		entity_class* entity = get_entity_of_id(entity_id);
		if (entity == NULL) {
			//QMessageBox::information(NULL, "entities_manager_class", "ERROR : Unable to find entity to set new entity description.", QMessageBox::Ok);
			return false;
		}
		entity->entity_description = entity_description;

		return true;
	}

	// ############################################################################

	// Return if have entity of same entity name entity_name for entity of category data type id entity_category_id
	bool have_duplicate_entity(string_type entity_name, idu_type entity_category_id) {
		if (find_entity(entity_name, entity_category_id) == NULL_ENTITY_ID)
			return false;

		return true;
	}

	// Return index in QVector entities list entity name entity_name for entity of category data type id entity_category_id
	index_type find_entity(string_type entity_name, idu_type entity_category_id) {
		for (index_type i = 0; i < entities.size(); i++) {
			if (entities[i].entity_name == entity_name && entities[i].entity_category_id == entity_category_id)
				return i;
		}
		return NULL_ENTITY_ID;
	}

	// Return index in QVector entities list entity name entity_name for entity of category data type name entity_category_name
	//index_type find_entity(string_type entity_name, string_type entity_category_name) {
		//category_struct_type category_data; // Structure data type variable to pass as a reference variable to retrieve entity data type or category information
		//if (!entity_categories.get_category(entity_category_name, category_data)) return NULL_ENTITY_ID;

	//	return find_entity(entity_name, category_data.category_id);
	//}

	// Return index in QVector entities list of entity of id number entity_id
	index_type find_entity(idu_type entity_id) {
		for (index_type i = 0; i < entities.size(); i++) {
			if (entities[i].entity_id == entity_id)
				return i;
		}
		return NULL_ENTITY_ID;
	}

	// Get pointer to set of set name set_name
	//set_class<index_type>* get_entity_set(set_name_type set_name) {
	//	return entity_sets->get_set_of_name(set_name);
	//}

	// !!!!!!!!!!!!!! SET OPERATIONS !!!!!!!!!!!!!!!!

	//bool initialise_entity_sets() {
	//	entity_sets = new set_category_class<index_type>;
	//	if (entity_sets == NULL) return false;

	//	return define_entity_sets();
	//}


private:
	id_key_manager_class<idu_type> entity_id_key;

	idu_type              next_available_entity_id = 0;
	std::vector<idu_type> free_entity_id_list;

	// Get a free or new entity id value to assign to a new entity
	idu_type get_entity_id() {
		idu_type entity_id = 0;
		if (free_entity_id_list.size() > 0) {    // Have a previously used ID value that can be reused
			entity_id = free_entity_id_list[0];  // get first free ID value in list of free ID values
			free_entity_id_list.erase(free_entity_id_list.begin()); // delete first free ID value in list of free ID values
		}
		else {
			entity_id = next_available_entity_id; // get next available unused ID value
			next_available_entity_id++;           // increment next available unused ID value
		}

		return entity_id; // return available entity ID value
	}

};