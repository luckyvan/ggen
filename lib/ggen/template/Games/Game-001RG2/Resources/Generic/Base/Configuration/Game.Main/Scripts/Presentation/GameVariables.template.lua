--[[ AVP Lua Script File
   (c) Copyright 2008 International Game Technology

   $Revision: 45$

   Purpose:
      Variables specific to the game

   Modifications:
      Author            Date          Explanation
      ---------------   ------------  ------------
      Jesse Stafford    July 8, 2008  Created
]]

-- The main scene name
scene_name           = "Main"
-- Base game resource key
key                  = "Game-00<%= gid %>." .. scene_name
game_name            = "Game." .. scene_name
path                 = game_name
stage_name           = "Stage." .. scene_name
context_name         = "Context." .. scene_name

meter_panel_context  = "Context.MeterPanel"
meter_panel_stage    = "Stage.MeterPanel"
meter_panel_key      = key

game_rules_context   = "Context.DynamicGameRules"

local unique_id = this:getStageConfigurationIdentifier ()

-- get the number of subbets (payline count) from the paytable config
local number_of_subbets = tonumber (this:getConfigItem ("NUMBER OF SUBBETS", unique_id))

-- Specify the paytable section of base game
local paytable_section = "base game"

-- include the symbol variables file
include (path .. "/Scripts/Presentation/SymbolVariables.lua")

number_of_game_sets = 1

-- Game configuration settings
config =
{
   uses_lua_wincycle = true,
   -- Win cycling pre/post bonus
   -- If set to true  - the pre bonus win cycle will complete one full cycle before the bonus is started
   --                   and there will be no win cycle after the bonus finishes
   -- If set to false - the pre bonus win cycle will stop when the rollup finishes and starts the bonus
   --                   and the win cycle will restart when the bonus completes
   full_win_cycle_before_bonus = true,
   -- Define Reels specific configurations
   reels =
   {
      -- Name of the reels object
      name         = "Reels." .. scene_name,
      script_path  = "Game.Main.Common/Scripts/Reels/",
      custom_symbol_functions = symbols.script_path .. "CustomSymbolFunctions.lua",
      paytable_section = paytable_section,
      --[[===========================================================================
      -- The following settings are overrideable per reel in the list table below
      --===========================================================================]]
      model =
      {
         filename = "Reels.model",
         root     = "GenericFlatReel",
      },
      -- Set the home_segment. This specifies the index of the reel model segment to evaluate as the first
      -- visible_symbol (index starts from 0).
      home_segment = 1,
      -- Set the visible symbols. If not defined here, it will by default be the "number of visible symbols" value
      -- defined by the paytable. This value may exceed what is defined in the "number of visible symbols" value
      -- by the paytable.
      visible_symbols = <%= visible_symbols %>,
      -- Set the animation plugin to use for this reel type
      --[[
          Values from the plugin will be used if not specified here.
          The default_anim__plugin selects from a list of available
          plugin in the anim_plugin_config table.
      --]]
      default_anim_plugin = "FlatReelPlugin",
      anim_plugin_config =
      {
         FlatReelPlugin =
         {
            -- Specify whether to preface the reel spin with a pre_start animation where the reel pulls back before it starts spinning
            include_reel_pre_start = true,
            -- Specify the reel spin velocity in symbols/second.
            spin_velocity = 17,
            -- Specify the spin direction ("down" is default). Possible Values include "down", "up, "right", "left"
            spin_direction = "down",
            -- Specify the delay to wait before the reel starts spinning (used to offset the reel spin start times). This makes more sense to use
            delay_before_spin = 0,
            -- Specify the time between stops in milliseconds
            time_between_reel_stop = 370,
            -- Specify the desired duration of the bounce at the end of the reel spin. 0 disables the bounce, but things like IntelliSymbols will still function.
            bounce_duration = 200,
         },
      },
      -- Specify the mimimum spin time for all reels to spin. This is the time from the first reel spinning to the last reel stopping.
      --  The actual spin time will vary slightly in order to achieve the desired spin_velocity, but it will always meet the minimum requirement.
      minimum_spin_time = 3000,
      -- Specify the stop order by reel_index. This may be a table or a function that returns a stop_index when given a reel_index.
      stop_order = {0,1,2,3,4},
      -- Allow individual reel snapping. If you enable this, it's up to you to ensure it works properly with anticipation.
      allow_reel_touching = true,
      --[[===========================================================================
      -- Per Reel Override Settings List
      --===========================================================================]]
      -- Define the list of overrides per reel. (If not specified, the values from the parent table will be used)
      list =
      {
         [0] =
         {
         },
         [1] =
         {
            -- Start spinning reel1 100ms after the reels are told to spin
            anim_plugin_config =
            {
               FlatReelPlugin =
               {
                  delay_before_spin = 100,
               },
            },
         },
         [2] =
         {
            -- Start spinning reel2 200ms after the reels are told to spin
            anim_plugin_config =
            {
               FlatReelPlugin =
               {
                  delay_before_spin = 200,
               },
            },
         },
         [3] =
         {
            -- Start spinning reel3 300ms after the reels are told to spin
            anim_plugin_config =
            {
               FlatReelPlugin =
               {
                  delay_before_spin = 300,
               },
            },
         },
         [4] =
         {
            -- Start spinning reel4 400ms after the reels are told to spin
            anim_plugin_config =
            {
               FlatReelPlugin =
               {
                  delay_before_spin = 400,
               },
            },
         },
      },
   },
   -- Define Paylines specific configurations
   paylines =
   {
      key   = key,
      model =
      {
         filename = "Game.Main/Models/PaylineDisplay.model",
         root     = "PaylineDisplay",
      },
      paytable_section = paytable_section,
      -- Use config item to dynamically set sub bet (payline) count.
      count = number_of_subbets,
      -- Specify the file holding the payline colors
      color_file = "Game.Main/Scripts/Presentation/PaylineColors.lua",
      -- Define the symbol tag text configuration
      symbol_tag_text =
      {
         font           = "SymbolTagFont",
         justification  = "CENTERED",
         sizing         = "GROW_HEIGHT_SHRINK_WIDTH",
         spacing        = -0.008,
      },
      highlight =
      {
         channel = 1,
         mode = "ADD",
         image = path .. "/Images/Paylines/PaylineHighlight.tga"
      },
      end_cap =
      {
         channel = 1,
         mode = "ADD",
         orientations =
         {
            Left = "Game.Main/Images/Paylines/LineCapLeftHighlight.tga",
            Right = "Game.Main/Images/Paylines/LineCapRightHighlight.tga",
         },
      }
   },
   -- Define AnimateSlotWin specific configurations
   animate_slot_win =
   {
      name      = "WinCycleLite." .. scene_name,
      object    = "WinCycleLite",
      win_cycle_complete_event = "SWA:WinCycleComplete",
      group_paylines_display = false,
      paytable_section = paytable_section,
      stage_name = scene_name,
   },
   -- Define IntelliSymbols specific configurations
   intelli_symbols =
   {
      name        = "IntelliSymbolsController." .. scene_name,
      stage       = stage_name,
      reels       = "Reels." .. scene_name,
      list =
      {
         -- define a list of symbols that IntelliSymbols should be configured for as a list of the reels that they appear on
         [symbols.special.bonus] = {reels = {1,2,3}},
      }
   },
   -- Configuration for the control panels
   control_panels =
   {
      {
         type      = "GameControlPanel",
         name      = "GameControlPanel." .. scene_name,
         object    = "SlotControlPanelWG." .. scene_name,
         stage     = stage_name,
         key       = key,
      },
      {
         type      = "SlotControlPanelWG",
         name      = "SlotControlPanelWG." .. scene_name,
         object    = "SlotControlPanelWG." .. scene_name,
         {action = "setButtonName", parameters = {"DenominationButton",  "TokenizedDenomButton"}},
      },
   },
   bet =
   {
      -- Define the location of the button map definition file
      button_map_definition_path = "Game.Main/Scripts/Shared/Bet/" .. number_of_subbets .. "LineButtonMap.lua",
      -- Define the flag which indicates the sub bets is limited to the values in the button map
      limit_select_subbets_to_panel_button_values = true,
      -- Define the flag which indicates the bet per sub bets is limited to the values in the button map
      limit_bet_per_subbets_to_panel_button_values = false,
      -- Define the flag which indicates the bets is limited to the values in the button map
      limit_bets_to_panel_button_values = true,

      -- Variable name used in bet.
      bet_name_list =
      {
         -- Name of some used bet variables, they must exactly the same as in XML configurarion file.
         --- and in case of multiple variable sets, each name must contain a colon to seperate set name and variable name.
         --- and if there's only one variable set, name without colon is permitted.
         max_bet_items_name    = "MaxBetItems",
         max_bet_per_item_name = "MaxBetPerItem",
         max_bet_name          = "MaximumTotalBet",
         min_bet_name          = "MinimumTotalBet",
         bet_per_item_name     = "BetPerItem",
         bet_items_name        = "BetItems",
         total_bet_name        = "TotalBet",
         min_bet_in_cur_buttonmap_name = "MinimumTotalBetInCurButtonMap",
      },
   },
   dynamic_button_panel =
   {
      alias    = "GameButtonPanel",
      filename = "Game.Main/Images/DynamicButtons/" .. number_of_subbets .. "Line.dbuttons",
      start_feature_animation_id = 0,
      max_bet_animation_id       = 42,
      repeat_bet_animation_id    = 43,
      cashout_animation_id       = 40,
      stop_animation_id          = 44,
      evaluate_animation_id      = 41,
      double_up_animation_id     = 45,
      stop_autoplay_animation_id = 46,
   },
   meter_panel =
   {
      -- These scripts will be called from the meter panel config.  They should return a function.
      -- That function will be passed the stage object handle.
      -- This can be a single string, or a list of file paths.
      additional_scripts_to_run =
      {
         -- Configure bet per line sounds
         "Game.Main.MeterPanel/Scripts/Sounds/LineSounds.lua",
         "Game.Main.MeterPanel/Scripts/Sounds/BetPerLineSounds.lua",
         -- Configure the physical buttons
         "Game.Main.MeterPanel/Scripts/Presentation/RBGBDynamicButtons.lua",
         "Game.Main.MeterPanel/Scripts/Presentation/SynchronizeBetButton.lua",
         "Game.Main.MeterPanel/Scripts/Presentation/WGInformationConfig.lua",
         -- Configure rollup sounds
         "Game.Main.MeterPanel/Scripts/Sounds/RollupSounds.lua",
         -- Configure the denom menu
         "Game.DenomMenu/Scripts/Presentation/DenomMenuLoader.lua",
      },
      -- Flag indicating whether the meter panel uses textures or fonts, applied to multilingual meter panel.
      use_images = false,
      -- These scripts will be called from the meter panel config.  They should return a function.
      -- This can be a single string, or a list of file paths.
      sequence_event_translator_scripts =
      {
         -- Include the system-specific translators.
         "System.Miscellaneous/Scripts/SystemSET.lua",
         -- Include the game-specific translators.
         "Game.Main.Common/Scripts/Presentation/CmnGamePanelSET.lua",
         "Game.Main/Scripts/Presentation/WGPanelSET.lua",
         "Game.Main/Scripts/Presentation/WGBetPerLineSET.lua",
         "Game.Main.Common/Scripts/Presentation/CmnMachineActivitySET.lua",
         -- Include the events used in the slot evaluation utility
         "Game.Main.Common/Scripts/Presentation/RBGBEvaluatorPanelSET.lua",
         "Game.Main/Scripts/Presentation/BetSET.lua"
      },
      -- flag for the meterpanel to use libRollup
      uses_libRollup = true,
      uses_game_side_denom_menu = true,
      -- Sound to play during X of Y rollup of Free Games.
      -- This needs to be in Main since we are using a shared meter panel with Free Spins.
      increment_sound = "Game.FreeSpinBonus/Sounds/Rollup/FSIncrement.sound",

      -- Defines config information for the meters.
      meters =
      {
         WinMeter =
         {
            -- Defines the meter display mode for the meters configured.
            -- Meter amount display mode is in CREDIT by default, the configuration
            -- here is unnecessary, but is left here to serve as an example.
            display_mode = "CREDIT",
         },
      },
   },
   rollups =
   {
      BaseGameRollup =
      {
         config_path = "Game.Main/Scripts/Presentation/BaseGameRollupConfig.lua",
         initializeRollupEvent = "BaseGame:initializeRollup",
         slamRollupEvent = "AccelerateRollupRequested:SlotAwardState",
      },
      BaseGamePreBonusRollup =
      {
         config_path = "Game.Main/Scripts/Presentation/BaseGamePreBonusRollupConfig.lua",
         initializeRollupEvent = "PreBonusBaseRollupState:initializeRollup",
         slamRollupEvent = "AccelerateRollupRequested:PreBonusBaseRollupState",
      },
      DoubleUpWinRollup =
      {
         config_path = "Game.Main/Scripts/Presentation/DoubleUpWinRollupConfig.lua",
         initializeRollupEvent = "DoubleUpWinRollup:initializeRollup",
         slamRollupEvent = "AccelerateRollupRequested:DoubleUpWinRollup",
      },
      DoubleUpNoWinRollup =
      {
         config_path = "Game.Main/Scripts/Presentation/DoubleUpNoWinRollupConfig.lua",
         initializeRollupEvent = "DoubleUpNoWinRollup:initializeRollup",
         slamRollupEvent = "AccelerateRollupRequested:DoubleUpNoWinRollup",
      },
   },
   lights =
   {
      plugins =
      {
         "Game.Main/Scripts/Lights/ButtonLights.lua",
         "Game.Main/Scripts/Lights/SuzoHapp.lua",
         "Game.Main/Scripts/Lights/HaloLights.lua",
      },
   },
   -- Define the win categories needed in the state machine and phase 3 symbol rules
   win_categories =
   {
      -- Define the list of win categories that trigger the bonus as a list of numbers
      bonus_trigger = {<%= bonus_trigger%>},
   },
   big_pig_interface_config = dofile("Game.Main/Scripts/Presentation/BigPigInterfaceConfig.lua"),
}
