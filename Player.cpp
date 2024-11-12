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
    vector<Territory*> defendList;
    for (Territory* territory : ownedTerritories) {
        if (territory->getArmies() < 5 || 
            std::any_of(territory->getAdjacentTerritories().begin(), 
                        territory->getAdjacentTerritories().end(),
                        [this](Territory* adj) { return adj->getOwner() != this->getName(); })) {
            defendList.push_back(territory);
        }
    }
    // Sort by armies to prioritize defending territories with fewer armies
    std::sort(defendList.begin(), defendList.end(), [](Territory* a, Territory* b) {
        return a->getArmies() < b->getArmies();
    });
    return defendList;
}



// Method to decide where to attack
vector<Territory*> Player::toAttack() const {
    vector<Territory*> attackTargets;
    for (Territory* territory : ownedTerritories) {
        for (Territory* adj : territory->getAdjacentTerritories()) {
            if (adj->getOwner() != this->getName()) {
                attackTargets.push_back(adj);
            }
        }
    }
    // Sort by armies to prioritize attacking territories with fewer armies
    std::sort(attackTargets.begin(), attackTargets.end(), [](Territory* a, Territory* b) {
        return a->getArmies() < b->getArmies();
    });
    return attackTargets;
}

/*--------------------------- Update methods for A2_Part3 ------------------------*/
void Player::issueOrder() {
    // Step 1: Deploy Reinforcements if available, with a limit per round
    if (numberOfReinforcement > 0) {
        std::cout << "[LOG] " << name << " attempting to deploy reinforcements...\n";
        std::vector<Territory*> defendList = toDefend();
        if (!defendList.empty()) {
            Territory* territoryToDefend = defendList.front();
            int unitsToDeploy = std::min(numberOfReinforcement, 5); // Limit to deploying 5 units at a time
            numberOfReinforcement -= unitsToDeploy;

            // Add deploy order
            Order* deployOrder = new :: deployOrder(unitsToDeploy, territoryToDefend, this);
            playerOrders->addOrder(deployOrder);
            std::cout << "[INFO] " << name << " issues a Deploy Order to " 
                      << territoryToDefend->getName() << " with " 
                      << unitsToDeploy << " units. Remaining reinforcements: " 
                      << numberOfReinforcement << "\n";
            return;
        } else {
            std::cout << "[WARN] " << name << " has reinforcements but no territory to defend.\n";
        }
    }

    // Step 2: Issue Advance Orders for Defense
    if (!toDefend().empty()) {
        std::cout << "[LOG] " << name << " attempting to issue Advance Order for defense...\n";
        std::vector<Territory*> defendList = toDefend();
        for (Territory* defendTerritory : defendList) {
            for (Territory* sourceTerritory : ownedTerritories) {
                if (sourceTerritory != defendTerritory && sourceTerritory->getArmies() > 1) {
                    Order* advanceOrder = new :: advanceOrder(1, sourceTerritory, defendTerritory, this);
                    playerOrders->addOrder(advanceOrder);
                    std::cout << "[INFO] " << name << " issues an Advance Order to defend " 
                              << defendTerritory->getName() << " from " 
                              << sourceTerritory->getName() << ".\n";
                    return;
                }
            }
        }
    }

    // Step 3: Issue Advance Orders for Attack
    if (!toAttack().empty()) {
        std::cout << "[LOG] " << name << " attempting to issue Advance Order for attack...\n";
        std::vector<Territory*> attackList = toAttack();
        for (Territory* attackTerritory : attackList) {
            for (Territory* sourceTerritory : ownedTerritories) {
                if (sourceTerritory->getArmies() > 1 && 
                    std::find(sourceTerritory->getAdjacentTerritories().begin(), 
                              sourceTerritory->getAdjacentTerritories().end(), 
                              attackTerritory) != sourceTerritory->getAdjacentTerritories().end()) {
                    Order* advanceOrder = new :: advanceOrder(1, sourceTerritory, attackTerritory, this);
                    playerOrders->addOrder(advanceOrder);
                    std::cout << "[INFO] " << name << " issues an Advance Order to attack " 
                              << attackTerritory->getName() << " from " 
                              << sourceTerritory->getName() << ".\n";
                    return;
                }
            }
        }
    }

    // Step 4: Use Cards to Issue Special Orders when no reinforcements are left
    if (numberOfReinforcement == 0 && !playerHand.getHand().empty()) {
        Card* card = playerHand.getHand().front();
        playerHand.getHand().erase(playerHand.getHand().begin());  // Manually remove card from hand
        Order* specialOrder = nullptr;

        // Handle different card types
        if (card->getType() == "Bomb") {
            if (!toAttack().empty()) {
                specialOrder = new bombOrder(toAttack().front(), this);
                std::cout << "[INFO] " << name << " issues a Bomb Order.\n";
            }
        } else if (card->getType() == "Airlift") {
            if (!ownedTerritories.empty() && !toDefend().empty()) {
                specialOrder = new airliftOrder(5, ownedTerritories.front(), toDefend().front(), this);
                std::cout << "[INFO] " << name << " issues an Airlift Order.\n";
            }
        } else if (card->getType() == "Blockade") {
            if (!toDefend().empty()) {
                Player* neutralPlayer = new Player("Neutral");
                specialOrder = new blockadeOrder(5, this, neutralPlayer, toDefend().front());
                std::cout << "[INFO] " << name << " issues a Blockade Order.\n";
            }
        } else if (card->getType() == "Diplomacy") {
            Player* enemyPlayer = !toAttack().empty() ? toAttack().front()->getOwnerPlayer() : nullptr;
            if (enemyPlayer && enemyPlayer != this) {
                specialOrder = new negotiateOrder(this, enemyPlayer);
                std::cout << "[INFO] " << name << " issues a Diplomacy Order with " 
                          << enemyPlayer->getName() << ".\n";
            }
        }

        if (specialOrder) {
            playerOrders->addOrder(specialOrder);
            std::cout << "[INFO] " << name << " successfully issued a " << card->getType() 
                      << " Order using a card.\n";
        }

        // Clean up used card
        delete card;
    }
}


/*------------------------------------------------------*/

bool Player::hasMoreOrders() const {
    bool hasReinforcements = (numberOfReinforcement > 0);
    bool hasDefendTargets = !toDefend().empty() && hasReinforcements;
    bool hasAttackTargets = !toAttack().empty() && hasReinforcements;
    bool hasCards = !playerHand.getHand().empty();

    // Debugging output to track each condition
    std::cout << "[DEBUG] Checking hasMoreOrders for " << name << ":\n";
    std::cout << " - Reinforcements available: " << hasReinforcements << " (Remaining: " << numberOfReinforcement << ")\n";
    std::cout << " - Defend targets available: " << hasDefendTargets << "\n";
    std::cout << " - Attack targets available: " << hasAttackTargets << "\n";
    std::cout << " - Cards available: " << hasCards << "\n";

    // Only return true if there's a valid reason to issue an order
    return hasReinforcements || hasDefendTargets || hasAttackTargets || hasCards;
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
