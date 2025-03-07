#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
Map::Map() {
    for(int i=0;i<81;i++) {
        for (int j=0; j<81; j++) {
            distanceMatrix[i][j] = -1;
        }
    }
    this->resetVisited();
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);

    if(file.is_open()) {
        std::vector<std::vector<int>> matrix;
        std::string data;
        while (getline(file,data)) {
            std::stringstream ss(data);
            std::string temp;
            std::vector<int> row_data;
            while(getline(ss,temp,',')) {
                row_data.push_back(std::stoi(temp));
            }
            matrix.push_back(row_data);
        }
        file.close();

        for(int i=0;i<81;i++) {
            for (int j=0; j<81; j++) {
                distanceMatrix[i][j] = matrix[i][j];
            }
        }
    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
   if(getDistance(provinceA,provinceB) <= 250) {
        return true;
    }
    return false;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return visited[province] == true;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for(int i= 0; i<MAX_SIZE;i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for(int i = 0;i<MAX_SIZE;i++) {
        count += visited[i] == true?1:0;
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}

