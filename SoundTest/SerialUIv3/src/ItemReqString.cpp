/*
 * ItemReqString.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqString is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */
#include "includes/menuitem/requests/ItemReqString.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

String::String(TopLevelString initVal,
		StaticString key,
		StaticString help, uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
				TypedRequest<Type::String, TopLevelString>(initVal, key, help,
						vchng, validcb),
						_maxLen(maxlen)
{

}


String::String(uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
				TypedRequest<Type::String, TopLevelString>("", vchng, validcb),
						_maxLen(maxlen)
{

}

String::String(
		StaticString key,
		StaticString help, uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
				TypedRequest<Type::String, TopLevelString>("", key, help,
						vchng, validcb),
						_maxLen(maxlen)
{

}
bool String::getValue(Menu * callingMenu, TopLevelString * v) {
	return Globals::commSource()->getStringFor(id(), _maxLen, v);
}


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */
