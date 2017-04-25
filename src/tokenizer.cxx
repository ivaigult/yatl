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

#include "tokenizer.hpp"
#include "error.hpp"

#include <cctype>
#include <cassert>

namespace yatl {

void tokenizer::_emit_token(token& token) {
    if (token.content.empty())
        return;
    _token_stream.push_back(token);
    token = {};
}

const tokenizer::token_stream_t& tokenizer::tokenize(std::string line) {
    _token_stream.clear();
    // @todo: handle multiline tokens
    // @todo: handle strings
    // @todo: handle comments
    tokenizer::token current_token;
    for (char sym : line) {
        if (sym == '(') {
            _emit_token(current_token);
            _token_stream.push_back({tokenizer::token_type::left_bracket, "(" });
            continue;
        } else if (sym == ')') {
            _emit_token(current_token);
            _token_stream.push_back({tokenizer::token_type::right_bracket, ")" });
            continue;
        } else if (sym == '\'') {
            _emit_token(current_token);
            _token_stream.push_back({ tokenizer::token_type::apostrophe, "\'" });
            continue;
        } else if (std::isspace(sym)) {
            _emit_token(current_token);
            continue;
        }

        if (tokenizer::token_type::unknown == current_token.type) {
            assert(current_token.content.empty());
            if (std::isdigit(sym) || sym == '-') {
                current_token.type = tokenizer::token_type::number;
                current_token.content += sym;
                continue;
            } else {
                current_token.type = tokenizer::token_type::symbols;
            }
        }

        if (tokenizer::token_type::number == current_token.type) {
            if (!((sym == '.' && std::string::npos ==  current_token.content.find('.'))
                  || std::isdigit(sym))) {
                throw error::error().format("Invalid token ", current_token.content);
            }
        }
        
        current_token.content += sym;
    }
    _emit_token(current_token);
    return _token_stream;
}

}
