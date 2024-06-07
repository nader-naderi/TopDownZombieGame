// This file declares the ActionMap class template, 
// which stores a mapping of actions associated with specific keys or buttons. 

#pragma once
#include <unordered_Map>
#include "Action.h"

template<typename T = int>
class ActionMap
{
public:
    // ActionMap Default Constructor: The default constructor for the
    // ActionMap class is explicitly defaulted to the compiler-generated version.
    ActionMap(const ActionMap<T>&) = delete;
    ActionMap<T>& operator=(const ActionMap<T>&) = delete;

    ActionMap() = default;

    // This function adds a new mapping between a key/button and an Action to the ActionMap.
    void map(const T& key, const Action& action);

    //  This function retrieves the Action associated with a given key/button from the ActionMap.
    const Action& get(const T& key)const;

private:
    std::unordered_map<T, Action> _map;
};

#include "ActionMap.tpl"