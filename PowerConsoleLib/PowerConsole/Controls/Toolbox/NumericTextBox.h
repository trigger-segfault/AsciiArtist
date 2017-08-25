/*=================================================================|
|* File:				NumericTextBox.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef INTEGER_TEXT_BOX_H
#define INTEGER_TEXT_BOX_H

#include <PowerConsole/Controls/Toolbox/TextBox.h>
#include <PowerConsole/Helpers/Parsing.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Helpers;
using namespace PowerConsole::Input;

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

template<typename T> class NumericTextBox : public TextBox {

	//=========== MEMBERS ============
	#pragma region Members
	
	// Settings
	/* The current value of the text box. */
	T value;
	/* The maximum value of the text box. */
	T maximum;
	/* The minimum value of the text box. */
	T minimum;
	/* The value to use when there is a parsing error. */
	T errorValue;
	/* The increment to use when pressing up or down. */
	T increment;
	/* The radix to display in the text box. */
	Radixes radix;

	// Events
	/* Called when the value has been changed. */
	Event<ValueChangedEventArgs<T>> _eventValueChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the numeric text box. */
	NumericTextBox()
	 :	// Settings
		value((T)0),
		maximum(std::numeric_limits<T>().max()),
		minimum(std::numeric_limits<T>().min()),
		errorValue((T)0),
		increment((T)1),

		// Events
		_eventValueChanged(EventDirections::FirstInFirstCalled) {

		eventInitialize().add(bindEvent(NumericTextBox::onInitialize));
		eventTextChanged().add(bindEvent(NumericTextBox::onTextChanged));
		eventTextInput().add(bindEvent(NumericTextBox::onTextInput));
		eventKeyTyped().add(bindEvent(NumericTextBox::onKeyTyped));
		eventFocusLost().add(bindEvent(NumericTextBox::onFocusLost));
		setTextAlignment(HorizontalAlignments::Right);
	}
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e) {
		updateTextBox();
	}


	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the current value of the text box. */
	T getValue() const {
		return value;
	}
	/* Sets the current value of the text box. */
	void setValue(T value) {
		if (this->value != value) {
			this->value = value;
			updateTextBox();
		}
	}
	/* Gets the maximum value of the text box. */
	T getMaximum() const {
		return maximum;
	}
	/* Sets the maximum value of the text box. */
	void setMaximum(T maximum) {
		this->maximum = maximum;
	}
	/* Gets the minimum value of the text box. */
	T getMinimum() const {
		return minimum;
	}
	/* Sets the minimum value of the text box. */
	void setMinimum(T minimum) {
		this->minimum = minimum;
	}
	/* Gets the value to use when there is a parsing error. */
	T getErrorValue() const {
		return errorValue;
	}
	/* Sets the value to use when there is a parsing error. */
	void setErrorValue(T errorValue) {
		this->errorValue = errorValue;
	}
	/* Gets the increment to use when pressing up or down. */
	T getIncrement() const {
		return increment;
	}
	/* Sets the increment to use when pressing up or down. */
	void setIncrement(T increment) {
		this->increment = increment;
	}

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the value has been changed. */
	Event<ValueChangedEventArgs<T>>& eventValueChanged() {
		return _eventValueChanged;
	}

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Updates changes to the value. */
	void updateTextBox() {
		int caretIndex = getCaretIndex();
		setText(std::to_string(value));
		setCaretIndex(caretIndex);
		updateTextBoxError();
	}
	/* Updates changes to the error state. */
	void updateTextBoxError() {
		bool error = false;
		try {
			T newValue = Parsing::parseAnyType<T>(getText());
			if (newValue > maximum || newValue < minimum)
				error = true;
		}
		catch (...) {
			error = true;
		}
		setTextColor(toColor(BLACK, error ? RED : GRAY));
		setSelectionColor(toColor(error ? DARK_RED : BLUE, WHITE));
	}

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTextInput(ControlSPtr sender, TextInputEventArgs& e) {
		e.handled = true;

		T oldValue = value;
		bool invalidChar = false;
		for (int i = 0; i < (int)e.input.length() && !invalidChar; i++) {
			char c = e.input[i];
			if ((c < '0' || c > '9') && (c != '-' || getSelectionStart() != 0 || i > 0 || minimum >= (T)0))
				invalidChar = true;
		}
		if (!invalidChar) {
			replaceText(e.index, e.length, e.input);
			try {
				value = Parsing::parseAnyType<T>(getText());
				if (value > maximum) {
					value = maximum;
					updateTextBox();
					setCaretIndex(getLength());
				}
				else if (value < minimum) {
					value = minimum;
				}
			}
			catch (const std::overflow_error&) {
				if (getLength() > 0 && getText()[0] == '-') {
					// Underflow
					value = minimum;
					updateTextBox();
					setCaretIndex(getLength());
				}
				else {
					value = maximum;
					updateTextBox();
					setCaretIndex(getLength());
				}
			}
			catch (const std::invalid_argument&) {
				if (getText() == "-" || getText() == "") {
					// Don't worry, the user is just writing a negative number or typing a new number
					value = errorValue;
				}
				else {
					// Shouldn't happen?
					value = errorValue;
					updateTextBox();
					setCaretIndex(getLength());
				}
			}
			if (value != oldValue) {
				_eventValueChanged(shared_from_this(), ValueChangedEventArgs<T>(value, oldValue));
			}
		}
		updateTextBoxError();
	}
	void onTextChanged(ControlSPtr sender, TextChangedEventArgs& e) {
		T oldValue = value;
		try {
			value = Parsing::parseAnyType<T>(getText());
			if (value > maximum) {
				value = maximum;
				updateTextBox();
				setCaretIndex(getLength());
			}
			else if (value < minimum) {
				value = minimum;
			}
		}
		catch (std::overflow_error&) {
			if (getLength() > 0 && getText()[0] == '-') {
				// Underflow
				value = minimum;
				updateTextBox();
				setCaretIndex(getLength());
			}
			else {
				value = maximum;
				updateTextBox();
				setCaretIndex(getLength());
			}
		}
		catch (std::invalid_argument&) {
			if (getText() == "-" || getText() == "") {
				// Don't worry, the user is just writing a negative number or typing a new number
				value = errorValue;
			}
			else {
				// Shouldn't happen?
				value = errorValue;
				updateTextBox();
				setCaretIndex(getLength());
			}
		}
		if (value != oldValue) {
			_eventValueChanged(shared_from_this(), ValueChangedEventArgs<T>(value, oldValue));
		}
		updateTextBoxError();
	}
	void onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e) {
		T oldValue = value;
		if (e.inputKey == Keys::Up) {
			value = std::min(maximum, value + increment);
		}
		else if (e.inputKey == Keys::Down) {
			value = std::max(minimum, value - increment);
		}
		if (value != oldValue) {
			updateTextBox();
			setCaretIndex(getLength());
			_eventValueChanged(shared_from_this(), ValueChangedEventArgs<T>(value, oldValue));
		}

	}
	void onFocusLost(ControlSPtr sender, EventArgs& e) {
		updateTextBox();
	}

	#pragma endregion
};

typedef NumericTextBox<int> IntTextBox;
typedef NumericTextBox<float> FloatTextBox;

DECLARE_SMART_PTRS(IntTextBox);
DECLARE_SMART_PTRS(FloatTextBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
