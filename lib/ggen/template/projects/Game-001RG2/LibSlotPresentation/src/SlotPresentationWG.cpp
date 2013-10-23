/* (c) Copyright 2007 International Game Technology */

/*
   $Id: SlotPresentationWG.cpp,v 1.134, 2013-08-27 00:31:06Z, Greger.John$
   $Date: 8/27/2013 8:31:06 AM$
   $Revision: 135$

   Purpose:
      Defines the presentation for the world game standards

   Modifications:
      Author          Date          Explanation
      --------------- -----------   ------------
      Rob Savidge     07 Dec 2007   New
*/

#ifndef __ACTORHELPER_HPP__
#include "ActorHelper.hpp"
#endif
#ifndef __ANIMATESLOTWININTERFACE_HPP__
#include "AnimateSlotWinInterface.hpp"
#endif
#ifndef __DENOMINATIONRULES_HPP__
#include "DenominationRules.hpp"
#endif
#ifndef __DYNAMICSLOTBETBUTTONDEFS_HPP__
#include "DynamicSlotBetButtonDefs.hpp"
#endif
#ifndef __GAMEPRESENTATIONHELPER_HPP__
#include "GamePresentationHelper.hpp"
#endif
#ifndef __GAMESIDEBETINTERFACE_HPP__
#include "GameSideBetInterface.hpp"
#endif
#ifndef __ULOCALE_HPP__
#include "ULocale.hpp"
#endif
#ifndef __PAYLINESINTERFACE_HPP__
#include "PaylinesInterface.hpp"
#endif
#ifndef __PROPERTYINTERFACE_HPP__
#include "PropertyInterface.hpp"
#endif
#ifndef __REELSINTERFACE_HPP__
#include "ReelsInterface.hpp"
#endif
#ifndef __SHAREDOBJECTPOOLRESOURCEINTERFACE_HPP__
#include "SharedObjectPoolResourceInterface.hpp"
#endif
#ifndef __SLOTAWARDINTERFACE_HPP__
#include "SlotAwardInterface.hpp"
#endif
#ifndef __SLOTPRESENTATIONRESOURCEDEFS_HPP__
#include "SlotPresentationResourceDefs.hpp"
#endif
#ifndef __SLOTSEQUENCEEVENTS_HPP__
#include "SlotSequenceEvents.hpp"
#endif
#ifndef __SLOTSEQUENCEEVENTPARSER_HPP__
#include "SlotSequenceEventParser.hpp"
#endif
#ifndef __SLOTSHAREDESSENTIALRECORD_HPP__
#include "SlotSharedEssentialRecord.hpp"
#endif
#ifndef __THEATREOPENGLDEFS_HPP__
#include "TheatreOpenGLDefs.hpp"
#endif
#ifndef __USERSEQUENCEEVENT_HPP__
#include "UserSequenceEvent.hpp"
#endif
#ifndef __VIDEOREELSINTERFACE_HPP__
#include "VideoReelsInterface.hpp"
#endif
#ifndef __ANIMATESLOTWINWGINTERFACE_HPP__
#include "AnimateSlotWinWGInterface.hpp"
#endif
#ifndef __RULE_BASED_GAME_SIDE_BET_INTERFACE_HPP__
#include "RuleBasedGameSideBetInterface.hpp"
#endif
#ifndef __SCRIPTRESOURCEIMPORTERINTERFACE_HPP__
#include "ScriptResourceImporterInterface.hpp"
#endif
#ifndef __SLOTCONTROLPANELWG_HPP__
#include "SlotControlPanelWG.hpp"
#endif
#ifndef __SLOTCONTROLPANELWGINTERFACE_HPP__
#include "SlotControlPanelWGInterface.hpp"
#endif
#ifndef __SLOTGAMESETCONFIGURATIONWG_HPP__
#include "SlotGameSetConfigurationWG.hpp"
#endif
#ifndef __SLOTPRESENTATIONWG_HPP__
#include "SlotPresentationWG.hpp"
#endif
#ifndef __SLOTPRESENTATIONWGRESOURCEDEFS_HPP__
#include "SlotPresentationWGResourceDefs.hpp"
#endif
#ifndef __SLOTPRESENTATIONSTATESWG_HPP__
#include "SlotPresentationStatesWG.hpp"
#endif
#ifndef __SEQUENCEEVENTTRANSLATOR_HPP__
#include "SequenceEventTranslator.hpp"
#endif
#ifndef __TEXTUREATTRIBUTEINTERFACE_HPP__
#include "TextureAttributeInterface.hpp"
#endif
#ifndef __TEXTURERESOURCEINTERFACE_HPP__
#include "TextureResourceInterface.hpp"
#endif

#include <algorithm>

#ifdef __DEBUG__
// #define __VERBOSE__
#ifdef __VERBOSE__
//   #define __VERBOSE_METHOD__
//   #define __VERBOSE_METERS__
//   #define __VERBOSE_BET__
//   #define __VERBOSE_CONDITIONS__
//   #define __VERBOSE_INDICATORS__
#endif
#endif

#ifdef __VERBOSE__
 #define verboseMethodName() debug << "SlotPresentationWG::" << __FUNCTION__ << endl;
#else
 #define verboseMethodName()
#endif


// Type of object used in the scripts
const char *const SlotPresentationWG::object_type = SLOT_PRESENTATION_WG;

// Presentation state condition string constants.
const char *const SlotPresentationWG::CONDITION_EXTRA_GAMES_AWARDED = "EXTRA_GAMES_AWARDED";
const char *const SlotPresentationWG::CONDITION_NO_EXTRA_GAMES_AWARDED = "NO_EXTRA_GAMES_AWARDED";
const char *const SlotPresentationWG::CONDITION_SPIN_CAP_REACHED = "SPIN_CAP_REACHED";
const char *const SlotPresentationWG::CONDITION_NOT_SPIN_CAP_REACHED = "NOT_SPIN_CAP_REACHED";
const char *const SlotPresentationWG::CONDITION_RETRIGGER = "RETRIGGER";
const char *const SlotPresentationWG::CONDITION_NOT_RETRIGGER = "NOT_RETRIGGER";
const char *const SlotPresentationWG::CONDITION_TOTAL_BET_EQUALS_SPIN_BET = "CONDITION_TOTAL_BET_EQUALS_SPIN_BET";
const char *const SlotPresentationWG::CONDITION_TOTAL_BET_NOT_EQUALS_SPIN_BET = "CONDITION_TOTAL_BET_NOT_EQUALS_SPIN_BET";
const char *const SlotPresentationWG::CONDITION_TRIGGER = "TRIGGER:";
const char *const SlotPresentationWG::CONDITION_NOT_TRIGGER = "NOT_TRIGGER:";

// Property names
const char *const SlotPresentationWG::PROPERTY_TRIGGER_COUNT_VALUE = "SpinsAwardedValue";
const char *const SlotPresentationWG::PROPERTY_TRIGGER_COUNT_STRING = "SpinsAwarded";
const char *const SlotPresentationWG::PROPERTY_MAX_SPINS_VALUE = "MaxSpinsValue";
const char *const SlotPresentationWG::PROPERTY_MAX_SPINS_STRING = "MaxSpins";
const char *const SlotPresentationWG::PROPERTY_TOTAL_FREE_SPIN_WIN = "TotalFreeSpinWinAmount";
const char *const SlotPresentationWG::PROPERTY_TOTAL_SPINS_AWARDED = "TotalSpinsAwarded";
const char *const SlotPresentationWG::PROPERTY_SPINS_PLAYED = "SpinsPlayed";
const char *const SlotPresentationWG::PROPERTY_SPINS_REMAINING = "SpinsRemaining";
const char *const SlotPresentationWG::PROPERTY_GAMES_AWARDED_VALUE = "GamesAwardedValue";
const char *const SlotPresentationWG::PROPERTY_GAMES_AWARDED_STRING = "GamesAwardedString";

// Actor names
const char *const SlotPresentationWG::ACTOR_TEXT_TOTAL_FEATURE_WIN = "MathBoxText";

// Configuration item value names
const char *const SlotPresentationWG::CONFIG_ITEM_VALUE_AUTOPLAY_STATE_RUNNING = "RUNNING";
const char *const SlotPresentationWG::CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED = "DISABLED";
const char *const SlotPresentationWG::CONFIG_ITEM_VALUE_AUTOPLAY_TYPE_PLAYER = "PLAYER";
const char *const SlotPresentationWG::CONFIG_ITEM_VALUE_AUTOPLAY_TYPE_PLAYER_2_TOUCH = "PLAYER 2 TOUCH";

// Configuration Item Names
const char *const SlotPresentationWG::CONFIG_ITEM_AVAILABLE_TEST_SUBBETS = "AVAILABLE TEST SUBBETS";

// Game pays prefix configurations
const char *const SlotPresentationWG::WIN_TYPE_GAME_PAYS = "GAME_PAYS";
const char *const SlotPresentationWG::WIN_TYPE_TOTAL_GAME_PAYS = "TOTAL_GAME_PAYS";
const char *const SlotPresentationWG::WIN_TYPE_BASE_GAME_PAYS = "BASE_GAME_PAYS";
const char *const SlotPresentationWG::WIN_TYPE_DOUBLE_UP_PAYS = "DOUBLE_UP_PAYS";

//Game pays default strings
const char *const SlotPresentationWG::PREFIX_GAME_PAYS = "GAME PAYS ";
const char *const SlotPresentationWG::PREFIX_TOTAL_GAME_PAYS = "TOTAL GAME PAYS ";
const char *const SlotPresentationWG::PREFIX_BASE_GAME_PAYS = "BASE GAME PAYS ";
const char *const SlotPresentationWG::PREFIX_DOUBLE_UP_PAYS = "DOUBLE UP PAYS ";

// Create an instance of the SequenceEventParser required by the generic slot game presentations.  Since sequence event
// handling can be completely scripted, it is not guaranteed that the parser symbols will be included in the shared
// objects that link the libraries.  Thus, the parser declaration is required here to ensure that the symbols
// included during the linking process.
static const SlotSequenceEventParser slot_sequence_event_parser;

// Purpose:  Initialize the object for use.
void SlotPresentationWG::postConstructor (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the base classes first.
   SlotPresentation::postConstructor ();

   // Set the script functions.
   static const ScriptFunctionMap script_functions [] =
   {
      // Parameters:  Script Function ID, Arguments, Script function name.
      { SET_BET_MESSAGE_FORMAT,                 ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setBetMessageFormat"     },
      { SET_ROLLUP_AFTER_STAGE_DATA,            ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setRollupAfterStageData" },
      { SET_TOTAL_WIN_METER_AND_SHOW_SUMMARY,   0, "setTotalWinMeterAndShowSummary"     },
      { SET_TOTAL_WIN_METER_AND_SHOW_SUMMARY,   3, "setTotalWinMeterAndShowSummary"     },
      { SET_TRIGGER_PROPERTY,                   0, "setTriggerProperty"                 },
      { SET_TRIGGER_PROPERTY,                   1, "setTriggerProperty"                 },
      { SET_WIN_MESSAGE_PREFIX,                 2, "setWinMessagePrefix"                },
      { SYNCHRONIZE_FREE_SPINS,                 0, "synchronizeFreeSpins"               },
      { SYNCHRONIZE_SPIN_COUNT_METERS,          0, "synchronizeSpinCountMeters"         },
      { UPDATE_GAMES_AWARDED_PROPERTY,          0, "updateGamesAwardedProperty"         },
      { SET_SHARED_METER_PANEL,                 5, "setSharedMeterPanel"                },
      { REFRESH_CONTROL_PANEL,                  4, "refreshControlPanel"                },
      { LOCK_METERS,                            1, "lockMeters"                         },
      { ENABLE_BUTTON,                          2, "enableButton"                       },
      { SHOW_BUTTON,                            2, "showButton"                         },
      { SHOW_TEXT,                              2, "showText"                           },
      { ENABLE_METER,                           2, "enableMeter"                        },
      { SHOW_METER,                             2, "showMeter"                          },
      { SET_FREE_SPIN,                          1, "setFreeSpins"                       },
      { ROLLUP_FREE_GAMES_METER,                0, "rollupFreeGamesMeter"               },
      { INTIALIZE_FREE_GAMES_METER_ROLLUP,      0, "initializeFreeGamesMeterRollup"     },
      { SET_FREE_GAMES_METER_ROLLUP_THRESHOLD,  1, "setFreeGamesMeterRollupThreshold"   },
      { ADD_SUB_BET_INDICATOR_MAP_ENTRY,        3, "addSubBetIndicatorMapEntry"         },
      { ADD_SUB_BET_INDICATOR_MAP_ENTRY,        4, "addSubBetIndicatorMapEntry"         },
      { IS_SHARED_METER_PANEL_CONFIGURED,       3, "isSharedMeterPanelConfigured"       },
      { SET_METER_PANEL_CONTEXT,                1, "setMeterPanelContext"               },
      { SET_SHARED_METER_PANEL_BASE,            1, "setSharedMeterPanelBase"            },
      { SET_ROLLUP_AFTER_STAGE,                 1, "setRollupAfterStage"                },
      { SET_DEFAULT_ROLLUP_AFTER_STAGE_RULE,    3, "setDefaultRollupAfterStageRule"     },
      { SET_ROLLUP_AFTER_STAGE_RULE,            6, "setRollupAfterStageRule"            },
      { CALCULATE_ROLLUP_AFTER_STAGE_THRESHOLD, 0, "calculateRollupAfterStageThreshold" },
      { ADD_PRIMARY_STRING_DATA,                1, "addPrimaryStringData"               },
      { CLEAR_PRIMARY_STRING_DATA,              0, "clearPrimaryStringData"             },
      { ADD_SECONDARY_STRING_DATA,              1, "addSecondaryStringData"             },
      { CLEAR_SECONDARY_STRING_DATA,            0, "clearSecondaryStringData"           },
      { HIDE_IN_ACTIVE_SUB_BET_INDICATORS,      1, "hideInActiveSubBetIndicators"       },
      { ADD_WILD_SYMBOL_FOR_DYNAMIC_SEE_PAYS,   2, "addWildSymbolForDynamicSeePays"     },
      { EVALUATE_FOR_SEE_PAYS,                  ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "evaluateForSeePays" },
      { INITIALIZE_SEE_PAYS_VALUES,             0, "initializeSeePaysValues"            },
      { CREATE_GAME_RULES_EVALUATOR,            1, "createGameRulesEvaluator"           },
      { USE_LUA_BET_MESSAGE,                    1, "useLuaBetMessage"                   },
      { GET_NUMBER_OF_MULTIWAYS,                0, "getNumberOfMultiWays"               },
      // Terminator.
      { -1, 0, "" }
   };

   addScriptFunctionMap (script_functions);

   shared_meter_panel_base = false;
   // Initialize member variables
   bet_message_format = "";
   // default win message prefixes
   base_game_pays_prefix = PREFIX_BASE_GAME_PAYS;
   game_pays_prefix = PREFIX_GAME_PAYS;
   total_game_pays_prefix = PREFIX_TOTAL_GAME_PAYS;
   double_up_pays_prefix = PREFIX_DOUBLE_UP_PAYS;
   // set default win message prefix.
   win_message_prefix = PREFIX_GAME_PAYS;
   free_spin_presentation = false;
   limit_mech_paylines_to_button_values = false;
   limit_mech_bet_per_lines_to_button_values = false;
   free_game_meter_rollup_threshold = 15;
   free_game_meter_rollup_count = 0;
   rollup_after_stage = false;
   using_lua_bet_message = false;

   // Language support is not enabled by default. On synchronize, this flag will be adjusted
   language_supported = false;
   // Autoplay button is not enabled by default. On synchronize, this flag will be adjusted
   player_autoplay_supported = false;
}

/// <summary>Prepare the object for destruction.</summary>
void SlotPresentationWG::preDestructor (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Control panels.
   slot_control_panel_wg.clear ();
   // Variable messages to eliminate hard-coded strings
   bet_message_format.clear ();
   bet_message_parameters.clear ();
   sub_bet_indicator_map.clear ();
   dynamic_sub_bet_indicator_map.clear ();
   test_sub_bet_indicator_map.clear ();

   // Call the base class.
   SlotPresentation::preDestructor ();
}
bool SlotPresentationWG::addEventListener (Mojo<MultimediaEventListenerInterface> listener, const char *alias, const char *resource_key)
{
   bool success = false;
   // Retrieve the main stage from the resource manager.
   Mojo<ActorStageInterface> stage;
   if (!getResource (alias, resource_key, stage, false))
   {
      String error_message ("SlotPresentationWG::addEventListener, Stage has not been configured: alias: ");
      error_message << alias << ", key: " << resource_key;
      System::error (LogicError (LogicError::INVALID_VALUE, error_message));
   }

   // If the listener received and the stage extracted are both valid, add the listener to the stage.
   if (listener.isValid ())
   {
      listener->registerListener (stage);
      success = true;
   }

   return (success);
}

// Purpose: Add an entry to map a number of sub bets and extra bet to a sub bet indicator index
void SlotPresentationWG::addSubBetIndicatorMapEntry (uint32 number_of_sub_bets, uint32 extra_bet, uint32 sub_bet_indicator_index, String easy_bet_configuration)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   // Get the type of sub bets used by this game. No sub bet will result in a blank string
   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_evaluator (slot_evaluator);

   if (game_side_slot_evaluator.isValid ())
   {
      SubBetIndicatorMapSubKey new_sub_key (number_of_sub_bets, extra_bet);
      SubBetIndicatorMapKey new_key (new_sub_key, easy_bet_configuration);
      sub_bet_indicator_map [new_key] = sub_bet_indicator_index;
   }
}

// Purpose: Updates the slot win animator interface parameter received with the animator configured for the game set specified.
bool SlotPresentationWG::animateSlotWin (Mojo<AnimateSlotWinWGInterface> &animate_slot_win_wg, uint32 game_set_index)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   animate_slot_win_wg.release ();
   // Extract the game set information.
   Mojo<SlotGameSetInterface> slot_game_set;
   Mojo<AnimateSlotWinInterface> animate_slot_win;
   // Extract the animator from the game set.
   if (slotGameSet (slot_game_set, game_set_index))
   {
      slot_game_set->getAnimateSlotWin (animate_slot_win);
   }
   animate_slot_win_wg = animate_slot_win;

   return (animate_slot_win_wg.isValid ());
}

// Purpose:  Create the (game pays) win message.
void SlotPresentationWG::buildTotalWinMessage (const Amount &win_amount, String &win_message)
{
   #ifdef __VERBOSE_METERS__
   debug << "SlotPresentationWG::buildTotalWinMessage (win_amount, win_message)" << endl;
   debug << "   win_amount: " << win_amount << endl;
   #endif

   // Setup a temp evaluator to get the number of win categories
   Mojo <EvaluatorInterface> temp_evaluator;
   temp_evaluator = slot_evaluator;
   bool display_in_currency = false;
   Mojo<SlotAwardInterface> slot_award;
   WinCategoryId number_of_categories;

   Amount::Extractor amount_extractor (win_amount);
   Amount::AmountValue win_value = amount_extractor.extract (getDenomination ());

   // Clear out the win message string
   win_message.clear ();

   if (win_value == 0)
   {
      display_in_currency = true;
   }
   else if (slotAwardInterface (slot_award))
   {
      number_of_categories = temp_evaluator->getNumberOfWinCategories ();

      // Loop through the win categories to find progressive wins.
      for (WinCategoryId category_index = 0; category_index < number_of_categories; ++category_index)
      {
         // See if this category is progressive.
         if (temp_evaluator->getNumberOfProgressiveEvals (category_index) > 0)
         {
            // See if this category is a win.
            if (slot_award->getNumberOfCategoryAwardItems (category_index) != 0)
            {
               display_in_currency = true;
               break;
            }
         }
      }
   }

   // If we're displaying in currency, extract the win amount using BASE_UNIT instead.
   if (display_in_currency)
   {
      win_value = amount_extractor.extract (Denomination::BASE_UNIT);
   }

   // If there is a win, display the total amount in the control panel.
   if (win_amount > ZERO_AMOUNT)
   {
      // Need to add additional check if the win was actually doubled'up. If so, change the win message
      if (doubleUpWin ())
      {
         win_message = double_up_pays_prefix;
      }
      else
      {
         win_message = game_pays_prefix;
      }

      // This is a prelimiary implementation that should be reworked. The bonus manager
      // currently only supports multiplier type bonuses, but in the future could support
      // more.

      // Build the win amount string
      // Add any bonus manager adjustments
      String win_amount_string;
      uint32 award_multiplier = 0;
      if (extractBonusManagerMultiplierFromAward (award_multiplier))
      {
         win_amount_string << "x " << award_multiplier << " BONUS = ";
      }

      // Add the total win value
      win_amount_string << win_value;
      CountryCode::Country country = locale.moneyNativeCountryCode ();

      // Format the amount correctly, either in currency or credits
      if (display_in_currency)
      {
         locale.moneyFormatNumericWhole (country, win_value, win_amount_string);
      }
      else
      {
         locale.formatNumeric (win_amount_string);
      }
      win_message << win_amount_string;
   }
}

/// <summary>Compare the data in the "RollupData" vector to the bet and credits won to determine how to rollup.</summary>
/// <remarks>By default, this function is passed the current resource key. However, if this function is being called in
/// a bonus, we must pass it the key to the base game so that it can access the property (that lives in the base
/// game) that holds the loop count of the fireworks..</remarks>
/// <param name="resource_key">[in] The resource key used to access properties.</param>
void SlotPresentationWG::calculateRollupAfterStageThreshold (String resource_key)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // The current threshold index into the vector.
   uint32 current_rollup_threshold (0);
   // If an acceptable threshold was found, set this flag.
   bool current_rollup_threshold_found (false);
   bool single_threshold_found (false);
   int32 rollup_loop_count = 0;

   // Get the win amount.
   Amount total_win_amount_from_award (ZERO_AMOUNT);
   // Get the Amount of the TOTAL winnings (Base Game + Bonus)
   extractTotalWinFromAward (total_win_amount_from_award);
   Amount::AmountValue total_win_amount_value_from_award;
   total_win_amount_value_from_award = Amount::Extractor (total_win_amount_from_award).extract (getDenomination ());

    // Check for the win amount in case of partial credit too.
   if (total_win_amount_from_award.isNotZero ())
   {
      RollupRule rollup_rule = rollup_after_stage_rule_default;
      // Take the first RollupRule that passes, otherwise use default rule.
      for (uint32 i = 0; i < rollup_after_stage_rule_vector.size (); i++)
      {
         if (processRollupRule(rollup_after_stage_rule_vector[i]))
         {
            rollup_rule = rollup_after_stage_rule_vector[i];
            break;
         }
      }
      uint32 start_index = rollup_rule.rollup_data_start_index;
      // If the end index is larger than the size of the rollup_after_stage_data_vector, then set the end_index to the rollup_after_stage_data_vector size.
      uint32 end_index = (rollup_rule.rollup_data_end_index >= rollup_after_stage_data_vector.size ()) ?
                          rollup_after_stage_data_vector.size () : rollup_rule.rollup_data_end_index;

      // Get the total bet
      Amount total_bet_amount = bet->getBetAmount ();
      uint32 total_bet_value = static_cast<uint32> (Amount::Extractor (total_bet_amount).extract (bet->getDenomination ()));

      // If the win amount is less than the single credit rollup threshold, then post an event below to signal it.
      if (total_win_amount_value_from_award < single_credit_rollup_threshold)
      {
         current_rollup_threshold_found = true;
         single_threshold_found = true;
      }
      // If the bet amount is greater than the threshold set in script, then calculate the rollup theshold by the percentage range.
      else if (total_bet_value > rollup_rule.celebration_bet_threshold)
      {
         // Find the ratio between the bet and the win.  Use this to find out what range we fall into.
         float f_bet_value (total_bet_value);
         float f_win_value (total_win_amount_value_from_award);
         float win_to_bet_ratio (f_win_value / f_bet_value);

         for (uint32 i = start_index; i <= end_index; i++)
         {
            // Special case for 0 ratio. If it gets in here, there is a win amount which might not be more than a credit.
            // In that case, if the rollup data has a low range of 0, then use that for rollup threshold.
            if ((win_to_bet_ratio == 0.0f && win_to_bet_ratio == rollup_after_stage_data_vector[i].win_to_bet_ratio_low_range) ||
                ((win_to_bet_ratio > rollup_after_stage_data_vector[i].win_to_bet_ratio_low_range) &&
                 (win_to_bet_ratio <= rollup_after_stage_data_vector[i].win_to_bet_ratio_high_range)))
            {
               current_rollup_threshold = rollup_after_stage_data_vector[i].event_threshold_index;
               rollup_loop_count = rollup_after_stage_data_vector[i].celebration_loop_count;
               current_rollup_threshold_found = true;
               break;
            }
         }
      }
      // Otherwise, calculate the rollup threshold by the credit win range.
      else
      {
         for (uint32 i = start_index; i <= end_index; i++)
         {
            // Special case for 0 credit award.  If it gets in here, there is a win amount which might not be more than a credit.
            // In that case, if the rollup data has a low range of 0, then use that for rollup threshold.
            if ((total_win_amount_value_from_award == 0 && total_win_amount_value_from_award == rollup_after_stage_data_vector[i].low_range) ||
                ((total_win_amount_value_from_award > rollup_after_stage_data_vector[i].low_range) &&
                 (total_win_amount_value_from_award <= rollup_after_stage_data_vector[i].high_range)))
            {
               current_rollup_threshold = rollup_after_stage_data_vector[i].event_threshold_index;
               rollup_loop_count = rollup_after_stage_data_vector[i].celebration_loop_count;
               current_rollup_threshold_found = true;
               break;
            }
         }
      }
   }

   if (current_rollup_threshold_found)
   {
      // Extract the property associated from the resource list.
      Mojo<PropertyInterface> property;
      if (resource_list->getResource<PropertyInterface> ("CelebrationLoopCount", resource_key, property))
      {
         // Update the resource with the current value.
         property->setAbsoluteValue (rollup_loop_count);
         postEventAll ("CelebrationLoopCount:Set");
      }

      // Inform the presentation that a threshold was found.
      String event = "RollupAfterStageThresholdFound";
      postEventAll (event);

      // Append the specific threshold index that was found (1 based).
      if (single_threshold_found)
      {
         event << ":SingleRollupThreshold";
      }
      else
      {
         event << ":" << current_rollup_threshold;
      }
      postEventAll (event);

      if (rollup_loop_count > 0)
      {
         postEventAll ("Enable:Celebration");
      }
   }
   else
   {
      // Inform the presentation that a threshold was NOT found.
      postEventAll ("RollupThresholdNotFound");
   }
}

// Purpose: Compare the data in the "RollupData" vector to the bet and credits won to determine how to rollup.
// By default, this function is passed the current resource key.  However, if this function is being called in
// a bonus, we must pass it the key to the base game so that it can access the property (that lives in the base
// game) that holds the loop count of the fireworks.
void SlotPresentationWG::calculateRollupThreshold (String resource_key)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // The current threshold index into the vector.
   uint32 current_rollup_threshold (0);
   // If an acceptable threshold was found, set this flag.
   bool current_rollup_threshold_found (false);
   bool single_threshold_found (false);
   int32 rollup_loop_count = 0;

   // Get the win amount.
   Amount total_win_amount_from_award (ZERO_AMOUNT);
   // Get the Amount of the TOTAL winnings (Base Game + Bonus)
   extractTotalWinFromAward (total_win_amount_from_award);
   Amount::AmountValue total_win_amount_value_from_award;
   total_win_amount_value_from_award = Amount::Extractor (total_win_amount_from_award).extract (getDenomination ());

    // Check for the win amount in case of partial credit too.
   if (total_win_amount_from_award.isNotZero ())
   {
      RollupRule rollup_rule = rollup_rule_default;
      // Take the first RollupRule that passes, otherwise use default rule.
      for (uint32 i = 0; i < rollup_rule_vector.size (); i++)
      {
         if (processRollupRule(rollup_rule_vector[i]))
         {
            rollup_rule = rollup_rule_vector[i];
            break;
         }
      }
      uint32 start_index = rollup_rule.rollup_data_start_index;
      // If the end index is larger than the size of the rollup_data_vector, then set the end_index to the rollup_data_vector size.
      uint32 end_index = (rollup_rule.rollup_data_end_index >= rollup_data_vector.size ()) ?
                            rollup_data_vector.size () : rollup_rule.rollup_data_end_index;

      // Get the total bet
      Amount total_bet_amount = bet->getBetAmount ();
      uint32 total_bet_value = static_cast<uint32> (Amount::Extractor (total_bet_amount).extract (bet->getDenomination ()));

      // If the win amount is less than the single credit rollup threshold, then post an event below to signal it.
      if (total_win_amount_value_from_award < single_credit_rollup_threshold)
      {
         current_rollup_threshold_found = true;
         single_threshold_found = true;
      }
      // If the bet amount is greater than the threshold set in script, then calculate the rollup theshold by the percentage range.
      else if (total_bet_value > rollup_rule.celebration_bet_threshold)
      {
         // Find the ratio between the bet and the win.  Use this to find out what range we fall into.
         float f_bet_value (total_bet_value);
         float f_win_value (total_win_amount_value_from_award);
         float win_to_bet_ratio (f_win_value / f_bet_value);

         for (uint32 i = start_index; i <= end_index; i++)
         {
            // Special case for 0 ratio. If it gets in here, there is a win amount which might not be more than a credit.
            // In that case, if the rollup data has a low range of 0, then use that for rollup threshold.
            if ((win_to_bet_ratio == 0.0f && win_to_bet_ratio == rollup_data_vector[i].win_to_bet_ratio_low_range) ||
                ((win_to_bet_ratio > rollup_data_vector[i].win_to_bet_ratio_low_range) &&
                 (win_to_bet_ratio <= rollup_data_vector[i].win_to_bet_ratio_high_range)))
            {
               current_rollup_threshold = rollup_data_vector[i].event_threshold_index;
               rollup_loop_count = rollup_data_vector[i].celebration_loop_count;
               current_rollup_threshold_found = true;
               break;
            }
         }
      }
      // Otherwise, calculate the rollup threshold by the credit win range.
      else
      {
         for (uint32 i = start_index; i <= end_index; i++)
         {
            // Special case for 0 credit award.  If it gets in here, there is a win amount which might not be more than a credit.
            // In that case, if the rollup data has a low range of 0, then use that for rollup threshold.
            if ((total_win_amount_value_from_award == 0 && total_win_amount_value_from_award == rollup_data_vector[i].low_range) ||
                ((total_win_amount_value_from_award > rollup_data_vector[i].low_range) &&
                 (total_win_amount_value_from_award <= rollup_data_vector[i].high_range)))
            {
               current_rollup_threshold = rollup_data_vector[i].event_threshold_index;
               rollup_loop_count = rollup_data_vector[i].celebration_loop_count;
               current_rollup_threshold_found = true;
               break;
            }
         }
      }
   }

   if (current_rollup_threshold_found)
   {
      // Extract the property associated from the resource list.
      Mojo<PropertyInterface> property;
      if (resource_list->getResource<PropertyInterface> ("CelebrationLoopCount", resource_key, property))
      {
         // Update the resource with the current value.
         property->setAbsoluteValue (rollup_loop_count);
         postEventAll ("CelebrationLoopCount:Set");
      }

      // Inform the presentation that a threshold was found.
      String event = "RollupThresholdFound";
      postEventAll (event);

      // Append the specific threshold index that was found (1 based).
      if (single_threshold_found)
      {
         event << ":SingleRollupThreshold";
      }
      else
      {
         event << ":" << current_rollup_threshold;
      }
      postEventAll (event);

      if (rollup_loop_count > 0)
      {
         postEventAll ("Enable:Celebration");
      }
   }
   else
   {
      // Inform the presentation that a threshold was NOT found.
      postEventAll ("RollupThresholdNotFound");
   }
}

// Purpose: Overridden in order to allow the total win message to be displayed
//   during the pre-bonus rollup when. In the WG transition, the pre-bonus rollup
//   happens in the flow state STATE_EVALUATE instead of STATE_EVALUATE_COMPELTE
bool SlotPresentationWG::canDisplayAward (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // The award can always be displayed while in free spins
   bool can_display_award = isFreeSpins ();

   // If the game is not free spins, check the flow state
   if (can_display_award == false)
   {
      GameStateValue flow_state = getFlowState ();

      // Only return true if the award has been evaluated and we're not in an idle state
      switch (flow_state)
      {
         case GameStateValue::STATE_EVALUATE_COMPLETE:
         // Intentional fall-through
         case GameStateValue::STATE_EVALUATE:
         // Intentional fall-through
         case GameStateValue::STATE_AWARD:
         // Intentional fall-through
         case GameStateValue::STATE_AWARD_COMPLETE:
         // Intentional fall-through
         case GameStateValue::STATE_COMPLETE:
         // Intentional fall-through
         case GameStateValue::STATE_REPLAY:
         // Intentional fall-through
         case GameStateValue::STATE_PLAY:
         {
            can_display_award = true;
         }
         break;

         // If it's not displayable by the current flow state, let the game presentation
         // check other conditions
         default:
         {
            // If not in flow states that now allow win display, use base logic.
            // Call the grandparent because the SlotPresentation will be incorrect
            can_display_award = GamePresentation::canDisplayAward ();
         }
      }
   }

   return (can_display_award);
}

//Purose: This method updates the language swap button.
void SlotPresentationWG::changeGameLanguage (void)
{
   #ifdef __VERBOSE_LANGUAGE__
   debug << "SlotPresentationWG::changeGameLanguage" << endl;
   #endif
   // only attempt to change language in idle state.
   if (isFlowStateIdle ())
   {
      // Ensure that we have valid config ID's before continuing
      if (languageConfigsSupported ())
      {
         // Holds the enabled licensed languages bitfield
         BitField language_bitfield;

         // Get licensed languages bit field
         config.getValueBitField (language_licensed_bitfield_config_id, language_bitfield);

         // Get the size of the "LICENSED LANGUAGE TYPES SUPPORTED LIST".
         uint32 language_supported_list_size (config.getListSize (language_supported_list_config_id));

         if ((language_supported_list_size > 1) && (language_bitfield.numberOfBitsSet () > 1))
         {
            String current_language = "";
            String new_language = "";
            config.getValueString (game_language_config_id, current_language);
            for (uint32 language_bit_index = 0; language_bit_index < language_supported_list_size; ++language_bit_index)
            {
               if (language_bitfield.getBit (language_bit_index))
               {
                  config.getListItem (language_supported_list_config_id, language_bit_index, new_language);
                  if (new_language != current_language)
                  {
                     // start language swap timer
                     postEventContext (SlotControlPanelWG::EVENT_LANGUAGE_SWAP_START, meter_panel_context);

                     // Disable control panels during swapping of language resources
                     enableControlPanels (false);

                     // Update the language config with the new language
                     config.setValue (game_language_config_id, new_language);
                     break;
                  }
               }
            }
         }
      }
   }
}

// Purpose: Provides state transition decision making for presentation states.
// Returns true if all the conditions specified in the string are true.
// All unsupported conditions are stored in unsupported_conditions.
// Unsupported conditions don't effect the return value.
bool SlotPresentationWG::checkPostWaitEventConditions (const vector<String> &conditions, vector<String> &unsupported_conditions, bool force_all)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_CONDITIONS__)
      verboseMethodName()
   #endif
   #if defined (__VERBOSE_CONDITIONS__)
      for (uint32 condition_index = 0; condition_index < conditions.size (); ++condition_index)
      {
         debug << "   " << conditions [condition_index] << ", ";
      }
      debug << endl;
   #endif
   // Initialize return values. All conditions true until one is determined false.
   bool result = true;

   unsupported_conditions.clear ();

   // For every condition specified...
   for (uint32 i = 0; (i < conditions.size ()) && (force_all || result); ++i)
   {
      if (conditions [i] == CONDITION_EXTRA_GAMES_AWARDED)
      {
         result &= processConditionExtraGamesAwarded ();
      }
      else if (conditions [i] == CONDITION_NO_EXTRA_GAMES_AWARDED)
      {
         result &= !processConditionExtraGamesAwarded ();
      }
      else if (conditions [i] == CONDITION_SPIN_CAP_REACHED)
      {
         // Return true if the spin cap has been reached
         result &= processConditionSpinCapReached ();
      }
      else if (conditions [i] == CONDITION_NOT_SPIN_CAP_REACHED)
      {
         // Return true if the spin cap has been reached
         result &= !processConditionSpinCapReached ();
      }
      else if (conditions [i] == CONDITION_RETRIGGER)
      {
         // Return true if the spin cap has been reached
         result &= processConditionRetrigger ();
      }
      else if (conditions [i] == CONDITION_NOT_RETRIGGER)
      {
         // Return true if the spin cap has been reached
         result &= !processConditionRetrigger ();
      }
      else if (conditions [i] == CONDITION_TOTAL_BET_EQUALS_SPIN_BET)
      {
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= processConditionTotalBetEqualsSpinBet ();
      }
      else if (conditions [i] == CONDITION_TOTAL_BET_NOT_EQUALS_SPIN_BET)
      {
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionTotalBetEqualsSpinBet ();
      }
      // we have to check condition NOT_TRIGGER: before TRIGGER: because contains
      // ("TRIGGER:") will be true for both TRIGGER: and NOT_TRIGGER:
      else if (conditions [i].contains (CONDITION_NOT_TRIGGER))
      {
         // Example string is "TRIGGER:Stage1,Stage2".
         // Return true if a hit win category matches one of those specified.
         result &= !processConditionTrigger (conditions [i].right (conditions [i].length () - strlen (CONDITION_NOT_TRIGGER)));
      }
      else if (conditions [i].contains (CONDITION_TRIGGER))
      {
         // Example string is "TRIGGER:Stage1,Stage2".
         // Return true if a hit win category matches one of those specified.
         result &= processConditionTrigger (conditions [i].right (conditions [i].length () - strlen (CONDITION_TRIGGER)));
      }
      else
      {
         // Report conditions not handled by this routine.
         unsupported_conditions.push_back (conditions [i]);
      }
   }

   // If any unhandled conditions...
   if ((force_all || result) && unsupported_conditions.size ())
   {
      // Call base class.
      vector<String> remaining_conditions = unsupported_conditions;
      result &= SlotPresentation::checkPostWaitEventConditions (remaining_conditions, unsupported_conditions, force_all);
   }

   // Return true if all the handled conditions were true.
   return (result);
}

// Purpose: Configure Auto Play message on control panel
void SlotPresentationWG::configureAutoPlayMessage (void)
{
   ConfigID show_autoplay_message_config_id = config.getId (CONFIG_ID_SHOW_AUTOPLAY_MESSAGE, game_configuration_identifier, false);
   if ((show_autoplay_message_config_id != INVALID_CONFIGID) && config.getValue (show_autoplay_message_config_id))
   {
      String autoplay_state_value;
      config.getValueString (autoplay_state_config_id, autoplay_state_value);
      synchronizeAutoplayMessage ();
      //Refresh the autoplay message.
      if (autoplay_state_value != CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED)
      {
         postEventContext (SlotControlPanelWG::EVENT_AUTOPLAY, meter_panel_context);
      }
      else
      {
         postEventContext (SlotControlPanelWG::EVENT_HIDE_GAME_OVER, meter_panel_context);
      }
   }
}

// The following method updates the control panel interface received with the registered world game slot control panel.
// If the control panel is valid, this method will return true.
bool SlotPresentationWG::controlPanel (SlotControlPanelWGExtractor &control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Initialize the return value.
   bool success = false;
   // Since the validity of the control panel is being used as the return value, free the control panel received
   // just in case it is valid mojo.
   control_panel.release ();
   // If the control panel alias has been registered, retrieve the control panel by alias name.
   if (slot_control_panel_wg.length () != 0)
   {
      Mojo<ControlPanelInterface> control_panel_interface;
      // Extract the control panel by registered alias name.
      // Call the grandparent. This function is overridden from SlotPresentation. Calling the
      // grandparent avoids duplicated and incorrect functionality.
      success = GamePresentation::controlPanel (slot_control_panel_wg, control_panel_interface);
      // Initialize the return control panel with the control panel extracted above.
      control_panel.initialize (control_panel_interface);
   }

   // Return true if the control panel configured is valid.
   return (success && control_panel.isValid ());
}

// Purpose: Creates a new game set configuration as appropriate for this presentation.
bool SlotPresentationWG::createGameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Create a new game set configuration manager for this presentation.
   Mojo<SlotGameSetConfigurationWG, GameSetConfigurationInterface> slot_game_set_configuration;
   slot_game_set_configuration.create<SlotGameSetConfigurationWG> ();
   // Extract the GameSetConfigurationInterface and update the reference parameter.
   game_set_configuration_interface = slot_game_set_configuration;

   return (game_set_configuration_interface.isValid ());
}

// Return an interface to an object given its id.
bool SlotPresentationWG::createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared)
{
   #if defined (__VERBOSE_SCRIPTABLE_OBJECTS__)
      verboseMethodName()
   #endif
   bool object_created (false);
   switch (object_id)
   {
      case SEQUENCE_EVENT_TRANSLATOR:
      {
         // Create a new SequenceEventTranslator.
         Mojo<SequenceEventTranslator, SequenceEventTranslatorInterface> sequence_event_translator;
         sequence_event_translator.create<SequenceEventTranslator> ();
         if (!sequence_event_translator.isValid ())
         {
            System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::createScriptableObjectById, Unable to create SequenceEventTranslator."));
         }

         // Allow the object to operate on its parameters.
         sequence_event_translator->initializeCreatedObject (parameter_list);
         // Configure the stage used to post events.
         sequence_event_translator->setStage (parameter_list [1].asString (), parameter_list [2].asString ());
         // Configure the translator's sequence event poster.
         sequence_event_translator->setSequenceEventPoster (sequence_event_poster);

         // Add the translator to the list of MultimediaEventListeners.
         Mojo<MultimediaEventListenerInterface> multimedia_event_listener (sequence_event_translator);
         addEventListener (multimedia_event_listener, parameter_list [1].asString (), parameter_list [2].asString ());
         assertion (multimedia_event_listener.isValid ());
         // Add the translator to the list of SequenceEventListeners.
         Mojo<SequenceEventListenerInterface> sequence_event_listener (sequence_event_translator);
         GamePresentation::addEventListener (sequence_event_listener);
         assertion (sequence_event_listener.isValid ());

         // Update the return value with the script interface for the new translator.
         new_object = sequence_event_translator;
         assertion (new_object.isValid ());
         object_created = true;
      }
      break;

      default:
         object_created = SlotPresentation::createScriptableObjectById (new_object, object_id, parameter_list, new_declared);
         break;
   }

   return (object_created);
}

/// <summary> Shows or hides the game pays message.</summary>
/// <remarks> The section of code to support using_win_cycle_lite can be removed once
///   the information bars are controlled on the script side.</remarks>
/// <param name = "show">[in] Boolean flag to show or hide the game pays message.</param>
void SlotPresentationWG::displayTotalWinMessage (bool show)
{
   #if defined (__VERBOSE_WIN__) || defined (__VERBOSE_METHOD__)
   debug << "SlotPresentationWG::displayTotalWinMessage (" << (show ? "true" : "false") << ")" << endl;
   #endif

   bool using_win_cycle_lite = false;
   Mojo<SlotGameSetInterface> slot_game_set;

   if (slotGameSet (slot_game_set, currentGameSetIndex ()))
   {
      using_win_cycle_lite = slot_game_set->isWinCycleLiteConfigured ();
   }

   // The section of code below to handle the win cycle lite case would not be needed once
   // the information bars are controlled by script changes. It is added to support a special case
   // for double up win pays messages.
   if (using_win_cycle_lite)
   {
      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         // Get a handle to the game control panel interface
         Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();

         if (show)
         {
            Amount total_game_win = ZERO_AMOUNT;
            extractTotalWinFromAward (total_game_win);

            if (total_game_win != ZERO_AMOUNT)
            {
               if (doubleUpWin ())
               {
                  if (using_lua_bet_message)
                  {
                     int64 double_up_win = Amount::Extractor (total_game_win).extract (denomination);
                     ScriptTableParameter payload;
                     payload.script_table ["double_up_win"] = ScriptParameter (double_up_win, ParameterType::INT64);
                     postEventContextExtended ("ShowDoubleUpPaysString", "Context.MeterPanel", 0, &payload);
                  }
                  else
                  {
                     String total_win_string;
                     total_win_string << Amount::Extractor (total_game_win).extract (denomination);
                     String win_message = double_up_pays_prefix;
                     win_message << total_win_string;
                     gcp_interface->setText (SlotControlPanelWG::TEXT_INFORMATION2, win_message);
                  }
               }
            }
            else // there is no win information to show when there is not a win.
            {
               show = false;
            }
         }

         gcp_interface->showText (SlotControlPanelWG::TEXT_INFORMATION2, show);
         gcp_interface->showText (SlotControlPanelWG::TEXT_TOTAL_WIN_INFO, show);
      }
   }
   else
   {
      if (show)
      {
         win_display.update (this);
         win_display.start ();
      }
      else
      {
         win_display.stop ();
      }
   }
}

// Purpose: To force enable/disable the autoplay button
void SlotPresentationWG::enableAutoplayButton (bool new_state)
{
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get a handle to the interface from the control panel extractor
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getControlPanelInterface ();
      // Make sure auto play stop button is off
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, false);
      gcp_interface->showButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, false);
      // enable auto play button.
      gcp_interface->enableButton (GameControlPanel::BUTTON_AUTOPLAY, new_state);
   }
}

// Purpose: To force enable/disable the autoplay stop button
void SlotPresentationWG::enableAutoplayStopButton (bool new_state)
{
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get a handle to the interface from the control panel extractor
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getControlPanelInterface ();
      if (new_state)
      {
         // Update the auto play button to enable or disable the exact opposite of auto play stop.
         // We want only one of the buttons to be enabled at a time.
         gcp_interface->enableButton (GameControlPanel::BUTTON_AUTOPLAY, !new_state);
         // show/enable auto play stop button.
         gcp_interface->enableButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, new_state);
         gcp_interface->showButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, new_state);
      }
   }
}

// Purpose: Enable a button by its name
void SlotPresentationWG::enableButton (const char *button_name, bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      uint32 button_id =  slot_control_panel_wg.getGameControlPanelInterface ()->getButtonIdByName (button_name);
      slot_control_panel_wg.getGameControlPanelInterface ()->enableButton (button_id, enable);
   }
}

// Purpose: Show a button by its name
void SlotPresentationWG::showButton (const char *button_name, bool show)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      uint32 button_id =  slot_control_panel_wg.getGameControlPanelInterface ()->getButtonIdByName (button_name);
      slot_control_panel_wg.getGameControlPanelInterface ()->showButton (button_id, show);
   }
}

// Purpose: Enable a meter by its name
void SlotPresentationWG::enableMeter (const char *meter_name, bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      uint32 meter_id =  slot_control_panel_wg.getGameControlPanelInterface ()->getMeterIdByName (meter_name);
      slot_control_panel_wg.getGameControlPanelInterface ()->enableMeter (meter_id, enable);
   }
}

// Purpose: Show a meter by its name
void SlotPresentationWG::showMeter (const char *meter_name, bool show)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      uint32 meter_id =  slot_control_panel_wg.getGameControlPanelInterface ()->getMeterIdByName (meter_name);
      slot_control_panel_wg.getGameControlPanelInterface ()->showMeter (meter_id, show);
   }
}

// Purpose: Show a text area by its name
void SlotPresentationWG::showText (const char *text_name, bool show)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      uint32 text_id = slot_control_panel_wg.getGameControlPanelInterface ()->getTextIdByName (text_name);
      slot_control_panel_wg.getGameControlPanelInterface ()->showText (text_id, show);
   }
}

// Purpose:  Called to enable all known control panels.
void SlotPresentationWG::enableControlPanels (bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the grandparent. This function is overridden from SlotPresentation. Calling the
   // grandparent avoids duplicated and incorrect functionality.
   GamePresentation::enableControlPanels (enable);
   // Enable the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   // There should never be an iGameControlPanel and a SlotControlPanelWG in the same presentation
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getControlPanelInterface ()->enable (enable);
   }
}

// Purpose:  enable the Double up Listener to allow rollup for double up wins to the credit meter...
void SlotPresentationWG::enableDoubleUpListener ()
{
   const EssentialRecord *essential_record = getEssentialRecord ();

   if (essential_record->doubleup_permitted)
   {
      postEventContext ("EnableDoubleUpListener", meter_panel_context);
   }
}

void SlotPresentationWG::enableGameMenuButtons (bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Enable the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getGameControlPanelInterface ()->enableButton (SlotControlPanelWG::BUTTON_GAME_MENU, enable);
   }
}

void SlotPresentationWG::enableMoreGamesButton (bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Enable the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      if (numberOfAvailablePlayerGames () > 1)
      {
         slot_control_panel_wg.getGameControlPanelInterface ()->enableButton (SlotControlPanelWG::BUTTON_MORE_GAMES, enable && !bank_bet.isBankLocked ());
      }
   }
}

// Purpose: Enable the appropriate "play" button for enrollment double-tap feature.
void SlotPresentationWG::enableEnrollDoubleTapButton (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Enable the orginally pressed "play" button (if needed) to support player
   // double tap (i.e. player convenience feature to enroll & spin by double tapping the same button).
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get a handle to the interface from the control panel extractor
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getControlPanelInterface ();

      switch (getEnrollmentButtonDoubleTapButtonId ())
      {
         case ENROLL_DBL_TAP_SPIN_BUTTON:
         {
            // No need to enable spin button.
         }
         break;

         case ENROLL_DBL_TAP_MAX_BET_BUTTON:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_MAX_BET, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_1:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_1, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_2:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_2, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_3:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_3, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_4:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_4, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_5:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_5, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_6:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_6, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_7:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_7, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_8:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_8, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_9:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_9, true /* ok_to_enable */);
         }
         break;

         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_10:
         {
            gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_10, true /* ok_to_enable */);
         }
         break;

         default:
            break;
      }
   }
}

// Enable the max bet and bet N buttons for enrollment double-tap feature.
void SlotPresentationWG::enableBettingButtonsForDoubleTap (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Enable the max bet and bet N buttons to support player
   // double tap (i.e. player convenience feature to enroll & spin by double tapping the same button).

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      String value_string;
      Config config;
      ConfigID enrollment_start_method_id = config.getId ("GAME ENROLLMENT START METHOD");
      config.getValueString (enrollment_start_method_id, value_string);

      // Get a handle to the interface from the control panel extractor
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getGameControlPanelInterface ();

      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_MAX_BET, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_1, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_2, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_3, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_4, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_5, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_6, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_7, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_8, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_9, true);
      gcp_interface->enableButton (SlotControlPanelWG::BUTTON_BET_N_COINS_10, true);

      if (value_string == "ON_GAME_START_REQUEST")
      {
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_MAX_BET, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_1, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_2, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_3, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_4, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_5, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_6, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_7, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_8, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_9, true);
         gcp_interface->flashButton (SlotControlPanelWG::BUTTON_BET_N_COINS_10, true);
      }
   }
}

// Purpose: Refresh the control panel if a shared meter panel is being used
bool SlotPresentationWG::enterGameMode (State& state, const GameMode &game_mode, const ContextMode &context_mode)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   if(shared_meter_panel_base)
   {
      postEventAll ("ControlPanel:RefreshResources");
   }
   return SlotPresentation::enterGameMode (state, game_mode, context_mode);
}

// Purpose:  Execute a function given its id.
bool SlotPresentationWG::execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_SCRIPT__)
      verboseMethodName()
   #endif
   #if defined (__VERBOSE_SCRIPT__)
      debug << "   func_id: " << func_id << endl;
      debug << "   parameter_list: " << endl;
      for (uint32 p_count = 0; p_count < parameter_list.getParameterCount (); ++p_count)
      {
         if (parameter_list[p_count].parameterType () == ParameterType::STRING)
         {
            debug << parameter_list[p_count].asString () << ", ";
         }
         else if (parameter_list[p_count].parameterType () == ParameterType::BOOL)
         {
            debug << "bool: " << parameter_list[p_count].asBool () << ", ";
         }
         else if (parameter_list[p_count].parameterType () == ParameterType::UINT64)
         {
            debug << parameter_list[p_count].asUint64 () << ", ";
         }
      }
      debug << endl;
   #endif

   bool success (true);
   switch (func_id)
   {
      case SET_BET_MESSAGE_FORMAT:
      {
         uint32 parameter_count = parameter_list.getParameterCount ();
         if (parameter_count < 1)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, "SET_BET_MESSAGE_FORMAT needs at least one parameter.").errorMessage (), whoAreYou ());
            return false;
         }
         bet_message_format = parameter_list [0].asString ();

         bet_message_parameters.clear ();
         for (uint32 parameter_index = 1; parameter_index < parameter_count; ++parameter_index)
         {
            bet_message_parameters.push_back (parameter_list [parameter_index].asString ());
         }
      }
      break;

      case SET_TOTAL_WIN_METER_AND_SHOW_SUMMARY:
      {
         // Default to the main stage
         String _stage_name (getStageName ());
         String _resource_key (resource_list->resourceKey ());
         String _actor_name (ACTOR_TEXT_TOTAL_FEATURE_WIN);
         // If a parameter was passed, use that as the stage on which the meter to be
         // updated exists.
         if (parameter_list.getParameterCount () == 3)
         {
            _actor_name = parameter_list [0].asString ();
            _stage_name = parameter_list [1].asString ();
            _resource_key = parameter_list [2].asString ();
         }
         setTotalWinMeterAndShowSummary (_actor_name, _stage_name, _resource_key);
      }
      break;

      case SET_TRIGGER_PROPERTY:
      {
         bool post_update_message (true);
         if (parameter_list.getParameterCount () == 1)
         {
            post_update_message = parameter_list [0].asBool ();
         }
         setTriggerProperty (post_update_message);
      }
      break;

      case SET_WIN_MESSAGE_PREFIX:
      {
         setWinMessagePrefix (parameter_list [0].asString (), parameter_list [1].asString ());
      }
      break;

      case SYNCHRONIZE_FREE_SPINS:
      {
         synchronizeFreeSpins ();
      }
      break;

      case SYNCHRONIZE_SPIN_COUNT_METERS:
      {
         synchronizeSpinCountMeters ();
      }
      break;

      case UPDATE_GAMES_AWARDED_PROPERTY:
      {
         updateGamesAwardedProperty ();
      }
      break;

      case SET_SHARED_METER_PANEL:
      {
         setSharedMeterPanel (parameter_list [0].asString (), parameter_list [1].asString (),
            parameter_list [2].asString (), parameter_list [3].asString (), parameter_list [4].asString ());
      }
      break;

      case REFRESH_CONTROL_PANEL:
      {
         SlotControlPanelWGExtractor slot_control_panel_wg;
         if (controlPanel (slot_control_panel_wg))
         {
            if(!shared_meter_panel_base)
            {
               SlotControlPanelWGExtractor slot_control_panel_wg;
               if (controlPanel (slot_control_panel_wg))
               {
                  Mojo <ResourceInterface> context;
                  Mojo <ResourceInterface> context2;
                  if(getResource <ResourceInterface> (parameter_list [1].asString (), parameter_list [3].asString (), context2, false) &&
                     getResource <ResourceInterface> (parameter_list [1].asString (), parameter_list [2].asString (), context, false))
                  {
                     //slot_control_panel_wg.getSlotControlPanelWGInterface ()->releaseResources ();
                     resource_list->removeResource (context2);
                  }
               }
            }
            slot_control_panel_wg.getSlotControlPanelWGInterface ()->refreshResources(parameter_list [0].asString (), parameter_list [2].asString (),parameter_list [3].asString ());
         }
      }
      break;

      case LOCK_METERS:
      {
         lockMeters (parameter_list [0].asBool ());
      }
      break;

      case ENABLE_BUTTON:
      {
         enableButton (parameter_list [0].asString (), parameter_list [1].asBool ());
      }
      break;

      case SHOW_BUTTON:
      {
         showButton (parameter_list [0].asString (), parameter_list [1].asBool ());
      }
      break;

      case ENABLE_METER:
      {
         enableMeter (parameter_list [0].asString (), parameter_list [1].asBool ());
      }
      break;

      case SHOW_METER:
      {
         showMeter (parameter_list [0].asString (), parameter_list [1].asBool ());
      }
      break;

      case SHOW_TEXT:
      {
         showText (parameter_list [0].asString (), parameter_list [1].asBool ());
      }
      break;

      case SET_FREE_SPIN:
      {
         free_spin_presentation = parameter_list [0].asBool ();
      }
      break;

      case ROLLUP_FREE_GAMES_METER:
      {
         rollupFreeGamesMeter ();
      }
      break;

      case INTIALIZE_FREE_GAMES_METER_ROLLUP:
      {
         initializeFreeGamesMeterRollup ();
      }
      break;
      case SET_FREE_GAMES_METER_ROLLUP_THRESHOLD:
      {
         free_game_meter_rollup_threshold = parameter_list [0].asUint32 ();
      }
      break;

      case ADD_SUB_BET_INDICATOR_MAP_ENTRY:
      {
         if (parameter_list.getParameterCount () == 3)
         {
            addSubBetIndicatorMapEntry (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (), parameter_list [2].asUint32 (), "");
         }
         else if (parameter_list.getParameterCount () == 4)
         {
            addSubBetIndicatorMapEntry (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (), parameter_list [2].asUint32 (), parameter_list [3].asString ());
         }
      }
      break;

      case IS_SHARED_METER_PANEL_CONFIGURED:
      {
         bool is_shared_meter_panel_configured = isSharedMeterPanelConfigured (parameter_list [0].asString (),
            parameter_list [1].asString (), parameter_list [2].asString ());
         ScriptParameter return_parameter (is_shared_meter_panel_configured, ParameterType::BOOL);
         parameter_list.add (return_parameter);
      }
      break;

      case SET_METER_PANEL_CONTEXT:
      {
         setMeterPanelContext (parameter_list [0].asString ());
      }
      break;

      case SET_SHARED_METER_PANEL_BASE:
      {
         setSharedMeterPanelBase (parameter_list [0].asBool ());
      }
      break;

      case SET_ROLLUP_AFTER_STAGE:
      {
         rollup_after_stage = parameter_list [0].asBool ();
      }
      break;

      case SET_ROLLUP_AFTER_STAGE_DATA:
      {
         float win_to_bet_ratio_high_range (0.0f);
         Amount::AmountValue high_range (0);
         uint32 event_threshold_index (0);
         if (parameter_list.getParameterCount() != 5 && parameter_list.getParameterCount() != 6)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupAfterStageData has invalid number of parameters").errorMessage (), whoAreYou ());
            return false;
         }
         // Check for max float in ratio range.
         if (parameter_list [1].parameterType () == ParameterType::STRING)
         {
            if (!strcmp (parameter_list [1].asString (), "FLOAT_MAX"))
            {
               win_to_bet_ratio_high_range = _FLOAT_MAX;
            }
            else
            {
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupAfterStageData has invalid parameter string const MAX_FLOAT.").errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            win_to_bet_ratio_high_range = parameter_list [1].asFloat ();
         }

         // Check for max int in win range.
         if (parameter_list [3].parameterType () == ParameterType::STRING)
         {
            // Comparison to UINT32_MAX is here for backwards compatibility.
            // As a note, Lua integers can only be a maximum of 32-bits.
            if (String (parameter_list [3].asString ()) == "UINT32_MAX" ||
                String (parameter_list [3].asString ()) == "UINT64_MAX" ||
                String (parameter_list [3].asString ()) == "INT64_MAX")
            {
               high_range = Amount::MAX_ABSOLUTE_AMOUNT_VALUE;
            }
            else
            {
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupAfterStageData has invalid parameter string const MAX INT.").errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            high_range = parameter_list [3].asInt64 ();
         }

         if (parameter_list.getParameterCount() == 6)
         {
            event_threshold_index = parameter_list [5].asUint32 ();
         }
         // Parameters are: win_to_bet_ratio_low_range, win_to_bet_ratio_high_range, low_range,
         //                 high_range, celebration_loop_count
         setRollupAfterStageData (parameter_list [0].asFloat (), win_to_bet_ratio_high_range, parameter_list [2].asInt64 (),
                                  high_range, parameter_list [4].asInt32 (), event_threshold_index);
      }
      break;

      case SET_DEFAULT_ROLLUP_AFTER_STAGE_RULE:
      {
         uint32 start_index = parameter_list [0].asUint32 ();
         uint32 end_index = parameter_list [1].asUint32 ();

         // Sanity check
         if (end_index >= rollup_after_stage_data_vector.size ())
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "setDefaultRollupAfterStageRule (), end index exceeds number of RollupModes defined").errorMessage (), whoAreYou ());
            return false;
         }
         if (start_index > end_index)
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "setDefaultRollupAfterStageRule(), start index must be less than end index").errorMessage (), whoAreYou ());
            return false;
         }
         setDefaultRollupAfterStageRule (start_index, end_index, parameter_list [2].asUint32 ());
      }
      break;

      case SET_ROLLUP_AFTER_STAGE_RULE:
      {
         RollupCriteria criteria = ROLLUP_CRITERIA_NONE;
         RollupCompareTarget target = ROLLUP_COMPARE_NONE;

         if (parameter_list [3].parameterType () == ParameterType::STRING)
         {
            if (strcmp (parameter_list [3].asString (), "MAX_BET_PER_LINE") == 0)
               target = ROLLUP_COMPARE_MAX_BET_PER_LINE;
            else if (strcmp (parameter_list [3].asString (), "DENOM") == 0)
               target = ROLLUP_COMPARE_DENOM;
            else
            {
               String error_message;
               error_message << "setRollupAfterStageRule: RollupTarget " << parameter_list [3].asString () << " not recognized.";
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, error_message).errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupAfterStageRule, expected String in arg4").errorMessage (), whoAreYou ());
            return false;
         }

         if (parameter_list [4].parameterType () == ParameterType::STRING)
         {
            if (strcmp (parameter_list [4].asString (), "LESS_THAN") == 0)
               criteria = ROLLUP_CRITERIA_LESS_THAN;
            else if (strcmp (parameter_list [4].asString (), "LESS_THAN_OR_EQUALS") == 0)
               criteria = ROLLUP_CRITERIA_LESS_THAN_OR_EQUALS;
            else if (strcmp (parameter_list [4].asString (), "GREATER_THAN") == 0)
               criteria = ROLLUP_CRITERIA_GREATER_THAN;
            else if (strcmp (parameter_list [4].asString (), "GREATER_THAN_OR_EQUALS") == 0)
               criteria = ROLLUP_CRITERIA_GREATER_THAN_OR_EQUALS;
            else if (strcmp (parameter_list [4].asString (), "EQUALS") == 0)
               criteria = ROLLUP_CRITERIA_EQUALS;
            else
            {
               String error_message;
               error_message << "setRollupAfterStageRule: RollupCriteria " << parameter_list [3].asString () << " not recognized.";
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, error_message).errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupAfterStageRule, expected String in arg5").errorMessage (), whoAreYou ());
            return false;
         }
         setRollupAfterStageRule (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (),
                                  parameter_list [2].asUint32 (), target, criteria, parameter_list [5].asUint32 ());
      }
      break;

      case CALCULATE_ROLLUP_AFTER_STAGE_THRESHOLD:
      {
         Mojo<ResourceList, ScriptInterface> resource_list;
         if (resourceList (resource_list))
         {
            calculateRollupAfterStageThreshold (resource_list->resourceKey ());
         }
      }
      break;

      case ADD_PRIMARY_STRING_DATA:
      {
         String primary_string;
         parameter_list [0].getParameterString (primary_string);
         addPrimaryStringData (primary_string);
      }
      break;

      case CLEAR_PRIMARY_STRING_DATA:
      {
         clearPrimaryStringData ();
      }
      break;

      case ADD_SECONDARY_STRING_DATA:
      {
         String secondary_string;
         parameter_list [0].getParameterString (secondary_string);
         addSecondaryStringData (secondary_string);
      }
      break;

      case CLEAR_SECONDARY_STRING_DATA:
      {
         clearSecondaryStringData ();
      }
      break;

      case HIDE_IN_ACTIVE_SUB_BET_INDICATORS:
      {
         // Get a mojo to the control panel interface.
         SlotControlPanelWGExtractor slot_control_panel_wg;
         if (controlPanel (slot_control_panel_wg))
         {
            bool hide_inactive_sub_bet_indicators = parameter_list [0].asBool ();
            Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface ();
            // Show or hide the inactive sub bet indicators accordingly
            scpwg_interface->hideInActiveSubBetIndicators (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, hide_inactive_sub_bet_indicators);
            scpwg_interface->hideInActiveSubBetIndicators (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, hide_inactive_sub_bet_indicators);
         }
      }
      break;

      case ADD_WILD_SYMBOL_FOR_DYNAMIC_SEE_PAYS:
      {
         addWildSymbolForDynamicSeePays (parameter_list[0].asString (), parameter_list[1].asString ());
      }
      break;
      
      case EVALUATE_FOR_SEE_PAYS:
      {
         uint32 parameter_count = parameter_list.getParameterCount ();
         if (parameter_count == 1)
         {
            evaluateForSeePays (parameter_list[0].asString ());
         }
         else if (parameter_count == 2)
         {
            evaluateForSeePays (parameter_list[0].asString (), parameter_list[1].asBool ());
         }
         else
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, "EVALUATE_FOR_SEE_PAYS has invalid parameter count.").errorMessage (), whoAreYou ());
            return false;
         }
      }
      break;

      case INITIALIZE_SEE_PAYS_VALUES:
      {
         initializeSeePaysValues ();
      }
      break;
      
      case CREATE_GAME_RULES_EVALUATOR:
      {
         createGameRulesEvaluator (parameter_list [0].asString ());
      }
      break;
      
      //Purpose: to switch off the usage of setBetMessage and utilize a lua based multi language bet message
      case USE_LUA_BET_MESSAGE:
         {
            uint32 parameter_count = parameter_list.getParameterCount ();
            if (parameter_count != 1)
            {
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, "SET_BET_MESSAGE_FORMAT needs at least one parameter.").errorMessage (), whoAreYou ());
               return false;
            }
            
            if (parameter_list[0].parameterType () != ParameterType::BOOL)
            {
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, "SET_IGNITE_STYLE_BEHAVIORS paramater one must be of type bool").errorMessage (), whoAreYou ());
               return false;
            }
            using_lua_bet_message = parameter_list[0].asBool ();
         }
         break;
         
      case GET_NUMBER_OF_MULTIWAYS:
         {
            uint16 multiways = numberOfMultiWaysSelected();
            ScriptParameter return_parameter (multiways, ParameterType::UINT16);
            parameter_list.add (return_parameter);
         }
         break;
         
      default:
      {
         success = (SlotPresentation::execScriptableFunctionById (func_id, parameter_list));
      }
      break;
   }

   return (success && (!error_set_by_function));
}

// Purpose: This method is called by the leavingObjectScope method to finalize the configuration
// of the control panels registered by this game.
void SlotPresentationWG::finalizeControlPanelConfiguration (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the grandparent. This function is overridden from SlotPresentation. Calling the
   // grandparent avoids duplicated and incorrect functionality.
   GamePresentation::finalizeControlPanelConfiguration ();

   // Finalize the configuration of all control panels registered by this game.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Configure the denomination of the new control panel.
      slot_control_panel_wg.getControlPanelInterface ()->setDenomination (denomination);
      // Configure the control panel with this class' sequence event poster.  This assignment ensures that
      // sequence events posted by the control panel are properly configured with game and stage information.
      slot_control_panel_wg.getControlPanelInterface ()->setSequenceEventPoster (sequence_event_poster);

      // Grab an interface to the current game stage
      StageInterfaces stage;
      if (getStage (stage, false))
      {
         // Set the notification context for the control panel to the current stage context
         // This will allow the state machine to hear meter notifications
         slot_control_panel_wg.getSlotControlPanelWGInterface ()->setNotificationContext (stage.getActorStageInterface ()->getContext ());
      }
   }
}

//Purpose:  Get the bet multiplier with or without the word "CREDIT(S)" appended to it
String SlotPresentationWG::getBetMultiplierString (bool append_credits_string)
{
   String bet_string ("");

   //make sure the bet is valid
   if (bet.isValid ())
   {
      Amount::Extractor amount_extractor (bet->getSubBetAmount (0));
      uint32 bet_value = amount_extractor.extract (denomination);
      String credits_string ((bet_value == 1 ? " CREDIT" : " CREDITS"));
      bet_string << bet_value << (append_credits_string ? credits_string : "");
   }
   return bet_string;
}

/// <summary>Get the range of sub bets allowed by the current bet configuration.</summary>
/// <param name="bet_per_sub_bet_min">[out] minimum bet per sub bet</param>
/// <param name="bet_per_sub_bet_max">[out] maximum bet per sub bet</param>
void SlotPresentationWG::getBetPerSubBetRange (uint32 &bet_per_sub_bet_min, uint32 &bet_per_sub_bet_max)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif

   if (doesBetSupportGameSideBetInterface ())
   {
      // Use the bet options to determine if the bet has been limited to a permitted list
      BetOptions bet_options;
      bet->getBetOptions (bet_options, false);

      // If the bet has been limited to a permitted bet list...
      if (bet_options.limit_bets_to_permitted_list)
      {
         // Get the game side bet interface
         Mojo<GameSideBetInterface> game_side_bet_interface (bet);
         // Get the max and min bet ranges
         game_side_bet_interface->getBetPerSubbetRangesFromPermittedBetList (bet_per_sub_bet_min, bet_per_sub_bet_max);
      }
      // Else, base the min and max bet per sub bet on the current config item values
      else
      {
         // Used in calculating the max bet per sub bet
         ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
         int32 sub_bets_per_credit (config.getValue (config_id));

         // Used for calculating the max bet per sub bet
         config_id = config.getId ("NUMBER OF SUBBETS", stage_configuration_identifier);
         int32 max_number_of_sub_bets (config.getValue (config_id));

         // The minumum bet per sub bet is the minimum bet
         config_id = config.getId ("MINIMUM TOTAL BET", stage_configuration_identifier);
         bet_per_sub_bet_min = (config.getValue (config_id));

         // Used in calculating the max bet per sub bet
         config_id = config.getId ("MAXIMUM TOTAL BET", game_configuration_identifier);
         int32 max_total_bet (config.getValue (config_id));

         Mojo <GameSideBetInterface> game_side_bet_interface (bet);

         if (game_side_bet_interface.isValid ())
         {
            max_total_bet = game_side_bet_interface->getMaxBetValue ();
         }

         // If there is a bonus bet, it must be taken into account for the max bet per sub bet
         if (bonusBetTypeGame ())
         {
            config_id = config.getId ("BONUS BET MULTIPLIER", stage_configuration_identifier);
            int32 bonus_bet_multiplier (config.getValue (config_id));

            // Remove the bonus bet factor when calculating the maximum bet per line
            bet_per_sub_bet_max = (max_total_bet * sub_bets_per_credit) / (bonus_bet_multiplier + max_number_of_sub_bets);
         }
         else
         {
            // Since there is no bonus bet, the max bet per line is only based on the number of lines
            bet_per_sub_bet_max = ((max_total_bet * sub_bets_per_credit) / max_number_of_sub_bets);
         }
      }
   }
   #if defined (__VERBOSE_BET__)
      debug << "   bet_per_sub_bet_min: " << bet_per_sub_bet_min << endl;
      debug << "   bet_per_sub_bet_max: " << bet_per_sub_bet_max << endl;
   #endif
}

//Purpose:  Get the bonus bet with or without the word "CREDIT(S)" appended to it
String SlotPresentationWG::getBonusBetString (bool append_credits_string)
{
   String bet_string ("");

   //make sure the bet is valid
   if (bet.isValid () && bet->getBetAmountByName ("bonus") > ZERO_AMOUNT)
   {
      Amount::Extractor amount_extractor (bet->getBetAmountByName ("bonus"));
      uint32 bonus_bet_value = amount_extractor.extract (denomination);
      String credits_string ((bonus_bet_value == 1 ? " CREDIT" : " CREDITS"));
      bet_string << bonus_bet_value << (append_credits_string ? credits_string : "");
   }
   return bet_string;
}

// Purpose: Query the BetNCoins buttons for the intervals to display on the sub bet indicators
void SlotPresentationWG::getLineIntervalsFromBetNButtons (uint32 number_of_intervals, vector<uint32> &sub_bet_intervals)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   // Make sure the output is cleared of any extra data and sized correctly
   sub_bet_intervals.clear ();
   sub_bet_intervals.resize (number_of_intervals);

   Mojo<GameSideBetInterface> game_side_bet_interface (bet);
   if (game_side_bet_interface.isValid ())
   {
      // Keep track of the number of paylines selected by each button
      uint32 payline_count = 0;
      // Keep track of the extra bet amount for each button
      uint32 extra_bet = 0;
      // Store the BetNButton data
      vector<ButtonData> bet_n_values;
      // Get the values to select from the physical bet_n_buttons
      game_side_bet_interface->getBetNButtonPanelData (bet_n_values);

      #if defined (__VERBOSE_INDICATORS__)
      debug << "   bet_n_values.size(): " << bet_n_values.size () << endl;
      #endif

      // For each different payline amount, add a listing to the primary strings
      int32 interval_index = number_of_intervals - 1;
      for (int32 bet_n_index = bet_n_values.size () - 1;
           bet_n_index >= 0 && interval_index >= 0; --bet_n_index)
      {
         // If the paylines to select from the current button index do not match that of the
         // previous button index, or the extra bets value of the current button is different
         // we add it to the sub bet indicators primary text area
         if (payline_count != bet_n_values [bet_n_index].sub_bets || extra_bet != bet_n_values [bet_n_index].extra_bet)
         {
            // Update the new current payline count value
            payline_count = bet_n_values [bet_n_index].sub_bets;
            // Update the new current extra bet value
            extra_bet = bet_n_values [bet_n_index].extra_bet;
            // Set the current index of the primary strings for the sub bet indicator
            sub_bet_intervals [interval_index] = payline_count;

            #if defined (__VERBOSE_INDICATORS__)
            debug << "---------------------" << endl;
            debug << "   bet_n_index: " << bet_n_index << endl;
            debug << "   interval_index: " << interval_index << endl;
            debug << "   payline_count: " << payline_count << endl;
            debug << "   extra_bet: " << extra_bet << endl;
            debug << "---------------------" << endl;
            #endif

            // Move to the next location for the primary strings
            --interval_index;
         }
      }
   }
}

/// <summary>Get the sub bet indicator intervals for a line game using the permitted bet list.</summary>
/// <param name="number_of_intervals">[in] Size of array to set up regardless of the number of sub bets in the permitted bet list.</param>
/// <param name="sub_bet_intervals">[out] Array of sub bets contained in permitted bet list.</param>
void SlotPresentationWG::getLineIntervalsFromPermittedBetList (uint32 number_of_intervals, vector<uint32> &sub_bet_intervals)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   vector<uint32> permitted_sub_bets;
   Mojo <GameSideBetInterface> game_side_bet_interface (bet);

   // Make sure the output is cleared of any extra data and sized correctly
   sub_bet_intervals.clear ();
   sub_bet_intervals.resize (number_of_intervals);

   // Check if the bet object supports the PermittedListBetInterface before calling on it.
   if (game_side_bet_interface.isValid ())
   {
      game_side_bet_interface->getSubbetValuesFromPermittedBetList (permitted_sub_bets);
   }

   //fill in the sub bet intervals from the top down
   int32 permitted_sub_bet_index = permitted_sub_bets.size () - 1;
   for (int32 interval_index(number_of_intervals - 1);
        interval_index >= 0 && permitted_sub_bet_index >= 0;
        --interval_index, --permitted_sub_bet_index)
   {
      sub_bet_intervals [interval_index] = permitted_sub_bets [permitted_sub_bet_index];

      #if defined (__VERBOSE_INDICATORS__)
         debug << "-----------------------------" << endl;
         debug << "   sub_bet_intervals [" << interval_index << "]: " << sub_bet_intervals [interval_index] << endl;
         debug << "-----------------------------" << endl;
      #endif
   }
}

// Purpose: Query the SelectNLines buttons for the values to display on the sub bet indicators
void SlotPresentationWG::getLineIntervalsFromSelectNButtons (uint32 number_of_intervals, vector<uint32> &sub_bet_intervals)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   Mojo<GameSideBetInterface> game_side_bet_interface (bet);
   if (game_side_bet_interface.isValid ())
   {
      // Keep track of the number of paylines selected by each button
      uint32 payline_count = 0;
      // Vector to store the line counts from the SelectNLines buttons
      vector<uint32> select_n_values;
      // Get the values to select from the physical select_n_buttons
      game_side_bet_interface->getSelectNButtonPanelValues (select_n_values);

      #if defined (__VERBOSE_INDICATORS__)
      debug << "   select_n_values.size(): " << select_n_values.size () << endl;
      #endif

      // Iterate through each payline button
      int32 interval_index = number_of_intervals - 1;
      for (int32 select_n_index = select_n_values.size () - 1;
           select_n_index >= 0 && interval_index >= 0; --select_n_index)
      {
         // If the number of paylines selected by this button is different from the previous button
         if (payline_count != select_n_values [select_n_index])
         {
            // Update the current payline count
            payline_count = select_n_values [select_n_index];

            // If this button is used to enable Bonus Bet or BuyAScatterBet,
            // only display the max number of paylines, instead of paylines + 1
            if (payline_count > numberOfPaylines ())
            {
               sub_bet_intervals [interval_index] = numberOfPaylines ();
            }
            else
            {
               sub_bet_intervals [interval_index] = select_n_values [select_n_index];
            }
            #if defined (__VERBOSE_INDICATORS__)
            debug << "-----------------------------" << endl;
            debug << "   select_n_index: " << select_n_index << endl;
            debug << "   interval_index: " << interval_index << endl;
            debug << "   payline_count: " << payline_count << endl;
            debug << "   sub_bet_intervals [" << interval_index << "]: " << sub_bet_intervals [interval_index] << endl;
            debug << "-----------------------------" << endl;
            #endif

            // Move to the next primary index
            --interval_index;
         }
      }
   }
}

// Purpose: Returns the number of games triggered by the current award
int32 SlotPresentationWG::getNumberOfGamesAwarded (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   Mojo<SystemAwardInterface> system_award;
   int32 trigger_count (0);
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   if (local_award.isValid ())
   {
      // Iterate through each award with the current transaction ID
      uint16 number_of_awards = local_award->getNumberOfAwards (evaluationTransactionId ());
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         // For each award, there is a number of system awards
         local_award->getAward (evaluationTransactionId (), award_index, system_award);
         uint16 number_of_system_awards = system_award->getNumberOfAwards ();
         for (uint16 system_award_index = 0; system_award_index < number_of_system_awards; ++system_award_index)
         {
            // For each system award, there may be triggers
            uint16 number_of_triggers = system_award->getNumberOfTriggers (system_award_index);
            for (uint16 trigger_index = 0; trigger_index < number_of_triggers; ++trigger_index)
            {
               const char *system_award_name = system_award->getTriggerString (system_award_index, trigger_index);
               if (strlen (system_award_name) == 0 || system_award_name[0] != TRIGGER_PREFIX_RETRIGGER)
               {
                  // If there are triggers, find out how many spins are awarded
                  trigger_count += system_award->getTriggerCount (system_award_index, trigger_index);
               }
            }
         }
         system_award.release ();
      }
   }

   return (trigger_count);
}

// Purpose: Return conditional that allows including award list credits during win rollups
bool SlotPresentationWG::includeAwardListCredits (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName()
   #endif

   // Call the base class first
   const EssentialRecord * essential_record = getEssentialRecord ();
   return (SlotPresentation::includeAwardListCredits () || (!essential_record->moneyWinCalled () && !isRollupAfterStage ()));
}

// Purpose: Initialize the Free Games meters
//          Set the total spins and spins played to the proper
//          start values.
void SlotPresentationWG::initializeFreeGamesMeterRollup (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Set both the play count amount and games award amount to zero to display '0 of 0' on the Free Games meter.
      uint32 start_game_amount (0);
      uint32 start_play_count (0);
      Amount play_count_amount (start_play_count, Denomination::BASE_UNIT);
      Amount games_awarded_amount (start_game_amount, Denomination::BASE_UNIT);

      Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();
      gcp_interface->enableMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, true);
      gcp_interface->setMeter (SlotControlPanelWG::METER_SPINS_PLAYED, play_count_amount);
      gcp_interface->setMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, games_awarded_amount);
   }
}

// Purpose: Initialize the presentation.
//          This call is made before the game runs.  This is the point where all shared
//          objects should be loaded and checked for validity.
//          The current state show enable/show/play the correct items once they
//          are loaded here.
bool SlotPresentationWG::init (State& state, bool cold_start_flag)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool success = SlotPresentation::init (state, cold_start_flag);

   if (success)
   {
      // Set the ranges of the sliders.
      initializeSliders ();
      initializeSubBetIndicators ();
   }

   return (success);
}

/// <summary>Initialize the sliders for use. The bet up/bet down buttons are implemented as a slider.</summary>
/// <remarks>Set the range and lockdown values of sliders used by the presentation.</remarks>
void SlotPresentationWG::initializeSliders (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
   int32 number_of_lines_interval (config.getValue (config_id));

   config_id = config.getId ("NUMBER OF SUBBETS", stage_configuration_identifier);
   number_of_lines_max_value = (config.getValue (config_id));

   config_id = config.getId ("MINIMUM TOTAL BET", stage_configuration_identifier);
   uint32 bet_per_sub_bet_min (config.getValue (config_id));

   config_id = config.getId ("MAXIMUM TOTAL BET", game_configuration_identifier);
   int32 max_total_bet (config.getValue (config_id));

   Mojo <GameSideBetInterface> game_side_bet_interface (bet);

   if (game_side_bet_interface.isValid ())
   {
      max_total_bet = game_side_bet_interface->getMaxBetValue ();
   }

   initializeBonusBet();

   if (bonusBetTypeGame() == true)
   {
      config_id = config.getId ("BONUS BET MULTIPLIER", stage_configuration_identifier);
      int32 bonus_bet_multiplier (config.getValue (config_id));

      // Remove the bonus bet factor when calculating the maximum bet per line
      bet_per_line_max_value = (max_total_bet * number_of_lines_interval) / (bonus_bet_multiplier + number_of_lines_max_value);
   }
   else
   {
      // Since there is no bonus bet, the max bet per line is only based on the number of lines
      bet_per_line_max_value = ((max_total_bet * number_of_lines_interval) / number_of_lines_max_value);
   }

   // Make sure we have a valid world game slot control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Grab a mojo to the interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
      // Get the range of valid bets
      getBetPerSubBetRange (bet_per_sub_bet_min, (uint32 &) bet_per_line_max_value);
      // Initialize the slider in the control panel
      scpwg_interface->initializeSlider(SlotControlPanelWG::SLIDER_BET_PER_SUB_BET, bet_per_sub_bet_min, bet_per_line_max_value, 1);
   }
}

/// <summary>Redistribute passed sub bet intervals for display purposes.</summary>
/// <remarks>The non-zero values in the array are distributed so that they are aligned at max index of the array
/// and contain an equal number of spaces between each value.
/// The size of the array is not changed.
/// The order of the non-zero values is not changed.</remarks>
/// <param name="sub_bet_intervals">[inout] Array with values to redistribute.</param>
/// <example><b>Example:</b><br>
/// @extExampleStart{SlotPresentationWG}
/// @extExample{evenlySpaceSubBetIndicators}
/// @extExampleEnd
/// </example>
void SlotPresentationWG::evenlySpaceSubBetIndicators (vector<uint32> &sub_bet_intervals)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   uint32 sub_bet_count (0), space_size(0), max_index(0);

   // count the number of sub bets
   for (uint32 interval_index (0); interval_index < sub_bet_intervals.size (); ++interval_index)
   {
      if (sub_bet_intervals [interval_index] != 0)
      {
         ++sub_bet_count;
      }
   }

   // if there is more than one sub bet interval and more than one blank space then redistribute the intervals
   if (sub_bet_count > 1 && (sub_bet_intervals.size () - sub_bet_count) > 1)
   {
      // determine the size of the spacing between the sub bet intervals
      while (((space_size + 1) * (sub_bet_count - 1) + sub_bet_count) <= sub_bet_intervals.size ())
      {
         ++space_size;
      }

      max_index = sub_bet_intervals.size () - 1;

      // apply new spacing to sub bet intervals
      for (uint32 current_index (0); current_index < max_index; ++current_index)
      {
         if (sub_bet_intervals [current_index] != 0)
         {
            uint32 new_index = max_index - (max_index - current_index) * (space_size + 1);
            sub_bet_intervals [new_index] = sub_bet_intervals [current_index];
            sub_bet_intervals [current_index] = 0;
         }
      }

      #if defined (__VERBOSE_INDICATORS__)
         debug << "-----------------------------" << endl;
         for (uint32 interval_index (0); interval_index < sub_bet_intervals.size (); ++interval_index)
         {
            debug << "   sub_bet_intervals [" << interval_index << "]: " << sub_bet_intervals [interval_index] << endl;
         }
         debug << "-----------------------------" << endl;
      #endif
   }
}

/// <summary>Initialize the sub bet indicator bars based on the current bet configuration.</summary>
void SlotPresentationWG::initializeSubBetIndicators (void)
{
   // Make sure we have a valid world game slot control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;

   if (!doesBetSupportGameSideBetInterface () && controlPanel (slot_control_panel_wg))
   {
      // Grab a mojo to the interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();

      // There's no sense in calculating this if the sub bet indicator has already been configured
      // (through this function, or from script). This assumes the left indicator exists, and if the
      // left indicator has been cofigured, the right one was configured at the same time.
      if (scpwg_interface->isSubBetIndicatorConfiguredFromScript (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT) == false)
      {
         // Initialize the sub bet indicators
         scpwg_interface->initializeSubBetIndicator (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, primary_string_list, secondary_string_list);
         scpwg_interface->initializeSubBetIndicator (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, primary_string_list, secondary_string_list);
      }
      else
      {
         scpwg_interface->setIndicatorEasyBetConfiguration (getEasyBetConfiguration ());
      }
   }
   else
   {
      initializeSubBetIndicatorsWithButtonMap ();
   }
}

/// <summary>Initialize the sub bet indicator bars based on the current bet configuration.</summary>
void SlotPresentationWG::initializeSubBetIndicatorsWithButtonMap (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   if (bet.isValid () == false)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::initializeSubBetIndicators(), bet is not valid."));
   }

   // Make sure we have a valid world game slot control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;
   // Make sure we have a valid game side bet interface
   Mojo<GameSideBetInterface> game_side_bet_interface (bet);
   if (game_side_bet_interface.isValid () && controlPanel (slot_control_panel_wg))
   {
      // Grab a mojo to the interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();

      // There's no sense in calculating this if the sub bet indicator has already been configured
      // (through this function, or from script). This assumes the left indicator exists, and if the
      // left indicator has been cofigured, the right one was configured at the same time.
      if (scpwg_interface->isSubBetIndicatorConfiguredFromScript (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT) == false)
      {
         // Get the values to select from the physical select_n_buttons
         vector<uint32> select_n_values;
         game_side_bet_interface->getSelectNButtonPanelValues (select_n_values);

         // Set the sub bet indicator interval count and resize the vectors accordingly
         uint32 interval_count = scpwg_interface->getSubBetIndicatorIntervalCount (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT);
         vector<String> primary_strings (interval_count);
         vector<String> secondary_strings (interval_count);
         vector<uint32> sub_bet_intervals (interval_count);

         #if defined (__VERBOSE_INDICATORS__)
         debug << "   select_n_values.size (): " << select_n_values.size () << endl;
         debug << "   interval_count: " << interval_count << endl;
         #endif

         // Get the type of sub bets used by this game. No sub bet will result in a blank string
         Mojo<GameSideSlotEvaluatorInterface> game_side_slot_evaluator (slot_evaluator);
         if (game_side_slot_evaluator.isValid ())
         {
            // If this is a lines only game
            if (slot_evaluator->getNumberOfLines () > 0 && game_side_slot_evaluator->getNumberOfMultiWaySymbols () == 0)
            {
               #if defined (__VERBOSE_INDICATORS__)
               debug << "   Sub bet type: LINE" << endl;
               #endif

               BetOptions bet_options;
               bet->getBetOptions (bet_options, true);

               Mojo <GameSideBetInterface> game_side_bet_interface (bet);
               vector<uint32> valid_subbets;

               Config config;
               ConfigID test_subbets_id = config.getId (CONFIG_ITEM_AVAILABLE_TEST_SUBBETS, theme_configuration_identifier, false);
               // If the game is in Test Mode, get custom intervals
               if (bet->getBetMode () == BetInterface::BET_MODE_TEST && (test_subbets_id != INVALID_CONFIGID))
               {
                  uint32 list_size = config.getListSize (test_subbets_id);
                  String list_string;

                  test_sub_bet_indicator_map.clear ();
                  for (uint32 indicator_index = 0; indicator_index < list_size; ++indicator_index)
                  {
                     config.getListItem (test_subbets_id, indicator_index, list_string);
                     sub_bet_intervals [indicator_index] = list_string.asUint32 ();

                     SubBetIndicatorMapSubKey new_sub_key (list_string.asUint32 (), 0);
                     SubBetIndicatorMapKey new_key (new_sub_key, getEasyBetConfiguration ());
                     test_sub_bet_indicator_map [new_key] = indicator_index + 1;
                  }
               }
               // If the permitted bet list used, get intervals from it.
               else if (bet_options.limit_bets_to_permitted_list)
               {
                  getLineIntervalsFromPermittedBetList (interval_count, sub_bet_intervals);
                  evenlySpaceSubBetIndicators (sub_bet_intervals);

                  //set up the dynamic sub bet indicator map
                  dynamic_sub_bet_indicator_map.clear ();
                  for (uint32 indicator_index (0); indicator_index < sub_bet_intervals.size (); ++indicator_index)
                  {
                     SubBetIndicatorMapSubKey new_sub_key (sub_bet_intervals[indicator_index], 0);
                     SubBetIndicatorMapKey new_key (new_sub_key, getEasyBetConfiguration ());
                     dynamic_sub_bet_indicator_map [new_key] = indicator_index + 1;
                  }
               }
               // If easy bet is enabled, or the select lines buttons don't exist, use bet n data
               else if (select_n_values.size () == 0 || game_side_bet_interface->getEasyBetConfiguration () != 0)
               {
                  getLineIntervalsFromBetNButtons (interval_count, sub_bet_intervals);
               }
               else // Not easy bet, with Select N Lines buttons
               {
                  getLineIntervalsFromSelectNButtons (interval_count, sub_bet_intervals);
               }

               // Populate the primary and secondary string vectors
               for (uint32 interval_index = 0; interval_index < interval_count; ++interval_index)
               {
                  // Convert each select n value to a string
                  if (sub_bet_intervals [interval_index] == 0)
                  {
                     // The sub bet value is unsupportred. clear the strings for this index
                     primary_strings [interval_index] = "";
                     secondary_strings [interval_index] = "";
                  }
                  else
                  {
                     primary_strings [interval_index] << sub_bet_intervals [interval_index];

                     secondary_strings [interval_index] = "LINE";
                     if (sub_bet_intervals [interval_index] > 1)
                     {
                        secondary_strings [interval_index] << "S";
                     }
                  }
               }
            }

            #if defined (__VERBOSE_INDICATORS__)
            else if (game_side_slot_evaluator->getNumberOfMultiWaySymbols () > 0)
            {
               // There is currently no sensible way to map a sub bet count to
               // the number of ways selected, so for now multi-way games will
               // have to manually configure their sub bet indicators through script.
               debug << "   Sub bet type: WAY" << endl;
            }
            #endif
         }

         // Initialize the sub bet indicators
         scpwg_interface->initializeSubBetIndicator (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, primary_strings, secondary_strings);
         scpwg_interface->initializeSubBetIndicator (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, primary_strings, secondary_strings);
      }
      else
      {
         scpwg_interface->setIndicatorEasyBetConfiguration (getEasyBetConfiguration ());
      }
   }
}

// Purpose: Is Auto Play currently in a running state?
bool SlotPresentationWG::isAutoPlayRunning (void)
{
   String autoplay_state_value = "";
   config.getValueString (autoplay_state_config_id, autoplay_state_value);
   return (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_RUNNING);
}

// Purpose: Is this presenation a freespin presentation
bool SlotPresentationWG::isFreeSpins (void)
{
   return (free_spin_presentation);
}

// Purpose: Is this presentation using the rollup after bonus feature
bool SlotPresentationWG::isRollupAfterStage (void)
{
   return (rollup_after_stage);
}

/// <summary>Returns true if the meter panel has been configured on the meter panel stage, or the current stage.
///    Returns false if the stage was not found with the given resource key.</summary>
/// <param name="stage">[in] The name of the stage the meter panel should be configured on</param>
/// <param name="meter_panel_key">[in] The resource key that should be used to configure the meter panel in normal game play</param>
/// <param name="resource_key">[in] The resource key used to configure the meter panel in the current game stage</param>
/// <returns>True if the stage was found with either of the given resource keys, otherwise false</returns>
/// <example><b>Example:</b><br>
/// @extExampleStart{SlotPresentationWG}
/// @extExample{isSharedMeterPanelConfigured}
/// @extExampleEnd
/// </example>
/// @scriptableFunction{SlotPresentationWG,isSharedMeterPanelConfigured}
bool SlotPresentationWG::isSharedMeterPanelConfigured (const char *stage, const char *meter_panel_key, const char *resource_key)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool meter_panel_configured (false);
   Mojo<ActorStageInterface> stage_interface;

   // Check to see if the meter panel exists using the meter panel resource key
   if (getResource <ActorStageInterface> (stage, meter_panel_key, stage_interface, false))
   {
      meter_panel_configured = true;
   }
   // If the meter panel didn't exist in the meter panel key, and the meter panel key doesn't match the
   // current presentation key, check to see if the meter panel stage exists in the current presentation key
   else if (strcmp (meter_panel_key, resource_key) != 0)
   {
      if (getResource <ActorStageInterface> (stage, resource_key, stage_interface, false))
      {
         meter_panel_configured = true;
      }
   }

   return (meter_panel_configured);
}

// Purpose: Called when we are about to leave the object's scope.
void SlotPresentationWG::leavingObjectScope (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the base class.
   SlotPresentation::leavingObjectScope ();
}

// Purpose: Lock or unlock the common meters to prevent/enable updating
void SlotPresentationWG::lockMeters (bool lock)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Get a handle to the control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // lock or unlock the meters
      slot_control_panel_wg.getGameControlPanelInterface ()->lockMeter (GameControlPanel::METER_WAGERABLE, lock);
      slot_control_panel_wg.getGameControlPanelInterface ()->lockMeter (GameControlPanel::METER_CREDIT, lock);
      slot_control_panel_wg.getGameControlPanelInterface ()->lockMeter (GameControlPanel::METER_CASH, lock);
      slot_control_panel_wg.getGameControlPanelInterface ()->lockMeter (GameControlPanel::METER_WIN, lock);
      slot_control_panel_wg.getGameControlPanelInterface ()->lockMeter (GameControlPanel::METER_TOTAL_BET, lock);
   }
}

// Purpose: Loads the properties that are used to toggle the win messages in history after a bonus.
void SlotPresentationWG::loadWinMessagesInHistoryProperties (bool toggle_win_message, bool show_total_win_credit_meter_value)
{
   #ifdef __VERBOSE_METERS__
      debug << "SlotPresentationWG::loadWinMessagesInHistoryProperties ()" << endl;
   #endif

   // Fetch the ER for data grabbing.
   const SlotEssentialRecord* essential_record = getSlotEssentialRecord ();
   assertion (essential_record);

   Amount total_win_amount_from_award (ZERO_AMOUNT);
   Amount bonus_win_amount_from_award (ZERO_AMOUNT);
   Amount base_win_amount_from_award (ZERO_AMOUNT);
   Amount mismatch_win_amount_offset_from_award (ZERO_AMOUNT);
   Amount displayed_game_pays_amount (ZERO_AMOUNT);

   // Get the Amount of JUST the bonus winnings
   extractBonusWinFromAward (bonus_win_amount_from_award);
   // Get the Amount of the TOTAL winnings (Base Game + Bonus)
   extractTotalWinFromAward (total_win_amount_from_award);
   // Get the Amount of the mismatch adjustment (expected amount - actual amount won)
   extractMismatchWinAdjustmentFromAward (mismatch_win_amount_offset_from_award);

   base_win_amount_from_award = total_win_amount_from_award - bonus_win_amount_from_award;
   displayed_game_pays_amount = total_win_amount_from_award - mismatch_win_amount_offset_from_award;

   Amount::AmountValue total_win_amount_value_from_award;
   Amount::AmountValue bonus_win_amount_value_from_award;
   Amount::AmountValue total_base_game_win_amount_value;
   Amount::AmountValue base_game_credit_meter_before_bonus_value;

   total_win_amount_value_from_award = Amount::Extractor (total_win_amount_from_award).extract (Denomination::BASE_UNIT);
   bonus_win_amount_value_from_award = Amount::Extractor (bonus_win_amount_from_award).extract (Denomination::BASE_UNIT);
   total_base_game_win_amount_value = total_win_amount_value_from_award - bonus_win_amount_value_from_award;

   base_game_credit_meter_before_bonus_value = (essential_record->accounting.player_amount_at_stage_start +
    (total_win_amount_value_from_award - bonus_win_amount_value_from_award)) - essential_record->accounting.total_bet;

   Mojo<PropertyInterface> property;

   // Set the base game pays string property
   String base_game_pays_string;
   if (toggle_win_message)
   {
      base_game_pays_string = base_game_pays_prefix;
   }
   else
   {
      if (doubleUpWin ())
      {
         base_game_pays_string = double_up_pays_prefix;
      }
      else
      {
         base_game_pays_string = game_pays_prefix;
      }
   }

   Amount::Extractor base_win_amount_extractor (base_win_amount_from_award);
   String base_game_pays_win_amount_string;
   base_game_pays_win_amount_string << base_win_amount_extractor.extract (denomination);
   locale.formatNumeric (base_game_pays_win_amount_string);
   base_game_pays_string << base_game_pays_win_amount_string;

   if (resource_list->getResource<PropertyInterface> (BASE_GAME_PAYS_HISTORY, resource_list->resourceKey (), property))
   {
      property->setAbsoluteValue (base_game_pays_string);
   }
   else
   {
      String error_message;
      error_message << "SlotPresentation::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << BASE_GAME_PAYS_HISTORY;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   // Set the total game pays string property
   String total_game_pays_string = total_game_pays_prefix;
   Amount::Extractor total_win_amount_extractor (displayed_game_pays_amount);
   String total_game_pays_win_amount_string;
   total_game_pays_win_amount_string << total_win_amount_extractor.extract (denomination);
   locale.formatNumeric (total_game_pays_win_amount_string);
   total_game_pays_string << total_game_pays_win_amount_string;

   if (resource_list->getResource<PropertyInterface> (TOTAL_GAME_PAYS_HISTORY, resource_list->resourceKey (), property))
   {
      property->setAbsoluteValue (total_game_pays_string);
   }
   else
   {
      String error_message;
      error_message << "SlotPresentationWG::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << TOTAL_GAME_PAYS_HISTORY;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   // Set the base game win meter property
   if (resource_list->getResource<PropertyInterface> (BASE_GAME_WIN_HISTORY, resource_list->resourceKey (), property))
   {
      property->setAbsoluteValue (total_base_game_win_amount_value);
   }
   else
   {
      String error_message;
      error_message << "SlotPresentationWG::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << BASE_GAME_WIN_HISTORY;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }
   // Set the total game win meter property
   if (resource_list->getResource<PropertyInterface> (TOTAL_GAME_WIN_HISTORY, resource_list->resourceKey (), property))
   {
      property->setAbsoluteValue (total_win_amount_value_from_award);
   }
   else
   {
      String error_message;
      error_message << "SlotPresentationWG::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << TOTAL_GAME_WIN_HISTORY;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   // Set the base game win meter property
   if (resource_list->getResource<PropertyInterface> (CREDIT_METER_VALUE_BEFORE_BONUS, resource_list->resourceKey (), property))
   {
      property->setAbsoluteValue (base_game_credit_meter_before_bonus_value);
   }
   else
   {
      String error_message;
      error_message << "SlotPresentationWG::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << CREDIT_METER_VALUE_BEFORE_BONUS;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
      if (!isBingo ())
      {
         if (toggle_win_message)
         {
            scpwg_interface->update (SlotControlPanelWG::UPDATE_TOGGLE_WIN_MESSAGES_IN_HISTORY);
         }
         else
         {
            scpwg_interface->update (SlotControlPanelWG::UPDATE_SHOW_BASE_GAME_WIN_MESSAGE);
         }

         if (!show_total_win_credit_meter_value)
         {
            // This event is posted if the base game credit meter in History on a bonus initiating act is to
            // diplay the pre-bonus credit meter value (includes initiating act winnings)
            scpwg_interface->update (SlotControlPanelWG::UPDATE_SHOW_PRE_BONUS_CREDIT_METER_VALUE);
         }
      }
   }
}

// Purpose: determine the max total plays
int32 SlotPresentationWG::maxTotalPlays (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Refresh the award
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   // assume trigger not found
   bool trigger_found = false;
   uint32 max_total_plays = 0;

   // Get the award
   Mojo <SystemAwardInterface> game_award;

   // find award that launched
   uint32 num_awards = local_award->getNumberOfAwards ();
   for (uint32 award_index = 0; award_index < num_awards; ++award_index)
   {
      // release the Mojo before using
      // here instead of end because we loop early
      game_award.release ();

      // Get the award
      bool success = local_award->getAward (award_index, game_award);
      assertion (success);
      // ignore self
      if (strcmp (game_award->getName (), currentPaytableSection ()) == 0)
      {
         continue;
      }

      uint32 num_award_items = game_award->getNumberOfAwards ();
      // loop through the award items
      for (uint32 award_item_index = 0; award_item_index < num_award_items; ++award_item_index)
      {
         uint32 num_triggers = game_award->getNumberOfTriggers (award_item_index);
         // loop through the trigger items
         for (uint32 trigger_index = 0; trigger_index < num_triggers; ++trigger_index)
         {
            // If trigger name is valid, compare it to our target trigger
            const char* trigger_name = game_award->getTriggerString (award_item_index, trigger_index);
            if (trigger_name && strlen (trigger_name))
            {
               // looking to trigger that launched current stage
               if (strcmp (trigger_name, currentPaytableSection ()) == 0)
               {
                  if (!trigger_found)
                  {
                     max_total_plays = game_award->getTriggerMaxTotalPlays (award_item_index, trigger_index);
                     trigger_found = true;
                  }
                  else
                  {
                     System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::maxTotalPlays: more than one initial trigger found"));
                  }
               }
            }
         }
      }
   }

   return (max_total_plays);
}

// Purpose: Final preparations before this presentation is unloaded.
bool SlotPresentationWG::prepareForUnloading (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Release custom evaluators
   for (uint32 game_side_evaluator_index = 0; game_side_evaluator_index < dynamic_game_rules_evaluators.size (); ++ game_side_evaluator_index)
   {
      dynamic_game_rules_evaluators[game_side_evaluator_index].game_side_evaluator_interface.release ();
      dynamic_game_rules_evaluators[game_side_evaluator_index].evaluator_interface.release ();
   }

   slot_control_panel_wg.clear ();

   // Give the base class a chance to unload its resources.
   return (SlotPresentation::prepareForUnloading ());
}

// Purpose: Returns the number of extra free games were awarded this act
uint16 SlotPresentationWG::extraGamesAwarded (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Get a pointer to the essential record
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);

   return slot_essential_record->totalRetriggers ();
}

// Purpose: Returns true if extra free games were awarded this act
bool SlotPresentationWG::processConditionExtraGamesAwarded (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   return (extraGamesAwarded () > 0);
}

// Purpose: Return true if this act retriggered
bool SlotPresentationWG::processConditionRetrigger (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool monetary_win, trigger_another, trigger_self, success;
   success = GamePresentationHelper::awardSummary (this, monetary_win, trigger_another, trigger_self);

   #ifdef __VERBOSE_CONDITIONS__
   debug << "SlotPresentationWG::processConditionRetrigger?" ;
   debug << (trigger_self ? "Yes" : "No") << endl;
   #endif

   return (success && trigger_self);
}

// Purpose: Return true if this game is done playing.
bool SlotPresentationWG::processConditionSpinCapReached (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool spin_cap_reached = false;

   if (playCount (ACT_TYPE_BONUS) == maxTotalPlays ())
   {
      spin_cap_reached = true;
   }
   #ifdef __VERBOSE_CONDITIONS__
   debug << "SlotPresentationWG::processConditionSpinCapReached?" ;
   debug << (spin_cap_reached ? "Yes" : "No") << endl;
   #endif

   return (spin_cap_reached);
}

// Purpose: Return true if the retrigger was good for additional spins
bool SlotPresentationWG::processConditionTotalBetEqualsSpinBet (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool result = false;
   // grab a copy of the essential record
   const SlotEssentialRecord* record = getSlotEssentialRecord ();

   // Get initiating bet.
   Amount spin_bet_amount;
   if (record)
   {
      if (bet.isValid () && (bet->getBetMode () == BetInterface::BET_MODE_TEST))
      // Only for the slot test page, get initiating bet from the bet object.
      {
         spin_bet_amount = bet->getBetAmount ();
      }
      else
      // Other wise get the initiating bet.
      {
         spin_bet_amount = record->initiatingBet ();
      }
      // Update the total bet information.  Post events concerning our current max bet situation.
      Amount total_bet_amount;
      totalBetDisplayAmount (total_bet_amount);
      //check to see if they are equal
      result = (getAmountValue (total_bet_amount) == getAmountValue (spin_bet_amount)) ? true : false;
   }

   #ifdef __VERBOSE_CONDITIONS__
   debug << "SlotPresentationWG::processConditionTotalBetEqualsSpinBet? " ;
   debug << (result ? "Yes" : "No") << endl;
   #endif

   return (result);
}

// Purpose: Return true if a stage triggered this award matches one passed
bool SlotPresentationWG::processConditionTrigger (const String &parameters)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_CONDITIONS__)
      verboseMethodName()
   #endif
   #if defined (__VERBOSE_CONDITIONS__)
      debug << "   " << parameters << endl;
   #endif

   bool found = false;
   // Example string is "Stage1,Stage2".
   // Separate the parameter string into a vector of parameters.
   vector<String> parameter_list;

   // Separate the stages out into separate strings
   uint32 number_of_params = parameters.numberOfMatches (',') + 1;
   int32 current_index, previous_index = 0;
   for (uint32 param_index = 0; param_index < number_of_params; ++param_index)
   {
      current_index = parameters.find (',', previous_index);
      String current_parameter;
      if (current_index > 0)
      {
         current_parameter = parameters.mid (previous_index, current_index - previous_index);
      }
      else
      {
         current_parameter = parameters.right (parameters.length () - previous_index);
      }
      parameter_list.push_back (current_parameter);
      previous_index = current_index + 1;
   }

   // If no parameters were specified...
   if (!parameter_list.size ())
   {
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition TRIGGER does not specify any stages."));
   }
   // Get current award information.
   Mojo<AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   // Iterate through the award list and find any triggers
   uint16 number_of_awards = local_award->getNumberOfAwards (evaluationTransactionId ());
   for (uint16 award_index = 0; !found && award_index < number_of_awards; ++award_index)
   {
      // Get the current system award interface
      Mojo<SystemAwardInterface> system_award;
      local_award->getAward (evaluationTransactionId (), award_index, system_award);

      // Iterate through each system award award
      uint16 number_of_system_awards = system_award->getNumberOfAwards ();
      for (uint16 system_award_index = 0; !found && system_award_index < number_of_system_awards; ++system_award_index)
      {
         // Look for any award with a trigger
         uint16 number_of_triggers = system_award->getNumberOfTriggers (system_award_index);
         for (uint16 trigger_index = 0; !found && trigger_index < number_of_triggers; ++trigger_index)
         {
            // Check to see if the trigger matches one of the passed parameters
            String trigger_string = system_award->getTriggerString (system_award_index, trigger_index);
            uint16 number_of_params = parameter_list.size ();
            for (uint16 parameter_index = 0; !found && parameter_index < number_of_params; ++parameter_index)
            {
               if (parameter_list[parameter_index] == trigger_string)
               {
                  found = true;
               }
            }
         }
      }
   }

   #ifdef __VERBOSE_CONDITIONS__
      debug << "SlotPresentationWG::processConditionTrigger(" << parameters << ')' << endl;
      debug << "  parameter_list = ";
      for (uint32 i = 0; i < parameter_list.size (); ++i)
      {
         debug << parameter_list [i] << ',';
      }
      debug << endl;
      debug << "  result = " << uint32 (found) << endl;
   #endif

   // Return result.
   return (found);
}

// Purpose: Handle multimedia events
bool SlotPresentationWG::processEvent (const MultimediaEvent &event)
{
   #if defined (__VERBOSE_MULTIMEDIA_EVENT__)
      verboseMethodName()
      debug << "   " << event.getString () << endl;
   #endif
   // the rest of the functionality for this multimedia event is handled in parent class.
   if (event == "LanguageSwap:SynchronizeControlPanels")
   {
      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg) && bet.isValid ())
      {
         // Get a mojo to the control panel interface
         Mojo<SlotControlPanelWGInterface> scpwg_interface = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
         // Turn physical bet buttons back on after a language swap.
         scpwg_interface->synchronizeBet (bet);

         // Turn Bet Per Sub Bet Slider back on after a language swap.
         synchronizeBetPerSubBetSlider (bet->getSubBetAmount (0));
      }
   }
   else if (event == WinDisplayWG::HIDE_EVENT_WG)
   {
      win_display.hide ();
   }
   else if (event == WinDisplayWG::SHOW_NEXT_EVENT_WG)
   {
      win_display.showNext ();
   }

   // Enable / Disable necessary game rules elements
   else if (event == "Enable:NFor1SeePaysElements")
   {
      // First thing, disable all previous N for 1 Elements
      postEventContext ("Disable:NFor1SeePaysElements", "Context.DynamicGameRules");
      // Post enable / disable events for see pays elements that are dependent on the subbets per credit
      ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
      uint32 sub_bets_pre_credit (config.getValue (config_id));
      String event_to_post = "Enable:";
      event_to_post << sub_bets_pre_credit;
      event_to_post << "For1SeePaysElements";
      postEventContext (event_to_post, "Context.DynamicGameRules");
   }
   else if (event == "Enable:BetSpecificSeePaysElements")
   {
      if (bet->getBetAmount () == ZERO_AMOUNT)
      {
         postEventAll ("Disable:BetScalingSeePaysElements");
         postEventAll ("Enable:StaticBetSeePaysElements");
      }
      else
      {
         Config config;
         bool scale_bet (false);
         // Save the location into the ER
         ConfigID id = config.getId ("GAME_RULES_SCALED_BY_BET", theme_configuration_identifier, false);
         // if the config item exists, extract the values
         if (id != INVALID_CONFIGID)
         {
            // the item exists, so get the value
            scale_bet = config.getValue (id);
         }

         if (scale_bet)
         {
            postEventAll ("Enable:BetScalingSeePaysElements");
            postEventAll ("Disable:StaticBetSeePaysElements");
         }
         else
         {
            postEventAll ("Disable:BetScalingSeePaysElements");
            postEventAll ("Enable:StaticBetSeePaysElements");
         }
      }
   }
   
   // Reinitialize game rules when changing denom
   else if (event == "DenominationMode:MultiDenom"       || 
            event == "DynamicGameRules:Loading:Complete" || 
            event == "GamePresentation:enterGameMode:Play")
   {
      Config config;
      // If this config item exists, then post the events necessary for synchronizing the game rules.
      ConfigID id (config.getId ("DYNAMIC_GAME_RULES", theme_configuration_identifier, false));
      if (id != INVALID_CONFIGID && config.getValue (id) == true)
      {
         // Send an event to set up the denomination
         uint32 denom_value = bet->getDenomination ().getValue ();

         // Create the script parameter
         ScriptTableParameter event_payload;
         ScriptParameter denom_script_param (denom_value, ParameterType::UINT32);
         event_payload.script_table ["denom"] = denom_script_param;
   
         // Send off the event with the payload
         Mojo<ActorStageInterface> actor_stage_interface;
         getStage (stage_name, actor_stage_interface);

         if (actor_stage_interface.isValid ())
         {
            actor_stage_interface->postEventContextExtended ("SetSeePaysDenom", "Context.DynamicGameRules", 0, &event_payload);
         }
      }
   }

   return (SlotPresentation::processEvent (event));
}

// Purpose: Handle sequence events
bool SlotPresentationWG::processEvent (const SequenceEvent &event)
{
   #if defined (__VERBOSE_SEQUENCE_EVENT__)
      verboseMethodName()
   #endif

   return (SlotPresentation::processEvent (event));
}

// The following method assigns the new control panel to the game-specific control panel mojo as is
// appropriate for the control panel type name received.
bool SlotPresentationWG::registerControlPanel (const char *type, const char *alias)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Initialize the return value.
   bool registered = false;
   if (type)
   {
      if(strcmp (type, SLOT_CONTROL_PANEL_WG) == 0 || strcmp (type, SLOT_CONTROL_PANEL_WG_LITE) == 0)
      {
         slot_control_panel_wg = alias;
         registered = true;
      }
   }

   // If the control panel was not registered by this game, allow the base class an opportunity to register it.
   if (!registered)
   {
      registered = SlotPresentation::registerControlPanel (type, alias);
   }

   return (registered);
}

/// <summary>get Game pays in Credits, use for settings strings.</summary>
int64 SlotPresentationWG::gamePaysInCredits ()
{

    Amount amount_in_award (ZERO_AMOUNT);
   uint64 credits_in_award = 0;
   extractWinForAllActs (amount_in_award);
   Amount::Extractor amount_extractor (amount_in_award);
   credits_in_award = amount_extractor.extract (getDenomination ());
   
   return credits_in_award;
}

/// <summary>Get the ER values and create a payload.</summary>
/// <param name="payload">[out] The ScriptTableParameter to store the data in.</param>
void SlotPresentationWG::getERData (ScriptTableParameter &payload)
{
   GamePresentation::getERData (payload);
   
   payload.script_table ["game_pays"]               = ScriptParameter (gamePaysInCredits(), ParameterType::INT64);
   payload.script_table ["games_awarded"]           = ScriptParameter (getNumberOfGamesAwarded(), ParameterType::INT32);
   payload.script_table ["spin_cap"]                = ScriptParameter (maxTotalPlays(), ParameterType::INT32);
   payload.script_table ["is_retrigger"]            = ScriptParameter (processConditionRetrigger(), ParameterType::BOOL);
   payload.script_table ["extra_games_awarded"]     = ScriptParameter (extraGamesAwarded(), ParameterType::UINT16);
   payload.script_table ["double_up_pending"]       = ScriptParameter (checkForDoubleUpEnable(), ParameterType::BOOL);
}

// Purpose: Rollup the total free games meter
//          The meter increments by one until
//          the theshold is met. Once the thershold is
//          met rollup the rest of the free games.
void SlotPresentationWG::rollupFreeGamesMeter (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Set the spins played property
   uint16 play_count = 0;
   uint16 total_games = 0;
   uint16 games_remaining = gamesRemaining ();
   int32 games_awarded = getNumberOfGamesAwarded ();
   playCountStatus (play_count, total_games);

   uint32 start_count (0);
   if (games_awarded > 0)
   {
      start_count = total_games - games_awarded;
   }

   // Rollup the free games count meter in the following conditions
   // games_awarded > 0: There were more free games awarded this act (retrigger), OR
   // This is the first game, there are games to be played, AND
   // There are games left to roll up.
   if ((games_awarded > 0 || (play_count == 1 && games_remaining > 0)) && (start_count + free_game_meter_rollup_count < total_games))
   {
      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         //Make sure that the actor meter is enabled
         Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();
         gcp_interface->enableMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, true);
         //check to see which rollup sound is going to play
         if (free_game_meter_rollup_count < free_game_meter_rollup_threshold)
         {
            free_game_meter_rollup_count++;
            postEvent ("FreeGamesMeterRollupSound:Play");
            postEvent ("FreeGamesMeterRollupSound:Increment");
            Amount games_awarded_amount (start_count + free_game_meter_rollup_count, Denomination::BASE_UNIT);
            gcp_interface->setMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, games_awarded_amount, RollupDefs::SNAP_TO_AMOUNT);
         }
         else if (free_game_meter_rollup_count >= free_game_meter_rollup_threshold)
         {
            free_game_meter_rollup_count = 0;
            Amount games_total_amount ((total_games), Denomination::BASE_UNIT);
            gcp_interface->setMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, games_total_amount, RollupDefs::ROLLUP_SLOW);
            postEvent ("FreeGamesMeterRollupSound:Terminator");
            postEvent ("FreeGamesMeterRollup:Done");
         }
      }
   }
   else
   {
      free_game_meter_rollup_count = 0;
      postEvent ("FreeGamesMeterRollup:Done");
   }
}

void SlotPresentationWG::rollupWin (RollupDefs::ROLLUP_MODE rollup_mode)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName()
   #endif

   Amount credit_meter_from (ZERO_AMOUNT);
   Amount credit_meter_to (ZERO_AMOUNT);
   Amount win_meter_from (ZERO_AMOUNT);
   Amount win_meter_to (ZERO_AMOUNT);
   Amount credits_in_bank (ZERO_AMOUNT);
   Amount credits_won_this_win (ZERO_AMOUNT);
   Amount credits_in_award (ZERO_AMOUNT);
   Amount credits_in_bank_and_in_award (ZERO_AMOUNT);

   // Enable the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get an interface to the control panel
      Mojo<GameControlPanelInterface> cp_interface = slot_control_panel_wg.getGameControlPanelInterface ();

      creditDisplayAmount (credits_in_bank);
      Mojo <AwardInterface> local_award;
      readAwardFromEssentialRecord (local_award);
      local_award->totalDisplayableAmount (credits_in_award);
      credits_in_bank_and_in_award = credits_in_award + credits_in_bank;
      // If we're not in the base game or is is a free act, include award list credits in the inital values
      if (isFreeSpins () || processConditionIsActType ("FREE"))
      {
         win_meter_to = credits_in_award;
         local_award->totalAmount (credits_won_this_win, evaluationTransactionId ());
         win_meter_from = win_meter_to - credits_won_this_win;
         credit_meter_from = (credits_in_bank_and_in_award) - credits_won_this_win;
      }
      // If we're in the base game, do not include award list credits in the initial values
      else
      {
         credit_meter_from = credits_in_bank;
         win_meter_to = credits_in_award;
      }

      // only get the new CREDIT meter value if we are supposed rollup now, not at the end of the stage
      if (!isRollupAfterStage ())
      {
         credit_meter_to = credits_in_bank_and_in_award;
      }
      else
      {
         credit_meter_to = credits_in_bank;
      }

      // Update the meter values
      cp_interface->setWinMeter(win_meter_from, RollupDefs::SNAP_TO_AMOUNT);
      cp_interface->setWinMeter(win_meter_to, rollup_mode);
      // If double is permitted and/or double up is enabled, dont add the win amount to credit meter so that player can double up
      // doubleUpEnable check is needed for PreBonus Rollup scenario.
      if(!doubleUpPermitted () && !doubleUpEnabled())
      {
         cp_interface->setCreditMeter(credit_meter_from, RollupDefs::SNAP_TO_AMOUNT);
         cp_interface->setCreditMeter(credit_meter_to, rollup_mode);
      }
      slot_control_panel_wg.getSlotControlPanelWGInterface ()->update (SlotControlPanelWG::UPDATE_SHOW_WIN_METER_ONLY);
   }
}


// Purpose:  Set the press button to start bonus message. Part of the new 2006 style.
void SlotPresentationWG::setAndShowPressButtonToStartBonusMessage (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getGameControlPanelInterface ()->setText (SlotControlPanelWG::TEXT_INFORMATION2, press_button_to_start_bonus_text);
      slot_control_panel_wg.getGameControlPanelInterface ()->showText (SlotControlPanelWG::TEXT_INFORMATION2, true);
   }
}

// Purpose: Format the current bet information string and update the respective property and ActorText
void SlotPresentationWG::setBetMessage (uint32 text_id, bool _append_good_luck)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif

   Mojo<PropertyInterface> property;
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;
   if (!resourceList (resource_list))
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::setBetMessage(): Could not get resource list.").errorMessage (), whoAreYou ());
      return;
   }

   // Only proceed if we have record and resource and the win amount is 0
   if (resource_list->getResource<PropertyInterface> (BET_MESSAGE_PROPERTY_NAME, resource_list->resourceKey (), property, false))
   {
      if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
      {
         setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::setBetMessagePropertyString(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
         return;
      }

      String bet_message_string (bet_message_format);
      for (uint32 parameter_index = 0; parameter_index < bet_message_parameters.size (); ++parameter_index)
      {
         // Stores the value associated with the string parameter
         String parameter_value ("");

         // The current token in the format string to be replaced. Ex: {0}, {1}, {2}...
         String current_token ("{");
         current_token << parameter_index << "}";
         //bet multiplier plus the word CREDIT or CREDITS attached (depending on the bet)
         if (bet_message_parameters [parameter_index] == "BET_MULTIPLIER")
         {
            parameter_value << getBetMultiplierString (true);
         }
         //bet multipler only (no CREDIT or CREDITS attached)
         else if (bet_message_parameters [parameter_index] == "BET_MULTIPLIER_NO_CREDITS")
         {
            parameter_value << getBetMultiplierString (false);
         }
         else if (bet_message_parameters [parameter_index] == "BONUS_BET")
         {
            parameter_value << getBonusBetString (true);
         }
         else if (bet_message_parameters [parameter_index] == "BONUS_BET_NO_CREDITS")
         {
            parameter_value << getBonusBetString (false);
         }
         else if (bet_message_parameters [parameter_index] == "NUMBER_OF_SELECTED_LINES")
         {
            parameter_value << numberOfSelectedPaylines () << (numberOfSelectedPaylines () == 1 ? " LINE" : " LINES");
         }
         else if (bet_message_parameters [parameter_index] == "NUMBER_OF_MULTI_WAYS_BET")
         {
            parameter_value << game_side_evaluator->getNumberOfMultiWaysBet (bet) << (game_side_evaluator->getNumberOfMultiWaysBet (bet) == 1 ? " WAY" : " WAYS");
         }
         else if (bet_message_parameters [parameter_index].contains ("GOOD_LUCK:"))
         {
            if (_append_good_luck)
            {
               // Strip the "GOOD_LUCK:" from the parameter and pass it in.
               // Note: If any additional {n} tokens are in this string, they
               // will need to be a greater index than this current parameter.
               parameter_value << bet_message_parameters [parameter_index].right (
                  bet_message_parameters [parameter_index].length () - 10);
            }
         }
         else if (bet_message_parameters [parameter_index].contains ("BONUS:"))
         {
            // If this is not a bonus bet type game, replace this token with a null string.
            if (bonusBetTypeGame () == true)
            {
               // If the bonus bet is not enabled, replace this token with a null string.
               Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");
               if (bonus_bet_amount > ZERO_AMOUNT)
               {
                  // Strip the "BONUS:" from the parameter and pass it in.
                  // Note: If any additional {n} tokens are in this string, they
                  // will need to be a greater index than this current parameter.
                  parameter_value << bet_message_parameters [parameter_index].right (
                     bet_message_parameters [parameter_index].length () - 6);
               }
            }
         }
         else
         {
            parameter_value = bet_message_parameters [parameter_index];
         }

         bet_message_string.replace (current_token, parameter_value);
      }

      #if defined (__VERBOSE_BET__)
      debug << BET_MESSAGE_PROPERTY_NAME << " Value: " << bet_message_string << endl;
      #endif

      // Update the property string
      property->setAbsoluteValue (bet_message_string);

      // If we have a valid world game slot control panel, update the actor directly
      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         slot_control_panel_wg.getGameControlPanelInterface ()->setText (text_id, bet_message_string);
         slot_control_panel_wg.getSlotControlPanelWGInterface ()->setPropertyValue (SlotControlPanelWG::PROPERTY_BET_MESSAGE, bet_message_string);
      }
   }
}

/// <summary>Sets the parameters of the default RollupRule.</summary>
/// <remarks>This will set the default rollup data to be used while calculating thresholds for the Rollup After
/// Stage feature. No other rules need to be specified, unless specific celebrations/rollup thresholds are to
/// be used for specific bet thresholds.</remarks>
/// <param name="rollup_data_start_index">[in] The first index of rollup_after_stage_data_vector to be checked.</param>
/// <param name="rollup_data_end_index">[in] The last index of rollup_after_stage_data_vector to be checked.</param>
/// <param name="celebration_bet_threshold">[in] The minimum bet needed from the player to consider the rules specified by the indices.</param>
void SlotPresentationWG::setDefaultRollupAfterStageRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index, uint32 celebration_bet_threshold)
{
   rollup_after_stage_rule_default.rollup_data_start_index = rollup_data_start_index;
   rollup_after_stage_rule_default.rollup_data_end_index = rollup_data_end_index;
   rollup_after_stage_rule_default.celebration_bet_threshold = celebration_bet_threshold;
}

/// <summary>Creates a Rollup Rule.</summary>
/// <remarks>This will set the rollup data to be used while calculating thresholds for the Rollup After Stage
/// feature. The rules set by this function will be analyzed before using the default rollup rule.</remarks>
/// <param name="rollup_data_start_index">[in] The first index of rollup_after_stage_data_vector to be checked.</param>
/// <param name="rollup_data_end_index">[in] The last index of rollup_after_stage_data_vector to be checked.</param>
/// <param name="celebration_bet_threshold">[in] The minimum bet needed from the player to consider the rules
/// specified by the indices.</param>
/// <param name="compare_target">[in] Is a string in scripts (converted by execScriptableFunction). Allows for
/// more rule qualification. Example: ROLLUP_COMPARE_MAX_BET_PER_LINE.</param>
/// <param name="rollup_criteria">[in] Is a string in scripts (converted by execScriptableFunction). Specifies how
/// to compare compare_target. Example: ROLLUP_CRITERIA_LESS_THAN.</param>
/// <param name="compare_value">[in] The value compared against compare_target.</param>
void SlotPresentationWG::setRollupAfterStageRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index,
                                                  uint32 celebration_bet_threshold, RollupCompareTarget compare_target,
                                                  RollupCriteria rollup_criteria, uint32 compare_value)
{
   // Sanity check
   if (rollup_data_end_index >= rollup_after_stage_data_vector.size ())
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setRollupAfterStageRule (), end index exceeds number of RollupModes defined"));
   if (rollup_data_start_index > rollup_data_end_index)
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setRollupAfterStageRule (), start index must be less than end index"));

   RollupRule rollup_rule;
   rollup_rule.rollup_data_start_index = rollup_data_start_index;
   rollup_rule.rollup_data_end_index = rollup_data_end_index;
   rollup_rule.celebration_bet_threshold = celebration_bet_threshold;
   rollup_rule.rollup_criteria = rollup_criteria;
   rollup_rule.compare_target = compare_target;
   rollup_rule.compare_value = compare_value;
   rollup_after_stage_rule_vector.push_back (rollup_rule);
}

/// <summary>Instantiate a "RollupData" struct and add it to the vector.</summary>
/// <remarks>This will set up rollup data parameters used to find rollup thresholds.</remarks>
/// <param name="win_to_bet_ratio_low_range">[in] The low boundary to check for win-bet ratio.</param>
/// <param name="win_to_bet_ratio_high_range">[in] The high boundary to check for win-bet ratio.</param>
/// <param name="low_range">[in] The low boundary to check for credits won.</param>
/// <param name="high_range">[in] The high boundary to check for credits won.</param>
/// <param name="celebration_loop_count">[in] The number of times to loop the celebration.</param>
/// <param name="event_threshold_index">[in] The index posted with the rollup threshold event if this data passes.</param>
void SlotPresentationWG::setRollupAfterStageData (float win_to_bet_ratio_low_range, float win_to_bet_ratio_high_range,
                                                  Amount::AmountValue low_range, Amount::AmountValue high_range, int32 celebration_loop_count,
                                                  uint32 event_threshold_index)
{
   RollupData rollup_data;
   rollup_data.win_to_bet_ratio_low_range = win_to_bet_ratio_low_range;
   rollup_data.win_to_bet_ratio_high_range = win_to_bet_ratio_high_range;
   rollup_data.low_range = low_range;
   rollup_data.high_range = high_range;
   rollup_data.celebration_loop_count = celebration_loop_count;
   // Event threshold index is 1 based. A value of 0 will mean that the index will be automatically assigned to be the current rollup data index.
   rollup_data.event_threshold_index = event_threshold_index ? event_threshold_index : rollup_after_stage_data_vector.size() + 1;
   rollup_after_stage_data_vector.push_back (rollup_data);
}

/// <summary>Set whether or not the meter panel is configured from the base stage</summary>
/// <param name="is_base">[in] True if the meter panel is configured from the base stage.
///    False if the meter panel is configured from a stage other than the base game (free spins or other bonus stages).</param>
/// <example><b>Example:</b><br>
/// @extExampleStart{SlotPresentationWG}
/// @extExample{isSharedMeterPanelConfigured}
/// @extExampleEnd
/// </example>
/// @scriptableFunction{SlotPresentationWG,setSharedMeterPanelBase}
void SlotPresentationWG::setSharedMeterPanelBase (bool is_base)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   shared_meter_panel_base = is_base;
}

// Purpose: Set the context that the presentation posts to for meter panel updates
void SlotPresentationWG::setMeterPanelContext (const char *_meter_panel_context)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   meter_panel_context = _meter_panel_context;
}

// Purpose: Show the bet message during history and utility states.
//  This uses the game over text so it can be backward compatible with all games.
void SlotPresentationWG::showBetMessageInHistoryAndUtilityStates (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      if (using_lua_bet_message)
      {
         ScriptTableParameter payload;
         payload.script_table ["append_good_luck"] = ScriptParameter (false, ParameterType::BOOL);
         postEventContextExtended (SlotControlPanelWG::EVENT_HISTORY_AND_UTILITY_BET_MESSAGE, "Context.MeterPanel", 0, &payload);
      }
      else
      {
         // Update the bet message property string, so we can show it in the game over message.
         setBetMessage (SlotControlPanelWG::TEXT_INFORMATION3, false);
         // Update the game over text display.
         slot_control_panel_wg.getGameControlPanelInterface ()->showText (SlotControlPanelWG::TEXT_INFORMATION3, true);
         // Display the bet message in history using the "Game Over" text actor
         postEvent (SlotControlPanelWG::EVENT_HISTORY_AND_UTILITY_BET_MESSAGE);
      }
      slot_control_panel_wg.getSlotControlPanelWGInterface ()->update (SlotControlPanelWG::UPDATE_HISTORY_AND_UTILITY_BET_MESSAGE);
   }
}

// Purpose: Show or hide the prompt that is displayed in the meter panel.
void SlotPresentationWG::showBetMessageMeterPanel (bool show, bool _append_good_luck)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      //make sure we have a valid bet
      show &= !numberOfSelectedSubBetsIsZero ();

      // Don't show the bet message if the game pays message is being displayed.
      Amount total_win (ZERO_AMOUNT);
      if (canDisplayAward ())
      {
         extractTotalWinFromAward (total_win);
      }
      show &= (total_win == ZERO_AMOUNT);

      // Do not update on a double up loss, we need to show "Double Up Complete"
      // Do not update when award is available, when a new game is started canDisplayAward ()
      // should return false and we should then update the bet Message.
      show &= (!checkDoubleUpLoss () || !canDisplayAward ());

      if (show)
      {
         // Hide the game pays message in case it is showing
         displayTotalWinMessage (false);
         // Hide the line pays message in case it is showing
         slot_control_panel_wg.getGameControlPanelInterface ()->showText (SlotControlPanelWG::TEXT_INFORMATION1, false);

         if (using_lua_bet_message)
         {
            ScriptTableParameter payload;
            payload.script_table ["append_good_luck"] = ScriptParameter (_append_good_luck, ParameterType::BOOL);
            postEventContextExtended ("UpdateBetMessage:MeterPanel", "Context.MeterPanel", 0, &payload);
         }
         else
         {
            // Update the bet message with the current information
            setBetMessage (SlotControlPanelWG::TEXT_INFORMATION4, _append_good_luck);
            // Show the bet message
            slot_control_panel_wg.getGameControlPanelInterface ()->showText (SlotControlPanelWG::TEXT_INFORMATION4, true);
            // let the presentation scripts know that this value is ready for use
            postEvent ("UpdateBetMessage:MeterPanel");
         }
      }
      else
      {
         // Ensure the bet message text is hidden
         slot_control_panel_wg.getGameControlPanelInterface ()->showText (SlotControlPanelWG::TEXT_INFORMATION4, false);
      }
   }
}

// Purpose: Called to configure the denomination of all known control panels.
void SlotPresentationWG::setControlPanelDenomination (const Denomination& denomination)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the grandparent. This function is overridden from SlotPresentation. Calling the
   // grandparent avoids duplicated and incorrect functionality.
   GamePresentation::setControlPanelDenomination (denomination);

   // Retrieve the game control panel and update the denomination if the control panel exists in this game.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getControlPanelInterface ()->setDenomination (denomination);
   }
}

// Purpose: Post events to position configurable on screen button(s) correctly for the current button configuration.
void SlotPresentationWG::setConfigurableOnScreenButtonsPosition (const Mojo <SlotControlPanelWGInterface> &scpwg_interface)
{
   if (scpwg_interface.isValid ())
   {
      if (doesBetSupportGameSideBetInterface ())
      {
         if (shouldShowTokenizedDenomButton ())
         {
            if (isAutoPlayEnabled ())
            {
               postEventContext (SlotControlPanelWG::EVENT_UPDATE_MIN_BET_DENOM_VALUE_WITH_AUTOPLAY, meter_panel_context);
            }
            else
            {
               postEventContext (update_min_bet_denomination_event, meter_panel_context);
            }
         }
         else
         {
            //post event to meter panel to shift international buttons correctly.
            if (isAutoPlayEnabled ())
            {
               postEventContext (SlotControlPanelWG::EVENT_HIDE_MIN_BET_DENOM_VALUE_WITH_AUTOPLAY, meter_panel_context);
            }
            else
            {
               postEventContext (hide_min_bet_denomination_event, meter_panel_context);
            }
         }
      }
      // Update the text for the denomination button.
      scpwg_interface->update (SlotControlPanelWG::UPDATE_DENOM_BUTTON);
   }
}

/// <summary>Inform the game about receiving central determination RNG seeds.</summary>
/// <returns>Returns true if the seed has arrived.</returns>
bool SlotPresentationWG::setGameEnrollmentSeedsArrived (void)
{
   // Make sure we have a valid world game slot control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();

      if (scpwg_interface.isValid () && okayToShowDrawMessages ())
      {
         scpwg_interface->update (SlotControlPanelWG::UPDATE_DRAW_RECIEVED);
      }
   }
   return SlotPresentation::setGameEnrollmentSeedsArrived ();
}

// Purpose: Update and post event to position logo banner correctly for the current button configuration.
void SlotPresentationWG::setLogoBannerPosition (void)
{
   // default
   bool tokenized_denom_btn_showing = false;
   bool auto_play_btn_showing = false;
   bool language_or_double_up_btn_showing = false;
   String event_to_post = "";
   event_to_post << SlotControlPanelWG::EVENT_SHIFT_LOGO_BANNER_FOR;

   // get current on screen button configuration
   tokenized_denom_btn_showing = shouldShowTokenizedDenomButton ();
   language_or_double_up_btn_showing = (doubleUpEnabled () || isMultiLingualAvailableForPlayer ());
   auto_play_btn_showing = isAutoPlayEnabled ();

   // build event string to shift logo banner for international buttons currently visible.
   if (auto_play_btn_showing && language_or_double_up_btn_showing)
   {
      // Shift logo banner for Langauge and AutoPlay button.
      event_to_post << SlotControlPanelWG::EVENT_LANGUAGE_AUTOPLAY_BANNER_POSITION;
   }
   else if (auto_play_btn_showing && !language_or_double_up_btn_showing)
   {
      // Shift logo banner for AutoPlay  button.
      event_to_post << SlotControlPanelWG::EVENT_AUTOPLAY_BANNER_POSITION;
   }
   else if (!auto_play_btn_showing && language_or_double_up_btn_showing)
   {
      // Shift logo banner for Language button.
      event_to_post << SlotControlPanelWG::EVENT_LANGUAGE_BANNER_POSITION;
   }
   else
   {
      // Default: just the Denom or Tokenized (Min. Bet) denom button.
   }


   // Add which denom button is showing for correct logo banner position.
   if (tokenized_denom_btn_showing)
   {
       // Shift logo banner for Tokenized (Min. Bet) denom button.
       event_to_post << SlotControlPanelWG::EVENT_TOKENIZED_DENOM_BANNER_POSITION;
   }
   else
   {
       // Shift logo banner for Denom button.
       event_to_post << SlotControlPanelWG::EVENT_DENOM_BANNER_POSITION;
   }

   // post event to set correct logo banner position for current on screen button configuration.
   postEvent (event_to_post);
}

// Purpose: Define the shared meter panel. If the meter panel is normally loaded from a different resource key,
//  check to make sure it's not already loaded before loading it.
void SlotPresentationWG::setSharedMeterPanel (const char *meterpanel_resource_list, const char *stage,
   const char * context, const char * meter_panel_key, const char *resource_key)
{
   String file (meterpanel_resource_list);
   Mojo<ActorStageInterface> stage_interface;
   // set the context
   setMeterPanelContext (context);

   // Load the meter panel if it is not configured or this is the base game stage and it is not
   // configured from the base game stage
   if (file.length () > 0 && !isSharedMeterPanelConfigured (stage, meter_panel_key, resource_key))
   {
      // Create a script parser for the meter panel resource list
      Mojo<ScriptResourceImporterInterface> script_parser;
      if (!createResource<ScriptResourceImporterInterface> (file, resource_key, script_parser, 0, false, false))
      {
         setErrorString (LogicError (LogicError::INVALID_PARAMETER, "setSharedMeterPanel: failed to create parser").errorMessage (), whoAreYou ());
         return;
      }

      // Parse the script
      Mojo<ScriptInterface> scriptable_object (resource_list);
      script_parser->parseFile (scriptable_object);
      // Get the meter panel stage (defined in the script)
      getResource (stage, resource_key, stage_interface);
      // Add the meter panel stage to the meter panel context
      if (!stage_interface->addToContext ())
      {
         String error_message ("SlotPresentationWG: Couldn't add stage ");
         error_message << stage << ", to context = " << stage_interface->getContext ();
         setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
         return;
      }

      // Set whether or not the meter panel was configured from the base stage
      if (strcmp (meter_panel_key, resource_key) == 0)
      {
         setSharedMeterPanelBase (true);
      }
      else
      {
         setSharedMeterPanelBase (false);
      }
   }
}

// Purpose:  Set the (game pays) win message.
void SlotPresentationWG::setTotalWinMessage (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Update the win display.  If the award can not be shown, set the win to zero (it will not display).
   Amount total_win (ZERO_AMOUNT);
   Amount mismatch_win_offset (ZERO_AMOUNT);
   if (canDisplayAward ())
   {
      extractTotalWinFromAward (total_win);
      extractMismatchWinAdjustmentFromAward (mismatch_win_offset);
   }

   setTotalWinMessage (total_win - mismatch_win_offset);
}

// Purpose: Maintain a version of setTotalWinMessage that has the same prototype as
//   the old function.
void SlotPresentationWG::setTotalWinMessage (const Amount &win_amount)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   String win_message;
   buildTotalWinMessage (win_amount, win_message);
   setTotalWinMessage (win_message);
}

// Purpose: Set the (game pays) win message.
void SlotPresentationWG::setTotalWinMessage (const String &win_message)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
      debug << "   win_message: " << win_message << endl;
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get a handle to the game control panel interface
      Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();
      // Set and show the win message text area
      if(!isBingo())
      {
         if (!using_lua_bet_message)
         {
            gcp_interface->setText (SlotControlPanelWG::TEXT_INFORMATION2, win_message);
            gcp_interface->setText (SlotControlPanelWG::TEXT_TOTAL_WIN_INFO, win_message);
         }
      }
      gcp_interface->showText (SlotControlPanelWG::TEXT_INFORMATION2, true);
      gcp_interface->showText (SlotControlPanelWG::TEXT_TOTAL_WIN_INFO, true);

      // Resource list from GamePresentation.
      Mojo<ResourceList, ScriptInterface> resource_list;
      // Do not update the bonus bet if the resource list is not valid yet.
      if (resourceList (resource_list))
      {
         // Extract the property associated with the multiplier from the resource list.
         Mojo<PropertyInterface> property;
         // In the getResource call, the last parameter is optional.  A true value will cause the system
         // to error if the resource can not be retrieved by name and resource key.  An error in this case
         // generally means that the resource has not been registered with the resource manager.  Refer to
         // steps 1 and 2 above.
         if (resource_list->getResource<PropertyInterface> (BASE_GAME_PAYS_STRING, resource_list->resourceKey (), property, false))
         {
            // Update the resource with the current SelectLines value.
            property->setAbsoluteValue (win_message);
         }
      }
      else
      {
         displayTotalWinMessage (false);
      }
   }
}

// Purpose: Update the total free spin win meter and post an event to show the appropriate summary screen
void SlotPresentationWG::setTotalWinMeterAndShowSummary (const char *_actor_name, const char *_stage_name, const char *_resource_key)
{
   Mojo<ActorStageInterface> actor_stage;
   if (!getResource (_stage_name, _resource_key, actor_stage, false))
   {
      String error_message ("SlotPresentationWG::setTotalWinMeterAndShowSummary, Stage has not been configured: alias: ");
      error_message << _stage_name << ", key: " << _resource_key;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }
   // Cast the actor_stage to an actor so we can search it
   Mojo <ActorInterface> stage (actor_stage);
   if (stage.isValid () == false)
   {
      String error_message ("SlotPresentationWG::setTotalWinMeterAndShowSummary, ActorInterface stage is not valid.");
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   // Whether or not an actor was updated
   bool actor_updated (false);
   // Declare an actor interface to find the initial actor...
   Mojo <ActorInterface> actor_interface;

   // Base string for showing the summary screen
   String summary_screen_event ("FeatureSummary:");

   // Get the total win amount from the award list
   Mojo <AwardInterface> local_award_list;
   readAwardFromEssentialRecord (local_award_list);
   Amount total_win_amount (ZERO_AMOUNT);
   local_award_list->totalDisplayableAmount (total_win_amount);

   // this test should always use the base game bet amount
   Amount bet_amount (ZERO_AMOUNT);
   if (gameStage ().stageIndex () == BASE_GAME_STAGE)
   {
      if (bet.isValid ())
      {
         bet_amount = bet->getBetAmount ();
      }
   }
   else
   {
      bet_amount = getSlotEssentialRecord ()->totalBetAtStageStart ();
   }

   // Only show the congratulations screen if the total win is greater than the total bet
   if (total_win_amount > bet_amount)
   {
      if (ActorHelper::findActor (stage, _actor_name, true, actor_interface))
      {
         // Assign the ActorTextInterface to the found actor and make sure it's valid
         Mojo <ActorTextInterface> actor_text_interface (actor_interface);
         // If the actor isn't text, then it could be meter.
         Mojo <ActorMeterInterface> actor_meter_interface (actor_interface);

         if (actor_text_interface.isValid ())
         {
            // Extract the base credits from the amount
            Amount::Extractor total_win_extractor (total_win_amount);
            int64 total_win_value = total_win_extractor.extract (denomination);
            // Load the credits value into a string
            String total_win_string ("");
            total_win_string << total_win_value;
            actor_text_interface->setString (total_win_string);

            // Flag that the actor was updated
            actor_updated = true;
         }
         else if (actor_meter_interface.isValid ())
         {
            // Make sure the meter has the correct denomination
            actor_meter_interface->setDenomination (denomination);
            // Set the meter amount
            actor_meter_interface->setAmount (total_win_amount);

            // Flag that the actor was updated
            actor_updated = true;
         }
         else
         {
            String error_message ("SlotPresentationWG::setTotalWinMeterAndShowSummary, ");
            error_message << _actor_name << " is not a valid ActorText or ActorMeter";
            setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
            return;
         }
      }
   }
   // Only show the credits won if the actor was actually updated.
   if (actor_updated)
   {
      summary_screen_event << "CreditsWon:Show";
   }
   else
   {
      summary_screen_event << "FeatureComplete:Show";
   }
   actor_stage->postEvent (summary_screen_event);
}

// Purpose: Set properties pertaining to retrigger amounts and limits, and post events to display
//    retrigger messages.
void SlotPresentationWG::setTriggerProperty (bool post_update_message)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   String retrigger_message ("Retrigger:");

   // Extract the property associated with the theme from the resource list.
   Mojo<PropertyInterface> property;

   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);

   int64 total_retriggers_awarded (slot_essential_record->totalPlays () - slot_essential_record->totalPlaysAtActStart ());
   #ifdef __VERBOSE_CHECK_FOR_RETRIGGERS__
   debug << "Total Plays: " << slot_essential_record->totalPlays () << endl;
   debug << "Total Plays at Act Start: " << slot_essential_record->totalPlaysAtActStart () << endl;
   debug << "Total Retriggers Awarded: " << total_retriggers_awarded << endl << endl;
   #endif

   #ifdef __VERBOSE_CHECK_FOR_RETRIGGERS__
   int32 cap_count_this_act (slot_essential_record->capCountThisAct ());
   int32 current_cap_count (slot_essential_record->capCount ());
   debug << "  - cap count this act       = " << cap_count_this_act << endl;
   debug << "  - current_cap_count        = " << current_cap_count << endl;
   debug << "  - total retriggers awarded = " << total_retriggers_awarded << endl;
   debug << "  - total plays              = " << slot_essential_record->totalPlays () << endl;
   // These two don't show capped until we're on the spin that caps it. capCountThisAct () shows how many were removed to
   // meet the cap.
   debug << "  - replay capped            ? " << (slot_essential_record->replayCapped () ? "y" : "n") << endl;
   debug << "  - replay capped this act   ? " << (slot_essential_record->replayCappedThisAct () ? "y" : "n") << endl;
   #endif

   // If we've had retriggers check to see which message to show the player
   // NOTE: totalRetriggers () returns true if the retriggers for this spin can/will be added to the total plays meter.
   // Otherwise,
   if (slot_essential_record->replayCappedThisAct () || slot_essential_record->replayCapped ())
   {
      #ifdef __VERBOSE_CHECK_FOR_RETRIGGERS__
      debug << "Retriggers Awarded:" << total_retriggers_awarded << endl;
      #endif
      // NOTE: Cap was hit and no additional spins were awarded.
      // TopBoxNoAdditionalSpins
      // FREE SPINS PLAYED LIMIT IS xx
      // NO ADDITIONAL SPINS AWARDED
      //
      // NOTE: The cap was reached and they were awarded 1 additional spin.
      // TopBoxOneAdditionalSpin
      // FREE SPINS PLAYED LIMIT IS xx
      // YOU'VE WON 1 ADDITIONAL SPIN!
      //
      // NOTE: This message below is used when the cap has been reached and we're showing how many spins were awarded after
      //       the cap.
      // TopBoxPlayedAdditionalSpins
      // FREE SPINS PLAYED LIMIT IS xx
      // YOU'VE WON yy ADDITIONAL SPINS!
      if (resource_list->getResource<PropertyInterface> (PROPERTY_MAX_SPINS_VALUE, resource_list->resourceKey (), property, false))
      {
         int32 maximum_retriggers (slot_essential_record->totalPlays ());
         // Update the property.
         property->setAbsoluteValue (maximum_retriggers);
      }
      if (resource_list->getResource<PropertyInterface> (PROPERTY_MAX_SPINS_STRING, resource_list->resourceKey (), property, false))
      {
         // Update MAX_SPINS_PROPERTY_NAME for backwards compatability
         String maximum_retriggers_string;
         maximum_retriggers_string << slot_essential_record->totalPlays ();
         property->setAbsoluteValue (maximum_retriggers_string);
      }

      switch (total_retriggers_awarded)
      {
         case 0:
         {
            // The cap is reached and no additional spins are awarded
            retrigger_message << "NoAdditionalSpins:";
         }
         break;

         case 1:
         {
            // The cap is reached and one additional spin is awarded
            retrigger_message << "OneAdditionalSpinCapped:";
         }
         break;

         default:
         {
            // The cap is reached and more than one additional spin is awarded
            retrigger_message << "AdditionalSpinsCapReached:";
         }
         break;
      }
   }
   else
   {
      // Retrigger Messages
      //
      // NOTE: Cap NOT hit, all spins awarded
      // TopBoxWonAdditionalSpin
      // YOU'VE JUST WON xx ADDITIONAL FREE SPINS
      //

      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      switch (total_retriggers_awarded)
      {
         case 1:
         {
            // The cap is not reached and one additional spin is awarded
            retrigger_message << "OneAdditionalSpin:";
         }
         break;

         default:
         {
            // The cap is not reached and more than one additional spin is awarded
            retrigger_message << "AdditionalSpins:";
         }
         break;
      }
   }
   // Update the trigger property name property for backwards compatability
   String free_spin_string;
   free_spin_string << total_retriggers_awarded;

   //Set the property for the TriggerName
   if (resource_list->getResource<PropertyInterface> (PROPERTY_TRIGGER_COUNT_STRING, resource_list->resourceKey (), property, false))
      property->setAbsoluteValue (free_spin_string);

   //Set the property for the TriggerValue
   if (resource_list->getResource<PropertyInterface> (PROPERTY_TRIGGER_COUNT_VALUE, resource_list->resourceKey (), property, false))
      property->setAbsoluteValue (total_retriggers_awarded);

   if (post_update_message)
   {
      // Post messages to update the number of spins won
      postEvent (retrigger_message + "SetMeter");
      postEvent (retrigger_message + "Show");
   }
   else
   {
      //Update the free spins message text and make it visible
      postEvent (retrigger_message + "Show:True");
   }

   //For backwards compatibility
   postEvent ("UpdateFreeSpinsInitiatedCount");
}

// Purpose: Set the win message prefix for various win types
bool SlotPresentationWG::setWinMessagePrefix (const String &win_type, const String &prefix)
{
   bool result = true;

   if (win_type == WIN_TYPE_GAME_PAYS)
   {
      game_pays_prefix = prefix;
   }
   else if (win_type == WIN_TYPE_TOTAL_GAME_PAYS)
   {
      total_game_pays_prefix = prefix;
   }
   else if (win_type == WIN_TYPE_BASE_GAME_PAYS)
   {
      base_game_pays_prefix = prefix;
   }
   else if (win_type == WIN_TYPE_DOUBLE_UP_PAYS)
   {
      double_up_pays_prefix = prefix;
   }
   else
   {
      result = false;
   }

   return (result);
}

// Purpose:  Called to enable all known control panels.
void SlotPresentationWG::showControlPanels (bool show)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the base class first.
   SlotPresentation::showControlPanels (show);
   // Show the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getControlPanelInterface ()->show (show);
   }
}

// Display the Game Over message in the far right of the information bar
// Purpose: Set every actor in the presentation to a known state because the context just entered or its mode changed.
void SlotPresentationWG::synchronize (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the grandparent. This function is overridden from SlotPresentation. Calling the
   // grandparent avoids duplicated and incorrect functionality.
   GamePresentation::synchronize ();

   // Update Game Theme license for the current system setup.
   updateGameThemeLicenseStatus ();

   // Get the bet amount for this payline and extract the displayable value with the current denomination.
   // Get bet amounts for each payline.
   int32 number_of_lines_selected = numberOfSelectedPaylines ();
   uint16 number_of_ways_selected = numberOfMultiWaysSelected ();

   if (limit_mech_paylines_to_button_values
       && (number_of_lines_selected > 0 || number_of_ways_selected > 0))
   {
      snapPaylinesToClosestConfig ();
   }
   if (limit_mech_bet_per_lines_to_button_values
       && (number_of_lines_selected > 0 || number_of_ways_selected > 0))
   {
      snapBetPerLineToClosestConfig ();
   }

   // Make sure that the reels are in a known display state.
   synchronizeReels ();


   // Set the new card request button to its default settings.
   synchronizeNewCardButton ();

   SlotControlPanelWGExtractor slot_control_panel_wg;
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (slot_control_panel_wg) && GamePresentation::controlPanel (game_control_panel))
   {
      // Get a mojo to the control panel interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface = slot_control_panel_wg.getSlotControlPanelWGInterface  ();

      // Set control panel start buttons to their default settings.
      synchronizeStartButtons (scpwg_interface, game_control_panel.getGameControlPanelInterface ());

      // Set the ranges of the sliders.
      initializeSliders ();
      // Set the values of the sub bet indicators
      initializeSubBetIndicators ();
      if (doesBetSupportGameSideBetInterface ())
      {
         // Hide the inactive intervals if necessary
         synchronizeHideInActiveSubBetIndicators ();
      }

      if (bet.isValid ())
      {
         // Synchronize the bet per sub bet slider position.
         synchronizeBetPerSubBetSlider (bet->getSubBetAmount (0));
         // Synchronize the sub bet indicators
         synchronizeSubBetIndicators ();
      }

      synchronizeOnScreenActors (slot_control_panel_wg.getControlPanelInterface ());
   }

   if (isFreeSpins ())
   {
      postEvent ("FreeSpinPresentation:Synchronize");
   }
   else
   {
      postEvent ("BaseGamePresentation:Synchronize");
   }
}

/// <summary>Maintain Autoplay button presentation syncronization for any changes in states except play state.</summary>
/// <param name="force_sync">[in] force a synchronize.</param>
void SlotPresentationWG::synchronizeAutoPlayButtonForNonPlayState (bool force_sync)
{
  // Do not call down to parent, this functionality is not needed for WG.
  // Overloaded to ignore auto play stop request by "service" button presses from the system.
}

// Purpose: Maintain Autoplay button presentation synchronization for any changes in states.
void SlotPresentationWG::synchronizeAutoplayButtonPresentation (bool force_sync)
{
   #ifdef __VERBOSE_AUTOPLAY__
   debug << "SlotPresentationWG::synchronizeAutoplayButtonPresentation" << endl;
   #endif

   if (player_autoplay_supported || force_sync)
   {
      // If presentation allows auto play button.
      if (allow_autoplay_button && player_autoplay_supported)
      {
         String autoplay_state_value;
         config.getValueString (autoplay_state_config_id, autoplay_state_value);
         bool can_play_auto_play = canPlayAutoPlay ();

         switch (getFlowState ())
         {
            case GameStateValue::STATE_IDLE:
               {
                  if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED)
                  {
                     updateAutoplayButtonPresentation (force_sync);
                  }
                  else if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_RUNNING)
                  {
                     if (can_play_auto_play)
                     {
                        enableAutoplayStopButton (can_play_auto_play);
                     }
                     else
                     {
                        enableAutoplayButton (can_play_auto_play);
                     }
                  }
               }
               break;

            case GameStateValue::STATE_PLAY:
               {
                  if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED)
                  {
                     enableAutoplayButton (false);
                  }
                  else if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_RUNNING)
                  {
                     //During play if auto play is running there is no reason to not allow the
                     //player to issue the stop
                     enableAutoplayStopButton (true);
                  }
               }
               break;
            case GameStateValue::STATE_AWARD:
               {
                  if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED)
                  {
                     enableAutoplayButton (false);
                  }
                  else if (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_RUNNING)
                  {
                     //If there is enough money already to cover the next bet show the stop button
                     //However if the current game is going to award enough money to cover the next
                     //bet, the stop button must also be shown otherwise the player will be forced to
                     //play the next game
                     bool awarded_amount_enough = false;
                     if(!can_play_auto_play && bet.isValid () && award_list.isValid ()
                        && award_list->getNumberOfAwards () > 0)
                     {
                        Amount total_bet_amount = bet->getBetAmount ();
                        Amount total_award_amount;
                        Amount credit_amount;
                        award_list->totalAmount (total_award_amount, current_paytable_section);
                        wagerCreditDisplayAmount (credit_amount);

                        Amount total_playable_amount = total_award_amount + credit_amount;

                        awarded_amount_enough = (total_bet_amount.isNotZero () && total_playable_amount.isNotZero ())
                           && (total_playable_amount >= total_bet_amount);
                     }

                     if (can_play_auto_play || awarded_amount_enough)
                     {
                        enableAutoplayStopButton (true);
                     }
                     else
                     {
                        enableAutoplayButton (false);
                     }
                  }
               }
               break;

            default:
               break;
         }
      }
      else
      {
         // Turn off auto play button if not in Base game stage.
         SlotControlPanelWGExtractor slot_control_panel_wg;
         if (controlPanel (slot_control_panel_wg))
         {
            Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
            enableAutoplayButton (false);
            gcp_interface->showButton (GameControlPanel::BUTTON_AUTOPLAY, false);
            gcp_interface->showButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, false);
         }
      }
   }
}

// Purpose: Synchronize the visual bet components
void SlotPresentationWG::synchronizeBet (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif

   // Call the grandparent because SlotPresentation uses the IGameControlPanel
   GamePresentation::synchronizeBet ();

   if (bet.isValid ())
   {
      // Synchronize the Easy Bet
      SlotPresentation::synchronizeEasyBet ();

      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         // Give the bet per line to the control panel.
         updateBetPerLineMeter (bet->getSubBetAmount (0));
         // Give the number of lines currently selected to the control panel
         updateSelectLinesMeter (numberOfSelectedPaylines ());

         // Grab a Mojo to the control panel interface
         Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface ();
         Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();
         // Do not update the buttons if the game is not operating in its idle state.
         if (canBet ())
         {
            synchronizeBetButtons ();
            // Enabled/Disable the start button according to the bet and available credits.
            switch (getFlowState ())
            {
               case GameStateValue::STATE_IDLE:
               {
                  // If need to enroll before game play...
                  if (enrollmentBeforeGameStart ())
                  {
                     gcp_interface->enableEnrollButton (bet->canStartGame ());
                  }
                  else if (slot_control_panel_wg.isValid ())
                  {
                     gcp_interface->enableButton (SlotControlPanelWG::BUTTON_SPIN, bet->canStartGame ());
                  }

                  // Enable/Disable the new card request button.
                  synchronizeNewCardButton ();

                  // Post whether the handle should be locked or unlocked.
                  postHandleMessage ();

                  // If this is the first game of the free games feature, don't show the bet message
                  // because it is transitioning
                  if (isFreeSpins () && processConditionFirstGame ())
                  {
                     showBetMessageMeterPanel (false, false);
                  }
                  else
                  {
                     showBetMessageMeterPanel (true, false);
                  }
                  synchronizeAutoplayButtonPresentation (true);
                  synchronizeGameLanguage ();
                  updateDoubleUpButton ();
               }
               break;

               case GameStateValue::STATE_UTILITY:
               {
                  if (slot_control_panel_wg.isValid ())
                  {
                     slot_control_panel_wg.getGameControlPanelInterface  ()->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, true);
                  }
               }
               break;
            }
         }
      }
      synchronizeBetPerSubBetSlider (bet->getSubBetAmount (0));
      if (shouldShowSpinBetMeter ())
      {
         synchronizeSpinBetMeter ();
      }

      // Get the current number of subbets
      uint32 number_of_paylines;
      number_of_paylines = numberOfSelectedPaylines ();

      // Get the maximum number of subbets
      uint32 number_of_sub_bets;
      numberOfSubBets (number_of_sub_bets);
      
      // If current payline count equals maximum
      if (number_of_paylines == number_of_sub_bets)
      {
         postEvent ("MaxPaylinesSelected:True");
      }
      else
      {
         postEvent ("MaxPaylinesSelected:False");
      }
   }
}

/// <summary> Synchronize the betting buttons. </summary>
void SlotPresentationWG::synchronizeBetButtons (void)
{
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Grab a mojo to the control panel interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface ();
      scpwg_interface->synchronizeBet (bet);
   }
}

/// <summary>Synchronize the visual bet components due to a bet configuration change, (loading the game,
/// exiting the operator menu, messaging from host central determination, etc).</summary>
void SlotPresentationWG::synchronizeBetConfig (void)
{
   // Let the base class handle this first
   SlotPresentation::synchronizeBetConfig ();
   // If the game uses bonus bet, initialize the bonus bet settings
   initializeBonusBet ();

   // Update the selected and displayed sub bet indicators
   initializeSubBetIndicators ();
   synchronizeSubBetIndicators ();

   // Update the dynamic button images
   if(current_game_mode != GAME_MODE_HISTORY)
   {
      postEventContext ("SynchronizeDynamicSlotBetButtons", meter_panel_context);
   }

   // Check to see if the paylines are limited to mechanical buttons
   ConfigID config_id = config.getId ("LIMIT MECH PAYLINES TO BUTTON VALUES", theme_configuration_identifier, false);
   limit_mech_paylines_to_button_values = config_id != INVALID_CONFIGID && config.getValue (config_id);

   // Check to see if the bet per sub bet is limited to mechanical buttons
   config_id = config.getId ("LIMIT MECH BET PER LINES TO BUTTON VALUES", theme_configuration_identifier, false);
   limit_mech_bet_per_lines_to_button_values = config_id != INVALID_CONFIGID && config.getValue (config_id);
}

/// <summary>Update the bet up/bet down buttons.</summary>
/// <param name="sub_bet_amount">[in] Position value to set slider to.</param>
void SlotPresentationWG::synchronizeBetPerSubBetSlider (const Amount &sub_bet_amount)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif
   #if defined (__VERBOSE_BET__)
      debug << "   sub_bet_amount: " << sub_bet_amount << endl;
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg) && doesBetSupportGameSideBetInterface ())
   {
      // Get a mojo to the world game slot control panel interface
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
      Amount::Extractor amount_extractor (sub_bet_amount);
      Amount::AmountValue new_value = amount_extractor.extract (denomination);

      //Check to see if the mechanical button configs are present
      //Assume if this config item exists, then the other do as well.  For any that do not exist config.getValue() will return 0.
      ConfigID mechanical_bet_per_line_button_1_id = config.getId ("MECHANICAL BET PER LINE BUTTON 1", theme_configuration_identifier, false);

      BetOptions bet_options;
      bet->getBetOptions (bet_options, false);

      if (bet_options.limit_bets_to_permitted_list)
      {
         vector<uint32> valid_bet_per_subbet_values;
         vector<int32> valid_bet_per_subbet_slider_values;
         vector<uint32>::iterator value_iter;

         Mojo <GameSideBetInterface> game_side_bet_interface (bet);
         // Get the new valid values from permitted list
         game_side_bet_interface->getValidBetPerSubbetValuesFromPermittedBetList (valid_bet_per_subbet_values);

         // Change the values as int32 from uint32
         value_iter = valid_bet_per_subbet_values.begin ();
         while (value_iter != valid_bet_per_subbet_values.end())
         {
            valid_bet_per_subbet_slider_values.insert (valid_bet_per_subbet_slider_values.end (), (int32)*value_iter);
            ++value_iter;
         }

         scpwg_interface->setSliderPosition(SlotControlPanelWG::SLIDER_BET_PER_SUB_BET,
            new_value,
            valid_bet_per_subbet_slider_values,
            true);
      }
      else if (mechanical_bet_per_line_button_1_id != INVALID_CONFIGID)
      {
         vector<int32> mech_bet_per_sub_bet_button_config;
         bool limit_mech_bet_per_sub_bet_to_button_values = false;

         mech_bet_per_sub_bet_button_config.push_back (config.getValue (mechanical_bet_per_line_button_1_id));
         mech_bet_per_sub_bet_button_config.push_back (config.getValue (config.getId ("MECHANICAL BET PER LINE BUTTON 2", theme_configuration_identifier, false)));
         mech_bet_per_sub_bet_button_config.push_back (config.getValue (config.getId ("MECHANICAL BET PER LINE BUTTON 3", theme_configuration_identifier, false)));
         mech_bet_per_sub_bet_button_config.push_back (config.getValue (config.getId ("MECHANICAL BET PER LINE BUTTON 4", theme_configuration_identifier, false)));
         mech_bet_per_sub_bet_button_config.push_back (config.getValue (config.getId ("MECHANICAL BET PER LINE BUTTON 5", theme_configuration_identifier, false)));
         limit_mech_bet_per_sub_bet_to_button_values = config.getValue (config.getId ("LIMIT MECH BET PER LINES TO BUTTON VALUES", theme_configuration_identifier, false));

         scpwg_interface->setSliderPosition(SlotControlPanelWG::SLIDER_BET_PER_SUB_BET,
            new_value,
            mech_bet_per_sub_bet_button_config,
            limit_mech_bet_per_sub_bet_to_button_values);
      }
      else
      {
         scpwg_interface->setSliderPosition(SlotControlPanelWG::SLIDER_BET_PER_SUB_BET, new_value);
      }

      if (canBet () && !isFreeSpins () && !isAutoPlayRunning () && !doubleUpPermitted ())
      {
         scpwg_interface->enableSlider (SlotControlPanelWG::SLIDER_BET_PER_SUB_BET, true);
      }
      else
      {
         scpwg_interface->enableSlider (SlotControlPanelWG::SLIDER_BET_PER_SUB_BET, false);
      }
   }
}

// Purpose:  Called to synchronize all known control panels.
void SlotPresentationWG::synchronizeControlPanels (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Call the grandparent. This function is overridden from SlotPresentation. Calling the
   // grandparent avoids duplicated and incorrect functionality.
   GamePresentation::synchronizeControlPanels ();
   // Synchronize the control panels known by this class.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getGameControlPanelInterface ();
      gcp_interface->synchronize ();

      // determine if denom texts should be shown
      synchronizeDenomination ();
   }
}

// Purpose: Syncrhonize the visual display of the player amount (player credits)
void SlotPresentationWG::synchronizePlayerAmount (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName()
   #endif

   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode ();
   if (GamePresentation::controlPanel (game_control_panel))
   {
      // Read the value of the player's credit meter
      Amount credit_amount;
      Amount total_win_amount_from_award (ZERO_AMOUNT);

      if (!in_tournament_mode)
      {
         if (includeAwardListCredits () && canDisplayAward ())
         {
            Mojo <AwardInterface> local_award_list;
            readAwardFromEssentialRecord (local_award_list);
            local_award_list->totalDisplayableAmount (total_win_amount_from_award);
         }
         // Read the value of the player's credit meter
         creditDisplayAmount (credit_amount);
         game_control_panel.getGameControlPanelInterface ()->showCreditMeter (true);
      }
      else
      {
         credit_amount = Amount (getTournamentCredits (), denomination);
         bool in_time_only_tournament_mode = false;
         String tournament_mode;
         if (getTournamentMode (tournament_mode))
         {
            in_time_only_tournament_mode = (tournament_mode == TOURNAMENT_MODE_TIME_ONLY);
         }

         if (in_time_only_tournament_mode)
         {
            game_control_panel.getGameControlPanelInterface ()->showCreditMeter (false);
         }
         else
         {
            game_control_panel.getGameControlPanelInterface ()->showCreditMeter (true);
         }
      }
      #if defined (__VERBOSE_METERS__)
         debug << "   credit_amount: " << credit_amount << endl;
         debug << "   total_win_amount_from_award: " << total_win_amount_from_award << endl;
         debug << "   credit_meter_value: " << credit_amount + total_win_amount_from_award << endl;
      #endif

      // Get the current essential record information.
      const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
      assertion (slot_essential_record != 0);

      // Give the credit meter to the control panel for display.
      // if DoubleUp, Allow player to enter double up, do not show the credit meter with the win amount
      if (!doubleUpPermitted () && !doubleUpWin () && !doubleUpEnabled () && !checkForRmlpAward ())
      {
         credit_amount += total_win_amount_from_award;
      }

      // Allow player to enter double up, do not show the credit meter with the win amount.
      game_control_panel.getGameControlPanelInterface ()->setCreditMeter (credit_amount);
   }
}

// Purpose: Synchronize the presentation's game language button
void SlotPresentationWG::synchronizeGameLanguage (bool force_sync)
{
   #ifdef __VERBOSE_LANGUAGE__
   debug << "SlotPresentationWG::synchronizeGameLanguage" << endl;
   #endif

   if (language_supported || force_sync)
   {
      // assume that multiple languages are not available for player.
      multiple_languages_available_for_player = false;

      String current_language = "";
      String from_language = "";

      // Ensure we have valid config ID's before continuing and presentation allows language button.
      if (languageConfigsSupported () && allow_language_button)
      {
         // Holds the enabled licensed languages bitfield
         BitField language_bitfield;

         // Get the size of the "LICENSED LANGUAGE TYPES SUPPORTED LIST".
         uint32 language_supported_list_size (config.getListSize (language_supported_list_config_id));

         // Get licensed languages bit field
         config.getValueBitField (language_licensed_bitfield_config_id, language_bitfield);

         // If only one, non-English language is licensed make sure it gets set.
         if (language_bitfield.numberOfBitsSet() == 1)
         {
            uint32 highest_bit_index = 0;
            // Get the current language.
            config.getValueString (game_language_config_id, current_language);

            // Get the licensed language
            language_bitfield.highestBitSet(highest_bit_index);
            config.getListItem (language_supported_list_config_id, highest_bit_index, from_language);

            if ((current_language != from_language) && (highest_bit_index > 0))
               config.setValue (game_language_config_id, from_language);
         }
         // Turn the language button on if more one language supported and licensed.
         else if ((language_supported_list_size > 1) && (language_bitfield.numberOfBitsSet () > 1))
         {
            // Verify that a language button exists in the interface before enabling it
            if (languageButtonIsValid ())
            {
               // Update language button accordingly to the amount of licensed games.
               enableLanguageButton (canEnableLanguage () && !checkForDoubleUpEnable () && !isAutoPlayRunning ());
               multiple_languages_available_for_player = true;
               language_supported = true;  // Sync flag here in case force_sync is passed to this routine
               uint32 language_bit_index = 0;
               config.getValueString (game_language_config_id, current_language);
               for (language_bit_index = 0; language_bit_index < language_supported_list_size; ++language_bit_index)
               {
                  if (language_bitfield.getBit (language_bit_index))
                  {
                     config.getListItem (language_supported_list_config_id, language_bit_index, from_language);
                     if (from_language != current_language)
                     {
                        // Set the language button movie to the correct section.
                        String event = "";
                        event << SlotControlPanelWG::EVENT_LANGUAGE_SWAP_DISPLAY_PRESS_FOR_PRE_FIX << from_language;
                        postEventContext (event, meter_panel_context);
                     }
                  }
               }
            }
            else
               System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::synchronizeGameLanguage (), language button not found in model."));
         }
      }

      if (current_language != "")
      {
         String other_event = "CurrentLanguage:";
         other_event << current_language;
         postEventAll(other_event);
      }

      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
         if (multiple_languages_available_for_player)
         {
            // Hide Language button if double up is enabled, show language button if not.
            if (checkForDoubleUpEnable ())
            {
               gcp_interface->showButton (GameControlPanel::BUTTON_LANGUAGE, false);
               postEventContext (disable_language_button_event, meter_panel_context);
            }
            else
            {
               gcp_interface->showButton (GameControlPanel::BUTTON_LANGUAGE, true);
               postEventContext ("LanguageSwap:Supported", meter_panel_context);
            }
         }
         else
         {
            enableLanguageButton (false);
            gcp_interface->showButton (GameControlPanel::BUTTON_LANGUAGE, false);
            language_supported = false;
            postEventContext ("LanguageSwap:NotSupported", meter_panel_context);
         }
      }
   }
}

// Purpose: Determine whether or not to hide the unused values on the sub bet indicators
//    when easy bet is enabled.
void SlotPresentationWG::synchronizeHideInActiveSubBetIndicators (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   // Get a mojo to the control panel interface.
   SlotControlPanelWGExtractor slot_control_panel_wg;

   if (doesBetSupportGameSideBetInterface () && controlPanel (slot_control_panel_wg))
   {
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface ();
      Mojo <GameSideBetInterface> game_side_bet_interface (bet);

      bool hide_inactive_sub_bet_indicators = false;
      if (game_side_bet_interface.isValid ())
      {
         if (game_side_bet_interface->getEasyBetConfiguration() != 0)
         {
            // By default, hide the inactive indicators when easy bet is enabled
            hide_inactive_sub_bet_indicators = true;

            // Get the vectors associated with the commital bets or BetNButtons
            vector<ButtonData> bet_n_list;
            game_side_bet_interface->getBetNButtonPanelData (bet_n_list);

            // Get the number of lines selected by the first element of the button panel as
            // a base to compare the other selected_lines values to. If they are all equal, then
            // only the top one needs to show (hide_inactive_sub_bet_indicators = true). Otherwise,
            // all of the possible values should show.
            uint32 selected_sub_bets (0);
            if (bet_n_list.size () > 0)
            {
               // Get the number of lines selected by the first button index
               selected_sub_bets = bet_n_list [0].sub_bets;
            }
            // Check the selected_lines from each button in the button map and compare it to the first
            // If any of them are not equal, then do not hide the inactive indexes
            for (uint32 button_index = 1; hide_inactive_sub_bet_indicators == true &&
                 button_index < bet_n_list.size (); ++button_index)
            {
               if (bet_n_list [button_index].sub_bets != selected_sub_bets)
               {
                  hide_inactive_sub_bet_indicators = false;
               }
            }
         }
      }
      // Show or hide the inactive sub bet indicators accordingly
      scpwg_interface->hideInActiveSubBetIndicators (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, hide_inactive_sub_bet_indicators);
      scpwg_interface->hideInActiveSubBetIndicators (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, hide_inactive_sub_bet_indicators);
   }
}

// Purpose: Synchronize the visual display of the win meter.
void SlotPresentationWG::synchronizeSpinBetMeter (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName()
   #endif

   // grab a copy of the essential record
   const SlotEssentialRecord* record = getSlotEssentialRecord ();

   // Get initiating bet.
   Amount spin_bet_amount;
   if (bet.isValid () && (bet->getBetMode () == BetInterface::BET_MODE_TEST))
   // Only for the slot test page, get initiating bet from the bet object.
   {
      spin_bet_amount = bet->getBetAmount ();
   }
   else
   // Other wise get the initiating bet.
   {
      spin_bet_amount = record->initiatingBet ();
   }
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      // set the value
      game_control_panel.getGameControlPanelInterface ()->setMeter (SlotControlPanelWG::METER_SPIN_BET, spin_bet_amount);
   }
}

// Purpose: Synchronize the visual display of the win meter.
void SlotPresentationWG::synchronizeWinAmount (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName()
   #endif

   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode ();
   if (GamePresentation::controlPanel (game_control_panel))
   {
      // Default to zero if we're not in tournament mode, or a bonus game, or the award can't be displayed yet
      Amount total_win_amount (ZERO_AMOUNT);

      if (in_tournament_mode)
      {
         total_win_amount = Amount (getTournamentWon (), denomination);
      }
      if (canDisplayAward ())
      {
         // Include the award list credits for the win meter if we're in idle as well
         Mojo <AwardInterface> local_award_list;
         readAwardFromEssentialRecord (local_award_list);
         local_award_list->totalDisplayableAmount (total_win_amount);
      }
      // Snap the value if in history state.
      if (getFlowState () == GameStateValue::STATE_HISTORY)
      {
         game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount, RollupDefs::SNAP_TO_AMOUNT);
      }
      else
      {
         // If double up win.
         if (doubleUpWin () && doubleup_rollup_needed)
         {
            // Set the win meter to zero, so the roll begins from zero in the base game.
            game_control_panel.getGameControlPanelInterface ()->setWinMeter (ZERO_AMOUNT, RollupDefs::SNAP_TO_AMOUNT);
         }
         else
         {
            if (rollupSlamButtonSet () && last_game_win_amount.isNotZero ())
            {
               // Show the amount won on the last game because it was slammed.
               game_control_panel.getGameControlPanelInterface ()->setWinMeter (last_game_win_amount, RollupDefs::SNAP_TO_AMOUNT);
            }
            else
            {
               // Give the meter to the control panel for display.
               game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount, RollupDefs::SNAP_TO_AMOUNT);
            }
         }
      }
   }
}

// Purpose:  This method synchronizes the flashing of the win and paid meter displays.
void SlotPresentationWG::synchronizeWinPaidMeterCycle (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
      verboseMethodName ()
   #endif

   SlotPresentation::synchronizeWinPaidMeterCycle ();

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<SlotControlPanelWGInterface> scpwg_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface  ();
      switch (getFlowState ())
      {
         case GameStateValue::STATE_HISTORY:
         case GameStateValue::STATE_UTILITY:
         {
            scpwg_interface->update (SlotControlPanelWG::UPDATE_SHOW_WIN_METER_ONLY);
         }
         break;

         default:
         {
            // The presentation of the meters depends on the value of the paid meter.  We will
            // show the paid meter if the paid meter value is non zero.  Otherwise, we'll show
            // the win meter. Extract the value for use in determining how to display the meters.
            Amount paid_amount;
            paidDisplayAmount (paid_amount);
            if (paid_amount == ZERO_AMOUNT)
               scpwg_interface->update (SlotControlPanelWG::UPDATE_SHOW_WIN_METER_ONLY);
            else
               scpwg_interface->update (SlotControlPanelWG::UPDATE_SHOW_WIN_AND_PAID_METERS);
         }
         break;
      }
   }
}

// Purpose: Determine whether or not the denomination button should be enabled, and enable/disable it
void SlotPresentationWG::synchronizeDenomButton (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getSlotControlPanelWGInterface ();

      uint32 active_denom_id = 0;
      uint32 inactive_denom_id = 0;
      // Always show the denomination button
      if (shouldShowTokenizedDenomButton ())
      {
         active_denom_id = SlotControlPanelWG::BUTTON_DENOMINATION_TOKENIZED;
         inactive_denom_id = SlotControlPanelWG::BUTTON_DENOMINATION;
      }
      else
      {
         inactive_denom_id = SlotControlPanelWG::BUTTON_DENOMINATION_TOKENIZED;
         active_denom_id = SlotControlPanelWG::BUTTON_DENOMINATION;
      }

      if (getFlowState () == GameStateValue::STATE_IDLE
          && isNormalPlay ()
          && processConditionPlayerSelectableDenomination ()
          && !isFreeSpins ())
      {
         gcp_interface->enableButton (active_denom_id, true);

         // As the BUTTON_DENOMINATION and BUTTON_DENOMINATION_TOKENIZED are binded to model file in WG,
         // here we post event to handle Denom Button enable/disable in S1ClassicPanel.
         postEvent ("EnableDenomButton");
      }
      else
      {
         gcp_interface->enableButton (active_denom_id, false);

         // As the BUTTON_DENOMINATION and BUTTON_DENOMINATION_TOKENIZED are binded to model file in WG,
         // here we post event to handle Denom Button enable/disable in S1ClassicPanel.
         postEvent ("DisableDenomButton");
      }

      // If the active denom and the inactive denom are the same button, don't disable or hide
      if (strcmp(gcp_interface->getButtonNameById (active_denom_id), gcp_interface->getButtonNameById (inactive_denom_id)) != 0)
      {
         gcp_interface->enableButton (inactive_denom_id, false);
         gcp_interface->showButton (inactive_denom_id, false);
      }

      gcp_interface->showButton (active_denom_id, true);
   }
}

// Purpose: Set the denomination from the SlotControlpanelWG
void SlotPresentationWG::synchronizeDenomination (void)
{
   #if defined (__VERBOSE_METHOD__)
   verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Retrieve the resource list.
      Mojo<ResourceList, ScriptInterface> resource_list;
      assertion (resourceList (resource_list));

      Mojo<PropertyInterface> system_denomination_property;
      Mojo<PropertyInterface> minimum_bet_property;
      String current_denomination_value ("");

      // Grab a mojo to the required property - the system denomination
      if (resource_list->getResource<PropertyInterface> (system_denomination_property_name, resource_list->resourceKey (), system_denomination_property, false))
      {
         // Get the current denomination from a property set by the system
         system_denomination_property->getAbsoluteValue (current_denomination_value);
         bool min_bet_found = false;

         // Make sure there is a valid bet, and the game is not in history mode
         Mojo <GameSideBetInterface> game_side_bet_interface (bet);
         if ((currentGameMode () != GAME_MODE_HISTORY) && game_side_bet_interface.isValid () )
         {
            uint32 minimum_bet = game_side_bet_interface->getMinBetButtonValue ();
            if (minimum_bet > 1)
            {
               // Convert the minimum bet to credits for formatting
               uint32 minimum_bet_money = minimum_bet * denomination.getValue ();
               String minimum_bet_money_string;
               // Used to format amount display strings.
               ULocale locale;
               Amount::AmountValue whole_value;

               // Format minimum bet
               CountryCode::Country country = locale.moneyNativeCountryCode ();
               locale.moneyGetUnitWholeValue (country, whole_value);
               if (minimum_bet_money >= whole_value)
               {
                  locale.moneyFormatNumericWhole (country, minimum_bet_money, minimum_bet_money_string, false /* omit base units if they're 0 */);
               }
               else
               {
                  locale.moneyFormatNumericBase (country, minimum_bet_money, minimum_bet_money_string);
               }

               // Convert the minimum bet value to a string
               String min_bet_string = "";
               min_bet_string << mininum_bet_denomination_text << minimum_bet_money_string;
               if (resource_list->getResource<PropertyInterface> (minimum_bet_property_name, resource_list->resourceKey (), minimum_bet_property, false))
               {
                  minimum_bet_property->setAbsoluteValue (min_bet_string);
               }
               #ifdef __VERBOSE_DENOMINATION_UPDATES__
               debug << "  - setting minimum bet value to: " << min_bet_string << endl;
               #endif

               min_bet_found = true;
               // Update all of the appropriate text values for the tokenized denom button
               slot_control_panel_wg.getGameControlPanelInterface ()->setText (SlotControlPanelWG::TEXT_TOKENIZED_DENOMINATION_1, current_denomination_value);
               slot_control_panel_wg.getGameControlPanelInterface ()->setText (SlotControlPanelWG::TEXT_TOKENIZED_DENOMINATION_2, "MIN. BET");
               slot_control_panel_wg.getGameControlPanelInterface ()->setText (SlotControlPanelWG::TEXT_TOKENIZED_DENOMINATION_3, minimum_bet_money_string);
            }
         }

         Mojo<RuleBasedGameSideBetInterface> rule_based_game_bet (bet);
         if ((currentGameMode () != GAME_MODE_HISTORY) && rule_based_game_bet.isValid ())
         {
            if (getEasyBetConfiguration () != EASY_BET_BOOLEAN_FALSE_STRING)
               min_bet_found = true;
         }

         if (min_bet_found)
            postEvent (update_min_bet_denomination_event);
         else
            postEvent (hide_min_bet_denomination_event);

         // Update the denom property value
         slot_control_panel_wg.getSlotControlPanelWGInterface ()->setPropertyValue (SlotControlPanelWG::PROPERTY_DENOM_TEXT, current_denomination_value);
         #ifdef __VERBOSE_DENOMINATION_UPDATES__
         debug << "  - setting game denomination text value to: " << current_denomination_value << endl;
         #endif
      }
   }
}

// Purpose: Synchronize the presentation
void SlotPresentationWG::synchronizePresentation (uint32 what_to_synchronize, uint32 bet_request_type, uint32 bet_request_data)
{
   #ifdef __VERBOSE_VOLUME__
      debug << "SlotPresentationWG::synchronizePresentation ()" << endl;
   #endif

   if(currentContextMode () != CONTEXT_MODE_INVALID)
   {
      SlotPresentation::synchronizePresentation (what_to_synchronize, bet_request_type, bet_request_data);

      if (UserSequenceEvent::INVALID_REQUEST_CATEGORY != bet_request_type)
      {
         // Make sure the sliders and bet indicators are updated
         synchronizeSubBetIndicators ();
         synchronizeBetPerSubBetSlider (bet->getSubBetAmount (0));
      }
   }
}

// Purpose: Update the volume property from the SlotControlPanelWG
void SlotPresentationWG::setVolumeProperty (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Mojo<ResourceList, ScriptInterface> resource_list;
      Mojo<PropertyInterface> property;
      assertion (resourceList (resource_list));

      // get the volume config
      ConfigID config_id = config.getId (CONFIG_ID_VOLUME_GAME_SOUNDS_RELATIVE);
      float current_relative_volume = config.getValueFloat (config_id);

      // Extract the property associated with the bet indicator from the resource list.  Update the value if
      // the property has been configured.  Do not error if the resource does not exist.
      if (resource_list->getResource<PropertyInterface> (PLAYER_VOLUME_LEVEL, resource_list->resourceKey (), property, false))
      {
         if (current_relative_volume < 0)
         {
            #ifdef __VERBOSE_VOLUME__
            debug << "relative volume " << VOLUME_LEVEL_LOW_STRING << " Level: " << current_relative_volume << endl;
            #endif
            String property_value (VOLUME_LEVEL_LOW_STRING);
            property->setAbsoluteValue (property_value);
            slot_control_panel_wg.getSlotControlPanelWGInterface ()->setPropertyValue (SlotControlPanelWG::PROPERTY_VOLUME_LEVEL, property_value);
         }
         else if (current_relative_volume > 0)
         {
            #ifdef __VERBOSE_VOLUME__
            debug << "relative volume " << VOLUME_LEVEL_HIGH_STRING << " Level: " << current_relative_volume << endl;
            #endif
            String property_value (VOLUME_LEVEL_HIGH_STRING);
            property->setAbsoluteValue (property_value);
            slot_control_panel_wg.getSlotControlPanelWGInterface ()->setPropertyValue (SlotControlPanelWG::PROPERTY_VOLUME_LEVEL, property_value);
         }
         else
         {
            #ifdef __VERBOSE_VOLUME__
            debug << "relative volume " << VOLUME_LEVEL_DEFAULT_STRING << " Level: " << VOLUME_LEVEL_DEFAULT << endl;
            #endif
            String property_value (VOLUME_LEVEL_DEFAULT_STRING);
            property->setAbsoluteValue (property_value);
            const char * test = VOLUME_LEVEL_DEFAULT_STRING;
            slot_control_panel_wg.getSlotControlPanelWGInterface ()->setPropertyValue (SlotControlPanelWG::PROPERTY_VOLUME_LEVEL, String (test));
         }
         postEvent (SlotControlPanelWG::EVENT_UPDATE_PLAYER_VOLUME_LEVEL_DISPLAY);
         slot_control_panel_wg.getSlotControlPanelWGInterface ()->update (SlotControlPanelWG::UPDATE_VOLUME_BUTTON_IMAGE);
      }
   }
}

// Purpose: Determine whether the tokenized denom button should be showing
bool SlotPresentationWG::shouldShowTokenizedDenomButton ()
{
   bool show_tokenized_button (false);
   // Get a game-side bet interface
   Mojo <GameSideBetInterface> game_side_bet_interface (bet);
   if (game_side_bet_interface.isValid ())
   {
      if ((game_side_bet_interface->getEasyBetConfiguration() != 0)
       && (game_side_bet_interface->getMinBetButtonValue () > 1)
       && (getFlowState () != GameStateValue::STATE_HISTORY))
      {
         show_tokenized_button = true;
      }
   }
   else
   {
      // Get a rule based game-side bet interface
      Mojo <RuleBasedGameSideBetInterface> rule_based_game_side_bet (bet);
      if (rule_based_game_side_bet.isValid ())
      {
         if ((getEasyBetConfiguration () != EASY_BET_BOOLEAN_FALSE_STRING)
          && (getMinBetButtonValue () > 1)
          && (getFlowState () != GameStateValue::STATE_HISTORY))
         {
             show_tokenized_button = true;
         }
      }
   }
   return show_tokenized_button;
}

// Purpose: Determine whether the spin count meters should be showing
bool SlotPresentationWG::shouldShowSpinCountMeters (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
      debug << "   gamesRemaining () > 0: " << (gamesRemaining () ? "true" : "false")  << ", Total Plays: " << totalPlays () << endl;
   #endif
   bool result = true;

   result &= isFreeSpins ();
   result &= (totalPlays () >= 1);
   result &= (currentContextMode () != CONTEXT_MODE_INVALID);
   return result;
}

// Purpose: Determine whether the more games button should be showing
bool SlotPresentationWG::shouldShowMoreGames (void)
{
   return (!isFreeSpins () && (numberOfAvailablePlayerGames () > 1));
}

// Purpose: Determine whether or not the spin bet meter should be showing
bool SlotPresentationWG::shouldShowSpinBetMeter (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   return ((isFreeSpins () && processConditionTotalBetEqualsSpinBet () == false) || getFlowState () == GameStateValue::STATE_UTILITY);
}

void SlotPresentationWG::incrementVolume (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotPresentation::incrementVolume ();
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      slot_control_panel_wg.getSlotControlPanelWGInterface ()->update (SlotControlPanelWG::UPDATE_VOLUME_BUTTON);
   }
}

// Purpose: synchronize buttons with bet amount
void SlotPresentationWG::synchronizeDynamicSlotBetButtons (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   if(update_dynamic_bet_buttons)
   {
      // Update the dynamic buttons if the buttom map is not empty
      if (!dynamic_slot_bet_button_map.empty ())
      {
         // Tell the control panel to update the button images
         SlotControlPanelWGExtractor slot_control_panel_wg;
         if (controlPanel (slot_control_panel_wg))
         {
            slot_control_panel_wg.getSlotControlPanelWGInterface  ()->synchronizeDynamicSlotBetButtons (bet, dynamic_slot_bet_button_map, dynamic_slot_bet_button_alias, dynamic_button_aux_data);
            postEvent ("UpdateDynamicSlotBetButtons");
         }
      }
   }
}

// Purpose: Synchronize the free spin actors
void SlotPresentationWG::synchronizeFreeSpins (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Get whether or not to show meters from the presentation
      bool show_spin_count_meters = shouldShowSpinCountMeters ();
      bool show_spin_bet_meter = shouldShowSpinBetMeter ();

      // Show or hide the spin count and spin bet meters
      slot_control_panel_wg.getGameControlPanelInterface ()->showMeter (SlotControlPanelWG::METER_SPINS_REMAINING, show_spin_count_meters);
      slot_control_panel_wg.getGameControlPanelInterface ()->showMeter (SlotControlPanelWG::METER_SPINS_PLAYED, show_spin_count_meters);
      slot_control_panel_wg.getGameControlPanelInterface ()->showMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, show_spin_count_meters);
      slot_control_panel_wg.getGameControlPanelInterface ()->showMeter (SlotControlPanelWG::METER_SPIN_BET, show_spin_bet_meter);

      // Post events to inform the non-meter actors of the show state
      postEventContext (show_spin_count_meters ? "ShowSpinCountMeters" : "HideSpinCountMeters", meter_panel_context);
      postEventContext (show_spin_bet_meter ? "ShowSpinBetMeter" : "HideSpinBetMeter", meter_panel_context);
   }
}

// Purpose: Synchronize all of the on screen actors in the interface to the appropriate state
void SlotPresentationWG::synchronizeOnScreenActors (const Mojo<GameControlPanelInterface> &gcp_interface)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   Mojo <SlotControlPanelWGInterface> scpwg_interface (gcp_interface);
   if (scpwg_interface.isValid ())
   {
      // Always show the game menu button if the control panel is available
      gcp_interface->showButton (SlotControlPanelWG::BUTTON_GAME_MENU, true);
      // synchronize the denom button to enable/show the appropriate one
      synchronizeDenomButton();

      // Always show the bet per sub bet slider
      scpwg_interface->showSlider (SlotControlPanelWG::SLIDER_BET_PER_SUB_BET, true);

      // If player selectable games, post events to notify the actors to move accordingly.
      if (shouldShowMoreGames ())
      {
         gcp_interface->showButton (SlotControlPanelWG::BUTTON_MORE_GAMES, true);
         postEventContext ("MoreThanOnePlayerSelectableGame:true", meter_panel_context);
      }
      else
      {
         gcp_interface->showButton (SlotControlPanelWG::BUTTON_MORE_GAMES, false);
         postEventContext ("MoreThanOnePlayerSelectableGame:false", meter_panel_context);
      }

      // Synchronize the volume button
      synchronizeVolumeButton (true);
      
      // Post events to position configurable on screen button(s) correctly for the current button configuration.
      setConfigurableOnScreenButtonsPosition (scpwg_interface);

      // set the correct logo banner position for the current button configuration
      if (doesBetSupportGameSideBetInterface ())
      {
         setLogoBannerPosition ();
      }

      // If we're in the prestart state, take care of misc. buttons and the message bar.
      if (getFlowState () == GameStateValue::STATE_PRESTART)
      {
         scpwg_interface->disableBetNCoinsButtons ();
         scpwg_interface->disableSelectSubBetButtons ();

         // Disable the game menu button
         gcp_interface->enableButton (SlotControlPanelWG::BUTTON_GAME_MENU, false);
         // Disable the more games button
         gcp_interface->enableButton (SlotControlPanelWG::BUTTON_MORE_GAMES, false);

         // Hide the game pays win display.
         displayTotalWinMessage (false);

         if (enrollmentRequired () && okayToShowDrawMessages ())
         {
            postEvent (SlotControlPanelWG::EVENT_DRAW_REQUESTED);
            scpwg_interface->update (SlotControlPanelWG::UPDATE_DRAW_REQUESTED);
         }
         else if (okayToShowAutoplayMessage ())
         {
            postEvent (SlotControlPanelWG::EVENT_AUTOPLAY);
            scpwg_interface->update (SlotControlPanelWG::UPDATE_AUTOPLAY);
         }
      }
   }
}

// It replaces synchronizeGameCountMeters in synchronizeMeters to prevent the flickering of the game counters
// after a powerhit
void SlotPresentationWG::synchronizeSpinCountMeters (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Retrieve the resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   assertion (resourceList (resource_list));

   // Set the spins played property
   int64 play_count = playCount (ACT_TYPE_BONUS);
   int64 games_remaining = gamesRemaining ();

   #ifdef __VERBOSE_METER__
   debug << "  Flow state " << getFlowState () << endl;
   int64 total_plays = totalPlays ();
   debug << "  games remaining: " << games_remaining << ", played: " << play_count << endl;
   debug << "  total plays " << total_plays << endl;
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      Amount play_count_amount (play_count, Denomination::BASE_UNIT);
      Amount games_remaining_amount (games_remaining, Denomination::BASE_UNIT);
      Amount games_awarded_amount (games_remaining + play_count, Denomination::BASE_UNIT);

      Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getGameControlPanelInterface ();
      gcp_interface->setMeter (SlotControlPanelWG::METER_SPINS_PLAYED, play_count_amount);
      gcp_interface->setMeter (SlotControlPanelWG::METER_SPINS_REMAINING, games_remaining_amount);
      gcp_interface->setMeter (SlotControlPanelWG::METER_TOTAL_SPINS_AWARDED, games_awarded_amount);
   }
}

// Purpose: Set the control panel buttons to their default settings
void SlotPresentationWG::synchronizeStartButtons (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (slot_control_panel_wg) && GamePresentation::controlPanel (game_control_panel))
   {
      synchronizeStartButtons (slot_control_panel_wg.getSlotControlPanelWGInterface  (), game_control_panel.getGameControlPanelInterface ());
   }
}

// Purpose: Set control panel buttons to their default settings.
void SlotPresentationWG::synchronizeStartButtons (const Mojo<SlotControlPanelWGInterface> &scpwg_interface , const Mojo<GameControlPanelInterface> &game_control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   if (!scpwg_interface .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }
   if (!game_control_panel.isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid GameControlPanelInterface"));
   }

   // Show/Enable buttons for various flow states.
   switch (getFlowState ())
   {
      case GameStateValue::STATE_IDLE:
      {
         synchronizeStartButtonsIdleState (scpwg_interface , game_control_panel);
      }
      break;

      case GameStateValue::STATE_ENROLL:
      {
         synchronizeStartButtonsEnrollState (scpwg_interface , game_control_panel);
      }
      break;

      case GameStateValue::STATE_PRESTART:
      {
         synchronizeStartButtonsPreStartState (scpwg_interface, game_control_panel);
      }
      break;

      case GameStateValue::STATE_START:
      {
         synchronizeStartButtonsStartState (scpwg_interface, game_control_panel);
      }
      break;

      case GameStateValue::STATE_PLAY:
      {
         synchronizeStartButtonsPlayState (scpwg_interface , game_control_panel);
      }
      break;

      // For all the "usual" states after the play state ...
      case GameStateValue::STATE_EVALUATE:
      case GameStateValue::STATE_EVALUATE_COMPLETE:
      case GameStateValue::STATE_AWARD:
      case GameStateValue::STATE_AWARD_COMPLETE:
      {
         // Regardless of a win or not, show the "spin" (repeat the bet) button
         // and hide the "stop" (stop reels) button.  Keep them both in a disabled
         // state.

         // Show the spin button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, true);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

         // Hide the stop button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, false);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);

         // Hide the evaluate button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

         // Get the game control panel.
         game_control_panel->showEnrollButton (false);
         game_control_panel->enableEnrollButton (false);
      }
      break;

      default:
      {
         // Hide disabled spin button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, true);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

         // Hide the stop spin button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, false);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);

         // Hide the evaluate button
         game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

         game_control_panel->showEnrollButton (false);
         game_control_panel->enableEnrollButton (false);
      }
      break;
   }
}

/// <summary>Make sure all of the start buttons are in a known state when in the Enroll state.</summary>
/// <param name="scpwg_interface">[in] Slot Control Panel WG Interface Pointer.</param>
/// <param name="game_control_panel">[in] Game Control Panel Interface Pointer.</param>
void SlotPresentationWG::synchronizeStartButtonsEnrollState (const Mojo<SlotControlPanelWGInterface> &scpwg_interface , const Mojo<GameControlPanelInterface> &game_control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Verify that the control panels are valid before using them
   if (!scpwg_interface .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }
   if (!game_control_panel.isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid GameControlPanelInterface"));
   }

   // If you need to enroll before game play...
   if (enrollmentBeforeGameStart ())
   {
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, true);
      if (!getEnrollmentButtonDoubleTap ())
      {
         game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, true);
         game_control_panel->flashButton (SlotControlPanelWG::BUTTON_SPIN, true);
      }

      // Do not show/enable the stop button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);

      // Do not show/enable the evaluate button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

      // Do not show/enable the enroll button
      game_control_panel->showEnrollButton (false);
      game_control_panel->enableEnrollButton (false);
   }
   else
   {
      // Do not show/enable the spin button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

      // Show, but do not enable the stop button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, true);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);

      // Do not show/enable the evaluate button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

      // Do not show/enable the enroll button
      game_control_panel->showEnrollButton (false);
      game_control_panel->enableEnrollButton (false);
   }
}

// Purpose: Make sure all of the start buttons are in a known state when in the Idle state
void SlotPresentationWG::synchronizeStartButtonsIdleState (const Mojo<SlotControlPanelWGInterface> &scpwg_interface , const Mojo<GameControlPanelInterface> &game_control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Make sure the control panels are valid
   if (!scpwg_interface .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }
   if (!game_control_panel.isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid GameControlPanelInterface"));
   }

   // Clear the flag because we're in idle. There's no reason to draw over the spin button with the stop button.
   setOkayToHideSpinButton (false);

   // Determine if there is a valid bet.
   bool valid_bet = false;
   if (bet.isValid () && canBet ())
   {
      valid_bet = bet->canStartGame ();
   }

   // If need to enroll before game play...
   if (enrollmentBeforeGameStart ())
   {
      // Do not show/enable the stop button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);

      // Do not show/enable the evaluate button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

      // Show enroll button and enable if bet is valid.
      game_control_panel->showEnrollButton (true);
      game_control_panel->enableEnrollButton (valid_bet);

      // Do not show/enable the spin button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);
   }
   // Else, no need to enroll before game play.
   else
   {
      // Do not show/enable the enroll button because enrollment is not required
      game_control_panel->showEnrollButton (false);
      game_control_panel->enableEnrollButton (false);

      // Show the spin button, and enable it if we have a valid bet
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, true);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, valid_bet);

      // Do not show/enable the stop button
      game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, false);
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);
   }
   synchronizeGameLanguage ();
}

// Purpose: Make sure all of the start buttons are in a known state when in the Play state
void SlotPresentationWG::synchronizeStartButtonsPlayState (const Mojo<SlotControlPanelWGInterface> &scpwg_interface, const Mojo<GameControlPanelInterface> &game_control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Make sure the control panels are valid
   if (!scpwg_interface .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }
   if (!game_control_panel.isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid GameControlPanelInterface"));
   }

   // Do not show/enable the enroll button
   game_control_panel->showEnrollButton (false);
   game_control_panel->enableEnrollButton (false);

   // Do not show/enable the evaluate button
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_EVALUATE, false);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_EVALUATE, false);

   // Do not show/enable the spin button
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, false);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

   // If we are allowed to enable the stop button...
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, true);
   // If the minimum reel spin time is greater than zero, the stop button must be disabled
   if (!enrollmentRequired () || enrollmentBeforeGameStart () || !getEnrollmentButtonDoubleTap ())
   {
      // Enable the stop button.
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, okayToSlamReels ());
   }
   else
   {
      // Disable the stop button.
      game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);
   }
}

// Purpose: Make sure all of the start buttons are in a known state when in the Pre-Start state
void SlotPresentationWG::synchronizeStartButtonsPreStartState (const Mojo<SlotControlPanelWGInterface> &scpwg_interface, const Mojo<GameControlPanelInterface> &game_control_panel)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Make sure the control panel is valid
   if (!game_control_panel .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }

   // Do not show/enable the spin button
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, false);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

   // Show, but do not enable the stop spin button
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, true);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);
}

// Purpose: Make sure all of the start buttons are in a known state when in the Start state
void SlotPresentationWG::synchronizeStartButtonsStartState (const Mojo<SlotControlPanelWGInterface> &scpwg_interface, const Mojo<GameControlPanelInterface> &game_control_panel )
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Make sure the control panel is valid
   if (!game_control_panel .isValid ())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::synchronizeStartButtons(): Invalid SlotControlPanelWGInterface "));
   }

   // Do not show/enable the spin button
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_SPIN, false);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_SPIN, false);

   // Show a disabled stop button.  We have to enable and then disable for it to appear
   // correctly.
   game_control_panel->showButton (SlotControlPanelWG::BUTTON_STOP, true);
   game_control_panel->enableButton (SlotControlPanelWG::BUTTON_STOP, false);
}

/// <summary>Synchronize the active sub bet indicator with the current bet level.</summary>
/// <remarks>If the number of sub bets in the current bet level is not found, no indicator will be active.</remarks>
void SlotPresentationWG::synchronizeSubBetIndicators (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_INDICATORS__)
      verboseMethodName()
   #endif

   // Get a mojo to the control panel interface.
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Grab a mojo to the interface this function uses
      Mojo<SlotControlPanelWGInterface> scpwg_interface = slot_control_panel_wg.getSlotControlPanelWGInterface ();

      Mojo<GameSideBetInterface> game_side_bet_interface (bet);
      Mojo<RuleBasedGameSideBetInterface> rule_based_game_bet (bet);
      if (game_side_bet_interface.isValid () || rule_based_game_bet.isValid ())
      {
         // True if the current bet maps to a displayed sub bet indicator
         bool key_found (false);
         // Initialize the bonus bet amount to zero
         Amount bonus_bet_amount (ZERO_AMOUNT);

         BetOptions bet_options;
         if (game_side_bet_interface.isValid ())
         {
            bet->getBetOptions (bet_options, false);
         }
         // If it's not a bonus bet type game, the bonus bet amount will remain 0
         if (bonusBetTypeGame () == true)
         {
            // Get the amount of the bonus bet
            bonus_bet_amount = bet->getBetAmountByName ("bonus");
         }

         // Convert the amount to a value
         Amount::Extractor bonus_amount_extractor (bonus_bet_amount);
         uint32 bonus_bet_value = static_cast<uint32>(bonus_amount_extractor.extract (denomination));
         // Get the number of selecected sub bets
         uint32 number_of_active_sub_bets = bet->numberOfActiveSubBets ();
         SubBetIndicatorMapIterator sub_bet_indicator_map_iterator;

         SubBetIndicatorMapSubKey map_sub_key (number_of_active_sub_bets, bonus_bet_value);
         SubBetIndicatorMapKey map_key (map_sub_key, getEasyBetConfiguration ());

         Config config;
         ConfigID test_subbets_id = config.getId (CONFIG_ITEM_AVAILABLE_TEST_SUBBETS, theme_configuration_identifier, false);
         // If in Test Mode, look for sub bet indicators that are specifically for test mode.
         if (bet->getBetMode () == BetInterface::BET_MODE_TEST && (test_subbets_id != INVALID_CONFIGID))
         {
            sub_bet_indicator_map_iterator = test_sub_bet_indicator_map.find (map_key);
            key_found = sub_bet_indicator_map_iterator != test_sub_bet_indicator_map.end ();
         }
         // If the permitted bet list is used, get index from dynamic indicator map.
         else if (game_side_bet_interface.isValid () && bet_options.limit_bets_to_permitted_list)
         {
            sub_bet_indicator_map_iterator = dynamic_sub_bet_indicator_map.find (map_key);
            key_found = sub_bet_indicator_map_iterator != dynamic_sub_bet_indicator_map.end ();
         }
         else
         {
            sub_bet_indicator_map_iterator = sub_bet_indicator_map.find (map_key);
            key_found = sub_bet_indicator_map_iterator != sub_bet_indicator_map.end ();
         }

         // If map key is not found
         if (!key_found)
         {
            // Try to find a default easy bet configuration for map key
            map_key.second = "";
            sub_bet_indicator_map_iterator = sub_bet_indicator_map.find (map_key);
            key_found = sub_bet_indicator_map_iterator != sub_bet_indicator_map.end ();
         }

         if (key_found)
         {
            scpwg_interface->setSubBetIndicatorActiveIndex (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, sub_bet_indicator_map_iterator->second);
            scpwg_interface->setSubBetIndicatorActiveIndex (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, sub_bet_indicator_map_iterator->second);
         }
         else
         {
            // If no mapping is found, set the indicators to the invalid index
            scpwg_interface->setSubBetIndicatorActiveIndex (SlotControlPanelWG::SUB_BET_INDICATOR_LEFT, 0);
            scpwg_interface->setSubBetIndicatorActiveIndex (SlotControlPanelWG::SUB_BET_INDICATOR_RIGHT, 0);
         }
      }
   }
}

// set the the current volume button state and resets the volume if it isn't shown
void SlotPresentationWG::synchronizeVolumeButton (bool enable)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   bool okay_to_enable_volume_button = okayToEnableVolumeButton ();
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      // show the button
      game_control_panel.getGameControlPanelInterface ()->showVolumeButton (true);

      if (okay_to_enable_volume_button)
      {
         // enable/disable the button
         game_control_panel.getGameControlPanelInterface ()->enableVolumeButton (enable);
      }
      else
      {
         // disable the button
         game_control_panel.getGameControlPanelInterface ()->enableVolumeButton (false);
      }
   }

   // The player menu will always have a volume button if it is ok to enable it.
   String event = "EnableVolumeButton:PlayerMenu:";
   if (okay_to_enable_volume_button)
   {
      event << (enable ? "true" : "false");

      // Update the property to the current volume level
      setVolumeProperty ();
   }
   else
   {
      event << "false";

      // reset the relative volume
      resetVolume ();

      // Dim the button in the player menue and on the regular interface
      postEvent ("DimVolumeButton:PlayerMenu");
      postEvent ("DimVolumeButton:SlotInterface");
   }

   postEvent (event);
}

void SlotPresentationWG::synchronizeNewCardButton (void)
{
  #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeNewCardButton" << endl;
  #endif

   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      //check to see if the player has credits
      Amount credit_amount (ZERO_AMOUNT);
      wagerCreditDisplayAmount (credit_amount);

      // OK to enable new card button in the idle state when the game has credits
      if (getFlowState () == GameStateValue::STATE_IDLE &&
         credit_amount != ZERO_AMOUNT)
      {
         slot_control_panel_wg.getGameControlPanelInterface ()->showButton (SlotControlPanelWG::BUTTON_NEW_CARD, true);
         slot_control_panel_wg.getGameControlPanelInterface ()->enableButton (SlotControlPanelWG::BUTTON_NEW_CARD, true);
      }
      else
      {
         slot_control_panel_wg.getGameControlPanelInterface ()->showButton (SlotControlPanelWG::BUTTON_NEW_CARD, false);
         slot_control_panel_wg.getGameControlPanelInterface ()->enableButton (SlotControlPanelWG::BUTTON_NEW_CARD, false);
      }
   }
}

// Purpose: Maintain Autoplay button presentation syncronization for any changes in states.
void SlotPresentationWG::updateAutoplayButtonPresentation (bool force_sync)
{
   SlotControlPanelWGExtractor slot_control_panel_wg;

   if ((force_sync || player_autoplay_supported) && controlPanel (slot_control_panel_wg))
   {
      Mojo<GameControlPanelInterface> gcp_interface = slot_control_panel_wg.getControlPanelInterface ();

      // if forcing sync, update autoplay support flag
      if ( force_sync )
      {
         String value_string = "";
         config.getValueString (who_can_start_autoplay_config_id, value_string);
         player_autoplay_supported = (value_string == CONFIG_ITEM_VALUE_AUTOPLAY_TYPE_PLAYER || value_string == CONFIG_ITEM_VALUE_AUTOPLAY_TYPE_PLAYER_2_TOUCH)
                                  && (allow_autoplay_button);
      }

      if ( player_autoplay_supported )
      {
         String autoplay_state_value;
         config.getValueString (autoplay_state_config_id, autoplay_state_value);

         // should we enable the button?
         bool enable_autoplay_button = canPlayAutoPlay ()
                                    && (currentGameMode () == GAME_MODE_PLAY)
                                    && (getFlowState() == GameStateValue::STATE_IDLE)
                                    && (autoplay_state_value == CONFIG_ITEM_VALUE_AUTOPLAY_STATE_DISABLED);

         // Player Enabled, show the autoplay button
         gcp_interface->showButton (GameControlPanel::BUTTON_AUTOPLAY, true);

         // disable the auto play button unless there is a valid condition.
         enableAutoplayButton (enable_autoplay_button);
         postEventContext ("AutoPlayButton:PlayerEnabled", meter_panel_context);
      }
      // Auto play is not player interactive or not available
      else
      {
        // "HOST" Enabled or "NONE" or not base stage, hide the autoplay button
         enableAutoplayButton (false);
         gcp_interface->showButton (GameControlPanel::BUTTON_AUTOPLAY, false);
         gcp_interface->showButton (SlotControlPanelWG::BUTTON_AUTOPLAY_STOP, false);
         player_autoplay_supported = false;
         postEventContext ("AutoPlayButton:None", meter_panel_context);
      }
   }
}

// Purpose: Toggle between Double Up button and Language button based on win.
void SlotPresentationWG::updateDoubleUpButton (void)
{
   GameControlPanelExtractor game_control_panel;
   bool control_panel_enabled (false);
   if (GamePresentation::controlPanel (game_control_panel))
      control_panel_enabled = game_control_panel.getControlPanelInterface ()->isEnabled ();
   if (control_panel_enabled)
   {
      SlotControlPanelWGExtractor slot_control_panel_wg;
      if (controlPanel (slot_control_panel_wg))
      {
         // Get a handle to the interface from the control panel extractor
         Mojo<GameControlPanelInterface> gcp_interface  = slot_control_panel_wg.getControlPanelInterface ();
         // if double up enable is allowed: show/enable double up button.
         bool double_up_allowed (false);
         if (doubleUpEnabled ())
         {
            double_up_allowed = checkForDoubleUpEnable ();
         }
         gcp_interface->enableButton (SlotControlPanelWG::BUTTON_DOUBLE_UP, double_up_allowed /* ok_to_enable */);
         gcp_interface->showButton (SlotControlPanelWG::BUTTON_DOUBLE_UP, double_up_allowed /* ok_to_show */);
      }
   }
}

// Purpose: Update the appropriate properties with the number of free games awarded
void SlotPresentationWG::updateGamesAwardedProperty (void)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   int32 number_of_games_value = getNumberOfGamesAwarded ();
   String number_of_games_string;
   number_of_games_string << number_of_games_value;

   Mojo<PropertyInterface> property;
   // Update the numeric value property if it exists
   if (resource_list->getResource<PropertyInterface> (PROPERTY_GAMES_AWARDED_VALUE, resource_list->resourceKey (), property, false))
   {
      property->setAbsoluteValue (number_of_games_value);
   }
   property.release ();

   // Update the string value of the property if it exists
   if (resource_list->getResource<PropertyInterface> (PROPERTY_GAMES_AWARDED_STRING, resource_list->resourceKey (), property, false))
   {
      property->setAbsoluteValue (number_of_games_string);
   }
   property.release ();

   postEvent ("GamesAwardedProperty:Updated");
}

//Purpose: Show the game over message, unless a replay is awarded
void SlotPresentationWG::synchronizeGameOver (void)
{
   if (SlotPresentation::currentAwardContainsNewRunnableTriggers ())
   {
      postEventContext (SlotControlPanelWG::EVENT_HIDE_GAME_OVER, meter_panel_context);
   }
   else
   {
      postEventContext (SlotControlPanelWG::EVENT_GAME_OVER, meter_panel_context);
   }
}

// Purpose:  Update the TotalBetMeter with the given value.
void SlotPresentationWG::updateTotalBetMeter (const Amount &value)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_BET__)
      verboseMethodName()
   #endif
   #if defined (__VERBOSE_BET__)
      debug << "   Amount: " << value << endl;
   #endif

   // Call the base class first
   SlotPresentation::updateTotalBetMeter (value);

   // Make sure we have a game control panel
   SlotControlPanelWGExtractor slot_control_panel_wg;
   if (controlPanel (slot_control_panel_wg))
   {
      // Update the total bet meter actor
      slot_control_panel_wg.getGameControlPanelInterface ()->setTotalBetMeter (value);
      slot_control_panel_wg.getGameControlPanelInterface ()->showTotalBetMeter (true);
   }
}

// Return the type of this scriptable object.
const char *SlotPresentationWG::whoAreYou (void) const
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   return (object_type);
}

/// <summary> Add a primary string value to the primary strings vector. </summary>
/// <param name="primary_string">String that will be added to the vector. </param>
/// @scriptableFunction{SlotPresentationWG,addPrimaryStringData}
void SlotPresentationWG::addPrimaryStringData (String primary_string)
{
   primary_string_list.push_back (primary_string);
}

/// <summary> Clear the primary strings vector. </summary>
/// @scriptableFunction{SlotPresentationWG,clearPrimaryStringData}
void SlotPresentationWG::clearPrimaryStringData (void)
{
   primary_string_list.clear ();
}

/// <summary> Add a secondary string value to the secondary strings vector. </summary>
/// <param name="secondary_string">String that will be added to the vector. </param>
/// @scriptableFunction{SlotPresentationWG,addSecondaryStringData}
void SlotPresentationWG::addSecondaryStringData (String secondary_string)
{
   secondary_string_list.push_back (secondary_string);
}

/// <summary> Clear the secondary strings vector. </summary>
/// @scriptableFunction{SlotPresentationWG,clearSecondaryStringData}
void SlotPresentationWG::clearSecondaryStringData (void)
{
   secondary_string_list.clear ();
}

// Purpose: Add a wild symbol to the vector of wilds in the evaluator.
void SlotPresentationWG::addWildSymbolForDynamicSeePays (String wild_symbol, String paytable_section)
{
   // Find the evaluator via the given paytable section
   uint32 current_evaluator_index (0);
   for (uint32 evaluator_index = 0; evaluator_index < dynamic_game_rules_evaluators.size (); ++ evaluator_index)
   {
      if (!strcmp (dynamic_game_rules_evaluators [evaluator_index].evaluator_interface->getPayTableSectionName (), paytable_section))
      {
         current_evaluator_index = evaluator_index;
         // Found what we need, get out of the loop
         evaluator_index = dynamic_game_rules_evaluators.size ();
      }
   }
   dynamic_game_rules_evaluators [current_evaluator_index].game_side_evaluator_interface->addWildSymbol (wild_symbol);
}

// Purpose: Update all see pays values with the current bet information.
void SlotPresentationWG::evaluateForSeePays (String paytable_section, bool use_base_game_bet)
{
   // Find the evaluator via the given paytable section
   uint32 current_evaluator_index (0);
   bool paytable_section_found = false;

   for (uint32 evaluator_index = 0; evaluator_index < dynamic_game_rules_evaluators.size (); ++ evaluator_index)
   {
      if (!strcmp (dynamic_game_rules_evaluators [evaluator_index].evaluator_interface->getPayTableSectionName (), paytable_section))
      {
         current_evaluator_index = evaluator_index;
         // Found what we need, get out of the loop
         paytable_section_found = true;
         evaluator_index = dynamic_game_rules_evaluators.size ();
      }
   }
   
   if (paytable_section_found == false)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::evaluateForSeePays: Attempting to evaluate with an evaluator that has not been created."));
   }

   uint32 total_bet_value (0);
   uint32 sub_bet_value (0);

   Config config;
   bool scale_bet (false);
   // Save the location into the ER
   ConfigID id = config.getId ("GAME_RULES_SCALED_BY_BET", theme_configuration_identifier, false);
   // if the config item exists, extract the values
   if (id != INVALID_CONFIGID)
   {
      // the item exists, so get the value
      scale_bet = config.getValue (id);
   }
   
   // If we are using the base game's bet, and we should scale the values by that bet,
   // then get the necessary bet values.
   if (use_base_game_bet && scale_bet)
   {
      /////////////////////////////////////
      //////////// TOTAL BET //////////////
      /////////////////////////////////////
      // Get the total bet
      Amount total_bet_amount = bet->getBetAmount ();
      total_bet_value = static_cast<uint32> (Amount::Extractor (total_bet_amount).extract (bet->getDenomination ()));

      /////////////////////////////////////
      //////////// SUB BET ////////////////
      /////////////////////////////////////
      // Sub bet
      Amount sub_bet_amount;
      sub_bet_amount = bet->getSubBetAmount (0);

      // Put the bet value into a usable form.
      sub_bet_value = static_cast<uint32> (Amount::Extractor (sub_bet_amount).extract (bet->getDenomination ()));
   }

   dynamic_game_rules_evaluators [current_evaluator_index].game_side_evaluator_interface->doEvaluationForSeePays (total_bet_value, sub_bet_value, scale_bet);
   createSeePaysValuesTable (paytable_section);
}

// Purpose: Create a table that holds the relevant See Pays data for use in the lua script
void SlotPresentationWG::createSeePaysValuesTable (String paytable_section)
{
   // --------------------------------------------------------
   // This function builds a nested table for use in Lua.  It
   // contains all the information needed to display the symbol
   // pay values in the dynamic game rules or pop up help.  The
   // table looks something like this:

   // master_table = 
   // {
   //    PaylineWins =
   //    {
   //       M1 = 
   //       {
   //          symbol_name = "M1",
   //          symbol_pay_5 = 
   //          {
   //             symbol_count = 5,
   //             pay_value = 500,
   //          }
   //          symbol_pay_4 = 
   //          {
   //             symbol_count = 4,
   //             pay_value = 200,
   //          }
   //          symbol_pay_3 = 
   //          {
   //             symbol_count = 3,
   //             pay_value = 75,
   //          }
   //       }
   //       M2 = 
   //       {
   //          symbol_name = "M2",
   //          .
   //          .
   //          .
   //       }
   //    }
   //    ScatterWins =
   //    {
   //       .
   //       .
   //       .
   //    }
   //    MultiwayWins =
   //    {
   //       .
   //       .
   //       .
   //    }
   // }
   // --------------------------------------------------------
   
   // Instantiate the local variables
   ScriptTableParameter master_table;
   String scratch_string;
   
   ScriptTableParameter payline_win_type_table;
   ScriptTableParameter scatter_win_type_table;
   ScriptTableParameter multiway_win_type_table;

   vector <ScriptTableParameter> payline_wins_symbols_tables;
   vector <ScriptTableParameter> scatter_wins_symbols_tables;
   vector <ScriptTableParameter> multiway_wins_symbols_tables;
   vector <ScriptTableParameter> *current_symbols_table = NULL;
   
   bool symbol_table_found (false);

   String symbol_name;
   int8 symbol_count;
   uint32 pay_value;
   int8 win_type;

   vector <ProcessedCategoryData> processed_category_data;
   ProcessedCategoryData temp_category_struct;

   // Find the evaluator via the given paytable section
   uint32 current_evaluator_index (0);
   for (uint32 evaluator_index = 0; evaluator_index < dynamic_game_rules_evaluators.size (); ++ evaluator_index)
   {
      if (!strcmp (dynamic_game_rules_evaluators [evaluator_index].evaluator_interface->getPayTableSectionName (), paytable_section))
      {
         current_evaluator_index = evaluator_index;
         // Found what we need, get out of the loop
         evaluator_index = dynamic_game_rules_evaluators.size ();
      }
   }
   WinCategoryId number_of_win_categories = dynamic_game_rules_evaluators [current_evaluator_index].evaluator_interface->getNumberOfWinCategories ();

   for (WinCategoryId category_index = 0; category_index < number_of_win_categories; ++category_index)
   {
      // Set up the variables needed in this loop
      ScriptTableParameter temp_symbol_table;
      ScriptTableParameter symbol_pay_table;
   
      // Set up the variables needed in this loop
      dynamic_game_rules_evaluators [current_evaluator_index].game_side_evaluator_interface->getPaytableValues (category_index, symbol_name, symbol_count, pay_value, win_type);
      
      ScriptParameter symbol_name_script_param (symbol_name, ParameterType::STRING);
      ScriptParameter symbol_count_script_param (symbol_count, ParameterType::INT8);
      ScriptParameter pay_value_script_param (pay_value, ParameterType::UINT32);
      ScriptParameter category_index_script_param (category_index, ParameterType::UINT32);
      
      // Fill the symbol_pay_table with data
      symbol_pay_table.script_table ["symbol_count"] = symbol_count_script_param;
      symbol_pay_table.script_table ["pay_value"] = pay_value_script_param;
         
      // Save off the index when we find the symbol table.
      uint32 symbol_table_index (0);
      symbol_table_found = false;

      switch (win_type)
      {
         case 0:
            break;

         case 1:
            current_symbols_table = &payline_wins_symbols_tables;
            break;

         case 2:
            current_symbols_table = &scatter_wins_symbols_tables;
            break;

         case 3:
            current_symbols_table = &multiway_wins_symbols_tables;
            break;
      }
      
      if (current_symbols_table != NULL)
      {
         // See if we have already created a table for this symbol name
         for (symbol_table_index = 0; symbol_table_index < current_symbols_table->size (); ++ symbol_table_index)
         {
            // Get ahold of the symbol name for this table
            scratch_string = current_symbols_table->operator[](symbol_table_index).script_table ["symbol_name"].asString ();
         
            // Compare this against the symbol name we got from the win category
            if (scratch_string == symbol_name)
            {
               symbol_table_found = true;
               // Found what we need, get out of this loop
               break;
            }
         }
      
         // Set up the string constants used to index into the tables
         scratch_string = "symbol_pay_";
         scratch_string << symbol_count;
         const char* symbol_pay_table_name = scratch_string;
      
         // If this symbol table does not exist in our vector, then add it
         if (!symbol_table_found)
         {
            // Add the symbol pay sub table
            temp_symbol_table.script_table [symbol_pay_table_name] = ScriptParameter (symbol_pay_table, ParameterType::TABLE);
            // Add the symbol name
            temp_symbol_table.script_table ["symbol_name"] = symbol_name_script_param;
         
            // Put this table in the vector
            current_symbols_table->push_back (temp_symbol_table);
         }
         else
         {
            bool category_processed (false);
            for (uint32 processed_category_index = 0; processed_category_index < processed_category_data.size (); ++ processed_category_index)
            {
               // See if we have already processed a similar win.
               // Check for symbol count, symbol name, and win type.
               // If we have already processed a win that matches these parameters, then ignore subsequent ones.
               if ((processed_category_data [processed_category_index].count == symbol_count)       && 
                   (!strcmp (processed_category_data [processed_category_index].name, symbol_name)) &&
                   (processed_category_data [processed_category_index].win_type == win_type))
               {
                  category_processed = true;
                  // Get out of the loop
                  processed_category_index = processed_category_data.size ();
               }
            }
         
            if (!category_processed)
            {
               current_symbols_table->operator[](symbol_table_index).script_table [symbol_pay_table_name] = ScriptParameter (symbol_pay_table, ParameterType::TABLE);
            }
         }

         // Add the values to the vector of items that have been processed.
         temp_category_struct.count = symbol_count;
         temp_category_struct.name = symbol_name;
         temp_category_struct.win_type = win_type;
         processed_category_data.push_back (temp_category_struct);
      }
   }

   // After we build the vector of tables, add them to the main table
   // PAYLINE WINS
   for (uint32 payline_table_index = 0; payline_table_index < payline_wins_symbols_tables.size (); ++ payline_table_index)
   {
      // Get ahold of the symbol name for this table
      scratch_string = payline_wins_symbols_tables [payline_table_index].script_table ["symbol_name"].asString ();
      const char* table_name = scratch_string;

      payline_win_type_table.script_table [table_name] = ScriptParameter (payline_wins_symbols_tables [payline_table_index], ParameterType::TABLE);
   }
   // Add to the master table
   if (payline_wins_symbols_tables.size () > 0)
   {
      master_table.script_table ["PaylineWins"] = ScriptParameter (payline_win_type_table, ParameterType::TABLE);
   }

   // SCATTER WINS
   for (uint32 scatter_table_index = 0; scatter_table_index < scatter_wins_symbols_tables.size (); ++ scatter_table_index)
   {
      // Get ahold of the symbol name for this table
      scratch_string = scatter_wins_symbols_tables [scatter_table_index].script_table ["symbol_name"].asString ();
      const char* table_name = scratch_string;

      scatter_win_type_table.script_table [table_name] = ScriptParameter (scatter_wins_symbols_tables [scatter_table_index], ParameterType::TABLE);
   }
   // Add to the master table
   if (scatter_wins_symbols_tables.size () > 0)
   {
      master_table.script_table ["ScatterWins"] = ScriptParameter (scatter_win_type_table, ParameterType::TABLE);
   }

   // MULTIWAY WINS
   for (uint32 multiway_table_index = 0; multiway_table_index < multiway_wins_symbols_tables.size (); ++ multiway_table_index)
   {
      // Get ahold of the symbol name for this table
      scratch_string = multiway_wins_symbols_tables [multiway_table_index].script_table ["symbol_name"].asString ();
      const char* table_name = scratch_string;

      multiway_win_type_table.script_table [table_name] = ScriptParameter (multiway_wins_symbols_tables [multiway_table_index], ParameterType::TABLE);
   }
   // Add to the master table
   if (multiway_wins_symbols_tables.size () > 0)
   {
      master_table.script_table ["MultiwayWins"] = ScriptParameter (multiway_win_type_table, ParameterType::TABLE);
   }
   
   // Once our table is built, send it off to the lua scripts
   Mojo<ActorStageInterface> actor_stage_interface;
   getStage (stage_name, actor_stage_interface);

   if (actor_stage_interface.isValid ())
   {
      scratch_string = "DynamicGameRulesTableBuilt:";
      scratch_string << paytable_section;

      // Post the event with payload
      actor_stage_interface->postEventContextExtended (scratch_string, "Context.DynamicGameRules", 0, &master_table);
      actor_stage_interface->postEventContextExtended (scratch_string, "Context.Main", 0, &master_table);
   }

}

// Purpose: Hook into the evaluator to initialize the see pays at load time.
void SlotPresentationWG::initializeSeePaysValues (void)
{
   // Find the evaluator via the given paytable section
   for (uint32 evaluator_index = 0; evaluator_index < dynamic_game_rules_evaluators.size (); ++ evaluator_index)
   {
      dynamic_game_rules_evaluators [evaluator_index].game_side_evaluator_interface->initializeSeePaysValues ();
   }

   // Send an event to set up the denomination
   uint32 denom_value = bet->getDenomination ().getValue ();

   // Create the script parameter
   ScriptTableParameter event_payload;
   ScriptParameter denom_script_param (denom_value, ParameterType::UINT32);
   event_payload.script_table ["denom"] = denom_script_param;

   // Send off the event with the payload
   Mojo<ActorStageInterface> actor_stage_interface;
   getStage (stage_name, actor_stage_interface);

   if (actor_stage_interface.isValid ())
   {
      actor_stage_interface->postEventContextExtended ("SetSeePaysDenom", "Context.DynamicGameRules", 0, &event_payload);
   }
}

// Create slot evaluators used for evauating in the game rules
void SlotPresentationWG::createGameRulesEvaluator (String paytable_section)
{
   Mojo <GameSideSlotEvaluatorInterface> temp_game_side_slot_evaluator_interface;
   DynamicGameRulesEvaluators temp_game_rules_evaluators;
   
   // Check to see if we need to get ahold of our base game evaluator
   if (!strcmp (paytable_section, evaluator->getPayTableSectionName ()))
   {
      if (gameSideSlotEvaluatorInterface (temp_game_side_slot_evaluator_interface) == false)
      {
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentationWG::createGameRulesEvaluator(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface."));
      }

      // Based on the passed in paytable section, we need our current evaluators.  Add them to the vectors
      temp_game_rules_evaluators.game_side_evaluator_interface = temp_game_side_slot_evaluator_interface;
      temp_game_rules_evaluators.evaluator_interface = evaluator;
   }
   // Not the base game, create necessary evaluators
   else
   {
      Mojo <SlotEvaluatorInterface> slot_evaluator;
      Mojo <EvaluatorInterface> evaluator_interface;
      
      paytable_parser->createEvaluator (evaluator_interface, paytable_section);

      slot_evaluator = evaluator_interface;
      assertion (slot_evaluator.isValid ());

      // From the slot evaluator we just set up, get a game side slot evaluator interface
      SlotEvaluatorInterfaces slot_evaluator_interfaces;
      if (slot_evaluator_interfaces.initialize (slot_evaluator))
      {
         temp_game_side_slot_evaluator_interface = slot_evaluator_interfaces.getGameSideSlotEvaluatorInterface ();
         temp_game_rules_evaluators.game_side_evaluator_interface = temp_game_side_slot_evaluator_interface;
         temp_game_rules_evaluators.evaluator_interface = evaluator_interface;
      }
   }
   dynamic_game_rules_evaluators.push_back (temp_game_rules_evaluators);
}

