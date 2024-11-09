cmake_minimum_required(VERSION 3.29)
project(COMP345_A1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COMP345_A1
    Cards.cpp
    Cards.h
    CardsDriver.cpp
    CommandProcessingDriver.cpp
    CommandProcessingDriver.h
    CommandProcessor.cpp
    CommandProcessor.h
    GameEngine.cpp
    GameEngine.h
    GameEngineDriver.cpp
    MainDriver.cpp
    Map.cpp
    Map.h
    MapDriver.cpp
    MapDriver.h
    OrderDriver.cpp
    OrderDriver.h
    Orders.cpp
    Orders.h
    Player.cpp
    Player.h
    PlayerDriver.cpp)
