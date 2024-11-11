#include "GameEngine.h"
#include <iostream>

// void testGameStates() {
// }

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
    gameEngine.transition(START);  // Start the game

    // Setup players and territories
    gameEngine.startupPhase();  // This should add players and distribute territories

    gameEngine.gamestart(gameEngine);

    // Reinforcement phase testing: Check reinforcement counts manually for each player
    std::cout << "\n=== Testing Reinforcement Phase ===\n";
    gameEngine.reinforcementPhase();
    for (Player* player : gameEngine.getPlayerList()) {
        std::cout << player->getName() << " has " << player->getNumberOfReinforcement() << " units.\n";
    }

    // Issuing Orders testing: Run issueOrdersPhase() and observe order types
    std::cout << "\n=== Testing Issuing Orders Phase ===\n";
    gameEngine.issueOrdersPhase();
    
    // Execute Orders testing: Ensure correct behavior during execution
    std::cout << "\n=== Testing Execution Phase ===\n";
    gameEngine.executeOrdersPhase();

    // Run the full game loop to simulate until a win condition is met
    std::cout << "\n=== Full Main Game Loop ===\n";
    gameEngine.mainGameLoop();
}

/*----------------------------------------------------------------*/

// Main function that now calls the testStartupPhase function
int main() {
    std::cout << "Running testStartupPhase...\n";
    // testStartupPhase();
    
    std::cout << "\nRunning testMainGameLoop...\n";
    testMainGameLoop();

    return 0;
}
