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


  if(strategy_name == "human") {
    return new Human(player);
  } 
  else if(strategy_name == "aggressive") {
    return new Aggressive(player);
  } 
  else if(strategy_name == "benevolent") {
    return new Benevolent(player);
  } 
  else if(strategy_name == "neutral") {
    return new Neutral(player);
  } 
  else if (strategy_name == "cheater") {
    return new Cheater(player);
  } 
  else if (strategy_name == "random") {
    if (rand() % 2 == 0) {
      return new Neutral(player);
    } else {
      return new Benevolent(player);
    }
  } else {
    throw std::runtime_error("Invalid strategy name");
  }
}

Human::Human(Player *player) {
  this->player = player;
}
std::vector<Territory*> Human::toAttack() {

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
std::vector<Territory *> Human::toDefend() {
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

void Human::issueOrder() {
    Hand playerHand=player->getHand();
    std::cout << player->getName()<<", which order would you like to issue?" << std::endl;
    std::cout <<"The List of order:\n1.Deploy\n2.Advance\n3.Bomb\n4.Blockade\n5.Airlift\n6.Negotiate"<< std::endl;
    int choice;
    std::cin>>choice;
    switch(choice) {
        case 1: {
            //add deploy order
            int numberTODeploy;
            int indexOfTerritory;
            std::cout<<"Please enter number of armies you want to deploy:"<<endl;
            std::cin>>numberTODeploy;
            std::cout<<"Please select the territory you want to deploy to:"<<endl;
            for(int i=0;i<player->getOwnedTerritories().size();i++) {
                std::cout << "Territory at index " << i << ": " << player->getOwnedTerritories()[i]->getName() << "\n";
            }
            std::cin>>indexOfTerritory;
            //std::cout<<"Original territory army number:"<<player->getOwnedTerritories()[indexOfTerritory]->getArmies()<<endl;

            deployOrder* deploy_order=new deployOrder(numberTODeploy,player->getOwnedTerritories()[indexOfTerritory],player);
            deploy_order->execute();
            //std::cout<<"After deploy:"<<player->getOwnedTerritories()[indexOfTerritory]->getArmies()<<endl;
            //player->getOrders().addOrder(deploy_order);
            break;
        }
        case 2: {
            //add advance order
            int numberTOAdvance;
            int indexOfTerritory;
            int indexOfAdj;

            std::cout<<"Please enter number of armies you want to advance:"<<endl;
            std::cin>>numberTOAdvance;
            std::cout<<"Select a territory as armies resource"<<endl;
            for(int i=0;i<player->getOwnedTerritories().size();i++) {
                std::cout << "Territory at index " << i << ": " << player->getOwnedTerritories()[i]->getName() << "\n";
            }
            std::cin>>indexOfTerritory;
            std::cout<<"Select an adjacent territory of your source territory:"<<endl;
            for(int i=0;i<player->getOwnedTerritories()[indexOfTerritory]->getAdjacentTerritories().size();i++) {
                std::cout << "Territory at index " << i << ": " << player->getOwnedTerritories()[indexOfTerritory]->getAdjacentTerritories()[i]->getName() << "\n";
            }
            std::cin>>indexOfAdj;
            advanceOrder* advance_order=new advanceOrder(numberTOAdvance,player->getOwnedTerritories()[indexOfTerritory],player->getOwnedTerritories()[indexOfTerritory]->getAdjacentTerritories()[indexOfAdj],player);
            advance_order->execute();
            //player->getOrders().addOrder(advance_order);
            break;
        }
        case 3: {
            //add bomb order
            //There should be bomb card to play bomb order

            //check if there is bomb card in player's hand
            bool haveOrNot=playerHand.hasCardType("Bomb");


            if(haveOrNot) {
                //have bomb card, keep forward
                //find the index of the bomb card in hand and then remove it from hand
                for(int i=0;i<playerHand.getHand().size();i++) {
                    Card* card=playerHand.getHand()[i];
                    if(card->getType()=="Bomb") {
                        playerHand.removeCard(*card);
                        break;

                    }
                }
                //removed card, do the bomb order now
                Territory* target;
                //display all the valid target territory
                for(int i=0;i< player->getOwnedTerritories().size();i++) {
                    for(int j=0;j<player->getOwnedTerritories()[i]->getAdjacentTerritories().size();i++) {
                        for(Territory* a:player->getOwnedTerritories()) {
                            if(a!=player->getOwnedTerritories()[i]->getAdjacentTerritories()[j]) {
                                std::cout << "Territory with two index " << i << " and " << j << "\n";
                            }

                        }
                    }
                }
                std::cout<<"Please enter the two index:"<<endl;
                int firstindex;
                int secondindex;
                std::cin>>firstindex;
                std::cin>>secondindex;
                bombOrder* bomb_order=new bombOrder(player->getOwnedTerritories()[firstindex]->getAdjacentTerritories()[secondindex],player);
                bomb_order->execute();
                //player->getOrders().addOrder(bomb_order);



            }
            else {
                std::cout<<"There is no bomb card in hand, you can not play bomb order"<<endl;
            }
            break;
        }
        case 4: {
            //add blockade order
            bool haveOrNot=playerHand.hasCardType("Blockade");


            if(haveOrNot) {
                //have blockade card, keep forward
                //find the index of the blockade card in hand and then remove it from hand
                for(int i=0;i<playerHand.getHand().size();i++) {
                    Card* card=playerHand.getHand()[i];
                    if(card->getType()=="Blockade") {
                        playerHand.removeCard(*card);
                        break;

                    }
                }
                //removed card, do the blockade order now
                int numberOfArmies;
                int indexOfTerritory;
                std::cout<<"Please enter number of armies :"<<endl;
                std::cin>>numberOfArmies;
                for(int i=0;i<player->getOwnedTerritories().size();i++) {
                    std::cout << "Territory at index " << i << ": " << player->getOwnedTerritories()[i] << "\n";
                }
                std::cout<<"Please select the territory you want to blockade:"<<endl;
                std::cin>>indexOfTerritory;
                blockadeOrder* blockade_order=new blockadeOrder(numberOfArmies,player,player->getOwnedTerritories()[indexOfTerritory]);
                blockade_order->execute();
                //player->getOrders().addOrder(blockade_order);

            }
            else {
                std::cout<<"There is no blockade card in hand, you can not play blockade order"<<endl;
            }
            break;
        }

        case 5: {
            //add airlift order
            bool haveOrNot=playerHand.hasCardType("Airlift");


            if(haveOrNot) {
                //have airlift card, keep forward
                //find the index of the airlift card in hand and then remove it from hand
                for(int i=0;i<playerHand.getHand().size();i++) {
                    Card* card=playerHand.getHand()[i];
                    if(card->getType()=="Airlift") {
                        playerHand.removeCard(*card);
                        break;

                    }
                }
                //removed card, do the airlift order now
                int numberOfArmies;
                int indexOfTerritory1;
                int indexOfTerritory2;

                std::cout<<"Please enter number of armies you want to airlift:"<<endl;
                std::cin>>numberOfArmies;
                for(int i=0;i<player->getOwnedTerritories().size();i++) {
                    std::cout << "Territory at index " << i << ": " << player->getOwnedTerritories()[i]->getName() << "\n";
                }
                std::cout<<"Please select the source territory you want to take armies out:"<<endl;
                std::cin>>indexOfTerritory1;
                std::cout<<"Please select the target territory you want to take armies to:"<<endl;
                std::cin>>indexOfTerritory2;
                airliftOrder* airlift_order=new airliftOrder(numberOfArmies,player->getOwnedTerritories()[indexOfTerritory1],player->getOwnedTerritories()[indexOfTerritory2],player);
                airlift_order->execute();
                //player->getOrders().addOrder(airlift_order);
            }
            else {
                std::cout<<"There is no airlift card in hand, you can not play airlift order"<<endl;
            }
            break;
        }


    }

}

Aggressive::Aggressive(Player *player) {
  this->player = player;
}

std::vector<Territory*> Aggressive::toAttack() {

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
std::vector<Territory *> Aggressive::toDefend() {
    // does not defend
    return {};
}
void Aggressive::issueOrder() {
    //issue order for attack
    std::vector<Territory*> attackList;
    // std::vector<Territory*> defendList;
    attackList=player->toAttack();
    int temp=0;
    Territory* territoryTemp;
    int indexOfTerritory;
    // for (Territory* a:player->getOwnedTerritories()) {
    //     temp=temp+a->getArmies();
    //     a->setArmies(0);
    // }
    // player->getOwnedTerritories()[0]->setArmies(temp);
    for(int i=1;i<player->getOwnedTerritories().size();i++) {
        int temp=player->getOwnedTerritories()[i]->getArmies();
        advanceOrder* ad=new advanceOrder(temp,player->getOwnedTerritories()[i],player->getOwnedTerritories()[0],player);
        ad->execute();
    }

    if(player->getNumberOfReinforcement()>0) {
        //do the deploy order if there is reinforcement left in the pool
        //add all the armies into first territory
        deployOrder* deploy_order=new deployOrder(player->getNumberOfReinforcement(),player->getOwnedTerritories()[0],player);
        deploy_order->execute();
    }
    //attack all the adjacent territory that not belongs to player
    //if there is no place to attack, advance teh armies to another territory then do teh same thing until there is no armies left

    int count=0;
    for(Territory* a:player->getOwnedTerritories()) {
        if(a->getArmies()>0&&attackList.size()>0) {
            for(Territory*b:attackList) {
                //try to attack all the territory in the attack list
                //if the territory is not adjacent with a, it can not pass the validation, won't affect the result
                advanceOrder* attackOrder=new advanceOrder(player->getOwnedTerritories()[count]->getArmies(),player->getOwnedTerritories()[count],b,player);
                if(attackOrder->validate()==true) {
                    attackOrder->execute();

                    //delete this territory from the attack list
                    auto it = std::find(attackList.begin(), attackList.end(), b);
                    if (it != attackList.end()) {
                        attackList.erase(it);
                    }

                }
            }
            advanceOrder* advance_order=new advanceOrder(player->getOwnedTerritories()[count]->getArmies(),player->getOwnedTerritories()[count],player->getOwnedTerritories()[count+1],player);
            advance_order->execute();
            count++;
        }
        else {
            std::cout<<"This aggressive player runs out his/her armies "<<endl;
            break;
        }



    }


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

// Temporary decideCard()
// Order* Benevolent::decideCard(Card* card) {
//     if (!card) {
//         std::cout << "No card provided. Benevolent player takes no action.\n";
//         return nullptr;
//     }
//
//     std::cout << "Benevolent player does not use aggressive cards like " << card->getType() << ".\n";
//
//     // Benevolent players may only use certain types of cards (e.g., Reinforcement cards).
//     if (card->getType() == "Reinforcement") {
//         std::cout << "Benevolent player uses a Reinforcement card.\n";
//         // Implement logic for Reinforcement card.
//         // Example: Create a specific order using this card.
//         // Return an order if appropriate.
//         return nullptr; // Replace with the actual order object if needed.
//     }
//
//     // Benevolent player does not use other cards.
//     std::cout << "Benevolent player chooses not to use this card.\n";
//     return nullptr;
// }



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

// Order* Neutral::decideCard(Card* card) {
//     std::cout << player->getName() << " (Neutral Player) does not use cards.\n";
//     return nullptr; // Does nothing
// }

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

// Order* Cheater::decideCard(Card* card) {
//     std::cout << player->getName() << " (Cheater Player) does not use cards.\n";
//     return nullptr; // Does nothing
// }

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