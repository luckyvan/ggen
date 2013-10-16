require 'test_helper'

module Ggen
  class TestUtil < Test::Unit::TestCase
    include Ggen::Util

    must "game id has valid formate such as '1RG2'" do
      assert_equal true, valid_game_id?('1RG2')
      assert_equal false, valid_game_id?('1RG2has')
    end

    must "reference game id only has 1RG2, 1RG4" do
      assert_equal true, valid_rgame_id?('1RG2')
      assert_equal true, valid_rgame_id?('1RG4')
      assert_equal false, valid_rgame_id?('2RG4')
    end

  end
end
