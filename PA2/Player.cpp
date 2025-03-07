#include "Player.h"
#include <iostream>
// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
          current_ammo(max_ammo), lives(lives){
    // TODO: Your code here, if you want to do further initializations
    length = (shape[1]).size();
    for(int i=shape[1].size()-1;i>=0;i--) {
        int middle = spacecraft_shape.size()/2;
        if(spacecraft_shape[middle][i] == 0) {
            length--;
        }else{break;}
    }
}

// Move player left within the grid boundaries
void Player::move_left() {
    // TODO: Your code here
    if(position_col !=0)
        position_col -=1;
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    // TODO: Your code here
    if(position_col+spacecraft_shape[0].size() <grid_width)
        position_col +=1;

}

// Move player up within the grid boundaries
void Player::move_up() {
    if(position_row !=0)
        position_row -=1;
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    if(position_row+spacecraft_shape.size() <grid_height)
        position_row +=1;

}