#include "Arduino.h"
#include "AButt.h"

AButt::AButt(int pin, bool intPullup, bool isDigital, bool inv, int debounce){
	_pin = pin;
	_intPullup = intPullup;
	_isDigital = isDigital;
	_inv = inv;
	_debounce = debounce;
	if(_intPullup){
		pinMode(_pin, INPUT_PULLUP);
	}else{
		pinMode(_pin, INPUT);
	}
}
/*
void AButt::pressed(){
  digitalWrite(13, HIGH);  
}

void AButt::released(){
  digitalWrite(13, LOW);
}
*/

void AButt::setPosFlank(void (*callback)()){
	_posFlankCallback = callback;
}

void AButt::setNegFlank(void (*callback)()){
	_negFlankCallback = callback;
}

void AButt::setHold(unsigned long time, void (*callback)()){
	_holdTime = time;
	_holdCallback = callback;
}

bool AButt::getState(){
	return _isPressed;
}

bool AButt::getDirectState(){
	bool state;
	if(_isDigital){
		state = digitalRead(_pin);
	}else{
		state = analogRead(_pin)>512;
	}

	if(_inv){
		state = !state;
	}
	
	return state;
}

void AButt::update(){
	//Check the real button state
	bool state;
	if(_isDigital){
		state = digitalRead(_pin);
	}else{
		state = analogRead(_pin)>512;
	}

	if(_inv){
		state = !state;
	}
	
	//Manage _isPressed var
	if(state){
		if(!_lastState){//potential new btn press
			_startLastPress = millis();
		}else{
			if(millis()-_startLastPress >= _debounce){
				_isPressed = true; //keeps setting the _isPressed to true as long as the button is pressed for longer then de-bounce 
			}
		}
	}
	
	if(!state){
		if(_lastState){//potential new btn release
			_startLastRelease = millis();
		}else{
			if(millis()-_startLastRelease >= _debounce){
				_isPressed = false;
			}
		}
	}
	_lastState = state;
	
	//Check for pos flank
	if(_isPressed && !_wasPressed){
		if(_posFlankCallback){//if no function is assigned yet, do nothing
			(*_posFlankCallback)();
		}
		_wasPressed = _isPressed;
	}

	
	//Check for neg flank
	if(!_isPressed && _wasPressed){
		if(_negFlankCallback){//if no function is assigned yet, do nothing
			(*_negFlankCallback)();
		}
		_wasPressed = _isPressed;
	}
	
	//Check for hold (including the de-bounce)
	if(_isPressed){
		if(!_lastHoldState){//potential new hold
			_startLastPressInc = millis();
		}else{
			if(millis()-_startLastPressInc >= _holdTime){
				_isHold = true; //keeps setting the _isPressed to true as long as the button is pressed for longer then de-bounce 
			}
		}
	}else{
		_isHold = false;
	}
	_lastHoldState = _isPressed;
	
	//Check for pos hold flank
	if(_isHold && !_wasHeld){
		if(_holdCallback){//if no function is assigned yet, do nothing
			(*_holdCallback)();
		}
		_wasHeld = _isHold;
	}
	//Check for neg hold flank (response not yet implemented)
	if(!_isHold && _wasHeld){
		_wasHeld = _isHold;
	}
}
