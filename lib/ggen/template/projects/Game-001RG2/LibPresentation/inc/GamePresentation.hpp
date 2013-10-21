/* (c) Copyright 2000-2003 International Game Technology */

/*
   $Id: GamePresentation.hpp,v 1.0, 2013-08-23 18:28:05Z, Shaw, Jeffrey$
   $Date: 3/12/2012 4:24:42 PM
   $Revision: 1$
   Based on revision 334

   Purpose:
      Defines the base class of all AVP game presentation SOs.

   Modifications:
      Author          Date       Explanation
      --------------- ---------- ------------
      Dwayne Nelson   Oct 20, 99 New
*/

#ifndef __GAMEPRESENTATION_HPP__
#define __GAMEPRESENTATION_HPP__

#include <map>
#include <vector>
#ifndef __ACTORINTERFACE_HPP__
#include "ActorInterface.hpp"
#endif
#ifndef __ACTORSHELLPAGEINTERFACE_HPP__
#include "ActorShellPageInterface.hpp"
#endif
#ifndef __ACTORSTAGEINTERFACE_HPP__
#include "ActorStageInterface.hpp"
#endif
#ifndef __ACTUABLE_HPP__
#include "Actuable.hpp"
#endif
#ifndef __ACTUATORINTERFACE_HPP__
#include "ActuatorInterface.hpp"
#endif
#ifndef __AWARDINTERFACE_HPP__
#include "AwardInterface.hpp"
#endif
#ifndef __BANKBET_HPP__
#include "BankBet.hpp"
#endif
#ifndef __BETINTERFACE_HPP__
#include "BetInterface.hpp"
#endif
#ifndef __BINGOMETERDATA_HPP__
#include "BingoMeterData.hpp"
#endif
#ifndef __CONFIGGROUP_HPP__
#include "ConfigGroup.hpp"
#endif
#ifndef __CONFIGURATIONCHANGEDHANDLERINTERFACE_HPP__
#include "ConfigurationChangedHandlerInterface.hpp"
#endif
#ifndef __CONTEXTDEFS_HPP__
#include "ContextDefs.hpp"
#endif
#ifndef __CONTROLPANELINTERFACE_HPP__
#include "ControlPanelInterface.hpp"
#endif
#ifndef __CRITICALDATA_HPP__
#include "CriticalData.hpp"
#endif
#ifndef __DENOMINATION_HPP__
#include "Denomination.hpp"
#endif
#ifndef __DRAWABLEINTERFACE_HPP__
#include "DrawableInterface.hpp"
#endif
#ifndef __ESSENTIALRECORD_HPP__
#include "EssentialRecord.hpp"
#endif
#ifndef __EVALUATORINTERFACE_HPP__
#include "EvaluatorInterface.hpp"
#endif
#ifndef __EVENTLISTENERCONTROLLER_HPP__
#include "EventListenerController.hpp"
#endif
#ifndef __GAMECONTROLPANELINTERFACE_HPP__
#include "GameControlPanelInterface.hpp"
#endif
#ifndef __GAMECOUNTCONTROLPANELINTERFACE_HPP__
#include "GameCountControlPanelInterface.hpp"
#endif
#ifndef __GAMEPRESENTATIONQUERYINTERFACE_HPP__
#include "GamePresentationQueryInterface.hpp"
#endif
#ifndef __GAMEREQUESTEVENT_HPP__
#include "GameRequestEvent.hpp"
#endif
#ifndef __GAMESETCONFIGURATIONINTERFACE_HPP__
#include "GameSetConfigurationInterface.hpp"
#endif
#ifndef __GAMESETINTERFACE_HPP__
#include "GameSetInterface.hpp"
#endif
#ifndef __GAMESIDEBETINTERFACE_HPP__
#include "GameSideBetInterface.hpp"
#endif
#ifndef __GAMESTATEVALUE_HPP__
#include "GameStateValue.hpp"
#endif
#ifndef __GENERICSEQUENCEEVENTS_HPP__
#include "GenericSequenceEvents.hpp"
#endif
#ifndef __IGTCONNECTGAMECLIENT_HPP__
#include "IGTConnectGameClient.hpp"
#endif
#ifndef __IGTCONNECTDISPLAYCLIENT_HPP__
#include "IGTConnectDisplayClient.hpp"
#endif
#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif
#ifndef __LIGHTSYSTEMINTERFACE_HPP__
#include "LightSystemInterface.hpp"
#endif
#ifndef __ULOCALE_HPP__
#include "ULocale.hpp"
#endif
#ifndef __METERAMOUNT_HPP__
#include "MeterAmount.hpp"
#endif
#ifndef __MULTIMEDIAEVENT_HPP__
#include "MultimediaEvent.hpp"
#endif
#ifndef __MULTIMEDIAEVENTLISTENER_HPP__
#include "MultimediaEventListener.hpp"
#endif
#ifndef __MULTIMEDIAEVENTLISTENERINTERFACE_HPP__
#include "MultimediaEventListenerInterface.hpp"
#endif
#ifndef __PAYLOADEVENTHANDLERINTERFACE_HPP__
#include "PayloadEventHandlerInterface.hpp"
#endif
#ifndef __PAYTABLEPARSERINTERFACE_HPP__
#include "PayTableParserInterface.hpp"
#endif
#ifndef __PRESENTATIONEXTENSIONCONTAINERINTERFACE_HPP__
#include "PresentationExtensionContainerInterface.hpp"
#endif
#ifndef __PRESENTATIONEXTENSIONINTERFACE_HPP__
#include "PresentationExtensionInterface.hpp"
#endif
#ifndef __PRESENTATIONSTATEINTERFACE_HPP__
#include "PresentationStateInterface.hpp"
#endif
#ifndef __PROPERTYINTERFACE_HPP__
#include "PropertyInterface.hpp"
#endif
#ifndef __QUICKPICK_HPP__
#include "QuickPick.hpp"
#endif
#ifndef __REGISTRY_HPP__
#include "Registry.hpp"
#endif
#ifndef __RESOURCEALIASINTERFACE_HPP__
#include "ResourceAliasInterface.hpp"
#endif
#ifndef __RESOURCELIST_HPP__
#include "ResourceList.hpp"
#endif
#ifndef __RESPONDER_HPP__
#include "Responder.hpp"
#endif
#ifndef __SEQUENCEEVENTLISTENERINTERFACE_HPP__
#include "SequenceEventListenerInterface.hpp"
#endif
#ifndef __SEQUENCEEVENTPOSTER_HPP__
#include "SequenceEventPoster.hpp"
#endif
#ifndef __SHAREDESSENTIALRECORD_HPP__
#include "SharedEssentialRecord.hpp"
#endif
#ifndef __STATE_HPP__
#include "State.hpp"
#endif
#ifndef __STATEMACHINE_HPP__
#include "StateMachine.hpp"
#endif
#ifndef __STATEPAIR_HPP__
#include "StatePair.hpp"
#endif
#ifndef __SYSTEM_HPP__
#include "System.hpp"
#endif
#ifndef __SYSTEMFLAGS_HPP__
#include "SystemFlags.hpp"
#endif
#ifndef __SYSTEMAWARDINTERFACE_HPP__
#include "SystemAwardInterface.hpp"
#endif
#ifndef __THEATREINTERFACE_HPP__
#include "TheatreInterface.hpp"
#endif
#ifndef __THEATREOPENGLDEFS_HPP__
#include "TheatreOpenGLDefs.hpp"
#endif
#ifndef __PROGRESSIVEMANAGER_HPP__
#include "ProgressiveManager.hpp"
#endif
#ifndef __PERSISTENTPROPERTYINTERFACE_HPP__
#include "PersistentPropertyInterface.hpp"
#endif
#ifndef __PERSISTENTPROPERTYLIST_HPP__
#include "PersistentPropertyList.hpp"
#endif
#ifndef __SERIALIZERHELPER_HPP__
#include "SerializerHelper.hpp"
#endif

class GamePresentation : public ConfigurationChangedHandlerInterface,
                         public GamePresentationQueryInterface,
                         public PresentationExtensionInterface,
                         public Actuable,
                         public SequenceEventListenerInterface,
                         public MultimediaEventListener,
                         public SerializerHelper::SynchronCallbackInterface,
                         public PayloadEventHandlerInterface
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
   ///////////////////// Award & Bet & Evaluator & Record //////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // Get the act index.
      ActIndex actIndex (void) const
         { return act_index; }
      // Retrieve the Amount of the current player wagerable meter.
      virtual void wagerableDisplayAmount (Amount &wagerable_amount);
      // The following method returns true if the game is in a state that allows betting.
      virtual bool canBet (void);
      // Returns true the flow has determined that it's ok to show the award to the player.
      virtual bool canDisplayAward (void);
      // Determine if the game can start from its current state.
      virtual bool canStart (void);
      // Retrieve the Amount of the current player credit meter.
      virtual void creditDisplayAmount (Amount &credit);
      // Retrieve the Amount of the current wager credit meter.
      virtual void wagerCreditDisplayAmount (Amount &credit);
      // Returns the current paytable section.
      virtual bool currentPaytableSection (String &paytable_section) const;
      // Return true if the game can start playing before game enrollment is complete.
      bool enrollmentBeforeGameStart (void) const;
      // Return true if game enrollment is required for this jursdiction.
      bool enrollmentRequired (void) const;
      // Find the evaluator to use for the given paytable section. Returns true on success.
      virtual bool evaluatorForPaytableSection (Mojo<EvaluatorInterface> &evaluator, const char *section);
      // Returns the transaction id to use for games evaluations.
      uint32 evaluationTransactionId (void) const;
      // Extract the bonus win from the award interface. Returns true if successful.
      virtual bool extractBonusWinFromAward (Amount &bonus_win_amount);
      // Extract the total win multiplier from the award interface. The Bonus Manager may add a
      // multiplier to the total win in the form of an additional award item in the base game.
      virtual bool extractBonusManagerMultiplierFromAward (uint32 &total_win_multiplier);
      // Extract the mismatch win adjustment amount from the award interface. Returns true if successful.
      virtual bool extractMismatchWinAdjustmentFromAward (Amount &total_mismatch_win);
      /// <summary> Extract the total win from the current act to the last act using an award interface.</summary>
      virtual bool extractTotalWinFromAward (Amount &total_win_amount);
      /// <summary> Extract the total win for the current act from the award interface.</summary>
      virtual bool extractWinForThisAct (Amount &win_amount);
      /// <summary> Extract the total win from all acts from the award interface.</summary>
      virtual bool extractWinForAllActs (Amount &win_amount);
      // Return the game identifier
      const GameIdentifier &gameIdentifier (void) const;
      // Return true if a game was unenrolled since the last play.
      bool gameWasUnenrolled (void);
      // Return the number of times this game still needs to be played.
      virtual uint16 gamesRemaining (void);
      // Get the current award interface. Use slotAwardInterface to retrieve
      // the current slot award information stored in critical data.
      bool getCurrentAward (Mojo<AwardInterface> &award);
      // Return the current game denomination.
      const Denomination& getDenomination (void) const
         { return denomination; }
      // Return true if the central determination RNG seeds have arrived for this game.
      bool getGameEnrollmentSeedsArrived (void);
      // Get the initiating act index.
      ActIndex initiatingAct (void) const;
      // Get the initiating stage index.
      StageIndex initiatingStage (void) const;
      // Returns true if the game is operating at max bet.
      virtual bool isBetAtMaxBet (void);
      // Return true if one or more progressives are linked.
      virtual bool isProgressiveLinked (void);
      // Retrieve the Amount of the current sub bet meter.
      virtual void numberOfSubBets (uint32 &number_of_sub_bets);
      // Retrieve the Amount of the current player paid meter.
      virtual void paidDisplayAmount (Amount &paid);
      // Returns true if the provided paytable section name is supported by this game stage.
      virtual bool paytableSectionSupported (const char *section);
      // Return the play count for the given act type.
      // ACT_TYPE_MAX will return the 'original' playCount set in GameFlow::start()
      uint16 playCount (ActType act_type = ACT_TYPE_MAX);
      // Ask the game for its current play count information
      bool playCountStatus (uint16& play_count, uint16& total_plays);
      // with the current award data. Return true if the award information has changed since the
      // last time the information was requested.
      virtual bool readAwardFromEssentialRecord (Mojo<AwardInterface> &award);
      /// <summary># posts an event to tell the event-driven control panel presentation to perform the specified action.</summary>
      bool postPresentationAction (ScriptTableParameter & payload);
      /// <summary># Adds an action and the associated data to the passed payload.</summary>
      void addPresentationAction (ScriptTableParameter & payload, const char * action, ScriptTableParameter * action_data);
      /// <summary>Get the state machine values and bundle them up in a payload.</summary>
      void createStateInfoPayload (ScriptTableParameter & payload);
      // Get the session id.
      uint32 sessionId (void) const
         { return session_id; }
      // Get the stage index.
      StageIndex stageIndex (void) const
         { return stage_index; }
      // Fill in the total amount won from the award_list on the given stage index
      // Returns true on success.
      bool totalAmountWonOnStage (StageIndex stage_index, Amount &amount);
      // Retrieve the Amount of the current total bet meter.
      virtual void totalBetDisplayAmount (Amount &total_bet);
      // Return the number of times this game has been played.
      virtual uint16 totalPlays (void);
      // Update the System display timer text with a formatted string for the given value.
      virtual void updateSystemTimer (const uint32 timer_seconds);
      // Update the System display timer text property with the given string
      bool updateTimerString (String new_timer_string);
      // Update the TotalBetMeter with the given value.
      virtual void updateTotalBetMeter (const Amount &value);
      // notify presentation elements of tournament changes
      virtual void updateTournamentInfo (void);
      // Retrieve the Amount of the current win meter.
      virtual void winDisplayAmount (Amount &win);
      // Return true if the current game win has been cashed out (because of a handpay, etc).
      bool winWasCashedOut (void);

      /// <summary>Returns the igt_connect object.</summary>
      bool getIGTConnect(Mojo<IGTConnectGameInterface> &output);
      /// <summary>Returns the igt_connect_display object.</summary>
      bool getIGTConnectDisplay (Mojo<IGTConnectDisplayClientInterface> &output);

      // Game sounds volume methods
      // increment the relative game sounds volume.  Wrap to the lowest value if we are at the highest
      virtual void incrementVolume (void);
      // Returns the game speed config value.
      virtual int32 getGameSpeed (void);
      // Returns the maximum avialable game speed config value.
      virtual int32 getMaxGameSpeed (void);
      // Returns the minimum avialable game speed config value.
      virtual int32 getMinGameSpeed (void);
      // Return if the speed is player modifiable.
      virtual bool isSpeedPlayerModifiable (void);
      // Returns the current value of the volume config.
      virtual bool okayToEnableVolumeButton (void);
      // Returns the current value of the suspend game display config.
      virtual bool okayToSuspendGameDisplayEarly (void);
      // reset the relative volume to the default (0)
      virtual void resetVolume (void);
      // set the the current volume level in a property
      virtual void setVolumeProperty (void);
      // set the the current volume button state and resets the volume if it isn't shown
      virtual void synchronizeVolumeButton (bool enable);

      //Game Celebration methods
      //Posts events that determine whether or not to show celebration, what rollup sound to play, and what rollup modifier to use
      virtual void compareWinToBetMultiplier (String mode, float multiplier, String if_event, String else_event);
      // Extract the value from and Amount object
      int64 getAmountValue (const Amount &amount);
      // Return conditional that allows including award list credits during win rollups
      virtual bool includeAwardListCredits (void);
      // Check to see if machine is running Bingo
      virtual bool isBingo (void);
      // Identify if game is unlicenced.
      virtual bool isGameThemeUnlicensed (void);
      /// <summary>Get the ER values and create a payload.</summary>
      virtual void getERData (class ScriptTableParameter &payload);
   public:
      /// <summary> Determine if the bet object supports the GameSideBetInterface </summary>
      bool doesBetSupportGameSideBetInterface ();

   protected:
      // DATA:
      static const uint32 MAX_SAFE_STORE_NAME_PRESENTATION_THEME_NAME_SIZE = 100;
      static const char * const SAFE_STORE_NAME_PRESENTATION_INFORMATION;
      static const uint32 WARM_BOOT_RESET_SEQUENCE_ID = 0xFFFFFFFF;
      // This structure contains all presentation specific (non-pay) information that needs to be stored in safe storage. Member is safe_store_game_presentation.
      struct GamePresentationSafeStorage
      {
         // Name of the current theme.
         char current_theme [MAX_SAFE_STORE_NAME_PRESENTATION_THEME_NAME_SIZE];
         // Last warm power up breakpoint index. Used for stepping through various power hits in the states as a test.
         uint32 warm_boot_index;
      };
      // Information about the stage that initiating this stage. Set by IPC.
      struct Initiating
      {
         Initiating (void) : stage (NO_STAGE), act (NO_ACT) {}
         StageIndex stage;                // The stage index of the stage which initiated this stage.
         ActIndex act;                    // The act index of the act which initiated this act.
      } initiating;
      // IPC initialization data.
      ActIndex act_index;                 // Act index of the game.
      String award_object_name;           // Name of the object in the Award SO.
      String award_so_file_name;          // Name of the Award SO for this game.
      String current_paytable_section;    // Name of the current paytable section.
      Denomination denomination;          // Current game denomination.
      String essential_award_filename;    // Path and name of the award essential data block.
      String essential_bet_filename;      // Path and name of the bet essential data block.
      String essential_persistent_properties_directory; // Root path to persistent properties essential data blocks.
      String essential_record_directory;  // Root path to the essential data blocks.
      String essential_record_shared_directory;  // Root path to the shared essential data blocks.
      String essential_record_filename;   // Path and name of the record essential data block.
      String shared_essential_record_filename;   // Path and name of the shared essential record data block.
      String evaluator_object_name;       // Name of the object in the Evaluator SO.
      String evaluator_so_file_name;      // Name of the Evaluator SO for this game.
      GameIndex game_index;               // Game index of the game.
      GameIdentifier game_identifier;     // The game identifier
      String paytable_filename;           // Name of the paytable file for this game.
      SafeStore safe_store_directory;     // The root most directory of all safe storage for this game.
      String safe_store_directory_name;   // The path of the root most directory of all safe storage for this game.
      uint32 session_id;                  // Session id of the current game.
      StageIndex stage_index;             // Stage index of the game.
      // Non-volatile storage for the display category information. This information does
      // not affect the game outcome, only which pictures/animations/sounds the player sees.
      SafeStore safe_store_game_presentation;
      // Award.
      Mojo<AwardInterface> award_list;    // One game outcome could generate a list of awards like slot, poker, keno, etc.
      // Bet.
      Mojo <BetInterface> bet;
      // IGTConnect (MJP/WAN)
      Mojo <IGTConnectGameInterface> igt_connect;

      Mojo <IGTConnectDisplayClientInterface> igt_connect_display;
      bool igt_connect_action_required;

      // Evaluator.
      typedef map<String, Mojo <EvaluatorInterface> > EvaluatorList;
      typedef EvaluatorList::iterator EvaluatorIterator;
      typedef EvaluatorList::const_iterator ConstEvaluatorIterator;
      typedef pair<String, Mojo <EvaluatorInterface> > EvaluatorPair;
      EvaluatorList evaluator_list;
      Mojo<EvaluatorInterface> evaluator;
      Mojo<PayTableParserInterface> paytable_parser;
      // Game enrollment for central determination.
      bool enrollment_required;
      bool enrollment_before_game_start;
      // Record.
      CriticalData essential_record;
      CriticalData shared_essential_record;
      CriticalData essential_record_award;
      // Used to format amount display strings.
      ULocale locale;

      // Tournament events
      String update_tournament_info_event;
      String update_tournament_meters_event;
      String formatted_timer_text_property_updated_event;

      // Tournament properties
      String system_tournament_mode_property_name;
      String system_tournament_status_property_name;
      String system_tournament_credits_property_name;
      String system_tournament_won_property_name;
      String formatted_timer_text_property_name;

      // System Display Timer events
      String update_system_display_timer_event;
      // System Display Timer properties
      String system_display_timer_property_name;

      // Denomination text
      String mininum_bet_denomination_text;
      // Denomination events
      String update_system_denomination_event;
      String update_game_movie_denomination_event;
      String update_game_font_denomination_event;
      String update_min_bet_denomination_event;
      String hide_min_bet_denomination_event;
      String blank_movie_section;
      // Denomination properties
      String system_denomination_property_name;
      String game_denomination_property_name;
      String game_denomination_text_property_name;
      String minimum_bet_property_name;
      String denomination_movie_actor_name;

      // Used to check if it's OK to enable some buttons.
      BankBet bank_bet;
      // Flags the game as unlicensed.
      bool game_theme_is_unlicensed;

      // METHODS:
      // Execute/parse the give bet script.
      bool executeBetScript (const String &script_file);
      // Get the property by name and resource key.  Fill the property with the current denomination string.
      void formatDenomination (const Denomination &denomination, String &denomination_string);
      // Get the essential record data (don't cache the pointer).
      const EssentialRecord *getEssentialRecord (void);
      // Get the essential record data (don't cache the pointer).
      const SharedEssentialRecord *getSharedEssentialRecord (void);
      // Get the basic evaluator.
      EvaluatorInterface *getEvaluator (void)
         { return evaluator; }
      // Set the root path of safe storage blocks for this presentation.
      bool setSafeStoreDirectory (const char *name);
      // Returns the current paytable section.
      const char *currentPaytableSection (void) const;
      // Return the configured paytable file name.
      const String& getPaytableFilename (void) const
         { return paytable_filename; }
      // Configuration script file.
      const String& getScriptFilename (void) const
         { return script_filename; }
      /// <summary>Determine evaluator type</summary>
      String getEvaluatorType() const;

      /// <summary> Initialize rule based game bet object from its configuration file ,it will initalize the
      ///    bet varaibles. bet rule and bet function plugin which are configured from the configuration file. </summary>
      void loadBetConfiguration (void);
      // Load the paytable provided. If 0 is passed for the paytable filename, the current paytable will be loaded. If
      // If 0 is passed for the section name, the default paytable section will be loaded.
      virtual bool loadPaytable (const char *filename = 0, const char *section = 0);
      // Build and award from the essential record information. This method builds the game
      // presentation's local award list. This method should only be called by the game presentation.
      // to update the award information without requiring a reference mojo to the information. Any
      // presentation element that needs to use this data must utilize it's own mojo.
      virtual bool readAwardFromEssentialRecord (bool force_refresh = false);
      // Starts the credit rollup.
      virtual void startRollup (void);
      // Set the award configuration.
      virtual bool setAwardInformation (const char *so_filename, const char *object_name);
      // Load the betting shared object.
      virtual bool setBetInformation (const char *so_filename, const char *object_name);
      // Set the current paytable section.
      virtual bool setCurrentPaytableSection (const char *section);
      // The the denomination for showing meters values, betting and playing the game
      // Returns true on success.
      virtual bool setDenomination (const Denomination& denomination);
      // Tell the game where its essential data should be stored.
      // This is the information required for game history.
      // The directory is the full path of the directory where the information is stored.
      // essential_record is the name used for the essential record in the directory
      // bet is the name used for the bet in the directory
      // award is the name used for the award in the directory
      // Returns true on success.
      virtual bool setEssentialRecordInformation (const char *directory, const char *shared_directory,
         const char *essential_record, const char *bet, const char *award);
      // Set the current evaluation configuration information.
      bool setEvalInformation (const char *so_filename, const char *object_name);
      // Set the style of game enrollment to use.
      bool setGameEnrollmentInformation (bool enrollment_required, bool enrollment_before_game_start);
      // Inform the game about receiving central determination RNG seeds.
      virtual bool setGameEnrollmentSeedsArrived (void);
      // Provide the game and stage index the game should be using.
      bool setGameIndex (GameIndex game_index, StageIndex stage_index, ActIndex act_index, const GameIdentifier &game_identifier);
      // Give a game stage the infomration about the stage and act that initiated it.
      // Returns true on success.
      bool setInitiatingInformation (StageIndex initiating_stage, ActIndex initiating_act_index);
      // Set the the current menu option in a property
      virtual void setMenuProperty (void);
      // Set the paytable to be used.
      virtual bool setPaytableFilename (const char *filename);
      // Set the name of the script that will configure the presentation.
      void setScriptFilename (const String &name)
         { script_filename = name; }
      // Support the given paytable section name.
      virtual void supportPaytableSection (const char *section);
      /// <summary>Allow for derived classes to update their info when switching between paytables within the same theme.</summary>
      virtual void updateAfterPaytableSwitch (void);
      /// <summary> Update to the current meter panel configuration.</summary>
      virtual void updateMeterPanelConfiguration (void);
      // Used for stepping through various power hits in the states as a test.
      // The order_id corresponds to the order the breakpoints warm boot. They must
      // be sequential and go from 0 to N. 0xFFFFFFFF is reserved as WARM_BOOT_RESET_SEQUENCE_ID.
      bool updateWarmBootId (uint32 order_id = WARM_BOOT_RESET_SEQUENCE_ID);
      // Release all the PresistentProperty objects from critical data storage.
      void releasePersistentProperties (void);
      // Reload all the PresistentProperty objects from critical data storage.
      virtual void reloadPersistentProperties (void);
      // Enable/disable all persistent properties ablities to write to safe storage.
      virtual void enablePersistentPropertiesSaveToStorage (bool enable = true);
      // Update protected variable used to identify unlicensed game.
      virtual void updateGameThemeLicenseStatus (void);
      // Identifies game theme callouts for licensing requirement.
      virtual bool isGameLicenseRequired (void);
      // Check to see if game has been registered as licensed.
      virtual bool isThemeLicensed (void);
      // Get displayable amount of the bingo grand prize.
      void getDisplayableBingoGrandPrizeAmount (Amount &bingo_grand_prize);
      /// <summary>Get the config item value.</summary>
      ScriptParameter getConfigItem (ConfigID config_id);
      /// <summary>Get accounting ER values and bundle them up in a payload.</summary>
      void getAccountingData (ScriptTableParameter & payload);

   private:
      // Used to store value of OKAY_TO_SUSPEND_GAME_DISPLAY_EARLY config item
      bool okay_to_suspend_game_display_early;
      /// <summary># Current setting string of configuration item "MONEY HANDLING STYLE".</summary>
      String money_handling_style;
      static const char* const MONEY_HANDLING_STYLE_DEFAULT;
      static const char* const MONEY_HANDLING_STYLE_BANKED_CREDITS;

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// ConfigurationChangedHandlerInterface //////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   protected:
      virtual void configurationChanged (const ConfigID &config_id);
      ///////////////////// Configuration Items ///////////////////////////////////////////////////////
      // DATA:
      // Config items the game cares about.
      Config config;
      ConfigGroup config_group;
      // List of loaded objects that support the configuration changed interface.
      typedef vector<Mojo<ConfigurationChangedHandlerInterface> > ConfigurationChangedList;
      typedef ConfigurationChangedList::iterator ConfigurationChangedIterator;
      ConfigurationChangedList configuration_changed_list;
      // This is the unique value (set by IPC) that game uses to retrieve game configuration values.
      String game_configuration_identifier;
      // This is the unique value (set by IPC) that game uses to retrieve theme configuration values.
      String theme_configuration_identifier;
      // This is the unique value (set by IPC) that game uses to retrieve stage configuration values.
      String stage_configuration_identifier;
      // METHODS:
      // Query the specified object for interfaces that should be put into lists.
      virtual void addToLists (UnknownInterface *object);
      // Remove the specified object from the lists.
      virtual void removeFromLists (UnknownInterface *object);
      // Get a list of config items that the game cares about.  If the pointer is non zero, the
      // number_of_items parameter will be set to how many items the pointer points at.
      const char *const *configItems (uint32 &number_of_items) const;
      virtual void parseConfigurationFile (Registry& registry);
      void readConfigurationFile (void);
      bool setConfigurationFile (const char *filename);
      // Set the theme configuration identifier
      bool setConfigurationIdentifier (const char *game_identifier, const char *theme_identifier, const char *stage_identifier);
      bool setMountPoint (const char *mount_point);
      /// <summary> Get the current resource_mount_point.</summary>
      const char * getMountPoint (void) const;
      // Update the specified config item.
      virtual void updateConfigItem (const String &config_id);

      // Cache the config id for game language
      ConfigID game_language_config_id;
      // Config ID for number of playable themes
      ConfigID number_of_playable_themes_config_id;
      ConfigID pay_all_mismatched_wins_to_credit_meter_id;
      ConfigID autoplay_state_config_id;
      ConfigID who_can_start_autoplay_config_id;
      ConfigID theme_displayed_to_player_denominations_id;
      ConfigID terminal_enabled_player_denominations_id;
      ConfigID language_licensed_bitfield_config_id;
      ConfigID language_supported_list_config_id;
      // Config ID for the minimum wager interval
      ConfigID minimum_wager_interval_id;
      ConfigID all_meter_display_behavior_config_id;
      // Used to determine if CDS Manual Button Configuration is available.
      ConfigID manual_button_config_id;

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Control Panel /////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // DATA:
      // Define the game control panel interface extractors.
      typedef InterfaceExtractor2 (ControlPanelInterface, GameControlPanelInterface) GameControlPanelExtractor;
      typedef InterfaceExtractor2 (ControlPanelInterface, GameCountControlPanelInterface) GameCountControlPanelExtractor;
      // METHODS:
      // Accelerate the rollup rate of the game control panel meters.
      void accelerateRollup (void);
      // Configure Auto Play message on control panel
      virtual void configureAutoPlayMessage (void);
      // The following method updates the control panel interface received with the registered game control panel.
      // If the control panel is valid, this method will return true.
      virtual bool controlPanel (GameControlPanelExtractor &control_panel);
      // The following method updates the control panel interface received with the registered game count control panel.
      // If the control panel is valid, this method will return true.
      virtual bool controlPanel (GameCountControlPanelExtractor &control_panel);
      // Get the control panel being used by the game.
      virtual bool controlPanel (const char *alias, Mojo<ControlPanelInterface> &control_panel);
      // Sets the win meter to zero
      virtual void clearWinMeter ();
      // Called to enable all known control panels.
      virtual void enableControlPanels (bool enable);
      // This method is called by the leavingObjectScope method to finalize the configuration
      // of the control panels registered by this game.
      virtual void finalizeControlPanelConfiguration (void);
      // Get the value of the double tap flag. Set to true when a CDS game has hit the spin button twice.
      bool getEnrollmentButtonDoubleTap (void) const;
      // Returns true if the rollup is finished.
      virtual bool isRollupComplete (void);
      // The following method assigns the new control panel to the game-specific control panel mojo as is
      // appropriate for the control panel type name received.
      virtual bool registerControlPanel (const char *type, const char *alias);
      // Called to show all known control panels.
      virtual void setControlPanelDenomination (const Denomination& denomination);
      // Set the value of the double tap flag. Set to true when a CDS game has hit the spin button twice.
      void setEnrollmentButtonDoubleTap (bool double_tapped);
      // Called to show all known control panels.
      virtual void showControlPanels (bool show);
      // Post an event for the current background color.
      virtual void synchronizeBackGroundColor (void);
      // Retrieve the Amount of the current sub bet meter.
      virtual void subBetDisplayAmount (Amount &sub_bet);
      // Called to synchronize the games denomination actor.
      virtual void synchronizeDenomination (void);
      // Called to synchronize all known control panels.
      virtual void synchronizeControlPanels (void);
      // Syncrhonize the visual display of the player wagerable meter
      virtual void synchronizeWagerableAmount (void);
      // Synchronize the visual bet components
      virtual void synchronizeBet (void);
      /// <summary> Synchronize the betting buttons. </summary>
      virtual void synchronizeBetButtons (void);
      // Synchronize the visual bet components do to a (host central determination) bet configuration change.
      virtual void synchronizeBetConfig (void);
      // Synchronize the visual cashout components.
      virtual void synchronizeCashout (void);
      // Enable or disable the dollar sign shower
      virtual void synchronizeCashoutCelebration (void);
      // Synchronize the visual transfer components.
      virtual void synchronizeTransferButton (void);
      // Synchronize the visual display of the credit amount (player amount and/or wagerable amount).
      virtual void synchronizeCreditAmount (void);
      // Synchronize the visual display of the player amount (player credits).
      virtual void synchronizePlayerAmount (void);
      // This method is called to synchronize the current values displayed by the game count and and games remaining meters.
      virtual void synchronizeGameCountMeters (void);
      // Game menu button methods
      // This method sets the the current menu button property
      virtual void synchronizeMenuProperty (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state.
      virtual void synchronizeMeters (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state after a MONEY_BET event.
      virtual void synchronizeMoneyBet (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state after a MONEY_IN event.
      virtual void synchronizeMoneyIn (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state after a MONEY_MOVED event.
      virtual void synchronizeMoneyMoved (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state after a MONEY_OUT event.
      virtual void synchronizeMoneyOut (void);
      // Synchronize the visual display of the player paid (paid meter).
      virtual void synchronizePaidAmount (void);
      // This method is called to ensure that the game meter displays are shown correctly
      // for the current game state after a SYSTEM_METERS_CHANGED event.
      virtual void synchronizeSystemMetersChanged (void);
      // Synchronize the visual display of tournament items.
      virtual void synchronizeTournament (void);
      // Synchronize the visual display of the player win meter.
      virtual void synchronizeWinAmount (void);
      // Synchronize the visual display of the player paid (paid meter).
      virtual void synchronizeBonusWinAmount (void);
      // Synchronize the visual display of the Bingo win amounts
      virtual void synchronizeBingoWinAmounts (void);

      // This method synchronizes the flashing of the win and paid meter displays.
      virtual void synchronizeWinPaidMeterCycle (void);
      // Return true if a win mismatch is pending.
      virtual bool winMismatchPending (void);
      // Returns whether or not presentation should slam credits during a win mismatch
      virtual bool okayToSlamMismatchWins (void);
      virtual bool okayToShowDrawMessages (void);
      virtual bool okayToShowAutoplayMessage (void);
      // To force enable/disable the autoplay button
      virtual void enableAutoplayButton (bool new_state);
      virtual void enableAutoplayStopButton (bool new_state);
      // Allow auto play button to be shown/Hide for this presentation.
      // Normal config rules still have to pass for the button to be shown.
      virtual void allowAutoPlayButton (bool allow_button);
      // Allow language button to be shown/Hide for this presentation.
      // Normal config rules still have to pass for the button to be shown.
      virtual void allowLanguageButton (bool allow_button);
      // Perform slot related logic for updates to Autoplay button presentation.
      virtual void updateAutoplayButtonPresentation (bool force_sync = false);
      bool canPlayAutoPlay (void);
      /// <summary>Can player start auto play?</summary>
      virtual bool canPlayerStartAutoPlay (void);
      // Is Auto Play enabled for this game?
      virtual bool isAutoPlayEnabled (void);

      virtual void checkIfDoubleUpTriggered (void);
      virtual void rollupDoubleUpWin (void);
      virtual void enableDoubleUpListener (void);

      //This method checks if Double is allowed to run
      virtual bool doubleUpPermitted (void);
      //This method checks if Double up is enabled
      virtual bool doubleUpEnabled (void);

      // This method synchronizes the denomination mode (single denom or multi denom).
      virtual void synchronizeDenominationMode (void);
      // This method is called to ensure that Denom Button is correctly enabled/disabled
      // after a SYNCRONIZE_PRESENTATION event.
      virtual void synchronizeDenomButton (void);
      // To get the bingo specific win amounts from the award list
      virtual void getBingoWinAmounts (BingoMeterData &bingo_meters);
      // Synchronize any autoplay messages
      virtual void synchronizeAutoplayMessage (void);
      // Maintain Autoplay button presentation syncronization for any changes in states.
      virtual void synchronizeAutoplayButtonPresentation (bool force_sync = false);

      // Synchronize the game language
      virtual void synchronizeGameLanguage (bool force_sync = false);
      // To determine whether the language configs are supported by the game
      virtual bool languageConfigsSupported (void);
      // This method changes the game language.
      virtual void changeGameLanguage (void);
      // To force enable/disable the language button
      virtual void enableLanguageButton (bool new_state);
      // Check if the language button can be enabled
      virtual bool canEnableLanguage (void);
      // Verify that the language button exists
      virtual bool languageButtonIsValid (void);
      // Are there multiple languages available for player.
      virtual bool isMultiLingualAvailableForPlayer (void);

      // Event posted that disables the language button event. This is needed when the double up button is
      // present in the following cases:
      // 1. Double up button and language button are using the same on-screen real estate (want it hidden)
      // 2. Double up button and language button are _not_ using hte same on-screen real estate (want it dim'ed)
      virtual void postDisableLanguageButtonEvent (void);

   protected:
      // DATA:
      // Control Panel aliases.
      String game_control_panel;
      String game_count_control_panel;
      bool enrollment_button_double_tap;
      bool win_mismatch_pending;
      // METHODS:

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Game Set //////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // DATA:
      // Game set configuration type definitions.
      typedef map<String, Mojo<GameSetConfigurationInterface> > GameSetMap;
      typedef GameSetMap::iterator GameSetMapIterator;
      typedef GameSetMap::const_iterator ConstGameSetMapIterator;
      // METHODS:
      // Returns the index of the currently configured game set.
      virtual uint32 currentGameSetIndex (void);
      // Updates the game set parameter received with this game's configured game set.
      virtual bool gameSet (Mojo<GameSetInterface> &game_set, uint32 game_set_index);
      // Updates the game set configuration parameter received with this game's current configuration.
      virtual bool gameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface);
      // Updates the game set configuration parameter received with the configuration for the paytable section name received.
      virtual bool gameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface, String &section_name);
      // Returns a constant reference to this game's game set configuration map.
      virtual const GameSetMap &gameSetConfigurationMap (void);
      // Returns the index of the currently configured game set.
      virtual void setCurrentGameSetIndex (uint32 game_set_index);

   protected:
      // DATA:
      // Represents the current game set index. In the default game presentation, there is only one game set by default.
      static const uint32 DEFAULT_GAME_SET_INDEX = 0;
      // The currently selected game set (is the index in the list of game sets).
      uint32 current_game_set_index;
      // Defines the map of registered game sets.
      typedef pair<String, Mojo<GameSetConfigurationInterface> > GameSetMapPair;
      GameSetMap game_set_configuration_map;
      // METHODS:
      // Creates a new game set configuration as appropriate for this presentation.
      virtual bool createGameSetConfiguration (Mojo<GameSetConfigurationInterface> &game_set_configuration_interface);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Game State ////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // DATA:
      // Presentation state condition string constants.
      static const char *const CONDITION_FIRST_GAME;
      static const char *const CONDITION_NOT_FIRST_GAME;
      static const char *const CONDITION_PLAY_COUNT;
      static const char *const CONDITION_NOT_PLAY_COUNT;
      static const char *const CONDITION_GAMES_REMAINING;
      static const char *const CONDITION_LAST_GAME;
      static const char *const CONDITION_NOT_LAST_GAME;
      static const char *const CONDITION_WIN_THIS_ACT;
      static const char *const CONDITION_WIN_ALL_ACTS;
      static const char *const CONDITION_NO_WIN_THIS_ACT;
      static const char *const CONDITION_NO_WIN_ALL_ACTS;
      static const char *const CONDITION_AWARD_THIS_ACT;
      static const char *const CONDITION_NO_AWARD_THIS_ACT;
      static const char *const CONDITION_IS_ACT_TYPE;
      static const char *const CONDITION_NOT_ACT_TYPE;
      static const char *const CONDITION_WIN_CATEGORY;
      static const char *const CONDITION_NOT_WIN_CATEGORY;
      static const char *const CONDITION_ROLLUP_COMPLETE;
      static const char *const CONDITION_NOT_ROLLUP_COMPLETE;
      static const char *const CONDITION_PROGRESSIVE_LINKED;
      static const char *const CONDITION_NO_PROGRESSIVE_LINKED;
      static const char *const CONDITION_PLAYER_SELECTABLE_DENOMINATION;
      static const char *const CONDITION_NO_PLAYER_SELECTABLE_DENOMINATION;
      static const char *const CONDITION_PLAYER_SELECTABLE_GAME;
      static const char *const CONDITION_NO_PLAYER_SELECTABLE_GAME;
      static const char *const CONDITION_ENROLLMENT_REQUIRED;
      static const char *const CONDITION_NOT_ENROLLMENT_REQUIRED;
      static const char *const CONDITION_ENROLLMENT_BEFORE_GAME_START;
      static const char *const CONDITION_NOT_ENROLLMENT_BEFORE_GAME_START;
      static const char *const CONDITION_GAME_WAS_UNENROLLED;
      static const char *const CONDITION_NOT_GAME_WAS_UNENROLLED;
      static const char *const CONDITION_GAME_ENROLLMENT_SEEDS_ARRIVED;
      static const char *const CONDITION_NOT_GAME_ENROLLMENT_SEEDS_ARRIVED;
      static const char *const CONDITION_ENROLLMENT_BUTTON_DOUBLE_TAP;
      static const char *const CONDITION_NOT_ENROLLMENT_BUTTON_DOUBLE_TAP;
      static const char *const CONDITION_PAYTABLE_SECTION;
      static const char *const CONDITION_NOT_PAYTABLE_SECTION;
      static const char *const CONDITION_WIN_GREATER_THAN_TOTAL_BET_X;
      static const char *const CONDITION_WIN_NOT_GREATER_THAN_TOTAL_BET_X;
      static const char *const CONDITION_WIN_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X;
      static const char *const CONDITION_WIN_NOT_GREATER_THAN_OR_EQUAL_TO_TOTAL_BET_X;
      static const char *const CONDITION_WIN_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X;
      static const char *const CONDITION_WIN_NOT_GREATER_THAN_SUB_BET_X_MAX_SUB_BETS_X;
      static const char *const CONDITION_WIN_GREATER_THAN;
      static const char *const CONDITION_WIN_NOT_GREATER_THAN;
      static const char *const CONDITION_WIN_THIS_ACT_GREATER_THAN;
      static const char *const CONDITION_WIN_THIS_ACT_NOT_GREATER_THAN;
      static const char *const CONDITION_PLAYER_SELECTABLE_VOLUME;
      static const char *const CONDITION_NO_PLAYER_SELECTABLE_VOLUME;
      static const char *const CONDITION_CAN_DISPLAY_AWARD;
      static const char *const CONDITION_CANNOT_DISPLAY_AWARD;
      static const char *const CONDITION_SPEED_PLAYER_MODIFIABLE;
      static const char *const CONDITION_SPEED_NOT_PLAYER_MODIFIABLE;
      static const char *const CONDITION_CAN_START_GAME;
      static const char *const CONDITION_CAN_NOT_START_GAME;
      static const char *const CONDITION_TOURNAMENT_ENABLED;
      static const char *const CONDITION_TOURNAMENT_DISABLED;
      static const char *const CONDITION_GET_FLOW_STATE_IDLE;
      static const char *const CONDITION_GET_FLOW_STATE_NOT_IDLE;
      static const char *const CONDITION_AWARD_TYPE;
      static const char *const CONDITION_DOUBLEUP_NOT_PLAYED;
      static const char *const CONDITION_DOUBLEUP_WON;
      static const char *const CONDITION_DOUBLEUP_LOST;
      static const char *const CONDITION_AUTO_PLAY_SIMULATE_INPUT;
      static const char *const CONDITION_RMLP_TRIGGERED;
      static const char *const CONDITION_RMLP_NOT_TRIGGERED;

      // Presentation-wide multimedia events.
      static const char *const ADVANCE_PRESENTATION_STATE;

      // Defines the volume level strings.
      static const char *const VOLUME_LEVEL_LOW_STRING;
      static const char *const VOLUME_LEVEL_DEFAULT_STRING;
      static const char *const VOLUME_LEVEL_HIGH_STRING;

      // Defines the property strings.
      static const char *const PLAYER_VOLUME_LEVEL;
      static const char *const GAME_MAX_BET_VALUE;
      static const char *const BASE_GAME_PRESENTATION_EVALUATION;
      static const char *const PROPERTY_SHOW_AUTOPLAY_MESSAGE;

      // Defines the config ID strings.
      static const char *const CONFIG_ID_GAME_SPEED;
      static const char *const CONFIG_ID_PAY_SPEED;
      static const char *const CONFIG_ID_PAY_SPEED_SPEED_PLAYER_MODIFIABLE;
      static const char *const CONFIG_ID_BACKGROUND_COLOR;
      static const char *const CONFIG_ID_VOLUME_PLAYER_SELECTABLE;
      static const char *const CONFIG_ID_VOLUME_GAME_SOUNDS_RELATIVE;
      static const char *const CONFIG_ID_VOLUME_GAME_SOUNDS;
      static const char *const CONFIG_ID_THEME_DISPLAYED_TO_PLAYER_DENOMINATIONS;
      static const char *const CONFIG_ID_TERMINAL_ENABLED_PLAYER_DENOMINATIONS;
      static const char *const CONFIG_ID_NUMBER_OF_PLAYABLE_THEMES;
      static const char *const CONFIG_ID_AUTOPLAY_STATE;
      static const char *const CONFIG_ID_SHOW_AUTOPLAY_MESSAGE;
      static const char *const CONFIG_ID_SHOW_DRAW_STATE_MESSAGE;
      static const char *const CONFIG_ID_CASHOUT_CELEBRATION_ENABLED;
      static const char *const CONFIG_ID_OKAY_TO_SUSPEND_GAME_DISPLAY_EARLY;
      static const char *const CONFIG_ID_EASY_BET_ENABLED;
      static const char *const CONFIG_ID_EASY_BET_ENABLED_LIST;
      static const char *const CONFIG_ID_MAXIMUM_TOTAL_BET;
      static const char *const CONFIG_ID_BINGO_SYSTEM_GAMING;
      static const char *const CONFIG_ID_MINIMUM_WAGER_INTERVAL;

      // Easy Bet Configuration Names
      static const char *const EASY_BET_BOOLEAN_FALSE_STRING;
      static const char *const EASY_BET_BOOLEAN_TRUE_STRING;
      // Defines the volume levels.
      static const float VOLUME_LEVEL_DEFAULT;
      static const float VOLUME_LEVEL_LOW_BELOW_40;
      static const float VOLUME_LEVEL_LOW_40_TO_49;
      static const float VOLUME_LEVEL_LOW_50_AND_ABOVE;
      static const float VOLUME_LEVEL_HIGH_BELOW_40;
      static const float VOLUME_LEVEL_HIGH_40_TO_49;
      static const float VOLUME_LEVEL_HIGH_50_TO_59;
      static const float VOLUME_LEVEL_HIGH_60_TO_69;
      static const float VOLUME_LEVEL_HIGH_70_AND_ABOVE;

      // current volume is on a scale from 10000 to 0, where 10000 is the softest and 0 is the loudest.
      // Each percent on the game sounds slider corresponds to 1000.  The scale maps linearly to db (-64db to 0db)
      // Defines the volume levels to compare the current volume to
      static const uint32 GAME_VOLUME_LEVEL_70_PERCENT  = 3000;
      static const uint32 GAME_VOLUME_LEVEL_60_PERCENT  = 4000;
      static const uint32 GAME_VOLUME_LEVEL_50_PERCENT  = 5000;
      static const uint32 GAME_VOLUME_LEVEL_40_PERCENT  = 6000;

      // tournament mode strings
      static const char * const TOURNAMENT_MODE_DISABLED;
      static const char * const TOURNAMENT_MODE_TIME_ONLY;
      static const char * const TOURNAMENT_MODE_CREDITS_ONLY;
      static const char * const TOURNAMENT_MODE_TIME_AND_CREDITS;

      // METHODS:
      // Ask the game to change context modes.
      virtual bool changeContextMode (State &state, const ContextMode &context_mode);
      // Provides state transition decision making for presentation states.
      // Returns true if all the conditions specified in the string are true.
      // All unsupported conditions are stored in unsupported_conditions.
      // Unsupported conditions don't effect the return value.
      virtual bool checkPostWaitEventConditions (const vector<String> &conditions, vector<String> &unsupported_conditions, bool force_all = false);
      // Return the current context mode.
      virtual const ContextMode& currentContextMode (void) const;
      // Return the current game mode of this context. These are modes specific to the game context.
      // Refer to GameDefs.hpp for valid modes.
      virtual const GameMode& currentGameMode (void) const;
      // NOTE: Very rarely should this method be overriden. This method coordinates the changes between
      // game and context modes. To customize the functionality of a given mode's entry, override the
      // mode-specific functions (enterModeHistory (), enterModeHibernate (), enterModeNormal (), and
      // enterModeUtility ()).
      virtual bool enterGameMode (State& state, const GameMode &game_mode, const ContextMode &context_mode);
      // NOTE: Very rarely should this method be overriden. This method is called when the enterGameMode
      // game_mode == HIBERNATE method is called. To customize history display functinality, override
      // the enterHistory method at the game presentation level.
      virtual bool enterModeHibernate (void);
      // NOTE: Very rarely should this method be overriden. This method is called when the enterGameMode
      // game_mode == HISTORY method is called. To customize history display functinality, override
      // the enterHistory method at the game presentation level.
      virtual bool enterModeHistory (void);
      // NOTE: Very rarely should this method be overriden. This method is called when the enterGameMode
      // game_mode == NORMAL method is called. To customize history display functinality, override
      // the enterHistory method at the game presentation level.
      virtual bool enterModePlay (void);
      // NOTE: Very rarely should this method be overriden. This method is called when the enterGameMode
      // game_mode == UTILITY method is called. To customize history display functinality, override
      // the enterHistory method at the game presentation level.
      virtual bool enterModeUtility (void);
      // Called when Game Manager exits our screen context.
      virtual bool exitGameMode (State& state, const GameStageExitReason &reason);
      // Given a condition list, extract the matching hit win categories. Return true if win categories are found.
      virtual bool extractHitWinCategoriesFromConditions (const vector<String> &conditions, vector<WinCategoryId> &win_categories);
      /// <summary>Sets the credit meter to display the amount in cash if the credit meter display behavior is set to "CURRENCY".</summary>
      void setCreditMeterDisplayBehavior (bool check_game_activity_status);
      // Return the easy bet configuration (disabled, 5 button, 10 button)
      virtual String getEasyBetConfiguration (void) const;
      /// <summary>Return the minimum total bet value for the current button map.</summary>
      virtual uint32 getMinBetButtonValue (void) const;
      // Get the current flow state id.
      StateId getFlowState (void) const;
      // Returns the name of the specified game mode.
      virtual bool getGameModeName (String &game_mode_name, const GameMode game_mode);
      // Returns the last reason given for exiting game mode on this presentation.
      const GameStageExitReason &getLastExitGameModeReason (void) const;
      // Get the current presentation substate.
      StateId getPresentationSubstate (void) const;
      // Get the previous flow state id.
      StateId getPreviousFlowState (void) const;
      // Get the previous presentation substate id.
      StateId getPreviousPresentationSubstate (void) const;
      // Get the previous state pair.
      StatePair getPreviousState (void) const;
      // Get the current state id.
      StatePair getState (void) const;
      // Game initialization.
      virtual bool init (State& state, bool cold_start_flag);
      // Return true if the passed context mode will cause the presentation to stop playing.
      virtual bool isASuspendContext (const ContextMode &new_mode) const;
      // Returns true if the game side bet is precommitted or game bet is committed.
      virtual bool isBetCommittedOrPrecommitted (void);
      // Retrurn true if the flow state is Idle
      virtual bool isFlowStateIdle (void);
      // This returns the current activity status. true = active, false = inactive.
      virtual bool isGameActive (void);
      // This method returns true if the game is currently hibernating.
      virtual bool isHibernating (void);
      // This method returns true if the game is in normal play mode..
      virtual bool isNormalPlay (void);
      //  Return true if a powerhit occurred during game play.
      virtual bool isPresentationPlaying (void);
      // Parse the game switch script
      void parseGameSwitchScript (void);
      // Post a state complete sequence event to the game flow for the current
      // state and presentation substate.
      void postStateComplete (void);
      // Set the last applied context mode.
      virtual const ContextMode& previousContextMode (void) const;
      // Return true if this is the first game play.
      virtual bool processConditionFirstGame (void);
      // Return true if central determination enrollment is required to complete before the game starts.
      virtual bool processConditionEnrollmentBeforeGameStart (void);
      // Return true if a central determination game has hit the spin button twice.
      virtual bool processConditionEnrollmentButtonDoubleTap (void);
      // Return true if central determination enrollment is required at all for this game.
      virtual bool processConditionEnrollmentRequired (void);
      // Return true if the central determination RNG seeds have arrived for this game.
      virtual bool processConditionGameEnrollmentSeedsArrived (void);
      // Return true if a game was unenrolled since the last play.
      virtual bool processConditionGameWasUnenrolled (void);
      // Return true if the games remaining count matches one of the specified values.
      virtual bool processConditionGamesRemaining (String parameters);
      // Return true if the play count matches one of the specified values for a certain act type.
      virtual bool processConditionPlayCount (String parameters);
      // Return true if this is the last game play.
      virtual bool processConditionLastGame (void);
      // Return true if this is multi-denom.
      virtual bool processConditionPlayerSelectableDenomination (void);
      // Return true if this is multi-game.
      virtual bool processConditionPlayerSelectableGame (void);
      // Return true if the paytable_section is equal to the current paytable section.
      virtual bool processConditionPaytableSection (String paytable_section);
      // Return true if one or more progressives are linked to the game/game stage.
      virtual bool processConditionProgressiveLinked (void);
      // Returns true if the rollup is finished.
      virtual bool processConditionRollupComplete (void);
      // Return true if a win is greater than a specified amount.
      virtual bool processConditionWinCreditRange (String parameters, const Amount &win_amount);
      // Return true if the current act resulted in a monetary win.
      virtual bool processConditionWinThisAct (void);
      // Return true if the current act resulted in a monetary win or bonus trigger.
      virtual bool processConditionAnyAwardThisAct (void);
      // Purpose: Return true if the current act_type is equal to act_name.
      virtual bool processConditionIsActType (String act_name);
      // Return true if no hit win category matches those specified.
      virtual bool processConditionNotWinCategory (String parameters);
      // Return true if a hit win category matches one of those specified.
      virtual bool processConditionWinCategory (String parameters);
      // Return true if a win matches the type specified.
      virtual bool processConditionAwardType (String parameter);
      // Return true if a win is greater than a specified amount.
      virtual bool processConditionWinGreaterThanTotalBetX (String parameters);
      // Return true if a win is greater than or equal to a specified amount.
      virtual bool processConditionWinGreaterThanOrEqualToTotalBetX (String parameters);
      // Return true if a win is greater than a specified amount.
      virtual bool processConditionWinGreaterThanSubBetXMaxSubBetsX (String parameters);
      // Return true if a win is greater than a specified amount.
      virtual bool processConditionWinGreaterThan (String parameters);
      // Return true if a win is greater than a specified amount.
      virtual bool processConditionWinThisActGreaterThan (String parameters);
      // Return true if the player can adjust the volume.
      virtual bool processConditionPlayerSelectableVolume (void);
      /// <summary>Determine whether or not AutoPlay is enabled and simulating player input</summary>
      virtual bool processConditionAutoPlaySimulateInput (void);

      // Handle the sequence event for machine activity
      virtual void processMachineActivity (const SequenceEvent &event);
      virtual bool checkForDoubleUpEnable ();

      // Request the presentation substate of the current flow state be changed.
      // Returns false if the request cannot be handled.
      // Returns true if the request will be attempted.
      bool requestPresentationSubstate (StateId substate);
      // Request the presentation substate of the current flow state be changed
      // Returns false if the request cannot be handled.
      // Returns true if the request will be attempted.
      bool requestPresentationSubstate (const char *substate_name);
      // Request the presentation substate be returned to the previous substate.
      void requestReturnToPreviousPresentationSubstate (void);
      // Set the current context mode.
      virtual bool setCurrentContextMode (const ContextMode &new_mode);
      // Set the current mode for this game context. These are modes specific to the game context.
      // Refer to GameDefs.hpp for valid modes.
      virtual bool setCurrentGameMode (const GameMode &new_mode);
      // Set the last applied context mode.
      virtual bool setPreviousContextMode (const ContextMode &new_mode);
      // This method is called to hide/show game's main_stage.
      virtual void showStage (bool show);
      // This method is used for transitioning between stages seamlessly.
      virtual void setNVMode (bool _nv_mode);
      // Purpose:  This method is used by setState to help in seamless transitions between stages.
      virtual void setShowStageOnEnter (bool _show_stage_on_enter);
      // Stop every actor in the presentation because the context is suspending or exiting.
      virtual void suspend (void);
      // Set every actor in the presentation to a known state because the context just entered or its mode changed.
      virtual void synchronize (void);
      // Returns true if a monetary win is award for any acts in the current game.
      bool winAllActs (void);  
   protected:
      // DATA:
      ProgressiveManager progressive_manager;      // Client to talk to Progressive Manager process.
      bool adjust_presentation_to_new_mode;  // True if presentation needs to be adjusted due to a context/game mode changed.
      bool current_game_activity_status;     // This flag keeps the current activity status of the machine.
      GameMode current_game_mode;            // Represents the current game play mode. See GameDefs.hpp for valid modes.
      ContextMode current_context_mode;      // Represents the current context mode. See ContextPresentation.hpp for valid modes.
      ContextMode previous_context_mode;     // Represents the last applied context mode.
      StatePair current_state;               // The current flow state and presentation state.
      StatePair previous_state;              // The previous flow state and presentation state.
      bool presentation_is_entered;          // True when the presentation has been entered. False when it has been exited.
      bool need_to_synchronize_presentation_state; // True when the next presentation state needs to be synchronized.
      bool told_to_show_stage;               // Used only by showStage. True if last posted a GamePresentation:ShowStage event.
      bool nv_mode;                          // Modified only by setNVMode. True by default.
      bool show_stage_on_enter;              // Modified only by setShowStageOnEnter. True by default.
      bool awaiting_substate_change;         // True when waiting for a setState in response to a substate request.
      StateId last_requested_flow_state_id;  // Id of last flow state requested.
      StateId last_requested_presentation_state_id; // Id of last presentation state requested.
      // Manages a list of PresentationStates.
      Mojo<StateMachine, SequenceEventListenerInterface> state_machine;
      GameStageExitReason last_exit_game_mode_reason; // Last reason given for why this presentation exited game mode.
      // METHODS:
      // Ask the game for game specific state informatin string.
      // This information is copied in the provided "state_information" buffer which has
      // been allocated to "size" bytes.
      // Returns true on success, false otherwise.
      virtual bool gameSpecificStateInformation (char * state_information, size_t size);
      // Extract trigger name string from the input string.
      void parseTriggerName (const String &string, String &trigger_name);
      // Separate the string of comma separated elements into a vector.
      template<class T> void parseCommaSeparatedList (const String &string, vector<T> &result) const;
      // Set the state.
      virtual void setState (StatePair new_state, StatePair new_previous_state);
      // Request to start the game The session id is the id of the game about to start.
      virtual bool start (State &state, uint32 session_id);
      // Return the betting mode for the given game mode
      BetInterface::BetMode betModeForGameMode (GameMode game_mode);
      // This stage needs to switch games (i.e. game id's, paytable, etc.)
      virtual bool switchGame (State &state,
            const char *registry_filename,
            const char *safe_store_directory,
            const char *essential_record_sub_directory, const char *shared_essential_record_sub_directory,
               const char *er_file_name, const char *bet_er_file_name, const char *award_er_file_name,
            const char *game_config_mgr_identifier,
               const char *theme_config_mgr_identifier, const char *stage_config_mgr_identifier,
            GameIndex game_index, StageIndex stage_index, ActIndex act_index, const GameIdentifier &game_identifier,
            StageIndex initiating_stage, ActIndex initiating_act_index,
            const Denomination& new_denom,
            const char *bet_so_filename, const char *bet_object_name,
            const char *award_so_filename, const char *award_object_name,
            const char* paytable_file_name,
            const char *evaluator_so_filename, const char *evaluator_object_name,
            bool enrollment_required, bool enrollment_before_game_start,
            const char *mount_point);

      // Synchronize the presentation.
      virtual void synchronizePresentation (const SynchronizePresentationSequenceEvent& event);
      virtual void synchronizePresentation (uint32 what_to_synchronize, uint32 bet_request_type = 0, uint32 bet_request_data = 0);
      /// <summary>Check whether the hit win category matches one of the checked categories.</summary>
      bool matchWinCategory (const vector<WinCategoryId> checked_categories, const char *trigger = 0);

  private:
      /// <summary>Simple class for retrieving game flow state and presentation substate pairs.</summary>
      class GameFlowStatePair
      {
        public:
           /// <summary>Constructor.</summary>
           GameFlowStatePair (void) :
              state (0),
              presentation_substate (0)
              {}
           /// <summary>Get the flow state of the pair.</summary>
           /// <returns>Flow state ID.</summary>
           uint32 getState (void) const
              { return state; }
           /// <summary>Get the presentation state of the pair.</summary>
           /// <returns>Presentation state ID.</returns>
           uint32 getPresentationSubstate (void) const
              { return presentation_substate; }

        protected:
           /// <summary>ID of the game flow state.</summary>
           uint32 state;
           /// <summary>ID of the flow state's related presentation state.</summary>
           uint32 presentation_substate;
      };

      /// <summary># The game state information.</summary>
      CriticalData read_only_game_state;

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// GamePresentationQueryInterface ////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // Get the GameStage.
      GameStage gameStage (void) const
         { return GameStage (game_identifier, stage_index); }

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Misc //////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // Tell the game to start/stop displaying its win information.
      virtual void displayWin (bool show = true, bool start_new_win_cycle = false);
      // Post a multimedia event describing if more than one player selectable game
      // is available to the player.
      virtual void postPlayerSelectableGameStatus (void);
      // Return the property value for the given property with the given key.
      bool getPropertyValueUint32 (const char * property_name, const char * key, uint32 & value);
      // Return the property value for the given property with the given key.
      bool getPropertyValueInt64 (const char * property_name, const char * key, int64 & value);
      // Get the property String for the given property.
      bool getPropertyString (const char * property_name, const char * key, String & value);

      // Get the current tournament mode property String.
      bool getTournamentMode (String & tournament_mode);
      // Get the current tournament status property String.
      bool getTournamentStatus (String & tournament_status);
      // Return the number of player credits in the current tournament session.
      int64 getTournamentCredits (void);
      // Return the number of credits won in the current tournament session.
      int64 getTournamentWon (void);
      // Get this presentation's configured resource list. Return true on success.
      bool resourceList (Mojo<ResourceList, ScriptInterface> &resource_list) const;
      // Update the Max bet property in script
      virtual void updateMaxBetProperty (void);
      // Check to see if we need to inform that there was a lose in double up
      virtual bool checkDoubleUpLoss (void);
      // Update the Max bet property in script
      virtual bool doubleUpWin (void);


      // Support functions for paytable awardnames
      virtual void updatePaytableAwardProperties (bool progressive_update = false) {};
      /// <summary>Updates the paytable award properties and the given progressive level.</summary>
      virtual void updatePaytableAwardProperties (bool progressive_update, uint32 progressive_level,
                                                  bool &progressive_properties_updated,
                                                  bool &non_progressive_properties_updated) {};
      void assignVirtualSpectrumLevelAssociations (void);
      virtual void updateDigitalGlass (void) {};
      // Load any neccessary information from the registry files.
      virtual void loadRegistyData (void) {};

      // METHODS:
      // Return the amount of denominations the player may choose from
      uint32 numberOfAvailablePlayerDenominations (void);
      // Return the amount of games the player may choose from
      uint32 numberOfAvailablePlayerGames (void);

      /// <summary>Sets the baseGamePresentationEvaluation flag for gamepresentation.  This flag determines
      ///   whether the reels, cards, tiles, etc. are displayed or not.  Used for Bingo</summary>
      virtual void setBaseGamePresentationEvaluationFlag (bool base_game_presentation_evaluation);
      /// <summary>Gets the baseGamePresentationEvaluation flag for gamepresentation.  This flag determines
      ///   whether the reels, cards, tiles, etc. are displayed or not.  Used for Bingo</summary>
      virtual bool baseGamePresentationEvaluation (void) const;

      // Sets the base game meter display clear flag, this allows clearing of display meters when enabling/disabling of base game (Bingo).
      virtual void setBaseGamePresentationMeterClearFlag (bool base_game_presentation_meter_display_clear);
      virtual bool baseGamePresentationMeterDisplayClear (void) const;
      // Returns the number of replays of the same paytable section the current act has awarded.
      //         Returns 0 if the award is not retriggering the same paytable section.
      virtual int32 awardIndicatesReplay (void);

      // Determine if the award list for the current act contains runnable triggers.
      virtual bool currentAwardContainsNewRunnableTriggers (void);

      /// <summary>Process a game request event.</summary>
      virtual bool processGameRequestEvent (const GameRequestEvent &event);

      struct VirtualSpectrumAssociation
      {
         uint32 virtual_spectrum_level;
         uint32 linked_progressive_game_level;
      };
      /// <summary># Scriptable function to post game side tilt.</summary>
      bool postGameSideTilt (const String& title, const String& message, bool _out_of_service);

   private:
      bool base_game_presentation_evaluation_flag;
      // This allows the bingo game to not display previous wins should the reels be enabled/disabled (Bingo).
      bool base_game_presentation_meter_clear_flag;

   protected:
      // DATA:
      // Static cashed interface to the light sytem
      static Mojo <class LightSystemInterface> light_system;
      // Random number generator for presentation items only. This rng should never be used to determine game outcome.
      QuickPick quick_pick;

      list<VirtualSpectrumAssociation> virtual_spectrum_associations;

      // METHODS:

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// MultimediaEventListenerInterface //////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      virtual bool processEvent (const MultimediaEvent &event);
      ///////////////////// Multimedia Events /////////////////////////////////////////////////////////
      // Adds the listener to the stage's listener list. Returns true on success.
      virtual bool addEventListener (Mojo<MultimediaEventListenerInterface> listener);
      // Removes the listener from the stage's listener list. Returns true on success.
      virtual bool removeEventListener (Mojo<MultimediaEventListenerInterface> listener);
      // Post act outcome events.
      bool postActOutcomeEvents (void);
      // Post game start events communicating generic and play count events.
      void postGameStartEvents (void);
      // Post a sequence event. Returns true on success.
      bool postEvent (SequenceEvent &event);
      // Posts a multimedia event through the stage's interface to the queue. Returns true on success.
      bool postEvent (MultimediaEvent& multimedia_event);
      // Posts a multimedia event through the stage's interface to the queue. Returns true on success.
      bool postEvent (const char *event_string);
      // Posts a multimedia event through the main_stage's interface to the queue.
      bool postEventAll (MultimediaEvent& multimedia_event);
      // Posts a multimedia event through the main_stage's interface to the queue. Returns true on success.
      bool postEventAll (const char *event_string);
      // Posts a MultimediaEvent to all the stages in our context.
      bool postEventContext (class MultimediaEvent& multimedia_event);
      // Posts a MultimediaEvent to all the stages in the defined context.
      bool postEventContext (class MultimediaEvent& multimedia_event, const char *context_name);
      // Posts a MultimediaEvent with the string received to all the stages in our context.
      bool postEventContext (const char *event_string);
      // Posts a MultimediaEvent to all the stages in the defined context.
      bool postEventContext (const char *event_string, const char *context_name);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// PayloadEventHandlerInterface //////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // Called by the listener controller, this method is used to process MultimediaEvents with payloads.
      virtual bool processEvent (const class MultimediaEvent& event, class ScriptTableParameter *payload);

      // Posts the MultimediaEvent received to the MultimediaEvent queue.
      bool postEventExtended (class MultimediaEvent& multimedia_event, uint64 delay, class ScriptTableParameter* payload = 0);
      // Posts a MultimediaEvent with the event string received MultimediaEvent queue.
      bool postEventExtended (const char *multimedia_event_string, uint64 delay, class ScriptTableParameter* payload = 0);
      // Posts a MultimediaEvent to all the stages in our context.
      bool postEventContextExtended (class MultimediaEvent& multimedia_event, uint64 delay, class ScriptTableParameter* payload = 0);
      // Posts a MultimediaEvent to all the stages in the defined context.
      bool postEventContextExtended (class MultimediaEvent& multimedia_event, const char *context_name, uint64 delay, class ScriptTableParameter* payload = 0);
      // Posts a MultimediaEvent with the string received to all the stages in our context.
      bool postEventContextExtended (const char *multimedia_event_string, uint64 delay, class ScriptTableParameter* payload = 0);
      // Posts a MultimediaEvent to all the stages in the defined context.
      bool postEventContextExtended (const char *multimedia_event_string, const char *context_name, uint64 delay, class ScriptTableParameter* payload = 0);


   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// PresentationExtensionInterface/////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      virtual bool handleMessage (cid_t client_coid, const MessageHeader&, Responder &responder);
      // Final preparations before this presentation is unloaded.
      virtual bool prepareForUnloading (void);
      // Set the container.
      virtual void setExtensionContainer (class PresentationExtensionContainerInterface *container);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// ScriptInterface ///////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // Return the type of this scriptable object.
      virtual const char *whoAreYou (void) const;

   protected:
      // Return an interface to an object given its id.
      virtual bool createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared);
      // Execute a function given its id.
      virtual bool execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list);
      // Called when we are about to leave the object's scope.
      virtual void leavingObjectScope (void);

      // Script function IDs. Base class script IDs start at 0. Derived classes must start at MAX_FUNCTION_ID.
      enum
      {
         ACCELERATE_ROLLUP = Actuable::MAX_FUNCTION_ID,
         ADD_STAGE_TO_CONTEXT,
         CONFIGURE_FROM_SCRIPT,
         DISPLAY_WIN,
         ENABLE_CONTROL_PANELS,
         POST_EVENT,
         POST_EVENT_ALL,
         POST_EVENT_CONTEXT,
         POST_ACT_OUTCOME_EVENTS,
         POST_GAME_START_EVENTS,
         REGISTER_CONTROL_PANEL,
         REGISTER_STAGE,
         SET_BET_SCRIPT,
         SET_CURRENT_GAME_SET_INDEX,
         SYNCHRONIZE_BACKGROUND_COLOR,
         SYNCHRONIZE_METERS,
         SYNCHRONIZE_BET_BUTTONS,
         START_ROLLUP,
         SUPPORT_PAYTABLE_SECTION,
         WARM_BOOT,
         WARM_BOOT_RESET_SEQUENCE,
         COMPARE_WIN_TO_BET_MULTIPLIER,
         UPDATE_SYSTEM_DENOMINATION_EVENT,
         UPDATE_GAME_MOVIE_DENOMINATION_EVENT,
         UPDATE_GAME_FONT_DENOMINATION_EVENT,
         MINIMUM_BET_DENOMINATION_TEXT,
         UPDATE_MIN_BET_DENOMINATION_EVENT,
         HIDE_MIN_BET_DENOMINATION_EVENT,
         BLANK_DENOMINATION_MOVIE_SECTION,
         SYSTEM_DENOMINATION_PROPERTY_NAME,
         GAME_DENOMINATION_PROPERTY_NAME,
         GAME_DENOMINATION_TEXT_PROPERTY_NAME,
         MINIMUM_BET_PROPERTY_NAME,
         DENOMINATION_MOVIE_ACTOR_NAME,
         SET_NV_MODE,
         SET_AUTOPLAY_ALIAS_NAME,
         SET_DOUBLE_UP_ALIAS_NAME,
         CHECK_IF_DOUBLE_UP_TRIGGERED,
         ROLLUP_DOUBLE_UP_WIN,
         ENABLE_DOUBLE_UP_LISTENER,
         ENABLE_LANGUAGE_BUTTON,
         DISABLE_LANGUAGE_BUTTON_EVENT,
         GET_STAGE,
         GET_EASY_BET_CONFIGURATION,
         IS_AUTO_PLAY_ENABLED,
         IS_DOUBLE_UP_ENABLED,
         IS_LANGUAGE_ENABLED,
         ALLOW_AUTOPLAY_BUTTON,
         ALLOW_LANGUAGE_BUTTON,
         GET_BET,
         GET_CONFIG_ITEM,
         GET_THEME_CONFIGURATION_IDENTIFIER,
         GET_STAGE_CONFIGURATION_IDENTIFIER,
         GET_GAME_CONFIGURATION_IDENTIFIER,
         GET_MOUNT_POINT,
         IS_FLOW_STATE_HISTORY,
         GET_NUMBER_OF_ENABLED_DENOMS,
         CHANGE_DENOMINATION,
         GET_DENOMINATION_BUTTON_DATA,
         GET_DEFAULT_DENOMINATION,
         SET_SHOW_STAGE_ON_ENTER,
         GET_PAYTABLE_PARSER,
         GET_LOCALIZED_CURRENCY_STRING,
         GET_PAYTABLE_PERCENTAGE,
         GET_RESPONSIBLE_GAMING_DATA,
         GET_NUMBER_OF_LICENSED_LANGUAGE,
         GET_MACHINE_WIDE_WIN_CAP_LIMIT,
         GET_MINIMUM_BET_DENOMINATION_TEXT,
         IS_PRESENTATION_PLAYING,
         GET_STATE_INFO_PAYLOAD,
         GET_EVALUATOR_TYPE,
         IS_WIN_CATEGORY,
         WIN_THIS_ACT,
         WIN_DOUBLEUP,
         IS_DOUBLEUP_PLAYED,
         CAN_DISPLAY_AWARD,
         IS_PLAYER_SELECTABLE_DENOMINATION,
         IS_PLAYER_SELECTABLE_GAME,
         POST_GAME_SIDE_TILT,
         END_PRESENTATION_STATE,
         RETURN_TO_PREVIOUS_PRESENTATION_STATE,
         GET_ER_DATA,
         WIN_ALL_ACTS,
         GET_IGT_CONNECT,
         GET_IGT_CONNECT_DISPLAY,
         GET_IGT_CONNECT_ACTION_REQUIRED,
         PROCESS_IGT_CONNECT_NOTIFICATIONS,
         RMLP_BONUS_TRIGGERED,
         IS_RMLP_FEATURE_ENABLED,

         // MAX_FUNCTION_ID must always be last in the list.
         MAX_FUNCTION_ID,
      };
      // Script object IDs. Base class script IDs start at 0. Derived classes must start at MAX_OBJECT_ID.
      enum
      {
         GAME_SET_CONFIGURATION = Actuable::MAX_OBJECT_ID,
         RESOURCE_LIST,
         SEQUENCE_EVENT_TRANSLATOR,
         STATE_MACHINE,
         PERSISTENT_PROPERTY_LIST,
         // MAX_OBJECT_ID must always be last in the list.
         MAX_OBJECT_ID,
      };
      String configuration_filename;                     // Path and filename of the registry file for this game.
      String resource_mount_point;                       // The mount point used by this game.
      Mojo<PersistentPropertyList, ScriptInterface> persistent_property_list; // Maintains a list of persistent properties.
      Mojo<ResourceList, ScriptInterface> resource_list; // Maintains a list of registered game properties.
      String script_filename;                            // Name of the presentation's script file read out of the registry.
      String bet_script;                                 // Bet script file name.
      String autoplay_alias_name;                        // Name of alias used in Autoplay object definition
      String double_up_alias_name;                       // Name of alias used in Doubleup object definition

      // METHODS:
      // Allow the presentation script to be broken up into smaller scripts.
      virtual void configureFromScript (const char *script_file_name);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// SequenceEventListenerInterface ////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      virtual bool processEvent (const SequenceEvent &event);
      ///////////////////// Sequence Events ///////////////////////////////////////////////////////////
      // Adds a listener to the sequence event listener controller.
      virtual bool addEventListener (Mojo<SequenceEventListenerInterface> listener);
      // Removes a listener from the sequence event listener controller.
      virtual bool removeEventListener (Mojo<SequenceEventListenerInterface> listener);
      // Returns a reference to this presentation's sequence event poster.
      SequenceEventPoster &sequenceEventPoster (void)
         { return sequence_event_poster; }

   protected:
      // DATA:
      // SequenceEventListener support.
      static const uint32 EVENT_LISTENER_GAME_PRESENTATION  = 0;
      static const uint32 EVENT_LISTENER_STATE_MACHINE      = 1;
      // Sequence event poster.
      SequenceEventPoster sequence_event_poster;
      // The sequence event listener controller is repsonsible for routing SequenceEvents to all of its
      // registered elements. The event controller class maintains a list of listeners as defined
      // by the first template parameter. It routes the events as defined by the second parameter to each
      // of the elements in its list.
      Mojo<EventListenerController<SequenceEventListenerInterface, SequenceEvent>, SequenceEventListenerInterface> sequence_event_listener_controller;

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Sound /////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // This method is called to enable/disable the sounds configured for this game presentation.
      void enableSounds (bool enable);
      // This method is called to mute/unmute the sounds configured for this game presentation.
      virtual void muteSounds (bool mute);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Stage /////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   public:
      // DATA:
      typedef InterfaceExtractor2 (ActorInterface, ActorStageInterface) StageInterfaces;
      // METHODS:
      // The following method updates the stage parameter received with the registered stage. If the stage is valid
      // this method will return true.
      virtual bool getStage (StageInterfaces &stage, bool error_on_failure = true) const;
      // The following method updates the stage parameter received with the registered stage. If the stage is valid
      // this method will return true.
      virtual bool getStage (const char *alias, Mojo<ActorStageInterface> &actor_stage_interface, bool error_on_failure = true) const;
      // Get the alias name of the main stage.
      const char *getStageName (void) const;
      // Try to find a given interface on another presentation stage of this game.
      template<class T> bool findInterface (Mojo<T> &interface)
      {
         bool found = false;  // Found the interface?
         bool stage_found = false;  // Found a stage?
         PresentationExtensionHandle handle; // Handle for the stage.
         // If this presentation is in history or utility...
         if ((currentGameMode () == GAME_MODE_HISTORY) || (currentGameMode () == GAME_MODE_UTILITY))
            System::error (LogicError (LogicError::INVALID_VALUE, ERROR_MESSAGE_FIND_INTERFACE));
         // Check each stage until found.
         stage_found = findFirstStage (handle);
         while (stage_found && !found)
         {
            // Convert this handle into an interface.
            Mojo<PresentationExtensionInterface> extension_interface;
            assertion (presentation_extension_container->getPresentationExtensionInterface (handle, extension_interface));
            // If we can convert to the given interface, we have a match.
            interface = extension_interface;
            if (interface.isValid ())
               found = true;
            else
               // Continue looking for a stage.
               stage_found = findNextStage (handle, handle);
         }
         return found;
      }
      // Try to find a given interface on the provided game stage.
      template<class T> bool findInterface (Mojo<T> &interface, const GameStage &game_stage)
      {
         bool found = false;  // Found the interface?
         PresentationExtensionHandle handle; // Handle for the stage.
         // If this presentation is in history or utility...
         if ((currentGameMode () == GAME_MODE_HISTORY) || (currentGameMode () == GAME_MODE_UTILITY))
            System::error (LogicError (LogicError::INVALID_VALUE, ERROR_MESSAGE_FIND_INTERFACE));
         // Get the requested stage handle.
         found = findStage (game_stage, handle);
         if (found)
         {
            // Convert this handle into an interface.
            Mojo<PresentationExtensionInterface> extension_interface;
            assertion (presentation_extension_container->getPresentationExtensionInterface (handle, extension_interface));
            // If we can convert to the given interface, we have a match.
            interface = extension_interface;
            if (interface.isValid ())
               found = true;
         }
         return found;
      }

   protected:
      // DATA:
      // Stage names.
      static const char *const MAIN_STAGE;
      static const char *const HELP_DISPLAY_STAGE;
      static const char *const PAYTABLE_DISPLAY_STAGE;
      static const char *const TRANSITION_STAGE;
      // System error messages.
      static const char *const ERROR_MESSAGE_FIND_INTERFACE;
      // Help display.
      typedef InterfaceExtractor2 (ActorShellPageInterface, ActorInterface) UserDisplayInterfaces;
      UserDisplayInterfaces help_display;
      String help_display_stage_alias;
      // Main Stage for this presentation.
      String stage_name;
      // Paytable display.
      UserDisplayInterfaces paytable_display;
      String paytable_display_stage_alias;
      // Hold a mojo to PresentationManager's presentation extension container. Used for searching for Stages across SOs.
      Mojo<PresentationExtensionContainerInterface> presentation_extension_container;
      // METHODS:
      // Find another stage of this game.
      // Returns true if a stage is found, false if there are no other stages for this game
      bool findFirstStage (PresentationExtensionHandle& handle);
      // Find another stage of this game.
      // Returns true if a stage is found, false if there are no other stages for this game
      bool findNextStage (const PresentationExtensionHandle& current_handle, PresentationExtensionHandle& handle);
      // Find another stage of this game.
      // Returns true if a stage is found, false otherwise.
      bool findStage (const GameStage& game_stage, PresentationExtensionHandle& handle);
      // The following method assigns the stage alias name and prepares the stage for use by the game.
      virtual bool registerHelpDisplayStage (const char *alias);
      // The following method assigns the stage alias name and prepares the stage for use by the game.
      virtual bool registerPaytableDisplayStage (const char *alias);
      // The following method assigns the stage name for the game presentation.
      virtual bool registerStage (const char *stage_name);

      enum ProgressiveShowMode
      {
         SHOW_NONE,
         SHOW_TEXT,  // This is the generic progressive text.
         SHOW_METER, // This is either the meter amount or the prize string of a particular progressive level.
      };

      //Show no progressive information
      static const uint8 NO_PROGRESSIVE = 0;
      //Show the progressive meter value
      static const uint8 PROGRESSIVE_AMOUNT = 1;
      //Show the progressive prize string
      static const uint8 PROGRESSIVE_STRING = 2;

      // Changes the denomination of the game to the index of the selected button
      void changeDenomination(uint32 new_denom_bit_index);
      // Builds the progressive information for a denom button
      ScriptParameter buildDenomButtonProgressiveInformation (ProgressiveShowMode progressive_show_mode,
         uint32 denomination_bit_index, String advertisement_string,
         Denomination denomination);
      // Retrieve the string that should be used for the progressive advertisement
      bool getAdvertisementString (String &advertisement_string);

      // Determines the progressive values for a given denomination
      uint8 determineProgressiveValues(String &progressive_prize_string,
         Amount &progressive_amount, Denomination denom);

      // Builds a Lua table of the denom button information so the actors can be configured in script
      // A diagram of the table that is returned is on the wiki
      void getDenominationButtonData (ScriptTableParameter &table, const uint32 &max_buttons);

      //Returns the bit index of the default player denomination; if the default
      //denom is not enabled the lowest denomination is returned
      uint32 getDefaultDenomination ();

      //The advertisement config string
      static const char *const ADVERTISEMENT_CONFIG_ITEM;
      //The default player denomination config string
      static const char *const DEFAULT_PLAYER_DENOMINATION;

      //The value of the DisplayStringInsteadOfProgressiveValue theme flag
      bool display_advertisement_text;

      // DATA:
      // Menu button defines
      // Defines the property string.
      static const char *const PLAYER_MENU_OPTION;

      // Defines the menu option strings.
      static const char *const MENU_OPTION_MAIN_MENU;
      static const char *const MENU_OPTION_SELECT_VALUE;
      static const char *const MENU_OPTION_ONE_CREDIT_EQUALS;

      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// Serializer ////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Used by CriticalDataSynchron class (only) to notify this class that a CriticalDataSynchron has been constructed.
      // This class can then (decide) to load/unserialize.
      virtual void synchron_constructed (State * _state_ptr, bool load_object_data = true);
      // Used by CriticalDataSynchron class (only) to notify this class that a CriticalDataSynchron has been destructed.
      // This class can then (decide) to save/serialize.
      virtual void synchron_destructed (bool save_object_data);

      // Construct the object that will contain and serialize the essential record data.
      virtual void constructEssentialRecord (void) = 0;  // Made pure virtual to insure derived classes overload.
      // Destroy the object that contains and serializes the essential record data.  Allow derived
      // classes to override if necessary.
      virtual void destructEssentialRecord (void);

      // Construct the object that will contain and serialize the shared essential record data.
      virtual void constructSharedEssentialRecord (void);
      // Destroy the object that contains and serializes the shared essential record data.  Allow derived
      // classes to override if necessary.
      virtual void destructSharedEssentialRecord (void);

      EssentialRecord * essential_record_ptr;     // ptr to object that contains essential record data.

      SharedEssentialRecord * shared_essential_record_ptr;     // ptr to object that contains shared essential record data.

      bool language_supported;
      bool player_autoplay_supported;
      bool multiple_languages_available_for_player;
      String disable_language_button_event;
      // Do not allow international buttons by default. Scriptable function call is needed to enable intl buttons.
      bool allow_autoplay_button;
      bool allow_language_button;

/*!
   @scriptableFunctionsPage
   @scriptableFunctionsSubPage{GamePresentation}
   @scriptableFunctionsPageDescription
*/

/// @name Scriptable Functions
/// @{
   public:
      /*! @appendScriptableFunctionsPage{GamePresentation}
          @addFunctionLink{GamePresentation,getConfigItem} */
      /// <summary>Get the config item value.</summary>
      ScriptParameter getConfigItem (const String& config_item_name, const String& unique_id = "", bool error = false);
      /*! @appendScriptableFunctionsPage{GamePresentation}
          @addFunctionLink{GamePresentation,getBet} */
      /// <summary>Get the current bet mojo</summary>
      virtual bool getBet (Mojo<BetInterface> &_bet);
      /*! @appendScriptableFunctionsPage{GamePresentation}
          @addFunctionLink{GamePresentation,getThemeConfigurationIdentifier} */
      /// <summary>Get the theme configuration identifier string.</summary>
      String getThemeConfigurationIdentifier ();
      /*! @appendScriptableFunctionsPage{GamePresentation}
          @addFunctionLink{GamePresentation,getStageConfigurationIdentifier} */
      /// <summary>Get the stage configuration identifier string.</summary>
      String getStageConfigurationIdentifier ();
      /*! @appendScriptableFunctionsPage{GamePresentation}
          @addFunctionLink{GamePresentation,getGameConfigurationIdentifier} */
      /// <summary>Get the game configuration identifier string.</summary>
      String getGameConfigurationIdentifier ();
      // Convert credits to a Currency string
      String getLocalizedCurrencyString (uint32 credits);
      // return if a RMLP bonus was triggered during the current session
      bool rmlpTriggered ();
      // indicate if a rmlp bonus has been triggered during the current session
      bool rmlp_triggered;
      // return if rmlp feature is active
      bool rmlpEnabled ();
      // indicate if RMLP feature is enabled
      bool rmlp_feature_enabled;

/// @}

   protected:
      /// <summary># Get the number of bet configuration items.</summary>
      uint32 getBetConfigurationItemCount (void) const;
      /// <summary># Get the string identifier for the configuration item.</summary>
      void getBetConfigurationString (uint32 bet, String& config_item_name) const;
      /// <summary># To determine if the bet config is required for games that require enrollment.</summary>
      bool isCDSBetConfigRequired (void) const;
      /// <summary># Process the bet configuration item and store into permitted_bet_list.</summary>
      void processBetConfigItem (const char* config_item, vector <BetPermittedData>& permitted_bet_list, uint16 line);
      /// <summary># Update the bet configuration.</summary>
      bool updateBetConfiguration ();
      /// <summary> Update bet permitted bet list </summary>
      void updatePermittedBetList (const vector <BetPermittedData> &permitted_bet_list);

   public:
      IMPLEMENT_INTERFACE_BEGIN (GamePresentation)
         IMPLEMENT_INTERFACE (ConfigurationChangedHandlerInterface)
         IMPLEMENT_INTERFACE (GamePresentationQueryInterface)
         IMPLEMENT_INTERFACE (PresentationExtensionInterface)
         IMPLEMENT_INTERFACE (SequenceEventListenerInterface)
         IMPLEMENT_INTERFACE (PayloadEventHandlerInterface)
         CHAIN_PARENT_INTERFACE (Actuable)
         CHAIN_PARENT_INTERFACE (MultimediaEventListener)
      IMPLEMENT_INTERFACE_END ()
};

#endif

