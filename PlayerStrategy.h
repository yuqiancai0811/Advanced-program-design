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

class Neutral : public PlayerStrategy {
  explicit Neutral(Player* player);
};

class Cheater : public PlayerStrategy {
  explicit Cheater(Player* player);
};