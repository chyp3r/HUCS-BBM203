#include "AsteroidDash.h"
#include "fstream"
#include <sstream>
#include <algorithm>
#include <memory>

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {
    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    string line;
    vector<int> vector;
    int temp;

    while(getline(file,line)) {
        istringstream ss(line);
        while(ss>>temp) {
            vector.push_back(temp);
        }
        space_grid.push_back(vector);
        vector.clear();
    }
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    std::ifstream file(player_file_name);
    string line;
    int row,col;
    char temp;
    vector<vector<bool>> player_grid;
    vector<bool> player_temp_grid;
    file>>row>>col;
    file.ignore();
    while(getline(file,line)) {
        istringstream ss(line);
        while(ss>>temp) {
            player_temp_grid.push_back(temp == '1');
        }
        player_grid.push_back(player_temp_grid);
        player_temp_grid.clear();
    }
    player = new Player(player_grid,row,col,player_name);
    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    string line;
    vector<vector<bool>> shape;
    vector<bool> temp_shape;
    bool is_shape = false,is_s = false,is_t=false,is_e=false;
    string left_top;
    string time;
    int type = 0;
    ObjectType types[] = {ASTEROID,AMMO,LIFE_UP};
    bool is_pushed = false;
    while(getline(file,line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty() && !is_pushed) {
            is_shape = false,is_s = false,is_t=false,is_e=false;
            try {
                generate_celestial(shape,types[type],stoi(left_top),stoi(time));
                is_pushed = true;
            }catch (...) {
            }
        }

        istringstream ss(line);
        char ch;
        if(is_shape) {
            shape.push_back(temp_shape);
        }
        temp_shape.clear();
        while(ss.get(ch)) {
            string s(1,ch);
            if(s==":") {
                continue;
            }
            if(s=="[" || s=="{"){
                is_pushed = false;
                shape.clear();
                temp_shape.clear();
                left_top = "";
                time = "";
                type = 0;
                is_shape = true;
                continue;
            }
            if(s == "]" || s=="}" ){
                shape.push_back(temp_shape);
                temp_shape.clear();
                is_shape = false;
                continue;
            }
            if(s=="s") {
                is_s = true;
                continue;
            }
            if(s=="t") {
                is_s = false;
                is_t = true;
                continue;
            }
            if(s=="e") {
                is_t = false;
                is_e = true;
                continue;
            }

            if(is_shape && isdigit(ch)) {
                temp_shape.push_back(stoi(s));
            }
            if(is_s && isdigit(ch)) {
                left_top+=s;
            }
            if (is_t && isdigit(ch)) {
                time+=s;
            }
            if(is_e) {
                if(s =="a") {
                    type =1;
                }else if(s=="l") {
                    type =2;
                }
                is_e = false;
            }
        }
    }

    if(!is_pushed) {
        try {
            generate_celestial(shape,types[type],stoi(left_top),stoi(time));
        }catch (...) {
        }
    }

    file.close();
}
CelestialObject AsteroidDash::rotate_obj(CelestialObject& obj){
    vector<vector<bool>> rotated(obj.shape[0].size(),vector<bool>(obj.shape.size()));
    for(int i=0;i<obj.shape.size();i++) {
        for(int b=0;b<obj.shape[0].size();b++) {
            rotated[obj.shape[0].size()-b-1][i]=obj.shape[i][b];
        }
    }
    return {rotated,obj.object_type,obj.starting_row,obj.left_tick};
}

bool is_equal(CelestialObject i1,CelestialObject i2) {
    if (i1.shape.size()!=i2.shape.size()) return false;
    for(int i=0;i<i1.shape.size();i++) {
        if (i1.shape[i].size()!=i2.shape[i].size()) return false;
        for(int b=0;b<i1.shape[i].size();b++) {
            if(i1.shape[i][b] != i2.shape[i][b]) {
                return false;
            }
        }
    }
    return true;
}

void AsteroidDash::generate_celestial(const vector<vector<bool>>& shape,ObjectType type,int left_top,int time) {
    auto* new_celestial = new CelestialObject(shape,type,left_top,time);
    auto* pre_celestial = new_celestial;

    for(int i=0;i<3;i++) {
        auto rotatedObj = rotate_obj(*pre_celestial);
        const auto* temp = new_celestial;
        bool found = false;

        while(temp != nullptr) {
            if(is_equal(temp,rotatedObj)) {
                found = true;
                break;
            }
            temp= temp->left_rotation;
        }
        if(!found) {
            auto* tempObj = new CelestialObject(rotatedObj.shape, rotatedObj.object_type, rotatedObj.starting_row, rotatedObj.time_of_appearance);
            pre_celestial->left_rotation=tempObj;
            tempObj->right_rotation=pre_celestial;
            pre_celestial = tempObj;
        }else {
            break;
        }
    }

    pre_celestial->left_rotation = new_celestial;
    new_celestial->right_rotation= pre_celestial;

    if(celestial_objects_list_head == nullptr) {
        celestial_objects_list_head = new_celestial;
    }else {
        auto* temp = celestial_objects_list_head;
        while(temp->next_celestial_object != nullptr) {
            temp = temp->next_celestial_object;
        }
        temp->next_celestial_object = new_celestial;
        temp->left_rotation->next_celestial_object = new_celestial;
        temp->left_rotation->left_rotation->next_celestial_object = new_celestial;
        temp->left_rotation->left_rotation->left_rotation->next_celestial_object = new_celestial;
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    for(const vector<int>& i:space_grid) {
        for(const int b : i) {
            if(b == 0) {
                std::cout<<unoccupiedCellChar;
            }else{
                std::cout<<occupiedCellChar;
            }
        }
        std::cout<<endl;
    }
}

void AsteroidDash::hit_rotate(CelestialObject& obj,const bool upper) {
    if(upper) {
        obj.shape = rotate_obj(obj).shape;
        obj.shape = rotate_obj(obj).shape;
        obj.shape = rotate_obj(obj).shape;
        obj.left_rotation->shape = obj.shape;
        obj.left_rotation->left_rotation->shape = obj.shape;
        obj.left_rotation->left_rotation->left_rotation->shape = obj.shape;
        auto temp = obj.left_rotation;
        int rotate_count = 1;
        while(temp != &obj) {
            for(int i =0;i<rotate_count;i++) {
                temp->shape = rotate_obj(*temp).shape;
            }
            rotate_count++;
            temp = temp->left_rotation;
        }
    }
    else {
        obj.shape = rotate_obj(obj).shape;
        obj.left_rotation->shape = obj.shape;
        obj.left_rotation->left_rotation->shape = obj.shape;
        obj.left_rotation->left_rotation->left_rotation->shape = obj.shape;
        auto temp = obj.left_rotation;
        int rotate_count = 1;
        while(temp != &obj) {
            for(int i =0;i<rotate_count;i++) {
                temp->shape = rotate_obj(*temp).shape;
            }
            rotate_count++;
            temp = temp->left_rotation;
        }
    }
}

bool AsteroidDash::is_in_player(const int row,const int col) const {
    for (int i = 0;i<player->spacecraft_shape.size();i++) {
        for(int b=0;b<player->spacecraft_shape[0].size();b++) {
            if(player->spacecraft_shape[i][b] == 1 && row == player->position_row+i && col == player->position_col+b) {
                return true;
            }
        }
    }
    return false;
}
// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    clear();

    // Projectile Movement
    for(auto & project_tile : project_tiles) {
        project_tile.col+=1;
        if(project_tile.col<space_grid[0].size()) {
            space_grid[project_tile.row][project_tile.col] = 1;
        }
    }

    // Player Movement
    const int row = player->position_row;
    const int col = player->position_col;
    int row_counter=0,col_counter=0;
    for(const auto& data:player->spacecraft_shape) {
        for(const bool d:data) {
            if(d != 0 && player->lives >0)
                space_grid[row+row_counter][col+col_counter] = d;
            col_counter++;
        }
        row_counter++;
        col_counter=0;
    }

    // Obj Movement
    auto temp = celestial_objects_list_head;
    CelestialObject *last = nullptr;
    while(temp != nullptr) {
        if(temp->left_tick >=1) {
            temp->left_tick -=1;
        }else {
            if(temp->active_col != temp->max_active_col) {
                temp->active_col+=1;
            }
            temp->left_top_col+=1;
            const int coordinat = space_grid[0].size() - temp->left_top_col;
            if(coordinat+1<space_grid[0].size()) {
                obj_movement(temp,coordinat+1);
            }
            obj_movement(temp,coordinat);

            if(!(temp->destroy)) {
                int row_counter=0;
                for(int i=0;i<temp->shape.size();i++) {
                    for(int b=0;b<temp->active_col;b++) {
                        if (coordinat + b >= space_grid[0].size() || (temp->starting_row + row_counter) >= space_grid.size()) {
                            continue;
                        }
                        if(temp->shape[i][b] !=0) {
                            space_grid[temp->starting_row+row_counter][coordinat+b] = 1;
                        }
                    }
                    row_counter++;
                }
            }
        }

        if(!(temp->destroy)) {
            last = temp;
            temp = temp->next_celestial_object;
        }else {
            if(last != nullptr) {
                last->next_celestial_object = temp->next_celestial_object;
            }else {
                celestial_objects_list_head = temp->next_celestial_object;
            }
            CelestialObject::delete_rotations(temp);
            delete temp;
            if(last != nullptr) {
                temp = last->next_celestial_object;
            }else {
                temp = celestial_objects_list_head;
            }
        }
    }
}

void AsteroidDash::obj_movement(CelestialObject* temp,int coordinat) {
    bool hit = false;
    int row_counter=0;
    int hit_row = 0;
        for(int i=0;i<temp->shape.size();i++) {
            for(int b=0;b<temp->active_col;b++) {
                if (coordinat + b >= space_grid[0].size() || (temp->starting_row + row_counter) >= space_grid.size()) {
                    continue;
                }

                if(coordinat+b-1<0) {
                    continue;
                }
                if(temp->shape[i][b] != 0) {
                    if(temp->object_type == ASTEROID){
                        const int final_coord = (coordinat) + b;
                        if(delete_projecttile(temp->starting_row + row_counter, final_coord)) {
                            temp->shape[i][b] = false;
                            space_grid[temp->starting_row + row_counter][final_coord] = 0;
                            hit = true;
                            hit_row = i;
                            temp->particle_count--;
                            current_score+=10;
                            if(temp->particle_count == 0) {
                                current_score += (temp->max_particle_count-temp->particle_count) *100;
                            }
                        }else if(is_in_player(temp->starting_row + row_counter, final_coord)){
                            if(temp->destroy == false) {
                                hit_check_game = true;
                                player->lives--;
                            }
                            temp->destroy= true;
                            if(player->lives == 0) {
                                game_over = true;
                            }
                        }
                    }else if(temp->object_type == LIFE_UP) {
                        const int final_coord = static_cast<int>(coordinat) + b;
                        if(is_in_player(temp->starting_row + row_counter, final_coord)){
                            if(temp->destroy == false)
                                player->lives++;
                            temp->destroy= true;
                        }
                    }else if(temp->object_type == AMMO) {
                        const int final_coord = static_cast<int>(coordinat) + b;
                        if(is_in_player(temp->starting_row + row_counter, final_coord)){
                            if(temp->destroy == false)
                                player->current_ammo = player->max_ammo;
                            temp->destroy= true;
                        }
                    }
                }
            }
            row_counter++;
        }

    if(hit) {
        const int middle = static_cast<int>(temp->shape.size()) / 2;
        if(temp->shape.size() % 2 == 0) {
            if(hit_row<=middle) {
                hit_rotate(*temp,true);
            }else {
                hit_rotate(*temp,false);
            }
        }else {
            if(hit_row<middle) {
                hit_rotate(*temp,true);
            }else if(hit_row == middle){
                temp->left_rotation->shape = temp->shape;
                temp->left_rotation->left_rotation->shape = temp->shape;
                temp->left_rotation->left_rotation->left_rotation->shape = temp->shape;
                auto temp2 = temp->left_rotation;
                int rotate_count = 1;
                while(temp2 != temp) {
                    for(int i =0;i<rotate_count;i++) {
                        temp2->shape = rotate_obj(*temp2).shape;
                    }
                    rotate_count++;
                    temp2 = temp2->left_rotation;
                }
            }else if(hit_row > middle){
                hit_rotate(*temp,false);
            }
        }
        temp->active_col = static_cast<int>(temp->shape[0].size());
        temp->max_active_col = static_cast<int>(temp->shape[0].size());
    }
}
bool AsteroidDash::delete_projecttile(const int row,const int col) {
    for(int i=0;i<project_tiles.size();i++) {
        const auto temp = project_tiles[i];
        if(temp.row == row && temp.col == col) {
            for(int index = i+1;index<project_tiles.size();index++) {
                project_tiles[index-1] = project_tiles[index];
            }
            project_tiles.pop_back();
            return true;
        }
    }
    return false;
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    if(player->current_ammo>0) {
        player->current_ammo-=1;
        const int middle = static_cast<int>(player->spacecraft_shape.size())/2;
        const int row = middle+player->position_row,col= player->position_col+player->length;
        project_tile temp{};
        temp.row = row;
        temp.col = col-1;
        project_tiles.push_back(temp);
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    delete player;
    auto temp = celestial_objects_list_head;
    while(temp!= nullptr) {
        const auto next = temp->next_celestial_object;
        CelestialObject::delete_rotations(temp);
        delete temp;
        temp = next;
    }
}

void AsteroidDash::clear() {
    for(auto & i : space_grid) {
        for(int & b : i) {
            b = 0;
        }
    }
}