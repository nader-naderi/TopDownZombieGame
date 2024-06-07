// This file contains the implementation of the Action class.
// The Action class is responsible for representing and 
// managing different types of actions triggered by keyboard and mouse events.


#include "Action.h"

// Action Constructors: There are two constructors for the Action class, one for keyboard actions and one for mouse actions.
//  These constructors initialize the _type and _event members based on the input parameters.
Action::Action(const sf::Keyboard::Key& key, int type) : _type(type)
{
	_event.type = sf::Event::EventType::KeyPressed;
	_event.key.code = key;
}

Action::Action(const sf::Mouse::Button& button, int type) : _type(type)
{
	_event.type = sf::Event::EventType::MouseButtonPressed;
	_event.mouseButton.button = button;
}
/// <summary>
/// This overload compares an Action object with either another Action object or an sf::Event. 
/// It checks if the event types match and, in the case of KeyPressed or MouseButtonPressed events, 
/// compares the key or mouse button codes.
/// </summary>
/// <param name="event"></param>
/// <returns></returns>
bool Action::operator==(const sf::Event& event)const
{
	bool res = false;

	switch (event.type)
	{
	case sf::Event::EventType::KeyPressed:
	{
		if (_type & Type::Pressed and _event.type == sf::Event::EventType::KeyPressed)
			res = event.key.code == _event.key.code;
	}break;
	case sf::Event::EventType::KeyReleased:
	{
		if (_type & Type::Released and _event.type == sf::Event::EventType::KeyPressed)
			res = event.key.code == _event.key.code;
	}break;
	case sf::Event::EventType::MouseButtonPressed:
	{
		if (_type & Type::Pressed and _event.type == sf::Event::EventType::MouseButtonPressed)
			res = event.mouseButton.button == _event.mouseButton.button;
	}break;
	case sf::Event::EventType::MouseButtonReleased:
	{
		if (_type & Type::Released and _event.type == sf::Event::EventType::MouseButtonPressed)
			res = event.mouseButton.button == _event.mouseButton.button;
	}break;
	default: break;
	}
	return res;
}

bool Action::operator==(const Action& other)const
{
	return _type == other._type and other == _event;
}

/// <summary>
/// This function tests whether a specific Action is occurring in real-time by checking the current state of the keyboard or
/// mouse button associated with the action.
/// </summary>
/// <returns></returns>
bool Action::test()const
{
	bool res = false;
	if (_event.type == sf::Event::EventType::KeyPressed)
	{
		if (_type & Type::Pressed)
			res = sf::Keyboard::isKeyPressed(_event.key.code);
	}
	else if (_event.type == sf::Event::EventType::MouseButtonPressed)
	{
		if (_type & Type::Pressed)
			res = sf::Mouse::isButtonPressed(_event.mouseButton.button);
	}
	return res;
}