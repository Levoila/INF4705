#include "helper.h"

#include <iostream>
#include <random>
#include <iomanip>
#include <limits>

#include "windows.h"

struct Result
{
	std::vector<unsigned int> locations;
	double revenu;
};

unsigned int chooseRandomIndex(const std::vector<float>& rentabilities, float sumRentability, unsigned int numberOfItems)
{
    static std::random_device rd;
    static std::default_random_engine randomEngine(rd());

    // Between 0 and sumRentability
    std::uniform_real_distribution<float> distribution(0.0f, sumRentability);
    float value = distribution(randomEngine);
    for (unsigned int i = 0; i < numberOfItems; ++i)
    {
        value -= rentabilities[i];
        if (value < 0.0f)
        {
            return i;
        }
    }
    // Approximation error
    return numberOfItems - 1;
}

template <typename T>
void remove(std::vector<T>& v, unsigned int index)
{
    v[index] = v.back();
    v.pop_back();
}


Result greedyAlgorithm(const Inputs& inputs)
{
    std::vector<Location> locations = inputs.fileInput.locations;
    std::vector<float> rentabilities(inputs.fileInput.locations.size());

    float sumRentability = 0.0;
    for (unsigned int i = 0; i < rentabilities.size(); ++i)
    {
        rentabilities[i] = (float)locations[i].revenue / (float)locations[i].quantity;
        sumRentability += rentabilities[i];
    }
    const float totalSumRentability = sumRentability;

    std::vector<unsigned int> maxLocations;
    float maxRevenue = 0.0;

    for (unsigned int i = 0; i < 10; ++i) {
        int remainingCapacity = inputs.fileInput.capacity;
        float totalRevenue = 0;
        sumRentability = totalSumRentability;

        std::vector<Location> tempLocations = locations;
        std::vector<float> tempRentabilities = rentabilities;
        std::vector<unsigned int> chosenLocations;

        while (true) {
            unsigned int chosenIndex = chooseRandomIndex(tempRentabilities, sumRentability, tempLocations.size());

            // If the currently chosen index makes us go over the limit, stop the process
            if (remainingCapacity < (int)tempLocations[chosenIndex].quantity) {
                if (totalRevenue > maxRevenue) {
                    maxRevenue = totalRevenue;
                    maxLocations = chosenLocations;
                }
                break;
            } else {
                // If we reach here, we're sure that we can make the sale
                chosenLocations.push_back(tempLocations[chosenIndex].id);

                remainingCapacity -= tempLocations[chosenIndex].quantity;
                sumRentability -= tempRentabilities[chosenIndex];
                totalRevenue += tempLocations[chosenIndex].revenue;

                remove(tempRentabilities, chosenIndex);
                remove(tempLocations, chosenIndex);
            }
        }
    }

	Result r;
	r.locations = maxLocations;
	r.revenu = maxRevenue;

	return r;
}

Result dynamicAlgorithm(const Inputs& inputs)
{
	std::vector<std::vector<double>> R(inputs.fileInput.locations.size()+1, std::vector<double>(inputs.fileInput.capacity+1, 0.0));

	for (unsigned int i = 1; i < R.size(); ++i) {
		double r = inputs.fileInput.locations[i - 1].revenue;
		double q = inputs.fileInput.locations[i - 1].quantity;

		for (unsigned int j = 1; j < R[0].size(); ++j) {
			R[i][j] = max(r + (j - q < 0 ? -(std::numeric_limits<double>::max)() : R[i - 1][j - q]), R[i - 1][j]);
		}
	}

	Result r;

	unsigned int i = R.size() - 1;
	unsigned int j = R[0].size() - 1;

	r.revenu = R[i][j];

	//Find path
	while (j != 0 && i != 0) {
		if (R[i][j] != R[i - 1][j]) {
			r.locations.push_back(inputs.fileInput.locations[i - 1].id);
			j -= inputs.fileInput.locations[i - 1].quantity;
		}
		--i;
	}

	return r;
}

Result localAlgorithm(const Inputs& inputs)
{
	Result initial = greedyAlgorithm(inputs);

	while (true) {

	}

	Result r;
	
	return r;
}


int main(int argc, char** argv)
{
    Inputs inputs = readArgs(argc, argv);

	LARGE_INTEGER frequency;
	LARGE_INTEGER begin;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&begin);

	Result r;
    switch (inputs.algorithm)
    {
        case Algorithm::GREEDY:
            r = greedyAlgorithm(inputs);
            break;
        case Algorithm::DYNAMIC:
			r = dynamicAlgorithm(inputs);
            break;
        case Algorithm::LOCAL:
			r = localAlgorithm(inputs);
            break;
        default:
            std::cout << "Error, unknown algorithm" << std::endl;
            break;
    }

	QueryPerformanceCounter(&end);

	double elapsed = static_cast<double>(end.QuadPart - begin.QuadPart) / frequency.QuadPart;

	if (inputs.benchmark) {
		std::cout << r.revenu << std::endl << elapsed;
	} else {
		if (inputs.print) {
			std::cout << "Emplacements : ";
			for (auto loc : r.locations) {
				std::cout << loc << " ";
			}
			std::cout << std::endl << "Revenu : " << r.revenu << std::endl;
		}

		std::cout << "Time : " << elapsed << "s" << std::endl;
	}
}