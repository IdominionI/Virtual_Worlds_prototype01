#pragma once

/*
	An ID key manager class to manage assigning a unique ID number to what ever C++ data class model requires one.

	Unique ID keys are maintained by assigning a new ID number as an increment of the current existing ID number
	if no ID key number exists in a list of previously used ID keys that have been stored in a vector free_id_keys
	and are available to be reassigned.
*/
#include <vector>

// Note that the type for this class can only be of one of the integer data types
// A template is used just to make things easier : initial values and datatype assumes 
// unsigned integer is the data type for T

template <class T>
class id_key_manager_class {
public:

	// If data type T is not of type unsigned int, not setting these values to a min and max values within that data type
	// range can cause problems of return values being of the incorrect values eg if T is of type int then initial value
	// returned of next_available_id_key will be -1 for the settings below
	T min_key_value     = 0;
	T max_key_value     = UINT_MAX;
	T invalid_key_value = UINT_MAX;

	std::vector<T> free_id_keys;  // Vector to store id keys previously used and can be reassigned

	// Get a unique ID key number that exists within this class
	T get_available_id_key() {
		T id_key = max_key_value;
		if (free_id_keys.size() > 0) {                   // Have a previous used ID key that can be reassigned
			id_key = free_id_keys[0];                    // get previous used ID key that can be reassigned
			free_id_keys.erase(free_id_keys.begin());    // delete previous used ID key as it is now reassigned
		}
		else {										 // Have no previous used ID key that can be reassigned
			if (next_available_id_key < max_key_value) {	 // Get the next new ID key
				id_key = next_available_id_key;
				next_available_id_key++;				 // Assign the next new ID key that is to be assigned
			}
		}

		return id_key;
	}

	// If need to find what the next ID key value is without it being assigned
	T get_next_available_id_key() {
		T id_key = max_key_value;
		if (free_id_keys.size() > 0) {
			id_key = free_id_keys[0];
		}
		else {
			if (next_available_id_key < max_key_value)
				id_key = next_available_id_key;
		}

		return id_key;
	}

	// If an ID key is no longer used
	void assign_free_id_key(T unused_free_id_key) {
		free_id_keys.push_back(unused_free_id_key);
	}

	// If a deletion or reassignment of all ID keys is performed 
	void reinitialise() {
		free_id_keys.clear();
		next_available_id_key = min_key_value;
	}

private:
	T next_available_id_key = 0; // next new ID key that is to be assigned
};


//  Management of a negative id key type where new key ids are a decrement of the currently
//  newest used negative id key

class neg_id_key_manager_class {
public:

	// If data type T is not of type unsigned int, not setting these values to a min and max values within that data type
	// range can cause problems of return values being of the incorrect values eg if T is of type int then initial value
	// returned of next_available_id_key will be -1 for the settings below
	int min_key_value     = INT_MIN;
	int max_key_value     = -1;
	int invalid_key_value = INT_MIN;

	std::vector<int> free_id_keys;  // Vector to store id keys previously used and can be reassigned

	// Get a unique ID key number that exists within this class
	int get_available_id_key() {
		int id_key = max_key_value;
		if (free_id_keys.size() > 0) {                   // Have a previous used ID key that can be reassigned
			id_key = free_id_keys[0];                    // get previous used ID key that can be reassigned
			free_id_keys.erase(free_id_keys.begin());    // delete previous used ID key as it is now reassigned
		}
		else {										 // Have no previous used ID key that can be reassigned
			if (next_available_id_key > min_key_value) {	 // Get the next new ID key
				id_key = next_available_id_key;
				next_available_id_key--;				 // Assign the next new ID key that is to be assigned
			}
		}

		return id_key;
	}

	// If need to find what the next ID key value is without it being assigned
	int get_next_available_id_key() {
		int id_key = max_key_value;
		if (free_id_keys.size() > 0) {
			id_key = free_id_keys[0];
		}
		else {
			if (next_available_id_key > min_key_value)
				id_key = next_available_id_key;
		}

		return id_key;
	}

	// If an ID key is no longer used
	void assign_free_id_key(int unused_free_id_key) {
		free_id_keys.push_back(unused_free_id_key);
	}

	// If a deletion or reassignment of all ID keys is performed 
	void reinitialise() {
		free_id_keys.clear();
		next_available_id_key = max_key_value;
	}

//private:
	int next_available_id_key = -1; // next new ID key that is to be assigned
};