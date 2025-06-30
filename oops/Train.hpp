#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <chrono>
#include "TrainDistance.hpp"
#include "Customer.hpp"
#include "RandomGenerator.hpp"
using namespace std;

class Train : public TrainDistance {
private:
    // Train data
    string trainSchedule_;
    string trainNumber_;
    string fromStation_;
    string toStation_;
    string platform_;
    int numOfSeats_{0};
    double distMiles_{0.0};
    double distKm_{0.0};
    string travelTime_;
    vector<Customer*> registeredPassengers_;
    size_t passengerIndex_{0};

    // Static collection of all trains
    static vector<unique_ptr<Train>> trainList_;
    static int nextTrainDay_;

    // Helpers
    string formatDateTime(const chrono::system_clock::time_point& tp,
                          const char* fmt) const;
    string calculateTravelTime(double miles);

    // New helper to compute next schedule string
    string computeNextSchedule();

public:
    // default and parameterized constructors
    Train();
    Train(const string& schedule,
          const string& number,
          int seats,
          const array<array<string,3>,2>& chosenStations,
          const array<string,3>& distances,
          const string& platform);

    // Scheduler and CRUD
    void trainScheduler(); 
    void addNewPassengerToTrain(Customer* cust);
    void addTicketsToExistingPassenger(Customer* cust, int numTickets);
    bool isPassengerAlreadyAdded(const vector<Customer*>& list,
                                 Customer* cust);
    void deleteTrain(const string& trainNum);

    // Time & distance
    string fetchArrivalTime() const;
    array<string,3> calculateDistance(double lat1,double lon1,
                                      double lat2,double lon2) const override;

    // Display
    static void displayTrainSchedule();
    string toString(int index) const override;

    // Getters
    int getNoOfSeats() const;
    const string& getTrainNumber() const;
    static const vector<unique_ptr<Train>>& getTrainList();
    const vector<Customer*>& getRegisteredPassengers() const;
    const string& getTrainSchedule() const;
    const string& getFromStation() const;
    const string& getToStation() const;
    const string& getPlatform() const;
    const string& getTravelTimeStr() const;
};

#endif // TRAIN_HPP
