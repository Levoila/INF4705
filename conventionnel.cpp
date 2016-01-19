#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

#include "helper.h"

using namespace std;

typedef std::vector<std::vector<int>> Matrix;

//Assuming same-sized matrices
void multiply(const Matrix& m1, const Matrix& m2, Matrix& res)
{
	for (unsigned int y = 0; y < m1.size(); ++y) {
		for (unsigned int x = 0; x < m1.size(); ++x) {
			for (unsigned int i = 0; i < m1.size(); ++i) {
				res[y][x] += m1[y][i] * m2[i][x];
			}
		}
	}
}

int main(int argc, char** argv)
{
	Inputs inputs = readArgs(argc, argv);
	
	if (inputs.mat1.size() == 0 || inputs.mat2.size() == 0) {
		cout << "No matrices." << endl;
		return 0;
	}
	
	unsigned size = inputs.mat1.size();
	auto res = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
	
	auto start = chrono::high_resolution_clock::now();
	multiply(inputs.mat1, inputs.mat2, res);
	auto end = chrono::high_resolution_clock::now();
	
	chrono::duration<double> elapsed = end - start;
	cout << fixed << setprecision(8) << elapsed.count() << endl;
	
	if (inputs.print) {
		printMatrix(res);
	}
	
	return 0;
}
