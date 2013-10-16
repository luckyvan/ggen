require 'pathname'
require 'rubygems'
require 'active_support/inflector'

module Ggen
  class PaytableScanner
    attr_accessor :elements, :stages
    attr_accessor :rmlp, :base, :doubleup, :bonus

    def initialize
      @elements = {}
      @stages   = []
      @rmlp = nil
      @base = nil
    end

    def insert(h, k, v)
      if(h[k])
        h[k] = ([h[k]] << v).flatten
      else
        h[k] = v
      end
    end

    # paytable -> elems
    # elems -> elem elems | E
    def parse(tokens)
      while (!tokens.empty?) do
        tokens = element( tokens, @elements )
      end
    end

    private

    # elem -> '[' id ']' '{' content '}'
    def element( tokens, parent)
      tokens =  match('[', tokens)

      id = tokens[0]
      tokens =  match(id, tokens)
      tokens =  match(']', tokens)
      tokens =  match('{', tokens)

      hash, tokens = content(tokens)

      tokens = match('}', tokens)

      # translation
      extend_hash(hash, id)
      insert(parent, id, hash)

      return tokens
    end


    # content -> element | equation | win category
    def content(tokens)
      result = {}
      while('}' != tokens[0]) do
        if('[' == tokens[0]) then
          tokens = element( tokens, result)
        elsif ('=' == tokens[0])
          tokens = equation(tokens, result)
        elsif ('{' == tokens[0] )
          tokens = win_category( tokens, result )
        end
      end
      return result, tokens
    end


    # equation -> '=' lop rop // modified by tokenizer from midfix to prefix expression
    def equation( tokens, parent )
      name = tokens[1]
      content = tokens[2]
      tokens =  match('=', tokens)
      tokens =  match(tokens[0], tokens)
      tokens =  match(tokens[0], tokens)

      insert(parent, name, content)
      return tokens
    end


    # win_category -> '{' win other_equations '}'
    # win -> equation #{ with lop == 'win' }
    # other_equations -> equation other_equations | E
    def win_category( tokens, parent )
      tokens =  match('{', tokens)

      equations = {}
      while('}' != tokens[0]) do
        tokens = equation( tokens, equations )
      end
      tokens =  match('}', tokens)

      insert(parent, "wins", equations)
      return tokens
    end

# helper functions
    def match( token, tokens )
      raise "\'#{tokens[0]}\' does not match \'#{token}\'" unless (token == tokens[0])

      return tokens[1..-1]
    end

    # add accessor to element to locate its sub element
    def extend_hash(hash, id)
      # basic extension by name
      hash.each_pair do |key, value|
        method_name = key.split(/\s+/).join('_')
        method_name = method_name.pluralize if value.respond_to?(:[])
        hash.define_singleton_method(method_name.intern) do
          value
        end
      end

      # stage specific extension
      case id
      when /base game/ then
        hash.extend(Stage).extend(SlotGame).extend(Base)
        hash.index = 0
        @base = hash
        stages << hash
      when /Stage(\d)/ then
        index = $1
        hash.extend(Stage)
        if (hash.has_key?("slot")) then
          hash.extend(SlotGame)
          @bonus = hash
        else
          @doubleup = hash
        end
        hash.index = index
        @stages << hash
      when /RMLPFlash/ then
        hash.extend(Stage)
        @rmlp = hash; stages << @rmlp
        @rmlp.index = stages.length - 1
      end
      hash
    end
  end

  class PaytableTokenizer
    @delims = ['[', ']', '{', '}', '=']
    attr_accessor :tokens

    def initialize()
      @tokens = []
    end

    def parse (input)
      input.each_line do |line|
        #erase comment and spaces both front and back ends.
        line = line.gsub(/\/\/.*/, '').gsub(/^\s*/,'').gsub(/\s*$/,'')

        if('[' == line[0]) then
          raise "line #{line} does not end with ']' when begins with '['" unless line[-1] == ']'
          @tokens << '[' << line[1..-2] << ']'
        elsif('{' == line[0] or '}' == line[0]) then
          @tokens << line[0]
        elsif(line =~ /\s*=\s*/)
          @tokens << '='
          @tokens << line.gsub(/\s*=.*/, '')
          @tokens << line.gsub(/.*=\s*/, '')
        end
      end
    end
  end

  module Stage
    attr_accessor :index
  end

  module SlotGame
    def paylines
      paylines = pay_lines.values.flatten if (respond_to?(:pay_lines) and (not pay_lines.empty?))
    end

    def wins
      win_evaluations.wins
    end

    def symbols
      symbol_defines.values.flatten
    end

    def visible_symbols
      visible_symbols = reels[0].number_of_visible_symbols
    end
  end

  module Base
    def trigger_index
      wins.index do |w|
        w.values[0].include?("trigger")
      end
    end

    def bonus_symbol
      scatters = wins[trigger_index].values[1].split(/\s*,\s*/)
      substitutions = symbol_substitutions

      sym = nil
      scatters.each do |s|
        if ((s != "XX") and (substitutions.keys.include?(s))) then
          sym = substitutions[s].split(/\s*,\s*/)[0]
        else
          sym = s if symbols.include? s
        end
      end
      sym
    end
  end
end
