
/*
 * SerialChannel.cpp
 *
 *  Created on: Sep 22, 2018
 *      Author: Pat Deegan
 *
 *  SerialChannel is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */


#include "includes/comm/CommChannelSerial.h"
#include "includes/SUIGlobals.h"
#include "includes/SerialUIStrings.h"
#include "includes/SerialUIPlatform.h"
#include "includes/menuitem/requests/requests.h"

#ifdef SERIALUI_PLATFORM_ARDUINOSTANDARD
namespace SerialUI {
namespace Comm {


SerialChannel::SerialChannel(Mode::Selection forMode, SourceType * port) : Channel(forMode),
		_serPort(port)
{
	_serPort->setTimeout(30000);
}



size_t SerialChannel::print(Tracked::State * state) {
	size_t rVal = this->printFieldStart(state);

	rVal += _serPort->print(state->name());
	rVal += this->printFieldSeparator(Menu::Item::Type::State);
	// rVal += _serPort->print(SUI_STR(": "));
	state->printValueTo(this);
	// rVal += _serPort->println();
	rVal += this->printFieldEnd(Menu::Item::Type::State);

	return rVal;
}

size_t SerialChannel::printHeading(Menu::Item::SubMenu * mnu) {
	// const char * tabs = "\t\t";

	size_t rVal = this->printFieldStart(mnu);
	rVal += _serPort->println(mnu->key());
	if (mnu->help()) {

		rVal+= this->printFieldSeparator(Menu::Item::Type::Menu);
		// rVal += _serPort->print('\t');
		rVal += _serPort->println(mnu->help());
	}
	rVal += this->printFieldEnd(Menu::Item::Type::Menu);

	return rVal;



}

typedef struct ItemTypePrefixMapStruct {
	Menu::Item::Type::Value type;
	char prefix;
	ItemTypePrefixMapStruct(Menu::Item::Type::Value t, char p) :
		type(t), prefix(p) {}
} ItemTypePrefixMap;



size_t SerialChannel::printHelp(Menu::Item::Item * itm) {
	// size_t rVal = 0;
	size_t rVal = this->printHelpFieldStart(itm);
	// const char * tabs = "\t";
	// rVal = _serPort->print(tabs);
	switch(itm->type()) {
	case Menu::Item::Type::Command:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::Command>());
		break;
	case Menu::Item::Type::DynamicText:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::DynamicText>());
		break;
	case Menu::Item::Type::Input:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::Request::Request>());
		break;
	case Menu::Item::Type::Group:
		/* fall-through */
	case Menu::Item::Type::Menu:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::SubMenu>());
		break;
	case Menu::Item::Type::StaticText:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::Text>());
		break;
	case Menu::Item::Type::TrackedView:
		rVal += printHelpItem(itm->castAsSubType<Menu::Item::TrackedView>());
		break;
	default:
		break;

	}

	rVal += this->printHelpFieldEnd(itm);
	return rVal;

}

void SerialChannel::printError(StaticString errMsg) {
	this->printErrorStart();
	_serPort->print(SUI_STR("ERROR: "));
	_serPort->print(errMsg);
	this->printErrorEnd();
}


size_t SerialChannel::printHelpItemStandardDetails(Menu::Item::Item * itm) {
	size_t rVal = _serPort->print(itm->key());
	rVal += this->printHelpFieldSeparator(itm, staticStringLength(itm->key()));
	if (itm->help()) {
		// rVal += printSpaces(staticStringLength(itm->key()));
		rVal += _serPort->print(itm->help());

	}

	rVal += this->printHelpFieldData(itm);

	return rVal;
}

size_t SerialChannel::printHelpItem(Menu::Item::Command* itm) {
	size_t rVal = 0; // _serPort->print('*');
	rVal += printHelpItemStandardDetails(itm);
	return rVal;
}

size_t SerialChannel::printHelpItem(Menu::Item::SubMenu* itm) {
	size_t rVal = 0; // _serPort->print('+');
	rVal += printHelpItemStandardDetails(itm);
	return rVal;
}


size_t SerialChannel::printHelpItem(Menu::Item::TrackedView * itm) {
	size_t rVal = 0; // _serPort->print('>');
		rVal += _serPort->print(itm->key());

		if (itm->help()) {
			rVal += this->printHelpFieldSeparator(itm, rVal);
			// rVal += printSpaces(rVal);
			rVal += _serPort->print(itm->help());
		}

		rVal += this->printHelpFieldData(itm);
		return rVal;
}

size_t SerialChannel::printHelpItem(Menu::Item::Request::Request* itm) {
	size_t rVal = 0; // _serPort->print('>');
	rVal += _serPort->print(itm->key());

	if (Globals::state()->mode() != Mode::Program) {
		rVal += _serPort->print(SUI_STR(" ("));
		rVal += printRequestCurrentValue(itm);
		 _serPort->print(')');
	}


	if (itm->help()) {
		rVal += this->printHelpFieldSeparator(itm, rVal);
		// rVal += printSpaces(rVal);
		rVal += _serPort->print(itm->help());
	}

	rVal += this->printHelpFieldData(itm);

	return rVal;
}

size_t SerialChannel::printHelpItem(Menu::Item::Text* itm) {

	size_t rVal = 0; // _serPort->print(SUI_STR("--- "));
	rVal += _serPort->print(itm->title());
		if (itm->contents()) {
			rVal += this->printHelpFieldSeparator(itm, rVal);
			// rVal += printSpaces(rVal);
			rVal += _serPort->print(itm->contents());
		}
	return rVal;
}

size_t SerialChannel::printHelpItem(Menu::Item::DynamicText* itm) {
	size_t rVal = _serPort->print(itm->title());
	if (itm->contents().length()) {
		rVal += this->printHelpFieldSeparator(itm, itm->title().length());
		// rVal += printSpaces(itm->title().length());
		rVal += _serPort->print(itm->contents());
	}

	return rVal;
}


size_t SerialChannel::printListing(Menu::Item::Item * itm) {

}








int SerialChannel::available(void) {

	while (_serPort->available()) {
		int pVal = _serPort->peek();
		if (pVal == '\r' || pVal == '\n' || pVal == ' ') {
			_serPort->read();
		} else {
			return _serPort->available();
		}
	}
	return 0;
}

int SerialChannel::peek(void) {
	return _serPort->peek();
}

int SerialChannel::read(void) {
	return _serPort->read();
}

void SerialChannel::flushAllWhitespaces() {
	while (_serPort->available()) {
		uint8_t nextB = _serPort->peek();
		if (nextB == '\r' || nextB == '\n' || nextB == ' ') {
			_serPort->read();
			SUI_PLATFORM_DELAY_MS(10);
		} else {
			return ;
		}
	}
	return;
}
void SerialChannel::flush(void) {
	while (_serPort->available()) {
		_serPort->read();
	}
}

bool SerialChannel::getBoolFor(Menu::Item::ID mid, bool* into) {

	int v = _serPort->parseInt();
	if (v) {
		*into = true;
	} else {
		*into = false;
	}
	return true;
}



uint8_t SerialChannel::readUntilAny(uint8_t * rawReadBuf, uint8_t maxLen, uint8_t * vals, uint8_t numvals) {

	bool done=false;
	uint8_t idx=0;
	uint32_t timeoutTime = millis() + 20000;
	while ( (! done) && (millis() < timeoutTime) ) {
		while (_serPort->available() && (idx < maxLen) && ! done ) {
			rawReadBuf[idx] = _serPort->read();
			for (uint8_t i=0; i<numvals; i++) {
				if (rawReadBuf[idx] == vals[i]) {
					done = true;
					rawReadBuf[idx+1] = 0;
				}
			}
			idx++;
		}
	}

	return idx;
}
uint8_t SerialChannel::readUntilEOF(char * rawReadBuf, uint8_t maxLen) {
	uint8_t vals[] = {'\n', '\r'};
	return this->readUntilAny((uint8_t*)rawReadBuf, maxLen, vals, 2);

}

int SerialChannel::timedPeek() {

	int c;
	TimeValue startMs = SUI_PLATFORM_TIMENOW_MS();
	TimeValue tout = _serPort->getTimeout();
	do {
	  c = _serPort->read();
	  if (c >= 0) return c;
	} while(SUI_PLATFORM_TIMENOW_MS() - startMs < tout);
	return -1;
}
bool SerialChannel::getCharFor(Menu::Item::ID mid, char* into) {
	int pVal = this->timedPeek();
	if (pVal >= 0) {
		*into = (char)pVal;
		return true;
	}
	return false;

}

bool SerialChannel::getLongIntFor(Menu::Item::ID mid, long int* into) {
	long int val = _serPort->parseInt();
	*into = val;
	return true;
}

bool SerialChannel::getLongUIntFor(Menu::Item::ID mid, unsigned long int* into) {
	long int val = _serPort->parseInt();
	*into = val;
	return true;
}

bool SerialChannel::getFloatFor(Menu::Item::ID mid, float* into) {
	float val = _serPort->parseFloat();
	*into = val;
	return true;
}

bool SerialChannel::getStringFor(Menu::Item::ID mid, uint8_t maxLength,
		::String* into) {

	// TODO:FIXME HARDCODED MAX STR...
	char buf[51];
	buf[50] = '\0';
	uint8_t numRead = this->readUntilEOF(buf, 50);
	if (numRead) {
		*into = buf;
	if (numRead >= 49)
	{
		this->flush();
	}
	}	
	return true;
}

size_t SerialChannel::write(uint8_t unsignedChar) {
	return _serPort->write(unsignedChar);
}




void SerialChannel::printTracked(bool force) {

	uint8_t numTracked = Globals::trackedStates()->numItems();
	if (numTracked) {
		if (Globals::state()->mode() != Mode::Program) {
			this->println(SUI_STR("**** Tracked Values ****"));
		}
		for (uint8_t i=0; i<numTracked ; i++) {
			Tracked::State * st = Globals::trackedStates()->itemByIndex(i);
			if (force || st->hasChanged()) {
				this->print(st);
				st->changeRegistered();
			}
		}
	}

}

size_t SerialChannel::printRequestCurrentValue(Menu::Item::Request::Request * req) {
	size_t rVal = 0;
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Boolean:
		/* fall - through */
	case Menu::Item::Request::Type::Toggle:
		rVal = _serPort->print( req->castAsSubType<Menu::Item::Request::Boolean>()->currentValue()  ? '1' : '0');
		break;
	case Menu::Item::Request::Type::BoundedLongInt:
		rVal = _serPort->print( req->castAsSubType<Menu::Item::Request::BoundedLong>()->currentValue());
		break;
		/* fall - through */
	case Menu::Item::Request::Type::OptionsList:
		rVal = _serPort->print( req->castAsSubType<Menu::Item::Request::OptionsList>()->currentSelection());
		break;

	case Menu::Item::Request::Type::Float:
		rVal = _serPort->print( req->castAsSubType<Menu::Item::Request::Float>()->currentValue());
		break;
	case Menu::Item::Request::Type::LongInt:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::Long>()->currentValue());
		break;
	case  Menu::Item::Request::Type::UnsignedLongInt:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::UnsignedLong>()->currentValue());
		break;

	case Menu::Item::Request::Type::Event:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::Event>()->currentValue());
		break;

	case Menu::Item::Request::Type::Time:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::Time>()->currentValue());
		// rVal = _serPort->print(F("42"));
		break;

	case Menu::Item::Request::Type::DateTime:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::DateTime>()->currentValue());
		break;


	case Menu::Item::Request::Type::Character:
		rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::Character>()->currentValue());
		break;
	case Menu::Item::Request::Type::String:
		if (req->castAsSubType<Menu::Item::Request::String>()->currentValue().length()) {
			rVal = _serPort->print(req->castAsSubType<Menu::Item::Request::String>()->currentValue());
		} else {
			rVal = _serPort->print(' ');
		}
		break;

	default:
		this->printError(SUI_STR("Unknown type"));
		break;



	}

	return rVal;
}





} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_PLATFORM_ARDUINOSTANDARD */
