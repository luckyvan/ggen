/* (c) Copyright 2001 International Game Technology */
/*
   $Id: SequenceEvent.hpp,v 1.13, 2011-03-25 15:29:31Z, Lam, Rex$
   $Date: 3/25/2011 11:29:31 PM$
   $Revision: 14$

   Purpose:
      Defines the base SequenceEvent class.

   Modifications:
      Author            Date        Explanation
      ---------------   ----------  ------------
      Dwayne Nelson     Dec 28, 98  Created.
*/

#ifndef __SEQUENCEEVENT_HPP__
#define __SEQUENCEEVENT_HPP__

#ifndef __GAMEDEFS_HPP__
#include "GameDefs.hpp"
#endif
#ifndef __IGTDEFS_HPP__
#include "IGTDefs.hpp"
#endif

// Align event structures
#include "pack4.h"

class SequenceEvent
{
   public:
      // Destination flags.  Indicate the destination(s) for a sequence event.
      typedef uint8 SequenceEventDestination;
      static const SequenceEventDestination INVALID_DESTINATION   = 0;
      static const SequenceEventDestination TO_PRESENTATION       = 0x01;
      static const SequenceEventDestination TO_FLOW               = 0x02;
      static const SequenceEventDestination TO_GAME_MANAGER       = 0x04;
      static const SequenceEventDestination TO_GAMES              = TO_PRESENTATION | TO_FLOW;
      static const SequenceEventDestination TO_ALL                = TO_PRESENTATION | TO_FLOW | TO_GAME_MANAGER;
      // Define the indices into the sequence event data.
      static const uint32 DATA_INDEX_DATA_0 = 0;
      static const uint32 DATA_INDEX_DATA_1 = 1;
      static const uint32 DATA_INDEX_DATA_2 = 2;
      static const uint32 DATA_INDEX_DATA_3 = 3;
      static const uint32 DATA_INDEX_DATA_4 = 4;
      static const uint32 DATA_INDEX_DATA_5 = 5;

      // Sequence event types
      typedef uint32 SequenceEventType;
      static const SequenceEventType INVALID_SEQUENCE_EVENT = 0;
      // Game specific sequence events should start here.
      // The idea is that if someone were to forget to make their first
      // sequence event equal to this magic value, it will still work
      // since the first standard event starts at a much higher number.
      static const SequenceEventType FIRST_GAME_SPECIFIC_SEQUENCE_EVENT = 1;

      // Game Manager specific events
      static const SequenceEventType GAME_MANAGER_SEQUENCE_EVENT  = 0x4000;
      // All generic sequence events start here.  These events are common across games
      // and can be interpreted safely by anyone in the system.
      static const SequenceEventType GENERIC_SEQUENCE_EVENT       = 0x5000;
      // Events used to communicate player input requests.
      static const SequenceEventType USER_REQUEST                 = 0x5001;
      // Used to post utility-specific events.
      static const SequenceEventType UTILITY                      = 0x5002;
      // Used to post configuration item changes requests.
      static const SequenceEventType MAIN_MENU_REQUEST            = 0x5003;
      // Used to post Bingo-specific events.
      static const SequenceEventType BINGO_SEQUENCE_EVENT         = 0x5004;
	  // Used to post EBG-specific event.
      static const SequenceEventType EBG_COMMAND_SEQUENCE_EVENT   = 0x5005;
      // This must be the last item.
      static const SequenceEventType MAX_SEQUENCE_EVENTS          = 0x1FFFFFFF;

      // Constructors
      SequenceEvent (void);
      SequenceEvent (uint32 type, uint8 dest);
      SequenceEvent (uint32 type, const char *destination);
      SequenceEvent (const SequenceEvent &other);
      SequenceEvent (const char *type, uint8 destination = 0, uint32 data_0 = 0);
      SequenceEvent (const char *type, const char *destination, uint32 data_0 = 0);
      // Assignment operator
      SequenceEvent &operator= (const SequenceEvent &other);
      // Get the game stage information for this event
      const GameStage& gameStage (void) const;
      // Get the data at the specified index.  True will be returned if the value was set
      uint32 getData (uint32 index) const;
      // Get the data at the specified index.  True will be returned if the value was set
      bool getData (uint32 index, char *data) const;
      // Get the maximum number of data indices.
      static uint32 getMaxData (void)
         { return MAX_DATA_COUNT; }
      // Get the sequence event type.  If the value passed in is set true will be returned
      uint32 getType (void) const;
      // Get the sequence event type.  If the value passed in is set true will be returned
      bool getType (const char *type) const;
      // Returns true if the event is flagged to go to the game flow, false otherwise.
      bool isForGameFlow (void) const;
      // Returns true if the event is flagged to go to the game manager, false otherwise.
      bool isForGameManager (void) const;
      // Returns true if the event is flagged to go to the game presentation, false otherwise.
      bool isForGamePresentation (void) const;
      // Set the data at the specified index.
      void setData (uint32 index, uint32 data);
      // Set the data at the specified index.  For this method to work the type must be set
      void setData (uint32 index, const char *data);
      // Sets the destination for the sequence event.
      void setDestination (uint8 destination);
      // Sets the destination for the sequence event.
      void setDestination (const char *destination);
      // Set the game stage information
      void setGameStage (const GameStage& game_stage);
      // Set the sequence event type.
      void setType (const char *type);
      // Set the sequence event type.
      void setType (uint32 type);

   protected:
      uint32 type             : 29; // Type of sequence event
      uint32 to_presentation  : 1;  // Bit set if destination is game presentation
      uint32 to_flow          : 1;  // Bit set if destination is game flow
      uint32 to_game_manager  : 1;  // Bit set if destination is game manager

      GameStage game_stage;         // Game/Stage for this event
      // Max number of event specific data entries.
      static const uint32 MAX_DATA_COUNT = 6;

   public:
      uint32 data [MAX_DATA_COUNT];               // Sequence event specific data

   friend ostream& operator << (ostream &o, const SequenceEvent &event)
   {
      GameStage game_stage = event.gameStage ();

      o << "### Seq event: " << endl;
      o << "   " <<  event.game_stage.gameIdentifier ().asString ();
      o << "   stage: " <<  game_stage.stageIndex ();
      o << "   type: 0x" << hex << event.type << dec;
      o << "   to_pres: " << event.to_presentation;
      o << "   to_flow: " << event.to_flow;
      o << "   to_gm: " << event.to_game_manager << endl;

      o << "   Data: ";
      for (uint32 index = 0; index < MAX_DATA_COUNT; ++index)
         o << hex << event.data [index] << " ";
      o << dec << endl;
      return o;
   }

};

// Restore previous alignment
#include "packpop.h"

#endif
