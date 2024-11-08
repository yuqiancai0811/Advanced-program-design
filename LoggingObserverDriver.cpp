#include "LoggingObserver.h"
#include "Orders.h"
#include "GameEngine.h"
#include "CommandProcessor.h"

void testLoggingObserver()
{
    /// Attach a log observer
    LogObserver *logger = new LogObserver("gamelog.txt");

    // // CommandProcessor
    // CommandProcessor cmdProcessor;
    // cmdProcessor.Attach(logger);
    // // cmdProcessor.saveCommand("Attack Command");


    // Order
    Order order;
    order.Attach(logger);

    order.execute();

    // // Command
    // Command cmd;
    // cmd.Attach(logger);

    // cmd.saveEffect("Command executed with effect");


    // // OrderList
    // orderList ordersList;
    // ordersList.Attach(logger);

    // ordersList.addOrder(new deployOrder());
    // ordersList.addOrder(new advanceOrder());
    // ordersList.addOrder(new bombOrder());
    // ordersList.addOrder(new blockadeOrder());
    // ordersList.addOrder(new airliftOrder());
    // ordersList.addOrder(new negotiateOrder());

    // // GameEngine
    // GameEngine gameEngine;
    // gameEngine.Attach(logger);

    // gameEngine.transition("Setup");
    // gameEngine.transition("In Progress");
    // gameEngine.transition("Game Over");

    std::cout << "Logging completed. Check gamelog.txt for results." << std::endl;
}

int main()
{
    testLoggingObserver();
    return 0;
}
