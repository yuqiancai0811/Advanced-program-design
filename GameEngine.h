#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <iostream>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

#include "CommandProcessor.h"
#include "LoggingObserver.h"

class CommandProcessor;

/*-------------------- Phases for Part 3 -------------------------*/
extern const std::string START;
extern const std::string MAPLODADED;
extern const std::string PLAYERSADDED;
extern const std::string MAPVALIDATED;
extern const std::string ASSIGN_REINFORCEMENT;
extern const std::string ISSUE_ORDERS;
extern const std::string EXECUTE_ORDERS;
extern const std::string WIN;
/*---------------------------------------------------------------*/

class GameEngine : public Subject, public ILoggable{
private:
    

public:

    CommandProcessor* commandProcessor;//
    Map* selectedMap;                // The game map
    std::vector<Player*> playerList; // List of players
    Deck deck;                       // The deck of cards for the game
    vector <Player*> eliminatedPlayers;
    Player *currentPlayer;
    vector <Player*> playerOder;
    string currentState;  // The current state of the game added by Yuqian Cai
    CommandProcessor *commandProcessor;

    GameEngine();  // Constructor to initialize the game
    ~GameEngine(); // Destructor to clean up resources
    void setCommandProcessor(CommandProcessor *processor) {}
     // Method to get the player list for testing
    std::vector<Player*> getPlayerList() const; 

    // Handles the gameplay phase (assigning reinforcement, issuing orders, executing orders)
    void playGame();

    // Function to draw and play cards
    void playCards(Player* player);



    /*  --------------  Game state part ------------------*/
    // Get the current game state as a string
    std::string getCurrentState();
    void setcurrentState(std::string newGameState);
    // Helper function to transition between states
    void transition(const std::string& newState);
    void setProcessor(CommandProcessor *processor);
    void setMap(Map *selectedMap);

    



    /* Assignment 2 -------------- Part 2 ------------------*/
    // By lucasS
    // Additional game setup and management
    void AssignTerritories();         // Assigns territories to players
    void assignTerritoryToPlayer(Territory* territory, Player* player);

    void randomizeOrderOfPlay();      // Randomly determines the order of play for players
    
    GameEngine startupPhase();              // Handles the startup phase commands and game setup
    // 1) use the loadmap <filename> command to select a map from a list of map files as stored in a directory,
    // which results in the map being loaded in the game.
    // 2) use the validatemap command to validate the map (i.e. it is a connected graph, etc – see assignment 1).
    // 3) use the addplayer <playername> command to enter players in the game (2-6 players)

    void gamestart(GameEngine &game); // Handles the game start operations after setup is complete
    // a) fairly distribute all the territories to the players
    // b) determine randomly the order of play of the players in the game
    // c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
    // d) let each player draw 2 initial cards from the deck using the deck’s draw() method

    /* End  -------------- Part 2 ------------------*/





    /* -------------- Part 3 ------------------*/
    Player* winner;                  // Pointer to the player who won the game
    // Game phases
    void reinforcementPhase();        // Reinforcement phase of the game
    void issueOrdersPhase();          // Issuing orders phase of the game
    void executeOrdersPhase();        // Execution of orders phase
    void resetGame(); //Not sure if it's part of MainLoop...
    void removeEliminatedPlayers(); //Helper function for mainGameLoop

    // bool checkWinCondition();
    
    // Main game loop
    void mainGameLoop();            // Main game loop that handles phase transitions and checks win condition
     /* -------------- End of P3 ------------------*/

    // Set and Get current game state
    void setCurrentState(const std::string& newGameState); // Sets the current game state

    // Command processing
    void handleUserCommand(const std::string& command, GameEngine &game);  // Processes user commands

    // Print and display
    void printWelcomeMessage();       // Prints the welcome message when the game starts
    //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;
};
#endif // GAMEENGINE_H
