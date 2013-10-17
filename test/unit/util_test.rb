require 'test_helper'

module Ggen
  class TestUtil < Test::Unit::TestCase
    include Ggen::Util

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
        check_rgame_id('1RG4')
      end

      assert_raise RuntimeError do
        check_rgame_id('2RG4')
      end
    end

  end
end
