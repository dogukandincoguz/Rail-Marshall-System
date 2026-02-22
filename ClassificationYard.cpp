#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/

    Train* newTrain = new Train(trainName, dest);
    
    // Check for hazardous cargo - only one hazardous wagon per train
    WagonList& hazardousList = blockTrains[static_cast<int>(dest)][static_cast<int>(CargoType::HAZARDOUS)];
    Wagon* hazardousForTrain = nullptr;
    
    // If there are hazardous wagons, take only the FIRST one in the list (front = heaviest)
    // Others stay in the list for future trains
    if (!hazardousList.isEmpty())
    {
        // Take the front wagon (first in the sorted list = heaviest)
        hazardousForTrain = hazardousList.detachById(hazardousList.getFront()->getID());
        // Other hazardous wagons remain in the list for future ASSEMBLE_TRAIN calls
    }
    
    // Collect non-hazardous wagons first
    for (int cargo = 0; cargo < static_cast<int>(CargoType::NUM_CARGOTYPES); ++cargo)
    {
        if (cargo == static_cast<int>(CargoType::HAZARDOUS))
            continue; // Skip hazardous for now
            
        if (!blockTrains[static_cast<int>(dest)][cargo].isEmpty())
        {
            newTrain->appendWagonList(blockTrains[static_cast<int>(dest)][cargo]);
        }
    }
    
    // Add hazardous cargo at the very end (regardless of weight)
    if (hazardousForTrain)
    {
        // Add the hazardous wagon to the rear, regardless of its weight
        newTrain->addWagonToRear(hazardousForTrain);
    }
    
    return newTrain;
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty()){
                return false;
            }
        }
    }
    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            blockTrains[i][j].clear();
        }
    }
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}