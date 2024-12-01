#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Map.h"    // For Territory class
#include "Orders.h" // For Order and orderList classes
#include "Cards.h"  // For Hand, Deck, and Card classes
#include "PlayerStrategy.h"


class orderList; // Forward declaration for orderList

class Player {
private:
    std::string name;  // Player's name
    std::vector<Territory*> ownedTerritories;  // Player owns a collection of territories
    Hand playerHand;  // Player owns a hand of Warzone cards
    orderList* playerOrders;  // Pointer to list of orders issued by the player
    int numberOfReinforcement; //*Variable for reinforcement units
    bool negotiate;  // Boolean flag for negotiation status
    PlayerStrategy* strategy;
    std::string negoPartnerName;

public:
    // Default constructor
    Player();
    
    // New Constructor
    Player(const std::string& name, const std::string& strategyType);
    
    // Parameterized constructor
    Player(const std::string& name);

    // Copy constructor
    Player(const Player& other);

    // Assignment operator
    Player& operator=(const Player& other);

    // Destructor
    ~Player();

    void setNegoPartnerName(const std::string& name);
    std::string getNegoPartnerName() const;

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
    orderList& getOrders();  // Return a reference instead of a pointer

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    // Print Player's info
    void printPlayerInfo() const;

    //--------------------Add free function for A3_Part1 Neutral Player -----------------//
    void setStrategy(PlayerStrategy* newStrategy);
    PlayerStrategy* getStrategy() const; 
    void handleAttack();
};

#endif // PLAYER_H
