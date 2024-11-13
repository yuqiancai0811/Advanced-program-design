#include "GameEngine.h"
#include "MapDriver.h"
#include "Orders.h"
#include "Player.h"
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <iostream>
#include <algorithm>  // For max
#include "CommandProcessor.h"
#include <cstdlib>    
#include <ctime>     

using namespace std;
///
//g++ -o main GameEngine.cpp CommandProcessor.cpp CommandProcessingDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp LoggingObserver.cpp

/*-------------------- Phases for Part 3 -------------------------*/
extern const std::string START = "start";
extern const std::string MAPLODADED = "map_loaded";
extern const std::string GAMESTART = "gamestart";
extern const std::string PLAYERSADDED = "players_added";
extern const std::string MAPVALIDATED = "map_validated";
extern const std::string ASSIGN_REINFORCEMENT = "assignReinforcement";
extern const std::string ISSUE_ORDERS = "issueOrders";
extern const std::string EXECUTE_ORDERS = "executeOrders";
extern const std::string PLAY = "play";
extern const std::string WIN = "win";
/*---------------------------------------------------------------*/

//Setter for associating CommandProcessor with GameEngine
void GameEngine::setProcessor(CommandProcessor *processor){
    commandProcessor=processor;
};

void GameEngine::setMap(Map *map){
    selectedMap=map;
}

/*--------------------------------------------------------  Phases for Part 2 -------------------------------------------------------------*/
//By Lucas
GameEngine* GameEngine::startupPhase(){

GameEngine *game =  new GameEngine();
printWelcomeMessage();
CommandProcessor *commandProcessor= new CommandProcessor();
commandProcessor->setGameEngine(game);
game->setProcessor(commandProcessor);
game->transition(START);
return game;
}

//fairly distribute all the territories to the players using BFS, (add Adjacent terrtory first)
void GameEngine::AssignTerritories() {
    cout << "Debug: AssignTerritories is running.\n";
    
    int numPlayers = playerList.size();
    int numTerritories = selectedMap->getTerritories().size();
    int targetSize = numTerritories / numPlayers;
    int extra = numTerritories % numPlayers;
    vector<Territory*> territories = selectedMap->getTerritories();

    map<string, bool> assigned;
    int playerIndex = 0;
    int territoriesAssignedToCurrentPlayer = 0;

    for (Territory* territory : territories) {
        if (assigned[territory->getName()]) continue; // Skip if already assigned

        vector<Territory*> q;
        q.push_back(territory);

        while (!q.empty() && territoriesAssignedToCurrentPlayer < targetSize + (extra > 0 ? 1 : 0)) {
            Territory* current = q.front();
            q.erase(q.begin());

            if (!assigned[current->getName()]) {
                assignTerritoryToPlayer(current, playerList[playerIndex]);
                assigned[current->getName()] = true;
                territoriesAssignedToCurrentPlayer++;
            }

            // Add unassigned neighbors to queue
            for (Territory* neighbor : current->getAdjacentTerritories()) {
                if (!assigned[neighbor->getName()]) {
                    q.push_back(neighbor);
                }
            }
        }

        // Move to the next player if current player’s quota is filled
        if (territoriesAssignedToCurrentPlayer == targetSize + (extra > 0 ? 1 : 0)) {
            if (extra > 0) extra--;
            playerIndex = (playerIndex + 1) % numPlayers;
            territoriesAssignedToCurrentPlayer = 0;
        }
    }
}

void GameEngine::assignTerritoryToPlayer(Territory* territory, Player* player) {
    player->addTerritory(territory);
    territory->setOwner(player->getName());
    territory->setPlayer(player);
}


//determine randomly the order of play of the players in the game
void GameEngine::randomizeOrderOfPlay() {

    cout << "Debug: randomizeOrderOfPlay is runing.\n";
    // using random number
     srand(static_cast<unsigned int>(time(0)));
     vector<Player*> playerListCopy(playerList);

    while (!playerListCopy.empty()) {
        // random create playerList.size() - 1 number
        int index = rand() % playerListCopy.size();
        playerOder.push_back(playerListCopy[index]);
        playerListCopy.erase(playerListCopy.begin() + index);
    }

    cout << "Order of play:\n";
    for (Player* player : playerOder) {
        if (player) { 
            cout << player->getName() << " ";
        } else {
            cout << "(null player) ";
        }
    }
    cout << endl;
}

void GameEngine::gamestart( GameEngine &game){
    game.AssignTerritories();        //fairly distribute all the territories to the players

    for(Player* player: game.playerList){
        vector<Territory*> ownedTerritory;
        ownedTerritory= (player->getOwnedTerritories());
        for(Territory* terr:ownedTerritory){
            terr->printTerritoryInfo();
            cout << "Owner of this Territory: "<<terr->getOwner() <<endl;
        }
    }
    game.randomizeOrderOfPlay();     //determine randomly the order of play of the players in the game


    //let each player draw 2 initial cards from the deck using the deck’s draw() method
    for(Player* player : game.playerOder){
        player->getHand().addCard(deck.draw());
        player->getHand().addCard(deck.draw());
    }


    //give 50 initial army units to the players, which are placed in their respective reinforcement pool
    //Tempaly add the data to the player class!
    for(Player* player : game.playerOder){
        player->setNumberOfReinforcement(50);
    }

    //switch the game to the play phase
    game.transition(PLAY);

    game.mainGameLoop();
    
    game.transition(WIN);

}

/*-------------------------------------------------------- End of Phases for Part 2 -------------------------------------------------------------*/


void GameEngine::printWelcomeMessage() {
    cout << "Welcome to the Warzone game!\n";
    cout << "Type the command follow to play the game.\n";
    cout << "loadmap <mapfile>\n";
    cout << "addplayer <playername>\n";
    cout << "gamestart\n";
    cout << "replay\n";
    cout << "quit\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
}

void GameEngine::setcurrentState(string newGameState){
    currentState=newGameState;
}

// Destructor cleans up resources
GameEngine::~GameEngine() {
    // Clean up currentPlayer, if dynamically allocated
    if (currentPlayer != nullptr) {
        delete this->currentPlayer;
        currentPlayer = nullptr;
    }
    // Clean up selectedMap, if dynamically allocated
    if (selectedMap != nullptr) {
        delete this->selectedMap;
        selectedMap = nullptr;
    }
    // Clean up playerList
    for (Player* player : playerList) {
        delete player;
    }
    playerList.clear();  // Clear the vector to remove any remaining pointers
    // Clean up eliminatedPlayers
    for (Player* player : eliminatedPlayers) {
        delete player;
    }
    eliminatedPlayers.clear();  // Clear the vector
    // The `deck` is an object, so its destructor will be automatically called,
}

string GameEngine::getCurrentState(){
    return currentState;
}



// Processes user commands
void GameEngine::handleUserCommand(const string& command, GameEngine &game) {
    if (command == "start") {
        game.startupPhase();
    } else if (command == "play") {
        game.gamestart(game);
    } else if (command == "end") {
        transition("END");
    } else {
        cout << "Invalid command.\n";
    }
}

std::vector<Player*> GameEngine::getPlayerList() const {
    return playerList;
}

//PART1
//Setter for the command processor


// Transition to the next game state
void GameEngine::transition(const string& newState) {
    setcurrentState(newState);
    cout << "Game state changed to: " << getCurrentState() << endl;
    currentState=getCurrentState();//added by Yuqian Cai
     Notify(this); // Part 5: trigger the writing of the entry in the log file 
}

// Part5: Implementing the stringToLog() function from ILoggable
string GameEngine::stringToLog() const {
    return "GameEngine: State transitions to '" + currentState + "'";
}

/*
------------------------------ Part 3 mainGameLoop() ----------------------------
Implements the main game loop following the official rules of the Warzone game.
3 phases:
- Reinforcement Phase
- Issuing Orders Phase
- Orders Execution Phase
*/
void GameEngine::mainGameLoop() {
    std::cout << "=== Main Game Loop ===" << std::endl;
    transition(ASSIGN_REINFORCEMENT);  // Start from the reinforcement phase
    bool gameOver = false;

    while (!gameOver) {
        std::cout << "\n--- New Game Phase ---\n";

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string input;
        cin>>input;
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        // Reinforcement Phase
        if (currentState == ASSIGN_REINFORCEMENT) {
            std::cout << "[INFO] Entering Reinforcement Phase..." << std::endl;
            reinforcementPhase();
            transition(ISSUE_ORDERS);
        }
        
        // Issue Orders Phase
        else if (currentState == ISSUE_ORDERS) {
            std::cout << "[INFO] Entering Issuing Orders Phase..." << std::endl;
            issueOrdersPhase();
            transition(EXECUTE_ORDERS);
        }
        
        // Execute Orders Phase
        else if (currentState == EXECUTE_ORDERS) {
            std::cout << "[INFO] Entering Orders Execution Phase..." << std::endl;
            executeOrdersPhase();
            removeEliminatedPlayer();

            // Check win condition: only one player remains
            if (playerList.size() == 1) {
                std::cout << "Player " << playerList.front()->getName() << " is the last player remaining and wins the game!" << std::endl;
                winner = playerList.front();
                gameOver = true;
                break;
            } else {
                // Check if a player controls all territories
                for (Player* player : playerList) {
                    if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size()) {
                        std::cout << "Player " << player->getName() << " controls all territories and wins the game!" << std::endl;
                        winner = player;
                        gameOver = true;
                        break;
                    }
                }
                // If no win, go back to the reinforcement phase
                if (!gameOver) {
                    transition(ASSIGN_REINFORCEMENT);
                }
            }
        }

        // Additional check to prevent infinite loop
        if (gameOver) {
            std::cout << "[INFO] Game Over detected in main loop.\n";
            break;
        }
    }

    if (gameOver) {
        std::cout << "Game ended successfully." << std::endl;
    } else {
        std::cout << "[WARN] Exiting main loop without game over. Possible logic error." << std::endl;
    }
}


/* ------------- Helper function for mainGameLoop() ---------------- */
// Enhanced `removeEliminatedPlayers` Method with Logging
void GameEngine::removeEliminatedPlayers() {
    auto it = playerList.begin();
    while (it != playerList.end()) {
        Player* player = *it;
        if (player->getOwnedTerritories().empty()) {
            std::cout << "[LOG] Removing player " << player->getName() << " as they have no territories left.\n";
            eliminatedPlayers.push_back(player);  // Track eliminated players
            it = playerList.erase(it);  // Remove player from the list
        } else {
            ++it;
        }
    }

    std::cout << "[LOG] Remaining players in the game: ";
    for (const Player* remainingPlayer : playerList) {
        std::cout << remainingPlayer->getName() << " ";
    }
    std::cout << "\n";
}

/* ------------- end of `removeEliminatedPlayers` ---------------- */

void GameEngine::removeEliminatedPlayer() {
    if (playerList.empty()) {
        std::cout << "[LOG] No players to remove.\n";
        return;
    }

    // Seed the random number generator for true randomness each time it's called
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate a random index within the bounds of the playerList
    int randomIndex = std::rand() % playerList.size();

    // Iterator to the randomly selected player
    auto it = playerList.begin();
    std::advance(it, randomIndex);

    // Remove and log the player
    Player* player = *it;
    eliminatedPlayers.push_back(player);
    playerList.erase(it);

    // Log remaining players
    std::cout << "[LOG] Remaining players in the game: ";
    for (const Player* remainingPlayer : playerList) {
        std::cout << remainingPlayer->getName() << " ";
    }
    std::cout << "\n";
}


/* ---- Helper function, After win. if choose replay,reset the game ---- */
void GameEngine::resetGame() {
    // Reset the winner
    winner = nullptr;

    // Clear player list
    for (Player* player : playerList) {
        delete player;  // Free dynamically allocated memory
    }
    playerList.clear();

    // Clear eliminated players
    for (Player* player : eliminatedPlayers) {
        delete player;  // Free dynamically allocated memory
    }
    eliminatedPlayers.clear();

    // Reset the map
    if (selectedMap != nullptr) {
        delete selectedMap;  // Free dynamically allocated memory for the map
        selectedMap = nullptr;
    }

    // Reset other game-related data
    playerOder.clear();  // Clear the player order vector
    currentState = START;  // Reset the state to START

    // Optionally reset the deck or other components if needed
    deck = Deck();  // Reinitialize the deck with default cards
}

/* 
------------------------------ Part 3 reinforcementPhase() -----------------------------
1) Players are given a number of army units that depends on the number of territories they own, 
(# of territories owned divided by 3, rounded down).
2) If a player owns all territories on a continent, they receive additional army units equal to that continent’s control bonus.
3) Add the calculated army units to the player's reinforcement pool each turn.
*/
void GameEngine::reinforcementPhase() {
    cout << "=== Reinforcement Phase ===" << endl;
   
    // Iterate over each player in the game
    for (Player* player : playerList) {
        // Skip players without territories since they will be eliminated in executeOrdersPhase()
        if (player->getOwnedTerritories().empty()) {
            cout << player->getName() << " has no territories and will be removed at the end of executeOrdersPhase." << endl;
            continue;
        }

        // Reinforcements based on the number of territories
        int numTerritories = player->getOwnedTerritories().size();
        int reinforcementFromTerritories = max(3, numTerritories / 3); // Minimum of 3 reinforcements
        int reinforcementFromContinents = 0;

        // Reinforcements from controlled continents
        for (Continent* continent : selectedMap->getContinents()) {
            bool controlsContinent = true;
            
            // Verify if the player owns all territories in the continent
            for (Territory* territory : continent->getTerritories()) {
                if (territory->getOwner() != player->getName()) {
                    controlsContinent = false;
                    break;
                }
            }

            if (controlsContinent) {
                reinforcementFromContinents += continent->getControlValue();
                cout << player->getName() << " controls the continent: " << continent->getName()
                     << " and receives " << continent->getControlValue() << " additional reinforcements." << endl;
            }
        }

        // Total reinforcements
        int totalReinforcements = reinforcementFromTerritories + reinforcementFromContinents;
        player->setNumberOfReinforcement(player->getNumberOfReinforcement() + totalReinforcements);

        cout << player->getName() << " receives total " << totalReinforcements
             << " reinforcement units (" << reinforcementFromTerritories << " from territories, "
             << reinforcementFromContinents << " from continents)." << endl;
    }

    cout << "=== End of Reinforcement Phase ===" << endl;
}

// g++ -std=c++11 GameEngineDriver.cpp GameEngine.cpp Map.cpp Player.cpp Orders.cpp
/* 
------------------------------ Part 3 issueOrdersPhase() -----------------------------
1) Players issue orders and place them in their order list through a call to the Player::issueOrder() method.
2) This method is called in round-robin fashion across all players by the game engine.
   -> Round-robin means each player takes turns issuing one order at a time.
3) This phase ends when all players have signified that they don’t have any more orders to issue for this turn. 
4) It will call a function/method named issueOrdersPhase() in the game engine.
*/

// /* Method with log */
std::map<Player*, int> savedReinforcements;

void GameEngine::issueOrdersPhase() {
    std::cout << "=== Starting Issue Orders Phase ===\n";

    // Step 1: Save the initial reinforcement count for each player
    std::map<Player*, int> savedReinforcements;
    for (Player* player : playerList) {
        savedReinforcements[player] = player->getNumberOfReinforcement();
    }

    bool ordersPending;
    int round = 1;

    // Continue issuing orders in a round-robin fashion until no orders are pending
    do {
        std::cout << "\n--- Round " << round++ << " ---\n";
        ordersPending = false;

        for (Player* player : playerList) {
            // Check if the player has more orders to issue
            if (player->hasMoreOrders()) {
                std::cout << player->getName() << " is issuing an order...\n";
                player->issueOrder();  // Issue one order for the player
                ordersPending = true;  // Flag that there are still orders pending
            } 
        }
    } while (ordersPending);
    // /* For debug */
    // for (Player* player : playerList) {
    //     std::cout << "\n[INFO] " << player->getName() << " has completed issuing all orders. \n";
    //         std::cout << " --- Here is information of " << player->getName() << " --- \n";
    //         player->printPlayerInfo();
    // }

    std::cout << "=== End of Issue Orders Phase ===\n";

    // Store the saved reinforcement data in a member variable if you need to access it later
    this->savedReinforcements = savedReinforcements;
}


/* 
------------------------------ Part 3 executeOrdersPhase() -----------------------------
1)In a round-robin fashion, each player executes one order at a time from their order list.
2)Deploy orders are prioritized and executed first before any other type of order.
3)Each order is validated and executed (e.g., deploying armies, advancing troops, using special orders).
4)The phase repeats until all players have no more orders to execute.
*/
/* Method with log*/
void GameEngine::executeOrdersPhase() {
    std::cout << "=== Starting Order Execution Phase ===\n";

    // Restore each player's reinforcement count before executing orders
    for (Player* player : playerList) {
        if (savedReinforcements.find(player) != savedReinforcements.end()) {
            player->setNumberOfReinforcement(savedReinforcements[player]);
        }
    }

    bool ordersRemaining;
    int round = 1;  // Track rounds for clarity

    do {
        std::cout << "\n--- Execution Round " << round++ << " ---\n";
        ordersRemaining = false;

        for (Player* player : playerList) {
            if (player->getOrders().hasMoreOrders()) {
                Order* order = player->getOrders().getNextOrder();
                if (order) {
                    std::cout << "\n" << player->getName() << " is executing the order.\n";
                    order->execute();
                    ordersRemaining = true;
                }
            } else {
                std::cout << player->getName() << " has no more orders to execute.\n";
            }
        }
    } while (ordersRemaining);

    std::cout << "=== Order Execution Phase Complete ===\n";
}


/*------------------------------- End of Methods for P3 ----------------------------------------*/