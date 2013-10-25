require 'test_helper'

module Ggen
  class TestUtil < Test::Unit::TestCase
    include Ggen::Util

    def setup
      @rg2 = TemplateGame.get_game("1RG2")
      @rg4 = TemplateGame.get_game("4RG4")
    end

    must "game id has valid formate such as '1RG2'" do
      assert_nothing_raised do
        check_game_id('1RG2')
      end
      assert_raise RuntimeError do
        check_game_id('1RG2has')
      end
    end

    must "reference game id only has 1RG2, 1RG4" do
      assert_nothing_raised do
        check_rgame_id('1RG2')
        check_rgame_id('4RG4')
      end

      assert_raise RuntimeError do
        check_rgame_id('2RG4')
      end
    end

    must "get correct config scripts names from template game" do
      assert_equal @rg2.config_scripts,
                  ["G001RG2.binreg", "Dev00.registry",
                   "100L1RG2.themereg", "100L1RG2-000.config",
                   "100L1RG2-00-000.config",
                   "100L1RG2-01-000.config",
                   "100L1RG2-02-000.config",
                   "100L1RG2-03-000.config",
                   "RuleBasedGameBetConfig.xml",
                   "RuleBasedGameBetLoaderConfig.xml",
                   "GameVariables.lua",
                  ]

    end

    must "RG4 has no need to modify config scripts" do
       assert_equal false, @rg4.respond_to?(:config_scripts)
       assert_equal true, @rg2.respond_to?(:config_scripts)
    end
  end
end
