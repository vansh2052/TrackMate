#include "TrainDistance.hpp"
#include <iostream>
#include <iomanip>
#include <string>

void TrainDistance::displayMeasurementInstructions() const {
    const std::string symbols = "+---------------------------+";
    const std::string title   = "|   SOME IMPORTANT GUIDELINES   |";
    const int totalWidth = 100;

    // Helper to center a line within totalWidth
    auto printCentered = [&](const std::string& line){
        int padding = std::max(0, (totalWidth - static_cast<int>(line.size())) / 2);
        std::cout << std::string(padding, ' ') << line << "\n";
    };

    std::cout << "\n\n";
    printCentered(symbols);
    printCentered(title);
    printCentered(symbols);
    std::cout << "\n"
              << "\t1. Distance calculations are based on station coordinates (latitudes & longitudes).\n\n"
              << "\t2. Actual train distances may vary based on rail tracks, terrain, and junctions.\n\n"
              << "\t3. Train travel time depends on factors such as track conditions, train type,\n"
                 "\t   scheduled halts, and signal delays.\n\n"
              << "\t4. Arrival time is approximate; please allow a buffer of ±15 minutes.\n\n"
              << "\t5. Departure time is when the train leaves the platform. Arrival time refers to\n"
              << "\t   the train's complete halt at the platform (not initial slowdown).\n\n";
}
