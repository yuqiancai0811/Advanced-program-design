#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <algorithm>  // Needed for std::remove

using namespace std; 

// Default constructor -> update to initialize playerOrders pointer
Player::Player() 
    : name("Unnamed Player"), playerHand(Hand()), playerOrders(new orderList()), numberOfReinforcement(0), negotiate(false) {}

// Parameterized constructor
Player::Player(const string& name) 
    : name(name), playerHand(Hand()), playerOrders(new orderList()), numberOfReinforcement(0), negotiate(false) {}

// Copy constructor (deep copy)
Player::Player(const Player& other) {
    name = other.name;  
    ownedTerritories = other.ownedTerritories;  
    playerHand = other.playerHand;  
    playerOrders = new orderList(*other.playerOrders);  // Deep copy the orderList
    numberOfReinforcement = other.numberOfReinforcement; 
    negotiate = other.negotiate;  
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;  
        ownedTerritories = other.ownedTerritories;  
        playerHand = other.playerHand; 
        delete playerOrders;  // Delete existing orderList to avoid memory leak
        playerOrders = new orderList(*other.playerOrders);  // Deep copy new orderList
        numberOfReinforcement = other.numberOfReinforcement; 
        negotiate = other.negotiate;  
    }
    return *this;
}

// Destructor
Player::~Player() {
    delete playerOrders;  // Free allocated memory for playerOrders
}

/* ------------ Setter and getter for negotiate used in P4 -----------*/
void Player::setNegotiate(bool status) {
    negotiate = status;
}

bool Player::isNegotiating() const {
    return negotiate;
}

/* ---------------- Getters ----------------------*/
string Player::getName() const {
    return name;  // Return player's name
}

vector<Territory*> Player::getOwnedTerritories() const {
    return ownedTerritories;  // Return owned territories
}

Hand& Player::getHand() {
    return playerHand;  // Return reference to player's hand
}

// Getter for numberOfReinforcement
int Player::getNumberOfReinforcement() const {
    return numberOfReinforcement;
}

// Get the list of issued orders
orderList& Player::getOrders() {
    return *playerOrders;  // Dereference the pointer to return the actual object
}

/* ---------------- Setters ----------------------*/
void Player::setName(const string& name) {
    this->name = name;  // Set player's name
}

void Player::addTerritory(Territory* territory) {
    ownedTerritories.push_back(territory);  // Add territory to player's owned territories
}

void Player::removeTerritory(Territory* territory) {
    ownedTerritories.erase(remove(ownedTerritories.begin(), ownedTerritories.end(), territory), ownedTerritories.end());
}

// Setter for numberOfReinforcement
void Player::setNumberOfReinforcement(int number) {
    numberOfReinforcement = number;
}

/*-------------------------------------------*/
// Method to decide where to defend
vector<Territory*> Player::toDefend() const {
    return ownedTerritories;  
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

/*--------------------------- Update methods for A2_Part3 ------------------------*/
void Player::issueOrder() {
    // Step 1: Deploy Reinforcements if available
    std::cout << "[LOG] " << name << " attempting to deploy reinforcements...\n";
    if (numberOfReinforcement > 0) {
        std::vector<Territory*> defendList = toDefend();
        if (!defendList.empty()) {
            Territory* territoryToDefend = defendList.front();
            int unitsToDeploy = std::min(numberOfReinforcement, 5); // Deploy up to 5 units at a time
            numberOfReinforcement -= unitsToDeploy;

            // Create and add deploy order
            Order* deployOrder = new ::deployOrder(unitsToDeploy, territoryToDefend, this);
            playerOrders->addOrder(deployOrder);
            std::cout << name << " issues a Deploy Order to " << territoryToDefend->getName()
                      << " with " << unitsToDeploy << " units.\n";
            return;
        }
    }

    // Step 2: Advance Orders for Defense
    std::cout << "[LOG] " << name << " attempting to issue advance orders for defense...\n";
    std::vector<Territory*> defendList = toDefend();
    for (Territory* defendTerritory : defendList) {
        for (Territory* sourceTerritory : ownedTerritories) {
            if (sourceTerritory != defendTerritory && sourceTerritory->getArmies() > 1) {
                // Issue an advance order to move troops to defend territory
                Order* advanceOrder = new ::advanceOrder(1, sourceTerritory, defendTerritory, this);
                playerOrders->addOrder(advanceOrder);
                std::cout << name << " issues an Advance Order to defend " << defendTerritory->getName()
                          << " from " << sourceTerritory->getName() << ".\n";
                return;
            }
        }
    }

    // Step 3: Advance Orders for Attack
    std::cout << "[LOG] " << name << " attempting to issue advance orders for attack...\n";
    std::vector<Territory*> attackList = toAttack();
    for (Territory* attackTerritory : attackList) {
        for (Territory* sourceTerritory : ownedTerritories) {
            if (sourceTerritory->getArmies() > 1 && 
                std::find(sourceTerritory->getAdjacentTerritories().begin(), 
                          sourceTerritory->getAdjacentTerritories().end(), 
                          attackTerritory) != sourceTerritory->getAdjacentTerritories().end()) {
                
                // Issue an advance order to move troops to attack
                Order* advanceOrder = new ::advanceOrder(1, sourceTerritory, attackTerritory, this);
                playerOrders->addOrder(advanceOrder);
                std::cout << name << " issues an Advance Order to attack " << attackTerritory->getName()
                          << " from " << sourceTerritory->getName() << ".\n";
                return;
            }
        }
    }

    // Step 4: Use Cards to Issue Orders
    std::cout << "[LOG] " << name << " attempting to use a card to issue an order...\n";
    if (!playerHand.getHand().empty()) {
        Card* card = playerHand.getHand().front();
        playerHand.removeCard(*card); // Remove the card from hand after using
        Order* specialOrder = nullptr;

    if (card->getType() == "Bomb") {
        if (!attackList.empty()) {
            specialOrder = new bombOrder(attackList.front(), this);
            std::cout << name << " issues a Bomb Order on " << attackList.front()->getName() << ".\n";
        }
    } else if (card->getType() == "Airlift") {
        if (!ownedTerritories.empty() && !defendList.empty()) {
            specialOrder = new airliftOrder(5, ownedTerritories.front(), defendList.front(), this);
            std::cout << name << " issues an Airlift Order to move armies to " << defendList.front()->getName() << ".\n";
        }
    } else if (card->getType() == "Blockade") {
        if (!defendList.empty()) {
            Player* neutralPlayer = new Player("Neutral");  // Create a neutral player instance
            specialOrder = new blockadeOrder(5, this, neutralPlayer, defendList.front());
            std::cout << name << " issues a Blockade Order on " << defendList.front()->getName() 
                      << " with " << neutralPlayer->getName() << " as the neutral player.\n";
        }
    } else if (card->getType() == "Diplomacy") {
        // Select an enemy player from the attackList if available
        Player* enemyPlayer = nullptr;
        if (!attackList.empty()) {
            enemyPlayer = attackList.front()->getOwnerPlayer();  // Assuming getOwnerPlayer() returns the player who owns the territory
        }

        if (enemyPlayer && enemyPlayer != this) {  // Ensure we have a valid enemy player
            specialOrder = new negotiateOrder(this, enemyPlayer);
            std::cout << name << " issues a Diplomacy Order with " << enemyPlayer->getName() << ".\n";
        }
    }

        if (specialOrder) {
            playerOrders->addOrder(specialOrder);
            std::cout << name << " issues a " << card->getType() << " Order using a card.\n";
        }
        delete card; // Clean up used card
    }
}



// Checks if the player has more orders to issue this turn
bool Player::hasMoreOrders() const {
    // Check if there are still reinforcement units available to deploy
    if (numberOfReinforcement > 0) {
        return true;
    }

    // Check if there are territories to defend (i.e., potential advance orders for defense)
    if (!toDefend().empty()) {
        return true;
    }

    // Check if there are territories to attack (i.e., potential advance orders for attack)
    if (!toAttack().empty()) {
        return true;
    }

    // Check if there are any cards in hand (i.e., potential special orders)
    if (!playerHand.getHand().empty()) {
        return true;
    }

    // If none of the conditions are met, return false
    return false;
}
/*--------------------------- End of Update methods for A2_Part3 ------------------------*/


// Stream insertion operator
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player: " << player.name << "\n";
    os << "Owned Territories: ";
    for (Territory* territory : player.ownedTerritories) {
        os << territory->getName() << " ";
    }
    os << "\nOrders: \n";
    player.playerOrders->showAllOrders();
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
    playerOrders->showAllOrders();  // Show all the orders in the orderList
}
