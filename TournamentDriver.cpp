#include "CommandProcessor.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


void testTournament() {
    GameEngine* gameEngine=new GameEngine;

     // Setup players and territories
     gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
     while(true){
        gameEngine->commandProcessor->getCommand();

     }
     
}

int main() {
    testTournament();
    return 0;
}

