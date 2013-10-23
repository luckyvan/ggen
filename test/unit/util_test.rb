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

    must "get correct dst directory for certain template file" do
      output_dir = "/test"
      template_dir = "/template"
      file_path = "/template/a/b/c/d.template"

      assert_equal Pathname.new("/test/a/b/c"), get_dst_dir(file_path,template_dir,output_dir)
    end
  end
end
