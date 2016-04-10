#ifndef _CHAINE_ECOLIERS
#define _CHAINE_ECOLIERS

#include <utility>  //std::pair
#include <string>   //std::string
#include <vector>   //std::vector

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
        return (getNeighboursNeeded() == 0);
    }

    inline unsigned int getNeighboursNeeded()
    {
        return (frontEcolierIndex == 0 + backEcolierIndex == 0);
    }
};

class ChaineEcoliers
{
public:
    ChaineEcoliers(const Ecolier& ecolier);
    ChaineEcoliers();
    ~ChaineEcoliers();

    std::vector<Ecolier> getListeEcoliers();
    unsigned int getListeSize();
    Ecolier getFrontEcolier();
    Ecolier getBackEcolier();
    Ecolier getEcolier(unsigned int index);

    void mergeBack(ChaineEcoliers& backChaine);

    void spin();

private:

    std::vector<Ecolier> _listeEcoliers;
};

#endif //_CHAINE_ECOLIERS