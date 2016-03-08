#include "helper.h"

#include <iostream>
#include <random>

unsigned int chooseRandomIndex(const std::vector<float>& rentabilities, float sumRentability, unsigned int numberOfItems)
{
    static std::random_device rd;
    static std::default_random_engine randomEngine(rd());

    // Between 0 and sumRentability
    std::uniform_real_distribution<float> distribution(0.0, sumRentability);
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

void greedyAlgorithm(Inputs inputs)
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

            chosenLocations.push_back(tempLocations[chosenIndex].id);

            if (remainingCapacity <= tempLocations[chosenIndex].quantity) {
                totalRevenue += remainingCapacity * tempRentabilities[chosenIndex];

                if (totalRevenue > maxRevenue) {
                    maxRevenue = totalRevenue;
                    maxLocations = chosenLocations;
                }
                break;
            }

            remainingCapacity -= tempLocations[chosenIndex].quantity;
            sumRentability -= tempRentabilities[chosenIndex];
            totalRevenue += tempLocations[chosenIndex].revenue;

            remove(tempRentabilities, chosenIndex);
            remove(tempLocations, chosenIndex);
        }
    }

    if (inputs.print) {
        for (unsigned int i = 0; i < maxLocations.size(); ++i) {
            std::cout << maxLocations[i] << " ";
        }
        std::cout << std::endl << "Revenu total : " << maxRevenue << std::endl;
    }

}


int main(int argc, char** argv)
{
    Inputs inputs = readArgs(argc, argv);

    switch (inputs.algorithm)
    {
        case Algorithm::GREEDY:
            greedyAlgorithm(inputs);
            break;
        case Algorithm::DYNAMIC:
            break;
        case Algorithm::LOCAL:
            break;
        default:
            std::cout << "Error, unknown algorithm" << std::endl;
            break;
    }
}