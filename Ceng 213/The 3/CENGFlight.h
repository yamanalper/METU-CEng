#ifndef CENG_FLIGHT_H
#define CENG_FLIGHT_H

#include "HashTable.h"
#include "MultiGraph.h"

#define FLIGHT_TABLE_SIZE 29

struct HaltedFlight
{
    std::string airportFrom;
    std::string airportTo;
    std::string airline;
    float w0;
    float w1;
};

class CENGFlight
{
    private:
    HashTable<FLIGHT_TABLE_SIZE> lruTable;
    MultiGraph                   navigationMap;

    // Print Related
    // Use these
    static void PrintCanNotHalt(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName);
    static void PrintCanNotResumeFlight(const std::string& airportFrom,
                                        const std::string& airportTo,
                                        const std::string& airlineName);
    static void PrintFlightFoundInCache(const std::string& airportFrom,
                                        const std::string& airportTo,
                                        bool isCostWeighted);
    static void PrintFlightCalculated(const std::string& airportFrom,
                                      const std::string& airportTo,
                                      bool isCostWeighted);
    static void PrintPathDontExist(const std::string& airportFrom,
                                   const std::string& airportTo);

    static void PrintSisterAirlinesDontCover(const std::string& airportFrom);

    //
    std::vector<HaltedFlight>    haltedFlights;
    //

    protected:
    public:
    // Constructors & Destructor
            CENGFlight(const std::string& flightMapPath);

    // Members
    // Halting/Continuing flights

    // (Direct Function call)
    void    HaltFlight(const std::string& airportFrom,
                       const std::string& airportTo,
                       const std::string& airlineName);

    // (Direct Function call)
    void    ContinueFlight(const std::string& airportFrom,
                           const std::string& airportTo,
                           const std::string& airlineName);
    // Flight Finding
    // (Direct Function call)
    void    FindFlight(const std::string& startAirportName,
                       const std::string& endAirportName,
                       float alpha);
    // (Direct Function call)
    void    FindSpecificFlight(const std::string& startAirportName,
                               const std::string& endAirportName,
                               float alpha,
                               const std::vector<std::string>& unwantedAirlineNames) const;

    // Print the transfers from a specific airport, only for certain depth
    void    FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const;

    // (Direct Function call)
    int     FurthestTransferViaAirline(const std::string& airportName,
                                       const std::string& airlineName) const;

    // Printing Functions
    void    PrintMap();
    void    PrintCache();
};

#endif // CENG_FLIGHT_H