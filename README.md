
# Train Reservation System – C++ OOP Project

A terminal-based application to simulate a simple railway booking system using **Object-Oriented Programming (OOP)** principles in C++. The project models real-world functionalities such as train generation, admin and passenger roles, booking/cancellation, and authentication.

---

## File Structure

```
 TrainReservationSystem/
 ├── Customer.hpp / Customer.cpp
 ├── Train.hpp / Train.cpp
 ├── TrainDistance.hpp / TrainDistance.cpp
 ├── TrainReservation.hpp / TrainReservation.cpp
 ├── RolesAndPermissions.hpp / RolesAndPermissions.cpp
 ├── RandomGenerator.hpp / RandomGenerator.cpp
 ├── User.hpp / User.cpp          ← contains main()
 └── DisplayClass.hpp             (abstract base class)
```

---

## Class Overview

| Class                   | Type           | Description                                                                 |
|------------------------|----------------|-----------------------------------------------------------------------------|
| `TrainDistance`         | Abstract Base  | Provides distance calculation methods & instructions.                       |
| `Train`                 | Core           | Manages train properties, schedule, and passenger list.                     |
| `Customer`              | Core           | Represents a registered passenger. Stores bookings and contact info.       |
| `TrainReservation`      | Core Controller| Manages booking, cancellation, and ticket records.                          |
| `RolesAndPermissions`   | Utility        | Handles authentication and registration for admins & passengers.           |
| `RandomGenerator`       | Utility        | Generates random train numbers, stations, seats, platforms, etc.           |
| `User`                  | Controller     | Main entrypoint & menu system. Contains UI interaction logic.              |
| `DisplayClass`          | Abstract Base  | Provides structured formatting and display override interface.             |

---

##  Compilation Instructions

>  Requirement: A C++17-compatible compiler (e.g., GCC 7+, Clang 6+, MSVC 2017+)

###  Step 1: Compile

For Linux/macOS/WSL:

```bash
g++ -std=c++17 -Wall -O2 *.cpp -o train_app
```

For Windows (MinGW):

```bash
g++ -std=c++17 -static -static-libstdc++ -static-libgcc -O2 *.cpp -o train_app.exe
```

---

### Step 2: Run

```bash
# On Linux/macOS:
./train_app

# On Windows:
train_app.exe
```

--

###  Default Credentials

| Role    | Username | Password |
|---------|----------|----------|
| Admin   | Standard | 1234     |

---

## Program Workflow

1. User runs the program → `User::run()` is invoked.
2. `RandomGenerator` creates train data.
3. Main Menu:
   - Admin Login/Register
   - Passenger Login/Register
   - View User Manual
4. Admin can manage passenger records and train data.
5. Passenger can book, cancel, or view their train bookings.

---
