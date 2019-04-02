/*
 * TestBasic.cpp
 *
 *  Created on: May 29, 2018
 *      Author: Pat Deegan
 *
 *  TestBasic is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */


#ifdef SERIALUI_BUILD_TESTS

#include "includes/test/TestBasic.h"
#include "SerialUI.h"

//define SUI_OLDSCHOOL
namespace SerialUI {
namespace Test {

static SerialUI MySUI(6);



void hoChanged();

void boundedChanged(); // forward decl
void optChanged();



#ifdef SUI_OLDSCHOOL

float myFloat;
String myStr;

long int bLongReq;
long int optLong;

#else

Tracked::Float myFloat(SUI_STR("Temp"), 22.0);
Tracked::String myStr(SUI_STR("State"), "this is a test");


typedef struct MyInputsStruct {
	::SerialUI::Menu::Item::Request::Long longReq;
	::SerialUI::Menu::Item::Request::BoundedLong bLongReq;
	::SerialUI::Menu::Item::Request::OptionsList optList;
	::SerialUI::Menu::Item::Request::OptionsList optList2;
	MyInputsStruct() :
		longReq(1234,
				SUI_STR("long"), SUI_STR("Ho value"), hoChanged),
				bLongReq(5, 100, 20, SUI_STR("blong"), SUI_STR("bounded value"), boundedChanged),
				optList(1, SUI_STR("opts"),
						SUI_STR("optionlist value"),
						SUI_STR("First is wurst"),
						SUI_STR("Second is reckon'd"),
						SUI_STR("This is the wurd"),
						NULL,
						NULL,
						NULL,
						optChanged),

						optList2(1, SUI_STR("more"),
								SUI_STR("some more opts"),
								SUI_STR("ABC, 123"),
								SUI_STR("Clumsy and the kytels"),
								SUI_STR("FunkyDelic Prince"),
								SUI_STR("Jack & the Jackals"),
								SUI_STR("Sweet Todd"),
								NULL,
								optChanged)
				{

				}
}MyInputsContainer;

MyInputsContainer MyInputs;


#endif

#define CREATECALLBACK(name, desc) \
	void name() { \
	MySUI.println(SUI_STR(desc)); \
}

CREATECALLBACK(callbackA, "CB A!");
CREATECALLBACK(callbackB, "Now, B!");
CREATECALLBACK(callbackC, "Now, C!");

void upTemp() {
	myFloat += 1.5;
	MySUI.print(SUI_STR("Temp is now: "));
	MySUI.println(myFloat);

}

void hoChanged() {
	MySUI.print(SUI_STR("LONG CHANGE TO: "));
	MySUI.println(MyInputs.longReq);
}



void boundedChanged() {

	MySUI.print(SUI_STR("BLONG CHANGE TO: "));
	MySUI.println(MyInputs.bLongReq);


}

void optChanged() {

	MySUI.print(SUI_STR("OPT CHANGE TO: "));
#ifdef SUI_OLDSCHOOL
	MySUI.println(optLong);
#else
	MySUI.println(MyInputs.optList.currentSelection());
#endif
}


Basic::Basic() {
	// TODO Auto-generated constructor stub

}

#define DIE_HORRIBLY(...) for (;;) { MySUI.println(__VA_ARGS__); delay(2000); }

bool Basic::setup() {

	MySUI.begin(57600);
	Serial.begin(57600);
	  // basic SUI setup
		MySUI.setGreeting(SUI_STR("Test greets"));
		// SerialUI acts just like (is actually a facade for)
		// Serial.  Use _it_, rather than Serial, throughout the
		// program.
		// basic setup of SerialUI:
		// MySUI.begin(57600); // serial line open/setup
		// MySUI.setTimeout(25000);   // timeout for reads (in ms), same as for Serial.
		MySUI.setMaxIdleMs(40000);    // timeout for user (in ms)
		// how we are marking the "end-of-line" (\n, by default):
		MySUI.setReadTerminator('\n');
		// project UID -- will be used to remember state in Druid...
		MySUI.setUID(SUI_STR("08a0574074914bed8e8d0c039c85c353N4AD2BFZEU"));

#ifdef SUI_OLDSCHOOL

		MySUI.trackState(
			SUI_STR("Da float"),
			&(myFloat));

		MySUI.trackState(
			SUI_STR("Da String"),
			&(myStr));

#else
		// a few error messages we hopefully won't need
		MySUI.trackState(myFloat);
		MySUI.trackState(myStr);
#endif

		StaticString CouldntCreateMenuErr = F("Could not create menu?");
		StaticString CouldntAddItemErr = F("Could not add item?");

		// get top level menu, to start adding items
		Menu::Menu * topMenu = MySUI.topLevelMenu();
		if (! topMenu ) {
			// well, that can't be good...
			MySUI.returnError(F("Very badness in sEriALui!1"));
			return false;
		}


#ifdef SUI_OLDSCHOOL
		topMenu->addRequest(
			&(ho),
			SUI_STR("ho"),
			SUI_STR("Ho Value"),
			hoChanged);


		topMenu->addRequest(
			&(bLongReq),
			SUI_STR("ho"),
			SUI_STR("Ho Value"),
			hoChanged);
		topMenu->addRequest(
			&(bLongReq),
			SUI_STR("Yo"),
			SUI_STR("yo"),
			5,
			100,
			boundedChanged);

		topMenu->addRequest( &(optLong),
			SUI_STR("opts"),
					SUI_STR("optionlist value"),
					SUI_STR("First is wurst"),
					SUI_STR("Second is reckon'd"),
					SUI_STR("This is the wurd"),
					NULL,
					NULL,
					NULL,
					optChanged
					);

#else

		topMenu->addText(SUI_STR("The inputs!"));
		topMenu->addRequest(MyInputs.longReq);
		topMenu->addRequest(MyInputs.optList2);
		topMenu->addRequest(MyInputs.bLongReq);
		topMenu->addRequest(MyInputs.optList);
		topMenu->addText(SUI_STR("Other stuff..."));
#endif

		if( ! topMenu->addCommand(
			SUI_STR("Stock Levels"),
			callbackA,
			SUI_STR("Show stock levels"))) {
			MySUI.returnError(CouldntAddItemErr);
			return false;
		}



		if( ! topMenu->addCommand(
			SUI_STR("up temp"),
			upTemp,
			SUI_STR("increase temp"))) {
			MySUI.returnError(CouldntAddItemErr);
			return false;
		}



#ifndef SUI_OLDSCHOOL
		topMenu->addText(SUI_STR("Subs"), SUI_STR("More stuff in submenus"));
#endif

	  // create all our sub-menus
		Menu::Menu * submen1 = topMenu->subMenu(
			SUI_STR("Tigers"),
			SUI_STR("Rarrr"));
		if (! submen1 ) {
			DIE_HORRIBLY(CouldntCreateMenuErr);
		}

		Menu::Menu * submen2 = topMenu->subMenu(
			SUI_STR("Monkeys"), NULL);
		if (! submen2 ) {
			DIE_HORRIBLY(CouldntCreateMenuErr);
		}


	  // add everything to the right sub-menu

		/* *** Animals -> Tigers *** */


		if( ! submen1->addCommand(
			SUI_STR("Empty Litter"),
			callbackB,
			SUI_STR("Clean it up..."))) {
			MySUI.returnError(CouldntAddItemErr);
			return false;
		}

		if( ! submen1->addCommand(
			SUI_STR("Feed"),
			callbackC,
			SUI_STR("Feed the tigers"))) {
			MySUI.returnError(CouldntAddItemErr);
			return false;
		}


		/* *** Animals -> Monkeys *** */


		if( ! submen2->addCommand(
			SUI_STR("Feed"),
			callbackA,
			SUI_STR("Feed the monkeys"))) {
			MySUI.returnError(CouldntAddItemErr);
			return false;
		}


		return true;
}
bool Basic::run() {

   for (;;) {
   if (MySUI.checkForUser(100)) {

	 while (MySUI.userPresent()) {
	      // actually respond to requests, using
	      MySUI.handleRequests();

	      // you could add a quick task here, to perform
	      // after *every* request, but it's better to use
	      // the setUserPresenceHeartbeat-related methods
	    }
	  }

	}
	  return true;
}


} /* namespace Test */
} /* namespace SerialUI */


#endif
