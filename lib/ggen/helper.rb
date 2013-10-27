# encoding: utf-8
require 'pathname'

module Ggen
  # A module containing various useful classes
  module Helper
    class Workspace < Pathname
      def initialize(p)
        super(p)
      end

      def development
        self
      end

      def games
        development + 'Games'
      end

      def projects
        development + 'projects'
      end
    end

    class GamePath < Workspace
      def initialize(p, id)
        super(p)
        @id = "Game-00#{id}"
      end

      def game_path
        games + @id
      end

      def game_base
        game_path + "Resources/Generic/Base"
      end

      def configuration
        game_base + "Configuration"
      end

      def paytables
        configuration + "Paytables"
      end

      def themes
        configuration + "Themes"
      end

      def bins
        configuration + "Bins"
      end

      def registries
        configuration + "Registries"
      end

      def proj_path
        projects + @id
      end
    end

    class TemplateGame
      def respond_to?(method)
        @hash.has_key?(method) || super
      end

      def method_missing(name, *args)
        super unless respond_to?(name)
        @hash[name]
      end

      def self.get_game(id)
        case id
        when "1RG2" then
          return GAME001RG2
        when "4RG4" then
          return GAME004RG4
        end
      end

      private
      def initialize(h = {})
        @hash = ::Hash.new.merge(h)
      end

      GAME004RG4 = TemplateGame.new(
        :payline_num => 100,
        :symbol_scripts => ["SymbolVariables.lua",
                            "CustomSymbolFunctions.lua"],
        :proj_configs => ["CommonConfigProps.props",],
      )
      GAME001RG2 = TemplateGame.new(
        :payline_num => 100,
        :config_scripts => ["G001RG2.binreg", "Dev00.registry",
                   "100L1RG2.themereg", "100L1RG2-000.config",
                   "100L1RG2-00-000.config",
                   "100L1RG2-01-000.config",
                   "100L1RG2-02-000.config",
                   "100L1RG2-03-000.config",
                   "RuleBasedGameBetConfig.xml",
                   "RuleBasedGameBetLoaderConfig.xml",
                   "GameVariables.lua",
                  ],
        :symbol_scripts =>  ["SymbolVariables.lua", "CustomSymbolFunctions.lua",
                             "BaseGameSymbolImageTranslations.lua",
                             "FreeSpinSymbolImageTranslations.lua",
                             "DynamicActorTextureList.lua",
                             "SymbolInfoValuesTable.lua",
                             "SymbolInfoImageTranslations.lua",
                             "SymbolInfoTableTranslations.lua"],
        :proj_configs => ["CommonConfigProps.props",],
      )
    end

    class StageConfig
      attr_accessor :name, :flow_bin, :presentation_bin, :bet_bin, :type
      attr_accessor :description, :feature_descriptor
      attr_accessor :utility_mode
      attr_accessor :root_dir, :flow_script, :presentation_script
      attr_accessor :files

      def initialize(h)
        h.each {|k, v| send("#{k}=", v)}
      end

      @@stages = {
        :base_game => StageConfig.new(
          :name => "Lua Ref Game 2",
          :description => "Lua Ref Game 2, Main",
          :utility_mode => "enable",
          :root_dir => "Game.Main",
          :flow_script => "FlowMain.lua",
          :presentation_script => "Main.lua",
          :type => "Slot",
          :bet_bin => "RuleBasedGameBetLoader.so",
          :flow_bin => "SlotFlow.so",
          :presentation_bin => "SlotPresentationWG.so",
        ),
        :free_spin => StageConfig.new(
          :name => "Free Games Feature",
          :description => "Lua Ref Game 2, Free Games Feature",
          :utility_mode => "enable",
          :root_dir => "Game.FreeSpinBonus",
          :flow_script => "FlowMain.lua",
          :presentation_script => "FreeSpinBonus.lua",
          :type => "Slot",
          :bet_bin => "RuleBasedGameBetLoader.so",
          :flow_bin => "FreeSpinSlotFlow.so",
          :presentation_bin => "SlotPresentationWG.so",
        ),
        :doubleup => StageConfig.new(
          :name => "INTERNATIONAL",
          :description => "INTERNATIONAL",
          :utility_mode => "disable",
          :root_dir => "Game.Doubleup",
          :flow_script => "FlowMain.lua",
          :presentation_script => "DoubleUp.lua",
          :type => "DoubleUp",
          :bet_bin => "RuleBasedGameBetLoader.so",
          :flow_bin => "DoubleUpIntlFlow.so",
          :presentation_bin => "DoubleUpIntlPresentation.so",
        ),
        :rmlp => StageConfig.new(
          :name => "RMLPFlash Bonus",
          :description => "RMLPFlash",
          :utility_mode => "disable",
          :root_dir => "Game.RMLPFlash",
          :flow_script => "FlowMain.lua",
          :presentation_script => "RMLPFlashSetResourceKey.script",
          :type => "Bonus",
          :bet_bin => "RuleBasedGameBetLoader.so",
          :flow_bin => "RMLPFlashFlow.so",
          :presentation_bin => "RMLPFlashPresentation.so",
          :feature_descriptor => "EBG",
          :files => [
            "EBG.ebgreg", "Game.RMLPFlash",
            "RMLPFlashFlow", "RMLPFlashPresentation",
            "LibPresentation", "LibSlotPresentation",
            "LibSys", "WinCycleLite"
          ]
        ),
      }

      def self.config(key, &block)
        stage = @@stages[key]
        stage.instance_eval(&block) if block
        stage
      end

    end
  end
end
