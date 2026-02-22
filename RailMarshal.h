/**
 * Central controller managing the railway system and user commands.
 *
 * The RailMarshal class acts as the system orchestrator. It manages the
 * ClassificationYard and DepartureYard (collection of TrainTracks),
 * processes user commands such as adding wagons, assembling trains,
 * dispatching trains, and printing the system state.
 */

#ifndef RAIL_MARSHAL_H
#define RAIL_MARSHAL_H

#include "Enums.h"
#include "TrainTrack.h"
#include "ClassificationYard.h"

class RailMarshal
{
private:
    ClassificationYard classificationYard;
    TrainTrack departureYard[NUM_DESTINATIONS_INT] = {};

public:
    RailMarshal();
    ~RailMarshal();

    // Getters
    ClassificationYard &getClassificationYard();
    TrainTrack &getDepartureYard(Destination dest);

    // Command handling
    void processCommand(const std::string &commandLine);

    // System state display
    void printDepartureYard() const;
    void dispatchFromTrack(Destination dest);
    void printStatus() const;
};

#endif // RAIL_MARSHAL_H
