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

int main(int argc, char** argv)
{
    const char* script_file_name = nullptr;
    for (size_t ii = 1; ii < argc && !script_file_name; ++ii) {
        script_file_name = argv[ii];
    }
    std::istream* istream = &std::cin;
    std::ostream* ostream = &std::cout;
    std::ifstream script_file;
    std::ofstream dev_null;
    if (script_file_name) {
        script_file.open(script_file_name);
        istream = &script_file;
        ostream = &dev_null;
    }

    return yatl::repl(*istream, *ostream, std::cerr).exec();
}
