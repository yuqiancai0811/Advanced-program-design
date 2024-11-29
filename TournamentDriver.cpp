#include "GameEngine.h"
#include "LoggingObserver.h"
#include "Map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int main() {
    // Example usage
    GameEngine gameEngine;
    gameEngine.params = {{"Map1.txt", "Map2.txt"}, {"Aggressive", "Defensive"}, 3, 30};
    gameEngine.tournamentResults = {
        {"Map1.txt", "Win", "Loss", "Win"},
        {"Map2.txt", "Loss", "Loss", "Win"}
    };

    cout << gameEngine.displayTournamentResults() << endl;
    return 0;
}
