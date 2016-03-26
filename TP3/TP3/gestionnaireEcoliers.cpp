#include "gestionnaireEcoliers.h"

#include <fstream>  //std::ifstream
#include <iostream> //std::cout
#include <chrono>   //std::chrono

GestionnaireEcoliers::GestionnaireEcoliers(int argc, char** argv)
    :_print(false), _benchmark(false)
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

        _ecoliers = new Ecolier[_nbEcoliers];

        // Fill the two tables
        for (unsigned int i = 0; i < _nbEcoliers; ++i)
        {
            _ecoliers[i].index = i;
            _ecoliers[i].frontEcolierIndex = 0;
            _ecoliers[i].backEcolierIndex = 0;
            fileIn >> _ecoliers[i].height;
        }

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
    delete(_ecoliers);
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

    for (unsigned int i = 0; i < _nbEcoliers; ++i)
    {
        if ( !(_ecoliers[i].isLinked()) && (possibleNeighboursAmount[i] < tempMostRestrainedAmount) )
        {
            tempMostRestrainedAmount = possibleNeighboursAmount[i];
            mostRestrained = i;
        }
    }
    return mostRestrained;
}

/* GestionnaireEcoliers::getClosestShorterTaller
    Receives the student to compare to and a list of possible neighbours
    Returns the shorter/taller neighbours that are as close as possible to his height
*/
std::pair<unsigned int, unsigned int> GestionnaireEcoliers::getClosestShorterTaller(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours)
{
    unsigned int currShorterIndex = 0;
    unsigned int currShorterHeight = 0;
    unsigned int currTallerIndex = 0;
    unsigned int currTallerHeight = 0; // unsigned -1 == max(unsigned int)

    unsigned int tempHeight = 0;

    for (unsigned int i = 0; i < possibleNeighbours.size(); ++i)
    {
        // possibleNeighbours contains the index of the neighbours, so we go get their height from tailleEcoliers_
        tempHeight = possibleNeighbours[i].height;

        // Currently checked neighbour is taller than me but smaller than the currently assigned possible neighbour
        if (tempHeight > ecolier.height && possibleNeighbours[i].frontEcolierIndex == 0 && tempHeight < currTallerHeight)
        {
            currTallerIndex = possibleNeighbours[i].index;
            currTallerHeight = tempHeight;
        }
        // Currently checked neighbour is shorter than me but smaller than the currently assigned possible neighbour
        else if (tempHeight < ecolier.height && possibleNeighbours[i].backEcolierIndex == 0 && tempHeight > currShorterHeight)
        {
            currShorterIndex = possibleNeighbours[i].index;
            currShorterHeight = tempHeight;
        }
    }
    return std::pair<unsigned int, unsigned int>(currShorterIndex, currTallerIndex);
}

unsigned int GestionnaireEcoliers::getFrontStudent(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours)
{
    unsigned int currShorterIndex = 0;
    unsigned int currShorterHeight = 0;
    unsigned int currTallerIndex = 0;
    unsigned int currTallerHeight = 0; // unsigned -1 == max(unsigned int)

    unsigned int tempHeight = 0;

    for (unsigned int i = 0; i < possibleNeighbours.size(); ++i)
    {
        // possibleNeighbours contains the index of the neighbours, so we go get their height from tailleEcoliers_
        tempHeight = possibleNeighbours[i].height;

        // Currently checked neighbour is taller than me but smaller than the currently assigned possible neighbour
        if (tempHeight > ecolier.height && possibleNeighbours[i].frontEcolierIndex == 0 && tempHeight < currTallerHeight)
        {
            currTallerIndex = possibleNeighbours[i].index;
            currTallerHeight = tempHeight;
        }
        // Currently checked neighbour is shorter than me but smaller than the currently assigned possible neighbour
        else if (tempHeight < ecolier.height && possibleNeighbours[i].backEcolierIndex == 0 && tempHeight > currShorterHeight)
        {
            currShorterIndex = possibleNeighbours[i].index;
            currShorterHeight = tempHeight;
        }
    }

    if (currShorterIndex == 0)
    {
        return currShorterIndex;
    }
    return currTallerIndex;
}

unsigned int GestionnaireEcoliers::getBackStudent(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours)
{
    unsigned int currShorterIndex = 0;
    unsigned int currShorterHeight = 0;
    unsigned int currTallerIndex = 0;
    unsigned int currTallerHeight = 0; // unsigned -1 == max(unsigned int)

    unsigned int tempHeight = 0;

    for (unsigned int i = 0; i < possibleNeighbours.size(); ++i)
    {
        // possibleNeighbours contains the index of the neighbours, so we go get their height from tailleEcoliers_
        tempHeight = possibleNeighbours[i].height;

        // Currently checked neighbour is taller than me but smaller than the currently assigned possible neighbour
        if (tempHeight > ecolier.height && possibleNeighbours[i].frontEcolierIndex == 0 && tempHeight < currTallerHeight)
        {
            currTallerIndex = possibleNeighbours[i].index;
            currTallerHeight = tempHeight;
        }
        // Currently checked neighbour is shorter than me but smaller than the currently assigned possible neighbour
        else if (tempHeight < ecolier.height && possibleNeighbours[i].backEcolierIndex == 0 && tempHeight > currShorterHeight)
        {
            currShorterIndex = possibleNeighbours[i].index;
            currShorterHeight = tempHeight;
        }
    }

    if (currTallerIndex == 0)
    {
        return currTallerIndex;
    }
    return currShorterIndex;
}

void GestionnaireEcoliers::linkEcoliers(Ecolier front, Ecolier back)
{
    front.backEcolierIndex = back.index;
    back.frontEcolierIndex = front.index;
}

Ecolier GestionnaireEcoliers::getEcolier(unsigned int index)
{
    return _ecoliers[index - 1];
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
        /* Get the most restrained student */
        unsigned int chosenIndex = getMostRestrained(possibleNeighbours);

        // Index - 1 because Ecolier.index is from 1 to _nbEcoliers
        //   -> _ecoliers[X] == _ecoliers[X].index - 1
        Ecolier chosenEcolier = getEcolier(chosenIndex);

        /* Get the student's possible neighbours */
        std::vector<Ecolier> studentPossibleNeighbours;
        for (unsigned int i = 0; i < _nbPairesValides; ++i)
        {
            if (_pairesValides[i].first == chosenEcolier.index)
            {
                studentPossibleNeighbours.push_back(getEcolier(_pairesValides[i].second));
            }
            else if (_pairesValides[i].second == chosenEcolier.index)
            {
                studentPossibleNeighbours.push_back(getEcolier(_pairesValides[i].first));
            }
        }

        std::pair<unsigned int, unsigned int> bothStudentsIndex;
        unsigned int chosenStudentIndex;
        switch (chosenEcolier.getNeighboursNeeded())
        {
        case NEED_BOTH:
            bothStudentsIndex = getClosestShorterTaller(chosenEcolier, studentPossibleNeighbours);
            linkEcoliers(getEcolier(bothStudentsIndex.first), chosenEcolier);
            linkEcoliers(chosenEcolier, getEcolier(bothStudentsIndex.second));
            break;
        case NEED_FRONT:
            chosenStudentIndex = getFrontStudent(chosenEcolier, studentPossibleNeighbours);
            linkEcoliers(getEcolier(chosenStudentIndex), chosenEcolier);
            break;
        case NEED_BACK:
            chosenStudentIndex = getFrontStudent(chosenEcolier, studentPossibleNeighbours);
            linkEcoliers(chosenEcolier, getEcolier(chosenStudentIndex));
            break;
        case NEED_NONE:
        default:
            break;
        }
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