#include <string>
#include <vector>

#ifndef MAP_H
#define MAP_H

class Territory {
     private:
    int x,y;
    int armies;
    std::string name;
    std::string owner;
    std::string continent;
    std::vector<std::string> adjacentTerritoryNames;
    std::vector<Territory*> adjacentTerritories;


    public:

    Territory(std::string name, int x, int y, std::string continent, const std::vector<std::string>& adjNames); //Constructor(Name + X + Y + Continent + vertor (adjacentTerritoryNames) );
    ~Territory();

    //Set functions;
    void setOwner(const std::string& owner);
    void setArmies(int numArmies);
    void addAdjacentTerritory(Territory* territory);
    
    //Get functions;
    int getArmies() const;
    std::string getName() const;
    std::string getOwner() const;
    std::string getContinent() const;
    std::vector<std::string> getAdjacentTerritoryNames() const;
    std::vector<Territory*> getAdjacentTerritories() const;
    

    void printTerritoryInfo() const;  // print function

};


class Continent {
    private:
    std::string name;
    int controlValue;
    std::vector<std::string> continents_TerritoryNames;
    std::vector<Territory*> continents_Territory;
    

    public:
    Continent(std::string name, int controlValue);//Constructor(Name + controlValue );
    ~Continent();
    
    void addTerritory(Territory* territory);

    std::vector<Territory*> getTerritories() const;
    std::string getName() const;
    int getControlValue() const;
    //Get functions;

    void printContinentInfo() const;  // print function

};

class Map {
private:
    std::string author;
    std::string warn;
    std::string image;
    std::string wrap;
    std::string horizontal;

    std::vector<Territory*> territories;
    std::vector<Continent*> continents;


public:
    Map();
    ~Map();

    // Set functions
    void setAuthor(const std::string& newAuthor);
    void setWarn(const std::string& newWarn); 
    void setImage(const std::string& newImage); 
    void setWrap(const std::string& newWrap); 
    void setHorizontal(const std::string& newHorizontal);

    // Get functions
    std::string getAuthor() const;
    std::string getWarn() const; 
    std::string getImage() const;
    std::string getWrap() const;
    std::string getHorizontal() const;
    std::vector<Continent*> getContinents() const;
    std::vector<Territory*> getTerritories() const;


    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);
    Map testLoadMaps(std::string filename);
    

    bool isConnectedGraph() const ; // the map is a connected graph

    bool areContinentsConnected() const;// continents are connected subgraphs
    

    bool territoryBelongsToOneContinentsConnected() const;// each country belongs to one and only one continent

    Map* loadMapFromFile(const std::string& filename);
    
    bool validate() const;

};

#endif 