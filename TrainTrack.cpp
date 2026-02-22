#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    // TODO: If track is deconstucting, 
    // depart all of the trains
    // Handle pointers as necessary
    while (firstLocomotive)
    {
        Train *removed = firstLocomotive;
        firstLocomotive = removed->getNext();
        removed->setNext(nullptr);
        delete removed;
    }
    lastLocomotive = nullptr;
    totalWeight = 0;
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    // TODO: Add a train to the end (rear) of this track
    // TODO: Hadle Auto-dispatch rule:
    //   If adding this train causes AUTO_DISPATCH_LIMIT to overflow
    //   and auto-dispatch is enabled, repeatedly dispatch (depart) trains
    //   from the front until there is enough capacity.
    //      use: std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";

    if (!train)
        return;

    train->setDestination(destination);
    train->setNext(nullptr);

    if (!isEmpty() && autoDispatch && (totalWeight + train->getTotalWeight() > AUTO_DISPATCH_LIMIT))
    {
        while (totalWeight + train->getTotalWeight() > AUTO_DISPATCH_LIMIT)
        {
            Train *departed = departTrain();
            if (!departed)
                break;
            std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";
            delete departed;
        }
    }
    // If track is empty
    if (isEmpty()){
        firstLocomotive = train;
        lastLocomotive = train;
        totalWeight += train->getTotalWeight();
        return;
    }
    // Setting pointers to add at the end
    lastLocomotive->setNext(train);
    lastLocomotive = train;
    totalWeight += train->getTotalWeight();

}

Train *TrainTrack::departTrain()
{
    // TODO: Remove the first train (front of the track) and return it.
    // use: std::cout << "Train " << removed->name << " departed from Track " << destinationToString(destination) << "." << std::endl;
    if (isEmpty()){
        return nullptr;
    }
    Train* removed = firstLocomotive;
    firstLocomotive = removed->getNext();
    if (removed == lastLocomotive){
        lastLocomotive = nullptr;
    }
    removed->setNext(nullptr);
    totalWeight -= removed->getTotalWeight();
    if (totalWeight < 0)
        totalWeight = 0;
    std::cout << "Train " << removed->getName() << " departed from Track " << destinationToString(destination) << "." << std::endl;
    return removed;

}

bool TrainTrack::isEmpty() const
{
    // TODO: Return true if there are no trains on this track.
    if (!firstLocomotive){
        return true;
    }
    return false;
}


Train *TrainTrack::findTrain(const std::string &name) const
{
    // TODO: Search for a train by name.
    // Return pointer to train if found, nullptr otherwise.
    Train* found = firstLocomotive;
    while (found){
        if (found->getName() == name){
            return found;
        }
        found = found->getNext();
    }
    return nullptr;
}

// Given to you ready
void TrainTrack::printTrack() const
{

    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}