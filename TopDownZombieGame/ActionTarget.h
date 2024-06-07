// The ActionTarget class template is designed to bind functions to specific
// actions and process those actions based on input events.

#ifndef ACTIONTARGET_H
#define ACTIONTARGET_H

#include "ActionMap.h"
#include<functional>
#include <utility>
#include <list>

template<typename T = int>
class ActionTarget
{
public:
    // The default constructor takes a reference to an ActionMap object and initializes the _actionMap member.
    ActionTarget(const ActionTarget<T>&) = delete;
    ActionTarget<T>& operator=(const ActionTarget<T>&) = delete;

    using FuncType = std::function<void(const sf::Event&)>;

    ActionTarget(const ActionMap<T>& map);

    // This function processes a single event by iterating through the list of bound functions for _eventsPoll
    // and calling the associated function if the event matches the action.
    bool processEvent(const sf::Event& event)const;

    // This function processes real-time actions by iterating through the list of bound functions for _eventsRealTime
    // and calling the associated function if the action's test function returns true.
    void processEvents()const;

    //  This function binds a function to a specific key or button in the ActionMap. 
    // It checks whether the action is real-time or poll-based and adds the function to the appropriate list.
    void bind(const T& key, const FuncType& callback);

    //  This function removes a function bound to a specific key or button in the ActionMap. 
    //  It checks whether the action is real-time or poll-based and removes the function from the appropriate list.
    void unbind(const T& key);

private:
    std::list<std::pair<T, FuncType>> _eventsRealTime;
    std::list<std::pair<T, FuncType>> _eventsPoll;

    const ActionMap<T>& _actionMap;
};

#include "ActionTarget.tpl"
#endif // ACTIONTARGET_H
