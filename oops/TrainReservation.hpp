#ifndef TRAIN_RESERVATION_HPP
#define TRAIN_RESERVATION_HPP

#include <string>
#include <vector>
#include "DisplayClass.hpp"
#include "Train.hpp"
#include "Customer.hpp"
using namespace std;

class TrainReservation : public DisplayClass {
private:
    // Index of a customer’s train in their personal list
    int trainIndexInCustomerList_{-1};

    // Helpers
    void addTicketsToExistingBooking(Customer& customer, int numTickets);
    void addTicketsToNewBooking(Customer& customer, int numTickets);
    bool isTrainInCustomerList(const vector<Train*>& trains, 
                               Train* train);
    int  trainPosition(const vector<Train*>& trains, 
                       Train* train);
    string trainStatus(Train* train) const;

    // Formatting helpers(used to display in a clean way)
    string bookingToString(int serial, Train* train, 
                           const Customer& customer) const;
    string userToString(int serial, const Customer& customer, 
                        int ticketIndex) const;

public:
    // Booking and cancellation
    void bookTrain(const string& trainNo, int numTickets, 
                   const string& userID);
    void cancelTrain(const string& userID);

    // DisplayClass implementations
    void displayTrainsRegisteredByOneUser(const string& userID) override;
    void displayHeaderForUsers(const Train& train,
                               const vector<Customer>& users) override;
    void displayRegisteredUsersForAllTrains() override;
    void displayRegisteredUsersForASpecificTrain(
        const string& trainNo) override;
    void displayArtWork(int option) override;
};

#endif // TRAIN_RESERVATION_HPP
