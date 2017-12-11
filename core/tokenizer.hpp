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
#pragma once

#include <vector>
#include <string>

namespace yatl {
class tokenizer {
public:
    enum class token_type {
        unknown,
        symbols,
        string,
        apostrophe,
        left_bracket,
        right_bracket,
    };
    
    struct token {
        token_type  type;
        std::string content;
    };

    typedef std::vector<token> token_stream_t;

    void add_char(token_stream_t& tokens, char sym);
    
    const token_stream_t& tokenize(std::string line);
private:
    void _emit_token(token_stream_t& token_stream, token& token);
    token_stream_t _token_stream;
    tokenizer::token _current_token = {};
    bool _parsing_string = false;
    bool _parsing_comment = false;
    bool _escape = false;

};
}
