#include <iostream>
#include "Player.h"
#include "PlayerStrategy.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

void testPlayerStrategies() {

    // Create players with different strategies
    std::cout << "Testing Player Strategies...\n";

    //--------------------------Human player and aggressive player testing part--------------------------
    Player* human=new Player("Human");
    human->setStrategy(new Human(human));

    Player* aggressive=new Player("Aggressive");
    aggressive->setStrategy(new Aggressive(aggressive));


    Territory* a1=new Territory("TerritoryA1",3,3,"Continent1",{});
    Territory* a2=new Territory("TerritoryA2",4,4,"Continent1",{});
    Territory* a3=new Territory("TerritoryA3",5,5,"Continent1",{});
    Territory* a4=new Territory("TerritoryA4",6,6,"Continent1",{});
    Territory* a5=new Territory("TerritoryA5",7,7,"Continent1",{});
    Territory* a6=new Territory("TerritoryA6",8,8,"Continent1",{});
    a1->addAdjacentTerritory(a2);
    a1->addAdjacentTerritory(a3);
    a1->addAdjacentTerritory(a5);
    a2->addAdjacentTerritory(a3);
    a2->addAdjacentTerritory(a1);
    a2->addAdjacentTerritory(a4);
    a3->addAdjacentTerritory(a1);
    a3->addAdjacentTerritory(a2);
    a3->addAdjacentTerritory(a6);


    a4->addAdjacentTerritory(a2);
    a4->addAdjacentTerritory(a5);
    a4->addAdjacentTerritory(a6);
    a5->addAdjacentTerritory(a1);
    a5->addAdjacentTerritory(a4);
    a5->addAdjacentTerritory(a6);
    a6->addAdjacentTerritory(a3);
    a6->addAdjacentTerritory(a4);
    a6->addAdjacentTerritory(a5);

    human->addTerritory(a1);
    human->addTerritory(a2);
    human->addTerritory(a3);

    aggressive->addTerritory(a4);
    aggressive->addTerritory(a5);
    aggressive->addTerritory(a6);
    human->issueOrder();

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

    //----------------------------Benevolent player testing part ------------------------
    Player* benevolent = new Player("BenevolentPlayer");
    benevolent->setStrategy(new Benevolent(benevolent));
    Territory* b1=new Territory("TerritoryB1", 10, 0, "Continent1", {});
    Territory* b2=new Territory("TerritoryB2", 11, 0, "Continent1", {});
    Territory* b3=new Territory("TerritoryB3", 12, 0, "Continent1", {});
    b1->addAdjacentTerritory(b2);
    b1->addAdjacentTerritory(a6);
    b1->addAdjacentTerritory(b3);
    b2->addAdjacentTerritory(b1);
    b2->addAdjacentTerritory(b3);
    b3->addAdjacentTerritory(b1);
    b3->addAdjacentTerritory(b2);
    benevolent->addTerritory(b1);
    benevolent->addTerritory(b2);
    benevolent->addTerritory(b3);

}

int main() {
    testPlayerStrategies();
    return 0;
}
