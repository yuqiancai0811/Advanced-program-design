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
std::vector<Territory*> Player::toDefend() const {
    std::vector<Territory*> defendList;

    
    for (Territory* territory : ownedTerritories) {
        if (territory == nullptr) {
            std::cerr << "Error: Null territory pointer in ownedTerritories!" << std::endl;
            continue; // 
        }

        
        if (territory->getArmies() < 5) {
            defendList.push_back(territory);
            continue; // 
        }

        
        bool hasEnemyAdjacent = false; 
        for (Territory* adj : territory->getAdjacentTerritories()) {
            if (adj == nullptr) {
                std::cerr << "Error: Null pointer in adjacent territories!" << std::endl;
                continue; 
            }

            if (adj->getOwner() != this->getName()) {
                hasEnemyAdjacent = true;
                break; 
            }
        }

        if (hasEnemyAdjacent) {
            defendList.push_back(territory);
        }
    }

    // std::sort(defendList.begin(), defendList.end(), [](Territory* a, Territory* b) {
    //     return a->getArmies() < b->getArmies();
    // });
    return defendList;
}



// Method to decide where to attack
vector<Territory*> Player::toAttack() const {
    vector<Territory*> attackTargets;
    for (Territory* territory : ownedTerritories) {
         if (!territory) continue;  // Skip if the territory is a nullptr

        for (Territory* adj : territory->getAdjacentTerritories()) {
           if (!adj) continue; // Skip if the territory is a nullptr

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
/*--------------------------- Update methods for A2_Part3 ------------------------*/
void Player::issueOrder() {
    // Step 1: Deploy Reinforcements if available, with a limit per round
    

    // Step 2: Issue Advance Orders for Defense
    std::vector<Territory*> defendList = toDefend();
    if (!defendList.empty()) {
    std::cout << "[LOG] " << name << " attempting to issue Advance Order for defense...\n";
    int numberOfReinforcement2 = this->getNumberOfReinforcement();

    for (Territory* defendTerritory : defendList) {

        
        if (numberOfReinforcement2 > 15) {
            std::cout << "[LOG] " << name << " attempting to deploy reinforcements...\n";
            std::vector<Territory*> defendList = toDefend();

        
            
                
            if (numberOfReinforcement2 > 15){

                        
                int unitsToDeploy = std::min(numberOfReinforcement2, 5); // Limit to deploying 5 units at a time
                numberOfReinforcement2 -= unitsToDeploy;

                // Add deploy order
                Order* deployOrder = new :: deployOrder(unitsToDeploy, defendTerritory, this);
                playerOrders->addOrder(deployOrder);
                std::cout << "[INFO] " << name << " issues a Deploy Order to " 
                << defendTerritory->getName() << " with " 
                << unitsToDeploy << " units. Remaining reinforcements: " 
                << numberOfReinforcement2 << "\n";
                continue;
                        
            }         
        }


        for (Territory* sourceTerritory : this->ownedTerritories) {
            // Check if the pointers are valid before accessing their properties
            if (sourceTerritory == nullptr || defendTerritory == nullptr) {
                std::cerr << "[ERROR] Null pointer detected for source or defend territory.\n";
                continue;
            }

            if (sourceTerritory->getArmies() ==0)
            {
                continue;
            }
            

            // Get the list of adjacent territory names for sourceTerritory
            std::vector<std::string> adjacentTerritories = sourceTerritory->getAdjacentTerritoryNames();
            std::string defendTerritoryName = defendTerritory->getName();

            // Check if defendTerritory is adjacent to sourceTerritory
            bool isAdjacent = false;

            for (const std::string& adjName : adjacentTerritories) {
                if (adjName == defendTerritoryName) {
                    isAdjacent = true;
                    break;
                }
            }

            // If the territories are adjacent, create the Advance Order
            if (isAdjacent && (sourceTerritory->getArmies()>10 )) {
                Order* advanceOrder = new ::advanceOrder(10, sourceTerritory, defendTerritory, this);
                playerOrders->addOrder(advanceOrder);
                break;
            } 
        }
        }
    }


    // Step 3: Issue Advance Orders for Attack
    std::vector<Territory*> attackList = toAttack();
    if (!attackList.empty()) {
    std::cout << "[LOG] " << name << " attempting to issue Advance Order for attack...\n";
    std::cout << "[DEBUG] Attack list size: " << attackList.size() << std::endl;

    for (Territory* attackTerritory : attackList) {
        for (Territory* sourceTerritory : ownedTerritories) {
            // Check if the pointers are valid before accessing their properties
            if (sourceTerritory == nullptr || attackTerritory == nullptr) {
                std::cerr << "[ERROR] Null pointer detected for source or attack territory.\n";
                continue;
            }

            if (sourceTerritory->getArmies() <5)
            {
                continue;
            }

            // Get the list of adjacent territory names for sourceTerritory
            std::vector<std::string> adjacentTerritories = sourceTerritory->getAdjacentTerritoryNames();
            std::string attackTerritoryName = attackTerritory->getName();

            // Check if attackTerritory is adjacent to sourceTerritory
            bool isAdjacent = false;
            for (const std::string& adjName : adjacentTerritories) {
                if (adjName == attackTerritoryName) {
                    isAdjacent = true;
                    break;
                }
            }

            // If the territories are adjacent, create the Advance Order
            if (isAdjacent && (sourceTerritory->getArmies()>10 )) {
                Order* advanceOrder = new ::advanceOrder(10, sourceTerritory, attackTerritory, this);
                playerOrders->addOrder(advanceOrder);
                break;
            } 
        }
    }
}


    // Step 4: Use Cards to Issue Special Orders when no reinforcements are left
       if (!playerHand.getHand().empty()){
        std::cout << "[DEBUG] Cards available in hand: " << playerHand.getHand().size() << "\n";
        Card* card = playerHand.getHand().front();
        std::cout << "[DEBUG] Card type: " << card->getType() << "\n";
       
        if (card->getType() == "Reinforcement") {
            Card* tempCard = card;
            playerHand.removeCard(*tempCard);
            delete tempCard;
            return;
        }

        Order* specialOrder;
        // Handle different card types
        if (card->getType() == "Bomb") {
                specialOrder = new bombOrder(toAttack().front(), this);
                std::cout << "[INFO] " << name << " issues a Bomb Order.\n";
        } else if (card->getType() == "Airlift") {
                specialOrder = new airliftOrder(5, ownedTerritories.front(), toDefend().front(), this);
                std::cout << "[INFO] " << name << " issues an Airlift Order.\n";
        } else if (card->getType() == "Blockade") {
                Player* neutralPlayer = new Player("Neutral");
                specialOrder = new blockadeOrder(5, this, neutralPlayer, toDefend().front());
                std::cout << "[INFO] " << name << " issues a Blockade Order.\n";
        } 
        else if (card->getType() == "Diplomacy") {
            if (!toAttack().empty()) {
            Territory* targetTerritory = toAttack().front();
            Player* enemyPlayer = targetTerritory ? targetTerritory->getOwnerPlayer() : nullptr;
            specialOrder = new negotiateOrder(this, enemyPlayer);
            std::cout << "[INFO] " << name << " issues a Diplomacy Order.\n";
        }
        }
        if (specialOrder) {
            playerOrders->addOrder(specialOrder);
            std::cout << "[INFO] " << name << " successfully issued a " << card->getType() 
                      << " Order using a card.\n";
        }

        playerHand.removeCard(*card);

        // std::cout << "[DEBUG] Deleting card of type: " << card->getType() << "\n";
        delete card;
        // std::cout << "[DEBUG] Card deleted successfully.\n";
    }
}


/*------------------------------------------------------*/

bool Player::hasMoreOrders() const {
    bool hasReinforcements = (numberOfReinforcement > 0);
    bool hasDefendTargets = !toDefend().empty() && hasReinforcements;
    bool hasAttackTargets = !toAttack().empty() && hasReinforcements;
    bool hasCards = !playerHand.getHand().empty();

    // // Debugging output to track each condition
    // std::cout << "[DEBUG] Checking hasMoreOrders for " << name << ":\n";
    // std::cout << " - Reinforcements available: " << hasReinforcements << " (Remaining: " << numberOfReinforcement << ")\n";
    // std::cout << " - Defend targets available: " << hasDefendTargets << "\n";
    // std::cout << " - Attack targets available: " << hasAttackTargets << "\n";
    // std::cout << " - Cards available: " << hasCards << "\n";

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
