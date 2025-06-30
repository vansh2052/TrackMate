#include "User.hpp"

#include "RolesAndPermissions.hpp"
#include "Train.hpp"
#include "TrainReservation.hpp"
#include "Customer.hpp"

#include <iostream>
#include <limits>
#include <sstream>
using namespace std;


int User::run() {

    //default admin with default username and password with just view ability
    RolesAndPermissions auth({{"Standard","1234"}});

    // Generate a batch of trains
    Train trainSchedulerInstance;
    trainSchedulerInstance.trainScheduler();

    // Booking engine
    TrainReservation bookingSystem;

    //Welcome sign and text and display of the main menu
    cout << "\n\t\t\t+++++++++++++ Welcome to BAV Railways +++++++++++++\n\n"
              << "To Further Proceed, Please enter a value.\n\n"
              << "***** Default Username & Password is Standard/1234 *****\n"
              << "  Using default credentials grants view-only access.\n\n";
    displayMainMenu();

    //user input for valid choice
    int choice;
    cin >> choice;
    while (choice < 0 || choice > 5) {
        cout << "ERROR! Enter a value between 0 and 5: ";
        cin >> choice;
    }

    //main loop
    do {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //choice 0 is for logout
        if (choice == 0) {
            welcomeScreen(-1);
            break;
        }
        //choice 1 is for login as admin
        else if (choice == 1) {
            printArtWork(1);
            string user, pass;
            cout << "Username: "; getline(cin, user);
            cout << "Password: "; getline(cin, pass);

            int idx = auth.isPrivilegedUserOrNot(user, pass);
            if (idx < 0) {
                cout << "ERROR: Invalid admin credentials.\n";
            }
            else if (idx == 0) {
                cout << "Logged in as standard admin (view only).\n";
                Customer::displayCustomersData(true);
            }
            else {
                cout << "Logged in as \"" << user << "\".\n";
                int sub;
                do {
                    cout << "\n\t+++++ Admin Menu (" << user << ") +++++\n"
                                 " 1) Add Passenger\n"
                                 " 2) Search Passenger\n"
                                 " 3) Update Passenger\n"
                                 " 4) Delete Passenger\n"
                                 " 5) List All Passengers\n"
                                 " 6) Passenger's Trains\n"
                                 " 7) Passengers by Train\n"
                                 " 8) Delete Train\n"
                                 " 0) Logout\n"
                                 "Choice: ";
                    cin >> sub;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (sub) {
                        case 1:
                            Customer::displayArtWork(1);
                            Customer::addNewCustomer();
                            break;
                        case 2: {
                            Customer::displayArtWork(2);
                            Customer::displayCustomersData(false);
                            cout << "Enter CustomerID: ";
                            string id; getline(cin, id);
                            Customer::searchUser(id);
                            break;
                        }
                        case 3: {
                            bookingSystem.displayArtWork(2);
                            Customer::displayCustomersData(false);
                            cout << "Enter CustomerID: ";
                            string id; getline(cin, id);
                            Customer::editUserInfo(id);
                            break;
                        }
                        case 4: {
                            bookingSystem.displayArtWork(3);
                            Customer::displayCustomersData(false);
                            cout << "Enter CustomerID: ";
                            string id; getline(cin, id);
                            Customer::deleteUser(id);
                            break;
                        }
                        case 5:
                            Customer::displayArtWork(3);
                            Customer::displayCustomersData(false);
                            break;
                        case 6: {
                            bookingSystem.displayArtWork(6);
                            Customer::displayCustomersData(false);
                            cout << "Enter CustomerID: ";
                            string id; getline(cin, id);
                            bookingSystem.displayTrainsRegisteredByOneUser(id);
                            break;
                        }
                        case 7: {
                            Customer::displayArtWork(4);
                            cout << "Show all trains? (Y/N): ";
                            char c; cin >> c; cin.ignore();
                            if (c == 'Y' || c == 'y') {
                                bookingSystem.displayRegisteredUsersForAllTrains();
                            } else {
                                Train trainInstance;
                                trainInstance.displayTrainSchedule();
                                cout << "Train#: ";
                                string f; getline(cin, f);
                                bookingSystem.displayRegisteredUsersForASpecificTrain(f);
                            }
                            break;
                        }
                        case 8: {
                            Customer::displayArtWork(5);
                            Train trainInstance;
                            trainInstance.displayTrainSchedule();
                            cout << "Train#: ";
                            string f; getline(cin, f);
                            trainInstance.deleteTrain(f);
                            break;
                        }
                    }
                } while (sub != 0);
            }
        }
        //choice 2 is for registration of a new admin
        else if (choice == 2) {
            printArtWork(2);
            string u, p;
            cout << "New admin username: "; getline(cin, u);
            cout << "Password: "; getline(cin, p);
            auth.registerAdmin(u, p);
        }
        //choice 3 is for login as a passenger
        else if (choice == 3) {
            printArtWork(3);
            string e, p;
            cout << "Email: "; getline(cin, e);
            cout << "Password: "; getline(cin, p);
            string res = auth.isPassengerRegistered(e, p);
            if (res[0] == '1') {
                string uid = res.substr(2);
                int sub;
                cout << "Logged in as user " << e << "\n";
                Train trainInstance;
                do {
                    cout << "\n\t+++++ Passenger Menu (" << e << ") +++++\n"
                             " 1) Book Train\n"
                             " 2) Update Info\n"
                             " 3) Delete Account\n"
                             " 4) View Schedule\n"
                             " 5) Cancel Train Booking\n"
                             " 6) My Bookings\n"
                             " 0) Logout\n"
                             "Choice: ";
                    cin >> sub;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (sub) {
                        case 1: {
                            bookingSystem.displayArtWork(1);
                            trainInstance.displayTrainSchedule();
                            string fn; int n;
                            cout << "Train#: "; getline(cin, fn);
                            cout << "Tickets: "; cin >> n; cin.ignore();
                            bookingSystem.bookTrain(fn, n, uid);
                            break;
                        }
                        case 2:
                            bookingSystem.displayArtWork(2);
                            Customer::editUserInfo(uid);
                            break;
                        case 3: {
                            bookingSystem.displayArtWork(3);
                            cout << "Confirm delete (Y/N): ";
                            char c; cin >> c; cin.ignore();
                            if (c == 'Y' || c == 'y') {
                                Customer::deleteUser(uid);
                                sub = 0;
                            }
                            break;
                        }
                        case 4:
                            bookingSystem.displayArtWork(4);
                            trainInstance.displayTrainSchedule();
                            trainInstance.displayMeasurementInstructions();
                            break;
                        case 5:
                            bookingSystem.displayArtWork(5);
                            bookingSystem.cancelTrain(uid);
                            break;
                        case 6:
                            bookingSystem.displayArtWork(6);
                            bookingSystem.displayTrainsRegisteredByOneUser(uid);
                            break;
                    }
                } while (sub != 0);
            }
            else {
                cout << "ERROR: Invalid passenger credentials.\n";
            }
        }
        //choice 2 is for registration of a new passenger
        else if (choice == 4) {
            printArtWork(4);
            Customer::addNewCustomer();
        }
        //choice 5 is for display of user manual
        else if (choice == 5) {
            manualInstructions();
        }

        displayMainMenu();
        cin >> choice;
    } while (choice != 0);

    return 0;
}

void User::displayMainMenu() {
    cout << "\n(a) 0=Exit   (b) 1=Admin Login   (c) 2=Admin Register\n"
                 "(d) 3=Passenger Login   (e) 4=Passenger Register   (f) 5=User Manual\n"
                 "Choice: ";
}

void User::manualInstructions() {
    printArtWork(5);
    cout << "(1) Admin have full CRUD on customers.\n"
                 "(2) Register admin via option 2 in main menu.\n"
                 "(3) Passenger can book/cancel trains up to 10 tickets.\n"
                 "(4) Use ±1h buffer on arrival times.\n"
                 "...(see on-screen prompts for details)\n";
}

void User::welcomeScreen(int option) {
    if (option == 1)
        cout << "Welcome to BAV Railways\n";
    else
        cout << "Traveling With Trust For Five Decades\n";
}

void User::printArtWork(int option) {
    static const vector<string> arts = {
        "", "ADMIN LOGIN", "ADMIN REGISTER", "PASSENGER LOGIN",
        "PASSENGER REGISTER", "USER MANUAL"
    };
    if (option >= 1 && option < static_cast<int>(arts.size())) {
        cout << "\n=== " << arts[option] << " ===\n";
    }
}

// Define the global main
int main() {
    return User::run();
}
