require 'test_helper'

module Ggen
  class TestUtil < Test::Unit::TestCase
    include Ggen::Helper

    must "base stage has correct configurations" do
      p  StageConfig.stage(:base_game){ type }
    end

  end
end
