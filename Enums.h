
#ifndef ENUMS_H
#define ENUMS_H

#include <string>

/**Destination and CargoType enums
 * and their helper functions.
 * All functions in this header are shared ready made with you in Enums.cpp
 */

enum class Destination
{
    ANKARA,
    IZMIR,
    ESKISEHIR,
    ADANA,
    OTHERS,
    NUM_DESTINATIONS
};
enum class CargoType
{
    COAL,
    OIL,
    HAZARDOUS,
    LIVESTOCK,
    MAIL,
    OTHER,
    NUM_CARGOTYPES
};

constexpr int NUM_DESTINATIONS_INT = static_cast<int>(Destination::NUM_DESTINATIONS);
constexpr int NUM_CARGOTYPES_INT = static_cast<int>(CargoType::NUM_CARGOTYPES);

std::string destinationToString(Destination d);
std::string cargoTypeToString(CargoType c);
std::string toUpper(const std::string &s);
Destination parseDestination(const std::string &s);
CargoType parseCargo(const std::string &s);

#endif