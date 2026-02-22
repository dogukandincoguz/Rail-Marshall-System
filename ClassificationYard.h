#ifndef CLASSIFICATION_TRACK_H
#define CLASSIFICATION_TRACK_H

#include "WagonList.h"
#include "Train.h"
#include <string>

class ClassificationYard
{
private:
    // 2D matrix of WagonLists: [Destination][CargoType]
    WagonList blockTrains[NUM_DESTINATIONS_INT][NUM_CARGOTYPES_INT];

public:
    // Constructor and destructor
    ClassificationYard();
    ~ClassificationYard();

    // Getters
    WagonList &getBlockTrain(int destination, int cargoType);
    WagonList *getBlocksFor(Destination dest);

    // Core operations
    void insertWagon(Wagon *w);
    Train *assembleTrain(Destination dest, const std::string &trainName);

    // Utility functions
    bool isEmpty() const;
    void print() const;
    void clear();
};

#endif