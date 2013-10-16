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
        opts.on('-s', '--stdin', :NONE, 'Read input from standard input instead of an input file') do
          @options[:input] = $stdin
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

      # Processes the options set by the command-line arguments.
      # In particular, sets `@options[:input]` to get input parameters
      #
      # This is meant to be overridden by subclasses
      # so they can run their respective programs.
      def process_result
        input = @options[:input]
        args = @args.dup
        input ||=
          begin
            filename = args.shift
            p "filename"
            @options[:filename] = filename
            open_file(filename) || $stdin
          end
        @options[:input] = input
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

        opts.on('-m', '--merge-resource', "Merge resources") do
          @options[:merge_resource] = true
        end

        opts.on('-y', '--symbol-scripts', "Generate Symbol related lua scripts") do
          @options[:symbol_scripts] = true
        end

        opts.on('-c', '--configuation-scripts', "Generate Configuration Files based on Stage Information") do
          @options[:configuration_files] = true
        end

        opts.on('-r', '--rmlp', "Add rmlp feature to the game") do
          @options[:rmlp] = true
        end

        opts.on('-A', '--All', "Perform all generation tasks sequentially") do
          @options[:new_game] = true
          @options[:merge_resource] = true
          @options[:symbol_scripts] = true
          @options[:configuration_files] = true
          @options[:rmlp] = true
        end
      end

      # Processes the options set by the command-line arguments,
      # and runs the Ggen compiler appropriately.
      def process_result
        super
        @options[:for_engine][:filename] = @options[:filename]
        input = @options[:input]

        parameters = input.read()
        input.close() if input.is_a? File

        @options[:requires].each {|f| require f}

        begin
          engine = ::Ggen::Engine.new(parameters, @options[:for_engine])
          if @options[:check_syntax]
            puts "Syntax OK"
            return
          end

          if @options[:parse]
            pp engine.parser.root
            return
          end

          result = engine.to_html
        rescue Exception => e
          raise e if @options[:trace]

          case e
          when ::Ggen::SyntaxError; raise "Syntax error on line #{get_line e}: #{e.message}"
          when ::Ggen::Error;       raise "Ggen error on line #{get_line e}: #{e.message}"
          else raise "Exception on line #{get_line e}: #{e.message}"
          end
        end
      end
    end
  end
end
