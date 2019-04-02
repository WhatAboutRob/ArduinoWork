/*
 * SubMenu.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SubMenu is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/SubMenu.h"
#include "includes/SUIGlobals.h"
#include "includes/menuitem/trackedview/trackedviews.h"
#include "includes/menuitem/SubGroup.h"

namespace SerialUI {
namespace Menu {
namespace Item {

SubMenu::SubMenu(ID parentId, StaticString key, StaticString help) :
		Item(Type::Menu, key, help) {
	this->setParentId(parentId);

}
SubMenu::SubMenu(ID id, ID parentId, StaticString key, StaticString help) :
		Item(id, Type::Menu, key, help) {
	this->setParentId(parentId);
}

void SubMenu::call(Menu * callingMenu) {
	// implies "entering" the submenu.
	Globals::state()->enterMenu(this);

}

uint8_t SubMenu::numItems() {
	return Globals::menuStructure()->numItemsWithParent(id());
}

Item* SubMenu::itemById(ID itemId) {
	Item * itm = Globals::menuStructure()->getItemById(itemId);
	if (!(itm && (itm->parentId() == id()))) {
		return NULL;
	}

	return itm;
}

Item * SubMenu::itemByKey(DynamicString keyToFind) {
	return Globals::menuStructure()->itemByParentAndKey(id(), keyToFind, true);
}

Item * SubMenu::itemByIndex(uint8_t idx) {
	return Globals::menuStructure()->itemByParentAndIndex(id(), idx);
}

SubMenu * SubMenu::handleRequest() {
	Comm::Request nextReq;

	if (!Globals::commSource()->getNextRequest(this->id(), &nextReq)) {
		if (nextReq.requestId != SERIAL_UI_REQUEST_NOINPUT) {
			Globals::commChannel()->printError(SUI_STR("Unrecognized request"));
			Globals::commSource()->flush();
		}
		return this;
	}

	if (nextReq.isForMenuItem()) {

		Globals::state()->setCurrentRequestContext(&nextReq);
		Globals::commChannel()->printCommandProcessingStart();
		if (nextReq.menuItem->type() == Type::Input) {
			Globals::commChannel()->printInputRequest(
					nextReq.menuItem->castAsSubType<Request::Request>());
		}

		nextReq.trigger(this);
		Globals::state()->clearCurrentRequestContext();
		Globals::commChannel()->printCommandProcessingDone();
		return Globals::state()->currentMenu();
	}

	// builtin...
	if (!nextReq.isForBuiltIn()) {
		// something wrong...

		return this;
	}

	return handleBuiltinRequest(nextReq.builtIn);

}

SubMenu * SubMenu::handleBuiltinRequest(
		::SerialUI::Request::BuiltIn::Selection sel) {

	switch (sel) {
	case SerialUI::Request::BuiltIn::Exit:
		this->exitRequested();
		return NULL;
		break;
	case SerialUI::Request::BuiltIn::Help:
		this->outputHelp();
		break;
	case SerialUI::Request::BuiltIn::ListMenu:
		this->outputListing();
		break;
	case SerialUI::Request::BuiltIn::RefreshTracked:
		this->refreshTracking();
		break;
	case SerialUI::Request::BuiltIn::UpLevel:
		this->upLevel();
		break;
	case SerialUI::Request::BuiltIn::ModeProgram:
		this->switchModeProgram();
		break;
	case SerialUI::Request::BuiltIn::ModeUser:
		this->switchModeUser();
		break;
	default:
		return this;
	}

	return Globals::state()->currentMenu();

}

void SubMenu::upLevel() {
	if (parentId()) {
		SubMenu * mnu = Globals::menuStructure()->getSubMenuById(parentId());
		if (mnu) {

			Globals::state()->enterMenu(mnu);
		}
	}

	return;
}

void SubMenu::outputListing() {

	uint8_t nmItm = this->numItems();
	for (uint8_t i = 0; i < nmItm; i++) {
		Item * itm = this->itemByIndex(i);
		if (itm) {
			Globals::commChannel()->printListing(itm);
		}
	}

}

void SubMenu::outputHelp() {

	bool inProgMode = (Globals::state()->mode() == Mode::Program);

	if (inProgMode) {
		if (!this->parentId()) {
			Globals::commChannel()->printHelpListStart(this);

			Globals::commChannel()->printTracked(true);
		}
	} else {
		Globals::commChannel()->printHeading(this);
	}

	uint8_t nmItm = this->numItems();
	for (uint8_t i = 0; i < nmItm; i++) {
		//Serial.print(F("SBM GETITM "));
		// Serial.println((int)i);
		Item * itm = this->itemByIndex(i);
		if (itm) {

			Globals::commChannel()->printHelp(itm);
			if (inProgMode) {
				if (itm->type() == Type::Menu || itm->type() == Type::Group) {
					// Serial.println(F("SBM GETITM SUB..."));
					itm->castAsSubType<SubMenu>()->outputHelp();
				}
			}
		}
	}

	if (!this->parentId()) {

		if (inProgMode) {
			Globals::commChannel()->printHelpListEnd(this);
		} else {

			Globals::commChannel()->printTracked(true);
		}
	}

}

uint8_t SubMenu::refreshTracking(bool force) {

	uint8_t numStates = Globals::trackedStates()->numItems();
	uint8_t numReturned = 0;
	for (uint8_t i = 0; i < numStates; i++) {
		Tracked::State * aState = Globals::trackedStates()->itemByIndex(i);

		if (force || aState->hasChanged()) {
			Globals::commChannel()->print(aState);
			numReturned++;
		}
	}
	return numReturned;
}
void SubMenu::switchModeProgram() {
	switchMode(Mode::Program);
}
void SubMenu::switchModeUser() {
	switchMode(Mode::User);
}

void SubMenu::switchMode(Mode::Selection switchTo) {
	Globals::state()->setMode(switchTo);
}

bool SubMenu::addText(StaticString title, StaticString contents) {
	Text * txt = new Text(title, contents);
	if (!txt) {
		return false;
	}

	txt->setParentId(this->id());

	return Globals::menuStructure()->addItem(txt);

}
bool SubMenu::addCommand(StaticString key_str, Command::Callback callback,
		StaticString help_str) {
	Command * cmd = new Command(callback, key_str, help_str);
	if (!cmd) {
		return false;
	}
	cmd->setParentId(this->id());

	return Globals::menuStructure()->addItem(cmd);

}

bool SubMenu::addView(TrackedView & trackedView) {
	trackedView.setParentId(this->id());
	return Globals::menuStructure()->addItem(&trackedView);
}
bool SubMenu::addRequest(Request::Request & req) {
	req.setParentId(this->id());
	return Globals::menuStructure()->addItem(&req);
}

Menu * SubMenu::addGroup(StaticString key_str, StaticString help_str) {
	Group * grp = new Group(this->id(), key_str, help_str);
	if (!grp) {
		return NULL;
	}
	if (Globals::menuStructure()->addItem(grp)) {
		return grp;
	}

	delete grp;
	return NULL;

}
Menu * SubMenu::subMenu(StaticString key_str, StaticString help_str) {
	SubMenu * smnu = new SubMenu(this->id(), key_str, help_str);
	if (!smnu) {
		return NULL;
	}

	if (Globals::menuStructure()->addItem(smnu)) {
		return smnu;
	}

	delete smnu;
	return NULL;

}
void SubMenu::exitRequested() {
	Globals::state()->enterMenu(Globals::menuStructure()->topLevelMenu());

}

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

