require 'ggen/paytable_scanner'
require 'ggen/options'
require 'fileutils'

module Ggen
  # This is the frontend for using Ggen programmatically.
  # It can be directly used by the user by creating a
  # new instance with proper options and perform the following task
  # for example:
  #
  #     engine = Ggen::Engine.new(options)
  #     1. engine.new_game
  #     2. engine.parse_paytable
  #     3. engine.merge_resource
  #     4. engine.generate_symbol_scripts
  #     5. engine.generate_config_scripts
  #     6. engine.generate_#{feature}
  class Engine
    include Ggen::Util
    # The Ggen::Options instance.
    # See {file:REFERENCE.md#options the Ggen options documentation}.
    #
    # @return Ggen::Options
    attr_accessor :options

    def initialize(options={})
      @options = Options.new(options)
    end

    def new_game
      raise "Invalid Game ID:#{options[:game_id]}" unless valid_game_id?(options[:game_id])
      raise "Invalid Reference Game ID:#{options[:reference_game_id]}" unless valid_rgame_id?(options[:reference_game_id])

      p "haha"
      root = RootPathname.new(@options[:root], @options[:game_id])
      p root.projects
      output = RootPathname.new(@options[:output])
    end
  end
end

