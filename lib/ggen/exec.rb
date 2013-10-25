require 'optparse'
require 'rbconfig'
require 'pp'

module Ggen
  # This module handles the various Ggen executables (`ggen` and `ggen-convert`).
  module Exec
    # An abstract class that encapsulates the executable code for all three executables.
    class Generic
      # @param args [Array<String>] The command-line arguments
      def initialize(args)
        @args = args
        @options = {:for_engine => {}}
      end

      # Parses the command-line arguments and runs the executable.
      # Calls `Kernel#exit` at the end, so it never returns.
      #
      # @see #parse
      def parse!
        begin
          parse
        rescue Exception => e
          raise e if @options[:trace] || e.is_a?(SystemExit)

          $stderr.print "#{e.class}: " unless e.class == RuntimeError
          $stderr.puts "#{e.message}"
          $stderr.puts "  Use --trace for backtrace."
          exit 1
        end
        exit 0
      end

      # Parses the command-line arguments and runs the executable.
      # This does not handle exceptions or exit the program.
      #
      # @see #parse!
      def parse
        @opts = OptionParser.new(&method(:set_opts))
        @opts.parse!(@args)

        process_result

        @options
      end

      # @return [String] A description of the executable
      def to_s
        @opts.to_s
      end

      protected

      # Finds the line of the source template
      # on which an exception was raised.
      #
      # @param exception [Exception] The exception
      # @return [String] The line number
      def get_line(exception)
        # SyntaxErrors have weird line reporting
        # when there's trailing whitespace,
        # which there is for Ggen documents.
        return (exception.message.scan(/:(\d+)/).first || ["??"]).first if exception.is_a?(::SyntaxError)
        (exception.backtrace[0].scan(/:(\d+)/).first || ["??"]).first
      end

      # Tells optparse how to parse the arguments
      # available for all executables.
      #
      # This is meant to be overridden by subclasses
      # so they can add their own options.
      #
      # @param opts [OptionParser]
      def set_opts(opts)
        opts.on('--trace', :NONE, 'Show a full traceback on error') do
          @options[:trace] = true
        end
        opts.on_tail("-?", "-h", "--help", "Show this message") do
          puts opts
          exit
        end

        opts.on_tail("-v", "--version", "Print version") do
          puts("Ggen #{::Ggen::VERSION}")
          exit
        end
      end


      # Same as `Kernel.puts`, but doesn't print anything if the `--quiet` option is set.
      #
      # @param args [Array] Passed on to `Kernel.puts`
      def puts(*args)
        return if @options[:for_engine][:quiet]
        Kernel.puts(*args)
      end

      private

      def open_file(filename, flag = 'r')
        return if filename.nil?
        File.open(filename, flag)
      end

      def handle_load_error(err)
        dep = err.message[/^no such file to load -- (.*)/, 1]
        raise err if @options[:trace] || dep.nil? || dep.empty?
        $stderr.puts <<MESSAGE
Required dependency #{dep} not found!
    Run "gem install #{dep}" to get it.
  Use --trace for backtrace.
MESSAGE
        exit 1
      end
    end

    # The `ggen` executable.
    class Ggen < Generic
      # @param args [Array<String>] The command-line arguments
      def initialize(args)
        super
        @options[:for_engine] = {}
        @options[:requires] = []
      end

      # Tells optparse how to parse the arguments.
      #
      # @param opts [OptionParser]
      def set_opts(opts)
        super

        opts.banner = <<END
Usage: ggen [options] [INPUT] [OUTPUT]

Description:
  Converts Ggen files to HTML.

Options:
END
        opts.on('-n', '--new-game',  "Generate a new game based on reference game id and new game id") do
          @options[:new_game] = true
        end

        opts.on('--game-id ID', "Generated Game ID") do |id|
          @options[:for_engine][:game_id] = id
        end

        opts.on('--reference-game-id ID',  "Game Id for the reference game") do |id|
          @options[:for_engine][:reference_game_id] = id
        end

        opts.on('--output DIR',  "Output Root directory, default is './Generate'") do |dir|
          @options[:for_engine][:output_root] = dir
        end

        opts.on('-m', '--merge-resource', "Merge resources") do
          @options[:merge_resource] = true
        end

        opts.on('--resource-paths LIST', "Comma separated paths in which all resouce files are stored") do |paths|
          @options[:for_engine][:resource_paths] = paths.split(/,/)
        end

        opts.on('-s', '--symbol-scripts', "Generate Symbol related lua scripts") do
          @options[:symbol_scripts] = true
        end

        opts.on('--base-symbols LIST', "Comma separated symbol list of base game") do |symbols|
          @options[:for_engine][:base_symbols] = symbols.split(/,/)
        end

        opts.on('--bonus-symbols LIST', "Comma separated symbol list of base game") do |symbols|
          @options[:for_engine][:bonus_symbols] = symbols.split(/,/)
        end

        opts.on('--bonus-symbol SYMBOL', "Bonus symbol which can trigger into bonus stage") do |symbol|
          @options[:for_engine][:bonus_symbol] = symbol
        end

        opts.on('--wild-symbol SYMBOL', "Wild symbol") do |symbol|
          @options[:for_engine][:wild] = symbol
        end

        opts.on('--scatter-symbol SYMBOL', "scatter symbol") do |symbol|
          @options[:for_engine][:scatter] = symbol
        end

        opts.on('-g', '--add-stages', "Generate stage-specific configuration and source code") do
          @options[:add_stages] = true
          @options[:parse_paytable] = true
        end

        opts.on('-p', '--parse-paytable', "") do
          @options[:parse_paytable] = true
        end

        opts.on('--paytable PAYTABLE')  do |paytable|
          @options[:for_engine][:paytable] = paytable
        end

        opts.on('--paytable-config CONFIG')  do |config|
          @options[:for_engine][:paytable_config] = config
        end

        opts.on('-v', '--verbose', "Perform all files on verbose mode") do
          @options[:for_engine][:verbose] = true
        end

        opts.on('-A', '--All', "Perform all generation tasks sequentially") do
          @options[:new_game] = true
          @options[:parse_paytable] = true
          @options[:merge_resource] = true
          @options[:symbol_scripts] = true
          @options[:add_stages] = true
        end
      end

      # Processes the options set by the command-line arguments,
      # and runs the Ggen compiler appropriately.
      def process_result
        begin
          engine = ::Ggen::Engine.new(@options[:for_engine])

          if @options[:new_game]
            engine.new_game
          end

          if @options[:parse_paytable]
            engine.parse_paytable
          end

          if @options[:merge_resource]
            engine.merge
          end

          if @options[:symbol_scripts]
            engine.generate_symbol_scripts
          end

          if @options[:add_stages]
            engine.generate_stages
          end
        rescue Exception => e
          # raise e if @options[:trace]
          raise e

          # case e
          # # when ::Ggen::SyntaxError; raise "Syntax error on line #{get_line e}: #{e.message}"
          # # when ::Ggen::Error;       raise "Ggen error on line #{get_line e}: #{e.message}"
          # else raise "Exception on line #{get_line e}: #{e.message}"
          # end
        end
      end
    end
  end
end
