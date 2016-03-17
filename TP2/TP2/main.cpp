#include "helper.h"

#include <iostream>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <chrono>
#include <set>

struct Result
{
	std::vector<Location> locations;
	unsigned int quantity;
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

	std::vector<Location> maxLocations;
	int maxRevenue = 0;
	unsigned int maxQuantity = 0;

	for (unsigned int i = 0; i < 10; ++i) {
		int remainingCapacity = inputs.fileInput.capacity;
		int totalRevenue = 0;
		sumRentability = totalSumRentability;

		std::vector<Location> tempLocations = locations;
		std::vector<float> tempRentabilities = rentabilities;
		std::vector<Location> chosenLocations;

		while (true) {
			unsigned int chosenIndex = chooseRandomIndex(tempRentabilities, sumRentability, tempLocations.size());

			// If the currently chosen index makes us go over the limit, stop the process
			if (remainingCapacity < (int)tempLocations[chosenIndex].quantity) {
				if (totalRevenue > maxRevenue) {
					maxRevenue = totalRevenue;
					maxLocations = chosenLocations;
					maxQuantity = inputs.fileInput.capacity - remainingCapacity;
				}
				break;
			}
			else {
				// If we reach here, we're sure that we can make the sale
				chosenLocations.push_back(tempLocations[chosenIndex]);

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
	r.quantity = maxQuantity;

	return r;
}

Result dynamicAlgorithm(const Inputs& inputs)
{
	std::vector<std::vector<int>> R(inputs.fileInput.locations.size() + 1, std::vector<int>(inputs.fileInput.capacity + 1, 0));

	for (unsigned int i = 1; i < R.size(); ++i) {
		int r = inputs.fileInput.locations[i - 1].revenue;
		int q = static_cast<int>(inputs.fileInput.locations[i - 1].quantity);

		for (int j = 1; j < R[0].size(); ++j) {
			R[i][j] = std::max(r + (j - q < 0 ? -(std::numeric_limits<int>::max)() : R[i - 1][j - q]), R[i - 1][j]);
		}
	}

	Result r;

	int i = R.size() - 1;
	int j = R[0].size() - 1;

	r.revenu = R[i][j];

	//Find path
	while (j != 0 && i != 0) {
		if (R[i][j] != R[i - 1][j]) {
			r.locations.push_back(inputs.fileInput.locations[i - 1]);
			j -= inputs.fileInput.locations[i - 1].quantity;
		}
		--i;
	}

	r.quantity = 0;
	for (Location l : r.locations)
		r.quantity += l.quantity;

	return r;
}

Result findBestLocalPermutation(const Inputs& inputs, const Result& result)
{
	Result bestResult = result;

	auto choice = inputs.fileInput.locations;
	//Remove locations that are already in the solution
	for (Location loc : result.locations)
		choice.erase(std::remove_if(choice.begin(), choice.end(), [loc](const Location l){ return l.id == loc.id; }));

	//Remove 1, put 1 back permutations
	for (unsigned int i = 0; i < result.locations.size(); ++i) {
		Location locToRemove = result.locations[i];
		for (Location locToAdd : choice) {
			unsigned int newQ = result.quantity + locToAdd.quantity - locToRemove.quantity;
			if (newQ > inputs.fileInput.capacity) {
				continue;
			}
			else {
				int newR = result.revenu + locToAdd.revenue - locToRemove.revenue;
				if (newR > bestResult.revenu) {
					bestResult.locations = result.locations;
					bestResult.locations[i] = locToAdd;
					bestResult.revenu = newR;
					bestResult.quantity = newQ;
				}
			}
		}
	}

	//Remove 2, put 1 back permutations
	if (result.locations.size() >= 2) {
		for (unsigned int i = 0; i < result.locations.size(); ++i) {
			Location locToRemove1 = result.locations[i];

			for (unsigned int j = i + 1; j < result.locations.size(); ++j) {
				Location locToRemove2 = result.locations[j];

				for (Location locToAdd : choice) {
					unsigned int newQ = result.quantity + locToAdd.quantity - locToRemove1.quantity - locToRemove2.quantity;
					if (newQ > inputs.fileInput.capacity) {
						continue;
					}
					else {
						int newR = result.revenu + locToAdd.revenue - locToRemove1.revenue - locToRemove2.revenue;
						if (newR > bestResult.revenu) {
							bestResult.locations = result.locations;
							bestResult.locations[i] = locToAdd;
							bestResult.locations[j] = bestResult.locations.back();
							bestResult.locations.pop_back();
							bestResult.revenu = newR;
							bestResult.quantity = newQ;
						}
					}
				}
			}
		}
	}

	//Remove 1, put 2 back permutations
	if (result.locations.size() >= 2) {
		for (unsigned int i = 0; i < result.locations.size(); ++i) {
			Location locToRemove = result.locations[i];

			for (unsigned int j = 0; j < choice.size(); ++j) {
				Location locToAdd1 = choice[j];

				for (unsigned int k = j + 1; k < choice.size(); ++k) {
					Location locToAdd2 = choice[k];

					unsigned int newQ = result.quantity + locToAdd1.quantity + locToAdd2.quantity - locToRemove.quantity;
					if (newQ > inputs.fileInput.capacity) {
						continue;
					}
					else {
						int newR = result.revenu + locToAdd1.revenue + locToAdd2.revenue - locToRemove.revenue;
						if (newR > bestResult.revenu) {
							bestResult.locations = result.locations;
							bestResult.locations[i] = locToAdd1;
							bestResult.locations.push_back(locToAdd2);
							bestResult.revenu = newR;
							bestResult.quantity = newQ;
						}
					}
				}
			}
		}
	}

	//Since we can't add locations that were already in the initial result, we can't simply add a location
	//using "remove 1, put 2 back" permutations. So, we add a special case for simply adding a new location
	for (Location locToAdd : choice) {
		unsigned int newQ = result.quantity + locToAdd.quantity;
		if (newQ > inputs.fileInput.capacity) {
			continue;
		} else {
			int newR = result.revenu + locToAdd.revenue;
			if (newR > bestResult.revenu) {
				bestResult.locations = result.locations;
				bestResult.locations.push_back(locToAdd);
				bestResult.revenu = newR;
				bestResult.quantity = newQ;
			}
		}
	}

	return bestResult;
}

Result localAlgorithm(const Inputs& inputs)
{
	Result best = greedyAlgorithm(inputs);

	while (true) {
		Result next = findBestLocalPermutation(inputs, best);

		if (next.revenu <= best.revenu) {
			break;
		} else {
			best = next;
		}
	}

	return best;
}

//Used to debug algorithms
bool isValid(const Inputs& inputs, const Result& r)
{
	std::set<unsigned int> idSet;
	for (Location loc : r.locations)
		idSet.insert(loc.id);

	if (idSet.size() != r.locations.size()) {
		std::cout << "Duplicate IDs in result!\n";
		return false;
	}

	unsigned int Q = 0;
	int R = 0;
	for (Location loc : r.locations) {
		Q += loc.quantity;
		R += loc.revenue;
	}

	if (Q != r.quantity) {
		std::cout << "Quantity does not match!\n";
		return false;
	}

	if (R != r.revenu) {
		std::cout << "Revenu does not match! " << R << " vs " << r.revenu << std::endl;
		return false;
	}

	if (Q > inputs.fileInput.capacity) {
		std::cout << "Quantity too big!\n";
		return false;
	}

	return true;
}


int main(int argc, char** argv)
{
	Inputs inputs = readArgs(argc, argv);

	auto begin = std::chrono::high_resolution_clock::now();

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

	auto end = std::chrono::high_resolution_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1000000000.0;

	if (inputs.benchmark) {
		std::cout << r.revenu << std::endl << elapsed;
	}
	else {
		if (inputs.print) {
			std::cout << "Emplacements : ";
			for (auto loc : r.locations) {
				std::cout << loc.id << " ";
			}
			std::cout << std::endl << "Revenu : " << r.revenu << std::endl;
		}

		std::cout << "Temps : " << elapsed << "s" << std::endl;
	}
}