#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    for(const auto& s:realmShapers) {
        delete s;
    }
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    realmShapers = std::move(shapers);
}

int ShaperTree::getSize()
{
    return static_cast<int>(realmShapers.size());
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    return index < this->getSize();
}

void ShaperTree::insert(RealmShaper *shaper)
{
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    const int deleteIndex = findIndex(shaper);
    if(deleteIndex != -1) {
        delete realmShapers[deleteIndex];
        const int size = this->getSize();
        for(int s = deleteIndex;s<size-1;s++) {
            realmShapers[s] = realmShapers[s+1];
        }
        realmShapers.pop_back();
    }
    return deleteIndex;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    int count = 0;
    for(const auto s:realmShapers) {
        if(s == shaper) {
            return count;
        }
        count++;
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    const int index = findIndex(shaper);
    if(index != -1) {
        return static_cast<int>(log2(index+1));
    }
    return -1;
}

int ShaperTree::getDepth()
{
    return static_cast<int>(log2(getSize()));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    const auto parent = realmShapers[(findIndex(challenger)-1)/2];
    if(result) {
        replace(parent,challenger);
        challenger->gainHonour();
        parent->loseHonour();
        std::cout << "[Duel] " << challenger->getName()<< " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
        if(parent->getHonour()<=0) {
            if(parent->getIsle() != nullptr)
                parent->getIsle()->decreaseShaperCount();
            std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
            remove(parent);
        }
        return *challenger;
    }

    challenger->loseHonour();
    parent->gainHonour();
    std::cout << "[Duel] " << challenger->getName()<< " lost the duel" << std::endl;
    std::cout << "[Honour] " << "New honour points: ";
    std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
    std::cout << parent->getName() << " - " << parent->getHonour() << std::endl;
    if(challenger->getHonour()<=0) {
        if(challenger->getIsle() != nullptr)
            challenger->getIsle()->decreaseShaperCount();
        std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
        remove(challenger);
    }
    return *parent;


}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    int index = findIndex(shaper);
    if(index != -1) {
        parent = realmShapers[(index-1)/2];
    }
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    const int indexLow = findIndex(player_low);
    const int indexHigh = findIndex(player_high);
    const auto temp = player_high;
    realmShapers[indexHigh] = realmShapers[indexLow];
    realmShapers[indexLow] = temp;}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{

    for(const auto& s:realmShapers) {
        if(s->getName() == shaper.getName()) {
            return s;
        }
    }
    return nullptr;
}

RealmShaper *ShaperTree::findPlayer(std::string name)
{
    for(const auto& s:realmShapers) {
        if(s->getName() == name) {
             return s;
        }
    }
    return nullptr;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    traversalHelperVector.clear();
    inOrderHelper(realmShapers[index]);
    result = std::move(traversalHelperVector);
    traversalHelperVector.clear();
    return result;
}

void ShaperTree::inOrderHelper(RealmShaper *rs){
    const int index = findIndex(rs);
    if (index != -1) {
        const int left = index * 2 + 1;
        const int right = index * 2 + 2;

        if (left < getSize() && realmShapers[left] != nullptr)
            inOrderHelper(realmShapers[left]);
        traversalHelperVector.push_back(rs->getName());
        if (right < getSize() && realmShapers[right] != nullptr)
            inOrderHelper(realmShapers[right]);
    }
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    traversalHelperVector.clear();
    preOrderHelper(realmShapers[index]);
    result = std::move(traversalHelperVector);
    traversalHelperVector.clear();
    return result;
}
void ShaperTree::preOrderHelper(RealmShaper *rs){
    const int index = findIndex(rs);
    if (index != -1) {
        const int left = index * 2 + 1;
        const int right = index * 2 + 2;
        traversalHelperVector.push_back(rs->getName());
        if (left < getSize() && realmShapers[left] != nullptr)
            preOrderHelper(realmShapers[left]);
        if (right < getSize() && realmShapers[right] != nullptr)
            preOrderHelper(realmShapers[right]);
    }
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    traversalHelperVector.clear();
    postOrderHelper(realmShapers[index]);
    result = std::move(traversalHelperVector);
    traversalHelperVector.clear();
    return result;
}

void ShaperTree::postOrderHelper(RealmShaper *rs){
    const int index = findIndex(rs);
    if (index != -1) {
        const int left = index * 2 + 1;
        const int right = index * 2 + 2;
        if (left < getSize() && realmShapers[left] != nullptr)
            postOrderHelper(realmShapers[left]);
        if (right < getSize() && realmShapers[right] != nullptr)
            postOrderHelper(realmShapers[right]);
        traversalHelperVector.push_back(rs->getName());
    }
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    std::vector<std::string> result = {};

    traversalHelperVector.clear();
    preOrderHelper(realmShapers[index]);
    result = std::move(traversalHelperVector);
    traversalHelperVector.clear();

    for (const auto& nodeName : result) {
        outFile << nodeName << std::endl;
    }
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    std::queue<RealmShaper*> nodes;
    if(!realmShapers.empty()) {
        nodes.push(realmShapers[0]);
    }

    while(!nodes.empty()) {
        const auto last = nodes.front();
        const int index = findIndex(last);
        if(index != -1) {
            const int left = 2 * index + 1;
            const int right = 2 * index + 2;

            if (left < getSize() && realmShapers[left] != nullptr) {
                nodes.push(realmShapers[left]);
            }

            if (right < getSize() && realmShapers[right] != nullptr) {
                nodes.push(realmShapers[right]);
            }
        }
        outFile << last->getName() << std::endl;
        nodes.pop();
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = index*2+1;
    int right = index*2+2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream file(filename);
    breadthFirstTraversal(file);
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    preOrderTraversal(0,file);
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
