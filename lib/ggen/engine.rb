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

    end

    def new_game
      reference_game = options.reference_game
      output_game = options.output_game
      template_game = options.template_game

      sh_template = options.template_root + 'new_game.sh.template'

      script = "ng.sh"
      begin
        print "Creating #{script}\n"
        generate_by_template(sh_template, script, binding())

        system("sh -x ./#{script}")

      ensure
        FileUtils.rm script
      end

      #libShared
      names = proj_specific_configurations(options.reference_game_id)
      generate_by_template_file_names(names, options.template_game.proj_path,
                                     options.output_game.proj_path, binding())
      puts "Done creation of new game: #{options.output_game.game_path}"
    end

    def merge
      puts "merge resources"
      output_game = options.output_game
      check_dir(output_game.game_path)

      #delete existing Symbol tgas and movies
      puts "remove original symbol resources"
      symbol_resouces = resources(output_game).select {|f| f =~ /Symbols/}
      symbol_resouces.each do |f|
        FileUtils.rm(f, :verbose => true)
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
            FileUtils.cp src, dst, :verbose => true
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


      symbol_scripts = symbol_scripts(options.reference_game_id)
      generate_by_template_file_names(symbol_scripts, options.template_game.game_path,
                                      options.output_game.game_path, binding())
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

      options.base_symbols = scanner.base.symbols if scanner.respond_to?(:base)
      options.bonus_symbols = scanner.bonus.symbols if scanner.respond_to?(:bonus)
      options.bonus_symbol = scanner.base.bonus_symbol if scanner.base.respond_to?(:bonus_symbol)
      options.paytable_scanner = scanner
      options.wild = scanner.base.symbols[0]
    end

    def generate_stages
      puts "generate stages"
      check_game_id(options[:game_id])
      game = GamePath.new(@options.output_root,
                               @options.game_id)
      game_path = game.game_path
      proj_path = game.proj_path

      reference_game = GamePath.new(options.template_root, options.reference_game_id)
      check_dir(game_path)


      # rm game original config files
      ["Registries", "Themes", "Bins"].each do |dir|
        output_dir = game_path + "Resources/Generic/Base/Configuration" + dir
        FileUtils.rm_rf output_dir
      end

      # generate config files
      rgid    = options.reference_game_id
      gid     = options.game_id
      scanner = options.paytable_scanner
      stages = scanner.stages
      stage_count = stages.length
      rmlp = options.paytable_scanner.respond_to?(:rmlp)
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
      config_scripts_basenames = config_scripts_basenames(options.reference_game_id)
      generate_by_template_file_names(config_scripts_basenames, options.template_game.game_path,
                                      options.output_game.game_path, binding())
      # config_scripts_root = options.template_root + "Games/Game-00#{options.reference_game_id}"
      # config_scripts_templates = templates(config_scripts_root).select do |t|
        # config_scripts_basenames.include?(t.basename.sub(".template", "").to_s)
      # end

      # config_scripts_templates.each do |t|
        # dst = game_path + t.relative_path_from(config_scripts_root)
        # FileUtils.mkdir_p dst.dirname

        # dst = dst.to_s.sub(".template", "").gsub(rgid, gid).gsub("100L", "#{payline_num}L")

        # erb = ERB.new(File.open(t).read).result(binding)
        # File.open(dst, "w").write(erb)
      # end

      #payline file name modification
      payline_num_r = reference_game_payline_num(rgid)
      Dir.glob(File.join(game_path, "**", "#{payline_num_r}L*")).each do |src|
        dst = src.gsub("#{payline_num_r}", "#{payline_num}")

        FileUtils.mv src, dst if (src != dst)
      end

      #game id file name modification
      Dir.glob(File.join(game_path, "**", "*#{rgid}*")).each do |src|
        p src
        dst = src.gsub("#{rgid}", "#{gid}")
        p dst

        FileUtils.mv src, dst if (src != dst)
      end
      #paytable
      paytable_dir = game_path + "Resources/Generic/Base/Configuration/Paytables"
      FileUtils.cp options.paytable, paytable_dir
      FileUtils.cp options.paytable_config, paytable_dir

      #rmlp
      if rmlp then
        ebgreg_src = Pathname.new(Dir.glob(File.join(options.template_game.game_path, "**", "*.ebgreg"))[0])
        ebgreg_dst = game_path + ebgreg_src.relative_path_from(options.template_game.game_path)
        ["Resources/Generic/Base/Game.RMLPFlash",
         "../../projects/Game-00#{rgid}/RMLPFlashFlow",
         "../../projects/Game-00#{rgid}/RMLPFlashPresentation"].each do |path|
           FileUtils.cp_r options.template_game.game_path + path, game_path + path.sub(rgid, gid)
         end

        FileUtils.cp ebgreg_src, ebgreg_dst
      end

    end
  end
end

