/*
 * MenuStructure.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  MenuStructure is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_
#define SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_

#include "../menuitem/SubMenu.h"
#include "../menuitem/ItemCommand.h"
#include "../menuitem/requests/requests.h"
#include "../SerialUIConfig.h"
#include "../GrowableList.h"

namespace SerialUI {
namespace Menu {

class Structure : public GrowableList<Item::Item, Item::ID>{
public:
	Structure(uint8_t numItems=SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP);

	uint8_t numItemsWithParent(Item::ID parentId);
	Item::Item * itemByParentAndIndex(Item::ID parentId, uint8_t idx);
	Item::Item * itemByParentAndKey(Item::ID parentId,
			DynamicString aKey, bool allowPartialMatch=true);
	Item::SubMenu * topLevelMenu() { return &(_topLevel);}

	template<Item::Type::Value TYPENAME, class TYPE>
	TYPE * getItemByIdCast(Item::ID id) {
		Item::Item * itm = this->getItemById(id);
		if (! itm) {
			return NULL;
		}
		if (itm->type() != TYPENAME) {
			return NULL;
		}
		return (TYPE*)itm;
	}

	inline Item::SubMenu * getSubMenuById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Menu, Item::SubMenu>(id);
	}

	inline Item::Command * getCommandById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Command, Item::Command>(id);
	}

	inline Item::Request::Request * getRequestById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Input, Item::Request::Request>(id);
	}

private:

	Item::SubMenu _topLevel;

};

} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_ */
