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
      [/1RG2/, /4RG4/].each do |r|
        return if r.match(rgid)
      end
      raise "Invalid Reference Game Id: #{rgid}"
    end

    # Return destination directory for following copy command
    # @param :path, source file path
    def get_dst_dir(file)
      src_base, dst_base = get_base_directories(file)
      file_pn = Pathname.new(file)
      (dst_base + file_pn.relative_path_from(src_base)).dirname
    end

    # Use ERB to generate output file
    def generate_by_template(template, binding)
      pn = Pathname.new(template)
      dst = get_dst_dir(pn) + pn.basename.sub(".template", "")

      erb = ERB.new(File.open(template).read)
      content = erb.result( binding )

      FileUtils.mkdir_p dst.dirname unless dst.dirname.exist?

      File.open(dst, "w") do |f|
        f.write(content)
      end
    end

    def modify_file_contents(base_directory, from_pattern, to_pattern)
       Dir.glob(File.join(base_directory, "**", "*")).each do |f|
        begin
          if File.file?(f)
            text = File.read(f)
            File.open(f, "w"){|file| file.puts text.gsub(from_pattern, to_pattern)}
          end
        rescue ArgumentError
        end
      end
   end

    def modify_file_names(base_directory, from_pattern, to_pattern, verbose = false)
      Dir.glob(File.join(base_directory, "**", "*#{from_pattern}*")).each do |src|
        dst = src.gsub("#{from_pattern}", "#{to_pattern}")
        if (src != dst and File.file?(src))
          dirname = Pathname.new(dst).dirname
          if not dirname.exist?
            FileUtils.mkdir_p dirname, :verbose => verbose
          end
          FileUtils.mv src, dst, :verbose => verbose
        end
      end
    end

    # @param :path, source file path should be under template game directory
    # @return src and dst base directories.
    def get_base_directories(path)
      game_path = @options.template_game.game_path
      proj_path = @options.template_game.proj_path

      case path.to_s
      when /#{game_path}/
        return game_path, (@options.output_game.game_path)
      when /#{proj_path}/
        return proj_path, (@options.output_game.proj_path)
      else
        raise "Invalid Template File path: #{path}"
      end
    end

    def generate_by_names(names, binding = nil)
      src_paths = [@options.template_game.game_path,
                   @options.template_game.proj_path]

      paths = names.inject([]) do |re, n|
        src_paths.inject(re) do |re, p|
          re << Dir.glob(File.join(p, "**", "#{n}"))
          re << Dir.glob(File.join(p, "**", "#{n}.template"))
        end
      end

      paths.flatten.each do |path|
        pn = Pathname.new(path)
        dst_dir = get_dst_dir(path)
        if path.end_with?(".template") then
          generate_by_template(path, binding)
        else
          FileUtils.cp_r pn, dst_dir
        end
      end
    end


    def check_dir(pn)
      raise "Invalid Path: #{pn}" unless Pathname.new(pn).directory?
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

    def reference_game_payline_num(rgid)
      {
        '1RG2' => "100"
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
            # puts"Warning: Redundant Symbol Resources: #{res}"
          end
        end
      end
      result[:tga].each_pair do |s,r|
        raise "symbol #{s} has no image resources" unless r
      end
      result
    end
  end
end
