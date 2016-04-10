#include "gestionnaireEcoliers.h"

#include <fstream>      //std::ifstream
#include <iostream>     //std::cout
#include <chrono>       //std::chrono
#include <algorithm>    //std::max

GestionnaireEcoliers::GestionnaireEcoliers(int argc, char** argv)
    :_print(false), _benchmark(false), _solutionFound(false)
{
    // Parse inputs
    std::string filename;
    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "-p") {
            _print = true;
        }
        else if (arg == "-f" && i + 1 < argc) {
            filename = std::string(argv[i + 1]);
        }
        else if (arg == "-b") {
            _benchmark = true;
        }
    }

    // Parse file
    std::ifstream fileIn(filename);
    if (fileIn.is_open())
    {
        fileIn >> _nbEcoliers >> _nbPairesValides;

        _chaines.resize(_nbEcoliers);

        // Fill _chaines vector
        Ecolier tempEcolier;
        for (unsigned int i = 0; i < _nbEcoliers; ++i)
        {
            tempEcolier.index = i;
            tempEcolier.frontEcolierIndex = 0;
            tempEcolier.backEcolierIndex = 0;
            fileIn >> tempEcolier.height;
            _chaines[i] = ChaineEcoliers(tempEcolier);
        }

        // Fill the _pairesValides table
        unsigned int tempValide1 = 0;
        unsigned int tempValide2 = 0;
        _pairesValides = new std::pair<unsigned int, unsigned int>[_nbPairesValides];
        for (unsigned int i = 0; i < _nbPairesValides; ++i)
        {
            fileIn >> _pairesValides[i].first >> _pairesValides[i].second;
        }
    }
    else {
        std::cout << "Error reading file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
}

GestionnaireEcoliers::~GestionnaireEcoliers()
{
    delete(_pairesValides);
}

/* GestionnaireEcoliers::getMostRestrained
Receives a table containing all the possible neighbours for each student
Returns the index of the most restrained student
*/
unsigned int GestionnaireEcoliers::getMostRestrained(unsigned int*& possibleNeighboursAmount)
{
    unsigned int mostRestrained = 0;
    unsigned int tempMostRestrainedAmount = -1; // The goal is to get the one with the least amount of neighbours, so we start at max(unsigned int) and minimize

    for (unsigned int i = 0; i < _chaines.size(); ++i)
    {
        /*Idea:
        1- Get the front/back students of the sub-chain
        2- Get the possible neighbours for each ^
        3- Decide if we merge 1->2 or 2->1 and merge them
        */
    }
    std::cout << "Most Restrained: " << mostRestrained << std::endl;
    return mostRestrained;
}

void GestionnaireEcoliers::calculateHidden()
{
    _nbCaches = 0;
    std::vector<Ecolier> listeEcoliers = _chaines[0].getListeEcoliers();
    unsigned int currentTallestStudent = listeEcoliers[0].height;

    // For _nbEcoliers - 1 because we start with one student outside the loop
    for (unsigned int i = 1; i < listeEcoliers.size(); ++i)
    {
        // The current student is blocked by the currently tallest student
        if (currentTallestStudent > listeEcoliers[i].height)
        {
            ++_nbCaches;
        }
        else
        {
            currentTallestStudent = listeEcoliers[i].height;
        }
    }
}

void GestionnaireEcoliers::calculerSolution()
{
    auto begin = std::chrono::high_resolution_clock::now();

    // Create a table containing the number of possible neighbours of each student
    unsigned int* possibleNeighbours = new unsigned int[_nbEcoliers];
    for (unsigned int i = 0; i < _nbEcoliers; ++i)
    {
        possibleNeighbours[i] = 0;
    }
    for (unsigned int i = 0; i < _nbPairesValides; ++i)
    {
        ++possibleNeighbours[_pairesValides[i].first];
        ++possibleNeighbours[_pairesValides[i].second];
    }

    while (!_solutionFound)
    {
        /*Idea:
            1- Get the most restrained student /sub-chain
            2- Get the possible neighbours / sub-chains
            3- Decide if we merge 1->2 or 2->1 and merge them
            4- Update the restrained table, sub-chains vector, etc.
        */
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1000000000.0;

    printOutput(elapsed);
}

void GestionnaireEcoliers::printOutput(double elapsed)
{
    if (_benchmark) {
        std::cout << "Elapsed: " << elapsed << std::endl;
    }
    std::cout << _nbCaches << std::endl;
    if (_print) {
        /*
        for solution
        std::cout << taille de l'etudiant a l'index << std::endl;
        */
    }
    std::cout << "fin";
}