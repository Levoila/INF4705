#ifndef _GESTIONNAIRE_ECOLIERS_
#define _GESTIONNAIRE_ECOLIERS_

#include <utility>  //std::pair
#include <string>   //std::string
#include <vector>   //std::vector

enum NeighboursNeeded
{
    NEED_BOTH = 0,
    NEED_FRONT,
    NEED_BACK,
    NEED_NONE,
};

struct Ecolier
{
    unsigned int index;
    unsigned int height;
    unsigned int frontEcolierIndex;
    unsigned int backEcolierIndex;

    void spin()
    {
        unsigned int temp = frontEcolierIndex;
        frontEcolierIndex = backEcolierIndex;
        backEcolierIndex = temp;
    }

    inline bool isLinked()
    {
        // front and back are == 0 if unlinked
        return (frontEcolierIndex && backEcolierIndex);
    }

    inline NeighboursNeeded getNeighboursNeeded()
    {
        // There's someone in the front
        if (frontEcolierIndex != 0)
        {
            // There's someone in the back
            if (backEcolierIndex != 0)
            {
                return NEED_NONE;
            }
            // There's nobody in the back
            else
            {
                return NEED_BACK;
            }
        }
        // There's nobody in front
        else
        {
            // There's someone in the back
            if (backEcolierIndex != 0)
            {
                return NEED_FRONT;
            }
            // There's nobody in the back
            else
            {
                return NEED_BOTH;
            }
        }
    }
};

class GestionnaireEcoliers
{
public:
    GestionnaireEcoliers(int argc, char** argv);
    ~GestionnaireEcoliers();

    void calculerSolution();

    Ecolier getEcolier(unsigned int index);


private:
    unsigned int GestionnaireEcoliers::getMostRestrained(unsigned int*& possibleNeighboursAmount);
    std::pair<unsigned int, unsigned int> getClosestShorterTaller(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours);
    unsigned int GestionnaireEcoliers::getFrontStudent(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours);
    unsigned int GestionnaireEcoliers::getBackStudent(const Ecolier& ecolier, const std::vector<Ecolier>& possibleNeighbours);
    void linkEcoliers(Ecolier front, Ecolier back);

    void printOutput(double elapsed);

    unsigned int _nbEcoliers;
    unsigned int _nbPairesValides;
    Ecolier* _ecoliers;
    std::pair<unsigned int, unsigned int>* _pairesValides;
    bool _print;
    bool _benchmark;
    unsigned int _nbCaches;
    bool _solutionFound;
};

#endif //_GESTIONNAIRE_ECOLIERS_