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


#include "repl.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>

enum class parse_result {
    ok,
    stop,
    error
};

int main(int argc, char** argv)
{
    size_t arg_counter = 1;
    std::istream* istream = &std::cin;
    std::ostream* ostream = &std::cout;
    std::ifstream script_file;
    std::string   expression;
    std::istringstream expression_stream;
    std::ofstream dev_null;

    auto print_help = [&argv]() -> parse_result {
        std::cerr << "Usage: " << argv[0] << " [script_path] [script_args]" << std::endl;
        std::cerr << "       " << argv[0] << " [-e|--expr] expression" << std::endl;
        return parse_result::error;
    };

    auto set_expression = [&]() {
        const char* expression_str = argv[++arg_counter];
        expression = expression_str? expression_str : "";
        expression_stream.str(expression);
        istream = &expression_stream;
        return parse_result::stop;
    };

    std::map<std::string, std::function<parse_result(void)> > arg_parser = {   
        {"-h",      print_help },
        { "--help", print_help },
        {"-e",      set_expression },
        { "--expr", set_expression },
    };

    for (; arg_counter < argc; ++arg_counter) {
        auto found = arg_parser.find(argv[arg_counter]);
        if (found != arg_parser.end()) {
            parse_result result = found->second();
            switch (result) {
            case parse_result::ok:
                continue;
            case parse_result::stop:
                goto done_parsing;
            case parse_result::error:
                exit(EXIT_FAILURE);
            }
        } else {
            script_file.open(argv[arg_counter]);
            if (!script_file) {
                std::cerr << "Unable to open " << argv[arg_counter] << std::endl;
                exit(EXIT_FAILURE);
            }
            istream = &script_file;
            ostream = &dev_null;
            break;
        }
    }
done_parsing:;

    return yatl::repl(*istream, *ostream, std::cerr).exec();
}
