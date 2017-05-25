# Copyright (c) 2017 Ivan Vaigult
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

project(yatl)

cmake_minimum_required(VERSION 3.3)

set (CMAKE_CXX_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(yatl
  src/yatl.cxx
  src/apply_workaround.hpp
  src/error.hpp
  src/error.cxx
  src/function_helpers.hpp
  src/language_core.cxx
  src/lambda.cxx
  src/lambda.hpp
  src/lisp_abi.hpp
  src/list_view.hpp
  src/machine.cxx
  src/machine.hpp
  src/object.hpp
  src/object_cast.hpp
  src/object_io.hpp
  src/object_io.cxx
  src/parser.cxx
  src/parser.hpp
  src/repl.cxx
  src/repl.hpp
  src/signature_validation.hpp
  src/sym_binding.cxx
  src/sym_binding.hpp
  src/tokenizer.cxx
  src/tokenizer.hpp
)

add_subdirectory(tests)