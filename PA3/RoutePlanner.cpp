#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    for (int & priorityProvince : priorityProvinces) {
        priorityProvince = -1;
    }
    for (int & priorityProvince : weatherRestrictedProvinces) {
        priorityProvince = -1;
    }
    map.loadDistanceData(distance_data);
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    map.resetVisited();
    RoutePlanner::maxDistance = maxDistance;

}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::regex number(R"(\d+)");
    if(file.is_open()) {
        std::string line;
        std::smatch match;
        while (getline(file, line)) {
            std::string::const_iterator searchStart(line.cbegin());
            while (std::regex_search(searchStart, line.cend(), match, number)) {
                priorityProvinces[numPriorityProvinces] = std::stoi(match[0]);
                numPriorityProvinces++;
                searchStart = match.suffix().first;
            }
        }
        file.close();
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::regex number(R"(\d+)");
    if(file.is_open()) {
        std::string line;
        std::smatch match;
        numWeatherRestrictedProvinces = 0;
        while(getline(file,line)) {
            std::string::const_iterator searchStart(line.cbegin());
            while (std::regex_search(searchStart, line.cend(), match, number)) {
                weatherRestrictedProvinces[numWeatherRestrictedProvinces] = std::stoi(match[0]);
                numWeatherRestrictedProvinces++;
                searchStart = match.suffix().first;
            }
        }
        file.close();
    }

}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for(const int priorityProvince : priorityProvinces) {
        if(priorityProvince == -1) {
            return false;
        }
        if(priorityProvince == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for(const int priorityProvince : weatherRestrictedProvinces) {
        if(priorityProvince == -1) {
            return false;
        }
        if(priorityProvince == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    if(!map.isVisited(startingCity)) {
        route.push_back(startingCity);
        map.markAsVisited(startingCity);
        stack.push(startingCity);
    }

    enqueueNeighbors(startingCity);

    int place=-1;
    while(true) {
        if(!queue.isEmpty()) {
            place= queue.dequeue();
            if(map.isWithinRange(startingCity,place,maxDistance)) {
                totalDistanceCovered +=map.getDistance(startingCity,place);
                break;
            }
        }else {
            place = -1;
            break;
        }
    }

    if(place != -1) {
        exploreRoute(place);
    }
    else {
        if(!stack.isEmpty()) {
            stack.pop();
            if(!stack.isEmpty()) {
                exploreRoute(stack.peek());
            }else {
                displayResults();
            }
        }
    }
}
// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
        enqueueNeighbors(province);
}

void RoutePlanner::enqueueNeighbors(int province) {
    for(int i =0;i<81;i++) {
        if(map.isWithinRange(province,i,maxDistance)) {
            if(isWeatherRestricted(i)) {
                std::cout<<"Province "<<cities[i]<<" is weather-restricted. Skipping."<<std::endl;
                continue;
            }
            if(map.isVisited(i))
                continue;
            if(isPriorityProvince(i))
                queue.enqueuePriority(i);
            else
                queue.enqueue(i);
        }
    }

}

void RoutePlanner::backtrack() {
    stack.pop();
}

bool RoutePlanner::isExplorationComplete() const {
    if(queue.isEmpty() && stack.isEmpty())
        return true;
    if(map.countVisitedProvinces() == 81)
        return true;

    return false;
}

void RoutePlanner::displayResults() const {
    std::cout<<"----------------------------"<<std::endl;
    std::cout<<"Journey Completed!"<<std::endl;
    std::cout<<"----------------------------"<<std::endl;
    std::cout<<"Total Number of Provinces Visited: "<<map.countVisitedProvinces()<<std::endl;
    std::cout<<"Total Distance Covered: "<<totalDistanceCovered<<" km"<<std::endl;
    std::cout<<"Route Taken:"<<std::endl;
    for (auto it = route.begin(); it != route.end(); ++it) {
        std::cout << cities[*it]<< " -> ";
    }
    std::cout << "End"<<std::endl;
    std::cout<<std::endl<<"Priority Provinces Status:"<<std::endl;
    int count = 0;
    for(auto i:priorityProvinces) {
        if(i == -1)
            break;
        if(std::find(route.begin(),route.end(),i) != route.end()) {
            std::cout<<"- "<<cities[i]<<" ("<<"Visited"<<")"<<std::endl;
            count++;
        }else{
            std::cout<<"- "<<cities[i]<<" ("<<"Not Visited"<<")"<<std::endl;
        }
    }
    std::cout<<std::endl<<"Total Priority Provinces Visited: "<<count<<" out of "<<numPriorityProvinces<<std::endl;
    if(count == numPriorityProvinces)
        std::cout<<"Success: All priority provinces were visited.";
    else
        std::cout<<" Warning: Not all priority provinces were visited.";
}


