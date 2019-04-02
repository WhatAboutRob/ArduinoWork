

/*
 * callbacks.cpp -- part of the SoundTest project.
 * Implementation of callbacks -- YOUR code goes here!
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



/* 
 * In addition to any custom globals you declared,
 * here you have access to:
 * 
 * *** MySUI -- the SerialUI instance.
 * Use it as you would the Serial device, e.g.
 * 	MySUI.println(F("Helloooo..."));
 * 	
 * 	
 * *** MyInputs -- a container for 
 * values submitted by users. Contents:
 *  
 *    MyInputs.SetTime (Time)
 *    MyInputs.StartTest (Toggle)
 *  
 *  
 *  
 * 
 */






/* ********* callbacks and validation functions ********* */



/* *** Main Menu *** */
namespace MainMenu {


void doBuzzerTest() {

    /* Beep triggered */
    MySUI.println(F("Beep triggered!"));

}

void SetTimeChanged() {

   /* Set Time value was modified.
    * It is a unsigned long int accessible in MyInputs.SetTime
    * The raw value, MyInputs.SetTime, is some ugly uint32, 
    * instead, we use the timeValue() member method to get a
    * Time::TimeElements
    */
    
    SerialUI::Menu::Item::Request::Time::TimeElements te = MyInputs.SetTime.timeValue();
    
    MySUI.print(F("Time is set to "));
    
    if (te.hour < 10)
        MySUI.print('0');
    MySUI.print((int)te.hour);
    
    MySUI.print(':'); 
    
    if (te.minute < 10)
        MySUI.print('0');
    MySUI.print((int)te.minute);
    
    MySUI.print(':');
    
    if (te.second < 10)
        MySUI.print('0');
    MySUI.println((int)te.second);


}

void StartTestChanged() {

    /* StartTest value was modified.
    * It is a bool accessible in MyInputs.StartTest
    */
    MySUI.print(F("StartTest is now:"));
    MySUI.println(MyInputs.StartTest ? F("ON") : F("OFF"));

}






} /* namespace MainMenu */




