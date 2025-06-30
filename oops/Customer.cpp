#include "Customer.hpp"
#include "RandomGenerator.hpp"  // implementation of ID generator
#include "Train.hpp"            // For Train*

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

// Static vector to store customers defined here
vector<Customer> Customer::customerCollection;


//definition of helper functions for proper display of items
string Customer::randomIDDisplay(const string& id) const {
    ostringstream oss;
    for (size_t i = 0; i < id.size(); ++i) {
        if (i == 3) oss << ' ';
        oss << id[i];
    }
    return oss.str();
}

string Customer::toString(int index) const {
    ostringstream oss;
    oss << setw(10) << ""
        << "| " << setw(5)  << index
        << " | " << setw(8)  << randomIDDisplay(userID_)
        << " | " << left << setw(32) << name_
        << " | " << setw(3)  << age_
        << " | " << setw(29) << email_
        << " | " << setw(35) << address_
        << " | " << setw(23) << phone_
        << " |";
    return oss.str();
}

void Customer::displayHeader() {
    cout 
      << setw(10) << "" 
      << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+\n"
      << setw(10) << "" 
      << "| SerialNum  |   UserID   | Passenger Names                  | Age     | EmailID                      | Home Address                        | Phone Number            |\n"
      << setw(10) << "" 
      << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+\n\n";
}

void Customer::displayArtWork(int option) {
    switch (option) {
        case 1: cout << "=== New Customer ===\n"; break;
        case 2: cout << "=== Search Customer ===\n"; break;
        case 3: cout << "=== Showing All Passengers ===\n"; break;
        case 4: cout << "=== Registered Passengers In Train ===\n"; break;
        case 5: cout << "=== Delete Customer ===\n"; break;
        default: break;
    }
}

//definition of default and parameterized constructors
Customer::Customer() = default;

Customer::Customer(const string& name,
                   const string& email,
                   const string& password,
                   const string& phone,
                   const string& address,
                   int age)
    : email_(email),
      name_(name),
      phone_(phone),
      password_(password),
      address_(address),
      age_(age)
{
    RandomGenerator rnd;
    rnd.randomIDGen();
    userID_ = rnd.getRandomNumber();
}


//method definition for additon of new customer
void Customer::addNewCustomer() {
    displayArtWork(1);
    string name, email, password, phone, address;
    int age;

    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your email address: ";
    getline(cin, email);

    cout << "Enter your password: ";
    getline(cin, password);
    cout << "Enter your phone number: ";
    getline(cin, phone);
    cout << "Enter your address: ";
    getline(cin, address);
    cout << "Enter your age: ";
    cin >> age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    customerCollection.emplace_back(name, email, password, phone, address, age);
}
//method definition to check whether a customer already exists or not
bool Customer::isUniqueData(const string& email) {
    return none_of(
        customerCollection.begin(),
        customerCollection.end(),
        [&](const Customer& c){ return c.email_ == email; }
    );
}
//method definition to display data of all customers
void Customer::displayCustomersData(bool showHeader) {
    if (showHeader) displayArtWork(3);
    displayHeader();
    int idx = 1;
    for (const auto& c : customerCollection) {
        cout << c.toString(idx++) << "\n";
        cout << setw(10) << ""
                  << "+------------+------------+----------------------------------+---------+-----------------------------+-------------------------------------+-------------------------+\n";
    }
}
//static method to search for a customer
void Customer::searchUser(const string& userID) {
    displayArtWork(2);
    auto it = find_if(
        customerCollection.begin(),
        customerCollection.end(),
        [&](const Customer& c){ return c.userID_ == userID; }
    );
    if (it != customerCollection.end()) {
        cout << "Customer Found! Full Record:\n";
        displayHeader();
        cout << it->toString(1) << "\n";
    } else {
        cout << "No Customer with ID " << userID << " found.\n";
    }
}
//method definition to edit the info of a customer
void Customer::editUserInfo(const string& userID) {
    auto it = find_if(
        customerCollection.begin(),
        customerCollection.end(),
        [&](Customer& c){ return c.userID_ == userID; }
    );
    if (it == customerCollection.end()) {
        cout << "No Customer with ID " << userID << " found.\n";
        return;
    }

    string name, email, phone, address;
    int age;

    cout << "Enter new name: ";
    getline(cin, name);
    cout << "Enter new email: ";
    getline(cin, email);
    cout << "Enter new phone: ";
    getline(cin, phone);
    cout << "Enter new address: ";
    getline(cin, address);
    cout << "Enter new age: ";
    cin >> age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    it->name_    = name;
    it->email_   = email;
    it->phone_   = phone;
    it->address_ = address;
    it->age_     = age;

    displayCustomersData(false);
}
//method definition to delete a customer
void Customer::deleteUser(const string& userID) {
    auto it = remove_if(
        customerCollection.begin(),
        customerCollection.end(),
        [&](const Customer& c){ return c.userID_ == userID; }
    );
    if (it != customerCollection.end()) {
        customerCollection.erase(it, customerCollection.end());
        cout << "Deleted customer " << userID << ".\n Updated list:\n";
        displayCustomersData(false);
    } else {
        cout << "No Customer with ID " << userID << " found.\n";
    }
}

//Train related methods to add a new train booked by a customer and to add more tickets to an already booked train

void Customer::addNewTrainToCustomerList(Train* t) {
    trainsBookedByCustomer_.push_back(t);
    numOfTicketsBookedByCustomer_.push_back(1);
}

void Customer::addExistingTrainToCustomerList(size_t idx, int numTickets) {
    if (idx < numOfTicketsBookedByCustomer_.size())
        numOfTicketsBookedByCustomer_[idx] += numTickets;
}

//getters definition
const string& Customer::getUserID()  const { return userID_;  }
const string& Customer::getEmail()   const { return email_;   }
const string& Customer::getName()    const { return name_;    }
const string& Customer::getPhone()   const { return phone_;   }
const string& Customer::getAddress() const { return address_; }
int                Customer::getAge()     const { return age_;     }

//setters definition
void Customer::setName(const string& v)    { name_    = v; }
void Customer::setEmail(const string& v)   { email_   = v; }
void Customer::setPhone(const string& v)   { phone_   = v; }
void Customer::setAddress(const string& v) { address_ = v; }
void Customer::setAge(int v)                    { age_     = v; }

const string& Customer::getPassword() const {
    return password_;
}

//methods for TrainReservation
vector<int>& Customer::getTicketCounts() {
    return numOfTicketsBookedByCustomer_;
}

int Customer::getTicketCount(size_t idx) const {
    if (idx < numOfTicketsBookedByCustomer_.size())
        return numOfTicketsBookedByCustomer_[idx];
    return 0;
}

string Customer::getFormattedUserID() const {
    return randomIDDisplay(userID_);
}

const vector<Train*>& Customer::getTrains() const {
    return trainsBookedByCustomer_;
}

void Customer::removeTrainAt(size_t idx) {
    if (idx < trainsBookedByCustomer_.size()) {
        trainsBookedByCustomer_.erase(trainsBookedByCustomer_.begin() + idx);
        numOfTicketsBookedByCustomer_.erase(numOfTicketsBookedByCustomer_.begin() + idx);
    }
}

void Customer::setTicketCount(size_t idx, int count) {
    if (idx < numOfTicketsBookedByCustomer_.size()) {
        numOfTicketsBookedByCustomer_[idx] = count;
    }
}
