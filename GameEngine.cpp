#include "GameEngine.h"
#include "MapDriver.h"
#include "Orders.h"
#include "Player.h"
#include "CommandProcessor.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <algorithm> // For max

using namespace std;
///
// g++ -o main GameEngine.cpp CommandProcessor.cpp CommandProcessingDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp LoggingObserver.cpp

/*-------------------- Phases for Part 3 -------------------------*/
extern const string START = "start";
extern const string MAPLODADED = "map_loaded";
extern const string GAMESTART = "gamestart";
extern const string PLAYERSADDED = "players_added";
extern const string MAPVALIDATED = "map_validated";
extern const string ASSIGN_REINFORCEMENT = "assignReinforcement";
extern const string ISSUE_ORDERS = "issueOrders";
extern const string EXECUTE_ORDERS = "executeOrders";
extern const string PLAY = "play";
extern const string WIN = "win";
/*---------------------------------------------------------------*/

TournamentParameters params;

// Setter for associating CommandProcessor with GameEngine
void GameEngine::setProcessor(CommandProcessor *processor)
{
    commandProcessor = processor;
};

void GameEngine::setMap(Map *map)
{
    selectedMap = map;
}

/*--------------------------------------------------------  Phases for Part 2 -------------------------------------------------------------*/
// By Lucas
GameEngine *GameEngine::startupPhase()
{

    GameEngine *game = new GameEngine();
    printWelcomeMessage();
    CommandProcessor *commandProcessor = new CommandProcessor();
    commandProcessor->setGameEngine(game);
    game->setProcessor(commandProcessor);
    game->transition(START);
    return game;
}

// fairly distribute all the territories to the players using BFS, (add Adjacent terrtory first)
void GameEngine::AssignTerritories()
{
    cout << "Debug: AssignTerritories is running.\n";

    int numPlayers = playerList.size();
    int numTerritories = selectedMap->getTerritories().size();
    int targetSize = numTerritories / numPlayers;
    int extra = numTerritories % numPlayers;
    vector<Territory *> territories = selectedMap->getTerritories();

    map<string, bool> assigned;
    int playerIndex = 0;
    int territoriesAssignedToCurrentPlayer = 0;

    for (Territory *territory : territories)
    {
        if (assigned[territory->getName()])
            continue; // Skip if already assigned

        vector<Territory *> q;
        q.push_back(territory);

        while (!q.empty() && territoriesAssignedToCurrentPlayer < targetSize + (extra > 0 ? 1 : 0))
        {
            Territory *current = q.front();
            q.erase(q.begin());

            if (!assigned[current->getName()])
            {
                assignTerritoryToPlayer(current, playerList[playerIndex]);
                assigned[current->getName()] = true;
                territoriesAssignedToCurrentPlayer++;
            }

            // Add unassigned neighbors to queue
            for (Territory *neighbor : current->getAdjacentTerritories())
            {
                if (!assigned[neighbor->getName()])
                {
                    q.push_back(neighbor);
                }
            }
        }

        // Move to the next player if current player’s quota is filled
        if (territoriesAssignedToCurrentPlayer == targetSize + (extra > 0 ? 1 : 0))
        {
            if (extra > 0)
                extra--;
            playerIndex = (playerIndex + 1) % numPlayers;
            territoriesAssignedToCurrentPlayer = 0;
        }
    }
}

void GameEngine::assignTerritoryToPlayer(Territory *territory, Player *player)
{
    player->addTerritory(territory);
    territory->setOwner(player->getName());
    territory->setPlayer(player);
}

// determine randomly the order of play of the players in the game
void GameEngine::randomizeOrderOfPlay()
{

    cout << "Debug: randomizeOrderOfPlay is runing.\n";
    // using random number
    srand(static_cast<unsigned int>(time(0)));
    vector<Player *> playerListCopy(playerList);

    while (!playerListCopy.empty())
    {
        // random create playerList.size() - 1 number
        int index = rand() % playerListCopy.size();
        playerOder.push_back(playerListCopy[index]);
        playerListCopy.erase(playerListCopy.begin() + index);
    }

    cout << "Order of play:\n";
    for (Player *player : playerOder)
    {
        if (player)
        {
            cout << player->getName() << " ";
        }
        else
        {
            cout << "(null player) ";
        }
    }
    cout << endl;
}

void GameEngine::gamestart(GameEngine &game)
{
    //  //ASG3_part2: Tournament Mode adding players:
    //  if(tournamentMode)
    //  {initializeTournamentPlayers(params.playerStrategies);}

    this->AssignTerritories(); // fairly distribute all the territories to the players

    for (Player *player : this->playerList)
    {
        vector<Territory *> ownedTerritory;
        ownedTerritory = (player->getOwnedTerritories());
        for (Territory *terr : ownedTerritory)
        {
            terr->printTerritoryInfo();
            cout << "Owner of this Territory: " << terr->getOwner() << endl;
        }
    }
    this->randomizeOrderOfPlay(); // determine randomly the order of play of the players in the game

    // let each player draw 2 initial cards from the deck using the deck’s draw() method
    for (Player *player : this->playerOder)
    {
        player->getHand().addCard(deck.draw());
        player->getHand().addCard(deck.draw());
    }

    // give 50 initial army units to the players, which are placed in their respective reinforcement pool
    // Tempaly add the data to the player class!
    for (Player *player : game.playerOder)
    {
        player->setNumberOfReinforcement(50);
    }

    // switch the game to the play phase
    game.transition(PLAY);

    game.mainGameLoop();

    game.transition(WIN);
}

/*-------------------------------------------------------- End of Phases for Part 2 -------------------------------------------------------------*/

void GameEngine::printWelcomeMessage()
{
    cout << "Welcome to the Warzone game!\n";
    cout << "Type the command follow to play the game.\n";
    cout << "tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\n";

    cout << "loadmap <mapfile>\n";
    cout << "addplayer <playername>\n";
    cout << "gamestart\n";
    cout << "replay\n";
    cout << "quit\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr)
{
}

void GameEngine::setcurrentState(string newGameState)
{
    currentState = newGameState;
}

// Destructor cleans up resources
GameEngine::~GameEngine()
{
    // Clean up currentPlayer, if dynamically allocated
    if (currentPlayer != nullptr)
    {
        delete this->currentPlayer;
        currentPlayer = nullptr;
    }
    // Clean up selectedMap, if dynamically allocated
    if (selectedMap != nullptr)
    {
        delete this->selectedMap;
        selectedMap = nullptr;
    }
    // Clean up playerList
    for (Player *player : playerList)
    {
        delete player;
    }
    playerList.clear(); // Clear the vector to remove any remaining pointers
    // Clean up eliminatedPlayers
    for (Player *player : eliminatedPlayers)
    {
        delete player;
    }
    eliminatedPlayers.clear(); // Clear the vector
    // The `deck` is an object, so its destructor will be automatically called,
}

string GameEngine::getCurrentState()
{
    return currentState;
}

// Processes user commands
void GameEngine::handleUserCommand(const string &command, GameEngine &game)
{
    if (command == "start")
    {
        game.startupPhase();
    }
    else if (command == "play")
    {
        game.gamestart(game);
    }
    else if (command == "end")
    {
        transition("END");
    }
    else
    {
        cout << "Invalid command.\n";
    }
}

vector<Player *> GameEngine::getPlayerList() const
{
    return playerList;
}

// PART1
// Setter for the command processor

// Transition to the next game state
void GameEngine::transition(const string &newState)
{
    setcurrentState(newState);
    cout << "Game state changed to: " << getCurrentState() << endl;
    currentState = getCurrentState(); // added by Yuqian Cai
    Notify(this);                     // Part 5: trigger the writing of the entry in the log file
}

// Part5: Implementing the stringToLog() function from ILoggable
string GameEngine::stringToLog() const
{
    if (tournamentMode)
        return displayTournamentResults();

    else
    {
        return "GameEngine: State transitions to '" + currentState + "'";
    }
}

/*
------------------------------ Part 3 mainGameLoop() ----------------------------
Implements the main game loop following the official rules of the Warzone game.
3 phases:
- Reinforcement Phase
- Issuing Orders Phase
- Orders Execution Phase
*/
void GameEngine::mainGameLoop()
{
    cout << "=== Main Game Loop ===" << endl;
    transition(ASSIGN_REINFORCEMENT); // Start from the reinforcement phase
    bool gameOver = false;
        int currentTurn = 0;  // Initialize the turn counter

    cout << "Starting game loop with a max of " << params.maxTurns << " turns." << endl;
    while (!gameOver && currentTurn < params.maxTurns)
    {
        cout << "\n--- New Game Phase ---\n";

        // Reinforcement Phase
        if (currentState == ASSIGN_REINFORCEMENT)
        {
            cout << "[INFO] Entering Reinforcement Phase..." << endl;
            reinforcementPhase();
            transition(ISSUE_ORDERS);
        }

        // Issue Orders Phase
        else if (currentState == ISSUE_ORDERS)
        {
            cout << "[INFO] Entering Issuing Orders Phase..." << endl;
            issueOrdersPhase();
            transition(EXECUTE_ORDERS);
        }

        // Execute Orders Phase
        else if (currentState == EXECUTE_ORDERS)
        {
            cout << "[INFO] Entering Orders Execution Phase..." << endl;
            executeOrdersPhase();
            removeEliminatedPlayers();

            // Check win condition: only one player remains
            if (playerList.size() == 1)
            {
                cout << "Player " << playerList.front()->getName() << " is the last player remaining and wins the game!" << endl;
                winner = playerList.front();
                gameOver = true;
                break;
            }
            else
            {
                // Check if a player controls all territories
                for (Player *player : playerList)
                {
                    if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size())
                    {
                        cout << "Player " << player->getName() << " controls all territories and wins the game!" << endl;
                        winner = player;
                        gameOver = true;
                        break;
                    }
                }
                // If no win, go back to the reinforcement phase
                if (!gameOver)
                {
                    transition(ASSIGN_REINFORCEMENT);
                }
            }

        }

        // Additional check to prevent infinite loop
        if (gameOver)
        {
            cout << "[INFO] Game Over detected in main loop.\n";
            break;
        }
    currentTurn++;  // Increment the turn counter
    }

    if (gameOver)
    {
        cout << "Game ended successfully." << endl;
    }
    else
    {
        cout << "[WARN] Exiting main loop without game over. Possible logic error." << endl;
    }
}

/* ------------- Helper function for mainGameLoop() ---------------- */
// Enhanced `removeEliminatedPlayers` Method with Logging
void GameEngine::removeEliminatedPlayers()
{
    auto it = playerList.begin();
    while (it != playerList.end())
    {
        Player *player = *it;
        if (player->getOwnedTerritories().empty())
        {
            cout << "[LOG] Removing player " << player->getName() << " as they have no territories left.\n";
            eliminatedPlayers.push_back(player); // Track eliminated players
            it = playerList.erase(it);           // Remove player from the list
        }
        else
        {
            ++it;
        }
    }

    cout << "[LOG] Remaining players in the game: ";
    for (const Player *remainingPlayer : playerList)
    {
        cout << remainingPlayer->getName() << " ";
    }
    cout << "\n";
}

/* ------------- end of `removeEliminatedPlayers` ---------------- */

void GameEngine::removeEliminatedPlayer()
{
    if (playerList.empty())
    {
        cout << "[LOG] No players to remove.\n";
        return;
    }

    // Seed the random number generator for true randomness each time it's called
    srand(static_cast<unsigned>(time(nullptr)));

    // Generate a random index within the bounds of the playerList
    int randomIndex = rand() % playerList.size();

    // Iterator to the randomly selected player
    auto it = playerList.begin();
    advance(it, randomIndex);

    // Remove and log the player
    Player *player = *it;
    eliminatedPlayers.push_back(player);
    playerList.erase(it);

    // Log remaining players
    cout << "[LOG] Remaining players in the game: ";
    for (const Player *remainingPlayer : playerList)
    {
        cout << remainingPlayer->getName() << " ";
    }
    cout << "\n";
}

/* ---- Helper function, After win. if choose replay,reset the game ---- */
void GameEngine::resetGame()
{
    // Reset the winner
    winner = nullptr;

    // // Clear player list
    // for (Player *player : playerList)
    // {
    //     delete player; // Free dynamically allocated memory
    // }
    // playerList.clear();

    // // Clear eliminated players
    // for (Player *player : eliminatedPlayers)
    // {
    //     delete player; // Free dynamically allocated memory
    // }
    // eliminatedPlayers.clear();

    // // Reset the map
    // if (selectedMap != nullptr)
    // {
    //     delete selectedMap; // Free dynamically allocated memory for the map
    //     selectedMap = nullptr;
    // }

    // Reset other game-related data
    playerOder.clear();   // Clear the player order vector
    // currentState = START; // Reset the state to START

    // Optionally reset the deck or other components if needed
    deck = Deck(); // Reinitialize the deck with default cards
}

/*
------------------------------ Part 3 reinforcementPhase() -----------------------------
1) Players are given a number of army units that depends on the number of territories they own,
(# of territories owned divided by 3, rounded down).
2) If a player owns all territories on a continent, they receive additional army units equal to that continent’s control bonus.
3) Add the calculated army units to the player's reinforcement pool each turn.
*/
void GameEngine::reinforcementPhase()
{
    cout << "=== Reinforcement Phase ===" << endl;

    // Iterate over each player in the game
    for (Player *player : playerList)
    {
        // Skip players without territories since they will be eliminated in executeOrdersPhase()

        if (player->getOwnedTerritories().empty())
        {
            cout << player->getName() << " has no territories and will be removed at the end of executeOrdersPhase." << endl;
            continue;
        }

        // Reinforcements based on the number of territories
        int numTerritories = player->getOwnedTerritories().size();
        int reinforcementFromTerritories = max(3, numTerritories / 3); // Minimum of 3 reinforcements
        int reinforcementFromContinents = 0;

        // Reinforcements from controlled continents
        for (Continent *continent : selectedMap->getContinents())
        {
            bool controlsContinent = true;

            // Verify if the player owns all territories in the continent
            for (Territory *territory : continent->getTerritories())
            {
                if (territory->getOwner() != player->getName())
                {
                    controlsContinent = false;
                    break;
                }
            }

            if (controlsContinent)
            {
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
map<Player *, int> savedReinforcements;

void GameEngine::issueOrdersPhase()
{
    cout << "=== Starting Issue Orders Phase ===\n";
    int phaseTurn = 0;
    // Step 1: Save the initial reinforcement count for each player
    map<Player *, int> savedReinforcements;
    for (Player *player : playerList)
    {
        savedReinforcements[player] = player->getNumberOfReinforcement();
    }

    bool ordersPending;
    int round = 1;

    // Continue issuing orders in a round-robin fashion until no orders are pending
    do
    {
        if (phaseTurn >= params.maxTurns)
        { // Check if the max number of turns has been reached
            cout << "[DRAW] Max turns reached for issuing orders. Ending phase.\n";
            break; // Exit the loop if max turns limit is reached
        }
        cout << "\n--- Turn " << round++ << " ---\n";
        ordersPending = false;

        for (Player *player : playerList)
        {
            // Check if the player has more orders to issue
            if (player->hasMoreOrders())
            {
                cout << player->getName() << " is issuing an order...\n";
                player->issueOrder(); // Issue one order for the player
                ordersPending = true; // Flag that there are still orders pending
            }
        }
                    phaseTurn++; // Increment the turn counter each round
    } while (ordersPending);
    // /* For debug */
    // for (Player* player : playerList) {
    //     cout << "\n[INFO] " << player->getName() << " has completed issuing all orders. \n";
    //         cout << " --- Here is information of " << player->getName() << " --- \n";
    //         player->printPlayerInfo();
    // }

    cout << "=== End of Issue Orders Phase ===\n";

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
void GameEngine::executeOrdersPhase()
{
    cout << "=== Starting Order Execution Phase ===\n";
    int phaseTurn = 0; // Initialize the phase turn counter

    // Restore each player's reinforcement count before executing orders
    for (Player *player : playerList)
    {
        if (savedReinforcements.find(player) != savedReinforcements.end())
        {
            player->setNumberOfReinforcement(savedReinforcements[player]);
        }
    }

    bool ordersRemaining;
    int round = 1; // Track rounds for clarity

    do
    {
        cout << "\n--- Execution Turn " << round++ << " ---\n";
        ordersRemaining = false;
        if (phaseTurn >= params.maxTurns)
        { // Check if the max number of turns has been reached
            cout << "[INFO] Max turns reached for executing orders. Ending phase.\n";
            break; // Exit the loop if max turns limit is reached
        }

        for (Player *player : playerList)
        {
            if (player->getOrders().hasMoreOrders())
            {
                Order *order = player->getOrders().getNextOrder();
                if (order)
                {
                    cout << "\n"
                         << player->getName() << " is executing the order.\n";
                    order->execute();
                    ordersRemaining = true;
                }
            }
            else
            {
                cout << player->getName() << " has no more orders to execute.\n";
            }
        }
        phaseTurn++; // Increment the turn counter each round

    } while (ordersRemaining);

    cout << "=== Order Execution Phase Complete ===\n";
}

/*------------------------------- End of Methods for P3 ----------------------------------------*/

/*----------------------TODO--------- Assignement3 _ part2 ----------------------------------------*/
void GameEngine::startTournament(const TournamentParameters &params)
{
    cout << "Starting Tournament..." << endl;
    if (tournamentMode)
    {
        setupTournament(params);
        initializeTournamentPlayers(params.playerStrategies);
    }
    cout << "Total maps to load: " << params.mapFiles.size() << endl; // Debug: Check how many maps are there to load

    vector<string> tournamentResults;

    // Loop through each map in the parameters
    for (const auto &mapFile : params.mapFiles)
    {
        Map *map = new Map();
        map = map->loadMapFromFile(mapFile);
        this->setMap(map);

        // load map
        if (this->selectedMap == nullptr)
        {
            cout << "Failed to load map: " << mapFile << ". Skipping this map." << endl;
            this->selectedMap = map;
            delete map;
            continue;
        }
        else
        {
            cout << "Map loaded successfully: " << mapFile << endl; // Confirm map loaded
        }
        // validate map
        if (!this->selectedMap->validate())
        {
            cout << "Validation failed for map: " << mapFile << ". Skipping this map." << endl;
            delete map;
            continue;
        }
        else
        {
            cout << "Map validated successfully: " << mapFile << endl; // Confirm map validated
        }

        // Simulate the specified number of games
        for (int i = 0; i < params.numberOfGames; ++i)
        {
            // Setup the game environment
            cout << "------------------Game in : " << i <<"--------------------"<< endl;
            gamestart(*this);
            cout<<displayTournamentResults();
            resetGame(); // Reset the game for the next run

        }
        resetGame();
    }

    cout << "Tournament completed." << endl;
    displayTournamentResults();
}

// Display overall results after all tournaments are completed
string GameEngine::displayTournamentResults() const
{
    stringstream str;
    const char separator = ' ';
    const int mapNameWidth = 25;
    const int nameWidth = 15;

    str << "Tournament Mode:" << endl;
    str << "M: ";
    for (int i = 0; i < params.mapFiles.size(); i++)
    {
        str << params.mapFiles.at(i) << ((i != params.mapFiles.size() - 1) ? ',' : ' ');
    }
    str << endl
        << "P: ";
    for (int i = 0; i < params.playerStrategies.size(); i++)
    {
        str << params.playerStrategies.at(i) << ((i != params.playerStrategies.size() - 1) ? ',' : ' ');
    }
    str << endl
        << "G: " << params.numberOfGames << endl
        << "D: " << params.maxTurns << endl;
    str << left << setw(mapNameWidth) << setfill(separator) << "Map Name";

    for (int s = 1; s <= params.numberOfGames; s++)
    {
        str << left << setw(nameWidth) << setfill(separator) << ("Game " + to_string(s));
    }
    str << endl;

    // Assuming tournamentResults contains a vector of results for each map, in the same order as mapFiles
    for (auto &tournamentResult : tournamentResults)
    {
        str << left << setw(mapNameWidth) << setfill(separator) << tournamentResult.at(0);

        for (int j = 1; j < tournamentResult.size(); j++)
        {
            str << left << setw(nameWidth) << setfill(separator) << tournamentResult.at(j);
        }
        str << endl;
    }
    return str.str();
    // Notify(this);
}
void GameEngine::updateTournamentResults(const vector<vector<string>> &newResults)
{
    this->tournamentResults = newResults;
    Notify(this); // Notify observers that there's new data to log
}

bool GameEngine::isTournamentMode() const
{
    return tournamentMode;
}
void GameEngine::setTournamentMode(bool mode)
{
    tournamentMode = mode;
}

// Initializing tournament players with strategies
void GameEngine::initializeTournamentPlayers(const vector<string> &strategies)
{
    cout << "Initializing tournament players with strategies:" << endl;
    playerList.clear(); // Clear existing players

    for (const auto &strategyName : strategies)
    {
        cout << "  Strategy: " << strategyName << " - ";

        // Assuming each player is named after their strategy for clarity in debugging and gameplay
        Player *player = new Player(strategyName, strategyName);

        // Check if strategy is properly set within the Player constructor
        if (player->getStrategy() != nullptr)
        {
            playerList.push_back(player);
            cout << "Initialized successfully" << endl;
        }
        else
        {
            cout << "Initialization failed" << endl;
            delete player; // Clean up if strategy creation failed
        }
    }

    cout << "Total players initialized: " << playerList.size() << endl;
}

void GameEngine::setupTournament(const TournamentParameters &params)
{
    this->params = params; // Storing parameters in the class
}