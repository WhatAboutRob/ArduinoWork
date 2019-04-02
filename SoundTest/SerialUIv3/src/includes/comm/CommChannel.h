/*
 * CommChannel.h
 *
 *  Created on: May 27, 2018
 *      Author: Pat Deegan
 *
 *  CommChannel is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_COMMCHANNEL_H_
#define SERIALUI_SRC_INCLUDES_COMM_COMMCHANNEL_H_

#include "../SerialUIPlatform.h"
#include "CommSink.h"
#include "CommSource.h"
#include "../tracked/TrackedVariable.h"
#include "../menuitem/items.h"
namespace SerialUI {
namespace Comm {

class Channel : public Source {
public:
	Channel(Mode::Selection forMode);
	using Source::print;
	using Source::println;
	virtual size_t print(Tracked::State * state) = 0;
	virtual size_t printHeading(Menu::Item::SubMenu * mnu) = 0;
	virtual size_t printHelpListStart(Menu::Item::SubMenu * mnu) = 0;
	virtual size_t printHelpListEnd(Menu::Item::SubMenu * mnu) = 0;
	virtual size_t printHelp(Menu::Item::Item * itm) = 0;
	virtual size_t printListing(Menu::Item::Item * itm) = 0;
	virtual void printTracked(bool force=false) = 0;
	virtual void showPrompt() = 0;
	virtual void printInputRequest(Menu::Item::Request::Request * req) = 0;
	virtual void printError(StaticString errMsg) = 0;
	inline Mode::Selection mode() { return _mode;}

	virtual size_t printCommandProcessingStart() = 0;
	virtual size_t printCommandProcessingDone() = 0;



protected:
	Mode::Selection _mode;


};

} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_COMM_COMMCHANNEL_H_ */
