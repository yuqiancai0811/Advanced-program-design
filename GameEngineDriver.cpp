#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "CommandProcessor.h"
#include <iostream>
#include <vector>

using namespace std;

//g++ -o main GameEngine.cpp CommandProcessor.cpp GameEngineDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp LoggingObserver.cpp

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
// void testMainGameLoop() {
//     GameEngine* gameEngine=new GameEngine;

//      // Setup players and territories
//      gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
//      while(true){
//         gameEngine->commandProcessor->getCommand();

//      }

// }

void testStartupPhase(){
    
     GameEngine* gameEngine=new GameEngine;

     // Setup players and territories
     gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
     while(true){
        gameEngine->commandProcessor->getCommand();

     }
 }

/*----------------------------------------------------------------*/

// Main function that now calls the testStartupPhase function
int main() {
    std::cout << "Running testStartupPhase...\n";
    testStartupPhase();
    
    // std::cout << "\nRunning testMainGameLoop...\n";
    // testMainGameLoop();


    return 0;
}
