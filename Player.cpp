#include "Player.h"
#include <iostream>
#include <algorithm>  // Needed for std::remove

using namespace std;  // Allows using standard library names without std:: prefix

// Default constructor
Player::Player() : name("Unnamed Player"), playerHand(Hand()), playerOrders(orderList()) {}

// Parameterized constructor
Player::Player(const string& name) : name(name), playerHand(Hand()), playerOrders(orderList()) {}

// Copy constructor (deep copy)
Player::Player(const Player& other) {
    name = other.name;  // Copy player's name
    ownedTerritories = other.ownedTerritories;  // Copy owned territories
    playerHand = other.playerHand;  // Copy player's hand
    playerOrders = other.playerOrders;  // Copy player's orders
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;  // Copy player's name
        ownedTerritories = other.ownedTerritories;  // Copy owned territories
        playerHand = other.playerHand;  // Copy player's hand
        playerOrders = other.playerOrders;  // Copy player's orders
    }
    return *this;
}

Player::~Player() {
    // No need to delete orders, as orderList's destructor handles it
}

// Getters
string Player::getName() const {
    return name;  // Return player's name
}

vector<Territory*> Player::getOwnedTerritories() const {
    return ownedTerritories;  // Return owned territories
}

Hand& Player::getHand() {
    return playerHand;  // Return reference to player's hand
}

// Setters
void Player::setName(const string& name) {
    this->name = name;  // Set player's name
}

void Player::addTerritory(Territory* territory) {
    ownedTerritories.push_back(territory);  // Add territory to player's owned territories
}

void Player::removeTerritory(Territory* territory) {
    ownedTerritories.erase(remove(ownedTerritories.begin(), ownedTerritories.end(), territory), ownedTerritories.end());
}

// Method to decide where to defend
vector<Territory*> Player::toDefend() const {
    return ownedTerritories;  // Return the player's owned territories
}

// Method to decide where to attack
vector<Territory*> Player::toAttack() const {
    vector<Territory*> attackTargets;

    // Arbitrarily choose adjacent territories for attack
    for (Territory* territory : ownedTerritories) {
        for (Territory* adj : territory->getAdjacentTerritories()) {
            attackTargets.push_back(adj);
        }
    }
    return attackTargets;
}

// Method to issue orders (creates a new order internally)
void Player::issueOrder() {
    // Create a new Order
    Order* newOrder = new deployOrder();  
    playerOrders.addOrder(newOrder);  // Add the created order to the player's orders list
}

// Get the list of issued orders
orderList& Player::getOrders() {
    return playerOrders;  // Return the player's orders
}

// Stream insertion operator
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player: " << player.name << "\n";
    os << "Owned Territories: ";
    for (Territory* territory : player.ownedTerritories) {
        os << territory->getName() << " ";
    }
    os << "\nOrders: \n";
    player.playerOrders.showAllOrders();
    return os;
}

// Print Player info
void Player::printPlayerInfo() const {
    cout << "Player: " << name << "\n";
    cout << "Owned Territories: \n";
    for (Territory* territory : ownedTerritories) {
        territory->printTerritoryInfo();
    }

    cout << "Orders: \n";
    playerOrders.showAllOrders();  // Show all the orders in the orderList
}
