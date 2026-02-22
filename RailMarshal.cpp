#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        departureYard[i].setDestination(static_cast<Destination>(i));
    }
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
    classificationYard.clear();
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.
    std::istringstream iss(line);
    std::string command ;
    iss >> command;

    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;

    if (command == "ADD_WAGON")
    {
        int id,weight,maxCouplerLoad;
        Destination dest;
        CargoType cargo;
        std::string destStr, cargoStr;
        iss >> id >> cargoStr >> destStr >> weight >> maxCouplerLoad;
        dest = parseDestination(destStr);
        cargo = parseCargo(cargoStr);
        if(iss.fail()){
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }
        Wagon* w = new Wagon(id,cargo,dest,weight,maxCouplerLoad);
        classificationYard.insertWagon(w);
        std::cout << "Wagon " << *w << " added to yard." << std::endl;
        return;
    }

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;
    if (command == "REMOVE_WAGON")
    {
        int id;
        iss >> id;
        if(iss.fail()){
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i){
            for (int j = 0; j < NUM_CARGOTYPES_INT; ++j){
                Wagon* removed = classificationYard.getBlockTrain(i,j).detachById(id);
                if (removed){
                    std::cout << "Wagon " << id << " removed." << std::endl;
                    delete removed;
                    return;
                }
            }
        }
        std::cout << "Error: Wagon " << id << " not found." << std::endl;
        return;
    }

    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
    if (command == "ASSEMBLE_TRAIN")
    {
        std::string destStr;
        iss >> destStr;
        if(iss.fail()){
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        Train* t = classificationYard.assembleTrain(dest, "Train_"+destStr);
        if (t->getTotalWeight() == 0){
            std::cout << "No wagons to assemble for " << destStr << std::endl;
            delete t;
            return;
        }

        TrainTrack &track = departureYard[static_cast<int>(dest)];
        const std::string numberedName = track.generateTrainName();
        t->setName(numberedName);

        int splitCounter = 0;
        while (true){
            Train* newTrain = t->verifyCouplersAndSplit(splitCounter + 1);
            if (newTrain){
                ++splitCounter;
                track.addTrain(newTrain);
                std::cout << "Train " << newTrain->getName() << " assembled after split with "
                          << newTrain->getWagons()<< " wagons." << std::endl;
            } else {
                break;
            }
        }

        track.addTrain(t);
        std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
        return;
    }

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;
    if (command == "DISPATCH_TRAIN")
    {
        std::string destStr;
        iss >> destStr;
        if(iss.fail()){
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        TrainTrack& track = departureYard[static_cast<int>(dest)];
        if (track.isEmpty()){
            std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
            return;
        }
        Train* t = track.departTrain();
        std::cout << "Dispatching " << t->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;
        delete t; 
        return;
    }

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";
    if (command == "PRINT_YARD")
    {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
        return;
    }

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
    if (command == "PRINT_TRACK")
    {
        std::string destStr;
        iss >> destStr;
        if(iss.fail()){
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        departureYard[static_cast<int>(dest)].printTrack();
        return;
    }

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"
    if (command == "AUTO_DISPATCH")
    {
        std::string state;
        iss >> state;
        if(iss.fail()){
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        if (state == "ON"){
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled" << std::endl;
        } else if (state == "OFF"){
            TrainTrack::autoDispatch = false;
            std::cout << "Auto dispatch disabled" << std::endl;
        } else {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }
        return;
    }

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;
    if (command == "CLEAR")
    {
        classificationYard.clear();
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i){
            while (!departureYard[i].isEmpty()){
                Train* t = departureYard[i].departTrain();
                delete t;
            }
        }
        std::cout << "System cleared." << std::endl;
        return;
    }

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    std::cout << "Error: Unknown command '" << command << "'" << std::endl;
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
    Train* t = departureYard[static_cast<int>(track)].departTrain();
    if (!t) {
        std::cout << "Error: No trains to dispatch from Track " << static_cast<int>(track) << ".\n";
        return;
    }
    std::cout << "Train " << t->getName()
              << " departed from Track " << static_cast<int>(track)
              << " (" << destinationToString(static_cast<Destination>(track)) << ").\n";
    delete t;
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
