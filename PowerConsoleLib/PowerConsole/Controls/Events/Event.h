/*=================================================================|
|* File:				Event.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <deque>
#include <functional>
#include <PowerConsole/Declarations.h>

/* Adds and binds the memeber function to an event listener with the same name as the member function. Requires: ClassName::FunctionName. */
#define bindEvent(eventName, ...) #eventName, bind(&eventName, this, std::placeholders::_1, std::placeholders::_2, __VA_ARGS__)
/* Gets the name of the event so it can be removed from the event. */
#define unbindEvent(eventName) #eventName

namespace PowerConsole::Controls {
DECLARE_STRUCT(EventArgs);
DECLARE_CLASS_AND_SMART_PTRS(Control);
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/

/* The direction to call event listeners in. */
enum class EventDirections {
	/* Event listeners registered first will be called first. */
	FirstInFirstCalled,
	/* Event listeners registered first will be called last. */
	FirstInLastCalled
};

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Event

template<class Args> struct EventListener {

	//=========== MEMBERS ============
	#pragma region Members

	/* The function type to be called by the event. */
	typedef std::function<void(ControlSPtr, Args&)> ArgsFunction;
	/* The name of the event listener. */
	std::string name;
	/* The function of the event listener. */
	ArgsFunction function;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs an event listener with a name and function. */
	EventListener(const std::string& name, ArgsFunction function)
		: name(name), function(function) {}

	#pragma endregion
};

/* An event that stores listeners and calls them when the event is raised. */
template<class Args> class Event {

	static_assert(std::is_base_of<EventArgs, Args>::value, "Event's Args class must be a descendant of EventArgs");

	//=========== TYPEDEFS ===========
	#pragma region Typedefs
public:
	/* The function type to be called by the event. */
	typedef std::function<void(ControlSPtr, Args&)> ArgsFunction;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The deque of listener functions to call when an event is raised. */
	std::deque<EventListener<Args>> listeners;
	/* The direction to call event listeners in. */
	EventDirections direction;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default event. */
	Event(EventDirections direction)
		: direction(direction) {}

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// Calling
	/* Calls all listeners registered to the event, but with default arguments. */
	void operator()(ControlSPtr sender) {
		Args e;
		(*this)(sender, e);
	}
	/* Calls all listeners registered to the event. */
	void operator()(ControlSPtr sender, Args e) {
		if (direction == EventDirections::FirstInFirstCalled) {
			for (auto it = listeners.begin(); it != listeners.end(); it++) {
				it->function(sender, e);
				if (e.handled)
					break;
			}
		}
		else {
			for (auto it = listeners.rbegin(); it != listeners.rend(); it++) {
				it->function(sender, e);
				if (e.handled)
					break;
			}
		}
	}

	#pragma endregion
	//========== LISTENERS ===========
	#pragma region Listeners

	// Registering & Removal
	/* Adds a listener to the event. */
	void add(const std::string& name, ArgsFunction function) {
		if (!name.empty()) {
			auto it = std::find_if(listeners.begin(), listeners.end(),
				[&name](const EventListener<Args>& listener) { return listener.name == name; }
			);
			if (it != listeners.end())
				throw std::exception("Event::add listener already exists in the event");
		}
		listeners.push_back(EventListener<Args>(name, function));
	}
	/* Removes a listener from the event. */
	void remove(const std::string& name) {
		if (name.empty())
			throw std::exception("Event::remove cannot remove an empty name");
		auto it = std::find_if(listeners.begin(), listeners.end(),
			[&name](const EventListener<Args>& listener) { return listener.name == name; }
		);
		if (it == listeners.end())
			throw std::exception("Event::remove listener does not exist in the event");
		listeners.erase(it);
	}

	// Anonymous Removal
	/* Removes the last registered listener from the event. */
	void pop() {
		if (listeners.size() == 0)
			throw std::exception("Event::pop() no listeners available to remove");
		listeners.pop_back();
	}
	/* Removes all listeners from the event. */
	void clear() {
		listeners.clear();
	}
	/* Returns true if the event listener with the specified name is contained in the list. */
	bool contains(const std::string& name) {
		return std::find_if(listeners.begin(), listeners.end(),
			[&name](const EventListener<Args>& listener) { return listener.name == name; }
		) != listeners.end();
	}

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the number of listeners attatched to the event. */
	int count() {
		return (int)listeners.size();
	}

	#pragma endregion
};

#pragma endregion
//--------------------------------
#pragma region EventCaller

/* A class for temporarily storing an event to call. */
template<class Args> class EventCaller {

	//=========== MEMBERS ============
	#pragma region Members

	/* The event to call. */
	Event<Args>* event;
	/* The sender of the event. */
	ControlWPtr sender;
	/* The event args to send. */
	Args eventArgs;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the event caller with all the information needed to execute the event. */
	EventCaller(Event<Args>* event, ControlWPtr sender, Args eventArgs)
		: event(event),
		sender(sender),
		eventArgs(eventArgs) {}

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Calls the event. */
	void operator()() {
		// No reason to send the event if the control doesn't exist anymore.
		if (!sender.expired()) {
			(*event)(sender.lock(), eventArgs);
		}
	}

	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
