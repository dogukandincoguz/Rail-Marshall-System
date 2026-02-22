#ifndef WAGON_H
#define WAGON_H

#include <string>
#include "Enums.h"


class Wagon
{

private:
    int id;
    CargoType cargoType;
    Destination destination;
    int weight; 
    int maxCouplerLoad; // Max weight a wagon can pull

    Wagon *prev; // Previous wagon on the WagonList
    Wagon *next; // Next wagon on the WagonList

public:
    Wagon(int _id, CargoType _cargo, Destination _dest, int _weight, int _maxCouplerLoad);

    ~Wagon();

    // Getter and Setters
    int getWeight() const;
    int getID() const;
    CargoType getCargoType() const;
    Destination getDestination() const;
    int getMaxCouplerLoad() const;
    Wagon *getPrev();
    Wagon *getNext();
    void setNext(Wagon *n);
    void setPrev(Wagon *p);

    // Overloaded operation for custom print
    friend std::ostream& operator<<(std::ostream& os, const Wagon& w);

    friend class WagonList;
};

#endif