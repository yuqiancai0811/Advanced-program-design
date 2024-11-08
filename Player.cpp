#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <algorithm>  // Needed for std::remove

using namespace std; 

// Default constructor -> update to include #ofReinforcement
Player::Player() : name("Unnamed Player"), playerHand(Hand()), playerOrders(orderList()), numberOfReinforcement(0) {}

// Parameterized constructor
Player::Player(const string& name) : name(name), playerHand(Hand()), playerOrders(orderList()), numberOfReinforcement(0) {}

// Copy constructor (deep copy)
Player::Player(const Player& other) {
    name = other.name;  
    ownedTerritories = other.ownedTerritories;  
    playerHand = other.playerHand;  
    playerOrders = other.playerOrders;  
    numberOfReinforcement = other.numberOfReinforcement; // Copy reinforcement units
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;  
        ownedTerritories = other.ownedTerritories;  
        playerHand = other.playerHand; 
        playerOrders = other.playerOrders;  
        numberOfReinforcement = other.numberOfReinforcement; // Copy reinforcement units
    }
    return *this;
}

Player::~Player() {
    // When an object of Player is destroyed, the destructor for each member object 
    // (like Hand and orderList) is automatically called 
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
    if (numberOfReinforcement > 0) {
        vector<Territory*> defendList = toDefend();
        if (!defendList.empty()) {
            Territory* territoryToDefend = defendList.front();
            int unitsToDeploy = min(numberOfReinforcement, 5);  // Deploy up to 5 units
            numberOfReinforcement -= unitsToDeploy;

            // Create and add deploy order
            Order* deployOrder = new deployOrder();  // Ensure deployOrder is valid
            playerOrders.addOrder(deployOrder);

            cout << name << " issues a Deploy Order to " << territoryToDefend->getName()
                 << " with " << unitsToDeploy << " units.\n";
            return;
        }
    }

    // Step 2: Advance Units for Defense
    vector<Territory*> defendList = toDefend();
    for (Territory* defendTerritory : defendList) {
        for (Territory* sourceTerritory : ownedTerritories) {
            if (sourceTerritory != defendTerritory && sourceTerritory->getArmies() > 1) {
                Order* advanceOrder = new advanceOrder();  // Ensure advanceOrder is valid
                playerOrders.addOrder(advanceOrder);

                cout << name << " issues an Advance Order to defend " << defendTerritory->getName()
                     << " from " << sourceTerritory->getName() << ".\n";
                return;
            }
        }
    }

    // Step 3: Advance Units for Attack
    vector<Territory*> attackList = toAttack();
    for (Territory* attackTerritory : attackList) {
        for (Territory* sourceTerritory : ownedTerritories) {
            if (find(sourceTerritory->getAdjacentTerritories().begin(),
                     sourceTerritory->getAdjacentTerritories().end(),
                     attackTerritory) != sourceTerritory->getAdjacentTerritories().end() &&
                sourceTerritory->getArmies() > 1) {
                Order* advanceOrder = new advanceOrder();  // Ensure advanceOrder is valid
                playerOrders.addOrder(advanceOrder);

                cout << name << " issues an Advance Order to attack " << attackTerritory->getName()
                     << " from " << sourceTerritory->getName() << ".\n";
                return;
            }
        }
    }

    // Step 4: Use Cards for Orders
    if (!playerHand.getHand().empty()) {
        Card* card = playerHand.getHand().front();
        playerHand.removeCard(*card);
        Order* specialOrder = nullptr;

        if (card->getType() == "Bomb") {
            specialOrder = new bombOrder();
        } else if (card->getType() == "Airlift") {
            specialOrder = new airliftOrder();
        } else if (card->getType() == "Blockade") {
            specialOrder = new blockadeOrder();
        } else if (card->getType() == "Diplomacy") {
            specialOrder = new negotiateOrder();
        }

        if (specialOrder) {
            playerOrders.addOrder(specialOrder);
            cout << name << " issues a " << card->getType() << " Order using a card.\n";
        }

        delete card;  // Cleanup used card
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
