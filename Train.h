#ifndef TRAIN_H
#define TRAIN_H

#include "WagonList.h"
#include <string>

class Train
{

private:
    std::string name;
    Destination destination;
    int totalWeight;
    WagonList wagons;
    Train *nextLocomotive; // Pointer to next train in the same track

public:
    Train();
    Train(const std::string& _name, Destination _destination);
    ~Train();

    // Getters
    std::string getName() const { return name; }
    Destination getDestination() const { return destination; }
    int getTotalWeight() const { return totalWeight; }
    Train *getNext() const { return nextLocomotive; }
    void setNext(Train *next) { nextLocomotive = next; }
    void setName(const std::string& newName);
    void setDestination(Destination newDestination);
    WagonList& getWagons() { return wagons; }

    // Core operations
    void addWagonToRear(Wagon *w); // Add wagon to end of the train, does not protect weight order
    void appendWagonList(WagonList& wl); // Append two wagon lists together

    // Coupler safety check — splits if overloaded
    Train *verifyCouplersAndSplit(int splitCounter);

    // Utility functions
    void clear();
    void print() const;

    friend class TrainTrack;
};

#endif