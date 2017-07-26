/*
  
*/
#ifndef AButt_h
#define AButt_h

#include "Arduino.h"

class AButt
{
  public:
    AButt(int pin, bool intPullup, bool isDigital, bool inv, int debounce);
	void update();
	void setPosFlank(void (*callback)());
	void setNegFlank(void (*callback)());
	void setHold(unsigned long time, void (*callback)()) ;//when button is held for x time execute
	bool getState(); //return state including de-bounce 
	bool getDirectState(); //return state excluding de-bounce, including inversion etc
	
  private:
    int _pin; //Pin conected to the button
	bool _intPullup; //Set high if internal pullup is used
	bool _isDigital; //Is the pin digital, digital read on analog pins can cause problems sometime
	bool _inv; //if inv is true, the button is pressed when there is 0V on Pin
	int _debounce; //nr of ms the btn needs to stay pressed
	bool _isPressed; //is btn pressed. This INCLUDES the debounce compensation.
	bool _isHold;// is the btn beeing hold?
	bool _wasPressed; //was btn pressed. This INCLUDES the debounce compensation.
	bool _wasHeld; //was btn being held
	bool _lastState; //state of button in previous update 
	bool _lastHoldState; //state of hold in previous update 
	unsigned long _startLastPress; //start last btn use
	unsigned long _startLastPressInc; //start last btn use, including de-bounce
	unsigned long _startLastRelease; //start last btn release
	unsigned long _holdTime; //start last btn release
	void (*_posFlankCallback)(void);
	void (*_negFlankCallback)(void);
	void (*_holdCallback)(void);
	
};

#endif	
