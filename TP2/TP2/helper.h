#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

enum Algorithm {GREEDY, DYNAMIC, LOCAL};

struct Location
{
    int rentability;
    unsigned int quantity;
};
struct FileInput
{
    unsigned int capacity;
    std::vector<Location> locations;
};

struct Inputs
{
	Inputs() : print(false), algorithm(GREEDY) {}

    FileInput fileInput;
    Algorithm algorithm;
	bool print;
};

FileInput readFile(const std::string& filename)
{

}

Inputs readArgs(int argc, char** argv)
{
	Inputs inputs;
	for (unsigned int i = 1; i < argc; ++i) {
		std::string arg = std::string(argv[i]);
		
		if (arg == "-p") {
			inputs.print = true;
        } else if (arg == "-f" && i + 1 < argc) {
            inputs.fileInput = readFile(std::string(argv[i + 1]));
        } else if (arg == "-a" && i + 1 < argc) {
            std::string algo = std::string(argv[i + 1]);

            if (algo == "greedy") {
                inputs.algorithm = GREEDY;
            } else if (algo == "dynamic") {
                inputs.algorithm = DYNAMIC;
            } else if (algo == "local") {
                inputs.algorithm = LOCAL;
            }
        }
	}
	return inputs;
}

#endif
