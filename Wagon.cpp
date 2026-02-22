#include "Wagon.h"
#include <iostream>

Wagon::Wagon(int _id, CargoType _cargo, Destination _dest, int _weight, int _maxCouplerLoad)
    : id(_id), cargoType(_cargo), destination(_dest),
      weight(_weight), maxCouplerLoad(_maxCouplerLoad),
      prev(nullptr), next(nullptr) {}

Wagon::~Wagon()
{
}

// Getters are already implemented
int Wagon::getWeight() const { return weight; }
int Wagon::getID() const { return id; }
CargoType Wagon::getCargoType() const { return cargoType; }
Destination Wagon::getDestination() const { return destination; }
int Wagon::getMaxCouplerLoad() const { return maxCouplerLoad; }

Wagon *Wagon::getPrev() { return prev; }
Wagon *Wagon::getNext() { return next; }
void Wagon::setNext(Wagon *n) { next = n; };
void Wagon::setPrev(Wagon *p) { prev = p; };

// Given to you ready to help with uniformaty at output
std::ostream &operator<<(std::ostream &os, const Wagon &w)
{
    os << "[ID:" << w.id
       << " | Cargo:" << cargoTypeToString(w.cargoType)
       << " | Dest:" << destinationToString(w.destination)
       << " | Weight:" << w.weight
       << " | MaxCoupler:" << w.maxCouplerLoad << "]";
    return os;
}
