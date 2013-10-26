/* (c) Copyright 2006 International Game Technology */

/*
   $Id: SlotPresentation.hpp,v 1.0, 2013-08-23 18:28:05Z, Shaw, Jeffrey$
   $Date: 8/24/2013 2:28:05 AM$
   $Revision: 1$
   Based on revision 288

   Purpose:
      Defines the base slot presentation class

   Modifications:
      Author          Date         Explanation
      --------------- ----------   ------------
      Dwayne Nelson   Oct 20, 99   New
*/

#ifndef __SLOTPRESENTATION_HPP__
#define __SLOTPRESENTATION_HPP__

#include <vector>
#ifndef __DYNAMICSLOTBETBUTTONDEFS_HPP__
#include "DynamicSlotBetButtonDefs.hpp"
#endif
#ifndef __GAMEPRESENTATION_HPP__
#include "GamePresentation.hpp"
#endif
#ifndef __GAMEPRESENTATIONINTERFACE_HPP__
#include "GamePresentationInterface.hpp"
#endif
#ifndef __GAMESIDESLOTEVALUATORINTERFACE_HPP__
#include "GamesideSlotEvaluatorInterface.hpp"
#endif
#ifndef __IGAMECONTROLPANEL_HPP__
#include "IGameControlPanel.hpp"
#endif
#ifndef __INTELLISYMBOLSCONTROLLERINTERFACE_HPP__
#include "IntelliSymbolsControllerInterface.hpp"
#endif
#ifndef __GENERICGAMEREQUESTEVENT_HPP__
#include "GenericGameRequestEvent.hpp"
#endif
#ifndef __PAYTABLEAWARDPROPERTYLIST_HPP__
#include "PaytableAwardPropertyList.hpp"
#endif
#ifndef __ROLLUPDEFS_HPP__
#include "RollupDefs.hpp"
#endif
#ifndef __SEQUENCEEVENT_HPP__
#include "SequenceEvent.hpp"
#endif
#ifndef __SLOTAWARDINTERFACE_HPP__
#include "SlotAwardInterface.hpp"
#endif
#ifndef __SLOTESSENTIALRECORD_HPP__
#include "SlotEssentialRecord.hpp"
#endif
#ifndef __SLOTEVALUATORINTERFACE_HPP__
#include "SlotEvaluatorInterface.hpp"
#endif
#ifndef __SLOTEVALUATORINTERFACE_HPP__
#include "SlotEvaluatorInterface.hpp"
#endif
#ifndef __SLOTGAMESETCONFIGURATION_HPP__
#include "SlotGameSetConfiguration.hpp"
#endif
#ifndef __SLOTGAMESETINTERFACE_HPP__
#include "SlotGameSetInterface.hpp"
#endif
#ifndef __SLOTINTERFACE_HPP__
#include "SlotInterface.hpp"
#endif
#ifndef __SLOTSHAREDESSENTIALRECORD_HPP__
#include "SlotSharedEssentialRecord.hpp"
#endif
#ifndef __SPECTRUMUPDATEMANAGER_HPP__
#include "SpectrumUpdateManager.hpp"
#endif
#ifndef __WIN_CYCLE_LITE_INTERFACE_HPP__
#include "WinCycleLiteInterface.hpp"
#endif
#ifndef __WINDISPLAY_HPP__
#include "WinDisplay.hpp"
#endif
#include <queue>
class SlotPresentation : public SlotInterface, public GamePresentation
{
   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// UnknownInterface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Initialize the object for use.
      virtual void postConstructor (void);
      // Prepare the object for destruction.
      virtual void preDestructor (void);

   public:
     // Config names
      static const char *const CONFIG_ID_DIGITAL_GLASS_PAYTABLE_ENABLED;//DGTAG
      static const char *const CONFIG_ID_SUPPORT_WIN_UP_TO_SEEPAYS;
      static const char *const CONFIG_ID_SUPPORT_WIN_UP_TO_ATTRACT;
      static const char *const CONFIG_ID_SUPPORT_WIN_UP_TO_BONUS_SEEPAYS;
      static const char *const CONFIG_ID_WIN_UP_TO_BONUS_WIN_STRING;
      static const char *const CONFIG_ID_WIN_UP_TO_SECONDARY_STRING;
      static const char *const CONFIG_ID_GAMERULES_VERSION;
      static const char *const CONFIG_ID_AUTO_START_GAME_ON_ROLLUP_SLAM;
      static const char *const CONFIG_ID_EASY_BET_MODE;
      static const char *const CONFIG_ID_NUMBER_OF_SUBBETS;

      typedef InterfaceExtractor2 (ControlPanelInterface, IGameControlPanelInterface) IGameControlPanelExtractor;
      typedef InterfaceExtractor2 (GameSideSlotEvaluatorInterface, SlotEvaluatorInterface) SlotEvaluatorInterfaces;

      // Type of object used in the scripts
      static const char *const object_type;

      // Define ids for buttons that may need to be manipulated for Central Determination enrollment double-tap feature.
      typedef uint32 EnrollDblTapButtonId;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_INVALID_BUTTON       = 0;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_SPIN_BUTTON          = 1;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_MAX_BET_BUTTON       = 2;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_1 = 3;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_2 = 4;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_3 = 5;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_4 = 6;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_5 = 7;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_6  = 8;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_7  = 9;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_8  = 10;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_9  = 11;
      static const EnrollDblTapButtonId ENROLL_DBL_TAP_BET_N_COINS_BUTTON_10 = 12;

      // Presentation state condition string constants.
      static const char *const CONDITION_UNINITIALIZED;
      static const char *const CONDITION_LAST_NUDGE_GAME;
      static const char *const CONDITION_NOT_LAST_NUDGE_GAME;
      static const char *const CONDITION_NUDGE_TRIGGERED;
      static const char *const CONDITION_NOT_NUDGE_TRIGGERED;

      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// SlotInterface /////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      virtual bool processConditionLastNudge (void);
      // Return the current reel set.
      virtual uint32 currentReelSet (void);
      // Return the number of reel sets in the game.
      virtual uint32 numberOfReelSets (void);
      // Return the number of reels in the set.
      virtual uint32 numberOfReels (uint32 reel_set);
      // Return the number of stops on the given reel.
      virtual uint32 numberOfReelStops (uint32 reel_set, uint32 reel);
      // Return the number of visible symbols on the given reel.
      virtual uint32 numberOfVisibleSymbols (uint32 reel_set, uint32 reel);
      // Return the reel stop.
      virtual uint32 reelStop (uint32 reel_set, uint32 reel_index);
      // Get the reel stop for the specified reel.
      bool reelIsHeld (uint32 reel_set, uint32 reel_index);
      // Return the name of the symbol at the given visible stop.
      // Returns 0 for invalid reel set, reel or visible stop index.
      virtual const char *symbolName (uint32 reel_set, uint32 reel, uint32 visible_stop_index);
      // Returns true if all reels in the given reel_set are of SlotEssentialRecord::STANDARD type
      virtual bool isStandardReelsType (uint32 reel_set);

      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// Slot Presentation /////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Called to confirm that at least one set of paylines have been configured.
      virtual void confirmPaylinesAreConfigured (void);
      // Get the nudge flag from the essential record.
      virtual uint8 getQueuedReels (void);
      // Get the reel stop on the current reel set for the provided stop index.
      uint32 reelStop (uint32 reel_index);
      // Return the physical reel stop.
      virtual uint32 reelStopPhysical (uint32 reel_set, uint32 reel_index);
      // Get the virtual reel stop on the current reel set for the provided stop index.
      uint32 reelStopVirtual (uint32 reel_index);
      // Move the next reel in queue.
      void moveNextQueuedReel (void);
      // Return the virtual reel stop.
      virtual uint32 reelStopVirtual (uint32 reel_set, uint32 reel_index);
      // Loads the properties that are used to toggle the win messages in history after a bonus.
      virtual void loadWinMessagesInHistoryProperties (bool toggle_win_message, bool show_total_win_credit_meter_value);
      // Retrieve the minimum time the reels need to spin before they start stopping
      virtual uint32 minimumReelSpinTime (void);
      // Set reel spin delay property
      virtual void setReelSpinDelay (void);

      // Return the number of machines.
      virtual uint16 getNumberOfMachines (void);

      // Return the number of reels on current reel set.
      virtual uint32 numberOfReels (void);
      // Returns the number of reel stops configured for the reel id received.
      uint32 getNumberOfReelStops (uint32 reel_index);
      // Returns the home segment for the given reel
      virtual uint32 getReelHomeSegment (uint32 reel_number);
      // Returns the number of segment pieces in the model for the given reel
      virtual uint32 getReelNumberOfSegmentPieces (uint32 reel_number);
      // Purpose: Returns the number of visible segments as defined by presentation for a reel.
      virtual uint32 getReelNumberOfVisibleSegments (uint32 reel_number);

      // Can the current award be displayed
      bool canDisplayAward (void);
      // Returns true if the paylines displays can safely be updated.
      virtual bool canUpdatePaylines (void);
      // Start/Stop the WinDisplay cycle, or show/hide the game pays message
      virtual void displayTotalWinMessage (bool show);
      /// <summary> Starts or stops the win cycle animation.</summary>
      virtual void displayWin (bool show = true, bool start_new_win_cycle = false);
      // Prepares the IntelliSymbolsController with information about the current game.
      virtual void initializeIntelliSymbolList (void);
      // Play sequence on guiding arrows.
      void playArrowsSequence (const char* sequence_name, bool start_sequence);
      // Update the BetPerLineMeter with the given value.
      virtual void updateBetPerLineMeter (const Amount &value);
      // Synchronize the bet per line scroll bar
      virtual void synchronizeBetPerLineSlider (const Amount &value);
      // Enable the bet per line scroll bar
      virtual void enableBetPerLineSlider (bool enable_slider);
      // Update the SelectLinesMeter with the given value.
      virtual void updateSelectLinesMeter (uint32 lines_selected);
      // Update the WaysSelected property with the current number of ways selected
      virtual void updateWaysSelected ();
      virtual bool processEvent (const MultimediaEvent &event);
      virtual bool processEvent (const SequenceEvent &event);
      virtual void processSpectrumEvent (const MultimediaEvent &event, const String& progressive_event_trunk, const String& property_root_name);
      // Set the press button to start bonus message. Part of the new 2006 style.
      virtual void setAndShowPressButtonToStartBonusMessage (void);
      // Synchronize the select lines scroll bar
      virtual void synchronizeSelectLinesSlider (uint32 lines_selected);
      // Synchronize the win meter with the appropriate value
      virtual void synchronizeWinAmount (void);

      // Return the number of paylines
      virtual uint32 numberOfPaylines (void);
      /// <summary> Gets the number of selected paylines from the bet object.</summary>
      virtual uint32 numberOfSelectedPaylines (void);
      // Return true if there are zero subbets
      virtual bool numberOfSelectedSubBetsIsZero (void);
      // Return the number of multi-ways
      virtual uint16 numberOfMultiWays (void);
      // Return the number of multi-ways selected
      virtual uint16 numberOfMultiWaysSelected (void);
      // Return the number of scatters
      virtual uint16 numberOfScatters (void);
      // Return the number of patterns
      virtual uint16 numberOfPatterns (void);
      // Returns the Maximum size of Visible Symbols of all the reels.
      virtual uint32 findMaxNumberOfVisibleSymbols(uint32 _reel_set, uint32 _num_of_reels);

      // Helper methods to extract information from the slot game set.
      // Updates the slot win animator interface parameter received with the animator configured for the game set specified.
      virtual bool animateSlotWin (Mojo<AnimateSlotWinInterface> &animate_slot_win, uint32 game_set_index);
      // Updates the IntelliSymbolsControllerInterface parameter received with the controller configured for the game set specified.
      virtual bool intelliSymbolsController (Mojo<IntelliSymbolsControllerInterface> &intellisymbols_controller, uint32 game_set_index);
      // Return the flag that determines the payline draw behavior.
      virtual bool getPaylineDrawFlag (void);
      // Updates the payline interface parameter received with the paylines configured for the game set specified.
      virtual bool getPaylines (Mojo<PaylinesInterface> &paylines, uint32 game_set_index);
      // Updates the reels interface parameter received with the reels configured for the game set specified.
      virtual bool getReels (Mojo<ReelsInterface> &reels, uint32 game_set_index);

      // The following method updates the control panel interface received with the registered igame control panel.
      // If the control panel is valid, this method will return true.
      virtual bool controlPanel (IGameControlPanelExtractor &control_panel);
      // Called to enable all known control panels.
      virtual void enableControlPanels (bool enable);
      virtual void enableGameMenuButtons (bool enable);
      // Enable More Games button on interface if more then one player selectable game.
      virtual void enableMoreGamesButton (bool enable);

      // Add a bet configuration to the dynamic button image map
      virtual void addDynamicSlotBetButton (DynamicBetNValue bet_n_value, DynamicSelectNValue select_n_value, DynamicExtraNValue extra_n_value, DynamicImageIndex image_index);
      // Set image list alias for dynamic button image map
      virtual void setDynamicSlotBetButtonAlias (const String &alias);
      // Synchronize Dynami Bet Buttons to display images set in map
      virtual void synchronizeDynamicSlotBetButtons (void);
      // Enable or disable Dynamic Slot Bet Buttons
      virtual void enableDynamicSlotBetButtons (bool enable, bool update = false);
      // Get State of Dynamic Slot Bet Buttons
      virtual bool dynamicSlotBetButtonsAreEnabled (void);

      // Enable the appropriate "play" button for enrollment double-tap feature.
      virtual void enableEnrollDoubleTapButton (void);
      // Enable the max bet and bet N buttons for enrollment double-tap feature.
      virtual void enableBettingButtonsForDoubleTap (void);
      // This method is called by the leavingObjectScope method to finalize the configuration
      // of the control panels registered by this game.
      virtual void finalizeControlPanelConfiguration (void);
      // Get the id of the button that may need to be enabled to support CDS double-tap feature.
      virtual EnrollDblTapButtonId getEnrollmentButtonDoubleTapButtonId (void);
      // The following method assigns the new control panel to the game-specific control panel mojo as is
      // appropriate for the control panel type name received.
      virtual bool registerControlPanel (const char *type, const char *alias);
      // Set the id of the button that may need to be enabled to support CDS double-tap feature.
      virtual void setEnrollmentButtonDoubleTapButtonId (EnrollDblTapButtonId button_id);
      // Called to show all known control panels.
      virtual void setControlPanelDenomination (const Denomination& denomination);
      // Called to show switch for current bet.
      virtual void showPlayXPerLineSwitch (bool okay_to_show);
      // Determine whether paylines will be drawn individually or all at once.
      virtual void drawPaylinesIndividually (bool draw_individual);
      // Called to show switch for current bet.
      virtual void showLinesSwitch (bool okay_to_show);

      // Called to show all known control panels.
      virtual void showControlPanels (bool show);
      // Called to synchronize all known control panels.
      virtual void synchronizeControlPanels (void);
      // Purpose:  Called to synchronize Easy Bet.
      virtual void synchronizeEasyBet (void);
      // Determine if the game menu buttons should be shown or not
      // these buttons include Game Menu, Small Game Menu, Volume and More Games
      virtual void synchronizeGameMenuButtons (void);

      // Set control panel buttons to their default settings.
      virtual void synchronizeStartButtons (void);
      virtual void synchronizeStartButtonsEnrollState (void);
      virtual void synchronizeStartButtonsIdleState (void);
      virtual void synchronizeStartButtonsPreStartState (void);
      virtual void synchronizeStartButtonsStartState (void);
      virtual void synchronizeStartButtonsPlayState (void);
      virtual void synchronizeVolumeButton (bool enable);
      virtual void synchronizeNewCardButton (void);

      /// <summary># Maintain Autoplay button presentation syncronization for any changes in states except play state.</summary>
      virtual void synchronizeAutoPlayButtonForNonPlayState (bool force_sync);

      // Synchronize 'Win Up To' displays.
      virtual void synchronizeWinUpToItems (void);

      /// <summary># Update the Select the Bet button on synchronize.</summary>
      virtual void synchronizeSelectTheBet (void);

      // Get the reel stop for the specified reel.
      bool allReelsAreHeld (void);
      // Returns true if the reel state in the essential record is HELD.
      bool reelIsHeld (uint32 reel_index);
      // Count and return the number of reels currently held.
      uint32 totalReelsHeld (void);

      /// <summary> Gets a reference to the GameSideSlotEvaluatorInterface.</summary>
      virtual bool gameSideSlotEvaluatorInterface (Mojo<GameSideSlotEvaluatorInterface> &game_side_slot_evaluator, uint32 reel_set_index = 0);
      // Synchronize the visual payline display.
      virtual void hideAllPaylinesDisplays (void);
      // Initialize the IntelliSymbolController with the win information for the current game.
      virtual void postSymbolsOnReels (String name, String event_prefix);
      // Sets the okay_to_hide_spin_button to the boolean received.
      virtual void setOkayToHideSpinButton (bool released);
      /// <summary>
      /// # Bonus string updates will not force a digital glass refresh until set to true.
      /// </summary>
      virtual void setOkayToUpdateBonusString (bool ready);
      // Used for X lines / credit games.
      void setPaylinesPerCreditProperty (void);

      /// <summary># Sets the stop display text for all reels of the specific reel set so that it reflects the current stops.</summary>
      virtual void setStopDisplayText (uint32 reel_set);
      /// <summary># Sets the stop display text for all reels of current reel set so that it reflects the current stops.</summary>
      virtual void setStopDisplayText (void);
      // Set the (game pays) win message.
      virtual void setTotalWinMessage (void);
      // Set the (game pays) win message with the win amount received.
      virtual void setTotalWinMessage (const Amount &win_amount);
      // Get the slot award interface if there is one for this game
      virtual bool slotAwardInterface (Mojo<SlotAwardInterface> &slot_award_interface);
      // Get the slot evaluator mojo
      virtual bool slotEvaluatorInterface (Mojo<SlotEvaluatorInterface> &slot_evaluator_interface, bool fail_if_invalid = true, uint32 set_index = 0);
      // Updates the game set parameter received with this game's configured game set.
      virtual bool slotGameSet (Mojo<SlotGameSetInterface> &game_set, uint32 set_index = 0);
      // Show guiding arrows.
      virtual void showArrows (bool show_state);
      // Show/hide the stop display text.
      virtual void showStopDisplayText (uint32 reel_index, bool okay_to_show);
      // Show/hide the stop display text.
      virtual void showStopDisplayText (bool okay_to_show);
      // Returns the current value of okay_to_hide_spin_button.
      virtual bool okayToHideSpinButton (void);
	  /// <summary>
      /// # Bonus string updates will not force a digital glass refresh unless this function returns true.
      /// </summary>
      virtual bool okayToUpdateBonusString (void);
      /// <summary># Check whether the game has the ability to slam reels.</summary>
      virtual bool okayToSlamReels ();
      // This method rolls any credits won for the current game on the win meter.
      virtual void rollupWin (RollupDefs::ROLLUP_MODE rollup_mode);
      // Snap the reels to the stopping position
      virtual void snapReelsToStopPosition (void);
      // Returns true if the presentation has been configured to spin the held reels.
      virtual bool spinHeldReels (bool all_reels_held);
      // Spin the reels forever. Return true if the reels were started.
      virtual bool spinReelsForever (void);
      // Synchronize the visual bet components
      virtual void synchronizeBet (void);
      /// <summary> Synchronize the betting buttons </summary>
      virtual void synchronizeBetButtons (void);
      // Synchronize the visual payline display.
      virtual void synchronizePaylines (bool draw_paylines);
      // Draws only the currently selected payline.
      virtual void drawIndividualPayline (void);

      // Set the property that holds the bet info.
      virtual void setBetMessagePropertyString (bool append_good_luck);
      // Show the bet message during history and utility states.
      virtual void showBetMessageInHistoryAndUtilityStates (void);
      // Show or hide the prompt that is displayed in the meter panel.
      virtual void showBetMessageMeterPanel (bool show, bool _append_good_luck);
      // Show the prompt that is displayed in the slider menu.
      virtual void showBetMessagePlayerMenu (bool show);

      // Set the property that holds the bonus bet info.
      virtual void setBonusBetMessagePropertyString (void);
      // Show the prompt that is displayed in the game player menu.
      virtual void showBonusBetMessagePlayerMenu (bool show);
      // Show and hide the DoubleUp Button based on the win.
      virtual void updateDoubleUpButton (void);
      // rollup double up wins in idle
      virtual void rollupDoubleUpWin (void);
      virtual bool checkForDoubleUpEnable ();
      // Check the award list for an RMLP award.
      virtual bool checkForRmlpAward ();
      // Get the RMLP bonus award amount, returns 0 if no award
      virtual uint32 getRmlpAwardAmount ();

      // Set every actor in the presentation to a known state because the context just entered or its mode changed.
      virtual void synchronize (void);
      /// <summary> # Make sure the reels are display at correct stops.</summary>
      virtual void synchronizeReels ();
      // Send the appropriate messaging to highlight the active symbols on the screen.
      virtual void updateMultiWayActiveSymbolMask (bool with_active_scatter_symbols);
      // Send the appropriate messaging to highlight the active symbols on the screen.
      virtual void updateMultiWayActiveSymbolMask (bool with_active_scatter_symbols, int8 which_reel);
      // Store the textures of the overlay symbols to persistant properties.
      virtual void storeTransitionOverlaySymbols (void);
      // Reset the textures of the overlay symbols to null value.
      virtual void resetTransitionOverlaySymbols (void);
      // Enable or disable transition overlays
      virtual void enableTransitionOverlays (bool _transition_overlay_enable);
      // Return transition overlays is enable or not
      virtual bool transitionOverlaysEnabled (void);
      // Return if the overlays are currently storing valid symbols or not
      virtual bool areTransitionOverlaySymbolsValid (void);
      virtual void setTransitionOverlayResourceKey (String new_transition_overlays_resource_key);
      // Find the evaluator to use for the given paytable section. Returns true on success.
      virtual bool evaluatorForPaytableSection (Mojo<EvaluatorInterface> &evaluator, const char *section);

      // Called when Game Manager exits our screen context.
      virtual bool exitGameMode (State &state, const GameStageExitReason &reason);

      // DATA:
      // Conditions methods
      // Provides state transition decision making for presentation states.
      // Returns true if all the conditions specified in the string are true.
      // All unsupported conditions are stored in unsupported_conditions.
      // Unsupported conditions don't effect the return value.
      virtual bool checkPostWaitEventConditions (const vector<String> &conditions, vector<String> &unsupported_conditions, bool force_all = false);

      // Return true if no wins, triggers, etc. are in current award
      virtual bool isALoser (void);
      // Query the bet object and post whether the handle should be locked or unlocked.
      virtual void postHandleMessage (void);

      // Allow for derived classes to update their info.
      virtual void updateAfterPaytableSwitch (void);

      /// <summary> Sets the base game presentation evaluation flag. Shows the reels if the flag is set to true and hides the reels otherwise.</summary>
      virtual void setBaseGamePresentationEvaluationFlag (bool base_game_presentation_evaluation);

      /// <summary> Sets the reel positions for games that have the base game presentation evaluation flag set to false.</summary>
      virtual void setReelPositionForNonBaseGamePresentationEvaluation (void);

      // Determine whether to show the arrows
      virtual void showArrowsInSlotHistoryState (bool okay_to_show);

      // Return the flag that determines whether to show the arrows
      virtual bool getShowArrowsInHistoryFlag (void);

      // Return the flag that determines whether to spin the reels after a power hit during gameplay.
      virtual bool getPowerhitSpinReelsFlag (void);

      // Parse a string and change the meter panel color accordingly
      virtual void parseMeterPanelColor (String &rgb_string);
      // Set the meter panel color shift properties.
      virtual void setMeterPanelColorShift (float red_value, float green_value, float blue_value);
      // Game initialization.
      virtual bool init (State& state, bool cold_start_flag);
      // Return true if the enrollment state has started
      virtual bool isEnrollmentStarted (void);
      // Set whether enrollment state has started, clear in idle state
      virtual void setEnrollmentStarted (bool enrollment_flag);
      // Set whether a double up rollup is needed.
      virtual void setDoubleUpRollupNeededFlag (bool set);
      // Set the segments of the given non-standard reel to the positions in the Essential Record
      virtual void setSegmentsToPosition (uint32 reel_set_index, uint32 reel_index);
      // Force the segments of the non-standard given reel to the positions in the Essential Record
      virtual void snapSegmentsToPosition (uint32 reel_set_index, uint32 reel_index);
      // Add a reel number to the queued reels.
      void addToReelQueue (uint8 reel_number);
      // Light auto start lamps if available
      virtual void showStartGameOnRollupSlamLamps (void);
      // Store the button which slammed the animation/rollup.
      virtual void storeRollupSlamButton (String button);
      // If the award state animation/rollup was slammed set the class member flag denoting it.
      virtual void resetRollupSlamButton (void);
      // Return the private class member flag rollup_slam_button_set.
      virtual bool rollupSlamButtonSet (void);
      // Return the private class member string rollup_slam_button_string.
      virtual String getRollupSlamButton (void);
      // Check if the game is configured to auto start if previous games rollup was slammed and request start if appropriate.
      virtual bool autoStartGameOnRollupSlam (void);
      //Check to see if we are allowed to auto start a game after a credit rollup slam is enabled
      virtual bool autoStartGameOnRollupSlamAllowed (void);
      //Check whether slamming mechanical reels is allowed
      virtual bool slamMechanicalReelsAllowed (void);
      // Purpose: Stop every actor in the presentation because the context is suspending or exiting.
      virtual void suspend (void);

      SlotEssentialRecord::ReelType getSlotEssentialRecordReelType (uint32 reel_index);

      /// <summary> Updates the win animation info for win cycle displays.</summary>
      virtual void updateWinOutcome (bool show_utility_info = false);

   private:
      // Flag denoting that the last games win animation/credit rollup was slammed.
      bool rollup_slam_button_set;
      // String containing the name of the button which caused the win animation/credit rollup slam
      String rollup_slam_button_string;

   protected:
      /// <summary> Flag to indicate we are going to start a game from a bet request. </summary>
      bool bet_request_started;
      //Flag to indicate we have entered the enrollment state.
      bool enrollment_started;
      // Flag to indicate if game is of a bonus bet type.
      bool bonus_bet_type_game;
      // Games configured max bet per line value
      int32 bet_per_line_max_value;
      // Bonus Bet Games configured max bonus bet multiplier
      int32 max_bonus_bet_multiplier;
      // Bonus Bet Games configured number of bonus bet levels
      int32 number_bonus_bet_levels;
      // Games configured max number of lines
      int32 number_of_lines_max_value;
      // Show bonus message longer for maxbet
      bool maxbet_bonus_enable;
      // Show bet message button properly
      bool bonus_bet_message;
      // Store the transition overlay status
      bool transition_overlays_enabled;
      // Store the transition overlay resource key
      String transition_overlays_resource_key;
      // Win display ("Game Pays") controls
      WinDisplay win_display;
      // Double Up rollup needed flag
      bool doubleup_rollup_needed;
      // Flag indicates whether slamming mechanical reels is allowed
      bool enable_mechanical_slam;
      // Member function to retrieve the variable bonus_bet_type_game
      virtual bool bonusBetTypeGame (void);
      // Update the slider to the bonus bet level position
      virtual void updateSliderBonusPosition (void);
      // Update the Maxbet bonus flag
      virtual void updateMaxbetBonusStatus (bool);
      // Update the bonus bet flag
      virtual void updateBonusBetStatus (bool);
      // Determines if game is a bonus bet type and configures variables appropriately
      virtual void initializeBonusBet (void);
      // Member function to handle bonus bet specific events.
      virtual bool processBonusBetEvent (const MultimediaEvent &event);
      // Set the ranges of the sliders with the values from the config file.
      virtual void initializeSliders (void);
      // The reel-set index to use if there is only one reel-set - i.e. use the first (and only) reel-set.
      static const SlotEssentialRecord::ReelSetIndex SINGLE_REEL_SET_INDEX = 0;
      // The reel-set that we are currently operating on (support derived classes with multiple reel-sets).
      SlotEssentialRecord::ReelSetIndex current_reel_set_index;

      // Reconfigure reels
      virtual bool reconfigureReels (void);

      Mojo<SlotEvaluatorInterface> slot_evaluator;
      // If the spin button released event is received in the idle state, other states need to show the
      // correct buttons (i.e., the stop spin button).  The following flag will be true in such a case.
      bool okay_to_hide_spin_button;
      // If this flag is set, the paylines will draw one at a time.  If it is false, they will draw
      // mutliple paylines at once.
      bool draw_paylines_individually;
      // Each game requires the intellisymbols to be configured with the reel stops.  Declare a class member
      // to track the stop information instead of dynamically allocating memory every time we play a game.
      vector <uint32> intellisymbols_reel_stops;
      // Track previous number of lines selected to know when to enable the per-line slider
      uint32 previous_number_of_lines_selected;

      // Structure used for keeping track of values posted on digital paytable glass.
      PaytableAwardPropertyList paytable_award_property_list;


      // Creates a new game set configuration as appropriate for this presentation.
      virtual bool createGameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface);
      // Final preparations before this presentation is unloaded.
      virtual bool prepareForUnloading (void);
      // Load the betting shared object.
      virtual bool setBetInformation (const char *so_filename, const char *object_name);
      // Set the event to be sent out to hide multi-way segement mask.
      virtual void setMultiWayHideSegmentMaskEvent (String event);
      // Set the event to be sent out to show multi-way segement mask.
      virtual void setMultiWayShowSegmentMaskEvent (String event);
      // Update the specified config item
      virtual void updateConfigItem (const String &config_id);

      // Paytable support methods.

      // Load the paytable provided.  If 0 is passed for the paytable filename, the current paytable will be loaded.
      // If 0 is passed for the section name, the default paytable section will be loaded.
      virtual bool loadPaytable (const char *filename = 0, const char *section = 0);
      // Set the current paytable section
      virtual bool setCurrentPaytableSection (const char *section);
      // Inform the game about receiving central determination RNG seeds.
      virtual bool setGameEnrollmentSeedsArrived (void);

      virtual void snapPaylinesToClosestConfig (void);
      virtual void snapBetPerLineToClosestConfig (void);
      // Synchronize the visual bet components do to a (host central determination) bet configuration change.
      virtual void synchronizeBetConfig (void);

      /////////////////////////////////////////////////////////////////////
      // Compare the data in the "RollupData" vector to the bet and credits won to determine how to rollup.
      virtual void calculateRollupThreshold (String resource_key);
      // Initialize the mode of the rollup data vector
      virtual void initializeCelebrationMode (String mode);
      // Instantiate a "RollupData" struct and add it to the vector.
      virtual void setRollupData (float win_to_bet_ratio_low_range, float win_to_bet_ratio_high_range,
                                  Amount::AmountValue low_range, Amount::AmountValue high_range, int32 celebration_loop_count,
                                  uint32 threshold_event_index = 0);
      // Purpose:  Update the lowest player denomination with the given value.
      virtual void updateLowestPlayerDenomination (void);
      // Purpose:  Update the denomination associated with the maximum theme win value.
      virtual void updateWinUpToDenomination (void);
      // Update the WinUpToMaxBetMeter with the given value.
      virtual void updateWinUpToMaxBetMeter (void);
      // Update the WinUpToMaxWinMeter with the given value.
      virtual void updateWinUpToMaxWinMeter (void);
      // Update the MaximumWinUpToMaxWinMeter with the given value.
      virtual void updateMaximumWinUpToMaxWinMeter (void);
      // Update the BonusXWinUpToString with the given property_string.
      virtual void updateBonusWinUpToString (void);
      //Flag to indicate if the updateBonusWinUpToString function is allowed to redraw the digital glass.
	  bool okay_to_update_bonus_string;

      // Creates and initializes the paytable award property list
      virtual void initializePaytableAwardProperties (const Mojo<SlotEvaluatorInterface> &slot_evaluator_interface);
      // Load any neccessary information from the registry files.
      virtual void loadRegistyData (void);
      ///<summary>Updates all paytable award properties and virtual spectrum properties.</summary>
      virtual void updatePaytableAwardProperties (bool progressive_update = false);
      ///<summary>Updates the paytable award properties and the given progressive level's virtual spectrum.</summary>
      virtual void updatePaytableAwardProperties (bool progressive_update, uint32 progressive_level,
                                                  bool &progressive_properties_updated, bool &non_progressive_properties_updated);
      // Posts more detailed "ShowDigitalGlass" events.
      virtual void postShowDigitalGlassEvents (void);
      // Update digital glass shown.
      virtual void updateDigitalGlass (void);

      // Synchronize the presentation.
      virtual void synchronizePresentation (uint32 what_to_synchronize, uint32 bet_request_type = 0, uint32 bet_request_data = 0);
      // Synchronize the More Games Button.
      virtual void synchronizeMoreGamesButton (void);

      // This structure will hold data that is used to determine what rollup range to use.
      typedef struct
      {
         float               win_to_bet_ratio_low_range;
         float               win_to_bet_ratio_high_range;
         Amount::AmountValue low_range;
         Amount::AmountValue high_range;
         int32               celebration_loop_count;       // The number of times the celebration movie will loop.
         uint32              event_threshold_index;
      }RollupData;

      enum RollupCriteria
      {
         ROLLUP_CRITERIA_NONE = 0,
         ROLLUP_CRITERIA_LESS_THAN,
         ROLLUP_CRITERIA_LESS_THAN_OR_EQUALS,
         ROLLUP_CRITERIA_GREATER_THAN,
         ROLLUP_CRITERIA_GREATER_THAN_OR_EQUALS,
         ROLLUP_CRITERIA_EQUALS,
         MAX_ROLLUP_CRITERIA,
      };
      // Structure defining what to compare against.
      enum RollupCompareTarget
      {
         ROLLUP_COMPARE_NONE = 0,
         ROLLUP_COMPARE_MAX_BET_PER_LINE,
         ROLLUP_COMPARE_DENOM,
         MAX_ROLLUP_COMPARE,
      };
      // Structure for defining a RollupRule.
      typedef struct
      {
         uint32 rollup_data_start_index;
         uint32 rollup_data_end_index;
         uint32 celebration_bet_threshold;
         RollupCriteria rollup_criteria;
         RollupCompareTarget compare_target;
         uint32 compare_value;
      } RollupRule;

      vector <RollupRule> rollup_rule_vector;
      vector <RollupData> rollup_data_vector;
      RollupRule rollup_rule_default;

      uint32 celebration_bet_threshold;
      uint32 single_credit_rollup_threshold;
      String celebration_mode;

      // Sets the parameters of the default RollupRule.
      virtual void setDefaultRollupRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index, uint32 celebration_bet_threshold);
      // Creates a Rollup Rule.
      virtual void setRollupRule (uint32 rollup_data_start_index, uint32 rollup_data_end_index,
                                  uint32 celebration_bet_threshold, RollupCompareTarget compare_target,
                                  RollupCriteria rollup_criteria, uint32 compare_value);
      // Processes a rollup rule and returns whether its criteria passes.
      virtual bool processRollupRule (const RollupRule &rollup_rule);
      // If this flag is set, the arrows in Reels.model will be displayed in history.
      bool show_arrows_in_history;
      // If this flag is set, the reels will spin after a power hit during game play.
      bool powerhit_spin_reels;
      // If this flag is false, the volume button will be hidden when in multi-game.
      bool show_volume_button_with_more_games_enabled;

      virtual void addPaytableAwardPropertyLegacy (const char* award_name, uint16 progressive_level, bool insert,
                                                   uint32 category, uint32 bet_group_index);

      virtual void addPaytableAwardPropertyBBE (const char* award_name,
                                                bool insert,
                                                const char* prize_scale_name,
                                                const char* prize_name,
                                                uint32 pay_count,
                                                uint32 win_index,
                                                uint16 progressive_level);

      /// <summary> Return true if the flow state is utility. </summary>
      bool isFlowStateUtility ();

   public:
      static const char *const CELEBRATION_MODE_2006_STANDARDS;
      static const char *const CELEBRATION_MODE_2007_STANDARDS;
      static const char *const CELEBRATION_MODE_CUSTOM;
      Amount last_game_win_amount;
      virtual void storeLastGameWinAmount (void);
      virtual void clearLastGameWinAmount (void);
      // Queue of reels that need to be stopped.
      queue <uint8> reel_queue_list;

   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// Script Interface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Return the type of this scriptable object.
      virtual const char *whoAreYou (void) const;
      // Return an interface to an object given its id.
      virtual bool createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared);
      // Execute a function given its id.
      virtual bool execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list);
      // Called when we are about to leave the object's scope.
      virtual void leavingObjectScope (void);
      // Script function IDs. These IDs have to start at GamePresentation::MAX_FUNCTION_ID.
      // Derived classes must start at MAX_FUNCTION_ID.

      // Standalone progressive meter(s) behaviour flag
      bool delay_standalone_progressive_meter_reset;
      /// <summary>
      /// # Cause the virtual spectrum meters (stepper style or video style) to stop updating if set to false.
      /// </summary>
      /// <remarks>
      /// # This only affects the presentation.  Even though it is named with Standalone...it can be used with
      /// any stepper style virtual spectrum.
      /// </remarks> 
      virtual void delayStandaloneProgressiveMeterReset (const bool delay);
      /// <summary>
      /// # Update Standalone Progressive meters on a synchronize, if the meters exist.
      /// </summary>
      /// <remarks>
      /// # This only affects the presentation.  Even though it is named with Standalone...it can be used with
      /// any stepper style virtual spectrum.
      /// </remarks> 
      virtual void synchronizeStandaloneProgressiveMeters (void);
      /// <summary># Find the paid amount and update the virtual spectrum meter. </summary>
      /// <remarks># This only affects the presentation.</remarks> 
      virtual void synchronizeVirtualSpectrumWithRealValue (void);

      // Symbol overlay scripting support
      vector<String> multiway_overlay_symbol_name;
      vector<String> payline_overlay_symbol_name;
      vector<String> scatter_overlay_symbol_name;
      // Display the overlays on the selected MultiWay.
      virtual void displayMultiWayOverlay (uint32 multiway_number);
      // Display the wild overlays on the selected payline.
      virtual void displayPaylineOverlay (uint32 payline_number);
      // Display the wild overlays on the selected payline.
      virtual void displayScatterOverlay (uint32 scatter_number);
      // Set the multiway symbol that will have an overlay.
      virtual void setMultiWayOverlaySymbolName (ScriptParameterList &parameter_list);
      // Set the name of the wild symbol that doubles other wins
      virtual void setPaylineOverlaySymbolName (ScriptParameterList &parameter_list);
      // Set the name of the bonus overlay symbol
      virtual void setScatterOverlaySymbolName (ScriptParameterList &parameter_list);

      // Execute a function given its id.
      enum
      {
         SNAP_REELS_TO_STOP_POSITION = GamePresentation::MAX_FUNCTION_ID,
         SPIN_HELD_REELS,
         HIDE_ALL_PAYLINES_DISPLAYS,
         SHOW_LINES_SWITCH,
         SHOW_PLAY_X_PER_LINE_SWITCH,
         DRAW_PAYLINES_INDIVIDUALLY,
         CALCULATE_ROLLUP_THRESHOLD,
         INITIALIZE_CELEBRATION_MODE,
         SET_CELEBRATION_BET_THRESHOLD,
         SET_ROLLUP_DATA,
         DISPLAY_TOTAL_WIN_MESSAGE,
         DISPLAY_WIN,
         POST_SYMBOLS_ON_REELS,
         SET_MULTIWAY_OVERLAY_SYMBOL_NAME,
         SET_PAYLINE_OVERLAY_SYMBOL_NAME,
         SET_SCATTER_OVERLAY_SYMBOL_NAME,
         DISPLAY_MULTIWAY_OVERLAY,
         DISPLAY_PAYLINE_OVERLAY,
         DISPLAY_SCATTER_OVERLAY,
         SET_MULTI_WAY_HIDE_SEGMENT_MASK_EVENT,
         SET_MULTI_WAY_SHOW_SEGMENT_MASK_EVENT,
         SHOW_BET_MESSAGE_METER_PANEL,
         SHOW_BET_MESSAGE_PLAYER_MENU,
         LOAD_WIN_MESSAGES_IN_HISTORY_PROPERTIES,
         SHOW_BONUS_BET_MESSAGE_PLAYER_MENU,
         SHOW_STAGE,
         STORE_TRANSITION_OVERLAY_SYMBOLS,
         RESET_TRANSITION_OVERLAY_SYMBOLS,
         ENABLE_TRANSITION_OVERLAYS,
         SET_TRANSITION_OVERLAY_RESOURCE_KEY,
         UPDATE_MULTIWAY_ACTIVE_SYMBOL_MASK,
         SET_SLOT_EVALUATION_STOP_DISPLAY_ACTOR,
         SHOW_CONTROL_PANELS,
         SHOW_ARROWS_IN_SLOT_HISTORY_STATE,
         UPDATE_PAYTABLE_AWARD_PROPERTIES,
         ADD_DYNAMIC_BET_BUTTON,
         SET_DYNAMIC_BET_ALIAS,
         SYNCHRONIZE_DYNAMIC_SLOT_BET_BUTTONS,
         ENABLE_DYNAMIC_SLOT_BET_BUTTONS,
         SET_SINGLE_CREDIT_ROLLUP_THRESHOLD,
         POWERHIT_SPIN_REELS,
         DELAY_STANDALONE_PROGRESSIVE_METER_RESET,
         SYNCHRONIZE_STANDALONE_PROGRESSIVE_METERS,
         SET_DEFAULT_ROLLUP_RULE,
         SET_ROLLUP_RULE,
         CLEAR_LAST_GAME_WIN_AMOUNT,
         AUTO_START_GAME_ON_ROLLUP_SLAM,
         SYNCHRONIZE_VIRTUAL_SPECTRUM_WITH_REAL_VALUE,
         SET_DYNAMIC_BUTTON_BLANK_IMAGE,
         MOVE_NEXT_QUEUED_REEL,
         SHOW_VOLUME_BUTTON_WITH_MORE_GAMES_ENABLED,
         ADD_PAYTABLE_AWARD_PROPERTY,
         SYNCHRONIZE_PAYLINES,
         EXPLODE_SYMBOLS,
         ENABLE_SLAM_MECHANICAL_REELS,
         SET_DOUBLEUP_ROLLUP_NEEDED_FLAG,
         CHECK_RMLP_TRIGGERED,
         GET_RMLP_AWARD_AMOUNT,
         // MAX_FUNCTION_ID must always be last in the list.
         MAX_FUNCTION_ID,
      };
      // Script object IDs.  Base class script IDs start at 0.  Derived classes must start at MAX_OBJECT_ID.
      enum
      {
         // MAX_OBJECT_ID must always be last in the list.
         MAX_OBJECT_ID = GamePresentation::MAX_OBJECT_ID
      };

      // Defines the behavior of held reels.
      static const uint32 DO_NOT_SPIN_REELS_IF_REELS_ARE_HELD  = 0;
      static const uint32 SPIN_REELS_IF_REELS_ARE_HELD         = 1;
      static const uint32 SPIN_REELS_IF_ALL_REELS_ARE_HELD     = 2;
      uint32 held_reel_spin_config;

      // Defines the property strings.
      static const char *const BET_MESSAGE_PROPERTY_NAME;
      static const char *const WAYS_SELECTED_PROPERTY_NAME;
      static const char *const BASE_GAME_PAYS_STRING;
      static const char *const BASE_GAME_PAYS_HISTORY;
      static const char *const TOTAL_GAME_PAYS_HISTORY;
      static const char *const BASE_GAME_WIN_HISTORY;
      static const char *const TOTAL_GAME_WIN_HISTORY;
      static const char *const CREDIT_METER_VALUE_BEFORE_BONUS;
      static const char *const LOWEST_PLAYER_DENOMINATION;
      static const char *const WIN_UP_TO_DENOMINATION;
      static const char *const WIN_UP_TO_MAX_BET_METER;
      static const char *const WIN_UP_TO_MAX_WIN_METER;
      static const char *const MAXIMUM_WIN_UP_TO_MAX_WIN_METER;
      static const char *const BONUS_X_WIN_UP_TO_STRING;
      static const char *const METER_PANEL_COLOR_RED_VALUE;
      static const char *const METER_PANEL_COLOR_GREEN_VALUE;
      static const char *const METER_PANEL_COLOR_BLUE_VALUE;
      static const char *const PROPERTY_SPECTRUM_DISPLAY_VALUESTRING_ROOT;
      static const char *const PROPERTY_SPECTRUM_TEXT_DISPLAY_VALUE_ROOT;
      static const char *const COMBINE_BASE_AMOUNT_TRUNK;

      // Meter panel color properties
      static const uint8 NUMBER_OF_COLOR_VALUES;
      static const float MAX_COLOR_VALUE;

      // virtual spectrum properties
      static const char *const PROPERTY_SPECTRUM_AMOUNT_NOTIFICATION_ROOT;
      static const char *const PROPERTY_SPECTRUM_TEXT_NOTIFICATION_ROOT;

      // Defines the button strings.
      static const char *const BET_N_COINS_BUTTON;
      static const char *const STOP_BUTTON;
      static const char *const MAX_BET_BUTTON;

      EnrollDblTapButtonId enrollment_double_tap_button_id; // button id to support CDS double-tap feature.

      // Control panels.
      String igame_control_panel;

      // Win Message string
      String win_message_prefix;

      // set default press button to start the bonus message. This is used for the New 2006 Style Bonusing.
      String press_button_to_start_bonus_text;

      // hide/show multi-way segment mask events
      String hide_multi_way_segment_mask_event;
      String show_multi_way_segment_mask_event;

       // The glass name related to the current theme.
      String DG_load_event_base;
      String current_glass_name;

      // Map to determine a picture index from button bet data
      DynamicSlotBetButtonMap dynamic_slot_bet_button_map;
      String dynamic_slot_bet_button_alias;

      DynamicSlotBetButtonAuxiliaryData dynamic_button_aux_data;
      // If true, update dynamic bet buttons on syncbetconfig
      bool update_dynamic_bet_buttons;

      // This object is used to define the actors needed while in the slot evaluation page
      struct SlotEvaluationActorInformation
      {
         struct StopInformation
         {
            String actor_name;
            String parent_name;

            typedef InterfaceExtractor2 (ActorTextInterface, ActorInterface) ActorTextExtractor;
            ActorTextExtractor stop_actor;
         };

         // Actors used to display at which position the reel is stopped.
         vector <StopInformation> stop_display;
      };
      
      /// <summary> SpectrumUpdateManager object </summary>
      SpectrumUpdateManager spectrum_update_manager;

      ////////////////////////////////////////////////////////////////////////////
      /////////////////////  Serializer Support  /////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////
   protected:
      SlotEssentialRecord * slot_essential_record_ptr;     // ptr to object that contains slot essential record data.
      // Construct the object that will contain and unserialize the essential record data.
      virtual void constructEssentialRecord (void);
      // Return a pointer to the slot essential record.
      virtual const SlotEssentialRecord* getSlotEssentialRecord (void);

      SlotSharedEssentialRecord * slot_shared_essential_record_ptr;     // ptr to object that contains slot essential record data.
      // Construct the object that will contain and unserialize the essential record data.
      virtual void constructSharedEssentialRecord (void);
      // Return a pointer to the slot essential record.
      virtual const SlotSharedEssentialRecord* getSlotSharedEssentialRecord (void);
      /// <summary> get the number of sunBets from the configuration file</summary>
      uint32 getNumberOfSubBetsFromConfigFile();


      //////////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////// Symbol Substitution Support////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////
   public:
      /// <summary> Substitute symbol at specified reel position  </summary>
      /// <remarks> Please don't call this function alone from presentation. This function is designed to get caled by symbol substitution code and used together with symbol substitution code in flow side </remarks>
      virtual void substituteSymbolAtPosition (SlotEssentialRecord::ReelIndex reel_num, uint32 position, int16 symbol_id, SlotEssentialRecord::ReelIndex game_set_index = 0);
      /// <summary>Restore symbol at specified reel position.  </summary>
      virtual bool restoreSymbol (SlotEssentialRecord::ReelIndex reel_index, uint32 stop, SlotEssentialRecord::ReelSetIndex game_set_index = 0);

   protected:
      /// <summary> Initial symbol substitution.  </summary>
      virtual void initSymbolsSubstitution (void);

      ////////////////////////// End of code for Symbol Substitution Support/////////////////////////

/*!
   @scriptableFunctionsPage
   @scriptableFunctionsSubPage{SlotPresentation}
   @scriptableFunctionsPageDescription
*/

/// @name Scriptable Functions
/// @{
   public:
      /*! @appendScriptableFunctionsPage{SlotPresentation}
          @addFunctionLink{SlotPresentation,setDynamicButtonBlankImage,two_param} */
      /// <summary>Scriptable function that sets the image to use when a dynamic button is blank.</summary>
      virtual void setDynamicButtonBlankImage (String button_name, uint32 image_index);

      /*! @appendScriptableFunctionsPage{SlotPresentation}
          @addFunctionLink{SlotPresentation,setDynamicButtonBlankImage,three_param} */
      /// <summary>Scriptable function that sets the image to use when a dynamic button is blank.</summary>
      virtual void setDynamicButtonBlankImage (String button_name, uint32 image_index, String resource_alias);

      /*! @appendScriptableFunctionsPage{SlotPresentation}
          @addFunctionLink{SlotPresentation,explodeSymbols} */
      /// <summary>Scriptable function for symbol explodes.</summary>
      virtual void explodeSymbols (void);
/// @}

   public:
      IMPLEMENT_INTERFACE_BEGIN (SlotPresentation)
         IMPLEMENT_INTERFACE (SlotInterface)
         CHAIN_PARENT_INTERFACE (GamePresentation)
      IMPLEMENT_INTERFACE_END ()
};

#endif
