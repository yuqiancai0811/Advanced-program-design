#include "Orders.h"

// Order class methods
Order::Order() : effect(new std::string), executed(new bool(false)), name(new std::string) {}

Order::~Order() {
    delete effect;
    delete executed;
    delete name;
}

Order::Order(const Order& other) {
    effect = new std::string(*other.effect);
    executed = new bool(*other.executed);
    name = new std::string(*other.name);
}

Order& Order::operator=(const Order& other) {
    if (this == &other) return *this;

    delete effect;
    delete executed;
    delete name;

    effect = new std::string(*other.effect);
    executed = new bool(*other.executed);
    name = new std::string(*other.name);

    return *this;
}

bool Order::validate() const {
    return true;
}

void Order::execute() {
    if (validate()) {
        *effect = "Base order executed";
        *executed = true;
    }
}

std::string Order::toString() const {
    if (*executed && !name->empty()) {
        return "Order: " + *name + " is executed\n";
    } else if (!*executed && !name->empty()) {
        return "Order: " + *name + " is not executed\n";
    } else {
        return "Order: Null\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "Order: " << *order.name << (*order.executed ? " (Executed)" : " (Not executed)") << "\n";
    os << "Effect: " << *order.effect << "\n";
    return os;
}

// Subclass implementations
deployOrder::deployOrder() {
    *name = "deployOrder";
}

bool deployOrder::validate() const {
    return true;
}

void deployOrder::execute() {
    if (validate()) {
        *effect = "Deploy troops";
        *executed = true;
    }
}

advanceOrder::advanceOrder() {
    *name = "Advance Order";
}

bool advanceOrder::validate() const {
    return true;
}

void advanceOrder::execute() {
    if (validate()) {
        *effect = "Advance troops";
        *executed = true;
    }
}

bombOrder::bombOrder() {
    *name = "Bomb Order";
}

bool bombOrder::validate() const {
    return true;
}

void bombOrder::execute() {
    if (validate()) {
        *effect = "Bomb troops";
        *executed = true;
    }
}

blockadeOrder::blockadeOrder() {
    *name = "Blockade Order";
}

bool blockadeOrder::validate() const {
    return true;
}

void blockadeOrder::execute() {
    if (validate()) {
        *effect = "Blockade troops";
        *executed = true;
    }
}

airliftOrder::airliftOrder() {
    *name = "Airlift Order";
}

bool airliftOrder::validate() const {
    return true;
}

void airliftOrder::execute() {
    if (validate()) {
        *effect = "Airlift troops";
        *executed = true;
    }
}

negotiateOrder::negotiateOrder() {
    *name = "Negotiate Order";
}

bool negotiateOrder::validate() const {
    return true;
}

void negotiateOrder::execute() {
    if (validate()) {
        *effect = "Negotiate troops";
        *executed = true;
    }
}

// orderList methods
orderList::~orderList() {
    for (Order* order : orders) {
        delete order;
    }
}

void orderList::addOrder(Order* order) {
    orders.push_back(order);
}

void orderList::removeOrder(int index) {
    if (index >= 0 && index < orders.size()) {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void orderList::moveOrder(int oldIndex, int newIndex) {
    if (oldIndex >= 0 && oldIndex < orders.size() &&
        newIndex >= 0 && newIndex < orders.size()) {
        std::swap(orders[oldIndex], orders[newIndex]);
    }
}

void orderList::showAllOrders() const {
    for (const auto& order : orders) {
        std::cout << order->toString();
    }
}

/*--------------- Helper function for executeOrdersPhase() in Part3 -----------------*/
bool orderList::hasMoreOrders() const {
    return !orders.empty();
}

Order* orderList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}
/*-------------------------------------------------------------------------------*/

std::ostream& operator<<(std::ostream& os, const orderList& ordersList) {
    for (const auto& order : ordersList.orders) {
        os << *order;
    }
    return os;
}
