/* (c) Copyright 2010 International Game Technology */

/*
   $Id: WinCycleLite.cpp,v 1.14, 2012-11-29 22:29:13Z, Cassiday.Kevin$
   $Date: 11/30/2012 6:29:13 AM$
   $Revision: 15$

   Purpose:
      The win cycle lite module. This is an event driven module that sends win cycle information to
      the script side for display. Phase three rules validations are performed in this module.

      Event structure in the module:
      The event structure used for this module consists of the multimedia event name and a payload.
      The Multimedia event name is the resource alias for this resource followed by ":Client".
      "WinCycleLite.Main:Client"
      payload = {
                  actions = {
                              "1" = {
                                      action      = "Start",
                                      start_new_win_cycle = true,
                                    },
                            },
                }

      "WinCycleLite.Main:Client"
      payload = {
                  actions = {
                              "1" = {
                                      action     = "Stop",
                                    },
                            },
                }

      "WinCycleLite.Main:Client"
      payload = {
                  actions = {
                              "1" = {
                                      action     = "Win",
                                      parameters = {
                                                    "sound" = "WW", // [optional] name of the sound specified in sound_controller:addPrioritySound from script
                                                    "total_win_amount" = 15000, // amount display based on the current denomination
                                                    "total_bet" = 200, // total bet for the game.
                                                    "animate_intro" = {
                                                                       // winning lines to show during the "PreShow" state of the win cycle.
                                                                       [OPTIONAL] "lines" = {1, 5, 10, 15},
                                                                        //    { reel index, segment index, symbol name, animation action string for phase III symbol rules
                                                                                                                        specified in the script or false if the rule does not
                                                                                                                        pass. }
                                                                        "1" = { reel = 0, visible_symbol = 0, name = "WW", animate = "intro"},
                                                                        "2" = { reel = 1, visible_symbol = 0, name = "WW", animate = "intro"},
                                                                        "3" = { reel = 2, visible_symbol = 0, name = "WW", animate = "intro"},
                                                                        "4" = { reel = 3, visible_symbol = 0, name = "WW", animate = "intro"},
                                                                        "5" = { reel = 4, visible_symbol = 0, name = "WW", animate = "intro"},
                                                                        "6" = { reel = 0, visible_symbol = 1, name = "W1", animate = false},
                                                                        "7" = { reel = 0, visible_symbol = 2, name = "W2", animate = false},
                                                                        . . .
                                                                     },
                                                    "win_entries" = {
                                                                        "1" = {
                                                                               "animate" = "pure_win_cycle" // animation action string for phase III symbol rules
                                                                                                               specified in script or false if the rule does not pass. }
                                                                               "win_type" = {
                                                                                                "1" = "payline_win", // payline_win, scatter_win, pattern_win, multi_way_win, generic
                                                                                            },
                                                                                "win_classification" = {
                                                                                                           "1" = "basic_win", // basic_win, bonus_win, jackpot_win
                                                                                                       }
                                                                                "bonus_win_amount" = 5000,
                                                                                "win_amount" = 10000, // if lines are grouped, display the win amount per line. The win amount is displayed based on the current denomination.
                                                                                [line win : if Non grouping] "lines" = 5, // one based line index
                                                                                [line win : if Grouping] "lines" = "5, 15", // one based line index, also refer to the remarks on the group_payline_display flag for further info
                                                                                [pattern win] "pattern" = "0", // zero based pattern index as specified in the paytable.
                                                                                "multiplier" = 3,
                                                                                "winning_symbols = {
                                                                                                    "1" = {
                                                                                                            reel = 0,
                                                                                                            visible_symbol = 0,
                                                                                                            name = "WW"
                                                                                                          },
                                                                                                    "2" = {
                                                                                                            reel = 1,
                                                                                                            visible_symbol = 1,
                                                                                                            name = "WW"
                                                                                                          },
                                                                                                    "3" = {
                                                                                                            reel = 2,
                                                                                                            visible_symbol = 0,
                                                                                                            name = "W1"
                                                                                                          },
                                                                                                    },
                                                                              },
                                                                        "2" = {
                                                                                 . . .
                                                                              },
                                                                     },
                                                   },
                                                   bingo_win_entries = { // Created when the game is using the Class II Bingo system
                                                                        "1" = {
                                                                               "win_type" = "interim", // "interim_progressive, bingo_prize, grand_prize
                                                                               "win_amount_string" = "1200" // "$12.00"
                                                                               "coincidental_win_amount_string" = "1200" // "$12.00", this is an optional paramater with the interim win_type
                                                                        },
                                                                        "2" = {
                                                                                 . . .
                                                                        },
                                                   },
                                    },
                            },
                }

   Modifications:
      Author          Date        Explanation
      --------------- ----------  ------------
      Dorothy Cheung  05 Oct 2010 New
*/

#ifndef __ACTORSTAGEINTERFACE_HPP__
#include "ActorStageInterface.hpp"
#endif
#ifndef __ACTUABLEINTERFACE_HPP__
#include "ActuableInterface.hpp"
#endif
#ifndef __DEBUG_HPP__
#include "Debug.hpp"
#endif
#ifndef __ULOCALE_HPP__
#include "ULocale.hpp"
#endif
#ifndef __CONFIG_HPP__
#include "Config.hpp"
#endif
#ifndef __WIN_CYCLE_LITE_HPP__
#include "WinCycleLite.hpp"
#endif

#include "assertion.h"

#ifdef __VERBOSE__
 #define verboseMethodName() debug  << __FUNCTION__ << endl;
#else
 #define verboseMethodName()
#endif

/// <summary> Prints out debugging information on the winning_animations_list and the win_outcome.</summary>
/// <remarks> This is for debugging purpose only.<remarks>
/// <param name = "o">[out] Stream object on which the insertion operator is performed.</param>
/// <param name = "win_cycle_lite">[in] The win cycle lite object.</param>
/// <returns> The output stream.</returns>
ostream& operator << (ostream & o, WinCycleLite & win_cycle_lite)
{
   verboseMethodName ();
#ifdef __VERBOSE__
    o << "     ***** SlotWinningEntryWithGroupingList: *****" << endl;
   for (SlotWinningEntryWithGroupingListIterator iter = win_cycle_lite.winning_animations_list.begin ();
        iter != win_cycle_lite.winning_animations_list.end ();
        ++iter)
   {
      o << "    award_index: " << iter->award_index << endl;
      o << "    sub_award_index: " << iter->sub_award_index << endl;
      o << "    win_categroy: " << iter->win_category << endl;
      o << "    win_total: " << iter->win_total << endl;
      o << "    base_win: " << iter->base_win << endl;
      o << "    multiplier: " << iter->multiplier << endl;
      o << "    win classification: " << iter->win_classification << endl;
      o << "    win type: " << iter->win_type << endl;
      for (SlotWinningAnimationEntryListIterator iter2 = iter->begin ();
           iter2 != iter->end(); ++iter2)
      {
         o << "      reel_number: " << iter2->reel_number << endl;
         o << "      offset_from_home_position: " << iter2->offset_from_home_position << endl;
         o << "      symbol_name: " << iter2->symbol_name << endl;
      }
      o << "   number of paylines grouped: " << iter->award_indexes.size () << endl;
      for (uint32 index = 0; index != iter->award_indexes.size(); ++index)
      {
         o << "      winning_line: " << iter->award_indexes[index] << endl;
      }
   }

   o << "     ***** win outcome table: *****" << endl;
   o << "     winning symbols:" << endl;
   for (map<Coords, String>::iterator winning_symbol = win_cycle_lite.win_outcome.winning_symbols.begin ();
        winning_symbol != win_cycle_lite.win_outcome.winning_symbols.end(); ++winning_symbol)
   {
      Coords coords = { winning_symbol->first.reel, winning_symbol->first.offset };

      o << "         symbol name: " << winning_symbol->second << endl;
      o << "         reel: " << coords.reel << endl;
      o << "         offset: " << coords.offset << endl;
   }
   o << "      winning paylines:" << endl;
   for (vector <uint16>::iterator paylines_iter = win_cycle_lite.win_outcome.winning_paylines.begin ();
        paylines_iter != win_cycle_lite.win_outcome.winning_paylines.end (); ++paylines_iter)
   {
      o << "         payline: " << *paylines_iter << endl;
   }
   o << "            total win amount: " << win_cycle_lite.win_outcome.total_win_amount << endl;

   o << "*****     end     *****" << endl;
#endif
   return (o);
}

/// <summary> Adds a script table into an existing script table.</summary>
/// <param name = "payload">[out] Script table to append to.</param>
/// <param name = "data">[in] Script table to include.</param>
/// <param name = "_table_entry_name">[in] [optional] Name of the table entry in the script table to append to.</param>
void WinCycleLite::addScriptTableParameter (ScriptTableParameter & payload, ScriptTableParameter & data,
   const char * _table_entry_name)
{
   String table_entry_name;
   if (_table_entry_name)
   {
      table_entry_name = _table_entry_name;
   }
   else
   {
      table_entry_name << payload.script_table.size () + 1;
   }
   payload.script_table [table_entry_name.c_str ()] = ScriptParameter (data, ParameterType::TABLE);
}

/// <summary> Adds an action and the associated data to the passed payload.</summary>
/// <param name = "payload">[in/out] The payload to add the action to.</param>
/// <param name = "action">[in] The name of the action.</param>
/// <param name = "action_data">[in] An optional table containing the parameters for this action.</param>
void WinCycleLite::addWinCycleLiteAction (ScriptTableParameter & payload,
                                          const char * action,
                                          ScriptTableParameter * action_data)
{
   if (action)
   {
      // check if the payload has an actions table already
      if (payload.script_table.find("actions") == payload.script_table.end())
      {
         ScriptTableParameter actions_table;
         // add a new, empty actions table.
         payload.script_table ["actions"] = ScriptParameter (actions_table, ParameterType::TABLE);
      }

      // retrieve a reference to the existing actions table.
      ScriptTableParameter * actions_table = payload.script_table ["actions"].asTable ();

      // prepare the action
      ScriptTableParameter action_table;
      action_table.script_table ["action"] = ScriptParameter (action, ParameterType::STRING);
      if (action_data)
      {
         action_table.script_table ["parameters"] = ScriptParameter (*action_data, ParameterType::TABLE);
      }

      // add this action to the action table at the next available 'index'.
      String index;
      index << actions_table->script_table.size () + 1;
      actions_table->script_table [index.c_str ()] = ScriptParameter (action_table, ParameterType::TABLE);
   }
}

/// <summary> Applies the phase three symbol rules on the winning animation list created. Populate the phase three symbol rules structure in this class with the actions to execute.</summary>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] A list of awards for all acts in this stage.</param>
/// <param name = "denomination">[in] The denomination object.</param>
/// <param name = "bet">[in] The bet object.</param>
void WinCycleLite::checkSymbolRules (Mojo<SlotAwardInterface> & slot_award_interface,
                                     Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                     Mojo<AwardInterface> & award_list,
                                     const Denomination & denomination,
                                     Mojo<BetInterface> & bet)
{
   verboseMethodName ();

   Condition::GameInfo game_info =
   {
      slot_award_interface,
      slot_evaluator,
      award_list,
      bet,
      denomination
   };

   /* sort the phase three rules based on the priority set, the rules are only sorted once once they are sorted */
   if (!p3r_symbol_rules->rules_are_sorted)
   {
      p3r_symbol_rules->sortRules ();
   }

   Action::ActionStructures action_struct =
   {
      p3r_symbol_rules->sound_controller,
      p3r_symbol_rules->animation_map,
      p3r_symbol_rules->animation_coords_map,
      p3r_symbol_rules->animation_events_map,
      p3r_symbol_rules->animation_context_events_map,
      p3r_symbol_rules->overlay_events_map,
      p3r_symbol_rules->animation_durations,
      p3r_symbol_rules->max_animation_duration,
      p3r_symbol_rules->max_rule_duration
   };

   // check all the rules agains all the wins.
   list<Mojo<Rule,RuleInterface> >::iterator end_of_rules = p3r_symbol_rules->rules.end();
   SlotWinningEntryWithGroupingListIterator end_of_wins = winning_animations_list.end();
   for (SlotWinningEntryWithGroupingListIterator win = winning_animations_list.begin(); win != end_of_wins; ++win)
   {
      for (list<Mojo<Rule,RuleInterface> >::iterator rule = p3r_symbol_rules->rules.begin();
           rule != end_of_rules; ++rule)
      {
         // debug << "Checking" << endl << **rule << endl;
         if ((*rule)->pass (game_info, static_cast <SlotWinEntry &> (*win)))
         {
             (*rule)->executeActions (static_cast <SlotWinEntry &> (*win), action_struct);
         }
      }
   }
}

/// <summary> Clears the win outcome and the winning animation list.</summary>
void WinCycleLite::clearWinOutcome ()
{
   winning_animations_list.clear ();

   win_outcome.total_win_amount = ZERO_AMOUNT;
   win_outcome.winning_paylines.clear ();
   win_outcome.winning_symbols.clear ();

   p3r_symbol_rules->resetAnimationData();
}

/// <summary> Updates the winning animations list with the slot win entries for multiway wins.</summary>
/// <remarks> Only accounts for displayable awards.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createMultiWayWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();

   uint32 number_of_multi_way_awards = slot_award->getNumberOfMultiWayAwardItems ();

   for (uint32 multi_way_award_index = 0; multi_way_award_index < number_of_multi_way_awards; ++multi_way_award_index)
   {
      /* only accounts for displayable awards */
      if (slot_award->getIsMultiWayAmountDisplayable (multi_way_award_index))
      {
         SlotWinEntryWithGrouping slot_win_entry;

         int16 number_of_winning_symbols = slot_award->getNumberOfMultiWayWinningSymbols (multi_way_award_index);

         for (uint16 symbol_index = 0; symbol_index < number_of_winning_symbols; ++symbol_index)
         {
            SlotWinningAnimationEntry slot_winning_animation_entry;

            slot_winning_animation_entry.reel_number = slot_award->getMultiWaySymbolX (multi_way_award_index, symbol_index);
            slot_winning_animation_entry.offset_from_home_position = slot_award->getMultiWaySymbolY (multi_way_award_index, symbol_index);
            slot_winning_animation_entry.symbol_name = slot_evaluator->getLibrarySymbolName (slot_award->getMultiWaySymbolId (multi_way_award_index, symbol_index));

            slot_win_entry.push_back (slot_winning_animation_entry);

            Coords coords = { slot_winning_animation_entry.reel_number,
                              slot_winning_animation_entry.offset_from_home_position };
            if (win_outcome.winning_symbols.count (coords) == 0)
            {
               win_outcome.winning_symbols.insert ( pair <Coords, String> (coords, slot_winning_animation_entry.symbol_name));
            }
         }

         slot_win_entry.win_type = SlotWinEntry::MULTI_WAY_WIN;
         slot_win_entry.award_index = multi_way_award_index;
         slot_win_entry.win_category = slot_award->getMultiWayCategoryNumber (multi_way_award_index);

         slot_win_entry.win_total = slot_award->getMultiWayAmount (multi_way_award_index);

         int32 multiplier = slot_award->getMultiWayMultiplier (multi_way_award_index);
         if (multiplier < 1)
            multiplier = 1;
         slot_win_entry.base_win = slot_win_entry.win_total / multiplier;
         slot_win_entry.multiplier = multiplier;

         slot_win_entry.bonus_win_amount = ZERO_AMOUNT;

         // set the win classification for sorting.
         if (slot_award->getMultiWayNumberOfTriggers (multi_way_award_index))
         {
            slot_win_entry.win_classification |= SlotWinEntry::BONUS_WIN;
            /* retrieve all win amount beyond the current act as bonus win amount
               excluding the double up wins. */

            Amount double_up_win_amount (ZERO_AMOUNT);
            for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
            {
               Mojo<SystemAwardInterface> system_award;
               if (award_list->getAward (award_index, system_award))
               {
                  String name = system_award->getName ();
                  if (!strcmp(name, "#DoubleUpLoss") || !strcmp(name, "#DoubleUpWin"))
                  {
                     system_award->getTotalAwardAmountDisplayable (double_up_win_amount);
                  }
               }
            }

            Mojo<SystemAwardInterface> current_system_award (slot_award);
            award_list->totalDisplayableAmount (slot_win_entry.bonus_win_amount,
                                                current_system_award->getTransactionId () + 1);
            slot_win_entry.bonus_win_amount -= double_up_win_amount;
            slot_win_entry.win_total += slot_win_entry.bonus_win_amount;
         }
         if (slot_award->getIsMultiWayValidProgressive (multi_way_award_index))
         {
            slot_win_entry.win_classification |= SlotWinEntry::JACKPOT_WIN;
         }
         else if (slot_win_entry.win_total.isNotZero())
         {
            slot_win_entry.win_classification |= SlotWinEntry::BASIC_WIN;
         }
         winning_animations_list.push_front (slot_win_entry);
      }
   }
}

/// <summary> Updates the winning animations list with the slot win entries for pattern wins.</summary>
/// <remarks> Only accounts for displayable awards.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createPatternWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();

   uint32 number_of_pattern_awards = slot_award->getNumberOfPatterns ();

   for (uint32 pattern_award_index = 0; pattern_award_index < number_of_pattern_awards; ++pattern_award_index)
   {
      uint32 number_of_pattern_award_items = slot_award->getNumberOfPatternAwardItems (pattern_award_index);

      for (uint32 pattern_award_item_index = 0; pattern_award_item_index < number_of_pattern_award_items;
          ++pattern_award_item_index)
      {
         if (slot_award->getIsPatternAmountDisplayable (pattern_award_index, pattern_award_item_index))
         {
            SlotWinEntryWithGrouping slot_win_entry;

            int16 number_of_winning_symbols = slot_award->getNumberOfPatternWinningSymbols (pattern_award_index,
                                                                                            pattern_award_item_index);

            for (uint16 symbol_index = 0; symbol_index < number_of_winning_symbols; ++symbol_index)
            {
               SlotWinningAnimationEntry slot_winning_animation_entry;

               slot_winning_animation_entry.reel_number = slot_award->getPatternSymbolX (pattern_award_index, pattern_award_item_index, symbol_index);
               slot_winning_animation_entry.offset_from_home_position = slot_award->getPatternSymbolY (pattern_award_index, pattern_award_item_index, symbol_index);
               slot_winning_animation_entry.symbol_name = slot_evaluator->getLibrarySymbolName (slot_award->getPatternSymbolId (pattern_award_index, pattern_award_item_index, symbol_index));

               slot_win_entry.push_back (slot_winning_animation_entry);

               Coords coords = { slot_winning_animation_entry.reel_number,
                                 slot_winning_animation_entry.offset_from_home_position };
               if (win_outcome.winning_symbols.count (coords) == 0)
               {
                  win_outcome.winning_symbols.insert ( pair <Coords, String> (coords, slot_winning_animation_entry.symbol_name));
               }
            }

            slot_win_entry.win_type = SlotWinEntry::PATTERN_WIN;
            slot_win_entry.award_index = pattern_award_index;
            slot_win_entry.sub_award_index = pattern_award_item_index;
            slot_win_entry.win_category = slot_award->getPatternCategoryNumber (pattern_award_index, pattern_award_item_index);

            slot_win_entry.win_total = slot_award->getPatternAmount (pattern_award_index, pattern_award_item_index);

            int32 multiplier = slot_award->getPatternMultiplier (pattern_award_index, pattern_award_item_index);
            if (multiplier < 1)
               multiplier = 1;
            slot_win_entry.base_win = slot_win_entry.win_total / multiplier;
            slot_win_entry.multiplier = multiplier;

            slot_win_entry.bonus_win_amount = ZERO_AMOUNT;

            // set the win classification for sorting.
            if (slot_award->getPatternNumberOfTriggers (pattern_award_index, pattern_award_item_index))
            {
               slot_win_entry.win_classification |= SlotWinEntry::BONUS_WIN;
               /* retrieve all win amount beyond the current act as bonus win amount
                  excluding the double up wins. */

               Amount double_up_win_amount (ZERO_AMOUNT);
               for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
               {
                  Mojo<SystemAwardInterface> system_award;
                  if (award_list->getAward (award_index, system_award))
                  {
                     String name = system_award->getName ();
                     if (!strcmp(name, "#DoubleUpLoss") || !strcmp(name, "#DoubleUpWin"))
                     {
                        system_award->getTotalAwardAmountDisplayable (double_up_win_amount);
                     }
                  }
               }

               Mojo<SystemAwardInterface> current_system_award (slot_award);
               award_list->totalDisplayableAmount (slot_win_entry.bonus_win_amount,
                                                   current_system_award->getTransactionId () + 1);
               slot_win_entry.bonus_win_amount -= double_up_win_amount;
               slot_win_entry.win_total += slot_win_entry.bonus_win_amount;
            }
            if (slot_award->getIsPatternValidProgressive (pattern_award_index, pattern_award_item_index))
            {
               slot_win_entry.win_classification |= SlotWinEntry::JACKPOT_WIN;
            }
            else if (slot_win_entry.win_total.isNotZero())
            {
               slot_win_entry.win_classification |= SlotWinEntry::BASIC_WIN;
            }
            winning_animations_list.push_front (slot_win_entry);
         }
      }
   }
}

/// <summary> Updates the winning animations list with the slot win entries for paylines wins. Grouping paylines displays as necessary.</summary>
/// <remarks> Only accounts for displayable awards.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createPaylinesWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();

   // update the win list with the local award list for the current act.
   if (slot_award->getNumberOfAwards ())
   {
      uint32 number_of_paylines = slot_award->getNumberOfLines ();
      for (uint32 payline_index = 0; payline_index < number_of_paylines; ++payline_index)
      {
         int16 number_of_line_award_items = slot_award->getNumberOfLineAwardItems (payline_index);
         for (int16 line_award_item_index = 0; line_award_item_index < number_of_line_award_items; ++line_award_item_index)
         {
            // account for line awards that are displayable.
            if (slot_award->getIsLineAmountDisplayable (payline_index, line_award_item_index) &&
                slot_award->getLineBet (payline_index, line_award_item_index).isNotZero ())
            {
               win_outcome.winning_paylines.push_back (payline_index);

               SlotWinningEntryWithGroupingListIterator slot_win_entry_ptr;

               /* don't group line that triggers progressive or bonus wins */
               if (group_payline_display &&
                   findSlotWinEntryGroup (slot_evaluator, slot_award,
                                          payline_index, line_award_item_index,
                                          slot_win_entry_ptr) &&
                   !slot_award->getLineNumberOfTriggers (payline_index, line_award_item_index) &&
                   !slot_award->getIsLineValidProgressive (payline_index, line_award_item_index))
               {
                  /* multiple awards on the same line are grouped together */
                  /* line wins having the same category and win coordinates are grouped together */
                  slot_win_entry_ptr->award_indexes.push_back (payline_index);

                  /* for grouping sums up all paylines wins */
                  slot_win_entry_ptr->win_total +=
                     slot_award->getLineAmount (payline_index, line_award_item_index);
               }
               else
               {
                  SlotWinEntryWithGrouping slot_win_entry;

                  uint16 number_of_winning_symbols = slot_award->getNumberOfLineWinningSymbols (payline_index,
                                                                                                line_award_item_index);

                  slot_win_entry.win_type = SlotWinEntry::PAYLINE_WIN;
                  slot_win_entry.win_category = slot_award->getLineCategoryNumber (payline_index, line_award_item_index);
                  slot_win_entry.award_index = payline_index; /* default award index for grouping is the first payline award */
                  slot_win_entry.award_indexes.push_back (payline_index);
                  slot_win_entry.sub_award_index = line_award_item_index; /* default line award item for grouping is the line award_item for the same payline award */

                  for (uint16 symbol_index = 0; symbol_index < number_of_winning_symbols; ++symbol_index)
                  {
                     SlotWinningAnimationEntry slot_winning_animation_entry;

                     slot_winning_animation_entry.reel_number = slot_award->getLineSymbolX (payline_index, line_award_item_index, symbol_index);
                     slot_winning_animation_entry.offset_from_home_position = slot_award->getLineSymbolY (payline_index, line_award_item_index, symbol_index);
                     slot_winning_animation_entry.symbol_name  = slot_evaluator->getLibrarySymbolName (slot_award->getLineSymbolId (payline_index, line_award_item_index, symbol_index));
                     slot_winning_animation_entry.winning_symbol = true;

                     slot_win_entry.push_back (slot_winning_animation_entry);

                     Coords coords = { slot_winning_animation_entry.reel_number,
                                       slot_winning_animation_entry.offset_from_home_position };
                     if (win_outcome.winning_symbols.count (coords) == 0)
                     {
                        win_outcome.winning_symbols.insert ( pair <Coords, String> (coords, slot_winning_animation_entry.symbol_name));
                     }
                  }

                  slot_win_entry.win_total = slot_award->getLineAmount (payline_index, line_award_item_index);

                  /* set the base win without the multiplier, for grouping assumes all lines grouped
                     have the same bet. */
                  int32 multiplier = slot_award->getLineMultiplier (payline_index, line_award_item_index);
                  if (multiplier < 1)
                     multiplier = 1;
                  slot_win_entry.base_win = slot_win_entry.win_total / multiplier;
                  slot_win_entry.multiplier = multiplier;

                  slot_win_entry.bonus_win_amount = ZERO_AMOUNT;

                  // set the win classification for sorting.
                  /* check for bonus wins this line triggers */
                  if (slot_award->getLineNumberOfTriggers (payline_index, line_award_item_index))
                  {
                     slot_win_entry.win_classification |= SlotWinEntry::BONUS_WIN;
                  /* retrieve all win amount beyond the current act as bonus win amount
                     excluding the double up wins. */

                  Amount double_up_win_amount (ZERO_AMOUNT);
                  for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
                  {
                     Mojo<SystemAwardInterface> system_award;
                     if (award_list->getAward (award_index, system_award))
                     {
                        String name = system_award->getName ();
                        if (!strcmp(name, "#DoubleUpLoss") || !strcmp(name, "#DoubleUpWin"))
                        {
                           system_award->getTotalAwardAmountDisplayable (double_up_win_amount);
                        }
                     }
                  }

                  Mojo<SystemAwardInterface> current_system_award (slot_award);
                  award_list->totalDisplayableAmount (slot_win_entry.bonus_win_amount,
                                                      current_system_award->getTransactionId () + 1);
                  slot_win_entry.bonus_win_amount -= double_up_win_amount;
                  slot_win_entry.win_total += slot_win_entry.bonus_win_amount;
                  }

                  /* check for jackpot wins this line has */
                  if (slot_award->getIsLineValidProgressive (payline_index, line_award_item_index))
                  {
                     slot_win_entry.win_classification |= SlotWinEntry::JACKPOT_WIN;
                  }
                  else if (slot_win_entry.win_total.isNotZero()) /* basic win is anything but a jackpot win */
                  {
                     slot_win_entry.win_classification |= SlotWinEntry::BASIC_WIN;
                  }
                  winning_animations_list.push_back (slot_win_entry);
               }
            }
         }
      }
   }
}

/// <summary> Updates the winning animations list with the slot win entries for RMLP wins.</summary>
/// <remarks> Only accounts for displayable awards.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createRMLPWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();
   
   for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
   {
      Mojo<SystemAwardInterface> system_award;
      if (award_list->getAward (award_index, system_award))
      {
         String name = system_award->getName ();
         if (!strcmp(name, "RMLPAward"))
         {
            SlotWinEntryWithGrouping slot_win_entry;
            slot_win_entry.win_type = SlotWinEntry::RMLP_WIN;
            slot_win_entry.award_index = award_index;
            slot_win_entry.win_category = UNKNOWN_CATEGORY;
            slot_win_entry.base_win = ZERO_AMOUNT;
            slot_win_entry.multiplier = 0;
            system_award->getTotalAwardAmountDisplayable (slot_win_entry.bonus_win_amount);
            slot_win_entry.win_total = slot_win_entry.bonus_win_amount;

            slot_win_entry.win_classification |= SlotWinEntry::BONUS_WIN;

            winning_animations_list.push_front (slot_win_entry);
            break;
         }
      }
   }
}

/// <summary> Updates the winning animations list with the slot win entries for scatter wins.</summary>
/// <remarks> Only accounts for displayable awards.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createScatterWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();

   uint32 number_of_scatter_awards = slot_award->getNumberOfScatterAwardItems ();
   for (uint32 scatter_award_index = 0; scatter_award_index < number_of_scatter_awards; ++scatter_award_index)
   {
      if (slot_award->getIsScatterAmountDisplayable (scatter_award_index))
      {
         SlotWinEntryWithGrouping slot_win_entry;

         uint16 number_of_winning_symbols = slot_award->getNumberOfScatterWinningSymbols (scatter_award_index);
         for (uint32 symbol_index = 0; symbol_index < number_of_winning_symbols; ++symbol_index)
         {
            SlotWinningAnimationEntry animation_entry;

            animation_entry.reel_number = slot_award->getScatterSymbolX (scatter_award_index, symbol_index);
            animation_entry.offset_from_home_position = slot_award->getScatterSymbolY (scatter_award_index, symbol_index);
            animation_entry.symbol_name = slot_evaluator->getLibrarySymbolName (slot_award->getScatterSymbolId (scatter_award_index, symbol_index));

            slot_win_entry.push_back (animation_entry);

            Coords coords = { animation_entry.reel_number,
                              animation_entry.offset_from_home_position };
            if (win_outcome.winning_symbols.count (coords) == 0)
            {
               win_outcome.winning_symbols.insert ( pair <Coords, String> (coords, animation_entry.symbol_name));
            }
         }

         slot_win_entry.win_type = SlotWinEntry::SCATTER_WIN;
         slot_win_entry.award_index = scatter_award_index;
         slot_win_entry.win_category = slot_award->getScatterCategoryNumber (scatter_award_index);

         slot_win_entry.win_total = slot_award->getScatterAmount (scatter_award_index);

         int32 multiplier = slot_award->getScatterMultiplier (scatter_award_index);
         if (multiplier < 1)
            multiplier = 1;
         slot_win_entry.base_win = slot_win_entry.win_total / multiplier;
         slot_win_entry.multiplier = multiplier;

         slot_win_entry.bonus_win_amount = ZERO_AMOUNT;

         // set the win classification for sorting.
         if (slot_award->getScatterNumberOfTriggers (scatter_award_index))
         {
            slot_win_entry.win_classification |= SlotWinEntry::BONUS_WIN;

            /* retrieve all win amount beyond the current act as bonus win amount
               excluding the double up wins. */

            Amount double_up_win_amount (ZERO_AMOUNT);
            for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
            {
               Mojo<SystemAwardInterface> system_award;
               if (award_list->getAward (award_index, system_award))
               {
                  String name = system_award->getName ();
                  if (!strcmp(name, "#DoubleUpLoss") || !strcmp(name, "#DoubleUpWin"))
                  {
                     system_award->getTotalAwardAmountDisplayable (double_up_win_amount);
                  }
               }
            }

            Mojo<SystemAwardInterface> current_system_award (slot_award);
            award_list->totalDisplayableAmount (slot_win_entry.bonus_win_amount,
                                                current_system_award->getTransactionId () + 1);
            slot_win_entry.bonus_win_amount -= double_up_win_amount;
            slot_win_entry.win_total += slot_win_entry.bonus_win_amount;
         }
         if (slot_award->getIsScatterValidProgressive (scatter_award_index))
         {
            slot_win_entry.win_classification |= SlotWinEntry::JACKPOT_WIN;
         }
         else if (slot_win_entry.win_total.isNotZero())
         {
            slot_win_entry.win_classification |= SlotWinEntry::BASIC_WIN;
         }
         winning_animations_list.push_front (slot_win_entry);
      }
   }
}

/// <summary> Updates the bingo win structure with the displayable win ammounts awarded by bingo.</summary>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createBingoWinList (const Mojo <AwardInterface> & award_list)
{
   verboseMethodName ();

   // Reset amounts first
   bingo_meters.interim_win = ZERO_AMOUNT;
   bingo_meters.bingo_prize = ZERO_AMOUNT;
   bingo_meters.bingo_grand_prize = ZERO_AMOUNT;
   bingo_meters.bingo_interim_progressive_prize = ZERO_AMOUNT;
   bingo_meters.bingo_coincidental_win = ZERO_AMOUNT;

   if (award_list.isValid ())
   {
      uint16 number_of_awards = award_list->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         Mojo <SystemAwardInterface> game_award;
         // Pointer to SystemAward.
         if (award_list->getAward (award_index, game_award))
         {
            if (game_award->getAwardType () == SystemAwardInterface::BINGO)
            {
               Mojo<BingoAwardInterface> bingo_award;
               bingo_award = game_award;
               // Now sum up Bingo prize and Bingo grand prize amounts
               uint16 number_of_awards_items = bingo_award->getNumberOfAwards ();
               for (uint16 awards_item_index = 0; awards_item_index < number_of_awards_items; ++awards_item_index)
               {
                  if (game_award->getIsAmountDisplayable (awards_item_index))
                  {
                     String name (game_award->getName (awards_item_index));
                     if (name.contains (bingo_win_name))
                     {
                        bingo_meters.bingo_prize += bingo_award->getAmount (awards_item_index);
                     }
                     else if (name.contains (bingo_coincidental_win_name))
                     {
                        bingo_meters.bingo_coincidental_win += bingo_award->getAmount (awards_item_index);
                     }
                     // The progressive amount is not contained in the getAmount, so dig further for it.
                     uint16 number_of_progressive_hits = game_award->getNumberOfPossibleProgressiveHits (awards_item_index);

                     for (uint16 progressive_hit_index = 0; progressive_hit_index < number_of_progressive_hits; ++progressive_hit_index)
                     {
                        if (name.contains (bingo_interim_progressive_win_name))
                           bingo_meters.bingo_interim_progressive_prize += game_award->getProgressiveDefaultPrizeAmount (awards_item_index, progressive_hit_index);
                        else
                           bingo_meters.bingo_grand_prize += game_award->getProgressiveDefaultPrizeAmount (awards_item_index, progressive_hit_index);
                     }
                  }
               }
            }
            else
            {
               Amount game_award_amount (ZERO_AMOUNT);
               game_award->getTotalAwardAmountDisplayable (game_award_amount);
               bingo_meters.interim_win += game_award_amount;
            }
         }
      }
   }
}

/// <summary> Creates a scriptable object given its id.</summary>
/// <param name="new_object">[out] Object container for the newly created object.</param>
/// <param name="object_id">[in] Type ID of the object to be created.</param>
/// <param name="parameter_list">[in/out] Parameters used for creating the object.</param>
/// <param name="new_declared">[in] True if 'new' was declared for the object creation.</param>
/// <returns> True if the object is created, false othewise.</returns>
bool WinCycleLite::createScriptableObjectById (Mojo<ScriptInterface> &new_object,
   int32 object_id, ScriptParameterList& parameter_list, bool new_declared)
{
   bool object_created = false;

   switch (object_id)
   {
      case WIN_ANIMATION_LIST:
         if (!p3r_symbol_rules.isValid ())
            p3r_symbol_rules.create<WinAnimationListWG> ();
         new_object = p3r_symbol_rules;
         assertion (new_object.isValid ());
         object_created = true;
         break;

      default:
         break;
   }
   return object_created;
}

/// <summary> Updates the winning animations list and the win outcome for win cycle displays.</summary>
///<remarks> Combine processing of all win types into one function once the slot award class is reworked
///   to store award items in a more managable way indifference to the win type, allowing this
///   function to process each award in the award list without regards to the win type.</remarks>
/// <param name = "slot_award">[in] The award for the current act.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] Awards for all acts for this stage.</param>
void WinCycleLite::createWinList (const Mojo <SlotAwardInterface> & slot_award,
   const Mojo <SlotEvaluatorInterface> & slot_evaluator, const Mojo <AwardInterface> & award_list)
{
   createPaylinesWinList (slot_award, slot_evaluator, award_list);
   createRMLPWinList (slot_award, slot_evaluator, award_list);
   createScatterWinList (slot_award, slot_evaluator, award_list);
   createPatternWinList (slot_award, slot_evaluator, award_list);
   createMultiWayWinList (slot_award, slot_evaluator, award_list);
   if (bingo_game)
   {
      createBingoWinList (award_list);
   }
}

/// <summary> Displays the selected payline(s).</summary>
/// <param name = "bet">[in] The bet object used to obtain the number of paylines selection.</param>
void WinCycleLite::displaySelectedPaylines (const Mojo <BetInterface> &bet)
{
   if (bet.isValid () && event_queue.isValid ())
   {
      uint32 selected_payline_count = bet->numberOfActiveSubBets ();

      ScriptTableParameter payload;
      payload.script_table ["payline_count"] =
         ScriptParameter (selected_payline_count, ParameterType::UINT32);

      String event_string (resourceAlias ());
      event_string << ":DisplaySelectedPaylines";
      event_queue->postEventExtended(event_string, 0, &payload);
   }
}

/// <summary> Starts or stops the win cycle with the option to play sounds when win cycle starts.</summary>
/// <param name = "show">[in] Starts the win cycle if set to true, and stops the win cycle if set to false.</param>
/// <param name = "start_new_win_cycle"> [in] Flag that indicates whether this is
///   the first time the game should start win cycling a new set of win combiniations.
///   This can be used to play sounds or perform special animations when the win cycle
///   starts for the first time.</param>
void WinCycleLite::displayWin (const bool show, const bool start_new_win_cycle)
{
   verboseMethodName ();

   ScriptTableParameter payload;

   if (show)
   {
      ScriptTableParameter parameters_table;
      parameters_table.script_table ["start_new_win_cycle"] = ScriptParameter (start_new_win_cycle, ParameterType::BOOL);
      addWinCycleLiteAction (payload, "Start", &parameters_table);
   }
   else
   {
      addWinCycleLiteAction (payload, "Stop");
   }
   postWinCycleLiteAction (payload);

   /* once the win cycle is stopped, an update to the win outcome is needed when
      the function: updateWinList is called. In addition, the function: updateWinList
      must be called before this function is called with "show" set to true. */
   update_win_list = !show;
}

/// <summary> Execute a function based on the function_id.</summary>
/// <param name = "function_id">[in] The identifier used for the function to be executed.</param>
/// <param name = "parameter_list">[in] Parameters for the function specified by <paramref name = "function_id"/>.</param>
/// <returns> True if the function was executed, false othewise.</returns>
/// <exception cref="LogicError::INVALID_VALUE"> Thrown when the actor passed in is not supported by the ActorInterface.</exception>
bool WinCycleLite::execScriptableFunctionById (int32 function_id, ScriptParameterList& parameter_list)
{
   bool success = true;

   switch (function_id)
   {
      case SET_ACTOR_FOR_EVENT_QUEUE:
         {
            Mojo <ActorInterface> actor;
            if (convertScriptParameterToMojo (parameter_list [0], actor))
            {
               setActorForEventQueue (actor);
            }
            else
            {
               /* no need to return false on system fail since the error_set_by_function flag is set for
                  the caller of this function. */
               String error_msg = String ("Actor: ") + parameter_list [0].asString () + String (" is not supported.");
               setErrorString (LogicError (LogicError::INVALID_VALUE, error_msg).errorMessage (), whoAreYou ());
            }
         }
         break;

      default:
         success = false;
         break;
   }
   return success;
}

/// <summary> Searches for an existing payline display group in the winning animations list that the payline may belong in.</summary>
/// <remarks> Uses for grouping paylines for displays only.</remarks>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "slot_award">[in] The award list for the current act.</param>
/// <param name = "award_index">[in] Award index for the current award in slot_award.</param>
/// <param name = "sub_award_index">[in] The sub award index in the current award in slot_award.</param>
/// <param name = "winning_entry_iter">[out] Pointer to the winning entry group that this payline may belong to.</param>
/// <returns> True if the payline belongs to an existing payline display group, and false otherwise.</returns>
bool WinCycleLite::findSlotWinEntryGroup (const Mojo <SlotEvaluatorInterface> & slot_evaluator,
                                          const Mojo <SlotAwardInterface> & slot_award,
                                          const uint32 award_index, const int16 sub_award_index,
                                          SlotWinningEntryWithGroupingListIterator & winning_entry_iter)
{
   bool done = false;
   bool found = false;

   WinCategoryId category = slot_award->getLineCategoryNumber (award_index, sub_award_index);
   winning_entry_iter = winning_animations_list.begin ();
   uint16 number_of_winning_symbols = slot_award->getNumberOfLineWinningSymbols (award_index, sub_award_index);

   while (!done && winning_entry_iter != winning_animations_list.end ())
   {
      /* matching category in the winning animations list */
      if (winning_entry_iter->win_category == category &&
          winning_entry_iter->size () == number_of_winning_symbols)
      {
         bool coord_check = true;
         int16 award_offset_x, award_offset_y;

         /* check for matching coordinates */
         for (int16 symbol_index = number_of_winning_symbols - 1;
              coord_check && symbol_index >= 0; --symbol_index)
         {
            award_offset_x = slot_award->getLineSymbolX (award_index, sub_award_index, symbol_index);
            award_offset_y = slot_award->getLineSymbolY (award_index, sub_award_index, symbol_index);

            if (award_offset_x != winning_entry_iter->at (symbol_index).reel_number ||
                award_offset_y != winning_entry_iter->at (symbol_index).offset_from_home_position)
            {
               coord_check = false;
            }
         }
         if (coord_check) /* passed coordinates check, done searching */
         {
            done = true;
            found = true;
         }
         else
         {
            ++winning_entry_iter;
         }
      }
      else
      {
         ++winning_entry_iter;
      }
   }
   return found;
}

/// <summary> Converts the win type enumeration index into a win type string.</summary>
/// <param name = "win_type">[in] The win type enumeration index.</param>
/// <param name = "win_type_str">[out] The win type string.</param>
void WinCycleLite::getWinTypeString (const SlotWinEntry::WinType win_type, String & win_type_str)
{
   switch (win_type)
   {
      case SlotWinEntry::PAYLINE_WIN:
         win_type_str << "payline_win";
         break;

      case SlotWinEntry::RMLP_WIN:
         win_type_str << "rmlp_win";
         break;

      case SlotWinEntry::SCATTER_WIN:
         win_type_str << "scatter_win";
         break;

      case SlotWinEntry::MULTI_WAY_WIN:
         win_type_str << "multi_way_win";
         break;

      case SlotWinEntry::PATTERN_WIN:
         win_type_str << "pattern_win";
         break;

      default:
         break;
   }
}

/// <summary> Initializes this module including the option to display paylines in groups.</summary>
/// <remarks> Also refer to the remarks on the group_payline_display flag for information on how paylines are grouped.</remarks>
/// <param name = "_group_payline_display">[in] Sets the group_payline_display flag to display paylines in groups.
///   By default it is not grouped.</param>
void WinCycleLite::init (const bool _group_payline_display)
{
   // This is intentionally disabled pending the resolution of a licensing agreement
   // group_payline_display = _group_payline_display;
}

/// <summary> Initializes the object for use.</summary>
void WinCycleLite::postConstructor (void)
{
   verboseMethodName();

   Config config;
   bingo_game = config.getValue (config.getId ("BINGO SYSTEM GAMING"));

   Scriptable::postConstructor ();
   ResourceTemplate<WinCycleLiteInterface>::postConstructor ();

   group_payline_display = false;
   update_win_list = true;

   static const ScriptFunctionMap script_functions [] =
   {
      // Parameters:  Script function id, Number of paramters, function name.
      { SET_ACTOR_FOR_EVENT_QUEUE, 1, "setActorForEventQueue"                  },
      // Terminator.
      { -1, 0, "" }
   };
   addScriptFunctionMap (script_functions);

   // Add the scriptable objects to the object map
   static const ScriptObjectMap script_objects [] =
   {
      // Parameters: Object ID, New requirement, Object type string
      { WIN_ANIMATION_LIST,      ScriptObjectMap::NEW_NOT_ALLOWED, WinAnimationListWG::object_type         },
      // Terminator
      {-1, ScriptObjectMap::NEW_NOT_ALLOWED, "" }
   };
   addScriptObjectMap (script_objects);
}

/// <summary> Posts an event to the event driven client to perform a specific action defined in the payload.</summary>
/// <param name="payload">[in] The payload to be posted with the event.</param>
/// <exception cref="LogicError::INVALID_VALUE"> Thrown when the event queue for this module is not set.</exception>
void WinCycleLite::postWinCycleLiteAction (ScriptTableParameter & payload)
{
   String event_string (resourceAlias ());
   event_string << ":Client";

   if (event_queue.isValid())
   {
      event_queue->postEventExtended (event_string.c_str (), 0, &payload);
   }
   else
   {
      String error_msg = String ("The event queue for this module is not set.");
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_msg).errorMessage (), whoAreYou ());
   }
}

/// <summary> Posts win information to the script side for animations.</summary>
/// <remarks> Payline index is one based. Executes the following phase three rules:
///   win intro action, win cycle action, and sound to play during the win intro from the phase three sound
///   controller.</remarks>
/// <param name = "bet">[in] The bet object.</param>
/// <param name = "denomination">[in] The denomination object.</param>
void WinCycleLite::postWinList (Mojo<BetInterface> & bet, const Denomination & denomation)
{
   verboseMethodName ();

   ScriptTableParameter parameters_table;

   /* construct the animation intro table */
   ScriptTableParameter animate_intro_table;
   {
      for (map<Coords, String>::iterator winning_symbol = win_outcome.winning_symbols.begin ();
           winning_symbol != win_outcome.winning_symbols.end(); ++winning_symbol)
      {
         ScriptTableParameter table_entry;

         /* AnimationAction (ie: WinIntroAction) */
         Coords coords = { winning_symbol->first.reel, winning_symbol->first.offset };

         /* searches for an existing coordinates in the intro animation map */
         if (p3r_symbol_rules->animation_map.count (coords))
         {
            table_entry.script_table["animate"] =
               ScriptParameter (p3r_symbol_rules->animation_map[coords], ParameterType::STRING);
         }
         else
         {
            table_entry.script_table["animate"] =
               ScriptParameter (false, ParameterType::BOOL);
         }

         table_entry.script_table["reel"] = ScriptParameter (coords.reel, ParameterType::UINT16);
         table_entry.script_table["visible_symbol"] = ScriptParameter (coords.offset, ParameterType::UINT16);
         table_entry.script_table["name"] = ScriptParameter (winning_symbol->second, ParameterType::STRING);

         addScriptTableParameter (animate_intro_table, table_entry);
      }

      /* add winning paylines information after the winning symbols table has been added */
      ScriptTableParameter winning_paylines_table;
      for (uint16 winning_payline_index = 0; winning_payline_index < win_outcome.winning_paylines.size();
           ++winning_payline_index)
      {
         String winning_payline_index_str;
         winning_payline_index_str << winning_payline_index + 1;

         /* one based payline index */
         winning_paylines_table.script_table[winning_payline_index_str.c_str()] =
            ScriptParameter (static_cast <uint16> (win_outcome.winning_paylines[winning_payline_index] + 1),
                             ParameterType::UINT16);
      }
      if (winning_paylines_table.script_table.size ())
      {
         addScriptTableParameter (animate_intro_table, winning_paylines_table, "lines");
      }
   }
   addScriptTableParameter (parameters_table, animate_intro_table, "animate_intro");

   /* additional information */
   /* The desired sound to play is added to the sound controller via addPrioritySound().
      Only one sound is played. */
   if (p3r_symbol_rules->sound_controller->hasSound())
   {
      parameters_table.script_table["sound"] =
         ScriptParameter (p3r_symbol_rules->sound_controller->getSound().c_str(), ParameterType::STRING);
   }

   parameters_table.script_table ["total_win_amount"] =
      ScriptParameter (Amount::Extractor (win_outcome.total_win_amount).extract (denomation),
                       ParameterType::INT64);

   /* get the total bet for the game */
   Amount total_bet_amount = bet->getBetAmount ();
   parameters_table.script_table ["total_bet"] =
      ScriptParameter (Amount::Extractor (total_bet_amount).extract (Denomination::BASE_UNIT),
                       ParameterType::INT64);

   /* construct the win entries table for win cycling */
   ScriptTableParameter win_entries_table;
   {
      for (SlotWinningEntryWithGroupingListIterator iterator (winning_animations_list.begin());
           iterator != winning_animations_list.end(); ++iterator)
      {
         ScriptTableParameter award_table;

         /* OverlayAction (ie: WinCycleAction) */
         /* searches for an existing win item in the overlay_events_map. */
         Action::WinItem current_win_item = { iterator->award_index, iterator->win_type, iterator->sub_award_index };
         if (p3r_symbol_rules->overlay_events_map.count (current_win_item))
         {
            award_table.script_table["animate"] =
               ScriptParameter (p3r_symbol_rules->overlay_events_map[current_win_item].event, ParameterType::STRING);
         }
         else
         {
             award_table.script_table["animate"] = ScriptParameter (false, ParameterType::BOOL);
         }

         Amount::AmountValue bonus_win_amount = Amount::Extractor (iterator->bonus_win_amount).extract (denomation);
         award_table.script_table["bonus_win_amount"] = ScriptParameter (bonus_win_amount, ParameterType::INT64);

         Amount::AmountValue win_amount = Amount::Extractor (iterator->win_total).extract (denomation);

         if (iterator->win_type == SlotWinEntry::PAYLINE_WIN)
         {
            if (group_payline_display)
            {
               uint32 num_paylines_in_group = iterator->award_indexes.size ();

               if (group_payline_display && (num_paylines_in_group != 0))
               {
                  /* display total win per line if lines are grouped */
                  win_amount /= num_paylines_in_group;
               }

               String grouped_paylines_str;
               for (uint32 paylines_index = 0; paylines_index < num_paylines_in_group; ++paylines_index)
               {
                  if (paylines_index != 0)
                  {
                     grouped_paylines_str << ", ";
                  }
                  grouped_paylines_str << (iterator->award_indexes.at(paylines_index) + 1);
               }
               award_table.script_table["lines"] = ScriptParameter (grouped_paylines_str, ParameterType::STRING);
            }
            else
            {
               uint16 line_index = iterator->award_index + 1;
               award_table.script_table["lines"] = ScriptParameter (line_index, ParameterType::UINT16);
            }
         }
         else if (iterator->win_type == SlotWinEntry::RMLP_WIN)
         {
            award_table.script_table["rmlp"] = ScriptParameter (iterator->award_index, ParameterType::UINT16);
         }
         else if (iterator->win_type == SlotWinEntry::PATTERN_WIN)
         {
            /* send a zero based pattern index since it is zero based in the paytable */
            award_table.script_table["pattern"] = ScriptParameter (iterator->award_index, ParameterType::UINT16);
         }
         else if (iterator->win_type == SlotWinEntry::MULTI_WAY_WIN)
         {
            award_table.script_table ["multi_way_multiplier"] = ScriptParameter (iterator->multiplier, ParameterType::INT32);
            Amount::AmountValue base_win = Amount::Extractor (iterator->base_win).extract (denomation);
            award_table.script_table ["multi_way_base_win"] = ScriptParameter (base_win, ParameterType::INT64);
         }

         award_table.script_table["win_amount"] = ScriptParameter (win_amount, ParameterType::INT64);
         award_table.script_table["multiplier"] = ScriptParameter (iterator->multiplier, ParameterType::INT32);

         String win_type_string;
         getWinTypeString (iterator->win_type, win_type_string);
         award_table.script_table["win_type"] = ScriptParameter (win_type_string, ParameterType::STRING);

         ScriptTableParameter win_classification_table;
         if (iterator->win_classification & SlotWinEntry::BASIC_WIN)
         {
            String index_str;
            index_str << (win_classification_table.script_table.size() + 1);
            win_classification_table.script_table[index_str.c_str()] = ScriptParameter ("basic_win", ParameterType::STRING);
         }
         if (iterator->win_classification & SlotWinEntry::BONUS_WIN)
         {
            String index_str;
            index_str << (win_classification_table.script_table.size() + 1);
            win_classification_table.script_table[index_str.c_str()] = ScriptParameter ("bonus_win", ParameterType::STRING);
         }
         if (iterator->win_classification & SlotWinEntry::JACKPOT_WIN)
         {
            String index_str;
            index_str << (win_classification_table.script_table.size() + 1);
            win_classification_table.script_table[index_str.c_str()] = ScriptParameter ("jackpot_win", ParameterType::STRING);
         }
         addScriptTableParameter (award_table, win_classification_table, "win_classification");

         ScriptTableParameter win_entry_table;
         for (SlotWinningAnimationEntryListIterator entry_iter (iterator->begin());
              entry_iter != iterator->end(); ++entry_iter)
         {
            ScriptTableParameter win_entry_item_table;

            // only sends the winning symbols.
            win_entry_item_table.script_table["reel"] =
               ScriptParameter (entry_iter->reel_number, ParameterType::INT16);
            win_entry_item_table.script_table["visible_symbol"] =
               ScriptParameter (entry_iter->offset_from_home_position, ParameterType::INT16);
            win_entry_item_table.script_table["name"] =
               ScriptParameter (entry_iter->symbol_name, ParameterType::STRING);

            addScriptTableParameter (win_entry_table, win_entry_item_table);
         }
         addScriptTableParameter (award_table, win_entry_table, "winning_symbols");

         addScriptTableParameter (win_entries_table, award_table);
      }
   }
   addScriptTableParameter (parameters_table, win_entries_table, "win_entries");

   if (bingo_game)
   {
      /* construct the win entries table for win cycling */
      ScriptTableParameter bingo_win_entries_table;

      formatBingoWinTable (bingo_win_entries_table, denomation);

      addScriptTableParameter (parameters_table, bingo_win_entries_table, "bingo_win_entries");
   }

   /* create the actions table */
   ScriptTableParameter payload;
   addWinCycleLiteAction (payload, "Win", &parameters_table);
   postWinCycleLiteAction (payload);
}

/// <summary> Adds the individual bingo wins to the bingo_win_entries table.</summary>
/// <param name = "bingo_win_entries_table">[out] The scriptable table to add the bingo win infromation.</param>
/// <param name = "denomination">[in] The denomination object.</param>
void WinCycleLite::formatBingoWinTable (ScriptTableParameter & bingo_win_entries_table, const Denomination & denomination)
{
   String win_amount_string;
   String win_message;
   String index_str;

   int table_index = bingo_win_entries_table.script_table.size();

   // If there is an interim win or a coincidental win by itself create both a cash and cedit formated entry
   if (bingo_meters.interim_win != ZERO_AMOUNT)
   {
      ScriptTableParameter win_entry_item_table_currency;
      ScriptTableParameter win_entry_item_table_credits;

      // Currency
      formatAmountToMonetaryString (bingo_meters.interim_win, win_amount_string);
      win_entry_item_table_currency.script_table["win_type"] = ScriptParameter ("interim", ParameterType::STRING);
      win_entry_item_table_currency.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      // Credits
      formatAmountToCreditString (bingo_meters.interim_win, win_amount_string, denomination);
      win_entry_item_table_credits.script_table["win_type"] = ScriptParameter ("interim", ParameterType::STRING);
      win_entry_item_table_credits.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      if (bingo_meters.bingo_coincidental_win != ZERO_AMOUNT)
      {
         // Currency
         formatAmountToMonetaryString (bingo_meters.bingo_coincidental_win, win_amount_string);
         win_entry_item_table_currency.script_table["coincidental_win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

         // Credits
         formatAmountToCreditString (bingo_meters.bingo_coincidental_win, win_amount_string, denomination);
         win_entry_item_table_credits.script_table["coincidental_win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);
      }

      // Currency
      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_currency, index_str.c_str ());

      // Credits
      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_credits, index_str.c_str ());

   }
   else if (bingo_meters.bingo_coincidental_win != ZERO_AMOUNT)
   {
      ScriptTableParameter win_entry_item_table_currency;
      win_entry_item_table_currency.script_table["win_type"] = ScriptParameter ("interim", ParameterType::STRING);

      formatAmountToMonetaryString (bingo_meters.bingo_coincidental_win, win_amount_string);
      win_entry_item_table_currency.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_currency, index_str.c_str ());
   }

   // Create a cash formatted interim progressive win
   if (bingo_meters.bingo_interim_progressive_prize != ZERO_AMOUNT)
   {
      ScriptTableParameter win_entry_item_table_currency;

      formatAmountToMonetaryString (bingo_meters.bingo_interim_progressive_prize, win_amount_string);
      win_entry_item_table_currency.script_table["win_type"] = ScriptParameter ("interim_progressive", ParameterType::STRING);
      win_entry_item_table_currency.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_currency, index_str.c_str ());
   }

   // Create a cash formatted bingo grand prize win
   if (bingo_meters.bingo_grand_prize != ZERO_AMOUNT)
   {
      ScriptTableParameter win_entry_item_table_currency;

      formatAmountToMonetaryString (bingo_meters.bingo_grand_prize, win_amount_string);
      win_entry_item_table_currency.script_table["win_type"] = ScriptParameter ("grand_prize", ParameterType::STRING);
      win_entry_item_table_currency.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_currency, index_str.c_str ());
   }

   // Create a cash formatted bino win
   if (bingo_meters.bingo_prize != ZERO_AMOUNT)
   {
      ScriptTableParameter win_entry_item_table_currency;

      formatAmountToMonetaryString (bingo_meters.bingo_prize, win_amount_string);
      win_entry_item_table_currency.script_table["win_type"] = ScriptParameter ("bingo_prize", ParameterType::STRING);
      win_entry_item_table_currency.script_table["win_amount_string"] = ScriptParameter (win_amount_string.c_str (), ParameterType::STRING);

      ++table_index;
      index_str.clear ();
      index_str << table_index;
      addScriptTableParameter (bingo_win_entries_table, win_entry_item_table_currency, index_str.c_str ());
   }
}

/// <summary> Format a win Ammount to a localized currency string.</summary>
/// <param name = "win_amount">[in] The win Ammount.</param>
/// <param name = "win_amount_string">[out] The localized currency string.</param>
void WinCycleLite::formatAmountToMonetaryString (const Amount &win_amount, String &win_amount_string)
{
   ULocale locale;
   Amount::Extractor amount_extractor (win_amount);
   Amount::AmountValue win_value;

   win_value = amount_extractor.extract (Denomination::BASE_UNIT);
   locale.moneyFormatNumericWhole (locale.moneyNativeCountryCode (), win_value, win_amount_string);
}

/// <summary> Format a win Ammount to a credit string.</summary>
/// <param name = "win_amount">[in] The win Ammount.</param>
/// <param name = "win_amount_string">[out] The credit string.</param>
void WinCycleLite::formatAmountToCreditString (const Amount &win_amount, String &win_credit_string, const Denomination & denomination)
{
   ULocale locale;
   Amount::Extractor amount_extractor (win_amount);

   win_credit_string.clear ();
   win_credit_string << amount_extractor.extract (denomination);
   locale.formatNumeric (win_credit_string);
}

/// <summary> Prepares the object for destruction.</summary>
void WinCycleLite::preDestructor (void)
{
   verboseMethodName ();

   p3r_symbol_rules.release ();

   ResourceTemplate<WinCycleLiteInterface>::preDestructor ();
   Scriptable::preDestructor ();
}

/// <summary> Sets the event queue for this module where events are posted. The event queue is set from the actor passed in.</summary>
/// <param name = "actor">[in] The actor containing the event queue to post to.</param>
/// <exception cref = "LogicError::INVALID_VALUE">Thrown when there is no valid event queue set.</exception>
/// @scriptableFunction{WinCycleLite,setActorForEventQueue}
/// <example><b>Example:</b><br>
/// @extExampleStart{WinCycleLite}
/// @extExample{setActorForEventQueue}
/// @extExampleEnd
/// </example>
void WinCycleLite::setActorForEventQueue (const Mojo <ActorInterface> &actor)
{
   verboseMethodName ();

   /* try getting the local queue first */
   Mojo <ActuableInterface> actuable (actor);
   if (actuable.isValid ())
   {
      Mojo <MultimediaEventQueueInterface> event_queue = actuable->getLocalQueue ();
   }

   if (!event_queue.isValid ())
   {
      Mojo <ActorStageInterface> actor_stage (actor);
      if (actor_stage.isValid ())
      {
         actor_stage->getMultimediaEventQueue (event_queue);
      }

      if (!event_queue.isValid())
      {
         String error_msg = String ("Unable to set the event queue.");
         setErrorString (LogicError (LogicError::INVALID_VALUE, error_msg).errorMessage (), whoAreYou ());
      }
   }
}

/// <summary> Updates the winning animations list and the win outcome for win cycle displays.</summary>
/// <param name = "slot_award">[in] The local award list.</param>
/// <param name = "slot_evaluator">[in] The slot evaluator object.</param>
/// <param name = "award_list">[in] A list of awards for all acts in this stage.</param>
/// <param name = "denomination">[in] The denomination object.</param>
/// <param name = "bet">[in] The bet object.</param>
/// <exception cref="LogicError::INVALID_VALUE"> Thrown when the bet object is not valid.</exception>
void WinCycleLite::updateWinList (Mojo <SlotAwardInterface> & slot_award,
   Mojo <SlotEvaluatorInterface> & slot_evaluator, Mojo <AwardInterface> & award_list,
   const Denomination & denomination, Mojo<BetInterface> & bet)
{
   verboseMethodName ();

   /* check to see if there is any award to display */
   if (bingo_game || (update_win_list && slot_award.isValid () && slot_award->getNumberOfAwards() != 0))
   {
      /* check for a valid bet object prior to proceeding */
      if (!bet.isValid ())
      {
         String error_msg = String ("updateWinList - Bet object is invalid.");
         setErrorString (LogicError (LogicError::INVALID_VALUE, error_msg).errorMessage (), whoAreYou ());
      }

      clearWinOutcome ();

      createWinList (slot_award, slot_evaluator, award_list);

      if (!winning_animations_list.empty ())
      {
         Mojo <SystemAwardInterface> system_award (slot_award);
         award_list->totalDisplayableAmount (win_outcome.total_win_amount,
                                             system_award->getTransactionId ());

         /* sort the winning_animations_list by win classifications and total wins in descending
            order under the same win classification. */
         winning_animations_list.sort (greater <SlotWinEntryWithGrouping> ());

         /* rules take care of animations priorities */
         checkSymbolRules (slot_award, slot_evaluator, award_list, denomination, bet);
         postWinList (bet, denomination);
      }
      // Even if there are no wins on the reels there can still be Bingo wins.
      else if (bingo_game)
      {
         postWinList (bet, denomination);
      }

   }
}

/// <summary> Returns the name of this object.</summary>
const char * WinCycleLite::whoAreYou (void) const
{
   return "WinCycleLite";
}
