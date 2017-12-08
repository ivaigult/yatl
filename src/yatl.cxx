/* Copyright (c) 2017 Ivan Vaigult
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "machine.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>


const char repl_soruce[] = {
    #include <repl.inl>
};

namespace yatl {
namespace language_core {
void init_language_core(machine& m);
}
}

int bootstrap(const char* src, size_t size) {
    yatl::machine m;
    yatl::language_core::init_language_core(m);
    
    yatl::tokenizer tokenizer;
    yatl::parser parser(m);

    yatl::tokenizer::token_stream_t tokens;
    
    for(size_t ii = 0; ii < size; ++ii) {
	tokenizer.add_char(tokens, src[ii]);
	yatl::parser::object_stream_t objects = parser.parse(tokens);
	tokens.clear();
	for (yatl::lisp_abi::object* o : objects) {
	    m.eval(o);
	}
    }
    return 0;
}

int main(int argc, char** argv)
{
    return bootstrap(repl_soruce, sizeof(repl_soruce));
}
