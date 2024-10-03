#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Map.h"    // For Territory class
#include "Orders.h" // For Order and orderList classes
#include "Cards.h"  // For Hand, Deck, and Card classes

using namespace std;

class Player {
private:
    string name;  // Player's name
    vector<Territory*> ownedTerritories;  // List of territories owned by the player
    Hand playerHand;  // The player's hand of cards
    orderList playerOrders;  // The player's issued orders

public:
    // Default constructor
    Player();

    // Parameterized constructor
    Player(const string& name);

    // Copy constructor
    Player(const Player& other);

    // Assignment operator
    Player& operator=(const Player& other);

    // Destructor
    ~Player();

    // Getters
    std::string getName() const;
    std::vector<Territory*> getOwnedTerritories() const;
    Hand& getHand();  // Return reference to player's hand of cards

    // Setters
    void setName(const std::string& name);
    void addTerritory(Territory* territory);  // Add a territory to the player's control
    void removeTerritory(Territory* territory);  // Remove a territory from player's control

    // Methods to decide where to defend and attack
    std::vector<Territory*> toDefend() const;  // Returns a list of territories the player will defend
    std::vector<Territory*> toAttack() const;  // Returns a list of territories the player will attack

    // Methods for issuing orders
    void issueOrder(Order* order);  // Issues an order and adds it to the player's order list
    orderList& getOrders();  // Returns the list of orders issued by the player


    // Stream insertion operator
    friend ostream& operator<<(ostream& os, const Player& player);

    // Print Player's info
    void printPlayerInfo() const;
};

#endif // PLAYER_H
