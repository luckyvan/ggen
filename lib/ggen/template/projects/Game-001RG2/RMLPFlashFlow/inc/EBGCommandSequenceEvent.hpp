/* (c) Copyright 2003 International Game Technology */

/*
   $Id: EBGCommandSequenceEvent.hpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
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
#define __EBGCOMMANDSEQUENCEEVENT_HPP__

#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif
#ifndef __SEQUENCEEVENT_HPP__
#include "SequenceEvent.hpp"
#endif

// Align event structures
#include "pack4.h"

// No virtual functions are allowed in the SequenceEvent.  If a virtual
// function is introduced, please review the memcpy and other code here.
class EBGCommandSequenceEvent : public SequenceEvent
{
   public:
      typedef uint32 EBGCommandType;
      static const EBGCommandType EBG_COMMAND_STOP           = 2;
      static const EBGCommandType EBG_COMPLETE_LAST_CHANCE   = 3;
      static const EBGCommandType EBG_CLEAN_PRESENTATION     = 4;
      static const EBGCommandType EBG_RMLP_UPDATE_MAINGAME_CONTENT  = 5;
      static const EBGCommandType EBG_RMLP_UPDATE_SEEPAYS_CONTENT  = 6;
      static const EBGCommandType EBG_RMLP_UPDATE_TOPGLASS_CONTENT  = 7;
      static const EBGCommandType EBG_RMLP_SUSPEND_GAME      = 8;
      static const EBGCommandType EBG_RMLP_RESUME_GAME       = 9;
      static const EBGCommandType EBG_RMLP_START_GAME        = 10;
      static const EBGCommandType EBG_RMLP_BUTTON1_RELEASED  = 20;
      static const EBGCommandType EBG_RMLP_BUTTON2_RELEASED  = 21;
      static const EBGCommandType EBG_RMLP_BUTTON3_RELEASED  = 22;
      static const EBGCommandType EBG_RMLP_BUTTON4_RELEASED  = 23;
      static const EBGCommandType EBG_RMLP_BUTTON5_RELEASED  = 24;
      static const EBGCommandType EBG_RMLP_BUTTON11_RELEASED = 25;
      static const EBGCommandType EBG_RMLP_BUTTON12_RELEASED = 26;
      static const EBGCommandType EBG_RMLP_BUTTON13_RELEASED = 27;
      static const EBGCommandType EBG_RMLP_BUTTON14_RELEASED = 28;
      static const EBGCommandType EBG_RMLP_BUTTON15_RELEASED = 29;
      static const EBGCommandType EBG_RMLP_ABORT_BONUS_CHECK = 30;
      static const EBGCommandType EBG_RMLP_ABORT_BONUS       = 31;
      static const EBGCommandType EBG_RMLP_CHANGE_LANGUAGE   = 32;
      // Define the indices into the data.
      static const uint32 EBG_COMMAND_INDEX = DATA_INDEX_DATA_0;
      static const uint32 EBG_COMMAND_NEXT_INDEX = EBG_COMMAND_INDEX + 1;
      static const uint32 EBG_BUTTON_DATA_INDEX = EBG_COMMAND_NEXT_INDEX + 1;

      // Constructor
      EBGCommandSequenceEvent (uint32 command_type, uint8 dest);
      // Event methods
      uint32 commandType (void) const;
};

// Restore previous alignment
#include "packpop.h"
#endif
