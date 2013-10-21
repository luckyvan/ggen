/* (c) Copyright 2006 International Game Technology */

/*
   $Id: RMLPFlashFlow.cpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Implements the flow for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      Jason Kremer               May 03, 2006      New
*/
#ifndef __RMLPFLASHFLOW_HPP__
#include "RMLPFlashFlow.hpp"
#endif
#ifndef __GAMEEVENTS_HPP__
#include "GameEvents.hpp"
#endif
#ifndef __BONUSEVALUATORINTERFACE_HPP__
#include "BonusEvaluatorInterface.hpp"
#endif
#ifndef __SYSTEMAWARDINTERFACE_HPP__
#include "SystemAwardInterface.hpp"
#endif
#ifndef __RMLPFLOWINTERFACE_HPP__
#include "RMLPFlowInterface.hpp"
#endif
#ifndef __EBGCOMMANDSEQUENCEEVENT_HPP__
#include "EBGCommandSequenceEvent.hpp"
#endif

#include "assertion.h"

#ifdef __DEBUG__
   //#define __VERBOSE__
   //#define __VERBOSE_STATE__
   //#define __VERBOSE_SERIALIZE__
   //#define __VERBOSE_EVENT__
   //#define __VERBOSE_HISTORY_LIST__
   //#define __FORCE_ALL_RED_CARDS__
   //#define __CHECK_IF_WIN_LOSE_OR_TAKE_WIN__
#endif

#ifdef __VERBOSE__
 #define verboseMethodName() debug << __PRETTY_FUNCTION__ << endl;
 #define verboseGameMethodName() debug << gameName () << "|" << __PRETTY_FUNCTION__ << endl;
 #define verboseLabeledData(data)   debug << "  " << #data << ": " << (data);
#else
 #define verboseMethodName()
 #define verboseGameMethodName()
 #define verboseLabeledData(data)
#endif


// Name of object used in the scripts
const char *const RMLPFlashFlow::object_name = "RMLPFlashFlow";


// Purpose: Initialize the object for use.
void RMLPFlashFlow::postConstructor (void)
{
   // Call the base classes first.
   GameFlow::postConstructor ();
}

// Purpose: Prepare the object for destruction.
void RMLPFlashFlow::preDestructor (void)
{
   // Call the base classes last.
   GameFlow::preDestructor ();
}

// Purpose:  Execute a function given its id.
bool RMLPFlashFlow::execScriptableFunctionById (int32 function_id, ScriptParameterList& parameter_list)
{
   //verboseGameMethodName();
   #ifdef __VERBOSE_SCRIPT__
   debug << game_name << "|RMLPFlashFlow::execScriptableFunctionById (), function_id " << function_id << endl;
   #endif
   bool result = true;

   switch (function_id)
   {
      default:
         result = GameFlow::execScriptableFunctionById (function_id, parameter_list);
         break;
   }
   return result;
}

/// @StateLink{RMLPFlashFlow,enterStateStart}
void RMLPFlashFlow::enterStateStart (State &state)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashFlow::" << __FUNCTION__ << endl;
   #endif

   SerializerHelper::CriticalDataSynchron synchron (this, state);
   RMLPFlashEssentialRecord *record = getRMLPFlashEssentialRecord ();

   record->sequence_id = getBonusId();
   strcpy(record->history_game_uri ,getHistoryBonusUri());
   strcpy(record->normal_game_uri ,getNormalBonusUri());
   /// <remarks>call the base class first</remarks>
   GameFlow::enterStateStart (state);
}

// Common function for building Double Up ER history record
void RMLPFlashFlow::buildHistoryList(int8 card, int8 pick, bool final)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashFlow::" << __FUNCTION__ << endl;
   #endif
}

/// @StateLink{GameFlow,enterStateEvaluate}
/// <summary># Enter the evaluate state.</summary>
/// <param name="state">#[inout] Current state, for critical data.</param>
void RMLPFlashFlow::enterStateEvaluateComplete (State &state)
{
   #ifdef __VERBOSE_STATE__
   verboseGameMethodName();
   debug << "    RMLPFlashFlow::enterStateEvaluateComplete" << endl;
   debug << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
   debug << "gameIdentifier: " << gameIdentifier () << endl;
   debug << "stageIndex: " << stageIndex () << endl;
   debug << "actIndex: " << actIndex () << endl;
   debug << "sessionId: " << sessionId () << endl;
   debug << "gameListIndex: " << gameListIndex () << endl;
   debug << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
   #endif
}

// Return the RMLPFlashFlow-specific essential record pointer
RMLPFlashEssentialRecord *RMLPFlashFlow::getRMLPFlashEssentialRecord (void) const
{
   return (rmlp_flash_essential_record_ptr);
}

// Purpose: Return the name of this scriptable object.
const char *RMLPFlashFlow::whoAreYou (void) const
{
   return (object_name);
}

// Purpose: Process sequence events we receive.
bool RMLPFlashFlow::processSequenceEvent (State &state, const SequenceEvent &event)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashFlow::" << __FUNCTION__ << endl;
   #endif
   #ifdef __VERBOSE_EVENT__
   verboseGameMethodName();
   #endif

   bool handled (false);
   RMLPFlashEssentialRecord *record = getRMLPFlashEssentialRecord ();

   switch (event.getType ())
   {
      case SequenceEvent::EBG_COMMAND_SEQUENCE_EVENT:
         {
            const EBGCommandSequenceEvent &ebg_event = reinterpret_cast<const EBGCommandSequenceEvent &> (event);
            switch (ebg_event.commandType ())
            {
               case EBGCommandSequenceEvent::EBG_RMLP_ABORT_BONUS_CHECK:
               {
                  if(getAbortEBGFlag())
                  {
                     EBGCommandSequenceEvent rmlp_ebg_event (EBGCommandSequenceEvent::EBG_RMLP_ABORT_BONUS, SequenceEvent::TO_PRESENTATION);
                     rmlp_ebg_event.setGameStage(record->game_stage);
                     sequence_event_poster.post (rmlp_ebg_event);
                  }
                  handled = true;
               }
               break;
            }
         }
         break;
   }
   return handled ? handled : GameFlow::processSequenceEvent (state, event);
}

/// <summary># Tell the game to end.</summary>
/// <remarks># This is called by the system when the main game has ended, not when all game play has
///   ended. For example: in a Doubleup enabled slot game, this is called when the slot game has
///   finished, before the player is given the opportunity to play Doubleup.</remarks>
/// <param name="state">#[inout] Current state, for critical data.</param>
/// <param name="meters_finalized">#[in] Whether the meters have completely final. In the above example
///   this would be false, because the system is waiting for the player to enter Doubleup. If the player
///   weren't allowed to enter Doubleup, this would be true. If false, @ref finalize will be called
///   later. If true, @ref finalize will not be called.</param>
/// <returns># True if the game ends.</returns>
bool RMLPFlashFlow::end (State &state, bool meters_finalized)
{
   verboseGameMethodName();
   #ifdef __VERBOSE__
   debug << "RMLPFlashFlow::end()" << endl;
   #endif

   // Get the current state for convenience
   uint32 current_state = getState ();
   // We can end if we are in the AWARD_COMPLETE state or IDLE (already ended)
   bool can_end = (current_state == GameStateValue::STATE_AWARD_COMPLETE) ||
                  (current_state == GameStateValue::STATE_IDLE);
   // Go to IDLE if in AWARD_COMPLETE.  Its that simple.
   if (current_state == GameStateValue::STATE_AWARD_COMPLETE)
      setState (state, GameStateValue::STATE_IDLE);
   #ifdef __VERBOSE__
   if (!can_end)
   {
      #ifdef __VERBOSE__
      debug << "WARNING: GameFlow::end() returning FALSE." << endl;
      #endif
   }
   #endif
   return can_end;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////  Serializer Support  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Construct the object that will contain and unserialize the essential record data
void RMLPFlashFlow::constructEssentialRecord (void)
{
   #ifdef __VERBOSE_SERIALIZE__
      debug << "RMLPFlashFlow:" << __FUNCTION__ << endl;
   #endif

   // Destroy the current essential record (if there is one) before constructing a new one
   destructEssentialRecord ();

   // Create the object that contains essential record/critical data
   rmlp_flash_essential_record_ptr = new RMLPFlashEssentialRecord;

   // Set all ER pointers that the base classes need
   essential_record_ptr = rmlp_flash_essential_record_ptr;

   // Identify the class/type of the essential record for the serialize/unserialize functions
   // It is recommended that derived classes of essential record use an id that is different from any
   // of its base classes (for error checking purposes)
   assertion (essential_record_ptr);
   essential_record_ptr->setSerializerTypeId (RMLPFlashEssentialRecord::RMLP_FLASH_ER_SERIALIZER_TYPE_ID);
}
int64 RMLPFlashFlow::getBonusId(void)
{
   // If can get the base game flow...
   Mojo <GameFlowInterface> base_game_flow;
   if (!findRMLPInterface <GameFlowInterface>(base_game_flow, GameStage (gameIdentifier (), BASE_GAME_STAGE)))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GameFlow of a bonus cannot find the base game\'s GameFlow."));
   }

   Mojo <RMLPFlowToFlowInterface> flow2flow_interface;
   flow2flow_interface = base_game_flow;
   return flow2flow_interface->getBonusId ();

}

String RMLPFlashFlow::getHistoryBonusUri(void)
{
   // If can get the base game flow...
   Mojo <GameFlowInterface> base_game_flow;
   if (!findRMLPInterface <GameFlowInterface>(base_game_flow, GameStage (gameIdentifier (), BASE_GAME_STAGE)))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GameFlow of a bonus cannot find the history game\'s GameFlow."));
   }

   Mojo <RMLPFlowToFlowInterface> flow2flow_interface;
   flow2flow_interface = base_game_flow;
   return flow2flow_interface->getHistoryBonusUri();
}

String RMLPFlashFlow::getNormalBonusUri(void)
{
   // If can get the base game flow...
   Mojo <GameFlowInterface> base_game_flow;
   if (!findRMLPInterface <GameFlowInterface>(base_game_flow, GameStage (gameIdentifier (), BASE_GAME_STAGE)))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GameFlow of a bonus cannot find the normal base game\'s GameFlow."));
   }

   Mojo <RMLPFlowToFlowInterface> flow2flow_interface;
   flow2flow_interface = base_game_flow;
   return flow2flow_interface->getNormalBonusUri();
}

bool RMLPFlashFlow::getAbortEBGFlag(void)
{
   // If can get the base game flow...
   Mojo <GameFlowInterface> base_game_flow;
   if (!findRMLPInterface <GameFlowInterface>(base_game_flow, GameStage (gameIdentifier (), BASE_GAME_STAGE)))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GameFlow of a bonus cannot find the normal base game\'s GameFlow."));
   }

   Mojo <RMLPFlowToFlowInterface> flow2flow_interface;
   flow2flow_interface = base_game_flow;
   return flow2flow_interface->getAbortEBGFlag();
}

