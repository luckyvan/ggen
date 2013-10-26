require 'test_helper'

module Ggen
  class TestPaytableScanner < Test::Unit::TestCase
    def setup
      @tokenizer = PaytableTokenizer.new()
      @tokenizer.parse(File.open("test/unit/paytable/AVV040998.paytable").read)
      @scanner = PaytableScanner.new()
      @scanner.parse(@tokenizer.tokens)

      # 5RJ8  Diamond Reflections
      @tokenizer1 = PaytableTokenizer.new()
      @tokenizer1.parse(File.open("test/unit/paytable/AVV040999.paytable").read)
      @scanner1 = PaytableScanner.new()
      @scanner1.parse(@tokenizer1.tokens)

      # 5RJ7  Disco Nights
      @tokenizer2 = PaytableTokenizer.new()
      @tokenizer2.parse(File.open("test/unit/paytable/AVV040974.paytable").read)
      @scanner2 = PaytableScanner.new()
      @scanner2.parse(@tokenizer2.tokens)


      # 5PD7 Dogs
      @tokenizer3 = PaytableTokenizer.new()
      @tokenizer3.parse(File.open("test/unit/paytable/AVV040073.paytable").read)
      @scanner3 = PaytableScanner.new()
      @scanner3.parse(@tokenizer3.tokens)
    end

    must "top level elements" do
      assert_equal 5, @scanner.elements.length
    end

    must "has 2-4 stages" do
      assert_equal 2, @scanner.stages.length
      assert_equal 4, @scanner1.stages.length
      assert_equal 4, @scanner2.stages.length
      assert_equal 4, @scanner3.stages.length
      assert_equal [:base_game, :free_spin, :doubleup, :rmlp], @scanner3.stages.map {|s| s.name}
      assert_equal false, @scanner.has_stage?(:doubleup)
      assert_equal false, @scanner.has_stage?(:rmlp)
      assert_equal true, @scanner.has_stage?(:base_game)
      assert_equal true, @scanner.has_stage?(:free_spin)
    end

    must "include RMLP" do
      assert_nil @scanner.rmlp
      assert_not_nil @scanner1.rmlp
      assert_not_nil @scanner2.rmlp
      assert_not_nil @scanner3.rmlp
    end

    must "include doubleup based on paytable" do
      assert_not_nil @scanner3.doubleup
      assert_not_nil @scanner2.doubleup
      assert_not_nil @scanner1.doubleup
      assert_nil @scanner.doubleup
    end

    must "include bonus based on paytable" do
      assert_not_nil @scanner3.bonus
      assert_not_nil @scanner2.bonus
      assert_not_nil @scanner1.bonus
      assert_not_nil @scanner.bonus
    end

    must "include correct paylines " do
      assert_nil   @scanner1.base.paylines
      assert_equal 40, @scanner2.base.paylines.length
      assert_equal 40, @scanner2.bonus.paylines.length
      assert_equal 30, @scanner3.bonus.paylines.length
      assert_equal 30, @scanner3.base.paylines.length
    end

    must "has correct symbol sets" do
      assert_equal ["TT", "DD", "7A", "7B", "7C",
                    "7D", "7E", "F1", "F2", "F3", "F4", "BN"], @scanner.base.symbols
      assert_equal ["WW", "M1", "M2", "M3", "M4",
                    "F5", "F6", "F7", "F8", "D1","B1"], @scanner1.base.symbols
      assert_equal ["WW", "M1", "M2", "M3", "M4",
                    "F5", "F6", "F7", "F8", "F9","B1"], @scanner2.base.symbols
      assert_equal ["WW", "M1", "M2", "M3", "M4",
                    "F5", "F6", "F7", "F8", "F9"], @scanner2.bonus.symbols
      assert_equal ["LG", "H1", "H2", "H3", "H4", "H5",
                    "D1", "D2", "D3", "D4", "D5", "L1", "L2", "L3", "L4", "SC", "SD"], @scanner3.base.symbols
      assert_equal ["LG", "H1", "H2", "H3", "H4", "H5",
                    "D1", "D2", "D3", "D4", "D5", "L1", "L2", "L3", "L4"], @scanner3.bonus.symbols
    end

    must "has a win category which can lead to Stage1" do
      assert_equal 150, @scanner.base.trigger_index
      assert_equal 80, @scanner1.base.trigger_index
      assert_equal 27, @scanner2.base.trigger_index
      assert_equal 101, @scanner3.base.trigger_index
    end

    must "have valid bonus symbol" do
      assert_equal "BN", @scanner.base.bonus_symbol
      assert_equal "B1", @scanner1.base.bonus_symbol
      assert_equal "B1", @scanner2.base.bonus_symbol
      assert_equal "SD", @scanner3.base.bonus_symbol
    end

    must "have valid reels visible symbol" do
      assert_equal "3", @scanner.base.visible_symbols
      assert_equal "4", @scanner2.base.visible_symbols
    end
  end
end
