#ifndef PLAYER_STRATEGY_H
#define PLAYER_STRATEGY_H

#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include "Player.h"

class PlayerStrategy {
public:
  Player* player = nullptr;
public:
  virtual void issueOrder() = 0;
  virtual Order* decideCard(Card*) = 0;
  virtual std::vector<Territory *> toDefend() = 0;
  virtual std::vector<Territory *> toAttack() = 0;
  static PlayerStrategy* createStrategy(Player* player, const std::string& strategy);
};

class Human : public PlayerStrategy {
 explicit Human(Player* player);
};

class Aggressive : public PlayerStrategy {
  explicit Aggressive(Player* player);
};

class Benevolent : public PlayerStrategy {
public:
  explicit Benevolent(Player* player);
  void issueOrder() override;
  Order* decideCard(Card* card) override;
  std::vector<Territory *> toDefend() override;
  std::vector<Territory *> toAttack() override;

  Order* playReinforcementCard();
  Order* playBlockadeCard();
  Order* playDiplomacyCard();
  Order* playAirliftCard();
};

//-------------------------- Neutral player & Cheater player Part --------------------------//
class Neutral : public PlayerStrategy {
public:
    explicit Neutral(Player* player); // Constructor
    void issueOrder() override;       // Does nothing
    Order* decideCard(Card* card) override; // Does not play cards
    std::vector<Territory *> toDefend() override; // Empty list
    std::vector<Territory *> toAttack() override; // Empty list
};

class Cheater : public PlayerStrategy {
public:
    explicit Cheater(Player* player); // Constructor
    void issueOrder() override;       // Automatically conquer adjacent territories
    Order* decideCard(Card* card) override; // Does not play cards
    std::vector<Territory *> toDefend() override; // All owned territories
    std::vector<Territory *> toAttack() override; // All adjacent territories
};
#endif // PLAYER_STRATEGY_H