/*
 * ItemText.cpp
 *
 *  Created on: May 31, 2018
 *      Author: Pat Deegan
 *
 *  ItemText is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/ItemText.h"

namespace SerialUI {
namespace Menu {
namespace Item {
Text::Text(StaticString t, StaticString c)
		: Item(Type::StaticText, t, c) {

}
Text::Text(ID id, StaticString t, StaticString c)
: Item(id, Type::StaticText, t, c) {

}

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */
