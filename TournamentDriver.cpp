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

int main()
{
    GameEngine gameEngine;
    LogObserver logger("tournament_results.txt");
    gameEngine.Attach(&logger);
    gameEngine.setTournamentMode(true);

    // Example setup and execution of a tournament
    gameEngine.updateTournamentResults({{"Map1", "Win", "Loss", "Win"},
                                        {"Map2", "Loss", "Draw", "Win"}});
    string tournamentResults = gameEngine.displayTournamentResults();
    std::cout << tournamentResults;
    // Detach the logger when done
    gameEngine.Detach(&logger);
    return 0;
}
