#include "Enums.h"
#include <algorithm>

/*Enum operations
* While not forbitten, you should not have anything to change in this file*/

std::string destinationToString(Destination d)
{
    static constexpr const char *names[] = {"ANKARA", "IZMIR", "ESKISEHIR", "ADANA", "OTHERS"};
    int i = static_cast<int>(d);
    if (i >= 0 && i < NUM_DESTINATIONS_INT)
        return names[i];
    return "OTHERS";
}

std::string cargoTypeToString(CargoType c)
{
    static constexpr const char *names[] = {"COAL", "OIL", "HAZARDOUS", "LIVESTOCK", "MAIL", "OTHER", "NUM_CARGOTYPES"};
    int i = static_cast<int>(c);
    if (i >= 0 && i < NUM_CARGOTYPES_INT)
        return names[i];
    return "OTHER";
}

std::string toUpper(const std::string &s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::toupper);
    return r;
}

Destination parseDestination(const std::string &s)
{
    std::string up = toUpper(s);
    if (up == "ANKARA")
        return Destination::ANKARA;
    if (up == "IZMIR")
        return Destination::IZMIR;
    if (up == "ESKISEHIR")
        return Destination::ESKISEHIR;
    if (up == "ADANA")
        return Destination::ADANA;
    return Destination::OTHERS;
}

CargoType parseCargo(const std::string &s)
{
    std::string up = toUpper(s);
    if (up == "COAL")
        return CargoType::COAL;
    if (up == "OIL")
        return CargoType::OIL;
    if (up == "HAZARDOUS")
        return CargoType::HAZARDOUS;
    if (up == "LIVESTOCK")
        return CargoType::LIVESTOCK;
    if (up == "MAIL")
        return CargoType::MAIL;
    return CargoType::OTHER;
}
