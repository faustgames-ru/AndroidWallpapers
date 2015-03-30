// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// OpenSteerManager
//
// This class encapsulates the state of the OpenSteerManager application and the
// services it provides to its plug-ins.  It is never instantiated, all of
// its members are static (belong to the class as a whole.)
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#include <algorithm>
#include <sstream>
#include "UnitManagement\OpenSteer.h"

// ----------------------------------------------------------------------------
// currently selected plug-in (user can choose or cycle through them)


OpenSteer::PlugIn* OpenSteer::OpenSteerManager::selectedPlugIn = NULL;


// ----------------------------------------------------------------------------
// currently selected vehicle.  Generally the one the camera follows and
// for which additional information may be displayed.  Clicking the mouse
// near a vehicle causes it to become the Selected Vehicle.


// ----------------------------------------------------------------------------
// phase: identifies current phase of the per-frame update cycle


int OpenSteer::OpenSteerManager::phase = OpenSteer::OpenSteerManager::overheadPhase;

// ----------------------------------------------------------------------------
// XXX apparently MS VC6 cannot handle initialized static const members,
// XXX so they have to be initialized not-inline.


const int OpenSteer::OpenSteerManager::overheadPhase = 0;
const int OpenSteer::OpenSteerManager::updatePhase = 1;


// ----------------------------------------------------------------------------
// initialize OpenSteerManager application

namespace {

    void printPlugIn (OpenSteer::PlugIn& pi) {std::cout << " " << pi << std::endl;} // XXX

} // anonymous namespace

void 
OpenSteer::OpenSteerManager::initialize(void)
{
    // select the default PlugIn
    selectDefaultPlugIn ();

    // initialize the default PlugIn
    openSelectedPlugIn ();
}


// ----------------------------------------------------------------------------
// main update function: step simulation forward and redraw scene


void OpenSteer::OpenSteerManager::update(const float currentTime, const float elapsedTime)
{
    //  start the phase timer (XXX to accurately measure "overhead" time this
    //  should be in displayFunc, or somehow account for time outside this
    //  routine)
    initPhaseTimers (currentTime);

    // run selected PlugIn (with simulation's current time and step size)
	updateSelectedPlugIn(currentTime, elapsedTime);
}


// ----------------------------------------------------------------------------
// exit OpenSteerManager with a given text message or error code


void 
OpenSteer::OpenSteerManager::errorExit(const char* message)
{
    exit (-1);
}


void 
OpenSteer::OpenSteerManager::exit(int exitCode)
{
    ::exit (exitCode);
}


// ----------------------------------------------------------------------------
// select the default PlugIn


void 
OpenSteer::OpenSteerManager::selectDefaultPlugIn(void)
{
    PlugIn::sortBySelectionOrder ();
    selectedPlugIn = PlugIn::findDefault ();
}


// ----------------------------------------------------------------------------
// select the "next" plug-in, cycling through "plug-in selection order"


void 
OpenSteer::OpenSteerManager::selectNextPlugIn(void)
{
    closeSelectedPlugIn ();
    selectedPlugIn = selectedPlugIn->next ();
    openSelectedPlugIn ();
}


// ----------------------------------------------------------------------------
// handle function keys an a per-plug-in basis


void 
OpenSteer::OpenSteerManager::functionKeyForPlugIn(int keyNumber)
{
    selectedPlugIn->handleFunctionKeys (keyNumber);
}


// ----------------------------------------------------------------------------
// return name of currently selected plug-in


const char* 
OpenSteer::OpenSteerManager::nameOfSelectedPlugIn(void)
{
    return (selectedPlugIn ? selectedPlugIn->name() : "no PlugIn");
}


// ----------------------------------------------------------------------------
// open the currently selected plug-in


void 
OpenSteer::OpenSteerManager::openSelectedPlugIn(void)
{
    selectedPlugIn->open ();
}


// ----------------------------------------------------------------------------
// do a simulation update for the currently selected plug-in


void 
OpenSteer::OpenSteerManager::updateSelectedPlugIn(const float currentTime,
                                                const float elapsedTime)
{
    // switch to Update phase
    pushPhase (updatePhase, currentTime);

    // service queued reset request, if any
    doDelayedResetPlugInXXX ();

    // invoke selected PlugIn's Update method
    selectedPlugIn->update (currentTime, elapsedTime);

    // return to previous phase
    popPhase (currentTime);
}


// ----------------------------------------------------------------------------
// close the currently selected plug-in


void 
OpenSteer::OpenSteerManager::closeSelectedPlugIn(void)
{
    selectedPlugIn->close ();
}


// ----------------------------------------------------------------------------
// reset the currently selected plug-in


void 
OpenSteer::OpenSteerManager::resetSelectedPlugIn(void)
{
    selectedPlugIn->reset ();
}


namespace {

    // ----------------------------------------------------------------------------
    // XXX this is used by CaptureTheFlag
    // XXX it was moved here from main.cpp on 12-4-02
    // XXX I'm not sure if this is a useful feature or a bogus hack
    // XXX needs to be reconsidered.


    bool gDelayedResetPlugInXXX = false;

} // anonymous namespace
    
    
void 
OpenSteer::OpenSteerManager::queueDelayedResetPlugInXXX(void)
{
    gDelayedResetPlugInXXX = true;
}


void 
OpenSteer::OpenSteerManager::doDelayedResetPlugInXXX(void)
{
    if (gDelayedResetPlugInXXX)
    {
        resetSelectedPlugIn ();
        gDelayedResetPlugInXXX = false;
    }
}


// ----------------------------------------------------------------------------
// return a group (an STL vector of AbstractVehicle pointers) of all
// vehicles(/agents/characters) defined by the currently selected PlugIn


const OpenSteer::AVGroup& 
OpenSteer::OpenSteerManager::allVehiclesOfSelectedPlugIn(void)
{
    return selectedPlugIn->allVehicles ();
}


// ----------------------------------------------------------------------------
// manage OpenSteerManager phase transitions (xxx and maintain phase timers)


int OpenSteer::OpenSteerManager::phaseStackIndex = 0;
const int OpenSteer::OpenSteerManager::phaseStackSize = 5;
int OpenSteer::OpenSteerManager::phaseStack[OpenSteer::OpenSteerManager::phaseStackSize];


void 
OpenSteer::OpenSteerManager::pushPhase(const int newPhase, const float currentTime)
{
    // update timer for current (old) phase: add in time since last switch
	updatePhaseTimers(currentTime);

    // save old phase
    phaseStack[phaseStackIndex++] = phase;

    // set new phase
    phase = newPhase;

    // check for stack overflow
    if (phaseStackIndex >= phaseStackSize) errorExit ("phaseStack overflow");
}


void 
OpenSteer::OpenSteerManager::popPhase(const float currentTime)
{
    // update timer for current (old) phase: add in time since last switch
	updatePhaseTimers(currentTime);

    // restore old phase
    phase = phaseStack[--phaseStackIndex];
}


// ----------------------------------------------------------------------------


float OpenSteer::OpenSteerManager::phaseTimerBase = 0;
float OpenSteer::OpenSteerManager::phaseTimers[updatePhase + 1];


void 
OpenSteer::OpenSteerManager::initPhaseTimers(const float currentTime)
{
    phaseTimers[updatePhase] = 0;
    phaseTimers[overheadPhase] = 0;
	phaseTimerBase = currentTime;
}


void 
OpenSteer::OpenSteerManager::updatePhaseTimers(const float currentTime)
{
	phaseTimers[phase] += currentTime - phaseTimerBase;
	phaseTimerBase = currentTime;
}


// ----------------------------------------------------------------------------
