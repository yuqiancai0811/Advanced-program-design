#include <iostream>
#include "Player.h"
#include "PlayerStrategy.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

void testPlayerStrategies() {
    // Create players with different strategies
    std::cout << "Testing Player Strategies...\n";


    //-------------------------- Neutral player & Cheater player testing Part --------------------------//
    // Create a Neutral Player
    Player* neutralPlayer = new Player("NeutralPlayer");
    neutralPlayer->setStrategy(new Neutral(neutralPlayer));
    std::cout << "\nNeutral Player Behavior:\n";
    neutralPlayer->issueOrder(); // Should do nothing

    // Simulate an attack on the Neutral player
    std::cout << "\nSimulating an attack on Neutral Player:\n";
    neutralPlayer->handleAttack(); // Switch to Aggressive
    neutralPlayer->issueOrder();   // Should now behave aggressively

    // Create a Cheater Player
    Player* cheaterPlayer = new Player("CheaterPlayer");
    cheaterPlayer->setStrategy(new Cheater(cheaterPlayer));

    // Create some territories
    Territory* t1 = new Territory("Territory1", 0, 0, "Continent1", {});
    Territory* t2 = new Territory("Territory2", 1, 1, "Continent1", {});
    Territory* t3 = new Territory("Territory3", 2, 2, "Continent1", {});

    // Set up adjacent territories
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t2);

    // Assign territories to players
    cheaterPlayer->addTerritory(t1);
    t2->setOwner("AnotherPlayer");
    t2->setPlayer(new Player("AnotherPlayer"));
    t3->setOwner("AnotherPlayer");
    t3->setPlayer(new Player("AnotherPlayer"));

    std::cout << "\nCheater Player Behavior:\n";
    cheaterPlayer->issueOrder(); // Automatically conquers adjacent territories

    // Verify ownership changes
    std::cout << "Territory2 is now owned by: " << t2->getOwner() << "\n";
    std::cout << "Territory3 is now owned by: " << t3->getOwner() << "\n";

    // Clean up dynamically allocated memory
    delete neutralPlayer;
    delete cheaterPlayer;
    delete t1;
    delete t2;
    delete t3;

    std::cout << "\nTesting completed.\n";
}

int main() {
    testPlayerStrategies();
    return 0;
}
