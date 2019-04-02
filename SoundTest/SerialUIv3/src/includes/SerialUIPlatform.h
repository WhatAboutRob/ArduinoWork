/*
 * SerialUIPlatform.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUIPlatform is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_
#define SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_

#include "SerialUIConfig.h"

#ifdef SERIALUI_PLATFORM_ARDUINOSTANDARD
#include "platform/SUIPlatArduino.h"
#endif


namespace SerialUI {


size_t staticStringLength(StaticString aKey);

bool staticStringMatch(StaticString aKey,
		DynamicString theTest,
		bool allowPartials=true);
}

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_ */
