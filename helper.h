#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct Inputs
{
	Inputs() : N(0), print(false) {}

	std::vector<std::vector<int>> mat1;
	std::vector<std::vector<int>> mat2;
	unsigned int N;
	bool print;
	unsigned int threshold; //Only used by strassenSeuilCalcule
};

std::vector<std::vector<int>> readMatrix(const std::string& filename)
{
	std::ifstream file(filename);
	
	if (!file) {
		std::cout << "Matrix file " << filename << "does not exist." << std::endl;
		return std::vector<std::vector<int>>();
	}
	
	unsigned int N;
	file >> N;
	
	unsigned int size = 1 << N;
	auto matrix = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
	
	for (unsigned int y = 0; y < size; ++y) {
		for (unsigned int x = 0; x < size; ++x) {
			int number;
			file >> number;
			matrix[y][x] = number;
		}
	}
	
	return matrix;
}

Inputs readArgs(int argc, char** argv)
{
	Inputs inputs;
	for (unsigned int i = 1; i < argc; ++i) {
		std::string arg = std::string(argv[i]);
		
		if (arg == "-p") {
			inputs.print = true;
		} else if (arg == "-f") {
			if (i+2 >= argc) {
				continue;
			}
			
			inputs.mat1 = readMatrix(std::string(argv[i+1]));
			inputs.mat2 = readMatrix(std::string(argv[i+2]));
		} else if (arg == "-s") {
			if (i+1 >= argc) {
				continue;
			}
			
			inputs.threshold = atoi(arg.c_str()); //Quick and dirty
		}
	}
	return inputs;
}

#endif
