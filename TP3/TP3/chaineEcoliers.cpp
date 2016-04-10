#include "chaineEcoliers.h"

ChaineEcoliers::ChaineEcoliers(const Ecolier& ecolier)
{
    _listeEcoliers.push_back(ecolier);
}

ChaineEcoliers::ChaineEcoliers()
{
}

ChaineEcoliers::~ChaineEcoliers(){ }

std::vector<Ecolier> ChaineEcoliers::getListeEcoliers()
{
    return _listeEcoliers;
}

unsigned int ChaineEcoliers::getListeSize()
{
    return _listeEcoliers.size();
}

Ecolier ChaineEcoliers::getFrontEcolier()
{
    return _listeEcoliers.front();
}

Ecolier ChaineEcoliers::getBackEcolier()
{
    return _listeEcoliers.back();
}

Ecolier ChaineEcoliers::getEcolier(unsigned int index)
{
    return _listeEcoliers[index];
}

void ChaineEcoliers::mergeBack(ChaineEcoliers& backChaine)
{
    // Save the previous size
    unsigned int prevSize = _listeEcoliers.size();

    // Resize
    _listeEcoliers.resize(_listeEcoliers.size() + backChaine.getListeSize());

    // Copy
    for (unsigned int i = 0; i < backChaine.getListeSize(); ++i)
    {
        _listeEcoliers[prevSize + i] = backChaine.getEcolier(i);
    }

    // Link
    _listeEcoliers[prevSize - 1].backEcolierIndex = _listeEcoliers[prevSize].index;
    _listeEcoliers[prevSize].frontEcolierIndex = _listeEcoliers[prevSize - 1].index;
}

void ChaineEcoliers::spin()
{
    // Reverse the vector itself
    std::reverse(_listeEcoliers.begin(), _listeEcoliers.end());

    // Spin the ecoliers
    for (unsigned int i = 0; i < _listeEcoliers.size(); ++i)
    {
        _listeEcoliers[i].spin();
    }
}