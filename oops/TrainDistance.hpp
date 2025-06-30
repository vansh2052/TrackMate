#ifndef TRAINDISTANCE_HPP
#define TRAINDISTANCE_HPP

#include <string>
#include <array>

class TrainDistance {
public:
    virtual ~TrainDistance() = default;

    // Return a formatted string representation (e.g., table row).
    virtual std::string toString(int index) const = 0;

    //calculate distance between two cities
    //vector of strings of following format is returned:
    //{distance(nautical miles), distance(KM), distance(statute miles)}
    virtual std::array<std::string,3>
    calculateDistance(double lat1, double lon1,
                      double lat2, double lon2) const = 0;


    //Prints guidelines about how distances and times are measured.
    void displayMeasurementInstructions() const;
};

#endif // TRAINDISTANCE_HPP
