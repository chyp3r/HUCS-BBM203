#include "Leaderboard.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);
    if(file.is_open()) {
        string line;
        while(getline(file,line)) {
            istringstream ss(line);
            int score,time;
            string name;
            ss >>score>>time>>name;
            auto entry = new LeaderboardEntry(score,time,name);
            insert(entry);
        }
        file.close();
    }
    else{
        ofstream temp(filename);
        temp.close();
    }
}

// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    ofstream  file(filename);
    if(file.is_open()) {
        auto temp = head_leaderboard_entry;
        while(temp!=nullptr) {
            file <<temp->score<<" "<<temp->last_played<<" "<<temp->player_name<<endl;
            temp = temp->next;
        }
        file.close();
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    cout<<"Leaderboard"<<endl;
    cout<<"-----------"<<endl;
    auto temp = head_leaderboard_entry;
    int counter = 1;
    while(temp != nullptr) {
        cout<<counter<<". "<<temp->player_name<<" "<<temp->score<<" "<< put_time(localtime(&temp->last_played),"%H:%M:%S/%d.%m.%Y") <<endl;
        counter++;
        temp = temp->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    LeaderboardEntry* last = nullptr;
    for(LeaderboardEntry* temp = head_leaderboard_entry; temp != nullptr && temp->score>=new_entry->score;temp = temp->next) {
        last = temp;
    }
    if(last != nullptr) {
        new_entry->next = last->next;
        last->next = new_entry;
    }else {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    }
    if(find_length()>MAX_LEADERBOARD_SIZE) {
        auto temp = head_leaderboard_entry;
        int count = 0;
        while(count < 9) {
            count++;
            temp = temp->next;
        }
        delete temp->next;
        temp->next = nullptr;

    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    auto temp = head_leaderboard_entry;
    while(temp!=nullptr) {
        LeaderboardEntry *next = temp->next;
        delete temp;
        temp = next;
    }
}

int Leaderboard::find_length() const {
    auto temp = head_leaderboard_entry;
    int count = 0;
    while(temp!=nullptr) {
        count++;
        temp = temp->next;
    }
    return count;
}
