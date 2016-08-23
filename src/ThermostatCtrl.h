/*
 * ThermostatCtrl.h
 *
 *  Created on: 21.08.2016
 *      Author: ian
 */

#ifndef SRC_THERMOSTATCTRL_H_
#define SRC_THERMOSTATCTRL_H_

#include <HomieNode.hpp>
#include <OLEDFrame.h>
#include <OLEDOverlay.h>
#include <SensorNode.h>

#include <Automaton.h>

class ThermostatCtrl: public HomieNode, public OLEDFrame, public OLEDOverlay, public Machine {
public:
	ThermostatCtrl(const SensorNode & sens);

	// Interface HomieNode
	virtual bool handleInput(String const &property, HomieRange range, String const &value) override;

	// Interface OLEDFrame
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

	// Interface OLEDOverlay
	virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) override;

private:
	const SensorNode & sensor;

public:
 enum { THERM_10_SHOWTEMP, THERM_20_SHOW_SETTEMP, THERM_30_CHANGE_SETTEMP, THERM_40_LOG, THERM_50_SETTINGS }; // STATES
 enum { EVT_PREV, EVT_NEXT, EVT_ENTER, ELSE }; // EVENTS
 ThermostatCtrl& begin( void );
 ThermostatCtrl& trace( Stream & stream );
 ThermostatCtrl& trigger( int event );
// int state( void );
// ThermostatCtrl& prev( void );
// ThermostatCtrl& next( void );
// ThermostatCtrl& enter( void );

private:
 enum { ENT_THERM_30_CHANGE_SETTEMP, EXT_THERM_30_CHANGE_SETTEMP }; // ACTIONS
 int event( int id );
 void action( int id );

 float setTemp;

 Atm_encoder encoder;
 Atm_button button;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
 <machine name="ThermostatCtrl">
   <states>
     <THERM_10_SHOWTEMP index="0">
       <EVT_PREV>THERM_50_SETTINGS</EVT_PREV>
       <EVT_NEXT>THERM_30_CHANGE_SETTEMP</EVT_NEXT>
       <EVT_ENTER>THERM_20_SHOW_SETTEMP</EVT_ENTER>
     </THERM_10_SHOWTEMP>
     <THERM_20_SHOW_SETTEMP index="1">
       <EVT_PREV>THERM_30_CHANGE_SETTEMP</EVT_PREV>
       <EVT_NEXT>THERM_30_CHANGE_SETTEMP</EVT_NEXT>
       <EVT_ENTER>THERM_40_LOG</EVT_ENTER>
     </THERM_20_SHOW_SETTEMP>
     <THERM_30_CHANGE_SETTEMP index="2" on_enter="ENT_THERM_30_CHANGE_SETTEMP">
       <EVT_PREV>THERM_30_CHANGE_SETTEMP</EVT_PREV>
       <EVT_NEXT>THERM_30_CHANGE_SETTEMP</EVT_NEXT>
       <EVT_ENTER>THERM_20_SHOW_SETTEMP</EVT_ENTER>
     </THERM_30_CHANGE_SETTEMP>
     <THERM_40_LOG index="3">
       <EVT_PREV>THERM_40_LOG</EVT_PREV>
       <EVT_NEXT>THERM_40_LOG</EVT_NEXT>
       <EVT_ENTER>THERM_50_SETTINGS</EVT_ENTER>
     </THERM_40_LOG>
     <THERM_50_SETTINGS index="4">
       <EVT_PREV>THERM_50_SETTINGS</EVT_PREV>
       <EVT_NEXT>THERM_50_SETTINGS</EVT_NEXT>
       <EVT_ENTER>THERM_10_SHOWTEMP</EVT_ENTER>
     </THERM_50_SETTINGS>
   </states>
   <events>
     <EVT_PREV index="0" access="MIXED"/>
     <EVT_NEXT index="1" access="MIXED"/>
     <EVT_ENTER index="2" access="MIXED"/>
   </events>
   <connectors>
   </connectors>
   <methods>
   </methods>
 </machine>
</machines>

Automaton::ATML::end
*/

#endif /* SRC_THERMOSTATCTRL_H_ */
