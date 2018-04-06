#ifndef AButt_h
#define AButt_h

class AButt
{
public:
	AButt(int pin, int debounce = 50, bool inverted = false, bool intPullup = true, bool isDigital = true);

	void update();

	void onClick(void (*callback)(int));
	void onHold(void (*start)(), void (*end)());

	//return state including de-bounce
	bool getState();

	//return state excluding de-bounce, including inversion etc
	bool getDirectState(); 

	// time before a click is registered as a new sequense
	void setClickDelay(unsigned int delay);
	int getClickDelay();

	// time before a click becomes a hold
	void setHoldDelay(unsigned int delay);
	int getHoldDelay();

private:
	int _pin; //Pin conected to the button
	int _debounce; //nr of ms the btn needs to stay pressed
	bool _inverted; //if inv is true, the button is pressed when there is 0V on Pin
	bool _isDigital; //Is the pin digital, digital read on analog pins can cause problems sometime

	bool _isPressed; //is btn pressed. This INCLUDES the debounce compensation.
	bool _isHold;// is the btn beeing hold?
	bool _wasPressed; //was btn pressed. This INCLUDES the debounce compensation.
	bool _wasHeld; //was btn being held
	bool _lastState; //state of button in previous update 
	bool _lastHoldState; //state of hold in previous update 
	unsigned long _startLastPress; //start last btn use
	unsigned long _startLastPressInc; //start last btn use, including de-bounce
	unsigned long _startLastRelease; //start last btn release

	unsigned int _clickDelay; //time before a click is registered as a new sequense
	unsigned int _holdDelay; //time before a click becomes a hold

	void (*_clickCallback)(int);
	void (*_holdStartCallback)();
	void (*_holdEndCallback)();
};

#endif	
