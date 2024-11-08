#include "GameEngine.h"
#include "MapDriver.h"
#include "Orders.h"
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <iostream>
#include <algorithm>  // For max
#include "CommandProcessor.h"

using namespace std;
///
//g++ GameEngine.cpp GameEngineDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp

/*-------------------- Phases for Part 3 -------------------------*/
const std::string START = "start";
const std::string ASSIGN_REINFORCEMENT = "assignReinforcement";
const std::string ISSUE_ORDERS = "issueOrders";
const std::string EXECUTE_ORDERS = "executeOrders";
const std::string WIN = "win";
/*---------------------------------------------------------------*/

// Part 2: Game startup phase
// Provide a startupPhase() method in the GameEngine class that implements a command-based user interaction
// mechanism to start the game by allowing the user to proceed with the game startup phase:
// 1) use the loadmap <filename> command to select a map from a list of map files as stored in a directory,
// which results in the map being loaded in the game.
// 2) use the validatemap command to validate the map (i.e. it is a connected graph, etc – see assignment 1).
// 3) use the addplayer <playername> command to enter players in the game (2-6 players)
// 4) use the gamestart command to
// a) fairly distribute all the territories to the players
// b) determine randomly the order of play of the players in the game
// c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
// d) let each player draw 2 initial cards from the deck using the deck’s draw() method
// e) switch the game to the play phase
// This must be implemented as part of the pre-existing .cpp/.h file duo named GameEngine.cpp/GameEngine.h
// You must deliver a driver as a free function named testStartupPhase() that demonstrates that 1-4 explained
// above are implemented correctly, using either console input or file input of the commands (see Part 1). This driver
// function must be in the GameEngineDriver.cpp file. 

void GameEngine::startupPhase(){

cout << "Please select the name of the map you want to load: ";
        string mapName;
        cin >> mapName;

        selectedMap = selectedMap -> loadMapFromFile(mapName);
        
        bool result =  (selectedMap == nullptr);
        if (!result) {
            cout << "Map " << mapName << " loaded successfully!\n";
            setcurrentState("MAP_LOADED");
        } else {
            cout << "Failed to load the map. Please try again.\n";
        }

while (currentState == "MAP_LOADED") {     //state2
        cout << "Validating the map...\n";
          
        bool mapValidated = selectedMap->validate(); 

        if (mapValidated) {
            cout << "Map validated successfully!\n";
            cout << "Change the Game state to Addplayer \n";
            cout << "Add 2-6 players \n";
            currentState = "ADD_Player";            //state3
        } else {
            cout << "Map validation failed. Please load a valid map.\n";
            currentState = "START"; 
        }
    }

    // Adding players
    string playerName;
    while (true) {
        if (playerList.size()==6){break;}
        cout << "Enter player name (or 'done' to finish): ";
        cin >> playerName;
        if (playerName == "done") {if(playerList.size()>=2){break;} else cout << "Need at least 2 players.\n";}
        
        else
            {playerList.push_back(new Player(playerName));
            cout << "Player " << playerName << " added.\n";}
        
        
        cout << "Total number of play\n" << playerList.size()<< endl;
        for(Player* player : playerList){
                cout << "Player: " << player->getName() << " \n";
            }
    }

    if (!playerList.empty()) {
        transition("PLAYERS_ADDED");     //state4
    } else {
        cout << "No players added. Exiting...\n";
        return;
    }


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
}


//determine randomly the order of play of the players in the game
void GameEngine::randomizeOrderOfPlay() {

    cout << "Debug: randomizeOrderOfPlay is runing.\n";
    // using random number
     srand(static_cast<unsigned int>(time(0)));
     vector<Player*> playerListCopy(playerList);
    cout << "Debug: randomizeOrderOfPlay is runing.\n";

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

    for(Player* player:playerList){
        vector<Territory*> ownedTerritory;
        ownedTerritory= (player->getOwnedTerritories());
        for(Territory* terr:ownedTerritory){
            terr->printTerritoryInfo();
            cout << "Owner of this Territory: "<<terr->getOwner() <<endl;
        }
    }
    game.randomizeOrderOfPlay();     //determine randomly the order of play of the players in the game


    //let each player draw 2 initial cards from the deck using the deck’s draw() method
    for(Player* player : playerOder){
        player->getHand().addCard(deck.draw());
        player->getHand().addCard(deck.draw());
        // need to fix since right now each deck only have 5 card
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }



    //give 50 initial army units to the players, which are placed in their respective reinforcement pool
    //Tempaly add the data to the player class!
    for(Player* player : playerOder){
        player->setNumberOfReinforcement(50);
    }




    //switch the game to the play phase
    currentState = "Play";

}

<<<<<<< HEAD
/*-------------------------------------------------------- End of Phases for Part 2 -------------------------------------------------------------*/
// By lucasS
=======

>>>>>>> 3a94b2a1b3a5eba0c0a6953f774e8b6011bc4a18

void GameEngine::printWelcomeMessage() {
    cout << "Welcome to the Warzone game!\n";
    cout << "Type 'start' to start the game.\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
    printWelcomeMessage();
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
//adjust----

// Main gameplay loop
void GameEngine::promptNextActionPlay() {
    
    transition("reinforcement");// Start from the reinforcement phase
    //setcurrentState("reinforcement");// Start from the reinforcement phase
    
    while (getCurrentState() != "win") {
        // Handle the Reinforcement phase
        if (getCurrentState() == "reinforcement") {
            reinforcementPhase();  // Assign reinforcements
        }
        // Handle the Issue Orders phase
        else if (getCurrentState() == "issue orders") {
            issueOrdersPhase();  // Players issue orders
        }
        // Handle the Execute Orders phase
        else if (getCurrentState() == "execute orders") {
            executeOrdersPhase();  // Execute orders

            cout << "Press Enter to 'end' to the end the game or enter 'continue' to continue playing ...\n";
            string userInput;
            cin>> userInput;

            if(userInput=="end"){
                transition("win");
            }
        }


        // If a player has won, transition to the win state
        //checkWinCondition();
        
        // // Ask for confirmation before moving to the next phase
        // if (currentState != "win") {
        //     cout << "Press Enter to proceed to the next phase...\n";
        //     cin.ignore();
        //     string userInput;
        //     getline(cin, userInput);  // Wait for user input (pressing Enter)
        // }
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    cout << "Game Over! You have won!\n";
}

// New function to check win condition
void GameEngine::checkWinCondition() {
    for (Player* player : playerList) {
        // Check if the player owns all territories (winning condition)
        if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size()) {
            cout << "Player " << player->getName() << " has won the game!\n";
            transition("win");  // Transition to the win state
            break;
        }
    }
}

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
     cout << "=== Main Game Loop ===" << endl;
    transition(ASSIGN_REINFORCEMENT);  // Start from the reinforcement phase
    bool gameOver = false;

    while (!gameOver) {
        // Reinforcement Phase
        if (currentState == ASSIGN_REINFORCEMENT) {
            std::cout << "Entering Reinforcement Phase..." << std::endl;
            reinforcementPhase();
            transition(ISSUE_ORDERS);  // Move to the next phase
        }
        // Issuing Orders Phase
        else if (currentState == ISSUE_ORDERS) {
            std::cout << "Entering Issuing Orders Phase..." << std::endl;
            issueOrdersPhase();
            transition(EXECUTE_ORDERS);  // Move to the next phase
        }
        // Orders Execution Phase
        else if (currentState == EXECUTE_ORDERS) {
            std::cout << "Entering Orders Execution Phase..." << std::endl;
            executeOrdersPhase();

            // Check and remove players with no territories after execution
            removeEliminatedPlayers();

            // Check if only one player remains
            if (playerList.size() == 1) {
                std::cout << "Player " << playerList.front()->getName() << " is the last player remaining and wins the game!" << std::endl;
                winner = playerList.front();
                transition(WIN);  // Transition to win state
            } else {
                transition(ASSIGN_REINFORCEMENT);  // Restart the loop from the reinforcement phase
            }
        }

        // Check Win Condition: A player controls all territories
        for (Player* player : playerList) {
            if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size()) {
                std::cout << "Player " << player->getName() << " controls all territories! They win the game!" << std::endl;
                winner = player;
                transition(WIN);  // Transition to win state
                break;
            }
        }

        // Handle the WIN state
        if (currentState == WIN) {
            std::cout << "Game Over! Do you want to replay or quit? (replay/quit): ";
            std::string userInput;
            std::cin >> userInput;

            if (userInput == "replay") {
                std::cout << "Restarting the game..." << std::endl;

                // Reset everything for a new game
                resetGame();
                transition(START);  // Go back to the start state
                startupPhase();       // Restart the startup phase
                transition(ASSIGN_REINFORCEMENT);  // Transition to the play phase
            } else if (userInput == "quit") {
                std::cout << "Quitting the game. Thanks for playing!" << std::endl;
                gameOver = true;
            }
        }
    }
}

/* ------------- Helper function for mainGameLoop() ---------------- */
void GameEngine::removeEliminatedPlayers() {
    auto it = playerList.begin();
    while (it != playerList.end()) {
        Player* player = *it;
        if (player->getOwnedTerritories().empty()) {
            std::cout << player->getName() << " has no territories and is eliminated from the game." << std::endl;
            it = playerList.erase(it);  // Remove player from the list
            eliminatedPlayers.push_back(player);  // Optionally track eliminated players
        } else {
            ++it;
        }
    }
}

/* ---- Helper function for mainGameLoop(), After win. if choose replay,reset the game ---- */
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

        cout << player->getName() << " receives " << totalReinforcements
             << " reinforcement units (" << reinforcementFromTerritories << " from territories, "
             << reinforcementFromContinents << " from continents)." << endl;
    }

    cout << "=== End of Reinforcement Phase ===" << endl;
}



/* 
------------------------------ Part 3 issueOrdersPhase() -----------------------------
1) Players issue orders and place them in their order list through a call to the Player::issueOrder() method.
2) This method is called in round-robin fashion across all players by the game engine.
   -> Round-robin means each player takes turns issuing one order at a time.
3) This phase ends when all players have signified that they don’t have any more orders to issue for this turn. 
4) It will call a function/method named issueOrdersPhase() in the game engine.
*/
void GameEngine::issueOrdersPhase() {
    cout << "=== Starting Issue Orders Phase ===\n";

    bool ordersPending;
    int round = 1;  // Track the round number for clarity

    // Continue issuing orders in a round-robin manner until no orders are pending
    do {
        cout << "\n--- Round " << round++ << " ---\n";
        ordersPending = false;

        for (Player* player : playerList) {
            // Check if the player has any orders to issue
            if (player->hasMoreOrders()) {
                cout << player->getName() << " is issuing an order...\n";
                player->issueOrder();  // Issue one order for the player
                ordersPending = true;  // Flag that there are still orders pending
            } else {
                cout << player->getName() << " has no more orders to issue.\n";
            }
        }
    } while (ordersPending);  // Continue rounds until no orders are pending

    cout << "=== End of Issue Orders Phase ===\n";
    
    // Transition to the next phase after issuing orders
    transition(EXECUTE_ORDERS);
}

/* 
------------------------------ Part 3 executeOrdersPhase() -----------------------------
1) Once all the players have signified in the same turn that they are not issuing one more order,
the game engine proceeds to execute the top order on the list of orders of each player in a round-robin fashion
2) Once all the players’ orders have been executed, the main game loop goes back to the reinforcement phase.
3) When the game engine asks the player to give them their next order, the player returns the next order in their order list. 
Once the game engine receives the order, it calls execute() on the order, which should first validate the order, 
then enact the order (see Part 4: orders execution implementation) and record a narrative of its effect stored in the order object. 
The game engine should execute all the deploy orders before it executes any other kind of order. 
This goes on in round-robin fashion across the players until all the players’ orders have been executed.
*/
void GameEngine::executeOrdersPhase() {
    std::cout << "=== Starting Order Execution Phase ===\n";

    bool ordersRemaining;
    int round = 1;  // Track rounds for clarity

    // Continue executing orders in a round-robin manner until no orders remain
    do {
        std::cout << "\n--- Execution Round " << round++ << " ---\n";
        ordersRemaining = false;

        // Loop through each player to execute one order per player in a round-robin fashion
        for (Player* player : playerList) {
            orderList& orders = player->getOrders();  // Get the player's order list

            // Check if the player has any orders to execute
            if (orders.hasMoreOrders()) {
                // Execute the next order in the player's list
                Order* currentOrder = orders.getNextOrder();
                if (currentOrder != nullptr) {
                    currentOrder->execute();  // Execute the order
                    std::cout << player->getName() << " executed order: " << currentOrder->toString() << "\n";
                    delete currentOrder;  // Clean up the order after execution
                }
                ordersRemaining = true;  // Mark that at least one player had an order
            } else {
                std::cout << player->getName() << " has no more orders to execute.\n";
            }
        }
    } while (ordersRemaining);  // Continue until no players have orders remaining

    std::cout << "=== Order Execution Phase Complete ===\n";
    
    // Transition back to the reinforcement phase after execution is complete
    transition(ASSIGN_REINFORCEMENT);
}
/*------------------------------- End of Methods for P3 ----------------------------------------*/