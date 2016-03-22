#include "gestionnaireEcoliers.h"

#include <fstream>  //std::ifstream
#include <iostream> //std::cout
#include <chrono>   //std::chrono

GestionnaireEcoliers::GestionnaireEcoliers(int argc, char** argv)
    :print_(false), benchmark_(false)
{
    // Parse inputs
    std::string filename;
    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "-p") {
            print_ = true;
        }
        else if (arg == "-f" && i + 1 < argc) {
            filename = std::string(argv[i + 1]);
        }
        else if (arg == "-b") {
            benchmark_ = true;
        }
    }

    // Parse file
    std::ifstream fileIn(filename);
    if (fileIn.is_open())
    {
        fileIn >> nbEcoliers_ >> nbPairesValides_;

        tailleEcoliers_ = new unsigned int[nbEcoliers_];

        for (unsigned int i = 0; i < nbEcoliers_; ++i)
        {
            fileIn >> tailleEcoliers_[i];
        }

        unsigned int tempValide1 = 0;
        unsigned int tempValide2 = 0;
        pairesValides_ = new std::pair<unsigned int, unsigned int>[nbPairesValides_];
        for (unsigned int i = 0; i < nbPairesValides_; ++i)
        {
            fileIn >> pairesValides_[i].first >> pairesValides_[i].second;
        }
    }
    else {
        std::cout << "Error reading file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
}

GestionnaireEcoliers::~GestionnaireEcoliers()
{
    delete(tailleEcoliers_);
    delete(pairesValides_);
}

void GestionnaireEcoliers::calculerSolution()
{
    auto begin = std::chrono::high_resolution_clock::now();

    /* Algorithme */

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1000000000.0;

    printOutput(elapsed);
}

void GestionnaireEcoliers::printOutput(double elapsed)
{
    if (benchmark_) {
        std::cout << nbCaches_ << std::endl << elapsed;
    }
    std::cout << nbCaches_ << std::endl;
    if (print_) {
        /*
        for solution
        std::cout << taille de l'etudiant a l'index << std::endl;
        */
    }
    std::cout << "fin";
}