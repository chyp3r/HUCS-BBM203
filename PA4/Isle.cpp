#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}

const std::string& Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = false;

    shaperCount++;
    if(shaperCount>capacity) {
        isFull = true;
    }
    return isFull;
}

bool Isle::decreaseShaperCount()
{
    if(shaperCount>0)
        shaperCount--;

    return shaperCount <= 0;
}

bool Isle::operator==(const Isle &other) const
{
    return this->name==other.name;
}

bool Isle::operator<(const Isle &other) const
{
    return this->getName()<other.getName();
}

bool Isle::operator>(const Isle &other) const
{
    return this->getName()>other.getName();
}

// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    std::ifstream file(filename);
    if(file.is_open()) {
        std::string line;
        while(getline(file,line)) {
            std::stringstream ss(line);
            std::string name,point;
            ss>>name;
            auto tempIsle = new Isle(name);
            isles.push_back(tempIsle);
        }
    }

    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}