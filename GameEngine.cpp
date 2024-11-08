#include "GameEngine.h"
#include "MapDriver.h"
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <cstdlib>   // std::rand, std::srand
#include <ctime>     // std::time
#include <iostream>

//g++ GameEngine.cpp GameEngineDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp

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

std::cout << "Please select the name of the map you want to load: ";
        std::string mapName;
        std::cin >> mapName;

        selectedMap = selectedMap -> loadMapFromFile(mapName);
        
        bool result =  (selectedMap == nullptr);
        if (!result) {
            std::cout << "Map " << mapName << " loaded successfully!\n";
            setcurrentState("MAP_LOADED");
        } else {
            std::cout << "Failed to load the map. Please try again.\n";
        }

while (currentState == "MAP_LOADED") {     //state2
        std::cout << "Validating the map...\n";
          
        bool mapValidated = selectedMap->validate(); 

        if (mapValidated) {
            std::cout << "Map validated successfully!\n";
            std::cout << "Change the Game state to Addplayer \n";
            std::cout << "Add 2-6 players \n";
            currentState = "ADD_Player";            //state3
        } else {
            std::cout << "Map validation failed. Please load a valid map.\n";
            currentState = "START"; 
        }
    }

    // Adding players
    std::string playerName;
    while (true) {
        if (playerList.size()==6){break;}
        std::cout << "Enter player name (or 'done' to finish): ";
        std::cin >> playerName;
        if (playerName == "done") {if(playerList.size()>2){break;} else std::cout << "Need at least 2 players.\n";}
        playerList.push_back(new Player(playerName));
        std::cout << "Player " << playerName << " added.\n";
        
        std::cout << "Total number of play\n" << playerList.size()<< endl;
        for(Player* player : playerList){
                std::cout << "Player: " << player->getName() << " \n";
            }
    }

    if (!playerList.empty()) {
        transitionTo("PLAYERS_ADDED");     //state4
    } else {
        std::cout << "No players added. Exiting...\n";
        return;
    }


}
//fairly distribute all the territories to the players using BFS, (add Adjacent terrtory first)
void GameEngine::AssignTerritories(){

    std::cout << "Debug: AssignTerritories is runing.\n";
    int numPlayers = playerList.size();
    int numTerritories = selectedMap->getTerritories().size();
    int targetSize = numTerritories / numPlayers;
    int extra = numTerritories % numPlayers;
    std::vector<Territory*> territories = selectedMap->getTerritories();

    std::map <std::string, bool> assigned;
    int playerIndex = 0;

    for (Territory* territory : territories){
        
        if (assigned[territory->getName()]) continue;
        

            std::queue<Territory*> q;
            q.push(territory);
            int territoriesAssignedToCurrentPlayer = 0;


            while (!q.empty() && territoriesAssignedToCurrentPlayer < targetSize + (extra > 0 ? 1 : 0)) {
                //if extra if bigger than 0, then take 1 give to play
                
                Territory* current = q.front();
                
                q.pop();
                if (assigned[territory->getName()]) continue;
                playerList[playerIndex]->addTerritory(current);
                current->setOwner(playerList[playerIndex]->getName());
                //asign current territory to player and set territory owner to play


                assigned[current->getName()] = true;
                territoriesAssignedToCurrentPlayer++;
                
                for (Territory* neighbor : current->getAdjacentTerritories()) {
                    
                    if ( !assigned[neighbor->getName()]) {q.push(neighbor);}
                    else continue;
                }
            
        }

        if (extra > 0) extra--;
        playerIndex = (playerIndex + 1) % numPlayers;


    }
        
}


//determine randomly the order of play of the players in the game
void GameEngine::randomizeOrderOfPlay() {

    std::cout << "Debug: randomizeOrderOfPlay is runing.\n";
    // using random number
     std::srand(static_cast<unsigned int>(std::time(0)));
     std::vector<Player*> playerListCopy(playerList);
    std::cout << "Debug: randomizeOrderOfPlay is runing.\n";

    while (!playerListCopy.empty()) {
        // random create playerList.size() - 1 number
        int index = std::rand() % playerListCopy.size();
        playerOder.push_back(playerListCopy[index]);
        playerListCopy.erase(playerListCopy.begin() + index);
    }

    std::cout << "Order of play:\n";
    for (Player* player : playerOder) {
        if (player) { 
            std::cout << player->getName() << " ";
        } else {
            std::cout << "(null player) ";
        }
    }
    std::cout << std::endl;
}

void GameEngine::gamestart( GameEngine &game){
    game.AssignTerritories();        //fairly distribute all the territories to the players

    for(Player* player:playerList){
        std::vector<Territory*> ownedTerritory;
        ownedTerritory= (player->getOwnedTerritories());
        for(Territory* terr:ownedTerritory){
            terr->printTerritoryInfo();
            std::cout << "Owner of this Territory: "<<terr->getOwner() <<endl;
        }
    }
    game.randomizeOrderOfPlay();     //determine randomly the order of play of the players in the game


    //let each player draw 2 initial cards from the deck using the deck’s draw() method
    for(Player* player : playerOder){
        player->getHand().addCard(deck.draw());
        player->getHand().addCard(deck.draw());
    }



    //give 50 initial army units to the players, which are placed in their respective reinforcement pool
    for(Player* player : playerOder){
        player->setNumberOfReinforcement(50);
    }




    //switch the game to the play phase
    currentState = "Play";

}



void GameEngine::printWelcomeMessage() {
    std::cout << "Welcome to the Warzone game!\n";
    std::cout << "Type 'start' to start the game.\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
    printWelcomeMessage();
}

void GameEngine::setcurrentState(std::string newGameState){
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

std::string GameEngine::getCurrentState(){
    return currentState;
}



// Processes user commands
void GameEngine::handleUserCommand(const std::string& command, GameEngine &game) {
    if (command == "start") {
        game.startupPhase();
    } else if (command == "play") {
        game.gamestart(game);
    } else if (command == "end") {
        transitionTo("END");
    } else {
        std::cout << "Invalid command.\n";
    }
}
//adjust----

// Main gameplay loop
void GameEngine::promptNextActionPlay() {
    
    transitionTo("reinforcement");// Start from the reinforcement phase
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

            std::cout << "Press Enter to 'end' to the end the game or enter 'continue' to continue playing ...\n";
            std::string userInput;
            std::cin>> userInput;

            if(userInput=="end"){
                transitionTo("win");
            }
        }


        // If a player has won, transition to the win state
        //checkWinCondition();
        
        // // Ask for confirmation before moving to the next phase
        // if (currentState != "win") {
        //     std::cout << "Press Enter to proceed to the next phase...\n";
        //     std::cin.ignore();
        //     std::string userInput;
        //     std::getline(std::cin, userInput);  // Wait for user input (pressing Enter)
        // }
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    std::cout << "Game Over! You have won!\n";
}

// Reinforcement phase with looping mechanism
void GameEngine::reinforcementPhase() {
    std::cout << "Entering Reinforcement Phase...\n";
    bool done1 = false;
    
    while (!done1) {
        std::cout << "Enter 'next' to finish the Reinforcement Phase or type any command for actions...\n";
        std::string userInput2;
        std::cin >> userInput2;
        
        if (userInput2=="next") {  // If user presses Enter without input, end the phase
            done1 = true;}
        else {
             // Here you can handle any additional reinforcement actions
             std::cout << "Reinforcement Action: " << userInput2 << std::endl;
             }
    }
    transitionTo("issue orders");
}

// Issuing Orders phase with looping mechanism
void GameEngine::issueOrdersPhase() {
    std::cout << "Entering Issue Orders Phase...\n";
    bool done2 = false;
    while (!done2) {
        std::cout << "Press 'next' to finish the Issue Orders Phase or type any command for actions...\n";

        std::string userInput3;
        std::cin>> userInput3;
        
        if (userInput3=="next") {  // If user presses Enter without input, end the phase
            done2 = true;
        } else {
            // Here you can handle any additional issue order actions
            std::cout << "Issue Order Action: " << userInput3 << std::endl;
        }
    }
    transitionTo("execute orders");
}

// Execute Orders phase - integrated logic for card drawing and playing
void GameEngine::executeOrdersPhase() {
    std::cout << "Entering Execute Orders Phase...\n";
    
    for (Player* player : playerList) {
        std::cout << "Player " << player->getName() << " is drawing cards...\n";
        
        Hand hand;  // Initialize the player's hand

        // Each player draws 3 cards
        for (int i = 0; i < 3; ++i) {
            Card* drawnCard = deck.draw();  // Drawing from the deck
            hand.addCard(drawnCard);
            std::cout << "Player " << player->getName() << " drew a card: " << drawnCard->getType() << std::endl;
        }

        // Play all cards in the hand
        // while (!hand.isEmpty()) {
        //     // hand.playFirstCard(deck);  // Assuming this method is implemented as we discussed earlier
        // }
    }

}

// New function to check win condition
void GameEngine::checkWinCondition() {
    for (Player* player : playerList) {
        // Check if the player owns all territories (winning condition)
        if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size()) {
            std::cout << "Player " << player->getName() << " has won the game!\n";
            transitionTo("win");  // Transition to the win state
            break;
        }
    }
}

// Transition to the next game state
void GameEngine::transitionTo(const std::string& newState) {
    setcurrentState(newState);
    std::cout << "Game state changed to: " << getCurrentState() << std::endl;
}


