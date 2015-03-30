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
// OpenSteerDemo
//
// This class encapsulates the state of the OpenSteerDemo application and
// the services it provides to its plug-ins
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: recast App class as OpenSteerDemo 
// 06-26-02 cwr: App class created 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_OPENSTEERDEMO_H
#define OPENSTEER_OPENSTEERDEMO_H


#include "UnitManagement/Vec3.h"
#include "UnitManagement/PlugIn.h"
#include "UnitManagement/Utilities.h"


namespace OpenSteer {


    class OpenSteerManager
    {
    public:
        // ------------------------------------------------------ component objects

        // ------------------------------------------ addresses of selected objects

        // currently selected plug-in (user can choose or cycle through them)
        static PlugIn* selectedPlugIn;

        // -------------------------------------------- initialize, update and exit

        // initialize OpenSteerDemo
        //     XXX  if I switch from "totally static" to "singleton"
        //     XXX  class structure this becomes the constructor
        static void initialize (void);

        // main update function: step simulation forward and redraw scene
		static void update(const float currentTime, const float elapsedTime);

        // exit OpenSteerDemo with a given text message or error code
        static void errorExit (const char* message);
        static void exit (int exitCode);

        // ------------------------------------------------------- PlugIn interface

        // select the default PlugIn
        static void selectDefaultPlugIn (void);
        
        // select the "next" plug-in, cycling through "plug-in selection order"
        static void selectNextPlugIn (void);

        // handle function keys an a per-plug-in basis
        static void functionKeyForPlugIn (int keyNumber);

        // return name of currently selected plug-in
        static const char* nameOfSelectedPlugIn (void);

        // open the currently selected plug-in
        static void openSelectedPlugIn (void);

        // do a simulation update for the currently selected plug-in
        static void updateSelectedPlugIn (const float currentTime,
                                          const float elapsedTime);

        // close the currently selected plug-in
        static void closeSelectedPlugIn (void);

        // reset the currently selected plug-in
        static void resetSelectedPlugIn (void);

        static const AVGroup& allVehiclesOfSelectedPlugIn(void);

        // ---------------------------------------------------- OpenSteerDemo phase

        static bool phaseIsUpdate   (void) {return phase == updatePhase;}
        static bool phaseIsOverhead (void) {return phase == overheadPhase;}

        static float phaseTimerUpdate   (void) {return phaseTimers[updatePhase];}

        // ------------------------------------------------------ delayed reset XXX

        // XXX to be reconsidered
        static void queueDelayedResetPlugInXXX (void);
        static void doDelayedResetPlugInXXX (void);

    private:

        static int phase;
        static int phaseStack[];
        static int phaseStackIndex;
        static float phaseTimers[];
        static float phaseTimerBase;
        static const int phaseStackSize;
		static void pushPhase(const int newPhase, const float currentTime);
		static void popPhase(const float currentTime);
		static void initPhaseTimers(const float currentTime);
		static void updatePhaseTimers(const float currentTime);

        // XXX apparently MS VC6 cannot handle initialized static const members,
        // XXX so they have to be initialized not-inline.
        // static const int drawPhase = 2;
        // static const int updatePhase = 1;
        // static const int overheadPhase = 0;
        static const int updatePhase;
        static const int overheadPhase;
    };

} // namespace OpenSteer


// ----------------------------------------------------------------------------
#endif // OPENSTEER_OPENSTEERDEMO_H
