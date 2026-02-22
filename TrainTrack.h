#ifndef TRAIN_TRACK_H
#define TRAIN_TRACK_H

#include "Train.h"
#include <string>

const int AUTO_DISPATCH_LIMIT = 2000;

// Trains can only depart from the front and be added to end
class TrainTrack
{

private:
    Train *firstLocomotive; // First locomative in the waiting line/track
    Train *lastLocomotive; // Last locomative in the waiting line/track
    Destination destination;
    int totalWeight;
    int trainCounter; // Index that will be used in newly created train's name

public:
    static bool autoDispatch; // Controls autoDispatch: on->true
public:
    TrainTrack();
    TrainTrack(Destination _dest);
    ~TrainTrack();

    // Getters
    Train *getFirst() const { return firstLocomotive; }
    Train *getLast() const { return lastLocomotive; }
    int getTotalWeight() const { return totalWeight; }
    // Setter for usability
    void setDestination(Destination dest) { destination = dest; }

    // Core operations
    void addTrain(Train *train);
    Train *departTrain(); // Makes first train in the track leave
    Train *findTrain(const std::string &name) const;

    // Utility functions
    std::string generateTrainName();
    bool isEmpty() const;
    void printTrack() const; // Debugging helper
};

#endif