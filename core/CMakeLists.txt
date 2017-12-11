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

include(scm2cxx)

add_scm_library(yatl_core STATIC
    apply_workaround.hpp
    error.cxx
    error.hpp
    function_helpers.hpp
    functor_traits.hpp
    internal_helpers.hpp
    lambda.cxx
    lambda.hpp
    language_core.cxx
    lisp_abi.hpp
    list_view.hpp
    machine.cxx
    machine.hpp
    object.hpp
    object_cast.hpp
    object_io.cxx
    object_io.hpp
    parser.cxx
    parser.hpp
    port.cxx
    port.hpp
    register_type.hpp
    signature_validation.hpp
    sym_binding.cxx
    sym_binding.hpp
    tokenizer.cxx
    tokenizer.hpp
    # scm sources
    scm/repl.scm
)

target_include_directories(yatl_core PUBLIC .)

