#include "CelestialObject.h"
#include <iostream>

// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here
    max_active_col = shape[0].size();
    left_tick = time_of_appearance;
    for(const auto& i:shape) {
        for(const auto b:i) {
            if(b == true) {
                particle_count++;
            }
        }
    }
    max_particle_count = particle_count;
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
    this->max_active_col = other->max_active_col;
    this->left_tick = other->left_tick;
    this->active_col = other->active_col;
    this->left_top_col = other->left_top_col;
    this->particle_count = other->particle_count;
    this->max_particle_count = other->max_particle_count;
    this->last_hit = other->last_hit;
    this->destroy = other->destroy;
    this->destroy_checker = other->destroy_checker;

    right_rotation = nullptr;
    left_rotation = nullptr;
    next_celestial_object = nullptr;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    if(target == nullptr) {
        return;
    }
    auto* temp = target->left_rotation;
    while (temp != nullptr && temp != target)
    {
        auto* next = temp->left_rotation;
        temp->left_rotation = nullptr;
        temp->right_rotation = nullptr;
        delete temp;
        temp = next;
    }
    target->left_rotation = nullptr;
    target->right_rotation = nullptr;
}

