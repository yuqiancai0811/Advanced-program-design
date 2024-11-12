#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "CommandProcessor.h"
#include <iostream>
#include <vector>

using namespace std;

void testStartupPhase() {
    GameEngine game;



    std::string command;
    while (game.getCurrentState() != "END") {
        std::cout << "Current game state: " << game.getCurrentState() << std::endl;
        std::cout << "Enter command (start/play/end): ";
        std::cin >> command;
        game.handleUserCommand(command,game);
    }
    std::cout << "Exiting the game ";
    
}
/* ------------------- testMainGameLoop() for Part3 -------------------*/
/*
You must deliver a driver as a free function named testMainGameLoop() that demonstrates that 
(1) a player receives the correct number of army units in the reinforcement phase (showing different cases); 
(2) a player will only issue deploy orders and no other kind of orders if they still have army units in their reinforcement pool; 
(3) a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists; 
(4) a player can play cards to issue orders; 
(5) a player that does not control any territory is removed from the game; 
(6) the game ends when a single player controls all the territories. 
All of this must be implemented in a single .cpp/.h file duo named GameEngine.cpp/GameEngine.h, 
except the issueOrder() method, which is a member of the Player class, implemented in the Player.h/Player.h file duo.
This driver function must be in the GameEngineDriver.cpp file.
*/
void testMainGameLoop() {
    GameEngine gameEngine;

    gameEngine.startupPhase();  // This should add players and distribute territories

    gameEngine.gamestart(gameEngine);

    gameEngine.mainGameLoop();

}

void testStartupPhase(){
    
    GameEngine* gameEngine=new GameEngine;

    // Setup players and territories
    gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
    while(true){
        gameEngine->commandProcessor->getCommand();

    }
   

}

// void testMainGameLoop() {
//     cout << "=== Test: Main Game Loop for Part 3 ===" << endl;

//     // Step 1: Initialize the game engine and map
//     GameEngine game;
//     Map* map = new Map();
//     game.selectedMap = map;

//     // Manually add continents and territories to simulate a game map
//     Continent* continent1 = new Continent("Continent1", 5);
//     Continent* continent2 = new Continent("Continent2", 3);
//     map->addContinent(continent1);
//     map->addContinent(continent2);

//     // Territories for testing reinforcement phase and order issuing
//     Territory* territory1 = new Territory("Territory1", 0, 0, "Continent1", {});
//     Territory* territory2 = new Territory("Territory2", 1, 1, "Continent1", {"Territory1"});
//     Territory* territory3 = new Territory("Territory3", 2, 2, "Continent2", {"Territory2"});

//     // Link territories as neighbors
//     territory1->addAdjacentTerritory(territory2);
//     territory2->addAdjacentTerritory(territory1);
//     territory2->addAdjacentTerritory(territory3);
//     territory3->addAdjacentTerritory(territory2);

//     map->addTerritory(territory1);
//     map->addTerritory(territory2);
//     map->addTerritory(territory3);

//     continent1->addTerritory(territory1);
//     continent1->addTerritory(territory2);
//     continent2->addTerritory(territory3);

//     // Step 2: Add players to the game
//     Player* player1 = new Player("Player1");
//     Player* player2 = new Player("Player2");
//     game.playerList.push_back(player1);
//     game.playerList.push_back(player2);

//     // Step 3: Assign territories
//     player1->addTerritory(territory1);
//     player1->addTerritory(territory2);
//     player2->addTerritory(territory3);
//     territory1->setOwner(player1->getName());
//     territory2->setOwner(player1->getName());
//     territory3->setOwner(player2->getName());

//     // Step 4: Give players initial cards and reinforcements
//     Deck deck(2);  // Deck with two sets of cards
//     player1->getHand().addCard(deck.draw());
//     player2->getHand().addCard(deck.draw());

//     player1->setNumberOfReinforcement(10); // Give Player1 10 reinforcements to deploy
//     player2->setNumberOfReinforcement(5);  // Give Player2 5 reinforcements

//     // Begin main game loop for testing
//     game.transition(ASSIGN_REINFORCEMENT);
//     game.mainGameLoop();

//     // Check end conditions manually in each phase
//     cout << "=== Checking conditions ===" << endl;

//     // (1) Reinforcement Phase: Verify correct reinforcements
//     cout << "Reinforcement Phase Check: " << endl;
//     cout << "Player1 reinforcements: " << player1->getNumberOfReinforcement() << endl;
//     cout << "Player2 reinforcements: " << player2->getNumberOfReinforcement() << endl;

//     // (2) Deploy orders only when reinforcements are available
//     cout << "Deploy Orders Check: " << endl;
//     player1->issueOrder(); // Expected to issue a deploy order
//     player2->issueOrder(); // Expected to issue a deploy order

//     // (3) Advance orders to defend or attack
//     cout << "Advance Orders Check: " << endl;
//     player1->setNumberOfReinforcement(0);  // Set reinforcements to 0 to test advance orders
//     player1->issueOrder(); // Expected to issue an advance order for defend or attack

//     // (4) Card play check
//     cout << "Card Play Check: " << endl;
//     player1->getHand().addCard(deck.draw());  // Add a card to ensure they can play
//     player1->issueOrder();  // Expected to play a card order if possible

//     // (5) Removing players with no territories
//     cout << "Player Removal Check: " << endl;
//     player2->removeTerritory(territory3);  // Remove all territories from Player2
//     game.removeEliminatedPlayers();  // Expected to remove Player2
//     cout << "Player2 should be eliminated if they have no territories." << endl;

//     // (6) End game when a single player controls all territories
//     cout << "Game End Condition Check: " << endl;
//     player1->addTerritory(territory3);  // Player1 now controls all territories
//     game.removeEliminatedPlayers();
//     if (game.getPlayerList().size() == 1) {
//         cout << "Game ends with " << game.getPlayerList().front()->getName() << " as the winner." << endl;
//     } else {
//         cout << "Game has not ended yet, multiple players are still active." << endl;
//     }

//     cout << "=== End of Test: Main Game Loop for Part 3 ===" << endl;

//     // Cleanup
//     delete map;
//     delete player1;
//     delete player2;
// }


/*----------------------------------------------------------------*/

// Main function that now calls the testStartupPhase function
int main() {
    std::cout << "Running testStartupPhase...\n";
    // testStartupPhase();
    
    std::cout << "\nRunning testMainGameLoop...\n";
    //testMainGameLoop();
    testStartupPhase();

    return 0;
}
