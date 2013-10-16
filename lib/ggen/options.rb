module Ggen
  # This class encapsulates all of the configuration options that Ggen
  # understands. Please see the {file:REFERENCE.md#options Ggen Reference} to
  # learn how to set the options.
  class Options

    @defaults = {
      :reference_game_id    => '1RG2'
      :game_id              => nil,
      :game_output          => nil,                      #output directory of generated game resource
      :proj_output          => nil,                      #output directory of generated game project
      :base_symbols         => nil,                      #symbols of base game
      :bonus_symbols        => nil,                      #symbols of bonus game
      :resources            => nil,                      #Array of directories in which *tga, *movies are contained.
      :stages               => nil,                      #Object handle which contains all stage related information, normally retrieved from a PaytableScanner
      :paytable             => nil,                      #Paytable
      :paytable_config      => nil,                      #Configure file for specific paytable

      :new_game             => false,
      :merge_resource       => false,
      :symbol_scripts       => false,
      :configuration_files  => false,
      :rmlp                 => false,
    }

    # The default option values.
    # @return Hash
    def self.defaults
      @defaults
    end

    # The flag to determine whether or not  generate new_game based on
    #    :reference_game_id
    #    :game_id
    attr_accessor:new_game

    # The flag to determine whether merge resources in the process, depending on
    #     :resources
    #     :game_output
    attr_accessor :merge_resource

    # The flag to determine whether or not generate symbol reference scripts only, depending on
    #     :game_output       #Unlike following :configuration_files, symbol scripts generation need further
    #                        #validation for the resources files.
    #     :base_symbols
    #     :bonus_symbols
    attr_accessor :symbol_scripts


    # The flag to determine whether or not generate Configuration scripts only, depending on
    #     :game_output
    #     :stages
    #     :rmlp
    attr_accessor :configuration_files


    # The flag to determine whether or not generate RMLP related C++ and scripts only, depending on
    #     :game_output
    #     :proj_output
    attr_accessor :rmlp


    attr_accessor :reference_game_id
    attr_accessor :game_id
    attr_accessor :game_output
    attr_accessor :proj_output
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

    undef :cdata
    def cdata
      xhtml? || @cdata
    end

    private

    def defaults
      self.class.defaults
    end
  end
end
