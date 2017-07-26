#include "AButt.h"

//create AButt object
//AButt(int pin, bool intPullup, bool isDigital, bool inv, int debounce){
AButt btn(12,true,true,true,50); //(normal digital btn with internal pull-up: connect btn to pin and ground)

void setup() {
	//link response functions to possible btn actions
	btn.setPosFlank(pos);
	btn.setNegFlank(neg);
	btn.setHold(2000,hold);
	Serial.begin(115200);
}

void loop() {
	//handle button (never use delays)
	btn.update();
}

void neg(){
	Serial.println("neg");
}

void pos(){
	Serial.println("pos");
}

void hold(){
  Serial.println("hold");
}
