#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include "Player.h"


using namespace std;

//g++ -o main.exe GameEngine.cpp CommandProcessor.cpp OrderDriver.cpp  Cards.cpp Map.cpp Orders.cpp Player.cpp LoggingObserver.cpp -g


// Order class methods

std::string getRandomCardType() {
    // Random number generation setup
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dist(1, 5);  // Range 1 to 5 for each card type

    // Select a card type based on the random number
    switch (dist(gen)) {
        case 1: return "BOMB";
        case 2: return "REINFORCEMENT";
        case 3: return "BLOCKADE";
        case 4: return "AIRLIFT";
        case 5: return "DIPLOMACY";
        default: return "";  // Should never happen
    }
}

Order::Order() : effect(new string), executed(new bool(false)), name(new string) {}


Order::~Order() {
    delete effect;
    delete executed;
    delete name;
}

Order::Order(const Order& other) {
    effect = new string(*other.effect);
    executed = new bool(*other.executed);
    name = new string(*other.name);
}

Order& Order::operator=(const Order& other) {
    if (this == &other) return *this;

    delete effect;
    delete executed;
    delete name;

    effect = new string(*other.effect);
    executed = new bool(*other.executed);
    name = new string(*other.name);

    return *this;
}

bool Order::validate() const {
    return true;
}

void Order::execute() {
    if (validate()) {
        *effect = "Base order executed";
        *executed = true;
    }
     Notify(this); // Part 5: trigger the writing of the entry in the log file 
}

string Order::toString() const {
    if (*executed && !name->empty()) {
        return "Order: " + *name + " is executed\n";
    } else if (!*executed && !name->empty()) {
        return "Order: " + *name + " is not executed\n";
    } else {
        return "Order: Null\n";
    }
}

ostream& operator<<(ostream& os, const Order& order) {
    os << "Order: " << *order.name << (*order.executed ? " (Executed)" : " (Not executed)") << "\n";
    os << "Effect: " << *order.effect << "\n";
    return os;
}
// Part5: Implementing the stringToLog() function from ILoggable
string Order::stringToLog() const {
      if (*executed && !name->empty()) {
        return "Order: " + *name + " is executed\n";
    } else if (!*executed && !name->empty()) {
        return "Order: " + *name + " is not executed\n";
    } else {
        return "Order: Null\n";
    }
}
// Subclass implementations
deployOrder::deployOrder(int armies, Territory* target, Player* player) {
    *name = "deployOrder";
    this->armies=armies;
    this->target=target;
    this->player=player;
}
deployOrder::deployOrder() {
    *name = "deployOrder";

}

deployOrder::~deployOrder(){}


bool deployOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {
        if(this->armies>this->player->getNumberOfReinforcement()) {
            std::cout<<"Not enough armies in the reinforcement pool";
            return false;
        }
        else {
            return true;
        }

    }
    else {
        return false;
    }
}

void deployOrder::execute() {
    if (validate()) {
        int currentReinforcements = this->player->getNumberOfReinforcement();
        std::cout << "Order of player: " << this->player->getName() << "\n";

        // Check if there are enough reinforcement units available
        if (currentReinforcements >= this->armies) {
            std::cout << "Original armies in the target territory: " << this->target->getArmies() << "\n";

            // Reduce the number of reinforcements and increase the number of armies in the target territory
            this->player->setNumberOfReinforcement(currentReinforcements - this->armies);
            this->target->setArmies(this->target->getArmies() + this->armies);

            std::cout << "Deployed " << this->armies << " units from the reinforcement pool to the target territory\n";
            std::cout << "After deployment, armies in the target territory: " << this->target->getArmies() << "\n";

        } else {
            std::cout << "Not enough units in the reinforcement pool\n";
        }
    } else {
        std::cout << "Order validation failed.\n";
    }
}



// void deployOrder::execute() {
//     if (validate()) {
//         std::cout<<"Origin armies in target territory:"<<this->target->getArmies()<<"\n";
//         this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement() - this->armies);
//         this->target->setArmies(this->target->getArmies() + this->armies);
//         this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement()-this->armies);
//         std::cout<<"Have taken "<<this->armies<<" from reinforcement pool to the target territory\n";
//         std::cout<<"After the operation:"<<this->target->getArmies()<<"\n";

//     }
// }
// void deployOrder::execute() {
//     if (validate()) {
//         std::cout << "Origin armies in target territory:" << this->target->getArmies() << "\n";
        
//         int newReinforcementCount = this->player->getNumberOfReinforcement() - this->armies;
//         this->player->setNumberOfReinforcement(newReinforcementCount);  // Only reduce once

//         this->target->setArmies(this->target->getArmies() + this->armies);
        
//         std::cout << "Have taken " << this->armies << " from reinforcement pool to the target territory\n";
//         std::cout << "After the operation:" << this->target->getArmies() << "\n";
//     }
// } //Add for testing P3

// // Part5: Override the stringToLog() function from ILoggable specific to the type of order
// string deployOrder::stringToLog() const {
//     stringstream log;
//     log << "Deploy Order: Deploy " << armies << " armies to " << target->getName() << ". ";
//     if (*executed) {
//         log << "Executed successfully. Current armies in target: " << target->getArmies();
//     } else {
//         log << "Execution failed.";
//     }
//     return log.str();
// }

advanceOrder::advanceOrder(int armies, Territory* source,Territory* target, Player* player) {
    *name = "Advance Order";
    this->armies=armies;
    this->source=source;
    this->target=target;
    this->player=player;
}
advanceOrder::~advanceOrder() {

    // Destructor body (can be empty)
}

// bool advanceOrder::validate() const {
//     //check if source territories belong to the player and target territory is adjacent to source   or not
//     if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->source) != this->player->getOwnedTerritories().end()&&std::find(this->source->getAdjacentTerritories().begin(),this->source->getAdjacentTerritories().end(),this->target)!=this->source->getAdjacentTerritories().end()) {



//         if(this->player->isNegotiating()==true) {
//             std::cout<<"Invalid order because negotiation\n";
//             return false;
//         }
//         else {

//             return true;
//         }

//     }
//     else {
//         std::cout<<"Invalid order advance\n";

//         return false;
//     }}

bool advanceOrder::validate() const {
    if (this->source == nullptr || this->target == nullptr) {
        std::cout << "Invalid order: source or target territory is null.\n";
        return false;
    }

    auto ownedTerritories = this->player->getOwnedTerritories();
    bool sourceBelongsToPlayer = false;
    for (Territory* territory : ownedTerritories) {
        if (territory->getName() == this->source->getName()) {
            sourceBelongsToPlayer = true;
            break;
        }
    }

    if (!sourceBelongsToPlayer) {
        std::cout << "Invalid order: source territory '" << this->source->getName()
                  << "' does not belong to player '" << this->player->getName() << "'.\n";
        return false;
    }

    // check adjacentTerritory
    auto adjacentTerritoryNames = this->source->getAdjacentTerritoryNames();
    std::string targetName = this->target->getName();
    bool targetIsAdjacent = false;

    for (const std::string& adjacentName : adjacentTerritoryNames) {
        if (adjacentName == targetName) {
            targetIsAdjacent = true;
            break;
        }
    }

    
    if (!targetIsAdjacent) {
        std::cout << "Invalid order: target territory '" << targetName
                << "' is not adjacent to source territory '" << this->source->getName() << "'.\n";
        return false;
    }


 
    if (this->player->isNegotiating()) {
        std::cout << "Invalid order: player '" << this->player->getName()
                  << "' is currently negotiating. Cannot issue advance order.\n";
        return false;
    }

    return true;
}




// void advanceOrder::execute() {
//     if (validate()) {
//         *executed = true;

//         // If it is not an attack, execute the defend (move) logic
//         if (this->source->getOwner() == this->target->getOwner()) {
//             std::cout << "Executing defend (move) order.\n";
//             // Move armies from the source territory to the target territory
//             this->source->setArmies(this->source->getArmies() - this->armies);
//             this->target->setArmies(this->target->getArmies() + this->armies);

//             // Output the result of the move
//             std::cout << "Moved " << this->armies << " armies from " << this->source->getName()
//                       << " to " << this->target->getName() << ".\n";
//             std::cout << "Source territory armies after move: " << this->source->getArmies() << "\n";
//             std::cout << "Target territory armies after move: " << this->target->getArmies() << "\n";

//         } else {
//             // Execute the attack logic if it is an attack order
//             std::cout << "Executing attack order.\n";

//             // Battle simulation loop
//             while (this->source->getArmies() > 0 && this->target->getArmies() > 0) {
//                 // Calculate the number of kills by the attacker and defender
//                 int sourceKill = std::max(1, static_cast<int>(this->source->getArmies() * 0.6));
//                 int targetKill = std::max(1, static_cast<int>(this->target->getArmies() * 0.7));

//                 // Update the number of armies after kills
//                 this->source->setArmies(std::max(0, this->source->getArmies() - targetKill));
//                 this->target->setArmies(std::max(0, this->target->getArmies() - sourceKill));

//                 // Output the result of each battle round
//                 std::cout << "Battle round result:\n";
//                 std::cout << "Attacker armies left: " << this->source->getArmies() << "\n";
//                 std::cout << "Defender armies left: " << this->target->getArmies() << "\n";
//             }

//             // Determine the result of the battle
//             if (this->target->getArmies() == 0) {
//                 // Attacker wins and captures the territory
//                 std::cout << "The attacker captures the territory " << this->target->getName() << ".\n";
//                 this->player->addTerritory(this->target);
//                 this->target->setArmies(this->source->getArmies());
//                 this->source->setArmies(0);
//                 this->target->getOwnerPlayer()->removeTerritory(target);
//                 this->winOrNot = true;

//                 // Output the result of the capture
//                 std::cout << "Captured territory armies after transfer: " << this->target->getArmies() << "\n";
//             } else {
//                 // Defender retains the territory
//                 std::cout << "Defender retains the territory " << this->target->getName() << ".\n";
//                 std::cout << "Remaining armies in source territory: " << this->source->getArmies() << "\n";
//                 std::cout << "Remaining armies in target territory: " << this->target->getArmies() << "\n";
//                 this->source->setArmies(0);
//             }
//         }

//         // Reward a card if the player successfully conquered a territory
//         if(winOrNot) {
//             //todo: card need to be given
//             //..........................
//             //..........................

//             //Card* card=new Card(getRandomCardType());
//             //this->player->getHand().addCard(card);
//         }
//     } else {
//         // Order validation failed
//         std::cout << "Order validation failed. Execution aborted.\n";
//     }
// }

/* Improved version*/
void advanceOrder::execute() {
    if (validate()) {
        *executed = true;

        // Check if it's a defend (move) order
        if (this->source->getOwner() == this->target->getOwner()) {
            std::cout << "Executing defend (move) order.\n";

            // Check if there are enough armies to move
            if (this->source->getArmies() >= this->armies) {
                // Move armies from source to target
                this->source->setArmies(this->source->getArmies() - this->armies);
                this->target->setArmies(this->target->getArmies() + this->armies);

                // Output the result of the move
                std::cout << "Moved " << this->armies << " armies from " << this->source->getName()
                          << " to " << this->target->getName() << ".\n";
                std::cout << "Source territory armies after move: " << this->source->getArmies() << "\n";
                std::cout << "Target territory armies after move: " << this->target->getArmies() << "\n";
            } else {
                // Not enough armies to move
                std::cout << "Not enough armies to move from " << this->source->getName() << ". Available: "
                          << this->source->getArmies() << ", Requested: " << this->armies << ".\n";
            }

        } else {
            // Execute attack logic
            std::cout << "Executing attack order.\n";
            int sourceKill = std::max(1, static_cast<int>(this->target->getArmies() * 0.7));
            int targetKill = std::max(1, static_cast<int>(this->armies * 0.6));

            // Battle simulation loop
            if (!(this->source->getArmies() >= this->armies)){std::cout << "The attacker do not have enough Arimies.\n";}

            if (this->source->getArmies() >= this->armies) {
                

                if (targetKill > this->target->getArmies())
                {
                    std::cout << "Defender  armies left: " << target->getArmies() - targetKill <<  "(min: 0 left)\n";
                    std::cout << "The attacker captures the territory " << this->target->getName() << ".\n";

                    this->target->getOwnerPlayer()->removeTerritory(target);
                    this->player->addTerritory(this->target);
                    target->setOwner(player->getName());
                    target->setPlayer(player);

                    this->target->setArmies(armies - sourceKill);
                    this->source->setArmies(source->getArmies() - armies);
                    
                    
                    this->winOrNot = true;  
                    std::cout << "Battle round result:\n";
                    std::cout << "Source territory armies left: " << source->getArmies() << "\n";
                    std::cout << "Target territory armies left: " << target->getArmies() << "\n";
                }
                // Determine the battle outcome
                else {
                std::cout << "Defender retains the territory " << this->target->getName() << ".\n";
                int targetNewArmies = this->target->getArmies() - targetKill;
                if (targetNewArmies < 0) {
                    targetNewArmies = 0; 
                }
                this->target->setArmies(targetNewArmies);

                
                int sourceNewArmies = this->source->getArmies() - sourceKill;
                if (sourceNewArmies < 0) {
                    sourceNewArmies = 0; 
                }

                this->source->setArmies(sourceNewArmies);
                // Output the result of each battle round
                std::cout << "Battle round result:\n";
                std::cout << "Attacker armies left: " << this->source->getArmies() << "\n";
                std::cout << "Defender armies left: " << this->target->getArmies() << "\n";
            }
                

            }
            
            
        }

        // Reward a card if the player successfully conquered a territory
        if (winOrNot) {
            // Card reward (to be implemented)
            //Card* card = new Card(getRandomCardType());
            //this->player->getHand().addCard(card);
        }
    } else {
        std::cout << "Order validation failed. Execution aborted.\n";
    }
}




bombOrder::bombOrder(Territory* target, Player* player) {
    *name = "Bomb Order";
    this->target=target;
    this->player=player;
}

bool bombOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {


        bool isValidOrder = false; // Flag to track if the order is valid

        for (Territory* t : this->player->getOwnedTerritories()) {


            // if (std::find(t->getAdjacentTerritoryNames().begin(), t->getAdjacentTerritoryNames().end(), this->target->getName()) == t->getAdjacentTerritoryNames().end() &&
            //     this->player->isNegotiating() == true &&
            //     this->target->getOwnerPlayer()->isNegotiating() == true) {
            //     std::cout << "Invalid order bomb1\n";
            //     isValidOrder = false;
            //     break;  // Exit loop if condition is met
            //     } else {
            //         isValidOrder = true;
            //     }
            for(Territory* i : t->getAdjacentTerritories()) {
                if(i->getName()==this->target->getName()) {
                    isValidOrder = true;
                }

            }
        }

        return isValidOrder;
    }
    else {
        return false;
    }
    return false;
}

void bombOrder::execute() {
    if (validate()) {
        *executed = true;
        std::cout<<"\nTarget territory before bomb:"<<this->target->getArmies();

        this->target->setArmies((this->target->getArmies())/2);
        std::cout<<"\nTarget territory after bomb:"<<this->target->getArmies();
    }
}

blockadeOrder::blockadeOrder(int armies,Player* player, Player* neutral,Territory* target) {
    *name = "Blockade Order";
    this->armies=armies;
    this->player=player;
    this->neutral=neutral;
    this->target=target;

}
blockadeOrder::blockadeOrder(int armies,Player* player, Territory* target) {
    *name = "Blockade Order";
    this->armies=armies;
    this->player=player;
    this->target=target;
    Player neutral=Player();

}

bool blockadeOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {
        if(this->player->getHand().hasCardType("BLOCKADE")) {
            return true;
        }
        else {

            return false;
        }    }
    else {
        std::cout<<"Invalid order blockade\n";
        return false;
    }
}

void blockadeOrder::execute() {
    if (validate()) {
        *executed = true;
        std::cout<<"Running the blockade order,\n";
        // std::cout<<"The armies number:"<<this->target->getArmies()<<"\n";

        // this->target->setArmies((this->target->getArmies())*2);
        // this->target->setOwner(this->neutral->getName());
        // this->neutral->addTerritory(this->target);
        // this->player->removeTerritory(this->target);
        // std::cout<<"The armies have doubled:"<<this->target->getArmies()<<"\n";

    }
}

airliftOrder::airliftOrder(int armies,Territory* source,Territory* target,Player* player) {
    *name = "Airlift Order";
    this->armies=armies;
    this->source=source;
    this->target=target;
    this->player=player;
}

bool airliftOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()&&std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->source) != this->player->getOwnedTerritories().end()) {
        if(this->player->getHand().hasCardType("Airlift")) {
            std::cout<<"valid order airlift\n";

            return true;
        }
        else {
            std::cout<<"Invalid order airlift\n";

            return false;
        }

    }
    else {
        std::cout<<"Invalid order airlift\n";
        return false;
    }

}

void airliftOrder::execute() {
    std::cout<<" order airlift execute\n";

    if (validate()) {
        *executed = true;
        std::cout<<"\nSource territory armies:"<<this->source->getArmies();
        std::cout<<"\nTarget territory armies:"<<this->target->getArmies();


        this->source->setArmies(this->source->getArmies() - this->armies);
        this->target->setArmies(this->target->getArmies() + this->armies);

        std::cout<<"\nSource territory armies(after airlift):"<<this->source->getArmies();
        std::cout<<"\nTarget territory armies(after airlift):"<<this->target->getArmies();
    }
}

//player is the player who issues this order
negotiateOrder::negotiateOrder(Player* player,Player* enemy) {
    // std::cout<<"debug in negotiateOrder \n";
    // std::cout<<"player"<<player->getName();
    // std::cout<<"enemy"<<enemy->getName();

    std::cout<<"valid order negotiation\n";
    *name = "Negotiate Order";
    this->player=player;
    this->enemy=enemy;

}

bool negotiateOrder::validate() const {
    if(player!=enemy) {
        std::cout<<"valid order negotiation\n";

        return true;
    }
    else {
        std::cout<<"Invalid order negotiation\n";
        return false;
    }

}

void negotiateOrder::execute() {
    // if (validate()) {
    //     *executed = true;
    //     this->player->setNegotiate(true);
    //     this->enemy->setNegotiate(true);
    // }
}

// orderList methods
orderList::orderList() {
    this->orders = vector<Order*>();
}

orderList::~orderList() {
    for (Order* order : orders) {
        delete order;
    }
}

void orderList::addOrder(Order* order) {
    orders.push_back(order);
    Notify(this); // Part 5: trigger the writing of the entry in the log file 

}

void orderList::removeOrder(int index) {
    if (index >= 0 && index < orders.size()) {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void orderList::moveOrder(int oldIndex, int newIndex) {
    if (oldIndex >= 0 && oldIndex < orders.size() &&
        newIndex >= 0 && newIndex < orders.size()) {
        swap(orders[oldIndex], orders[newIndex]);
    }
}

void orderList::showAllOrders() const {
    for (const auto& order : orders) {
        cout << order->toString();
    }
}

/*--------------- Helper function for executeOrdersPhase() in Part3 -----------------*/
bool orderList::hasMoreOrders() const {
    return !orders.empty();
}

Order* orderList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}
/*-------------------------------------------------------------------------------*/

ostream& operator<<(ostream& os, const orderList& ordersList) {
    for (const auto& order : ordersList.orders) {
        os << *order;
    }
    return os;
}

// Part5: IImplementing the stringToLog() function from ILoggable
string orderList::stringToLog() const {
    string log = "OrderList: Total orders = " + to_string(orders.size()) + "\n";

    for (size_t i = 0; i < orders.size(); ++i) {
        log += " - Order " + to_string(i + 1) + ": " + orders[i]->toString() + "\n";
    }

    return log;
}
