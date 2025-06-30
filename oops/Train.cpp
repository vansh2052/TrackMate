#include "Train.hpp"
#include <iomanip>
#include <sstream>
#include <random>
#include <iostream>
using namespace std;
using namespace chrono;

// Helper functions for angle conversions
static double deg2rad(double deg) {
    return deg * (3.14159265358979323846 / 180.0);
}

static double rad2deg(double rad) {
    return rad * (180.0 / 3.14159265358979323846);
}

// Static member definitions
vector<unique_ptr<Train>> Train::trainList_;
int Train::nextTrainDay_ = 0;

const vector<unique_ptr<Train>>&
Train::getTrainList() {
    return trainList_;
}

// Helper to format chrono::time_point to display date and time in a readable manner
string Train::formatDateTime(const system_clock::time_point& tp,
                             const char* fmt) const {
    auto timeT = system_clock::to_time_t(tp);
    ostringstream ss;
    ss << put_time(localtime(&timeT), fmt);
    return ss.str();
}

// Default constructor
Train::Train()
  : trainSchedule_(), trainNumber_(), fromStation_(),
    toStation_(), platform_() {}

// Parameterized construtor
Train::Train(const string& schedule,
             const string& number,
             int seats,
             const array<array<string,3>,2>& chosenStations,
             const array<string,3>& distances,
             const string& platform)
  : trainSchedule_(schedule),
    trainNumber_(number),
    fromStation_(chosenStations[0][0]),
    toStation_(chosenStations[1][0]),
    platform_(platform),
    numOfSeats_(seats),
    distMiles_(stod(distances[0])),
    distKm_(stod(distances[1]))
{
    travelTime_ = calculateTravelTime(distMiles_);
}

// Generate a batch of random trains
void Train::trainScheduler() {
    const int numOfTrains = 15;
    RandomGenerator rnd;
    for (int i = 0; i < numOfTrains; ++i) {
        auto stations = rnd.randomDestinations();               // array<array<string,3>,2>
        auto dists = calculateDistance(
            stod(stations[0][1]), stod(stations[0][2]),
            stod(stations[1][1]), stod(stations[1][2])
        );
        string schedule = computeNextSchedule();
        string number   = rnd.randomTrainNumberGen(2,1);
        int seats            = rnd.randomNumOfSeats();
        string plat        = rnd.randomTrainNumberGen(1,30);
        trainList_.push_back(make_unique<Train>(
            schedule, number, seats, stations, dists, plat
        ));
    }
}

string Train::computeNextSchedule() {
    using namespace chrono;
    system_clock::time_point now = system_clock::now();
    // Add nextTrainDay_ days to now
    system_clock::time_point trainTime = now + hours(24 * nextTrainDay_);
    // Increment for next call
    ++nextTrainDay_;
    // Format date/time string
    return formatDateTime(trainTime, "%Y-%m-%d %H:%M");
}

// Register or update tickets
void Train::addNewPassengerToTrain(Customer* cust) {
    registeredPassengers_.push_back(cust);
}

void Train::addTicketsToExistingPassenger(Customer* cust, int numTickets) {
    cust->addExistingTrainToCustomerList(passengerIndex_, numTickets);
}

bool Train::isPassengerAlreadyAdded(const vector<Customer*>& list,
                                    Customer* cust)
{
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i]->getUserID() == cust->getUserID()) {
            passengerIndex_ = i;
            return true;
        }
    }
    return false;
}

// Time and distance calculations
string Train::calculateTravelTime(double miles) {
    const double speed = 60.0; // average train speed in miles per hour
    double hours = miles / speed;
    int h = static_cast<int>(hours);
    int m = static_cast<int>((hours - h) * 60 + 0.5);
    m = (m + 2) / 5 * 5;        // round to nearest 5 minutes
    if (m >= 60) { m -= 60; ++h; }
    ostringstream ss;
    ss << setw(2) << setfill('0') << h
       << ':' << setw(2) << setfill('0') << m;
    return ss.str();
}

array<string,3> Train::calculateDistance(double lat1,double lon1,
                                         double lat2,double lon2) const
{
    double theta = lon1 - lon2;
    double dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2))
                + cos(deg2rad(lat1)) * cos(deg2rad(lat2))
                  * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist) * 60 * 1.1515;
    array<string,3> out;
    out[0] = to_string(dist * 0.8684);
    out[1] = to_string(dist * 1.609344);
    out[2] = to_string(round(dist * 100.0) / 100.0);
    return out;
}

string Train::fetchArrivalTime() const {
    // For now, return trainSchedule_ as placeholder
    return trainSchedule_;
}

// Remove by train number
void Train::deleteTrain(const string& trainNum) {
    auto it = remove_if(
        trainList_.begin(), trainList_.end(),
        [&](const unique_ptr<Train>& t){
            return t->getTrainNumber() == trainNum;
        }
    );
    if (it != trainList_.end()) {
        trainList_.erase(it, trainList_.end());
    }
    displayTrainSchedule();
}

// Console display
void Train::displayTrainSchedule() {
    cout << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+----------+------------------------+\n";
    cout << "| Num  | TRAIN SCHEDULE                            | TRAIN NO  |    Total Seats    | FROM                  | TO                     | ARRIVAL TIME              | TRAVEL TIME | PLATFORM | DISTANCE (M/K)         |\n";
    cout << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+----------+------------------------+\n";
    for (size_t i = 0; i < trainList_.size(); ++i) {
        cout << trainList_[i]->toString(static_cast<int>(i+1)) << "\n";
    }
    cout << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+----------+------------------------+\n";
}

string Train::toString(int idx) const {
    ostringstream ss;
    ss << "| " << setw(4) << idx
       << " | " << setw(41) << trainSchedule_
       << " | " << setw(9)  << trainNumber_
       << " | " << setw(16) << numOfSeats_
       << " | " << setw(21) << fromStation_
       << " | " << setw(22) << toStation_
       << " | " << setw(11) << fetchArrivalTime()
       << " | " << setw(9)  << travelTime_ << "Hrs"
       << " | " << setw(8)  << platform_
       << " | " << setw(5)  << distMiles_
       << "/"  << setw(5)  << distKm_
       << " |";
    return ss.str();
}

// Simple getters
int Train::getNoOfSeats() const { return numOfSeats_; }
const string& Train::getTrainNumber() const { return trainNumber_; }
const vector<Customer*>& Train::getRegisteredPassengers() const { return registeredPassengers_; }
const string& Train::getTrainSchedule() const { return trainSchedule_; }
const string& Train::getFromStation() const { return fromStation_; }
const string& Train::getToStation() const { return toStation_; }
const string& Train::getPlatform() const { return platform_; }
const string& Train::getTravelTimeStr() const { return travelTime_; }
