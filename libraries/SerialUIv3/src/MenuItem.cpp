/*
 * MenuItem.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  MenuItem is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/MenuItem.h"


namespace SerialUI {
namespace Menu {
namespace Item {
ID Item::id_counter = 2;


Item::Item(Type::Value itemType) :
	_parentId(0), _type(itemType), _key(NULL), _help(NULL) {

	_id = id_counter++;
}
Item::Item(Type::Value itemType, StaticString key, StaticString help) :
	_parentId(0), _type(itemType), _key(key), _help(help)
{
	_id = id_counter++;
}


Item::Item(ID id, Type::Value itemType, StaticString key, StaticString help) :
		_id(id), _parentId(0), _type(itemType), _key(key), _help(help)
{
	if (id_counter <= _id) {
		id_counter = _id+1;
	}
}


StaticStringLength Item::stringLength(StaticString str) {
	if (! str) {
		return 0;
	}
	return SUI_PLATFORM_STATICSTRING_LENGTH(str);
}


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

