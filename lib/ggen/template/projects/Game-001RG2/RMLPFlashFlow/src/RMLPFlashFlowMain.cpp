/* (c) Copyright 2003 International Game Technology */

/*
   $Id: RMLPFlashFlowMain.cpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Defines the main for the DoubleUpFlow shared object.
      This is a unique file that exports the objects from the shared object.

   Modifications:
      Author                  Date           Explanation
      -------------------     -----------    --------------------
      Jason Kremer            July 18, 03     Created.
*/

#ifndef __RMLPFLASHFLOW_HPP__
#include "RMLPFlashFlow.hpp"
#endif
#ifndef __SHAREDOBJECTMAIN_HPP__
#include "SharedObjectMain.hpp"
#endif
#ifndef __GAMEPRESENTATIONCLIENT_HPP__
#include "GamePresentationClient.hpp"
#endif


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Required Shared Object Information
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
DECLARE_MAIN_OBJECT_BEGIN ()
   DECLARE_OBJECT (HEAP, RMLPFlashFlow, "GameFlow")
   DECLARE_OBJECT (HEAP, GamePresentationClient, "GamePresentationClient")
DECLARE_MAIN_OBJECT_END ()

EXPORT_MAIN_OBJECT ()
