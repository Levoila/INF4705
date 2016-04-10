#ifndef _GESTIONNAIRE_ECOLIERS
#define _GESTIONNAIRE_ECOLIERS

#include <utility>  //std::pair
#include <string>   //std::string
#include <vector>   //std::vector

#include "chaineEcoliers.h"

class GestionnaireEcoliers
{
public:
    GestionnaireEcoliers(int argc, char** argv);
    ~GestionnaireEcoliers();

    void calculerSolution();


private:
    unsigned int getMostRestrained(unsigned int*& possibleNeighboursAmount);

    void calculateHidden();

    void printOutput(double elapsed);

    unsigned int _nbEcoliers;
    unsigned int _nbPairesValides;
    std::vector<ChaineEcoliers> _chaines;
    std::pair<unsigned int, unsigned int>* _pairesValides;

    bool _print;
    bool _benchmark;

    unsigned int _nbCaches;
    bool _solutionFound;

    unsigned int _firstStudentIndex;
    unsigned int _lastStudentIndex;
};

#endif //_GESTIONNAIRE_ECOLIERS