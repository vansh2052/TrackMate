#ifndef USER_HPP
#define USER_HPP

#include <string>

// Forward declarations to break cycles
class RolesAndPermissions;
class Train;
class TrainReservation;
class Customer;

class User {
public:
    /// Entrypoint: runs the entire console application for the train reservation system.
    static int run();

    /// Prints the top‐level menu for the train system.
    static void displayMainMenu();

    /// Prints the user manual/instructions for train booking and management.
    static void manualInstructions();

    /// A small welcome banner for the train reservation system.
    static void welcomeScreen(int option);

    /// Prints contextual ASCII banners (e.g. “ADMIN LOGIN”).
    static void printArtWork(int option);

protected:
    User() = default;  // allow derived classes to construct
};

#endif // USER_HPP
