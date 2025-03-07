#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

Isle *RealmShaper::getIsle() const {
    return isleIn;
}

void RealmShaper::setIsle(Isle *isle) {
    isleIn = isle;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;
    std::ifstream file(filename);
    if(file.is_open()) {
        std::string line;
        while(getline(file,line)) {
            std::stringstream ss(line);
            std::string name,point;
            ss>>name>>point;
            auto tempPlayer = new RealmShaper(name,stoi(point));
            players.push_back(tempPlayer);
        }
    }
    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    return this->getName() == other.getName();
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    return (os << p.name); // << std::endl);
}
