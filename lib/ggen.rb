# require 'ggen/version'

# The module that contains everything Ggen-related:
#
# * {Ggen::Parser} is Ggen's parser.
# * {Ggen::Compiler} is Ggen's compiler.
# * {Ggen::Engine} is the class used to render Ggen within Ruby code.
# * {Ggen::Options} is where Ggen's runtime options are defined.
# * {Ggen::Error} is raised when Ggen encounters an error.
#
# Also see the {file:REFERENCE.md full Ggen reference}.

require 'ggen/engine'
require 'ggen/version'
