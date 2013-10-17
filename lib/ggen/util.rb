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

    def symbol_scripts(gid)
      {'1RG2' => ["SymbolVariables.lua", "CustomSymbolFunctions.lua", "BaseGameSymbolImageTranslations.lua", "FreeSpinSymbolImageTranslations.lua", "DynamicActorTextureList.lua", "SymbolInfoValuesTable.lua", "SymbolInforImageTranslations.lua", "SymbolInfoTableTranslations.lua"]}[gid]
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
