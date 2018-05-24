#include <AButt.h>

const int BUTTON_PIN = 12;
//create AButt object
AButt btn(BUTTON_PIN); //normal digital btn 

void setup() {
	// set button pin to input with internal pull-up: connect btn to pin and ground)
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	//link response functions to possible btn actions
	btn.onClick(clicked);
	btn.onHold(holdStart, holdEnd);
	Serial.begin(115200);
}

void loop() {
	//handle button (never use delays)
	btn.update();

	if (btn.isHeld()) {
		Serial.println("btn is being held.");
	}
}

void clicked(unsigned short clicks) {
	Serial.print("button clicked ");
	Serial.print(clicks);
	Serial.println(" times.");
}

void holdStart() {
	Serial.println("start holding");
}
void holdEnd() {
	Serial.println("holding ended");
}
