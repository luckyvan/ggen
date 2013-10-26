require 'ggen/paytable_scanner'
require 'ggen/options'
require 'fileutils'
require 'erb'

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
  #     5. engine.generate_stages
  class Engine
    include Ggen::Util
    # The Ggen::Options instance.
    # See {file:REFERENCE.md#options the Ggen options documentation}.
    #
    # @return Ggen::Options
    attr_accessor :options

    def initialize(options={})
      @options = Options.new(options)

      check_game_id(@options.game_id)
      check_rgame_id(@options.reference_game_id)

      # game path for reference game, output game, and template game
      check_dir(@options.reference_root)
      @options.reference_game = GamePath.new(@options.reference_root, @options.reference_game_id)
      check_dir(@options.output_root)
      @options.output_game = GamePath.new(@options.output_root, @options.game_id)
      @options.template_game = GamePath.new(@options.template_root, @options.reference_game_id)
      check_dir(@options.template_game.game_path)
      check_dir(@options.template_game.proj_path)

      @tg = TemplateGame.get_game(@options.reference_game_id) #template game
    end

    def new_game
      verbose = options.verbose
      reference_game = options.reference_game
      output_game = options.output_game
      gid = options.game_id
      rgid = options.reference_game_id

      FileUtils.rm_rf output_game.game_path, :verbose => verbose
      FileUtils.rm_rf output_game.proj_path, :verbose => verbose
      FileUtils.mkdir_p output_game.games
      FileUtils.mkdir_p output_game.projects

<<<<<<< HEAD
      script = "ng.sh"
      begin
        print "Creating #{script}\n"
        generate_by_template(sh_template, script, binding())
      ensure
        FileUtils.rm script
      end
=======
      FileUtils.cp_r reference_game.game_path, output_game.game_path, :verbose => verbose
      FileUtils.cp_r reference_game.proj_path, output_game.proj_path, :verbose => verbose

      modify_file_names(output_game.configuration, "#{rgid}", "#{gid}", verbose)
      modify_file_names(output_game.proj_path, "#{rgid}", "#{gid}", verbose)

      modify_file_contents(output_game.configuration, "#{rgid}", "#{gid}")
      modify_file_contents(output_game.proj_path, "#{rgid}", "#{gid}")
>>>>>>> usb/develop

      #libShared
      names = @tg.proj_configs
      generate_by_names(names, binding())
      puts "Done creation of new game: #{options.output_game.game_path}"
    end

    def merge
      puts "merge resources"
      verbose = options.verbose
      output_game = options.output_game
      check_dir(output_game.game_path)

      #delete existing Symbol tgas and movies
      puts "remove original symbol resources"
      symbol_resouces = resources(output_game).select {|f| f =~ /Symbols/}
      symbol_resouces.each do |f|
        FileUtils.rm(f, :verbose => verbose)
      end

      exclusion = ["Messages.movie", "TransitionMessages.movie", "OverReelsMessages.movie", "MathBoxMessages.movie", "RetriggerMessages.movie"]

      # merge resources
      resource_paths = options.resource_paths
      resource_paths.each do |root|
        root_pn = Pathname.new(root)
        resources(root).each do |resource|
          pn = Pathname.new(resource)
          unless exclusion.include?(pn.basename.to_s) then
            src = pn
            dst = output_game.game_path + pn.relative_path_from(root_pn)
            FileUtils.mkdir_p dst.dirname.to_s unless dst.dirname.directory?
            FileUtils.cp src, dst, :verbose => verbose
          end
        end
      end
      puts "Done Merge Resources"
    end

    def generate_symbol_scripts
      puts "generate symbol scripts"
      base_path = options.output_game.game_base

      check_nil(:base_symbols, options)
      check_nil(:bonus_symbols, options) unless options.reference_game_id == '1RG4'

      base_resources, bonus_resources = nil, nil
      # collect resources
      if options.base_symbols
        base_resources = find_resources_by_symbols(base_path+"Game.Main", options.base_symbols)
      end
      if options.bonus_symbols
        bonus_resources = find_resources_by_symbols(base_path+"Game.FreeSpinBonus", options.bonus_symbols)
      end

<<<<<<< HEAD

      symbol_scripts = symbol_scripts(options.reference_game_id)
      generate_by_template_file_names(symbol_scripts, options.template_game.game_path,
                                      options.output_game.game_path, binding())
=======
      generate_by_names(@tg.symbol_scripts, binding)
>>>>>>> usb/develop
    end

    def parse_paytable
      puts "parse paytable"
      check_nil(:paytable, options)
      paytable_path = Pathname.new(options.paytable)
      check_file(paytable_path)


      tokenizer = PaytableTokenizer.new()
      tokenizer.parse(File.open(paytable_path).read)
      scanner = PaytableScanner.new()
      scanner.parse(tokenizer.tokens)

      options.base_symbols = scanner.base.symbols if scanner.base
      options.bonus_symbols = scanner.bonus.symbols if scanner.bonus
      options.bonus_symbol = scanner.base.bonus_symbol if scanner.base.bonus_symbol
      options.paytable_scanner = scanner
      options.wild = scanner.base.symbols[0]
    end

    def generate_stages
      puts "generate stages"
      output_game = @options.output_game
      game_path = @options.output_game.game_path
      proj_path = @options.output_game.proj_path
      verbose = options.verbose

      rmlp = options.paytable_scanner.respond_to?(:rmlp)

      # rm game original config files
      if @tg.respond_to?(:config_scripts)
        [output_game.themes, output_game.bins, output_game.registries ].each do |dir|
          FileUtils.rm_rf dir
          FileUtils.mkdir_p dir
        end

        # generate config files
        rgid    = options.reference_game_id
        gid     = options.game_id
        scanner = options.paytable_scanner
        stages = scanner.stages
        stage_count = stages.length
        paylines = scanner.base.paylines
        bonus_trigger = scanner.base.trigger_index
        visible_symbols = scanner.base.visible_symbols
        payline_num = (paylines)? paylines.length : 100
        paytable = Pathname.new(options.paytable).basename
        paytable_config = Pathname.new(options.paytable_config).basename
        themereg = "#{payline_num}L#{gid}.themereg"
        theme_config = "#{payline_num}L#{gid}-000.config"
        binreg = "G00#{gid}.binreg"

        # get config files templates
        # config_scripts_basenames = config_scripts_basenames(options.reference_game_id)
        generate_by_names(@tg.config_scripts, binding)

        #payline file name modification
        payline_num_r = @tg.payline_num(rgid)
        modify_file_names(game_path, "#{payline_num_r}L", "#{payline_num}L", verbose)
        #game id file name modification
        modify_file_names(game_path, "#{rgid}", "#{gid}", verbose)
      end

      #paytable
      paytable_dir = @options.output_game.paytables
      FileUtils.cp options.paytable, paytable_dir
      FileUtils.cp options.paytable_config, paytable_dir

      #rmlp
      if rmlp then
        names = ["EBG.ebgreg", "Game.RMLPFlash", "RMLPFlashFlow", "RMLPFlashPresentation",
         "LibPresentation", "LibSlotPresentation", "LibSys", "WinCycleLite"]
        generate_by_names(@tg.rmlp)
      end
    end
  end
end

