#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include <vector>
using namespace std;

// Forward declarations of classes
class Train;
class RandomGenerator;

class Customer {
private:
    //attributes
    string userID_;
    string email_;
    string name_;
    string phone_;
    string password_;
    string address_;
    int age_{0};

public:
    const string& getPassword() const;
    
    vector<Train*> trainsBookedByCustomer_;
    vector<int>    numOfTicketsBookedByCustomer_;

    // Helpers
    string randomIDDisplay(const string& id) const;
    string toString(int index) const;

public:
    static void displayArtWork(int option);
    static void displayHeader();

public:
    // Shared collection of all customers
    //declared static so that it is shared by all methods in common
    static vector<Customer> customerCollection;

    //Default and parameterized Constructors
    Customer();
    Customer(const string& name,
             const string& email,
             const string& password,
             const string& phone,
             const string& address,
             int age);

    //Customer management (CRUD operations)
    static void addNewCustomer();
    static bool isUniqueData(const string& email);
    static void displayCustomersData(bool showHeader = true);
    static void searchUser(const string& userID);
    static void editUserInfo(const string& userID);
    static void deleteUser(const string& userID);

    // Train-related
    void addNewTrainToCustomerList(Train* t);
    void addExistingTrainToCustomerList(size_t idx, int numTickets);

    // New methods to support TrainReservation usage
    vector<int>& getTicketCounts();
    int getTicketCount(size_t idx) const;
    string getFormattedUserID() const;
    const vector<Train*>& getTrains() const;
    void removeTrainAt(size_t idx);
    void setTicketCount(size_t idx, int count);

    // Getters
    const string& getUserID()  const;
    const string& getEmail()   const;
    const string& getName()    const;
    const string& getPhone()   const;
    const string& getAddress() const;
    int                getAge()     const;

    // Setters
    void setName(const string& v);
    void setEmail(const string& v);
    void setPhone(const string& v);
    void setAddress(const string& v);
    void setAge(int v);
};

#endif // CUSTOMER_HPP
