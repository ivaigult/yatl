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

#include <iostream>

#include "machine.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "error.hpp"

namespace yatl {

class repl {
public:
    repl(std::vector<std::reference_wrapper<std::istream> > streams, std::ostream& out, std::ostream& err) 
        : _input_streams(streams)
        , _out(out)
        , _err(err)
        , _tokenizer(*this)
        , _parser(*this) 
    {}

    int exec() {
        std::string line;
        int error_code = EXIT_SUCCESS;
        for (std::istream& in : _input_streams) {
            for (; std::getline(in, line); ) {
                try {
                    const tokenizer::token_stream_t& tokens = _tokenizer.tokenize(line);
                    parser::object_stream_t objects = _parser.parse(tokens);
                    for (lisp_abi::object* o : objects) {
                        auto result = m.eval(o);
                        _out << *result << std::endl;
                    }
                }
                catch (error::error& e) {
                    _err << e.what() << std::endl;
                    error_code = EXIT_FAILURE;
                }
            }
        }
        return error_code;
    }

    machine       m;
private:
    std::vector<std::reference_wrapper<std::istream> > _input_streams;
    std::ostream& _out;
    std::ostream& _err;
    tokenizer     _tokenizer;
    parser        _parser;
};

}
