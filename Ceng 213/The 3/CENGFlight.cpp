#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    std::vector<float> weights(2,0);
    if(navigationMap.findEdgeWeight(airportFrom,airportTo,airlineName,weights)){
        HaltedFlight halted;
        halted.airportFrom = airportFrom;
        halted.airportTo = airportTo;
        halted.airline = airlineName;
        halted.w0 = weights[0];
        halted.w1 = weights[1];
        haltedFlights.push_back(halted);
        navigationMap.RemoveEdge(airlineName,airportFrom,airportTo);
    }
    
    else{
        PrintCanNotHalt(airportFrom,airportTo,airlineName);
    }
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    for(int i = 0 ; i< haltedFlights.size() ; i++){
        if(haltedFlights[i].airportFrom == airportFrom && haltedFlights[i].airportTo == airportTo && haltedFlights[i].airline == airlineName){
            navigationMap.AddEdge(airlineName, airportFrom, airportTo, haltedFlights[i].w0, haltedFlights[i].w1);
            haltedFlights.erase(haltedFlights.begin() + i);
            return;
        }
    }
    PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    std::vector<int> intArray;
    int startIndx = navigationMap.findIndex(startAirportName);
    int endIndx = navigationMap.findIndex(endAirportName);
    if(alpha == 0){
        if(lruTable.Find(intArray, startIndx, endIndx, true, true)){
            PrintFlightFoundInCache(startAirportName, endAirportName, true);
            navigationMap.PrintPath(intArray, alpha, true);
            return;
        } 
    }
    else if(alpha == 1){
        if(lruTable.Find(intArray, startIndx, endIndx, false, true)){
            PrintFlightFoundInCache(startAirportName, endAirportName, false);
            navigationMap.PrintPath(intArray, alpha, true);
            return;
        } 
    }
    if( navigationMap.HeuristicShortestPath(intArray, startAirportName, endAirportName, alpha) ){
        if(alpha == 0){
            try{
                lruTable.Insert(intArray, true);
            }
            catch(TableCapFullException a){
                int lruCount = lruTable.lruElementCount();
                lruTable.RemoveLRU(lruCount);
                lruTable.Insert(intArray, true);
                
            }
            PrintFlightCalculated(startAirportName, endAirportName, true);
        } 
        else if (alpha == 1){
            try{
                lruTable.Insert(intArray, false);
            }
            catch(TableCapFullException a){
                int lruCount = lruTable.lruElementCount();
                lruTable.RemoveLRU(lruCount);
                lruTable.Insert(intArray, true);
                
            }
            PrintFlightCalculated(startAirportName, endAirportName, false);
        }
        navigationMap.PrintPath(intArray, alpha, true);
    }
    else PrintPathDontExist(startAirportName, endAirportName);
    
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    std::vector<int> intArray;
    if( navigationMap.FilteredShortestPath(intArray, startAirportName, endAirportName, alpha, unwantedAirlineNames) ){
        navigationMap.PrintPath(intArray, alpha, true);
    }
    
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    int indx = navigationMap.findIndex(airportName);
    if(indx >= 0){
        
    }
    else PrintSisterAirlinesDontCover(airportName);
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    try{
        int i = navigationMap.MaxDepthViaEdgeName(airportName, airlineName);
        return i;
    }
    catch(VertexNotFoundException a){
        return -1;
    }
}