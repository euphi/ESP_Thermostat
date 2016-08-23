/*
 * ThermostatCtrl.cpp
 *
 *  Created on: 21.08.2016
 *      Author: ian
 */

#include <ThermostatCtrl.h>
#include <gfx_resources.h>

ThermostatCtrl::ThermostatCtrl(const SensorNode& sens): \
  HomieNode("Thermostat", "thermostat"), sensor(sens), setTemp(22.5)
{
	advertise("SetTemp")->settable();
}

bool ThermostatCtrl::handleInput(const String& property, HomieRange range,
		const String& value) {
	return false;
}

void ThermostatCtrl::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& UIstate, int16_t x, int16_t y) {
	bool blink = ((millis()>>8) % 2) == 0;

	String temp(sensor.getTemperatur());
	String hum(sensor.getHumidity());
	String setTempStr(setTemp);

	switch (Machine::state()) {

	case THERM_10_SHOWTEMP:
		display.setFont(ArialMT_Plain_24);
		display.setTextAlignment(TEXT_ALIGN_CENTER);
		temp.concat("°C");
		display.drawString(63 + x, 16 + y, temp);
		display.setFont(ArialMT_Plain_16);
		hum.concat("% rel");

		display.drawString(63 + x, 42 + y, hum);
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		break;

	case THERM_20_SHOW_SETTEMP:
		blink = true;
		/* no break */
	case THERM_30_CHANGE_SETTEMP:
		if (blink) {
			display.setFont(ArialMT_Plain_24);
			display.setTextAlignment(TEXT_ALIGN_CENTER);
			setTempStr.concat("°C");
			display.drawString(63 + x, 16 + y, setTempStr);
		}
		display.setFont(ArialMT_Plain_16);
		temp.concat("°C / ");
		temp.concat(sensor.getHumidity());
		temp.concat("%");
		display.drawString(63 +x, 45, temp);
		break;

	case THERM_40_LOG:
		display.setFont(Dialog_plain_7);
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		display.drawLogBuffer(0,16);
		break;

	case THERM_50_SETTINGS:
		display.setFont(ArialMT_Plain_24);
		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.drawString(63,16,"Settings");
		break;

	}

}

void ThermostatCtrl::drawOverlay(OLEDDisplay& display,
		OLEDDisplayUiState& state, uint8_t idx) {
}

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

ThermostatCtrl& ThermostatCtrl::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                                               ON_ENTER  ON_LOOP  ON_EXIT                                   EVT_PREV                 EVT_NEXT              EVT_ENTER  ELSE */
    /*       THERM_10_SHOWTEMP */                          -1,      -1,      -1,                          THERM_50_SETTINGS,       THERM_30_CHANGE_SETTEMP, THERM_20_SHOW_SETTEMP,   -1,
    /*   THERM_20_SHOW_SETTEMP */                          -1,      -1,      -1,                          THERM_30_CHANGE_SETTEMP, THERM_30_CHANGE_SETTEMP, THERM_40_LOG,            -1,
    /* THERM_30_CHANGE_SETTEMP */ ENT_THERM_30_CHANGE_SETTEMP,      -1,      EXT_THERM_30_CHANGE_SETTEMP, THERM_30_CHANGE_SETTEMP, THERM_30_CHANGE_SETTEMP, THERM_20_SHOW_SETTEMP,   -1,
    /*            THERM_40_LOG */                          -1,      -1,      -1,                          THERM_40_LOG,            THERM_40_LOG,            THERM_50_SETTINGS,       -1,
    /*       THERM_50_SETTINGS */                          -1,      -1,      -1,                          THERM_50_SETTINGS,       THERM_50_SETTINGS,       THERM_10_SHOWTEMP,       -1,
  };
  // clang-format on
	encoder.begin(12, 13, 4);
	button.begin(14).debounce(10);

	encoder.onChange(true, *this, EVT_NEXT);
	encoder.onChange(false, *this, EVT_PREV);
	button.onPress(*this, EVT_ENTER);

	pinMode(12, INPUT_PULLUP);
	pinMode(13, INPUT_PULLUP);

	encoder.trace(Serial);
	button.trace(Serial);
	this->trace(Serial);
	Machine::begin(state_table, ELSE);
	return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int ThermostatCtrl::event( int id ) {
  switch ( id ) {
    case EVT_PREV:
      return 0;
    case EVT_NEXT:
      return 0;
    case EVT_ENTER:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void ThermostatCtrl::action(int id) {
//	switch (id) {
//	case ENT_THERM_30_CHANGE_SETTEMP:
//		encoder.onChange(true, *this, EVT_MORE);
//		encoder.onChange(false, *this, EVT_LESS);
//		button.onPress(*this, EVT_ENTER);
//		return;
//
//	case EXT_THERM_30_CHANGE_SETTEMP:
//		encoder.onChange(true, *this, EVT_NEXT);
//		encoder.onChange(false, *this, EVT_PREV);
//		return;
//	}
}

///* Optionally override the default trigger() method
// * Control how your machine processes triggers
// */
//
ThermostatCtrl& ThermostatCtrl::trigger( int event ) {
  Serial.println("Event triggered");
  Serial.flush();
  Serial.printf("No: %02x", event);
  Serial.flush();
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

//int ThermostatCtrl::state( void ) {
//  return Machine::state();
//}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

//ThermostatCtrl& ThermostatCtrl::prev() {
//  trigger( EVT_PREV );
//  return *this;
//}
//
//ThermostatCtrl& ThermostatCtrl::next() {
//  trigger( EVT_NEXT );
//  return *this;
//}
//
//ThermostatCtrl& ThermostatCtrl::enter() {
//  trigger( EVT_ENTER );
//  return *this;
//}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

ThermostatCtrl& ThermostatCtrl::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "THERMOSTATCTRL\0EVT_PREV\0EVT_NEXT\0EVT_ENTER\0ELSE\0THERM_10_SHOWTEMP\0THERM_20_SHOW_SETTEMP\0THERM_30_CHANGE_SETTEMP\0THERM_40_LOG\0THERM_50_SETTINGS" );
  return *this;
}
