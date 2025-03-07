#ifndef MASTERS_H
#define MASTERS_H

#include <vector>
#include <iostream>
#include <string>
#include "RealmShaper.h"

// This tree has a vector/array based implementation
class ShaperTree
{
protected:
    // Array to implement the tree
    std::vector<RealmShaper *> realmShapers; // Might be possible to use array as well
    std::vector<std::string> traversalHelperVector;

    // Helper function for safe index checking
    bool isValidIndex(int index);

    //Change places of two shapers depending on duel resuls
    void replace(RealmShaper *player_low, RealmShaper *player_high);

public:
    // Constructor declaration
    ShaperTree();
    // Destructor
    ~ShaperTree();

    // Getters
    int getSize();
    std::vector<RealmShaper *> getTree(); // If array is used instead of an vector this function and ONLY this function should be changed
    RealmShaper *getParent(RealmShaper *shaper);
    int getDepth(RealmShaper *shaper); // Determines the depth level of a shaper within the tree.
    int getDepth();                    // Total depth of the tree

    void initializeTree(std::vector<RealmShaper *> players); // Initilize tree from a vector

    // Tree operations
    void insert(RealmShaper *shaper);
    int remove(RealmShaper *shaper);
    /// Search
    int findIndex(RealmShaper *shaper);
    RealmShaper *findPlayer(std::string name);
    RealmShaper *findPlayer(RealmShaper shaper);

    // Duel operations
    RealmShaper duel(RealmShaper *challenger, bool result);

    // Traversal functions
    std::vector<std::string> inOrderTraversal(int index);

    void inOrderHelper(RealmShaper *rs);

    std::vector<std::string> preOrderTraversal(int index);

    void preOrderHelper(RealmShaper *rs);

    std::vector<std::string> postOrderTraversal(int index);

    void postOrderHelper(RealmShaper *rs);

    void breadthFirstTraversal(std::ofstream &outFile);
    
    // Terminal and file outputs
    void displayTree();
    void printTree(int index, int level, const std::string &prefix);
    void preOrderTraversal(int index, std::ofstream &outFile);
    void writeToFile(const std::string &filename);
    void writeShapersToFile(const std::string &filename);
};

#endif