#include "TrainReservation.hpp"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <limits>

//helper methods definitions
void TrainReservation::addTicketsToExistingBooking(Customer& customer, 
                                                   int numTickets)
{
    auto& counts = customer.getTicketCounts();
    counts[trainIndexInCustomerList_] += numTickets;
}

void TrainReservation::addTicketsToNewBooking(Customer& customer, 
                                              int numTickets)
{
    customer.getTicketCounts().push_back(numTickets);
}

bool TrainReservation::isTrainInCustomerList(const vector<Train*>& trains, 
                                             Train* train)
{
    for (size_t i = 0; i < trains.size(); ++i) {
        if (trains[i]->getTrainNumber() == train->getTrainNumber()) {
            trainIndexInCustomerList_ = static_cast<int>(i);
            return true;
        }
    }
    return false;
}

int TrainReservation::trainPosition(const vector<Train*>& trains, 
                                    Train* train)
{
    auto it = find_if(trains.begin(), trains.end(),
        [&](Train* t){ return t->getTrainNumber() == train->getTrainNumber(); });
    return (it != trains.end()) 
         ? static_cast<int>(distance(trains.begin(), it)) 
         : -1;
}

string TrainReservation::trainStatus(Train* train) const
{
    for (const auto& tptr : Train::getTrainList()) {
        if (tptr->getTrainNumber() == train->getTrainNumber())
            return "As Per Schedule";
    }
    return "   Cancelled   ";
}

string TrainReservation::bookingToString(int serial, Train* trn, 
                                         const Customer& cust) const
{
    ostringstream ss;
    ss << "| " << setw(4) << serial
       << " | " << setw(41) << trn->getTrainSchedule()
       << " | " << setw(9)  << trn->getTrainNumber()
       << " | " << setw(16) << cust.getTicketCount(serial)
       << " | " << setw(21) << trn->getFromStation()
       << " | " << setw(22) << trn->getToStation()
       << " | " << setw(11) << trn->fetchArrivalTime()
       << " | " << setw(9)  << trn->getTravelTimeStr() << "Hrs"
       << " | " << setw(6)  << trn->getPlatform()
       << " | " << setw(10) << trainStatus(trn)
       << " |";
    return ss.str();
}

string TrainReservation::userToString(int serial, 
                                      const Customer& cust, 
                                      int idx) const
{
    ostringstream ss;
    ss << setw(10) << ""
       << "| " << setw(5) << (serial+1)
       << " | " << setw(8) << cust.getFormattedUserID()
       << " | " << left << setw(32) << cust.getName()
       << " | " << setw(3) << cust.getAge()
       << " | " << setw(29) << cust.getEmail()
       << " | " << setw(35) << cust.getAddress()
       << " | " << setw(23) << cust.getPhone()
       << " | " << setw(5) << cust.getTicketCount(idx)
       << " |";
    return ss.str();
}

//PUBLIC METHODS

//method definition to book a train
void TrainReservation::bookTrain(const string& trainNo, 
                                 int numTickets, 
                                 const string& userID)
{
    bool booked = false;
    const auto& trains = Train::getTrainList();
    for (const auto& tptr : trains) {
        if (trainNo == tptr->getTrainNumber()) {
            for (auto& cust : Customer::customerCollection) {
                if (userID == cust.getUserID()) {
                    booked = true;

                    auto& regList = tptr->getRegisteredPassengers();
                    if (!tptr->isPassengerAlreadyAdded(regList, &cust)) {
                        tptr->addNewPassengerToTrain(&cust);
                        addTicketsToNewBooking(cust, numTickets);
                    }
                    const auto& userTrains = cust.getTrains();
                    if (isTrainInCustomerList(userTrains, tptr.get())) {
                        addTicketsToExistingBooking(cust, numTickets);
                    } else {
                        cust.addNewTrainToCustomerList(tptr.get());
                        addTicketsToNewBooking(cust, numTickets);
                    }
                    break;
                }
            }
            break;
        }
    }
    if (!booked) {
        cout << "Invalid Train Number \"" << trainNo << "\".\n";
    } else {
        cout << setw(50) 
             << "You've booked " << numTickets 
             << " tickets for Train \"" << trainNo << "\".\n";
    }
}

//method definition to cancel a train
void TrainReservation::cancelTrain(const string& userID)
{
    string trainNo;
    int index = 0;
    bool found = false;

    for (auto& cust : Customer::customerCollection) {
        if (userID == cust.getUserID()) {
            const auto& userTrains = cust.getTrains();
            if (userTrains.empty()) {
                cout << "No trains registered under ID \"" 
                     << userID << "\".\n";
                return;
            }

            displayTrainsRegisteredByOneUser(userID);
            cout << "Enter Train Number to cancel: ";
            getline(cin, trainNo);
            cout << "Enter number of tickets to cancel: ";
            int numTickets; 
            cin >> numTickets;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            for (auto it = userTrains.begin(); 
                 it != userTrains.end(); 
                 ++it, ++index)
            {
                if (trainNo == (*it)->getTrainNumber()) {
                    found = true;
                    int booked   = cust.getTicketCount(index);
                    int toReturn = min(numTickets, booked);

                    if (toReturn == booked) {
                        cust.removeTrainAt(index);
                    } else {
                        cust.setTicketCount(index, booked - toReturn);
                    }
                    break;
                }
            }

            if (!found) {
                cout << "No booking found for train \"" 
                     << trainNo << "\".\n";
            }
            return;
        }
    }
}

//the following four methods are method definitions to display trains as per need
void TrainReservation::displayTrainsRegisteredByOneUser(
    const string& userID)
{
    displayArtWork(6);
    cout << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+--------+-----------------+\n"
         << "| Num  | TRAIN SCHEDULE                           | TRAIN NO  | Booked Tickets   | FROM                  | TO                     | ARRIVAL TIME              | TRAVEL TIME | PLATFORM | TRAIN STATUS   |\n"
         << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+--------+-----------------+\n";
    for (const auto& cust : Customer::customerCollection) {
        if (cust.getUserID() == userID) {
            const auto& trains = cust.getTrains();
            for (size_t i = 0; i < trains.size(); ++i) {
                cout << bookingToString(i, trains[i], cust) << "\n"
                     << "+------+-------------------------------------------+-----------+------------------+-----------------------+------------------------+---------------------------+-------------+--------+-----------------+\n";
            }
            break;
        }
    }
}

void TrainReservation::displayHeaderForUsers(
    const Train& train, 
    const vector<Customer>& users)
{
    cout << "\n"
         << setw(65) 
         << "Displaying Registered Customers for Train No. \"" 
         << train.getTrainNumber() << "\"\n\n"
         << setw(10) << "" 
         << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+----------------+\n"
         << setw(10) << "" 
         << "| SerialNum  |   UserID   | Passenger Names                  | Age     | EmailID                      | Home Address                        | Phone Number            | Tickets         |\n"
         << setw(10) << "" 
         << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+----------------+\n";

    for (size_t i = 0; i < users.size(); ++i) {
        cout << userToString(static_cast<int>(i), users[i], trainPosition(
                             users[i].getTrains(), const_cast<Train*>(&train))) << "\n"
             << setw(10) << "" 
             << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+----------------+\n";
    }
}

void TrainReservation::displayRegisteredUsersForASpecificTrain(
    const string& trainNo)
{
    displayArtWork(4);
    const auto& trains = Train::getTrainList();
    for (const auto& tptr : trains) {
        if (tptr->getTrainNumber() == trainNo) {
            vector<Customer> customers;
            for (auto custPtr : tptr->getRegisteredPassengers()) {
                customers.push_back(*custPtr);
            }
            displayHeaderForUsers(*tptr, customers);
            break;
        }
    }
}

void TrainReservation::displayRegisteredUsersForAllTrains()
{
    displayArtWork(4);
    const auto& trains = Train::getTrainList();
    for (const auto& tptr : trains) {
        auto users = tptr->getRegisteredPassengers();
        if (!users.empty()) {
            vector<Customer> customers;
            for (auto custPtr : users) {
                customers.push_back(*custPtr);
            }
            displayHeaderForUsers(*tptr, customers);
        }
    }
}

//menu options
void TrainReservation::displayArtWork(int option)
{
    static const char* titles[] = {
        "","Book Train","Edit Info","Delete Account",
        "Random Train Schedule","Cancel Train",
        "Registered Trains by Passenger","Logged Out"
    };
    if (option >= 1 && option <= 7)
        cout << "=== " << titles[option] << " ===\n";
}
