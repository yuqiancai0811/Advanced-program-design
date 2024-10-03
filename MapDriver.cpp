#include <fstream>
#include <vector>
#include <iostream>
#include "MapDriver.h"
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include "Map.h"



bool testLoadMaps(){
    std::vector<std::string> filename;
    filename.push_back("USA.map");
    filename.push_back("Europe.map");
    
    bool result;
    


    for(const std::string& filename : filename){
        Map* selectedMap = selectedMap -> loadMapFromFile(filename);
        bool readResult =  (selectedMap == nullptr);
        if (!readResult) {
            std::cout << "Map " << filename << " loaded successfully!\n";
        } else {
            std::cout << "Failed to load the map"<< filename <<". Please try again.\n";
        }

        bool validateResult = selectedMap->validate();

        if (validateResult) {
            std::cout << "Map " << filename << "validated successfully!\n";
        } else {
            std::cout << "Map " << filename << "Map validation failed. \n";
        }
    }

    return result;
} 

int main() {
    Map* map = map->loadMapFromFile("USA.map");
    
    std::vector<Continent*> continentsList;
    std::vector<Territory*> territoriesList;
    std::vector<std::string>territoriesListName;

    continentsList=map ->getContinents();
    for(Continent* continent : continentsList){
        continent->printContinentInfo();
        territoriesListName=continent->getContinents_TerritoryNames();
        for(std::string name:territoriesListName){
            std::cout<<name<< std::endl;
        }
    }

    territoriesList=map ->getTerritories();
    for(Territory* territory : territoriesList){
        territory ->printTerritoryInfo();
    }

    

    bool mapTestResult = testLoadMaps();
    std::cout << mapTestResult << std::endl;
    



    return 0;
}