/* (c) Copyright 2006 International Game Technology */

/*
   $Id: RMLPFlashFlow.hpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Defines the flow for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      Jason Kremer               May 03, 2006      New
*/

#ifndef __RMLPFLASHFLOW_HPP__
#define __RMLPFLASHFLOW_HPP__

#ifndef __GAMEFLOW_HPP__
#include "GameFlow.hpp"
#endif

#ifndef __RMLPFLASHESSENTIALRECORD_HPP__
#include "RMLPFlashEssentialRecord.hpp"
#endif
#ifndef __EVENTPOSTER_HPP__
#include "EventPoster.hpp"
#endif

class RMLPFlashFlow : public GameFlow
{
   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// UnknownInterface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Initialize the object for use.
      virtual void postConstructor (void);
      // Prepare the object for destruction.
      virtual void preDestructor (void);

      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// GameFlowInterface /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Overidden states.
      virtual void enterStateStart (State &state);
      virtual void buildHistoryList(int8 card, int8 pick, bool final);

      /// <summary># Enter the evaluate state.</summary>
      virtual void enterStateEvaluateComplete (State &state);

      /// <summary># Process the signalling of a completed state from the presentation.</summary>
   //   virtual bool processStateComplete (State &state, const GameStateValue::StateValue& completed_state);


      ////////////////////////////////////////////////////////////////////////////
      /////////////////////  Serializer Support  /////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////
      // Return the RMLPFlash-specific essential record pointer
      RMLPFlashEssentialRecord *getRMLPFlashEssentialRecord (void) const;
      RMLPFlashEssentialRecord *rmlp_flash_essential_record_ptr;     // ptr to object that contains essential record data
      // Construct the object that will contain and unserialize the essential record data.
      virtual void constructEssentialRecord (void);

   public:

      // Event methods
      virtual bool processSequenceEvent (State &state, const SequenceEvent &event);

      /// <summary># Tell the game to end.</summary>
      virtual bool end (class State &state, bool meters_finalized);
      virtual int64 getBonusId(void);
      virtual String getHistoryBonusUri(void);
      virtual String getNormalBonusUri(void);
      virtual bool getAbortEBGFlag(void);

      SequenceEventPoster sequence_event_poster;

   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// Script Interface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Name of object used in the scripts
      static const char *const object_name;
      // Return the name of this scriptable object.
      virtual const char *whoAreYou (void) const;

      // Execute a function given its id.
      virtual bool execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list);
      // Script function IDs.  Base class script IDs start at 0.  Derived classes must start at MAX_FUNCTION_ID.
      enum
      {
         // MAX_FUNCTION_ID must always be last in the list.
         MAX_FUNCTION_ID = GameFlow::MAX_FUNCTION_ID,
      };

      int16 last_draw_history_size;

   public:
      IMPLEMENT_INTERFACE_BEGIN (RMLPFlashFlow)
         IMPLEMENT_INTERFACE (GameFlowInterface)
         IMPLEMENT_INTERFACE (GameFlowDebugInterface)
         CHAIN_PARENT_INTERFACE (GameFlow)
      IMPLEMENT_INTERFACE_END ()
};

#endif
