#include "LoggingObserver.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include "CommandProcessor.h"

void testLoggingObserver()
{
    // initilize a game log file
   // LogObserver logger("gamelog.txt");
    // effectively using the Observer patterns’ Notify(Subject) method on:

    // CommandProcessor::saveCommand()
    //   CommandProcessor cmdProcessor();
    // cmdProcessor.Attach(&logger);

    // Order::execute()
    //   Order order;
    //order.Attach(&logger);

    // Command::saveEffect()
    // Command cmd;
    // cmd.Attach(&logger);


    // OrderList::addOrder()


    // and GameEngine::transition()
    //  Create subjects and add the logger as an observer


  

    // Trigger log events
    // cmdProcessor.saveCommand("Attack Command");
    // cmd.saveEffect("Command executed with effect");
    // order.execute();

    // std::cout << "Logging completed. Check gamelog.txt for results." << std::endl;
}

// int main()
// {
//     testLoggingObserver();
//     return 0;
// }
