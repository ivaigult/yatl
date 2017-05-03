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

#include <cassert>
#include <cstdlib>

namespace yatl {

parser::object_stream_t& parser::parse(const tokenizer::token_stream_t& tokens)
{
    _object_stream.clear();
    for(const tokenizer::token& token: tokens) {
        if (tokenizer::token_type::left_bracket == token.type) {
            _list_stack.push(utility::list_view(_repl.m, nullptr));
        } else if (tokenizer::token_type::right_bracket == token.type) {
            if (_list_stack.empty()) {
                throw error::error().format("unexpected token \'", token.content, "\'");
            }
            _complete_list();
        } else if (tokenizer::token_type::apostrophe == token.type) {
            _list_stack.push(utility::list_view(_repl.m, nullptr));
            _list_stack.top().push_back(_repl.m.alloc<lisp_abi::symbol>("quote"));
            _quote_stack.push(_list_stack.top().front_pair());
        } else {
            lisp_abi::object* new_item = nullptr;
            new_item = _repl.m.alloc<lisp_abi::symbol>(token.content);
            
            _complete_object(new_item);
        }
    }
    return _object_stream;
}

void parser::_complete_object(lisp_abi::object* obj) {
    if (_list_stack.empty()) {
        _object_stream.push_back(obj);
        return;
    } else {
        _list_stack.top().push_back(obj);
    }

    if (!_quote_stack.empty() && _quote_stack.top() == _list_stack.top().front_pair()) {
        _quote_stack.pop();
        _complete_list();
    }
}

void parser::_complete_list() {
    lisp_abi::pair* obj = _list_stack.top().front_pair();
    _list_stack.pop();
    _complete_object(obj);
}

}
