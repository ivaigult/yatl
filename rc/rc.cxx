#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include <cstdlib>

int main(int argc, char** argv) {
    if (argc != 3) {
	std::string app_name = argv[0];
	size_t slash_pos = app_name.find_last_of("/\\");
	app_name = app_name.substr(slash_pos + 1);
	std::cerr << "Usage:\n";
	std::cerr << "\t" << app_name
		  << " <input-file> <output-file> <array-name>" << std::endl;
	exit(EXIT_FAILURE);
    }

    std::string input_filename = argv[1];
    std::ifstream input (input_filename, std::ios_base::binary);
    std::ofstream output(argv[2]);

    if (!input) {
	std::cerr << "File: " << input_filename
		  << " not found!" << std::endl;
	exit(EXIT_FAILURE);
    }
    
    std::vector<char> content((std::istreambuf_iterator<char>(input)),
                               std::istreambuf_iterator<char>());

    output << std::showbase << std::hex;
    
    uint32_t bytest_cnt = 0;
    static const uint32_t bytest_per_line = 40;
    size_t last_byte = static_cast<size_t>(content.size() - 1);
    for(size_t ii = 0; ii < content.size(); ++ii) {
	if (!bytest_cnt) {
	    output << "    ";
	}
	
	output << std::setw(4) << (unsigned) content[ii];
	if (ii != last_byte) {
	    output << ", ";
	}
	
	++bytest_cnt;
	if (bytest_per_line == bytest_cnt) {
	    output << "\n";
	    bytest_cnt = 0;
	}
    }
    output << std::flush;

    exit(EXIT_SUCCESS);
}
