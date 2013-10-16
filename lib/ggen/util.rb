# encoding: utf-8
require 'pathname'

module Ggen
  # A module containing various useful functions.
  module Util
    extend self

    def valid_game_id?(gid)
      /\A\d\w\w\d\Z/.match(gid) != nil
    end

    def valid_rgame_id?(rgid)
      [/1RG2/, /1RG4/].each do |r|
        return true if r.match(rgid)
      end
      false
    end

    class RootPathname < Pathname
      def initialize(p, id)
        super(p)
        @id = id
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

      def game_output
        games + "/Game-00#{@id}"
      end


      def proj_output
        projects + "/Game-00#{@id}"
      end
    end
  end
end
