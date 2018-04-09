#ifndef AButt_h
#define AButt_h

class AButt
{
public:
	AButt(int pin, unsigned int debounce = 50, bool inverted = false, bool intPullup = true, bool isDigital = true);

	void update();

	void onClick(void(*callback)(unsigned short));
	void onHold(void(*start)(), void(*end)());

	//return state including de-bounce
	bool isPressed();
	bool isHeld();

	//return state excluding de-bounce, including inversion etc
	bool getState(); 

	// time before a click is registered as a new sequense
	void setClickDelay(unsigned int delay);
	int getClickDelay();

	// time before a click becomes a hold
	void setHoldDelay(unsigned int delay);
	int getHoldDelay();

	// maximum number of clicks in a sequence
	void setMaxClicks(unsigned short clicks);
	short getMaxClicks();

private:
	int _pin; //pin conected to the button
	bool _inverted; //if inv is true, the button is pressed when there is 0V on Pin
	bool _isDigital; //is the pin digital, digital read on analog pins can cause problems sometime

	bool _isPressed; //is btn pressed. This INCLUDES the debounce compensation.
	bool _isHeld; //is the btn beeing hold?
	bool _wasPressed; //was btn pressed. This INCLUDES the debounce compensation.

	short clickCount; //counting the number of double clicks

	bool _lastState; //state of button in previous update 
	unsigned long _lastDebounceTime; //start last btn state change
	unsigned long _lastPressTime; //start of the last btn press, including de-bounce
	unsigned long _lastClickTime; //start of the last btn release, including de-bounce

	unsigned int _debounceDelay; //nr of ms the btn needs to stay pressed
	unsigned int _clickDelay; //time before a click is registered as a new sequense
	unsigned int _holdDelay; //time before a click becomes a hold

	unsigned short _maxClicks; //maximum number of clicks in a sequence

	void(*_clickCallback)(unsigned short);
	void(*_holdStartCallback)();
	void(*_holdEndCallback)();

	void finishClick();

	void startHold();
	void endHold();
};

#endif	
