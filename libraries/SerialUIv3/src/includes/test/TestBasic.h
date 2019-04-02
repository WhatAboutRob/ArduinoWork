/*
 * TestBasic.h
 *
 *  Created on: May 29, 2018
 *      Author: Pat Deegan
 *
 *  TestBasic is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_TEST_TESTBASIC_H_
#define SERIALUI_SRC_INCLUDES_TEST_TESTBASIC_H_

#include "../SerialUIConfig.h"

#ifdef SERIALUI_BUILD_TESTS

namespace SerialUI {
namespace Test {

class Basic {
public:
	Basic();
	bool setup();
	bool run();
};

} /* namespace Test */
} /* namespace SerialUI */

#endif

#endif /* SERIALUI_SRC_INCLUDES_TEST_TESTBASIC_H_ */
