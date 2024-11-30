#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "Player.h"
#include "Orders.h"
#include "PlayerStrategy.h"

PlayerStrategy* PlayerStrategy::createStrategy(Player *player, const std::string& strategy) {

  std::string strategy_name;
  for(auto& c : strategy){strategy_name += (char)std::tolower(c);}


  // if(strategy_name == "human") {
  //   return new Human(player);
  // } else if(strategy_name == "aggressive") {
  //   return new Aggressive(player);
  // } else if(strategy_name == "benevolent") {
  //   return new Benevolent(player);
  // } else if(strategy_name == "neutral") {
  //   return new Neutral(player);
  // } else if (strategy_name == "cheater") {
  //   return new Cheater(player);
  // } else if (strategy_name == "random") {
  //   if (rand() % 2 == 0) {
  //     return new Aggressive(player);
  //   } else {
  //     return new Benevolent(player);
  //   }
  // } else {
  //   throw std::runtime_error("Invalid strategy name");
  // }
}

Human::Human(Player *player) {
  this->player = player;
}

Aggressive::Aggressive(Player *player) {
  this->player = player;
}


// -------------------------------------------------------------------------------Benevolent player by Lucas----------------------------------------------------------------------------------------------------------
Benevolent::Benevolent(Player *player) {
  this->player = player;
}

void Benevolent::issueOrder() {
   
    // Step 2: Issue Orders for Defense
    std::vector<Territory*> defendList = toDefend();
    if (!defendList.empty()) {
    std::cout << "[LOG] " << player->getName() << " attempting to issue Advance Order for defense...\n";
    int numberOfReinforcement2 = player->getNumberOfReinforcement();

    for (Territory* defendTerritory : defendList) {

        
        if (numberOfReinforcement2 > 15) {
            std::cout << "[LOG] " << player->getName() << " attempting to deploy reinforcements...\n";
            std::vector<Territory*> defendList = toDefend();

        
            
                
            if (numberOfReinforcement2 > 15){

                        
                int unitsToDeploy = std::min(numberOfReinforcement2, 5); // Limit to deploying 5 units at a time
                numberOfReinforcement2 -= unitsToDeploy;

                // Add deploy order
                Order* deployOrder = new:: deployOrder(unitsToDeploy, defendTerritory, player);
                player->getOrders().addOrder(deployOrder);
                std::cout << "[INFO] " << player->getName() << " issues a Deploy Order to " 
                << defendTerritory->getName() << " with " 
                << unitsToDeploy << " units. Remaining reinforcements: " 
                << numberOfReinforcement2 << "\n";
                continue;
                        
            }         
        }


        for (Territory* sourceTerritory : player->getOwnedTerritories()) {
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
                Order* advanceOrder = new ::advanceOrder(10, sourceTerritory, defendTerritory, player);
                
                player->getOrders().addOrder(advanceOrder);
                break;
            } 
        }
        }
    }


    

    // Step 4: Use Cards to Issue Special Orders when no reinforcements are left
        Hand playerhand= player->getHand();
        vector<Card *> cardlist = playerhand.getHand();
       if (!cardlist.empty()){
        std::cout << "[DEBUG] Cards available in hand: " << cardlist.size() << "\n";
        Card* card = cardlist.front();
        std::cout << "[DEBUG] Card type: " << card->getType() << "\n";
       
        if (card->getType() == "Reinforcement") {
            Card* tempCard = card;
            playerhand.removeCard(*tempCard);
            delete tempCard;
            return;
        }

        Order* specialOrder;
        // Handle different card types
        if (card->getType() == "Bomb") {
          std::cout << "[INFO] " << player->getName() << " is Benevolent can not issue Bomb\n";

          } 
        else if (card->getType() == "Airlift") {
          specialOrder = new airliftOrder(5, player->getOwnedTerritories().front(), toDefend().front(), player);
          std::cout << "[INFO] " << player->getName() << " issues an Airlift Order.\n";
          } 
        else if (card->getType() == "Blockade") {
          std::cout << "[INFO] " << player->getName() << " is Benevolent can not issue Blockade\n";
                
          } 
        else if (card->getType() == "Diplomacy") {
           std::cout << "[INFO] " << player->getName() << " is Benevolent can not issue Diplomacy\n";
        }
        if (specialOrder) {
            player->getOrders().addOrder(specialOrder);
            std::cout << "[INFO] " << player->getName() << " successfully issued a " << card->getType() 
                      << " Order using a card.\n";
        }

        playerhand.removeCard(*card);
        delete card;
        // std::cout << "[DEBUG] Deleting card of type: " << card->getType() << "\n";
       
        // std::cout << "[DEBUG] Card deleted successfully.\n";
    }
}
  


std::vector<Territory *> Benevolent::toDefend() {
    std::vector<Territory*> defendList;

    std::vector<Territory*>ownedTerritories=player->getOwnedTerritories();

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

            if (adj->getOwner() != player->getName()) {
                hasEnemyAdjacent = true;
                break; 
            }
        }

        if (hasEnemyAdjacent) {
            defendList.push_back(territory);
        }
    }

     std::sort(defendList.begin(), defendList.end(), [](Territory* a, Territory* b) {
         return a->getArmies() < b->getArmies();
     });
    return defendList;
}

std::vector<Territory *> Benevolent::toAttack() {
  // does not attack
  return {};
}



// -------------------------------------------------------------------------------End of Benevolent player -----------------------------------------------------------------------


//------------------------------------------------ Neutral player & Cheater player by M.M ------------------------------------------------//
/* NeutralPlayerStrategy
Behavior:
- Computer player that never issues any order, nor uses any cards, though it may have or receive cards. 
- If a Neutral player is attacked, it becomes an Aggressive player.
*/

Neutral::Neutral(Player* player) {
    this->player = player;
}

void Neutral::issueOrder() {
    std::cout << player->getName() << " (Neutral Player) does not issue any orders.\n";
}

Order* Neutral::decideCard(Card* card) {
    std::cout << player->getName() << " (Neutral Player) does not use cards.\n";
    return nullptr; // Does nothing
}

std::vector<Territory*> Neutral::toDefend() {
    std::cout << player->getName() << " (Neutral Player) has no territories to defend.\n";
    return {}; // Empty list
}

std::vector<Territory*> Neutral::toAttack() {
    std::cout << player->getName() << " (Neutral Player) does not attack.\n";
    return {}; // Empty list
}


/*
CheaterPlayerStrategy
Behavior:
- Automatically conquers all adjacent territories to its own (once per turn).
- Does not use cards, though it may have or receive cards.
*/
Cheater::Cheater(Player* player) {
    this->player = player;
}

void Cheater::issueOrder() {
    std::cout << player->getName() << " (Cheater Player) automatically conquers all adjacent territories!\n";

    for (Territory* ownedTerritory : player->getOwnedTerritories()) {
        for (Territory* adjacent : ownedTerritory->getAdjacentTerritories()) {
            if (adjacent->getOwner() != player->getName()) {
                // Conquer the adjacent territory
                std::cout << "Conquering territory: " << adjacent->getName() 
                          << " owned by: " << adjacent->getOwner() << "\n";

                // Change ownership
                Player* previousOwner = adjacent->getOwnerPlayer();
                if (previousOwner) {
                    previousOwner->removeTerritory(adjacent);
                }
                adjacent->setOwner(player->getName());
                adjacent->setPlayer(player);
                player->addTerritory(adjacent);

                // Optionally reset armies in the conquered territory
                adjacent->setArmies(1);
            }
        }
    }
}

Order* Cheater::decideCard(Card* card) {
    std::cout << player->getName() << " (Cheater Player) does not use cards.\n";
    return nullptr; // Does nothing
}

std::vector<Territory*> Cheater::toDefend() {
    std::cout << player->getName() << " (Cheater Player) defends all owned territories.\n";
    return player->getOwnedTerritories();
}

std::vector<Territory*> Cheater::toAttack() {
    std::cout << player->getName() << " (Cheater Player) targets all adjacent territories.\n";
    std::vector<Territory*> attackList;

    for (Territory* ownedTerritory : player->getOwnedTerritories()) {
        for (Territory* adjacent : ownedTerritory->getAdjacentTerritories()) {
            if (adjacent->getOwner() != player->getName()) {
                attackList.push_back(adjacent);
            }
        }
    }

    return attackList;
}

//------------------------------------------------ End of Neutral player & Cheater player  ------------------------------------------------//