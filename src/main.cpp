/*
 * main.cpp
 *
 *  Created on: 21.08.2016
 *      Author: ian
 */


#include <Homie.hpp>

#include <SensorNode.h>
#include <LoggerNode.h>

// Platformio libs necessary for non-pio libs in lib/ dir
#include <Wire.h>
#include <Sensors.h>
#include <Automaton.h>
//#include <Atm_encoder.hpp>


// Display & UI
#include <SSD1306.h>
#include <OLEDDisplayUi.h>
#include <OLEDStatusIndicator.h>



#include <ThermostatCtrl.h>

// Connected peripherals
SSD1306Wire display(0x3c, SDA, SCL);
OLEDDisplayUi ui(&display);
OLEDStatusIndicator status;


// HomieNodes
SensorNode sensor;
ThermostatCtrl thermostat(sensor);


void setup() {
	// Serial
	Serial.begin(115200);
	Serial.println("Starting Thermostat.");
	Serial.flush();

	// Logging
	LN.setLoglevel(LoggerNode::DEBUG);

	// Display and UI
//	display.setLogBuffer(4,200);

	ui.setTargetFPS(10);
	ui.setFrameAnimation(SLIDE_LEFT);
	ui.disableAutoTransition();
	ui.disableAllIndicators();
	ui.init();
	display.flipScreenVertically();

	// Homie
	Homie.setLoggingPrinter(&Serial);
	Homie.onEvent([](HomieEvent event) {status.Event(event);});
	Homie.setup();

	thermostat.begin();

	Serial.begin(115200);
	Serial.println("Started Thermostat.");
	Serial.flush();
}


void loop() {
	Homie.loop();
	ui.update();
	automaton.run();
}

