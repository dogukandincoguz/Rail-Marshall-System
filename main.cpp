#include "RailMarshal.h"
#include <iostream>
#include <fstream>
#include <string>

/* You will not submit the main.cpp, 
  as such you can make any changes you want to it during testing.
  You can, for example, call individual functions from here to test their results. */


int main(int argc, char* argv[]) {

    // Code to work with an input file

    if (argc < 2) {
        std::cerr << "Usage: ./HUTrain <input_file>\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Error: Cannot open input file.\n";
        return 1;
    }

    RailMarshal marshal;
    std::string line;

    while (std::getline(infile, line)) {
        // skip empty lines or comment lines if needed
        if (line.empty() || line[0] == '#') continue;
        marshal.processCommand(line);
    }

    return 0;
}
