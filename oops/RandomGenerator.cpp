#include "RandomGenerator.hpp"
#include <chrono>
using namespace std;

//Static data
//List of some destinations along with latitude and longitude
const vector<RandomGenerator::Destination> RandomGenerator::DESTINATIONS = {
    {"New Delhi", "28.6139", "77.2090"},
    {"Mumbai", "19.0760", "72.8777"},
    {"Chennai", "13.0827", "80.2707"},
    {"Kolkata", "22.5726", "88.3639"},
    {"Bengaluru", "12.9716", "77.5946"},
    {"Hyderabad", "17.3850", "78.4867"},
    {"Pune", "18.5204", "73.8567"},
    {"Ahmedabad", "23.0225", "72.5714"},
    {"Jaipur", "26.9124", "75.7873"},
    {"Lucknow", "26.8467", "80.9462"},
    {"Bhopal", "23.2599", "77.4126"},
    {"Patna", "25.5941", "85.1376"},
    {"Nagpur", "21.1458", "79.0882"},
    {"Chandigarh", "30.7333", "76.7794"},
    {"Indore", "22.7196", "75.8577"},
    {"Kanpur", "26.4499", "80.3319"},
    {"Surat", "21.1702", "72.8311"},
    {"Ranchi", "23.3441", "85.3096"},
    {"Thiruvananthapuram", "8.5241", "76.9366"},
    {"Guwahati", "26.1445", "91.7362"}
};

//constructor definition
RandomGenerator::RandomGenerator()
  : mt_(static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count())),
    distID_(0, 999999),
    distSeats_(0, 499),
    distLetter_(0, 25),
    distDest_(0, DESTINATIONS.size() - 1)
{}

//method for generation of user ID
void RandomGenerator::randomIDGen() {
    int id;
    do {
        id = distID_(mt_);
    } while (id < 20000);
    randomNum_ = to_string(id);
}

//method to return the last userID generated
const string& RandomGenerator::getRandomNumber() const {
    return randomNum_;
}

//method to pick two random places (from city, to city)
array<RandomGenerator::Destination, 2> RandomGenerator::randomDestinations() {
    size_t i = distDest_(mt_);
    size_t j;
    do {
        j = distDest_(mt_);
    } while (j == i);
    return { DESTINATIONS[i], DESTINATIONS[j] };
}

//method to generate number of seats in a train
int RandomGenerator::randomNumOfSeats() {
    int seats;
    do {
        seats = distSeats_(mt_);
    } while (seats < 75);
    return seats;
}

//method to generate train number
string RandomGenerator::randomTrainNumberGen(int lettersCount, int divisor) {
    string s;
    s.reserve(lettersCount + 5);
    for (int k = 0; k < lettersCount; ++k) {
        // Use uppercase letters for train numbers
        char c = static_cast<char>('A' + distLetter_(mt_));
        s.push_back(c);
    }
    s.push_back('-');
    int seats = randomNumOfSeats();
    s += to_string(seats / (divisor == 0 ? 1 : divisor));
    return s;
}
