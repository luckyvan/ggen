/* (c) Copyright 1999-2005 International Game Technology */
/*
   $Id: GamePresentation.cpp,v 1.0, 2013-08-23 18:28:05Z, Shaw, Jeffrey$
   $Date: 8/24/2013 2:28:05 AM$
   $Revision: 1$
   Based on revision 619

   Purpose:
      Defines the class GamePresentation

   Modifications:
      Author          Date       Explanation
      --------------- ---------- ------------
      Dwayne Nelson   Oct 20, 99 New
*/

#include <algorithm>
#include <errno.h>
#ifndef __ACTORHELPER_HPP__
#include "ActorHelper.hpp"
#endif
#ifndef __ACTORINTERFACE_HPP__
#include "ActorInterface.hpp"
#endif
#ifndef __AUTOPLAYINTERFACE_HPP__
#include "AutoplayInterface.hpp"
#endif
#ifndef __BETSEQUENCEEVENTS_HPP__
#include "BetSequenceEvents.hpp"
#endif
#ifndef __BETSEQUENCEEVENTPARSER_HPP__
#include "BetSequenceEventParser.hpp"
#endif
#ifndef __BINGOAWARDINTERFACE_HPP__
#include "BingoAwardInterface.hpp"
#endif
#ifndef __COMMONGRAPHICSDEFS_HPP__
#include "CommonGraphicsDefs.hpp"
#endif
#ifndef __DEBUG_HPP__
#include "Debug.hpp"
#endif
#ifndef __DENOMINATIONRULES_HPP__
#include "DenominationRules.hpp"
#endif
#ifndef __DOUBLEUPINTERFACE_HPP__
#include "DoubleUpInterface.hpp"
#endif
#ifndef __GAMECONTROLPANEL_HPP__
#include "GameControlPanel.hpp"
#endif
#ifndef __GAMEPRESENTATION_HPP__
#include "GamePresentation.hpp"
#endif
#ifndef __GAMEPRESENTATIONHELPER_HPP__
#include "GamePresentationHelper.hpp"
#endif
#ifndef __GAMEPRESENTATIONINTERFACE_HPP__
#include "GamePresentationInterface.hpp"
#endif
#ifndef __GAMEPRESENTATIONRESOURCEDEFS_HPP__
#include "GamePresentationResourceDefs.hpp"
#endif
#ifndef __GAMEPRESENTATIONSTATES_HPP__
#include "GamePresentationStates.hpp"
#endif
#ifndef __GAMEREQUESTEVENT_HPP__
#include "GameRequestEvent.hpp"
#endif
#ifndef __GAMESETCONFIGURATION_HPP__
#include "GameSetConfiguration.hpp"
#endif
#ifndef __GAMESIDEBETINTERFACE_HPP__
#include "GameSideBetInterface.hpp"
#endif
#ifndef __GAMESTATEVALUE_HPP__
#include "GameStateValue.hpp"
#endif
#ifndef __GENERICGAMEREQUESTEVENT_HPP__
#include "GenericGameRequestEvent.hpp"
#endif
#ifndef __GENERICSEQUENCEEVENTPARSER_HPP__
#include "GenericSequenceEventParser.hpp"
#endif
#ifndef __MAINMENUREQUESTSEQUENCEEVENTS_HPP__
#include "MainMenuRequestSequenceEvents.hpp"
#endif
#ifndef __IGTCONNECTDISPLAYCLIENT_HPP__
#include "IGTConnectDisplayClient.hpp"
#endif
#ifndef __MAINMENUREQUESTSEQUENCEEVENTPARSER_HPP__
#include "MainMenuRequestSequenceEventParser.hpp"
#endif
#ifndef __MODELITERATORINTERFACE_HPP__
#include "ModelIteratorInterface.hpp"
#endif
#ifndef __PROGRESSIVEMANAGER_HPP__
#include "ProgressiveManager.hpp"
#endif
#ifndef __REGISTRY_HPP__
#include "Registry.hpp"
#endif
#ifndef __REPLACECRITICALDATAMODIFIER_HPP__
#include "ReplaceCriticalDataModifier.hpp"
#endif
#ifndef __RESOURCEMANAGERFUNCTIONS_HPP__
#include "ResourceManagerFunctions.hpp"
#endif
#ifndef __RESPONSIBLEGAMINGINFORMATIONINTERFACE_HPP__
#include "ResponsibleGamingInformationInterface.hpp"
#endif
#ifndef __RULE_BASED_GAME_BET_LEGACY_VARIABLE_INTERFACE_HPP__
#include "RuleBasedGameBetLegacyVariableInterface.hpp"
#endif
#ifndef __RULE_BASED_GAME_BET_INTERFACE_HPP__
#include "RuleBasedGameSideBetInterface.hpp"
#endif
#ifndef __SCRIPTRESOURCEIMPORTERINTERFACE_HPP__
#include "ScriptResourceImporterInterface.hpp"
#endif
#ifndef __SEQUENCEEVENTTRANSLATOR_HPP__
#include "SequenceEventTranslator.hpp"
#endif
#ifndef __SOUNDSYSTEMINTERFACE_HPP__
#include "SoundSystemInterface.hpp"
#endif
#ifndef __STATE_HPP__
#include "State.hpp"
#endif
#ifndef __TEXTUREATTRIBUTEINTERFACE_HPP__
#include "TextureAttributeInterface.hpp"
#endif
#ifndef __TEXTURERESOURCEINTERFACE_HPP__
#include "TextureResourceInterface.hpp"
#endif
#ifndef __THEATREOPENGLDEFS_HPP__
#include "TheatreOpenGLDefs.hpp"
#endif
#ifndef __USERSEQUENCEEVENT_HPP__
#include "UserSequenceEvent.hpp"
#endif
#ifndef __USERSEQUENCEEVENTPARSER_HPP__
#include "UserSequenceEventParser.hpp"
#endif
#ifndef __UTILITYSEQUENCEEVENTPARSER_HPP__
#include "UtilitySequenceEventParser.hpp"
#endif
#ifndef __GAMETILTMANAGERDEFS_HPP__
#include "GameTiltManagerDefs.hpp"
#endif
#ifndef __TILTGAMEREQUESTEVENT_HPP__
#include "TiltGameRequestEvent.hpp"
#endif
#ifndef __GAMEREQUESTEVENTPOSTER_HPP__
#include "GameRequestEventPoster.hpp"
#endif

//#define __CHECK_MEM__
#ifdef __CHECK_MEM__
   #ifndef __CHECKMEMORY_HPP__
   #include "CheckMemory.hpp"
   #endif

   //#define __TRACK_SPECIFIC_SIZE__
   #ifdef __TRACK_SPECIFIC_SIZE__
      #define MINIMUM_ALLOCATION_SIZE 1024
      #define MAXIMUM_ALLOCATION_SIZE 2048
   #endif
#endif

#ifdef __DEBUG__
// #define __VERBOSE__
// #define __VERBOSE_AWARD__
// #define __VERBOSE_BET__
// #define __VERBOSE_CONDITIONS__
// #define __VERBOSE_DENOM__
// #define __VERBOSE_IDLE__
// #define __VERBOSE_MENU__
// #define __VERBOSE_METERS__
// #define __VERBOSE_PAYTABLE__
// #define __VERBOSE_MULTIMEDIA_EVENT__
// #define __VERBOSE_MULTIMEDIA_EVENT_FILTER_CONTROLS__
// #define __VERBOSE_STATE__
// #define __VERBOSE_MODE__
// #define __VERBOSE_SCRIPT__
// #define __VERBOSE_VOLUME__
// #define __VERBOSE_SERIALIZE__
// #define __VERBOSE_HANDLE_MESSAGE__
// #define __VERBOSE_GAME_SWITCH__
// #define __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
// #define __VERBOSE_DENOMINATION_UPDATES__
// #define __VERBOSE_TIMER__
// #define __VERBOSE_LANGUAGE__
// #define __VERBOSE_AUTOPLAY__
// #define __VERBOSE_DOUBLEUP__
#endif

#ifdef __VERBOSE__
 #define verboseMethodName() debug << __PRETTY_FUNCTION__ << endl;
#else
 #define verboseMethodName()
#endif


// Define some debug/timing measurement support.
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




// Static variable to the light system
Mojo <class LightSystemInterface> GamePresentation::light_system;

// Create an instance of the SequenceEventParser required by the generic game presentations.  Since sequence event
// handling can be completely scripted, it is not guaranteed that the parser symbols will be included in the shared
// objects that link the libraries.  Thus, the parser declaration is required here to ensure that the symbols
// included during the linking process.
static const GenericSequenceEventParser generic_sequence_event_parser;
static const UserSequenceEventParser user_sequence_event_parser;
static const UtilitySequenceEventParser utility_sequence_event_parser;
static const MainMenuRequestSequenceEventParser main_menu_request_sequence_event_parser;
static const BetSequenceEventParser bet_changed_sequence_event_parser;

// Presentation state condition string constants.
const char *const GamePresentation::CONDITION_FIRST_GAME = "FIRST_GAME";
const char *const GamePresentation::CONDITION_NOT_FIRST_GAME = "NOT_FIRST_GAME";
const char *const GamePresentation::CONDITION_PLAY_COUNT = "PLAY_COUNT:";
const char *const GamePresentation::CONDITION_NOT_PLAY_COUNT = "NOT_PLAY_COUNT:";
const char *const GamePresentation::CONDITION_GAMES_REMAINING = "GAMES_REMAINING:";
const char *const GamePresentation::CONDITION_LAST_GAME = "LAST_GAME";
const char *const GamePresentation::CONDITION_NOT_LAST_GAME = "NOT_LAST_GAME";
const char *const GamePresentation::CONDITION_WIN_THIS_ACT = "WIN_THIS_ACT";
const char *const GamePresentation::CONDITION_NO_WIN_THIS_ACT = "NO_WIN_THIS_ACT";
const char *const GamePresentation::CONDITION_AWARD_THIS_ACT = "AWARD_THIS_ACT";
const char *const GamePresentation::CONDITION_NO_AWARD_THIS_ACT = "NO_AWARD_THIS_ACT";
const char *const GamePresentation::CONDITION_IS_ACT_TYPE = "IS_ACT_TYPE:";
const char *const GamePresentation::CONDITION_NOT_ACT_TYPE = "NOT_ACT_TYPE:";
const char *const GamePresentation::CONDITION_WIN_CATEGORY = "WIN_CATEGORY:";
const char *const GamePresentation::CONDITION_NOT_WIN_CATEGORY = "NOT_WIN_CATEGORY:";
const char *const GamePresentation::CONDITION_ROLLUP_COMPLETE = "ROLLUP_COMPLETE";
const char *const GamePresentation::CONDITION_NOT_ROLLUP_COMPLETE = "NOT_ROLLUP_COMPLETE";
const char *const GamePresentation::CONDITION_PROGRESSIVE_LINKED = "PROGRESSIVE_LINKED";
const char *const GamePresentation::CONDITION_NO_PROGRESSIVE_LINKED = "NO_PROGRESSIVE_LINKED";
const char *const GamePresentation::CONDITION_PLAYER_SELECTABLE_DENOMINATION = "PLAYER_SELECTABLE_DENOMINATION";
const char *const GamePresentation::CONDITION_NO_PLAYER_SELECTABLE_DENOMINATION = "NO_PLAYER_SELECTABLE_DENOMINATION";
const char *const GamePresentation::CONDITION_PLAYER_SELECTABLE_GAME = "PLAYER_SELECTABLE_GAME";
const char *const GamePresentation::CONDITION_NO_PLAYER_SELECTABLE_GAME = "NO_PLAYER_SELECTABLE_GAME";
const char *const GamePresentation::CONDITION_ENROLLMENT_REQUIRED = "ENROLLMENT_REQUIRED";
const char *const GamePresentation::CONDITION_NOT_ENROLLMENT_REQUIRED = "NOT_ENROLLMENT_REQUIRED";
const char *const GamePresentation::CONDITION_ENROLLMENT_BEFORE_GAME_START = "ENROLLMENT_BEFORE_GAME_START";
const char *const GamePresentation::CONDITION_NOT_ENROLLMENT_BEFORE_GAME_START = "NOT_ENROLLMENT_BEFORE_GAME_START";
const char *const GamePresentation::CONDITION_GAME_WAS_UNENROLLED = "GAME_WAS_UNENROLLED";
const char *const GamePresentation::CONDITION_NOT_GAME_WAS_UNENROLLED = "NOT_GAME_WAS_UNENROLLED";
const char *const GamePresentation::CONDITION_GAME_ENROLLMENT_SEEDS_ARRIVED = "GAME_ENROLLMENT_SEEDS_ARRIVED";
const char *const GamePresentation::CONDITION_NOT_GAME_ENROLLMENT_SEEDS_ARRIVED = "NOT_GAME_ENROLLMENT_SEEDS_ARRIVED";
const char *const GamePresentation::CONDITION_ENROLLMENT_BUTTON_DOUBLE_TAP = "ENROLLMENT_BUTTON_DOUBLE_TAP";
const char *const GamePresentation::CONDITION_NOT_ENROLLMENT_BUTTON_DOUBLE_TAP = "NOT_ENROLLMENT_BUTTON_DOUBLE_TAP";
const char *const GamePresentation::CONDITION_PAYTABLE_SECTION = "PAYTABLE_SECTION:";
const char *const GamePresentation::CONDITION_NOT_PAYTABLE_SECTION = "NOT_PAYTABLE_SECTION:";
const char *const GamePresentation::CONDITION_WIN_GREATER_THAN_TOTAL_BET_X = "WIN_GREATER_THAN_TOTAL_BET_X:";
const char *const GamePresentation::CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X = "WIN_NOT_GREATER_THAN_TOTAL_BET_X:";
const char *const GamePresentation::CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X = "WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X:";
const char *const GamePresentation::CONDITION_WIN_NOT_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X = "WIN_NOT_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X:";
const char *const GamePresentation::CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X = "WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X:";
const char *const GamePresentation::CONDITION_WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X = "WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X:";
const char *const GamePresentation::CONDITION_WIN_GREATER_THAN = "WIN_GREATER_THAN:";
const char *const GamePresentation::CONDITION_WIN_NOT_GREATER_THAN = "WIN_NOT_GREATER_THAN:";
const char *const GamePresentation::CONDITION_WIN_THIS_ACT_GREATER_THAN = "WIN_THIS_ACT_GREATER_THAN:";
const char *const GamePresentation::CONDITION_WIN_THIS_ACT_NOT_GREATER_THAN = "WIN_THIS_ACT_NOT_GREATER_THAN:";
const char *const GamePresentation::CONDITION_PLAYER_SELECTABLE_VOLUME = "PLAYER_SELECTABLE_VOLUME";
const char *const GamePresentation::CONDITION_NO_PLAYER_SELECTABLE_VOLUME = "NO_PLAYER_SELECTABLE_VOLUME";
const char *const GamePresentation::CONDITION_CAN_DISPLAY_AWARD = "CAN_DISPLAY_AWARD";
const char *const GamePresentation::CONDITION_CANNOT_DISPLAY_AWARD = "CANNOT_DISPLAY_AWARD";
const char *const GamePresentation::CONDITION_SPEED_PLAYER_MODIFIABLE = "SPEED_PLAYER_MODIFIABLE";
const char *const GamePresentation::CONDITION_SPEED_NOT_PLAYER_MODIFIABLE = "SPEED_NOT_PLAYER_MODIFIABLE";
const char *const GamePresentation::CONDITION_CAN_START_GAME = "CAN_START_GAME";
const char *const GamePresentation::CONDITION_CAN_NOT_START_GAME = "CAN_NOT_START_GAME";
const char *const GamePresentation::CONDITION_TOURNAMENT_ENABLED = "TOURNAMENT_ENABLED";
const char *const GamePresentation::CONDITION_TOURNAMENT_DISABLED = "TOURNAMENT_DISABLED";
const char *const GamePresentation::CONDITION_GET_FLOW_STATE_IDLE = "GET_FLOW_STATE_IDLE";
const char *const GamePresentation::CONDITION_GET_FLOW_STATE_NOT_IDLE = "GET_FLOW_STATE_NOT_IDLE";
const char *const GamePresentation::CONDITION_AWARD_TYPE = "AWARD_TYPE:";
const char *const GamePresentation::CONDITION_DOUBLEUP_NOT_PLAYED = "DOUBLEUP_NOT_PLAYED";
const char *const GamePresentation::CONDITION_DOUBLEUP_WON = "DOUBLEUP_WON";
const char *const GamePresentation::CONDITION_DOUBLEUP_LOST = "DOUBLEUP_LOST";
const char *const GamePresentation::CONDITION_AUTO_PLAY_SIMULATE_INPUT = "AUTO_PLAY_SIMULATE_INPUT";
const char *const GamePresentation::CONDITION_RMLP_TRIGGERED = "RMLP_TRIGGERED";
const char *const GamePresentation::CONDITION_RMLP_NOT_TRIGGERED = "RMLP_NOT_TRIGGERED";
const char *const GamePresentation::CONDITION_WIN_ALL_ACTS = "WIN_ALL_ACTS";
const char *const GamePresentation::CONDITION_NO_WIN_ALL_ACTS = "NO_WIN_ALL_ACTS";
// Presentation-wide multimedia events.
const char *const GamePresentation::ADVANCE_PRESENTATION_STATE = "AdvancePresentationState";
// Define the stage types supported by the presentation.
const char *const GamePresentation::MAIN_STAGE = "MAIN_STAGE";
const char *const GamePresentation::HELP_DISPLAY_STAGE = "HELP_DISPLAY_STAGE";
const char *const GamePresentation::PAYTABLE_DISPLAY_STAGE = "PAYTABLE_DISPLAY_STAGE";
const char *const GamePresentation::TRANSITION_STAGE = "TRANSITION_STAGE";
// System error messages.
const char *const GamePresentation::ERROR_MESSAGE_FIND_INTERFACE = "GamePresentation::findInterface: Operation only allowed when the entire game is loaded.";
// Property names
const char *const GamePresentation::PLAYER_MENU_OPTION  = "PlayerMenuOption";
const char *const GamePresentation::PLAYER_VOLUME_LEVEL = "PlayerVolumeLevel";
const char *const GamePresentation::GAME_MAX_BET_VALUE = "GameMaxBetValue";
const char *const GamePresentation::BASE_GAME_PRESENTATION_EVALUATION = "BaseGamePresentationEvaluation";
const char *const GamePresentation::PROPERTY_SHOW_AUTOPLAY_MESSAGE = "ShowAutoplayMessage";
// Defines the menu option strings.
const char *const GamePresentation::MENU_OPTION_MAIN_MENU         = "MainMenu";
const char *const GamePresentation::MENU_OPTION_SELECT_VALUE      = "SelectValue";
const char *const GamePresentation::MENU_OPTION_ONE_CREDIT_EQUALS = "OneCreditEquals";

// Defines the volume level strings.
const char *const GamePresentation::VOLUME_LEVEL_LOW_STRING      = "Low";
const char *const GamePresentation::VOLUME_LEVEL_DEFAULT_STRING  = "Default";
const char *const GamePresentation::VOLUME_LEVEL_HIGH_STRING     = "High";

// config names
const char *const GamePresentation::CONFIG_ID_GAME_SPEED = "GAME SPEED";
const char *const GamePresentation::CONFIG_ID_PAY_SPEED = "PAY SPEED";
const char *const GamePresentation::CONFIG_ID_PAY_SPEED_SPEED_PLAYER_MODIFIABLE = "SPEED PLAYER MODIFIABLE";
const char *const GamePresentation::CONFIG_ID_BACKGROUND_COLOR = "BACKGROUND COLOR";
const char *const GamePresentation::CONFIG_ID_VOLUME_PLAYER_SELECTABLE = "VOLUME PLAYER SELECTABLE";
const char *const GamePresentation::CONFIG_ID_VOLUME_GAME_SOUNDS_RELATIVE = "VOLUME GAME SOUNDS RELATIVE";
const char *const GamePresentation::CONFIG_ID_VOLUME_GAME_SOUNDS = "VOLUME GAME SOUNDS";
const char *const GamePresentation::CONFIG_ID_THEME_DISPLAYED_TO_PLAYER_DENOMINATIONS = "THEME DISPLAYED TO PLAYER DENOMINATIONS";
const char *const GamePresentation::CONFIG_ID_TERMINAL_ENABLED_PLAYER_DENOMINATIONS = "MACHINE WIDE ENABLED PLAYER DENOMINATIONS";
const char *const GamePresentation::CONFIG_ID_NUMBER_OF_PLAYABLE_THEMES = "NUMBER OF PLAYABLE THEMES";
const char *const GamePresentation::CONFIG_ID_AUTOPLAY_STATE = "AUTOPLAY STATE";
const char *const GamePresentation::CONFIG_ID_SHOW_AUTOPLAY_MESSAGE = "SHOW AUTOPLAY MESSAGE";
const char *const GamePresentation::CONFIG_ID_SHOW_DRAW_STATE_MESSAGE = "SHOW DRAW STATE MESSAGE";
const char *const GamePresentation::CONFIG_ID_CASHOUT_CELEBRATION_ENABLED = "CASHOUT CELEBRATION ANIMATION ENABLED";
const char *const GamePresentation::CONFIG_ID_OKAY_TO_SUSPEND_GAME_DISPLAY_EARLY = "OKAY TO SUSPEND GAME DISPLAY EARLY";
const char *const GamePresentation::CONFIG_ID_MAXIMUM_TOTAL_BET = "MAXIMUM TOTAL BET";
const char *const GamePresentation::CONFIG_ID_BINGO_SYSTEM_GAMING = "BINGO SYSTEM GAMING";
const char *const GamePresentation::CONFIG_ID_EASY_BET_ENABLED = "EASY BET ENABLED";
const char *const GamePresentation::CONFIG_ID_EASY_BET_ENABLED_LIST = "EASY BET ENABLED LIST";
const char *const GamePresentation::CONFIG_ID_MINIMUM_WAGER_INTERVAL = "MINIMUM WAGER INTERVAL";

// Easy Bet Configuration Names
const char *const GamePresentation::EASY_BET_BOOLEAN_FALSE_STRING = "Easy Bet Disabled";
const char *const GamePresentation::EASY_BET_BOOLEAN_TRUE_STRING = "Easy Bet 5 Button";

// tournament mode strings
const char * const GamePresentation::TOURNAMENT_MODE_DISABLED = "TournamentModeDisabled";
const char * const GamePresentation::TOURNAMENT_MODE_TIME_ONLY = "TournamentModeTimeOnly";
const char * const GamePresentation::TOURNAMENT_MODE_CREDITS_ONLY = "TournamentModeCreditsOnly";
const char * const GamePresentation::TOURNAMENT_MODE_TIME_AND_CREDITS = "TournamentModeTimeAndCredits";

// data
const char *const GamePresentation::SAFE_STORE_NAME_PRESENTATION_INFORMATION = "/PresentationPresentation";

// Defines the volume levels.
const float GamePresentation::VOLUME_LEVEL_DEFAULT            = 0.0f;
const float GamePresentation::VOLUME_LEVEL_LOW_BELOW_40       = -0.12f;
const float GamePresentation::VOLUME_LEVEL_LOW_40_TO_49       = -0.10f;
const float GamePresentation::VOLUME_LEVEL_LOW_50_AND_ABOVE   = -0.08f;
const float GamePresentation::VOLUME_LEVEL_HIGH_BELOW_40      = 0.10f;
const float GamePresentation::VOLUME_LEVEL_HIGH_40_TO_49      = 0.08f;
const float GamePresentation::VOLUME_LEVEL_HIGH_50_TO_59      = 0.06f;
const float GamePresentation::VOLUME_LEVEL_HIGH_60_TO_69      = 0.05f;
const float GamePresentation::VOLUME_LEVEL_HIGH_70_AND_ABOVE  = 0.04f;

// Denominations
const char *const GamePresentation::ADVERTISEMENT_CONFIG_ITEM = "MAIN MENU ADVERTISE PROGRESSIVE TEXT";
const char *const GamePresentation::DEFAULT_PLAYER_DENOMINATION = "DEFAULT PLAYER DENOMINATION";

// Money handling styles
const char* const GamePresentation::MONEY_HANDLING_STYLE_DEFAULT  = "DEFAULT";
const char* const GamePresentation::MONEY_HANDLING_STYLE_BANKED_CREDITS = "BANKED_CREDITS";

// Initialize the object for use.
void GamePresentation::postConstructor (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::postConstructor ()" << endl;
   #endif
   // Call the base classes first.
   Actuable::postConstructor ();
   MultimediaEventListener::postConstructor ();

   // Construct the object that will contain and unserialize the essential record data.
   essential_record_ptr = 0;
   constructEssentialRecord ();
   essential_record_ptr->initCritDataSynchronSerializer (essential_record);  // "synchron's" need the serializer to know what critical data object to use.

   // Construct the object that will contain and unserialize the shared essential record data.
   shared_essential_record_ptr = 0;
   constructSharedEssentialRecord ();
   shared_essential_record_ptr->initCritDataSynchronSerializer (shared_essential_record);  // "synchron's" need the serializer to know what critical data object to use.

   // Initialize MJP WAN.
   igt_connect.create <IGTConnectGameClient> ();
   igt_connect_display.create <IGTConnectDisplayClient> ();
   if (igt_connect_display.isValid () && igt_connect_display->load ())
   {
      // IGT Connect Display is a static object, therefore only one presentation 
      // needs to initialize the object.  Let the base stage do this.
      if (stageIndex () == BASE_GAME_STAGE)
      {
         igt_connect_display->init (getThemeConfigurationIdentifier ());
         addToLists (igt_connect_display->getInternalInterface ());
      }
   }
   igt_connect_action_required = false;

   //
   // Set default denomination events/properties
   //
   display_advertisement_text = false;

   // This event is posted from the system telling the game the denomination value has changed
   update_system_denomination_event     = "UpdateDenominationValue";

   // Tournament multimedia events
   update_tournament_info_event = "TournamentEvent:TournamentInfoUpdated";
   update_tournament_meters_event = "TournamentEvent:TournamentMetersUpdated";
   formatted_timer_text_property_updated_event = "UpdateDisplayTimerText";

   // Tournament property names
   system_tournament_mode_property_name = "TournamentMode";
   system_tournament_status_property_name = "TournamentStatus";
   system_tournament_credits_property_name = "TournamentCredits";
   system_tournament_won_property_name = "TournamentWon";
   formatted_timer_text_property_name = "DisplayTimerText";

   // Set system display timer properties
   system_display_timer_property_name   = "SystemDisplayTimer";

   // This event is posted from the system telling the game the system display timer value has changed
   update_system_display_timer_event     = "SystemDisplayTimerUpdated";
   // Set system display timer properties
   system_display_timer_property_name   = "SystemDisplayTimer";

   // Defines the text before the minimum bet number
   mininum_bet_denomination_text = "Min. Bet = ";

   // These two events are posted based on whether the denomination value exists in the denomonination.movie
   update_game_movie_denomination_event = "UpdateGameMovieDenominationValue";
   update_game_font_denomination_event  = "UpdateGameFontDenominationValue";

   // This event is posted when the minimum bet should be shown on the denomination
   update_min_bet_denomination_event    = "UpdateMinimumBetDenominationValue";
   hide_min_bet_denomination_event    = "HideMinimumBetDenominationValue";

   // Default to green background blank movie section (in denomination.movie)
   // Set to "Blank" to have the green background "button" appear behind the denom
   // Set to "Alpha" to have nothing show up behind the denom
   blank_movie_section                  = "Blank";

   // Set game specific denomination properties for re-interpreting the system denomination value
   system_denomination_property_name    = "DenominationValue";
   game_denomination_property_name      = "GameDenominationValue";
   game_denomination_text_property_name = "GameDenominationTextValue";
   minimum_bet_property_name            = "MinimumBet";
   denomination_movie_actor_name        = "DenomButton";

   // Autoplay
   autoplay_alias_name = "";
   double_up_alias_name = "";

   // Language button is enabled by default. On synchronize, this flag will be adjusted
   language_supported = true;
   // More than one language is available to player is not enabled by default. On synchronize, this flag will be adjusted
   multiple_languages_available_for_player = false;

   // Autoplay button is enabled by default. On synchronize, this flag will be adjusted
   player_autoplay_supported = true;
   disable_language_button_event = "LanguageButton:Hide";

   // Do not allow international buttons by default. Scriptable function call is needed to enable intl buttons.
   allow_autoplay_button = false;
   allow_language_button = false;

   // Initialize all class members here.
   current_state = StatePair (GameStateValue::STATE_COLD_START, 0);
   previous_state = StatePair (GameStateValue::STATE_COLD_START, 0);
   game_index = NO_GAME;
   stage_index = NO_STAGE;
   act_index = NO_ACT;
   session_id = 0;
   current_context_mode = CONTEXT_MODE_INVALID;
   previous_context_mode = CONTEXT_MODE_INVALID;
   adjust_presentation_to_new_mode = true;
   presentation_is_entered = false;
   need_to_synchronize_presentation_state = true;
   current_game_mode = GAME_MODE_UNKNOWN;
   told_to_show_stage = false;
   nv_mode = true;
   show_stage_on_enter = true;
   current_game_activity_status = true;
   awaiting_substate_change = false;
   last_requested_flow_state_id = GameStateValue::STATE_COLD_START;
   last_requested_presentation_state_id = 0;
   enrollment_required = false;
   enrollment_before_game_start = false;
   enrollment_button_double_tap = false;
   win_mismatch_pending = false;
   last_exit_game_mode_reason = STAGE_EXIT_REASON_UNKNOWN;

   Config config;
   ConfigID id;
   // Get the config id for game language
   game_language_config_id = config.getId ("GAME LANGUAGE");
   // Get the config id for the number of playable themes.
   number_of_playable_themes_config_id = config.getId (CONFIG_ID_NUMBER_OF_PLAYABLE_THEMES);
   pay_all_mismatched_wins_to_credit_meter_id = config.getId ("PAY ALL MISMATCHED WINS TO CREDIT METER");
   autoplay_state_config_id = config.getId (CONFIG_ID_AUTOPLAY_STATE);
   theme_displayed_to_player_denominations_id = config.getId (CONFIG_ID_THEME_DISPLAYED_TO_PLAYER_DENOMINATIONS);
   terminal_enabled_player_denominations_id = config.getId (CONFIG_ID_TERMINAL_ENABLED_PLAYER_DENOMINATIONS);
   // These are theme specific configs that will be set when the theme configuration information is set for this game
   language_licensed_bitfield_config_id = INVALID_CONFIGID;
   language_supported_list_config_id = INVALID_CONFIGID;
   who_can_start_autoplay_config_id = config.getId ("WHO CAN START AUTOPLAY");
   minimum_wager_interval_id = config.getId(CONFIG_ID_MINIMUM_WAGER_INTERVAL);
   all_meter_display_behavior_config_id = config.getId ("ALL METER DISPLAY BEHAVIOR", false);
   manual_button_config_id = INVALID_CONFIGID;

   okay_to_suspend_game_display_early = config.getValue (config.getId (CONFIG_ID_OKAY_TO_SUSPEND_GAME_DISPLAY_EARLY));

   // Retrieve the current setting of configuration item "MONEY HANDLING STYLE".
   money_handling_style = MONEY_HANDLING_STYLE_DEFAULT;
   id = config.getId ("MONEY HANDLING STYLE", false);
   if (id != INVALID_CONFIGID)
   {
      config.getValueString (id, money_handling_style);
   }

   // At the base game presentation level, this variable will not changes.  If change is required, it must be
   // performed in a derived presentation class.
   current_game_set_index = DEFAULT_GAME_SET_INDEX;

   // Create the list of sequence event listeners and verify its validity.
   sequence_event_listener_controller.create<EventListenerController<SequenceEventListenerInterface, SequenceEvent> > ();
   if (!sequence_event_listener_controller.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::GamePresentation Unable to create SequenceEventListenerController."));

   // Create the list state machine and verify its validity.
   state_machine.create<StateMachine> ();
   if (!state_machine.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::GamePresentation Unable to create StateMachine."));
   state_machine->setGamePresentation (this);

   // Get a pointer to the LightSystem if one has not been retrieved
   if (!light_system.isValid ())
      createResource <LightSystemInterface> (LIGHT_SYSTEM, "Default", light_system);

   // Set the script functions.
   static const ScriptFunctionMap script_functions [] =
   {
      // Parameters:  Script Function ID, Arguments, Script function name.
      { ACCELERATE_ROLLUP,                   0,    "accelerateRollup"         },
      { ADD_STAGE_TO_CONTEXT,                1,    "addStageToContext"        },
      { CONFIGURE_FROM_SCRIPT,               1,    "configureFromScript"      },
      { DISPLAY_WIN,                         2,    "displayWin"               },
      { ENABLE_CONTROL_PANELS,               1,    "enableControlPanels"      },
      { POST_EVENT,                          1,    "postEvent"                },
      { POST_EVENT_ALL,                      1,    "postEventAll"             },
      { POST_EVENT_CONTEXT,                  2,    "postEventContext"         },
      { POST_ACT_OUTCOME_EVENTS,             0,    "postActOutcomeEvents"     },
      { POST_GAME_START_EVENTS,              0,    "postGameStartEvents"      },
      { REGISTER_CONTROL_PANEL,              2,    "registerControlPanel"     },
      { REGISTER_STAGE,                      1,    "registerStage"            },
      { SET_BET_SCRIPT,                      1,    "setBetScript"             },
      { SET_CURRENT_GAME_SET_INDEX,          1,    "setCurrentGameSetIndex"   },
      { START_ROLLUP,                        0,    "startRollup"              },
      { SUPPORT_PAYTABLE_SECTION,            1,    "supportPaytableSection"   },
      { SYNCHRONIZE_BACKGROUND_COLOR,        0,    "synchronizeBackGroundColor"},
      { SYNCHRONIZE_METERS,                  0,    "synchronizeMeters"        },
      { SYNCHRONIZE_BET_BUTTONS,             0,    "synchronizeBetButtons"    },
      { COMPARE_WIN_TO_BET_MULTIPLIER,       4,    "compareWinToBetMultiplier"},
      #ifdef __DEBUG__
      { WARM_BOOT,                           0,    "warmBoot"                 },
      { WARM_BOOT,                           1,    "warmBoot"                 },
      { WARM_BOOT_RESET_SEQUENCE,            0,    "warmBootResetSequence"    },
      #endif
      { UPDATE_SYSTEM_DENOMINATION_EVENT,    1,    "updateSystemDenominationEvent"     },
      { UPDATE_GAME_MOVIE_DENOMINATION_EVENT,1,    "updateGameMovieDenominationEvent"  },
      { UPDATE_GAME_FONT_DENOMINATION_EVENT, 1,    "updateGameFontDenominationEvent"   },
      { MINIMUM_BET_DENOMINATION_TEXT,       1,    "minimumBetDenominationText"        },
      { UPDATE_MIN_BET_DENOMINATION_EVENT,   1,    "updateMinBetDenominationEvent"     },
      { HIDE_MIN_BET_DENOMINATION_EVENT,     1,    "hideMinBetDenominationEvent"       },
      { BLANK_DENOMINATION_MOVIE_SECTION,    1,    "blankDenominationMovieSection"     },
      { SYSTEM_DENOMINATION_PROPERTY_NAME,   1,    "systemDenominationPropertyName"    },
      { GAME_DENOMINATION_PROPERTY_NAME,     1,    "gameDenominationPropertyName"      },
      { GAME_DENOMINATION_TEXT_PROPERTY_NAME,1,    "gameDenominationTextPropertyName"  },
      { MINIMUM_BET_PROPERTY_NAME,           1,    "minimumBetPropertyName"            },
      { DENOMINATION_MOVIE_ACTOR_NAME,       1,    "denominationMovieActorName"        },
      { SET_NV_MODE,                         1,    "setNVMode"                         },
      { SET_AUTOPLAY_ALIAS_NAME,             1,    "setAutoplayAliasName"              },
      { SET_DOUBLE_UP_ALIAS_NAME,            1,    "setDoubleUpAliasName"              },
      { CHECK_IF_DOUBLE_UP_TRIGGERED,        0,    "checkIfDoubleUpTriggered"          },
      { ROLLUP_DOUBLE_UP_WIN,                0,    "rollupDoubleUpWin"                 },
      { ENABLE_DOUBLE_UP_LISTENER,           0,    "enableDoubleUpListener"            },
      { ENABLE_LANGUAGE_BUTTON,              1,    "enableLanguageButton"              },
      { DISABLE_LANGUAGE_BUTTON_EVENT,       1,    "disableLanguageButtonEvent"        },
      { GET_STAGE,                           0,    "getStage"                          },
      { GET_EASY_BET_CONFIGURATION,          0,    "getEasyBetConfiguration"           },
      { IS_AUTO_PLAY_ENABLED,                0,    "isAutoPlayEnabled"                 },
      { IS_DOUBLE_UP_ENABLED,                0,    "isDoubleUpEnabled"                 },
      { IS_LANGUAGE_ENABLED,                 0,    "isLanguageEnabled"                 },
      { ALLOW_AUTOPLAY_BUTTON,               1,    "allowAutoPlayButton"               },
      { ALLOW_LANGUAGE_BUTTON,               1,    "allowLanguageButton"               },
      { GET_BET,                             0,    "getBet"                            },
      { GET_CONFIG_ITEM,                     1,    "getConfigItem"                     },
      { GET_CONFIG_ITEM,                     2,    "getConfigItem"                     },
      { GET_CONFIG_ITEM,                     3,    "getConfigItem"                     },
      { GET_THEME_CONFIGURATION_IDENTIFIER,  0,    "getThemeConfigurationIdentifier"   },
      { GET_STAGE_CONFIGURATION_IDENTIFIER,  0,    "getStageConfigurationIdentifier"   },
      { GET_GAME_CONFIGURATION_IDENTIFIER,   0,    "getGameConfigurationIdentifier"    },
      { GET_MOUNT_POINT,                     0,    "getMountPoint"                     },
      { IS_FLOW_STATE_HISTORY,               0,    "isFlowStateHistory"                },
      { GET_NUMBER_OF_ENABLED_DENOMS,        0,    "getNumberOfEnabledDenoms"          },
      { CHANGE_DENOMINATION,                 1,    "changeDenomination"                },
      { GET_DENOMINATION_BUTTON_DATA,        1,    "getDenominationButtonData"         },
      { GET_DEFAULT_DENOMINATION,            0,    "getDefaultDenomination"            },
      { SET_SHOW_STAGE_ON_ENTER,             1,    "setShowStageOnEnter"               },
      { GET_PAYTABLE_PARSER,                 0,    "getPaytableParser"                 },
      { GET_LOCALIZED_CURRENCY_STRING,       1,    "getLocalizedCurrencyString"        },
      { GET_PAYTABLE_PERCENTAGE,             0,    "getPaytablePercentage"             },
      { GET_RESPONSIBLE_GAMING_DATA,         0,    "getResponsibleGamingData"          },
      { GET_NUMBER_OF_LICENSED_LANGUAGE,     0,    "getNumberOfLicensedLanguage"       },
      { GET_MACHINE_WIDE_WIN_CAP_LIMIT,      0,    "getMachineWideWinCapLimit"         },
      { GET_MINIMUM_BET_DENOMINATION_TEXT,   0,    "getMinimumBetDenominationText"     },
      { IS_PRESENTATION_PLAYING,             0,    "isPresentationPlaying"             },
      { GET_STATE_INFO_PAYLOAD,              0,    "getStateInfoPayload"               },
      { GET_EVALUATOR_TYPE,                  0,    "getEvaluatorType"                  },
      { IS_WIN_CATEGORY,                     1,    "isWinCategory"                     },
      { IS_WIN_CATEGORY,                     2,    "isWinCategory"                     },
      { WIN_THIS_ACT,                        0,    "winThisAct"                        },
      { WIN_DOUBLEUP,                        0,    "winDoubleUp"                       },
      { IS_DOUBLEUP_PLAYED,                  0,    "isDoubleUpPlayed"                  },
      { CAN_DISPLAY_AWARD,                   0,    "canDisplayAward"                   },
      { IS_PLAYER_SELECTABLE_DENOMINATION,   0,    "isPlayerSelectableDenomination"    },
      { IS_PLAYER_SELECTABLE_GAME,           0,    "isPlayerSelectableGame"            },
      { POST_GAME_SIDE_TILT,                 3,    "postGameSideTilt"                  },
      { END_PRESENTATION_STATE,              0,    "endPresentationState"              },
      { END_PRESENTATION_STATE,              1,    "endPresentationState"              },
      { RETURN_TO_PREVIOUS_PRESENTATION_STATE,  0,    "returnToPreviousPresentationState" },
      { GET_ER_DATA,                         0,    "getERData"                         },
      { WIN_ALL_ACTS,                        0,    "winAllActs"                        },
      { GET_IGT_CONNECT,                     0,    "getIGTConnect"                     },
      { GET_IGT_CONNECT_DISPLAY,             0,    "getIGTConnectDisplay"              },
	  { GET_IGT_CONNECT_ACTION_REQUIRED,     0,    "getIGTConnectActionRequired"       },
      { RMLP_BONUS_TRIGGERED,                0,    "rmlpBonusTriggered"                },
      { IS_RMLP_FEATURE_ENABLED,             0,    "isRMLPEnabled"                     },
	  { PROCESS_IGT_CONNECT_NOTIFICATIONS,   0,    "processIGTConnectNotifications",   },

      // Terminator.
      { -1, 0, "" }
   };
   addScriptFunctionMap (script_functions);

   // Set the object maps.
   static const ScriptObjectMap script_objects [] =
   {
      // Parameters: Object ID, New requirement, Object type string
      { GAME_SET_CONFIGURATION,              ScriptObjectMap::NEW_REQUIRED,      GameSetConfiguration::object_type   },
      { RESOURCE_LIST,                       ScriptObjectMap::NEW_NOT_ALLOWED,   "ResourceList"                      },
      { SEQUENCE_EVENT_TRANSLATOR,           ScriptObjectMap::NEW_NOT_ALLOWED,   ::SEQUENCE_EVENT_TRANSLATOR         },
      { STATE_MACHINE,                       ScriptObjectMap::NEW_NOT_ALLOWED,   "StateMachine"                      },
      { PERSISTENT_PROPERTY_LIST,            ScriptObjectMap::NEW_NOT_ALLOWED,   "PersistentPropertyList"            },
      // Terminator.
      { -1, ScriptObjectMap::NEW_NOT_ALLOWED, "" }
   };
   addScriptObjectMap (script_objects);

   base_game_presentation_evaluation_flag = true;
   base_game_presentation_meter_clear_flag = false;
   rmlp_triggered = false;
   rmlp_feature_enabled = false;
}

// Prepare the object for destruction.
void GamePresentation::preDestructor (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::preDestructor ()" << endl;
   #endif
   // Unload the game.
   prepareForUnloading ();

   // Destroy the object that contains and unserializes the essential record data.
   destructEssentialRecord ();
   destructSharedEssentialRecord ();

   // Call the base classes last.
   Actuable::preDestructor ();
   MultimediaEventListener::preDestructor ();
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::preDestructor (), complete" << endl;
   #endif
}

/// <summary>Sets the baseGamePresentationEvaluation flag for gamepresentation.  This flag determines
///   whether the reels, cards, tiles, etc. are displayed or not.  Used for Bingo</summary>
/// <param name = "base_game_presentation_evaluatation">[in] New flag value to be stored (0 or 1)</param>
void GamePresentation::setBaseGamePresentationEvaluationFlag (bool base_game_presentation_evaluation)
{
   BaseGamePresentationEvaluationSequenceEvent base_event (base_game_presentation_evaluation);
   postEvent (base_event);
   base_game_presentation_evaluation_flag = base_game_presentation_evaluation;

   Mojo<PropertyInterface> property;
   String base_game_presentation_evaluation_property_name = BASE_GAME_PRESENTATION_EVALUATION;

   if (resource_list->getResource<PropertyInterface> (base_game_presentation_evaluation_property_name, resource_list->resourceKey (), property, false))
   {
      bool current_value;
      property->getAbsoluteValue (current_value);
      if (current_value != base_game_presentation_evaluation)
      {
         property->setAbsoluteValue (base_game_presentation_evaluation);
         String event ("Update");
         event << base_game_presentation_evaluation_property_name;
         postEvent (event);
      }
   }
}

/// <summary>Gets the baseGamePresentationEvaluation flag for gamepresentation.  This flag determines
///   whether the reels, cards, tiles, etc. are displayed or not.  Used for Bingo</summary>
bool GamePresentation::baseGamePresentationEvaluation (void) const
{
   return base_game_presentation_evaluation_flag;
}

// Sets the base game evaluation clear flag, this allows clearing of display meters when enabling/disabling of base game (Bingo).
void GamePresentation::setBaseGamePresentationMeterClearFlag (bool base_game_presentation_meter_display_clear)
{
   base_game_presentation_meter_clear_flag = base_game_presentation_meter_display_clear;
}
// Return the clear flag.
bool GamePresentation::baseGamePresentationMeterDisplayClear (void) const
{
   return base_game_presentation_meter_clear_flag;
}

// Purpose: Accelerate the rollup rate of the game control panel meters.
void GamePresentation::accelerateRollup (void)
{
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel) && !isRollupComplete ())
     game_control_panel.getGameControlPanelInterface ()->accelerateRollup ();
}

// Purpose:  Adds the listener to the main_stage's listener list.  Returns true on success.
bool GamePresentation::addEventListener (Mojo<MultimediaEventListenerInterface> listener)
{
   bool success = false;
   // Retrieve the main stage from the resource manager.
   StageInterfaces main_stage;
   getStage (main_stage);
   // If the listener received and the stage extracted are both valid, add the listener to the stage.
   if (listener.isValid () && main_stage.isValid ())
   {
      listener->registerListener (main_stage.getActorStageInterface ());
      success = true;
   }
   return success;
}

// Purpose:  Adds the listener received to the sequence event listener list.  Returns
// true if the listener is added successfully.
bool GamePresentation::addEventListener (Mojo<SequenceEventListenerInterface> listener)
{
   bool success = false;
   if (listener.isValid () && sequence_event_listener_controller.isValid ())
      success = sequence_event_listener_controller->add (listener);
   return success;
}

// Purpose: Query the specified object for interfaces that should be put into lists
void GamePresentation::addToLists (UnknownInterface *object)
{
   // Does this object support the configuration changed interface?
   Mojo<ConfigurationChangedHandlerInterface> changed = object;
   if (changed.isValid ())
      configuration_changed_list.push_back (object);
}

// Purpose:  Retrieve the Amount of the current player wagerable meter
void GamePresentation::wagerableDisplayAmount (Amount &wagerable_amount)
{
   verboseMethodName ();
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record)
      wagerable_amount = essential_record->wagerableAmount ();
   else
      wagerable_amount = ZERO_AMOUNT;
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::wagerableDisplayAmount = " << wagerable_amount << endl;
   #endif
}

// Purpose:  The following method returns true if the game is in a state that allows betting.
bool GamePresentation::canBet (void)
{
   bool can_bet = ((isFlowStateIdle () && /*(currentAwardContainsNewRunnableTriggers () == false) &&*/ stageIndex () == BASE_GAME_STAGE) || (getFlowState () == GameStateValue::STATE_UTILITY));
   #ifdef __VERBOSE_BET__
   debug << game_index << ":" << stage_index << "|GamePresentation::canBet=" << (can_bet ? "true" : "false") << " FlowState=" << getFlowState () << endl;
   #endif
   return can_bet;
}

// Purpose:  Returns true the flow has determined that it's ok to show the award to the player.
bool GamePresentation::canDisplayAward (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   #ifdef __VERBOSE_METERS__
   bool can_display_award = (essential_record != 0 && essential_record->canDisplayAward ()) || getFlowState () == GameStateValue::STATE_HISTORY;
   debug << game_index << ":" << stage_index << "|GamePresentation::canDisplayAward ():  " << (int) can_display_award << endl;
   #endif
   return (((essential_record != 0 && essential_record->canDisplayAward ()) || getFlowState () == GameStateValue::STATE_HISTORY)
             && !baseGamePresentationMeterDisplayClear ());
}

// Purpose: Determine if the game can start from its current state.
bool GamePresentation::canStart (void)
{
   return (isFlowStateIdle ());
}

// Purpose:  Ask the game to change context modes.
bool GamePresentation::changeContextMode (State &state, const ContextMode &context_mode)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::changeContextMode (" << context_mode << ")" << endl;
   #endif
   // Initialize the return value.
   bool success = true;
   // If the game is not operating in a known mode, ignore this message.  If it is,
   // change the current context mode and keep the current game mode.
   if (currentGameMode () != GAME_MODE_UNKNOWN && context_mode != currentContextMode ())
   {
      success = enterGameMode (state, currentGameMode (), context_mode);
   }
   #ifdef __VERBOSE_MODE__
   else
      debug << game_index << ":" << stage_index << "|GamePresentation::changeContextMode (), ignoring" << endl;
   #endif
   return success;
}

// Purpose: Provides state transition decision making for presentation states.
// Returns true if all the conditions specified in the string are true.
// All unsupported conditions are stored in unsupported_conditions.
// Unsupported conditions don't effect the return value.
bool GamePresentation::checkPostWaitEventConditions (const vector<String> &conditions, vector<String> &unsupported_conditions, bool force_all)
{
   verboseMethodName ();
   // Initialize return values. All conditions true until one is determined false.
   bool result = true;
   unsupported_conditions.clear ();
   // For every condition specified...
   for (uint32 index = 0; (index < conditions.size ()) && (force_all || result); ++index)
   {
      if (conditions [index] == CONDITION_FIRST_GAME)
         // Return true if this is the first game play.
         result &= processConditionFirstGame ();
      else if (conditions [index] == CONDITION_NOT_FIRST_GAME)
         // Return true if this is NOT the first game play.
         result &= !processConditionFirstGame ();
      else if (strncmp (conditions [index], CONDITION_PLAY_COUNT, strlen (CONDITION_PLAY_COUNT)) == 0)
         // Example string is "PLAY_COUNT:BONUS:0,1,2".
         // Return true if the play count matches one of the specified values for that act type.
         result &= processConditionPlayCount (conditions [index].right (conditions [index].length () - strlen (CONDITION_PLAY_COUNT)));
      else if (strncmp (conditions [index], CONDITION_NOT_PLAY_COUNT, strlen (CONDITION_NOT_PLAY_COUNT)) == 0)
         // Example string is "NOT_PLAY_COUNT:BONUS:0,1,2".
         // Return true if the play count matches one of the specified values for that act type.
         result &= !processConditionPlayCount (conditions [index].right (conditions [index].length () - strlen (CONDITION_NOT_PLAY_COUNT)));
      else if (strncmp (conditions [index], CONDITION_GAMES_REMAINING, strlen (CONDITION_GAMES_REMAINING)) == 0)
         // Example string is "GAMES_REMAINING:0,1,2".
         // Return true if the games remaining count matches one of the specified values.
         result &= processConditionGamesRemaining (conditions [index].right (conditions [index].length () - strlen (CONDITION_GAMES_REMAINING)));

      else if (conditions [index] == CONDITION_LAST_GAME)
         // Return true if this is the last game play.
         result &= processConditionLastGame ();
      else if (conditions [index] == CONDITION_NOT_LAST_GAME)
         // Return true if this is NOT the last game play.
         result &= !processConditionLastGame ();

      else if (conditions [index] == CONDITION_WIN_THIS_ACT)
         // Returns true if an amount was won for the current game.
         result &= processConditionWinThisAct ();
      else if (conditions [index] == CONDITION_NO_WIN_THIS_ACT)
         // Returns true if an amount was won for the current game.
         result &= !processConditionWinThisAct ();
      else if (conditions [index] == CONDITION_AWARD_THIS_ACT)
         // Returns true if an amount or trigger was won for the current game.
         result &= processConditionAnyAwardThisAct ();
      else if (conditions [index] == CONDITION_NO_AWARD_THIS_ACT)
         // Returns true if an amount or trigger was NOT won for the current game.
         result &= !processConditionAnyAwardThisAct ();
      else if (strncmp (conditions [index], CONDITION_IS_ACT_TYPE, strlen (CONDITION_IS_ACT_TYPE)) == 0)
         // Returns true if the current act_type is act_name.
         result &= processConditionIsActType (conditions [index].right (conditions [index].length () - strlen (CONDITION_IS_ACT_TYPE)));
      else if (strncmp (conditions [index], CONDITION_NOT_ACT_TYPE, strlen (CONDITION_NOT_ACT_TYPE)) == 0)
         // Returns true if the current act_type is NOT act_name.
         result &= !processConditionIsActType (conditions [index].right (conditions [index].length () - strlen (CONDITION_NOT_ACT_TYPE)));

      else if (strncmp (conditions [index], CONDITION_WIN_CATEGORY, strlen (CONDITION_WIN_CATEGORY)) == 0)
         // Example string is "WIN_CATEGORY:1,2,3".
         // Return true if a hit win category matches one of those specified.
         result &= processConditionWinCategory (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_CATEGORY)));
      else if (strncmp (conditions [index], CONDITION_NOT_WIN_CATEGORY, strlen (CONDITION_NOT_WIN_CATEGORY)) == 0)
         // Example string is "NOT_WIN_CATEGORY:1,2,3".
         // Return true if no hit win category matches those specified.
         result &= processConditionNotWinCategory (conditions [index].right (conditions [index].length () - strlen (CONDITION_NOT_WIN_CATEGORY)));

      else if (strncmp (conditions [index], CONDITION_WIN_GREATER_THAN_TOTAL_BET_X, strlen (CONDITION_WIN_GREATER_THAN_TOTAL_BET_X)) == 0)
         // Example string is "CONDITION_WIN_GREATER_THAN_TOTAL_BET_X:7". win is greater than 7 times total bet
         // Example string is "CONDITION_WIN_GREATER_THAN_TOTAL_BET_X:7,15". win is between 7 and 15 times total bet inclusive
         // Return true if the win is greater than or equal to the specified value or within the specified range
         result &= processConditionWinGreaterThanTotalBetX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_GREATER_THAN_TOTAL_BET_X)));
      else if (strncmp (conditions [index], CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X, strlen (CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X)) == 0)
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionWinGreaterThanTotalBetX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X)));

      else if (strncmp (conditions [index], CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X, strlen (CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X)) == 0)
         // Example string is "CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X:7". win is greater than or equal to 7 times total bet
         // Example string is "CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X:7,15". win is between 7 and 15 times total bet inclusive
         // Return true if the win is greater than or equal to the specified value or within the specified range
         result &= processConditionWinGreaterThanOrEqualToTotalBetX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X)));
      else if (strncmp (conditions [index], CONDITION_WIN_NOT_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X, strlen (CONDITION_WIN_NOT_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X)) == 0)
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionWinGreaterThanOrEqualToTotalBetX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X)));

      else if (strncmp (conditions [index], CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X, strlen (CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X)) == 0)
         // Example string is "CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X:7". win is greater than 7 times total bet
         // Example string is "CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X:7,15". win is between 7 and 15 times total bet inclusive
         // Return true if the win is greater than or equal to the specified value or within the specified range
         result &= processConditionWinGreaterThanSubBetXMaxSubBetsX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X)));
      else if (strncmp (conditions [index], CONDITION_WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X, strlen (CONDITION_WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X)) == 0)
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionWinGreaterThanSubBetXMaxSubBetsX (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X)));

      else if (strncmp (conditions [index], CONDITION_WIN_GREATER_THAN, strlen (CONDITION_WIN_GREATER_THAN)) == 0)
         // Example string is "CONDITION_WIN_GREATER_THAN:7". win is greater than 7 credits
         // Example string is "CONDITION_WIN_GREATER_THAN:7,15". win is greater than 7 less than or equal to 15 credits
         // Return true if the win is greater than or equal to the specified value or within the specified range
         result &= processConditionWinGreaterThan (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_GREATER_THAN)));
      else if (strncmp (conditions [index], CONDITION_WIN_NOT_GREATER_THAN, strlen (CONDITION_WIN_NOT_GREATER_THAN)) == 0)
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionWinGreaterThan (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_NOT_GREATER_THAN)));

      else if (strncmp (conditions [index], CONDITION_WIN_THIS_ACT_GREATER_THAN, strlen (CONDITION_WIN_THIS_ACT_GREATER_THAN)) == 0)
         // Example string is "CONDITION_WIN_THIS_ACT_GREATER_THAN:7". win is greater than 7 credits
         // Example string is "CONDITION_WIN_THIS_ACT_GREATER_THAN:7,15". win is greater than 7 less than or equal to 15 credits
         // Return true if the win is greater than or equal to the specified value or within the specified range
         result &= processConditionWinThisActGreaterThan (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_THIS_ACT_GREATER_THAN)));
      else if (strncmp (conditions [index], CONDITION_WIN_THIS_ACT_NOT_GREATER_THAN, strlen (CONDITION_WIN_THIS_ACT_NOT_GREATER_THAN)) == 0)
         // Return true if the win is not greater than or equal to the specified value or within the specified range
         result &= !processConditionWinThisActGreaterThan (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_THIS_ACT_NOT_GREATER_THAN)));

      else if (conditions [index] == CONDITION_ROLLUP_COMPLETE)
         // Returns true if the rollup is complete.
         result &= processConditionRollupComplete ();
      else if (conditions [index] == CONDITION_NOT_ROLLUP_COMPLETE)
         // Returns true if the rollup is complete.
         result &= !processConditionRollupComplete ();

      else if (conditions [index] == CONDITION_PROGRESSIVE_LINKED)
         // Returns true if the progressive is linked.
         result &= processConditionProgressiveLinked ();
      else if (conditions [index] == CONDITION_NO_PROGRESSIVE_LINKED)
         // Returns true if there is no progressive linked.
         result &= !processConditionProgressiveLinked ();

      else if (conditions [index] == CONDITION_PLAYER_SELECTABLE_DENOMINATION)
         // Return true if multi-denom is enabled.
         result &= processConditionPlayerSelectableDenomination ();
      else if (conditions [index] == CONDITION_NO_PLAYER_SELECTABLE_DENOMINATION)
         // Return true if multi-denom is NOT enabled.
         result &= !processConditionPlayerSelectableDenomination ();

      else if (conditions [index] == CONDITION_PLAYER_SELECTABLE_GAME)
         // Return true if multi-game is enabled.
         result &= processConditionPlayerSelectableGame ();
      else if (conditions [index] == CONDITION_NO_PLAYER_SELECTABLE_GAME)
         // Return true if multi-game is NOT enabled.
         result &= !processConditionPlayerSelectableGame ();

      else if (conditions [index] == CONDITION_ENROLLMENT_REQUIRED)
         // Return true if central determination enrollment is required at all for this game.
         result &= processConditionEnrollmentRequired ();
      else if (conditions [index] == CONDITION_NOT_ENROLLMENT_REQUIRED)
         // Return true if central determination enrollment is NOT required at all for this game.
         result &= !processConditionEnrollmentRequired ();

      else if (conditions [index] == CONDITION_ENROLLMENT_BEFORE_GAME_START)
         // Return true if central determination enrollment is required to complete before the game starts.
         result &= processConditionEnrollmentBeforeGameStart ();
      else if (conditions [index] == CONDITION_NOT_ENROLLMENT_BEFORE_GAME_START)
         // Return true if central determination enrollment is NOT required to complete before the game starts.
         result &= !processConditionEnrollmentBeforeGameStart ();

      else if (conditions [index] == CONDITION_GAME_WAS_UNENROLLED)
         // Return true if a game was unenrolled since the last play.
         result &= processConditionGameWasUnenrolled ();
      else if (conditions [index] == CONDITION_NOT_GAME_WAS_UNENROLLED)
         // Return true if a game was NOT unenrolled since the last play.
         result &= !processConditionGameWasUnenrolled ();

      else if (conditions [index] == CONDITION_GAME_ENROLLMENT_SEEDS_ARRIVED)
         // Return true if the central determination RNG seeds have arrived for this game.
         result &= processConditionGameEnrollmentSeedsArrived ();
      else if (conditions [index] == CONDITION_NOT_GAME_ENROLLMENT_SEEDS_ARRIVED)
         // Return true if the central determination RNG seeds have NOT arrived for this game.
         result &= !processConditionGameEnrollmentSeedsArrived ();

      else if (conditions [index] == CONDITION_ENROLLMENT_BUTTON_DOUBLE_TAP)
         // Return true if a central determination game has hit the spin button twice.
         result &= processConditionEnrollmentButtonDoubleTap ();
      else if (conditions [index] == CONDITION_NOT_ENROLLMENT_BUTTON_DOUBLE_TAP)
         // Return true if a central determination game has NOT hit the spin button twice.
         result &= !processConditionEnrollmentButtonDoubleTap ();

      else if (strncmp (conditions [index], CONDITION_PAYTABLE_SECTION, strlen (CONDITION_PAYTABLE_SECTION)) == 0)
         // Example string is "PAYTABLE_SECTION:Stage1".
         // Return true if the current paytable section is Stage1.
         result &= processConditionPaytableSection (conditions [index].right (conditions [index].length () - strlen (CONDITION_PAYTABLE_SECTION)));
      else if (strncmp (conditions [index], CONDITION_NOT_PAYTABLE_SECTION, strlen (CONDITION_NOT_PAYTABLE_SECTION)) == 0)
         // Example string is "NOT_PAYTABLE_SECTION:Stage1".
         // Return true if the current paytable section is NOT Stage1.
         result &= !processConditionPaytableSection (conditions [index].right (conditions [index].length () - strlen (CONDITION_NOT_PAYTABLE_SECTION)));
      else if (conditions [index] == CONDITION_PLAYER_SELECTABLE_VOLUME)
         // Return true if no award digits still need to be picked.
         result &= processConditionPlayerSelectableVolume ();
      else if (conditions [index] == CONDITION_NO_PLAYER_SELECTABLE_VOLUME)
         // Return true if any award digits still need to be picked.
         result &= !processConditionPlayerSelectableVolume ();
      else if (conditions [index] == CONDITION_CAN_DISPLAY_AWARD)
         // Return true if the award can be displayed.
         result &= canDisplayAward ();
      else if (conditions [index] == CONDITION_CANNOT_DISPLAY_AWARD)
         // Return true if the award can't be displayed.
         result &= !canDisplayAward ();
      else if (conditions [index] == CONDITION_SPEED_PLAYER_MODIFIABLE)
         // Return true if the player can modify the speed.
         result &= isSpeedPlayerModifiable ();
      else if (conditions [index] == CONDITION_SPEED_NOT_PLAYER_MODIFIABLE)
         // Return true if the player can not modify the speed.
         result &= !isSpeedPlayerModifiable ();
      else if (conditions [index] == CONDITION_CAN_START_GAME)
         // Return true if the player can modify the speed.
         result &= bet->canStartGame ();
      else if (conditions [index] == CONDITION_CAN_NOT_START_GAME)
         // Return true if the player can not modify the speed.
         result &= !bet->canStartGame ();
      else if (conditions [index] ==  CONDITION_TOURNAMENT_ENABLED)
         // Return true if tournament has been enabled for the system.
         result &= SystemFlags ().systemInTournamentMode ();
      else if (conditions [index] ==  CONDITION_TOURNAMENT_DISABLED)
         // Return true if tournament is disabled for the system.
         result &= !(SystemFlags ().systemInTournamentMode ());
      else if (conditions [index] == CONDITION_GET_FLOW_STATE_IDLE)
      {
         result &= isFlowStateIdle ();
      }
      else if (conditions [index] == CONDITION_GET_FLOW_STATE_NOT_IDLE)
      {
         result &= !isFlowStateIdle ();
      }
      else if (strncmp (conditions [index], CONDITION_AWARD_TYPE, strlen (CONDITION_AWARD_TYPE)) == 0)
      {
         // Example string is "AWARD_TYPE:BINGO".
         // Return true if an award matches the type requested.
         result &= processConditionAwardType (conditions [index].right (conditions [index].length () - strlen (CONDITION_AWARD_TYPE)));
      }
      else if (conditions [index] == CONDITION_DOUBLEUP_NOT_PLAYED)
      {
         result &= !((doubleUpWin () || checkDoubleUpLoss ()));
      }
      else if (conditions [index] == CONDITION_DOUBLEUP_WON)
      {
         result &= doubleUpWin ();
      }
      else if (conditions [index] == CONDITION_DOUBLEUP_LOST)
      {
         result &= checkDoubleUpLoss ();
      }
      else if (conditions [index] == CONDITION_AUTO_PLAY_SIMULATE_INPUT)
      {
         result &= processConditionAutoPlaySimulateInput ();
      }
      else if (conditions [index] == CONDITION_WIN_ALL_ACTS)
      {
         // returns true if a monetary win is award for any acts in the current game.
         result &= winAllActs ();
      }
      else if (conditions [index] == CONDITION_NO_WIN_ALL_ACTS)
      {
         // returns true if a monetary win is not award for any acts in the current game.
         result &= !winAllActs ();
      }
      else if (conditions [index] == CONDITION_RMLP_TRIGGERED)
      {
         result &= rmlpTriggered ();
      }
      else if (conditions [index] == CONDITION_RMLP_NOT_TRIGGERED)
      {
         result &= !rmlpTriggered ();
      }
      else
         // Report conditions not handled by this routine.
         unsupported_conditions.push_back (conditions [index]);
   }
   // Return true if all the handled conditions where true.
   return result;
}

/// <summary>Sets the win meter to zero</summary>
void GamePresentation::clearWinMeter ()
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::clearWinMeter ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->setWinMeter (ZERO_AMOUNT, RollupDefs::SNAP_TO_AMOUNT);
   }
}

// Purpose: Compares players win to players bet times a multiplier
void GamePresentation::compareWinToBetMultiplier (String mode, float multiplier, String if_event, String else_event)
{
   String event_to_post;
   // Get the amount of the current bet.
   Amount credits_bet = bet->getBetAmount ();
   int64 credits_bet_value = getAmountValue (credits_bet);
   // Get the amount of credits won for the current act
   Amount credits_won;
   extractWinForThisAct (credits_won);
   int64 credits_won_value = getAmountValue (credits_won);

   #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
      debug << endl << "compareWinToBetMultiplier:Credits_Won: " << credits_won_value << "  Bet: " <<  credits_bet_value  << endl;
   #endif

   if (!strcmp (mode, "COMPARE_LESS_THAN"))
   {
      event_to_post = (credits_won_value < (int64)((multiplier * credits_bet_value) + 0.5)) ? if_event : else_event;

      #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
         debug << "event_to_post -> (" << credits_won_value << ") < (int64)((" << multiplier << " * " << credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") < (int64)((" << multiplier  * credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") < " << (int64)((multiplier  * credits_bet_value ) + 0.5 ) << " ? " << if_event << " : " << else_event << endl;
      #endif
   }
   else if (!strcmp (mode, "COMPARE_LESS_THAN_OR_EQUALS"))
   {
      event_to_post = (credits_won_value <= (int64)((multiplier * credits_bet_value) + 0.5)) ? if_event : else_event;

      #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
         debug << "event_to_post -> (" << credits_won_value << ") <= (int64)((" << multiplier << " * " << credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") <= (int64)((" << multiplier  * credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") <= " << (int64)((multiplier  * credits_bet_value ) + 0.5 ) << " ? " << if_event << " : " << else_event << endl;
      #endif
   }
   else if (!strcmp (mode, "COMPARE_GREATER_THAN"))
   {
      event_to_post = (credits_won_value > (int64)((multiplier * credits_bet_value) + 0.5)) ? if_event : else_event;

      #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
         debug << "event_to_post -> (" << credits_won_value << ") > (int64)((" << multiplier << " * " << credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event <endl;
         debug << "event_to_post -> (" << credits_won_value << ") > (int64)((" << multiplier  * credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") > " << (int64)((multiplier  * credits_bet_value ) + 0.5) << " ? " << if_event << " : " << else_event << endl;
      #endif
   }
   else if (!strcmp (mode, "COMPARE_GREATER_THAN_OR_EQUALS"))
   {
      event_to_post = (credits_won_value >= (int64)((multiplier * credits_bet_value) + 0.5)) ? if_event : else_event;

      #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
         debug << "event_to_post -> (" << credits_won_value << ") >= (int64)((" << multiplier << " * " << credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") >= (int64)((" << multiplier  * credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") >= " << (int64)((multiplier  * credits_bet_value ) + 0.5) << " ? " << if_event << " : " << else_event << endl;
      #endif
   }
   else if (!strcmp (mode, "COMPARE_EQUALS"))
   {
      event_to_post = (credits_won_value == (int64)((multiplier * credits_bet_value) + 0.5)) ? if_event : else_event;

      #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
         debug << "event_to_post -> (" << credits_won_value << ") = (int64)((" << multiplier << " * " << credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") = (int64)((" << multiplier  * credits_bet_value << ") + 0.5)) ? " << if_event << " : " << else_event << endl;
         debug << "event_to_post -> (" << credits_won_value << ") = " << (int64)((multiplier  * credits_bet_value ) + 0.5) << " ? " << if_event << " : " << else_event << endl;
      #endif
   }
   else
   {
      String error_message;
      error_message <<"compareWinToBetMultiplier: Invalid compare mode" << mode;
      System::error (LogicError (LogicError::INVALID_VALUE, error_message));
   }

   #ifdef __VERBOSE_COMPARE_WIN_TO_BET_MULTIPLIER__
      debug << "compareWinToBetMultiplier: event_to_post -> " << event_to_post << endl;
   #endif
   postEvent (event_to_post);
}

// Purpose: Get a list of config items that the game cares about.  If the pointer is non zero, the
//          number_of_items parameter will be set to how many items the pointer points at.
const char *const *GamePresentation::configItems (uint32 &number_of_items) const
{
   return 0;
}

// Purpose: Allow the presentation script to be broken up into smaller scripts.
void GamePresentation::configureFromScript (const char *script_file_name)
{
   verboseMethodName ();
   if (!resource_list.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::configureFromScript: Resource list invalid."));
   if (!persistent_property_list.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::configureFromScript: Persistent property list invalid."));
   if (!script_file_name)
      System::error (LogicError (LogicError::INVALID_PARAMETER, "GamePresentation::configureFromScript: Script name is null."));

   // Create the script resource importer.  Rely on the resource manager to error if the parser can not be created.
   Mojo<ScriptResourceImporterInterface> script_parser;
   createResource<ScriptResourceImporterInterface> (script_file_name, resource_list->resourceKey (), script_parser);
   // Now parse the script.  By default, the following call will system error upon failure.
   Mojo<ScriptInterface> scriptable_object (this);
   script_parser->parseFile (scriptable_object);
}

// Purpose: Configure Auto Play message on control panel
void GamePresentation::configureAutoPlayMessage (void)
{
   ConfigID show_autoplay_message_config_id = config.getId (CONFIG_ID_SHOW_AUTOPLAY_MESSAGE, game_configuration_identifier, false);
   String autoplay_state_value;
   config.getValueString (autoplay_state_config_id, autoplay_state_value);
   if ((show_autoplay_message_config_id != INVALID_CONFIGID) && config.getValue (show_autoplay_message_config_id))
   {
      synchronizeAutoplayMessage ();
      //Refresh the autoplay message.
      if (autoplay_state_value != "DISABLED")
      {
         postEvent ("ControlPanel:AutoPlay");
      }
      else
      {
         postEvent ("ControlPanel:HideGameOver");
      }
   }
}

// Purpose: Handle the configuration item that has changed
void GamePresentation::configurationChanged (const ConfigID &config_id)
{
   // Is this an item that should be updated?
   if (config_group.notifyItemChanged (config_id))
      updateConfigItem (config_group.configID (config_id));
   // Notify any other objects that were loaded.
   ConfigurationChangedIterator end (configuration_changed_list.end ());
   for (ConfigurationChangedIterator iterator (configuration_changed_list.begin ()); iterator != end; ++iterator)
      (*iterator)->configurationChanged (config_id);
   if (config_id == game_language_config_id)
      postEvent ("ConfigurationChanged:GameLanguage");
   else if (config_id == language_licensed_bitfield_config_id)
   {
      synchronizeGameLanguage (true);
   }
   else if (config_id == number_of_playable_themes_config_id)
   {
      setMenuProperty ();
      postPlayerSelectableGameStatus ();
   }
   else if (config_id == autoplay_state_config_id)
   {
      // configure Auto Play Message every time auto play state has changed.
      configureAutoPlayMessage ();
   }
   else if (config_id == who_can_start_autoplay_config_id)
   {
      synchronizeAutoplayButtonPresentation (true); // Force synchronization when the config item changes
   }
   else if (config_id == all_meter_display_behavior_config_id)
   {
      // Synchronize the presentation.
      synchronizeMeters ();
   }
   else if (config_id == theme_displayed_to_player_denominations_id
            || config_id == terminal_enabled_player_denominations_id)
   {
      // Synchronize denomination button and denomination mode upon a denomination change
      synchronizeDenomButton ();
      synchronizeDenominationMode ();

      postEvent ("DenomMenu:DenominationConfigChanged");
   }
   else if(config_id == minimum_wager_interval_id)
   {
      uint32 min_interval = config.getValue (minimum_wager_interval_id);
      ScriptTableParameter payload;
      ScriptParameter min_interval_param (min_interval, ParameterType::UINT32);
      payload.script_table ["min_interval"] = min_interval_param;

      postEventExtended ("MinimumWagerInterval:ConfigChanged", 0, &payload);
   }

   // Update the max bet and paytable awards properties for display due to a max bet configuration change
   String config_name;
   if ((config_id != INVALID_CONFIGID) && config.getName (config_id, config_name, false))
   {
       if (config_name.length () >= strlen (CONFIG_ID_MAXIMUM_TOTAL_BET)&&
          strncmp (CONFIG_ID_MAXIMUM_TOTAL_BET, config_name.c_str (), strlen (CONFIG_ID_MAXIMUM_TOTAL_BET)) == 0)
      {
         updateMaxBetProperty ();
         updatePaytableAwardProperties ();
      }
      StageInterfaces main_stage;
      getStage (main_stage);
      // Post a multimedia event to the lua to notify the config item changed
      if (main_stage.isValid ())
      {
         ScriptTableParameter table_param;
         table_param.script_table ["changed_config_item_name"] = ScriptParameter (config_name, ParameterType::STRING);
         main_stage.getActorStageInterface ()->postEventExtended ("Presentation:ConfigurationItemChanged", 0, &table_param);
      }
   }
}

// The following method updates the control panel interface received with the registered game control panel.
// If the control panel is valid, this method will return true.
bool GamePresentation::controlPanel (GameControlPanelExtractor &control_panel)
{
   verboseMethodName ();
   // Initialize the return value.
   bool success = false;
   // Since the validity of the control panel is being used as the return value, free the control panel received
   // just in case it is valid mojo.
   control_panel.release ();
   // If the control panel alias has been registered, retrieve the control panel by alias name.
   if (game_control_panel.length () != 0)
   {
      Mojo<ControlPanelInterface> control_panel_interface;
      // Extract the control panel by registered alias name.
      success = controlPanel (game_control_panel, control_panel_interface);
      // Initialize the return control panel with the control panel extracted above.
      control_panel.initialize (control_panel_interface);
   }
   // Return true if the control panel configured is valid.
   return (success && control_panel.isValid ());
}

// The following method updates the control panel interface received with the registered game control panel.
// If the control panel is valid, this method will return true.
bool GamePresentation::controlPanel (GameCountControlPanelExtractor &control_panel)
{
   verboseMethodName ();
   // Initialize the return value.
   bool success = false;
   // Since the validity of the control panel is being used as the return value, free the control panel received
   // just in case it is valid.
   control_panel.release ();
   // If the control panel alias has been registered, retrieve the control panel by alias name.
   if (game_count_control_panel.length () != 0)
   {
      Mojo<ControlPanelInterface> control_panel_interface;
      // Extract the control panel by registered alias name.
      success = controlPanel (game_count_control_panel, control_panel_interface);
      // Initialize the return control panel with the control panel extracted above.
      control_panel.initialize (control_panel_interface);
   }
   // Return true if the control panel configured is valid.
   return (success && control_panel.isValid ());
}

// Purpose: Get the control panel being used by the game.
bool GamePresentation::controlPanel (const char *alias, Mojo<ControlPanelInterface> &control_panel)
{
   verboseMethodName ();
   // Retrieve the control panel by alias from the resource list.
   assertion (resource_list.isValid ());
   return resource_list->getResource (alias, resource_list->resourceKey (), control_panel);
}

// Purpose:  Creates a new game set configuration as appropriate for this presentation.
bool GamePresentation::createGameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface)
{
   verboseMethodName ();
   // Create a new game set configuration manager for this presentation.
   Mojo<GameSetConfiguration, GameSetConfigurationInterface> game_set_configuration;
   game_set_configuration.create<GameSetConfiguration> ();
   // Extract the GameSetConfigurationInterface and update the reference parameter.
   game_set_configuration_interface = game_set_configuration;
   return game_set_configuration_interface.isValid ();
}

// Return an interface to an object given its id.
bool GamePresentation::createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared)
{
   verboseMethodName ();
   // Initialize the return value
   bool object_created = false;

   switch (object_id)
   {
      case PERSISTENT_PROPERTY_LIST:
         {
            // Logic error if the persistent property list doesn't exist...
            assertion (persistent_property_list.isValid ());
            persistent_property_list->setResourceKey (resource_list->resourceKey ());
            persistent_property_list->setDefaultStorageDirectory (essential_persistent_properties_directory);

            // Optionally, parse the script give by the name.
            if (parameter_list.getParameterCount () > 0)
               persistent_property_list->configureFromScript (parameter_list [0].asString ());

            // Allow the persistent property list to parse.
            new_object = persistent_property_list;
            assertion (new_object.isValid ());
            object_created = true;
         }
         break;

      case GAME_SET_CONFIGURATION:
         {
            // Create a new game set config manager.
            Mojo<GameSetConfigurationInterface> game_set_configuration_interface;
            assertion (createGameSetConfiguration (game_set_configuration_interface));

            // Make sure that the paytable has been loaded and a parser has been created.
            assertion (paytable_parser.isValid ());
            // The second parameter to the game set configuration call is the optional paytable section name.
            // If this name is provided, verify that it is a valid paytable section name.  If it is not provided,
            // assign the default section name.
            String section_name;
            if (parameter_list.getParameterCount () > 1)
            {
               section_name = parameter_list [1].asString ();
               // Verify that the section name received is valid.  The method will return -1 if the section is not valid.
               if (paytable_parser->getPayTableSectionIndex (section_name) == -1)
               {
                  String error_message ("GAME_SET_CONFIGURATION: Invalid paytable section name received: ");
                  error_message << section_name;
                  System::error (LogicError (LogicError::INVALID_VALUE, error_message));
               }
            }
            else
               section_name = paytable_parser->getPayTableSectionName (stageIndex ());

            // Verify that the paytable section is supported by this presentation.
            if (!paytableSectionSupported (section_name))
            {
               String error_message ("GAME_SET_CONFIGURATION: Paytable section (");
               error_message << section_name << ") not supported.  Use supportPaytableSection ()";
               System::error (LogicError (LogicError::INVALID_VALUE, error_message));
            }

            // Verify that a game set for the defined paytable section has not already been defined.
            GameSetMapIterator iterator = game_set_configuration_map.find (section_name);
            if (iterator == game_set_configuration_map.end ())
               game_set_configuration_map.insert (GameSetMapPair (section_name, game_set_configuration_interface));
            else
            {
               String error_message ("GAME_SET_CONFIGURATION: multiple game set configurations defined for paytable section: ");
               error_message << section_name;
               System::error (LogicError (LogicError::INVALID_VALUE, error_message));
            }

            // Prepare the game set manager for use.
            game_set_configuration_interface->setPresentation (this);
            // Prepare the game set manager for use.
            game_set_configuration_interface->setPaytableSection (section_name);
            // Return the new configuration manager's script interface.
            new_object = game_set_configuration_interface;
            object_created = new_object.isValid ();
         }
         break;

      case RESOURCE_LIST:
         // If the resource list has not yet been created, generate an error.
         assertion (resource_list.isValid ());
         new_object = resource_list;
         assertion (new_object.isValid ());
         // Allow the object to operate on its parameters.
         new_object->initializeCreatedObject (parameter_list);
         // Set the return value
         object_created = true;
         break;

      case SEQUENCE_EVENT_TRANSLATOR:
         {
            // Create a new SequenceEventTranslator.
            Mojo<SequenceEventTranslator, SequenceEventTranslatorInterface> sequence_event_translator;
            sequence_event_translator.create<SequenceEventTranslator> ();
            if (!sequence_event_translator.isValid ())
               System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::createScriptableObjectById, Unable to create SequenceEventTranslator."));

            // Allow the object to operate on its parameters.
            sequence_event_translator->initializeCreatedObject (parameter_list);

            // Configure the stage used to post events.
            sequence_event_translator->setStage (parameter_list [1].asString (), parameter_list [2].asString ());
            // Configure the translator's sequence event poster.
            sequence_event_translator->setSequenceEventPoster (sequence_event_poster);

            // Add the translator to the list of MultimediaEventListeners.
            Mojo<MultimediaEventListenerInterface> multimedia_event_listener (sequence_event_translator);
            addEventListener (multimedia_event_listener);
            assertion (multimedia_event_listener.isValid ());
            // Add the translator to the list of SequenceEventListeners.
            Mojo<SequenceEventListenerInterface> sequence_event_listener (sequence_event_translator);
            addEventListener (sequence_event_listener);
            assertion (sequence_event_listener.isValid ());

            // Update the return value with the script interface for the new translator.
            new_object = sequence_event_translator;
            assertion (new_object.isValid ());
            object_created = true;
         }
         break;

      case STATE_MACHINE:
         {
            // If the state machine doesn't exist...
            if (!state_machine.isValid ())
               System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::createScriptableObjectById: Unable to configure state machine because it does not exist."));
            // If no script was passed...
            if (parameter_list.getParameterCount () < 1)
               System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::createScriptableObjectById: State machine requires a script as a parameter."));
            // Allow the object to operate on its parameters.
            state_machine->initializeCreatedObject (parameter_list);
            // Allow the state machine to script itself.
            new_object = state_machine;
            assertion (new_object.isValid ());
            object_created = true;
         }
         break;

      default:
         object_created = Actuable::createScriptableObjectById (new_object, object_id, parameter_list, new_declared);
         break;
   }
   return object_created;
}

// Purpose:  Retrieve the Amount of the current player credit meter
void GamePresentation::creditDisplayAmount (Amount &credit)
{
   verboseMethodName ();
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record)
      credit = essential_record->playerAmount ();
   else
      credit = ZERO_AMOUNT;

   #ifdef __VERBOSE_METERS__
   Amount player_amount;
   if (essential_record)
      player_amount = essential_record->playerAmount ();
   Amount::Extractor player_amount_extractor (player_amount);
   debug << game_index << ":" << stage_index << "|GamePresentation::creditDisplayAmount, player_amount = " << player_amount_extractor.extract (denomination) << endl;
   Amount::Extractor amount_extractor (credit);
   debug << game_index << ":" << stage_index << "|GamePresentation::creditDisplayAmount, credit = " << amount_extractor.extract (denomination) << endl;
   #endif
}

/// <summary># Retrieve the Amount of the current wager credit meter.</summary>
/// <remarks># Retrieve the amount of the meter which is used to wager.
///   If there has a bank meter, the wagerable meter (current wagerable amount) is used to wager credit;
///   if there hasn't a bank meter, the credit meter (current player amount)is used to wager credit</remarks>
/// <param name="credit">#[in] The credit amount.</param>
void GamePresentation::wagerCreditDisplayAmount (Amount &credit)
{
   verboseMethodName ();
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record)
   {
      if (money_handling_style == MONEY_HANDLING_STYLE_BANKED_CREDITS)
         credit = essential_record->wagerableAmount ();
      else
         credit = essential_record->playerAmount ();
   }
   else
      credit = ZERO_AMOUNT;

   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::wagerCreditDisplayAmount, credit = " << credit << endl;
   #endif
}

// Purpose:  Return the current context mode.
const ContextMode& GamePresentation::currentContextMode (void) const
{
   return current_context_mode;
}

// Purpose: Return the current game mode of this context.   These are modes specific to the game context.
// Refer to GameDefs.hpp for valid modes.
const GameMode& GamePresentation::currentGameMode (void) const
{
   return current_game_mode;
}

// Returns the index of the currently configured game set.
uint32 GamePresentation::currentGameSetIndex (void)
{
   return current_game_set_index;
}

// Purpose: Returns the current paytable section.
const char *GamePresentation::currentPaytableSection (void) const
{
   return current_paytable_section;
}

// Returns the current paytable section.  If the section name has not been formally assigned,
// use the default paytable section name.
bool GamePresentation::currentPaytableSection (String &section_name) const
{
   // Determine the paytable section name.  If the section name has not been formally assigned,
   // use the default paytable section name.
   section_name = currentPaytableSection ();
   if (section_name.length () == 0 && paytable_parser.isValid ())
      section_name = paytable_parser->getPayTableSectionName (stageIndex ());
   return section_name.length () != 0;
}

/// <summary>Determines the type of evaluator the game is using</summary>
/// <returns>"AVP" for the old evaluator and "BBE" for the BBE</returns>
String GamePresentation::getEvaluatorType () const
{
   String paytable_name = getPaytableFilename ();
   size_t last_dot_position = paytable_name.findFromRight ('.');
   String extension = paytable_name.right (paytable_name.length () - last_dot_position - 1).toLowerCase ();
   String evaluator_type = "AVP";

   if (extension == "xml")
   {
      evaluator_type = "BBE";
   }

   return evaluator_type;
}

// Purpose:  Tell the game to start/stop displaying its win information.
void GamePresentation::displayWin (bool show, bool start_new_win_cycle)
{
}

/// <summary>Get the current bet mojo</summary>
/// <param name="_bet">[out] Reference to bet object mojo</param>
/// <returns>True if the mojo is valid, false otherwise</returns>
/// <example><b>Example:</b><br>
/// @extExampleStart{GamePresentation}
/// @extExample{gameBet}
/// @extExampleEnd
/// </example>
/// @scriptableFunction{GamePresentation,getBet}
bool GamePresentation::getBet (Mojo<BetInterface> &_bet)
{
   _bet = bet;
   return (_bet.isValid ());
}

/// <summary>Get the config item value.</summary>
/// <remarks>
///    The CONFIG_TYPE_LIST, CONFIG_TYPE_BITFIELD and CONFIG_TYPE_AMOUNT are not support.
///    The function is designed for compatibility with Flow object.
///    It is recommanded to use Theatre's getConfigItem() lua function normally.
/// </remarks>
/// <param name="config_item_name">[in] configuration item name</param>
/// <param name="unique_id">[in] String containing a unique indentifier for place of configuration file, such as
///    stage, game or theme.</param>
/// <param name="error">[in] If 'error' is true, a system error is generated if the name (with unique id) is not found
///    if 'error' is false, this method returns INVALID_CONFIGID</param>
/// <returns>ScriptParameter containing value for the config item</returns>
/// @scriptableFunction{GamePresentation,getConfigItem}
ScriptParameter GamePresentation::getConfigItem (const String& config_item_name, const String& unique_id, bool error)
{
   Config config;
   ConfigID config_id = INVALID_CONFIGID;
   if (unique_id.length ())
   {
      config_id = config.getId (config_item_name, unique_id, error);
   }
   else
   {
      config_id = config.getId (config_item_name, error);
   }

   if (config_id == INVALID_CONFIGID)
   {
      String error_message;
      error_message << "The ConfigID for " << unique_id << ' ' << config_item_name << " is invalid.";
      System::error (LogicError (LogicError::INVALID_VALUE, error_message));
   }
   return getConfigItem (config_id);
}

/// <summary>Get the config item value.</summary>
/// <remarks>The CONFIG_TYPE_LIST, CONFIG_TYPE_BITFIELD and CONFIG_TYPE_AMOUNT are not support.</remarks>
/// <param name="config_id">[in] configuration id</param>
/// <returns>ScriptParameter containing value for the config item</returns>
ScriptParameter GamePresentation::getConfigItem (ConfigID config_id)
{
   Config config;
   ConfigType config_type = config.getType (config_id);
   if (config_type == CONFIG_TYPE_INVALID)
   {
      String error_message;
      error_message << "The ConfigType of ConfigID " << config_id << " is invalid.";
      System::error (LogicError (LogicError::INVALID_VALUE, error_message));
   }

   ScriptParameter parameter;
   switch (config_type)
   {
      case CONFIG_TYPE_ITEM:
      case CONFIG_TYPE_LITERAL:
         {
            String value;
            config.getValueString (config_id, value);
            parameter = ScriptParameter (value, ParameterType::STRING);
         }
         break;

      case CONFIG_TYPE_NUMERIC:
         {
            parameter = ScriptParameter (config.getValue (config_id), ParameterType::UINT32);
         }
         break;

      case CONFIG_TYPE_BOOLEAN:
         {
            parameter = ScriptParameter (((bool)config.getValue (config_id)), ParameterType::BOOL);
         }
         break;

      case CONFIG_TYPE_NUMERIC64:
         {
            parameter = ScriptParameter (config.getValue64 (config_id), ParameterType::UINT64);
         }
         break;

      case CONFIG_TYPE_FLOAT:
         {
            parameter = ScriptParameter (config.getValueFloat (config_id), ParameterType::FLOAT);
         }
         break;

      default:
         {
            // CONFIG_TYPE_LIST, CONFIG_TYPE_BITFIELD and CONFIG_TYPE_AMOUNT are not supported.
            parameter = ScriptParameter ("The value is invalid because of the unsupported script parameter.", ParameterType::STRING);
         }
         break;
   }

   return parameter;
}

// Return the property value for the given property with the given key. If property is not found, false is returned and value is not modified.
bool GamePresentation::getPropertyValueUint32 (const char * property_name, const char * key, uint32 & value)
{
   bool success = false;

   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   if (resourceList (resource_list) && property_name)
   {
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.
      if (resource_list->getResource<PropertyInterface> (property_name, key, property, false))
      {
         property->getAbsoluteValue (value);
         success = true;
      }
   }
   return success;
}

// Return the property value for the given property with the given key. If property is not found, false is returned and value is not modified.
bool GamePresentation::getPropertyValueInt64 (const char * property_name, const char * key, int64 & value)
{
   bool success = false;
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   if (resourceList (resource_list) && property_name)
   {
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.
      if (resource_list->getResource<PropertyInterface> (property_name, key, property, false))
      {
         property->getAbsoluteValue (value);
         success = true;
      }
   }
   return success;
}


// Return the property String for the given property with the given key. If property is not found, false is returned and value is not modified.
bool GamePresentation::getPropertyString (const char * property_name, const char * key, String & value)
{
   bool success = false;
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   if (resourceList (resource_list) && property_name)
   {
      Mojo<PropertyInterface> property;
      // In the getResource call, the last parameter is optional.  A true value will cause the system
      // to error if the resource can not be retrieved by name and resource key.  An error in this case
      // generally means that the resource has not been registered with the resource manager.
      if (resource_list->getResource<PropertyInterface> (property_name, key, property, false))
      {
         property->getAbsoluteValue (value);
         success = true;
      }
   }
   return success;
}

// Return the current tournament mode.
bool GamePresentation::getTournamentMode (String & tournament_mode)
{
   return getPropertyString (system_tournament_mode_property_name, "Default", tournament_mode); // system property
}

// Return the current tournament status.
bool GamePresentation::getTournamentStatus (String & tournament_status)
{
   return getPropertyString (system_tournament_status_property_name, "Default", tournament_status); // system property
}

// Return the amount of player credits in the current tournament session.
int64 GamePresentation::getTournamentCredits (void)
{
   int64 tournament_credits = 0;
   getPropertyValueInt64 (system_tournament_credits_property_name, "Default", tournament_credits); // system property
   return tournament_credits;
}

// Return the amount of credits won in the current tournament session.
int64 GamePresentation::getTournamentWon (void)
{
   int64 tournament_won = 0;
   getPropertyValueInt64 (system_tournament_won_property_name, "Default", tournament_won); // system property

   return tournament_won;
}

// Purpose:  Called to enable all known control panels.
void GamePresentation::enableControlPanels (bool enable)
{
   verboseMethodName ();

   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enableControlPanels (" << (int) enable << ")" << endl;
   #endif

   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
      game_control_panel.getControlPanelInterface ()->enable (enable);
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
      game_count_control_panel.getControlPanelInterface ()->enable (enable);

   // Post a generic MultimediaEvent that will notify other ControlPanel actors to enable or disable.
   if (enable)
      postEvent ("ControlPanel:Enable");
   else
      postEvent ("ControlPanel:Disable");
}

// Purpose:  This method is called to mute/unmute the sounds configured for this game presentation.
void GamePresentation::enableSounds (bool enable)
{
   verboseMethodName ();

   Mojo<SoundSystemInterface> sound_system;
   getResource<SoundSystemInterface> (SOUND_SYSTEM, "Default", sound_system);
   // Enable/disable all sounds associated with a game presentation.
   sound_system->disableSoundGroup (SoundSystemInterface::VOCAL, !enable);
   sound_system->disableSoundGroup (SoundSystemInterface::GAME_SPECIAL, !enable);
   sound_system->disableSoundGroup (SoundSystemInterface::GAME_SOUNDS, !enable);
   sound_system->disableSoundGroup (SoundSystemInterface::GAME_PRESET_VOLUME_SOUNDS, !enable);
}

// Purpose: Return true if the game can start playing before game enrollment is complete.
bool GamePresentation::enrollmentBeforeGameStart (void) const
{
   verboseMethodName ();
   return enrollment_before_game_start && enrollmentRequired ();
}

// Purpose: Return true if game enrollment is required for this jursdiction.
bool GamePresentation::enrollmentRequired (void) const
{
   return enrollment_required;
}

// Purpose:  This method is used to guarantee a certain order of synchronization occurs when the context
// is entered.  First the presentation's synchronize is called, then the current state's synchronize.
bool GamePresentation::enterGameMode (State& state, const GameMode &game_mode, const ContextMode &context_mode)
{
   verboseMethodName ();

   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterGameMode ():" << " NewGmMode=" << game_mode << " OldGmMode=" << currentGameMode () << " Context=" << context_mode << endl;
   #endif

   // Initialize the return value.
   bool success = true;

   // Create an event to denote entering the current game mode.
   String event = "GamePresentation:enterGameMode:";
   String game_mode_name;
   assertion (getGameModeName (game_mode_name, game_mode));
   event << game_mode_name;
   postEvent (event);

   // Customize the presentation based on the game mode received.
   switch (game_mode)
   {
      case GAME_MODE_PLAY:
         if (context_mode == CONTEXT_MODE_HIBERNATE)
         {
            // If the game is powering up in hibernation mode, we need to fully synchronize
            // the game and then hibernate so that the game display can be guarnateed to
            // be in the correct state.
            if (currentGameMode () == GAME_MODE_UNKNOWN)
               enterModePlay ();
            // Send the game into hibernation.
            enterModeHibernate ();
         }
         else
            enterModePlay ();
         break;
      case GAME_MODE_HISTORY:
         enterModeHistory ();
         break;
      case GAME_MODE_UTILITY:
         enterModeUtility ();
         break;
      default:
         // Set the bet mode to the state which will not allow betting.
         if (bet.isValid ())
            bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_INVALID);
         break;
   }
   // Update the local mode members.
   setCurrentGameMode (game_mode);
   setCurrentContextMode (context_mode);
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterGameMode (), complete." << endl;
   #endif
   return success;
}

// Purpose: This method is called when the enterMode game_mode == HIBERNATE method is called.
bool GamePresentation::enterModeHibernate (void)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterModeHibernate ()" << endl;
   #endif
   // Set the bet mode to the correct game play betting mode.
   if (bet.isValid ())
      bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_INVALID);
   // Mute the sounds.
   muteSounds (true);
   // Disable the game play light pattern group.
   light_system->enableLightPatternGroup (false, LightPatternGroups::GAME_GROUP);
   // Disable the control panels supported by this game.
   enableControlPanels (false);
   return true;
}

// Purpose: This method is called when the enterMode game_mode == HISTORY method is called.
bool GamePresentation::enterModeHistory (void)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterModeHistory ()" << endl;
   #endif
   // Set the bet mode to the correct history betting mode.
   if (bet.isValid ())
      bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_INVALID);
   // Disable the sounds.
   enableSounds (false);
   // Disable the game play light pattern group.
   light_system->enableLightPatternGroup (false, LightPatternGroups::GAME_GROUP);
   // Disable the control panels supported by this game.
   enableControlPanels (false);
   return true;
}

// Purpose:  This method is called when the enterMode game_mode == NORMAL method is called.
bool GamePresentation::enterModePlay (void)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterModePlay ()" << endl;
   #endif
   // Set the bet mode to the correct game play betting mode.
   if (bet.isValid ())
      bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_GAME_PLAY);
   // Upon entering game play mode, make sure that the sounds are both enabled and audible.
   enableSounds (true);
   muteSounds (false);
   // Enable the game play light pattern group.
   light_system->enableLightPatternGroup (true, LightPatternGroups::GAME_GROUP);
   // Enable the control panels supported by this game.
   enableControlPanels (true);
   // Make sure the meters are displaying the correct information now that we know the flow state.
   synchronizeMeters ();
   // Synchronize the control panels supported by this game.
   synchronizeControlPanels ();

   return true;
}

// Purpose: This method is called when the enterMode game_mode == UTILITY method is called.
bool GamePresentation::enterModeUtility (void)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::enterModeUtility ()" << endl;
   #endif
   // Set the bet mode to the correct history betting mode.
   if (bet.isValid ())
      bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_TEST);
   // Disable the game presentation sounds.
   enableSounds (false);
   // Disable the game play light pattern group.
   light_system->enableLightPatternGroup (false, LightPatternGroups::GAME_GROUP);
   // Enable the control panels supported by this game.
   enableControlPanels (true);

   return true;
}

// Purpose: Returns the transaction id to use for games evaluations
uint32 GamePresentation::evaluationTransactionId (void) const
{
   // Use the act index for all evaluations from this game
   return act_index;
}

// Purpose: Find the evaluator to use for the given paytable section. Returns true on success.
bool GamePresentation::evaluatorForPaytableSection (Mojo<EvaluatorInterface> &evaluator, const char *section)
{
   // The evaluator will be used to determine the return value so we must release it before continuing.
   evaluator.release ();
   // If a valid section name has been received, attempt to find an evaluator for the section.
   if (section)
   {
      EvaluatorIterator iterator = evaluator_list.find (section);
      if (iterator != evaluator_list.end ())
         evaluator = iterator->second;
   }
   return evaluator.isValid ();
}

// Execute/parse the give bet script.
bool GamePresentation::executeBetScript (const String &script_file)
{
   bool result = false;

   if (bet.isValid ())
   {
      // Extract the script interface from the bet object.
      Mojo<ScriptInterface> bet_script_interface (bet);
      Mojo<ScriptResourceImporterInterface> script_parser;
      // Attempt to parse the script file.
      if (!createResource<ScriptResourceImporterInterface> (script_file, resource_list->resourceKey (), script_parser))
      {
         String error_message = "Cannot create script parser for ";
         error_message << script_file;
         System::error (LogicError (LogicError::INVALID_VALUE, error_message));
      }
      if (!script_parser->parseFile (bet_script_interface, resource_list->resourceKey (), false))
      {
         String error_message = "Cannot parse script: ";
         error_message << bet_script_interface->whoAreYou () << ", " << script_file;
         System::error (LogicError (LogicError::INVALID_VALUE, error_message));
      }
      result = true;
   }
   return result;
}


// Execute a function given its id.
bool GamePresentation::execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list)
{
   verboseMethodName ();
   bool result = true;

   switch (func_id)
   {
      case ACCELERATE_ROLLUP:
         accelerateRollup ();
         break;

      case ADD_STAGE_TO_CONTEXT:
         if (resource_list.isValid ())
         {
            Mojo <ActorStageInterface> stage;
            resource_list->getResource (parameter_list [0].asString (), resource_list->resourceKey (), stage);
            if (!stage->addToContext ())
            {
               String error_message ("GamePresentation: Couldn't add stage ");
               error_message << parameter_list [0].asString () << " to context = " << stage->getContext ();
               error_message << parameter_list [0].asString () << ", to context = " << stage->getContext ();
               System::error (LogicError (LogicError::INVALID_VALUE, error_message));
            }
         }
         break;

      case CONFIGURE_FROM_SCRIPT:
         configureFromScript (parameter_list [0].asString ());
         break;

      case DISPLAY_WIN:
         displayWin (parameter_list [0].asBool (), parameter_list [1].asBool ());
         break;

      case ENABLE_CONTROL_PANELS:
         enableControlPanels (parameter_list [0].asBool ());
         break;

      case POST_EVENT:
         postEvent (parameter_list [0].asString ());
         break;

      case POST_EVENT_ALL:
         postEventAll (parameter_list [0].asString ());
         break;

      case POST_EVENT_CONTEXT:
         postEventContext (parameter_list [0].asString (), parameter_list [1].asString ());
         break;

      case POST_ACT_OUTCOME_EVENTS:
         postActOutcomeEvents ();
         break;

      case POST_GAME_START_EVENTS:
         postGameStartEvents ();
         break;

      case REGISTER_CONTROL_PANEL:
         if (!registerControlPanel (parameter_list [0].asString (), parameter_list [1].asString ()))
         {
            String error_message ("REGISTER_CONTROL_PANEL: Unable to register control panel type: ");
            error_message << parameter_list [0].asString () << ", and alias: " << parameter_list [1].asString ();
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
         break;

      case REGISTER_STAGE:
         registerStage (parameter_list [0].asString ());
         break;

      case START_ROLLUP:
         startRollup ();
         break;

      case SET_CURRENT_GAME_SET_INDEX:
         setCurrentGameSetIndex (parameter_list [0].asUint32 ());
         break;

      case SET_BET_SCRIPT:
         bet_script = parameter_list [0].asString ();
         executeBetScript (bet_script);
         break;

      case SUPPORT_PAYTABLE_SECTION:
         #ifdef __VERBOSE_SCRIPT__
         debug << "GamePresentation::SUPPORT_PAYTABLE_SECTION:  " << parameter_list [0].asString () << endl;
         #endif
         supportPaytableSection (parameter_list [0].asString ());
         break;

      case SYNCHRONIZE_BACKGROUND_COLOR:
         synchronizeBackGroundColor ();
         break;

      case SYNCHRONIZE_METERS:
         synchronizeMeters ();
         break;

      case SYNCHRONIZE_BET_BUTTONS:
         synchronizeBetButtons ();
         break;

      case WARM_BOOT:
         // PROTOTYPE: warmBoot (void)          - always warm boots. Doesn't effect or check the last stored location id.
         // PROTOTYPE: warmBoot (bool order_id) - warm boots if the order_id is the next one that needs too. Stores it.
         // order_id       = The location warmBoot is being called from.
         // Note: Breakpoints will warm boot from 0 to N sequentially. They will not start at 0 again until
         // warmBootResetSequence is called.
         #ifdef __DEBUG__
            // If forcing warm boot OR need to warm boot at this breakpoint location...
            if (!parameter_list.getParameterCount () || updateWarmBootId (parameter_list [0].asUint32 ()))
            {
               // Store the location in the event log.
               String error_message ("GamePresentation warm booted at location = ", 128);
               if (parameter_list.getParameterCount ())
                  error_message << uint32 (parameter_list [0].asUint32 ());
               else
                  error_message << "Unspecified";

               System::log (LogError (error_message));

                // Allow telnet statements to finish.
               delay (1000);

               // Warm boot the AVP System.
               System::restart ();
            }
         #endif
         break;

      case WARM_BOOT_RESET_SEQUENCE:
         // PROTOTYPE: warmBootResetSequence (void) - Resets the sequence of warm boot locations.
         #ifdef __DEBUG__
         updateWarmBootId ();
         #endif
         break;
      case COMPARE_WIN_TO_BET_MULTIPLIER:
         if (parameter_list.getParameterCount () != 4)
         {
            System::error (LogicError (LogicError::INVALID_PARAMETER, " compareWinToBetMultiplier has invalid parameter count."));
         }
         else
         {
            compareWinToBetMultiplier (parameter_list [0].asString (), parameter_list [1].asFloat (), parameter_list [2].asString (), parameter_list [3].asString ());
         }
         break;

      case UPDATE_SYSTEM_DENOMINATION_EVENT:
         update_system_denomination_event = parameter_list [0].asString ();
         break;

      case UPDATE_GAME_MOVIE_DENOMINATION_EVENT:
         update_game_movie_denomination_event = parameter_list [0].asString ();
         break;

      case UPDATE_GAME_FONT_DENOMINATION_EVENT:
         update_game_font_denomination_event = parameter_list [0].asString ();
         break;

      case MINIMUM_BET_DENOMINATION_TEXT:
         mininum_bet_denomination_text = parameter_list [0].asString ();
         break;

      case UPDATE_MIN_BET_DENOMINATION_EVENT:
         update_min_bet_denomination_event = parameter_list [0].asString ();
         break;

      case HIDE_MIN_BET_DENOMINATION_EVENT:
         hide_min_bet_denomination_event = parameter_list [0].asString ();
         break;

      case BLANK_DENOMINATION_MOVIE_SECTION:
         blank_movie_section = parameter_list [0].asString ();
         break;

      case SYSTEM_DENOMINATION_PROPERTY_NAME:
         system_denomination_property_name = parameter_list [0].asString ();
         break;

      case GAME_DENOMINATION_PROPERTY_NAME:
         game_denomination_property_name = parameter_list [0].asString ();
         break;

      case GAME_DENOMINATION_TEXT_PROPERTY_NAME:
         game_denomination_text_property_name = parameter_list [0].asString ();
         break;

      case MINIMUM_BET_PROPERTY_NAME:
         minimum_bet_property_name = parameter_list [0].asString ();
         break;

      case DENOMINATION_MOVIE_ACTOR_NAME:
         denomination_movie_actor_name = parameter_list [0].asString ();
         break;

      case SET_NV_MODE:
         setNVMode (parameter_list [0].asBool ());
         break;

      case SET_AUTOPLAY_ALIAS_NAME:
         autoplay_alias_name = parameter_list [0].asString ();
         break;

      case SET_DOUBLE_UP_ALIAS_NAME:
         double_up_alias_name = parameter_list [0].asString ();
         break;

      case CHECK_IF_DOUBLE_UP_TRIGGERED:
         checkIfDoubleUpTriggered ();
         break;

      case ROLLUP_DOUBLE_UP_WIN:
         rollupDoubleUpWin ();
         break;

      case ENABLE_DOUBLE_UP_LISTENER:
         enableDoubleUpListener ();
         break;

      case ENABLE_LANGUAGE_BUTTON:
         enableLanguageButton (parameter_list [0].asBool ());
         break;

      case DISABLE_LANGUAGE_BUTTON_EVENT:
         disable_language_button_event = parameter_list [0].asString ();
         break;

      case GET_STAGE:
      {
         //This funtion id only useful for Lua
         //Returns the actor stage interface for the stage registered to the presentation
         //If no stage is registered and valid then nil is returned
         StageInterfaces main_stage;
         getStage (main_stage);
         if (main_stage.isValid ())
         {

            ActorStageInterface* stage_interface = main_stage.getActorStageInterface ();

            if (stage_interface)
             {
               ScriptParameter actor_stage_parameter (static_cast <void*> (stage_interface),
                                                      ParameterType::VOID_PTR);
               parameter_list.add (actor_stage_parameter);
            }
         }
      }
      break;

      case GET_EASY_BET_CONFIGURATION:
      {
         String easy_bet_configuration = getEasyBetConfiguration ();

         if (easy_bet_configuration && easy_bet_configuration.length () > 0)
         {
            // Return the easy bet mode string
            ScriptParameter result (easy_bet_configuration, ParameterType::STRING);
            parameter_list.add (result);
         }
         else
         {
            System::error (LogicError (LogicError::INVALID_PARAMETER, "getEasyBetConfiguration returned empty string."));
         }
      }
      break;

      case IS_AUTO_PLAY_ENABLED:
      {
         // return result to scriptable function
         ScriptParameter is_auto_play_enabled (isAutoPlayEnabled (), ParameterType::BOOL);
         parameter_list.add (is_auto_play_enabled);
      }
      break;

      case IS_DOUBLE_UP_ENABLED:
      {
         // return result to scriptable function
         ScriptParameter is_double_up_enabled (doubleUpEnabled (), ParameterType::BOOL);
         parameter_list.add (is_double_up_enabled);
      }
      break;

      case IS_LANGUAGE_ENABLED:
      {
         // return result to scriptable function
         ScriptParameter is_language_enabled (isMultiLingualAvailableForPlayer (), ParameterType::BOOL);
         parameter_list.add (is_language_enabled);
      }
      break;

      case ALLOW_AUTOPLAY_BUTTON:
      {
         allowAutoPlayButton (parameter_list [0].asBool ());
      }
      break;

      case ALLOW_LANGUAGE_BUTTON:
      {
         allowLanguageButton (parameter_list [0].asBool ());
      }
      break;

      case GET_BET:
      {
         Mojo<BetInterface> _bet;
         if (getBet (_bet))
         {
            ScriptParameter bet_parameter ((BetInterface *)_bet, ParameterType::VOID_PTR);
            parameter_list.add (bet_parameter);
         }
      }
      break;

      case GET_CONFIG_ITEM:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_CONFIG_ITEM:" << endl;
            #endif
            verifyParameterString (parameter_list, 0);

            ScriptParameter parameter;
            switch (parameter_list.getParameterCount ())
            {
               case 3:
                  {
                     verifyParameterString (parameter_list, 1);
                     verifyParameterBoolean (parameter_list, 2);
                     parameter = getConfigItem (parameter_list [0].asString (), parameter_list [1].asString (),
                                                parameter_list [2].asBool ());
                  }
                  break;

               case 2:
                  {
                     verifyParameterString (parameter_list, 1);
                     parameter = getConfigItem (parameter_list [0].asString (), parameter_list [1].asString ());
                  }
                  break;

               case 1:
                  {
                     parameter = getConfigItem (parameter_list [0].asString ());
                  }
                  break;
            }
            parameter_list.add (parameter);
         }
         break;

      case GET_THEME_CONFIGURATION_IDENTIFIER:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_THEME_CONFIGURATION_IDENTIFIER:" << endl;
            #endif
            ScriptParameter theme_configuraiton_id_param (getThemeConfigurationIdentifier (), ParameterType::STRING);
            parameter_list.add (theme_configuraiton_id_param);
         }
         break;

      case GET_STAGE_CONFIGURATION_IDENTIFIER:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_STAGE_CONFIGURATION_IDENTIFIER:" << endl;
            #endif
            ScriptParameter stage_configuraiton_id_param (getStageConfigurationIdentifier (), ParameterType::STRING);
            parameter_list.add (stage_configuraiton_id_param);
         }
         break;

      case GET_GAME_CONFIGURATION_IDENTIFIER:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_GAME_CONFIGURATION_IDENTIFIER:" << endl;
            #endif
            ScriptParameter game_configuraiton_id_param (getGameConfigurationIdentifier (), ParameterType::STRING);
            parameter_list.add (game_configuraiton_id_param);
         }
         break;

      case GET_MOUNT_POINT:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_MOUNT_POINT:" << endl;
            #endif
            ScriptParameter mount_point_param (resource_mount_point, ParameterType::STRING);
            parameter_list.add (mount_point_param);
         }
         break;

      case IS_FLOW_STATE_HISTORY:
         {
            ScriptParameter flow_state_history ((getFlowState () == GameStateValue::STATE_HISTORY), ParameterType::BOOL);
            parameter_list.add (flow_state_history);
         }
         break;

      case GET_NUMBER_OF_ENABLED_DENOMS:
         {
            uint32 enabled_denoms = numberOfAvailablePlayerDenominations ();
            ScriptParameter return_parameter(enabled_denoms, ParameterType::UINT32);
            parameter_list.add(return_parameter);
         }
         break;

      case CHANGE_DENOMINATION:
         changeDenomination(parameter_list[0].asUint32());
         break;

      case GET_DENOMINATION_BUTTON_DATA:
         {
            ScriptTableParameter table;
            getDenominationButtonData (table, parameter_list [0].asUint32 ());
            ScriptParameter table_param (table, ParameterType::TABLE);
            parameter_list.add (table_param);
         }
         break;

      case GET_DEFAULT_DENOMINATION:
         {
            uint32 default_denom = getDefaultDenomination ();
            ScriptParameter default_denom_param (default_denom, ParameterType::UINT32);
            parameter_list.add (default_denom_param);
         }
         break;

      case SET_SHOW_STAGE_ON_ENTER:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   SET_SHOW_STAGE_ON_ENTER:" << endl;
            #endif
            setShowStageOnEnter (parameter_list [0].asBool ());
         }
         break;

      case GET_PAYTABLE_PARSER:
         {
            if (paytable_parser.isValid ())
            {
               Mojo<ScriptInterface> scriptable_object (paytable_parser);

               // Only scriptable objects can be passed as parameters.
               // ParserWrapper is scriptable, but PaytableParser is not.
               if (scriptable_object.isValid ())
               {
                  ScriptParameter paytable_parser_parameter (&(*paytable_parser), ParameterType::VOID_PTR);
                  parameter_list.add (paytable_parser_parameter);
               }
            }
         }
         break;

      case GET_LOCALIZED_CURRENCY_STRING:
         {
            // Check that the paramters are valid
            if (verifyParameterNumeric (parameter_list, 0))
            {
               // Call the function
               String currency_string = getLocalizedCurrencyString (parameter_list [0].asUint32 ());

               // Return the value back to script
               ScriptParameter currency_string_parameter (currency_string, ParameterType::STRING);
               parameter_list.add (currency_string_parameter);
            }
         }
         break;

      case GET_PAYTABLE_PERCENTAGE:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_PAYTABLE_PERCENTAGE:" << endl;
            #endif
            ScriptParameter paytable_percentage (paytable_parser->getPayTablePercentage(), ParameterType::FLOAT);
            parameter_list.add (paytable_percentage);
         }
         break;

      case GET_RESPONSIBLE_GAMING_DATA:
         {
            // This table is only populated with values obtained directly from their source,
            //    no defaults are used when the value can't be determiend
            ScriptTableParameter table_param;

            Config config;
            ConfigID config_id = INVALID_CONFIGID;

            // Don't throw an error if this config item can't be found, simply omit the table entry
            config_id = config.getId ("DISPLAY GAME PAYBACK PERCENTAGES", false);
            if (config_id != INVALID_CONFIGID)
            {
               table_param.script_table ["operator_display_request"] = getConfigItem (config_id);
            }

            if (paytable_parser.isValid ())
            {
               // Attempt to extract the ResponsibleGamingInformationInterface from the paytable_parser
               Mojo<ResponsibleGamingInformationInterface> responsible_gaming_interface (paytable_parser);
               // If the paytable_parser supports the interface, build the Responsible Gaming lua table to return
               if (responsible_gaming_interface.isValid ())
               {
                  float min_baseline_percentage  = responsible_gaming_interface->getResponsibleGamingFirstCoinMinBetPercentage ();
                  float min_committal_percentage = responsible_gaming_interface->getResponsibleGamingCommittalMinBetPercentage ();
                  float max_percentage           = responsible_gaming_interface->getResponsibleGamingMaximumBetPercentage ();

                  // Pass all Responsible Gaming data back for the scripts to interpret directly
                  table_param.script_table ["min_baseline_percentage"] = ScriptParameter (min_baseline_percentage, ParameterType::FLOAT);
                  table_param.script_table ["min_committal_percentage"] = ScriptParameter (min_committal_percentage, ParameterType::FLOAT);
                  table_param.script_table ["max_percentage"] = ScriptParameter (max_percentage, ParameterType::FLOAT);
               }
            }

            // Only add the table to return if it got populated with at least 1 entry
            if (table_param.script_table.size ())
            {
               parameter_list.add (ScriptParameter (table_param, ParameterType::TABLE));
            }
         }
         break;

      case GET_NUMBER_OF_LICENSED_LANGUAGE:
         {
            uint32 number = 0;
            if (languageConfigsSupported () && allow_language_button)
            {
               // Holds the enabled licensed languages bitfield
               BitField language_bitfield;
               // Get licensed languages bit field
               config.getValueBitField (language_licensed_bitfield_config_id, language_bitfield);
               number = language_bitfield.numberOfBitsSet();
            }
            // Return the number of licensed language.
            ScriptParameter return_parameter (number, ParameterType::UINT32);
            parameter_list.add(return_parameter);
         }
         break;

      case GET_MACHINE_WIDE_WIN_CAP_LIMIT:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_MACHINE_WIDE_WIN_CAP_LIMIT:" << endl;
            #endif
            ConfigID id = config.getId ("MACHINE WIDE WIN CAP LIMIT", false);
            if (id != INVALID_CONFIGID)
            {
               Amount amount;
               config.getAmount (id, amount);
               if (amount.isNotZero ())
               {
                  String format_string;
                  CountryCode::Country country = locale.moneyNativeCountryCode ();
                  Amount::AmountValue value = Amount::Extractor (amount).extract (Denomination::BASE_UNIT);
                  locale.moneyFormatNumericWhole (country, value, format_string);
                  ScriptParameter machine_wide_win_cap_limit (format_string, ParameterType::STRING);
                  parameter_list.add (machine_wide_win_cap_limit);
               }
            }
         }
         break;

      case GET_MINIMUM_BET_DENOMINATION_TEXT:
         {
            ScriptParameter mininum_bet_denomination_parameter (mininum_bet_denomination_text, ParameterType::STRING);
            parameter_list.add (mininum_bet_denomination_parameter);
         }
         break;

      case IS_PRESENTATION_PLAYING:
         {
            // Call the function
            bool powerhit_synchronize = isPresentationPlaying ();

            // Return the value back to script
            ScriptParameter powerhit_synchronize_param (powerhit_synchronize, ParameterType::BOOL);
            parameter_list.add (powerhit_synchronize_param);
         }
         break;

      case GET_STATE_INFO_PAYLOAD:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_STATE_INFO_PAYLOAD:" << endl;
            #endif
            ScriptTableParameter payload;
            // Assemble the payload.
            createStateInfoPayload (payload);
            // return the payload to the caller
            ScriptParameter _rd (payload, ParameterType::TABLE);
            parameter_list.add (_rd);
         }
         break;

      case GET_EVALUATOR_TYPE:
         {
            String evaluator_type = getEvaluatorType ();

            parameter_list.add (ScriptParameter (evaluator_type.c_str (), ParameterType::STRING));
         }
         break;

      case IS_WIN_CATEGORY:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   IS_WIN_CATEGORY:" << endl;
            #endif
            bool match = false;
            vector<WinCategoryId> win_categories;
            ScriptTableParameter *category_table = parameter_list [0].asTable ();
            ScriptTableParameter::ScriptParameterMap::const_iterator iter = category_table->script_table.begin ();
            for (; iter != category_table->script_table.end (); ++iter)
            {
               win_categories.push_back (iter->second.asInt32 ());
            }

            if (parameter_list.getParameterCount () == 1)
            {
               match = matchWinCategory (win_categories);
            }
            else if (parameter_list.getParameterCount () == 2)
            {
               match = matchWinCategory (win_categories, parameter_list [1].asString ());
            }

            ScriptParameter match_param (match, ParameterType::BOOL);
            parameter_list.add (match_param);
         }
         break;

      case WIN_THIS_ACT:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   WIN_THIS_ACT:" << endl;
            #endif
            bool match = processConditionWinThisAct ();
            ScriptParameter match_param (match, ParameterType::BOOL);
            parameter_list.add (match_param);
         }
         break;

      case WIN_DOUBLEUP:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   WIN_DOUBLEUP:" << endl;
            #endif
            bool win = doubleUpWin ();
            ScriptParameter win_param (win, ParameterType::BOOL);
            parameter_list.add (win_param);
         }
         break;

      case IS_DOUBLEUP_PLAYED:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   IS_DOUBLEUP_PLAYED:" << endl;
            #endif
            bool played = doubleUpWin () || checkDoubleUpLoss ();
            ScriptParameter played_param (played, ParameterType::BOOL);
            parameter_list.add (played_param);
         }
         break;

      case CAN_DISPLAY_AWARD:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   CAN_DISPLAY_AWARD:" << endl;
            #endif
            bool display = canDisplayAward ();
            ScriptParameter display_param (display, ParameterType::BOOL);
            parameter_list.add (display_param);
         }
         break;

      case IS_PLAYER_SELECTABLE_DENOMINATION:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   IS_PLAYER_SELECTABLE_DENOMINATION:" << endl;
            #endif
            bool selectable = processConditionPlayerSelectableDenomination ();
            ScriptParameter selectable_param (selectable, ParameterType::BOOL);
            parameter_list.add (selectable_param);
         }
         break;

      case IS_PLAYER_SELECTABLE_GAME:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   IS_PLAYER_SELECTABLE_GAME:" << endl;
            #endif
            bool selectable = processConditionPlayerSelectableGame ();
            ScriptParameter selectable_param (selectable, ParameterType::BOOL);
            parameter_list.add (selectable_param);
         }
         break;

      case POST_GAME_SIDE_TILT:
         {
            postGameSideTilt (parameter_list [0].asString (), parameter_list [1].asString (), parameter_list [2].asBool ());
         }
         break;

      case END_PRESENTATION_STATE:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   END_PRESENTATION_STATE:" << endl;
            #endif

            // This function is designed to replace PresentationState::waitEvent functionality by directly ending a state
            if (parameter_list.getParameterCount () == 0)
            {
               // If no presentation state is specified, flow state is done.
               postStateComplete ();
            }
            else
            {
               // Request the next presentation state.
               requestPresentationSubstate (parameter_list[0].asString ());
            }
         }
         break;

      case RETURN_TO_PREVIOUS_PRESENTATION_STATE:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   RETURN_TO_PREVIOUS_PRESENTATION_STATE:" << endl;
            #endif

            requestReturnToPreviousPresentationSubstate ();
         }
         break;

      case GET_ER_DATA:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   GET_ER_DATA:" << endl;
            #endif
            ScriptTableParameter payload;
            getERData (payload);
            ScriptParameter return_parameter (payload, ParameterType::TABLE);
            parameter_list.add (return_parameter);
         }
         break;

      case WIN_ALL_ACTS:
         {
            #ifdef __VERBOSE_SCRIPT__
            debug << "   WIN_ALL_ACTS:" << endl;
            #endif
            bool match = winAllActs ();
            ScriptParameter match_param (match, ParameterType::BOOL);
            parameter_list.add (match_param);
         }
         break;

      case GET_IGT_CONNECT:
         {
            if (igt_connect.isValid ())
            {
               ScriptParameter igt_connect_parameter ((IGTConnectGameInterface *) igt_connect, ParameterType::VOID_PTR);
               parameter_list.add (igt_connect_parameter);
            }
            else
            {
               String error_string ("IGT Connect feature is invalid in getIGTConnect scriptable function invocation.");
               setErrorString (error_string);
            }
         }
         break;

      case GET_IGT_CONNECT_DISPLAY:
         {
            if (igt_connect_display.isValid ())
            {
               ScriptParameter igt_connect_display_parameter ((IGTConnectDisplayClientInterface *) igt_connect_display, ParameterType::VOID_PTR);
               parameter_list.add (igt_connect_display_parameter);
            }
            else
            {
               String error_string ("IGT Connect Display is invalid in getIGTConnectDisplay scriptable function invocation.");
               setErrorString (error_string);
            }
         }
         break;

      case GET_IGT_CONNECT_ACTION_REQUIRED:
         {
            ScriptParameter igt_connect_action_required_parameter (igt_connect_action_required, ParameterType::BOOL);
            parameter_list.add (igt_connect_action_required_parameter);
         }
         break;

      case PROCESS_IGT_CONNECT_NOTIFICATIONS:
         {
            if (igt_connect.isValid ())
            {
               igt_connect->processNotifications ();
            }
         }
         break;

      case RMLP_BONUS_TRIGGERED:
         {
            bool rmlp = rmlpTriggered();
            ScriptParameter rmlp_param (rmlp, ParameterType::BOOL);
            parameter_list.add (rmlp_param);
         }
         break;

      case IS_RMLP_FEATURE_ENABLED:
         {
            bool rmlp_enabled = rmlpEnabled();
            ScriptParameter rmlp_enabled_param (rmlp_enabled, ParameterType::BOOL);
            parameter_list.add (rmlp_enabled_param);
         }
         break;

      default:
         result = false;
         break;
   }

   return result;
}

/// <summary>Returns the IGT Connect client object</summary>
/// <param name="output">[out] The Mojo containing the IGTConnectGameInterface</param>
/// <returns>True if igt_connect is valid, false otherwise.</returns>
bool GamePresentation::getIGTConnect (Mojo <IGTConnectGameInterface> &output)
{
   bool return_value = false;
   if (igt_connect.isValid ())
   {
      return_value = true;
      output = igt_connect;
   }
   return return_value;
}

/// <summary>Returns the IGT Connect Display object</summary>
/// <param name="output">[out] The Mojo containing the IGTConnectDisplayClientInterface</param>
/// <returns>True if igt_connect_display is valid, false otherwise.</returns>
bool GamePresentation::getIGTConnectDisplay (Mojo <IGTConnectDisplayClientInterface> &output)
{
   bool return_value = false;
   if (igt_connect_display.isValid ())
   {
      return_value = true;
      output = igt_connect_display;
   }

   return return_value;
}

// Purpose: Returns the bit index of the default denomination, if that denomination
// is not enabled, the lowest enabled denomination is used instead
uint32 GamePresentation::getDefaultDenomination ()
{
   uint32 default_bit_index;
   BitField enabled_denoms;
   DenominationRules denom_rules;

   denom_rules.enabledPlayerDenominations (enabled_denoms);

   Config config;
   ConfigID id = config.getId (DEFAULT_PLAYER_DENOMINATION);
   uint32 default_denom = config.getValue (id);
   denom_rules.denominationBitIndex (default_denom, default_bit_index);

   //Check to make sure the default denomination is enabled
   if (enabled_denoms.getBit(default_bit_index) == false)
   {
      //It isn't, so set the default bit index to the lowest enabled denom
      enabled_denoms.lowestBitSet (default_bit_index);
   }

   return default_bit_index;
}

// Purpose: Builds a Lua table with all the information required for the script to
//          configure the actors
/*
   PLEASE NOTE:
   If you change this function and alter the data returned in the table PLEASE
   update the Lua script for the classic panel to reflect the new format
*/
void GamePresentation::getDenominationButtonData( ScriptTableParameter &table, const uint32 &max_buttons )
{
   uint32 active_denom_button_id = 0xFFFFFFFE;
   DenominationRules rules;
   BitField denominations_to_display;
   rules.enabledPlayerDenominations (denominations_to_display);

   uint32 enabled_denoms = (uint32) denominations_to_display.numberOfBitsSet ();

   // Denomination that is to be shown as selected
   Denomination selected_denom;
   uint32 selected_denomination_bit_index;

   // Establish what the currently selected denom is
   if (currentGameMode () == GAME_MODE_HISTORY)
   {
      Denomination denom(getEssentialRecord ()->denominationValueAtStageStart ());
      selected_denom = denom;
      rules.denominationBitIndex (denom, selected_denomination_bit_index);
      denominations_to_display.clear ();
      denominations_to_display.setBit (selected_denomination_bit_index, true);
   }
   else
   {
      rules.playerDenomination (selected_denom, selected_denomination_bit_index);
   }

   // Decide what to show for progressive
   ProgressiveShowMode progressive_show_mode = SHOW_NONE;
   String advertisement_string;

   if(display_advertisement_text)
   {
      if (getAdvertisementString (advertisement_string))
      {
         progressive_show_mode = SHOW_TEXT;
      }
   }
   else
   {
      progressive_show_mode = SHOW_METER;
   }

   uint32 denomination_bit_index = 0;
   for (uint32 button_index = 0; button_index < enabled_denoms &&
      (max_buttons == 0 || button_index < max_buttons); button_index++)
   {
      // Advance through any non-set bits
      while (denomination_bit_index < denominations_to_display.numberOfBits ()
         && !denominations_to_display.getBit (denomination_bit_index))
      {
         denomination_bit_index++;
      }

      // Make sure the index did not increment off the end of the bitfield
      if (denomination_bit_index < denominations_to_display.numberOfBits ())
      {
         ScriptTableParameter button_table;

         Denomination denomination;
         String denom_string;
         // Get the denomination and string
         rules.lookUpDenomination(denomination_bit_index, denomination);

         // Save text string for the button.
         formatDenomination (denomination, denom_string);
         button_table.script_table ["DenomText"] = ScriptParameter (denom_string, ParameterType::STRING);

         // Save the denomination bit index
         button_table.script_table ["DenomBitIndex"] = ScriptParameter (denomination_bit_index, ParameterType::UINT32);

         // Save if the current button is the active denom
         button_table.script_table ["IsActiveDenom"] =
            ScriptParameter ((selected_denomination_bit_index == denomination_bit_index),
            ParameterType::BOOL);

         if (selected_denomination_bit_index == denomination_bit_index)
         {
            active_denom_button_id = button_index;
         }

         button_table.script_table ["Progressive"] =
            buildDenomButtonProgressiveInformation (progressive_show_mode,
            denomination_bit_index, advertisement_string, denomination);

         String index_string = "";
         index_string << button_index;
         table.script_table [index_string.c_str ()] =
            ScriptParameter (button_table, ParameterType::TABLE);
      }

      denomination_bit_index++;
   }

   table.script_table ["ActiveDenomButtonIndex"] =
      ScriptParameter (active_denom_button_id, ParameterType::UINT32);
}

/// <summary>Changes the denomination of the game to the index of the selected button</summary>
/// <param name="new_denom_bit_index">[in] The bit index of the new denomination</param>
/// <remarks>An assumption is made in this function that it is only called if the
///    denomination specified in new_denom_bit_index is different than the
///    current denomination.</remarks>
void GamePresentation::changeDenomination (uint32 new_denom_bit_index)
{
	Denomination new_denom;
   DenominationRules rules;

	//Get the newly selected denom
   rules.lookUpDenomination (new_denom_bit_index, new_denom);

	//Now actually change the denom
	MainMenuRequestDenominationSequenceEvent sequence_event (new_denom.getValue ());
	postEvent (sequence_event);

   // Stop the win cycle
   displayWin (false, false);
}

// Purpose: Builds the progressive information for a denom button
// This function is based off a displayProgressiveInformation in MainMenuProgressiveDisplayValue.cpp in the AI
ScriptParameter GamePresentation::buildDenomButtonProgressiveInformation (
   ProgressiveShowMode progressive_show_mode, uint32 denomination_bit_index,
   String advertisement_string, Denomination denomination)
{
   ScriptTableParameter progressive_table;
   bool is_progressive_linked = false;

   if (progressive_show_mode == SHOW_TEXT)
   {
      ProgressiveGameId progressive_game_id (game_identifier, NO_STAGE);
      uint32 levels = progressive_manager.numberOfLevels (
         progressive_game_id, denomination);

      for (uint32 index = 0; index < levels; index++)
      {
         ProgressiveControllerId prog_ctrl_id;
         uint32 prog_ctrl_level  = 0;
         bool is_paytable_formed = false;
         uint32 list_position    = 0;
         Denomination controller_denom;

         uint32 prog_game_level = progressive_manager.gameLevel (progressive_game_id,
            index, denomination);

         if (progressive_manager.getGameLink (progressive_game_id, prog_game_level, prog_ctrl_id, prog_ctrl_level,
            is_paytable_formed, list_position, denomination, controller_denom))
         {
            is_progressive_linked = true;
            break;
         }
      }
   }


   // To display progressive text
   if (progressive_show_mode == SHOW_TEXT && is_progressive_linked)
   {
      progressive_table.script_table ["Mode"] =
         ScriptParameter (String ("text"), ParameterType::STRING);

      progressive_table.script_table ["Text"] =
         ScriptParameter (advertisement_string, ParameterType::STRING);
   }
   // To display progressive meter or prize string
   else if (progressive_show_mode == SHOW_METER)
   {
      progressive_table.script_table ["Mode"] =
         ScriptParameter (String ("meter"), ParameterType::STRING);

      // Get the current denomination
      Denomination current_denomination;
      DenominationRules rules;
      rules.lookUpDenomination (denomination_bit_index, current_denomination);

	   String progressive_prize_string;
	   Amount progressive_amount;

      // Show progressive meter, if necessary
      switch (determineProgressiveValues (progressive_prize_string, progressive_amount, current_denomination))
      {
         case PROGRESSIVE_AMOUNT:
            {
               progressive_table.script_table ["MeterType"] =
                  ScriptParameter (String ("Amount"), ParameterType::STRING);

               Amount::Extractor amount_extractor (progressive_amount);
               progressive_table.script_table ["Amount"] =
                  ScriptParameter (amount_extractor.extract (Denomination::BASE_UNIT),
                  ParameterType::INT64);
            }
            break;

         case PROGRESSIVE_STRING:
            {
               progressive_table.script_table ["MeterType"] =
                  ScriptParameter (String ("String"), ParameterType::STRING);
               progressive_table.script_table["PrizeString"] =
                  ScriptParameter (progressive_prize_string, ParameterType::STRING);
            }
            break;

         default:
            progressive_table.script_table ["Mode"] =
               ScriptParameter (String ("none"), ParameterType::STRING);
            break;
      }

   }
   else
   {
      progressive_table.script_table ["Mode"] =
         ScriptParameter (String ("none"), ParameterType::STRING);
   }

   return ScriptParameter (progressive_table, ParameterType::TABLE);
}

// Purpose: Retrieve the string that should be used for advertisement.
// Return whether the string should be used.  False if the string is "".
bool GamePresentation::getAdvertisementString (String &advertisement_string)
{
   Config advertisement_config;
   // If the string can not be retrieved, use nothing
   advertisement_config.getValueString (advertisement_config.getId (ADVERTISEMENT_CONFIG_ITEM), advertisement_string);

   if (advertisement_string == "NONE")
   {
      advertisement_string = "";
   }
   return advertisement_string != "";
}

// Purpose: Determines the progressive values for a given denomination
uint8 GamePresentation::determineProgressiveValues (String &progressive_prize_string,
	Amount &progressive_amount, Denomination denom)
{
	progressive_prize_string.clear ();
   progressive_amount.reset ();
   uint8 style_to_use = NO_PROGRESSIVE;
	ProgressiveGameId progressive_game_id (game_identifier, NO_STAGE);

	uint32 num_game_levels = progressive_manager.numberOfLevels (progressive_game_id,
		denom);

	for (uint32 index = 0; index < num_game_levels && style_to_use != PROGRESSIVE_STRING;
		index++)
	{
		uint32 prog_game_level = progressive_manager.gameLevel (progressive_game_id,
			index, denom);
		ProgressiveControllerId prog_ctrl_id;
		uint32 prog_ctrl_level  = 0;
		bool is_paytable_formed = false;
		uint32 list_position    = 0;

		Amount minimum_pay (
			progressive_manager.gameLevelMinimumPay (progressive_game_id, index),
			denom);
      Denomination controller_denom;
      Denomination game_denom = denom;

		if (progressive_manager.getGameLink (progressive_game_id, prog_game_level, prog_ctrl_id, prog_ctrl_level,
			is_paytable_formed, list_position, game_denom, controller_denom))
		{
			if (stricmp (prog_ctrl_id.getType (), "none"))
			{
				SafeStorageHandle safestorage_handle;

            // Get the prize string from progressive manager
            progressive_manager.levelPrizeString (prog_ctrl_id, prog_ctrl_level, safestorage_handle, Denomination::BLANK);
            CriticalData prize_string;
				char progressive_prize_string_buffer [PROGRESSIVE_MANAGER_MAX_STRING_LENGTH];

				if (safestorage_handle.isDefined ())
				{
					prize_string.attach (safestorage_handle);
					strncpy (progressive_prize_string_buffer, static_cast <const char *> (prize_string.getMemory ()), PROGRESSIVE_MANAGER_MAX_STRING_LENGTH);
					progressive_prize_string_buffer [PROGRESSIVE_MANAGER_MAX_STRING_LENGTH - 1] = '\0';
				}

				// Only worry about the prize string, if it is the first progressive game level
            if (prog_game_level == 0 && strlen (progressive_prize_string_buffer))
            {
               progressive_prize_string = progressive_prize_string_buffer;
               style_to_use = PROGRESSIVE_STRING;
            }

				// Only use the amount, when a prize string is not set
            if (!strlen (progressive_prize_string_buffer))
            {
               MeterAmount current_amount_meter;
               progressive_manager.levelMeter (prog_ctrl_id, prog_ctrl_level, safestorage_handle);
               if (safestorage_handle.isDefined ())
               {
                  current_amount_meter.attach (safestorage_handle);
                  Amount amount = current_amount_meter.getAmount ();
                  // If the meter amount is less than the game's minimum pay,
                  // use the minimum pay for the comparison.
                  if (amount < minimum_pay)
                  {
                     amount = minimum_pay;
                  }

                  // If the amount is larger, there is a new highest amount
                  if (amount > progressive_amount && amount.isNotZero ())
                  {
                     progressive_amount = amount;
                     style_to_use = PROGRESSIVE_AMOUNT;
                  }
               }
            }
			 }
		 }
	}

	return style_to_use;
}

// Purpose: Called when Game Manager exits our screen context.
bool GamePresentation::exitGameMode (State& state, const GameStageExitReason &reason)
{
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::exitGameMode() GameMode=" << current_game_mode << " Reason=" << static_cast<uint32>(reason) << endl;
   #endif
   // Create an event to denote exiting the current game mode.
   String event = "GamePresentation:exitGameMode:";
   String game_mode_name;
   assertion (getGameModeName (game_mode_name, currentGameMode ()));
   event << game_mode_name;

   // If this presentation was ever drawn...
   if (currentGameMode () != GAME_MODE_UNKNOWN)
   {
      // Disable the control panels supported by this game.
      enableControlPanels (false);
      // Set the bet mode to the state which will not allow betting.
      if (bet.isValid ())
         bet->setBetMode ((BetInterface::BetMode)BetInterface::BET_MODE_INVALID);
      switch (reason)
      {
         // If the stage is going off the screen...
         case STAGE_EXIT_REASON_UNKNOWN:
         case STAGE_EXIT_REASON_EXIT_GAME:
            {
               // Make sure that the control panels are hidden so no panel switches remain active after leaving this stage.
               showControlPanels (false);
               // True if not in history or utility game mode.
               bool not_history_or_utility = (currentGameMode () != GAME_MODE_HISTORY) && (currentGameMode () != GAME_MODE_UTILITY);
               // Provide the state an opportunity to prepare for exit of context if it exists.
               state_machine->exit (false /*leave state on the screen*/, not_history_or_utility /*suspend*/);
               // Exit the state-wide presentation.
               suspend ();
               // Hide the stage.
               showStage (false);
               // Disable/mute sounds.
               enableSounds (false);
               muteSounds (true);
               // Disable the game play light pattern group.
               light_system->enableLightPatternGroup (false, LightPatternGroups::GAME_GROUP);
               // Reset the modes. Marks presentation as off the screen.
               setCurrentGameMode (GAME_MODE_UNKNOWN);
               setCurrentContextMode (CONTEXT_MODE_INVALID);
               // Post an exit mode event.
               postEvent (event);
            }
            break;

         case STAGE_EXIT_REASON_NEW_STAGE:
            if (nv_mode)
            {
               // Make sure that the control panels are hidden so no panel switches remain active after leaving this stage.
               showControlPanels (false);
            }
            // Provide the state an opportunity to prepare for exit of context if it exists.
            state_machine->exit (false /*leave state on the screen*/, false /*suspend*/);
            // Exit the state-wide presentation.
            suspend ();
            if (nv_mode)
            {
               // Hide the stage.
               showStage (false);
            }
            else
            {
               postEventAll ("DelayHidingBaseStage");
            }
            // Reset the modes. Marks presentation as off the screen.
            setCurrentGameMode (GAME_MODE_UNKNOWN);
            setCurrentContextMode (CONTEXT_MODE_INVALID);
            // Post an exit mode event.
            postEvent (event);
            break;

         case STAGE_EXIT_REASON_TO_ON_SCREEN:
            // Provide the state an opportunity to prepare for exit of context if it exists.
            state_machine->exit (true /*leave state on the screen*/, false /*suspend*/);
            // Leave it running. Don't suspend the presentation.
            break;
      }
      // Remember the reason.
      last_exit_game_mode_reason = reason;
   }
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::exitGameMode (), complete. GameMode="  << current_game_mode << endl;
   #endif
   return true;
}

// Purpose:  Extract the total win from the award interface.  Returns true if successful.
bool GamePresentation::extractBonusWinFromAward (Amount &bonus_win_amount)
{
   verboseMethodName ();
   bool success = false;

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   if (local_award.isValid ())
   {
      // Everything after this act index is the bonus amount
      local_award->totalDisplayableAmount (bonus_win_amount, actIndex () + 1);
      // Since the award was processed, we want to return a positive result.  Set the return value.
      success = true;
   }
   return success;
}

// Purpose: Given a condition list, extract the matching hit win categories. Return true if win categories are found.
bool GamePresentation::extractHitWinCategoriesFromConditions (const vector<String> &conditions, vector<WinCategoryId> &result)
{
   verboseMethodName ();
   // Reset return value.
   result.clear ();
   // Get current award information.
   Mojo<AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   // Get a list of hit win categories.
   const Mojo<GamePresentation, ScriptInterface> game_presentation (this);
   vector<WinCategoryId> win_categories;
   GamePresentationHelper::winCategories (game_presentation, local_award, win_categories);
   // Remove duplicates.
   sort (win_categories.begin (), win_categories.end ());
   win_categories.erase (unique (win_categories.begin (), win_categories.end ()), win_categories.end ());
   // STORE A LIST OF HIT NON-DUPLICATE WIN CATEGORIES THAT ARE BEING WATCHED IN RESULT.
   bool win_category_condition_was_specified = false;
   // For every condition specified...
   for (uint32 index = 0; index < conditions.size (); ++index)
      // If condition includes win categories...
      if (strncmp (conditions [index], CONDITION_WIN_CATEGORY, strlen (CONDITION_WIN_CATEGORY)) == 0)
      {
         win_category_condition_was_specified = true;
         // Example string is "WIN_CATEGORY:1,2,3".
         String parameters (conditions [index].right (conditions [index].length () - strlen (CONDITION_WIN_CATEGORY)));
         // Separate the parameter string into a vector of parameters.
         vector<WinCategoryId> parameter_list;
         parseCommaSeparatedList (parameters, parameter_list);
         // If no parameters were specified...
         if (!parameter_list.size ())
            System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition NOT_WIN_CATEGORY does not specify any win categories."));
         // For every win category hit...
         for (uint32 j = 0; j < win_categories.size (); ++j)
            // For every category parameter...
            for (uint32 k = 0; k < parameter_list.size (); ++k)
               // If match...
               if (win_categories [j] == parameter_list [k])
                  // Add new category to the result.
                  result.push_back (win_categories [j]);
      }
   // Remove duplicates.
   sort (result.begin (), result.end ());
   result.erase (unique (result.begin (), result.end ()), result.end ());
   // If a win category list was not specified in the conditions list...
   if (win_category_condition_was_specified == false)
      // Default to all win categories hit.
      result = win_categories;
   // REMOVE ANY SPECIFIED EXCLUSION WIN CATEGORIES.
   // For every condition specified...
   for (uint32 index = 0; index < conditions.size (); ++index)
      // If condition excludes win categories...
      if (strncmp (conditions [index], CONDITION_NOT_WIN_CATEGORY, strlen (CONDITION_NOT_WIN_CATEGORY)) == 0)
      {
         // Example string is "NOT_WIN_CATEGORY:1,2,3".
         String parameters (conditions [index].right (conditions [index].length () - strlen (CONDITION_NOT_WIN_CATEGORY)));
         // Separate the parameter string into a vector of parameters.
         vector<WinCategoryId> parameter_list;
         parseCommaSeparatedList (parameters, parameter_list);
         // If no parameters were specified...
         if (!parameter_list.size ())
            System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition NOT_WIN_CATEGORY does not specify any win categories."));
         // For every parameter...
         for (uint32 j = 0; j < parameter_list.size (); ++j)
         {
            // For every category currently in the result...
            vector<WinCategoryId>::iterator iterator (result.begin ());
            while (iterator != result.end ())
            {
               // If parameter matches a result element...
               if (parameter_list [j] == *iterator)
                  // Remove match from list.
                  iterator = result.erase (iterator);
               // Advance to the next element in the list.
               else
                  ++iterator;
            }
         }
      }
   #ifdef __VERBOSE_CONDITIONS__
   debug << "extractHitWinCategoriesFromConditions result=";
   for (uint32 i = 0; i < result.size (); ++i)
      debug << result [i] << ',';
   debug << endl;
   #endif
   // Return result.
   return result.size ();
}

// Purpose:  Extract the mismatch win adjustment amount from the award interface. Returns true if successful.
bool GamePresentation::extractMismatchWinAdjustmentFromAward (Amount &total_mismatch_win)
{
   verboseMethodName ();
   bool success = false;

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   total_mismatch_win = ZERO_AMOUNT;

   if (enrollmentRequired () && local_award.isValid ())
   {
      // Iterate through all awards in list
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         Mojo <SystemAwardInterface> game_award;
         if (local_award->getAward (award_index, game_award))
         {
            // Iterate through all award items to find any win mismatch adjustments
            uint16 number_of_game_awards = game_award->getNumberOfAwards ();
            for (uint16 game_award_index = 0; game_award_index < number_of_game_awards; ++game_award_index)
            {
               // If the award item is a win mismatch adjustment, add it to our adjustment total
               String item_name = game_award->getName (game_award_index);
               if (item_name == "GM Mismatch Adjust")
               {
                  total_mismatch_win += game_award->getAmount (game_award_index);
                  success = true;
               }
            }
         }
      }
   }
   return success;
}

/// <summary> Extract the total win from the current act to the last act using an award interface.</summary>
/// <param name = "total_win_amount">[out] Total win amount for current to last act.</param>
/// <returns> success or failure.</returns>
bool GamePresentation::extractTotalWinFromAward (Amount &total_win_amount)
{
   verboseMethodName ();
   bool success = false;
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   if (local_award.isValid ())
   {
      local_award->totalDisplayableAmount (total_win_amount, actIndex ());
      // Since the award was processed, we want to return a positive result.  Set the return value.
      success = true;
   }
   return success;
}

// Purpose: Extract the total win multiplier from the award interface. The Bonus Manager may add a
//          multiplier to the total win in the form of an additional award item in the base game.
bool GamePresentation::extractBonusManagerMultiplierFromAward (uint32 &total_win_multiplier)
{
   verboseMethodName ();
   bool success = false;

   // This is a prelimiary implementation that should be reworked. The bonus manager
   // currently only supports multiplier type bonuses, but in the future could support
   // more.

   // Get the local award list from the essential record.
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   if (local_award.isValid ())
   {
      Amount total_win_amount (ZERO_AMOUNT);
      Amount award_adjustment (ZERO_AMOUNT);

      // Get the total win amount from the award interface
      local_award->totalDisplayableAmount (total_win_amount, actIndex ());

      // Iterate through all awards in list
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         Mojo <SystemAwardInterface> game_award;
         if (local_award->getAward (award_index, game_award))
         {
            // Iterate through all award items to find any bonus manager adjustments
            uint16 number_of_game_awards = game_award->getNumberOfAwards ();
            for (uint16 game_award_index = 0; game_award_index < number_of_game_awards; ++game_award_index)
            {
               // If the award item is a bonus manager adjustment, add it to our adjustment total
               String item_name = game_award->getName (game_award_index);
               if (item_name == "@ BM Award Adjust")
               {
                  award_adjustment += game_award->getAmount (game_award_index);
                  success = true;
               }
            }
         }
      }

      // Calculate the bonus manager multiplier
      if (success)
      {
         // Extract the AmountValues
         Amount::AmountValue total_win_amount_value = Amount::Extractor (total_win_amount).extract (Denomination::BASE_UNIT);
         Amount::AmountValue award_adjustment_value = Amount::Extractor (award_adjustment).extract (Denomination::BASE_UNIT);

         total_win_amount_value -= award_adjustment_value;

         if (total_win_amount_value > 0)
            total_win_multiplier = static_cast <uint32> ((award_adjustment_value / total_win_amount_value)) + 1;
      }
   }
   return success;
}

/// <summary> Extract the total win for the current act from the award interface.</summary>
/// <param name = "win_amount">[out] Total win amount for current act.</param>
/// <returns> success or failure.</returns>
bool GamePresentation::extractWinForThisAct (Amount &win_amount)
{
   verboseMethodName ();
   bool success = false;
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   if (local_award.isValid ())
   {
      // Everything after this act index is the bonus amount
      local_award->totalDisplayableAmount (win_amount, actIndex (), actIndex ());
      // Since the award was processed, we want to return a positive result.  Set the return value.
      success = true;
   }
   return success;
}

/// <summary> Extract the total win from all acts from the award interface.</summary>
/// <param name = "win_amount">[out] Total win amount for first to last act.</param>
/// <returns> success or failure.</returns>
bool GamePresentation::extractWinForAllActs (Amount &win_amount)
{
   verboseMethodName ();
   bool success = false;

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   if (local_award.isValid ())
   {
      local_award->totalDisplayableAmount (win_amount);

      success = true;
   }
   return success;
}

// Purpose:  This method is called by the leavingObjectScope method to finalize the configuration
// of the control panels registered by this game.
void GamePresentation::finalizeControlPanelConfiguration (void)
{
   verboseMethodName ();
   setControlPanelDenomination (denomination);
   // Configure the control panel with this class' sequence event poster.  This assignment ensures that
   // sequence events posted by the control panel are properly configured with game and stage information.
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
      game_control_panel.getControlPanelInterface ()->setSequenceEventPoster (sequence_event_poster);
   // Configure the control panel with this class' sequence event poster.  This assignment ensures that
   // sequence events posted by the control panel are properly configured with game and stage information.
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
      game_count_control_panel.getControlPanelInterface ()->setSequenceEventPoster (sequence_event_poster);
}

// Purpose: Find another stage of this game
//          Returns true if a stage is found, false if there are no other stages for this game
bool GamePresentation::findFirstStage (PresentationExtensionHandle& handle)
{
   verboseMethodName ();
   bool found = false;  // Did we find a stage?
   // Be sure we have a container in which to search
   if (presentation_extension_container.isValid ())
   {
      bool found_extension = false; // Did we find a presentation extension?
      Mojo <PresentationExtensionInterface> extension_interface;  // Interface for the extension

      // Try to find the first extension
      found_extension = presentation_extension_container->getFirstPresentationExtensionInterface (handle);
      // Continue as long as we have extensions but have not found a game stage
      while (found_extension && !found)
      {
         // Get the interface from the handle
         assertion (presentation_extension_container->getPresentationExtensionInterface (handle, extension_interface));
         Mojo <GamePresentationQueryInterface> game_interface (extension_interface);
         // If this is a game stage and it is from our game but not ourself...
         if (game_interface.isValid () &&
             game_interface->gameStage ().gameIdentifier () == gameIdentifier () &&  // Same game
             game_interface->gameStage ().stageIndex () != stageIndex ())  // Different stage
         {
            // This is another stage of our game
            found = true;  // We have found one.
         }
         else
         {
            // Try to get another one
            PresentationExtensionHandle next_handle = handle;
            found_extension = presentation_extension_container->getNextPresentationExtensionInterface (handle, next_handle);
            handle = next_handle;
         }
      }
   }
   return found;
}

// Purpose: Find another stage of this game
//          Returns true if a stage is found, false if there are no other stages for this game
bool GamePresentation::findNextStage (const PresentationExtensionHandle& current_handle, PresentationExtensionHandle& handle)
{
   verboseMethodName ();
   bool found = false;  // Did we find a stage?
   // Be sure we have a container in which to search
   if (presentation_extension_container.isValid ())
   {
      bool found_extension = false; // Did we find a presentation extension?
      Mojo <PresentationExtensionInterface> extension_interface;  // Interface for the extension

      // Try to find the first extension
      found_extension = presentation_extension_container->getNextPresentationExtensionInterface (current_handle, handle);
      // Continue as long as we have extensions but have not found a game stage
      while (found_extension && !found)
      {
         // Get the interface from the handle
         assertion (presentation_extension_container->getPresentationExtensionInterface (handle, extension_interface));
         Mojo <GamePresentationQueryInterface> game_interface (extension_interface);
         // If this is a game stage and it is from our game but not ourself...
         if (game_interface.isValid () &&
             game_interface->gameStage ().gameIdentifier () == gameIdentifier () &&  // Same game
             game_interface->gameStage ().stageIndex () != stageIndex ())  // Different stage
         {
            // This is another stage of our game
            found = true;  // We have found one.
         }
         else
         {
            // Try to get another one
            PresentationExtensionHandle next_handle = handle;
            found_extension = presentation_extension_container->getNextPresentationExtensionInterface (handle, next_handle);
            handle = next_handle;
         }
      }
   }
   return found;
}

// Purpose: Find another stage of this game
//          Returns true if a stage is found, false otherwise
bool GamePresentation::findStage (const GameStage& game_stage, PresentationExtensionHandle& handle)
{
   verboseMethodName ();
   bool found = false;  // Did we find a stage?
   // Be sure we have a container in which to search
   if (presentation_extension_container.isValid ())
   {
      bool found_extension = false; // Did we find a presentation extension?
      Mojo <PresentationExtensionInterface> extension_interface;  // Interface for the extension

      // Try to find the first extension
      found_extension = presentation_extension_container->getFirstPresentationExtensionInterface (handle);
      // Continue as long as we have extensions but have not found a game stage
      while (found_extension && !found)
      {
         // Get the interface from the handle
         assertion (presentation_extension_container->getPresentationExtensionInterface (handle, extension_interface));
         Mojo <GamePresentationQueryInterface> game_interface (extension_interface);
         // If this is a game stage and it is from our game but not ourself...
         if (game_interface.isValid () &&
             game_interface->gameStage () == game_stage)
         {
            // This is another stage of our game
            found = true;  // We have found one.
         }
         else
         {
            // Try to get another one
            PresentationExtensionHandle next_handle = handle;
            found_extension = presentation_extension_container->getNextPresentationExtensionInterface (handle, next_handle);
            handle = next_handle;
         }
      }
   }
   return found;
}

// Get the property by name and resource key.  Fill the property with the current denomination string.
void GamePresentation::formatDenomination (const Denomination &denomination, String &denomination_string)
{
   verboseMethodName ();
   #ifdef __VERBOSE_DENOM__
   debug << "GamePresentation::formatDenomination" << endl;
   #endif
   ULocale locale;
   CountryCode::Country country = locale.moneyNativeCountryCode ();
   Amount::AmountValue whole_value;

   locale.moneyGetUnitWholeValue (country, whole_value);
   #ifdef __VERBOSE_DENOM__
   debug << "GamePresentation::formatDenomination whole_value amount: " << whole_value << endl;
   #endif

   if (denomination.getValue () >= whole_value)
   {
      #ifdef __VERBOSE_DENOM__
      debug << "GamePresentation::formatDenomination, moneyFormatNumericWhole: " << endl;
      #endif
      locale.moneyFormatNumericWhole (country, denomination.getValue (), denomination_string, true /* omit base units if they're 0 */);
   }
   else
   {
      #ifdef __VERBOSE_DENOM__
      debug << "GamePresentation::formatDenomination, moneyFormatNumericBase: " << endl;
      #endif
      locale.moneyFormatNumericBase (country, denomination.getValue (), denomination_string);
   }

   #ifdef __VERBOSE_DENOM__
   debug << "GamePresentation::formatDenomination, formatted_amount: " << denomination_string << endl;
   #endif
}

// Purpose: Return the game identifier
const GameIdentifier &GamePresentation::gameIdentifier (void) const
{
   return game_identifier;
}

// Purpose: Updates the game set parameter received with this game's configured game set.
bool GamePresentation::gameSet (Mojo<GameSetInterface> &game_set, uint32 game_set_index)
{
   game_set.release ();
   // Get the current game set configuration (deteremined by paytable section) and retrieve
   // the specified game set by index from the game set configuration.
   Mojo<GameSetConfigurationInterface> game_set_configuration_interface;
   if (gameSetConfiguration (game_set_configuration_interface))
      game_set_configuration_interface->gameSet (game_set, game_set_index);
   // Return true if the game set parameter has been updated.
   return game_set.isValid ();
}

// Purpose: Updates the game set configuration parameter received with this game's current configuration.
bool GamePresentation::gameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface)
{
   bool success = false;
   // Determine the paytable section name.
   String section_name;
   if (currentPaytableSection (section_name))
   {
      // Retrieve the game set configuration manager for the current paytable section.
      success = gameSetConfiguration (game_set_configuration_interface, section_name);
   }
   return success;
}

// Purpose: Updates the game set configuration parameter received with the configuration for the paytable section name received.
bool GamePresentation::gameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface, String &section_name)
{
   game_set_configuration_interface.release ();
   // Retrieve the game set configuration manager for the current paytable section.
   GameSetMapIterator iterator = game_set_configuration_map.find (section_name);
   // Now retrieve the specified game set by index from the game set configuration.
   if (iterator != game_set_configuration_map.end ())
      game_set_configuration_interface = iterator->second;
   // Return true if the game set configuration parameter has been updated.
   return game_set_configuration_interface.isValid ();
}

// Returns a constant reference to this game's game set configuration map.
const GamePresentation::GameSetMap &GamePresentation::gameSetConfigurationMap (void)
{
   return game_set_configuration_map;
}

// From GameFlowDebugInterface.  Ask the game for game specific state informatin string.
// This information is copied in the provided "state_information" buffer which has
// been allocated to "size" bytes.
// Returns true on success, false otherwise.
bool GamePresentation::gameSpecificStateInformation (char *state_information, size_t size)
{
   verboseMethodName ();
   // Cache the current state information.
   GameStateValue flow_state = getFlowState ();
   GameStateValue previous_flow_state = getPreviousFlowState ();
   StateId presentation_substate = getPresentationSubstate ();
   StateId previous_presentation_substate = getPreviousPresentationSubstate ();
   // Game and context mode.
   uint32 context_mode = currentContextMode ();
   uint32 game_mode = currentGameMode ();

   const char *flow_state_name = flow_state.stateName ();
   if (!flow_state_name)
      flow_state_name = "UNKNOWN";
   const char *previous_flow_state_name = previous_flow_state.stateName ();
   if (!previous_flow_state_name)
      previous_flow_state_name = "UNKNOWN";

   // Build the string using the caller's buffer.  Respect the caller's prescribed buffer size.
   SNPRINTF (state_information, size, "state:%s(%lu:%lu) previous:%s(%lu:%lu) game_mode:%lu context_mode:%lu",
             flow_state_name, static_cast<uint32>(flow_state), presentation_substate,
             previous_flow_state_name, static_cast<uint32>(previous_flow_state), previous_presentation_substate,
             game_mode, context_mode);
   return true;
}

// Purpose: Return true if a game was unenrolled since the last play.
bool GamePresentation::gameWasUnenrolled (void)
{
   verboseMethodName ();
   const EssentialRecord *essential_record = getEssentialRecord ();
   return (essential_record && essential_record->gameEnrollmentAborted () && enrollmentRequired ());
}

// Purpose:  Return the number of times this game still needs to be played.
uint16 GamePresentation::gamesRemaining (void)
{
   uint16 play_count, total_plays;
   assertion (playCountStatus (play_count, total_plays));
   // The total plays is whatever we knew about at the start of the stage (the essential record value)
   // plus whatever we have won (in the case when we won ourselves and are going to replay).
   uint16 games_remaining = (total_plays > play_count) ? (total_plays - play_count) : 0;
   #ifdef __VERBOSE__
   debug << "GamePresentation::gamesRemaining: " << games_remaining << endl;
   #endif
   return games_remaining;
}

// Purpose: Ask the game for its current play count information
//          Returns true on success
bool GamePresentation::playCountStatus (uint16& play_count, uint16& total_plays)
{
   // this can be used in the base game as well.  It it must retrun thet values
   // for the corrent stage type.
   if (gameStage ().stageIndex () == BASE_GAME_STAGE)
   {
      play_count = playCount (ACT_TYPE_BASE);
   }
   else
   {
      play_count = playCount (ACT_TYPE_BONUS);
   }

   total_plays = totalPlays ();
   return true;
}

// Purpose: Extract the value from an Amount object
int64 GamePresentation::getAmountValue (const Amount &amount)
{
   Amount::Extractor amount_extractor (amount);
   Amount::AmountValue new_value = amount_extractor.extract (Denomination::BASE_UNIT);

   return new_value;
}

// Get the current award interface.  Use slotAwardInterface to retrieve
// the current slot award information stored in critical data.
bool GamePresentation::getCurrentAward (Mojo<AwardInterface> &award)
{
   readAwardFromEssentialRecord (award);
   // Return true if the mojo points to valid data.
   return award.isValid ();
}

// Purpose: Get the essential record data (don't cache the pointer).
const EssentialRecord *GamePresentation::getEssentialRecord (void)
{
   return essential_record_ptr;
}

// Purpose: Get the shared essential record data (don't cache the pointer).
const SharedEssentialRecord *GamePresentation::getSharedEssentialRecord (void)
{
   return shared_essential_record_ptr;
}

// Purpose: Get the value of the double tap flag. Set to true when a CDS game has hit the spin button twice.
bool GamePresentation::getEnrollmentButtonDoubleTap (void) const
{
   return enrollment_button_double_tap;
}

// Purpose: Get the current flow state id.
StateId GamePresentation::getFlowState (void) const
{
   return current_state.flowState ();
}

// Purpose: Return true if the central determination RNG seeds have arrived for this game.
bool GamePresentation::getGameEnrollmentSeedsArrived (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   return (essential_record && essential_record->gameEnrollmentSeedsArrived ());
}

// Purpose: Returns the name of the specified game mode.
bool GamePresentation::getGameModeName (String &game_mode_name, const GameMode game_mode)
{
   bool success = true;

   // Get the specified game mode.
   switch (game_mode)
   {
      case GAME_MODE_UNKNOWN:
         game_mode_name = "Unknown";
         break;

      case GAME_MODE_PLAY:
         game_mode_name = "Play";
         break;

      case GAME_MODE_HISTORY:
         game_mode_name = "History";
         break;

      case GAME_MODE_UTILITY:
         game_mode_name = "Utility";
         break;

      case GAME_MODE_DEMO:
         game_mode_name = "Demo";
         break;

      default:
         success = false;
         break;
   }

   return success;
}

// Return the current game speed config value.  Each game will handle this differently
// so only return the value.
int32 GamePresentation::getGameSpeed (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::getGameSpeed ()" << endl;
   #endif

   // Define the default value.
   uint32 game_speed = 0;

   ConfigID config_id = config.getId (CONFIG_ID_GAME_SPEED, game_configuration_identifier, false);

   // If this game doesn't support the game speed then return the default value.
   if (config_id != INVALID_CONFIGID)
      game_speed = config.getValue (config_id);

   return game_speed;
}

///<summary>Converts credits to a currency String</summary>
/// <param name = "credits"> [in] credit value to be converted to currency </param>
///<returns>Returns the currency value as a string</returns>
String GamePresentation::getLocalizedCurrencyString (uint32 credits)
{
   String credits_to_currency_string;
   Amount::AmountValue credit_value = credits * denomination.getValue ();
   // Used to format amount display strings.
   ULocale locale;
   Amount::AmountValue whole_value;

   // Format credit to currency
   CountryCode::Country country = locale.moneyNativeCountryCode ();
   locale.moneyGetUnitWholeValue (country, whole_value);

   if (credit_value >= whole_value)
   {
      locale.moneyFormatNumericWhole (country, credit_value, credits_to_currency_string, false /* omit base units if they're 0 */);
   }
   else
   {
      locale.moneyFormatNumericBase (country, credit_value, credits_to_currency_string);
   }

   return (credits_to_currency_string);
}

// Returns the maximum avialable game speed config value.
int32 GamePresentation::getMaxGameSpeed (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::getMaxGameSpeed ()" << endl;
   #endif

   // Define the default value.
   uint32 max_game_speed = 0;

   ConfigID config_id = config.getId (CONFIG_ID_GAME_SPEED, game_configuration_identifier, false);

   // If this game doesn't support the game speed then return the default value.
   if (config_id != INVALID_CONFIGID)
      max_game_speed = config.getMaxValue (config_id);

   return max_game_speed;
}

// Returns the minimum avialable game speed config value.
int32 GamePresentation::getMinGameSpeed (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::getMinGameSpeed ()" << endl;
   #endif

   // Define the default value.
   uint32 min_game_speed = 0;

   ConfigID config_id = config.getId (CONFIG_ID_GAME_SPEED, game_configuration_identifier, false);

   // If this game doesn't support the game speed then return the default value.
   if (config_id != INVALID_CONFIGID)
      min_game_speed = config.getMinValue (config_id);

   return min_game_speed;
}

/// <summary> Get the current resource_mount_point.</summary>
/// <returns> Return the current resource_mount_point.</returns>
const char * GamePresentation::getMountPoint (void) const
{
   return resource_mount_point;
}

// Purpose: Returns the last reason given for exiting game mode on this presentation.
const GameStageExitReason &GamePresentation::getLastExitGameModeReason (void) const
{
   return last_exit_game_mode_reason;
}

// Purpose: Get the current presentation substate
StateId GamePresentation::getPresentationSubstate (void) const
{
   return current_state.presentationSubstate ();
}

// Purpose: Get the previous flow state id
StateId GamePresentation::getPreviousFlowState (void) const
{
   return previous_state.flowState ();
}

// Purpose: Get the previous presentation substate
StateId GamePresentation::getPreviousPresentationSubstate (void) const
{
   return previous_state.presentationSubstate ();
}

// Purpose: Returns the previous state pair.
StatePair GamePresentation::getPreviousState (void) const
{
   return previous_state;
}

// The following method updates the stage parameter received with the registered stage.  If the stage is valid
// this method will return true.
bool GamePresentation::getStage (StageInterfaces &stage, bool error_on_failure) const
{
   // Verify that a valid stage name has been configured.
   if (error_on_failure && stage_name.length () == 0)
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::getStage: The main stage has not been configured."));

   // Extract the stage by registered alias name and initialize the return stage.
   Mojo<ActorStageInterface> actor_stage_interface;
   if (getStage (stage_name, actor_stage_interface, error_on_failure))
      stage.initialize (actor_stage_interface);
   // Return true if the stage configured is valid.
   return (stage.isValid ());
}

// The following method updates the stage parameter received with the registered stage.  If the stage is valid
// this method will return true.
bool GamePresentation::getStage (const char *alias, Mojo<ActorStageInterface> &actor_stage_interface, bool error_on_failure) const
{
   actor_stage_interface.release ();
   // Initialize the return value.
   bool success = false;
   // Retrieve the stage by alias from the resource list.
   if (resource_list.isValid ())
      success = resource_list->getResource (alias, resource_list->resourceKey (), actor_stage_interface, error_on_failure);
   return success;
}

// Purpose: Get the alias name of the main stage.
const char *GamePresentation::getStageName (void) const
{
   return stage_name;
}

// Purpose: Returns the current state pair.
StatePair GamePresentation::getState (void) const
{
   return current_state;
}

// Purpose:
bool GamePresentation::handleMessage (cid_t client_coid, const MessageHeader& message, Responder &responder)
{
   verboseMethodName();

   bool handled = false;
   switch (message.category)
   {
      case MessageHeader::CATEGORY_GAME_PRESENTATION:
      {
         #ifdef __VERBOSE_HANDLE_MESSAGE__
         debug << "GamePresentation:" << __FUNCTION__ << "  Start" << endl;
         #endif

         // This unassuming "synchron" helper object loads/unserializes the main object from/to critical data
         // (when necessary) based on a construction/destruction counting scheme.  Syncron's are designed to be placed on the stack.
         SerializerHelper::CriticalDataSynchron synchron (this, essential_record.isAttached () /* load_from_critical_data */);

         handled = true;
         switch (message.method)
         {
            // Request to change the bet - THIS METHOD IS DEPRECATED AND DOES NOTHING
            case GamePresentationMessage::BET_REQUEST:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::BET_REQUEST: " << endl;
                  #endif

                  GamePresentationBetRequest::Result result = true;
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Ask if the game can start
            case GamePresentationMessage::CAN_START:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::CAN_START: " << endl;
                  #endif

                  GamePresentationCanStart::Result result = canStart ();
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Change context mode
            case GamePresentationMessage::CHANGE_CONTEXT_MODE:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::CHANGE_CONTEXT_MODE: " << endl;
                  #endif

                  #ifdef __VERBOSE_MODE__
                  debug << game_index << ":" << stage_index << "GamePresentationMessage::CHANGE_CONTEXT_MODE:" << endl;
                  #endif
                  const GamePresentationChangeContextMode& mode_message = static_cast<const GamePresentationChangeContextMode &>(message);
                  State state (mode_message.stateHandle ());
                  GamePresentationChangeContextMode::Result result = changeContextMode (state, mode_message.contextMode ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Enter a game mode.
            case GamePresentationMessage::ENTER_MODE:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::ENTER_MODE: " << endl;
                  #endif

                  #ifdef __VERBOSE_MODE__
                  debug << game_index << ":" << stage_index << '|' << "GamePresentationMessage::ENTER_MODE:" << endl;
                  #endif
                  const GamePresentationEnterMode& mode_message = static_cast<const GamePresentationEnterMode &>(message);
                  State state (mode_message.stateHandle ());
                  GamePresentationEnterMode::Result result = enterGameMode (state, mode_message.gameMode (), mode_message.contextMode ());
                  // Flag to sync the condition of the presentation to the new game mode.
                  adjust_presentation_to_new_mode = true;
                  // If the game manager is transitioning through presentation states in several state machines in the game without interruption, don't sync the next state.
                  if (mode_message.chainingStateMachines ()
                     || (mode_message.gameMode () == GAME_MODE_HISTORY)    // History always enters.
                     || (mode_message.gameMode () == GAME_MODE_UTILITY))   // Utility always enters.
                  {
                     need_to_synchronize_presentation_state = false;
                     #ifdef __VERBOSE_STATE__
                     debug << "Enter game mode set \"need to sync\" flag false." << endl;
                     #endif
                  }
                  else
                  {
                     #ifdef __VERBOSE_STATE__
                     debug << "  Enter game mode: chaining state machines=false" << endl;
                     #endif
                  }
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Exit game mode.
            case GamePresentationMessage::EXIT_MODE:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::EXIT_MODE: " << endl;
                  #endif

                  #ifdef __VERBOSE_MODE__
                  debug << game_index << ":" << stage_index << '|' << "GamePresentationMessage::EXIT_MODE:" << endl;
                  #endif
                  const GamePresentationExitMode& mode_message = static_cast<const GamePresentationExitMode &>(message);
                  State state (mode_message.stateHandle ());
                  GamePresentationExitMode::Result result = exitGameMode (state, mode_message.exitReason ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::GAME_ENROLLMENT_SEEDS_ARRIVED:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::GAME_ENROLLMENT_SEEDS_ARRIVED: " << endl;
                  #endif

                  // Inform the game about receiving central determination RNG seeds.
                  GamePresentationGameEnrollmentSeedsArrived::Result result = setGameEnrollmentSeedsArrived ();
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::GAME_SPECIFIC_STATE_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::GAME_SPECIFIC_STATE_INFORMATION: " << endl;
                  #endif

                  // Retreive information concerning the current state of this game presentation.
                  GamePresentationGameSpecificStateInformation::Result result;
                  result.result = gameSpecificStateInformation (result.description, MAXIMUM_GAME_SPECIFIC_STATE_INFORMATION_SIZE);
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Init game.
            case GamePresentationMessage::INIT:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::INIT: " << endl;
                  #endif

                  #ifdef __CHECK_MEM__
                     #ifdef __TRACK_SPECIFIC_SIZE__
                        enableNewDeleteTracking (true, MINIMUM_ALLOCATION_SIZE, MAXIMUM_ALLOCATION_SIZE);
                     #endif
                  #endif
                  #ifdef __VERBOSE_STATE__
                  debug << game_index << ":" << stage_index << "GamePresentationMessage::INIT:" << endl;
                  #endif
                  const GamePresentationInit& init_message = static_cast<const GamePresentationInit &>(message);
                  State state (init_message.stateHandle ());
                  GamePresentationInit::Result result = init (state, init_message.coldStartFlag ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::PREPARE_FOR_UNLOADING:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::PREPARE_FOR_UNLOADING: " << endl;
                  #endif

                  // Have the game unload its resources
                  GamePresentationPrepareForUnloading::Result result = prepareForUnloading ();
                  // List the available resources
                  listAvailableResources ();
                  #ifdef __CHECK_MEM__
                     // Write out the unreferenced memory
                     outputUnfreedMemoryBlocks ();
                  #endif
                  // Done
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::PROCESS_GAME_REQUEST_EVENT:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::PROCESS_GAME_REQUEST_EVENT: " << endl;
                  #endif

                  const GamePresentationProcessGameRequestEvent& event_message = static_cast<const GamePresentationProcessGameRequestEvent &>(message);

                  GameRequestEvent event;
                  event_message.getEvent (event);

                  GamePresentationProcessGameRequestEvent::Result result = processGameRequestEvent (event);
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SEQUENCE_EVENT:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SEQUENCE_EVENT: " << endl;
                  #endif

                  const GamePresentationSequenceEvent& event_message = static_cast<const GamePresentationSequenceEvent &>(message);
                  GamePresentationSequenceEvent::Result result = sequence_event_listener_controller->processEvent (event_message.getEvent ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Request to set the award information
            case GamePresentationMessage::SET_AWARD_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_AWARD_INFORMATION: " << endl;
                  #endif

                  #ifdef __VERBOSE_AWARD__
                  debug << game_index << ":" << stage_index << "|GamePresentationMessage::SET_AWARD_INFORMATION" << endl;
                  #endif
                  const GamePresentationSetAwardInformation& award_message = static_cast<const GamePresentationSetAwardInformation &>(message);
                  GamePresentationSetAwardInformation::Result result =
                     setAwardInformation (award_message.sharedObjectFilename (), award_message.objectFilename ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Request to set the bet information
            case GamePresentationMessage::SET_BET_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_BET_INFORMATION: " << endl;
                  #endif

                  // IPC message to tell the game flow where to find its configuration file
                  const GamePresentationSetBetInformation& bet_message = static_cast<const GamePresentationSetBetInformation &>(message);
                  GamePresentationSetBetInformation::Result result =
                     setBetInformation (bet_message.sharedObjectFilename (), bet_message.objectFilename ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_CONFIGURATION_FILE:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_CONFIGURATION_FILE: " << endl;
                  #endif

                  // IPC message to tell the game flow where to find its configuration file
                  const GamePresentationSetConfigurationFile& config_message = static_cast<const GamePresentationSetConfigurationFile &>(message);
                  GamePresentationSetConfigurationFile::Result result = setConfigurationFile (config_message.fileName ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_CONFIGURATION_IDENTIFIER:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_CONFIGURATION_IDENTIFIER: " << endl;
                  #endif

                  // IPC message to tell the game the unique configuration identifier
                  const GamePresentationSetConfigurationIdentifier& config_message = static_cast<const GamePresentationSetConfigurationIdentifier &>(message);
                  GamePresentationSetConfigurationIdentifier::Result result = setConfigurationIdentifier (config_message.gameConfigurationIdentifier (),
                                                                                 config_message.themeConfigurationIdentifier (),
                                                                                 config_message.stageConfigurationIdentifier ());

                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Set the current paytable section to use
            case GamePresentationMessage::SET_CURRENT_PAYTABLE_SECTION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_CURRENT_PAYTABLE_SECTION: " << endl;
                  #endif

                  const GamePresentationSetCurrentPaytableSection& paytable_message = static_cast<const GamePresentationSetCurrentPaytableSection &>(message);
                  GamePresentationSetCurrentPaytableSection::Result result = setCurrentPaytableSection (paytable_message.paytableSection ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Set the denomination
            case GamePresentationMessage::SET_DENOMINATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_DENOMINATION: " << endl;
                  #endif

                  const GamePresentationSetDenomination& denom_message = static_cast<const GamePresentationSetDenomination &>(message);
                  GamePresentationSetDenomination::Result result = setDenomination (denom_message.denomination ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Request to set the essential record information
            case GamePresentationMessage::SET_ESSENTIAL_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_ESSENTIAL_INFORMATION: " << endl;
                  #endif

                  #ifdef __VERBOSE_AWARD__
                  debug << game_index << ":" << stage_index << "|GamePresentationMessage::SET_ESSENTIAL_INFORMATION" << endl;
                  #endif
                  const GamePresentationSetEssentialInformation& essential_message = static_cast<const GamePresentationSetEssentialInformation &>(message);
                  GamePresentationSetEssentialInformation::Result result =
                     setEssentialRecordInformation (essential_message.essentialRecordDirectory (),
                        essential_message.sharedEssentialRecordDirectory (), essential_message.essentialRecordName (),
                        essential_message.betName (), essential_message.awardName ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Request to set the evalator information
            case GamePresentationMessage::SET_EVAL_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_EVAL_INFORMATION: " << endl;
                  #endif

                  // IPC message to tell the game flow evaluation information, evaluator file name and interface name.
                  const GamePresentationSetEvalInformation& eval_message = static_cast<const GamePresentationSetEvalInformation &>(message);
                  GamePresentationSetEvalInformation::Result result =
                     setEvalInformation (eval_message.sharedObjectFilename (), eval_message.objectFilename ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_GAME_ENROLLMENT_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_GAME_ENROLLMENT_INFORMATION: " << endl;
                  #endif

                  // IPC message to tell the game what enrollment style to use.
                  const GamePresentationSetGameEnrollmentInformation& enrollment_message = static_cast<const GamePresentationSetGameEnrollmentInformation &> (message);
                  GamePresentationSetGameEnrollmentInformation::Result result = setGameEnrollmentInformation (enrollment_message.enrollmentRequired (), enrollment_message.enrollmentBeforeGameStart ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_GAME_INDEX:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_GAME_INDEX: " << endl;
                  #endif



                  // IPC message to tell the game its game index and stage index
                  const GamePresentationSetGameIndex& config_message = static_cast<const GamePresentationSetGameIndex &> (message);
                  GamePresentationSetGameIndex::Result result = setGameIndex (config_message.gameIndex (), config_message.stageIndex (), config_message.actIndex (), config_message.gameIdentifier ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_INITIATING_INFORMATION:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_INITIATING_INFORMATION: " << endl;
                  #endif

                  const GamePresentationSetInitiatingInformation& initiating_message = static_cast<const GamePresentationSetInitiatingInformation &>(message);
                  GamePresentationSetCurrentPaytableSection::Result result = setInitiatingInformation (initiating_message.stageIndex (),
                                                                                                       initiating_message.actIndex ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_MOUNT_POINT:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_MOUNT_POINT: " << endl;
                  #endif

                  // IPC message to tell the game flow where its mount point is
                  const GamePresentationSetMountPoint& config_message = static_cast<const GamePresentationSetMountPoint &>(message);
                  GamePresentationSetMountPoint::Result result = setMountPoint (config_message.mountPoint ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Request to set the award information
            case GamePresentationMessage::SET_PAY_TABLE_FILE_NAME:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_PAY_TABLE_FILE_NAME: " << endl;
                  #endif

                  // IPC message to tell the game the paytable file name
                  const GamePresentationSetPayTableFileName& name_message = static_cast<const GamePresentationSetPayTableFileName &>(message);
                  GamePresentationSetPayTableFileName::Result result =
                     setPaytableFilename (name_message.payTableFileName ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            case GamePresentationMessage::SET_SAFE_STORE_DIRECTORY:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_SAFE_STORE_DIRECTORY: " << endl;
                  #endif

                  const GamePresentationSetSafeStoreDirectory& set_message = static_cast<const GamePresentationSetSafeStoreDirectory &>(message);
                  GamePresentationSetSafeStoreDirectory::Result result = setSafeStoreDirectory (set_message.safeStoreDirectory ());
                  responder.reply (client_coid, &result, sizeof (result));
               }
               break;

            // Set the system meter values the game should be showing.
            case GamePresentationMessage::SET_SYSTEM_METERS:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SET_SYSTEM_METERS: " << endl;
                  #endif

                  #ifdef __VERBOSE_METERS__
                  debug << game_index << ":" << stage_index << "|GamePresentationMessage::SET_SYSTEM_METERS" << endl;
                  #endif
                  // Update the stored meter values.
                  GamePresentationSetSystemMeters::Result result = true;
                  responder.reply (client_coid, &result, sizeof (result));
                  // Now give the presentation a chance to synchronize the meter displays with the new data.
                  synchronizeMeters ();
               }
               break;

            // Request to start the game
            case GamePresentationMessage::START_REQUEST:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::START_REQUEST: " << endl;
                  #endif

                  const GamePresentationStartRequest& start_message = static_cast<const GamePresentationStartRequest &>(message);
                  State state (start_message.getStateHandle ());
                  GamePresentationStartRequest::Result result = start (state, start_message.sessionId ());
                  responder.reply (client_coid, &result, sizeof(result));
               }
               break;

            // Tell the stage to switch games.
            case GamePresentationMessage::SWITCH_GAME:
               {
                  #ifdef __VERBOSE_HANDLE_MESSAGE__
                  debug << "   GamePresentationMessage::SWITCH_GAME: " << endl;
                  #endif

                  const GamePresentationSwitchGame& switch_game_message = static_cast<const GamePresentationSwitchGame &>(message);
                  State state (switch_game_message.getStateHandle ());
                  GamePresentationSwitchGame::SwitchGameInfo const & switch_game_info (*(switch_game_message.getSwitchGameInfo ()));
                  GamePresentationSwitchGame::Result result = switchGame (state,
                     switch_game_info.registry_filename,
                     switch_game_info.safe_store_directory,
                     switch_game_info.essential_record_sub_directory, switch_game_info.shared_essential_record_sub_directory,
                        switch_game_info.er_file_name, switch_game_info.bet_er_file_name, switch_game_info.award_er_file_name,
                     switch_game_info.game_config_mgr_identifier,
                        switch_game_info.theme_config_mgr_identifier, switch_game_info.stage_config_mgr_identifier,
                     switch_game_info.game_index, switch_game_info.stage_index, switch_game_info.act_index, switch_game_info.game_identifier,
                     switch_game_info.initiating_stage, switch_game_info.initiating_act_index,
                     switch_game_info.new_denom,
                     switch_game_info.bet_so_filename, switch_game_info.bet_object_name,
                     switch_game_info.award_so_filename, switch_game_info.award_object_name,
                     switch_game_info.paytable_file_name,
                     switch_game_info.evaluator_so_filename, switch_game_info.evaluator_object_name,
                     switch_game_info.enrollment_required, switch_game_info.enrollment_before_game_start,
                     switch_game_info.mount_point);
                  responder.reply (client_coid, &result, sizeof(result));
               }
               break;

            default:
               // This would be a serious error.  Nobody handled this message.
               handled = false;
               System::error (LogicError (LogicError::INVALID_MESSAGE, "GamePresentation::handleMessage unhandled message."));
               break;
         }

         break;
      }
   }

   return handled;
}

// increment the relative game sounds volume.  Wrap to the lowest level if we are at the highest level
void GamePresentation::incrementVolume (void)
{
   #ifdef __VERBOSE_VOLUME__
   debug << "GamePresentation::incrementVolume ()" << endl;
   debug << "volume button exists " << (okayToEnableVolumeButton () ? "Yes" : "No") << endl;
   #endif

   if (okayToEnableVolumeButton ())
   {
      // get the current volume
      ConfigID volume_config_id = config.getId (CONFIG_ID_VOLUME_GAME_SOUNDS);
      uint32 volume = config.getValue (volume_config_id);
      #ifdef __VERBOSE_VOLUME__
      debug << "current volume " << volume << endl;
      #endif
      // get the current relative volume
      ConfigID config_id = config.getId (CONFIG_ID_VOLUME_GAME_SOUNDS_RELATIVE);
      float relative_volume = config.getValueFloat (config_id);
      #ifdef __VERBOSE_VOLUME__
      debug << "current relative volume " << relative_volume << endl;
      #endif

      if (relative_volume > 0)
      {
         // old value is HIGH, so new value will be LOW
         // Note: a higher volume value means a quieter volume
         if (volume <= GAME_VOLUME_LEVEL_50_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_LOW_50_AND_ABOVE);
         }
         else if (volume <= GAME_VOLUME_LEVEL_40_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_LOW_40_TO_49);
         }
         else
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_LOW_BELOW_40);
         }
      }
      else if (relative_volume < 0)
      {
         // old value is LOW, so new value will be DEFAULT
         config.setValueFloat (config_id, VOLUME_LEVEL_DEFAULT);
      }
      else
      {
         // old value is DEFAULT, so new value will be HIGH
         // Note: a higher volume value means a quieter volume
         if (volume <= GAME_VOLUME_LEVEL_70_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_HIGH_70_AND_ABOVE);
         }
         else if (volume <= GAME_VOLUME_LEVEL_60_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_HIGH_60_TO_69);
         }
         else if (volume <= GAME_VOLUME_LEVEL_50_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_HIGH_50_TO_59);
         }
         else if (volume <= GAME_VOLUME_LEVEL_40_PERCENT)
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_HIGH_40_TO_49);
         }
         else
         {
            config.setValueFloat (config_id, VOLUME_LEVEL_HIGH_BELOW_40);
         }
      }

      // update the volume property
      setVolumeProperty ();

      postEvent ("IncrementVolume:Done");
   }
   else
      postEvent ("IncrementVolume:Cancelled");
}


// Purpose: Initialize the presentation.
//          This call is made before the game runs.  This is the point where all shared
//          objects should be loaded and checked for validity.
//          The current state show enable/show/play the correct items once they
//          are loaded here.
bool GamePresentation::init (State& state, bool cold_start_flag)
{
   verboseMethodName();

   #ifdef __VERBOSE__
      debug << game_index << ":" << stage_index << "|GamePresentation::init ()" << endl;
   #endif

   // Add the configuration items to the group and initialize them
   uint32 number_of_config_items = 0;
   const char *const *current_item = configItems (number_of_config_items);
   for (uint32 i = 0; i < number_of_config_items; ++i)
      configurationChanged (config_group.addItem (current_item [i], game_configuration_identifier));

   // Initialize the bet object
   if (bet.isValid ())
      bet->init (theme_configuration_identifier, stage_configuration_identifier, game_identifier.asString (), stage_index, act_index);

   // Init safe storeage
   if (safe_store_directory_name.length () == 0)
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation: safe store directory not set!"));

   #ifdef __VERBOSE__
   debug << "  safe_store_directory_name=" << safe_store_directory_name << endl;
   #endif

   // Read the configuration file
   readConfigurationFile ();

   // Use the configured paytable name to create the appropriate paytable parser.
   loadPaytable ();

   // If the resource list has not yet been created, create it now.
   if (!resource_list.isValid ())
      resource_list.create <ResourceList> ();

   // If the persistent property list has not yet been created, create it now.
   if (!persistent_property_list.isValid ())
      persistent_property_list.create <PersistentPropertyList> ();

   // Process the configuration script file
   if (script_filename.length () > 0)
   {
      // Create the script resource importer.  Rely on the resource manager to error if the parser can not be created.
      Mojo<ScriptResourceImporterInterface> script_parser;
      createResource<ScriptResourceImporterInterface> (script_filename, resource_list->resourceKey (), script_parser);
      // Now parse the script.  By default, the following call will system error upon failure.
      Mojo<ScriptInterface> scriptable_object (this);
      script_parser->parseFile (scriptable_object);
   }

   Mojo<AutoplayInterface> autoplay_interface;
   if (getResource (autoplay_alias_name, resource_list->resourceKey (), autoplay_interface, false))
   {
      if (autoplay_interface.isValid ())
         autoplay_interface->setConfig (config);
   }

   // Register this game as a sequence event listener.  The sequence event system will
   // not work correctly if this listener is invalid.  Further, the presentation must
   // process the sequence events first as the state's processEvent functionality may
   // depend on the results.  Thus, we add the presentation and the state machine to
   // the listener controller at a defined index location.
   Mojo<SequenceEventListenerInterface> game_sequence_event_listener_interface (this);
   assertion (game_sequence_event_listener_interface.isValid ());
   if (!sequence_event_listener_controller->add (game_sequence_event_listener_interface, EVENT_LISTENER_GAME_PRESENTATION))
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::init Unable to register SequenceEventListener (game)."));

   // Register the state machine as a sequence event listener.  The sequence event system will
   // not work correctly if this listener is invalid.
   Mojo<SequenceEventListenerInterface> state_machine_sequence_event_listener_interface (state_machine);
   assertion (state_machine_sequence_event_listener_interface.isValid ());
   if (!sequence_event_listener_controller->add (state_machine_sequence_event_listener_interface, EVENT_LISTENER_STATE_MACHINE))
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::init Unable to register SequenceEventListener (state machine)."));

   // Retrieve the stage used by this game.
   StageInterfaces main_stage;
   getStage (main_stage);

   // Register the game as a multimedia event listener.  This will allow the game to receive
   // multimedia events as they are routed from the theatre through the main_stage.
   // Retrieve the main stage from the resource manager.
   if (!main_stage.isValid ())
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::init Unable to register MultimediaEventListener (game)."));

   // Register the event listener with the new stage.
   registerListener (main_stage.getActorStageInterface ());

   // Register the game as a multimedia event listener.  This will allow the game to receive
   // multimedia events as they are routed from the theatre through the main_stage.
   Mojo<MultimediaEventListenerInterface> state_machine_multimedia_event_listener (state_machine);
   assertion (state_machine_multimedia_event_listener.isValid ());
   addEventListener (state_machine_multimedia_event_listener);

   // Set state machine to default state. Init all the states.
   state_machine->init (current_state);

   /* Sets the credit meter to display the amount in cash if necessary prior to meter
      synchronization. */
   setCreditMeterDisplayBehavior (false);

   // Used to determine if CDS Manual Button Configuration is available.
   // This Config ID must be configured before synchronizePresentation() as it is
   // used subsequently in synchronizeSelectTheBet().
   manual_button_config_id = config.getId ("CDS MANUAL BUTTON CONFIG", game_identifier.asString (), false);

   // Draw everything on the screen
   synchronizePresentation (SynchronizePresentationSequenceEvent::SYNCHRONIZE_ALL);
   updateMaxBetProperty ();

   // Initialization was completed successfully
   return true;
}

// Purpose: Get the initiating act index
ActIndex GamePresentation::initiatingAct (void) const
{
   return initiating.act;
}

// Purpose: Get the initiating stage index
StageIndex GamePresentation::initiatingStage (void) const
{
   return initiating.stage;
}

// Purpose: Return true if the passed context mode will cause the presentation to stop playing.
bool GamePresentation::isASuspendContext (const ContextMode &new_mode) const
{
   bool suspends = false;
   switch (new_mode)
   {
      case CONTEXT_MODE_INVALID:
      case CONTEXT_MODE_HIBERNATE:
         suspends = true;
         break;
   }
   return suspends;
}

bool GamePresentation::isFlowStateIdle (void)
{
   return (getFlowState () == GameStateValue::STATE_IDLE);
}

// Purpose:  Returns true if the game is operating at max bet.
bool GamePresentation::isBetAtMaxBet (void)
{
   verboseMethodName ();
   // Initialize the return value.
   bool is_at_max_bet = false;
   // If this game has a valid bet (usually a base game), get the current bet information from the bet object.
   if (bet.isValid ())
   {
      // If the bet is at max bet, update the return value.
      if (bet->isBetAtMaxBet ())
         is_at_max_bet = true;
   }
   // If the game does not have a valid bet, display the bet value stored in the essential record.
   else
   {
      // If the game was started at max bet, update the return value.
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (essential_record && essential_record->maxBetAtStageStart ())
         is_at_max_bet = true;
   }
   return is_at_max_bet;
}

// Purpose: Returns true if the game side bet is precommitted or game bet is committed.
bool GamePresentation::isBetCommittedOrPrecommitted (void)
{
   bool bet_committed = false;
   Mojo <GameSideBetInterface> game_side_bet (bet);

   if (game_side_bet.isValid ())
      bet_committed = game_side_bet->precommitted ();

   if (bet.isValid ())
      bet_committed |= bet->committed ();

   return (bet_committed);
}

// Purpose: This returns the current activity status.  true = active, false = inactive.
bool GamePresentation::isGameActive (void)
{
   return current_game_activity_status;
}

// Purpose:  This method returns true if the game is currently hibernating.
bool GamePresentation::isHibernating (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::isHibernating=";
   debug << ((currentGameMode () == GAME_MODE_PLAY && currentContextMode () == CONTEXT_MODE_HIBERNATE) ? "true" : "false");
   debug << " GamMode=" << currentGameMode () << " Context=" << currentContextMode () << endl;
   #endif
   return (currentGameMode () == GAME_MODE_PLAY && currentContextMode () == CONTEXT_MODE_HIBERNATE);
}

// Purpose:  This method returns true if the game is in normal play mode..
bool GamePresentation::isNormalPlay (void)
{
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::isNormalPlay=";
   debug << ((currentGameMode () == GAME_MODE_PLAY && currentContextMode () == CONTEXT_MODE_NORMAL) ? "true" : "false");
   debug << " GamMode=" << currentGameMode () << " Context=" << currentContextMode () << endl;
   #endif
   return (currentGameMode () == GAME_MODE_PLAY && currentContextMode () == CONTEXT_MODE_NORMAL);
}

// Purpose: Return true if one or more progressives are linked to the game/game stage.
bool GamePresentation::isProgressiveLinked (void)
{
   bool result = false;

   Denomination controller_denom = Denomination::BLANK;
   ProgressiveGameId progressive_game_id (game_identifier, stage_index);
   // for each progressive denom that this games supports
   uint32 progressive_denom_bitfield_value = progressive_manager.progressiveDenomBitField (progressive_game_id);

   BitField progressive_denom_bitfield;
   progressive_denom_bitfield.setUint32 (progressive_denom_bitfield_value);
   // for each denom
   Config denom_list_config;
   ConfigID reference_list_id = denom_list_config.getId ("ACCOUNTING DENOMINATION LIST");
   uint32 denoms_in_list = denom_list_config.getListSize (reference_list_id);

   for (uint32 bit_index = 0; (bit_index < denoms_in_list) && (!result); ++bit_index)
   {
      if (progressive_denom_bitfield.getBit (bit_index))
      {
         Denomination extracted_denomination;
         // Extract the denomination associated with the nth set bit from the reference list.//
         String denomination_string;
         denom_list_config.getListItem (reference_list_id, bit_index, denomination_string);
         uint32 denomination_value = atoi (denomination_string);
         extracted_denomination.setValue (denomination_value);

         uint32 number_of_levels = progressive_manager.numberOfLevels (progressive_game_id, extracted_denomination);

         for (uint32 level_index = 0; level_index < number_of_levels; ++level_index)
         {
            ProgressiveControllerId controller_id;
            uint32 controller_level;
            bool is_paytable_formed;
            uint32 list_position = 0;

            bool game_link_result = progressive_manager.getGameLink (progressive_game_id, level_index,
                     controller_id, controller_level, is_paytable_formed, list_position, extracted_denomination, controller_denom);

            #ifdef __VERBOSE__
            debug << "GamePresentation::isProgressiveLinked (void)" << endl;
            debug << "  progressive_game_id: " << progressive_game_id;
            debug << "  level_index: " << level_index << endl;;
            debug << "  controller_id: " << controller_id;
            debug << "  controller_level: " << controller_level << endl;
            debug << "  is_paytable_formed: " << (uint16) is_paytable_formed;
            debug << "  list_position: " << list_position << endl;
            debug << endl;
            #endif

            if (game_link_result)
            {
               result = true;
               break;
            }
         }
      }
   }
   return result;
}

// Purpose: Returns true if the rollup is finished.
bool GamePresentation::isRollupComplete (void)
{
   // Initialize the return value.
   bool rollup_complete = true;
   GamePresentation::GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
      rollup_complete = game_control_panel.getGameControlPanelInterface ()->isRollupComplete ();
   return rollup_complete;
}

// Return if the speed is player modifiable.  Each game will handle this differently
// so only return the value.
bool GamePresentation::isSpeedPlayerModifiable (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::isSpeedPlayerModifiable ()" << endl;
   #endif

   // Define the default value.
   bool return_value = false;

   ConfigID config_id = config.getId (CONFIG_ID_PAY_SPEED_SPEED_PLAYER_MODIFIABLE, game_configuration_identifier, false);

   // If this game doesn't support the game speed then return the default value.
   if (config_id != INVALID_CONFIGID)
      return_value = config.getValue (config_id);

   return return_value;
}

// Called when we are about to leave the object's scope.
void GamePresentation::leavingObjectScope (void)
{
   verboseMethodName ();
   // Call the base class first.
   Actuable::leavingObjectScope ();

   // Verify that the main stage has been confiugred.
   StageInterfaces main_stage;
   if (!getStage (main_stage))
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::leavingObjectScope: The main stage has not been configured for this game."));

   // Finalize the configuration of all control panels registered by this game.
   finalizeControlPanelConfiguration ();
}

/// <summary> Initialize bet object from its configuration file.</summary>
/// <remarks> This function must be called before attaching the bet object.</remarks>
void GamePresentation::loadBetConfiguration (void)
{
   // Open the configuration file
   Registry registry;
   registry.open (configuration_filename);

   String result;
   if (registry.find (result, "General", "Theme"))
   {
      Registry themereg;
      if (themereg.open (resource_mount_point + result))
      {
         if (themereg.find (result, "General", "RuleBasedBetLoaderConfig"))
         {
            if (bet.isValid ())
            {
               String bet_config_file_full_path = resource_mount_point + result;
               Mojo <RuleBasedGameSideBetInterface> rule_based_game_side_bet (bet);
               rule_based_game_side_bet->loadBetConfiguration (bet_config_file_full_path.c_str ());
            }
         }
         themereg.close ();
      }
   }
   registry.close();
}

// Load the paytable provided.  If 0 is passed for the paytable filename, the current paytable will be loaded.
// If 0 is passed for the section name, the default paytable section will be loaded.
// Returns: true on success.
bool GamePresentation::loadPaytable (const char *filename, const char *section)
{
   verboseMethodName ();

   // Check for the caller passing zero.
   if (filename != 0)
      setPaytableFilename (filename);

   #ifdef __VERBOSE_PAYTABLE__
   debug << game_index << ":" << stage_index << "|GamePresentation::loadPaytable () filename: " << getPaytableFilename () << endl;
   #endif

   // Use the configured paytable name to create the appropriate paytable parser.
   paytable_parser.release ();
   if (!createObject<PayTableParserInterface> (evaluator_so_file_name, evaluator_object_name, paytable_parser))
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::loadPaytable (): unable to create paytable_parser."));
   //  Open the paytable script file.
   paytable_parser->openPayTable (getPaytableFilename (), evaluator_so_file_name);

   // Support the default paytable section name.
   String section_name (paytable_parser->getPayTableSectionName (stageIndex ()));
   supportPaytableSection (section_name);
   // Make the default evalautor the active evaluator.
   if (!evaluatorForPaytableSection (evaluator, section_name))
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::loadPaytable (): unable to initialize default evaluator."));

   return true;
}

// Purpose:  This method is called to mute/unmute the sounds configured for this game presentation.
void GamePresentation::muteSounds (bool mute)
{
   Mojo<SoundSystemInterface> sound_system;
   getResource<SoundSystemInterface> (SOUND_SYSTEM, "Default", sound_system);
   // Mute/unmute all sounds associated with a game presentation.
   sound_system->muteSoundGroupVolume (SoundSystemInterface::VOCAL, mute);
   sound_system->muteSoundGroupVolume (SoundSystemInterface::GAME_SPECIAL, mute);
   sound_system->muteSoundGroupVolume (SoundSystemInterface::GAME_SOUNDS, mute);
   sound_system->muteSoundGroupVolume (SoundSystemInterface::GAME_PRESET_VOLUME_SOUNDS, mute);
}

// Purpose:  Retrieve the Amount of the current sub bet meter.
void GamePresentation::numberOfSubBets (uint32 &number_of_sub_bets)
{
   // If this game has a valid bet (usually a base game), get the current bet information from the bet object.
   if ((stageIndex () == BASE_GAME_STAGE) && bet.isValid ())
      number_of_sub_bets = bet->getNumberOfSubBets ();
   // If the game does not have a valid bet (usually a bonus game), display 0.
   else
      number_of_sub_bets = 0;

   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::numberOfSubBets = " << number_of_sub_bets << endl;
   #endif
}
// Purpose: Return the amount of denominations the player may choose from
uint32 GamePresentation::numberOfAvailablePlayerDenominations (void)
{
   // Get the list of displayable denominations for this theme.
   Config config;
   BitField theme_denom_bitfield;
   BitField terminal_denom_bitfield;

   ConfigID theme_id = config.getId (CONFIG_ID_THEME_DISPLAYED_TO_PLAYER_DENOMINATIONS);
   if (theme_id != INVALID_CONFIGID)
   {
      // Retreive the theme bitfield value.
      config.getValueBitField (theme_id, theme_denom_bitfield);
   }

   // Get the list of displayable denominations for the machine (i.e. all games.)
   ConfigID terminal_id = config.getId (CONFIG_ID_TERMINAL_ENABLED_PLAYER_DENOMINATIONS);
   if (terminal_id != INVALID_CONFIGID)
   {
      // Retreive the terminal bitfield value
      config.getValueBitField (terminal_id, terminal_denom_bitfield);
   }

   // The combined bitfield will be used to determine the number of available player denominations
   BitField combined_bitfield = theme_denom_bitfield.bitwiseAnd (terminal_denom_bitfield);

   return combined_bitfield.numberOfBitsSet ();
}

// Purpose: Return the number of games the player may choose from
uint32 GamePresentation::numberOfAvailablePlayerGames (void)
{
   uint32 available_player_games = 1;  // Assumes one game

   if (number_of_playable_themes_config_id != INVALID_CONFIGID)
      available_player_games = config.getValue (number_of_playable_themes_config_id);

   return available_player_games;
}
// Purpose:  Returns the value of the config.
bool GamePresentation::okayToEnableVolumeButton (void)
{
   ConfigID config_id = config.getId (CONFIG_ID_VOLUME_PLAYER_SELECTABLE);
   return (config.getValue (config_id));
}

// Purpose: Returns the value of the associated config.
// Explaination: If config item is true, game play should not never be suspended.
// Example: During enrollment, Class II/NYL slots should keep spinning if the door is opened if this item is TRUE.
bool GamePresentation::okayToSuspendGameDisplayEarly (void)
{
   return okay_to_suspend_game_display_early;
}

// Purpose:  Retrieve the Amount of the current player paid meter
void GamePresentation::paidDisplayAmount (Amount &paid)
{
   verboseMethodName ();
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record)
      paid = essential_record->playerPaid ();
   else
      paid = ZERO_AMOUNT;
   #ifdef __VERBOSE_METERS__
   Amount::Extractor amount_extractor (paid);
   debug << game_index << ":" << stage_index << "|GamePresentation::paidDisplayAmount = " << amount_extractor.extract (denomination) << endl;
   #endif
}

// Purpose: Returns true if the provided paytable section name is supported by this game.
bool GamePresentation::paytableSectionSupported (const char *section)
{
   verboseMethodName ();
   bool supported = false;
   // Verify that a game set for the defined paytable section has not already been defined.
   ConstEvaluatorIterator iterator = evaluator_list.find (section);
   if (iterator != evaluator_list.end ())
      supported = true;
   #ifdef __VERBOSE_PAYTABLE__
   debug << game_index << ":" << stage_index << "|GamePresentation::paytableSectionSupported, " << String (section) << (supported ? ":YES" : ":NO") << endl;
   #endif
   return supported;
}

// Purpose:  Return the number of times this game has been played.
uint16 GamePresentation::playCount (ActType act_type)
{
   uint16 count = 0;
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record)
   {
      count = essential_record->playCount (act_type);
   }

   #ifdef __VERBOSE__
   debug << "GamePresentation::playCount:  " << count << endl;
   #endif
   return count;
}

// Purpose: Separate the string of comma separated elements into a vector.
template<class T> void GamePresentation::parseCommaSeparatedList (const String &string, vector<T> &result) const
{
   // String is in the format: "1,2,3".
   result.clear ();
   // Make a copy of the string since strtok will modify it.
   String temp_string (string);
   const char *const_temp_string = temp_string;
   char *temp = const_cast<char *> (const_temp_string);
   // Make a list of delimiters.
   const char *delimiters = ", \t";
   // For every token in the string...
   char *p = strtok (temp, delimiters);
   while (p != 0)
   {
      // Add token to the vector.
      result.push_back (T ());
      result.back () = strtoul (p, 0, 10);
      // Next token.
      p = strtok (0, delimiters);
   }
}

// Purpose: Parse the registry file to get the script that configures the presentation.
void GamePresentation::parseConfigurationFile (Registry& registry)
{
   verboseMethodName();
   String result;

   String stage_section_name ("Stage"); // Name of the section in the registry file
   stage_section_name << (uint16)stage_index;
   if (registry.find (result, stage_section_name, "PresentationScript"))
      setScriptFilename (result);

   // Access the theme registry
   if (registry.find(result, "General", "Theme"))
   {
      Registry themereg;
      if(themereg.open(resource_mount_point + result))
      {
         String bool_string;
         if (themereg.find (bool_string, "SystemMainMenu", "DisplayStringInsteadOfProgressiveValue"))
         {
            bool_string.toLowerCase();
            display_advertisement_text = bool_string.asBool();
         }
         else
         {
            display_advertisement_text = false;
         }

         themereg.close();
      }
   }
}

/// <summary># Parse the trigger name from the given parameters.</summary>
void GamePresentation::parseTriggerName (const String &parameters, String &trigger_name)
{
   trigger_name.clear ();
   size_t index = parameters.findFromRight (':');

   if (index != static_cast <size_t> (-1) && index != parameters.length () - 1)
   {
      trigger_name = parameters.right (parameters.length () -1 - index);
   }
}

// Purpose: Post act outcome events
bool GamePresentation::postActOutcomeEvents (void)
{
   verboseMethodName ();
   bool monetary_win, trigger_another, trigger_self, success;
   success = GamePresentationHelper::awardSummary (this, monetary_win, trigger_another, trigger_self);
   if (success)
   {
      if (monetary_win)
         postEvent ("ActOutcome:MonetaryWin");
      else
         postEvent ("ActOutcome:MonetaryLoss");
      if (trigger_another)
         postEvent ("ActOutcome:TriggerAnother");
      if (trigger_self)
         postEvent ("ActOutcome:TriggerReplay");
      if (!trigger_another && !trigger_self)
         postEvent ("ActOutcome:TriggerNothing");
      if (!monetary_win && !trigger_another && !trigger_self)
         postEvent ("ActOutcome:Loser");
   }
   return success;
}

// Purpose:  Post game start events communicating generic and play count events.
void GamePresentation::postGameStartEvents (void)
{
   verboseMethodName ();
   // Post a generic game start event.
   String event_string ("GameStart");
   postEvent (event_string);
   // Post a numbered MultimediaEvent for GameStart
   uint16 play_count = playCount (ACT_TYPE_BONUS);
   event_string << play_count;
   postEvent (event_string);
}

// Purpose: Post a sequence event.
//          Returns true on success.
bool GamePresentation::postEvent (SequenceEvent &event)
{
   verboseMethodName ();
   return sequence_event_poster.post (event);
}

// Posts a multimedia event through the main_stage's interface to the queue.
bool GamePresentation::postEvent (MultimediaEvent& multimedia_event)
{
   bool success = false;
   StageInterfaces main_stage;
   getStage (main_stage);
   if (main_stage.isValid ())
      success = main_stage.getActorStageInterface ()->postEvent (multimedia_event);
   return success;
}

// Purpose: Posts a multimedia event through the main_stage's interface to the queue.  Returns true on success.
bool GamePresentation::postEvent (const char *event_string)
{
   // Construct an event with the string received.  This method relies on the multimedia event class'
   // error checking on the string.
   MultimediaEvent multimedia_event (event_string);
   return postEvent (multimedia_event);
}

// Posts a multimedia event through the main_stage's interface to the queue.
bool GamePresentation::postEventAll (MultimediaEvent& multimedia_event)
{
   bool success = false;
   StageInterfaces main_stage;
   getStage (main_stage);
   if (main_stage.isValid ())
      success = main_stage.getActorStageInterface ()->postEventAll (multimedia_event);
   return success;
}

// Purpose: Posts a multimedia event through the main_stage's interface to the queue.  Returns true on success.
bool GamePresentation::postEventAll (const char *event_string)
{
   // Construct an event with the string received.  This method relies on the multimedia event class'
   // error checking on the string.
   MultimediaEvent multimedia_event (event_string);
   return postEventAll (multimedia_event);
}

// Posts a MultimediaEvent to all the stages in our context.
bool GamePresentation::postEventContext (class MultimediaEvent& multimedia_event)
{
   bool success = false;
   StageInterfaces main_stage;
   getStage (main_stage);
   if (main_stage.isValid ())
      success = main_stage.getActorStageInterface ()->postEventContext (multimedia_event);
   return success;
}

// Posts a MultimediaEvent to all the stages in the defined context.
bool GamePresentation::postEventContext (class MultimediaEvent& multimedia_event, const char *context_name)
{
   bool success = false;
   StageInterfaces main_stage;
   getStage (main_stage);
   if (main_stage.isValid ())
      success = main_stage.getActorStageInterface ()->postEventContext (multimedia_event, context_name);
   return success;
}

// Posts a MultimediaEvent with the string received to all the stages in our context.
bool GamePresentation::postEventContext (const char *event_string)
{
   // Construct an event with the string received.  This method relies on the multimedia event class'
   // error checking on the string.
   MultimediaEvent multimedia_event (event_string);
   return postEventContext (multimedia_event);
}

// Posts a MultimediaEvent to all the stages in the defined context.
bool GamePresentation::postEventContext (const char *event_string, const char *context_name)
{
   // Construct an event with the string received.  This method relies on the multimedia event class'
   // error checking on the string.
   MultimediaEvent multimedia_event (event_string);
   return postEventContext (multimedia_event, context_name);
}

// Posts the MultimediaEvent received to the MultimediaEvent queue.
bool GamePresentation::postEventExtended (class MultimediaEvent& multimedia_event, uint64 delay, class ScriptTableParameter* payload)
{
   bool ok = false;
   StageInterfaces stage;
   getStage (stage);
   Mojo<ActorStageInterface> actor_stage_inteface = stage.getActorStageInterface ();
   if (actor_stage_inteface.isValid ())
   {
      ok = actor_stage_inteface->postEventExtended (multimedia_event, delay, payload);
   }

   return ok;
}

// Posts a MultimediaEvent with the event string received MultimediaEvent queue.
bool GamePresentation::postEventExtended (const char *multimedia_event_string, uint64 delay, class ScriptTableParameter* payload)
{
   MultimediaEvent multimedia_event (multimedia_event_string);
   return postEventExtended (multimedia_event, delay, payload);
}

// Posts a MultimediaEvent to all the stages in our context.
bool GamePresentation::postEventContextExtended (class MultimediaEvent& multimedia_event, uint64 delay, class ScriptTableParameter* payload)
{
   bool ok = false;
   StageInterfaces stage;
   getStage (stage);
   Mojo<ActorStageInterface> actor_stage_inteface = stage.getActorStageInterface ();
   if (actor_stage_inteface.isValid ())
   {
      ok = actor_stage_inteface->postEventContextExtended (multimedia_event, delay, payload);
   }

   return ok;
}

// Posts a MultimediaEvent to all the stages in the defined context.
bool GamePresentation::postEventContextExtended (class MultimediaEvent& multimedia_event, const char *context_name, uint64 delay, class ScriptTableParameter* payload)
{
   bool ok = false;
   StageInterfaces stage;
   getStage (stage);
   Mojo<ActorStageInterface> actor_stage_inteface = stage.getActorStageInterface ();
   if (actor_stage_inteface.isValid ())
   {
      ok = actor_stage_inteface->postEventContextExtended (multimedia_event, context_name, delay, payload);
   }

   return ok;
}

// Posts a MultimediaEvent with the string received to all the stages in our context.
bool GamePresentation::postEventContextExtended (const char *multimedia_event_string, uint64 delay, class ScriptTableParameter* payload)
{
   MultimediaEvent multimedia_event (multimedia_event_string);
   return postEventContextExtended (multimedia_event, delay, payload);
}

// Posts a MultimediaEvent to all the stages in the defined context.
bool GamePresentation::postEventContextExtended (const char *multimedia_event_string, const char *context_name, uint64 delay, class ScriptTableParameter* payload)
{
   MultimediaEvent multimedia_event (multimedia_event_string);
   return postEventContextExtended (multimedia_event, context_name, delay, payload);
}

// Purpose: Post a multimedia event describing if more than one player selectable game
//  is available to the player.
void GamePresentation::postPlayerSelectableGameStatus (void)
{
   // Create an event to denote if more than one player selectable game or not.
   String event = "MoreThanOnePlayerSelectableGame:";

   if (numberOfAvailablePlayerGames () > 1)
   {
      event << "true";
   }
   else
   {
      event << "false";
   }

   postEvent (event);
}

// Purpose: Post a state complete sequence event to the game flow for the current
//          state and presentation substate
void GamePresentation::postStateComplete (void)
{
   verboseMethodName ();
   StateCompleteSequenceEvent state_event (getFlowState (), getPresentationSubstate (), SequenceEvent::TO_FLOW);
   postEvent (state_event);
}

// Purpose: Parse the game switch script
void GamePresentation::parseGameSwitchScript (void)
{
   verboseMethodName();

   // Open the configuration file
   Registry registry;
   registry.open (configuration_filename);

   String script_filename;
   String stage_section_name ("Stage"); // Name of the section in the registry file
   stage_section_name << (uint16)stage_index;
   if (registry.find (script_filename, stage_section_name, "PresentationGameSwitchScript"))
   {
      // Process the configuration script file
      if (script_filename.length () > 0)
      {
         // Create the script resource importer.  Rely on the resource manager to error if the parser can not be created.
         Mojo<ScriptResourceImporterInterface> script_parser;
         createResource<ScriptResourceImporterInterface> (script_filename, resource_list->resourceKey (), script_parser);
         // Now parse the script.  By default, the following call will system error upon failure.
         Mojo<ScriptInterface> scriptable_object (this);
         if (!script_parser->parseFile (scriptable_object))
         {
            String error_message = "Cannot parse script: ";
            error_message << scriptable_object->whoAreYou () << ", " << script_filename;
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
      }
   }
   registry.close ();
}

// Purpose: Final preparations before this presentation is unloaded.
bool GamePresentation::prepareForUnloading (void)
{
   verboseMethodName();
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::prepareForUnloading ()" << endl;
   #endif
   // This event is used to let scripts that the game is about to unload
   // and it should do any clean up that is necessary
   postEventAll ("PrepareForUnload");
   // Release everything in the configuration changed list
   configuration_changed_list.clear ();
   config_group.clear ();

   // Remove the presentation from the sequence event listener controller
   if (sequence_event_listener_controller.isValid ())
      sequence_event_listener_controller->clear ();
   // Free the SequenceEvent listener controller.
   sequence_event_listener_controller.release ();

   // Clear any script resource importers that this presentation has a handle to.
   // This must be done before clearing out all of the resources held by this presentation.
   // If a callback is registered to a resource that is destroyed and the resource importer
   // is not destroyed, then the callback can still try to access the deleted object and crash.
   script_resource_importers.clear ();

   // Remove the presentation from the sequence event listener controller
   removeListener ();

   // Release any local lists/cached mojos
   // *******************************************************************
   // NOTE: This must be done before the resource list is destructed.  This
   //       ensures that when the resource list is released, the items within
   //       it are also released.
   // *******************************************************************
   game_set_configuration_map.clear ();
   award_list.release ();
   evaluator.release ();
   evaluator_list.clear ();
   paytable_parser.release ();
   bet.release ();
   presentation_extension_container.release ();
   state_machine.release ();

   game_control_panel.clear ();
   game_count_control_panel.clear ();

   // Release the resource list.
   resource_list.release ();
   // Release the persistent property list.
   persistent_property_list.release ();

   igt_connect.release ();
   igt_connect_display.release ();

   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::prepareForUnloading (), complete" << endl;
   #endif
   return true;
}

// Purpose: Return the last applied context mode.
const ContextMode& GamePresentation::previousContextMode (void) const
{
   return previous_context_mode;
}

// Purpose: Return true if central determination enrollment is required to complete before the game starts.
bool GamePresentation::processConditionEnrollmentBeforeGameStart (void)
{
   return enrollmentBeforeGameStart ();
}

// Purpose: Return true if a central determination game has hit the spin button twice.
bool GamePresentation::processConditionEnrollmentButtonDoubleTap (void)
{
   return getEnrollmentButtonDoubleTap ();
}

// Purpose: Return true if this is the first game play.
bool GamePresentation::processConditionFirstGame (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   return (essential_record && essential_record->playCount ()) ? false : true;
}

// Purpose: Return true if central determination enrollment is required at all for this game.
bool GamePresentation::processConditionEnrollmentRequired (void)
{
   return enrollmentRequired ();
}

// Purpose: Return true if the central determination RNG seeds have arrived for this game.
bool GamePresentation::processConditionGameEnrollmentSeedsArrived (void)
{
   return getGameEnrollmentSeedsArrived ();
}

// Purpose: Return true if the games remaining count matches one of the specified values.
bool GamePresentation::processConditionGamesRemaining (String parameters)
{
   bool match = false;
   uint16 games_remaining = gamesRemaining ();
   // Example string is "0,1,2".
   // Separate the parameter string into a vector of parameters.
   vector<uint16> parameter_list;
   parseCommaSeparatedList (parameters, parameter_list);
   // If no parameters were specified...
   if (!parameter_list.size ())
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition GAMES_REMAINING does not specify how many games remaining."));
   // For every game remaining value specified...
   for (uint32 i = 0; i < parameter_list.size (); ++i)
      // If games remaining matches...
      if (parameter_list [i] == games_remaining)
      {
         match = true;
         break;
      }
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionGamesRemaining (" << parameters << ')' << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
      debug << parameter_list [i] << ',';
   debug << endl;
   debug << "  games remaining = " << games_remaining << endl;
   debug << "  result = " << uint32 (match) << endl;
   #endif
   // Return result.
   return match;
}

// Purpose: Return true if the play count matches one of the specified values for a specific act type.
bool GamePresentation::processConditionPlayCount (String parameters)
{
   bool match = false;
   String play_count_type;
   String act_type_parms;
   ActType act_type_id = ACT_TYPE_UNKNOWN;
   uint16 current_play_count = 0;
   const EssentialRecord *essential_record = getEssentialRecord ();

   // Strip out and find which act type to check the play count against
   play_count_type = parameters.left (parameters.find (':', 0));
   act_type_parms = parameters.right (parameters.length () - play_count_type.length ());
   play_count_type.strip (':', String::STRIP_ALL);
   act_type_parms.strip (':', String::STRIP_ALL);

   // Loop thru all known act types
   if (essential_record)
   {
      for (ActType index = ACT_TYPE_UNKNOWN; index < ACT_TYPE_MAX; index++)
      {
         // Determine if our act type in the condition matches a known type
         if (essential_record->actTypeName (index) == play_count_type)
         {
            act_type_id = index;
            break;
         }
      }
      // Unknown type, throw an error
      if (act_type_id == ACT_TYPE_UNKNOWN)
      {
         System::error (LogicError (LogicError::INVALID_PARAMETER, "PLAY_COUNT does not specify a known act type."));
      }
      else
      {
         current_play_count = essential_record->playCount (act_type_id);
      }
   }
   else
   {
      System::error (LogicError (LogicError::INVALID_PARAMETER, "GamePresentation::processConditionPlayCount - No essential record found."));
   }

   // Example string is "0,1,2".
   // Separate the parameter string into a vector of parameters.
   vector<uint16> parameter_list;
   parseCommaSeparatedList (act_type_parms, parameter_list);
   // If no parameters were specified...
   if (!parameter_list.size ())
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition PLAY_COUNT does not specify a valid act index."));
   // For every play count value specified...
   for (uint32 i = 0; i < parameter_list.size (); ++i)
   {
      // If play count matches...
      if (parameter_list [i] == current_play_count)
      {
         match = true;
         break;
      }
   }
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionPlayCount (" << parameters << ')' << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
      debug << parameter_list [i] << ',';
   debug << endl;
   debug << "  play_count = " << current_play_count << endl;
   debug << "  result = " << uint32 (match) << endl;
   #endif
   // Return result.
   return match;
}

// Purpose: Return true if a game was unenrolled since the last play.
bool GamePresentation::processConditionGameWasUnenrolled (void)
{
   return gameWasUnenrolled ();
}

// Purpose: Return true if this is the last game play.
bool GamePresentation::processConditionLastGame (void)
{
   return ((gamesRemaining () == 0) && (currentAwardContainsNewRunnableTriggers () == false));
}

// Purpose: Return true if the paytable_section is equal to the current paytable section.
bool GamePresentation::processConditionPaytableSection (String paytable_section)
{
   bool match = false;

   // Since we the condition syntax can not handle spaces in section name, we need
   // to provide support for the special case for the base game section name; i.e.
   // handle the special case where "BASE_GAME" really means "base game".
   if (paytable_section == "BASE_GAME")
      paytable_section = "base game";

   if (paytable_section == currentPaytableSection ())
      match = true;

   // Return result.
   return match;
}

// Purpose: Return true if multi-denom is enabled.
bool GamePresentation::processConditionPlayerSelectableDenomination (void)
{
   bool tournament_mode_enabled = SystemFlags ().systemInTournamentMode ();
   return ((numberOfAvailablePlayerDenominations () > 1) && !tournament_mode_enabled && !bank_bet.isBankLocked ());
}

// Purpose: Return true if multi-game is enabled.
bool GamePresentation::processConditionPlayerSelectableGame (void)
{
   bool tournament_mode_enabled = SystemFlags ().systemInTournamentMode ();
   return ((numberOfAvailablePlayerGames () > 1) && !tournament_mode_enabled && !bank_bet.isBankLocked ());
}
// Purpose: Return true if no award digits still need to be picked.
bool GamePresentation::processConditionPlayerSelectableVolume (void)
{
   return okayToEnableVolumeButton ();
}

/// <summary>Determine whether or not AutoPlay is enabled and simulating player input</summary>
/// <returns>True if autoplay is enabled and simulating player input, false otherwise</returns>
bool GamePresentation::processConditionAutoPlaySimulateInput (void)
{
   Config config;
   bool autoplay_simulate_input = false;
   ConfigID autoplay_simulate_id = config.getId ("AUTOPLAY SIMULATE GAME INPUTS", false);

   if (autoplay_simulate_id != INVALID_CONFIGID)
   {
      autoplay_simulate_input = config.getValue (autoplay_simulate_id);
   }

   return autoplay_simulate_input;
}

// Purpose: Return true if one or more progressives are linked to the game/game stage.
bool GamePresentation::processConditionProgressiveLinked (void)
{
   return isProgressiveLinked ();
}

// Purpose:  Returns true if the rollup is finished.
bool GamePresentation::processConditionRollupComplete (void)
{
   return isRollupComplete ();
}

// Purpose: Return true if the current act resulted in a monetary win.
bool GamePresentation::processConditionWinThisAct (void)
{
   bool monetary_win = false, trigger_another = false, trigger_self = false;
   bool success = GamePresentationHelper::awardSummary (this, monetary_win, trigger_another, trigger_self);

   // Bingo grand prize only wins should count as winning games.
   if (isBingo () && !( monetary_win && success ))
   {
      Amount bingo_grand_prize (ZERO_AMOUNT);
      getDisplayableBingoGrandPrizeAmount (bingo_grand_prize);

      monetary_win |= bingo_grand_prize.isNotZero ();
      success = monetary_win;
   }

   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionWinThisAct ()" << endl;
   debug << "  result = " << uint32 (monetary_win && success) << endl;
   #endif
   // Return result.
   return monetary_win && success;
}

// Return true if the current act_type is equal to act_name.
bool GamePresentation::processConditionIsActType (String act_name)
{
   bool result = false;
   const EssentialRecord *essential_record = getEssentialRecord ();

   if (essential_record)
   {
      result = (essential_record->actTypeName(essential_record->act_type) == act_name);
   }
   // Return result.
   return result;
}

// Purpose: Return true if the current act resulted in a monetary win or bonus trigger.
bool GamePresentation::processConditionAnyAwardThisAct (void)
{
   bool monetary_win = false, trigger_another = false, trigger_self = false;
   bool success = GamePresentationHelper::awardSummary (this, monetary_win, trigger_another, trigger_self);

   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionAnyAwardThisAct ()" << endl;
   debug << "  result = " << uint32 (monetary_win && success) << endl;
   #endif
   // Return result.
   return success && (trigger_another || trigger_self || monetary_win);
}

// Purpose: Return true if a win matches the type specified.
bool GamePresentation::processConditionAwardType (String parameter)
{
   bool match = false;
   SystemAwardInterface::AwardType award_type = SystemAwardInterface::NO_TYPE;

   if (parameter == "SYSTEM_ONLY")
   {
      award_type = SystemAwardInterface::SYSTEM_ONLY;
   }
   else if (parameter == "SLOT")
   {
      award_type = SystemAwardInterface::SLOT;
   }
   else if (parameter == "POKER")
   {
      award_type = SystemAwardInterface::POKER;
   }
   else if (parameter == "KENO")
   {
      award_type = SystemAwardInterface::KENO;
   }
   else if (parameter == "GAME")
   {
      award_type = SystemAwardInterface::GAME;
   }
   else if (parameter == "BINGO")
   {
      award_type = SystemAwardInterface::BINGO;
   }
   else if (parameter == "WILDTILES")
   {
      award_type = SystemAwardInterface::WILDTILES;
   }
   #ifdef __VERBOSE_CONDITIONS__
   else
   {
      debug << "Award type \"" << parameters << "\" requested, using NO_TYPE" << endl;
   }
   #endif

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   for (uint16 award_index = 0; award_index < local_award->getNumberOfAwards (); award_index++)
   {
      Mojo<SystemAwardInterface> award;
      if (local_award->getAward (award_index, award) 
         && award->getAwardType () == award_type)
      {
         match = true;
         break;
      }
   }

   return match;
}

// Purpose: Return true if a hit win category matches one of those specified.
bool GamePresentation::processConditionWinCategory (String parameters)
{
   String trigger_name;
   parseTriggerName (parameters, trigger_name);   // Example string is "1,2,3:Stage1".
   // Separate the parameter string into a vector of parameters.
   vector<WinCategoryId> parameter_list;
   parseCommaSeparatedList (parameters.left (parameters.length () - trigger_name.length ()), parameter_list);

   return matchWinCategory (parameter_list, trigger_name.length () > 0 ? trigger_name.c_str () : 0);
}

/// <summary>Check whether the hit win category matches one of the checked categories.</summary>
/// <param name="checked_categories">[in] Win category vector to be matched.</param>
/// <param name="trigger">[in] The trigger name for the checked win categories.</param>
/// <returns>true if a hit win category matches one of those specified.</returns>
bool GamePresentation::matchWinCategory (const vector<WinCategoryId> checked_categories, const char *trigger)
{
   bool match = false;
   // If no categories were specified...
   if (checked_categories.size () == 0)
   {
      setErrorString (LogicError (LogicError::INVALID_PARAMETER, "GamePresentation::matchWinCategory (): No win categories are specified to check.").errorMessage (), whoAreYou ());
   }
   else
   {
      // Get current award information.
      Mojo<AwardInterface> local_award;
      readAwardFromEssentialRecord (local_award);
      // Get a list of hit win categories.
      const Mojo<GamePresentation, ScriptInterface> game_presentation (this);
      vector<WinCategoryId> win_categories;
      GamePresentationHelper::winCategories (game_presentation, local_award, win_categories, trigger);
      // For every win category hit...
      for (uint32 i = 0; (i < win_categories.size ()) && !match; ++i)
      {
         // For every category parameter...
         for (uint32 j = 0; (j < checked_categories.size ()) && !match; ++j)
         {
            // If match...
            if (win_categories [i] == checked_categories [j])
            {
               match = true;
            }
         }
      }

      #ifdef __VERBOSE_CONDITIONS__
      debug << "GamePresentation::matchWinCategory ()" << endl;
      debug << "  checked_categories = ";
      for (uint32 i = 0; i < checked_categories.size (); ++i)
         debug << checked_categories [i] << ',';
      debug << endl;
      debug << "  trigger = " << trigger << endl;
      debug << "  hit win categories = ";
      for (uint32 i = 0; i < win_categories.size (); ++i)
         debug << win_categories [i] << ',';
      debug << endl;
      debug << "  result = " << uint32 (match) << endl;
      #endif
   }

   // Return result.
   return match;
}

// Purpose: Return true if no hit win category matches those specified.
bool GamePresentation::processConditionNotWinCategory (String parameters)
{
   // Return result.
   return !processConditionWinCategory (parameters);
}

// Return true if a win is greater than a specified amount.
bool GamePresentation::processConditionWinCreditRange (String parameters, const Amount &win_amount)
{
   bool match = false;
   bool upper_limit_specified = false;
   uint32 credit_threshold = 0;
   int32 high_credit_threshold = 0;

   // Example string is "1,3".
   // Separate the parameter string into a vector of parameters.
   vector<WinCategoryId> parameter_list;
   parseCommaSeparatedList (parameters, parameter_list);
   // parameter is credit value
   if (parameter_list.size () == 1)
      credit_threshold = parameter_list[0];
   else if (parameter_list.size () == 2)
   {
      upper_limit_specified = true;
      // 1st parameter is lower credit value
      credit_threshold = parameter_list[0];
      // 2nd parameter is upper limit credit value.
      high_credit_threshold = parameter_list[1];
   }
   else
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition processCreditRangeCondition has invalid parameter count."));

   Amount::Extractor win_amount_extractor (win_amount);
   Amount::AmountValue win_value = win_amount_extractor.extract (denomination);

   if (win_value > credit_threshold)
   {
      // if there is no upper limit
      if (!upper_limit_specified)
         match = true;
      else if (win_value <= high_credit_threshold)
         match = true;
      else
         match = false;
   }
   else
      match = false;

   // Get a list of hit win categories.
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionWinThisActGreaterThan (" << parameters << ')' << endl;
   debug << "  parameter count = " << parameter_list.size () << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
      debug << parameter_list [i] << ',';
   debug << endl;
   debug << "Low credit threshold " << credit_threshold << endl;
   if (upper_limit_specified)
      debug << "Upper limit credit threshold " << high_credit_threshold << endl;
   debug << "Win Amount " << win_value << endl;
   if (upper_limit_specified)
      debug << "Upper limit credit threshold " << high_credit_threshold << endl;
   debug << "Match? " << (match ? "Yes" : "No") << endl;

   #endif
   // Return result.
   return match;
}

// Return true if a win is greater than a specified amount.
bool GamePresentation::processConditionWinGreaterThan (String parameters)
{
   // Get the total win for all acts.
   Amount win_amount;
   winDisplayAmount (win_amount);
   // Process the credit range based parameters.
   return processConditionWinCreditRange (parameters, win_amount);
}

// Return true if a win is greater than a specified amount.
bool GamePresentation::processConditionWinGreaterThanTotalBetX (String parameters)
{
   bool match = false;
   bool upper_limit_specified = false;
   uint32 multiplier = 0;
   int32 high_multiplier = 0;

   // Example string is "1,3".
   // Separate the parameter string into a vector of parameters.
   vector<WinCategoryId> parameter_list;
   parseCommaSeparatedList (parameters, parameter_list);
   if (parameter_list.size () == 1)
   {
      // parameter is total bet multiplier
      multiplier = parameter_list[0];
   }
   else if (parameter_list.size () == 2)
   {
      upper_limit_specified = true;
      // 1st parameter is total bet multiplier
      multiplier = parameter_list[0];
      // 2nd parameter is the upper limit total bet multiplier.
      high_multiplier = parameter_list[1];
   }
   else
   {
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition WIN_GREATER_THAN_TOTAL_BET_X has invalid parameter count."));
   }

   // Get the total bet
   Amount total_bet;
   totalBetDisplayAmount (total_bet);
   Amount::Extractor amount_extractor (total_bet);
   Amount::AmountValue total_bet_value = amount_extractor.extract (denomination);

   // Get the total win
   Amount win_amount;
   winDisplayAmount (win_amount);
   Amount::Extractor win_amount_extractor (win_amount);
   Amount::AmountValue win_value = win_amount_extractor.extract (denomination);

   if (win_value > (total_bet_value * multiplier))
   {
      if (!upper_limit_specified)
      {
         // if there is no upper limit
         match = true;
      }
      else if (win_value <= (total_bet_value * high_multiplier))
      {
         match = true;
      }
      else
      {
         match = false;
      }
   }
   else
   {
      match = false;
   }
   // Get a list of hit win categories.
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionWinGreaterThanTotalBetX (" << parameters << ')' << endl;
   debug << "  parameter count = " << parameter_list.size () << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
   {
      debug << parameter_list [i] << ',';
   }
   debug << endl;
   debug << "Total bet: " << total_bet_value << endl;
   debug << "Multiplier " << multiplier << endl;
   if (upper_limit_specified)
   {
      debug << "Upper limit Multiplier " << high_multiplier << endl;
   }
   debug << "Win Amount " << win_value << endl;
   debug << "total bet multiplied " << (total_bet_value * multiplier) << endl;
   if (upper_limit_specified)
   {
      debug << "Upper limit Multiplied " << (total_bet_value * high_multiplier) << endl;
   }
   debug << "Match? " << (match ? "Yes" : "No") << endl;

   #endif
   // Return result.
   return match;
}

// Return true if a win is greater than or equal to a specified amount.
bool GamePresentation::processConditionWinGreaterThanOrEqualToTotalBetX (String parameters)
{
   bool match = false;
   bool upper_limit_specified = false;
   uint32 multiplier = 0;
   int32 high_multiplier = 0;

   // Example string is "1,3".
   // Separate the parameter string into a vector of parameters.
   vector<WinCategoryId> parameter_list;
   parseCommaSeparatedList (parameters, parameter_list);
   if (parameter_list.size () == 1)
   {
      // parameter is total bet multiplier
      multiplier = parameter_list[0];
   }
   else if (parameter_list.size () == 2)
   {
      upper_limit_specified = true;
      // 1st parameter is total bet multiplier
      multiplier = parameter_list[0];
      // 2nd parameter is the upper limit total bet multiplier.
      high_multiplier = parameter_list[1];
   }
   else
   {
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X has invalid parameter count."));
   }

   // Get the total bet
   Amount total_bet;
   totalBetDisplayAmount (total_bet);
   Amount::Extractor amount_extractor (total_bet);
   Amount::AmountValue total_bet_value = amount_extractor.extract (denomination);

   // Get the total win
   Amount win_amount;
   winDisplayAmount (win_amount);
   Amount::Extractor win_amount_extractor (win_amount);
   Amount::AmountValue win_value = win_amount_extractor.extract (denomination);

   if (win_value >= (total_bet_value * multiplier))
   {
      if (!upper_limit_specified)
      {
         // if there is no upper limit
         match = true;
      }
      else if (win_value < (total_bet_value * high_multiplier))
      {
         match = true;
      }
      else
      {
         match = false;
      }
   }
   else
   {
      match = false;
   }
   // Get a list of hit win categories.
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionWinGreaterThanTotalBetX (" << parameters << ')' << endl;
   debug << "  parameter count = " << parameter_list.size () << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
   {
      debug << parameter_list [i] << ',';
   }
   debug << endl;
   debug << "Total bet: " << total_bet_value << endl;
   debug << "Multiplier " << multiplier << endl;
   if (upper_limit_specified)
   {
      debug << "Upper limit Multiplier " << high_multiplier << endl;
   }
   debug << "Win Amount " << win_value << endl;
   debug << "total bet multiplied " << (total_bet_value * multiplier) << endl;
   if (upper_limit_specified)
   {
      debug << "Upper limit Multiplied " << (total_bet_value * high_multiplier) << endl;
   }
   debug << "Match? " << (match ? "Yes" : "No") << endl;

   #endif
   // Return result.
   return match;
}


// Return true if a win is greater than a specified amount.
bool GamePresentation::processConditionWinGreaterThanSubBetXMaxSubBetsX (String parameters)
{
   bool match = false;
   bool upper_limit_specified = false;
   uint32 multiplier = 0;
   int32 high_multiplier = 0;

   // Example string is "1,3".
   // Separate the parameter string into a vector of parameters.
   vector<WinCategoryId> parameter_list;
   parseCommaSeparatedList (parameters, parameter_list);
   if (parameter_list.size () == 1)
   {
      // parameter is total bet multiplier
      multiplier = parameter_list[0];
   }
   else if (parameter_list.size () == 2)
   {
      upper_limit_specified = true;
      // 1st parameter is total bet multiplier
      multiplier = parameter_list[0];
      // 2nd parameter is the upper limit total bet multiplier.
      high_multiplier = parameter_list[1];
   }
   else
   {
      System::error (LogicError (LogicError::INVALID_PARAMETER, "Post/wait condition WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X has invalid parameter count."));
   }
   // Get the sub bet
   Amount sub_bet;
   subBetDisplayAmount (sub_bet);
   Amount::Extractor amount_extractor (sub_bet);
   Amount::AmountValue sub_bet_value = amount_extractor.extract (denomination);

   // Get the number of sub bets
   uint32 number_of_sub_bets;
   numberOfSubBets (number_of_sub_bets);

   // Get the total win
   Amount win_amount;
   winDisplayAmount (win_amount);
   Amount::Extractor win_amount_extractor (win_amount);
   Amount::AmountValue win_value = win_amount_extractor.extract (denomination);

   if (win_value > (sub_bet_value * number_of_sub_bets * multiplier))
   {
      if (!upper_limit_specified)
      {
         // if there is no upper limit
         match = true;
      }
      else if (win_value <= (sub_bet_value * number_of_sub_bets * high_multiplier))
      {
         match = true;
      }
      else
      {
         match = false;
      }
   }
   else
   {
      match = false;
   }
   // Get a list of hit win categories.
   #ifdef __VERBOSE_CONDITIONS__
   debug << "GamePresentation::processConditionWinGreaterThanSubBetXMaxSubBetsX (" << parameters << ')' << endl;
   debug << "  parameter count = " << parameter_list.size () << endl;
   debug << "  parameter_list = ";
   for (uint32 i = 0; i < parameter_list.size (); ++i)
   {
      debug << parameter_list [i] << ',';
   }
   debug << endl;
   debug << "Sub bet: " << sub_bet_value << endl;
   debug << "Number of Sub bets: " << number_of_sub_bets << endl;
   debug << "Multiplier " << multiplier << endl;
   if (upper_limit_specified)
      debug << "Upper limit Multiplier " << high_multiplier << endl;

   debug << "Win Amount " << win_value << endl;
   debug << "total bet multiplied " << (sub_bet_value * number_of_sub_bets * multiplier) << endl;
   if (upper_limit_specified)
   {
      debug << "Upper limit Multiplied " << (sub_bet_value * number_of_sub_bets * high_multiplier) << endl;
   }
   debug << "Match? " << (match ? "Yes" : "No") << endl;

   #endif
   // Return result.
   return match;
}

// Return true if a win is greater than a specified amount.
bool GamePresentation::processConditionWinThisActGreaterThan (String parameters)
{
   // Get the total win for the current act.
   Amount win_amount;
   extractWinForThisAct (win_amount);
   // Process the credit range based parameters.
   return processConditionWinCreditRange (parameters, win_amount);
}

// Handle the sequence event for machine activity
void GamePresentation::processMachineActivity (const SequenceEvent &event)
{
   // Update the game's machine activity status flag.
   const MachineActivitySequenceEvent& activity_event = static_cast<const MachineActivitySequenceEvent&> (event);
   current_game_activity_status = activity_event.isActive ();

   setCreditMeterDisplayBehavior (true);
}

// rollup any double up wins to the credit meter...
void GamePresentation::rollupDoubleUpWin ()
{
   // currently handled on the class level since different classes rollup wins differently
}

// rollup any double up wins to the credit meter...
void GamePresentation::enableDoubleUpListener ()
{
   const EssentialRecord *essential_record = getEssentialRecord ();

   if (essential_record->doubleup_permitted)
   {
      postEvent ("EnableDoubleUpListener");
   }
}

// Does the bet allow us to start a game.
bool GamePresentation::checkForDoubleUpEnable ()
{
   const EssentialRecord *essential_record = getEssentialRecord ();

   #ifdef __VERBOSE__
   debug << "GamePresentation::checkForDoubleUpEnable:" << __FUNCTION__ << endl;
   #endif
   String autoplay_state_value;
   config.getValueString (autoplay_state_config_id, autoplay_state_value);
   return (essential_record->doubleup_permitted && (!essential_record->doubleup_cancel_context)
           && (strcmp (autoplay_state_value, "DISABLED") == 0));

}

// Purpose: Check if double up was triggered
void GamePresentation::checkIfDoubleUpTriggered (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   if (essential_record->doubleupInitiated ())
   {
      // Start DoubleUp
      UserSequenceEvent double_up_start_event (UserSequenceEvent::TO_GAME_MANAGER,
                                               UserSequenceEvent::PLAY_REQUEST,
                                               UserSequenceEvent::START_DOUBLEUP_GAME_REQUEST);
      sequence_event_poster.post (double_up_start_event);

      // Reset doubleup initiated flag
      UserSequenceEvent cancel_double_up_initiated (UserSequenceEvent::TO_FLOW,
                                                    UserSequenceEvent::DOUBLEUP_REQUEST,
                                                    UserSequenceEvent::RESET_DOUBLEUP_AT_IDLE_REQUEST);
      sequence_event_poster.post (cancel_double_up_initiated);
   }
   else if (essential_record->doubleupPermitted ())
   {
      postEvent ("DoubleUp:Enable");
   }
   else
      postEvent ("DoubleUp:Disable");
}

// Purpose:  This method is called by the multimedia event listener controller with which this
// game is registered (the stage in this case).  Use this method to trigger presentation-specific
// operations based upon the receipt of a multimedia event.
bool GamePresentation::processEvent (const class MultimediaEvent& event, class ScriptTableParameter *payload)
{
   // Cycle the quick pick rng
   quick_pick.cycle ();
   #ifdef __VERBOSE_MULTIMEDIA_EVENT__
      #ifdef __VERBOSE_MULTIMEDIA_EVENT_FILTER_CONTROLS__
      if (strncmp (event.getString (), "Button:", 7)
         && strncmp (event.getString (), "Meter:", 6)
         && strncmp (event.getString (), "Paylines.", 9)
         && strncmp (event.getString (), "Game.Main", 9)
         && strncmp (event.getString (), "Stage.Main", 10)
         && strncmp (event.getString (), "ControlPanel", 12)
         )
      #endif
      debug << game_index << ":" << stage_index << "|GamePresentation::processEvent (" << event.getString () << ')' << endl;
   #endif

   if (event == update_system_denomination_event)
   {
      synchronizeDenomination ();
   }
   else if (event == "RequestToEnableDoubleUpButton")
   {
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (essential_record->doubleupPermitted ())
         postEvent ("DoubleUp:Enable");
      else
         postEvent ("DoubleUp:Disable");
   }
   else if (event == update_system_display_timer_event)
   {
      Mojo<PropertyInterface> system_display_timer_property;
      if (true == resource_list->getResource<PropertyInterface> (system_display_timer_property_name, "Default", system_display_timer_property, false))
      {
         uint32 timer_seconds;
         system_display_timer_property->getAbsoluteValue (timer_seconds);
         updateSystemTimer (timer_seconds);
      }
   }
   else if ((event == update_tournament_info_event) || (event == update_tournament_meters_event))
   {
      updateTournamentInfo ();
   }
   // Handlers to enable and disable base game presentation and evaluation.
   else if (strncmp (event.getString (), "BaseGamePresentationEvaluation:Off", strlen ("BaseGamePresentationEvaluation:Off")) == 0)
   {
      setBaseGamePresentationEvaluationFlag (false);
   }
   else if (strncmp (event.getString (), "BaseGamePresentationEvaluation:On", strlen ("BaseGamePresentationEvaluation:On")) == 0)
   {
      setBaseGamePresentationEvaluationFlag (true);
   }
   else if (event == "LanguageSwap:Requested")
   {
      changeGameLanguage ();
   }
   // Listen for base game enable/disable (Bingo) then set the flag.
   else if (strncmp (event.getString (), "BaseGamePresentationEvaluationClearFlag", strlen ("BaseGamePresentationEvaluationClearFlag")) == 0)
   {
      setBaseGamePresentationMeterClearFlag (true);
   }
   // Listen for base game win cycle off, the base game has been disabled/enabled (Bingo).
   else if (strncmp (event.getString (), "BaseGamePresentationEvaluationWinCycle:Off", strlen ("BaseGamePresentationEvaluationWinCycle:Off")) == 0)
   {
      setBaseGamePresentationMeterClearFlag (true);
   }
   // Listen for base game win cycle on. First game is being played since the base game has been disabled/enabled (Bingo).
   else if (strncmp (event.getString (), "BaseGamePresentationEvaluationWinCycle:On", strlen ("BaseGamePresentationEvaluationWinCycle:On")) == 0)
   {
      setBaseGamePresentationMeterClearFlag (false);
   }

   else if (event == "LanguageSwap:EnableControlPanels")
   {
      // Enable control panels here - if not enabled, the sliders will _NOT_ enable, because one of the
      // conditions for allowing the sliders to enable is that the control panel _must be_ enabled prior to enabling the
      // sliders. All the panel buttons, sliders, poker buttons, etc. should use :SynchronizeControlPanels when re-enabling.
      // Only if normal play, if enabled in the operator menu the control panel will be displayed briefly and dynamic buttons
      // will display game bet values.
      if (isNormalPlay ())
      {
         enableControlPanels (true);
      }
   }
   else if (event == "LanguageSwap:SynchronizeControlPanels")
   {
      // Set the state of the volume button
      synchronizeVolumeButton (true);

      // Enable the language button buttons
      enableLanguageButton (true);
      // Enable the autoplay button
      synchronizeAutoplayButtonPresentation ();
   }
   else if (event == "SlotColdStartState:CommonExitSuspend")
   {
      updateAutoplayButtonPresentation (true);
   }
   else if (event == "IGTConnect:ActionRequired" && payload)
   {
      if (payload->script_table.find ("action_required") != payload->script_table.end () && 
          payload->script_table["action_required"].parameterType () == ParameterType::BOOL)
      {
         igt_connect_action_required = payload->script_table["action_required"].asBool ();
      }
   }
   else if (event == "RMLP:MuteMusic")
   {
      rmlp_triggered = true;
   }
   else if (event == "SlotPlayState:Enter")
   {
      // entering a play state means a new game has started, so set everything back to false
      rmlp_triggered = false;
   }
   else if (event == "rmlpFeature:Active")
   {
      // this event get posted from the Feature-00RMLP presentation indicating that the FI is enabled
      rmlp_feature_enabled = true;
   }
   return false;
}

// Purpose: Process multimedia event
bool GamePresentation::processEvent (const class MultimediaEvent& event)
{
   return processEvent (event, 0);
}

// Purpose:  Processes the sequence event received.
bool GamePresentation::processEvent (const SequenceEvent &event)
{
   verboseMethodName ();
   bool handled = true;

   switch (event.getType())
   {
      case SequenceEvent::GENERIC_SEQUENCE_EVENT:
      {
         const GenericSequenceEvent &generic_event = reinterpret_cast<const GenericSequenceEvent &> (event);
         switch (generic_event.genericEventCategory ())
         {
            case GenericSequenceEvent::MONEY:
            {
               const MoneySequenceEvent &money_event = reinterpret_cast<const MoneySequenceEvent &> (event);
               if (money_event.eventType () == MoneySequenceEvent::MONEY_OUT)
               {
                  synchronizeMoneyOut ();
                  postEvent ("GamePresentation:MoneyEvent");
               }
               else if (money_event.eventType () == MoneySequenceEvent::MONEY_IN)
               {
                  synchronizeMoneyIn ();
                  postEvent ("GamePresentation:MoneyEvent");
               }
               else if (money_event.eventType () == MoneySequenceEvent::MONEY_BET)
               {
                  synchronizeMoneyBet ();
                  postEvent ("GamePresentation:MoneyEvent");
               }
               else if (money_event.eventType () == MoneySequenceEvent::MONEY_MOVED)
               {
                  synchronizeMoneyMoved ();
                  postEvent ("GamePresentation:MoneyEvent");
               }
               else
               {
                  synchronizeMeters ();
                  postEvent ("GamePresentation:MoneyEvent");
               }
               handled = true;
            }
            break;

            case GenericSequenceEvent::SYSTEM_METERS_CHANGED:
            {
               synchronizeSystemMetersChanged ();
            }
            break;

            case GenericSequenceEvent::SYNCHRONIZE_STATE:
            {
               const SynchronizeStateSequenceEvent& event_state = static_cast<const SynchronizeStateSequenceEvent&> (event);
               // We need to synchronize our state to the state of the game flow.
               setState (StatePair (event_state.gameFlowState (), event_state.presentationSubstate ()),
                  StatePair (event_state.previousGameFlowState (), event_state.previousPresentationSubstate ()));
            }
            break;

            case GenericSequenceEvent::SYNCHRONIZE_PRESENTATION:
            {
               // The presentation needs to be updated.
               // The event contains the information about which parts of the display
               // need to be updated.
               synchronizePresentation ((const SynchronizePresentationSequenceEvent&) event);
            }
            break;

            case GenericSequenceEvent::MACHINE_ACTIVITY:
            {
               processMachineActivity (event);
            }
            break;

            case GenericSequenceEvent::PRINTER:
            {
               const PrinterSequenceEvent &printer_event = reinterpret_cast<const PrinterSequenceEvent &> (event);
               if (printer_event.eventType () == PrinterSequenceEvent::PAPER_IN)
               {
                  // Post event to the system context
                  postEventAll ("GamePresentation:PrinterPaperInChute");
               }
               else if (printer_event.eventType () == PrinterSequenceEvent::PAPER_OUT)
               {
                  postEventAll ("GamePresentation:PrinterPaperOutChute");
               }
               else  //PrinterSequenceEvent::PRINTED_PAST_BARCODE
               {
                  postEventAll ("GamePresentation:PrinterPrintedPastBarcode");
               }
               handled = true;
            }
            break;

            case GenericSequenceEvent::WIN_MISMATCH:
            {
               // Update the game's machine activity status flag.
               const WinMismatchSequenceEvent& win_mismatch_event = static_cast<const WinMismatchSequenceEvent&> (event);
               win_mismatch_pending = win_mismatch_event.status () == WinMismatchSequenceEvent::MISMATCH_STATUS_PENDING;
            }
            break;

            case GenericSequenceEvent::GAME_FINALIZE_ROLLUP_DOUBLE_UP:
            {
               synchronizeCashout ();
               // Synchronize the visual bet components after double up
               synchronizeBet ();
            }
            break;

            default:
            {
               handled = false;
            }
         }
      }
      break;

      case SequenceEvent::USER_REQUEST:
      {
         const UserSequenceEvent &user_event = reinterpret_cast<const UserSequenceEvent &> (event);
         switch (user_event.getRequestCategory ())
         {
            case UserSequenceEvent::HELP_REQUEST:
            {
               switch (user_event.getRequestType ())
               {
                  case UserSequenceEvent::DISPLAY_HELP_REQUEST:
                  {
                     if (isFlowStateIdle ())
                     {
                        postEvent ("GameDisplayHelp");
                     }
                  }
                  break;

                  default:
                  {
                     handled = false;
                  }
                  break;
               }
            }
            break;

            case UserSequenceEvent::BET_REQUEST:
            {
               switch (user_event.getRequestType ())
               {
                  case UserSequenceEvent::NEW_CARD_REQUEST:
                  {
                     postEventAll ("NewCardRequest");
                  }
                  break;

                  default:
                  {
                     handled = false;
                  }
                  break;
               }
            }
            break;
         }
         break;
      }
      break;

      default:
      {
         handled = false;
      }
   }

   return (handled);
}

// Purpose:  Processes the game request event received.
bool GamePresentation::processGameRequestEvent (const GameRequestEvent &event)
{
   bool handled = false;

   GenericGameRequestEvent generic_event (event);

   switch (generic_event.getRequestType ())
   {
      case GenericGameRequestEvent::IGT_CONNECT_GAME_EVENT:
      case GenericGameRequestEvent::IGT_CONNECT_FRAMEWORK_EVENT:
      
         if (igt_connect.isValid () && stageIndex () == 0)
         {
            if (igt_connect_display.isValid ())
            {
               igt_connect_display->handleGameRequestEvent (generic_event);
            }

            handled = true;
         }
         break;

      default:
         break;
   }

   return handled;
}

// Purpose:  Build an award from the essential record information.  This method builds the game
// presentation's local award list.  This method should only be called by the game presentation.
bool GamePresentation::readAwardFromEssentialRecord (bool force_refresh)
{
   verboseMethodName ();
   bool award_changed = false;
   if (essential_record_award.isAttached ())
   {
      // Refresh our award information.  This is a very important operation.  The size of this block
      // is changing (potentially).  The call to refresh will return true if the award has changed.
      // If it has (or if the award list has not been built from the essential record), we'll read
      // the information.  If no change has occurred we'll bypass this time expensive code.
      award_changed = essential_record_award.refresh (force_refresh);
      #ifdef __VERBOSE_AWARD__
      debug << game_index << ":" << stage_index << "|GamePresentation::readAwardFromEssentialRecord, award_changed = " << (award_changed ? "Yes" : "No" )<< endl;
      debug << " -full_name=" << essential_record_award.getFullName () << endl;
      #endif
      // Create an empty award object if it doesn't exist already.
      if (!award_list.isValid () && award_so_file_name.length () && award_object_name.length ())
      {
         if (!createObject<AwardInterface> (award_so_file_name, award_object_name, award_list))
         {
            String error_message = String ("Unable to create award object ") + award_object_name + String (" from SO ") + award_so_file_name;
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
         award_changed = true;
      }
      // If we need to re-initialize the Award because the essential record has changed...
      if (award_changed)
      {
         if (award_list.isValid ())
            award_list->clear ();

         // In the cold power up case we could have an essential record that is 1 byte in size.
         // Check for the case when the award safe store size is less than the uint32 stored
         // in the header of the safe store
         if (essential_record_award.getSize () > 0)
         {
            // Only create the Award object if needed.
            #ifdef __VERBOSE_AWARD2__
            debug << "Presentation award =";
            uint8* data = (uint8*)essential_record_award.getMemory ();
            for (int x = 0; x < essential_record_award.getSize(); x++)
               debug << hex << (int)data[x] << " ";
            debug << dec << endl;
            #endif

            size_t fixed_size = 0;

            const uint8* award_buffer = reinterpret_cast<const uint8 *> (essential_record_award.getMemory());
            fixed_size = essential_record_award.getSize ();

            if (fixed_size)
            {
               #ifdef __VERBOSE_AWARD__
               debug << "Presentation has an award, fixed_size =" << fixed_size << endl;
               #endif

               award_list->createFromBuffer (award_buffer, fixed_size);
               award_list->verifyCRC ();

               #ifdef __VERBOSE_AWARD__
               if (award_list.isValid ())
                  award_list->displayAward ();
               #endif
            }
         }
      }
   }
   return award_changed;
}

// Purpose:  Build and award from the essential record information.  Update the award interface
// received with the current award data.  Return true if the award information has changed since
// the last time the information was requested.
bool GamePresentation::readAwardFromEssentialRecord (Mojo<AwardInterface> &award)
{
   verboseMethodName ();
   #ifdef __VERBOSE_AWARD__
   debug << game_index << ":" << stage_index << "|GamePresentation::readAwardFromEssentialRecord ()" << endl;
   #endif
   // The following call will reconstruct the local award_list member if the critical
   // data is different than what's held.  This optimization will ensure that we
   // do not read unchanged data multiple times as the read from NVRAM is expensive.
   bool award_changed = readAwardFromEssentialRecord ();
   // Update the mojo received with the current award information.
   award = award_list;
   // Give the callers a chance to opt out of a presentation update if the information
   // stored in the award list has not changed.
   return award_changed;
}

// Purpose:
void GamePresentation::readConfigurationFile (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::readConfigurationFile () filename = " << configuration_filename << endl;
   #endif

   // Open the configuration file
   Registry registry;
   registry.open (configuration_filename);

   // Call the virtual function to read the information from the configuration
   // file.  This is the hook for games to read anything special they might
   // have in the configuration file.
   parseConfigurationFile (registry);
   registry.close();
}

// The following method assigns the new control panel to the game-specific control panel mojo as is
// appropriate for the control panel type name received.
bool GamePresentation::registerControlPanel (const char *type, const char *alias)
{
   verboseMethodName ();
   #ifdef __VERBOSE__
   debug << "GamePresentation::registerControlPanel, type:" << type << endl;
   #endif
   // Initialize the return value.
   bool registered = false;
   if (type && strlen (type) != 0 && alias && strlen (alias) != 0)
   {
      // Determine which of this game's known control panels is being registered.  Store the alias
      // name appropriately.
      if (strcmp (type, GAME_CONTROL_PANEL) == 0)
      {
         game_control_panel = alias;
         registered = true;
      }
      else if  (strcmp (type, GAME_COUNT_CONTROL_PANEL) == 0)
      {
         game_count_control_panel = alias;
         registered = true;
      }
   }
   return registered;
}

// The following method assigns the stage alias name and prepares the stage for use by the game.
bool GamePresentation::registerHelpDisplayStage (const char *alias)
{
   help_display_stage_alias = alias;
   return help_display_stage_alias.length () != 0;
}

// The following method assigns the stage name for the game presentation.
bool GamePresentation::registerStage (const char *stage_name)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::registerStage, name = " << stage_name << endl;
   #endif
   // Initialize the return value.
   bool registered = false;
   // Verify that a valid stage name has been received.
   if (stage_name && strlen (stage_name) != 0)
   {
      // Assign the stage alias name.
      this->stage_name = stage_name;

      // Allow the presentation to hear stage events.
      Mojo<ActorStageInterface> actor_stage_interface;
      getStage (stage_name, actor_stage_interface, true);
      Actuable::setStage (actor_stage_interface);

      // Update the return value.
      registered = true;
   }
   return registered;
}

// The following method assigns the stage alias name and prepares the stage for use by the game.
bool GamePresentation::registerPaytableDisplayStage (const char *alias)
{
   paytable_display_stage_alias = alias;
   return paytable_display_stage_alias.length () != 0;
}

// Purpose: Release all the PresistentProperty objects from critical data storage.
void GamePresentation::releasePersistentProperties (void)
{
   // Clear list, create new one.
   if (persistent_property_list.isValid ())
   {
      persistent_property_list.release ();
      persistent_property_list.create <PersistentPropertyList> ();
   }
}

// Purpose: Reload all the PresistentProperty objects from critical data storage.
void GamePresentation::reloadPersistentProperties (void)
{
   verboseMethodName();
   if (persistent_property_list.isValid ())
   {
      persistent_property_list->setDefaultStorageDirectory (essential_persistent_properties_directory);
      persistent_property_list->setAllStorageDirectories (essential_persistent_properties_directory);
      persistent_property_list->reloadAll ();
   }
}

// reset the relative volume to the default
void GamePresentation::resetVolume (void)
{
   #ifdef __VERBOSE_VOLUME__
   debug << "GamePresentation::resetVolume ()" << endl;
   #endif
   // get the volume config
   ConfigID config_id = config.getId (CONFIG_ID_VOLUME_GAME_SOUNDS_RELATIVE);
   // reset the config value
   config.setValueFloat (config_id, VOLUME_LEVEL_DEFAULT);
   // set the property
   setVolumeProperty ();
}

// Purpose: Enable/disable all persistent properties ablities to write to safe storage.
void GamePresentation::enablePersistentPropertiesSaveToStorage (bool enable)
{
   verboseMethodName();
   if (persistent_property_list.isValid ())
      persistent_property_list->enableSaveToStorage (enable);
}

// Purpose:  Removes the listener from the main_stage's listener list.  Returns true on success.
bool GamePresentation::removeEventListener (Mojo<MultimediaEventListenerInterface> listener)
{
   // unregister the stage with our event listener generically
   bool success = false;
   if (listener.isValid ())
   {
      listener->removeListener ();
      success = true;
   }
   return success;
}

// Purpose:  Removes the listener received from the sequence event listener list.  Returns
// true if the listener was removed successfully.
bool GamePresentation::removeEventListener (Mojo<SequenceEventListenerInterface> listener)
{
   // Initialize the return value.
   bool success = false;
   // If a valid listener has been received and if the sequence event listener controller
   // has been created, attempt to remove the listener from the controller.
   if (listener.isValid () && sequence_event_listener_controller.isValid ())
      success = sequence_event_listener_controller->remove (listener);
   return success;
}

// Purpose: Remove the specified object from the lists
void GamePresentation::removeFromLists (UnknownInterface *object)
{
   // Simply iterate through the list of interface and find one that matches
   ConfigurationChangedIterator end (configuration_changed_list.end ());
   for (ConfigurationChangedIterator iterator (configuration_changed_list.begin ()); iterator != end; ++iterator)
   {
      Mojo<ConfigurationChangedHandlerInterface> interface = object;
      if (interface == *iterator)
      {
         configuration_changed_list.erase (iterator);
         break;
      }
   }
}

// Purpose: Request the presentation substate of the current flow state be changed.
//          Returns false if the request cannot be handled.
//          Returns true if the request will be attempted.
bool GamePresentation::requestPresentationSubstate (StateId substate)
{
   verboseMethodName();
   #ifdef __VERBOSE_STATE__
   debug << game_index << ":" << stage_index << "|GamePresentation::requestPresentationSubstate (substate: " << substate << ')' << endl;
   #endif
   bool success = false;
   // If we already requested this...
   if (awaiting_substate_change
      && (last_requested_flow_state_id == getFlowState ())
      && (last_requested_presentation_state_id == substate))
   {
      // Ignore it.
      #ifdef __VERBOSE_STATE__
         String msg ("Duplicate state request ignored: ", 128);

         // Get the name of the substate.
         String name;
         state_machine->find (StatePair (getFlowState (), substate), name);

         // Build and post the warning.
         msg << whoAreYou () << ", " << GameStateValue::stateName (getFlowState ()) << '-' << name;
         System::log (LogError (msg));
      #endif
   }
   // Request the new state...
   else
   {
      // Post sequence event to request this substate from the GameFlow.
      PresentationSubstateCompleteSequenceEvent state_event (getFlowState (), substate, SequenceEvent::TO_FLOW);
      postEvent (state_event);
      // Remember the state request.
      last_requested_flow_state_id = getFlowState ();
      last_requested_presentation_state_id = substate;
      awaiting_substate_change = true;
      // Flag request was made.
      success = true;
   }
   return success;
}

// Purpose: Request the presentation substate of the current flow state be changed.
//          Returns false if the request cannot be handled.
//          Returns true if the request will be attempted.
bool GamePresentation::requestPresentationSubstate (const char *substate_name)
{
   verboseMethodName();
   #ifdef __VERBOSE_STATE__
   debug << game_index << ":" << stage_index << "|GamePresentation::requestPresentationSubstate (" << substate_name << ')' << endl;
   #endif
   bool success = false;
   StatePair state;
   // Check if the substate string is valid.
   if (!substate_name)
   {
      setErrorString (LogicError (LogicError::INVALID_PARAMETER, "GamePresentation::requestPresentationSubstate: Null substate name passed in.").errorMessage (), whoAreYou ());
   }

   // Check if the substate name is a valid state in the state machine.
   if (!state_machine->find (substate_name, state))
   {
      String error_message ("GamePresentation::requestPresentationSubstate: Substate name \"");
      error_message << substate_name << "\" does not exist in the state machine.";
      setErrorString (LogicError (LogicError::INVALID_PARAMETER, error_message).errorMessage (), whoAreYou ());
   }

   // If this request is for the current flow state...
   if (state.flowState () == getFlowState ())
      // Ask GameManager to set this substate on GamePresentation.
      success = requestPresentationSubstate (state.presentationSubstate ());
   return success;
}

// Purpose: Request the presentation substate of the current flow state be changed
void GamePresentation::requestReturnToPreviousPresentationSubstate (void)
{
   verboseMethodName();
   requestPresentationSubstate (getPreviousPresentationSubstate ());
}

// Purpose: Returns the transaction id to use for games evaluations
bool GamePresentation::resourceList (Mojo<ResourceList, ScriptInterface> &resource_list) const
{
   resource_list = this->resource_list;
   return resource_list.isValid ();
}

/// <summary>posts an event to tell the event-driven control panel presentation
///    to perform the specified action.</summary>
/// <remarks>The event string is the resource alias followed by ":Client" (because
///    this is a command for the client).</remarks>
/// <param name="payload">[in] The payload to post with the event.</param>
/// <param name="delay">[in] The optional delay in ms before posting the event.</param>
/// <returns>true if the event was successfully posted, false otherwise</returns>
bool GamePresentation::postPresentationAction (ScriptTableParameter & payload)
{
   String event_string ("GamePresentation");
   event_string << ":Client";
   return postEventExtended (event_string.c_str (), 0, &payload);
}

/// <summary>Adds an action and the associated data to the passed payload.</summary>
/// <param name = "payload">[in/out] The payload to add the action to.</param>
/// <param name = "action">[in] The name of the action.</param>
/// <param name = "action_data">[in] An optional table containing the parameters for this action.</param>
void GamePresentation::addPresentationAction (ScriptTableParameter & payload, const char * action, ScriptTableParameter * action_data)
{
   if (action)
   {
      // check if the payload has an action table already
      if (payload.script_table.count ("actions") == 0)
      {
         ScriptTableParameter new_actions_table;
         // add a new, empty actions table.
         payload.script_table ["actions"] = ScriptParameter (new_actions_table, ParameterType::TABLE);
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

/// <summary>Get accounting ER values and bundle them up in a payload.</summary>
/// <remarks>All values that represent the current award information are
///             bundled up into a payload and returned to the calling function.</remarks>
/// <param name="event_post_name">[in] passed in payload to be filled</param>
void GamePresentation::getAccountingData (ScriptTableParameter & payload)
{
   Amount amount_in_bank (ZERO_AMOUNT);
   Amount amount_in_award (ZERO_AMOUNT);
   Amount amount_in_total_bet (ZERO_AMOUNT);
   Amount initial_win (ZERO_AMOUNT);
   Amount base_game_win (ZERO_AMOUNT);
   uint64 credits_in_bank = 0;
   uint64 credits_in_award = 0;
   uint64 total_bet_credits = 0;
   uint64 initial_win_credits = 0;
   uint64 base_game_win_credits = 0;

   const EssentialRecord *essential_record = getEssentialRecord ();
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   // credits before game play
   creditDisplayAmount (amount_in_bank);
   // awarded credits
   extractWinForAllActs (amount_in_award);
   // total bet
   amount_in_total_bet = essential_record->totalBetAtStageStart ();
   // base game wins
   local_award->totalDisplayableAmount (base_game_win, BASE_GAME_ACT, BASE_GAME_ACT);
   // all wins up to current act
   if (actIndex () > BASE_GAME_ACT)
   {
      local_award->totalDisplayableAmount (initial_win, BASE_GAME_ACT, actIndex () - 1);
   }

   // Have to send Credit Values since Lua only handles size double
   Amount::Extractor amount_extractor1 (amount_in_bank);
   credits_in_bank = amount_extractor1.extract (Denomination::BASE_UNIT);

   Amount::Extractor amount_extractor2 (amount_in_award);
   credits_in_award = amount_extractor2.extract (Denomination::BASE_UNIT);

   Amount::Extractor amount_extractor3 (amount_in_total_bet);
   total_bet_credits = amount_extractor3.extract (Denomination::BASE_UNIT);

   Amount::Extractor amount_extractor4 (initial_win);
   initial_win_credits =  amount_extractor4.extract (Denomination::BASE_UNIT);

   Amount::Extractor amount_extractor5 (base_game_win);
   base_game_win_credits =  amount_extractor5.extract (Denomination::BASE_UNIT);

   payload.script_table ["total_win"]                = ScriptParameter (credits_in_award, ParameterType::UINT64);
   payload.script_table ["starting_credits"]         = ScriptParameter (credits_in_bank, ParameterType::UINT64);
   payload.script_table ["wins_to_current_act"]      = ScriptParameter (initial_win_credits, ParameterType::UINT64);
   payload.script_table ["base_game_win"]            = ScriptParameter (base_game_win_credits, ParameterType::UINT64);
   payload.script_table ["total_bet_at_stage_start"] = ScriptParameter (total_bet_credits, ParameterType::UINT64);
}

/// <summary>Get the state machine values and bundle them up in a payload.</summary>
/// <remarks>All values that represent the current state machine information are
///             bundled up into a payload and posted.</remarks>
/// <param name="event_post_name">[in] passed in payload to be filled</param>
void GamePresentation::createStateInfoPayload (ScriptTableParameter & payload)
{
   ScriptTableParameter update_table;
   ScriptTableParameter attribute_table;
   String _presentation_state = "";

   if (state_machine.isValid ())
   {
      state_machine->find (state_machine->getCurrentStateId (), _presentation_state);
   }

   // Create a payload in the "Lite" format and post it to the passed in event.
   // payload = {
   //               actions = {
   //                           "1" = {
   //                                   action     = update,
   //                                   parameters = {
   //                                                  CurrentStateData = {
   //                                                                         presentation_substate = STRING,
   //                                                                         flow_state            = STRING,
   //                                                                     },
   //                                                },
   //                                 },
   //                         },
   //           }

   // state machine data is placed in the attribute
   attribute_table.script_table["presentation_substate"] = ScriptParameter (_presentation_state, ParameterType::STRING);
   attribute_table.script_table["flow_state"]            = ScriptParameter (String (GameStateValue::stateName (getFlowState ())), ParameterType::STRING);
   // attribute data is placed in the update table.
   update_table.script_table ["CurrentStateData"] = ScriptParameter (attribute_table, ParameterType::TABLE);
   GamePresentation::addPresentationAction (payload, "update", &update_table);
}

// Starts the credit rollup.
void GamePresentation::startRollup (void)
{
}

// Purpose:  Set the award configuration.
bool GamePresentation::setAwardInformation (const char *so_filename, const char *object_name)
{
   verboseMethodName();
   bool success = false;

   if (so_filename && object_name)
   {
      // Record the award object name
      award_object_name = object_name;
      // Record the file name of the SO that will create the award object.
      award_so_file_name = so_filename;
      success = true;
   }
   // Read any award in safe storeage.  Force a refresh.
   readAwardFromEssentialRecord (true);

   return success;
}

// Purpose: Load the betting shared object.
bool GamePresentation::setBetInformation (const char *so_filename, const char *object_name)
{
   verboseMethodName();
   // Note: If you decide to support changing the bet object on the fly by recreating, you need
   //       to support re-executing the bet script since after the release
   //       it will be invalid.  The bet script is specified by the main
   //       script.
   if (so_filename && object_name)
   {
      if (!bet.isValid ())
      {
         // Remove this existing bet from the lists of interfaces
         if (doesBetSupportGameSideBetInterface ())
         {
            removeFromLists (bet);
         }
         // Release any existing bet
         bet.release ();
         // Create the bet object
         if (!createObject<BetInterface> (so_filename, object_name, bet))
         {
            String error_message = String ("Unable to load bet shared object:") + String(so_filename);
            System::error (LogicError (LogicError::INVALID_VALUE, error_message));
         }
         Mojo <RuleBasedGameSideBetInterface> rule_game_side_bet (bet);
         if (rule_game_side_bet.isValid ())
         {
            char *duplicated_so_file_name = strdup (so_filename);
            rule_game_side_bet->setMountPoint (dirname (duplicated_so_file_name));
            free (duplicated_so_file_name);

            // Tell bet object it's configuration file's mount point.
            rule_game_side_bet->setConfigurationFileMountPoint (resource_mount_point.c_str ());
         }
         // Initialize the bet first
         bet->init (theme_configuration_identifier, stage_configuration_identifier, gameIdentifier ().asString (), stageIndex (), actIndex ());
         // Set the betting mode
         bet->setBetMode (betModeForGameMode (current_game_mode));
         if (!doesBetSupportGameSideBetInterface ())
         {
            loadBetConfiguration ();
         }
         // Attach/create the safe storage for the bet meter.
         bet->attach (essential_bet_filename);
         // Set the betting denomination
         bet->setDenomination (denomination);

         if (!rule_game_side_bet.isValid ())
         {
            // It is old Game side bet interface.
            // NOTE: the new Rule-based bet needn't add to config_changed_list in the Presentation side.
            addToLists (bet);
         }
      }

      // If the resource list is valid...
      Mojo<ResourceList, ScriptInterface> resource_list;
      if (resourceList (resource_list))
      {
         Mojo<AutoplayInterface> autoplay_interface;
         if (getResource (autoplay_alias_name, resource_list->resourceKey (), autoplay_interface, false))
         {
            if (autoplay_interface.isValid () && bet.isValid ())
               autoplay_interface->setBet (bet, game_identifier.asString (), stage_index);
         }
         Mojo<DoubleUpInterface> double_up_interface;
         if (getResource (double_up_alias_name, resource_list->resourceKey (), double_up_interface, false))
         {
            if (double_up_interface.isValid () && bet.isValid ())
               double_up_interface->setBet (bet, game_identifier.asString (), stage_index);
         }
      }
   }
   return true;
}

// Purpose:  Configures the configuration file for this presentation.
bool GamePresentation::setConfigurationFile (const char *filename)
{
   configuration_filename = filename;
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setConfigurationFile () = " << configuration_filename << endl;
   #endif
   return true;
}

// Purpose:
bool GamePresentation::setConfigurationIdentifier (const char *game_identifier, const char *theme_identifier, const char *stage_identifier)
{
   game_configuration_identifier = game_identifier;
   theme_configuration_identifier = theme_identifier;
   stage_configuration_identifier = stage_identifier;

   language_licensed_bitfield_config_id = config.getId ("LICENSED LANGUAGE", theme_configuration_identifier, false);
   language_supported_list_config_id = config.getId ("LICENSED LANGUAGE TYPES SUPPORTED LIST", theme_configuration_identifier, false);

   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setConfigurationIdentifier() = " << endl;
   debug << "Game Configuration Identifier : " << game_configuration_identifier << endl;
   debug << "Theme Configuration Identifier: " << theme_configuration_identifier << endl;
   debug << "Stage Configuration Identifier: " << stage_configuration_identifier << endl;
   #endif

   return true;
}

// Purpose: Called to configure the denomination of all known control panels.
void GamePresentation::setControlPanelDenomination (const Denomination& denomination)
{
   verboseMethodName();
   // Retrieve the game control panel and update the denomination if the control panel exists in this game.
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
   {
      game_control_panel.getControlPanelInterface ()->setDenomination (denomination);
      String formatted_denomination;
      formatDenomination (denomination, formatted_denomination);
      game_control_panel.getGameControlPanelInterface ()->setDenomination (formatted_denomination);
   }
   // Retrieve the game count control panel and update the denomination if the control panel exists in this game.
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
      game_count_control_panel.getControlPanelInterface ()->setDenomination (denomination);

   // Synchronize the bet here so that the bet meter doesn't jump between values
   synchronizeBet ();
}

// Purpose:  Set the current context mode.
bool GamePresentation::setCurrentContextMode (const ContextMode &new_mode)
{
   verboseMethodName();
   current_context_mode = new_mode;
   return true;
}

// Purpose:  Set the current mode for this game context.  These are modes specific to the game context.
// Refer to GameDefs.hpp for valid modes.
bool GamePresentation::setCurrentGameMode (const GameMode &new_mode)
{
   verboseMethodName();
   current_game_mode = new_mode;
   return true;
}

// Returns the index of the currently configured game set.
void GamePresentation::setCurrentGameSetIndex (uint32 game_set_index)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::setCurrentGameSetIndex, index: " << game_set_index << endl;
   #endif
   current_game_set_index = game_set_index;
   String event_string ("CurrentGameSet:");
   event_string << current_game_set_index;
   #ifdef __VERBOSE__
   debug << "GamePresentation::setCurrentGameSetIndex, posting event: " << event_string << endl;
   #endif
   postEvent (event_string);
}

// Purpose: Set the current paytable section
bool GamePresentation::setCurrentPaytableSection (const char *section)
{
   verboseMethodName();
   #ifdef __VERBOSE_PAYTABLE__
   debug << gameStage () << "|" << __FUNCTION__ << ":" << String (section) << endl;
   #endif
   // Initialize the return value.
   bool success = true;
   // Do not bother changing sections if this is the same section currently loaded.
   if (section && (current_paytable_section != section))
   {
      // Store the new section name.
      current_paytable_section = section;
      // Make the active evalutor the evaluator mapped to the current section name.
      if (!evaluatorForPaytableSection (evaluator, section))
      {
         String error_message ("GamePresentation::setCurrentPaytableSection (): no evalutor for section: ");
         error_message << section << ". Use supportPaytableSection.";
         System::error (LogicError (LogicError::INVALID_VALUE, error_message));
      }
      // Post an event that can be used by presentation components to update their displays based on the new seciton.
      String event_string ("PaytableSection:");
      event_string << section;
      postEvent (event_string);
   }
   return success;
}

// Purpose: The the denomination for showing meters values, betting and playing the game
//          Returns true on success
bool GamePresentation::setDenomination (const Denomination& denomination)
{
   verboseMethodName();
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setDenomination" << endl;
   #endif
   bool has_denomination_changed = this->denomination.getValue() != Denomination::INVALID
      && this->denomination != denomination;
   this->denomination = denomination;

   // Set the betting denomination
   if (bet.isValid())
   {
      bet->setDenomination (denomination);
      updateMaxBetProperty ();

      if (enrollmentRequired ())
      {
         updatePaytableAwardProperties (true);
         postEvent ("DigitalGlassPaytable:Synchronize");
      }

      // Let the lua change the button map and sync bet data for rule based game bet
      Mojo <RuleBasedGameSideBetInterface> game_side_bet (bet);
      if (game_side_bet.isValid ())
      {
         TransientState transient_state ("/presentation/betsync");
         game_side_bet->syncBetWithCriticalData (transient_state);
         ++transient_state;
      }
   }
   // Configure the denomination of all controls panels supported by this game.
   setControlPanelDenomination (denomination);
   if (has_denomination_changed)
   {
      // The win meter is cleared here to prevent the value from fluctuating before finally zeroing out
      clearWinMeter ();
   }

   return true;
}

// Purpose: Set the value of the double tap flag. Set to true when a CDS game has hit the spin button twice.
void GamePresentation::setEnrollmentButtonDoubleTap (bool double_tapped)
{
   enrollment_button_double_tap = double_tapped;
}

// Purpose: Tell the game where its essential data should be stored.
//          This is the information required for game history.
//          The directory is the full path of the directory where the information is stored.
//          essential_record is the name used for the essential record in the directory
//          bet is the name used for the bet in the directory
//          award is the name used for the award in the directory
//          Returns true on success
bool GamePresentation::setEssentialRecordInformation (const char *directory, const char *shared_directory,
   const char *essential_record_name, const char *bet_name, const char *award)
{
   verboseMethodName();
   assertion (directory && shared_directory && essential_record_name && bet_name && award);

   // Record the path information
   essential_record_directory = directory;
   essential_record_shared_directory = shared_directory;
   essential_award_filename = essential_record_directory + String (award);
   essential_record_filename = essential_record_directory + String (essential_record_name);
   shared_essential_record_filename = essential_record_shared_directory + String (essential_record_name)+"_Shared";
   essential_bet_filename = essential_record_shared_directory + String (bet_name);
   essential_persistent_properties_directory = directory;

   #ifdef __VERBOSE__
      debug << __PRETTY_FUNCTION__ << endl;
      debug << " -directory=" << directory << endl;
      debug << " -shared_directory=" << shared_directory << endl;
      debug << " -essential_record_name=" << essential_record_name << endl;
      debug << " -bet_name=" << bet_name << endl;
      debug << " -essential_record_filename: " << essential_record_filename << endl;
      debug << " -shared_essential_record_filename: " << shared_essential_record_filename << endl;
      debug << " -essential_bet_filename: " << essential_bet_filename << endl;
      debug << " -award=" << award << endl;
   #endif

   // Attach/Create critical data for the award
   essential_record_award.attach (essential_award_filename, /*sizeof (uint32)*/0, SafeStoreFlags::SIZE_IS_DEFAULT);

   // Attach/Create the critical data for the essential record using a default size (actual size is set by flow).
   essential_record.attach (essential_record_filename, 0 /* storage size */, SafeStoreFlags::SIZE_IS_DEFAULT);
   assertion (essential_record.getSize () > 0); // The flow should have this initalized before setEssentialRecordInformation () is called.

   // Attach/Create the critical data for the essential record using a default size (actual size is set by flow).
   shared_essential_record.attach (shared_essential_record_filename, 0 /* storage size */, SafeStoreFlags::SIZE_IS_DEFAULT);
   assertion (shared_essential_record.getSize () > 0); // The flow should have this initalized before setEssentialRecordInformation () is called.

   // Read any award in safe storage.
   if (award_so_file_name.length () && award_object_name.length ())
      // Force the award list to be updated to correspond with the new information.
      readAwardFromEssentialRecord (true);
   // Attach/Create the safe storage for the bet meter.
   if (bet.isValid ())
      bet->attach (essential_bet_filename);


   ///////////////////////////////////////////////////////////
   // Handle safestorage that is local to the game presentation.

   // Attach to the non-volatile information
   safe_store_game_presentation.attach (String (directory) + String (SAFE_STORE_NAME_PRESENTATION_INFORMATION), 1, SafeStoreFlags::SIZE_IS_DEFAULT);
   // Determine how much safe storage is required to store our information
   uint32 safe_store_size = sizeof (GamePresentationSafeStorage);

   // If the current block is too small, then it needs to be resized and initalized.
   if (safe_store_game_presentation.getSize () < safe_store_size)
   {
      safe_store_game_presentation.resize (safe_store_size);
      GamePresentationSafeStorage *game_presentation_safe_storage = reinterpret_cast<GamePresentationSafeStorage *> (safe_store_game_presentation.getMemory ());
      assertion (game_presentation_safe_storage);

      // Initalize data members (non-initalized data members default to all zeros).
      game_presentation_safe_storage->warm_boot_index = WARM_BOOT_RESET_SEQUENCE_ID;

      // Commit the values selected to safe storage.
      safe_store_game_presentation.flush ();
   }
   // Reload the persistent properties. Reloading is required to support history.
   reloadPersistentProperties ();

   return true;
}


// Purpose: Set the current evaluation configuration information.
bool GamePresentation::setEvalInformation (const char *so_filename, const char *object_name)
{
   bool success = false;

   if (so_filename && object_name)
   {
      // Record the evaluator object name
      evaluator_object_name = object_name;

      // Record the file name of the SO that will create the evaluator object.
      evaluator_so_file_name = so_filename;

      success = true;
   }
   return success;
}

// Purpose: Set the container
void GamePresentation::setExtensionContainer (class PresentationExtensionContainerInterface *container)
{
   presentation_extension_container = container;
}

// Purpose: Set the style of game enrollment to use. See GamePresentationSetGameEnrollmentInformation message for valid styles.
bool GamePresentation::setGameEnrollmentInformation (bool enrollment_required, bool enrollment_before_game_start)
{
   this->enrollment_required = enrollment_required;
   this->enrollment_before_game_start = enrollment_before_game_start;
   return true;
}

// Purpose: Inform the game about receiving central determination RNG seeds.
bool GamePresentation::setGameEnrollmentSeedsArrived (void)
{
   return true;
}

// Purpose: Provide the game and stage index the game should be using.
bool GamePresentation::setGameIndex (GameIndex game_index, StageIndex stage_index, ActIndex act_index, const GameIdentifier &game_identifier)
{
   this->game_index = game_index;
   this->stage_index = stage_index;
   this->act_index = act_index;
   this->game_identifier = game_identifier;

   // If we have loaded the IGT Connect feature, configure its game identifier for
   //   effective posting of Game Request Events.
   if (igt_connect.isValid () && stageIndex () == BASE_GAME_STAGE)
   {
      igt_connect->setGameIdentifier (gameIdentifier ());
   }

   // Have our sequence event poster use these values when posting.
   sequence_event_poster.setGameStage (GameStage (game_identifier, stage_index));

   return true;
}

// Purpose: Give a game stage the infomration about the stage and act that initiated it.
//          Returns true on success
bool GamePresentation::setInitiatingInformation (StageIndex initiating_stage, ActIndex initiating_act_index)
{
   initiating.stage = initiating_stage;
   initiating.act = initiating_act_index;
   return true;
}

// Purpose: Set the the current menu option in a property
void GamePresentation::setMenuProperty (void)
{
   verboseMethodName();
   #ifdef __VERBOSE_MENU__
   debug << "GamePresentation::setMenuProperty ()" << endl;
   #endif
   Mojo<ResourceList, ScriptInterface> resource_list;
   Mojo<PropertyInterface> property;
   assertion (resourceList (resource_list));

   // Extract the property associated with the bet indicator from the resource list.  Update the value if
   // the property has been configured.  Do not error if the resource does not exist.
   if (resource_list->getResource<PropertyInterface> (PLAYER_MENU_OPTION, resource_list->resourceKey (), property, false))
   {
      if (numberOfAvailablePlayerGames () > 1)
      {
         #ifdef __VERBOSE_MENU__
         debug << "Available games: " << available_games << ".  Setting Menu Button to Main Menu" << endl;
         #endif
         String property_value (MENU_OPTION_MAIN_MENU);
         property->setAbsoluteValue (property_value);
      }
      else if (numberOfAvailablePlayerDenominations () > 1)
      {
         #ifdef __VERBOSE_MENU__
         debug << "Available denominations: " << available_denominations << ".  Setting Menu Button to Select Value" << endl;
         #endif
         String property_value (MENU_OPTION_SELECT_VALUE);
         property->setAbsoluteValue (property_value);
      }
      else
      {
         #ifdef __VERBOSE_MENU__
         debug << "No other available games nor denominations.  Setting Menu Button to One Credit Equals" << endl;
         #endif
         String property_value (MENU_OPTION_ONE_CREDIT_EQUALS);
         property->setAbsoluteValue (property_value);
      }
      postEvent ("UpdatePlayerMenuOptionDisplay");
   }
}

// Purpose:  Configures the mount point for the resources
bool GamePresentation::setMountPoint (const char *new_mount_point)
{
   resource_mount_point = new_mount_point;
   #ifdef __VERBOSE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setMountPoint () = " << resource_mount_point << endl;
   #endif
   return true;
}

// Purpose:
bool GamePresentation::setPaytableFilename (const char *filename)
{
   paytable_filename = filename;

   #ifdef __VERBOSE_PAYTABLE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setPaytableFilename() filename=" << paytable_filename << endl;
   #endif

   return true;
}

// Purpose: Set the last applied context mode.
bool GamePresentation::setPreviousContextMode (const ContextMode &new_mode)
{
   previous_context_mode = new_mode;
   return true;
}

// Purpose:  Configures the safe store directory for this presentation.
bool GamePresentation::setSafeStoreDirectory (const char *name)
{
   verboseMethodName();
   bool result = true;
   if (name && strlen (name) != 0)
   {
      #ifdef __VERBOSE__
      debug << game_index << ":" << stage_index << "|GamePresentation::setSafeStoreDirectory ()=\"" << name << "\"" << endl;
      #endif
      safe_store_directory_name = name;
      // Game safe storage directory
      safe_store_directory.attach (safe_store_directory_name, SAFE_STORAGE_DIRECTORY_SIZE);
      enablePersistentPropertiesSaveToStorage (true);   // Enable ability of persistent properties to write to safe storage.

      String state_name = safe_store_directory.getFullName () + String ("/GameFlowState");
      read_only_game_state.attach (state_name, 0);

      // We don't want to attach to the bet object in this function because the path to the bet object hasn't been updated yet.
      // The bet object path is set in setEssentialRecordInformation(), which should always be called immediately after this function.
      // setEssentialRecordInformation() also attaches to the bet object - we don't lose anything by not attaching here.
   }
   else
   {
      // The safe storage directory is not longer valid so detach the safe storage/critical data items.
      safe_store_directory.detach ();
      safe_store_directory_name = "";

      essential_record.detach ();
      essential_record_award.detach ();
      safe_store_game_presentation.detach ();
      enablePersistentPropertiesSaveToStorage (false);   // Disable ability of persistent properties to write to safe storage.
      read_only_game_state.detach ();

      if (bet.isValid ())
         bet->attach(0); // Detach the bet object.
   }
   return result;
}

// Purpose: Set the state.
void GamePresentation::setState (StatePair new_presentation_state, StatePair new_previous_presentation_state)
{
   verboseMethodName ();

   // If if the game mode is valid, allow state changes.
   if (currentGameMode () != GAME_MODE_UNKNOWN)
   {
      #ifdef __VERBOSE_STATE__
      debug << game_index << ":" << stage_index << "|GamePresentation::setState ()" << endl;
      // Get state names.
      String new_presentation_state_name;
      state_machine->find (new_presentation_state, new_presentation_state_name);
      String old_presentation_state_name;
      state_machine->find (getState (), old_presentation_state_name);
      // Find the state name given the StatePair. Return true on success.
      debug << "  New=" << GameStateValue::stateName (new_presentation_state.flowState ()) << '-' << new_presentation_state_name << endl;
      debug << "  Old=" << GameStateValue::stateName (getState ().flowState ()) << '-' << old_presentation_state_name << endl;
      debug << "  Flow\'sPrev=" << GameStateValue::stateName (new_previous_presentation_state.flowState ()) << '-' << new_previous_presentation_state.presentationSubstate () << endl;
      debug << "  Synchronize=" << (need_to_synchronize_presentation_state ? "true" : "false") << endl;
      debug << "  Context=" << currentContextMode () << endl;
      #endif

      // No longer waiting for a substate change request.
      awaiting_substate_change = false;
      // If state_machine doesn't exist yet, error.
      if (!state_machine.isValid ())
         System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::setState: StateMachine not created yet."));
      // True if the state is changing.
      bool state_is_changing = getState () != new_presentation_state;
      // True if the new state is implemented by the game.
      bool game_implemented_state = state_machine->isValidStateId (new_presentation_state);
      // If changing the type of context we are in (i.e. between suspended and playing types)...
      if (isASuspendContext (previousContextMode ()) != isASuspendContext (currentContextMode ()))
         // Flag to sync the condition of the presentation to the context.
         adjust_presentation_to_new_mode = true;
      // If state is changing, update where we are going.
      if (state_is_changing)
      {
         current_state = new_presentation_state;
         previous_state = new_previous_presentation_state;
      }

      Mojo<ResourceList, ScriptInterface> resource_list;
      assertion (resourceList (resource_list));
      Mojo<AutoplayInterface> autoplay_interface;
      if (getResource (autoplay_alias_name, resource_list->resourceKey (), autoplay_interface, false))
      {
         if (autoplay_interface.isValid ())
            autoplay_interface->setState (current_state, previous_state);
      }
      Mojo<DoubleUpInterface> double_up_interface;
      if (getResource (double_up_alias_name, resource_list->resourceKey (), double_up_interface, false))
      {
         if (double_up_interface.isValid ())
            double_up_interface->setState (current_state, previous_state);
      }

      // If we need to enter the presentation because the last context had it suspended...
      if (adjust_presentation_to_new_mode && (presentation_is_entered == false))
      {
         // Show the stage if necessary.
         showStage (show_stage_on_enter);
         // Enter the state-wide presentation. Flag it entered.
         synchronize ();
      }
      // Go to the game state specified if it exists and needs to be updated.
      state_machine->setState (new_presentation_state, need_to_synchronize_presentation_state, isASuspendContext (currentContextMode ()));
      // If the game has implemented this state...
      if (game_implemented_state)
      {
         #ifdef __VERBOSE_STATE__
         debug << "  Valid state, posting state change." << endl;
         #endif
         need_to_synchronize_presentation_state = false;
         // Inform game manager that the presentation has executed an implemented state.
         StateChangeSequenceEvent state_event (new_previous_presentation_state.flowState (),
            new_previous_presentation_state.presentationSubstate (), new_presentation_state.flowState (),
            new_presentation_state.presentationSubstate ());
         postEvent (state_event);
      }
      // Notify the flow that it's okay to continue with it's state flow. This logic prevents
      // each presentation from having to know about the states that the flow will wait for the presentation
      // to complete before continuing through its state flow.
      else
      {
         #ifdef __VERBOSE_STATE__
         debug << "  Unknown state, posting state complete." << endl;
         #endif
         StateCompleteSequenceEvent state_event (new_presentation_state.flowState (),
            new_presentation_state.presentationSubstate (), SequenceEvent::TO_FLOW);
         postEvent (state_event);
      }
      // If the game/context mode has changed AND initialization is done...
      if (adjust_presentation_to_new_mode
         && (current_state.flowState () != GameStateValue::STATE_COLD_START)
         && (current_state.flowState () != GameStateValue::STATE_INIT))
      {
         // If presentation is currently entered but new context needs it suspended...
         if (presentation_is_entered && isASuspendContext (currentContextMode ()))
            // Exit the state-wide presentation. Flag it exited.
            suspend ();
         adjust_presentation_to_new_mode = false;
      }
      // Set the last applied context mode.
      setPreviousContextMode (currentContextMode ());
   }
}
// set the the current volume level in a property
void GamePresentation::setVolumeProperty (void)
{
   #ifdef __VERBOSE_VOLUME__
   debug << "GamePresentation::setVolumeProperty ()" << endl;
   #endif
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
      }
      else if (current_relative_volume > 0)
      {
         #ifdef __VERBOSE_VOLUME__
         debug << "relative volume " << VOLUME_LEVEL_HIGH_STRING << " Level: " << current_relative_volume << endl;
         #endif
         String property_value (VOLUME_LEVEL_HIGH_STRING);
         property->setAbsoluteValue (property_value);
      }
      else
      {
         #ifdef __VERBOSE_VOLUME__
         debug << "relative volume " << VOLUME_LEVEL_DEFAULT_STRING << " Level: " << VOLUME_LEVEL_DEFAULT << endl;
         #endif
         String property_value (VOLUME_LEVEL_DEFAULT_STRING);
         property->setAbsoluteValue (property_value);
      }
      postEvent ("UpdatePlayerVolumeLevelDisplay");
   }
}

// Purpose:  This method is called to hide/show the game main_stage.
void GamePresentation::showStage (bool show)
{
   verboseMethodName();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::showStage (" << (show ? "true" : "false") << ")" << endl;
   #endif
   // Retrieve the main stage from the resource manager.
   StageInterfaces main_stage;
   getStage (main_stage);
   // Do not attempt to show an invalid stage.
   if (main_stage.isValid ())
   {
      // If the show status is changing (as far as the game presentation knows).
      if (show != told_to_show_stage)
      {
         // Show or hide the stage.
         main_stage.getActorInterface ()->show (show);
         // If changing from hiding to showing the stage, update actuators.
         if (show)
            postEvent ("GamePresentation:ShowStage");
         // If changing from showing to hiding the stage, update actuators.
         else
            postEvent ("GamePresentation:HideStage");
         told_to_show_stage = show;
      }
   }
}

// Purpose:  This method is used by exitGameMode to help in seamless transitions between stages.  (Typically, Free Spins)
void GamePresentation::setNVMode (bool _nv_mode)
{
   verboseMethodName();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setNVMode (" << (nv_mode ? "true" : "false") << ")" << endl;
   #endif
   // Change the current mode.
   nv_mode = _nv_mode;
}

// Purpose:  This method is used by setState to help in seamless transitions between stages.
void GamePresentation::setShowStageOnEnter (bool _show_stage_on_enter)
{
   verboseMethodName();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::setShowStageOnEnter (" << (show_stage_on_enter ? "true" : "false") << ")" << endl;
   #endif
   // Change the current mode.
   show_stage_on_enter = _show_stage_on_enter;
}

// Purpose: Request to start the game
//          The session id is the id of the game about to start.
bool GamePresentation::start (State &_state, uint32 session_id)
{
   verboseMethodName();
   // Rely on the flow to determine start.  The presentation can always start.
   return true;
}

// Support the given paytable section name.
void GamePresentation::supportPaytableSection (const char *section)
{
   verboseMethodName ();
   #ifdef __VERBOSE_PAYTABLE__
   debug << game_index << ":" << stage_index << "|GamePresentation::supportPaytableSection, " << String (section) << endl;
   #endif
   // If the section is not already supported, add it to the list of supported paytable sections
   if (!paytableSectionSupported (section))
   {
      // Get the name of the paytable parser and the evaluator to use for this section.
      assertion (paytable_parser.isValid ());
      // Now use the evaluator name to create the appropriate evalautor.
      Mojo <EvaluatorInterface> evaluator;
      paytable_parser->createEvaluator (evaluator, section);

      // Output some basic paytable information to the debug log.
      #ifdef __VERBOSE_PAYTABLE__
      debug << game_index << ":" << stage_index << "|GamePresentation::supportPaytableSection () Paytable Information:" << endl;
      debug << "  -game name: " << evaluator->getGameDescription () << endl;
      debug << "  -paytable section name: " << section << endl;
      debug << "  -nun of win categories = " << evaluator->getNumberOfWinCategories() << endl;
      debug << "  -game percent = " << evaluator->getPercentage () << endl;
      debug << "  -min bet for max bonus = " << evaluator->getMinBetForMaxBonus () << endl;
      #endif

      // Add the evaluator to the map of evaluators.
      pair<EvaluatorIterator, bool> result = evaluator_list.insert (EvaluatorPair (section, evaluator));
      assertion (result.second);
   }
}

// Purpose: Stop every actor in the presentation because the context is suspending or exiting.
void GamePresentation::suspend (void)
{
   verboseMethodName();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::suspend ()" << endl;
   #endif

   // Set actors to a known state.
   postEvent ("GamePresentation:Suspend");
   // If presentation stopped, inform transition stages etc to stop.
   if ((currentGameMode () != GAME_MODE_UNKNOWN) && isASuspendContext (currentContextMode ()))
      postEventAll ("GamePresentation:ContextSuspended");
   // Flag the presentation as exited.
   presentation_is_entered = false;
   need_to_synchronize_presentation_state = true;
}

// Purpose:  This method is called to ensure that Denom Button is correctly enabled/disabled
// after a SYNCRONIZE_PRESENTATION event.
void GamePresentation::synchronizeDenomButton (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeDenomButton (), FlowState = " << flow_state << endl;
   #endif

   if (getFlowState () == GameStateValue::STATE_IDLE
       && isNormalPlay ()
       && processConditionPlayerSelectableDenomination ()
       && !isBetCommittedOrPrecommitted ())
   {
      postEvent ("EnableDenomButton");
   }
   else
   {
      postEvent ("DisableDenomButton");
   }
}

void GamePresentation::synchronizeDenomination (void)
{
   // Retrieve the main stage from the resource manager.
   StageInterfaces stage_interfaces;
   if (!getStage (stage_interfaces))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::synchronizeDenomination (), can not get stage."));
   }

   // Retrieve the resource list.
   Mojo<ResourceList, ScriptInterface> resource_list;
   assertion (resourceList (resource_list));

   Mojo<PropertyInterface> property;

   Mojo<ActorInterface> denom_movie_interface;
   bool found_movie_section (false);
   if (ActorHelper::findActor (stage_interfaces.getActorInterface (), denomination_movie_actor_name, true, denom_movie_interface))
   {
      // If we found a valid denomination movie (denom button), find a matching movie section
      if (denom_movie_interface.isValid ())
      {
         // Find the texture attribute
         Mojo<AttributeInterface> attribute;
         Mojo<TextureAttributeInterface> texture_attribute;

         AttributeIterator attribute_iterator;
         String denom_value (blank_movie_section);
         if(denom_movie_interface->getAttributeIterator (attribute_iterator))
         {
            // search the attribute list
            attribute_iterator->begin ();
            for (attribute_iterator->begin (); !attribute_iterator->isEnd () && !found_movie_section; ++(*attribute_iterator))
            {
               // do the names match?
               if (strcmp ((*(*attribute_iterator))->getType (), TEXTURE_ATTRIBUTE) == 0)
               {
                  // Get the texture attribute...
                  texture_attribute = *(*attribute_iterator);

                  if (texture_attribute.isValid ())
                  {
                     uint32 channel_count (texture_attribute->getChannelCount ());
                     //
                     // Get denomination property value
                     //
                     String property_name (system_denomination_property_name);

                     // Get the current denom property value set by the system
                     if (resource_list->getResource<PropertyInterface> (property_name, resource_list->resourceKey (), property, false))
                     {
                        if (property.isValid ())
                        {
                           property->getAbsoluteValue (denom_value);

                           #ifdef __VERBOSE_DENOMINATION_UPDATES__
                              debug << "        :: property value = " << denom_value << endl;
                           #endif

                           for (uint32 channel_id = 0; (channel_id < channel_count) && !found_movie_section; ++channel_id)
                           {
                              if (texture_attribute->size (channel_id))
                              {
                                 Mojo<TextureResourceInterface> texture_resource_interface;
                                 if (texture_attribute->getTextureResource (channel_id, 0UL, texture_resource_interface))
                                 {
                                    if (texture_resource_interface.isValid ())
                                    {
                                       for (uint32 section_index=0; section_index < texture_resource_interface->getSectionCount (); ++section_index)
                                       {
                                          String section_name (texture_resource_interface->getSectionName (section_index));
                                          #ifdef __VERBOSE_DENOMINATION_UPDATES__
                                             debug << "        -- section name = " << section_name << endl;
                                          #endif

                                          if (strcmp (denom_value, section_name) == 0)
                                          {
                                             found_movie_section = true;
                                             break;
                                          }
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
                  break;
               }
            }
         }
      }
   }

   //
   // Prepare to update game denomination value. If a movie section was found, then update the game denomination value
   // property with the same value as what the system set it to. Otherwise, update the game denomination value property
   // with "Blank" and show the denom text over top of it.
   //
   Mojo<PropertyInterface> game_denomination_property;
   Mojo<PropertyInterface> game_denomination_text_property;
   Mojo<PropertyInterface> system_denomination_property;
   Mojo<PropertyInterface> minimum_bet_property;

   String current_denomination_value ("");

   // Only proceed if we have record and resource and the win amount is 0
   if (resource_list->getResource<PropertyInterface> (system_denomination_property_name, resource_list->resourceKey (), system_denomination_property, false)
      && resource_list->getResource<PropertyInterface> (game_denomination_property_name, resource_list->resourceKey (), game_denomination_property, false)
      && resource_list->getResource<PropertyInterface> (game_denomination_text_property_name, resource_list->resourceKey (), game_denomination_text_property, false))
   {
      if (found_movie_section)
      {
         system_denomination_property->getAbsoluteValue (current_denomination_value);
         game_denomination_property->setAbsoluteValue (current_denomination_value);
         #ifdef __VERBOSE_DENOMINATION_UPDATES__
            debug << "  - setting game denomination value to: " << current_denomination_value << endl;
         #endif
         postEvent (update_game_movie_denomination_event);
      }
      else
      {
         current_denomination_value = blank_movie_section;
         bool min_bet_found = false;

         Mojo <GameSideBetInterface> game_side_bet_interface (bet);
         if (game_side_bet_interface.isValid ())
         {
            if ((currentGameMode () != GAME_MODE_HISTORY) && game_side_bet_interface.isValid () &&
               resource_list->getResource<PropertyInterface> (minimum_bet_property_name, resource_list->resourceKey (), minimum_bet_property, false))
            {
               uint32 minimum_bet = game_side_bet_interface->getMinBetButtonValue ();
               if (minimum_bet > 1)
               {
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
                     locale.moneyFormatNumericWhole (country, minimum_bet_money, minimum_bet_money_string, true /* omit base units if they're 0 */);
                  }
                  else
                  {
                     locale.moneyFormatNumericBase (country, minimum_bet_money, minimum_bet_money_string);
                  }

                  String min_bet_string = "";
                  min_bet_string << mininum_bet_denomination_text << minimum_bet_money_string;
                  minimum_bet_property->setAbsoluteValue (min_bet_string);
                  #ifdef __VERBOSE_DENOMINATION_UPDATES__
                  debug << "  - setting minimum bet value to: " << min_bet_string << endl;
                  #endif

                  min_bet_found = true;
               }
            }
         }

         if (min_bet_found)
            postEvent (update_min_bet_denomination_event);
         else
            postEvent (hide_min_bet_denomination_event);

         game_denomination_property->setAbsoluteValue (current_denomination_value);
         #ifdef __VERBOSE_DENOMINATION_UPDATES__
         debug << "  - setting game denomination value to: " << current_denomination_value << endl;
         #endif
         postEvent (update_game_movie_denomination_event);

         system_denomination_property->getAbsoluteValue (current_denomination_value);
         game_denomination_text_property->setAbsoluteValue (current_denomination_value);
         #ifdef __VERBOSE_DENOMINATION_UPDATES__
         debug << "  - setting game denomination text value to: " << current_denomination_value << endl;
         #endif
         postEvent (update_game_font_denomination_event);
      }
   }

   #ifdef __VERBOSE_DENOMINATION_UPDATES__
   debug << "  - setting game denomination value to: " << current_denomination_value << endl;
   #endif
}

// Purpose:  Called to synchronize all known control panels.
void GamePresentation::synchronizeControlPanels (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeControlPanels ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
      game_control_panel.getControlPanelInterface ()->synchronize ();
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
      game_count_control_panel.getControlPanelInterface ()->synchronize ();

   // Post a generic MultimediaEvent that will notify other ControlPanel actors to synchronize.
   postEvent ("ControlPanel:Synchronize");
}

// Purpose:  This method is called to synchronize the current values displayed by the game count and and games remaining meters.
void GamePresentation::synchronizeGameCountMeters (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeGameCountMeters" << endl;
   #endif
   // Do not attempt to update the meters if the control panel has not been configure for this game.
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
   {
      uint16 play_count = playCount (ACT_TYPE_BONUS);
      #ifdef __VERBOSE_METERS__
      uint16 total_plays = totalPlays ();
      debug << "   play_count: " << play_count << ", total_plays: " << total_plays << endl;
      #endif
      // Configure the meter for games played.
      game_count_control_panel.getGameCountControlPanelInterface ()->setGamesPlayedMeter (play_count);
      // Calculate the number of games remaining.
      uint16 games_remaining = gamesRemaining ();
      // Configure the games remaining meter.
      game_count_control_panel.getGameCountControlPanelInterface ()->setGamesRemainingMeter (games_remaining);
   }
}

// Purpose:  Called to show all known control panels.
void GamePresentation::showControlPanels (bool show)
{
   verboseMethodName ();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::showControlPanels (" << (int) show << ')' << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
      game_control_panel.getControlPanelInterface ()->show (show);
   GameCountControlPanelExtractor game_count_control_panel;
   if (controlPanel (game_count_control_panel))
      game_count_control_panel.getControlPanelInterface ()->show (show);

   // Post a generic MultimediaEvent that will notify other ControlPanel actors to show or hide.
   if (show)
      postEvent ("ControlPanel:Show");
   else
      postEvent ("ControlPanel:Hide");
}

// Purpose:  Retrieve the Amount of the current sub bet meter.
void GamePresentation::subBetDisplayAmount (Amount &sub_bet)
{
   verboseMethodName ();
   // If this game has a valid bet (usually a base game), get the current bet information from the bet object.
   if ((stageIndex () == BASE_GAME_STAGE) && bet.isValid ())
   {
      sub_bet = bet->getSubBetAmount (0);
   }
   // If the game does not have a valid bet (usually a bonus game), display the bet value stored in the essential record.
   else
   {
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (essential_record)
      {
         sub_bet = essential_record->totalBetAtStageStart ();
      }
      else
      {
         sub_bet = ZERO_AMOUNT;
      }
   }
   #ifdef __VERBOSE_METERS__
   Amount::Extractor amount_extractor (sub_bet);
   debug << game_index << ":" << stage_index << "|GamePresentation::subBetDisplayAmount = " << amount_extractor.extract (denomination) << endl;
   #endif
}

// Purpose: Set every actor in the presentation to a known state because the context just entered or its mode changed.
void GamePresentation::synchronize (void)
{
   verboseMethodName();
   #ifdef __VERBOSE_MODE__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronize ()" << endl;
   #endif

   // Make sure all the interface is configured correctly based on player game selectability status.
   postPlayerSelectableGameStatus ();
   // Make sure the meters are displaying the correct information now that we know the flow state.
   synchronizeMeters ();
   // Synchronize the control panels supported by this game.
   synchronizeControlPanels ();
   // Synchronize the volume button.
   synchronizeVolumeButton (true);
   // Synchronize the denomination button
   synchronizeDenomination ();
   // Enable or disable the cashout celebration based on the config item
   synchronizeCashoutCelebration ();
   // Synchronize the denomination mode
   synchronizeDenominationMode ();
   // Synchronize the autoplay message
   synchronizeAutoplayMessage ();
   // Set actors to a known state.
   postEvent ("GamePresentation:Synchronize");
   // Flag the presentation as entered.
   presentation_is_entered = true;

   // Post an event for the current background color.
   synchronizeBackGroundColor ();
   updateMaxBetProperty ();
   // Synchronizes the game language. Set flag to true to force synchronizing.
   synchronizeGameLanguage (true);
   // Update to current meter panel configuration
   // synchronizeGameLanguage must come before updateMeterPanelConfiguration due to
   // volume button moving depending on if the language button is present or not.
   updateMeterPanelConfiguration ();
}

// Purpose: Syncrhonize the visual display of the player wagerable meter
void GamePresentation::synchronizeWagerableAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeWagerableAmount ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode () &&
         currentGameMode () != GAME_MODE_HISTORY;
   if (controlPanel (game_control_panel))
   {
      // Read the value of the player's wagerable meter
      Amount wagerable_amount;
      if (!in_tournament_mode)
      {
         // Read the value of the player's wagerable meter
         wagerableDisplayAmount (wagerable_amount);
         game_control_panel.getGameControlPanelInterface ()->showMeter (GameControlPanel::METER_WAGERABLE, true);
         // Give the wagerable meter to the control panel for display.
         game_control_panel.getGameControlPanelInterface ()->setMeter (GameControlPanel::METER_WAGERABLE, wagerable_amount);
      }
      else
      {
         //wagerable_amount = Amount (getTournamentWagerables (), denomination);
         game_control_panel.getGameControlPanelInterface ()->showMeter (GameControlPanel::METER_WAGERABLE, false);
      }
   }
}

// Purpose:: Synchronize the visual bet components
void GamePresentation::synchronizeBet (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_BET__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeBet ()" << endl;
   #endif
   // Update the total bet information.  Post events concerning our current max bet situation.
   Amount total_bet_amount;
   totalBetDisplayAmount (total_bet_amount);
   // Update the meter display property with the bet value retrieved.
   updateTotalBetMeter (total_bet_amount);
   // Post events that reflect our current betting situation.  Note that this method can be called before the game
   // presentation has been completely configured.  Do not attempt to post an event if the stage isn't ready.
   StageInterfaces main_stage;
   if (getStage (main_stage, false))
   {
      if (isBetAtMaxBet ())
      {
         postEvent ("MaxBet:True");
      }
      else
      {
         postEvent ("MaxBet:False");
      }
   }
}

/// <summary> Synchronize the betting buttons. </summary>
void GamePresentation::synchronizeBetButtons (void)
{
   // do nothing here
}

// Purpose:: Synchronize the visual bet components do to a (host central determination) bet configuration change.
void GamePresentation::synchronizeBetConfig (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_BET__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeBetConfig ()" << endl;
   #endif

   updateBetConfiguration ();

   // Post events that reflect our current betting situation.  Note that this method can be called before the game
   // presentation has been completely configured.  Do not attempt to post an event if the stage isn't ready.
   StageInterfaces main_stage;

   if (getStage (main_stage, false))
      postEvent ("GamePresentation:SynchronizeBetConfig");

   synchronizeDenomination ();
}

// Purpose:  Syncrhonize the visual cashout components.
void GamePresentation::synchronizeCashout (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeCashout ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
   {
      // Do not update the buttons if the game is not operating in its idle state and base stage.  Make sure the
      // the bank says it's okay to enable the button before enabling it.
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (stageIndex () == BASE_GAME_STAGE && getFlowState () == GameStateValue::STATE_IDLE && bank_bet.okToEnableCashout (true))
      {
         game_control_panel.getGameControlPanelInterface ()->enableCashoutButton (true);
      }
      else if (getFlowState () == GameStateValue::STATE_IDLE &&
               essential_record->doubleupPermitted () && bank_bet.okToEnableCashout (true))
      {// it only for the final credit win double up
         game_control_panel.getGameControlPanelInterface ()->enableCashoutButton (true);
      }
      else
      {
         game_control_panel.getGameControlPanelInterface ()->enableCashoutButton (false);
      }
   }
}

// Purpose:  Enable or disable the cashout celebration
void GamePresentation::synchronizeCashoutCelebration (void)
{
   // Get the config id for the cashout celebration enabled flag
   Config config;
   ConfigID cashout_celebration_config_id = config.getId (CONFIG_ID_CASHOUT_CELEBRATION_ENABLED, false);

   // Make sure we got a valid ID
   if (cashout_celebration_config_id != INVALID_CONFIGID)
   {
      // If the flag is set to enabled, then post the event to enable the actuator to play the animation
      if (static_cast <bool> (config.getValue (cashout_celebration_config_id)) == true)
      {
         postEvent ("CashoutCelebration:Enabled");
      }
      // If the flag is set to disabled, then post the event to disable the actuator to play the animation
      else
      {
         postEvent ("CashoutCelebration:Disabled");
      }
   }
}

// Purpose:  Syncrhonize the visual transfer components.
void GamePresentation::synchronizeTransferButton (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeTransferButton ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
   {
      // Make sure it is okay to move money
      if (money_handling_style == MONEY_HANDLING_STYLE_BANKED_CREDITS &&
         bank_bet.okayToEnableMoveMoney () && !checkForDoubleUpEnable ())
      {
         game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_TRANSFER, true);
      }
      else
      {
         game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_TRANSFER, false);
      }
   }
}

// Purpose: Return conditional that allows including award list credits during win rollups
bool GamePresentation::includeAwardListCredits (void)
{
   // Get the current essential record information.
   const EssentialRecord * essential_record = getEssentialRecord ();
   return ((currentGameMode () != GAME_MODE_HISTORY && essential_record && essential_record->doubleupPermitted ()));
}

/// <summary># Syncrhonize the visual display of the player amount (player credits).</summary>
void GamePresentation::synchronizePlayerAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeCreditAmount ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode () &&
         currentGameMode () != GAME_MODE_HISTORY;
   if (controlPanel (game_control_panel))
   {
      // Read the value of the player's credit meter
      Amount credit_amount;
      Amount total_win_amount_from_award (ZERO_AMOUNT);
      if (!in_tournament_mode)
      {
         if (includeAwardListCredits ())
         {
            extractTotalWinFromAward (total_win_amount_from_award);
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
      // Give the credit meter to the control panel for display.
      game_control_panel.getGameControlPanelInterface ()->setCreditMeter (credit_amount + total_win_amount_from_award);
   }
}

/// <summary># Syncrhonize the visual display of the player amount (player credits).</summary>
/// <remarks># Games overriding synchronizeCreditAmount() in their custom component, wishing to utilize the banked credit meter
///    (wagerable) feature, will need to change their overridden function name to synchronizePlayerAmount() instead.  This will
///    cause calls to “synchronizeCreditAmount” elsewhere in the code to call down to the GamePresentation base class to help
///    account for banked meter (wagerable) support.</remarks>
void GamePresentation::synchronizeCreditAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeCreditAmount ()" << endl;
   #endif
   // Update the player amount and wagerable amount depend on the current meter values.
   synchronizePlayerAmount ();
   if (money_handling_style == MONEY_HANDLING_STYLE_BANKED_CREDITS)
      synchronizeWagerableAmount ();
}

// Purpose: Synchronize the visual display of the player paid (paid meter)
void GamePresentation::synchronizePaidAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizePaidAmount ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode () &&
         currentGameMode () != GAME_MODE_HISTORY;
   if (controlPanel (game_control_panel))
   {
      if (!in_tournament_mode)
      {
         // Read the value of the player paid meter.
         Amount paid_amount;
         paidDisplayAmount (paid_amount);
         // Configure the meter display with the paid amount.
         game_control_panel.getGameControlPanelInterface ()->setPaidMeter (paid_amount);
      }
      else
      {
         game_control_panel.getGameControlPanelInterface ()->showPaidMeter (false);
      }
   }
}

// Purpose: Set the the current menu button state and resets the menu button if it isn't shown
void GamePresentation::synchronizeMenuProperty (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_MENU__
   debug << "GamePresentation::synchronizeMenuProperty ()" << endl;
   #endif

   // Update the property to the current menu option
   setMenuProperty ();
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state.
void GamePresentation::synchronizeMeters (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeMeters (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeBet ();
   synchronizeCashout ();
   synchronizeTransferButton ();
   synchronizeCreditAmount ();
   synchronizeMenuProperty ();
   synchronizePaidAmount ();
   synchronizeWinAmount ();
   synchronizeBonusWinAmount ();
   synchronizeGameCountMeters ();
   synchronizeBingoWinAmounts ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Purpose: Synchronize the visual display of the bonus win meter.
void GamePresentation::synchronizeBonusWinAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeBonusWinAmount ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   if (controlPanel (game_control_panel))
   {
      Amount bonus_win_amount (ZERO_AMOUNT);

      extractBonusWinFromAward (bonus_win_amount);
      // Give the meter to the control panel for display.
      game_control_panel.getGameControlPanelInterface ()->setBonusWinMeter (bonus_win_amount, RollupDefs::SNAP_TO_AMOUNT);
   }
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state after a MONEY_BET event.
void GamePresentation::synchronizeMoneyBet (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeMoneyBet (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeBet ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state after a MONEY_IN event.
void GamePresentation::synchronizeMoneyIn (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeMoneyIn (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeBet ();            // Updates MAX_BET button status.
   synchronizeCashout ();
   synchronizeTransferButton ();
   synchronizeCreditAmount ();
   synchronizePaidAmount ();
   synchronizeGameCountMeters ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state after a MONEY_MOVED event.
void GamePresentation::synchronizeMoneyMoved (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeMoneyMoved (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeCashout ();
   synchronizeTransferButton ();
   synchronizeCreditAmount ();
   synchronizeBet ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state after a MONEY_OUT event.
void GamePresentation::synchronizeMoneyOut (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeMoneyOut (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeCashout ();
   synchronizeTransferButton ();
   synchronizeCreditAmount ();
   synchronizePaidAmount ();
   synchronizeGameCountMeters ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Purpose: Synchronize the presentation
void GamePresentation::synchronizePresentation (const SynchronizePresentationSequenceEvent& event)
{
   verboseMethodName();
   #ifdef __VERBOSE_STATE__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizePresentation (event)" << endl;
   #endif

   synchronizePresentation (event.whatToSynchronize(), event.betRequestType (), event.betRequestData ());

   bool tournament_mode_enabled = SystemFlags ().systemInTournamentMode ();
   if (tournament_mode_enabled)
   {
      synchronizeTournament (); // make sure tournament information is current after power hit
   }
}

// Purpose: Synchronize the presentation
void GamePresentation::synchronizePresentation (uint32 what_to_synchronize, uint32, uint32)
{
   verboseMethodName();
   #ifdef __VERBOSE_STATE__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizePresentation ()" << endl;
   #endif
   if ((what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_PANEL) != 0)
   {
      synchronizeDenomButton ();
      synchronizeCashout ();
      synchronizeTransferButton ();
   }

   // If the bet or player credit amounts need to be updated, we will update
   // both the bet and credit displays since their displays are interdependent..
   if ((0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_BET)) ||
       (0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_PLAYER_AMOUNT)) ||
       (0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_WAGERABLE_AMOUNT)) ||
       (0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_BET_CONFIG)))
   {
      synchronizeBet ();
      synchronizeCreditAmount ();
      synchronizeWinAmount ();
      synchronizeBonusWinAmount ();
      synchronizeBingoWinAmounts ();

      // If not already done above
      if (0 == (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_PANEL))
      {
         synchronizeTransferButton ();
      }
   }

   if (0 != (what_to_synchronize & SynchronizePresentationSequenceEvent::SYNCHRONIZE_BET_CONFIG))
      synchronizeBetConfig ();
}

// Purpose:  This method is called to ensure that the game meter displays are shown correctly
// for the current game state after a SYSTEM_METERS_CHANGED event.
void GamePresentation::synchronizeSystemMetersChanged (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   GameStateValue flow_state = getFlowState ();
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeSystemMetersChanged (), FlowState = " << flow_state << endl;
   #endif
   // Update the meter displays and all displays that depend on the current meter values.
   synchronizeCreditAmount ();
   synchronizePaidAmount ();
   synchronizeWinAmount ();
   synchronizeBonusWinAmount ();
   synchronizeGameCountMeters ();
   synchronizeBingoWinAmounts ();
   // If the game is operating in the award state, make the rollup complete quickly.  When the meter is
   // finished with its rollup, the state will exit normally.
   if (getFlowState () == GameStateValue::STATE_AWARD)
   {
      accelerateRollup ();
   }
   // Synchronizes the flashing of the win and paid meter displays.
   synchronizeWinPaidMeterCycle ();
}

// Synchronize the visual display of tournament items.
void GamePresentation::synchronizeTournament (void)
{
   verboseMethodName ();
   bool tournament_mode_enabled = SystemFlags ().systemInTournamentMode ();

   if (tournament_mode_enabled) // if currently in tournament mode, translate the tournament info into events the presentation scripts can use
   {
      synchronizeCreditAmount ();
      synchronizePaidAmount ();
      synchronizeWinAmount ();
      synchronizeBonusWinAmount ();
      synchronizeBet ();
      synchronizeBingoWinAmounts ();

      postEventAll ("TournamentEnabled"); // post for general tournament presentation changes

      // Make sure current time text is up to date.
      // This is necessary because system timer update is posted before game presentation is initialized on power hit.
      uint32 system_display_timer_milliseconds;
      if (getPropertyValueUint32 ("SystemDisplayTimer", "Default", system_display_timer_milliseconds))
      {
         updateSystemTimer (system_display_timer_milliseconds);
      }

      String string_tournament_mode;
      if (getTournamentMode (string_tournament_mode))
      {
         if (string_tournament_mode == "TournamentModeTimeOnly")
         {
            postEventAll ("TournamentMode:TournamentModeTimeOnly");
         }
         else if(string_tournament_mode == "TournamentModeCreditsOnly")
         {
            postEventAll ("TournamentMode:TournamentModeCreditsOnly");
         }
         else if(string_tournament_mode == "TournamentModeTimeAndCredits")
         {
            postEventAll ("TournamentMode:TournamentModeTimeAndCredits");
         }
      }

      String string_tournament_status;
      if (getTournamentStatus (string_tournament_status))
      {
         if (string_tournament_status == "TournamentSessionReady")
         {
            postEventAll ("TournamentStatus:TournamentSessionReady");

            // Enable game start if control is enabled
            GameControlPanelExtractor game_control_panel;

            bool enable_game_start = false;
            if (controlPanel (game_control_panel))
            {
               if (game_control_panel.getControlPanelInterface ()->isEnabled ())
               {
                  enable_game_start = true;
               }
            }

            //Post the proper event.
            if ((enable_game_start) &&
                (isFlowStateIdle ()))
               postEventAll ("EnableGameStartButtonForTournament");
            else
               // don't allow tournament play
               postEventAll ("DisableGameStartButtonForTournament");
         }
         else if(string_tournament_status == "TournamentSessionStarted")
         {
            postEventAll ("TournamentStatus:TournamentSessionStarted");
         }
         else if(string_tournament_status == "TournamentSessionEnded")
         {
            postEventAll ("TournamentStatus:TournamentSessionEnded");
            // don't allow tournament play
            postEventAll ("DisableGameStartButtonForTournament");
         }
      }
   }
   else
   {
      postEventAll ("TournamentDisabled"); // post for general tournament presentation changes
   }
}

// set the the current volume button state and resets the volume if it isn't shown
void GamePresentation::synchronizeVolumeButton (bool enable)
{
   #ifdef __VERBOSE_VOLUME__
   debug << "GamePresentation::synchronizeVolumeButton ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;

   if (controlPanel (game_control_panel))
   {
      // show the button
      game_control_panel.getGameControlPanelInterface ()->showVolumeButton (true);

      if (okayToEnableVolumeButton ())
      {
         // enable/disable the button
         game_control_panel.getGameControlPanelInterface ()->enableVolumeButton (enable);

         // Update the property to the current volume level
         setVolumeProperty ();
      }
      else
      {
         // Dim the button
         postEvent ("DimVolumeButton");

         // disable the button
         game_control_panel.getGameControlPanelInterface ()->enableVolumeButton (false);

         // reset the relative volume
         resetVolume ();
      }
   }

}

// Purpose: Synchronize the visual display of the win meter.
void GamePresentation::synchronizeWinAmount (void)
{
   verboseMethodName ();
   #ifdef __VERBOSE_METERS__
   debug << game_index << ":" << stage_index << "|GamePresentation::synchronizeWinAmount ()" << endl;
   #endif
   GameControlPanelExtractor game_control_panel;
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode () &&
         currentGameMode () != GAME_MODE_HISTORY;
   if (controlPanel (game_control_panel))
   {
      // Extract the bonus and total win values from the award if it is safe to show the win.
      if (canDisplayAward () || (in_tournament_mode))
      {
         Amount total_win_amount (ZERO_AMOUNT);
         extractTotalWinFromAward (total_win_amount);

         if (in_tournament_mode)
         {
            total_win_amount = Amount (getTournamentWon (), denomination);
         }
         // Give the meter to the control panel for display.
         game_control_panel.getGameControlPanelInterface ()->setWinMeter (total_win_amount, RollupDefs::SNAP_TO_AMOUNT);
      }
      // if can't display the award and not in tournament mode or in a free act, just set the amount to zero
      else if (!in_tournament_mode)
      {
         // Give the meter to the control panel for display.
         game_control_panel.getGameControlPanelInterface ()->setWinMeter (ZERO_AMOUNT, RollupDefs::SNAP_TO_AMOUNT);
      }
   }
}

// Purpose:  This method synchronizes the flashing of the win and paid meter displays.
void GamePresentation::synchronizeWinPaidMeterCycle (void)
{
   verboseMethodName ();
   switch (getFlowState ())
   {
      case GameStateValue::STATE_HISTORY:
      case GameStateValue::STATE_UTILITY:
         postEvent ("SynchronizeWinPaidMeter:ShowWin");
         break;

      default:
         {
            // The presentation of the meters depends on the value of the paid meter.  We will
            // show the paid meter if the paid meter value is non zero.  Otherwise, we'll show
            // the win meter. Extract the value for use in determining how to display the meters.
            Amount paid_amount;
            paidDisplayAmount (paid_amount);
            if (paid_amount == ZERO_AMOUNT)
               postEvent ("SynchronizeWinPaidMeter:ShowWin");
            else
               postEvent ("SynchronizeWinPaidMeter:ShowWinAndPaid");
         }
         break;
   }
}

// Purpose: Return true if a win mismatch is pending.
bool GamePresentation::winMismatchPending (void)
{
   return win_mismatch_pending;
}

// Returns whether or not presentation should slam credits during a win mismatch
bool GamePresentation::okayToSlamMismatchWins (void)
{
   return (config.getValue (pay_all_mismatched_wins_to_credit_meter_id));
}

// Returns whether or not we want to show the draw messages
bool GamePresentation::okayToShowDrawMessages (void)
{
   bool show = true;
   ConfigID config_id = config.getId (CONFIG_ID_SHOW_DRAW_STATE_MESSAGE, game_configuration_identifier, false);
   if (config_id != INVALID_CONFIGID)
   {
      show = config.getValue (config_id);
   }
   return (show);
}

// Returns whether or not game is in auto play mode.
bool GamePresentation::okayToShowAutoplayMessage (void)
{
   bool show = false;
   ConfigID config_id = config.getId (CONFIG_ID_SHOW_AUTOPLAY_MESSAGE, game_configuration_identifier, false);
   if (config_id != INVALID_CONFIGID)
   {
      show = config.getValue (config_id);
   }
   String autoplay_state_value;
   config.getValueString (autoplay_state_config_id, autoplay_state_value);
   return ((autoplay_state_value != "DISABLED") && show);
}

// This method synchronizes the denomination mode (single denom or multi denom).
void GamePresentation::synchronizeDenominationMode (void)
{
   verboseMethodName ();
   switch (getFlowState ())
   {
      // Always display single denom information in history.
      case GameStateValue::STATE_HISTORY:
         postEvent ("DenominationMode:SingleDenom");
         break;

      default:
         if (numberOfAvailablePlayerDenominations () > 1)
            postEvent ("DenominationMode:MultiDenom");
         else
            postEvent ("DenominationMode:SingleDenom");

         break;
   }
}

// Purpose: To get the bingo specific win amounts from the award list
void GamePresentation::getBingoWinAmounts (BingoMeterData &bingo_meters)
{
   verboseMethodName ();

   // Reset amounts first
   bingo_meters.interim_win = ZERO_AMOUNT;
   bingo_meters.bingo_prize = ZERO_AMOUNT;
   bingo_meters.bingo_grand_prize = ZERO_AMOUNT;
   bingo_meters.bingo_interim_progressive_prize = ZERO_AMOUNT;
   bingo_meters.bingo_coincidental_win = ZERO_AMOUNT;

   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   if (local_award.isValid ())
   {
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         Mojo <SystemAwardInterface> game_award;
         // Pointer to SystemAward.
         if (local_award->getAward (award_index, game_award))
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

// Purpose: Synchronize the visual display of the Bingo win amounts
void GamePresentation::synchronizeBingoWinAmounts (void)
{
   verboseMethodName ();
   // Only synchronize bingo related win amounts if it is bingo system
   if (isBingo ())
   {
      GameControlPanelExtractor game_control_panel;
      BingoMeterData bingo_meters;

      if (controlPanel (game_control_panel))
      {
         // get the bingo specific win amounts from the award list
         if (canDisplayAward ())
         {
            getBingoWinAmounts (bingo_meters);
         }

         // Give the meter to the control panel for display.
         game_control_panel.getGameControlPanelInterface ()->setBingoInterimWinMeter (bingo_meters.interim_win);
         game_control_panel.getGameControlPanelInterface ()->setBingoPrizeWinMeter (bingo_meters.bingo_prize);
         game_control_panel.getGameControlPanelInterface ()->setBingoGrandPrizeWinMeter (bingo_meters.bingo_grand_prize);
      }
   }
}

// Synchronize any autoplay messages
void GamePresentation::synchronizeAutoplayMessage (void)
{
   verboseMethodName ();
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the autoplay message if the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      Mojo<PropertyInterface> property;
      bool show = okayToShowAutoplayMessage ();
      if (resource_list->getResource<PropertyInterface> (PROPERTY_SHOW_AUTOPLAY_MESSAGE, resource_list->resourceKey (), property, false))
      {
         // Update the resource with the current value.
         property->setAbsoluteValue (show);
      }
   }
}

// Fill in the total amount won from the award_list on the given stage index
// Returns true on success
bool GamePresentation::totalAmountWonOnStage (StageIndex stage_index, Amount &amount)
{
   verboseMethodName ();
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   Mojo <GamePresentationQueryInterface> i;
   bool success = findInterface <GamePresentationQueryInterface> (i, GameStage (gameIdentifier (), stage_index));
   if (success)
   {
      amount = ZERO_AMOUNT;
      Mojo<SystemAwardInterface> game_award;
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         // If the current award is from the game that initiated this bonus game, cache the award and stop processing.
         if (local_award->getAward (award_index, game_award) && game_award.isValid () && i->paytableSectionSupported (game_award->getName ()))
         {
            Amount award_amount;
            game_award->getTotalAwardAmount (award_amount);
            amount += award_amount;
         }
      }
   }
   return success;
}

// Purpose:  Retrieve the Amount of the current total bet meter.
void GamePresentation::totalBetDisplayAmount (Amount &total_bet)
{
   verboseMethodName();
   // If this game has a valid bet (usually a base game), get the current bet information from the bet object.
   if ((stageIndex () == BASE_GAME_STAGE) && bet.isValid ()  && (getFlowState () != GameStateValue::STATE_HISTORY))
   {
      total_bet = bet->getBetAmount ();
   }
   // If the game does not have a valid bet (usually a bonus game), display the bet value stored in the essential record.
   else
   {
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (essential_record)
      {
         total_bet = essential_record->totalBetAtStageStart ();
      }
      else
      {
         total_bet = ZERO_AMOUNT;
      }
   }
   #ifdef __VERBOSE_METERS__
   Amount::Extractor amount_extractor (total_bet);
   debug << game_index << ":" << stage_index << "|GamePresentation::totalBetDisplayAmount = " << amount_extractor.extract (denomination) << endl;
   #endif
}

// Purpose:  Return the number of times this game has been played.
uint16 GamePresentation::totalPlays (void)
{
   const EssentialRecord* essential_record = getEssentialRecord ();
   assertion (essential_record);
   uint16 total = essential_record-> totalPlays ();
   #ifdef __VERBOSE__
      debug << "GamePresentation::totalPlays:  " << total << endl;
   #endif
   return total;
}

// Purpose: Update the specified config item
void GamePresentation::updateConfigItem (const String &config_id)
{
}

/// <summary># Get the number of bet configuration items.</summary>
/// <returns># Return the number of bet.</returns>
uint32 GamePresentation::getBetConfigurationItemCount (void) const
{
   Config config;
   ConfigID subbets_id = config.getId ("NUMBER OF SUBBETS", stage_configuration_identifier);
   ConfigID maxbet_id = config.getId ("MAXIMUM TOTAL BET", game_configuration_identifier);
   // Adjust for the slot games with multiple lines per credit (N for 1).
   ConfigID subbets_per_credit_id = config.getId ("SUBBETS PER CREDIT", stage_configuration_identifier);
   uint32 config_entry_count = (config.getValue (maxbet_id) * config.getValue (subbets_per_credit_id)) / config.getValue (subbets_id);

   return config_entry_count;
}

/// <summary># Get the string identifier for the configuration item.</summary>
/// <param name="bet">#[in] The number of bets.</param>
/// <param name="config_item_name">#[out] The string identifier for the configuration item.</param>
/// <exception cref="LogicError::INVALID_VALUE"># Thrown when bet out of bounds.</exception>
void GamePresentation::getBetConfigurationString (uint32 bet, String& config_item_name) const
{
   if (!(0 < bet && bet <= getBetConfigurationItemCount ()))
   {
      System::error (LogicError (LogicError::INVALID_VALUE, "GameFlow::getBetConfigurationString(): bet out of bounds."));
   }
   config_item_name = "BET ";
   config_item_name << bet << " CONFIG";
}

/// <summary># To determine if the bet config is required for games that require enrollment.</summary>
/// <returns># Return whether the bet config is required.</returns>
bool GamePresentation::isCDSBetConfigRequired (void) const
{
   Config config;
   ConfigID bingo_id = config.getId ("BINGO SYSTEM GAMING");

   return (enrollment_required && !config.getValue (bingo_id));
}

/// <summary># Process the bet configuration item and store into permitted_bet_list.</summary>
/// <param name="config_item">#[in] The bet config item.</param>
/// <param name="permitted_bet_list">#[inout] The permitted bet list.</param>
/// <param name="line">#[in] Bet per line.</param>
void GamePresentation::processBetConfigItem (const char* config_item, vector <BetPermittedData>& permitted_bet_list, uint16 line)
{
   Config config;
   ConfigID config_id;

   BitField bits;
   config_id = config.getId (config_item, game_configuration_identifier);

   config.getValueBitField (config_id, bits);

   ConfigID line_number_bit_field_list_id = config.getId ("LINE NUMBER BIT FIELD LIST", game_configuration_identifier, false);
   uint32 line_size = config.getListSize (line_number_bit_field_list_id) - 1;
   for (uint16 i = 0; i < line_size; ++i)
   {
      BetPermittedData bet_permitted_entry;
      if (bits.getBit (i) != 0)
      {
         bet_permitted_entry.selected_lines = i + 1;
         bet_permitted_entry.bet_per_line = line;
         permitted_bet_list.push_back (bet_permitted_entry);

         #ifdef __VERBOSE_CDS__
            debug << "            BET PAIR: " <<  bet_permitted_entry.selected_lines << ":" << bet_permitted_entry.bet_per_line << endl;
         #endif
      }
   }
}

/// <summary># Update the bet configuration.</summary>
bool GamePresentation::updateBetConfiguration ()
{
   // For rule based game bet, if it's a CDS game, update button map in lua script according to permitted bet list.
   if (isCDSBetConfigRequired ())
   {
      // Only for new bet object.
      Mojo<RuleBasedGameSideBetInterface> rule_based_game_side_bet (bet);
      if (rule_based_game_side_bet.isValid ())
      {
         // Get the list of permitted bets.
         vector <BetPermittedData> permitted_bet_list;
         String config_item_name;

         uint32 config_item_count = getBetConfigurationItemCount ();
         for (uint16 i = 1; i <= config_item_count; ++i)
         {
            getBetConfigurationString (i, config_item_name);
            processBetConfigItem (config_item_name, permitted_bet_list, i);
         }
         updatePermittedBetList (permitted_bet_list);
      }
   }

   return true;
}

/// <summary> Update bet permitted bet list </summary>
/// <param name="permitted_bet_list"> [in] New permitted bet list.</param>
void GamePresentation::updatePermittedBetList (const vector <BetPermittedData> &permitted_bet_list)
{
   // If permitted bet list is not empty, update it in lua.
   if (!permitted_bet_list.empty ())
   {
      StageInterfaces main_stage;
      if (getStage (main_stage, false))
      {
         // Construct the payload data.
         ScriptTableParameter permitted_bet_items_list;
         uint32 permitted_item_count = permitted_bet_list.size ();
         for (uint32 bet_item_index = 0; bet_item_index < permitted_item_count; ++bet_item_index)
         {
            ScriptTableParameter permitted_bet_item;
            permitted_bet_item.script_table ["bet_items"] =
               ScriptParameter (permitted_bet_list[bet_item_index].selected_lines, ParameterType::UINT32);
            permitted_bet_item.script_table ["bet_per_item"] =
               ScriptParameter (permitted_bet_list[bet_item_index].bet_per_line, ParameterType::UINT32);

            String string_index ("permitted_bet_item_");
            string_index << bet_item_index;
            permitted_bet_items_list.script_table [string_index.c_str ()] =
               ScriptParameter (permitted_bet_item, ParameterType::TABLE);
         }

         ScriptTableParameter permitted_bet_items_table;
         permitted_bet_items_table.script_table ["permitted_bet_items_table"] =
            ScriptParameter (permitted_bet_items_list, ParameterType::TABLE);
         // Post MMEvent with payload data containing permmitted bet list.
         main_stage.getActorStageInterface ()->postEventExtended ("PermittedBetListChanged", 0, &permitted_bet_items_table);
      }
   }
}

/// <summary>Allow for derived classes to update their info when switching between paytables within the same theme.</summary>
void GamePresentation::updateAfterPaytableSwitch (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::updateAfterPaytableSwitch ()" << endl;
   #endif

   /* Sets the credit meter to display the amount in cash if necessary prior to presentation
      synchronization. */
   setCreditMeterDisplayBehavior (false);

   // Allow the game to update its background color if applicable.
   synchronizeBackGroundColor ();
   updateMaxBetProperty ();
}

// Post an event for the current background color.
void GamePresentation::synchronizeBackGroundColor (void)
{
   #ifdef __VERBOSE__
   debug << "GamePresentation::synchronizeBackGroundColor ()" << endl;
   #endif

   ConfigID config_id = config.getId (CONFIG_ID_BACKGROUND_COLOR, game_configuration_identifier, false);

   // If this game doesn't support the game speed then return the default value.
   if (config_id != INVALID_CONFIGID)
   {
      String background_color;
      config.getValueString (config_id, background_color);
      String event_to_post ("Background:SetColor:");
      event_to_post << background_color;
      // Use post event all here so that all contexts get this message.
      postEventAll (event_to_post);
   }
}

/// <summary> Update to the current meter panel configuration.</summary>
void GamePresentation::updateMeterPanelConfiguration (void)
{
   // default
   bool auto_play_btn_showing = false;
   bool language_or_double_up_btn_showing = false;
   bool more_games_showing = false;

   String event_to_post = "";

   // Get current on screen button configuration
   // Check if language button or double up is present.
   language_or_double_up_btn_showing = (doubleUpEnabled () || isMultiLingualAvailableForPlayer ());

   //Check if auto play is enabled.
   updateAutoplayButtonPresentation (true);
   auto_play_btn_showing = isAutoPlayEnabled ();

   // Check if the More Games button will be present
   if (numberOfAvailablePlayerGames () > 1)
   {
      more_games_showing = true;
   }
   else
   {
      more_games_showing = false;
   }

   //Post events based on the current meter panel configuration
   if (auto_play_btn_showing  && language_or_double_up_btn_showing)
   {
      if (more_games_showing)
      {
         event_to_post << "AutoPlay:DoubleUp:MoreGames:";
      }
      else
      {
         event_to_post << "AutoPlay:DoubleUp:";
      }
   }
   else if (auto_play_btn_showing  && !language_or_double_up_btn_showing)
   {
      if (more_games_showing)
      {
         event_to_post << "AutoPlay:MoreGames:";
      }
      else
      {
        event_to_post << "AutoPlayOnly:";
      }
   }
   else if (!auto_play_btn_showing  && language_or_double_up_btn_showing)
   {
      if (more_games_showing)
      {
         event_to_post << "DoubleUp:MoreGames:";
      }
      else
      {
         event_to_post << "LanguageDoubleUpOnly:";
      }
   }
   else
   {
      if (more_games_showing)
      {
         event_to_post << "MoreGamesOnly:";
      }
      else
      {
         event_to_post << "NoOptions:";
      }
    }
    postEvent (event_to_post << "Enabled");
}
// Purpose: This updates the system display timer display with formatted text using the given timer value.
void GamePresentation::updateSystemTimer (const uint32 timer_seconds)
{
   const uint32 SECONDS_IN_A_MINUTE = 60;
   const uint32 SECONDS_IN_AN_HOUR = 3600;

   uint32 remaining_seconds = timer_seconds;

   uint8 hours = (remaining_seconds / SECONDS_IN_AN_HOUR);
   remaining_seconds -= (hours * SECONDS_IN_AN_HOUR);

   uint8 minutes = (remaining_seconds / SECONDS_IN_A_MINUTE);
   remaining_seconds -= (minutes * SECONDS_IN_A_MINUTE);

   uint8 seconds = remaining_seconds;

   String formatted_time_text;

   if (hours > 0) // if hours remaining, display in XX:XX:XX format
   {
      assertion ((hours <= 24) && (minutes <= 60) && (seconds <= 60));
      formatted_time_text.sprintf ("%02u:%02u:%02u", hours, minutes, seconds);
   }
   else // if less than 1 hour remaining, display in XX:XX format
   {
      assertion ((minutes <= 60) && (seconds <= 60));
      formatted_time_text.sprintf ("%02u:%02u", minutes, seconds);
   }
   updateTimerString (formatted_time_text);
   #ifdef __VERBOSE_TIMER__
   debug << "GamePresentation::updateSystemTimer " << formatted_time_text << endl;
   #endif
}

// Purpose: This updates the property that holds the timer string.
bool GamePresentation::updateTimerString (String new_timer_string)
{
   verboseMethodName();
   bool success = false;
   // Resource list from GamePresentation.
   Mojo<ResourceList, ScriptInterface> resource_list;
   // Do not update the resource list is not valid yet.
   if (resourceList (resource_list))
   {
      Mojo<PropertyInterface> property;
      if (resource_list->getResource<PropertyInterface> (formatted_timer_text_property_name, "Default", property, false))
      {
         property->setAbsoluteValue (new_timer_string);
         postEvent (formatted_timer_text_property_updated_event);
         success = true;
      }
   }
   return (success);
}

// Purpose:  Update the TotalBetMeter with the given value.
void GamePresentation::updateTotalBetMeter (const Amount &value)
{
   verboseMethodName();
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
      if (resource_list->getResource<PropertyInterface> ("TotalBetMeter", resource_list->resourceKey (), property, false))
      {
         Amount::Extractor amount_extractor (value);

         // Update the resource with the current multiplier value.
         property->setAbsoluteValue (amount_extractor.extract (Denomination::BASE_UNIT));
         postEvent ("UpdateTotalBetMeter");
      }
   }

   GameControlPanelExtractor game_control_panel;
   if (GamePresentation::controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->setTotalBetMeter (value);
   }
}

// notify presentation elements of tournament changes
void GamePresentation::updateTournamentInfo (void)
{
   bool tournament_mode_enabled = SystemFlags ().systemInTournamentMode ();

   if (tournament_mode_enabled)
   {
      synchronizeTournament ();
   }
}


// Purpose: Used for stepping through various power hits in the states as a test.
// The order_id corresponds to the order the breakpoints warm boot. They must
// be sequential and go from 0 to N. 0xFFFFFFFF is reserved as WARM_BOOT_RESET_SEQUENCE_ID.
bool GamePresentation::updateWarmBootId (uint32 order_id)
{
   bool changed = false;
   if (safe_store_game_presentation.isAttached ())
   {
      // Get pointer to presentation's safe storage data.
      safe_store_game_presentation.refresh ();
      GamePresentationSafeStorage *game_presentation_safe_storage = reinterpret_cast<GamePresentationSafeStorage *> (safe_store_game_presentation.getMemory ());
      if (game_presentation_safe_storage)
      {
         uint32 next_id_to_warm_boot_at = game_presentation_safe_storage->warm_boot_index + 1;
         bool requesting_value_change = game_presentation_safe_storage->warm_boot_index != order_id;
         // If the value will change AND the order id is the next one to break on (they are sequential 0-N) OR resetting the sequence...
         if (requesting_value_change
            && ((order_id == next_id_to_warm_boot_at) || (order_id == WARM_BOOT_RESET_SEQUENCE_ID)))
         {
            // Store new id in safe storage.
            game_presentation_safe_storage->warm_boot_index = order_id;
            safe_store_game_presentation.flush ();
            changed = true;
         }
      }
   }
   // Return true if the value changed.
   return changed;
}

// Return the type of this scriptable object.
const char *GamePresentation::whoAreYou (void) const
{
   return "GamePresentation";
}

// Purpose:  Retrieve the Amount of the current win meter.
void GamePresentation::winDisplayAmount (Amount &win)
{
   // If the flow has determined that it's okay to show the award to the player, update the win amount
   // parameter received with the total award amount applied to the credit meter.  Otherwise, update the
   // amount with zero.
   bool in_tournament_mode = SystemFlags ().systemInTournamentMode () &&
         currentGameMode () != GAME_MODE_HISTORY;
   if (canDisplayAward ())
   {
      const EssentialRecord *essential_record = getEssentialRecord ();
      if (essential_record)
      {
         win = essential_record->totalWinToPlayerAmount ();
      }
      else
      {
         win = ZERO_AMOUNT;
      }
      if (in_tournament_mode)
      {
         win = Amount (getTournamentWon (), denomination);
      }
   }
   else
   {
      win = ZERO_AMOUNT;
   }
   #ifdef __VERBOSE_METERS__
   Amount::Extractor amount_extractor (win);
   debug << game_index << ":" << stage_index << "|GamePresentation::winDisplayAmount = " << amount_extractor.extract (denomination) << endl;
   #endif
}

// Purpose: Return true if the current game win has been cashed out (because of a handpay, etc).
bool GamePresentation::winWasCashedOut (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   return (essential_record && essential_record->winWasCashedOut ());
}


////////////////////////////////////////////////////////////////////////////
/////////////////////  Serializer Support  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Purpose: Destroy the object that contains and serializes the essential record data.  Allow derived
// classes to override if necessary.
void GamePresentation::destructEssentialRecord (void)
{
   // Delete/deconstruct the essential record object.
   if (essential_record_ptr)
      essential_record_ptr->deleteThisObject (); // deleteThisObject () should be overridden by derived essential rec classes.
   essential_record_ptr = 0;
}


// Purpose: Destroy the object that contains and serializes the essential record data.  Allow derived
// classes to override if necessary.
void GamePresentation::destructSharedEssentialRecord (void)
{
   // Delete/deconstruct the essential record object.
   if (shared_essential_record_ptr)
      shared_essential_record_ptr->deleteThisObject (); // deleteThisObject () should be overridden by derived essential rec classes.
   shared_essential_record_ptr = 0;
}


// Purpose: Used by a Synchron class to notify this class that Synchron has been constructed.
// This class can then (decide) to load/unserialize.
void GamePresentation::synchron_constructed (State * _state_ptr, bool load_object_data)
{
   // Forward call to to ER.
   essential_record_ptr->synchron_constructed (_state_ptr, load_object_data);
   shared_essential_record_ptr->synchron_constructed (_state_ptr, load_object_data);
}

// Purpose: Used by a Synchron class to notify this class that a Synchron has been constructed.
// This class can then (decide) to load/unserialize.
void GamePresentation::synchron_destructed (bool save_object_data)
{
   // Forward call to to ER.
   assertion(!save_object_data);
   essential_record_ptr->synchron_destructed (save_object_data);
   shared_essential_record_ptr->synchron_destructed (save_object_data);
}

// Purpose: Return the betting mode for the given game mode
BetInterface::BetMode GamePresentation::betModeForGameMode (GameMode game_mode)
{
   BetInterface::BetMode bet_mode = BetInterface::BET_MODE_INVALID;
   switch (game_mode)
   {
      case GAME_MODE_UTILITY:
         bet_mode = BetInterface::BET_MODE_TEST;
         break;
      case GAME_MODE_PLAY:
         bet_mode = BetInterface::BET_MODE_GAME_PLAY;
         break;
      case GAME_MODE_HISTORY:
      case GAME_MODE_UNKNOWN:
      default:
         break;
   }
   return bet_mode;
}


// Purpose: This stage needs to switch games (i.e. game id's, paytable, etc.)
bool GamePresentation::switchGame (State &state,
      const char *new_registry_filename,
      const char *new_safe_store_directory,
      const char *new_essential_record_sub_directory, const char *new_shared_essential_record_sub_directory,
         const char *new_er_file_name, const char *new_bet_er_file_name, const char *new_award_er_file_name,
      const char *new_game_config_mgr_identifier,
         const char *new_theme_config_mgr_identifier, const char *new_stage_config_mgr_identifier,
      GameIndex new_game_index, StageIndex new_stage_index, ActIndex new_act_index, const GameIdentifier &new_game_identifier,
      StageIndex new_initiating_stage, ActIndex new_initiating_act_index,
      const Denomination& new_denom,
      const char *new_bet_so_filename, const char *new_bet_object_name,
      const char *new_award_so_filename, const char *new_award_object_name,
      const char* new_paytable_file_name,
      const char *new_evaluator_so_filename, const char *new_evaluator_object_name,
      bool new_enrollment_required, bool new_enrollment_before_game_start,
      const char *new_mount_point)
{
   verboseMethodName();


   #ifdef __VERBOSE_GAME_SWITCH__
   debug << "=================================================================="  << endl;
   debug << "GamePresentation:" << __FUNCTION__ << "   new_game_index: " << new_game_index << "   new_stage_index: " << new_stage_index << endl;
   debug << "   new_registry_filename: " << new_registry_filename << endl;
   debug << "   new_safe_store_directory: " << new_safe_store_directory << endl;
   debug << "   new_essential_record_sub_directory: " << new_essential_record_sub_directory << endl;
   debug << "   new_shared_essential_record_sub_directory: " << new_shared_essential_record_sub_directory << endl;
   debug << "   new_er_file_name: " << new_er_file_name << endl;
   debug << "   new_bet_er_file_name: " << new_bet_er_file_name << endl;
   debug << "   new_award_er_file_name: " << new_award_er_file_name << endl;
   debug << "   new_game_config_mgr_identifier: " << new_game_config_mgr_identifier << endl;
   debug << "   new_theme_config_mgr_identifier: " << new_theme_config_mgr_identifier << endl;
   debug << "   new_stage_config_mgr_identifier: " << new_stage_config_mgr_identifier << endl;
   debug << "   new_game_index: " << new_game_index << endl;
   debug << "   new_stage_index: " << new_stage_index << endl;
   debug << "   new_act_index: " << new_act_index << endl;
   debug << "   new_game_identifier: " << new_game_identifier.asString () << endl;
   debug << "   new_initiating_stage: " << new_initiating_stage << endl;
   debug << "   new_initiating_act_index: " << new_initiating_act_index << endl;
   debug << "   new_denom: " << new_denom.getValue () << endl;
   debug << "   new_bet_so_filename: " << new_bet_so_filename << endl;
   debug << "   new_bet_object_name: " << new_bet_object_name << endl;
   debug << "   new_award_so_filename: " << new_award_so_filename << endl;
   debug << "   new_award_object_name: " << new_award_object_name << endl;
   debug << "   new_paytable_file_name: " << new_paytable_file_name << endl;
   debug << "   new_evaluator_so_filename: " << new_evaluator_so_filename << endl;
   debug << "   new_evaluator_object_name: " << new_evaluator_object_name << endl;
   debug << "   new_enrollment_required: " << new_enrollment_required << endl;
   debug << "   new_enrollment_before_game_start: " << new_enrollment_before_game_start << endl;
   debug << "   new_mount_point: " << new_mount_point << endl;
   #endif


   #ifdef __VERBOSE_GAME_SWITCH__
   DebugTimer debug_timer;
   debug_timer.start ();
   #endif

   bool result = true;

   ////////////////////////////////////////
   // Release all persistant properties
   releasePersistentProperties ();

   ////////////////////////////////////////
   // Get attached to and load critical data/essential record.
   assertion (new_safe_store_directory);
   setSafeStoreDirectory (new_safe_store_directory);
   setEssentialRecordInformation (new_essential_record_sub_directory, new_shared_essential_record_sub_directory,
      new_er_file_name, new_bet_er_file_name, new_award_er_file_name);

   // Load/unserialize the essential record.
   assertion (essential_record.isAttached ());
   const uint8 * buffer = static_cast <const uint8 *> (essential_record.getMemory ());
   essential_record_ptr->unserialize (buffer); // Load/unserialize the essential record.
   debugTime (debug_timer, "unserialize");

   // Determine if we are changing themes.
   bool changing_themes = (strcmp (new_theme_config_mgr_identifier, theme_configuration_identifier) != 0);

   ////////////////////////////////////////
   // switchGameProcessing - change the settings.
   assertion (setConfigurationFile (new_registry_filename));
   assertion (setConfigurationIdentifier (new_game_config_mgr_identifier, new_theme_config_mgr_identifier, new_stage_config_mgr_identifier));
   assertion (setGameIndex (new_game_index, new_stage_index, new_act_index, new_game_identifier));
   assertion (setInitiatingInformation (new_initiating_stage, new_initiating_act_index));
   assertion (setMountPoint (new_mount_point));

   ////////////////////////////////////////
   // Parse the switch game script
   parseGameSwitchScript ();

   if (bet.isValid())
   {
      if (changing_themes)
      {
         /////////////////////////////////////////////////////
         // The theme changed, so the bet configuration
         // and bet's critical data state must be reloaded.  The only way
         // to do that right now is to release the bet and recreate/re-script it.
         assertion (new_bet_so_filename && new_bet_so_filename [0] != '\0');  // We are going to recreate the bet, so we need this
         assertion (new_bet_object_name && new_bet_object_name [0] != '\0');  // We are going to recreate the bet, so we need this

         if (doesBetSupportGameSideBetInterface ())
         {
            // Remove the bet object from any lists it might be in (i.e. config id change notification list).
            removeFromLists (bet);
         }

         // The release () method returns the number of references remaining.  Assert that we have the last reference to
         // the bet, so we are sure that the bet object is being destroyed.
         assertion (bet.release () == 0);         // Release/delete the bet object.

         // Create a new bet object.
         assertion (setBetInformation (new_bet_so_filename, new_bet_object_name)); // Will recreate and init the bet object.
         debugTime (debug_timer, "setBetInformation");
         assertion (bet.isValid ());

         // Re-script the new bet object.
         if (bet_script.length ())
         {
            executeBetScript (bet_script);
            debugTime (debug_timer, "executeBetScript");
         }
         else
         {
            postEvent ("ExecuteBetScript");
         }

         bet->setBetMode (betModeForGameMode (current_game_mode));
      }
      else
      {
         /////////////////////////////////////////////////////
         // Update the bet's game identifier information.  These values are mainly used to pass
         // messages/events and need to know where to send them. The rest of the bet information should be
         // valid as long as we are not changing the current theme.

         // We need these to update the game identifier.
         assertion (new_theme_config_mgr_identifier && new_theme_config_mgr_identifier [0] != '\0');
         assertion (new_stage_config_mgr_identifier && new_stage_config_mgr_identifier [0] != '\0');
         assertion (new_game_identifier.asString ());
         if (!bet->setGameIdentifier (new_theme_config_mgr_identifier, new_stage_config_mgr_identifier, new_game_identifier.asString (), new_stage_index))
         {
            System::error (LogicError (LogicError::INVALID_PARAMETER, "GamePresentation::switchGame: Theme index does not match."));
         }
         else if (!doesBetSupportGameSideBetInterface ())
         {
            // Rule based game bet need to update the button map when the game identifier is changed.
            // You can get more infomation from QC80529.
            postEvent("GameIdentifierChanged");
         }
      }
   }

   if (getPaytableFilename () != new_paytable_file_name)
   {
      postEvent("Paytable:Changed");
   }

   setDenomination (new_denom);
   assertion (setAwardInformation (new_award_so_filename, new_award_object_name));
   assertion (setPaytableFilename (new_paytable_file_name));
   assertion (setEvalInformation (new_evaluator_so_filename, new_evaluator_object_name));
   assertion (setGameEnrollmentInformation (new_enrollment_required, new_enrollment_before_game_start));

   /////////////////////////////////////////////////////
   // Recreate the group configuration items and initialize them.
   config_group.clear ();
   uint32 number_of_config_items = 0;
   const char *const *current_item = configItems (number_of_config_items);
   for (uint32 i = 0; i < number_of_config_items; ++i)
      configurationChanged (config_group.addItem (current_item [i], game_configuration_identifier));

   /////////////////////////////////////////////////////
   // Recreate the evaluators for the (old) section names from the new paytable.
   evaluator.release ();

   // Save the old evaluator list temporarily (we need the section names), and clear the
   // evaluator list (by using swap ()).
   EvaluatorList old_evaluator_list;
   swap (old_evaluator_list, evaluator_list);   // clears (global to class) evalutor_list.

   // creates new paytable parser, and (default) evaluator for this stage.
   loadPaytable ();

   // Go through old evaluator list getting the section names and creating new evaluators for the
   // same sections from the new paytable.
   EvaluatorIterator old_evaluator_list_iter (old_evaluator_list.begin ());
   for (; old_evaluator_list_iter != old_evaluator_list.end (); ++old_evaluator_list_iter)
      supportPaytableSection (old_evaluator_list_iter->first); // "first" is the section name.*/

   /////////////////////////////////////////////////////
   // Re-set the current paytable section.
   String temp_current_paytable_section = current_paytable_section;
   current_paytable_section.clear ();
   if (temp_current_paytable_section.length ())
      setCurrentPaytableSection (temp_current_paytable_section);

   /////////////////////////////////////////////////////
   // Allow for derived classes to update their specific info.
   updateAfterPaytableSwitch ();
   debugTime (debug_timer, "Presentation timer");

   return result;
}

// Construct the object that will contain and serialize the shared essential record data.  Allow derived
// classes to override if necessary.
void GamePresentation::constructSharedEssentialRecord (void)
{
   verboseMethodName ();

   // Destroy the current shared essential record before constructing a new one.
   destructSharedEssentialRecord ();

   // Create the object that contains essential record/critical data.
   shared_essential_record_ptr = new SharedEssentialRecord;

   // Identify the class/type of the essential record for the serialize/unserialize functions.
   // It is recommened that derived classes of essential record use an id that is different from any
   // of its base classes (for error checking purposes).
   assertion (shared_essential_record_ptr);
}


// Purpose: Given the progressive levels currently linked, assign these levels to virtual
//          spectrum levels. For example:
//
//   Let's say a game has two possible progressive levels, and each is assigned and linked to a
//   different denom,

//   1 cent, level0:                            2 cent, level1:
//   -----------------------                    -----------------------
//   | ******************* |                    | ******************* |
//   | *      $1,000     * |                    | *      $5,000     * |
//   | ******************* |                    | ******************* |
//   |                     |                    |                     |
//   |                     |                    |                     |
//   |                     |                    |                     |
//   |                     |                    |                     |
//   |_____________________|                    |_____________________|
//
//   The virtual spectrum properties that contain the values that need to be displayed are
//   found as such:
//          Spectrum:DisplayAmount:ValueString:Level:0
//          Spectrum:DisplayAmount:ValueString:Level:1
//
//   In this case though, they are using the same location on the digital glass to be
//   displayed, only for different denoms.  To remedy this, we create an association table
//   telling the presentation which string to put into which virtual spectrum meter.
void GamePresentation::assignVirtualSpectrumLevelAssociations (void)
{
   // For each progressive game level, check if linked, and assign to next virtual spectrum level.
   ProgressiveGameId progressive_game_id (game_identifier, stage_index);
   Denomination denomination = getDenomination();
   uint32 number_of_levels = progressive_manager.numberOfLevels (progressive_game_id, denomination);
   uint32 current_virtual_spectrum = 0;

   virtual_spectrum_associations.clear();

   for (uint32 level_index = 0; level_index < number_of_levels; ++level_index)
   {
      ProgressiveControllerId controller_id;
      Denomination controller_denom = Denomination::BLANK;
      uint32 controller_level;
      bool is_paytable_formed;
      uint32 list_position = 0;
      bool linked = progressive_manager.getGameLink (progressive_game_id, level_index, controller_id,
                                                     controller_level, is_paytable_formed, list_position,
                                                     denomination, controller_denom);

      if (linked)
      {
         VirtualSpectrumAssociation new_association;

         new_association.linked_progressive_game_level = level_index;
         new_association.virtual_spectrum_level = current_virtual_spectrum;

         virtual_spectrum_associations.push_back(new_association);
         current_virtual_spectrum++;
      }
   }
}

//Purose: Update the Max bet property in script
void GamePresentation::updateMaxBetProperty(void)
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
      // generally means that the resource has not been registered with the resource manager.
      if (resource_list->getResource<PropertyInterface> (GAME_MAX_BET_VALUE, resource_list->resourceKey (), property, false))
      {
         String event (GAME_MAX_BET_VALUE);
         event << "Updated";
         Mojo <GameSideBetInterface> game_side_bet (bet);

         if (game_side_bet.isValid ())
         {
            property->setAbsoluteValue(game_side_bet->getMaxBetValue());
         }
         else
         {
            Mojo <RuleBasedGameSideBetInterface> rule_based_game_bet (bet);
            if (rule_based_game_bet.isValid ())
            {
               property->setAbsoluteValue(rule_based_game_bet->getMaxBetValue());
            }
         }
         postEvent (event);
      }
   }
}

// Purpose: Check to see if we need to inform that there was a lose in double up
bool GamePresentation::checkDoubleUpLoss (void)
{
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   // Search the final award list for a #DoubleUpWin to see if we need to rollup the win
   uint16 number_of_awards = local_award->getNumberOfAwards ();
   Mojo<SystemAwardInterface> system_award;
   bool final_award_double_up_lose = false;

   // The last award in the list should be a win if we are going to post a rollup message
   if (local_award->getAward ((number_of_awards - 1), system_award))
   {
      const char *name = system_award->getName ();

      // This checks to see if a double up win was the last action...
      // If it wasn't, then we don't have an award to rollup
      if ((strcmp(name, "#DoubleUpLoss") == 0))
      {
         final_award_double_up_lose = true;
      }
   }
   return final_award_double_up_lose;
}

// Purpose: Check to see if we need to inform that there was a win in double up
//          It returns false if double up was entered and the player didn't double up.
bool GamePresentation::doubleUpWin (void)
{
   #ifdef __VERBOSE_DOUBLEUP__
   debug << "GamePresentation::doubleUpWin" << endl;
   #endif
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);
   // Search the final award list for a #DoubleUpWin to see if we need to rollup the win
   uint16 number_of_awards = local_award->getNumberOfAwards ();
   Mojo<SystemAwardInterface> system_award;
   bool double_up_win = false;

   // The last award in the list should be a win if we are going to post a rollup message
   if (local_award->getAward ((number_of_awards - 1), system_award))
   {
      const char *name = system_award->getName ();
      // This checks to see if a double up win was the last action...
      // If it wasn't, then we don't have an award to rollup
      if ((strcmp(name, "#DoubleUpWin") == 0))
      {
         double_up_win = true;
      }
   }
   #ifdef __VERBOSE_DOUBLEUP__
   debug << "  double_up_win = " << (double_up_win ? "true" : "false") << endl;
   #endif

   return double_up_win;
}

// Purpose: To determine whether the language configs are supported by the game
bool GamePresentation::languageConfigsSupported (void)
{
   return ((language_licensed_bitfield_config_id != INVALID_CONFIGID) && (language_supported_list_config_id != INVALID_CONFIGID));
}

//Purose: This method updates the language swap button.
void GamePresentation::changeGameLanguage (void)
{
   #ifdef __VERBOSE_LANGUAGE__
   debug << "GamePresentation::changeGameLanguage" << endl;
   #endif

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
         String current_language,new_language;
         config.getValueString (game_language_config_id, current_language);
         for (uint32 language_list_index = 0; language_list_index < language_supported_list_size; ++language_list_index)
         {
            if (language_bitfield.getBit (language_list_index))
            {
               config.getListItem (language_supported_list_config_id, language_list_index, new_language);
               if (new_language != current_language)
               {
                  // Disable control panels during swapping of language resources
                  enableControlPanels (false);

                  postEvent ("LanguageSwap:Start");

                  // Update the language config with the new language
                  config.setValue (game_language_config_id, new_language);
                  break;
               }
            }
         }
      }
   }
}

// Purpose: Check if the language button can be enabled
bool GamePresentation::canEnableLanguage (void)
{
   bool can_enable (false);

   // Enable the language button only at idle
   can_enable = (getFlowState () == GameStateValue::STATE_IDLE);

   return can_enable;
}

// Purpose: To force enable/disable the autoplay button
void GamePresentation::enableAutoplayButton (bool new_state)
{
   // Update the control panel language button
   GameControlPanelExtractor game_control_panel;

   postEvent ("AutoPlayStopButton:Disable");
   if (controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_AUTOPLAY, new_state);
   }
}

// Purpose: To force enable/disable the autoplay stop button
void GamePresentation::enableAutoplayStopButton (bool new_state)
{
   // Update the control panel language button
   GameControlPanelExtractor game_control_panel;

   if (controlPanel (game_control_panel))
   {
      if (new_state)
      {
         game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_AUTOPLAY, !new_state);
         postEvent ("AutoPlayStopButton:Enable");
      }
   }
}

// Purpose: To force enable/disable the language button
void GamePresentation::enableLanguageButton (bool new_state)
{
   // Update the control panel language button
   GameControlPanelExtractor game_control_panel;

   if (controlPanel (game_control_panel))
   {
      game_control_panel.getGameControlPanelInterface ()->enableButton (GameControlPanel::BUTTON_LANGUAGE, new_state);
   }
}

// Purpose: Verify that the language button exists in "base game" presentations
bool GamePresentation::languageButtonIsValid (void)
{
   bool is_valid (false);

   // The stage that _must_ suport the language button is the one that supports the "base game" paytable section.
   // If the current paytable section is "base game", then the current active stage's interface panel must have a
   // valid language button actor interface.
   if (strcmp ("base game", currentPaytableSection ()) == 0)
   {
      // Update the control panel language button
      GameControlPanelExtractor game_control_panel;
      if (controlPanel (game_control_panel))
         is_valid = game_control_panel.getGameControlPanelInterface ()->buttonIsValid (GameControlPanel::BUTTON_LANGUAGE);
   }
   else
      is_valid = true;

   return is_valid;
}

/// <summary>For multiple-language game, synchronize the game's current language.</summary>
/// <remarks>This is needed for bonus presentation.</remarks>
/// <param name = "force_sync">[in] Boolean flag to manually force the synchronization.</param>
void GamePresentation::synchronizeGameLanguage (bool force_sync)
{
   #ifdef __VERBOSE_LANGUAGE__
   debug << "GamePresentation::synchronizeGameLanguage" << endl;
   #endif

   if (language_supported || force_sync)
   {
      multiple_languages_available_for_player = false;
      // Ensure we have valid config ID's before continuing and presentation allows language button.
      if (languageConfigsSupported () && allow_language_button)
      {
         // Holds the enabled licensed languages bitfield
         BitField language_bitfield;

         // Holds the current index in the "LICENSED LANGUAGE TYPES SUPPORTED LIST".
         uint32 language_list_index = 0;

         // Get the size of the "LICENSED LANGUAGE TYPES SUPPORTED LIST".
         uint32 language_supported_list_size (config.getListSize (language_supported_list_config_id));

         // Get licensed languages bit field
         config.getValueBitField (language_licensed_bitfield_config_id, language_bitfield);

         // Get the current language.
         String current_language, new_language, temp_language;
         config.getValueString (game_language_config_id, current_language);
         // If only one, non-English language is licensed make sure it gets set.
         if (language_bitfield.numberOfBitsSet() == 1)
         {
            // Only one bit set in the bit field, so the highest bit set must be the licensed language.
            uint32 highest_bit_index = 0;

            // Get the licensed language.  There should only be one bit set in the language_bitfield.
            language_bitfield.highestBitSet(highest_bit_index);
            config.getListItem (language_supported_list_config_id, highest_bit_index, new_language);

            if ((new_language != "") && (current_language != new_language))
            {
               for (language_list_index = 0; language_list_index < language_supported_list_size; language_list_index++)
               {
                  config.getListItem (language_supported_list_config_id, language_list_index, temp_language);

                  // If the current language is a fully translated language
                  if (current_language == temp_language)
                  {
                     config.setValue (game_language_config_id, new_language);
                     postEventAll (String ("CurrentLanguage:") + new_language);
                     break;
                  }
                  else if (language_list_index == language_supported_list_size)
                  {
                     // Digital glass swap, so just post the current set digital glass language.
                     postEventAll (String ("CurrentLanguage:") + current_language);
                  }
               }
            }
            else
            {
               if (current_language != "")
                  postEventAll (String ("CurrentLanguage:") + current_language);
            }
         }
         // Turn the language button on if more than one language is supported and licensed.
         else if ((language_supported_list_size > 1) && (language_bitfield.numberOfBitsSet () > 1))
         {
            // Verify that a language button exists in the interface before enabling it
            if (languageButtonIsValid ())
            {
               // Post event used for multi-language FS positioning.
               if (current_language != "")
               {
                  postEventAll (String ("CurrentLanguage:") + current_language);
               }

               // Update language button accordingly to the amount of licensed games.
               enableLanguageButton (canEnableLanguage () && !checkForDoubleUpEnable ());
               multiple_languages_available_for_player = true;
               language_supported = true;  // Sync flag here in case force_sync is passed to this routine

               for (language_list_index = 0; language_list_index < language_supported_list_size; ++language_list_index)
               {
                  if (language_bitfield.getBit (language_list_index))
                  {
                     config.getListItem (language_supported_list_config_id, language_list_index, new_language);
                     if ((new_language != "") && (new_language != current_language))
                     {
                        postEvent (String ("LanguageSwapDisplayPressFor:") + new_language);
                        break;
                     }
                  }
               }
            }
            else
               System::error (LogicError (LogicError::INVALID_VALUE, "GamePresentation::synchronizeGameLanguage (), language button not found in model."));
         }
      }

      if (multiple_languages_available_for_player)
      {
         if (checkForDoubleUpEnable ())
         {
            // disable_language_button_event can be used to post events to:
            // 1) "hide" - Double Up Button and Language Button share same screen dimensions or
            // 2) "dim" - if Double Up and Language Button have separate screen dimensions.
            postEvent (disable_language_button_event);
         }
         else
         {
            postEvent ("LanguageSwap:Supported");
         }
      }
      else
      {
         enableLanguageButton (false);
         postEvent ("LanguageSwap:NotSupported");
         language_supported = false;
      }
   }
}

// Purpose: Event posted that disables the language button event. This is needed when the double up button is
//          present in the following cases:
//          1. Double up button and language button are using the same on-screen real estate (want it hidden)
//          2. Double up button and language button are _not_ using hte same on-screen real estate (want it dim'ed)
void GamePresentation::postDisableLanguageButtonEvent (void)
{
   if (disable_language_button_event.length () > 0)
      postEvent (disable_language_button_event);
}

// Purpose: Maintain Autoplay button presentation syncronization for any changes in states.
void GamePresentation::updateAutoplayButtonPresentation (bool force_sync)
{
   if (player_autoplay_supported || force_sync)
   {
      String value_string;
      config.getValueString (who_can_start_autoplay_config_id, value_string);

      bool enable_autoplay_button (false);
      bool hide_button (false);
      // if auto play is player enabled and presentation allows auto play button
      if ((value_string == "PLAYER" || value_string == "PLAYER 2 TOUCH") && allow_autoplay_button)
      {
         // This flag is set here in the case we're using force_sync to sync the flag too
         player_autoplay_supported = true;

         // Player Enabled event is an event that shows the autoplay button
         postEvent ("AutoPlayButton:PlayerEnabled");

         String autoplay_state_value;
         config.getValueString (autoplay_state_config_id, autoplay_state_value);

         if (autoplay_state_value == "DISABLED")
         {
            if(canPlayAutoPlay ())
            {
               enable_autoplay_button = true;
            }
         }
      }
      else if (value_string == "HOST")
      {
         postEvent ("AutoPlayButton:HostEnabled");
         hide_button = true;
         player_autoplay_supported = false;
      }
      else
      {
         hide_button = true;
         player_autoplay_supported = false;
      }

      if (enable_autoplay_button)
      {
         enableAutoplayButton (true);
      }
      else
      {
         enableAutoplayButton (false);
         if (hide_button)
            postEvent ("AutoPlayButton:None");
      }
   }
}

// Purpose: Maintain Autoplay button presentation synchronization for any changes in states.
void GamePresentation::synchronizeAutoplayButtonPresentation (bool force_sync)
{
   #ifdef __VERBOSE_AUTOPLAY__
   debug << "GamePresentation::synchronizeAutoplayButtonPresentation" << endl;
   #endif

   if (player_autoplay_supported || force_sync)
   {
      // If presentation allows auto play button.
      if (allow_autoplay_button)
      {
         String autoplay_state_value;
         config.getValueString (autoplay_state_config_id, autoplay_state_value);

         switch (getFlowState ())
         {
            case GameStateValue::STATE_IDLE:
               {
                  if (autoplay_state_value == "DISABLED")
                  {
                     updateAutoplayButtonPresentation (force_sync);
                  }
                  else if (autoplay_state_value == "RUNNING")
                  {
                     if(canPlayAutoPlay ())
                     {
                        enableAutoplayStopButton (true);
                     }
                     else
                     {
                        enableAutoplayButton (true);
                     }
                  }
               }
               break;

            case GameStateValue::STATE_PLAY:
               {
                  if (autoplay_state_value == "DISABLED")
                  {
                     enableAutoplayButton (false);
                  }
                  else if (autoplay_state_value == "RUNNING")
                  {
                     //During play if auto play is running there is no reason to not allow the
                     //player to issue the stop
                     enableAutoplayStopButton (true);
                  }
               }
               break;

            default:
               break;
         }
      }
      else
      {
         // Turn off auto play button if presentation doesn't allow auto play button.
         enableAutoplayButton (false);
         postEvent ("AutoPlayButton:None");
      }
   }
}

// Purpose: Returns the number of replays of the same paytable section the current
//          act has awarded.
//          Returns 0 if the award is not retriggering the same paytable section.
int32 GamePresentation::awardIndicatesReplay (void)
{
   int32 count = 0;
   Mojo<AwardInterface> local_award;
   Mojo<SystemAwardInterface> system_award;
   if (getCurrentAward (local_award))
   {
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         if (local_award->getAward (award_index, system_award))
         {
            uint16 number_of_system_awards = system_award->getNumberOfAwards ();
            for (uint16 system_index = 0; system_index < number_of_system_awards; ++system_index)
            {
               // Determine if the award (in the award list) is our award or another award
               // (which would be an earlier award that eventually ended up triggering our paytable section)
               if (system_award->getTransactionId () == actIndex ())
               {
                  for (uint16 trigger_index = 0; trigger_index < system_award->getNumberOfTriggers (system_index); trigger_index++)
                  {
                     const char *game_stage_trigger = system_award->getTriggerString (system_index, trigger_index);
                     if (game_stage_trigger && game_stage_trigger[0] == TRIGGER_PREFIX_RETRIGGER)
                     {
                        // Strip the non-bonus free game retrigger prefix from the stage name
                        ++game_stage_trigger;
                     }
                     // If the trigger is the same section we are on, then act won our paytable section
                     // That is a replay/retrigger
                     if (game_stage_trigger && strcmp(game_stage_trigger, currentPaytableSection ()) == 0)
                     {
                        int32 trigger_count = system_award->getTriggerCount (system_index, trigger_index);
                        count += trigger_count;
                     }
                  }
               }
            }
         }
      }
   }
   return count;
}

// Purpose: Check if double up is permitted to run
bool GamePresentation::doubleUpPermitted (void)
{
   const EssentialRecord *essential_record = getEssentialRecord ();
   return essential_record->doubleupPermitted ();
}

//Check to see if Double Up is enabled
bool GamePresentation::doubleUpEnabled (void)
{
    Config config;

    bool double_up_enabled(false);

   //use value from essential record if we are in history otherwise use the config item.
   if(currentGameMode () == GAME_MODE_HISTORY)
   {
      const EssentialRecord *essential_record = getEssentialRecord ();

      if (!essential_record)
      {
         String error_msg = String ("Essential Record Invalid");
         System::error (LogicError (LogicError::INVALID_VALUE, error_msg));
      }

      double_up_enabled = essential_record->doubleupEnabledThisGame ();
   }
   else
   {
      ConfigID id (config.getId ("DOUBLE UP ENABLED", game_configuration_identifier, false));
      if (id != INVALID_CONFIGID)
      {
         double_up_enabled = config.getValue (id);
      }
   }
   return double_up_enabled;
}

// Purpose: Check to see if machine is running Bingo
bool GamePresentation::isBingo (void)
{
   verboseMethodName ();

   Config config;
   ConfigID bingo_id = config.getId (CONFIG_ID_BINGO_SYSTEM_GAMING, false);
   bool is_bingo (false);

   if (bingo_id != INVALID_CONFIGID)
   {
      is_bingo = config.getValue (bingo_id);
   }
   return is_bingo;
}

// Return the easy bet configuration (disabled, 5 button, 10 button)
String GamePresentation::getEasyBetConfiguration (void) const
{
   Config config;
   String easy_bet_configuration(EASY_BET_BOOLEAN_FALSE_STRING);

   ConfigID id = config.getId (CONFIG_ID_EASY_BET_ENABLED, theme_configuration_identifier, false);

   if (id != INVALID_CONFIGID)
   {
      switch (config.getType (id))
      {
         // This is the legacy case.  The options are 5 button or disabled.
         case CONFIG_TYPE_BOOLEAN:
         {
            bool easy_bet_config_value = config.getValue (id);
            if (easy_bet_config_value)
            {
               easy_bet_configuration = EASY_BET_BOOLEAN_TRUE_STRING;
            }
            break;
         }
         case CONFIG_TYPE_ITEM:
         {
            config.getValueString (id, easy_bet_configuration);
            break;
         }
         default:
            System::error (LogicError(LogicError::INVALID_VALUE, "GameBet:Unsupported easy bet config type"));
            break;
      }
   }
   return easy_bet_configuration;
}

// Return the minimum total bet value for the current button map.
uint32 GamePresentation::getMinBetButtonValue (void) const
{
   uint32 return_value = 0;
   
   // Get a rule based game-side bet interface
   Mojo <RuleBasedGameBetLegacyVariableInterface> rule_based_game_bet_legacy (bet);
   if (rule_based_game_bet_legacy.isValid ())
   {
      Mojo <RuleBasedGameSideBetInterface> rule_based_game_side_bet (bet);
      String min_bet_button_variable_name = rule_based_game_bet_legacy->getMinBetInCurButtonMapVariableName ();
      if (rule_based_game_side_bet.isValid () && min_bet_button_variable_name.length () != 0)
      {
          return_value = rule_based_game_side_bet->getBetVariableValue (min_bet_button_variable_name.c_str ());
      }
   }
   return return_value;
}

void GamePresentation::getDisplayableBingoGrandPrizeAmount (Amount &bingo_grand_prize)
{
   Mojo <AwardInterface> local_award;
   readAwardFromEssentialRecord (local_award);

   bingo_grand_prize = ZERO_AMOUNT;
   if (local_award.isValid ())
   {
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         Mojo <SystemAwardInterface> game_award;
         // Pointer to SystemAward.
         if (local_award->getAward (award_index, game_award))
         {
            if (game_award->getAwardType () == SystemAwardInterface::BINGO)
            {
               Mojo<BingoAwardInterface> bingo_award;
               bingo_award = game_award;
               // Find Bingo Grand Prize amounts
               uint16 number_of_awards_items  = bingo_award->getNumberOfAwards ();
               for (uint16 awards_item_index = 0; awards_item_index < number_of_awards_items; ++awards_item_index)
               {
                  // The progressive amount is not contained in the getAmount, so dig further for it.
                  uint16 number_of_progressive_hits = game_award->getNumberOfPossibleProgressiveHits (awards_item_index);
                  for (uint16 progressive_hit_index = 0; progressive_hit_index < number_of_progressive_hits; ++progressive_hit_index)
                  {
                     // only add displayable grand prize amounts.
                     if (game_award->getIsAmountDisplayable (awards_item_index))
                     {
                        bingo_grand_prize += game_award->getProgressiveDefaultPrizeAmount (awards_item_index, progressive_hit_index);
                     }
                  }
               }
            }
         }
      }
   }
}

bool GamePresentation::canPlayAutoPlay(void)
{
   bool canPlay = false;
   if (bet.isValid ())
   {
      Amount total_bet_amount = bet->getBetAmount ();
      Amount credit_amount (ZERO_AMOUNT);
      wagerCreditDisplayAmount (credit_amount);

      if (credit_amount.isNotZero () && total_bet_amount.isNotZero () && (credit_amount >= total_bet_amount))
      {
         canPlay = true;
      }
   }
   return canPlay;
}

/// <summary>Can player start auto play?</summary>
/// <returns>True if player can start auto play, false otherwise</returns>
bool GamePresentation::canPlayerStartAutoPlay (void)
{
   String value_string = "";
   config.getValueString (who_can_start_autoplay_config_id, value_string);

   return ((value_string == "PLAYER" || value_string == "PLAYER 2 TOUCH") ? true : false);
}

// Purpose: Is Auto Play enabled for this game?
bool GamePresentation::isAutoPlayEnabled (void)
{
   return (allow_autoplay_button && canPlayerStartAutoPlay ());
}

// Purpose: Allow auto play button to be shown/Hide for this presentation.
// If set to true: auto play button is allowed, still have to pass config rules for this button to be shown.
// If set to false: auto play button will be hidden/disabled.
void GamePresentation::allowAutoPlayButton (bool allow_button)
{
   allow_autoplay_button = allow_button;
}

// Purpose: Allow language button to be shown/Hide for this presentation.
// If set to true: language button is allowed, still have to pass config rules for this button to be shown.
// If set to false: language button will be hidden/disabled.
void GamePresentation::allowLanguageButton (bool allow_button)
{
   allow_language_button = allow_button;
}


// Purpose: Is there multiple languages available for player.
bool GamePresentation::isMultiLingualAvailableForPlayer (void)
{
   return (multiple_languages_available_for_player);
}

bool GamePresentation::isGameLicenseRequired (void)
{
   bool license_required = false;
   ConfigID license_required_id = config.getId ("LICENSE REQUIRED", theme_configuration_identifier, false);
   if (license_required_id != INVALID_CONFIGID)
      license_required = config.getValue(license_required_id);
   return license_required;
}

bool GamePresentation::isThemeLicensed (void)
{
   //Always assume license is enabled, System will take care of unlicensed games.
   return true;
}

void GamePresentation::updateGameThemeLicenseStatus (void)
{
   if (isGameLicenseRequired() && !isThemeLicensed())
      game_theme_is_unlicensed = true;
    else
      game_theme_is_unlicensed = false;
}

bool GamePresentation::isGameThemeUnlicensed (void)
{
   return game_theme_is_unlicensed;
}

/// <summary>Get the ER values and create a payload.</summary>
/// <param name="payload">[out] The ScriptTableParameter to store the data in.</param>
void GamePresentation::getERData (ScriptTableParameter &payload)
{
   uint16 play_count = 0;
   uint16 total_games = 0;
   playCountStatus (play_count, total_games);

   payload.script_table ["play_count"]              = ScriptParameter (play_count, ParameterType::UINT16);
   payload.script_table ["total_games"]             = ScriptParameter (total_games, ParameterType::UINT16);
   payload.script_table ["is_free_act"]             = ScriptParameter (processConditionIsActType ("FREE"), ParameterType::BOOL);
   payload.script_table ["is_double_up_permitted"]  = ScriptParameter (doubleUpPermitted (), ParameterType::BOOL);
   payload.script_table ["is_double_up_enabled"]    = ScriptParameter (doubleUpEnabled (), ParameterType::BOOL);
   payload.script_table ["is_double_up_win"]        = ScriptParameter (doubleUpWin (), ParameterType::BOOL);
   payload.script_table ["win_was_cashed_out"]      = ScriptParameter (winWasCashedOut (), ParameterType::BOOL);
   getAccountingData (payload);
}

/// <summary>Get the theme configuration identifier string.</summary>
/// <returns>String containing a unique indentifier for the theme configuration file</returns>
/// @scriptableFunction{GamePresentation,getThemeConfigurationIdentifier}
String GamePresentation::getThemeConfigurationIdentifier ()
{
   return theme_configuration_identifier;
}

/// <summary>Get the stage configuration identifier string.</summary>
/// <returns>String containing a unique indentifier for the stage configuration file</returns>
/// @scriptableFunction{GamePresentation,getStageConfigurationIdentifier}
String GamePresentation::getStageConfigurationIdentifier ()
{
   return stage_configuration_identifier;
}

/// <summary>Get the game configuration identifier string.</summary>
/// <returns>String containing a unique indentifier for the game configuration file</returns>
/// @scriptableFunction{GamePresentation,getGameConfigurationIdentifier}
String GamePresentation::getGameConfigurationIdentifier ()
{
   return game_configuration_identifier;
}

// Purpose: Determine if the award list for the current act contains runnable triggers.
//          A runnable trigger must have a count greater than 0.
//          Returns:
//              True:  if triggers with positive counts were found.
//              False: if no triggers or only triggers with negative counts were found.
bool GamePresentation::currentAwardContainsNewRunnableTriggers (void)
{
   int32 count = 0;
   Mojo<AwardInterface> local_award;
   Mojo<SystemAwardInterface> system_award;
   if (getCurrentAward (local_award))
   {
      uint16 number_of_awards = local_award->getNumberOfAwards ();
      for (uint16 award_index = 0; award_index < number_of_awards; ++award_index)
      {
         if (local_award->getAward (award_index, system_award))
         {
            uint16 number_of_system_awards = system_award->getNumberOfAwards ();
            for (uint16 system_index = 0; system_index < number_of_system_awards; ++system_index)
            {
               // Determine if the award (in the award list) is our award or another award
               // (which would be an earlier award that eventually ended up triggering our paytable section)
               if (system_award->getTransactionId () == actIndex ())
               {
                  for (uint16 trigger_index = 0; trigger_index < system_award->getNumberOfTriggers (system_index); trigger_index++)
                  {
                     const char *game_stage_trigger = system_award->getTriggerString (system_index, trigger_index);
                     if (game_stage_trigger != 0)
                     {
                        if (game_stage_trigger[0] == TRIGGER_PREFIX_RETRIGGER)
                        {
                           // Strip the non-bonus free game retrigger prefix from the stage name
                           ++game_stage_trigger;
                        }
                        // If the trigger is the same section we are on, then act won our paytable section
                        // That is a replay/retrigger
                        if (strcmp(game_stage_trigger, currentPaytableSection ()) == 0)
                        {
                           int32 trigger_count = system_award->getTriggerCount (system_index, trigger_index);
                           if (trigger_count > 0)
                           {  // Only account for new triggers that the GameManager will be able to run...
                              // Negative trigger count are used to remove from existing triggers.
                              count += trigger_count;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
   return (count > 0);
}

/// <summary> Determine if the bet object supports the GameSideBetInterface. </summary>
/// <returns> Return true, if the bet support GameSideBetInterface, else return false. </returns>
bool GamePresentation::doesBetSupportGameSideBetInterface ()
{
   bool is_game_bet = false;
   Mojo <GameSideBetInterface> game_side_bet (bet);
   if (game_side_bet.isValid ())
   {
      is_game_bet = true;
   }
   return is_game_bet;
}

/// <summary>Sets the credit meter to display the amount in cash if the credit meter display behavior is set to "CURRENCY".</summary>
/// <remarks>Only force the credit meter to display cash if the game is not active when a check on game activity status is required.</remarks>
/// <param name = "check_game_activity_status">[in] Indicates that a check on game activity status is required.</param>
void GamePresentation::setCreditMeterDisplayBehavior (bool check_game_activity_status)
{
   // gets the credit meter display behavior without erroring.
   ConfigID config_id = config.getId ("CREDIT METER DISPLAY BEHAVIOR", false);
   bool set_behavior = true;

   // check the activity status of the game, and do not proceed if the game is currently active.
   if (check_game_activity_status && isGameActive ())
   {
      set_behavior = false;
   }

   // checks to see if the config item is valid and if the game is active.
   if ((config_id != INVALID_CONFIGID) && set_behavior)
   {
      String credit_meter_display_behavior;
      config.getValueString (config_id, credit_meter_display_behavior);

      /* since the script always defaults to display the amount in credit, only change the
         credit meter display to display the amount in cash if "CURRENCY" is selected. */
      if (credit_meter_display_behavior != "DEFAULT")
      {
         /* post an event to force the meter actor to display the amount in cash.
            post to all meter actors that are listening for this event. */
         String event_string;
         event_string << "CreditMeter:Behavior:Forced:" << credit_meter_display_behavior;

         postEventAll (event_string);
      }
   }
}

/// <summary> Determine if a powerhit occurred during game play. </summary>
/// <returns> Return true, if the game is coming up from a powerhit that occurred during game play. </returns>
bool GamePresentation::isPresentationPlaying ()
{
   verboseMethodName();
   bool powerhit_synchronize = false;

   if (read_only_game_state.isAttached ())
   {
      // Get the game state information
      const GameFlowStatePair * game_state_info = reinterpret_cast <const GameFlowStatePair*>(read_only_game_state.getMemory ());
      #ifdef __VERBOSE__
      debug << endl << "Power Hit Synchronize flow_state: " << game_state_info->getState () << endl;
      #endif

      // If the flow state is greater then 2 we are past the idle state.
      if (game_state_info->getState () > GameStateValue::STATE_IDLE)
      {
         powerhit_synchronize = true;
      }
      else
      {
         powerhit_synchronize = false;
      }
   }

   // Return true, if the game is coming up from a powerhit that occurred during game play
   return (powerhit_synchronize);
}

/// <summary># Scriptable function to post game side tilt.</summary>
/// <param name="title"># The message that appears in the title of the tilt presentation.</param>
/// <param name="message"># Text that appears as an operator message in the tilt presentation.</param>
/// <param name="_out_of_service"># A Boolean value. If true, this is a new tilt request. If false, this is a tilt clear request.</param>
/// <returns># True if success, otherwise false.</returns>
/// <remarks># This tilt by default is a hard tilt, normal priority and idle.</remarks>
bool GamePresentation::postGameSideTilt (const String& title, const String& message, bool _out_of_service)
{
   TiltGameRequestEvent tilt_request (gameIdentifier (), title, message, _out_of_service);
   GameRequestEventPoster gre_poster;
   return gre_poster.post (tilt_request);
}

/// <summary> Determine if a monetary win is award for any acts in the current game. </summary>
/// <returns> Return true, if a monetary win is award for any acts in the current game. </returns>
bool GamePresentation::winAllActs (void)
{
   bool result = false;
   Amount win_amount (ZERO_AMOUNT);
   if (extractWinForAllActs (win_amount))
   {
      result = win_amount.isNotZero ();
   }

   return result;
}

// return if there was a RMLP bonus triggered during the current session
bool GamePresentation::rmlpTriggered ()
{
   return rmlp_triggered;
}

// return if there is a RMLP feature enabled on EGM
bool GamePresentation::rmlpEnabled ()
{
   return rmlp_feature_enabled;
}
