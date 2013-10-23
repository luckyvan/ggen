/* (c) Copyright 2010 International Game Technology */

/*
   $Id: WinCycleLite.hpp,v 1.3, 2012-11-29 22:29:13Z, Cassiday.Kevin$
   $Date: 11/30/2012 6:29:13 AM$
   $Revision: 4$

   Purpose:
      The win cycle lite module. This is an event driven module that sends win cycle information to
      the script side for display. Phase three rules validations are performed in this module.

   Modifications:
      Author          Date        Explanation
      --------------- ----------  ------------
      Dorothy Cheung  05 Oct 2010 New
*/

#ifndef __WIN_CYCLE_LITE_HPP__
#define __WIN_CYCLE_LITE_HPP__

#include <functional>

#ifndef __ACTORINTERFACE_HPP__
#include "ActorInterface.hpp"
#endif
#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif
#ifndef __DENOMINATION_HPP__
#include "Denomination.hpp"
#endif
#ifndef __MULTIMEDIAEVENTLISTENER_HPP__
#include "MultimediaEventListener.hpp"
#endif
#ifndef __RESOURCETEMPLATE_HPP__
#include "ResourceTemplate.hpp"
#endif
#ifndef __SCRIPTABLE_HPP__
#include "Scriptable.hpp"
#endif
#ifndef __WINANIMATIONLISTWG_HPP__
#include "WinAnimationListWG.hpp"
#endif
#ifndef __WIN_CYCLE_LITE_INTERFACE_HPP__
#include "WinCycleLiteInterface.hpp"
#endif
#ifndef __BINGOAWARDINTERFACE_HPP
#include "BingoAwardInterface.hpp"
#endif
#ifndef __BINGOMETERDATA_HPP__
#include "BingoMeterData.hpp"
#endif

/// <summary> Modified slot win entry class to include additional information for paylines grouping and additional win entry information to support this module.</summary>
class SlotWinEntryWithGrouping : public SlotWinEntry
{
   public:
      /// <summary> Bonus win amount for this entry.</summary>
      Amount bonus_win_amount;

      /// <summary> A group of paylines indexes.</summary>
      vector <uint16> award_indexes;

      /// <summary> Overload operators for the predicates to sort the winning animations list based on the
      ///   win classification types: jackpot, bonus, basic, etc, and then by the winning amount.
      ///   If the classification for two wins are the same, sort the win total in DESCENDING order.</summary>

      /// <summary> Overload the < operator to sort in ascending order.</summary>
      bool operator < (const SlotWinEntryWithGrouping &rhs) const
      {
         if (win_classification == rhs.win_classification)
         {
            return win_total < rhs.win_total;
         }
         else
         {
            return win_classification < rhs.win_classification;
         }
      }

      /// <summary> Overload the > operator to sort in descending order.</summary>
      bool operator > (const SlotWinEntryWithGrouping &rhs) const
      {
         if (win_classification == rhs.win_classification)
         {
            return win_total > rhs.win_total;
         }
         else
         {
            return win_classification > rhs.win_classification;
         }
      }

      /// <summary> Overload the == operator for this class.</summary>
      bool operator == (const SlotWinEntryWithGrouping &rhs) const
      {
         if (SlotWinEntry::operator == (rhs))
         {
            return award_indexes == rhs.award_indexes;
         }
      }

      /// <summary> Overload the != operator for this class.</summary>
      bool operator != (const SlotWinEntryWithGrouping &rhs) const
      {
         return !(operator == (rhs));
      }
};

typedef list <SlotWinEntryWithGrouping> SlotWinningEntryWithGroupingList;
typedef list <SlotWinEntryWithGrouping>::iterator SlotWinningEntryWithGroupingListIterator;

/// <summary> An event based win cycle lite class.</summary>
class WinCycleLite :
   public ResourceTemplate<WinCycleLiteInterface>,
   public Scriptable
{
   protected:
/// @name UnknownInterface
/// <summary> UnknownInterface implementation</summary>
/// @{
      /// <summary>Initializes the object for use.</summary>
      virtual void postConstructor (void);
      /// <summary>Prepares the object for destruction.</summary>
      virtual void preDestructor (void);
/// @}

      /// <summary>
      /// Script function IDs.
      /// </summary>
      enum
      {
         SET_ACTOR_FOR_EVENT_QUEUE = 0,
         MAX_FUNCTION_ID // must be at the end of the enum
      };

      /// <summary>
      /// Object function IDs.
      /// </summary>
      enum
      {
         WIN_ANIMATION_LIST = 0,
         MAX_OBJECT_ID // must be at the end of the enum
      };

   public:
/// @name ResourceInterface module
/// <summary> ResourceInterface implementation</summary>
/// @{
      /// <summary> Gets the name of this resource type.</summary>
      virtual const char *resourceType (void) const { return whoAreYou(); }
      /// <summary> Gets whether or not this resource type is shareable.</summary>
      virtual bool shareable (void) const { return true; }
/// @}

/// @name Scriptable module
/// <summary> Scriptable implementation</summary>
/// @{
      /// <summary> Executes a function based on the function_id.</summary>
      virtual bool execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list);
      /// <summary> Creates a scriptable object given its id.</summary>
      virtual bool createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, ScriptParameterList& parameter_list, bool new_declared);
      /// <summary> Returns the name of this object.</summary>
      virtual const char *whoAreYou (void) const;
/// @}

/// @name WinCycleLiteInterface
/// <summary> WinCycleLiteInterface implementation</summary>
/// @{
      /// <summary> Displays the selected payline(s).</summary>
      virtual void displaySelectedPaylines (const Mojo <BetInterface> &bet);
      /// <summary> Starts or stops the win cycle.</summary>
      virtual void displayWin (const bool show, const bool start_new_win_cycle);
      /// <summary> Initializes this module including the option to display paylines in groups.</summary>
      virtual void init (const bool _group_payline_display = false);
      /// <summary> Updates the winning animations list and the win outcome for win cycle displays.</summary>
      virtual void updateWinList (Mojo <SlotAwardInterface> & slot_award,
         Mojo <SlotEvaluatorInterface> & slot_evaluator, Mojo <AwardInterface> & award_list,
         const Denomination & denomination, Mojo<BetInterface> & bet);
/// @}

   protected:
/// @name WinCycleLite module
/// <summary> WinCycleLite implementation</summary>
/// @{
      /// <summary> Adds an action and the associated data to the passed payload.</summary>
      virtual void addWinCycleLiteAction (ScriptTableParameter & payload, const char * action, ScriptTableParameter * action_data = 0);
      /// <summary> Applies the phase three symbol rules on the winning animation list created. Populate the phase three symbol rules structure in this class with the actions to execute.</summary>
      virtual void checkSymbolRules (Mojo<SlotAwardInterface> & slot_award_interface,
                                     Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                     Mojo<AwardInterface> & award_list,
                                     const Denomination & denomination,
                                     Mojo<BetInterface> & bet);
      /// <summary> Updates the winning animations list with the slot win entries for multiway wins.</summary>
      virtual void createMultiWayWinList (const Mojo <SlotAwardInterface> & slot_award,
                                          const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                          const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the winning animations list with the slot win entries for pattern wins.</summary>
      virtual void createPatternWinList (const Mojo <SlotAwardInterface> & slot_award,
                                         const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                         const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the winning animations list with the slot win entries for paylines wins. Grouping paylines displays as necessary.</summary>
      virtual void createPaylinesWinList (const Mojo <SlotAwardInterface> & slot_award,
                                          const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                          const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the winning animations list with the slot win entries for paylines wins. Grouping paylines displays as necessary.</summary>
      virtual void createRMLPWinList (const Mojo <SlotAwardInterface> & slot_award,
                                          const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                          const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the winning animations list with the slot win entries for scatter wins.</summary>
      virtual void createScatterWinList (const Mojo <SlotAwardInterface> & slot_award,
                                         const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                         const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the winning animations list and the win outcome for win cycle displays.</summary>
      virtual void createWinList (const Mojo <SlotAwardInterface> & slot_award,
                                  const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                  const Mojo <AwardInterface> & award_list);
      /// <summary> Updates the bingo win structure with the displayable win ammounts awarded by bingo.</summary>
      virtual void createBingoWinList (const Mojo <AwardInterface> & award_list);
      /// <summary> Adds the individual bingo wins to the bingo_win_entries table.</summary>
      virtual void formatBingoWinTable (ScriptTableParameter &bingo_win_entries_table, const Denomination & denomination);
      /// <summary> Format a win Ammount to a localized currency string.</summary>
      virtual void formatAmountToMonetaryString (const Amount &win_amount, String &win_amount_string);
      /// <summary> Format a win Ammount to a credit string.</summary>
      virtual void formatAmountToCreditString (const Amount &win_amount, String &win_credit_string, const Denomination & denomination);
      /// <summary> Posts win information to the script side for animations.</summary>
      virtual void postWinList (Mojo<BetInterface> & bet, const Denomination & denomation);
      /// <summary> Posts an event to the event driven client to perform a specific action defined in the payload.</summary>
      virtual void postWinCycleLiteAction (ScriptTableParameter & payload);

      /// <summary> Boolean flag that indicates whether or not to group paylines displays.</summary>
      /// <remarks> Lines that trigger the same category and trigger the exact same symbol coordinates are grouped.</summary>
      bool group_payline_display;
      /// <summary> An event queue where events from this module is posted to.</summary>
      Mojo<MultimediaEventQueueInterface> event_queue;
      /// <summary> Symbol rule animation list that stores the actions to execute based on the rule validations.</summary>
      Mojo<WinAnimationListWG, ScriptInterface> p3r_symbol_rules;
      /// <summary> Boolean flag that indicates whether or not the win outcome information needs to be updated and to be send to the script.</summary>
      bool update_win_list;

      /// <summary> A list consisting of information for win animation displays.</summary>
      SlotWinningEntryWithGroupingList winning_animations_list;

      /// <summary> Boolean flag to indicte if the game is using Bingo
      bool bingo_game;
      /// <summary> Structure to hold the ammounts of the bingo win types
      BingoMeterData bingo_meters;

/// @name WinOutcome
///	<summary> Win information for win cycle intro display.</summary>
/// @{
      typedef struct
      {
         Amount total_win_amount;
         vector <uint16> winning_paylines;
         map <Coords, String> winning_symbols;
      } WinOutcome;
      WinOutcome win_outcome;
/// @}

   private:
      /// <summary> Adds a script table into an existing script table.</summary>
      void addScriptTableParameter (ScriptTableParameter & payload, ScriptTableParameter & data,
                                    const char * _table_entry_name = 0);
      /// <summary> Clears the win outcome and the winning animation list.</summary>
      void clearWinOutcome ();
      /// <summary> Searches for an existing payline display group in the winning animations list that the payline may belong in.</summary>
      bool findSlotWinEntryGroup (const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                  const Mojo <SlotAwardInterface> & slot_award,
                                  const uint32 award_index, int16 sub_award_index,
                                  SlotWinningEntryWithGroupingListIterator & winning_entry_iter);
      /// <summary> Converts the win type enumeration index into a win type string.</summary>
      void getWinTypeString (const SlotWinEntry::WinType win_type, String & win_type_string);

   public:
/// @name Scriptable Functions
/// @{
      /*! @appendScriptableFunctionsPage{WinCycleLite}
          @addFunctionLink{WinCycleLite, setActorForEventQueue} */
      /// <summary> Sets the event queue for this module where events are posted. The event queue is set from the actor passed in.</summary>
      void setActorForEventQueue (const Mojo <ActorInterface> &actor);
/// @}

      /// <summary> Prints out debugging information on the winning_animations_list and the win_outcome.</summary>
      friend ostream& operator << (ostream & o, WinCycleLite & win_cycle_lite);
/// @}

      IMPLEMENT_INTERFACE_BEGIN (WinCycleLite)
         IMPLEMENT_INTERFACE (ScriptInterface)
         CHAIN_PARENT_INTERFACE (ResourceTemplate<WinCycleLiteInterface>)
         CHAIN_PARENT_INTERFACE (Scriptable)
      IMPLEMENT_INTERFACE_END ()
};

#endif
