#include <iostream>
#include "Player.h"
#include "PlayerStrategy.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

void testPlayerStrategies() {

    // Create players with different strategies
    std::cout << "================= Testing Player Strategies =================\n";

    //--------------------------Human player and aggressive player testing part--------------------------
    std::cout << "\n--- Testing Human and Aggressive Strategies ---\n";
    Player* human = new Player("Human");
    human->setStrategy(new Human(human));
    std::cout << "Created Human player with Human strategy.\n";

    Player* aggressive = new Player("Aggressive");
    aggressive->setStrategy(new Aggressive(aggressive));
    std::cout << "Created Aggressive player with Aggressive strategy.\n";

    Territory* a1 = new Territory("TerritoryA1", 3, 3, "Continent1", {});
    Territory* a2 = new Territory("TerritoryA2", 4, 4, "Continent1", {});
    Territory* a3 = new Territory("TerritoryA3", 5, 5, "Continent1", {});
    Territory* a4 = new Territory("TerritoryA4", 6, 6, "Continent1", {});
    Territory* a5 = new Territory("TerritoryA5", 7, 7, "Continent1", {});
    Territory* a6 = new Territory("TerritoryA6", 8, 8, "Continent1", {});
    std::cout << "Created 6 territories for Human and Aggressive players.\n";

    // Add adjacent territories
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
    std::cout << "Set up adjacency between territories.\n";

    human->addTerritory(a1);
    human->addTerritory(a2);
    human->addTerritory(a3);
    aggressive->addTerritory(a4);
    aggressive->addTerritory(a5);
    aggressive->addTerritory(a6);
    std::cout << "Assigned territories to Human and Aggressive players.\n";

    std::cout << "\n[Human Strategy] Issuing Orders:\n";
    human->issueOrder();

    //-------------------------- Neutral player & Cheater player testing Part --------------------------//
    std::cout << "\n--- Testing Neutral and Cheater Strategies ---\n";

    Player* neutralPlayer = new Player("NeutralPlayer");
    neutralPlayer->setStrategy(new Neutral(neutralPlayer));
    std::cout << "Created Neutral player with Neutral strategy.\n";

    Territory* n1 = new Territory("NeutralTerritory1", 9, 9, "Continent1", {});
    Territory* n2 = new Territory("NeutralTerritory2", 10, 10, "Continent1", {});
    Territory* n3 = new Territory("NeutralTerritory3", 11, 11, "Continent1", {});
    n1->addAdjacentTerritory(n2);
    n2->addAdjacentTerritory(n1);
    n2->addAdjacentTerritory(n3);
    n3->addAdjacentTerritory(n2);
    std::cout << "Created and set up territories for Neutral player.\n";

    neutralPlayer->addTerritory(n1);
    neutralPlayer->addTerritory(n2);
    neutralPlayer->addTerritory(n3);

    std::cout << "\n[Neutral Strategy] Behavior before attack:\n";
    neutralPlayer->issueOrder();

    std::cout << "\nSimulating an attack on Neutral Player...\n";
    neutralPlayer->handleAttack();
    std::cout << "Neutral player should now behave aggressively.\n";
    neutralPlayer->issueOrder();

    Player* cheaterPlayer = new Player("CheaterPlayer");
    cheaterPlayer->setStrategy(new Cheater(cheaterPlayer));
    std::cout << "Created Cheater player with Cheater strategy.\n";

    Territory* t1 = new Territory("Territory1", 0, 0, "Continent1", {});
    Territory* t2 = new Territory("Territory2", 1, 1, "Continent1", {});
    Territory* t3 = new Territory("Territory3", 2, 2, "Continent1", {});
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t2);
    cheaterPlayer->addTerritory(t1);
    t2->setOwner("AnotherPlayer");
    t2->setPlayer(new Player("AnotherPlayer"));
    t3->setOwner("AnotherPlayer");
    t3->setPlayer(new Player("AnotherPlayer"));
    std::cout << "Created and set up territories for Cheater player.\n";

    std::cout << "\n[Cheater Strategy] Behavior:\n";
    cheaterPlayer->issueOrder();
    std::cout << "Territory2 is now owned by: " << t2->getOwner() << "\n";
    std::cout << "Territory3 is now owned by: " << t3->getOwner() << "\n";

    //---------------------------- Benevolent player testing part ------------------------
    std::cout << "\n--- Testing Benevolent Strategy ---\n";

    Player* benevolent = new Player("BenevolentPlayer");
    benevolent->setStrategy(new Benevolent(benevolent));
    std::cout << "Created Benevolent player with Benevolent strategy.\n";

    Territory* b1 = new Territory("TerritoryB1", 10, 0, "Continent1", {});
    Territory* b2 = new Territory("TerritoryB2", 11, 0, "Continent1", {});
    Territory* b3 = new Territory("TerritoryB3", 12, 0, "Continent1", {});
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
    std::cout << "Assigned territories to Benevolent player.\n";

    std::cout << "\n[Benevolent Strategy] Issuing Orders:\n";
    benevolent->issueOrder();

    // Clean up dynamically allocated memory
    std::cout << "\n--- Cleaning up memory ---\n";

    delete human;
    delete aggressive;
    delete a1;
    delete a2;
    delete a3;
    delete a4;
    delete a5;
    delete a6;
    delete neutralPlayer;
    delete n1;
    delete n2;
    delete n3;
    delete cheaterPlayer;
    delete t1;
    delete t2;
    delete t3;
    delete benevolent;
    delete b1;
    delete b2;
    delete b3;

    std::cout << "All dynamically allocated memory has been cleaned up.\n";
    std::cout << "================= Testing Completed =================\n";
}


int main() {
    testPlayerStrategies();
    return 0;
}
