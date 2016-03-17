#include "helper.h"

#include <iostream>
#include <random>
#include <iomanip>
#include <limits>

#include "windows.h"

struct Result
{
	std::vector<unsigned int> locations;
	int revenu;
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
    int maxRevenue = 0;

    for (unsigned int i = 0; i < 10; ++i) {
        int remainingCapacity = inputs.fileInput.capacity;
        int totalRevenue = 0;
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
	std::vector<std::vector<int>> R(inputs.fileInput.locations.size()+1, std::vector<int>(inputs.fileInput.capacity+1, 0));

	for (unsigned int i = 1; i < R.size(); ++i) {
		int r = inputs.fileInput.locations[i - 1].revenue;
		unsigned int q = inputs.fileInput.locations[i - 1].quantity;

		for (unsigned int j = 1; j < R[0].size(); ++j) {
			R[i][j] = max(r + (j - q < 0 ? -(std::numeric_limits<int>::max)() : R[i - 1][j - q]), R[i - 1][j]);
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

//Returns -1 if the solution is not valid
int computeRevenu(const Inputs& inputs, const std::vector<unsigned int>& locations)
{
	unsigned int totalQ = 0;
	int totalR = 0;
	for (unsigned int locId : locations) {
		for (Location loc : inputs.fileInput.locations) {
			if (loc.id == locId) {
				totalQ += loc.quantity;
				totalR += loc.revenue;
			}
		}
	}
	return totalQ <= inputs.fileInput.capacity ? totalR : -1;
}

Result findBestLocalPermutation(const Inputs& inputs, const Result& result)
{
	Result bestResult = result;

	//Remove 1, put 1 back permutations
	for (unsigned int i = 0; i < result.locations.size(); ++i) {
		for (unsigned int j = 0; j < inputs.fileInput.locations.size(); ++j) {
			std::vector<unsigned int> temp = result.locations;
			temp[i] = inputs.fileInput.locations[j].id;

			int revenu = computeRevenu(inputs, temp);
			if (revenu > bestResult.revenu) {
				bestResult.locations = temp;
				bestResult.revenu = revenu;
			}
		}
	}

	//Remove 2, put 1 back permutations
	if (result.locations.size() >= 2) {
		for (unsigned int i = 0; i < result.locations.size(); ++i) {
			std::vector<unsigned int> removeOne = result.locations;
			removeOne.erase(removeOne.begin() + i);
			for (unsigned int j = 0; j < removeOne.size(); ++j) {
				for (unsigned int k = 0; k < inputs.fileInput.locations.size(); ++k) {
					std::vector<unsigned int> temp = removeOne;
					temp[j] = inputs.fileInput.locations[k].id;

					int revenu = computeRevenu(inputs, temp);
					if (revenu > bestResult.revenu) {
						bestResult.locations = temp;
						bestResult.revenu = revenu;
					}
				}
			}
		}
	}

	//Remove 1, put 2 back permutations
	if (result.locations.size() >= 2) {
		for (unsigned int i = 0; i < inputs.fileInput.locations.size(); ++i) {
			std::vector<unsigned int> addOne = result.locations;
			addOne.push_back(inputs.fileInput.locations[i].id);
			for (unsigned int j = 0; j < result.locations.size(); ++j) {
				for (unsigned int k = 0; k < inputs.fileInput.locations.size(); ++k) {
					std::vector<unsigned int> temp = addOne;
					temp[j] = inputs.fileInput.locations[k].id;

					int revenu = computeRevenu(inputs, temp);
					if (revenu > bestResult.revenu) {
						bestResult.locations = temp;
						bestResult.revenu = revenu;
					}
				}
			}
		}
	}

	return bestResult;
}

unsigned int n = 0;
Result localAlgorithm(const Inputs& inputs)
{
	Result best = greedyAlgorithm(inputs);

	while (true) {
		++n;
		Result next = findBestLocalPermutation(inputs, best);

		if (next.revenu <= best.revenu) {
			break;
		} else {
			best = next;
		}
	}

	return best;
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
		std::cout << r.revenu << std::endl << elapsed << std::endl << n;
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