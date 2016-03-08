#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

enum Algorithm {GREEDY, DYNAMIC, LOCAL};

struct Location
{
    int revenue;
    unsigned int quantity;
    unsigned int id;
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
    FileInput fileInput;

    std::ifstream fileIn(filename);
    if (fileIn.is_open())
    {
        int nbLocations;
        fileIn >> nbLocations;
        fileInput.locations.resize(nbLocations);
        for (int i = 0; i < nbLocations; ++i){
            fileIn >> fileInput.locations[i].id >> fileInput.locations[i].revenue >> fileInput.locations[i].quantity;
        }

        fileIn >> fileInput.capacity;
    } else {
        std::cout << "Error reading file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    return fileInput;
}

Inputs readArgs(int argc, char** argv)
{
	Inputs inputs;
	for (int i = 1; i < argc; ++i) {
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
