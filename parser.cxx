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

#include "parser.hpp"

#include "repl.hpp"
#include "error.hpp"

namespace yatl {

parser::object_stream_t& parser::parse(const tokenizer::token_stream_t& tokens)
{
    _object_stream.clear();
    for(const tokenizer::token& token: tokens) {
        if (tokenizer::token_type::left_bracket == token.type) {
            _list_stack.push(utility::list_view(_repl.machine, nullptr));
        }
        else if (tokenizer::token_type::right_bracket == token.type) {
            if (_list_stack.empty()) {
                throw error::error("unexpected token \'", token.content, "\'");
            }
            lisp_abi::pair* obj = _list_stack.top().front_pair();
            _list_stack.pop();
            if (_list_stack.empty()) {
                _object_stream.push_back(obj);
            }
            else {
                _list_stack.top().push_back(obj);
            }
        }
        else if (tokenizer::token_type::apostrophe == token.type) {
            _list_stack.push(utility::list_view(_repl.machine, nullptr));
        }
        else {
            lisp_abi::object* new_item = nullptr;
            if (tokenizer::token_type::symbols == token.type)
                new_item = _repl.machine.alloc<lisp_abi::symbol>(token.content);

            if (_list_stack.empty()) {
                _object_stream.push_back(new_item);
            } else {
                _list_stack.top().push_back(new_item);
            }
        }
    }
    return _object_stream;
}

}
