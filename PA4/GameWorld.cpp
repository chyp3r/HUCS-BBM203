#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = true;

    if(realmShaper == nullptr) {
        return false;
    }
    if(isle == nullptr) {
        return false;
    }

    if(shaperTree.findIndex(realmShaper) == -1) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
         return false;
    }

    if(shaperTree.findIndex(realmShaper) == 0) {
        hasAccess = true;
    }
    else if(shaperTree.getDepth(realmShaper) == shaperTree.getDepth() && mapTree.getIsleDepth(isle) != mapTree.getDepth()) {
        hasAccess = false;
    }else {
        const int minMapDepthAccess = getMapTree().calculateMinMapDepthAccess(shaperTree.getDepth(realmShaper),shaperTree.getDepth(),mapTree.getDepth());
        if(mapTree.getIsleDepth(isle)<minMapDepthAccess)
            hasAccess = false;
    }
    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    if(hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
        realmShaper->collectItem(isle->getItem());
        std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;

        if(realmShaper->getIsle() != nullptr) {
            realmShaper->getIsle()->decreaseShaperCount();
        }
        realmShaper->setIsle(isle);

        if(isle->increaseShaperCount()) {
            std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
            for(const auto &s : shaperTree.getTree()){
                if(s->getIsle() != nullptr && s->getIsle()->getName() == isle->getName()) {
                    s->setIsle(nullptr);
                }
            }
            mapTree.remove(isle);
        }

    }else {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << isle->getName() <<"."<< std::endl;
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    if(shaper->hasEnoughEnergy()) {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        const auto isle = new Isle(isleName);
        getMapTree().insert(isle);
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
        shaper->loseEnergy();
    }else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel()  << std::endl;
    }

}

void GameWorld::displayGameState()
{
    // mapTree.displayMap();
    // shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream access(accessLogs,std::ios::binary);
    std::ifstream duel(duelLogs,std::ios::binary);
    std::vector<std::vector<std::string>> accVector;
    std::vector<std::vector<std::string>> duelVector;

    if(access.is_open()) {
        std::string line;
        getline(access,line);
        while(getline(access,line)) {
            std::string player,isle;
            std::stringstream ss(line);
            std::vector<std::string> temp;
            ss>>player>>isle;
            temp.push_back(player);
            temp.push_back(isle);
            accVector.push_back(temp);
        }
    }

    if(duel.is_open()) {
        std::string line;
        getline(duel,line);
        while(getline(duel,line)) {
            std::string player,isle;
            std::stringstream ss(line);
            std::vector<std::string> temp;
            ss>>player>>isle;
            temp.push_back(player);
            temp.push_back(isle);
            duelVector.push_back(temp);
        }
    }

    int counterAcc = 0;
    int counterDuel = 0;
    for(auto s:accVector) {
        auto player = shaperTree.findPlayer(s[0]);
        auto node = mapTree.findNode(s[1]);
        if(node && node->isle)
            exploreArea(player,node->isle);
        else
            craft(player,s[1]);
        counterAcc++;
        if(counterAcc%5 == 0) {
            auto player2 = shaperTree.findPlayer(duelVector[counterDuel][0]);
            if(player2 != nullptr)
                shaperTree.duel(player2, static_cast<bool>(std::stoi(duelVector[counterDuel][1])));
            counterDuel++;
        }
    }
    for(int i = counterDuel;i<duelVector.size();i++) {
        auto player2 = shaperTree.findPlayer(duelVector[i][0]);
        if(player2 != nullptr)
            shaperTree.duel(player2, static_cast<bool>(std::stoi(duelVector[i][1])));
    }
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}