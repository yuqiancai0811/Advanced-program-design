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
    
    LogObserver *logger = new LogObserver("Tournament_results.txt");

     gameEngine = gameEngine->startupPhase(); 
         gameEngine->Attach(logger);
    
     while(true){
        gameEngine->commandProcessor->getCommand();
     }
          
}

int main() {
    testTournament();
    return 0;
}

