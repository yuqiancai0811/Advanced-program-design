#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Map.h"    // For Territory class
#include "Orders.h" // For Order and orderList classes
#include "Cards.h"  // For Hand, Deck, and Card classes


class Player {
private:
    std::string name;  // Player's name
    std::vector<Territory*> ownedTerritories;  // Player owns a collection of territories
    Hand playerHand;  // Player owns a hand of Warzone cards
    orderList playerOrders;  // List of orders issued by the player
    int numberOfReinforcement; //*Variable for reinforcement units
    bool negotiate;  // Boolean flag for negotiation status

public:
    // Default constructor
    Player();

    // Parameterized constructor
    Player(const std::string& name);

    // Copy constructor
    Player(const Player& other);

    // Assignment operator
    Player& operator=(const Player& other);

    // Destructor
    ~Player();

    // Setter and getter for negotiation status
    void setNegotiate(bool status);
    bool isNegotiating() const;

    // Getters
    std::string getName() const;
    std::vector<Territory*> getOwnedTerritories() const;
    Hand& getHand();  // Return reference to player's hand of cards
    int getNumberOfReinforcement() const; // *Getter for reinforcement units

    // Setters
    void setName(const std::string& name);
    void addTerritory(Territory* territory);  // Add a territory to the player's control
    void removeTerritory(Territory* territory);  // Remove a territory from player's control
    void setNumberOfReinforcement(int number); // *Setter for reinforcement units

    // Checks if the player has more orders to issue in this turn
    bool hasMoreOrders() const;

    // Methods to decide where to defend and attack
    std::vector<Territory*> toDefend() const;  // Returns a list of territories the player will defend
    std::vector<Territory*> toAttack() const;  // Returns a list of territories the player will attack

    // Method for issuing orders (creates an order internally)
    void issueOrder();

    // Get the list of issued orders
    orderList& getOrders();

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    // Print Player's info
    void printPlayerInfo() const;
};

#endif // PLAYER_H
