/*
 * SerialUI.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUI is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/SerialUI.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {


SerialUI::SerialUI(uint8_t num_top_level_menuitems_hint,  Comm::SourceType * commsrc) :
				_userLastInteractionMs(0),
				_commsrc(commsrc),
				_userPresenceTimeoutMs(25000),
				_genFlags(0),
				_readTerminatorChar('\n')
{

#ifdef SUI_SERIALUI_ECHO_ON
	_genFlags.echocomm()ands = true;
#endif

}
SerialUI::~SerialUI() {

}

bool SerialUI::begin(uint32_t param) {
	Comm::Channel* ch = Comm::ChannelManager::newChannelFor(Mode::User, _commsrc);

	if (! ch) {
		return false;
	}
	_commsrc->begin(param);

	return true;
}

void SerialUI::setGreeting(StaticString greets)
{
	Globals::state()->setGreeting(greets);
}

void SerialUI::setUID(StaticString u) {
	Globals::state()->setUID(u);
}

void SerialUI::setMaxIdleMs(uint16_t max_ms) {
	_userPresenceTimeoutMs = max_ms;
}

uint16_t SerialUI::maxIdleMs() {
	return _userLastInteractionMs;
}

void SerialUI::setReadTerminator(char setTo) {
	_readTerminatorChar = setTo;
}

char SerialUI::readTerminator() {
	return _readTerminatorChar;
}

size_t SerialUI::readBytesToEOL(char* buffer, size_t max_length, bool left_trim){
#warning "TODO:FIXME readBytesToEOL"
}

Menu::Menu * SerialUI::topLevelMenu(StaticString setNameTo){
	return Globals::menuStructure()->topLevelMenu();
}
void SerialUI::goToTopLevelMenu(){
	Globals::state()->enterMenu(Globals::menuStructure()->topLevelMenu());
}


bool SerialUI::checkForUser(uint16_t timeout_ms){
	TimeValue tmOut = (timeout_ms > 0) ? (SUI_PLATFORM_TIMENOW_MS() + timeout_ms) : 0;
	do {
		if (Globals::commSource()->available()) {
			// Serial.print('.');
			// this->println(Globals::commSource()->available());
			_genFlags.user_present = true;
			_genFlags.user_just_arrived = true;
			this->enter();
			return true;
		}

		SUI_PLATFORM_DELAY_MS(SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS);
	} while (SUI_PLATFORM_TIMENOW_MS() < tmOut);

	return false;
}

bool SerialUI::checkForUserOnce(uint16_t timeout_ms){
	if (_genFlags.user_check_performed) {
		return false;
	}
	_genFlags.user_check_performed = true;
	return checkForUser(timeout_ms);

}


bool SerialUI::userPresent(){
	if (!_genFlags.user_present) {

		return false;
	}
	// there _was_ a user, but let's see how long it's
	// been since they performed any actions
	if (_userLastInteractionMs &&
			((SUI_PLATFORM_TIMENOW_MS() - _userLastInteractionMs)
					> _userPresenceTimeoutMs)) {

		// yeah, looks like they're gone for coffee...
		exit(false);
	}

	return _genFlags.user_present;



}


void SerialUI::handleRequests(uint8_t maxRequests){

	Menu::Item::SubMenu * ret_menu;
	uint32_t timeNow;
	for (uint8_t i = 0; i <= maxRequests; i++) {

		timeNow = SUI_PLATFORM_TIMENOW_MS();


		if (comm()->available() > 0) {

			// we have input (and therefore a user), so we reset our presence counter to zero
			ret_menu = Globals::state()->currentMenu()->handleRequest();
			_userLastInteractionMs = SUI_PLATFORM_TIMENOW_MS();

			if (ret_menu) {
				if ( _genFlags.menu_manual_override)
				{
					// leave the current menu as is, as it was overridden in
					// the last command
					_genFlags.menu_manual_override = false;
				} else {
					// make certain we reflect whatever changes are required
					// (e.g. the item was a submenu)
					Globals::state()->enterMenu(ret_menu);
				}
			} else {
				exit(false);
				return;
			}
			showPrompt();

		} else {
#if defined(SERIALUI_HANDLEREQUESTS_DELAY_MS) and (SERIALUI_HANDLEREQUESTS_DELAY_MS > 0)
			SUI_PLATFORM_DELAY_MS(SERIALUI_HANDLEREQUESTS_DELAY_MS);
#endif
		}

		// trigger heartbeat--after handling any pending requests
		// so they don't interfere with any transactions coming in
		triggerHeartbeat((timeNow));
		if (Globals::state()->mode() == Mode::Program) {
			if (_genFlags.user_just_arrived) {
				if (_userLastInteractionMs
						&& ( (SUI_PLATFORM_TIMENOW_MS() - _userLastInteractionMs) > 4000) ) {
					_genFlags.user_just_arrived = false;

				}
			} else {

				Globals::commChannel()->printTracked();
			}
		}
	}

	return;


}

Menu::Menu * SerialUI::currentMenu(){
	return Globals::state()->currentMenu();
}


Menu::Item::Item * SerialUI::currentMenuItem(){
	return Globals::state()->currentItem();
}


void SerialUI::setCurrentMenu(Menu::Menu * setTo){
	Menu::Item::SubMenu * smItm =
			Globals::menuStructure()->getSubMenuById(setTo->id());

	if (smItm) {
		Globals::state()->enterMenu(smItm);
	}
}


void SerialUI::enter(){

	_genFlags.user_present = true;
	_userLastInteractionMs = 0;
	goToTopLevelMenu();
}


void SerialUI::exit(bool terminate_gui){
	_genFlags.user_present = false;
	_userLastInteractionMs = 0;
	goToTopLevelMenu();
	comm()->println(SUI_STR("Thanks for using SerialUI!"));

}


void SerialUI::returnOK(){
	comm()->println(SUI_STR("OK"));
}


void SerialUI::returnMessage(StaticString message){

	comm()->println(message);
}

void SerialUI::returnError(const char * errmsg){

	comm()->println(errmsg);
}

#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
void SerialUI::returnError(StaticString errmsg) {

	comm()->println(errmsg);
}
#endif


void SerialUI::showPrompt(){
	comm()->showPrompt();
}


void SerialUI::showEnterDataPrompt(){


}

void SerialUI::showEnterNumericDataPrompt(){

}
#ifdef SUI_ENABLE_STREAMPROMPTING
size_t SerialUI::showEnterStreamPromptAndReceive(char * bufferToUse, uint8_t bufferSize, streamInputCallback callback,
		streamInputStartCallback startupCallback, streamInputEndCallback
		completedCallback){

}
#endif

void SerialUI::setMode(Mode::Selection setTo){

	Globals::state()->setMode(setTo);

}

Comm::Channel * SerialUI::comm() {

	// return Comm::ChannelManager::currentChannel() ;

	if (Comm::ChannelManager::currentChannel() && Comm::ChannelManager::currentChannel()->mode() == Globals::state()->mode()) {
		return Comm::ChannelManager::currentChannel() ;
	}

	Comm::ChannelManager::releaseChannel();

	Comm::Channel * ch = Comm::ChannelManager::newChannelFor(Globals::state()->mode(), _commsrc);

	return ch;

}

Mode::Selection SerialUI::mode(){
	return Globals::state()->mode();
}


void SerialUI::setEchoCommands(bool setTo){
	_genFlags.echo_commands = setTo;

}

void SerialUI::setUserPresenceHeartbeat(HeartbeatFunction hbf){
	_heartbeat.func = hbf;

}
void SerialUI::setUserPresenceHeartbeatPeriod(uint16_t ms){
	_heartbeat.periodMs = ms;

}
uint16_t SerialUI::userPresenceHeartbeatPeriod(){
	return _heartbeat.periodMs;

}

bool SerialUI::trackState(StaticString name, Tracked::State & tracked) {
	tracked.setName(name);
	return Globals::trackedStates()->addItem(&tracked);
}


bool SerialUI::trackState(Tracked::State & tracked) {
	return Globals::trackedStates()->addItem(&tracked);
}


bool SerialUI::showTrackedState(bool force){
	return Globals::menuStructure()->topLevelMenu()->refreshTracking(force);

}


#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
size_t SerialUI::print(StaticString s) {
	return comm()->print(s);
}
size_t SerialUI::println(StaticString s) {
	return comm()->println(s);
}
#endif

#warning "const char* disabled"
/*
size_t SerialUI::print(const char* p) {
	// TODO:FIXME wtf??
	// return comm()->print(p);
#warning "const char* disabled"
	return 0;
}
*/
size_t SerialUI::print(const String& p) {
	return comm()->print(p);
}

size_t SerialUI::print(const char * p) {
	return comm()->print(p);
}

size_t SerialUI::print(char p) {
	return comm()->print(p);
}

size_t SerialUI::print(unsigned char p, int int1) {
	return comm()->print(p, int1);
}

size_t SerialUI::print(int int1, int int2) {
	return comm()->print(int1, int2);
}

size_t SerialUI::print(unsigned int p, int int1) {

	return comm()->print(p, int1);
}

size_t SerialUI::print(long p, int int1) {

	return comm()->print(p, int1);
}

size_t SerialUI::print(unsigned long p, int int1) {
	return comm()->print(p, int1);
}

size_t SerialUI::print(double p, int int1) {
	return comm()->print(p, int1);
}

/*
size_t SerialUI::println(const char* p) {
	return comm()->println(p);
}
*/

size_t SerialUI::println(const String& s) {
	return comm()->println(s);
}

size_t SerialUI::println(const char * p) {
	return comm()->println(p);
}

size_t SerialUI::println(char p) {
	// return comm()->print(p);
	return comm()->print((char)p);
}

size_t SerialUI::println(unsigned char p, int int1) {
	return comm()->println(p, int1);
}

size_t SerialUI::println(int p, int int2) {
	return comm()->println(p, int2);
}

size_t SerialUI::println(unsigned int p, int int1) {
	return comm()->println(p, int1);
}

size_t SerialUI::println(long p, int int1) {
	return comm()->println(p, int1);
}

size_t SerialUI::println(unsigned long p, int int1) {
	return comm()->println(p, int1);
}

size_t SerialUI::println(double p, int int1) {
	return comm()->println(p, int1);
}



size_t SerialUI::println(void) {
	return comm()->println();
}

void SerialUI::triggerHeartbeat(uint32_t timeNow) {

	if (_heartbeat.isSet())
	{
		if (timeNow >= _heartbeat.nextBeat())
		{
			// time to call again...
			_heartbeat.trigger(timeNow);
		}
	}

}

} /* namespace SerialUI */




