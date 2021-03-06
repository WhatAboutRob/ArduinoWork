

/*
 * setup.cpp -- part of the SoundTest project.
 * Setup of SerialUI and menu system
 * Mysterion the Anonymous
 * Rob  
 * 
 * Copyright (C) 2017 Mysterion the Anonymous
 * 
 * Generated by DruidBuilder [https://devicedruid.com/], 
 * as part of project "21086b99a1934041a9399b8a31cffed34of5fKqXxm",
 * aka SoundTest.
 * 
 * Druid4Arduino, Device Druid, Druid Builder, the builder 
 * code brewery and its wizards, SerialUI and supporting 
 * libraries, as well as the generated parts of this program 
 * are 
 *            Copyright (C) 2013-2018 Pat Deegan 
 * [https://psychogenic.com/ | https://flyingcarsandstuff.com/]
 * and distributed under the terms of their respective licenses.
 * See https://devicedruid.com for details.
 * 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE 
 * PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, 
 * YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
 * CORRECTION.
 * 
 * Keep in mind that there is no warranty and you are solely 
 * responsible for the use of all these cool tools.
 * 
 * Play safe, have fun.
 * 
 */


/* we need the SerialUI lib */
#include <SerialUIv3.h>
#include "SoundTestSettings.h"

/* our project specific types and functions are here */
#include "SoundTest.h"



/* MySUI
 * Our SerialUI Instance, through which we can send/receive
 * data from users. Actually instantiated here, for global use.
 */
SUI::SerialUI MySUI(4);


/*
 * The container for MyInputs, which
 * holds all the variables for the various inputs.
 * Actually instantiated here, for global use.
 */
MyInputsContainerSt MyInputs;










bool SetupSerialUI() {

	MySUI.setGreeting(F(serial_ui_greeting_str));
	// SerialUI acts just like (is actually a facade for)
	// Serial.  Use _it_, rather than Serial, throughout the
	// program.
	// basic setup of SerialUI:
	MySUI.begin(serial_baud_rate); // serial line open/setup
	// MySUI.setTimeout(serial_readtimeout_ms);   // timeout for reads (in ms), same as for Serial.
	MySUI.setMaxIdleMs(serial_maxidle_ms);    // timeout for user (in ms)
	// how we are marking the "end-of-line" (\n, by default):
	MySUI.setReadTerminator(serial_input_terminator);
	// project UID -- will be used to remember state in Druid...
	MySUI.setUID(SUI_STR("21086b99a1934041a9399b8a31cffed34of5fKqXxm"));

	
	
	
	
	
	// a few error messages we hopefully won't need
	
	SUI_FLASHSTRING CouldntAddItemErr = F("Could not add item?");
	
	// get top level menu, to start adding items
	SerialUI::Menu::Menu * topMenu = MySUI.topLevelMenu();
	if (! topMenu ) {
		// well, that can't be good...
		MySUI.returnError(F("Very badness in sEriALui!1"));
		return false;
	}
	
	
	
	/* *** Main Menu *** */

	
	if( ! topMenu->addText(
		SUI_STR("Sound Test"),
		NULL)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	

	
	if( ! topMenu->addCommand(
		SUI_STR("Beep"),
		MainMenu::doBuzzerTest)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	

	
	if( ! topMenu->addRequest(
		MyInputs.SetTime)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	

	
	if( ! topMenu->addRequest(
		MyInputs.StartTest)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	


	
	
	
	return true;
	
}


