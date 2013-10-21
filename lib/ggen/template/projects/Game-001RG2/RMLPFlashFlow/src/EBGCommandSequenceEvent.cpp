/* (c) Copyright 2003 International Game Technology */

/*
   $Id: EBGCommandSequenceEvent.cpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Defines the sequence events for the EBG Bonus.

   Modifications:
      Author                  Date           Explanation
      -------------------     -----------    --------------------
      Will Merryman          Sep 26, 08     Created.
*/

#ifndef __EBGCOMMANDSEQUENCEEVENT_HPP__
#include "EBGCommandSequenceEvent.hpp"
#endif

// Align event structures
#include "pack4.h"

// Contstructor.
EBGCommandSequenceEvent::EBGCommandSequenceEvent (uint32 command_type, uint8 dest)
   : SequenceEvent (SequenceEvent::EBG_COMMAND_SEQUENCE_EVENT, dest)
{
   setData (EBGCommandSequenceEvent::EBG_COMMAND_INDEX, command_type);
}

// Event methods.
uint32 EBGCommandSequenceEvent::commandType (void) const
{
   return getData (EBGCommandSequenceEvent::EBG_COMMAND_INDEX);
}

#include "packpop.h"
