require 'ggen/paytable_scanner'

module Ggen
  # This is the frontend for using Ggen programmatically.
  # It can be directly used by the user by creating a
  # new instance and calling \{#render} to render the template.
  # For example:
  #
  #     input = File.read('input_file')
  #     ggen_engine = Ggen::Engine.new(input)
  #     ggen_engine.generate
  #     puts output
  class Engine
  end
end

