#include "GameController.h"
#include "fstream"
#include "sstream"
#include <thread>
#include <chrono>

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    std::ifstream file(commands_file);
    string line;
    while(getline(file,line)) {
        game->game_time++;
        if(game->hit_check_game) {
            game->current_score--;
            game->hit_check_game = false;
        }
        bool is_printed = false;
        istringstream ss(line);
        ss>>line;
        if(line == "MOVE_UP")
            game->player->move_up();
        else if(line== "MOVE_DOWN")
            game->player->move_down(game->space_grid.size());
        else if(line== "MOVE_RIGHT")
            game->player->move_right(game->space_grid[0].size());
        else if(line== "MOVE_LEFT")
            game->player->move_left();
        else if(line == "SHOOT")
            game->shoot();
        else if(line == "PRINT_GRID") {
            game->update_space_grid();
            cout<<"Tick: "<<game->game_time<<endl;
            cout<<"Lives: "<<game->player->lives<<endl;
            cout<<"Ammo: "<<game->player->current_ammo<<endl;
            cout<<"Score: "<<game->current_score<<endl;
            if(game->leaderboard.head_leaderboard_entry)
                if(game->current_score > game->leaderboard.head_leaderboard_entry->score) {
                    cout<<"High Score: "<<game->current_score;
                }
                else {
                    cout<<"High Score: "<<game->leaderboard.head_leaderboard_entry->score<<endl;
                }
            else {
                cout<<"High Score: "<<game->current_score<<endl;
            }
            game->print_space_grid();
            cout<<endl;
            is_printed = true;
        }else if(line == "NOP") {
        }
        else {
            cout<<"Unknown command: "<<line<<endl;
        }if(!is_printed) {
            game->update_space_grid();
        }
        game->current_score++;
        if(game->game_over) {
            break;
        }
    }
    game->update_space_grid();
    print_gameover();
}
// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}

void GameController::print_gameover() const {
    if(game->game_over) {
        cout<<"GAME OVER!"<<endl;
        game->current_score--;
    }else {
        cout<<"GAME FINISHED! No more commands!"<<endl;
    }
    game->leaderboard.insert(new LeaderboardEntry(game->current_score,time(nullptr),game->player->player_name));
    cout<<"Tick: "<<game->game_time<<endl;
    cout<<"Lives: "<<game->player->lives<<endl;
    cout<<"Ammo: "<<game->player->current_ammo<<endl;
    cout<<"Score: "<<game->current_score<<endl;
    cout<<"High Score: "<<game->leaderboard.head_leaderboard_entry->score<<endl;
    cout<<"Player: "<<game->player->player_name<<endl;
    game->print_space_grid();
    cout<<endl;
    game->leaderboard.print_leaderboard();
    game->leaderboard.write_to_file(game->leaderboard_file_name);
}
