# encoding: utf-8
require 'pathname'

module Ggen
  # A module containing various useful functions.
  module Util
    extend self

    def check_game_id(gid)
      raise "Invalid Game Id #{gid}" unless /\A\d\w\w\d\Z/.match(gid) != nil
    end

    def check_rgame_id(rgid)
      [/1RG2/, /1RG4/].each do |r|
        return if r.match(rgid)
      end
      raise "Invalid Reference Game Id: #{rgid}"
    end

    def check_dir(pn)
      raise "Invalid Path: #{pn}" unless pn.directory?
    end

    def check_file(pn)
      raise "Invalid Path: #{pn}" unless pn.file?
    end

    def check_nil(key, options)
      raise "No values for #{key}" unless options[key]
    end

    def resources(path)
      ["tga", "movie", "sound"].inject([]) do |r, s|
          pattern = File.join(path, "**", "*.#{s}")
          r << Dir.glob(pattern)
          r.flatten
      end
    end

    def templates(root)
      Dir.glob(File.join(root, "**", "*.template")).map {|d| Pathname.new(d)}
    end

    def symbol_scripts(rgid)
      {'1RG2' => ["SymbolVariables.lua", "CustomSymbolFunctions.lua", "BaseGameSymbolImageTranslations.lua", "FreeSpinSymbolImageTranslations.lua", "DynamicActorTextureList.lua", "SymbolInfoValuesTable.lua", "SymbolInfoImageTranslations.lua", "SymbolInfoTableTranslations.lua"]}[rgid]
    end

    def config_scripts_basenames(rgid)
      {
        '1RG2' => ["G001RG2.binreg", "Dev00.registry",
                   "100L1RG2.themereg", "100L1RG2-000.config",
                   "100L1RG2-00-000.config",
                   "100L1RG2-01-000.config",
                   "100L1RG2-02-000.config",
                   "100L1RG2-03-000.config",
                   "RuleBasedGameBetConfig.xml",
                   "RuleBasedGameBetLoaderConfig.xml",
                  ],
      }[rgid]
    end

    def find_resources_by_symbols(stage_path, symbols)
      hash = {:tga => "Images/Symbols/*.tga",
              :movie => "Images/Symbols/*.movie",
              :sound => "Sounds/Symbols/*.sound"}
      result = {}
      hash.each_pair do |k,suffix|
        result[k] = {}
        Dir.glob(stage_path + suffix).each do |res|
          pn = Pathname.new(res)
          symbol = pn.basename.to_s[0..1]
          if symbols.include?(symbol)
            (result[k][symbol] ||= []) << Pathname.new(res).basename
          else
            puts"Warning: Redundant Symbol Resources: #{res}"
          end
        end
      end
      result[:tga].each_pair do |s,r|
        raise "symbol #{s} has no image resources" unless r
      end
      result
    end

    class Workspace < Pathname
      def initialize(p)
        super(p)
      end

      def development
        self + 'Development'
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

      def proj_path
        projects + @id
      end
    end


    class TemplatePath < Pathname
      def initialize(pn)
        super(pn)
      end

      def to_output_path(output_root)
        Pathname.new(output_root) + basename().sub('.template','')
      end
    end
  end
end
