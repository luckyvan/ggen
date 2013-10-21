require 'pathname'

module Ggen
  # This class encapsulates all of the configuration options that Ggen
  # understands. Please see the {file:REFERENCE.md#options Ggen Reference} to
  # learn how to set the options.
  class Options
    @defaults = {
      :resource_paths        => nil,
      :reference_root       => Pathname.new('/mnt'),
      :template             => Pathname.new(File.dirname(File.realdirpath(__FILE__))) + 'template',
      :reference_game_id    => '1RG2',
      :game_id              => nil,
      :base_symbols         => nil,
      :bonus_symbols        => nil,
      :output_root               => Pathname.new('./Generate'),
      :game_output_root          => nil,                      #output_root directory of generated game resource
      :proj_output_root          => nil,                      #output_root directory of generated game project

      :paytable_scanner     => nil,
      :base_symbols         => nil,                      #symbols of base game
      :bonus_symbol         => "B1",
      :wild                 => "WW",
      :scatter              => "B1",
      :bonus_symbols        => nil,                      #symbols of bonus game
      :resources            => nil,                      #Array of directories in which *tga, *movies are contained.
      :stages               => nil,                      #Object handle which contains all stage related information, normally retrieved from a PaytableScanner
      :paytable             => nil,                      #Paytable
      :paytable_config      => nil,                      #Configure file for specific paytable
    }

    # The default option values.
    # @return Hash
    def self.defaults
      @defaults
    end

    attr_accessor :paytable_scanner
    attr_accessor :template
    attr_accessor :base_symbols
    attr_accessor :bonus_symbol
    attr_accessor :wild
    attr_accessor :scatter
    attr_accessor :bonus_symbols
    attr_accessor :resource_paths
    attr_accessor :reference_root
    attr_accessor :output_root
    attr_accessor :reference_game_id
    attr_accessor :game_id
    attr_accessor :game_output_root
    attr_accessor :proj_output_root
    attr_accessor :base_symbols
    attr_accessor :bonus_symbols
    attr_accessor :resources
    attr_accessor :stages
    attr_accessor :paytable
    attr_accessor :paytable_config

    def initialize(values = {}, &block)
      defaults.each {|k, v| instance_variable_set :"@#{k}", v}
      values.each {|k, v| send("#{k}=", v) if defaults.has_key?(k) && !v.nil?}
      yield if block_given?
    end

    # Retrieve an option value.
    # @param key The value to retrieve.
    def [](key)
      send key
    end

    # Set an option value.
    # @param key The key to set.
    # @param value The value to set for the key.
    def []=(key, value)
      send "#{key}=", value
    end

    private

    def defaults
      self.class.defaults
    end
  end
end
