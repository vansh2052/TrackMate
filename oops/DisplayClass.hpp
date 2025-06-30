// DisplayClass.hpp
#ifndef DISPLAY_CLASS_HPP
#define DISPLAY_CLASS_HPP

#include <string>
#include <vector>

// Forward declarations
class Train;
class Customer;

class DisplayClass {
public:
    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~DisplayClass() = default;

    // Shows all passengers registered on every train
    virtual void displayRegisteredUsersForAllTrains() = 0;

    // Shows passengers registered on the train identified by trainNum
    virtual void displayRegisteredUsersForASpecificTrain(const std::string& trainNum) = 0;

    // Prints a header (or table) for a given train and list of customers
    virtual void displayHeaderForUsers(const Train& train,
                                       const std::vector<Customer>& customers) = 0;

    // Lists all trains a single user (by userID) has registered for
    virtual void displayTrainsRegisteredByOneUser(const std::string& userID) = 0;

    // Shows ASCII art or banners based on the option code
    virtual void displayArtWork(int option) = 0;
};

#endif // DISPLAY_CLASS_HPP
