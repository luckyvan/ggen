/* (c) Copyright 2002 International Game Technology */

/*
   $Id: RMLPFlashPresentationMain.cpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the main for the SuperSpinBonusPresentation shared object.
      This is a unique file that exports the objects from the shared object.

   Modifications:
      Author            Date       Explanation
      ---------------   ---------- ------------
      Jason Kremer      Aug 08, 04 New
*/

#ifndef __GAMECONTROLPANEL_HPP__
#include "GameControlPanel.hpp"
#endif
#ifndef __GAMEPRESENTATIONRESOURCEDEFS_HPP__
#include "GamePresentationResourceDefs.hpp"
#endif
#ifndef __PRESENTATIONSTATE_HPP__
#include "PresentationState.hpp"
#endif
#ifndef __RMLPFLASHPRESENTATION_HPP__
#include "RMLPFlashPresentation.hpp"
#endif
#ifndef __SHAREDOBJECTMAIN_HPP__
#include "SharedObjectMain.hpp"
#endif

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Required Shared Object Information
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

DECLARE_MAIN_OBJECT_BEGIN ()
   DECLARE_OBJECT (HEAP, RMLPFlashPresentation, "PresentationExtension")
   DECLARE_OBJECT (HEAP, GameControlPanel, GAME_CONTROL_PANEL)
   DECLARE_OBJECT (HEAP, PresentationState, "PresentationState")
DECLARE_MAIN_OBJECT_END ()

EXPORT_MAIN_OBJECT ()
