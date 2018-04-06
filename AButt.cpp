#include "Arduino.h"
#include "AButt.h"

AButt::AButt(int pin, int debounce = 50, bool inverted = false, bool intPullup = true, bool isDigital = true):
	_pin(pin),
	_debounce(debounce),
	_inverted(inverted),
	_isDigital(isDigital),

	_isPressed(false),
	_isHold(false),
	_wasPressed(false),
	_wasHeld(false),
	_lastState(false),
	_lastHoldState(false),
	_startLastPress(0),
	_startLastPressInc(0),
	_startLastRelease(0),
	_clickDelay(200),
	_holdDelay(500),

	_clickCallback(nullptr),
	_holdStartCallback(nullptr),
	_holdEndCallback(nullptr)
{
	if(intPullup) {
		pinMode(_pin, INPUT_PULLUP);
	} else {
		pinMode(_pin, INPUT);
	}
}

void AButt::setPosFlank(void (*callback)()) {
	_posFlankCallback = callback;
}

void AButt::setNegFlank(void (*callback)()) {
	_negFlankCallback = callback;
}

void AButt::setHold(unsigned long time, void (*callback)()){
	_holdTime = time;
	_holdCallback = callback;
}

bool AButt::getState() {
	return _isPressed;
}

bool AButt::getDirectState() {
	bool state;
	if(_isDigital) {
		state = digitalRead(_pin);
	} else {
		state = analogRead(_pin) > 512;
	}

	if(_inverted) {
		state = !state;
	}
	
	return state;
}

void AButt::update() {
	//Check the real button state
	bool state = getDirectState();
	
	//Manage _isPressed var
	if(state) {
		if(!_lastState) {//potential new btn press
			_startLastPress = millis();
		} else {
			if(millis()-_startLastPress >= _debounce) {
				_isPressed = true; //keeps setting the _isPressed to true as long as the button is pressed for longer then de-bounce 
			}
		}
	}
	
	if(!state) {
		if(_lastState) {//potential new btn release
			_startLastRelease = millis();
		} else {
			if(millis()-_startLastRelease >= _debounce) {
				_isPressed = false;
			}
		}
	}
	_lastState = state;
	
	//Check for pos flank
	if(_isPressed && !_wasPressed) {
		if(_posFlankCallback) { //if no function is assigned yet, do nothing
			(*_posFlankCallback)();
		}
		_wasPressed = _isPressed;
	}

	
	//Check for neg flank
	if(!_isPressed && _wasPressed) {
		if(_negFlankCallback) { //if no function is assigned yet, do nothing
			(*_negFlankCallback)();
		}
		_wasPressed = _isPressed;
	}
	
	//Check for hold (including the de-bounce)
	if(_isPressed) {
		if(!_lastHoldState) {//potential new hold
			_startLastPressInc = millis();
		} else {
			if(millis()-_startLastPressInc >= _holdTime) {
				_isHold = true; //keeps setting the _isPressed to true as long as the button is pressed for longer then de-bounce 
			}
		}
	} else {
		_isHold = false;
	}
	_lastHoldState = _isPressed;
	
	//Check for pos hold flank
	if(_isHold && !_wasHeld) {
		if(_holdCallback) { //if no function is assigned yet, do nothing
			(*_holdCallback)();
		}
		_wasHeld = _isHold;
	}
	//Check for neg hold flank (response not yet implemented)
	if(!_isHold && _wasHeld) {
		_wasHeld = _isHold;
	}
}
