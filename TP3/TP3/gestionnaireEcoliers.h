#ifndef _GESTIONNAIRE_ECOLIERS_
#define _GESTIONNAIRE_ECOLIERS_

#include <utility>  //std::pair
#include <string>   //std::string

class GestionnaireEcoliers
{
public:
    GestionnaireEcoliers(int argc, char** argv);
    ~GestionnaireEcoliers();

    void calculerSolution();


private:
    void printOutput(double elapsed);

    unsigned int nbEcoliers_;
    unsigned int nbPairesValides_;
    unsigned int* tailleEcoliers_;
    std::pair<unsigned int, unsigned int>* pairesValides_;
    bool print_;
    bool benchmark_;
    unsigned int nbCaches_;
};

#endif //_GESTIONNAIRE_ECOLIERS_