/*
 * SUIPlatArduino.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIPlatArduino is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_PLATFORM_SUIPLATARDUINO_H_
#define SERIALUIV3_SRC_INCLUDES_PLATFORM_SUIPLATARDUINO_H_

#include <Arduino.h>


#define SUI_PLATFORM_SOURCE_DEFAULT		Serial

#define SUI_STR(...)	F(__VA_ARGS__)
#define SUI_PLATFORM_TIMENOW_MS()				millis()
#define SUI_PLATFORM_DELAY_MS(d)				delay(d)

namespace SerialUI {

#ifdef DESKTOP_COMPILE
#define SUI_FLASHSTRING		const char *
typedef const char * StaticString;
typedef const char * DynamicString;
#else
#define SUI_STATICSTRING_IS_DISTINCT_TYPE
#define SUI_FLASHSTRING		const __FlashStringHelper*

typedef const __FlashStringHelper* StaticString;
typedef const char * DynamicString;
#endif

#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
#include <avr/pgmspace.h>
#define SUI_PLATFORM_STATICSTRING_LENGTH(s)		strlen_P(reinterpret_cast<PGM_P>(s))

#else

#define SUI_PLATFORM_STATICSTRING_LENGTH(s)		strlen(s)
#endif


namespace Comm {

typedef Stream SourceBase;
typedef Print SinkBase;
#ifdef DESKTOP_COMPILE
typedef GlobalSerialWrapper SourceType;
#else
typedef HardwareSerial SourceType;
#endif


}

}




#endif /* SERIALUIV3_SRC_INCLUDES_PLATFORM_SUIPLATARDUINO_H_ */
