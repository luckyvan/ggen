/* (c) Copyright 2007 International Game Technology */

/*
   $Id: SlotPresentation.cpp,v 1.0, 2013-08-23 18:28:05Z, Shaw, Jeffrey$
   $Date: 8/24/2013 2:28:05 AM$
   $Revision: 1$
   Based on revision 560

   Purpose:
      Defines the slot presentation

   Modifications:
      Author          Date         Explanation
      --------------- ----------   ------------
      Dwayne Nelson   Oct 20, 99   New
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
#ifndef __GAMECONTROLPANEL_HPP__
#include "GameControlPanel.hpp"
#endif
#ifndef __ULOCALE_HPP__
#include "ULocale.hpp"
#endif
#ifndef __MECHANICALREELSINTERFACE_HPP__
#include  "MechanicalReelsInterface.hpp"
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
#ifndef __RULE_BASED_GAME_SIDE_BET_INTERFACE_HPP__
#include "RuleBasedGameSideBetInterface.hpp"
#endif
#ifndef __SHAREDOBJECTPOOLRESOURCEINTERFACE_HPP__
#include "SharedObjectPoolResourceInterface.hpp"
#endif
#ifndef __SLOTAWARDINTERFACE_HPP__
#include "SlotAwardInterface.hpp"
#endif
#ifndef __SLOTPRESENTATION_HPP__
#include "SlotPresentation.hpp"
#endif
#ifndef __SLOTPRESENTATIONRESOURCEDEFS_HPP__
#include "SlotPresentationResourceDefs.hpp"
#endif
#ifndef __SLOTPRESENTATIONSTATES_HPP__
#include "SlotPresentationStates.hpp"
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
#ifndef __UTILITYSEQUENCEEVENTS_HPP__
#include "UtilitySequenceEvents.hpp"
#endif
#ifndef __USERSEQUENCEEVENT_HPP__
#include "UserSequenceEvent.hpp"
#endif
#ifndef __VIDEOREELSINTERFACE_HPP__
#include "VideoReelsInterface.hpp"
#endif
#ifndef __BINGOAWARDINTERFACE_HPP__
#include "BingoAwardInterface.hpp"
#endif
#ifndef __BBEPAYTABLEAWARDPROPERTYINTERFACE_HPP__
#include "BBEPaytableAwardPropertyInterface.hpp"
#endif

#include <algorithm>

#ifdef __DEBUG__
//#define __VERBOSE__
#ifdef __VERBOSE__
//   #define __CHANGING_REELS__
//   #define __VERBOSE_BET__
//   #define __VERBOSE_BONUS_WIN__
//   #define __VERBOSE_EVAL__
//   #define __VERBOSE_EVENTS__
//   #define __VERBOSE_METERS__
//   #define __VERBOSE_MODE__
//   #define __VERBOSE_OVERLAY__
//   #define __VERBOSE_PAYTABLE__
//   #define __VERBOSE_REELS__
//   #define __VERBOSE_WIN__
//   #define __VERBOSE_SERIALIZE__
//   #define __VERBOSE_AUTOPLAY__
//   #define __VERBOSE_DYNAMIC_BUTTONS__
#endif
#endif

// Define some debug/timing measurement support.
//#define __VERBOSE_GAME_SWITCH__
#ifdef __VERBOSE_GAME_SWITCH__
   #ifndef __STOPWATCH_HPP__
   #include "StopWatch.hpp"
   #endif

   class DebugTimer
   {
      public:
         void start (void) { last_time_micro_sec = 0; timer.start (); }
         void stop (void) { last_time_micro_sec = timer.elapsedMicroseconds (); timer.stop (); }
         StopWatch timer;
         uint64 last_time_micro_sec;

      friend ostream& operator << (ostream &o, DebugTimer &debug_timer)
      {
         uint64 now = debug_timer.timer.elapsedMicroseconds ();
         o << now << " - " << now - debug_timer.last_time_micro_sec << " ";
         debug_timer.last_time_micro_sec = now;
         return o;
      }
   };

   static void debugTime (DebugTimer &debug_timer, const char * message)
      { debug << debug_timer << "  " << message << endl; }
#else
   #define debugTime(x, y)
#endif

// Purpose:  release memories allocated for a two dimension array
#define releaseTwoDimensionArray(arrayPointer, count)    \
        {                                                \
           for (uint32 i = 0; i < (count); ++i)          \
           {                                             \
              delete [] ((arrayPointer) [i]);            \
           }                                             \
           delete [] (arrayPointer);                     \
        } 

// Type of object used in the scripts
const char *const SlotPresentation::object_type = "SlotPresentation";

// Config names //DGTAG
const char *const SlotPresentation::CONFIG_ID_DIGITAL_GLASS_PAYTABLE_ENABLED   = "STATIC PAYTABLE DIGITAL GLASS ENABLED";
const char *const SlotPresentation::CONFIG_ID_SUPPORT_WIN_UP_TO_SEEPAYS        = "SUPPORT WIN UP TO SEEPAYS";
const char *const SlotPresentation::CONFIG_ID_SUPPORT_WIN_UP_TO_ATTRACT        = "SUPPORT WIN UP TO ATTRACT";
const char *const SlotPresentation::CONFIG_ID_SUPPORT_WIN_UP_TO_BONUS_SEEPAYS  = "SUPPORT WIN UP TO BONUS SEEPAYS" ;
const char *const SlotPresentation::CONFIG_ID_WIN_UP_TO_BONUS_WIN_STRING       = "WIN UP TO BONUS WIN STRING";
const char *const SlotPresentation::CONFIG_ID_WIN_UP_TO_SECONDARY_STRING       = "WIN UP TO SECONDARY STRING";
const char *const SlotPresentation::CONFIG_ID_GAMERULES_VERSION                = "GAME RULES VERSION";        // Alternate SeePays Screen version
const char *const SlotPresentation::CONFIG_ID_AUTO_START_GAME_ON_ROLLUP_SLAM   = "AUTO START GAME ON ROLLUP SLAM";
const char *const SlotPresentation::CONFIG_ID_EASY_BET_MODE                    = "EASY BET ENABLED";
const char *const SlotPresentation::CONFIG_ID_NUMBER_OF_SUBBETS                = "NUMBER OF SUBBETS";

// Property names
const char *const SlotPresentation::WAYS_SELECTED_PROPERTY_NAME      = "WaysSelected";                // String
const char *const SlotPresentation::BET_MESSAGE_PROPERTY_NAME        = "BetMessage";                  // String
const char *const SlotPresentation::BASE_GAME_PAYS_STRING            = "BaseGamePaysString";          // String
const char *const SlotPresentation::BASE_GAME_PAYS_HISTORY           = "BaseGamePaysHistory";         // String
const char *const SlotPresentation::TOTAL_GAME_PAYS_HISTORY          = "TotalGamePaysHistory";        // String
const char *const SlotPresentation::BASE_GAME_WIN_HISTORY            = "BaseGameWinHistory";          // int64
const char *const SlotPresentation::TOTAL_GAME_WIN_HISTORY           = "TotalGameWinHistory";         // int64
const char *const SlotPresentation::CREDIT_METER_VALUE_BEFORE_BONUS  = "CreditMeterValueBeforeBonus"; // int64
const char *const SlotPresentation::LOWEST_PLAYER_DENOMINATION       = "LowestPlayerDenomination";    // uint32
const char *const SlotPresentation::WIN_UP_TO_DENOMINATION           = "WinUpToDenomination";         // String
const char *const SlotPresentation::WIN_UP_TO_MAX_BET_METER          = "WinUpToMaxBetMeter";          // int64
const char *const SlotPresentation::WIN_UP_TO_MAX_WIN_METER          = "WinUpToMaxWinMeter";          // int64
const char *const SlotPresentation::MAXIMUM_WIN_UP_TO_MAX_WIN_METER  = "MaximumWinUpToMaxWinMeter";   // int64
const char *const SlotPresentation::BONUS_X_WIN_UP_TO_STRING         = "Bonus%luWinUpToString";       // String
const char *const SlotPresentation::METER_PANEL_COLOR_RED_VALUE      = "MeterPanelColor:RedValue";    // float
const char *const SlotPresentation::METER_PANEL_COLOR_GREEN_VALUE    = "MeterPanelColor:GreenValue";  // float
const char *const SlotPresentation::METER_PANEL_COLOR_BLUE_VALUE     = "MeterPanelColor:BlueValue";   // float
const char *const SlotPresentation::PROPERTY_SPECTRUM_DISPLAY_VALUESTRING_ROOT = "Spectrum:DisplayAmount:ValueString:Level:";
const char *const SlotPresentation::PROPERTY_SPECTRUM_TEXT_DISPLAY_VALUE_ROOT = "Spectrum:DisplayText:String:Level:";

const char *const SlotPresentation::CONDITION_UNINITIALIZED          = "UnInitialized";               // String
const char *const SlotPresentation::CONDITION_LAST_NUDGE_GAME = "LAST_NUDGE_GAME";
const char *const SlotPresentation::CONDITION_NOT_LAST_NUDGE_GAME = "NOT_LAST_NUDGE_GAME";
const char *const SlotPresentation::CONDITION_NUDGE_TRIGGERED = "NUDGE_TRIGGERED";
const char *const SlotPresentation::CONDITION_NOT_NUDGE_TRIGGERED = "NOT_NUDGE_TRIGGERED";
// CELEBRATION CONFIGS
const char *const SlotPresentation::CELEBRATION_MODE_2006_STANDARDS = "CELEBRATION_MODE_2006_STANDARDS";
const char *const SlotPresentation::CELEBRATION_MODE_2007_STANDARDS = "CELEBRATION_MODE_2007_STANDARDS";
const char *const SlotPresentation::CELEBRATION_MODE_CUSTOM         = "CELEBRATION_MODE_CUSTOM";

// virtual spectrum properties
const char *const SlotPresentation::PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT = "Spectrum:DisplayAmount:Level:";
const char *const SlotPresentation::PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT = "Spectrum:DisplayText:Level:";

const char *const SlotPresentation::COMBINE_BASE_AMOUNT_TRUNK = "BaseAmountLevel";

// Meter panel color properties
const uint8 SlotPresentation::NUMBER_OF_COLOR_VALUES = 3;
const float SlotPresentation::MAX_COLOR_VALUE = 255.00;

// Button names
const char *const SlotPresentation::BET_N_COINS_BUTTON      = "BetNCoinsButton";
const char *const SlotPresentation::STOP_BUTTON             = "StopButton";
const char *const SlotPresentation::MAX_BET_BUTTON          = "MaxBetButton";

// Create an instance of the SequenceEventParser required by the generic slot game presentations.  Since sequence event
// handling can be completely scripted, it is not guaranteed that the parser symbols will be included in the shared
// objects that link the libraries.  Thus, the parser declaration is required here to ensure that the symbols
// included during the linking process.
static const SlotSequenceEventParser slot_sequence_event_parser;

// Purpose:  Initialize the object for use.
void SlotPresentation::postConstructor (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::postConstructor" << endl;
   #endif
   // Call the base classes first.
   GamePresentation::postConstructor ();

   // Initialize the class members.
   enrollment_started = false;
   okay_to_hide_spin_button = false;
   draw_paylines_individually = false;
   previous_number_of_lines_selected = 0;
   enrollment_double_tap_button_id = ENROLL_DBL_TAP_INVALID_BUTTON;
   bonus_bet_type_game = false;
   bet_per_line_max_value = 0;
   max_bonus_bet_multiplier = 0;
   number_bonus_bet_levels = 0;
   maxbet_bonus_enable = false;
   bonus_bet_message = false;
   transition_overlays_enabled = false;
   transition_overlays_resource_key = CONDITION_UNINITIALIZED;
   show_arrows_in_history = false;
   update_dynamic_bet_buttons = true;
   single_credit_rollup_threshold = 0;
   rollup_rule_default.rollup_data_end_index = _UINT32_MAX;
   current_glass_name = 0;
   powerhit_spin_reels = false;
   delay_standalone_progressive_meter_reset = false;
   okay_to_update_bonus_string = false;
   last_game_win_amount = ZERO_AMOUNT;
   enable_mechanical_slam = false;
   resetRollupSlamButton ();

   // Clear the button map
   dynamic_slot_bet_button_map.clear ();
   dynamic_slot_bet_button_alias = "";
   dynamic_button_aux_data.dynamic_button_blank_images.clear ();

   // Initialize the celebration to 2006 standards
   initializeCelebrationMode (CELEBRATION_MODE_2006_STANDARDS);

   // By default, held reels do not spin.
   held_reel_spin_config = DO_NOT_SPIN_REELS_IF_REELS_ARE_HELD;

   // set default win message prefix.
   win_message_prefix = "Game Pays ";
   // set default press button to start the bonus message. This is used for the New 2006 Style Bonusing.
   press_button_to_start_bonus_text = "Press button to start the bonus!";

   // At the base slot presentation level, this variable will not change.  If a change is required, the update
   // must be performed in a derived slot presentation class.
   current_reel_set_index = SINGLE_REEL_SET_INDEX;

   // Use this flag to snap win meter.
   setDoubleUpRollupNeededFlag (false);

   // By default, hide the volume button with more games enabled.
   show_volume_button_with_more_games_enabled = false;
   
   // Create a new spectrum update manager with the default delay.  To change the delay in a derived class, call 
   // spectrum_update_manager.init again with a GamePresentation pointer and the desired delay in milliseconds.
   spectrum_update_manager.init (this);

   // Set the script functions.
   static const ScriptFunctionMap script_functions [] =
   {
      // Parameters:  Script Function ID, Arguments, Script function name.
      { SNAP_REELS_TO_STOP_POSITION,             0, "snapReelsToStopPosition"                                              },
      { SPIN_HELD_REELS,                         1, "spinHeldReels"                                                        },
      { HIDE_ALL_PAYLINES_DISPLAYS,              0, "hideAllPaylinesDisplays"                                              },
      { SHOW_LINES_SWITCH,                       1, "showLinesSwitch"                                                      },
      { SHOW_PLAY_X_PER_LINE_SWITCH,             1, "showPlayXPerLineSwitch"                                               },
      { DRAW_PAYLINES_INDIVIDUALLY,              1, "drawPaylinesIndividually"                                             },
      { DISPLAY_TOTAL_WIN_MESSAGE,               1, "displayTotalWinMessage"                                               },
      { DISPLAY_WIN,                             2, "displayWin"                                                           },
      { POST_SYMBOLS_ON_REELS,                   2, "postSymbolsOnReels"                                                   },
      { CALCULATE_ROLLUP_THRESHOLD,              0, "calculateRollupThreshold"                                             },
      { CALCULATE_ROLLUP_THRESHOLD,              1, "calculateRollupThreshold"                                             },
      { INITIALIZE_CELEBRATION_MODE,             1, "initializeCelebrationMode"                                            },
      { SET_CELEBRATION_BET_THRESHOLD,           1, "setCelebrationBetThreshold"                                           },
      { SET_MULTIWAY_OVERLAY_SYMBOL_NAME,        ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setMultiWayOverlaySymbolName"},
      { SET_PAYLINE_OVERLAY_SYMBOL_NAME,         ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setPaylineOverlaySymbolName" },
      { SET_ROLLUP_DATA,                         ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setRollupData"               },
      { SET_SCATTER_OVERLAY_SYMBOL_NAME,         ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setScatterOverlaySymbolName" },
      { DISPLAY_MULTIWAY_OVERLAY,                1, "displayMultiWayOverlay"                                               },
      { DISPLAY_PAYLINE_OVERLAY,                 1, "displayPaylineOverlay"                                                },
      { DISPLAY_SCATTER_OVERLAY,                 0, "displayScatterOverlay"                                                },
      { DISPLAY_SCATTER_OVERLAY,                 1, "displayScatterOverlay"                                                },
      { SET_MULTI_WAY_HIDE_SEGMENT_MASK_EVENT,   1, "setMultiWayHideSegmentMaskEvent"                                      },
      { SET_MULTI_WAY_SHOW_SEGMENT_MASK_EVENT,   1, "setMultiWayShowSegmentMaskEvent"                                      },
      { SHOW_BET_MESSAGE_METER_PANEL,            0, "showBetMessageMeterPanel"                                             },
      { SHOW_BET_MESSAGE_METER_PANEL,            1, "showBetMessageMeterPanel"                                             },
      { SHOW_BET_MESSAGE_METER_PANEL,            2, "showBetMessageMeterPanel"                                             },
      { SHOW_BET_MESSAGE_PLAYER_MENU,            0, "showBetMessagePlayerMenu"                                             },
      { SHOW_BET_MESSAGE_PLAYER_MENU,            1, "showBetMessagePlayerMenu"                                             },
      { LOAD_WIN_MESSAGES_IN_HISTORY_PROPERTIES, 0, "loadWinMessagesInHistoryProperties"                                   },
      { LOAD_WIN_MESSAGES_IN_HISTORY_PROPERTIES, 1, "loadWinMessagesInHistoryProperties"                                   },
      { LOAD_WIN_MESSAGES_IN_HISTORY_PROPERTIES, 2, "loadWinMessagesInHistoryProperties"                                   },
      { SHOW_BONUS_BET_MESSAGE_PLAYER_MENU,      0, "showBonusBetMessagePlayerMenu"                                        },
      { SHOW_STAGE,                              1, "showStage"                                                            },
      { STORE_TRANSITION_OVERLAY_SYMBOLS,        0, "storeTransitionOverlaySymbols"                                        },
      { RESET_TRANSITION_OVERLAY_SYMBOLS,        0, "resetTransitionOverlaySymbols"                                        },
      { SET_TRANSITION_OVERLAY_RESOURCE_KEY,     1, "setTransitionOverlayResourceKey"                                      },
      { ENABLE_TRANSITION_OVERLAYS,              1, "enableTransitionOverlays"                                             },
      { UPDATE_MULTIWAY_ACTIVE_SYMBOL_MASK,      2, "updateMultiWayActiveSymbolMask"                                       },
      { SHOW_CONTROL_PANELS,                     1, "showControlPanels"                                                    },
      { SHOW_ARROWS_IN_SLOT_HISTORY_STATE,       1, "showArrowsInSlotHistoryState"                                         },
      { UPDATE_PAYTABLE_AWARD_PROPERTIES,        0, "updatePaytableAwardProperties"                                        },
      { UPDATE_PAYTABLE_AWARD_PROPERTIES,        1, "updatePaytableAwardProperties"                                        },
      { ADD_DYNAMIC_BET_BUTTON,                  3, "addDynamicSlotBetButton"                                              },
      { ADD_DYNAMIC_BET_BUTTON,                  4, "addDynamicSlotBetButton"                                              },
      { SET_DYNAMIC_BET_ALIAS,                   1, "setDynamicSlotBetButtonAlias"                                         },
      { SYNCHRONIZE_DYNAMIC_SLOT_BET_BUTTONS,    0, "synchronizeDynamicSlotBetButtons"                                     },
      { ENABLE_DYNAMIC_SLOT_BET_BUTTONS,         1, "enableDynamicSlotBetButtons"                                          },
      { ENABLE_DYNAMIC_SLOT_BET_BUTTONS,         2, "enableDynamicSlotBetButtons"                                          },
      { SET_SINGLE_CREDIT_ROLLUP_THRESHOLD,      1, "setSingleCreditRollupThreshold"                                       },
      { POWERHIT_SPIN_REELS,                     1, "powerhitSpinReels"                                                    },
      { DELAY_STANDALONE_PROGRESSIVE_METER_RESET,  1, "delayStandaloneProgressiveMeterReset"                               },
      { SYNCHRONIZE_STANDALONE_PROGRESSIVE_METERS, 0, "synchronizeStandaloneProgressiveMeters"                             },
      { SET_DEFAULT_ROLLUP_RULE,                 3, "setDefaultRollupRule"                                                 },
      { SET_ROLLUP_RULE,                         6, "setRollupRule"                                                        },
      { CLEAR_LAST_GAME_WIN_AMOUNT,              0, "clearLastGameWinAmount"                                               },
      { AUTO_START_GAME_ON_ROLLUP_SLAM,          0, "autoStartGameOnRollupSlam"                                            },
      { SYNCHRONIZE_VIRTUAL_SPECTRUM_WITH_REAL_VALUE,      0,    "synchronizeVirtualSpectrumWithRealValue"                 },
      { SET_DYNAMIC_BUTTON_BLANK_IMAGE,          ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "setDynamicButtonBlankImage"  },
      { MOVE_NEXT_QUEUED_REEL,                   0, "moveNextQueuedReel"                                                   },
      { SHOW_VOLUME_BUTTON_WITH_MORE_GAMES_ENABLED, 1, "showVolumeButtonWithMoreGamesEnabled"                              },
      { ADD_PAYTABLE_AWARD_PROPERTY, ScriptFunctionMap::VARIABLE_ARGUMENT_COUNT, "addPaytableAwardProperty"                },
      { SYNCHRONIZE_PAYLINES,                       1, "synchronizePaylines"                                               },
      { EXPLODE_SYMBOLS,                         0, "explodeSymbols"                                                       },
      { ENABLE_SLAM_MECHANICAL_REELS,            1, "enableSlamMechanicalReels"                                            },
      { SET_DOUBLEUP_ROLLUP_NEEDED_FLAG,         1, "setDoubleUpRollupNeededFlag"                                          },
      { CHECK_RMLP_TRIGGERED,                    0, "checkRmlpTriggered"                                                   },
      { GET_RMLP_AWARD_AMOUNT,                   0, "getRmlpAwardAmount"                                                   },
      // Terminator.
      { -1, 0, "" }
   };
   addScriptFunctionMap (script_functions);
}

// Purpose: Prepare the object for destruction.
void SlotPresentation::preDestructor (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::preDestructor" << endl;
   #endif
   // Call the base class.
   GamePresentation::preDestructor ();
}

/// <summary> Sets the reel positions for games that have the base game presentation evaluation flag set to false.</summary>
void SlotPresentation::setReelPositionForNonBaseGamePresentationEvaluation (void)
{
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      uint32 total_reels = numberOfReels ();

      for (uint32 reel_index = 0; reel_index < total_reels; reel_index++)
      {
         if (isStandardReelsType (currentReelSet ()))
         {
            // If Reels spin forever, they need to be told where to stop to before they can be stopped.
            uint32 physical_stop = 0;
            physical_stop = slot_evaluator->getPhysicalStop (reel_index,
                                                             slot_evaluator->getReelColdStartPosition (reel_index));
            reels->setToPosition (reel_index, physical_stop);
         }
         else
         {
            // This game has non standard reel types.
            // Let the SlotPresentation code set the symbol stops on each symbol
            setSegmentsToPosition (currentGameSetIndex (), reel_index);
         }
      }
   }
}

/// <summary> Sets the base game presentation evaluation flag. Shows the reels if the flag is set to true and hides the reels otherwise.</summary>
/// <param name="base_game_presentation_evaluation">[in] Value to set the base game presentation evaluation flag.</param>
void SlotPresentation::setBaseGamePresentationEvaluationFlag (bool base_game_presentation_evaluation)
{
   GamePresentation::setBaseGamePresentationEvaluationFlag (base_game_presentation_evaluation);

   if (!baseGamePresentationEvaluation ())
   {
      setReelPositionForNonBaseGamePresentationEvaluation ();
   }

   // Make sure that the wins are no longer animating.
   displayWin (false, false);

   // And hide all those paylines, just in case they are still around.
   synchronizePaylines (false);

   // Switching the reels off and on will change the bet message.. So we should update it.
   showBetMessageMeterPanel (true, false);

   // Hide the game pays win display.
   displayTotalWinMessage (false);
}

// Purpose: Add a value that links a bet configuration to an image
void SlotPresentation::addDynamicSlotBetButton (DynamicBetNValue bet_n_value, DynamicSelectNValue select_n_value, DynamicExtraNValue extra_n_value, DynamicImageIndex image_index)
{
   #ifdef __VERBOSE_DYNAMIC_BUTTONS__
   debug << "SlotPresentation::addDynamicSlotBetButton (" << bet_n_value << ", " << select_n_value << ", " << extra_n_value << ", " << (int)image_index <<")" << endl;
   #endif

   // Create a key based on the given data
   DynamicSlotBetButtonMapSubKey new_sub_key (bet_n_value, select_n_value);
   DynamicSlotBetButtonMapKey new_key (new_sub_key, extra_n_value);
   // Store the new data into the map.
   dynamic_slot_bet_button_map[new_key] = image_index;
}

// Purpose:  Get the reel stop for the specified reel.
bool SlotPresentation::allReelsAreHeld (void)
{
   // Initialize the return value.
   bool all_reels_are_held = true;
   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);
   // Make sure the essential record has been initialized.
   if (slot_essential_record->numberOfReels (currentReelSet ()) == 0)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::allReelsAreHeld (), Called before essential record is initialized."));
   // Try to find a reel that is not held.
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < slot_essential_record->numberOfReels (currentReelSet ()); ++reel_index)
   {
      if (slot_essential_record->reelState (currentReelSet (), reel_index) != SlotEssentialRecord::HELD)
      {
         all_reels_are_held = false;
         break;
      }
   }
   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::allReelsAreHeld: " << (int) all_reels_are_held << endl;
   #endif
   return all_reels_are_held;
}

// Purpose: Updates the slot win animator interface parameter received with the animator configured for the game set specified.
bool SlotPresentation::animateSlotWin (Mojo<AnimateSlotWinInterface> &animate_slot_win, uint32 game_set_index)
{
   animate_slot_win.release ();
   // Extract the game set information.
   Mojo<SlotGameSetInterface> slot_game_set;
   // Extract the animator from the game set.
   if (slotGameSet (slot_game_set, game_set_index))
      slot_game_set->getAnimateSlotWin (animate_slot_win);
   return animate_slot_win.isValid ();
}

// Purpose:  Member function to retrieve the variable bonus_bet_type_game.
bool SlotPresentation::bonusBetTypeGame (void)
{
   return (bonus_bet_type_game);
}

// Purpose:  Returns true the flow has determined that it's ok to show the award to the player.
bool SlotPresentation::canDisplayAward (void)
{
   bool can_display_award = false;

   // For slot games we need to display the award if in state evaluate complete in case of a jackpot handpay situation.
   can_display_award = (   GamePresentation::canDisplayAward ()
                       || (getFlowState () == GameStateValue::STATE_EVALUATE_COMPLETE) );

   return (can_display_award);
}

// Purpose:  Returns true if the paylines displays can safely be updated.
bool SlotPresentation::canUpdatePaylines (void)
{
   // Initialize the return value.
   bool can_update = false;
   // The paylines should not be updated during normal game play if the bet is not in an
   // enabled betting mode.
   if (bet.isValid () && bet->enabled ())
      can_update = true;
   return can_update;
}
// Unsupported conditions don't effect the return value.
bool SlotPresentation::checkPostWaitEventConditions (const vector<String> &conditions, vector<String> &unsupported_conditions, bool force_all)
{
   // Initialize return values. All conditions true until one is determined false.
   bool result = true;
   unsupported_conditions.clear ();
   // For every condition specified...
   for (uint32 index = 0; (index < conditions.size ()) && (force_all || result); ++index)
   {
      if (conditions [index] == CONDITION_LAST_NUDGE_GAME)
      {
         result &= (actIndex() > 0) ? processConditionLastNudge () : 0;
      }
      else if (conditions [index] == CONDITION_NOT_LAST_NUDGE_GAME)
      {
         result &= (actIndex() > 0) ? !processConditionLastNudge () : 0;
      }
      else if (conditions [index] == CONDITION_NUDGE_TRIGGERED)
      {
         result &= !processConditionLastNudge ();
      }
      else if (conditions [index] == CONDITION_NOT_NUDGE_TRIGGERED)
      {
         result &= processConditionLastNudge ();
      }
      else
      {
         // Report conditions not handled by this routine.
         unsupported_conditions.push_back (conditions [index]);
      }
   }
   // If any unhandled conditions...
   if ((force_all || result) && unsupported_conditions.size ())
   {
      // Call base class.
      vector<String> remaining_conditions = unsupported_conditions;
      result &= GamePresentation::checkPostWaitEventConditions (remaining_conditions, unsupported_conditions, force_all);
   }
   // Return true if all the handled conditions where true.
   return result;
}

// Purpose: Return true if we have no more reels to nudge.
bool SlotPresentation::processConditionLastNudge (void)
{
   return (getQueuedReels () == 0);
}

// Called to confirm that at least one set of paylines have been configured.
void SlotPresentation::confirmPaylinesAreConfigured (void)
{
   // At least one set of paylines needs to have been configured.
   Mojo<PaylinesInterface> paylines;
   if (!getPaylines (paylines, currentGameSetIndex ()))
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::confirmPaylinesAreConfigured: Paylines have not been configured."));
}

// Purpose: Returns the Maximum size of Visible Symbols of all the reels.
uint32 SlotPresentation::findMaxNumberOfVisibleSymbols(uint32 _reel_set, uint32 _num_of_reels)
{
   uint32 largest_visible_segment = 0;
   uint32 num_visible = 0;

   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < _num_of_reels; ++reel_index)
   {
      num_visible = numberOfVisibleSymbols (_reel_set, reel_index);
      if (num_visible > largest_visible_segment)
      {
        largest_visible_segment = num_visible;
      }
   }
   return (largest_visible_segment);
}

////////////////////////////////////////////////////////////////////////////
/////////////////////  Serializer Support  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Construct the object that will contain and serialize the essential record data.  Allow derived
// classes to override if necessary.
void SlotPresentation::constructEssentialRecord (void)
{
   #ifdef __VERBOSE_SERIALIZE__
   debug << "SlotPresentation:" << __FUNCTION__ << endl;
   #endif

   // Destroy the current essential record (if there is one) before constructing a new one.
   destructEssentialRecord ();

   // Create the object that contains essential record/critical data.
   slot_essential_record_ptr = new SlotEssentialRecord;

   // Set all ER pointers that the base classes need.
   essential_record_ptr = slot_essential_record_ptr;

   // Identify the class/type of the essential record for the serialize/unserialize functions.
   // It is recommened that derived classes of essential record use an id that is different from any
   // of its base classes (for error checking purposes).
   assertion (essential_record_ptr);
}

// Construct the object that will contain and serialize the essential record data.  Allow derived
// classes to override if necessary.
void SlotPresentation::constructSharedEssentialRecord (void)
{
   #ifdef __VERBOSE_SERIALIZE__
   debug << "SlotPresentation:" << __FUNCTION__ << endl;
   #endif

   // Destroy the current essential record (if there is one) before constructing a new one.
   destructSharedEssentialRecord ();

   // Create the object that contains essential record/critical data.
   slot_shared_essential_record_ptr = new SlotSharedEssentialRecord;

   // Set all ER pointers that the base classes need.
   shared_essential_record_ptr = slot_shared_essential_record_ptr;

   // Identify the class/type of the essential record for the serialize/unserialize functions.
   // It is recommened that derived classes of essential record use an id that is different from any
   // of its base classes (for error checking purposes).
   assertion (shared_essential_record_ptr);
}


// The following method updates the control panel interface received with the registered igame control panel.
// If the control panel is valid, this method will return true.
bool SlotPresentation::controlPanel (IGameControlPanelExtractor &control_panel)
{
   // Initialize the return value.
   bool success = false;
   // Since the validity of the control panel is being used as the return value, free the control panel received
   // just in case it is valid mojo.
   control_panel.release ();
   // If the control panel alias has been registered, retrieve the control panel by alias name.
   if (igame_control_panel.length () != 0)
   {
      Mojo<ControlPanelInterface> control_panel_interface;
      // Extract the control panel by registered alias name.
      success = GamePresentation::controlPanel (igame_control_panel, control_panel_interface);
      // Initialize the return control panel with the control panel extracted above.
      control_panel.initialize (control_panel_interface);
   }
   // Return true if the control panel configured is valid.
   return (success && control_panel.isValid ());
}

// Purpose:  Creates a new game set configuration as appropriate for this presentation.
bool SlotPresentation::createGameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface)
{
   // Create a new game set configuration manager for this presentation.
   Mojo<SlotGameSetConfiguration, GameSetConfigurationInterface> slot_game_set_configuration;
   slot_game_set_configuration.create<SlotGameSetConfiguration> ();
   // Extract the GameSetConfigurationInterface and update the reference parameter.
   game_set_configuration_interface = slot_game_set_configuration;
   return game_set_configuration_interface.isValid ();
}

// Return an interface to an object given its id.
bool SlotPresentation::createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared)
{
   return GamePresentation::createScriptableObjectById (new_object, object_id, parameter_list, new_declared);
}

// Purpose: Return the current reel set.
uint32 SlotPresentation::currentReelSet (void)
{
   return current_reel_set_index;
}

/// <summary>
/// # Cause the virtual spectrum meters (stepper style or video style) to stop updating if set to false.
/// </summary>
/// <remarks>
/// # The progressive actor meter script needs to be configurared to listen for the
/// # "Standalone:Spectrum..." event instead of the original "Spectrum:DisplayAmount:Level"
/// # if FALSE, just listen for the "Spectrum:DisplayAmount:Level" events OR it can be used with
/// any stepper style virtual spectrum.
/// </remarks> 
/// <example><b>Example:</b>
/// @code
/// this:delayStandaloneProgressiveMeterReset (true)
/// @endcode
/// </example>
void SlotPresentation::delayStandaloneProgressiveMeterReset (const bool delay)
{
   delay_standalone_progressive_meter_reset = delay;
}

// Purpose:  display the overlays on the selected MultiWay.
void SlotPresentation::displayMultiWayOverlay (uint32 multiway_number)
{
   #ifdef __VERBOSE_OVERLAY__
   debug << "displayMultiWayOverlay :" << multiway_number << endl;
   #endif
   // verify the data
   assertion (multiway_number >= 1);
   assertion (slot_evaluator.isValid ());
   // convert the payline data to 0 based
   multiway_number -= 1;

   // Extract the slot award interface so that we can update the win animations based on its information.
   Mojo<SlotAwardInterface> slot_award_interface;
   slotAwardInterface (slot_award_interface);
   // return if the slot award interface is no longer valid
   // this indicates that the user has slammed during the animation
   if (!slot_award_interface.isValid ())
   {
      return;
   }

   if (slot_award_interface->getNumberOfMultiWayAwardItems () == 0)
   {
      return;
   }

   // make sure that the symbol name has been set
   assertion (!multiway_overlay_symbol_name.empty ());

   // allocate strings and mojo's
   String symbol_name;

   // check all symbols
   // fill the symbol ids with the current values
   uint32 reel_set = currentReelSet ();

   // currently all reels have the same number of offsets
   uint32 num_reels = numberOfReels (reel_set);
   uint32 num_visible_symbols = numberOfVisibleSymbols (reel_set, 0);

   // storage for the matches
   enum OverlayMatchType
   {
      NO_MATCH = 0,        // not a wild match
      MATCH,               // match, action not yet decided
      MATCH_HIDE_OVERLAY,  // match, but not part of the win
      MATCH_SHOW_OVERLAY,  // match, and is part of the win
   };
   struct OverlayMatch
   {
      OverlayMatchType match_type;
      uint32 symbol_index;
   };

   OverlayMatch** overlay_match_array = new OverlayMatch* [num_reels];
   for (uint32 i = 0; i < num_reels; ++i)
   {
      overlay_match_array [i] = new OverlayMatch [num_visible_symbols];
   }

   // Go through every symbol and check it against the overlay symbol, if we have a match
   // set the variable and note the index of the match.
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // make sure the number of offsets match the number allocated
      assertion (num_visible_symbols == numberOfVisibleSymbols (reel_set, reel_index));
      // loop through each offset
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         overlay_match_array [reel_index][offset].match_type = NO_MATCH; // Initialize the symbol index
         symbol_name = symbolName (reel_set, reel_index, offset);
         #ifdef __VERBOSE_OVERLAY__
         debug << "Checking symbol_name:" << symbol_name << "x=" << reel_index << ", y=" << offset << ", with MultiWay " << multiway_number << endl;
         #endif
         for (uint32 symbol_index = 0; symbol_index < multiway_overlay_symbol_name.size (); symbol_index++)
         {
            // if the current symbol name at this position matches one of the overlay symbol names...
            if (multiway_overlay_symbol_name [symbol_index] == symbol_name)
            {
               overlay_match_array [reel_index][offset].match_type = MATCH;
               overlay_match_array [reel_index][offset].symbol_index = symbol_index;
               #ifdef __VERBOSE_OVERLAY__
               debug << "Overlay symbol found at: x= " << reel_index << ", y= " << offset << ", with MultiWay " << multiway_number << endl;
               #endif
            }
         }
      }
   }

   // Now we're going to see if the symbols that match the overlay symbol(s) are part of a win...
   // Get the number of winning symbols
   uint16 number_of_winning_symbols = slot_award_interface->getNumberOfMultiWayWinningSymbols (multiway_number);
   // Iterate through all the symbols in this multiway win and set their information in the symbol list
   for (uint32 current_symbol = 0; current_symbol < number_of_winning_symbols; ++current_symbol)
   {
      // Set the reel and offset for this symbol
      uint8 symbol_x_coordinate = slot_award_interface->getMultiWaySymbolX (multiway_number, current_symbol);
      uint8 symbol_y_coordinate = slot_award_interface->getMultiWaySymbolY (multiway_number, current_symbol);
      if (overlay_match_array [symbol_x_coordinate][symbol_y_coordinate].match_type == MATCH)
      {
         overlay_match_array [symbol_x_coordinate][symbol_y_coordinate].match_type = MATCH_SHOW_OVERLAY;
      }
   }

   // process the symbols and mark all remaining MATCH symbols to MATCH_HIDE_OVERLAY symbols.
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         if (overlay_match_array [reel_index][offset].match_type == MATCH)
         {
            overlay_match_array [reel_index][offset].match_type = MATCH_HIDE_OVERLAY;
         }
      }
   }

   String event; // post message to animate the symbols
   Mojo<ReelsInterface> reels;
   bool success = getReels (reels, currentGameSetIndex ());
   assertion (success);

   bool play_multiway_overlay = false;
   bool stop_overlay = false;
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // loop through offsets
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         switch (overlay_match_array [reel_index][offset].match_type)
         {
            case NO_MATCH:
               #ifdef __VERBOSE_OVERLAY__
               debug << "NO MULTIWAY " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH_SHOW_OVERLAY:
               event = multiway_overlay_symbol_name [overlay_match_array [reel_index][offset].symbol_index];
               event << ":MultiWayOverlayStart";
               reels->animateStop (reel_index, offset, event);
               play_multiway_overlay = true;
               #ifdef __VERBOSE_OVERLAY__
               debug << "MultiWayOverlayStart " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH_HIDE_OVERLAY:
               event = multiway_overlay_symbol_name [overlay_match_array [reel_index][offset].symbol_index];
               event << ":MultiWayOverlayStop";
               reels->animateStop (reel_index, offset, event);
               stop_overlay = true;
               #ifdef __VERBOSE_OVERLAY__
               debug << "MultiWayOverlayStop " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH:
            default:
               setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::displayMultiWayOverlay: invalid value found").errorMessage (), whoAreYou());
               releaseTwoDimensionArray (overlay_match_array, num_reels);
               return;
         }
      }
   }
   // Post a generic event indicating whether the symbol overlay will be over any symbols or not
   if (play_multiway_overlay)
   {
      postEvent ("MultiWayOverlay:Play");
   }
   else if (stop_overlay)
   {
      postEvent ("MultiWayOverlay:Stop");
   }
   
   releaseTwoDimensionArray (overlay_match_array, num_reels);
}

// Purpose:  display the wild overlays on the selected payline.
void SlotPresentation::displayPaylineOverlay (uint32 payline_number)
{
   // verify the data
   assertion (payline_number >= 1);
   assertion (slot_evaluator.isValid ());
   assertion (payline_number <= slot_evaluator->getNumberOfLines ());

   // convert the payline data to 0 based
   payline_number -= 1;

   // Extract the slot award interface so that we can update the win animations based on its information.
   Mojo<SlotAwardInterface> slot_award_interface;
   slotAwardInterface (slot_award_interface);
   // return if the slot award interface is no longer valid
   // this indicates that the user has slammed during the animation
   if (!slot_award_interface.isValid ())
   {
      return;
   }

   if (slot_award_interface->getNumberOfLineAwardItems (payline_number) != 1)
   {
      return;
   }

   // make sure that the symbol name has been set
   assertion (!payline_overlay_symbol_name.empty ());

   // allocate strings and mojo's
   String symbol_name;
   int16 x = 0;
   int16 y = 0;

   // check all symbols
   // fill the symbol ids with the current values
   uint32 reel_set = currentReelSet ();

   // currently all reels have the same number of offsets
   uint32 num_reels = numberOfReels (reel_set);
   uint32 num_visible_symbols = numberOfVisibleSymbols (reel_set, 0);

   // storage for the matches
   enum WildMatchType
   {
      NO_MATCH = 0,              // not a wild match
      MATCH,                     // match, action not yet decided
      MATCH_NO_ANIMATE,          // match, but not part of the win
      MATCH_ANIMATE_MULTIPLIER,      // match on payline with multiplier win
      MATCH_ANIMATE_PURE,        // match on payline with cleo win
   };

   struct WildMatch
   {
      WildMatchType match_type;
      uint32 symbol_index;
   };
   // Determin the maximum number of visible symbols dislpayed by all the reels.
   // The game may not be a simple 3x5 or 4x5 set of reels.
   uint32 max_num_visible_symbols = findMaxNumberOfVisibleSymbols(reel_set, num_reels);
   #ifdef __VERBOSE_OVERLAY__
   debug << "Maximum Number of Visible Symbols is: = " << max_num_visible_symbols << endl;
   #endif
   WildMatch** match_wild_array = new WildMatch* [num_reels];
   for (uint32 i = 0; i < num_reels; ++i)
      match_wild_array [i] = new WildMatch [max_num_visible_symbols];

   // Determine where the WILD symbols are displayed on the current Reel/Segment gride.
   // This pattern is saved in match_wild_array
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      num_visible_symbols = numberOfVisibleSymbols (reel_set, reel_index);
      // match sure the number of offsets does NOT exceed the number allocated
      assertion (num_visible_symbols <= max_num_visible_symbols );
      // loop through offsets
      // Determine if this is Left to Right or a Right to Left win.
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         // Initialize entry to NO_MACTH
         match_wild_array [reel_index][offset].match_type = NO_MATCH;
         symbol_name = symbolName (reel_set, reel_index, offset);
         for (uint32 symbol_index = 0; symbol_index < payline_overlay_symbol_name.size (); ++symbol_index)
         {
            if (payline_overlay_symbol_name [symbol_index] == symbol_name)
            {
               // Mark the entry as Wild Match and Store the symbol info ( the index).
               match_wild_array [reel_index][offset].match_type = MATCH;
               match_wild_array [reel_index][offset].symbol_index = symbol_index;
               #ifdef __VERBOSE_OVERLAY__
               debug << "Wild symbol found at: x= " << reel_index << ", y= " << offset << ", on line" << payline_number << endl;
               #endif
            }
         }
      }
   }

   // determine if payline type is a non-doubling pure wild win
   uint32 num_symbols_in_win = slot_award_interface->getNumberOfLineWinningSymbols (payline_number, 0);
   assertion (num_symbols_in_win >= 1);

   // find the payline
   assertion (slot_evaluator.isValid ());
   uint32 num_consecutive_wilds = 0;
   uint32 payline_length = slot_evaluator->getPayLineLength (payline_number);
   for (uint32 payline_symbol_index = 0; payline_symbol_index < payline_length; ++payline_symbol_index)
   {
      x = slot_evaluator->getPayLineXCoordinate (payline_number, payline_symbol_index);  // reel number
      y = slot_evaluator->getPayLineYCoordinate (payline_number, payline_symbol_index);  // segment number
      if (match_wild_array [x][y].match_type == MATCH)
      {
         ++num_consecutive_wilds;
      }
      else
      {
         break;
      }
   }

   // do we have a non-multiplier win
   bool pure_wild_win = false;
   if (num_consecutive_wilds == num_symbols_in_win)
   {
      pure_wild_win = true;
   }

   // Get a reference to the game side slot evaluator interface
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;
   if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::displayPaylineOverlay(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
      releaseTwoDimensionArray (match_wild_array, num_reels);
      return;
   }

   // If the reels are not set up as columns (such as independent reels), they will need to be
   // the x position of the first symbol will have to be translated to column coordinates
   uint16 reel_x, reel_y, column_x, column_y;
   reel_x = slot_award_interface->getLineSymbolX (payline_number, 0, 0);
   reel_y = slot_award_interface->getLineSymbolY (payline_number, 0, 0);

   // process the payline to mark the type of WILD symbol animation.
   // Determine if this is Left to Right or a Right to Left win.
   game_side_evaluator->getColumnXYFromReelXY (reel_x, reel_y, column_x, column_y);
   if (column_x < 1 )  // less than 1, indicates LEFT_TO_RIGHT WIN.
   {
      #ifdef __VERBOSE_OVERLAY__
      debug << "L2R payline length = " << payline_length << " num of win symbols = " << num_symbols_in_win << endl;
      #endif
      // Process the Left to Right payline win to mark the type of WILD symbol animation.
      for (uint32 payline_symbol_index = 0; payline_symbol_index < num_symbols_in_win; ++payline_symbol_index)
      {
         x = slot_evaluator->getPayLineXCoordinate (payline_number, payline_symbol_index);
         y = slot_evaluator->getPayLineYCoordinate (payline_number, payline_symbol_index);
         if (match_wild_array [x][y].match_type == MATCH)
         {
            if (pure_wild_win)
            {
               match_wild_array [x][y].match_type = MATCH_ANIMATE_PURE;
            }
            else
            {
               match_wild_array [x][y].match_type = MATCH_ANIMATE_MULTIPLIER;
            }
         }
      }
   }
   else
   {
      #ifdef __VERBOSE_OVERLAY__
      debug << "R2L payline length = " << payline_length << " num of win symbols = " << num_symbols_in_win << endl;
      #endif
      // Calculate the last symbol of test for a wild symbol match.
      uint16 last_symbol = payline_length - num_symbols_in_win;
      // Process the Right to Left payline win to mark the type of WILD symbol animation.
      for (int32 payline_symbol_index = payline_length-1; payline_symbol_index >= last_symbol; payline_symbol_index--)
      {
         x = slot_evaluator->getPayLineXCoordinate (payline_number, payline_symbol_index);
         y = slot_evaluator->getPayLineYCoordinate (payline_number, payline_symbol_index);

         if (match_wild_array [x][y].match_type == MATCH)
         {
            if (pure_wild_win)
            {
               match_wild_array [x][y].match_type = MATCH_ANIMATE_PURE;
            }
            else
            {
               match_wild_array [x][y].match_type = MATCH_ANIMATE_MULTIPLIER;
            }
            #ifdef __VERBOSE_OVERLAY__
            debug << "R2L Match found for X= " << x << ", y=  "<< y << ", symbol = " << payline_symbol_index << ", payline: " << payline_number << endl;
            #endif
         }
      }
   }

   // process the paylines and mark all remaining WILD Symbols to non-animating symbols.
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // loop through offsets
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         if (match_wild_array [reel_index][offset].match_type == MATCH)
         {
            match_wild_array [reel_index][offset].match_type = MATCH_NO_ANIMATE;
         }
      }
   }

   // post message to animate the symbols
   String event;
   // Now loop through the list of win animations and stop each one.
   Mojo<ReelsInterface> reels;
   bool success = getReels (reels, currentGameSetIndex ());
   assertion (success);

   #ifdef __VERBOSE_OVERLAY__
   debug << "Payline number " << payline_number << endl;
   #endif
   bool play_overlay = false;
   bool stop_overlay = false;
   bool no_overlay = false;
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // loop through offsets
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         switch (match_wild_array [reel_index][offset].match_type)
         {
            case NO_MATCH:
               #ifdef __VERBOSE_OVERLAY__
               debug << "NO MATCH " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH:
               setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::displayPaylineWildOverlay: value not set for symbol").errorMessage (), whoAreYou ());
               releaseTwoDimensionArray (match_wild_array, num_reels);
               return;

            case MATCH_NO_ANIMATE:
               event = payline_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
               event << ":OverlayStop";
               reels->animateStop (reel_index, offset, event);
               stop_overlay = true;
               #ifdef __VERBOSE_OVERLAY__
               debug << "STOP " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH_ANIMATE_MULTIPLIER:
               event = payline_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
               event << ":OverlayStart";
               reels->animateStop (reel_index, offset, event);
               play_overlay = true;
               #ifdef __VERBOSE_OVERLAY__
               debug << "START " << reel_index << " " << offset << endl;
               #endif
               break;

            case MATCH_ANIMATE_PURE:
               event = payline_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
               event << ":NoOverlay";
               reels->animateStop (reel_index, offset, event);
               no_overlay = true;
               #ifdef __VERBOSE_OVERLAY__
               debug << "NO OVERLAY " << reel_index << " " << offset << endl;
               #endif
               break;

            default:
               setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::displayPaylineWildOverlay: invalid value found").errorMessage (), whoAreYou ());
               releaseTwoDimensionArray (match_wild_array, num_reels);
               return;
         }
      }
   }
   // Post a generic event indicating whether the multiplier symbol overlay animation is going to be played on at least one symbol
   if (play_overlay)
   {
      postEvent ("PaylineOverlay:Play");
   }
   else if (no_overlay || stop_overlay) // Post a generic event indicating whether the multiplier symbol overlay should be stopped
   {
      postEvent ("PaylineOverlay:Stop");
   }

   releaseTwoDimensionArray (match_wild_array, num_reels);
}

// Purpose:  display the wild overlays on the selected payline.
void SlotPresentation::displayScatterOverlay (uint32 scatter_number)
{
   // verify the data
   assertion (slot_evaluator.isValid ());

   // Extract the slot award interface so that we can update the win animations based on its information.
   Mojo<SlotAwardInterface> slot_award_interface;
   slotAwardInterface (slot_award_interface);
   // return if the slot award interface is no longer valid
   // this indicates that the user has slammed during the animation
   if (!slot_award_interface.isValid ())
   {
      return;
   }

   if (slot_award_interface->getNumberOfScatterAwardItems () != 1)
   {
      return;
   }

   // make sure that the symbol name has been set
   assertion (!scatter_overlay_symbol_name.empty ());

   // allocate strings and mojo's
   String symbol_name;

   // check all symbols
   // fill the symbol ids with the current values
   uint32 reel_set = currentReelSet ();

   // currently all reels have the same number of offsets
   uint32 num_reels = numberOfReels (reel_set);
   uint32 num_visible_symbols = numberOfVisibleSymbols (reel_set, 0);
   // storage for the matches
   enum WildMatchType
   {
      NO_MATCH = 0,                 // not a wild match
      MATCH,                        // match, action not yet decided
      MATCH_ANIMATE_SCATTER,        // match animate symbol
      MATCH_ANIMATE_BONUS,          // match animate symbol
      MATCH_ANIMATE_RETRIGGER,      // match animate symbol
   };

   struct WildMatch
   {
      WildMatchType match_type;
      uint32 symbol_index;
   };

   WildMatch** match_wild_array = new WildMatch* [num_reels];
   for (uint32 i = 0; i < num_reels; ++i)
      match_wild_array [i] = new WildMatch [num_visible_symbols];

   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // match sure the number of offsets match the number allocated
      assertion (num_visible_symbols == numberOfVisibleSymbols (reel_set, reel_index));
      // loop through offsets
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         match_wild_array [reel_index][offset].match_type = NO_MATCH;
         symbol_name = symbolName (reel_set, reel_index, offset);
         for (uint32 symbol_index = 0; symbol_index < scatter_overlay_symbol_name.size (); symbol_index++)
         {
            if (scatter_overlay_symbol_name [symbol_index] == symbol_name)
            {
               match_wild_array [reel_index][offset].match_type = MATCH;
               match_wild_array [reel_index][offset].symbol_index = symbol_index;
            }
         }
      }
   }

   // Make sure that there is at least one award
   assertion (slot_award_interface->getNumberOfScatterAwardItems () == 1);
   // Get the number of winning symbols
   uint16 number_of_winning_symbols = slot_award_interface->getNumberOfScatterWinningSymbols (scatter_number);
   // Iterate through all the symbols in this scatter win and set their information in the symbol list
   for (uint32 current_symbol = 0; current_symbol < number_of_winning_symbols; ++current_symbol)
   {
      // Set the reel and offset for this symbol
      uint8 symbol_x_coordinate = slot_award_interface->getScatterSymbolX (0, current_symbol);
      uint8 symbol_y_coordinate = slot_award_interface->getScatterSymbolY (0, current_symbol);
      if (match_wild_array [symbol_x_coordinate][symbol_y_coordinate].match_type == MATCH)
      {
         // Default to animate the "SCATTER" feature.
         match_wild_array [symbol_x_coordinate][symbol_y_coordinate].match_type = MATCH_ANIMATE_SCATTER;

         // Now check if we should animate other than "SCATTER" feature.
         if (slot_award_interface->getScatterNumberOfTriggers (scatter_number))
         {
            String trigger_name = slot_award_interface->getScatterTriggerString (scatter_number, 0);

            // Check for empty string.
            if (trigger_name.length ())
            {
               // Triggering self?
               if (trigger_name == currentPaytableSection ())
               {
                  // Change to animate the "RETRIGGER" feature.
                  match_wild_array [symbol_x_coordinate][symbol_y_coordinate].match_type = MATCH_ANIMATE_RETRIGGER;
               }
               else
               {
                  // Change to animate the "BONUS" feature.
                  match_wild_array [symbol_x_coordinate][symbol_y_coordinate].match_type = MATCH_ANIMATE_BONUS;
               }
            }
         }
      }
   }

   // post message to animate the symbols
   String event;
   // Now loop through the list of win animations and stop each one.
   Mojo<ReelsInterface> reels;
   bool success = getReels (reels, currentGameSetIndex ());
   assertion (success);

   bool play_scatter_overlay = false;
   bool play_retrigger_overlay = false;
   bool play_bonus_overlay = false;

   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < num_reels; ++reel_index)
   {
      // loop through offsets
      for (uint32 offset = 0; offset < num_visible_symbols; ++offset)
      {
         switch (match_wild_array [reel_index][offset].match_type)
         {
            case NO_MATCH:
               {
                  #ifdef __VERBOSE_OVERLAY__
                  debug << "NO SCATTER " << reel_index << " " << offset << endl;
                  #endif
               }
               break;

            case MATCH:
               {
                  // If we got a match this means that we got a scatter symbol that was not part of the scatter win
                  event = scatter_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
                  event << ":ScatterOverlayStop";
                  reels->animateStop (reel_index, offset, event);
                  #ifdef __VERBOSE_OVERLAY__
                  debug << "SCATTER:STOP " << reel_index << " " << offset << endl;
                  #endif
               }
               break;

            case MATCH_ANIMATE_SCATTER:
               {
                  event = scatter_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
                  event << ":ScatterOverlayStart";
                  reels->animateStop (reel_index, offset, event);
                  play_scatter_overlay = true;
                  #ifdef __VERBOSE_OVERLAY__
                  debug << "SCATTER:START " << reel_index << " " << offset << endl;
                  #endif
               }
               break;

            case MATCH_ANIMATE_BONUS:
               {
                  event = scatter_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
                  event << ":BonusOverlayStart";
                  reels->animateStop (reel_index, offset, event);
                  play_bonus_overlay = true;
                  #ifdef __VERBOSE_OVERLAY__
                  debug << "BONUS:START " << reel_index << " " << offset << endl;
                  #endif
               }
               break;

            case MATCH_ANIMATE_RETRIGGER:
               {
                  event = scatter_overlay_symbol_name [match_wild_array [reel_index][offset].symbol_index];
                  event << ":RetriggerOverlayStart";
                  reels->animateStop (reel_index, offset, event);
                  play_retrigger_overlay = true;
                  #ifdef __VERBOSE_OVERLAY__
                  debug << "RETRIGGER:START " << reel_index << " " << offset << endl;
                  #endif
               }
               break;

            default:
               setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::displayPaylineWildOverlay: invalid value found").errorMessage (), whoAreYou ());
               return;
         }
      }
   }
   // Post a generic event indicating that a symbol overlay animation is going to be played on at least one symbol
   if (play_scatter_overlay)
   {
      postEvent ("ScatterOverlay:Play");
   }
   else if (play_bonus_overlay)
   {
      postEvent ("BonusOverlay:Play");
   }
   else if (play_retrigger_overlay)
   {
      postEvent ("RetriggerOverlay:Play");
   }

   for (uint32 i = 0; i < num_reels; ++i)
   {
      delete [] match_wild_array [i];
   }

   delete [] match_wild_array;
}

// Purpose: Start/Stop the WinDisplay cycle, or show/hide the game pays message
void SlotPresentation::displayTotalWinMessage (bool show)
{
   #if defined (__VERBOSE_WIN__) || defined (__VERBOSE_METHOD__)
   debug << "SlotPresentation::displayTotalWinMessage (" << (show ? "true" : "false") << ")" << endl;
   #endif

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

/// <summary> Starts or stops the win cycle animation.</summary>
/// <param name = "show"> [in] Boolean flag to start or to stop the win cycle.</param>
/// <param name = "start_new_win_cycle"> [in] Flag that indicates whether this is
///   the first time the game should start win cycling a new set of win combiniations.
///   This can be used to play sounds or perform special animations when the win cycle
///   starts for the first time.</param>
void SlotPresentation::displayWin (bool show, bool start_new_win_cycle)
{
   #ifdef __VERBOSE_WIN__
   debug << "SlotPresentation::displayWin (" << (int) show << ", " << (int) start_new_win_cycle << ")" << endl;
   #endif
   // Do the default
   GamePresentation::displayWin (show, start_new_win_cycle);

   // Configure the animate slot wins controller
   Mojo <SlotGameSetInterface> slot_game_set;
   if (slotGameSet (slot_game_set, currentGameSetIndex ()))
   {
      if (slot_game_set->isWinCycleLiteConfigured ())
      {
         Mojo <WinCycleLiteInterface> win_cycle_lite;
         slot_game_set->getWinCycleLite (win_cycle_lite);
         win_cycle_lite->displayWin (show, start_new_win_cycle);
      }
      else
      {
         Mojo<AnimateSlotWinInterface> animate_slot_win;
         if (slot_game_set->getAnimateSlotWin (animate_slot_win))
         {
            animate_slot_win->animateWins (show, start_new_win_cycle);
         }
      }
   }
}

// Purpose:  Draws only the currently selected payline.
void SlotPresentation::drawIndividualPayline (void)
{
   // Make sure that this game has a valid bet configuration, and has the reels enabled.
   if (bet.isValid () && baseGamePresentationEvaluation ())
   {
      // Get an interface to the paylines.
      Mojo<PaylinesInterface> paylines;
      if (getPaylines (paylines, currentGameSetIndex ()))
      {
         Mojo <ResourceInterface> paylines_resource (paylines);
         if (paylines_resource.isValid ())
         {
            // Post event to update bet indicator values.
            String bet_indicator_event (paylines_resource->resourceAlias ());
            bet_indicator_event << ":UpdateBetIndicators";
            postEvent (bet_indicator_event);
            paylines->showSelectedPaylines (bet, false);
            // If the SUBBETS PER CREDIT is greater than 1 we need to show the lines in-between the step
            uint32 selected_payline_count = numberOfSelectedPaylines ();
            ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
            uint32 number_of_lines_interval (config.getValue (config_id));
            for (uint32 payline = selected_payline_count - number_of_lines_interval; payline < selected_payline_count; ++payline)
            {
               // Draw the current payline.
               paylines->showSelectedPayline (payline, true, true);
            }
         }
      }
   }
}

// Purpose:  Determine whether paylines will be drawn individually or all at once..
void SlotPresentation::drawPaylinesIndividually (bool draw_individual)
{
   draw_paylines_individually = draw_individual;
}

// Purpose: return state of Dynamic Slot Bet Buttons
bool SlotPresentation::dynamicSlotBetButtonsAreEnabled (void)
{
   return (update_dynamic_bet_buttons);
}

// Purpose:  Enable/Disable the bet per line slider
void SlotPresentation::enableBetPerLineSlider (bool enable_slider)
{
   // Enable the bet per line slider.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->enableBetPerLineSlider (enable_slider);
   }
}

// Purpose:  Called to enable all known control panels.
void SlotPresentation::enableControlPanels (bool enable)
{
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::enableControlPanels ()" << endl;
   #endif
   // Call the base class first.
   GamePresentation::enableControlPanels (enable);
   // Enable the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
      igame_control_panel.getControlPanelInterface ()->enable (enable);
}

// Purpose: enable or disable updating of dynamicSlotBetButtons.  Update synchonizes bet buttons if true
void SlotPresentation::enableDynamicSlotBetButtons (bool enable, bool update)
{
   update_dynamic_bet_buttons = enable;

   if(enable && update)
   {
      synchronizeDynamicSlotBetButtons ();
   }
}

void SlotPresentation::enableGameMenuButtons (bool enable)
{
   // Enable the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      if (numberOfAvailablePlayerGames () > 1)
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableGameMenuButtonSmall (enable);
      }
      else
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableGameMenuButton (enable);
      }
   }
}

void SlotPresentation::enableMoreGamesButton (bool enable)
{
   // Enable the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      if (numberOfAvailablePlayerGames () > 1)
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableMoreGamesButton (enable && !bank_bet.isBankLocked ());
      }
   }
}

// Purpose: Enable the appropriate "play" button for enrollment double-tap feature.
void SlotPresentation::enableEnrollDoubleTapButton (void)
{
   // Enable the orginally pressed "play" button (if needed) to support player
   // double tap (i.e. player convenience feature to enroll & spin by double tapping the same button).

   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      switch (getEnrollmentButtonDoubleTapButtonId ())
      {
         case ENROLL_DBL_TAP_SPIN_BUTTON:
            // No need to enable spin button.
            break;
         case ENROLL_DBL_TAP_MAX_BET_BUTTON:
            igame_control_panel.getIGameControlPanelInterface ()->enableMaxBetButton (true /* ok_to_enable */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_1:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 1 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_2:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 2 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_3:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 3 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_4:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 4 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_5:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 5 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_6:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 6 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_7:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 7 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_8:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 8 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_9:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 9 /* bet_N_button_number */);
            break;
         case ENROLL_DBL_TAP_BET_N_COINS_BUTTON_10:
            igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 10 /* bet_N_button_number */);
            break;

         default:
            break;
      }
   }
}

// Enable the max bet and bet N buttons for enrollment double-tap feature.
void SlotPresentation::enableBettingButtonsForDoubleTap (void)
{
   // Enable the max bet and bet N buttons to support player
   // double tap (i.e. player convenience feature to enroll & spin by double tapping the same button).

   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      String value_string;
      Config config;
      ConfigID enrollment_start_method_id = config.getId ("GAME ENROLLMENT START METHOD");
      config.getValueString (enrollment_start_method_id, value_string);

      igame_control_panel.getIGameControlPanelInterface ()->enableMaxBetButton (true);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 1 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 2 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 3 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 4 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 5 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 6 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 7 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 8 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 9 /* bet_N_button_number */);
      igame_control_panel.getIGameControlPanelInterface ()->enableBetNCoinsButton (true /* ok_to_enable */, 10 /* bet_N_button_number */);

      if (value_string == "ON_GAME_START_REQUEST")
      {
         igame_control_panel.getIGameControlPanelInterface ()->flashMaxBetButton ();
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (1 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (2 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (3 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (4 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (5 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (6 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (7 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (8 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (9 /* bet_N_button_number */);
         igame_control_panel.getIGameControlPanelInterface ()->flashBetNCoinsButton (10 /* bet_N_button_number */);
      }
   }
}

// Purpose:  Execute a function given its id.
bool SlotPresentation::execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list)
{
   bool result = true;

   switch (func_id)
   {
      case HIDE_ALL_PAYLINES_DISPLAYS:
      {
         hideAllPaylinesDisplays ();
      }
      break;

      case CALCULATE_ROLLUP_THRESHOLD:
      {
         // By default, pass in the current resourc key.
         if (parameter_list.getParameterCount () == 0)
         {
            Mojo<ResourceList, ScriptInterface> resource_list;
            if (resourceList (resource_list))
            {
               calculateRollupThreshold (resource_list->resourceKey ());
            }
         }
         else
         {
            calculateRollupThreshold (parameter_list [0].asString ());
         }
      }
      break;

      case INITIALIZE_CELEBRATION_MODE:
         initializeCelebrationMode (parameter_list [0].asString ());
         break;

      case SET_CELEBRATION_BET_THRESHOLD:
         rollup_rule_default.celebration_bet_threshold = parameter_list [0].asUint32 ();
         break;

      case SET_ROLLUP_DATA:
      {
         float win_to_bet_ratio_high_range (0.0f);
         Amount::AmountValue high_range (0);
         uint32 event_threshold_index (0);
         if (parameter_list.getParameterCount() != 5 && parameter_list.getParameterCount() != 6)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupData has invalid number of parameters").errorMessage (), whoAreYou ());
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
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupData has invalid parameter string const MAX_FLOAT.").errorMessage (), whoAreYou ());
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
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupData has invalid parameter string const MAX INT.").errorMessage (), whoAreYou ());
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
         setRollupData (parameter_list [0].asFloat (), win_to_bet_ratio_high_range, parameter_list [2].asInt64 (),
                        high_range, parameter_list [4].asInt32 (), event_threshold_index);
      }
      break;

      case SNAP_REELS_TO_STOP_POSITION:
      {
         snapReelsToStopPosition ();
      }
      break;

      case SPIN_HELD_REELS:
      {
         if (parameter_list [0].parameterType () == ParameterType::STRING
            && strcmp (parameter_list [0].asString (), "ONLY_WHEN_ALL_REELS_ARE_HELD") == 0)
         {
            held_reel_spin_config = SPIN_REELS_IF_ALL_REELS_ARE_HELD;
         }
         else if (parameter_list [0].asBool ())
            held_reel_spin_config = SPIN_REELS_IF_REELS_ARE_HELD;
         else
         {
            held_reel_spin_config = DO_NOT_SPIN_REELS_IF_REELS_ARE_HELD;
         }
      }
      break;

      case SHOW_LINES_SWITCH:
      {
         showLinesSwitch (parameter_list [0].asBool ());
      }
      break;

      case SHOW_PLAY_X_PER_LINE_SWITCH:
      {
         showPlayXPerLineSwitch (parameter_list [0].asBool ());
      }
      break;

      case DRAW_PAYLINES_INDIVIDUALLY:
      {
         drawPaylinesIndividually (parameter_list [0].asBool ());
      }
      break;

      case DISPLAY_TOTAL_WIN_MESSAGE:
      {
         displayTotalWinMessage (parameter_list [0].asBool ());
      }
      break;

      case DISPLAY_WIN:
      {
         displayWin (parameter_list [0].asBool (), parameter_list [1].asBool ());
      }
      break;

      case POST_SYMBOLS_ON_REELS:
      {
         postSymbolsOnReels (parameter_list[0].asString (), parameter_list[1].asString ());
      }
      break;

      case DISPLAY_MULTIWAY_OVERLAY:
      {
         displayMultiWayOverlay (parameter_list [0].asUint32 ());
      }
      break;

      case DISPLAY_PAYLINE_OVERLAY:
      {
         displayPaylineOverlay (parameter_list [0].asUint32 ());
      }
      break;

      case DISPLAY_SCATTER_OVERLAY:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            displayScatterOverlay (0);
         }
         else if (parameter_list.getParameterCount () == 1)
         {
            displayScatterOverlay (parameter_list [0].asUint32 ());
         }
      }
      break;

      case SET_MULTIWAY_OVERLAY_SYMBOL_NAME:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setMultiWayOverlaySymbolName has invalid parameter count.").errorMessage (), whoAreYou ());
            return false;
         }
         else
         {
            setMultiWayOverlaySymbolName (parameter_list);
         }
      }
      break;

      case SET_PAYLINE_OVERLAY_SYMBOL_NAME:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setMultiplierWildSymbolName has invalid parameter count.").errorMessage (), whoAreYou ());
            return false;
         }
         else
         {
            setPaylineOverlaySymbolName (parameter_list);
         }
      }
      break;

      case SET_SCATTER_OVERLAY_SYMBOL_NAME:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setScatterOverlaySymbolName has invalid parameter count.").errorMessage (), whoAreYou ());
            return false;
         }
         else
         {
            setScatterOverlaySymbolName (parameter_list);
         }
      }
      break;

      case SET_MULTI_WAY_HIDE_SEGMENT_MASK_EVENT:
      {
         setMultiWayHideSegmentMaskEvent (parameter_list[0].asString ());
      }
      break;

      case SET_MULTI_WAY_SHOW_SEGMENT_MASK_EVENT:
      {
         setMultiWayShowSegmentMaskEvent (parameter_list[0].asString ());
      }
      break;

      case SHOW_BET_MESSAGE_METER_PANEL:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            showBetMessageMeterPanel (true, true);
         }
         else if (parameter_list.getParameterCount () == 1)
         {
            showBetMessageMeterPanel (true, parameter_list[0].asBool ());
         }
         else if (parameter_list.getParameterCount () == 2)
         {
            showBetMessageMeterPanel (parameter_list[0].asBool (), parameter_list[1].asBool ());
         }
      }
      break;

      case SHOW_BET_MESSAGE_PLAYER_MENU:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            showBetMessagePlayerMenu (true);
         }
         else if (parameter_list.getParameterCount () == 1)
         {
            showBetMessagePlayerMenu (parameter_list[0].asBool ());
         }
      }
      break;

      case SHOW_BONUS_BET_MESSAGE_PLAYER_MENU:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            showBonusBetMessagePlayerMenu (true);
         }
      }
      break;

      case LOAD_WIN_MESSAGES_IN_HISTORY_PROPERTIES:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            loadWinMessagesInHistoryProperties (true, true);
         }
         if (parameter_list.getParameterCount () == 1)
         {
            loadWinMessagesInHistoryProperties (parameter_list[0].asBool (), parameter_list[0].asBool ());
         }
         else if (parameter_list.getParameterCount () == 2)
         {
            loadWinMessagesInHistoryProperties (parameter_list[0].asBool (), parameter_list[1].asBool ());
         }
      }
      break;

      case STORE_TRANSITION_OVERLAY_SYMBOLS:
      {
         if (transition_overlays_enabled)
         {
            storeTransitionOverlaySymbols ();
         }
      }
      break;

      case RESET_TRANSITION_OVERLAY_SYMBOLS:
      {
         if (transition_overlays_enabled)
         {
            resetTransitionOverlaySymbols ();
         }
      }
      break;

      case SHOW_STAGE:
      {
         if (transition_overlays_enabled)
         {
            showStage (parameter_list [0].asBool ());
         }
      }
      break;

      case SET_TRANSITION_OVERLAY_RESOURCE_KEY:
      {
         if (transition_overlays_enabled)
         {
            setTransitionOverlayResourceKey (parameter_list[0].asString ());
         }
      }
      break;

      case ENABLE_TRANSITION_OVERLAYS:
      {
         enableTransitionOverlays (parameter_list [0].asBool ());
      }
      break;

      case UPDATE_MULTIWAY_ACTIVE_SYMBOL_MASK:
      {
         updateMultiWayActiveSymbolMask (parameter_list[0].asBool (), parameter_list[1].asInt8 ());
      }
      break;

      case SHOW_CONTROL_PANELS:
      {
         showControlPanels (parameter_list [0].asBool ());
      }
      break;

      case SHOW_ARROWS_IN_SLOT_HISTORY_STATE:
      {
         showArrowsInSlotHistoryState (parameter_list [0].asBool ());
      }
      break;

      case UPDATE_PAYTABLE_AWARD_PROPERTIES:
      {
         if (parameter_list.getParameterCount () == 0)
         {
            updatePaytableAwardProperties ();
         }
         else if (parameter_list.getParameterCount () == 1)
         {
            updatePaytableAwardProperties (parameter_list[0].asBool ());
         }
      }
      break;

      case ADD_DYNAMIC_BET_BUTTON:
      {
         if (parameter_list.getParameterCount () == 3)
         {
            addDynamicSlotBetButton (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (), 0, parameter_list [2].asUint32 ());
         }
         else if (parameter_list.getParameterCount () == 4)
         {
            addDynamicSlotBetButton (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (), parameter_list [2].asUint32 (),parameter_list [3].asUint32 ());
         }
      }
      break;
      case SET_DYNAMIC_BET_ALIAS:
      {
         setDynamicSlotBetButtonAlias (parameter_list [0].asString ());
      }
      break;
      case SYNCHRONIZE_DYNAMIC_SLOT_BET_BUTTONS:
      {
         synchronizeDynamicSlotBetButtons ();
      }
      break;

      case ENABLE_DYNAMIC_SLOT_BET_BUTTONS:
      {
         if (parameter_list.getParameterCount () == 1)
         {
            enableDynamicSlotBetButtons (parameter_list[0].asBool ());
         }
         else if (parameter_list.getParameterCount () == 2)
         {
            enableDynamicSlotBetButtons (parameter_list[0].asBool (), parameter_list[1].asBool ());
         }
      }
      break;

      case SET_SINGLE_CREDIT_ROLLUP_THRESHOLD:
         single_credit_rollup_threshold = parameter_list [0].asUint32 ();
      break;

      case POWERHIT_SPIN_REELS:
      {
         powerhit_spin_reels = parameter_list [0].asBool ();
      }
      break;

      case DELAY_STANDALONE_PROGRESSIVE_METER_RESET:
      {
         delayStandaloneProgressiveMeterReset (parameter_list [0].asBool ());
      }
      break;

      case SYNCHRONIZE_STANDALONE_PROGRESSIVE_METERS:
      {
         // Update all meters
         synchronizeStandaloneProgressiveMeters ();
      }
      break;

      case SET_DEFAULT_ROLLUP_RULE:
      {
         uint32 start_index = parameter_list [0].asUint32 ();
         uint32 end_index = parameter_list [1].asUint32 ();

         // Sanity check
         if (end_index >= rollup_data_vector.size ())
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "setDefaultRollupRule (), end index exceeds number of RollupModes defined").errorMessage (), whoAreYou ());
            return false;
         }

         if (start_index > end_index)
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "setDefaultRollupRule(), start index must be less than end index").errorMessage (), whoAreYou ());
            return false;
         }

         setDefaultRollupRule (start_index, end_index, parameter_list [2].asUint32 ());
      }
      break;

      case SET_ROLLUP_RULE:
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
               error_message << "setRollupRule: RollupTarget " << parameter_list [3].asString () << " not recognized.";
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, error_message).errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupRule, expected String in arg4").errorMessage (), whoAreYou ());
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
               error_message << "setRollupRule: RollupCriteria " << parameter_list [3].asString () << " not recognized.";
               setErrorString (LogicError (LogicError::INVALID_PARAMETER, error_message).errorMessage (), whoAreYou ());
               return false;
            }
         }
         else
         {
            setErrorString (LogicError (LogicError::INVALID_PARAMETER, " setRollupRule, expected String in arg5").errorMessage (), whoAreYou ());
            return false;
         }

         setRollupRule (parameter_list [0].asUint32 (), parameter_list [1].asUint32 (),
                           parameter_list [2].asUint32 (), target, criteria, parameter_list [5].asUint32 ());
      }
      break;


      case CLEAR_LAST_GAME_WIN_AMOUNT:
      {
         clearLastGameWinAmount ();
      }
      break;

      case AUTO_START_GAME_ON_ROLLUP_SLAM:
      {
         autoStartGameOnRollupSlam ();
      }
      break;

      case SYNCHRONIZE_VIRTUAL_SPECTRUM_WITH_REAL_VALUE:
      {
         // change the spectrum to the real value
         synchronizeVirtualSpectrumWithRealValue ();
      }
      break;

      case SET_DYNAMIC_BUTTON_BLANK_IMAGE:
      {
         if (parameter_list.getParameterCount () == 2)
         {
            setDynamicButtonBlankImage (parameter_list[0].asString (), parameter_list[1].asUint32 ());
         }
         else if (parameter_list.getParameterCount () == 3)
         {
            setDynamicButtonBlankImage (parameter_list[0].asString (), parameter_list[1].asUint32 (), parameter_list[2].asString ());
         }
      }
      break;

      case MOVE_NEXT_QUEUED_REEL:
      {
         moveNextQueuedReel ();
      }
      break;

      case SHOW_VOLUME_BUTTON_WITH_MORE_GAMES_ENABLED:
      {
         show_volume_button_with_more_games_enabled = parameter_list[0].asBool ();
      }
      break;

      case ADD_PAYTABLE_AWARD_PROPERTY:
      {
         Mojo<BBEPaytableAwardPropertyInterface> slot_bbe (slot_evaluator);

         if (slot_bbe.isValid ())
         {
            if (parameter_list.getParameterCount () == 6)
            {
               addPaytableAwardPropertyBBE (parameter_list [0].asString (),
                                            parameter_list [1].asBool (),
                                            parameter_list [2].asString (),
                                            parameter_list [3].asString (),
                                            parameter_list [4].asUint32 (),
                                            parameter_list [5].asUint32 (),
                                            _UINT16_MAX);
            }
            else if (parameter_list.getParameterCount () == 7)
            {
               addPaytableAwardPropertyBBE (parameter_list [0].asString (),
                                            parameter_list [1].asBool (),
                                            parameter_list [2].asString (),
                                            parameter_list [3].asString (),
                                            parameter_list [4].asUint32 (),
                                            parameter_list [5].asUint32 (),
                                            parameter_list [6].asUint16 ());
            }
         }
         else
         {
            uint16 progressive_level = _UINT16_MAX;
            //If the progressive level parameter is negative then
            //the progressive level passed should be _UINT16_max
            //which indicates that there is not a progressive.
            //Otherwise the level needs to be a uint16.
            if (parameter_list [1].asInt32 () > 0)
            {
               progressive_level = parameter_list[1].asUint16 ();
            }

            addPaytableAwardPropertyLegacy (parameter_list [0].asString (), progressive_level,
                                            parameter_list [2].asBool (), parameter_list [3].asUint32 (),
                                            parameter_list [4].asUint32 ());
         }
      }
      break;

      case SYNCHRONIZE_PAYLINES:
      {
         synchronizePaylines (parameter_list[0].asBool ());
      }
      break;

      case EXPLODE_SYMBOLS:
      {
         explodeSymbols ();
      }
      break;

      case ENABLE_SLAM_MECHANICAL_REELS:
      {
        enable_mechanical_slam = parameter_list[0].asBool ();
      }
      break;

      case SET_DOUBLEUP_ROLLUP_NEEDED_FLAG:
      {
         setDoubleUpRollupNeededFlag (parameter_list[0].asBool ());
      }
      break;

      case CHECK_RMLP_TRIGGERED:
      {
         bool was_rmlp_triggered = checkForRmlpAward ();
         ScriptParameter return_parameter (was_rmlp_triggered, ParameterType::BOOL);
         parameter_list.add (return_parameter);
      }
      break;

      case GET_RMLP_AWARD_AMOUNT:
      {
         uint32 rmlp_award_amount = getRmlpAwardAmount ();
         ScriptParameter return_parameter (rmlp_award_amount, ParameterType::UINT32);
         parameter_list.add (return_parameter);
      }
      break;

      default:
      {
         result = GamePresentation::execScriptableFunctionById (func_id, parameter_list);
      }
      break;
   }

   return (result && (!error_set_by_function));
}

// Purpose: Move the next reel in the queue to its stop position.
void SlotPresentation::moveNextQueuedReel (void)
{
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()) && reel_queue_list.size())
   {
      uint8 reel_num = reel_queue_list.front ();
      reels->moveToPosition (reel_num, reelStop (reel_num));
      reel_queue_list.pop ();
   }
}

// Purpose: Add a reel number to the queue
void SlotPresentation::addToReelQueue (uint8 reel_number)
{
   reel_queue_list.push (reel_number);
}

// Purpose: Change reel strips
bool SlotPresentation::reconfigureReels (void)
{
   #ifdef __CHANGING_REELS__
   debug << "SlotPresentation::reconfigureReels(): method Fired!" << endl;
   debug << "SlotPresentation::reconfigureReels(): current_paytable_section = " << currentPaytableSection () << endl;
   debug << "SlotPresentation::reconfigureReels(): currentGameSetIndex () = "<< currentGameSetIndex () << endl;
   #endif
   Mojo<SlotGameSetInterface> slot_game_set;
   if (!slotGameSet (slot_game_set, currentGameSetIndex ()))
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reconfigureReels : Could not find game set."));

   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);

   // Setting current paytable section to current one defined in current set
   setCurrentPaytableSection (slot_essential_record->payTableSectionName (currentGameSetIndex ()));

   // Create a list of the reel stops.
   uint32 total_reels = numberOfReels ();
   if (!total_reels)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reconfigureReels : game has no reels!"));
   }

   // Find the alias to use.
   String alias = slot_game_set->getReelsAlias ();

   // This is empty, but neccessary for the time being.
   assertion (resource_list.isValid ());

   // Reconfigure the reels with the current parameters.
   slot_game_set->configureReels (alias, resource_list->resourceKey (), currentPaytableSection ());

   if(isStandardReelsType (currentReelSet()))
   {
      vector <uint32> reel_stops;
      // Make sure there's enough memory allocated in the vector before extracting the stops.
      if (reel_stops.size () < total_reels)
         reel_stops.resize (total_reels);

      // aquire the reel stop information from the essential record.
      for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
         reel_stops [reel_index] = reelStop (reel_index);

      //Set an initial position for the reels.
      Mojo<ReelsInterface> reels;
      if (getReels (reels, currentGameSetIndex ()))
      {
         //The following code is unnecessary for mechanical reels.  See CR 6328
         Mojo<MechanicalReelsInterface> mechanical_reels (reels);
         if (!mechanical_reels.isValid ())
         {
            for (uint32 reel_index = 0; reel_index < reel_stops.size (); reel_index++)
            {
               #ifdef __CHANGING_REELS__ //LB
                  debug <<"SlotPresentation::reconfigureReels(): vecter reel stop-> Reel: "<< reel_index
                     <<", stop: " << reel_stops[reel_index] << endl;
               #endif
               reels->setToPosition (reel_index, reel_stops[reel_index]);
            }
         }
      }
   }
   else
   {
      // This game has non standard reel types.
      for (uint32 reel_index = 0; reel_index < total_reels; reel_index++)
      {
        snapSegmentsToPosition (currentGameSetIndex (), reel_index);   // set the symbol stops on each symbol
      }
   }

   return true;
}

// Purpose: Called when Game Manager exits our screen context.
bool SlotPresentation::exitGameMode (State& state, const GameStageExitReason &reason)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::exitGameMode ()" << endl;
   #endif
   // If this presentation was ever drawn...
   if (currentGameMode () != GAME_MODE_UNKNOWN)
   {
      switch (reason)
      {
         // If the stage is going off the screen...
         case STAGE_EXIT_REASON_UNKNOWN:
         case STAGE_EXIT_REASON_EXIT_GAME:
         case STAGE_EXIT_REASON_NEW_STAGE:
            {
               // Stop the spinning reels before leaving the context.
               snapReelsToStopPosition ();
            }
            break;

         // If the stage is staying on the screen...
         case STAGE_EXIT_REASON_TO_ON_SCREEN:
            // Leave it running.
            break;
      }
   }
   // Base class last.
   bool success = GamePresentation::exitGameMode (state, reason);
   #ifdef __VERBOSE__
   debug << "SlotPresentation::exitGameMode (), complete" << endl;
   #endif
   return success;
}

// Purpose: This method is called by the leavingObjectScope method to finalize the configuration
// of the control panels registered by this game.
void SlotPresentation::finalizeControlPanelConfiguration (void)
{
   // Call the base class.
   GamePresentation::finalizeControlPanelConfiguration ();

   // Finalize the configuration of all control panels registered by this game.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // Configure the denomination of the new control panel.
      igame_control_panel.getControlPanelInterface ()->setDenomination (denomination);
      // Configure the control panel with this class' sequence event poster.  This assignment ensures that
      // sequence events posted by the control panel are properly configured with game and stage information.
      igame_control_panel.getControlPanelInterface ()->setSequenceEventPoster (sequence_event_poster);
   }
}

/// <summary> Gets a reference to the GameSideSlotEvaluatorInterface.</summary>
/// <param name="game_side_slot_evaluator_interface">[out] Reference to the GameSideSlotEvaluatorInterface.</param>
/// <param name="reel_set_index">[in] (Optional) Index that identifies which reel set to retrieve the
///   GameSideSlotEvaluatorInterface reference from.</param>
/// <returns>True if success and false otherwise.</returns>
bool SlotPresentation::gameSideSlotEvaluatorInterface (Mojo<GameSideSlotEvaluatorInterface> &game_side_slot_evaluator_interface,
   uint32 reel_set_index)
{
   // Make sure we're starting with a free mojo
   game_side_slot_evaluator_interface.release ();
   SlotEvaluatorInterfaces slot_evaluator_interfaces;

   if (slot_evaluator_interfaces.initialize (slot_evaluator))
   {
      game_side_slot_evaluator_interface = slot_evaluator_interfaces.getGameSideSlotEvaluatorInterface ();
   }

   return (game_side_slot_evaluator_interface.isValid ());
}

// Purpose: Get the id of the button that may need to be enabled to support CDS double-tap feature.
SlotPresentation::EnrollDblTapButtonId SlotPresentation::getEnrollmentButtonDoubleTapButtonId (void)
{
   return enrollment_double_tap_button_id;
}

// Purpose:  Returns the number of reel stops configured for the reel id received.
uint32 SlotPresentation::getNumberOfReelStops (uint32 reel_index)
{
   assertion (reel_index < numberOfReels ());
   // Get slot evaluator
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);

   return slot_evaluator.isValid() ? slot_evaluator->getNumberOfReelStopsPhysical (reel_index) : 0;
}

// Updates the payline interface parameter received with the paylines configured for the game set specified.
bool SlotPresentation::getPaylines (Mojo<PaylinesInterface> &paylines, uint32 game_set_index)
{
   paylines.release ();
   // Extract the game set information.
   Mojo<SlotGameSetInterface> slot_game_set;
   // Extract the paylines from the game set.
   if (slotGameSet (slot_game_set, game_set_index))
      slot_game_set->getPaylines (paylines);
   return paylines.isValid ();
}

// Purpose: Return the flag that determines the payline draw behavior.
bool SlotPresentation::getPaylineDrawFlag ()
{
   return draw_paylines_individually;
}

// Purpose: Returns the home segment of the given reel
uint32 SlotPresentation::getReelHomeSegment (uint32 reel_number)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   uint32 home_segment (0);

   // Get the reels.
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // Cast the reels to the video reels interface
      Mojo<VideoReelsInterface> video_reels (reels);
      if (video_reels.isValid ())
      {
         home_segment = video_reels->getHomeSegment (reel_number);
      }
   }

   // For reel classes that don't define number of segments, the default should return 0.
   return (home_segment);
}

// Purpose: Get the total number of segments defined in the reel model
uint32 SlotPresentation::getReelNumberOfSegmentPieces (uint32 reel_number)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   uint32 segment_count (0);

   // Get the reels.
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // Cast the reels to the video reels interface
      Mojo<VideoReelsInterface> video_reels (reels);
      if (video_reels.isValid ())
      {
         segment_count = video_reels->getNumberOfSegmentPieces (reel_number);
      }
   }

   // For reel classes that don't define number of segments, the default should return 0.
   return (segment_count);
}

// Purpose: Gets the number of presentation visible segments for a given reel.
// The presentation count may differ from the evaluator when using oversized symbols.
uint32 SlotPresentation::getReelNumberOfVisibleSegments (uint32 reel_number)
{
   #if defined (__VERBOSE_METHOD__)
      verboseMethodName()
   #endif

   // Get the number of segment pieces defined in the model for video reels.
   uint32 segment_count = getReelNumberOfSegmentPieces (reel_number);
   if (segment_count > 0)
   {
      // Account for the off-screen actor used as a buffer for video reels.
      --segment_count;
   }
   else
   {
      // If video reels don't exist, get the number of visible symbols from the evaluator.
      segment_count = numberOfVisibleSymbols (currentReelSet (), reel_number);
   }

   return (segment_count);
}

// Updates the reels interface parameter received with the reels configured for the game set specified.
bool SlotPresentation::getReels (Mojo<ReelsInterface> &reels, uint32 game_set_index)
{
   reels.release ();
   // Extract the game set information.
   Mojo<SlotGameSetInterface> slot_game_set;
   // Extract the reels from the game set.
   if (slotGameSet (slot_game_set, game_set_index))
      slot_game_set->getReels (reels);
   return reels.isValid ();
}

// Light auto start lamps if available
void SlotPresentation::showStartGameOnRollupSlamLamps (void)
{
   //If we currently allow the player to slam the credit rollup and proceed to the next game, light all the
   // bet per line and play x lines lamps. Do not allow for prebonus rollup states (flow in STATE_EVALUATE) or in freespins
   bool monetary_win = false, trigger_another = false, trigger_self = false;
   bool success = GamePresentationHelper::awardSummary ((GamePresentation*) this, monetary_win, trigger_another, trigger_self);

   // Make sure the the current stage is the base game, one touch slamming is enabled,
   // the current state is STATE_AWARD and a monetary win was detected
   if (stageIndex () == BASE_GAME_STAGE && autoStartGameOnRollupSlamAllowed ()
    && (getFlowState () == GameStateValue::STATE_AWARD)
    && success && !trigger_another && !trigger_self && monetary_win)
   {
      String enable_rollup_slam_lamps ("autoStartGameOnRollupSlam:Lamps:Enable");
      postEventAll (enable_rollup_slam_lamps);
   }
}

// Return true if any monetary wins are present
bool SlotPresentation::isALoser (void)
{
   bool monetary_win, trigger_another, trigger_self, success, is_a_loser (false);
   success = GamePresentationHelper::awardSummary (this, monetary_win, trigger_another, trigger_self);
   if (success)
      is_a_loser = !monetary_win && !trigger_another && !trigger_self;
   return is_a_loser;
}

// Stop the win animations on the paylines.
void SlotPresentation::hideAllPaylinesDisplays (void)
{
   // Stop the win animations.
   Mojo<PaylinesInterface> paylines;
   if (getPaylines (paylines, currentGameSetIndex ()))
      paylines->hideAllDisplays ();
}

// Return a pointer to the slot essential record.
const SlotEssentialRecord* SlotPresentation::getSlotEssentialRecord (void)
{
   return slot_essential_record_ptr;
}

// Return a pointer to the slot shared essential record.
const SlotSharedEssentialRecord* SlotPresentation::getSlotSharedEssentialRecord (void)
{
   return slot_shared_essential_record_ptr;
}

/// <summary> get the number of sunBets from the configuration file</summary>
uint32 SlotPresentation::getNumberOfSubBetsFromConfigFile()
{
   Config config;
   ConfigID config_id = config.getId (CONFIG_ID_NUMBER_OF_SUBBETS, stage_configuration_identifier);
   if (config.getType (config_id) != CONFIG_TYPE_NUMERIC)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::getNumberOfSubBetsFromConfigFile (),\
         the configurartion type about number of sub bets is not right."));
   }

   uint32 number_sub_bets = config.getValue (config_id);

   return number_sub_bets;
}

// Purpose: Initialize the presentation.
//          This call is made before the game runs.  This is the point where all shared
//          objects should be loaded and checked for validity.
//          The current state show enable/show/play the correct items once they
//          are loaded here.
bool SlotPresentation::init (State& state, bool cold_start_flag)
{
   #ifdef __VERBOSE__
      debug << game_index << ":" << stage_index << "|SlotPresentation::init ()" << endl;
   #endif

   bool rv = GamePresentation::init (state, cold_start_flag);

   // load any information that we need from the registry files.
   loadRegistyData ();

   // update the digital glass.
   updateDigitalGlass();

   return rv;
}

// Updates the IntelliSymbolsControllerInterface parameter received with the controller configured for the game set specified.
bool SlotPresentation::intelliSymbolsController (Mojo<IntelliSymbolsControllerInterface> &intellisymbols_controller, uint32 game_set_index)
{
   intellisymbols_controller.release ();
   // Extract the game set information.
   Mojo<SlotGameSetInterface> slot_game_set;
   // Extract the reels from the game set.
   if (slotGameSet (slot_game_set, game_set_index))
      slot_game_set->getIntelliSymbolsController (intellisymbols_controller);
   return intellisymbols_controller.isValid ();
}

// Purpose:  Initialize the IntelliSymbolController with the win information for the current game.
void SlotPresentation::initializeIntelliSymbolList (void)
{
   // Create a list of the reel stops.
   uint32 total_reels = numberOfReels ();
   if (!total_reels)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::initializeIntelliSymbolList: game has no reels!"));

   // Make sure there's enough memory allocated in the vector before extracting the stops.
   if (intellisymbols_reel_stops.size () < total_reels)
      intellisymbols_reel_stops.resize (total_reels);
   // Update the reel stop information.
   for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
      intellisymbols_reel_stops [reel_index] = reelStopVirtual (reel_index);

   // Get the current slot evaluator.
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);
   // If the IntelliSymbolsController has been intialized, give it a chance to process the information for the current game.
   Mojo<IntelliSymbolsControllerInterface> intellisymbols_controller;
   if (intelliSymbolsController (intellisymbols_controller, currentGameSetIndex ()))
      intellisymbols_controller->initializeIntelliSymbolList (bet, slot_evaluator, &intellisymbols_reel_stops [0], intellisymbols_reel_stops.size ());
}

// Purpose: Determines if this game is of a bonus bet type, configures accordingly.
void SlotPresentation::initializeBonusBet (void)
{
   ConfigID config_id = config.getId ("BONUS BET MULTIPLIER", stage_configuration_identifier, false);
   // If the config item is not valid or We are using a bet object that doesn't support the game side bet interface
   // disable bonus bet type game.
   if (config_id == INVALID_CONFIGID)
   {
      bonus_bet_type_game = false;
      max_bonus_bet_multiplier = 1;
      number_bonus_bet_levels = 0;
      postEvent ("BonusBetTypeGame:DISABLED");
   }
   else
   {
      bonus_bet_type_game = true;
      max_bonus_bet_multiplier = (config.getValue (config_id));
      number_bonus_bet_levels = 1;
      postEvent ("BonusBetTypeGame:ENABLED");
      if (bet.isValid () && bet->getBetAmountByName ("bonus") > ZERO_AMOUNT)
      {
         postEvent ("BonusBetEnabled");
      }
      else
      {
         postEvent ("BonusBetDisabled");
      }
   }

   // Update the Select lines meter with proper bonus bet value
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;

   // Do not update the bonus bet if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      if (resource_list->getResource<PropertyInterface> ("MaxSelectLinesValue", resource_list->resourceKey (), property, false))
      {
         // Update the resource with the current SelectLines value.
         property->setAbsoluteValue (number_of_lines_max_value);
      }
      if (resource_list->getResource<PropertyInterface> ("BonusSelectLinesValue", resource_list->resourceKey (), property, false))
      {
         // Update the resource with the current SelectLines value.
         property->setAbsoluteValue (number_of_lines_max_value+number_bonus_bet_levels);
      }
   }
}
// Purpose: Set the ranges of the sliders with the values from the config file.
void SlotPresentation::initializeSliders (void)
{
   ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
   int32 number_of_lines_interval (config.getValue (config_id));

   config_id = config.getId ("MINIMUM NUMBER OF SUBBETS", stage_configuration_identifier);
   int32 number_of_lines_min_value (config.getValue (config_id));

   config_id = config.getId ("NUMBER OF SUBBETS", stage_configuration_identifier);
   number_of_lines_max_value = (config.getValue (config_id));

   config_id = config.getId ("MINIMUM TOTAL BET", stage_configuration_identifier);
   int32 bet_per_line_min_value (config.getValue (config_id));

   config_id = config.getId ("MAXIMUM TOTAL BET", game_configuration_identifier);
   int32 max_total_bet (config.getValue (config_id));

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

   // Initialize the ranges.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      bool handled_sliders = false;
      if (bet.isValid () && doesBetSupportGameSideBetInterface ())
      {
         BetOptions bet_options;
         bet->getBetOptions (bet_options, false);

         // Reinitialize and update the sliders as necessary if bet is limited to a permitted list
         if (bet_options.limit_bets_to_permitted_list)
         {
            Mojo <GameSideBetInterface> game_side_bet_interface (bet);
            uint32 subbet_min;
            uint32 subbet_max;
            uint32 bet_per_subbet_min;
            uint32 bet_per_subbet_max;
            game_side_bet_interface->getSubbetRangesFromPermittedBetList (subbet_min, subbet_max);
            game_side_bet_interface->getBetPerSubbetRangesFromPermittedBetList (bet_per_subbet_min, bet_per_subbet_max);
            bet_per_line_min_value = bet_per_subbet_min;
            bet_per_line_max_value = bet_per_subbet_max;
            number_of_lines_min_value = subbet_min;
            number_of_lines_max_value = subbet_max;
            // For permitted bet list, the steps for the slider can vary, so set that as minimum value for now
            igame_control_panel.getIGameControlPanelInterface ()->initializeBetPerLineSlider (bet_per_line_min_value, bet_per_line_max_value, 1);
            igame_control_panel.getIGameControlPanelInterface ()->initializeSelectLinesSlider (number_of_lines_min_value, number_of_lines_max_value+number_bonus_bet_levels, 1);
            handled_sliders = true;
         }
      }
      // If the sliders ranges has not been handled yet, do the default values
      if (!handled_sliders)
      {
         igame_control_panel.getIGameControlPanelInterface ()->initializeBetPerLineSlider (bet_per_line_min_value, bet_per_line_max_value, bet_per_line_min_value);
         igame_control_panel.getIGameControlPanelInterface ()->initializeSelectLinesSlider (number_of_lines_min_value, number_of_lines_max_value+number_bonus_bet_levels, number_of_lines_interval);
      }
      igame_control_panel.getIGameControlPanelInterface ()->setConfigurationIdentifier (theme_configuration_identifier);
   }
}

// Called when we are about to leave the object's scope.
void SlotPresentation::leavingObjectScope (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|SlotPresentation::leavingObjectScope" << endl;
   #endif
   // Call the base class.
   GamePresentation::leavingObjectScope ();

    // Verify that the slot game set has been configured.
   Mojo<SlotGameSetInterface> slot_game_set;
   if (!slotGameSet (slot_game_set))
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::leavingObjectScope: Slot game set not defined."));
}

//Purpose: Return true if the enrollment state has started
bool SlotPresentation::isEnrollmentStarted ()
{
   return (enrollment_started);
}

// Load the paytable provided.  If 0 is passed for the paytable filename, the current paytable will be loaded.
// If 0 is passed for the section name, the default paytable section will be loaded.
// Returns: true on success.
bool SlotPresentation::loadPaytable (const char *filename, const char *section)
{
   // Call the base class first.
   bool paytable_loaded = GamePresentation::loadPaytable (filename, section);
   if (paytable_loaded)
   {
      // Now attempt to get a slot evaluation pointer
      Mojo<SlotEvaluatorInterface> slot_evaluator;
      slotEvaluatorInterface (slot_evaluator);  // sets the (global to class) slot_evaluator data member.
      if (slot_evaluator.isValid ())
      {
         // We have a slot evaluation pointer, so the paytable must be a slot paytable.
         #ifdef __VERBOSE_PAYTABLE__
         debug << "SlotPresentation::loadPaytable () This is a slot paytable" << endl;
         #endif
         // Ouput some slot paytable information to the debug log
         #ifdef __VERBOSE_PAYTABLE__
         int number_of_reels = slot_evaluator->getNumberOfReels ();
         debug << "  -number of reels=" << number_of_reels << endl;
         debug << "  -number of stops=";
         for (int reel_index = 0; reel_index < number_of_reels; ++reel_index)
            debug << (int)slot_evaluator->getNumberOfReelStops (reel_index) << ", ";
         debug << endl;
         #endif

         initializePaytableAwardProperties(slot_evaluator);

         // We were expecting to get a slot paytable so the operation was
         // successful.
         paytable_loaded = true;

         //if any symbol replacement needed, do it here
         initSymbolsSubstitution ();
      }
      // This is fatal. A slot paytable and evaluator is expected for this game.
      else
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::loadPaytable(), Invalid slot eval mojo."));
   }

   // Make sure the number of paylines is equal to the number of betting subbets
   if (bet.isValid() && slot_evaluator.isValid())
   {
      if (slot_evaluator->getNumberOfLines() != 0)
      {
         if (getNumberOfSubBetsFromConfigFile() != slot_evaluator->getNumberOfLines())
         {
            char error_message [512];
            SNPRINTF (error_message, sizeof (error_message), "ERROR: Paytable has %lu lines and the bet has %lu sub bets", (uint32)slot_evaluator->getNumberOfLines(), (uint32)bet->getNumberOfSubBets());
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
      }
   }
   return paytable_loaded;
}

// Purpose: Loads the properties that are used to toggle the win messages in history after a bonus.
void SlotPresentation::loadWinMessagesInHistoryProperties (bool toggle_win_message, bool show_total_win_credit_meter_value)
{
   #ifdef __VERBOSE_METERS__
      debug << "SlotPresentation::loadWinMessagesInHistoryProperties ()" << endl;
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
      base_game_pays_string = "Base Game Pays ";
   }
   else
   {
      if (doubleUpWin ())
      {
         base_game_pays_string = "Double Up Pays ";
      }
      else
      {
         base_game_pays_string = "Game Pays ";
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
   String total_game_pays_string = "Total Game Pays ";
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
      error_message << "SlotPresentation::loadWinMessagesInHistoryProperties () "
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
      error_message << "SlotPresentation::loadWinMessagesInHistoryProperties () "
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
      error_message << "SlotPresentation::loadWinMessagesInHistoryProperties () "
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
      error_message << "SlotPresentation::loadWinMessagesInHistoryProperties () "
                    << "Unable to retrieve property: " << CREDIT_METER_VALUE_BEFORE_BONUS;
      setErrorString (LogicError (LogicError::INVALID_VALUE, error_message).errorMessage (), whoAreYou ());
      return;
   }

   if (!isBingo ())
   {
      // post history messages only when bingo is not active, bingo uses WinDisplay
      if (toggle_win_message)
      {
         postEvent ("ToggleWinMessagesInHistory:PropertiesLoaded");
      }
      else
      {
         postEvent ("ShowBaseGameWinMessage");
      }

      if (!show_total_win_credit_meter_value)
      {
         // This event is posted if the base game credit meter in History on a bonus initiating act is to
         // diplay the pre-bonus credit meter value (includes initiating act winnings)
         postEvent ("ShowPreBonusCreditMeterValue");
      }
   }
}

/// <summary> Gets the number of selected paylines from the bet object.</summary>
/// <remarks> Assumes that the Paylines object is valid if it is used.</remarks>
/// <returns> Number of selected paylines. </returns>
uint32 SlotPresentation::numberOfSelectedPaylines (void)
{
   uint32 selected_payline_count (0);

   if (numberOfPaylines () != 0)
   {
      // Make sure that this game has a valid bet configuration.
      if (bet.isValid ())
      {
         selected_payline_count = bet->numberOfActiveSubBets ();
      }
   }
   return (selected_payline_count);
}

/// <summary># Check whether the game has the ability to slam reels.</summary>
/// <returns># Flag that check whether the game has the ability to slam reels.</returns>
bool SlotPresentation::okayToSlamReels ()
{
   return !isBaseGameStage (stageIndex()) || (0 == minimumReelSpinTime ());
}

// Purpose: Retrieve the minimum time the reels need to spin before stopping
uint32 SlotPresentation::minimumReelSpinTime (void)
{
   // Cache the config id for the minimum reel spin time
   Config config;
   ConfigID config_id (config.getId ("MINIMUM WAGER INTERVAL", false));
   uint32 minimum_reel_spin_time (0);
   if (config_id != INVALID_CONFIGID)
      minimum_reel_spin_time = config.getValue (config_id);

   return minimum_reel_spin_time;
}

// Purpose: Set reel spin delay script property
void SlotPresentation::setReelSpinDelay (void)
{
   #ifdef __VERBOSE_METHOD__
      verboseMethodName ();
   #endif

   Mojo<ResourceList, ScriptInterface> resource_list;
   if (resourceList (resource_list))
   {

      // Get the reel spin delay property
      Mojo<PropertyInterface> reel_spin_delay_property;
      if (resource_list->getResource<PropertyInterface> ("ReelSpinDelay", resource_list->resourceKey (), reel_spin_delay_property, false))
      {
         // set the property to define a specfic reel spin time
         reel_spin_delay_property->setAbsoluteValue (minimumReelSpinTime ());
         // Post event to apply the extended reel spin
         postEvent ("ReelsState:SpinDelay");

      }
   }

}


// Purpose:  Check to see if there are zero subbets
bool SlotPresentation::numberOfSelectedSubBetsIsZero (void)
{
   bool return_value = true;

   // Make sure that this game has a valid bet configuration.
   if (bet.isValid ())
   {
      // If sub bet 0 is not 0
      if (!bet->getSubBetAmount (0).isZero ())
      {
         // There are not 0 sub bets selected
         return_value = false;
      }
   }
   return return_value;
}

// Purpose: Return the number of multi-ways
uint16 SlotPresentation::numberOfMultiWays (void)
{
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;
   if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfMultiWays(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface."));
   }

   // Return the appropriate number
   return (game_side_evaluator.isValid() ? game_side_evaluator->getNumberOfMultiWaySymbols () : 0);
}

// Purpose: Return the number of multi-ways selected
uint16 SlotPresentation::numberOfMultiWaysSelected (void)
{
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;
   if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfMultiWaysSelected(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface."));
   }

   // Return the appropriate number
   return (game_side_evaluator->getNumberOfMultiWaysBet (bet));
}

/// <summary>Get the number of paylines from the slot evaluator.</summary>
uint32 SlotPresentation::numberOfPaylines (void)
{
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);

   return slot_evaluator->getNumberOfLines ();
}

// Purpose: Return the number of reels on current reel set.
uint32 SlotPresentation::numberOfReels (void)
{
   // Get the current slot evaluator
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);

   // Return the appropriate number
   return slot_evaluator.isValid() ? slot_evaluator->getNumberOfReels () : 0;
}

// Purpose: Return the number of reels in the set.
uint32 SlotPresentation::numberOfReels (uint32 reel_set)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfReels (), Called before essential record is initialized.").errorMessage (), whoAreYou ());
      return 0;
   }
   // Return the number of reels.
   return slot_essential_record->numberOfReels (reel_set);
}

// Purpose: Return the number of reel sets in the game.
uint32 SlotPresentation::numberOfReelSets (void)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfReelSets (), Called before essential record is initialized."));
   // Return the size of a slot essential record since we are a slot game.
   return slot_essential_record->numberOfReelSets ();
}

// Purpose: Return the number of machines.
uint16 SlotPresentation::getNumberOfMachines (void)
{
   return 1;
}

// Purpose: Return the number of stops on the given reel.
uint32 SlotPresentation::numberOfReelStops (uint32 reel_set, uint32 reel)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfReelStops (), Called before essential record is initialized."));
   // Get the paytable section name used during the last changing of the reel-set data (null terminated).
   const char *paytable_section = slot_essential_record->payTableSectionName (reel_set);
   if (!paytable_section)
      System::error (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::numberOfReelStops (), Reel set index has no paytable section configured."));
   // Get the game evaluator for the given paytable section.
   Mojo<EvaluatorInterface> game_evaluator_mojo;
   if (!evaluatorForPaytableSection (game_evaluator_mojo, paytable_section))
   {
      String error;
      error = "SlotPresentation::numberOfReelStops (), '";
      error << paytable_section << "' section has no game evaluator configured.";
      System::error (LogicError (LogicError::INVALID_VALUE, error));
   }
   // Get a slot evaluator interface for the given paytable section.
   Mojo<SlotEvaluatorInterface> slot_evaluator_mojo;
   slot_evaluator_mojo = game_evaluator_mojo;
   if (!slot_evaluator_mojo.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfReelStops (), Paytable section\'s evaluator does not support slot games."));
   // Get the number of reel stops for the given reel.
   int32 stops = slot_evaluator_mojo->getNumberOfReelStops (reel);
   if (stops < 0)
      System::error (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::numberOfReelStops (), Reel index is invalid."));
   // Return result.
   return (uint32) stops;
}

// Purpose: Return the number of scatters
uint16 SlotPresentation::numberOfScatters (void)
{
   // Get the current slot evaluator
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);

   // Return the appropriate number
   return slot_evaluator.isValid() ? slot_evaluator->getNumberOfScatterSymbols () : 0;
}

// Purpose: Return the number of patterns
uint16 SlotPresentation::numberOfPatterns (void)
{
   // Get the current slot evaluator
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   slotEvaluatorInterface (slot_evaluator);

   // Return the appropriate number
   return slot_evaluator.isValid() ? slot_evaluator->getNumberOfPatterns () : 0;
}

// Purpose: Return the number of visible symbols on the given reel.
uint32 SlotPresentation::numberOfVisibleSymbols (uint32 reel_set, uint32 reel)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfVisibleSymbols (), Called before essential record is initialized.").errorMessage (), whoAreYou ());
      return 0;
   }
   // Get the paytable section name used during the last changing of the reel-set data (null terminated).
   const char *paytable_section = slot_essential_record->payTableSectionName (reel_set);
   if (!paytable_section)
   {
      setErrorString (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::numberOfVisibleSymbols (), Reel set index has no paytable section configured.").errorMessage (), whoAreYou ());
      return 0;
   }
   // Get the game evaluator for the given paytable section.
   Mojo<EvaluatorInterface> game_evaluator_mojo;
   if (!evaluatorForPaytableSection (game_evaluator_mojo, paytable_section))
   {
      String error;
      error = "SlotPresentation::numberOfVisibleSymbols (), '";
      error << paytable_section << "' section has no game evaluator configured.";
      setErrorString (LogicError (LogicError::INVALID_VALUE, error).errorMessage (), whoAreYou ());
      return 0;
   }
   // Get a slot evaluator interface for the given paytable section.
   Mojo<SlotEvaluatorInterface> slot_evaluator_mojo;
   slot_evaluator_mojo = game_evaluator_mojo;
   if (!slot_evaluator_mojo.isValid ())
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::numberOfVisibleSymbols (), Paytable section\'s evaluator does not support slot games.").errorMessage (), whoAreYou ());
      return 0;
   }
   // Get the number of visible symbols for this reel index.
   int16 visible_symbols = slot_evaluator_mojo->getNumberOfVisibleSymbols (reel);
   if (visible_symbols < 0)
   {
      setErrorString (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::numberOfVisibleSymbols (), Reel index is invalid.").errorMessage (), whoAreYou ());
      return 0;
   }
   // Return result.
   return (uint32) visible_symbols;
}

// Purpose:  Returns the value of the okay_to_hide_spin_button.
bool SlotPresentation::okayToHideSpinButton (void)
{
   return okay_to_hide_spin_button;
}

/// <summary>
/// # Bonus string updates will not force a digital glass refresh unless this function returns true.
/// </summary> 
/// <example><b>Example:</b>
/// @code
/// this:okayToUpdateBonusString ()
/// @endcode
/// </example>
// Purpose:  Returns the value of the okay_to_update_bonus_string.
bool SlotPresentation::okayToUpdateBonusString(void)
{
   return okay_to_update_bonus_string;
}

// Purpose: Play sequence on guiding arrows.
void SlotPresentation::playArrowsSequence (const char* sequence_name, bool start_sequence)
{
   // Use Reels resource alias to scope Multimedia events to particular Reel object
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      Mojo<ResourceInterface> reels_resource (reels);
      String reels_alias (reels_resource->resourceAlias ());

      uint32 total_reels = numberOfReels ();
      for (uint32 reel = 0; reel < total_reels; ++reel)
      {
         String arrows_event_string (reels_alias);
         if (start_sequence == true)
            arrows_event_string << ":Reel" << reel << ":StartArrowsSequence:" << sequence_name;
         else
            arrows_event_string << ":Reel" << reel << ":StopArrowsSequence:" << sequence_name;

         postEvent (arrows_event_string);
      }
   }
}
// Purpose:  Initialize the IntelliSymbolController with the win information for the current game.
void SlotPresentation::postSymbolsOnReels (String name, String event_prefix)
{
   // Get the current slot evaluator.
   Mojo<SlotEvaluatorInterface> slot_evaluator_interface;
   slotEvaluatorInterface (slot_evaluator_interface);

   // Create a list of the reel stops.
   uint32 total_reels = numberOfReels ();
   if (!total_reels)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::initializeIntelliSymbolList: game has no reels!").errorMessage (), whoAreYou ());
      return;
   }

   vector <uint32> reel_stops;
   // Make sure there's enough memory allocated in the vector before extracting the stops.
   if (reel_stops.size () < total_reels)
      reel_stops.resize (total_reels);

   // Update the reel stop information.
   for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
      reel_stops [reel_index] = reelStopVirtual (reel_index);

   uint32 number_of_reels = slot_evaluator_interface->getNumberOfReels ();
   for (uint32 reel_index = 0; reel_index < number_of_reels; ++reel_index)
   {
      // Search each of the current reel's stops.  In a scatter situation, a pattern
      // can be created if at least one stop displays the current symbol.  This implies
      // that processing on this reel can be stopped upon first location of the symbol
      // for each configured group.
      uint32 number_of_symbols = slot_evaluator_interface->getNumberOfVisibleSymbols (reel_index);
      for (uint32 stop_offset = 0; stop_offset < number_of_symbols; ++stop_offset)
      {
         // Calculate the current stop information.
         if (reel_index >= total_reels)
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE,  "SlotPresentation::initializeIntelliSymbolList, invalid reel index.").errorMessage (), whoAreYou ());
            return;
         }
         // Cache the name of the symbol represented by this reel and stop information.
         String symbol_name = slot_evaluator_interface->getSymbolName (reel_index, reel_stops [reel_index], stop_offset);
         #ifdef __VERBOSE__
         debug << "reel_index = " << reel_index << ", stop_index = " << reel_stops [reel_index] << ", stop_offset = " << stop_offset;
         debug << ":  " << symbol_name << endl;
         #endif
         if (symbol_name == name)
         {
            String event (event_prefix);
            event << ":" << symbol_name << ":Reel:" << reel_index << ":Offset:" << stop_offset;
            postEvent (event);
         }
      }
   }
}

// Purpose: Final preparations before this presentation is unloaded.
bool SlotPresentation::prepareForUnloading (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|SlotPresentation::prepareForUnloading ()" << endl;
   #endif

   // Release objects held by this class.
   slot_evaluator.release ();
   intellisymbols_reel_stops.resize (0);

   igame_control_panel.clear ();

   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|SlotPresentation::prepareForUnloading (), complete" << endl;
   #endif
   // Give the base class a chance to unload its resources.
   return GamePresentation::prepareForUnloading ();
}

// Purpose: Handle Bonus Bet specific multimedia events.
bool SlotPresentation::processBonusBetEvent (const MultimediaEvent &event)
{
   bool handled = true;

   if (bonusBetTypeGame() == true)
   {
      if (strcmp (event.getString (), "BetChanged:MaxBet") == 0)
      {
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

         if (bonus_bet_amount > ZERO_AMOUNT)
         {
            if (!bonus_bet_message)
            {
               postEvent ("BonusBetEnabled");
               updateBonusBetStatus (true);
            }

            // post the event for the maximum bonus bet value
            String bonus_bet_change_event ("BonusBetChanged:");
            bonus_bet_change_event << bet_per_line_max_value;
            postEvent (bonus_bet_change_event);
         }
         updateSliderBonusPosition();
      }
      else if (strcmp (event.getString (),"NumberOfLinesBetSlider:UpdateHandleText:Bonus") == 0)
      {
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

         if (bonus_bet_amount > ZERO_AMOUNT)
         {
            if (!bonus_bet_message)
            {
               postEvent ("BonusBetEnabled");
               updateBonusBetStatus (true);
            }
         }
         updateSliderBonusPosition();
      }
      else if (strcmp (event.getString (),"NumberOfLinesBetSlider:UpdateHandleText") == 0)
      {
         if (bonus_bet_message)
         {
            postEvent ("BonusBetDisabled");
            updateBonusBetStatus (false);
         }
      }
      else if (strcmp (event.getString (), "Slider:NumberOfLinesBetSlider:Enabled:SynchronizePosition") == 0)
      {
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

         if (bonus_bet_amount > ZERO_AMOUNT)
         {
            if (!bonus_bet_message)
            {
               String event_to_post;
               // This portion of code (else if statement this is contained within) will not be executed unless the NumberOfLinesBetSlider
               // is enabled,this is why the event below can be create and posted
               event_to_post << "Slider:NumberOfLinesBetSlider:Enabled:NewValueRequest:Position" << slot_evaluator->getNumberOfLines() + 1;

               postEvent ("BonusBetEnabled");
               postEvent ("ForcesSliderToBonusBet");
               postEvent (event_to_post);

               updateBonusBetStatus (true);
            }
         }
      }
      else if (strcmp (event.getString (), "SlotColdStartState:CommonExitSuspend") == 0)
      {
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

         if (bonus_bet_amount > ZERO_AMOUNT)
         {
            if (!bonus_bet_message)
            {
               postEvent ("BonusBetEnabled");
               postEvent ("ScaleSliderBonusBet");
               updateBonusBetStatus (true);
            }
         }
      }
   }
   else
   {
      handled = false;
   }

   return (handled);
}

bool SlotPresentation::processEvent (const MultimediaEvent &event)
{
   bool handled = true;

   bool bonus_handled = processBonusBetEvent (event);

   // listen for the event that says we are changing the lines slider meter
   if ((strcmp (event.getString (), "NumberOfLinesBetSlider:UpdateHandleText") == 0) ||
       (strcmp (event.getString (), "ScrollBar:NumberOfLinesBetSlider:Enabled:NewValue") == 0) ||
       (strcmp (event.getString (), "ScrollBar:NumberOfLinesBetSlider:Disabled:NewValue") == 0) ||
       (strcmp (event.getString (), "SynchronizeSelectLinesSlider") == 0) ||
       (strcmp (event.getString (), "NumberOfLinesBetSlider:UpdateHandleText") == 0))
   {
      setPaylinesPerCreditProperty ();
   }
   else if (strcmp (event.getString (), "DigitalGlassPaytable:Synchronize") == 0)//DGTAG Start
   {
      if (theme_configuration_identifier)
      {
         ConfigID digital_glass_paytable_enabled_id (config.getId (CONFIG_ID_DIGITAL_GLASS_PAYTABLE_ENABLED, theme_configuration_identifier, false));
         if (digital_glass_paytable_enabled_id != INVALID_CONFIGID)
         {
            // Update the digital glass in the idle state (if needed)
            bool digital_glass_paytable_enabled (config.getValue (digital_glass_paytable_enabled_id));
            String digital_glass_event ("DigitalGlassPaytable:");
            digital_glass_event << (digital_glass_paytable_enabled ? "Enabled" : "Disabled");
            postEvent (digital_glass_event);
         }
      }

      postShowDigitalGlassEvents();
   }
   //DGTAG End

   else if (!strncmp (event.getString (), PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT, strlen (PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT)))
   {
      processSpectrumEvent (event, PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT, PROPERTY_SPECTRUM_TEXT_DISPLAY_VALUE_ROOT);
   }
   else if (!strncmp (event.getString (), PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT, strlen (PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT)))
   {
      processSpectrumEvent (event, PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT, PROPERTY_SPECTRUM_DISPLAY_VALUESTRING_ROOT);
   }

   else if (strcmp (event.getString (), "GamePresentation:Synchronize") == 0)
   {
      updatePaytableAwardProperties();
   }

   else if (event == "WaitForPlayerInputState:CommonEnterSynchronize")
   {
      // If we are to auto play simulate game inputs, post the multimedia event to end the WaitForPlayerInputState
      Config config;
      ConfigID autoplay_simulate_id = config.getId ("AUTOPLAY SIMULATE GAME INPUTS");

      if (config.getValue (autoplay_simulate_id))
      {
         postEvent ("WaitForPlayerInputState:AdvancePresentationState");
      }
   }
   else if (event == "LanguageSwap:SynchronizeControlPanels")
   {
      // Ensure that the presentation of the win meter is correct for wins or losses after language swap.
      synchronizeControlPanels ();
      // Set control panel start buttons to their default settings.
      synchronizeStartButtons ();

      // Set control panel new card request button to its default setting.
      synchronizeNewCardButton ();

      // Allow cash out after language swap if desirable
      synchronizeCashout ();
      synchronizeTransferButton ();

      // Enable the necessary buttons
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableBetPerLineButton (true);

         if (numberOfSelectedSubBetsIsZero ())
         {
            igame_control_panel.getIGameControlPanelInterface ()->enableBetPerLineSlider (false);
         }
         else
         {
            igame_control_panel.getIGameControlPanelInterface ()->enableBetPerLineSlider (true);
         }
         igame_control_panel.getIGameControlPanelInterface ()->enableSelectLinesButton (true);
         igame_control_panel.getIGameControlPanelInterface ()->enableSelectLinesSlider (true);

         // This is required so that the current button panel buttons re-enable
         if (bet.isValid ())
         {
            igame_control_panel.getIGameControlPanelInterface ()->synchronizeBet (bet);
         }
      }
      // ControlPanelBase::synchronize () called by synchronizeControlPanels () will turn on
      // the display of all meters.  However, in tournament time-only mode, CreditMeter should
      // be hidden, which is done by synchronizeCreditAmount ().
      synchronizeCreditAmount ();
   }
   else if (event == "AutoPlay:StartEvent")
   {
      // This ensures that the Language Button is presented correctly when Auto Play is initiated.
      synchronizeGameLanguage ();
   }
   else if (event == "AutoPlay:StopEvent")
   {
      // Ensure AutoPlay button is presented correctly when Auto Play Stop is pressed outside of GameStateValue::STATE_PLAY.
      // NOTE: this call was added to allow an inheriting class the ability to change or not use the following functionality triggered by "AutoPlay:StopEvent".
      synchronizeAutoPlayButtonForNonPlayState (true);
   }
   else if (event == WinDisplay::HIDE_EVENT)
   {
      win_display.hide ();
   }
   else if (event == WinDisplay::SHOW_NEXT_EVENT)
   {
      win_display.showNext ();
   }
   else if (event == "DoubleUpPreStart")
   {
      setDoubleUpRollupNeededFlag (true);
   }
   else if (event == "checkRmlpTriggered")
   {
      if (checkForRmlpAward ())
      {
         // toggle between BASE WIN and TOTAL WIN.
         // postEventAll SlotHistoryMultimediaCleanup 
         // on IdleAnimateWinsState EXIT to stop; 
         // SlotHistoryDisplayState does this already.
         loadWinMessagesInHistoryProperties (true, true);
      }
   }

   handled = GamePresentation::processEvent (event);
   return (bonus_handled || handled);
}

// Store the button which slammed the symbol animation/rollup.
void SlotPresentation::storeRollupSlamButton (String button)
{
   // Only store once, and only do so in the award state.
   if ((!rollup_slam_button_set) &&
       (getFlowState () == GameStateValue::STATE_AWARD) &&
       (autoStartGameOnRollupSlamAllowed ()) &&
       (doesBetSupportGameSideBetInterface ()))
   {
      uint32 bet_n_coins_button_string_length = strlen (BET_N_COINS_BUTTON);

      // If this is a Bet N Coins button
      if (!strncmp (button, BET_N_COINS_BUTTON, bet_n_coins_button_string_length))
      {
         // Find the button number
         String button_number_string = button.right (strlen (button) - bet_n_coins_button_string_length);
         uint16 button_number = atoi (button_number_string);
         if (button_number > 0)
         {
            Mojo <GameSideBetInterface> game_side_bet_interface (bet);
            if (game_side_bet_interface.isValid ())
            {
               vector<ButtonData> button_data_list;
               game_side_bet_interface->getBetNButtonPanelData (button_data_list);

               // If button_number is within bounds of the ButtonPanel, check if this button is intended to start a game
               if ((button_number - 1 < (uint16) button_data_list.size ()) && game_side_bet_interface->getBetNButtonData (button_number-1).start_game)
               {
                  rollup_slam_button_set = true;
                  rollup_slam_button_string = button;
               }
            }
         }
      }
      else if (!strcmp (button, STOP_BUTTON)
            || !strcmp (button, MAX_BET_BUTTON))
      {
         rollup_slam_button_set = true;
         rollup_slam_button_string = button;
      }
   }
}

// Return the private class member flag rollup_slam_button_set.
bool SlotPresentation::rollupSlamButtonSet (void)
{
   return (rollup_slam_button_set);
}
// Return the private class member string rollup_slam_button_string.
String SlotPresentation::getRollupSlamButton (void)
{
   return (rollup_slam_button_string);
}

void SlotPresentation::processSpectrumEvent (const MultimediaEvent &event, const String& progressive_event_trunk, const String& property_root_name)
{
   String event_string (event.getString ());
   String progressive_level_number (event_string.right (event_string.length () - strlen (progressive_event_trunk)));
   Mojo<PropertyInterface> level_property;
   String progressive_property_string (property_root_name);

   progressive_property_string << progressive_level_number;
   if (resource_list->getResource<PropertyInterface> (progressive_property_string, "Default", level_property, false))
   {
      level_property->setAbsoluteValue (String (""));
   }
   
   // If disabled, we reset the meter display 
   if (!delay_standalone_progressive_meter_reset)
   {
      spectrum_update_manager.addToUpdateList (progressive_level_number.asUint32 ());
      spectrum_update_manager.startUpdate ();
   }
   
   // If enabled, we don't reset the meter display until after the win cycle
   if (delay_standalone_progressive_meter_reset && currentGameMode () == GAME_MODE_PLAY)
   {
      StateId current_flow_state = getFlowState ();
      if (current_flow_state != GameStateValue::STATE_EVALUATE
            && current_flow_state != GameStateValue::STATE_EVALUATE_COMPLETE
            && current_flow_state != GameStateValue::STATE_AWARD)
      {
         String standalone_event ("Standalone:");
         standalone_event << event.getString ();
         postEventAll (standalone_event);
      }
   }
}

bool SlotPresentation::processEvent (const SequenceEvent &event)
{
   return (GamePresentation::processEvent (event));
}

// Purpose:  Get the reel stop for the specified reel.
bool SlotPresentation::reelIsHeld (uint32 reel_set, uint32 reel_index)
{
   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);
   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::reelIsHeld (" << reel_index << "):  " << (int) (slot_essential_record->reelState (reel_set, reel_index) == SlotEssentialRecord::HELD) << endl;
   #endif
   if (slot_essential_record->numberOfReels (reel_set) == 0)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reelIsHeld (), Called before essential record is initialized."));
   return (slot_essential_record->reelState (reel_set, reel_index) == SlotEssentialRecord::HELD);
}

// Purpose:  Get the reel stop for the specified reel.
bool SlotPresentation::reelIsHeld (uint32 reel_index)
{
   return reelIsHeld (currentReelSet (), reel_index);
}

// Purpose: Get the reel stop on the current reel set for the provided stop index.
uint32 SlotPresentation::reelStop (uint32 reel_index)
{
   return reelStop (currentReelSet (), reel_index);
}

// Purpose: Return the reel stop for the reel set provided.
uint32 SlotPresentation::reelStop (uint32 reel_set, uint32 reel_index)
{
   return reelStopPhysical (reel_set, reel_index);
}

// Purpose: Return the physical reel stop for the reel set provided.
uint32 SlotPresentation::reelStopPhysical (uint32 reel_set, uint32 reel_index)
{
   const SlotSharedEssentialRecord *slot_shared_essential_record = getSlotSharedEssentialRecord ();
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();

   if (!slot_essential_record)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reelStopPhysical (), Called before essential record is initialized."));
   if (!slot_shared_essential_record)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reelStopPhysical (), Called before shared essential record is initialized."));

   uint32 reel_stop = 0;

   if (currentGameMode () == GAME_MODE_HISTORY || currentGameMode () == GAME_MODE_UTILITY ||
       (slot_shared_essential_record->physical_reel_stop.isPhysicalReelStopsInitialized () == false &&
        slot_essential_record->physical_reel_stop.isPhysicalReelStopsInitialized () == false))
   {
      Mojo<SlotEvaluatorInterface> slot_evaluator;
      slotEvaluatorInterface (slot_evaluator, true, reel_set);
      reel_stop = slot_evaluator->getPhysicalStop (reel_index, reelStopVirtual (reel_set, reel_index));
   }
   else
   {
      if (slot_essential_record->physical_reel_stop.isPhysicalReelStopsInitialized ())
         reel_stop = slot_essential_record->physical_reel_stop.physicalReelStop (reel_set, reel_index);
      else if (slot_shared_essential_record->physical_reel_stop.isPhysicalReelStopsInitialized ())
         reel_stop = slot_shared_essential_record->physical_reel_stop.physicalReelStop (reel_set, reel_index);
   }

   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::reelStopPhysical (" << reel_index << "):  " << reel_stop << endl;
   #endif

   return reel_stop;
}

// Get the virtual reel stop on the current reel set for the provided stop index.
uint32 SlotPresentation::reelStopVirtual (uint32 reel_index)
{
   return reelStopVirtual (currentReelSet (), reel_index);
}

// Purpose: Return the vurtual reel stop.
uint32 SlotPresentation::reelStopVirtual (uint32 reel_set, uint32 reel_index)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::reelStopVirtual (), Called before essential record is initialized.").errorMessage (), whoAreYou ());
      return 0;
   }

   // Check if reel set index is valid.
   if (!slot_essential_record->numberOfReels (reel_set))
   {
      setErrorString (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::reelStopVirtual (), Reel set index is invalid.").errorMessage (), whoAreYou ());
      return 0;
   }

   const SlotEssentialRecord::Reel * reel = slot_essential_record->reel (reel_set, reel_index);
   if (!reel)
   {
      setErrorString (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::reelStopVirtual (), reel is null.").errorMessage (), whoAreYou ());
      return 0;
   }

   uint32 reel_stop = reel->getVirtualStop ();

   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::reelStopVirtual (" << reel_index << "):  " << reel_stop << endl;
   #endif
   // Get the reel stop value for the given reel.
   return reel_stop;
}

// The following method assigns the new control panel to the game-specific control panel mojo as is
// appropriate for the control panel type name received.
bool SlotPresentation::registerControlPanel (const char *type, const char *alias)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::registerControlPanel, type:" << type << ", alias: " << alias << endl;
   #endif
   // Initialize the return value.
   bool registered = false;
   if (type)
   {
      if (strcmp (type, IGAME_CONTROL_PANEL) == 0)
      {
         igame_control_panel = alias;
         registered = true;
      }
   }

   // If the control panel was not registered by this game, allow the base class an opportunity to register it.
   if (!registered)
      registered = GamePresentation::registerControlPanel (type, alias);
   return registered;
}

// Purpose:  This method starts the credit rollup for the current game.
void SlotPresentation::rollupWin (RollupDefs::ROLLUP_MODE rollup_mode)
{
   #ifdef __VERBOSE_METERS__
      debug << "SlotPresentation::rollupWin (" << rollup_mode << ")" << endl;
   #endif

   Amount credit_meter_from (ZERO_AMOUNT);
   Amount credit_meter_to (ZERO_AMOUNT);
   Amount win_meter_from (ZERO_AMOUNT);
   Amount win_meter_to (ZERO_AMOUNT);
   Amount credits_in_bank (ZERO_AMOUNT);
   Amount credits_won_this_win (ZERO_AMOUNT);
   Amount credits_in_award (ZERO_AMOUNT);

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   // Enable the control panels known by this class.
   GameControlPanelExtractor slot_control_panel;
   if (GamePresentation::controlPanel (slot_control_panel))
   {
      // Get an interface to the control panel
      Mojo<GameControlPanelInterface> cp_interface = slot_control_panel.getGameControlPanelInterface ();

      creditDisplayAmount (credits_in_bank);
      extractWinForAllActs (credits_in_award);

      win_meter_to = credits_in_award;
      credit_meter_to = credits_in_bank + credits_in_award;

      // If it is a free act, include award list credits in the inital values
      if (processConditionIsActType ("FREE"))
      {
         local_award->totalDisplayableAmount (credits_won_this_win, evaluationTransactionId ());
         win_meter_from = win_meter_to - credits_won_this_win;
         credit_meter_from = credit_meter_to - credits_won_this_win;
      }
      // If we're in the base game and it's not a free act, do not include award list credits in the initial values
      else
      {
         credit_meter_from = credits_in_bank;
      }

      #ifdef __VERBOSE_METERS__
         // Update the meter values
         debug << "---------Meter Values--------" << endl;
         debug << "   credit_meter_from: " << credit_meter_from << endl;
         debug << "   credit_meter_to: " << credit_meter_to << endl;
         debug << "   win_meter_from: " << win_meter_from << endl;
         debug << "   win_meter_to: " << win_meter_to << endl;
         debug << "-----------------------------" << endl;
      #endif
      cp_interface->setCreditMeter(credit_meter_from, RollupDefs::SNAP_TO_AMOUNT);
      cp_interface->setWinMeter(win_meter_from, RollupDefs::SNAP_TO_AMOUNT);

      // If double up, don't add credits to the credit meter until after they've
      // decided if they're going to double up or not.
      if (!doubleUpPermitted() && !doubleUpEnabled())
         cp_interface->setCreditMeter(credit_meter_to, rollup_mode);

      cp_interface->setWinMeter(win_meter_to, rollup_mode);

      postEvent ("SynchronizeWinPaidMeter:ShowWin");
   }
}

// Purpose:  Set the press button to start bonus message. Part of the new 2006 style.
void SlotPresentation::setAndShowPressButtonToStartBonusMessage (void)
{
   #ifdef __VERBOSE_METERS__
      debug << "SlotPresentation::setAndShowPressButtonToStartBonusMessage ()" << endl;
   #endif
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->setTotalWinMessage (press_button_to_start_bonus_text);
      igame_control_panel.getIGameControlPanelInterface ()->showTotalWinMessage (true);
   }
}

// Purpose: Set whether enrollment state has started, clear in idle state.
void SlotPresentation::setEnrollmentStarted (bool enrollment_flag)
{
   enrollment_started = enrollment_flag;
}

// Purpose: Load the betting shared object.
bool SlotPresentation::setBetInformation (const char *so_filename, const char *object_name)
{
   bool success = GamePresentation::setBetInformation (so_filename, object_name);
   // Make sure the number of paylines is equal to the number of betting subbets
   if (bet.isValid () && slot_evaluator.isValid ())
   {
      if (slot_evaluator->getNumberOfLines() != 0)
      {
         if (bet->getNumberOfSubBets () != slot_evaluator->getNumberOfLines ())
         {
            char error_message [512];
            SNPRINTF (error_message, sizeof (error_message), "ERROR: Paytable has %lu lines and the bet has %lu sub bets", static_cast <uint32>(slot_evaluator->getNumberOfLines ()), static_cast<uint32>(bet->getNumberOfSubBets ()));
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
      }
   }

   return success;
}

// Purpose:  set the property that holds the bet info.
void SlotPresentation::setBetMessagePropertyString (bool _append_good_luck)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::setBetMessagePropertyString (append_good_luck=" << _append_good_luck << ")" << endl;
   #endif
   Mojo<PropertyInterface> property;
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;
   assertion (resourceList (resource_list));

   // Only proceed if we have record and resource and the win amount is 0
   if (resource_list->getResource<PropertyInterface> (BET_MESSAGE_PROPERTY_NAME, resource_list->resourceKey (), property, false))
   {
      String bet_message_string ("Playing ");

      if (baseGamePresentationEvaluation ())
      {
         if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setBetMessagePropertyString(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
            return;
         }

         int32 number_of_lines_selected (numberOfSelectedPaylines ());
         int32 number_of_ways (game_side_evaluator->getNumberOfMultiWaysBet (bet));
         String line_bet_string;
         //make sure we have a valid bet
         GamePresentationHelper::formatAmountStringByConfiguration (bet->getSubBetAmount (0), denomination, line_bet_string);

         //setup for games with lines and multiway
         if (number_of_lines_selected != 0  && number_of_ways != 0)
         {
            //format to say what your string wants to do for multiway and payline games
         }
         else  //normal line or multiway games
         {
            if (number_of_lines_selected != 0)  //lines
            {
               bet_message_string << number_of_lines_selected << (number_of_lines_selected == 1 ? " line" : " lines");
            }
            else if (number_of_ways != 0)  //multiway
            {
               bet_message_string << number_of_ways << (number_of_ways == 1 ? " way" : " ways");
            }
            else
            {
               //set the string up to display nothing
               bet_message_string.clear ();
            }
         }

         // If a bonus game AND there is a bonus bet, append the bonus tag
         if (bonusBetTypeGame () == true)
         {
            Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");
            if (bonus_bet_amount > ZERO_AMOUNT)
            {
               bet_message_string << " + Bonus";
            }
         }

         ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
         int32 number_of_lines_interval (config.getValue (config_id));

         // use bet multiplier if the lines per credit is greater than 1
         // or if it is a multiway game
         if ((number_of_lines_interval > 1 || number_of_ways) &&  (number_of_lines_selected))
         {
            //format to say what your string wants to do for multiway and payline games
         }
         else  //normal line or multiway games
         {
            if (number_of_lines_interval > 1 || number_of_ways)
            {
               bet_message_string << " with a bet multiplier of ";
               bet_message_string << line_bet_string;
            }
            else if (number_of_lines_selected)
            {
               bet_message_string << " at " << line_bet_string << 
                  ((GamePresentationHelper::allMeterDisplayBehavior () == GamePresentationHelper::METER_DISPLAY_BEHAVIOR_DEFAULT) ? (line_bet_string == "1" ? " credit " : " credits ") : " ") << "per line";
            }
            else
            {
               //set the string up to display nothing
               bet_message_string.clear ();
            }
         }
      }
      else
      {
         Amount credits_bet (ZERO_AMOUNT);
         totalBetDisplayAmount (credits_bet);
         String credits_bet_string;

         GamePresentationHelper::formatAmountStringByConfiguration (credits_bet, getDenomination (), credits_bet_string);
         bet_message_string << credits_bet_string;
         if (GamePresentationHelper::allMeterDisplayBehavior () == GamePresentationHelper::METER_DISPLAY_BEHAVIOR_DEFAULT)
            bet_message_string << " credits";
      }

      if (_append_good_luck)
      {
         bet_message_string << "...GOOD LUCK!";
      }

      #ifdef __VERBOSE__
      debug << BET_MESSAGE_PROPERTY_NAME << " Value: " << bet_message_string << endl;
      #endif

      property->setAbsoluteValue (bet_message_string);
   }
}

// Purpose: Show the bet message during history and utility states.
//  This uses the game over text so it can be backward compatible with all games.
void SlotPresentation::showBetMessageInHistoryAndUtilityStates (void)
{
   SlotPresentation::IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // Update the bet message property string, so we can show it in the game over message.
      setBetMessagePropertyString (false);
      // Update the game over text display.
      igame_control_panel.getIGameControlPanelInterface ()->showGameOverMessage (true);
      // Display the bet message in history using the "Game Over" text actor
      postEvent ("ControlPanel:HistoryAndUtilityBetMessage");
   }
}

// Purpose: Show or hide the prompt that is displayed in the meter panel.
void SlotPresentation::showBetMessageMeterPanel (bool show, bool _append_good_luck)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::showBetMessageMeterPanel (show=" << show << ")" << endl;
   #endif

   setBetMessagePropertyString (_append_good_luck);

   Mojo<PropertyInterface> property;
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;

   if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::showBetMessageMeterPanel(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
      return;
   }

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Only proceed if we have record and resource
   assertion (resourceList (resource_list));
   if (!resource_list->getResource<PropertyInterface> (BET_MESSAGE_PROPERTY_NAME, resource_list->resourceKey (), property, false))
   {
      show = false;
   }
   //make sure we have a valid bet
   if (numberOfSelectedSubBetsIsZero ())
   {
      show = false;
   }
   if (show)
   {
      // Don't show the bet message if the game pays message is being displayed.
      Amount total_win (ZERO_AMOUNT);
      if (canDisplayAward ())
      {
         extractTotalWinFromAward (total_win);
      }

      if (total_win == ZERO_AMOUNT)
      {
         // Hide the game pays message in case it is showing
         displayTotalWinMessage (false);

         // Do not update on a double up loss, we need to show "Double Up Complete"
         // Do not update when award is available, when a new game is started canDisplayAward ()
         // should return false and we should then update the bet Message.
         if (!checkDoubleUpLoss () || !canDisplayAward ())
            postEvent ("UpdateBetMessage:MeterPanel");
      }
   }
   // if the property exists but we don't want to show it
   else if (resourceList (resource_list)
      && resource_list->getResource<PropertyInterface> (BET_MESSAGE_PROPERTY_NAME, resource_list->resourceKey (), property, false))
   {
      postEvent ("HideBetMessage:MeterPanel");
   }
}

// Purpose: Show the prompt that is displayed in the slider menu.
void SlotPresentation::showBetMessagePlayerMenu (bool show)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::showBetMessagePlayerMenu (show=" << show << ")" << endl;
   #endif

   // Set up the Bet Message property without appending Good Luck
   setBetMessagePropertyString (false);

   Mojo<PropertyInterface> property;
   Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;

   if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
   {
     setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::showBetMessageMeterPanel(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
     return;
   }
   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;
   assertion (resourceList (resource_list));

   // Only proceed if we have record and resource
   if (!resource_list->getResource<PropertyInterface> (BET_MESSAGE_PROPERTY_NAME, resource_list->resourceKey (), property, false))
   {
      show = false;
   }
   //make sure we have a valid bet
   if (numberOfSelectedSubBetsIsZero ())
   {
      show = false;
   }
   if (show)
   {
      postEvent ("UpdateBetMessage:PlayerMenu");
   }
   else
   {
      postEvent ("HideBetMessage:PlayerMenu");
   }
}

// Purpose:  set the property that holds the bonus bet info.
void SlotPresentation::setBonusBetMessagePropertyString (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::setBonusBetMessagePropertyString ()" << endl;
   #endif
   Mojo<PropertyInterface> property;

   if (bonusBetTypeGame() == true)
   {
      // Retrieve the resource list.
      Mojo<ResourceList, ScriptInterface> resource_list;
      assertion (resourceList (resource_list));

      if (resource_list->getResource<PropertyInterface> ("NumberOfBonusBetCredits", resource_list->resourceKey (), property, false))
      {
         // Get the current Bonus Bet Amount and convert it to a string for display purposes
         String bonus_bet_credit_string;
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");
         int32 bonus_bet = Amount::Extractor (bonus_bet_amount).extract (getDenomination());

         bonus_bet_credit_string << "BONUS BET = " <<  bonus_bet << " Credits";
         property->setAbsoluteValue (bonus_bet_credit_string);
      }
   }
}

// Purpose: Show the prompt that is displayed in the game player menu.
void SlotPresentation::showBonusBetMessagePlayerMenu (bool show)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::showBonusBetMessagePlayerMenu (show=" << show << ")" << endl;
   #endif

   // The Bonus Bet Message is only relevant to Bonus Type games
   if (bonusBetTypeGame() == true)
   {

      // See if the bonus bet is active before going throught the work of displaying it.
      Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");
      int32 bonus_bet = Amount::Extractor (bonus_bet_amount).extract (getDenomination());

      if (bonus_bet != 0)
      {
         Mojo<PropertyInterface> property;

         // Retrieve the resource list.
         Mojo<ResourceList, ScriptInterface> resource_list;
         assertion (resourceList (resource_list));

         // Only proceed if we have record and resource
         if (resource_list->getResource<PropertyInterface> ("NumberOfBonusBetCredits", resource_list->resourceKey (), property, false))
         {
            // Set up the Bonus Bet Message property
            setBonusBetMessagePropertyString ();
         }
         else
         {
            // Override the passed in show variable. The resource is not configured, don't try and show
            show = false;
         }

         // Have the Bonus Bet Button indicate that the Bonus Bet is active
         postEvent ("BonusBetButton:PlayerMenu:On");
      }
      else
      {
         // Override the passed in show variable. The bonus bet is not active, do not display/show message
         show = false;
         // Have the Bonus Bet Button indicate that the Bonus Bet is not active
         postEvent ("BonusBetButton:PlayerMenu:Off");
      }

      // Show or Hide the Bonus Bet message
      if (show)
      {
         // Show the Bonus Bet message
         postEvent ("UpdateBonusBetMessage:PlayerMenu");
      }
      else
      {
         // Don't show the Bonus Bet message
         postEvent ("HideBetMessage:PlayerMenu");
      }

      // We always want this button to be displayed for either Bonus Bet active or inactive
      postEvent ("ShowBonusBetDoubleUp:PlayerMenu");

   }
   else
   {
      // This game does not support Bonus Bet, hide the Bonus Bet related display objects
      postEvent ("HideBetMessage:PlayerMenu");
      postEvent ("HideBonusBetDoubleUp:PlayerMenu");
   }
}


// Purpose: Called to configure the denomination of all known control panels.
void SlotPresentation::setControlPanelDenomination (const Denomination& denomination)
{
   // Call the base class first.
   GamePresentation::setControlPanelDenomination (denomination);

   // Retrieve the game control panel and update the denomination if the control panel exists in this game.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
      igame_control_panel.getControlPanelInterface ()->setDenomination (denomination);
}

// Set the current paytable section
bool SlotPresentation::setCurrentPaytableSection (const char *section)
{
   // Call the base class first.
   bool success = GamePresentation::setCurrentPaytableSection (section);
   // If the base class successfully updated the paytable section, perform all actions necessary to update the
   // section that this game's level.
   if (success)
   {
      // Update the slot evaluator.
      Mojo<SlotEvaluatorInterface> slot_evaluator;
      slotEvaluatorInterface (slot_evaluator);
      if (!slot_evaluator.isValid ())
      {
         String error_message ("SlotPresentation::setCurrentPaytableSection, section: ");
         error_message << section << " is not a valid slot evaluator.";
         System::error (LogicError (LogicError::INVALID_VALUE, error_message));
      }
   }
   return success;
}

// Purpose: Set image list alias for dynamic slot bet buttons
void SlotPresentation::setDynamicSlotBetButtonAlias (const String &alias)
{
   #ifdef __VERBOSE_DYNAMIC_BUTTONS__
   debug << "SlotPresentation::setDynamicSlotBetButtonAlias (" << alias << ")" << endl;
   #endif

   dynamic_slot_bet_button_alias = alias;
}

// Purpose: Inform the game about receiving central determination RNG seeds.
bool SlotPresentation::setGameEnrollmentSeedsArrived (void)
{
   if (okayToShowDrawMessages ())
   {
      postEvent ("ControlPanel:DrawReceived");
   }
   return GamePresentation::setGameEnrollmentSeedsArrived ();
}

// Purpose: Set the id of the button that may need to be enabled to support CDS double-tap feature.
void SlotPresentation::setEnrollmentButtonDoubleTapButtonId (EnrollDblTapButtonId button_id)
{
   enrollment_double_tap_button_id = button_id;
}

// Purpose:  Set the event to be sent out to hide multi-way segement mask.
void SlotPresentation::setMultiWayHideSegmentMaskEvent (String event)
{
   hide_multi_way_segment_mask_event = event;
}

// Purpose:  Set the event to be sent out to show multi-way segement mask.
void SlotPresentation::setMultiWayShowSegmentMaskEvent (String event)
{
   show_multi_way_segment_mask_event = event;
}

// Purpose:  Sets the okay_to_hide_spin_button to the boolean received.
void SlotPresentation::setOkayToHideSpinButton (bool released)
{
   okay_to_hide_spin_button = released;
}

/// <summary>
/// # Bonus string updates will not force a digital glass refresh until set to true.
/// </summary>
/// <example><b>Example:</b>
/// @code
/// this:setOkayToUpdateBonusString (true)
/// @endcode
/// </example>
// Purpose:  Sets the okay_to_update_bonus_string to the boolean received.
void SlotPresentation::setOkayToUpdateBonusString (bool ready)
{
   okay_to_update_bonus_string = ready;
}

void SlotPresentation::setPaylinesPerCreditProperty (void)
{
   Mojo<PropertyInterface> temp_property;
   Mojo<ResourceList, ScriptInterface> resource_list;

   String number_of_lines_bet_string;
   uint32 number_of_lines_bet;
   uint32 number_of_sub_bets;
   uint32 number_of_credits_bet;
   String number_of_credits_bet_string;

   // once we have this event, then get the number of lines
   if (resourceList (resource_list))
   {
      if (resource_list->getResource<PropertyInterface> ("NumberOfLinesBetSlider-Value", resource_list->resourceKey (), temp_property, false))
      {
         temp_property->getAbsoluteValue (number_of_lines_bet_string);
      }
   }

   // get the number of subbets
   ConfigID config_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
   number_of_sub_bets = config.getValue (config_id);

   // do the math, and set a property with the amount as a string
   number_of_lines_bet = (uint32) atoi (number_of_lines_bet_string);
   number_of_credits_bet = number_of_lines_bet / number_of_sub_bets;
   number_of_credits_bet_string = "";
   number_of_credits_bet_string << number_of_credits_bet;

   #ifdef __VERBOSE__
      debug <<  "       NumberOfLinesBetSlider:UpdateHandleText" << endl;
      debug <<  "       number_of_lines_bet_string: "            << number_of_lines_bet_string << endl;
      debug <<  "       number_of_sub_bets: "                    << number_of_sub_bets << endl;
      debug <<  "       number_of_lines_bet: "                   << number_of_lines_bet << endl;
      debug <<  "       number_of_credits_bet: "                 << number_of_credits_bet << endl;
      debug <<  "       number_of_credits_bet_string: "          << number_of_credits_bet_string << endl;
   #endif

   if (resource_list->getResource<PropertyInterface> ("NumberOfCreditsBet", resource_list->resourceKey (), temp_property, false))
   {
      temp_property->setAbsoluteValue (number_of_credits_bet_string);
   }

   // send out an update event to show the new value.
   postEventAll ("Update:NumberOfCreditsBet");
   if (number_of_credits_bet == 1)
   {
      postEventAll ("Update:NumberOfCreditsBet:Credit");
   }
   else if (number_of_credits_bet > 1)
   {
      postEventAll ("Update:NumberOfCreditsBet:Credits");
   }
}

// Purpose:  Set the multiway symbol that will have an overlay.
void SlotPresentation::setMultiWayOverlaySymbolName (ScriptParameterList &parameter_list)
{
   for (uint16 param_index = 0; param_index < parameter_list.getParameterCount (); ++param_index)
   {
      multiway_overlay_symbol_name.push_back (parameter_list [param_index].asString());
   }
}

// Purpose:  display the wild overlays on the selected payline.
void SlotPresentation::setPaylineOverlaySymbolName (ScriptParameterList &parameter_list)
{
   for (uint16 param_index = 0; param_index < parameter_list.getParameterCount (); ++param_index)
   {
      payline_overlay_symbol_name.push_back (parameter_list [param_index].asString());
   }
}

// Purpose: Sets the parameters of the default rollup rule.
void SlotPresentation::setDefaultRollupRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index, uint32 celebration_bet_threshold)
{
  rollup_rule_default.rollup_data_start_index = rollup_data_start_index;
  rollup_rule_default.rollup_data_end_index = rollup_data_end_index;
  rollup_rule_default.celebration_bet_threshold = celebration_bet_threshold;
}

// Purpose: Creates a RollupRule based on the passed criteria.
void SlotPresentation::setRollupRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index,
                                      uint32 celebration_bet_threshold, RollupCompareTarget compare_target,
                                      RollupCriteria rollup_criteria, uint32 compare_value)
{
   // Sanity check
   if (rollup_data_end_index >= rollup_data_vector.size ())
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setRollupRule (), end index exceeds number of RollupModes defined"));
   if (rollup_data_start_index > rollup_data_end_index)
         System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setRollupRule (), start index must be less than end index"));

   RollupRule rollup_rule;
   rollup_rule.rollup_data_start_index = rollup_data_start_index;
   rollup_rule.rollup_data_end_index = rollup_data_end_index;
   rollup_rule.celebration_bet_threshold = celebration_bet_threshold;
   rollup_rule.rollup_criteria = rollup_criteria;
   rollup_rule.compare_target = compare_target;
   rollup_rule.compare_value = compare_value;
   rollup_rule_vector.push_back (rollup_rule);
}

// Purpose: Takes a rollup rule and determines whether the criteria pass or fail the current conditions.
bool SlotPresentation::processRollupRule (const RollupRule &rollup_rule)
{
   bool can_process (false);
   bool can_evaluate (true);
   uint32 lvalue = 0;
   uint32 compare_value = rollup_rule.compare_value;
   switch (rollup_rule.compare_target)
   {
      case ROLLUP_COMPARE_MAX_BET_PER_LINE:
         lvalue = bet_per_line_max_value;
         break;

      case ROLLUP_COMPARE_DENOM:
         lvalue = denomination.getValue ();
         break;

      default:
         can_evaluate = false;
         break;
   }

   if (can_evaluate)
   {
      switch (rollup_rule.rollup_criteria)
      {
         case ROLLUP_CRITERIA_LESS_THAN:
            can_process = (lvalue < compare_value);
            break;

         case ROLLUP_CRITERIA_LESS_THAN_OR_EQUALS:
            can_process = (lvalue <= compare_value);
            break;

         case ROLLUP_CRITERIA_GREATER_THAN:
            can_process = (lvalue > compare_value);
            break;

         case ROLLUP_CRITERIA_GREATER_THAN_OR_EQUALS:
            can_process = (lvalue >= compare_value);
            break;

         case ROLLUP_CRITERIA_EQUALS:
            can_process = (lvalue == compare_value);
            break;

         default:
         break;
      }
   }

   return can_process;
}

// Purpose: Compare the data in the "RollupData" vector to the bet and credits won to determine how to rollup.
// By default, this function is passed the current resource key.  However, if this function is being called in
// a bonus, we must pass it the key to the base game so that it can access the property (that lives in the base
// game) that holds the loop count of the fireworks.
void SlotPresentation::calculateRollupThreshold (String resource_key)
{
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
      postEvent (event);

      // Append the specific threshold index that was found (1 based).
      if (single_threshold_found)
         event << ":SingleRollupThreshold";
      else
         event << ":" << current_rollup_threshold;
      postEvent (event);

      if (rollup_loop_count > 0)
      {
         postEventAll ("Enable:Celebration");
      }
   }
   else
   {
      // Inform the presentation that a threshold was NOT found.
      postEvent ("RollupThresholdNotFound");
   }
}

void SlotPresentation::initializeCelebrationMode (String mode)
{
   // Clear any previous initializations
   rollup_data_vector.clear ();

   celebration_mode = mode;
   single_credit_rollup_threshold = 0;
   // Set up the values for the 2006 standards
   if (celebration_mode == CELEBRATION_MODE_2006_STANDARDS)
   {
      // Reset the size of the vector
      rollup_data_vector.resize (16);

      // Index 0
      rollup_data_vector[0].win_to_bet_ratio_low_range   = 0.0f;
      rollup_data_vector[0].win_to_bet_ratio_high_range  = 0.2f;
      rollup_data_vector[0].low_range                    = 0;
      rollup_data_vector[0].high_range                   = 10;
      rollup_data_vector[0].celebration_loop_count       = 0;

      // Index 1
      rollup_data_vector[1].win_to_bet_ratio_low_range   = 0.2f;
      rollup_data_vector[1].win_to_bet_ratio_high_range  = 0.4f;
      rollup_data_vector[1].low_range                    = 10;
      rollup_data_vector[1].high_range                   = 20;
      rollup_data_vector[1].celebration_loop_count       = 0;

      // Index 2
      rollup_data_vector[2].win_to_bet_ratio_low_range   = 0.4f;
      rollup_data_vector[2].win_to_bet_ratio_high_range  = 0.6f;
      rollup_data_vector[2].low_range                    = 20;
      rollup_data_vector[2].high_range                   = 30;
      rollup_data_vector[2].celebration_loop_count       = 0;

      // Index 3
      rollup_data_vector[3].win_to_bet_ratio_low_range   = 0.6f;
      rollup_data_vector[3].win_to_bet_ratio_high_range  = 0.8f;
      rollup_data_vector[3].low_range                    = 30;
      rollup_data_vector[3].high_range                   = 40;
      rollup_data_vector[3].celebration_loop_count       = 0;

      // Index 4
      rollup_data_vector[4].win_to_bet_ratio_low_range   = 0.8f;
      rollup_data_vector[4].win_to_bet_ratio_high_range  = 1.0f;
      rollup_data_vector[4].low_range                    = 40;
      rollup_data_vector[4].high_range                   = 60;
      rollup_data_vector[4].celebration_loop_count       = 0;

      // Index 5
      rollup_data_vector[5].win_to_bet_ratio_low_range   = 1.0f;
      rollup_data_vector[5].win_to_bet_ratio_high_range  = 1.5f;
      rollup_data_vector[5].low_range                    = 60;
      rollup_data_vector[5].high_range                   = 80;
      rollup_data_vector[5].celebration_loop_count       = 0;

      // Index 6
      rollup_data_vector[6].win_to_bet_ratio_low_range   = 1.5f;
      rollup_data_vector[6].win_to_bet_ratio_high_range  = 2.0f;
      rollup_data_vector[6].low_range                    = 80;
      rollup_data_vector[6].high_range                   = 100;
      rollup_data_vector[6].celebration_loop_count       = 0;

      // Index 7
      rollup_data_vector[7].win_to_bet_ratio_low_range   = 2.0f;
      rollup_data_vector[7].win_to_bet_ratio_high_range  = 3.0f;
      rollup_data_vector[7].low_range                    = 100;
      rollup_data_vector[7].high_range                   = 150;
      rollup_data_vector[7].celebration_loop_count       = 0;

      // Index 8
      rollup_data_vector[8].win_to_bet_ratio_low_range   = 3.0f;
      rollup_data_vector[8].win_to_bet_ratio_high_range  = 4.0f;
      rollup_data_vector[8].low_range                    = 150;
      rollup_data_vector[8].high_range                   = 200;
      rollup_data_vector[8].celebration_loop_count       = 0;

      // Index 9
      rollup_data_vector[9].win_to_bet_ratio_low_range   = 4.0f;
      rollup_data_vector[9].win_to_bet_ratio_high_range  = 5.0f;
      rollup_data_vector[9].low_range                    = 200;
      rollup_data_vector[9].high_range                   = 300;
      rollup_data_vector[9].celebration_loop_count       = 0;

      // Index 10
      rollup_data_vector[10].win_to_bet_ratio_low_range   = 5.0f;
      rollup_data_vector[10].win_to_bet_ratio_high_range  = 9.99f;
      rollup_data_vector[10].low_range                    = 300;
      rollup_data_vector[10].high_range                   = 499;
      rollup_data_vector[10].celebration_loop_count       = 0;

      // Index 11
      rollup_data_vector[11].win_to_bet_ratio_low_range   = 9.99f;
      rollup_data_vector[11].win_to_bet_ratio_high_range  = 20.0f;
      rollup_data_vector[11].low_range                    = 499;
      rollup_data_vector[11].high_range                   = 1000;
      rollup_data_vector[11].celebration_loop_count       = 1;

      // Index 12
      rollup_data_vector[12].win_to_bet_ratio_low_range   = 20.0f;
      rollup_data_vector[12].win_to_bet_ratio_high_range  = 30.0f;
      rollup_data_vector[12].low_range                    = 1000;
      rollup_data_vector[12].high_range                   = 2000;
      rollup_data_vector[12].celebration_loop_count       = 2;

      // Index 13
      rollup_data_vector[13].win_to_bet_ratio_low_range   = 30.0f;
      rollup_data_vector[13].win_to_bet_ratio_high_range  = 50.0f;
      rollup_data_vector[13].low_range                    = 2000;
      rollup_data_vector[13].high_range                   = 3000;
      rollup_data_vector[13].celebration_loop_count       = 2;

      // Index 14
      rollup_data_vector[14].win_to_bet_ratio_low_range   = 50.0f;
      rollup_data_vector[14].win_to_bet_ratio_high_range  = 100.0f;
      rollup_data_vector[14].low_range                    = 3000;
      rollup_data_vector[14].high_range                   = 5000;
      rollup_data_vector[14].celebration_loop_count       = 2;

      // Index 15
      rollup_data_vector[15].win_to_bet_ratio_low_range   = 100.0f;
      rollup_data_vector[15].win_to_bet_ratio_high_range  = _FLOAT_MAX;
      rollup_data_vector[15].low_range                    = 5000;
      rollup_data_vector[15].high_range                   = Amount::MAX_ABSOLUTE_AMOUNT_VALUE; // Max AmountValue
      rollup_data_vector[15].celebration_loop_count       = 2;

      // Assign the event_threshold_index to be posted to each item.
      for (uint32 i = 0; i < rollup_data_vector.size(); i++)
      {
         rollup_data_vector[i].event_threshold_index = i + 1;
      }

      // Set the default rollup rule to encompass all the rollup_data
      setDefaultRollupRule (0, _UINT32_MAX, 60);
   }
   else if (celebration_mode == CELEBRATION_MODE_2007_STANDARDS)
   {
      // Reset the size of the vector
      rollup_data_vector.resize (16);

      // Index 0
      rollup_data_vector[0].win_to_bet_ratio_low_range   = 0.0f;
      rollup_data_vector[0].win_to_bet_ratio_high_range  = 0.2f;
      rollup_data_vector[0].low_range                    = 0;
      rollup_data_vector[0].high_range                   = 10;
      rollup_data_vector[0].celebration_loop_count       = 0;

      // Index 1
      rollup_data_vector[1].win_to_bet_ratio_low_range   = 0.2f;
      rollup_data_vector[1].win_to_bet_ratio_high_range  = 0.4f;
      rollup_data_vector[1].low_range                    = 10;
      rollup_data_vector[1].high_range                   = 20;
      rollup_data_vector[1].celebration_loop_count       = 0;

      // Index 2
      rollup_data_vector[2].win_to_bet_ratio_low_range   = 0.4f;
      rollup_data_vector[2].win_to_bet_ratio_high_range  = 0.6f;
      rollup_data_vector[2].low_range                    = 20;
      rollup_data_vector[2].high_range                   = 30;
      rollup_data_vector[2].celebration_loop_count       = 0;

      // Index 3
      rollup_data_vector[3].win_to_bet_ratio_low_range   = 0.6f;
      rollup_data_vector[3].win_to_bet_ratio_high_range  = 0.8f;
      rollup_data_vector[3].low_range                    = 30;
      rollup_data_vector[3].high_range                   = 40;
      rollup_data_vector[3].celebration_loop_count       = 0;

      // Index 4
      rollup_data_vector[4].win_to_bet_ratio_low_range   = 0.8f;
      rollup_data_vector[4].win_to_bet_ratio_high_range  = 1.0f;
      rollup_data_vector[4].low_range                    = 40;
      rollup_data_vector[4].high_range                   = 60;
      rollup_data_vector[4].celebration_loop_count       = 0;

      // Index 5
      rollup_data_vector[5].win_to_bet_ratio_low_range   = 1.0f;
      rollup_data_vector[5].win_to_bet_ratio_high_range  = 1.5f;
      rollup_data_vector[5].low_range                    = 60;
      rollup_data_vector[5].high_range                   = 80;
      rollup_data_vector[5].celebration_loop_count       = 0;

      // Index 6
      rollup_data_vector[6].win_to_bet_ratio_low_range   = 1.5f;
      rollup_data_vector[6].win_to_bet_ratio_high_range  = 2.0f;
      rollup_data_vector[6].low_range                    = 80;
      rollup_data_vector[6].high_range                   = 100;
      rollup_data_vector[6].celebration_loop_count       = 0;

      // Index 7
      rollup_data_vector[7].win_to_bet_ratio_low_range   = 2.0f;
      rollup_data_vector[7].win_to_bet_ratio_high_range  = 3.0f;
      rollup_data_vector[7].low_range                    = 100;
      rollup_data_vector[7].high_range                   = 150;
      rollup_data_vector[7].celebration_loop_count       = 0;

      // Index 8
      rollup_data_vector[8].win_to_bet_ratio_low_range   = 3.0f;
      rollup_data_vector[8].win_to_bet_ratio_high_range  = 4.0f;
      rollup_data_vector[8].low_range                    = 150;
      rollup_data_vector[8].high_range                   = 200;
      rollup_data_vector[8].celebration_loop_count       = 0;

      // Index 9
      rollup_data_vector[9].win_to_bet_ratio_low_range   = 4.0f;
      rollup_data_vector[9].win_to_bet_ratio_high_range  = 5.0f;
      rollup_data_vector[9].low_range                    = 200;
      rollup_data_vector[9].high_range                   = 300;
      rollup_data_vector[9].celebration_loop_count       = 0;

      // Index 10
      rollup_data_vector[10].win_to_bet_ratio_low_range   = 5.0f;
      rollup_data_vector[10].win_to_bet_ratio_high_range  = 9.99f;
      rollup_data_vector[10].low_range                    = 300;
      rollup_data_vector[10].high_range                   = 499;
      rollup_data_vector[10].celebration_loop_count       = 0;

      // Index 11
      rollup_data_vector[11].win_to_bet_ratio_low_range   = 9.99f;
      rollup_data_vector[11].win_to_bet_ratio_high_range  = 20.0f;
      rollup_data_vector[11].low_range                    = 499;
      rollup_data_vector[11].high_range                   = 1000;
      rollup_data_vector[11].celebration_loop_count       = 1;

      // Index 12
      rollup_data_vector[12].win_to_bet_ratio_low_range   = 20.0f;
      rollup_data_vector[12].win_to_bet_ratio_high_range  = 30.0f;
      rollup_data_vector[12].low_range                    = 1000;
      rollup_data_vector[12].high_range                   = 2000;
      rollup_data_vector[12].celebration_loop_count       = 2;

      // Index 13
      rollup_data_vector[13].win_to_bet_ratio_low_range   = 30.0f;
      rollup_data_vector[13].win_to_bet_ratio_high_range  = 50.0f;
      rollup_data_vector[13].low_range                    = 2000;
      rollup_data_vector[13].high_range                   = 3000;
      rollup_data_vector[13].celebration_loop_count       = 2;

      // Index 14
      rollup_data_vector[14].win_to_bet_ratio_low_range   = 50.0f;
      rollup_data_vector[14].win_to_bet_ratio_high_range  = 100.0f;
      rollup_data_vector[14].low_range                    = 3000;
      rollup_data_vector[14].high_range                   = 5000;
      rollup_data_vector[14].celebration_loop_count       = 2;

      // Index 15
      rollup_data_vector[15].win_to_bet_ratio_low_range   = 100.0f;
      rollup_data_vector[15].win_to_bet_ratio_high_range  = _FLOAT_MAX;
      rollup_data_vector[15].low_range                    = 5000;
      rollup_data_vector[15].high_range                   = Amount::MAX_ABSOLUTE_AMOUNT_VALUE; // Max AmountValue
      rollup_data_vector[15].celebration_loop_count       = 2;

      // Assign the event_threshold_index to be posted to each item.
      for (uint32 i = 0; i < rollup_data_vector.size(); i++)
      {
         rollup_data_vector[i].event_threshold_index = i + 1;
      }

      // Set the default rollup rule to encompass all the rollup_data
      setDefaultRollupRule (0, _UINT32_MAX, 60);
   }
   else if (celebration_mode != CELEBRATION_MODE_CUSTOM)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::initializeCelebrationMode (), Invalid Celebration Mode").errorMessage (), whoAreYou ());
      // Once the error occurs, we need to alter the program flow to prevent any code from being executed since
      // the object state is no longer valid due to a fatal error occurring.The following return is to make sure
      // to stop program flow even if some people add any more codes in the future
      return;
   }
}

/// <summary>Scriptable function that sets the image to use when a dynamic button is blank.</summary>
/// <remarks>By default, this two parameter function will call the three parameter version and pass
/// it the current dynamic button alias (typically "GameButtonPanel").</remarks>
/// <param name = "button_name">[in] Defines the name of the button to target for replacement. Using a button_name of "Default" will target all
/// buttons that do not have blank images defined. Useful for substituting a logo.</param>
/// <param name = "image_index">[in] The dynamic button image index.</param>
/// <example><b>Example:</b>
/// @code
/// this:setDynamicButtonBlankImage ("PlayNLinesButton1", 2)
/// this:setDynamicButtonBlankImage ("Default", 39)
/// @endcode
/// </example>
/// <returns># void</returns>
/// @scriptableFunction{SlotPresentation,setDynamicButtonBlankImage,two_param}
void SlotPresentation::setDynamicButtonBlankImage (String button_name, uint32 image_index)
{
   setDynamicButtonBlankImage (button_name, image_index, dynamic_slot_bet_button_alias);
}

/// <summary>Scriptable function that sets the image to use when a dynamic button is blank.</summary>
/// <param name = "button_name">[in] Defines the name of the button to target for replacement. Using a button_name of "Default" will target all
/// buttons that do not have blank images defined. Useful for substituting a logo.</param>
/// <param name = "image_index">[in] The dynamic button image index.</param>
/// <param name = "resource_alias">[in] The dynamic button's resource alias.</param>
/// <example><b>Example:</b>
/// @code
/// this:setDynamicButtonBlankImage ("PlayNLinesButton1", 2)
/// this:setDynamicButtonBlankImage ("Default", 39)
/// this:setDynamicButtonBlankImage ("PlayNSubBetsButton2", 1, "Celebration")
/// @endcode
/// </example>
/// <returns># void</returns>
/// @scriptableFunction{SlotPresentation,setDynamicButtonBlankImage,three_param}
void SlotPresentation::setDynamicButtonBlankImage (String button_name, uint32 image_index, String resource_alias)
{
   dynamic_button_aux_data.dynamic_button_blank_images [button_name].image_index = image_index;
   dynamic_button_aux_data.dynamic_button_blank_images [button_name].alias = resource_alias;
}

// Purpose: Instantiate a "RollupData" struct and add it to the vector.
void SlotPresentation::setRollupData (float win_to_bet_ratio_low_range, float win_to_bet_ratio_high_range,
                                      Amount::AmountValue low_range, Amount::AmountValue high_range, int32 celebration_loop_count,
                                      uint32 event_threshold_index)
{
   // In order to set up the individual elements of the vector, you MUST be in CUSTOM mode.
   if (celebration_mode == CELEBRATION_MODE_CUSTOM)
   {
      RollupData rollup_data;
      rollup_data.win_to_bet_ratio_low_range = win_to_bet_ratio_low_range;
      rollup_data.win_to_bet_ratio_high_range = win_to_bet_ratio_high_range;
      rollup_data.low_range = low_range;
      rollup_data.high_range = high_range;
      rollup_data.celebration_loop_count = celebration_loop_count;
      // Event threshold index is 1 based. A value of 0 will mean that the index will be automatically assigned to be the current rollup data index.
      rollup_data.event_threshold_index = event_threshold_index ? event_threshold_index : rollup_data_vector.size() + 1;
      rollup_data_vector.push_back (rollup_data);
   }
   // If the mode is not CUSTOM, throw an error.
   else
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setRollupData (), Celebration mode must be CELEBRATION_MODE_CUSTOM").errorMessage (), whoAreYou ());
      // Once the error occurs, we need to alter the program flow to prevent any code from being executed since
      // the object state is no longer valid due to a fatal error occurring.The following return is to make sure
      // to stop program flow even if some people add any more codes in the future
      return;
   }
}

// Purpose:  display the bonus overlays on the selected scatter win.
void SlotPresentation::setScatterOverlaySymbolName (ScriptParameterList &parameter_list)
{
   for (uint16 param_index = 0; param_index < parameter_list.getParameterCount (); ++param_index)
   {
      scatter_overlay_symbol_name.push_back (parameter_list [param_index].asString());
   }
}

/// <summary># Sets the stop display text for all reels of current reel set so that it reflects the current stops.</summary>
void SlotPresentation::setStopDisplayText (void)
{
   setStopDisplayText (currentGameSetIndex ());
}

/// <summary># Sets the stop display text for all reels of the specific reel set so that it reflects the current stops.</summary>
/// <param name="reel_set"># [in] Specific reel set to set.</param>
void SlotPresentation::setStopDisplayText (uint32 reel_set)
{
   int32 from = 0;
   int32 to = 0;

   Mojo<ReelsInterface> reels;
   if (getReels (reels, reel_set))
   {
      // Get the current slot evaluator
      Mojo<SlotEvaluatorInterface> slot_evaluator;
      slotEvaluatorInterface (slot_evaluator, true, reel_set);

      // If the game is using video reels...
      Mojo<VideoReelsInterface> video_reels_interface (reels);

      // Translate the value into a string and configure the reels stop display actor.
      uint32 total_reels = numberOfReels (reel_set);

      ScriptTableParameter payload;
      ScriptTableParameter update_table;
      ScriptTableParameter reel_info;

      Mojo<ResourceInterface> reels_resource (reels);
      assertion (reels_resource.isValid ());
      String reel_set_name (reels_resource->resourceAlias ());

      // Preallocate six bytes to avoid reallocation when this string is assigned in the loop below.
      String stop_text (0, (size_t) 20);
      for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
      {
         if (current_game_mode == GAME_MODE_HISTORY)
         {
            slot_evaluator->getVirtualRangeFromPhysicalStop (reel_index, reelStop (reel_set, reel_index), from, to);
            stop_text.clear ();
            if (to != -1)
            {
               stop_text = "V:";
               stop_text << reelStopVirtual (reel_set, reel_index) + 1;
               stop_text << "\r\nP:" << slot_evaluator->getPhysicalStop (reel_index, reelStopVirtual (reel_set, reel_index)) + 1;
            }
            else
            {
               stop_text << "P:";
               stop_text << reelStop (reel_set, reel_index) + 1;
            }
         }
         else
         {
            // When stops are displayed on screen, they must be adjusted by one so that the values
            // displayed match the paperwork against which our pays and wins are verified.
            // Translate the value into a string and configure the reels stop display actor.
            stop_text = "P:";
            stop_text << reelStop (reel_set, reel_index) + 1;

            slot_evaluator->getVirtualRangeFromPhysicalStop (reel_index, reelStop (reel_set, reel_index), from, to);
            if (to != -1)
            {
               String string_from;
               String string_to;
               string_from.setInteger (from + 1);
               string_to.setInteger (to + 1);
               stop_text << "\r\nV:" << string_from << " To " << string_to;
            }
         }

         // Post event to set display text.
         reel_info.script_table.clear ();
         reel_info.script_table["reel_index"] = ScriptParameter (reel_index, ParameterType::UINT32);
         reel_info.script_table["stop_text"] = ScriptParameter (stop_text, ParameterType::STRING);

         String index_string = "";
         index_string << reel_index + 1;
         update_table.script_table [index_string.c_str ()] = ScriptParameter (reel_info, ParameterType::TABLE);
      }

      payload.script_table["reel_set_name"] = ScriptParameter (reel_set_name, ParameterType::STRING);
      payload.script_table["update"] = ScriptParameter (update_table, ParameterType::TABLE);
      // Post event to set display text.
      postEventExtended ("SetStopDisplayText", 0, &payload);
   }
}

// Purpose:  Set the (game pays) win message.
void SlotPresentation::setTotalWinMessage (void)
{
   #ifdef __VERBOSE_METERS__
   debug << "SlotPresentation::setTotalWinMessage ()" << endl;
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

// Purpose:  Set the (game pays) win message.
void SlotPresentation::setTotalWinMessage (const Amount &win_amount)
{
   #ifdef __VERBOSE_METERS__
   debug << "SlotPresentation::setTotalWinMessage (win_amount)" << endl;
   #endif
   // Setup a temp evaluator to get the number of win categories
   Mojo <EvaluatorInterface> temp_evaluator;
   temp_evaluator = slot_evaluator;
   bool display_in_currency = (GamePresentationHelper::allMeterDisplayBehavior () == GamePresentationHelper::METER_DISPLAY_BEHAVIOR_CURRENCY);
   Mojo<SlotAwardInterface> slot_award;
   WinCategoryId number_of_categories;

   Amount::Extractor amount_extractor (win_amount);
   Amount::AmountValue win_value = amount_extractor.extract (getDenomination ());

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

   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // If there is a win, display the total amount in the control panel.
      if (win_amount > ZERO_AMOUNT)
      {
         // Set the total game win message
         // Need to add additional check if the win was actually doubled'up. If so, change the win message
         String win_message = doubleUpWin () ? "Double Up Pays " : win_message_prefix;

         // This is a prelimiary implementation that should be reworked. The bonus manager
         // currently only supports multiplier type bonuses, but in the future could support
         // more.

         // Build the win amount string
         // Add any bonus manager adjustments
         String win_amount_string;
         uint32 award_multiplier = 0;
         if (extractBonusManagerMultiplierFromAward (award_multiplier))
            win_amount_string << "x " << award_multiplier << " Bonus = ";

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

         if (!isBingo ())
         {
            igame_control_panel.getIGameControlPanelInterface ()->setTotalWinMessage (win_message);
         }
         igame_control_panel.getIGameControlPanelInterface ()->showTotalWinMessage (true);

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
      }
      else
      {
         igame_control_panel.getIGameControlPanelInterface ()->showTotalWinMessage (false);
      }
   }
}

// Purpose: Show guiding arrows.
void SlotPresentation::showArrows (bool show_state)
{
   // Use Reels resource alias to scope Multimedia events to particular Reel object
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      Mojo<ResourceInterface> reels_resource (reels);
      assertion (reels_resource.isValid ());
      String reels_alias (reels_resource->resourceAlias ());

      uint32 total_reels = numberOfReels ();
      for (uint32 reel = 0; reel < total_reels; ++reel)
      {
         String arrows_event_string (reels_alias);
         arrows_event_string << ":Reel" << reel;
         if (show_state == true)
            arrows_event_string << ":ShowArrows";
         else
            arrows_event_string << ":HideArrows";

         postEvent (arrows_event_string);
      }
   }
}

// Purpose:  Called to enable all known control panels.
void SlotPresentation::showControlPanels (bool show)
{
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::showControlPanels ()" << endl;
   #endif
   // Call the base class first.
   GamePresentation::showControlPanels (show);
   // Show the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
      igame_control_panel.getControlPanelInterface ()->show (show);
}

// Purpose:  Show switch for current bet.
void SlotPresentation::showPlayXPerLineSwitch (bool okay_to_show)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "SlotPresentation::showPlayXPerLineSwitch ()" << endl;
   #endif
   // Call the base class first.
   // Synchronize the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
      igame_control_panel.getIGameControlPanelInterface ()->showPlayXPerLineSwitch (bet, okay_to_show);
}

// Purpose:  Show switch for current bet.
void SlotPresentation::showLinesSwitch (bool okay_to_show)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "SlotPresentation::showLinesSwitch ()" << endl;
   #endif
   // Call the base class first.
   // Synchronize the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
      igame_control_panel.getIGameControlPanelInterface ()->showLinesSwitch (bet, okay_to_show);
}

// Purpose: Show/hide the stop display text.
void SlotPresentation::showStopDisplayText (bool okay_to_show)
{
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // If the game is using video reels...
      Mojo<VideoReelsInterface> video_reels_interface (reels);

      uint32 total_reels = numberOfReels ();

      for (uint32 reel = 0; reel < total_reels; ++reel)
      {
         showStopDisplayText (reel, okay_to_show);
         // Otherwise allow the reels class to handle this event.
      }
   }
}

// Purpose: Show/hide the stop display text.
void SlotPresentation::showStopDisplayText (uint32 reel_index, bool okay_to_show)
{
   // Post event to show/hide stop_display_text.
   // Use Reels resource alias to scope Multimedia events to particular Reel object
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      Mojo<ResourceInterface> reels_resource (reels);
      assertion (reels_resource.isValid ());
      String reels_alias (reels_resource->resourceAlias ());

      String event_string (reels_alias);
      event_string << ":Reel" << reel_index << ":";
      if (okay_to_show)
      {
         event_string << "ShowStopDisplayText";
      }
      else
      {
         event_string << "HideStopDisplayText"; 
      }
      postEvent (event_string);
   }
}

// Purpose: Get the slot award interface if there is one for this game
bool SlotPresentation::slotAwardInterface (Mojo<SlotAwardInterface> &slot_award_interface)
{
   // The return value will only be set to true if a slot award can be extracted from the award_list.
   bool return_value = false;
   // Retrieve a reference to the current award information.
   Mojo<AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   // Make sure the award list has been initialized.
   if (local_award.isValid())
   {
      // Get the number of awards for this game
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      // If there was an award, get the slot interface
      if (number_of_awards)
      {
         // Extract the slot award interface from the system award interface.
         Mojo<SystemAwardInterface> system_award;
         assertion (local_award->getAward (evaluationTransactionId(), 0, system_award));
         // If the interface was extracted, update the slot award mojo.
         if (system_award.isValid ())
         {
            slot_award_interface = system_award;
            return_value = slot_award_interface.isValid();
         }
      }
   }
   return return_value;
}

// Get the slot paytable evaluator interface
bool SlotPresentation::slotEvaluatorInterface (Mojo<SlotEvaluatorInterface> &slot_evaluator_interface, bool fail_if_invalid, uint32 set_index)
{
   // Set the (global) slot_evaluator data member.
   slot_evaluator = evaluator;

   slot_evaluator_interface = slot_evaluator;

   // Fail if the slot evaluator assigned to the parameter received is not valid.
   if (fail_if_invalid)
      assertion (slot_evaluator_interface.isValid());

   return slot_evaluator_interface.isValid();
}

// Updates the game set parameter received with this game's configured game set.
bool SlotPresentation::slotGameSet (Mojo<SlotGameSetInterface> &slot_game_set, uint32 game_set_index)
{
   Mojo<GameSetInterface> game_set;
   if (gameSet (game_set, game_set_index))
      slot_game_set = game_set;
   return slot_game_set.isValid ();
}

// Get the nudge flag from the essential record.
uint8 SlotPresentation::getQueuedReels (void)
{
   uint8 queued_reels = 0x00;
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();

   queued_reels = slot_essential_record->getQueuedReels (current_reel_set_index);

   return queued_reels;
}

// Purpose:  Snap the reels to the stopping position.
void SlotPresentation::snapReelsToStopPosition (void)
{
   // Extract the reels from the game set.
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // Set each reel to the reel stop value stored in critical data.
      uint32 number_of_reels = numberOfReels ();
      if (isStandardReelsType (currentReelSet ()))
      {
         // If configured for "spin forever" during enrollment (e.g. NY style), and not yet 
         // out of the enroll state then the reels need to be stopped on cold start position.
         if ((enrollmentRequired () && 
              !enrollmentBeforeGameStart () &&
              (gameWasUnenrolled () ||
               getFlowState () == GameStateValue::STATE_ENROLL  ||
               getFlowState () == GameStateValue::STATE_ENROLL_ABORT)) || 
             !baseGamePresentationEvaluation ())
         {
            // Extract the current slot evaluator.
            Mojo<SlotEvaluatorInterface> slot_evaluator_interface;
            slotEvaluatorInterface (slot_evaluator_interface);
            for (uint32 reel = 0; reel < number_of_reels; ++reel)
            {
               // If Reels spin forever, they need to be told where to stop to before they can be stopped.
               uint32 physical_stop = 0;
               physical_stop = slot_evaluator->getPhysicalStop (reel,
                                                                slot_evaluator->getReelColdStartPosition (reel));
               reels->setToPosition (reel, physical_stop);
            }
         }
         else
         {
            for (uint32 reel = 0; reel < number_of_reels; ++reel)
            {
               reels->setToPosition (reel, reelStop (reel));
            }
         }
      }
      else
      {
         for (uint32 reel = 0; reel < number_of_reels; ++reel)
         {
            snapSegmentsToPosition (currentGameSetIndex (), reel);
         }
         postEventAll ("AltSymbolAction:Show");
      }
   }
}

// Purpose:  Start spinning the reels.  This method returns true if the reels were started.
bool SlotPresentation::spinHeldReels (bool all_reels_held)
{
   // Initialize the return value.
   bool spin_reels = false;
   // If the game has been configured to spin the reels if all reels are hedl, return true if the flag received
   // indicates that all reels are currently held.
   if ((all_reels_held && held_reel_spin_config == SPIN_REELS_IF_ALL_REELS_ARE_HELD) || held_reel_spin_config == SPIN_REELS_IF_REELS_ARE_HELD)
      spin_reels = true;
   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::spinHeldReels " << (int) spin_reels  << endl;
   #endif
   return spin_reels;
}

// Purpose: Spin the reels forever. Return true if the reels were started.
bool SlotPresentation::spinReelsForever (void)
{
   // Initialize the return value.
   bool reels_are_spinning = false;

   // Cache a reels mojo. Make sure it's valid.
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // Record the number of reels we think are spinning. We will decrement this
      // counter with each spin stop sequence event received from the reels.
      uint32 total_reels = numberOfReels ();

      // If the reels are not already spinning, spin them.
      if (!reels->isAnyReelSpinning () && total_reels)
      {
         // Figure out how many of the reels have been held.
         uint32 held_reel_count = 0;
         for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
         {
            if (reelIsHeld (reel_index))
            {
               held_reel_count++;
            }
         }

         // Figure out if held reels are supposed to spin.
         bool spin_held_reels = spinHeldReels (held_reel_count == total_reels);

         // Spin the reels.
         String multimedia_event;
         for (uint32 reel_index = 0; reel_index < total_reels; ++reel_index)
         {
            // If the reel has not been held, or if it has been held but it supposed to spin, spin it to its stop.
            if ((!reelIsHeld (reel_index)) || spin_held_reels)
            {
               // Post an event notifying that a locked reel is spinning.
               if (reelIsHeld (reel_index))
               {
                  multimedia_event = "ReelState:SpinLocked:Reel";
                  multimedia_event << reel_index;
                  postEvent (multimedia_event);
               }

               // Spin the reel forever.
               reels->spinForever (reel_index);

               // Update the return value only if at least one reel is spinning.
               reels_are_spinning = true;
            }
            // If the reel has been held, make certain that it is displaying the correct stop and post a MultimediaEvent.
            else
            {
               // Post an event notifying that this reel is not spinning but is locked.
               multimedia_event = "ReelState:Locked:Reel";
               multimedia_event << reel_index;
               postEvent (multimedia_event);

               reels->setToPosition (reel_index, reelStop (reel_index));
            }
         }
      }
   }
   return reels_are_spinning;
}

// Purpose: Return the name of the symbol at the given visible stop.
// Returns 0 for invalid visible stop index.
const char *SlotPresentation::symbolName (uint32 reel_set, uint32 reel, uint32 visible_stop_index)
{
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      setErrorString(LogicError (LogicError::INVALID_VALUE, "SlotPresentation::symbolName (), Called before essential record is initialized.").errorMessage (), whoAreYou ());
      return NULL;
   }

   // Get the paytable section name used during the last changing of the reel-set data (null terminated).
   const char *paytable_section = slot_essential_record->payTableSectionName (reel_set);
   if (!paytable_section)
   {
      setErrorString (LogicError (LogicError::INVALID_INDEX, "SlotPresentation::symbolName (), Reel set index has no paytable section configured.").errorMessage (), whoAreYou ());
      return NULL;
   }

   // Get the game evaluator for the given paytable section.
   Mojo<EvaluatorInterface> game_evaluator_mojo;
   if (!evaluatorForPaytableSection (game_evaluator_mojo, paytable_section))
   {
      String error;
      error = "SlotPresentation::symbolName (), '";
      error << paytable_section << "' section has no game evaluator configured.";
      setErrorString (LogicError (LogicError::INVALID_VALUE, error).errorMessage (), whoAreYou ());
      return NULL;
   }
   // Get a slot evaluator interface for the given paytable section.
   Mojo<SlotEvaluatorInterface> slot_evaluator_mojo;
   slot_evaluator_mojo = game_evaluator_mojo;
   if (!slot_evaluator_mojo.isValid ())
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::symbolName (), Paytable section\'s evaluator does not support slot games.").errorMessage (), whoAreYou( ));
      return NULL;
   }

   // Get the current reel stop value for the given reel.
   uint32 reel_stop = reelStopVirtual (reel_set, reel);
   // Return symbol name at the given offset.
   return slot_evaluator_mojo->getSymbolName (reel, reel_stop, visible_stop_index);
}

void SlotPresentation::snapPaylinesToClosestConfig (void)
{
   uint32 number_of_lines_selected = numberOfSelectedPaylines ();
   uint16 number_of_ways_selected = numberOfMultiWaysSelected ();
   uint16 number_of_active_sub_bets = bet->numberOfActiveSubBets ();
   uint32 search_value = (number_of_ways_selected ? number_of_active_sub_bets : number_of_lines_selected);

   vector<uint32> mech_payline_configs;

   ConfigID config_id_1 = config.getId ("MECHANICAL PAYLINE BUTTON 1", theme_configuration_identifier);
   ConfigID config_id_2 = config.getId ("MECHANICAL PAYLINE BUTTON 2", theme_configuration_identifier);
   ConfigID config_id_3 = config.getId ("MECHANICAL PAYLINE BUTTON 3", theme_configuration_identifier);
   ConfigID config_id_4 = config.getId ("MECHANICAL PAYLINE BUTTON 4", theme_configuration_identifier);
   ConfigID config_id_5 = config.getId ("MECHANICAL PAYLINE BUTTON 5", theme_configuration_identifier);

   mech_payline_configs.push_back (config.getValue (config_id_1));
   mech_payline_configs.push_back (config.getValue (config_id_2));
   mech_payline_configs.push_back (config.getValue (config_id_3));
   mech_payline_configs.push_back (config.getValue (config_id_4));
   mech_payline_configs.push_back (config.getValue (config_id_5));

   //Check to see if the current number of selected lines is one of the lockdown config values.
   if (find (mech_payline_configs.begin (),
           mech_payline_configs.end (),
           search_value) == mech_payline_configs.end ())
   {
      //This event string can be found in the slider sequence event translator script
      String new_event("Slider:NumberOfLinesBetSlider:Enabled:NewValueRequest:Position");
      new_event << mech_payline_configs.front ();
      postEvent (new_event);
   }
}

void SlotPresentation::snapBetPerLineToClosestConfig (void)
{
   if (bet.isValid ())
   {
      Amount::Extractor amount_extractor (bet->getSubBetAmount (0));
      uint32 line_bet = amount_extractor.extract (denomination);

      vector<uint32> mech_bet_per_line_configs;

      ConfigID config_id_1 = config.getId ("MECHANICAL BET PER LINE BUTTON 1", theme_configuration_identifier);
      ConfigID config_id_2 = config.getId ("MECHANICAL BET PER LINE BUTTON 2", theme_configuration_identifier);
      ConfigID config_id_3 = config.getId ("MECHANICAL BET PER LINE BUTTON 3", theme_configuration_identifier);
      ConfigID config_id_4 = config.getId ("MECHANICAL BET PER LINE BUTTON 4", theme_configuration_identifier);
      ConfigID config_id_5 = config.getId ("MECHANICAL BET PER LINE BUTTON 5", theme_configuration_identifier);

      mech_bet_per_line_configs.push_back (config.getValue (config_id_1));
      mech_bet_per_line_configs.push_back (config.getValue (config_id_2));
      mech_bet_per_line_configs.push_back (config.getValue (config_id_3));
      mech_bet_per_line_configs.push_back (config.getValue (config_id_4));
      mech_bet_per_line_configs.push_back (config.getValue (config_id_5));

      //Check to see if the current bet per line is one of the lockdown config values.
      if (find (mech_bet_per_line_configs.begin (),
                mech_bet_per_line_configs.end (),
                line_bet) == mech_bet_per_line_configs.end ())
      {
         //This event string can be found in the slider sequence event translator script
         String new_event = "Slider:BetPerLineSlider:Enabled:NewValueRequest:Position";
         new_event << mech_bet_per_line_configs.front ();
         postEvent (new_event);
      }
   }
}

void SlotPresentation::storeLastGameWinAmount (void)
{
   #ifdef __VERBOSE_METERS__
   debug << "SlotPresentation::storeLastGameWinAmount" << endl;
   #endif

   Amount current_total_win (ZERO_AMOUNT);
   Mojo <AwardInterface> local_award_list;
   readAwardFromEssentialRecord (local_award_list);
   local_award_list->totalDisplayableAmount (current_total_win);

   last_game_win_amount = current_total_win;
}

void SlotPresentation::clearLastGameWinAmount (void)
{
   #ifdef __VERBOSE_METERS__
   debug << "SlotPresentation::clearLastGameWinAmount" << endl;
   #endif

   last_game_win_amount = ZERO_AMOUNT;
   synchronizeWinAmount ();
}

/// <summary> # Make sure the reels are display at correct stops.</summary>
void SlotPresentation::synchronizeReels () 
{
   // Cache the current flow state.
   GameStateValue flow_state = getFlowState ();

   // Get the reels.
   Mojo<ReelsInterface> reels;
   if (getReels (reels, currentGameSetIndex ()))
   {
      // Updated this to prevent the presentation from momentarily halting the reels when
      // syncing up. Without this change, the reels will stutter when closing the main door
      // if they're already spinning.

      // If the game is operating in its play state, the reels might be spinning. In this case
      // we must not snap the reels.
      if (enrollmentRequired ())
      {
         if ((flow_state != GameStateValue::STATE_ENROLL &&
            flow_state != GameStateValue::STATE_ENROLL_ABORT &&
            flow_state != GameStateValue::STATE_PRESTART &&
            flow_state != GameStateValue::STATE_START &&
            flow_state != GameStateValue::STATE_PLAY) || !reels->isAnyReelSpinning ())
         {
            snapReelsToStopPosition ();
         }

      }
      else if ((flow_state != GameStateValue::STATE_PLAY) || !reels->isAnyReelSpinning ())
      {
         snapReelsToStopPosition ();
      }
   }
}

// Purpose: Set every actor in the presentation to a known state because the context just entered or its mode changed.
void SlotPresentation::synchronize (void)
{
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::synchronize" << endl;
   #endif

   // Call the base class.
   GamePresentation::synchronize ();
   updateGameThemeLicenseStatus();

   // Get the bet amount for this payline and extract the displayable value with the current denomination.
   // Get bet amounts for each payline.
   int32 number_of_lines_selected = numberOfSelectedPaylines ();
   uint16 number_of_ways_selected = numberOfMultiWaysSelected ();
   uint16 number_of_active_sub_bets = bet->numberOfActiveSubBets ();

   ConfigID config_id = config.getId ("LIMIT MECH PAYLINES TO BUTTON VALUES", theme_configuration_identifier, false);

   if (config_id != INVALID_CONFIGID && config.getValue (config_id)
       && (number_of_lines_selected || number_of_ways_selected) )
   {
      snapPaylinesToClosestConfig ();
   }

   config_id = config.getId ("LIMIT MECH BET PER LINES TO BUTTON VALUES", theme_configuration_identifier, false);
   if (config_id != INVALID_CONFIGID && config.getValue (config_id)
       && (number_of_lines_selected || number_of_ways_selected) )
   {
      snapBetPerLineToClosestConfig ();
   }

   // Make sure the reels are display at correct stops.
   synchronizeReels ();

   // Set control panel start buttons to their default settings.
   synchronizeStartButtons ();

   // Set the new card request button to its default settings.
   synchronizeNewCardButton ();

   // Retrieve the IGameControlPanel.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // If we're in the prestart state, take care of misc. buttons and the message bar.
      if (getFlowState () == GameStateValue::STATE_PRESTART)
      {
         igame_control_panel.getIGameControlPanelInterface ()->disableBettingButtons ();
         igame_control_panel.getIGameControlPanelInterface ()->disableLineSelectionButtons ();

         // Disable the game menu button
         enableGameMenuButtons (false);

         enableMoreGamesButton (false);

         // Hide the game pays win display.
         displayTotalWinMessage (false);
         // Update the good luck display.
         igame_control_panel.getIGameControlPanelInterface ()->showGoodLuckMessage (true);
         // Update the game over text display.
         igame_control_panel.getIGameControlPanelInterface ()->showGameOverMessage (false);

         if (enrollmentRequired () && okayToShowDrawMessages ())
         {
            postEvent ("ControlPanel:DrawReceived");
         }
         else if (okayToShowAutoplayMessage ())
         {
            postEvent ("ControlPanel:AutoPlay");
         }
      }
   }

   // Synchronize the scroll bars

   // 0 out the slider text areas if there are no paylines selected.
   if (number_of_lines_selected == 0
       && number_of_ways_selected == 0)
      postEvent ("NoLinesSelected");
   else
      postEvent ("ValidNumberOfLinesSelected");

   // Set the ranges of the sliders.
   initializeSliders ();

   // Synchronize the scroll bars.
   synchronizeBetPerLineSlider (bet->getSubBetAmount (0));

   // Synchronize the Select Lines Slider.
   if (number_of_lines_selected)
   {
      // If bonus bet type of game Synchronize the bonus bet
      if (bonusBetTypeGame() == true)
      {
         // Get the bonus bet
         Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

         // If the game is operating in its play state, the reels might be spinning. In this case
         // we must not snap the reels.
         if (getFlowState () != GameStateValue::STATE_START)
         {
            // Synchronixe the lines slider based on bonus bet enabled or disabled
            if (bonus_bet_amount <= ZERO_AMOUNT)
            {
               // set the slider value to the nuimber of lines selected
               synchronizeSelectLinesSlider (number_of_lines_selected);
               // Inform the presentation that bonus bet has been disabled
               postEvent ("BonusBetDisabled");
            }
            else
            {
               // set the slider value to the bonus bet enabled position
               synchronizeSelectLinesSlider (number_of_lines_selected+number_bonus_bet_levels);
               // update the slider handle text to indicate that its at bonus bet position
               updateSliderBonusPosition ();

               // Inform the presentation that bonus bet has been enabled
               postEvent ("BonusBetEnabled");

               // post the event for the current bonus bet value
               int32 bonus_bet = Amount::Extractor (bonus_bet_amount).extract (getDenomination ());
               int32 bonus_lines = bonus_bet / max_bonus_bet_multiplier;
               String bonus_bet_changed_event ("BonusBetChanged:");
               bonus_bet_changed_event << bonus_lines;
               postEvent (bonus_bet_changed_event);
            }
         }
      }
      else
      {
         synchronizeSelectLinesSlider (number_of_lines_selected);
      }
   }
   else
   {
      synchronizeSelectLinesSlider (number_of_active_sub_bets);
   }

   if (transition_overlays_enabled)
   {
      if (areTransitionOverlaySymbolsValid ())
      {
         postEvent ("DisplayTransitionOverlays");
      }
      else    // The transition overlays do not have valid data; ensure that they are not shown.
      {
         postEvent ("HideTransitionOverlaySymbol");
      }
   }

   if (delay_standalone_progressive_meter_reset)
   {
      // synchronize Progressive meters
      synchronizeStandaloneProgressiveMeters ();
   }

   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::synchronize, complete" << endl;
   #endif
}

/// <summary>Maintain Autoplay button presentation syncronization for any changes in states except play state.</summary>
/// <param name="force_sync">[in] force a synchronize.</param>
void SlotPresentation::synchronizeAutoPlayButtonForNonPlayState (bool force_sync)
{
   // Ensure AutoPlay button is presented correctly when outside of GameStateValue::STATE_PLAY.
   if (getFlowState () != GameStateValue::STATE_PLAY)
   {
      synchronizeAutoplayButtonPresentation (true);
   }
}

// Purpose: Synchronize the visual bet components
void SlotPresentation::synchronizeBet (void)
{
   #ifdef __VERBOSE_BET__
   debug << "SlotPresentation::synchronizeBet ()" << endl;
   #endif
   // Do the default
   GamePresentation::synchronizeBet ();
   if (bet.isValid ())
   {
      // Synchronize the Easy Bet
      synchronizeEasyBet ();

      // Retrieve the resource list from the Game Presentation.
      Mojo<ResourceList, ScriptInterface> resource_list;
      assertion (resourceList (resource_list));
      // Holds the bet indicator.
      String bet_indicator_string;
      // Create a string to generate the bet indicator property names.
      String property_bet_indicator_name;
      // Holds the bet amount for the payline.
      Amount payline_bet;
      // Use to access the bet indicator properties.  Create just one mojo on the stack.
      Mojo<PropertyInterface> property;
      // Get bet amounts for each payline.
      uint32 number_of_paylines = numberOfPaylines ();
      for (uint32 payline = 0; payline < number_of_paylines; ++payline)
      {
         // Update the bet indicator properties.
         // Create bet indicator property name.  Note that the payline property names are one-based.
         property_bet_indicator_name.sprintf ("BetIndicator%02lu", payline + 1);

         // Extract the property associated with the bet indicator from the resource list.  Update the value if
         // the property has been configured.  Do not error if the resource does not exist.
         if (resource_list->getResource<PropertyInterface> (property_bet_indicator_name, resource_list->resourceKey (), property, false))
         {
            // Get the bet amount for this payline and extract the displayable value with the current denomination.
            payline_bet = bet->getSubBetAmount (payline);
            Amount::Extractor amount_extractor (payline_bet);

            // Convert amount to String
            bet_indicator_string.clear ();
            bet_indicator_string << amount_extractor.extract (denomination);
            property->setAbsoluteValue (bet_indicator_string);
         }
         #ifdef __VERBOSE_MULTIMEDIA_EVENTS__
         debug << "SlotPresentation::synchronizeBet () Event = " << bet_indicator_event << endl;
         #endif
      }

      #ifdef __VERBOSE_BET__
      debug << " -payline bet values=";
      for (uint32 payline = 0; payline < number_of_paylines; ++payline)
         debug << bet->getSubBetAmount(payline) << " ";
      debug << endl;
      #endif
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         // Give the bet per line to the control panel.
         updateBetPerLineMeter (bet->getSubBetAmount (0));
         // Give the number of lines currently selected to the control panel
         updateSelectLinesMeter (numberOfSelectedPaylines ());
         // Set the WaysSelected property
         updateWaysSelected ();

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
                     GameControlPanelExtractor game_control_panel;
                     if (GamePresentation::controlPanel (game_control_panel))
                        game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (bet->canStartGame ());
                  }
                  else
                  {
                     igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (bet->canStartGame ());
                  }
                  // Enable/Disable the new card request button.
                  synchronizeNewCardButton ();

                  // Post whether the handle should be locked or unlocked.
                  postHandleMessage ();

                  showBetMessageMeterPanel (true, false);
                  showBetMessagePlayerMenu (true);
                  showBonusBetMessagePlayerMenu (true);
                  synchronizeAutoplayButtonPresentation ();
                  synchronizeGameLanguage ();
                  updateDoubleUpButton ();
               }
               break;

               case GameStateValue::STATE_UTILITY:
                  igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (true);
                  break;
            }
         }
      }

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

/// <summary> Synchronize the betting buttons </summary>
void SlotPresentation::synchronizeBetButtons (void)
{
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // Enable/Disable the appropriate bet controls according to the current bet object.
      igame_control_panel.getIGameControlPanelInterface ()->synchronizeBet (bet);
   }
}

// Purpose:  Called to synchronize all known control panels.
void SlotPresentation::synchronizeControlPanels (void)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|SlotPresentation::synchronizeControlPanels ()" << endl;
   #endif
   // Call the base class first.
   GamePresentation::synchronizeControlPanels ();
   // Synchronize the control panels known by this class.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getControlPanelInterface ()->synchronize ();

      // determine if and or which game menu buttons should be shown
      synchronizeGameMenuButtons ();
   }
}

// Purpose: synchronize buttons with bet amount
void SlotPresentation::synchronizeDynamicSlotBetButtons (void)
{
   if(update_dynamic_bet_buttons)
   {
      // Update the dynamic buttons if the buttom map is not empty
      if (!dynamic_slot_bet_button_map.empty ())
      {
         // Tell the control panel to update the button images
         IGameControlPanelExtractor igame_control_panel;
         if (controlPanel (igame_control_panel))
         {
            igame_control_panel.getIGameControlPanelInterface()->synchronizeDynamicSlotBetButtons (bet, dynamic_slot_bet_button_map, dynamic_slot_bet_button_alias, dynamic_button_aux_data);
            postEvent ("UpdateDynamicSlotBetButtons");

            synchronizeSelectTheBet ();
         }
      }
   }
}

// Purpose:  Called to synchronize Easy Bet.
void SlotPresentation::synchronizeEasyBet (void)
{
   if (bet.isValid ())
   {
      // Post events that reflect our current betting situation.  Note that this method can be called before the game
      // presentation has been completely configured.  Do not attempt to post an event if the stage isn't ready.
      StageInterfaces main_stage;
      if (getStage (main_stage, false))
      {
         // Check bet to see if easy bet is enabled
         bool post_easy_bet_enabled_event = false;
         Mojo <GameSideBetInterface> game_side_bet_interface (bet);
         if (game_side_bet_interface.isValid ())
         {
            if (game_side_bet_interface->getEasyBetConfiguration() != 0)
               post_easy_bet_enabled_event = true;
         }

         if (post_easy_bet_enabled_event)
         {
            postEvent ("synchronizeEasyBet:Enabled");
         }
         else
         {
            postEvent ("synchronizeEasyBet:Disabled");
         }
      }
   }
}

// Purpose:  This method tells the paylines to display the current static payline information.
void SlotPresentation::synchronizePaylines (bool draw_paylines)
{
   // We don't want to draw any paylines if the reels are currently hidden.
   bool show_paylines = baseGamePresentationEvaluation () && draw_paylines;

   // Make sure that this game has a valid bet configuration.
   if (bet.isValid ())
   {
      // Get an interface to the paylines.
      Mojo<PaylinesInterface> paylines;
      if (getPaylines (paylines, currentGameSetIndex ()))
      {
         Mojo <ResourceInterface> paylines_resource (paylines);
         if (paylines_resource.isValid ())
         {
            // Post event to update bet indicator values.
            String bet_indicator_event (paylines_resource->resourceAlias ());
            bet_indicator_event << ":UpdateBetIndicators";
            postEvent (bet_indicator_event);
            paylines->showSelectedPaylines (bet, show_paylines);
         }
      }
   }

   // Enable the bet per line slider.
   if (numberOfSelectedSubBetsIsZero ())
   {
      enableBetPerLineSlider (false);
   }
 }

// Purpose:  Update the Select Lines Scroll Bar on a synchronize
void SlotPresentation::synchronizeSelectLinesSlider (uint32 lines_selected)
{
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->synchronizeSelectLinesSlider (lines_selected);
   }
}

/// <summary># Update the Select the Bet button on synchronize.</summary>
void SlotPresentation::synchronizeSelectTheBet (void)
{
   // Check if game is CDS
   if (enrollmentRequired () && !isBingo ())
   {
      String select_the_bet_event = "CustomButtonConfig:SelectTheBet:Enable";

      // If CDS Manual Button Configuration is enabled
      if (manual_button_config_id != INVALID_CONFIGID && config.getValue (manual_button_config_id))
      {
         ConfigID config_id = config.getId ("CDS SELECT THE BET POSITION", game_identifier.asString (), false);

         if (config_id != INVALID_CONFIGID)
         {
            String select_the_bet_position;
            select_the_bet_position << "CDS MANUAL BUTTON CONFIG " << config.getValue (config_id);

            config_id = config.getId (select_the_bet_position, game_identifier.asString (), false);

            if (config_id != INVALID_CONFIGID)
            {
               String select_the_bet_string;
               config.getValueString (config_id, select_the_bet_string);

               if (strcmp (select_the_bet_string, "SELECT THE BET") != 0)
               {
                  select_the_bet_event = "CustomButtonConfig:SelectTheBet:Disable";
               }
            }
         }
      }
      postEvent (select_the_bet_event);
   }
}

// Purpose: Set control panel buttons to their default settings.
void SlotPresentation::synchronizeStartButtons (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtons" << endl;
   #endif
   // Show/Enable buttons for various flow states.
   switch (getFlowState ())
   {
      case GameStateValue::STATE_IDLE:
         {
            synchronizeStartButtonsIdleState ();
         }
         break;

      case GameStateValue::STATE_ENROLL:
         {
            synchronizeStartButtonsEnrollState ();
         }
         break;

      case GameStateValue::STATE_PRESTART:
         {
            synchronizeStartButtonsPreStartState ();
         }
         break;

      case GameStateValue::STATE_START:
         {
            synchronizeStartButtonsStartState ();
         }
         break;

      case GameStateValue::STATE_PLAY:
         {
            synchronizeStartButtonsPlayState ();
         }
         break;

      // For all the "usual" states after the play state ...
      case GameStateValue::STATE_EVALUATE:
      case GameStateValue::STATE_EVALUATE_COMPLETE:
      case GameStateValue::STATE_AWARD:
      case GameStateValue::STATE_AWARD_COMPLETE:
         {
            // Configure the control panel's contents so that they are in a known state.
            IGameControlPanelExtractor igame_control_panel;
            if (controlPanel (igame_control_panel))
            {
               igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
               igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

               // Show disabled Stop Spin button.
               igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
               igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);

               igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
               igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);
            }

            // Get the game control panel.
            GameControlPanelExtractor game_control_panel;
            if (GamePresentation::controlPanel (game_control_panel))
            {
               game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
               game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
               game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
            }
         }
         break;

      default:
         {
            // Configure the control panel's contents so that they are in a known state.
            IGameControlPanelExtractor igame_control_panel;
            if (controlPanel (igame_control_panel))
            {
               igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (true);
               igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

               igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (false);
               igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);

               igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
               igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);
            }

            // Get the game control panel.
            GameControlPanelExtractor game_control_panel;
            if (GamePresentation::controlPanel (game_control_panel))
            {
               game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
               game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
               game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
            }
         }
         break;
   }
}


void SlotPresentation::synchronizeStartButtonsEnrollState (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtonsEnrollState" << endl;
   #endif

   // If you need to enroll before game play...
   if (enrollmentBeforeGameStart ())
   {
      // Configure the control panel's contents so that they are in a known state.
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         // Show spin button. Enable only if the enroll button hasn't been pressed
         // twice (otherwise, they've effectively pressed it already).
         igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (true);
         if (!getEnrollmentButtonDoubleTap ())
         {
            igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (true);
         }

         igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);

         igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);
      }

      // Get the game control panel.
      GameControlPanelExtractor game_control_panel;
      if (GamePresentation::controlPanel (game_control_panel))
      {
         game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
         game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
      }
   }
   else
   {
      // Configure the control panel's contents so that they are in a known state.
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         // Show stop button, but don't enable it.  We have to enable and then disable for the
         // button to appear correctly.
         igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (true);
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);

         igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

         igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);
      }

      // Get the game control panel.
      GameControlPanelExtractor game_control_panel;
      if (GamePresentation::controlPanel (game_control_panel))
      {
         game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
         game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
         game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
      }
   }
}


void SlotPresentation::synchronizeStartButtonsIdleState (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtonsIdleState" << endl;
   #endif

   // Determine if there is a valid bet.
   bool valid_bet = false;
   if (bet.isValid () && canBet ())
   {
      valid_bet = bet->canStartGame ();
   }

   Mojo<GameSideBetInterface> game_side_bet;
   game_side_bet = bet;
   if (game_side_bet.isValid ())
   {
      // If the game is not precommitted, the game didn't enroll, so enable the spin button again.
      if (!game_side_bet->precommitted ())
      {
         setOkayToHideSpinButton (false);
      }
   }

   // If need to enroll before game play...
   if (enrollmentBeforeGameStart ())
   {
      // Configure the control panel's contents so that they are in a known state.
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);

         igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);
      }

      // If spin button already pressed...
      if (okayToHideSpinButton ())
      {
         // Configure the control panel's contents so that they are in a known state.
         IGameControlPanelExtractor igame_control_panel;
         if (controlPanel (igame_control_panel))
         {
            // Reset the spin button and enable if bet is valid.
            igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (true);
            igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (valid_bet);
         }

         // Get the game control panel.
         GameControlPanelExtractor game_control_panel;
         if (GamePresentation::controlPanel (game_control_panel))
         {
            game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
            game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
         }
      }
      else
      {
         // Get the game control panel.
         GameControlPanelExtractor game_control_panel;
         if (GamePresentation::controlPanel (game_control_panel))
         {
            // Show enroll button and enable if bet is valid.
            game_control_panel.getGameControlPanelInterface ()->showEnrollButton (true);
            game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (valid_bet);
         }

         // Configure the control panel's contents so that they are in a known state.
         IGameControlPanelExtractor igame_control_panel;
         if (controlPanel (igame_control_panel))
         {
            igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
            igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);
         }
      }
   }
   // Else, no need to enroll before gameplay.
   else
   {
      // Get the game control panel.
      GameControlPanelExtractor game_control_panel;
      if (GamePresentation::controlPanel (game_control_panel))
      {
         game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
         game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
      }

      // Configure the control panel's contents so that they are in a known state.
      IGameControlPanelExtractor igame_control_panel;
      if (controlPanel (igame_control_panel))
      {
         igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
         igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);

         // If button already pressed...
         if (okayToHideSpinButton ())
         {
            // Show stop button and enable it.
            igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
            igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (true);

            igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
            igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);
         }
         else
         {
            // Show spin button and enable if bet is valid.
            igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (true);
            igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (valid_bet);

            igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (false);
            igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);
         }
      }
   }
   synchronizeGameLanguage ();
}


void SlotPresentation::synchronizeStartButtonsPlayState (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtonsPlayState" << endl;
   #endif

   // Get the game control panel.
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->showEnrollButton (false);
      game_control_panel.getGameControlPanelInterface ()->enableEnrollButton (false);
      game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
   }

   // Configure the control panel's contents so that they are in a known state.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->showEvaluateButton (false);
      igame_control_panel.getIGameControlPanelInterface ()->enableEvaluateButton (false);

      igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
      igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

      // If we are allowed to enable the stop button...
      igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
      if (!enrollmentRequired () || enrollmentBeforeGameStart () || !getEnrollmentButtonDoubleTap ())
      {
         // Enable the stop button.
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (okayToSlamReels ());
      }
      else
      {
         // Disable the stop button.
         igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);
      }
   }
}


void SlotPresentation::synchronizeStartButtonsPreStartState (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtonsPreStartState" << endl;
   #endif

   // Configure the control panel's contents so that they are in a known state.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // Disable the spin button.
      igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
      igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

      // Show a disabled stop button.  We have to enable and then disable for it to appear
      // correctly.
      igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
      igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (true);
      igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);
   }
   // Get the game control panel.
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
   }
}


void SlotPresentation::synchronizeStartButtonsStartState (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeStartButtonsStartState" << endl;
   #endif

   // Configure the control panel's contents so that they are in a known state.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->showSpinButton (false);
      igame_control_panel.getIGameControlPanelInterface ()->enableSpinButton (false);

      // Show a disabled stop button.  We have to enable and then disable for it to appear
      // correctly.
      igame_control_panel.getIGameControlPanelInterface ()->showStopSpinButton (true);
      igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (true);
      igame_control_panel.getIGameControlPanelInterface ()->enableStopSpinButton (false);
   }

   // Get the game control panel.
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, false);
   }
}

// set the the current volume button state and resets the volume if it isn't shown
void SlotPresentation::synchronizeVolumeButton (bool enable)
{
   #ifdef __VERBOSE_VOLUME__
      debug << "SlotPresentation::synchronizeVolumeButton ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;

   bool okayToEnableVolumeButton = GamePresentation::okayToEnableVolumeButton ();

   // Only allow the volume button to appear on the meter panel for non-player selectable game situations.
   if (GamePresentation::controlPanel (game_control_panel))
   {
      bool show_volume_button = ((numberOfAvailablePlayerGames () == 1) || show_volume_button_with_more_games_enabled);
      // Show or Hide the Button
      game_control_panel.getGameControlPanelInterface ()->showVolumeButton (show_volume_button);
      // Enable or Disable the button.
      game_control_panel.getGameControlPanelInterface ()->enableVolumeButton (okayToEnableVolumeButton && enable);

      if (!show_volume_button)
      {
         postEvent ("HideAndDisableVolumeButton:SlotInterface");
      }
   }

   // The player menu will always have a volume button if it is ok to enable it.
   String event = "EnableVolumeButton:PlayerMenu:";
   if (okayToEnableVolumeButton)
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

void SlotPresentation::synchronizeNewCardButton (void)
{
  #ifdef __VERBOSE__
   debug << "SlotPresentation::synchronizeNewCardButton" << endl;
  #endif

   // Configure the control panel's contents so that they are in a known state.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      //check to see if the player has credits
      Amount credit_amount (ZERO_AMOUNT);
      wagerCreditDisplayAmount (credit_amount);

      // OK to enable new card button in the idle state when the game has credits
      if (getFlowState () == GameStateValue::STATE_IDLE &&
         credit_amount != ZERO_AMOUNT)
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableNewCardButton (true);
      }
      else
      {
         igame_control_panel.getIGameControlPanelInterface ()->enableNewCardButton (false);
      }
   }
}

// Purpose:  Update the Bet Per Line Scroll Bar on a synchronize
void SlotPresentation::synchronizeBetPerLineSlider (const Amount &value)
{
   Amount::Extractor amount_extractor (value);
   Amount::AmountValue new_value = amount_extractor.extract (denomination);
   // Initialize the ranges.
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      igame_control_panel.getIGameControlPanelInterface ()->synchronizeBetPerLineSlider (new_value);
   }
}

// Purpose: Synchronize the presentation
void SlotPresentation::synchronizePresentation (uint32 what_to_synchronize, uint32 bet_request_type, uint32 bet_request_data)
{
   #ifdef __VERBOSE_VOLUME__
      debug << "SlotPresentation::synchronizePresentation ()" << endl;
   #endif

   GamePresentation::synchronizePresentation (what_to_synchronize, bet_request_type, bet_request_data);

   // If synchronizing due to a bet request
   if ((0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_BET)) &&
       (UserSequenceEvent::INVALID_REQUEST_CATEGORY != bet_request_type))
   {
      if (bet.isValid ())
      {
         bool play_n_lines_type_button = false;
         switch (bet_request_type)
         {
            case UserSequenceEvent::BET_N_COINS_REQUEST:
            {
               if (doesBetSupportGameSideBetInterface ())
               {
                  // Check to see if this is a play_n_lines style button
                  Mojo <GameSideBetInterface> game_side_bet_interface (bet);
                  if (game_side_bet_interface.isValid ())
                  {
                     vector<ButtonData> button_data_list;
                     game_side_bet_interface->getBetNButtonPanelData(button_data_list);

                     if (bet_request_data <= (uint32)button_data_list.size() &&
                        !game_side_bet_interface->getBetNButtonData(bet_request_data-1).bet_per_sub_bet)
                     {
                        play_n_lines_type_button = true;
                     }
                  }
               }
            }

            // Fall through on purpose
            case UserSequenceEvent::MAX_BET_REQUEST:
            {
               if (doesBetSupportGameSideBetInterface ())
               {
                     // Fall through into PlayNLines mode if correct button type
                  if (!play_n_lines_type_button)
                  {
                     // Make sure that the wins are no longer animating.
                     displayWin (false, false);
                     // Synchronize the payline and payline marker displays.  Do not draw the paylines.
                     synchronizePaylines (false);
                     break;
                  }
               }
            }

            // Fall through if it is a play_n_lines_type_button
            case UserSequenceEvent::PLAY_N_LINES_REQUEST:
            {
               if (doesBetSupportGameSideBetInterface ())
               {
                  // Request payline display state. Update the payline display.
                  if (isFlowStateIdle () && canUpdatePaylines ())
                  {
                     // Make sure that the wins are no longer animating.
                     displayWin (false, false);
      
                     Mojo <SlotGameSetInterface> slot_game_set;
                     if (slotGameSet (slot_game_set, currentGameSetIndex ()))
                     {
                        if (slot_game_set->isWinCycleLiteConfigured ())
                        {
                           Mojo <WinCycleLiteInterface> win_cycle_lite;
                           slot_game_set->getWinCycleLite (win_cycle_lite);
                           win_cycle_lite->displaySelectedPaylines (bet);
                        }
                        else
                        {
                           postEvent ("SlotDisplayPaylines");
                           synchronizePaylines (true);
                        }
                     }
                  }
               }
            }
            break;

            // Handle Max Bet request in the Utility Mode (Slot Evaluation Tests).
            case UserSequenceEvent::START_GAME_REQUEST:
            {
               if (isFlowStateUtility () && !doesBetSupportGameSideBetInterface ())
               {
                  // Make sure that the wins are no longer animating.
                  displayWin (false, false);

                  // Stop the win animations about the display
                  hideAllPaylinesDisplays ();

                  // Only shows the paylines selected when handling max bet request for the first time.
                  // Subsequent max bet requests at max bet should evaluate the game.
                  if (previous_number_of_lines_selected != bet->numberOfActiveSubBets ())
                  {
                     Mojo <SlotGameSetInterface> slot_game_set;
                     if (slotGameSet (slot_game_set, currentGameSetIndex ()))
                     {
                        if (slot_game_set->isWinCycleLiteConfigured ())
                        {
                           Mojo <WinCycleLiteInterface> win_cycle_lite;
                           slot_game_set->getWinCycleLite (win_cycle_lite);
                           win_cycle_lite->displaySelectedPaylines (bet);
                        }
                     }
                  }
                  else
                  {
                     // Hide paylines display for games using the LUA drive win cycle.
                     postEvent ("HideSelectedPaylines");

                     // Call this method to set the show wins flag in the SlotUtilityDisplayState
                     postEvent ("updateShowWinsFlagToTrue");

                     // Notify the flow that an evaluation is necessary.
                     StartUtilitySequenceEvent start_event (StartUtilitySequenceEvent::STATUS_START, SequenceEvent::TO_GAMES);
                     postEvent (start_event);
                  }

                  // show the bet message
                  showBetMessageInHistoryAndUtilityStates ();
               }
            }
            break;

            case UserSequenceEvent::SELECT_MORE_LINES_REQUEST:
            case UserSequenceEvent::SET_SELECTED_LINES_REQUEST:
            {
               // Request payline display state. Update the payline display.
               if ( (isFlowStateIdle () || (isFlowStateUtility () && !doesBetSupportGameSideBetInterface ())) && canUpdatePaylines ())
               {
                  // Make sure that the wins are no longer animating.
                  displayWin (false, false);

                  Mojo <SlotGameSetInterface> slot_game_set;
                  if (slotGameSet (slot_game_set, currentGameSetIndex ()))
                  {
                     if (slot_game_set->isWinCycleLiteConfigured ())
                     {
                        Mojo <WinCycleLiteInterface> win_cycle_lite;
                        slot_game_set->getWinCycleLite (win_cycle_lite);
                        win_cycle_lite->displaySelectedPaylines (bet);
                     }
                     else
                     {
                        postEvent ("SlotDisplayPaylines");
                        if (draw_paylines_individually)
                        {
                           drawIndividualPayline ();
                        }
                        else
                        {
                           synchronizePaylines (true);
                        }
                     }
                  }
               }
               // show the bet message
               if (isFlowStateUtility () && !doesBetSupportGameSideBetInterface ())
               {
                  showBetMessageInHistoryAndUtilityStates ();
               }
            }
            break;

            case UserSequenceEvent::BETUP_ALL_BETS_REQUEST:
            case UserSequenceEvent::SET_BET_PER_LINE_REQUEST:
               {
                  // Make sure that the wins are no longer animating.
                  displayWin (false, false);
                  postEvent ("SlotDisplayPaylines");
                  // Get an interface to the paylines.
                  Mojo<PaylinesInterface> paylines;
                  if (getPaylines (paylines, currentGameSetIndex ()))
                  {
                     Mojo <ResourceInterface> paylines_resource (paylines);
                     if (paylines_resource.isValid ())
                     {
                        // Post event to update bet indicator values.
                        String bet_indicator_event (paylines_resource->resourceAlias ());
                        bet_indicator_event << ":UpdateBetIndicators";
                        postEvent (bet_indicator_event);
                     }
                  }
               }
               break;

            default:
               break;
         } // Switch on bet_request_type
 
         // When changing from 0 lines selected, enable the bet per line slider
         if ((previous_number_of_lines_selected == 0) &&
             (bet->numberOfActiveSubBets () > 0))
         {
            enableBetPerLineSlider (true);
         }

         previous_number_of_lines_selected = bet->numberOfActiveSubBets ();
      } // Bet object is valid
   } // SYNCHRONIZE_BET
   else
   {
      if (0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_PANEL))
      {
         synchronizeMoreGamesButton ();
         synchronizeGameLanguage ();
      }
      else if (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_BET)
      {
         synchronizeGameLanguage ();
      }
   }
}

/// <summary>
/// # Update Standalone Progressive meters on a synchronize, if the meters exist.
/// </summary>
/// <remarks>
/// # This only affects the presentation.  Even though it is named with Standalone...it can be used with
/// any stepper style virtual spectrum.
/// </remarks> 
/// <example><b>Example:</b>
/// @code
/// this:synchronizeStandaloneProgressiveMeters ()
/// @endcode
/// </example>
void SlotPresentation::synchronizeStandaloneProgressiveMeters (void)
{
   ProgressiveGameId progressive_game_id (game_identifier, stage_index);

   Denomination controller_denom = Denomination::BLANK;
   ProgressiveControllerId controller_id;
   uint32 controller_level;
   bool is_paytable_formed;
   uint32 list_position = 0;


   updatePaytableAwardProperties (true);

   // Get the number of level progressives?
   uint32 number_of_levels = progressive_manager.numberOfLevels (progressive_game_id, denomination);
   // Update all progressive levels
   for (uint32 level_index = 0; level_index < number_of_levels; ++level_index)
   {
      bool linked = progressive_manager.getGameLink (progressive_game_id, level_index, controller_id,
                                                              controller_level, is_paytable_formed, list_position, denomination,
                                                              controller_denom);
      if (linked)
      {
         #ifdef __VERBOSE_STANDALONE_PROGRESSIVE__
         debug << "  linked progressive " << linked << endl;
         debug << "  progressive_game_id: " << progressive_game_id << endl;
         debug << "  level: " << level_index << endl;
         debug << "  controller_id: " << controller_id << endl;
         debug << "  controller_level: " << controller_level << endl;
         debug << "  is_paytable_formed: " << (uint16) is_paytable_formed << endl;
         debug << "  list_position: " << list_position << endl;
         debug << endl;
         #endif

         // Retrieve the resource list from GamePresentation.
         Mojo<ResourceList, ScriptInterface> resource_list;
         assertion (resourceList (resource_list));
         Mojo<PropertyInterface> property;

         // Extract the property associated with this progressive level from the resource list.
         String property_name (PROPERTY_SPECTRUM_DISPLAY_VALUESTRING_ROOT);
         property_name << level_index;
         if (resource_list->getResource<PropertyInterface> (property_name, "Default", property, false))
         {
            String event ("Standalone:");
            event << PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT << level_index;
            postEventAll (event);
         }
         // Update "prize string" property
         property_name = PROPERTY_SPECTRUM_TEXT_DISPLAY_VALUE_ROOT;
         property_name << level_index;
         if (resource_list->getResource<PropertyInterface> (property_name, "Default", property, false))
         {
            String event ("Standalone:");
            event << PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT << level_index;
            postEventAll (event);
         }
      }
   }
}

/// <summary># Find the paid amount and update the virtual spectrum meter. </summary>
/// <remarks># This only affects the presentation.</remarks> 
/// <example><b>Example:</b>
/// @code
/// this:synchronizeVirtualSpectrumWithRealValue ()
/// @endcode
/// </example>
void SlotPresentation::synchronizeVirtualSpectrumWithRealValue (void)
{
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   uint16 number_of_awards = local_award->getNumberOfAwards ();
   for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
   {
      Mojo<SystemAwardInterface> system_award;

      if (local_award->getAward (award_index, system_award))
      {
         if (system_award.isValid () && (system_award->getTransactionId () == evaluationTransactionId ()))
         {
            uint16 number_of_items = system_award->getNumberOfAwards ();
            for (uint16 item_index = 0; item_index < number_of_items; item_index++)
            {
               uint16 number_of_progressives = system_award->getNumberOfPossibleProgressiveHits (item_index);
               for (uint16 progressive_index = 0; progressive_index < number_of_progressives; progressive_index++)
               {
                  // Resolve the game level from the award item.
                  uint32 game_level = system_award->getPossibleProgressiveAwardLevel (item_index, progressive_index);

                  // Retrieve the awarded progressive amount.
                  Amount progressive_amount (system_award->getAmount (item_index));

                  // See if there's a COMBINE_BASE_AND_PROGRESSED situation and add it to the current amount...
                  String level_name;
                  level_name << COMBINE_BASE_AMOUNT_TRUNK << game_level;

                  // Iterate through the award list again.
                  for (uint16 base_index = 0; base_index < number_of_items; base_index++)
                  {
                     if (item_index != base_index && !strncmp (system_award->getName (base_index), level_name, strlen (level_name)))
                     {
                        progressive_amount += (system_award->getAmount (base_index));
                     }
                  }

                  String progressive_string;
                  // Used to format amount display strings.
                  ULocale locale;
                  Amount::Extractor amount_extractor (progressive_amount);
                  Amount::AmountValue amount_value = amount_extractor.extract (Denomination::BASE_UNIT);
                  CountryCode::Country country = locale.moneyNativeCountryCode ();

                  locale.moneyFormatNumericWhole (country, amount_value, progressive_string);

                  paytable_award_property_list.updateWithForcedValue (resource_list, virtual_spectrum_associations,
                                                                      game_level,
                                                                      progressive_string);
               }
            }
         }
      }
   }   

   postEvent ("ProgressivePaytableAwardPropertiesUpdated");
}

// Purpose:  This method is called to ensure that More Games Button is correctly enabled/disabled
// after a SYNCRONIZE_PRESENTATION event.
void SlotPresentation::synchronizeMoreGamesButton (void)
{
   #ifdef __VERBOSE_VOLUME__
      debug << "SlotPresentation::synchronizeMoreGamesButton ()" << endl;
   #endif

   if (getFlowState () == GameStateValue::STATE_IDLE && isNormalPlay () && stageIndex () == BASE_GAME_STAGE)
   {
      enableMoreGamesButton (true);
   }
   else
   {
      enableMoreGamesButton (false);
   }
}

// Purpose: Synchronize the visual display of the win meter.
void SlotPresentation::synchronizeWinAmount (void)
{
   #if defined (__VERBOSE_METHOD__) || defined (__VERBOSE_METERS__)
   debug << "SlotPresentation::synchronizeWinAmount" << endl;
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

      #if defined (__VERBOSE_METERS__)
         debug << "   total_win_amount: " << total_win_amount << endl;
      #endif

      // Give the meter to the control panel for display.
      if (rollupSlamButtonSet () && last_game_win_amount.isNotZero ())
      {
         game_control_panel.getGameControlPanelInterface ()->setWinMeter (last_game_win_amount, RollupDefs::SNAP_TO_AMOUNT);
      }
      else
      {
         game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount, RollupDefs::SNAP_TO_AMOUNT);
      }
   }
}

// Purpose: Count and return the number of reels currently held.
uint32 SlotPresentation::totalReelsHeld (void)
{
   // Initialize the return value.
   uint32 reels_held = 0;
   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);
   // Make sure the essential record has been initialized.
   if (slot_essential_record->numberOfReels (currentReelSet ()) == 0)
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::allReelsAreHeld (), Called before essential record is initialized."));
   // Try to find a reel that is not held.
   for (SlotEssentialRecord::ReelIndex reel_index = 0; reel_index < slot_essential_record->numberOfReels (currentReelSet ()); ++reel_index)
   {
      if (slot_essential_record->reelState (currentReelSet (), reel_index) == SlotEssentialRecord::HELD)
         reels_held++;
   }
   #ifdef __VERBOSE_REELS__
   debug << "SlotPresentation::totalReelsHeld: " << reels_held << endl;
   #endif
   return reels_held;
}

// Purpose:  Update the BetPerLineMeter with the given value.
void SlotPresentation::updateBetPerLineMeter (const Amount &value)
{
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (denomination.isValid () && resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> ("BetPerLineMeter", resource_list->resourceKey (), property, false))
      {
         // Update the resource with the current BetPerLine value.
         property->setAbsoluteValue (GamePresentationHelper::extractAmountValueByConfiguration (value, denomination));
         postEvent ("UpdateBetPerLineMeter");
      }
   }
}

// Purpose: Update the specified config item
void SlotPresentation::updateConfigItem (const String &config_id)
{
   // Do the default
   GamePresentation::updateConfigItem (config_id);
}


// --------------------------------------------------------------------
// Purpose: Send the appropriate messaging to dim the inactive
// symbols on a particular reel on the screen.
// --------------------------------------------------------------------
void SlotPresentation::updateMultiWayActiveSymbolMask (bool with_active_scatter_symbols)
{
   updateMultiWayActiveSymbolMask(with_active_scatter_symbols, -1);
}

void SlotPresentation::updateMultiWayActiveSymbolMask (bool with_active_scatter_symbols, int8 which_reel)
{
   Mojo<ReelsInterface> reels;
   bool use_transition_overlay_symbols = (areTransitionOverlaySymbolsValid() && (currentGameMode () == GAME_MODE_PLAY) && transition_overlays_enabled);

   if (getReels (reels, currentGameSetIndex ()))
   {
      // If the game is using video reels...
      Mojo<VideoReelsInterface> video_reels_interface (reels);
      if (!video_reels_interface.isValid ())
      {
         setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::synchronizeBet (), video reels not found.").errorMessage (), whoAreYou ());
         return;
      }

      StageInterfaces stage_interfaces;
      if (!getStage (stage_interfaces))
      {
         setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::synchronizeBet (), can not get stage.").errorMessage (), whoAreYou ());
         return;
      }

      Mojo<GameSideSlotEvaluatorInterface> game_side_evaluator;
      if (gameSideSlotEvaluatorInterface (game_side_evaluator) == false)
      {
         setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::synchronizeBet(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
         return;
      }

      size_t size = game_side_evaluator->sizeOfMultiWayMaskBuffer ();
      bool* buffer = new bool [size];
      uint32 reel_set = currentReelSet ();
      uint32 total_reels = numberOfReels (currentReelSet ());
      RandomNumber* reel_stops = new RandomNumber [total_reels];

      for (uint32 reel_index = 0; reel_index < total_reels; reel_index++)
      {
         reel_stops [reel_index] = reelStopVirtual (reel_set, reel_index);
      }

      if (with_active_scatter_symbols && !use_transition_overlay_symbols)
      {
         game_side_evaluator->getMultiWayMaskWithActiveScatters (reel_stops, bet, buffer, size);
      }
      else
      {
         game_side_evaluator->getMultiWayMask (reel_stops, bet, buffer, size);
      }

      // Record the number of reels we think are spinning. We will decrement this
      // counter with each spin stop sequence event received from the reels.
      uint16 i = 0;

      for (uint32 reel_index = 0; reel_index < total_reels; reel_index++)
      {
         // Attach local queue of reel container to this symbol mask
         Mojo<ActorInterface> reel_container;
         if (!ActorHelper::findActor (stage_interfaces.getActorInterface (), video_reels_interface->getModelPieceName (reel_index), true, reel_container))
         {
            String error_string;
            error_string << "SlotPresentation::synchronizeBet, Can't find \"" << video_reels_interface->getModelPieceName (reel_index) << "\"";
            setErrorString (LogicError (LogicError::INVALID_VALUE, error_string).errorMessage (), whoAreYou ());
            delete []buffer;
            delete []reel_stops;
            return;
         }

         if (!reel_container.isValid ())
         {
            setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::synchronizeBet (), reel container not found.").errorMessage (), whoAreYou ());
            delete []buffer;
            delete []reel_stops;
            return;
         }
         int16 number_of_vsible_symbols = numberOfVisibleSymbols (reel_set, reel_index);
         for (int16 symbol_offset = 0; symbol_offset < number_of_vsible_symbols; symbol_offset++)
         {
            String multi_way_segment_mask_event_string;

            // if transition overlays are enabled, and the game has symbols that should not be masked, apply the Multiway masks based on
            // the overlay symbols instead of the real ones.
            if (with_active_scatter_symbols && use_transition_overlay_symbols)
            {
               Mojo<PropertyInterface> property;
               String fake_symbol_property_string("TransitionOverlaySymbol:");
               String symbol_name;
               String scatter_symbol_name;
               fake_symbol_property_string << reel_index << ":" << symbol_offset;

               // If the overlay symbols persistent property is valid, extract symbol name for the current symbol
               if (resource_list->getResource<PropertyInterface> (fake_symbol_property_string, resource_list->resourceKey (), property))
               {
                  property->getAbsoluteValue (symbol_name, 0);
               }

               if (buffer[i++] == true)
               {
                  multi_way_segment_mask_event_string = hide_multi_way_segment_mask_event;
               }
               else
               {
                  // The symbol masks are shown by default
                  multi_way_segment_mask_event_string = show_multi_way_segment_mask_event;

                  // Loop through the list of active scatter symbols looking for a match.
                  for (uint32 symbol_index = 0; symbol_index < scatter_overlay_symbol_name.size (); symbol_index++)
                  {
                     scatter_symbol_name = "TransitionOverlaySymbol:";
                     scatter_symbol_name << scatter_overlay_symbol_name [symbol_index];

                     // Do not mask the symbols if a match is found.
                     if (scatter_symbol_name == symbol_name)
                     {
                        multi_way_segment_mask_event_string = hide_multi_way_segment_mask_event;
                     }
                  }
               }
            }

            // Transition overlays are not enabled, so apply the Multiway masks based on the actual symbols.
            else
            {
               if (buffer [i++] == true)
               {
                  multi_way_segment_mask_event_string = hide_multi_way_segment_mask_event;
               }
               else
               {
                  multi_way_segment_mask_event_string = show_multi_way_segment_mask_event;
               }
            }

            multi_way_segment_mask_event_string << symbol_offset;

            // which_reel is used to specify a particular reel, or can be passed in as -1 to specify all reels
            if (which_reel < 0 || which_reel == (int8) reel_index)
            {
               // Get local queue from Reel actor
               Mojo<ActuableInterface> actuable_reel_actor (reel_container);
               // Post events to the local queue of the reel container
               if (actuable_reel_actor.isValid ())
               {
                  Mojo<MultimediaEventQueueInterface> local_queue (actuable_reel_actor->getLocalQueue ());
                  if (local_queue.isValid ())
                     local_queue->postEvent (multi_way_segment_mask_event_string);
               }
            }
         }
      }

      delete []buffer;
      delete []reel_stops;
   }
}

// Purpose: A scriptable routine that enables or disables the transition overlays
void SlotPresentation::enableTransitionOverlays (bool _transition_overlays_enabled)
{
   transition_overlays_enabled = _transition_overlays_enabled;
}

// Purpose: Return transition overlays is enable or not
bool SlotPresentation::transitionOverlaysEnabled (void)
{
   return transition_overlays_enabled;
}

// Purpose: This routine clears the stored overlay symbols so we will know when the data is no longer valid.
void SlotPresentation::resetTransitionOverlaySymbols (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::resetTransitionOverlaySymbols ()" << endl;
   #endif

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      Mojo<PropertyInterface> property;

      // Set the base game pays string property
      String unset_symbol_property_string;
      String unset_symbol_property_value ("TransitionOverlaySymbol:XX");

      for (uint32 reel_index = 0; reel_index < numberOfReels (0); ++reel_index)
      {
         for (uint32 visible_stop_index = 0; visible_stop_index < numberOfVisibleSymbols (0, reel_index); ++visible_stop_index)
         {
            unset_symbol_property_string = "TransitionOverlaySymbol:";
            unset_symbol_property_string << reel_index << ":" << visible_stop_index;

            if (resource_list->getResource<PropertyInterface> (unset_symbol_property_string, resource_list->resourceKey (), property))
            {
               property->setAbsoluteValue (unset_symbol_property_value);
            }
         }
      }
   }
}

// --------------------------------------------------------------------
// Purpose: Grab the "other" Stage resource key to make the seamless
//          transition work.
// --------------------------------------------------------------------
void SlotPresentation::setTransitionOverlayResourceKey (String new_transition_overlays_resource_key)
{
   transition_overlays_resource_key = new_transition_overlays_resource_key;
}

// --------------------------------------------------------------------
// Purpose: This routine stores the textures for the Overlay symbols
// to be displayed on the Main interface.
// --------------------------------------------------------------------
void SlotPresentation::storeTransitionOverlaySymbols (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::storeOverlaySymbols ()" << endl;
   #endif

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      Mojo<PropertyInterface> property;

      // Set the base game pays string property
      String unset_symbol_property_string;
      String unset_symbol_property_value;

      for (uint32 reel_number = 0; reel_number < numberOfReels (0); ++reel_number)
      {
         for (uint32 visible_stop_index = 0; visible_stop_index < numberOfVisibleSymbols (0, reel_number); ++visible_stop_index)
         {
            unset_symbol_property_string = "TransitionOverlaySymbol:";
            unset_symbol_property_string << reel_number << ":" << visible_stop_index;

            unset_symbol_property_value = "TransitionOverlaySymbol:";
            unset_symbol_property_value << symbolName (0, reel_number, visible_stop_index);

            // Make sure the resource key has been properly initialized.
            if (strcmp (transition_overlays_resource_key, CONDITION_UNINITIALIZED) != 0)
            {
               if (resource_list->getResource<PropertyInterface> (unset_symbol_property_string, transition_overlays_resource_key, property))
               {
                  property->setAbsoluteValue (unset_symbol_property_value);
               }
            }
         }
      }
   }
}

// ----------------------------------------------------------------
// Purpose: Return the status of the Overlay Symbols.
// ----------------------------------------------------------------
bool SlotPresentation::areTransitionOverlaySymbolsValid (void)
{
   bool return_value = false;

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;

      // Our comparison string.
      String unset_symbol_property_string ("TransitionOverlaySymbol:0:0");

      if (resource_list->getResource<PropertyInterface> (unset_symbol_property_string, resource_list->resourceKey (), property, false))
      {
         String property_value;
         property->getAbsoluteValue (property_value);

         if (strcmp (property_value, "TransitionOverlaySymbol:XX") != 0)
         {
            return_value = true;
         }
      }
   }

   return return_value;
}

// Purpose:  Update the SelectLinesMeter with the given value.
void SlotPresentation::updateSelectLinesMeter (uint32 lines_selected)
{
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
      if (resource_list->getResource<PropertyInterface> ("SelectLinesMeter", resource_list->resourceKey (), property, false))
      {
         // Convert uint32 to String
         String lines_selected_string;
         lines_selected_string << lines_selected;
         // Update the resource with the current SelectLines value.
         property->setAbsoluteValue (lines_selected_string);
         postEvent ("UpdateSelectLinesMeter");
      }
   }
}

// Purpose:  Update the WaysSelected property with the current number of ways selected
void SlotPresentation::updateWaysSelected (void)
{
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
      if (resource_list->getResource<PropertyInterface> (WAYS_SELECTED_PROPERTY_NAME, resource_list->resourceKey (), property, false))
      {
         // Convert uint32 to String
         String ways_selected_string;
         // Put the number of ways selected into string form
         ways_selected_string << numberOfMultiWaysSelected ();
         // Update the resource with the current SelectLines value.
         property->setAbsoluteValue (ways_selected_string);
      }
   }
}

// Return the type of this scriptable object.
const char *SlotPresentation::whoAreYou (void) const
{
   return object_type;
}

// Purpose: Allow for derived classes to update their info when switching
// between paytables within the same theme.
void SlotPresentation::updateAfterPaytableSwitch (void)
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::updateAfterPaytableSwitch ()" << endl;
   #endif

   #ifdef __VERBOSE_GAME_SWITCH__
   DebugTimer debug_timer;
   debug_timer.start ();
   #endif

   // Call the base class.
   GamePresentation::updateAfterPaytableSwitch ();

   debugTime (debug_timer, "GamePresentation::updateAfterPaytableSwitch");

   // Reload the reels object using the new paytable information.
   if (!reconfigureReels ())
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::updateAfterPaytableSwitch: Could not reconfigure reels."));

   // load any information that we need from the registry files.
   loadRegistyData ();

   // update the digital glass.
   updateDigitalGlass ();

   debugTime (debug_timer, "reconfigureReels");
}

// Update bonus bet flag status
void SlotPresentation::updateBonusBetStatus(bool status)
{
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateBonusBetStatus" << endl;
   #endif

   bonus_bet_message = status;

   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateBonusBetStatus, complete" << endl;
   #endif
}

// Update maxbet bonus flag status
void SlotPresentation::updateMaxbetBonusStatus(bool status)
{
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateMaxbetBonusStatus" << endl;
   #endif

   maxbet_bonus_enable = status;

   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateMaxbetBonusStatus, complete" << endl;
   #endif
}

//Update the slider to the bonus position (bonus bet type games only)
void SlotPresentation::updateSliderBonusPosition(void)
{
   // Update the Select lines meter with proper bonus bet value
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;

   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateSliderBonusPosition" << endl;
   #endif

   // Do not update the bonus bet if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      if (resource_list->getResource<PropertyInterface> ("NumberOfLinesBetSlider-Value", resource_list->resourceKey (), property, false))
      {

         // We only need to do this if we are a bonus bet type game
         if (bonusBetTypeGame() == true)
         {
            // Get the bonus bet
            Amount bonus_bet_amount = bet->getBetAmountByName ("bonus");

            // if the bonus bet is actually enabled then...
            if (bonus_bet_amount > ZERO_AMOUNT)
            {

               // Convert uint32 to String
               String lines_selected_string;
               lines_selected_string << number_of_lines_max_value << "+";
               #ifdef __VERBOSE_MODE__
               debug << "SlotPresentation::updateSliderBonusPosition - Value = " << lines_selected_string << endl;
               #endif

               // Update the resource with the current SelectLines value.
               property->setAbsoluteValue (lines_selected_string);
               postEvent ("SynchronizeSelectLinesSlider");
            }
         }
      }
   }
   #ifdef __VERBOSE_MODE__
   debug << "SlotPresentation::updateSliderBonusPosition, Complete" << endl;
   #endif

}

// Purpose:: Synchronize the visual bet components do to a (host central determination) bet configuration change.
void SlotPresentation::synchronizeBetConfig (void)
{
   // Let the base class handle this first
   GamePresentation::synchronizeBetConfig ();
   // Set the ranges of the sliders.
   initializeSliders ();
   // Update properties related to bet configuration
   synchronizeWinUpToItems ();
   synchronizeDynamicSlotBetButtons ();
}

// Purpose:  Update the lowest player denomination with the given value.
void SlotPresentation::updateLowestPlayerDenomination (void)
{
   uint32 value = 0;
   DenominationRules denom_rules;
   Denomination denomination;
   denom_rules.firstEnabledDenomination (theme_configuration_identifier, denomination);
   value = denomination.getValue ();

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> (LOWEST_PLAYER_DENOMINATION, resource_list->resourceKey (), property))
      {
         property->setAbsoluteValue (value);
      }
   }
}

// Purpose:  Update the WinUpToDenomination with the given value.
void SlotPresentation::updateWinUpToDenomination (void)
{
   uint32 value = 0;
   uint64 top_award = 0;
   Config config;
   ConfigID config_id;
   String top_award_config_name;

   ConfigID id_top_award_item = config.getId ("TOP AWARD DENOM", game_configuration_identifier, false);
   if (id_top_award_item != INVALID_CONFIGID)
   {
      value = config.getValue (id_top_award_item);
   }
   else
   {
      // Find the denomination associated with the highest top award out of all the denomination top award.
      ConfigID denomination_list_id = config.getId ("GAME DENOMINATION LIST");
      uint32 total_bits = config.getListSize (denomination_list_id);
      for (uint32 bit_index = 0; bit_index < total_bits; ++bit_index)
      {
         top_award_config_name = "DENOM ";
         top_award_config_name << bit_index + 1 << " TOP AWARD";
         config_id = config.getId (top_award_config_name, game_identifier.asString (), false);
         if (config_id != INVALID_CONFIGID)
         {
            uint64 denom_top_award = config.getValue (config_id);
            if (denom_top_award > top_award)
            {
               top_award = denom_top_award;
               // Extract the denomination associated with the nth set bit from the denom list.
               String denomination_string;
               config.getListItem (denomination_list_id, bit_index, denomination_string);
               value = denomination_string.asUint32 ();
            }
         }
      }
   }

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> (WIN_UP_TO_DENOMINATION, resource_list->resourceKey (), property, false))
      {
         String current_denomination_value ("");
         // Used to format amount display strings.
         ULocale locale;
         Amount::AmountValue whole_value;
         // Format minimum bet
         CountryCode::Country country = locale.moneyNativeCountryCode ();
         locale.moneyGetUnitWholeValue (country, whole_value);
         if (value >= whole_value)
         {
            locale.moneyFormatNumericWhole (country, value, current_denomination_value, true /* omit base units if they're 0 */);
         }
         else
         {
            locale.moneyFormatNumericBase (country, value, current_denomination_value);
         }
         property->setAbsoluteValue (current_denomination_value);
      }
   }
}

// Purpose:  Update the WinUpToMaxBetMeter with the given value.
void SlotPresentation::updateWinUpToMaxBetMeter (void)
{
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> (WIN_UP_TO_MAX_BET_METER, resource_list->resourceKey (), property, false))
      {
         int64 value = 0;
         Mojo <GameSideBetInterface> game_side_bet_interface (bet);
         if (game_side_bet_interface.isValid ())
         {
            value = game_side_bet_interface->getMaxBetValue ();
         }
         else
         {
            Mojo <RuleBasedGameSideBetInterface> rule_based_game_bet_interface (bet);
            if (rule_based_game_bet_interface.isValid ())
            {
               value = rule_based_game_bet_interface->getMaxBetValue ();
            }
         }
         property->setAbsoluteValue (value);
      }
   }
}

// Purpose:  Update the WinUpToMaxWinMeter with the given value.
void SlotPresentation::updateWinUpToMaxWinMeter (void)
{
   Amount value = ZERO_AMOUNT;
   uint64 top_award = 0;
   Config config;
   ConfigID config_id;
   String top_award_config_name;

   ConfigID id_top_award_item = config.getId ("TOP AWARD CURRENT", game_configuration_identifier, false);
   if (id_top_award_item != INVALID_CONFIGID)
   {
      top_award = config.getValue (id_top_award_item);
      value = Amount (top_award, bet->getDenomination ());
   }
   else
   {
      // Find the top award associated with the current bet's denomination.
      ConfigID denomination_list_id = config.getId ("GAME DENOMINATION LIST");
      uint32 total_bits = config.getListSize (denomination_list_id);
      for (uint32 bit_index = 0; bit_index < total_bits; ++bit_index)
      {
         top_award_config_name = "DENOM ";
         top_award_config_name << bit_index + 1 << " TOP AWARD";
         config_id = config.getId (top_award_config_name, game_identifier.asString (), false);
         if (config_id != INVALID_CONFIGID)
         {
            String denomination_string;
            config.getListItem (denomination_list_id, bit_index, denomination_string);
            if (bet->getDenomination ().getValue () == denomination_string.asUint32 ())
            {
               // top award for current denomination has been found.
               top_award = config.getValue (config_id);
               value = Amount (top_award, bet->getDenomination ());
               break;
            }
         }
      }
   }

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> (WIN_UP_TO_MAX_WIN_METER, resource_list->resourceKey (), property, false))
      {
         Amount::Extractor amount_extractor (value);

         // Update the resource with the current multiplier value.
         Amount::AmountValue new_value = amount_extractor.extract (denomination);
         property->setAbsoluteValue (new_value);
      }
   }
}

// Purpose:  Update the MaximumWinUpToMaxWinMeter with the given value.
void SlotPresentation::updateMaximumWinUpToMaxWinMeter (void)
{
   Amount value = ZERO_AMOUNT;
   uint64 top_award = 0;
   Config config;
   ConfigID config_id;
   String top_award_config_name;

   ConfigID id_top_award_item = config.getId ("TOP AWARD OVERALL", game_configuration_identifier, false);
   if (id_top_award_item != INVALID_CONFIGID)
   {
      top_award = config.getValue (id_top_award_item);
      value = Amount (top_award, bet->getDenomination ());
   }
   else
   {
      // Find the highest top award out of all the denomination top award.
      ConfigID denomination_list_id = config.getId ("GAME DENOMINATION LIST");
      uint32 total_bits = config.getListSize (denomination_list_id);
      for (uint32 bit_index = 0; bit_index < total_bits; ++bit_index)
      {
         top_award_config_name = "DENOM ";
         top_award_config_name << bit_index + 1 << " TOP AWARD";
         config_id = config.getId (top_award_config_name, game_identifier.asString (), false);
         if (config_id != INVALID_CONFIGID)
         {
            uint64 denom_top_award = config.getValue (config_id);
            if (denom_top_award > top_award)
            {
               top_award = denom_top_award;
               value = Amount (top_award, bet->getDenomination ());
            }
         }
      }
   }

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the bonus bet if the denom or the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      // Extract the property associated with the multiplier from the resource list.
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.  Refer to
      // steps 1 and 2 above.
      if (resource_list->getResource<PropertyInterface> (MAXIMUM_WIN_UP_TO_MAX_WIN_METER, resource_list->resourceKey (), property, false))
      {
         Amount::Extractor amount_extractor (value);

         // Update the resource with the current multiplier value.
         Amount::AmountValue new_value = amount_extractor.extract (denomination);
         property->setAbsoluteValue (new_value);
      }
   }
}

// Purpose:  Update the BonusXWinUpToString with the given property_string.
void SlotPresentation::updateBonusWinUpToString (void)
{
   Config config;
   ConfigID bonus_count_config_id = config.getId ("BONUS COUNT", game_identifier.asString ());

   // update the bonus top award for each config item
   for (uint32 bonus_index = 1; bonus_index < config.getValue (bonus_count_config_id) + 1; ++bonus_index)
   {
      uint32 bonus_top_award = 0;
      String bonus_top_award_config_name;
      ConfigID config_id;

      // Find the bonus top award associated with the current bet's denomination.
      ConfigID denomination_list_id = config.getId ("GAME DENOMINATION LIST");
      uint32 total_bits = config.getListSize (denomination_list_id);
      for (uint32 bit_index = 0; bit_index < total_bits; ++bit_index)
      {
         // Try to get the current bonus award.
         bonus_top_award_config_name = "BONUS ";
         bonus_top_award_config_name << bonus_index << " TOP AWARD";
         config_id = config.getId (bonus_top_award_config_name, game_identifier.asString (), false);

         // If that didn't work, try to get the bonus award for this particular denom.
         if (config_id == INVALID_CONFIGID)
         {
            bonus_top_award_config_name = "BONUS ";
            bonus_top_award_config_name << bonus_index << " DENOM ";
            bonus_top_award_config_name << bit_index + 1 << " TOP AWARD";
            config_id = config.getId (bonus_top_award_config_name, game_identifier.asString (), false);
         }

         if (config_id != INVALID_CONFIGID)
         {
            String denomination_string;
            config.getListItem (denomination_list_id, bit_index, denomination_string);
            if (bet->getDenomination ().getValue () == denomination_string.asUint32 ())
            {
               // top award for current denomination has been found.
               bonus_top_award = config.getValue (config_id);
               break;
            }
         }
      }

      // string to be displayed on the bonus page
      String bonus_win_string = "";
      if (bonus_top_award)
      {
         String bonus_win_up_to_string;
         String config_bonus_id_win_up_to_string;
         
         config_bonus_id_win_up_to_string = "BONUS ";
         config_bonus_id_win_up_to_string << bonus_index << " " << CONFIG_ID_WIN_UP_TO_BONUS_WIN_STRING;
         
         // First try for BONUS X WIN UP TO BONUS WIN STRING
         ConfigID bonus_win_up_to_string_id = config.getId (config_bonus_id_win_up_to_string, theme_configuration_identifier, false);
         
         // If that didn't work, try the default: WIN UP TO BONUS WIN STRING
         if (bonus_win_up_to_string_id == INVALID_CONFIGID)
         {
            bonus_win_up_to_string_id = config.getId (CONFIG_ID_WIN_UP_TO_BONUS_WIN_STRING, theme_configuration_identifier, false);
         }
         
         if (bonus_win_up_to_string_id != INVALID_CONFIGID)
         {
            uint32 maximum_bet = 0;
            Mojo <GameSideBetInterface> game_side_bet_interface (bet);
            Mojo<RuleBasedGameSideBetInterface> rule_based_game_side_bet_interface (bet);

            if (doesBetSupportGameSideBetInterface () && game_side_bet_interface.isValid ())
            {
               maximum_bet = game_side_bet_interface->getMaxBetValue ();
            }
            else if (!doesBetSupportGameSideBetInterface () && rule_based_game_side_bet_interface.isValid ())
            {
               maximum_bet = rule_based_game_side_bet_interface->getMaxBetValue();
            }

            config.getValueString (bonus_win_up_to_string_id, bonus_win_up_to_string);

            // Message to be displayed: WIN UP TO X CREDITS WITH Y CREDITS BET.
            bonus_win_string.sprintf (bonus_win_up_to_string, bonus_top_award, maximum_bet);

            // Check to see if it has more to display.
            ConfigID show_secondary_string_id;
            String config_item_name  = "BONUS ";
            config_item_name << bonus_index << " SHOW WIN UP TO SECONDARY STRING";

            show_secondary_string_id = config.getId (config_item_name, theme_configuration_identifier, false);
            if ((show_secondary_string_id != INVALID_CONFIGID) && config.getValue (show_secondary_string_id))
            {
               String secondary_string;
               ConfigID secondary_string_id = config.getId (CONFIG_ID_WIN_UP_TO_SECONDARY_STRING, theme_configuration_identifier, false);
               if (secondary_string_id != INVALID_CONFIGID)
               {
                  config.getValueString (secondary_string_id, secondary_string);
                  bonus_win_string += "\r\n";
                  bonus_win_string += secondary_string;
               }
            }
         }
      }

      // Resource list from GamePresentation.
      Mojo<ResourceList, ScriptInterface> resource_list;

      String property_name;
      property_name.sprintf (BONUS_X_WIN_UP_TO_STRING, bonus_index);

      // Do not update the bonus bet if the denom or the resource list is not valid yet.
      if (resourceList (resource_list))
      {
         // Extract the property associated with the multiplier from the resource list.
         Mojo<PropertyInterface> property;
         // In the getResource call, the last parameter is optional.  A true value will cause the system
         // to error if the resource can not be retrieved by name and resource key.  An error in this case
         // generally means that the resource has not been registered with the resource manager.  Refer to
         // steps 1 and 2 above.
         if (resource_list->getResource<PropertyInterface> (property_name, resource_list->resourceKey (), property, false))
         {
            // Update the resource with the current value.
            property->setAbsoluteValue (bonus_win_string);
			
			if (okayToUpdateBonusString())
			{			   
			   postEvent ("PaytableAwardPropertiesUpdated");
			}
         }
      }
   }
}

// Purpose:  Determine whether to show the arrows in history.
void SlotPresentation::showArrowsInSlotHistoryState (bool okay_to_show)
{
   show_arrows_in_history = okay_to_show;
}

// Purpose:  Return the flag that determines whether to show the arrows in history.
bool SlotPresentation::getShowArrowsInHistoryFlag ()
{
   return show_arrows_in_history;
}

// Purpose:  Return the flag that determines whether to spin after a power hit in gameplay.
bool SlotPresentation::getPowerhitSpinReelsFlag ()
{
   return powerhit_spin_reels;
}

/// <summary>Scriptable function for creating paytable award properties for use with the legacy evaluator.</summary>
/// <param name = "award_name">[in] The award name for the property.</param>
/// <param name = "progressive_level">[in] The progressive level for the award. If the award is not progressive, then it should be -1.</param>
/// <param name = "insert">[in] If true this property will be an insert.</param>
/// <param name = "category">[in] The win category number for the property.</param>
/// <param name = "bet_group_index">[in] The index of the bet for the property, usually 0.</param>
/// <example><b>Example:</b>
/// @code
/// this:addPaytableAwardProperty ("AnyCombo5W1W2W3W4W5Paging_GDC1", -1, true, true, 1, 0);
/// @endcode
/// </example>
/// <returns></returns>
/// @scriptableFunction{SlotPresentation,addPaytableAwardPropertyLegacy, legacy}
void SlotPresentation::addPaytableAwardPropertyLegacy (const char* award_name,
                                                       uint16 progressive_level, bool insert,
                                                       uint32 category, uint32 bet_group_index)
{
   Mojo<EvaluatorInterface> evaluator_interface;
   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_eval_interface;

   if (gameSideSlotEvaluatorInterface (game_side_slot_eval_interface) == false)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::addPaytableAwardPropertyLegacy(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface.").errorMessage (), whoAreYou ());
      return;
   }

   evaluator_interface = slot_evaluator;
   if (!evaluator_interface.isValid ())
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::addPaytableAwardPropertyLegacy(): EvaluatorInterface is invalid.").errorMessage (), whoAreYou ());
      return;
   }

   Mojo<GameSideBetInterface> game_side_bet_interface (bet);

   uint32 base_pay = evaluator_interface->getWinCredits (category, bet_group_index);
   bool multiply = evaluator_interface->isMultiplier (category, bet_group_index);

   uint32 bet_from = evaluator_interface->getWinBetFrom (category, bet_group_index);
   uint32 bet_to = evaluator_interface->getWinBetTo (category, bet_group_index);

   if (multiply && (bet_from == bet_to) && (bet_from != ULONG_MAX))
   {
      base_pay = bet_from * base_pay;
   }

   Mojo <ParserInterface> parser;
   parser = evaluator_interface;
   if (parser.isValid () == false)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "invalid parser interface").errorMessage (), whoAreYou ());
      return;
   }

   bool needs_multiply = bet_from == ULONG_MAX &&
                         bet_to == ULONG_MAX &&
                         multiply;

   paytable_award_property_list.addAwardPropertyItem (award_name,
                                                      base_pay,
                                                      insert,
                                                      progressive_level,
                                                      needs_multiply,
                                                      "None",
                                                      bet_from,
                                                      bet_to,
                                                      parser->getProgressiveModification(progressive_level));
}

// Purpose: Create list of paytable awardname properties, their base pays and progressive associations.
//          This list gives the slot presentation us an easier and quicker way to update the properties for those awards shown
//          on the glass than having to parse the paytable structures each time.
void SlotPresentation::initializePaytableAwardProperties (const Mojo<SlotEvaluatorInterface> &slot_evaluator_interface)
{
   Mojo<EvaluatorInterface> evaluator_interface;
   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_eval_interface;
   Mojo<GameSideBetInterface> game_side_bet_interface (bet);
   uint32 max_bet_per_sub_bet = game_side_bet_interface.isValid() ? game_side_bet_interface->getMaxBetPerSubBet() : 0;

   if (gameSideSlotEvaluatorInterface (game_side_slot_eval_interface) == false)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::initializePaytableAwardProperties(): SlotEvaluatorInterfaces does not suppot GameSideSlotEvaluatorInterface."));
   }

   evaluator_interface = slot_evaluator_interface;
   if (!evaluator_interface.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::initializePaytableAwardProperties(): EvaluatorInterface is invalid."));

   // Paytable Award Properties in BBE are added staticly by script
   Mojo<BBEPaytableAwardPropertyInterface> slot_bbe (slot_evaluator);
   if (slot_bbe.isValid ())
   {
      return;
   }

   // Ensure the list is empty before adding the award items from the evaluator.
   paytable_award_property_list.clearAwardPropertyList();

   for (uint32 i = 0; i < evaluator_interface->getNumberOfWinCategories(); i++)
   {
      for (uint16 j = 0; j < evaluator_interface->getNumberOfAwardNames(i); j++)
      {
         if (evaluator_interface->isDefaultAwardName(i, j))
         {
            bool is_bet_group_multiplier = false;
            const String awardname(evaluator_interface->getPaytableAwardPropertyName(i, j));
            uint32 base_pay_amount = game_side_slot_eval_interface->getBasePayAmountForAwardNameAndMultiplier(i, j, max_bet_per_sub_bet,
                                                                                                              is_bet_group_multiplier);
            bool is_multiplier = game_side_slot_eval_interface->doesAwardNameRequireMaxBet(i, j) && is_bet_group_multiplier;
            uint16 progressive_level = evaluator_interface->getAwardNameProgressiveLevel(i, j);
            Mojo <ParserInterface> parser;
            parser = evaluator_interface;
            if (parser.isValid () == false)
            {
              System::error (LogicError (LogicError::INVALID_VALUE, "invalid parser interface"));
            }

            paytable_award_property_list.addAwardPropertyItem(awardname,
                                                              base_pay_amount,
                                                              evaluator_interface->isInsertAwardName(i, j),
                                                              progressive_level,
                                                              is_multiplier,
                                                              evaluator_interface->getAwardNameBetString(i, j),
                                                              evaluator_interface->getAwardNameGetBetFrom(i, j),
                                                              evaluator_interface->getAwardNameGetBetTo(i, j),
                                                              parser->getProgressiveModification(progressive_level));
         }
      }
   }
}

// Purpose: Load any neccessary information from the registry files.
void SlotPresentation::loadRegistyData ()
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation:" << __FUNCTION__ << endl;
   #endif

   Registry registry;

   // Open the configuration file
   registry.open (configuration_filename);

   // Get the theme file name
   String relative_themereg_filename;
   assertion (registry.find (relative_themereg_filename, "General", "Theme"));

   // Close the registry file.
   registry.close ();

   String themereg_filename (resource_mount_point);
   themereg_filename << relative_themereg_filename;

   // Now open the theme registry file
   registry.open (themereg_filename);

   // Get the name of the theme.  This will be used to load the correct glass content
   DG_load_event_base.clear ();
   if (registry.find (DG_load_event_base, "General", "Name"))
   {
      // Don't allow spaces in this name.
      DG_load_event_base.strip (' ');
   }

   // Buffer to hold the string of the meter panel.
   String rgb_string;
   // If the entry exists
   if (registry.find (rgb_string, "General", "MeterPanelColor"))
      parseMeterPanelColor (rgb_string);

   // Close the registry file.
   registry.close ();
}

// Purpose: Parse a string and set the meter panel color shift properties accordingly.
void SlotPresentation::parseMeterPanelColor (String &rgb_string)
{
   String* color_values (0);
   // Retrieve the number of values in the string.
   uint32 value_count = StringUtils::getDelimitedStrings (rgb_string, ',', color_values, true);
   // Only accept a specific number of color values, otherwise throw an error.
   if (value_count == NUMBER_OF_COLOR_VALUES)
   {
      // Convert the decimal numbers to a percentage.
      float converted_values [NUMBER_OF_COLOR_VALUES];
      for (uint8 index = 0; index < NUMBER_OF_COLOR_VALUES; index++)
      {
         if (color_values [index].asInt16 () > MAX_COLOR_VALUE || color_values [index].asInt16 () < 0)
            System::error (LogicError (LogicError::INVALID_VALUE, "MeterPanelColor value out of range in themereg file."));
         else
            converted_values [index] = (color_values [index].asInt16 () / MAX_COLOR_VALUE);

      }

      setMeterPanelColorShift (converted_values [0],
                                converted_values [1],
                                converted_values [2]);
   }
   else
   {
      String error_string;
      error_string << "MeterPanelColor value count mismatch in themereg file.";
      System::error (LogicError (LogicError::INVALID_VALUE, error_string));
   }
   // Clean the dynamically allocated color_values.
   StringUtils::deleteDelimitedStringsBuffer (color_values);
}

/// <summary> Updates the paytable award properties and the spectrum attached to the given progressive level. </summary>
/// <remarks> This method differs from updatePaytableAwardProperties(bool) in that only a single spectrum is 
/// updated, and no multimedia events are posted. </remarks>
/// <param name="progressive_update">[in] Flag indicating whether this is a progressive update. </param>
/// <param name="progressive_level">[in] The progressive level for the spectrum to update. </param>
/// <param name="progressive_properties_updated">[out] Set true if progressive paytable properties were updated </param>
/// <param name="non_progressive_properties_updated">[out] Set true if non progressive paytable properties were updated. </param>
void SlotPresentation::updatePaytableAwardProperties(bool progressive_update, uint32 progressive_level,
                                                     bool &progressive_properties_updated, 
                                                     bool &non_progressive_properties_updated)
{
   GamePresentation::updatePaytableAwardProperties(progressive_update, progressive_level,
                                                   progressive_properties_updated, non_progressive_properties_updated);
   GamePresentation::assignVirtualSpectrumLevelAssociations ();

   ProgressiveGameId progressive_game_id (game_identifier, stage_index);

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;

   if (resourceList (resource_list))
   {
      Denomination controller_denom = Denomination::BLANK;
      ProgressiveControllerId controller_id;
      uint32 controller_level;
      bool is_paytable_formed;
      uint32 list_position = 0;

      // Checking if this progressive level is linked
      bool linked = progressive_manager.getGameLink (progressive_game_id, progressive_level, controller_id,
                                                     controller_level, is_paytable_formed, list_position, denomination,
                                                     controller_denom);

      // If the paytable is linked don't update the properties unless this is a
      // progressive update, otherwise wait for one.
      // If there are no links, then it is safe to update.
      if ((progressive_update && linked) || !linked)
      {
         list<GamePresentation::VirtualSpectrumAssociation>::const_iterator index;
         for (index = virtual_spectrum_associations.begin (); index != virtual_spectrum_associations.end (); ++index)
         {
            if (index->virtual_spectrum_level == progressive_level)
               break;
         }

         if (index != virtual_spectrum_associations.end ())
         {
            // Updating spectrum
            progressive_properties_updated = paytable_award_property_list.updateSpectrumProperty (index->virtual_spectrum_level,
                                                                                                  index->linked_progressive_game_level,
                                                                                                  resource_list, bet, getDenomination());

            // Updating paytable properties
            paytable_award_property_list.updatePaytableProperties(resource_list, getDenomination(),
                                                                  progressive_game_id,
                                                                  game_configuration_identifier,
                                                                  theme_configuration_identifier,
                                                                  bet, &progressive_properties_updated,
                                                                  &non_progressive_properties_updated);
         }
         setOkayToUpdateBonusString (true);
      }
   }
}

// Purpose: Update the properties contained within the paytable awardname property list with
//          any new multiplier, denom or progressive values.
void SlotPresentation::updatePaytableAwardProperties (bool progressive_update)
{
   // Base Class
   GamePresentation::updatePaytableAwardProperties ();

   ProgressiveGameId progressive_game_id (game_identifier, stage_index);

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;

   if (resourceList (resource_list))
   {
      GamePresentation::assignVirtualSpectrumLevelAssociations ();

      bool linked_progressives = false;
      uint32 number_of_levels = progressive_manager.numberOfLevels (progressive_game_id, denomination);
      Denomination controller_denom = Denomination::BLANK;
      ProgressiveControllerId controller_id;
      uint32 controller_level;
      bool is_paytable_formed;
      uint32 list_position = 0;
      // Check and see if there are any progressives linked.
      for (uint32 level_index = 0; level_index < number_of_levels; ++level_index)
      {

         linked_progressives |= progressive_manager.getGameLink (progressive_game_id, level_index, controller_id,
                                                                 controller_level, is_paytable_formed, list_position, denomination,
                                                                 controller_denom);
      }

      // If the paytable is linked don't update the properties unless this is a
      // progressive update, otherwise wait for one.
      // If there are no links, then it is safe to update.
      if ((linked_progressives && progressive_update) || !linked_progressives)
      {
         bool progressive_properties_updated = false;
         bool non_progressive_properties_updated = false;

         // updating all spectrums
         list<GamePresentation::VirtualSpectrumAssociation>::const_iterator index;
         for (index = virtual_spectrum_associations.begin (); index != virtual_spectrum_associations.end (); ++index)
         {
            progressive_properties_updated = paytable_award_property_list.updateSpectrumProperty (index->virtual_spectrum_level,
                                                                                                  index->linked_progressive_game_level,
                                                                                                  resource_list, bet, getDenomination());
         }

         paytable_award_property_list.updatePaytableProperties(resource_list, getDenomination(),
                                                               progressive_game_id,
                                                               game_configuration_identifier,
                                                               theme_configuration_identifier,
                                                               bet, &progressive_properties_updated,
                                                               &non_progressive_properties_updated);

         if (progressive_properties_updated)
         {
            postEventAll ("ProgressivePaytableAwardPropertiesUpdated");
         }
         if (non_progressive_properties_updated)
         {
            postEventAll ("PaytableAwardPropertiesUpdated");
         }
         setOkayToUpdateBonusString (true);
      }
   }
}

// Purpose: Function appends more information on to the end of a "ShowDigitalGlass" event
//          based on any progressive links that currently exist.  Used for switching between
//          non-progressive and progressive digital glass.
void SlotPresentation::postShowDigitalGlassEvents (void)
{
   uint32 number_of_links = virtual_spectrum_associations.size();
   String show_digital_glass_event("ShowDigitalGlass:");

   if (number_of_links)
   {
      show_digital_glass_event << "Progressive";
      postEventContext(show_digital_glass_event, "Context.DigitalGlass");
      show_digital_glass_event << ":" << number_of_links;
      postEventContext(show_digital_glass_event, "Context.DigitalGlass");
   }
   else
   {
      show_digital_glass_event << "NonProgressive";
      postEventAll(show_digital_glass_event);
   }
}


// Purpose: Update the current digital glass being displayed.  Here we will post
//          the appropriate load event for the digital glass.  The corresponding theme's
//          models will then be configured.
//
//          This function is generally used for cases of 'stacked themes' in which multiple
//          pieces of content work with the same piece of math.
void SlotPresentation::updateDigitalGlass (void)
{
   // if the current glass name is not the same as the new glass name
   // post the load event.
   if (current_glass_name != DG_load_event_base)
   {
      String digital_glass_event ("DigitalGlassPaytable:");
      digital_glass_event << DG_load_event_base << ":Load";
      postEventContext (digital_glass_event, "Context.DigitalGlass");
      current_glass_name = DG_load_event_base;
   }
}


// Purpose: Determine if we can start the game.
void SlotPresentation::postHandleMessage (void)
{
    bool unlock = bet->canStartGame ();
    postEvent (unlock ? "SlotPresentation:UnlockHandle" : "SlotPresentation:LockHandle");
}

// Purpose: Overriden to properly set the SlotPresentation member slot_evaluator
bool SlotPresentation::evaluatorForPaytableSection (Mojo<EvaluatorInterface> &evaluator, const char *section)
{
   // Call our parent to set the evaluator Mojo reference.
   // Find the evaluator to use for the given paytable section. Returns true on success.
   bool return_val = GamePresentation::evaluatorForPaytableSection (evaluator, section);
   // Now update our slot evaluator member
   //    If the evaluator is invalid, the slot_evaluator should also be invalid.
   slot_evaluator = evaluator;
   return return_val;
}

// Determine if the game menu buttons should be shown or not
// these buttons include Game Menu, Small Game Menu, Volume and More Games
void SlotPresentation::synchronizeGameMenuButtons (void)
{
   IGameControlPanelExtractor igame_control_panel;
   if (controlPanel (igame_control_panel))
   {
      // If player selectable games, use the small game menu button to allow room for more games button.
      if (numberOfAvailablePlayerGames () > 1)
      {
         // hide the game menu and volume buttons
         igame_control_panel.getIGameControlPanelInterface ()->showGameMenuButton (false);
         synchronizeVolumeButton (show_volume_button_with_more_games_enabled);

         // Show the Small Game Menu Button and The More Games Button
         igame_control_panel.getIGameControlPanelInterface ()->showGameMenuButtonSmall (true);

         // MoreGames button should never be shown in tournament mode.
         igame_control_panel.getIGameControlPanelInterface ()->showMoreGamesButton ((!(SystemFlags ().systemInTournamentMode ())));
      }
      else
      {
         // hide the Small Game Menu Button and The More Games Button
         igame_control_panel.getIGameControlPanelInterface ()->showGameMenuButtonSmall (false);
         igame_control_panel.getIGameControlPanelInterface ()->showMoreGamesButton (false);

         // Show the Game Menu Button
         igame_control_panel.getIGameControlPanelInterface ()->showGameMenuButton (true);
         // Show the volume button
         synchronizeVolumeButton (true);
      }
   }
}

// Synchronize 'Win Up To' displays
void SlotPresentation::synchronizeWinUpToItems (void)
{
   ConfigID id;
   //Check if show game win up amount in seepays.
   id = config.getId (CONFIG_ID_SUPPORT_WIN_UP_TO_SEEPAYS, theme_configuration_identifier, false);
   if ((id != INVALID_CONFIGID) && config.getValue (id))
   {
      updateWinUpToMaxBetMeter ();
      updateWinUpToMaxWinMeter ();
   }

   //Check if show game win up amount in attract.
   id = config.getId (CONFIG_ID_SUPPORT_WIN_UP_TO_ATTRACT, theme_configuration_identifier, false);
   if ((id != INVALID_CONFIGID) && config.getValue (id))
   {
      updateWinUpToDenomination ();
      updateMaximumWinUpToMaxWinMeter ();
   }

   //Check if show bonus win up amount in seepays.
   id = config.getId (CONFIG_ID_SUPPORT_WIN_UP_TO_BONUS_SEEPAYS, theme_configuration_identifier, false);
   if ((id != INVALID_CONFIGID) && config.getValue (id))
   {
      updateBonusWinUpToString ();
   }
}

// Purpose: Set the meter panel color shift properties.
void SlotPresentation::setMeterPanelColorShift (float red_value, float green_value, float blue_value)
{
   Mojo<ResourceList, ScriptInterface> resource_list;
   if (resourceList (resource_list))
   {
      Mojo<PropertyInterface> red_value_property;
      Mojo<PropertyInterface> green_value_property;
      Mojo<PropertyInterface> blue_value_property;

      if (resource_list->getResource<PropertyInterface> (METER_PANEL_COLOR_RED_VALUE, resource_list->resourceKey (), red_value_property, false))
      {
         red_value_property->setAbsoluteValue (red_value);
      }
      if (resource_list->getResource<PropertyInterface> (METER_PANEL_COLOR_GREEN_VALUE, resource_list->resourceKey (), green_value_property, false))
      {
         green_value_property->setAbsoluteValue (green_value);
      }
      if (resource_list->getResource<PropertyInterface> (METER_PANEL_COLOR_BLUE_VALUE, resource_list->resourceKey (), blue_value_property, false))
      {
         blue_value_property->setAbsoluteValue (blue_value);
      }
   }
}

// Does the bet allow us to start a game.
bool SlotPresentation::checkForDoubleUpEnable ()
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::checkForDoubleUpEnable:" << __FUNCTION__ << endl;
   #endif

   return (!rollupSlamButtonSet () && GamePresentation::checkForDoubleUpEnable ());
}

// Check to see whether an RMLP bonus generated an award
bool SlotPresentation::checkForRmlpAward ()
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::checkForRmlpAward:" << __FUNCTION__ << endl;
   #endif
   
   // Get current award information.
   Mojo<AwardInterface> award_list;
   readAwardFromEssentialRecord (award_list);
   uint16 award_index      = 0;
   uint16 number_of_awards = award_list->getNumberOfAwards ();
   bool   rmlp_win_awarded = false;
   
   for (award_index = 0; award_index < number_of_awards; ++award_index)
   {
      Mojo<SystemAwardInterface> system_award;
      if (award_list->getAward (award_index, system_award))
      {
         char const *this_award_name = system_award->getName ();
         char const *rmlp_award_name = "RMLPAward";
         if (!strcmp(rmlp_award_name, this_award_name))
         {
            rmlp_win_awarded = true;
            break;
         }
      }
   }
   return rmlp_win_awarded;
}

// Get the RMLP bonus award amount, returns 0 if no award
uint32 SlotPresentation::getRmlpAwardAmount ()
{
   #ifdef __VERBOSE__
   debug << "SlotPresentation::getRmlpAwardAmount:" << __FUNCTION__ << endl;
   #endif
   
   // Get current award information.
   Mojo<AwardInterface> award_list;
   readAwardFromEssentialRecord (award_list);
   Amount rmlp_award_amount (ZERO_AMOUNT);
   uint32 rmlp_extracted_value = 0;
   
   for (uint16 award_index = 0; award_index < award_list->getNumberOfAwards (); ++award_index)
   {
      Mojo<SystemAwardInterface> system_award;
      if (award_list->getAward (award_index, system_award))
      {
         char const *this_award_name = system_award->getName ();
         char const *rmlp_award_name = "RMLPAward";
         if (!strcmp(rmlp_award_name, this_award_name))
         {
            system_award->getTotalAwardAmount (rmlp_award_amount);
            rmlp_extracted_value += static_cast<uint32>(Amount::Extractor (rmlp_award_amount).extract (getDenomination ()));
            break;
         }
      }
   }
   return rmlp_extracted_value;
}

//Purpose: Toggle between Double Up button and Language button based on win.
void SlotPresentation::updateDoubleUpButton (void)
{
   GameControlPanelExtractor game_control_panel;
   bool control_panel_enabled (false);
   if (GamePresentation::controlPanel (game_control_panel))
      control_panel_enabled = game_control_panel.getControlPanelInterface ()->isEnabled ();

   if (control_panel_enabled)
   {
      if (checkForDoubleUpEnable ())
      {
         postEvent ("EnableDoubleUpButton");
      }
      else
      {
         postEvent ("DisableDoubleUpButton");
      }
   }
   else
   {
      if (checkForDoubleUpEnable ())
      {
         postEvent ("DoubleUpButton:Dim");
      }
      else
      {
         postEvent ("DisableDoubleUpButton");
      }
   }
}
// rollup any double up wins to the credit meter...
void SlotPresentation::rollupDoubleUpWin (void)
{
   // Make adjustments to the rollup starting point based on any applicable bonus wins.
   Amount credit_amount (ZERO_AMOUNT);
   Amount total_win_amount_from_award (ZERO_AMOUNT);
   Amount total_win (ZERO_AMOUNT);

   // Retrieve the current credit information.  This value mirrors the credits held by the flow.
   creditDisplayAmount (credit_amount);
   // Synchronize transfer button to enable transfer money.
   synchronizeTransferButton ();
   // Extract the total win values from base game.
   extractWinForThisAct (total_win);
   // Extract the total win values from the award.
   extractTotalWinFromAward (total_win_amount_from_award);

   // Update the control panel win meter display with the bonus win.  This ensures that the bonus
   // wins are only rolled up in the bonus (and not rolled up again in the base game).
   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      if (total_win_amount_from_award > ZERO_AMOUNT)
      {
         // Check for Take Win or if the meters have already rolled.
         if (!doubleUpWin ())
         {
            // Snap Meters
            game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount_from_award, RollupDefs::SNAP_TO_AMOUNT);
            game_control_panel.getGameControlPanelInterface ()->setCreditMeter (credit_amount, RollupDefs::SNAP_TO_AMOUNT);
         }
         else
         {
            //We are going to Dbl Up rollup

            // Play DoubleUpRollup sound if a Handpay is not triggered
            if (!winWasCashedOut () && doubleUpWin ())
               postEvent ("PlayRollupSound:DoubleUpRollup");

            // Set each meter's rollup target and start the rollup.
            game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount_from_award, RollupDefs::ROLLUP_SLOW);
            // Set each meter's rollup target and start the rollup.
            game_control_panel.getGameControlPanelInterface ()->setCreditMeter (credit_amount, RollupDefs::ROLLUP_SLOW);
         }
         // If there are any interruptions after this point, then snap on synchronize.
         setDoubleUpRollupNeededFlag (false);
      }
   }
   synchronizeBet ();
}

// Set the double up flag to roll/snap the win meter.
void SlotPresentation::setDoubleUpRollupNeededFlag (bool set)
{
   doubleup_rollup_needed = set;
}

// Returns true if all reels in the given reel_set are of SlotEssentialRecord::STANDARD type
bool SlotPresentation::isStandardReelsType (uint32 reel_set)
{
   const SlotEssentialRecord* essential_record = getSlotEssentialRecord ();
   assertion (essential_record);
   return (essential_record->getReelsType(reel_set) == SlotEssentialRecord::STANDARD);
}

// Set the segments of the non-standard given reel to the positions in the Essential Record
void SlotPresentation::setSegmentsToPosition (uint32 reel_set_index, uint32 reel_index)
{
   #ifdef __VERBOSE__
   debug << __FUNCTION__ << " - Reel Set:" << reel_set_index << " - Reel: " << reel_index << endl;
   #endif
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setSegmentsToPosition (), Called before essential record is initialized."));
   }
   // Cache a reels mojo. Make sure it's valid.
   Mojo<ReelsInterface> reels_interface;
   if (!getReels (reels_interface, reel_set_index))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setSegmentsToPosition (), ReelsInterface is invalid."));
   }

   // Set the case number for this reel
   {
      Mojo<PropertyInterface> multistop_reel_property;
      String property_name ("Reel");
      property_name << reel_index << ":MultiStopCaseNumber";
      if (resource_list->getResource<PropertyInterface> (property_name, resource_list->resourceKey (), multistop_reel_property, false))
      {
         multistop_reel_property->setAbsoluteValue ((uint32)slot_essential_record->getWinFlags(reel_set_index, reel_index));
      }
   }

   // Use a video reels interface since a non-standard reel must be of video type
   Mojo<VideoReelsInterface> video_reels_interface (reels_interface);
   if (!video_reels_interface.isValid ())
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setSegmentsToPosition (), video reels not found.").errorMessage (), whoAreYou ());
      return;
   }

   video_reels_interface->setIsFirstAct (!processConditionIsActType("FREE"));
   uint32 _number_of_visible_symbols = numberOfVisibleSymbols (reel_set_index, reel_index);
   vector<uint32> visible_symbol_stops;
   for (uint32 symbol_index = 0; symbol_index < _number_of_visible_symbols; ++symbol_index)
   {
      uint32 _symbol_stop = slot_essential_record->reelStop (reel_set_index, reel_index, symbol_index);

      visible_symbol_stops.push_back(_symbol_stop);
   }

   const SlotEssentialRecord::Reel *reel = slot_essential_record->reel (reel_set_index, reel_index);

   if (reel)
   {

      // This function takes in the Reel Index, the MultiStop case number, the virtual stop and a list 
      // of stops for each segment. The MultiStop case number is really a bit field in which each bit 
      // represents a visible reel segment with bit 0 corresponding to visible segment index 0. If a 
      // bit is set to ‘0’ that is signifying that the symbol on the corresponding segment was not part 
      // of the win and will not be removed or replaced. If a bit is set to ‘1’ that is signifying that 
      // the symbol on the corresponding segment was part of the win and will be removed from the segment 
      // creating a “hole” that a non winning or new symbol will fall down to fill.
	   video_reels_interface->updateMultiStopSegments (reel_index, 
	         (uint32)slot_essential_record->getWinFlags(reel_set_index, reel_index),
	         reel->getVirtualStop (),
	         visible_symbol_stops);
   }
   else
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::setSegmentsToPosition (), reel not found.").errorMessage (), whoAreYou ());
      return;
   }
}

// Force the segments of the non-standard given reel to the positions in the Essential Record
void SlotPresentation::snapSegmentsToPosition (uint32 reel_set_index, uint32 reel_index)
{
   #ifdef __VERBOSE__
   debug << __FUNCTION__ << " - Reel: " << reel_index << endl;
   #endif
   // Get the slot essential record.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   if (!slot_essential_record)
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::snapSegmentsToPosition (), Called before essential record is initialized.").errorMessage (), whoAreYou ());
      return;
   }
   // Cache a reels mojo. Make sure it's valid.
   Mojo<ReelsInterface> reels_interface;
   if (!getReels (reels_interface, reel_set_index))
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::snapSegmentsToPosition (), ReelsInterface is invalid.").errorMessage (), whoAreYou ());
      return;
   }

   // Set the case number for this reel
   {
      Mojo<PropertyInterface> multistop_reel_property;
      String property_name ("Reel");
      property_name << reel_index << ":MultiStopCaseNumber";
      if (resource_list->getResource<PropertyInterface> (property_name, resource_list->resourceKey (), multistop_reel_property, false))
      {
         multistop_reel_property->setAbsoluteValue ((uint32)slot_essential_record->getWinFlags(reel_set_index, reel_index));
      }
   }

   // Use a video reels interface since a non-standard reel must be of video type
   Mojo<VideoReelsInterface> video_reels_interface (reels_interface);
   if (!video_reels_interface.isValid ())
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::snapSegmentsToPosition (), video reels not found.").errorMessage (), whoAreYou ());
      return;
   }

   video_reels_interface->setIsFirstAct (!processConditionIsActType("FREE"));
   uint32 _number_of_visible_symbols = numberOfVisibleSymbols (reel_set_index, reel_index);
   vector<uint32> visible_symbol_stops;
   for (uint32 symbol_index = 0; symbol_index < _number_of_visible_symbols; ++symbol_index)
   {
      uint32 _symbol_stop = slot_essential_record->reelStop (reel_set_index, reel_index, symbol_index);

      visible_symbol_stops.push_back(_symbol_stop);
   }

   const SlotEssentialRecord::Reel *reel = slot_essential_record->reel (reel_set_index, reel_index);

   if (reel)
   {
      // This function takes in the Reel Index, the MultiStop case number, the virtual stop and a list 
      // of stops for each segment. The MultiStop case number is really a bit field in which each bit 
      // represents a visible reel segment with bit 0 corresponding to visible segment index 0. If a 
      // bit is set to ‘0’ that is signifying that the symbol on the corresponding segment was not part 
      // of the win and will not be removed or replaced. If a bit is set to ‘1’ that is signifying that 
      // the symbol on the corresponding segment was part of the win and will be removed from the segment 
      // creating a “hole” that a non winning or new symbol will fall down to fill.
      video_reels_interface->snapUpdateMultiStopSegments (reel_index, 
         (uint32)slot_essential_record->getWinFlags(reel_set_index, reel_index),
         reel->getVirtualStop (),
         visible_symbol_stops);
   }
   else
   {
      setErrorString (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::snapSegmentsToPosition (), reel not found.").errorMessage (), whoAreYou ());
      return;
   }
}

// If the award state animation/rollup was slammed set the class member flag denoting it.
void SlotPresentation::resetRollupSlamButton ()
{
   #ifdef __VERBOSE__
      debug << "SlotPresentation::resetRollupSlamButton ()" << endl;
   #endif

   rollup_slam_button_set = false;
}

// Check if the game is configured to auto start if previous games rollup was slammed and request start if appropriate.
bool SlotPresentation::autoStartGameOnRollupSlam (void)
{
   #ifdef __VERBOSE__
      debug << "SlotPresentation::autoStartGameOnRollupSlam ()" << endl;
   #endif

   bool autostart_game = false;

   if (autoStartGameOnRollupSlamAllowed ())
   {
      // Turn off the slam lamps
      String disable_rollup_slam_lamps ("autoStartGameOnRollupSlam:Lamps:Disable");
      postEventAll (disable_rollup_slam_lamps);

      // If the game has been slammed
      if (rollupSlamButtonSet ())
      {
         autostart_game = true;
         String button_string = getRollupSlamButton ();
         uint32 bet_n_coins_button_string_length = strlen (BET_N_COINS_BUTTON);

         // If this is a Bet N Coins button
         if (!strncmp (button_string, BET_N_COINS_BUTTON, bet_n_coins_button_string_length))
         {
            String button_number_string = button_string.right (strlen (button_string) - bet_n_coins_button_string_length);
            uint16 button_number = atoi (button_number_string);
            if (button_number > 0)
            {
               UserSequenceEvent bet_request_event (UserSequenceEvent::TO_GAME_MANAGER,
                                                    UserSequenceEvent::BET_REQUEST,
                                                    UserSequenceEvent::BET_N_COINS_REQUEST,
                                                    button_number);
               postEvent (bet_request_event);
            }
         }
         else if (!strcmp (button_string, STOP_BUTTON))
         {
            // Auto start request repeat bet switch.
            UserSequenceEvent bet_request_event (UserSequenceEvent::TO_GAME_MANAGER,
                                                 UserSequenceEvent::PLAY_REQUEST,
                                                 UserSequenceEvent::START_GAME_REQUEST);
            postEvent (bet_request_event);
         }
         else if (!strcmp (button_string, MAX_BET_BUTTON))
         {
            // Auto start request max bet switch.
            UserSequenceEvent bet_request_event (UserSequenceEvent::TO_GAME_MANAGER,
                                                 UserSequenceEvent::BET_REQUEST,
                                                 UserSequenceEvent::MAX_BET_REQUEST);
            postEvent (bet_request_event);
         }

         // Skip the double up mode.
         postEvent ("DoTakeWin:Requested");
      }
   }

   #ifdef __VERBOSE__
      debug << "SlotPresentation::autoStartGameOnRollupSlam (), complete" << endl;
   #endif

   return (autostart_game);
}

//Check to see if we are allowed to auto start a game after a credit rollup slam is enabled
bool SlotPresentation::autoStartGameOnRollupSlamAllowed (void)
{
   #ifdef __VERBOSE__
      debug << "SlotPresentation::autoStartGameOnRollupSlamAllowed ()" << endl;
   #endif

   Config config;
   bool auto_start_allowed (false);

   ConfigID auto_start_allowed_id (config.getId (CONFIG_ID_AUTO_START_GAME_ON_ROLLUP_SLAM, theme_configuration_identifier, false));
   if (auto_start_allowed_id != INVALID_CONFIGID)
   {
      // Auto start on rollup slam can be enabled if the configuration item is true and
      // double up is NOT enabled for this theme.
      auto_start_allowed = config.getValue (auto_start_allowed_id) && !doubleUpEnabled ();
   }

   #ifdef __VERBOSE__
      debug << "SlotPresentation::autoStartGameOnRollupSlamAllowed (), complete" << endl;
   #endif

   return (auto_start_allowed);
}

// Check whether slamming mechanical reels is allowed
bool SlotPresentation::slamMechanicalReelsAllowed (void)
{
   return enable_mechanical_slam;
}

// Purpose: Stop every actor in the presentation because the context is suspending or exiting.
void SlotPresentation::suspend (void)
{
   #ifdef __VERBOSE__
      debug << "SlotPresentation::suspend ()" << endl;
   #endif

   // Call down to the base class
   GamePresentation::suspend ();

   // Cancel the rollup slammed flag
   resetRollupSlamButton ();

   #ifdef __VERBOSE__
      debug << "SlotPresentation::suspend, complete" << endl;
   #endif
}

SlotEssentialRecord::ReelType SlotPresentation::getSlotEssentialRecordReelType (uint32 reel_index)
{
   // Get the current essential record information.
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   assertion (slot_essential_record != 0);

   const SlotEssentialRecord::Reel *reel = slot_essential_record->reel (currentReelSet(), (SlotEssentialRecord::ReelIndex)reel_index);
   assertion (reel != 0);

   return reel->getReelType ();
}

/// <summary>Scriptable function for creating paytable award properties for use with the Black Box Evaluator.</summary>
/// <param name = "award_name">[in] The award name for the property.</param>
/// <param name = "insert">[in] If true this property will be an insert.</param>
/// <param name = "prize_scale_name">[in] The prize scale to lookup the prize in.</param>
/// <param name = "prize_name">[in] The name of the prize to use for the property.</param>
/// <param name = "pay_count">[in] The number of symbols which make up the pay.</param>
/// <param name = "win_index">[in] The index of the win amount in the pay.</param>
/// <param name = "progressive_level">
//// [in] The progressive level for the award, _UINT16_MAX for no progressive. In the scriptable
//// form it may be ommitted to indicate no progressive.
/// </param>
/// <example><b>Example:</b>
/// @code
/// this:addPaytableAwardProperty ("Prize_R7_5", false, "PrizeScale", "Prize_R7", 5, 0);
/// this:addPaytableAwardProperty ("Prize_R8_5", false, "PrizeScale", "Prize_R8", 5, 0, 1);
/// @endcode
/// </example>
/// <returns></returns>
/// @scriptableFunction{SlotPresentation,addPaytableAwardPropertyLegacy, BBE}
void SlotPresentation::addPaytableAwardPropertyBBE (const char* award_name,
                                                    bool insert,
                                                    const char* prize_scale_name,
                                                    const char* prize_name,
                                                    uint32 pay_count,
                                                    uint32 win_index,
                                                    uint16 progressive_level)
{
      Mojo<BBEPaytableAwardPropertyInterface> slot_bbe (slot_evaluator);
      if (slot_bbe.isValid ())
      {
         uint32 bet_from = 1;
         uint32 bet_to = ULONG_MAX;
         uint32 base_pay = 0;
         bool multiply = false;

         slot_bbe->getDigitalGlassInfo (prize_scale_name,
                                        prize_name,
                                        pay_count,
                                        win_index,
                                        bet_from,
                                        bet_to,
                                        base_pay,
                                        multiply);

         paytable_award_property_list.addAwardPropertyItem (award_name,
                                                            base_pay,
                                                            insert,
                                                            progressive_level,
                                                            multiply,
                                                            "None",
                                                            bet_from,
                                                            bet_to,
                                                            NONE);
      }
      else
      {
         setErrorString (LogicError(LogicError::INVALID_VALUE, "addPaytableAwardPropertyBBE requires a valid BBE instance.").errorMessage (), whoAreYou ());
         // Once the error occurs, we need to alter the program flow to prevent any code from being executed since
         // the object state is no longer valid due to a fatal error occurring.The following return is to make sure
         // to stop program flow even if some people add any more codes in the future
         return;
      }
}

/// <summary># Scriptable function for exploding symbols.</summary>
/// <returns># void</returns>
/// @scriptableFunction{SlotPresentation,explodeSymbols}
void SlotPresentation::explodeSymbols (void)
{
   uint32 explode_count = 0;
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();

   for (uint16 machine_index = 0; machine_index < getNumberOfMachines (); ++machine_index)
   {
      Mojo<ReelsInterface> reels;
      bool success = getReels (reels, machine_index);
      assertion (success);

      Mojo <SlotEvaluatorInterface> slot_evaluator;
      slotEvaluatorInterface (slot_evaluator, true, machine_index);

      uint32 num_reels = numberOfReels (machine_index);
      for (uint8 reel_num = 0; reel_num < num_reels; ++reel_num)
      {
         //get reel
         const SlotEssentialRecord::Reel* my_reel = slot_essential_record->reel (machine_index, reel_num);
         assertion (my_reel);

         uint32 num_visible_symbols = numberOfVisibleSymbols (machine_index, reel_num);
         for (uint8 seg_num = 0; seg_num < num_visible_symbols; ++seg_num)
         {
            bool is_part_of_win = my_reel->getWinFlag (seg_num);

            if (is_part_of_win)
            {
               //determine symbol name
               uint32 stop_index = my_reel->getStop (seg_num);
               const char* symbol_name = slot_evaluator->getSymbolNamePhysical (reel_num, stop_index);

               //Example event: "PlayExplode:<symbol_name>:<reel_number>:<segment_number>
               String event_to_post = "";
               event_to_post += "PlayExplode:";
               event_to_post << symbol_name;
               event_to_post += ":";
               event_to_post << (int) reel_num;
               event_to_post += ":";
               event_to_post << (int) seg_num;
               postEvent (event_to_post);

               reels->animateStop (reel_num, seg_num, "HideSymbolForExplode");
               explode_count++;
            }
         }
      }
   }

   if (explode_count > 0)
   {
      postEvent ("PlayExplodeSound");
   }
}

/// <summary> Return true if the flow state is utility. </summary>
/// <returns> Return true if the flow state is utility, else return false. <returns>
bool SlotPresentation::isFlowStateUtility ()
{
   return (getFlowState () == GameStateValue::STATE_UTILITY);
}

/// <summary> Update the win animation info for win cycle displays.</summary>
/// <param name="show_utility_info" [in] show utility specific information.</param>
void SlotPresentation::updateWinOutcome (bool show_utility_info)
{
   Mojo<SlotAwardInterface> slot_award_interface;
   slotAwardInterface (slot_award_interface);

   Mojo<SlotEvaluatorInterface> slot_evaluator_interface;
   slotEvaluatorInterface (slot_evaluator_interface);

   Mojo<AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   Mojo <SlotGameSetInterface> slot_game_set;
   if (slotGameSet (slot_game_set, currentGameSetIndex ()))
   {
      Mojo<AnimateSlotWinInterface> animate_slot_win;
      if (slot_game_set->getAnimateSlotWin (animate_slot_win))
      {
         animate_slot_win->initializeWinList (slot_award_interface, slot_evaluator_interface,
                                              local_award, denomination, bet, show_utility_info);
      }
      else
      {
         if (slot_game_set->isWinCycleLiteConfigured ())
         {
            Mojo <WinCycleLiteInterface> win_cycle_lite;
            slot_game_set->getWinCycleLite (win_cycle_lite);
            win_cycle_lite->updateWinList (slot_award_interface, slot_evaluator_interface, local_award,
                                           denomination, bet);
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Start: Code for Symbol Substitution support  ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/// <summary> initial symbol substitution. Calls when load up or power hit recover. It read symbol substition data from ER, then do symbol substitution</summary>
void SlotPresentation::initSymbolsSubstitution (void)
{
   const SlotEssentialRecord *slot_essential_record = getSlotEssentialRecord ();
   const SlotEssentialRecord::SymbolSubstitute* symbol = NULL;
   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_evaluator;

   assertion (slot_essential_record != 0);

   if (!gameSideSlotEvaluatorInterface (game_side_slot_evaluator))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid game side slot evaluator."));
   }

   for (uint16 i = 0; i < slot_essential_record->getNumberOfSubstituteSymbols (); i++)
   {
      symbol = slot_essential_record ->getSubstitutionSymbol (i);
      game_side_slot_evaluator->changeReelSymbol (symbol->getReelIndex (), symbol->getPhysicalStop (), symbol->getCurrentId ());
   }
}


/// <summary> Substitute symbol at specified reel position  </summary>
/// <remarks> Please don't call this function alone from presentation. This function is designed to get caled by symbol substitution code and used together with symbol substitution code in flow side </remarks>
/// <param name="reel_index"> [in] Index of the reel to be substituted. </param>
/// <param name="stop_index"> [in] stop number for the reel symbol. </param>
/// <param name="symbol_id"> [in] new symbol id for substitution. </param>
/// <param name="game_set_index"> [in] Index of the reel set. </param>
void SlotPresentation::substituteSymbolAtPosition (SlotEssentialRecord::ReelIndex reel_index, 
                                                   uint32 stop_index, 
                                                   int16 symbol_id, 
                                                   SlotEssentialRecord::ReelSetIndex game_set_index)
{
   Mojo<ReelsInterface> reels;
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_evaluator;

   if (!slotEvaluatorInterface (slot_evaluator) || !gameSideSlotEvaluatorInterface (game_side_slot_evaluator))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid slot evaluator or invalid game side slot evaluator."));
   }

   if (!game_side_slot_evaluator->isValidSymbolId (symbol_id))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid symbol id"));
   }

   game_side_slot_evaluator->changeReelSymbol (reel_index, stop_index, symbol_id);
   //update reel strip
   if (!getReels (reels, game_set_index))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::substituteSymbolAtPosition (), ReelsInterface is invalid."));
   }

   reels->changeReelSymbolAtPosition (reel_index, stop_index, slot_evaluator->getLibrarySymbolName (symbol_id));
}

/// <summary> Restore symbol to orginal one at specified reel position  </summary>
/// <param name="reel_index"> [in] Index of the reel to be substituted. </param>
/// <param name="stop"> [in] stop number for the reel symbol. </param>
/// <param name="game_set_index"> [in] Index of the reel set. </param>
/// <returns> true if sucess, fail otherwise. </returns>
bool SlotPresentation::restoreSymbol (SlotEssentialRecord::ReelIndex reel_index, 
                                      uint32 stop, 
                                      SlotEssentialRecord::ReelSetIndex game_set_index)
{
   Mojo<SlotEvaluatorInterface> slot_evaluator;
   Mojo<ReelsInterface> reels;
   const SlotEssentialRecord::SymbolSubstitute* symbol;
   const SlotEssentialRecord* slot_essential_record = getSlotEssentialRecord ();

   if (!slotEvaluatorInterface (slot_evaluator, true, game_set_index))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid slot evaluator."));
   }

   Mojo<GameSideSlotEvaluatorInterface> game_side_slot_evaluator (slot_evaluator);
   if (!game_side_slot_evaluator.isValid())
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid game side slot evaluator."));
   }

   assertion (slot_essential_record);
   symbol = slot_essential_record->findSubstitutionSymbol (reel_index, stop, game_set_index ); 
   if (!symbol)
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "Invalid symbol"));
   }

   game_side_slot_evaluator->changeReelSymbol (reel_index, stop, symbol->getOriginalId ());

   if (!getReels (reels, game_set_index))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "SlotPresentation::restoreSymbol (), ReelsInterface is invalid."));
   }

   int32 original_id = symbol->getOriginalId ();
   reels->changeReelSymbolAtPosition (reel_index, stop, slot_evaluator->getLibrarySymbolName (original_id));
   return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// End: Symbol Substitution Related Code  /////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
