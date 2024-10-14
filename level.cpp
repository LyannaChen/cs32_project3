//
//  level.cpp
//  project 3
//
//  Created by Lyanna Chen on 5/18/24.
//

#include "level.h"

// class room ----------------------------------------
int room::centerCol() const{ return (x+col/2); }
int room::centerRow() const{ return (y+row/2); }
bool room::overlap(room other) {
    if (other.x <= x+col && other.y <= y+row && x <= other.x + other.col && y <= other.y + other.row){
        return true;
    }
    return false;
}

// class map -----------------------------------------
map::map(int level) : m_level(level), m_player(nullptr) {}

map::~map(){
    for (vector<monsters*>::iterator p = m_monsters.begin(); p != m_monsters.end();)
    {
        delete *p;
        p= m_monsters.erase(p);
    }
    m_monsters.clear();
    for (vector<object*>::iterator q = m_objects.begin(); q != m_objects.end();)
    {
        delete *q;
        q = m_objects.erase(q);
    }
    m_objects.clear();
    delete m_player;
    
}

void map::createMap(int n_rooms){
    for (int i = 0; i < n_rooms; i++){
        int row = randInt(6, 10);
        int col = randInt(12, 18);
        
        int x = randInt(2, MAXCOL - col - 2);
        int y = randInt(2, MAXROW - row - 2);
        room r = room(row, col, x, y);
        m_room.push_back(r);
        bool overlap = false;
        
        for (int j = 0; j < i; j++){
            if (m_room[j].overlap(m_room[j+1])){
                m_room.pop_back();
                overlap = true;
                i--;
                break;
            }
        }
        if (!overlap){
            createRoom(row, col, x, y);
        }
        
    }
    
    sort(m_room.begin(), m_room.end());
    
    for (int m = 0; m < m_room.size(); m++)
    {
        if (m != m_room.size()-1)
        {
            corridor(m_room[m], m_room[m+1]);
            
        }
    }
}

void map::createLevel(int level){
    // initialize with walls
    for (int r = 0; r < MAXROW; r++)
        for (int c = 0; c < MAXCOL; c++)
            grid[r][c] = walls;
    
    // clear out previous level
    if (level > 0){
        for (vector<monsters*>::iterator p = m_monsters.begin(); p != m_monsters.end(); ) {
            delete *p;
            p = m_monsters.erase(p);
        }
        
        for (vector<object*>::iterator q = m_objects.begin(); q != m_objects.end(); ){
            delete *q;
            q = m_objects.erase(q);
        }
        
        m_room.clear();
    }
    
    // add rooms
    int n_rooms = randInt(3, 5);
    createMap(n_rooms);
    
    // add player (for level 0), teleport player to different spot for other levels
    int r_player = randInt(1, MAXROW-1);
    int c_player = randInt(1, MAXCOL-1);
    if (level == 0){
        while (!addPlayer(r_player, c_player)){
            r_player = randInt(1, MAXROW-1);
            c_player = randInt(1, MAXCOL-1);
        }
    }
    else {
        while (!m_player->teleport(r_player, c_player)){
            r_player = randInt(1, MAXROW-1);
            c_player = randInt(1, MAXCOL-1);
        }
    }
//    display();
    
    // add objects
    int r_obj = randInt(1, MAXROW-1);
    int c_obj = randInt(1, MAXCOL-1);
    int n_objects = randInt(2, 3);
    for (int o = 0; o < n_objects; o++) {
        int SorW = randInt(1, 2);
        do {
            r_obj = randInt(1, MAXROW - 1);
            c_obj = randInt(1, MAXCOL - 1);
        } while (!addObjects(r_obj, c_obj, (SorW == 1) ? ')' : '?'));
    }


    // add monsters
    int n_monsters = randInt(2, 5*(m_level+1)+1);
    int r_mon = randInt(1, MAXROW-1);
    int c_mon = randInt(1, MAXCOL-1);
    // level 0, 1 -> only goblins and snakewomen
    if (level == 1 || level == 0){
        for (int m = 0; m < n_monsters; m++){
            int monsterType = randInt(1, 2);
            if (monsterType == 1){
                while (!addMonsters(r_mon, c_mon, 'S')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
            if (monsterType == 2){
                while (!addMonsters(r_mon, c_mon, 'G')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
        }
    }
    // level 2 -> goblins, snakewomen, and bogeymen
    if (level == 2){
        for (int m = 0; m < n_monsters; m++){
            int monsterType = randInt(1, 3);
            if (monsterType == 1){
                if (monsterType == 1){
                    while (!addMonsters(r_mon, c_mon, 'S')){
                        r_mon = randInt(1, MAXROW-1);
                        c_mon = randInt(1, MAXCOL-1);
                    }
                }
                if (monsterType == 2){
                    while (!addMonsters(r_mon, c_mon, 'G')){
                        r_mon = randInt(1, MAXROW-1);
                        c_mon = randInt(1, MAXCOL-1);
                    }
                }
                if (monsterType == 3){
                    while (!addMonsters(r_mon, c_mon, 'B')){
                        r_mon = randInt(1, MAXROW-1);
                        c_mon = randInt(1, MAXCOL-1);
                    }
                }
            }
        }
    }
    // level 3, 4 -> all four monsters
    if (level == 3 || level == 4){
        for (int m = 0; m < n_monsters; m++){
            int monsterType = randInt(1, 4);
            if (monsterType == 1){
                while (!addMonsters(r_mon, c_mon, 'S')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
            if (monsterType == 2){
                while (!addMonsters(r_mon, c_mon, 'G')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
            if (monsterType == 3){
                while (!addMonsters(r_mon, c_mon, 'B')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
            if (monsterType == 4){
                while (!addMonsters(r_mon, c_mon, 'D')){
                    r_mon = randInt(1, MAXROW-1);
                    c_mon = randInt(1, MAXCOL-1);
                }
            }
        }
    }
    // add statue (only for level 4)
    if (level == 4){
        int r_idol = randInt(1, MAXROW-1);
        int c_idol = randInt(1, MAXCOL-1);
        while (!addIdol(r_idol, c_idol)){
            r_idol = randInt(1, MAXROW-1);
            c_idol = randInt(1, MAXCOL-1);
        }
    }
    
    // add stairs (except level 4)
    if (level != 4){
        int r_stair = randInt(1, MAXROW-1);
        int c_stair = randInt(1, MAXCOL-1);
        while (!addStairs(r_stair, c_stair)){
            r_stair = randInt(1, MAXROW-1);
            c_stair = randInt(1, MAXCOL-1);
        }
    }
}

void map::display(){
    char local_grid[MAXROW][MAXCOL];
    int r, c;
    for (r = 0; r < MAXROW; r++){
        for (c = 0; c < MAXCOL; c++){
            local_grid[r][c] = grid[r][c];
        }
    }
    
    // place objects
    if (m_objects.size() > 0){
        for (int i = 0; i < m_objects.size(); i++){
            char& objChar = local_grid[m_objects[i]->objRow()][m_objects[i]->objCol()];
            if (objChar != walls){
                objChar = m_objects[i]->objSymbol();
            }
        }
    }

    
    // place monsters
    if (m_monsters.size() > 0){
        for (int i = 0; i < m_monsters.size(); i++){
            char& monChar = local_grid[m_monsters[i]->monsterRow()][m_monsters[i]->monsterCol()];
            if (monChar != walls){
                monChar = m_monsters[i]->monsterSymbol();
            }
        }
    }
    // place player
    if (m_player != nullptr){
        char& playerChar = local_grid[m_player->row()][m_player->col()];
        if (playerChar != walls)
            playerChar = '@';
        else
            playerChar = '!';
    }
    // print out grid
    clearScreen();
    for (r = 0; r < MAXROW; r++){
        for (c = 0; c < MAXCOL; c++){
            cout<<local_grid[r][c];
        }
        cout<<endl;
    }
    cout<<endl;
    
    // print out player info
    cout<<"Level: "<<level()<<", Hit points: "<<m_player->playerHit()<<", Armor: "<<m_player->playerArmor()<<", Strength: "<<m_player->playerStrength()<<", Dexterity: "<<m_player->playerDexterity()<<endl;
    cout<<endl;
    
    // print out attack info
    if (m_player->playerMoved()){
        cout<<m_player->attacked();
    }
    
    for (int i = 0; i < m_monsters.size(); i++){
        if (m_monsters[i]->monsterMoved()){
            cout<<m_monsters[i]->attacked();
        }
    }
}

void map::createRoom(int r_row, int r_col, int r_x, int r_y){
    int r, c;
    for (r = r_y; r < r_row + r_y; r++){
        for (c = r_x; c < r_col + r_x; c++){
//            if (!inBound(r, c))
                grid[r][c] = space;
        }
    }
}

void map::corridor(room r1, room r2){
    int cr = r1.centerRow();
    int cc = r1.centerCol();
    
    while (cc != r2.centerCol()){
        grid[cr][cc] = space;
        cc++;
    }
    
    if (cr > r2.centerRow()){
        while (cr != r2.centerRow()){
            grid[cr][cc] = space;
            cr--;
            grid[cr][cc] = space;
        }
    }
    else if (cr < r2.centerRow())
    {
        while (cr != r2.centerRow())
        {
            grid[cr][cc] = space;
            cr++;
            grid[cr][cc] = space;
        }
    }
    else
        return;
}

bool map::addPlayer(int r, int c){
    if (m_player != nullptr)
        return false;
    
    if (getChar(r, c) != walls && !monstersAt(r, c)){
        m_player = new Player(this, r, c);
        return true;
    }
    return false;
}

bool map::addMonsters(int r, int c, char monster){
    if (getChar(r, c) != walls && (r != player()->row()) && (c != player()->col()) && !monstersAt(r, c) && getChar(r, c) != gold_idol){
        switch (monster){
            case 'S':
                m_monsters.push_back(new SnakeWomen(this, r, c));
                break;
            case 'B':
                m_monsters.push_back(new Bogeymen(this, r, c));
                break;
            case 'D':
                m_monsters.push_back(new Dragons(this, r, c));
                break;
            case 'G':
                m_monsters.push_back(new Goblins(this, r, c));
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

bool map::addObjects(int r, int c, char objects){
    if (getChar(r, c) == space){
        switch (objects){
            case ')':
                switch (randInt(1, 3)){
                    case 1:
                        m_objects.push_back(new Mace(r, c));
                        break;
                    case 2:
                        m_objects.push_back(new ShortSword(r, c));
                        break;
                    case 3:
                        m_objects.push_back(new LongSword(r, c));
                        break;
                    default:
                        break;
                }
                break;
            case '?':
                switch (randInt(1, 4)){
                    case 1:
                        m_objects.push_back(new Strength(r, c));
                        break;
                    case 2:
                        m_objects.push_back(new Health(r, c));
                        break;
                    case 3:
                        m_objects.push_back(new Armor(r, c));
                        break;
                    case 4:
                        m_objects.push_back(new Dexterity(r, c));
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

bool map::addStairs(int r, int c){
    if (getChar(r, c) == space){
        grid[r][c] = stairs;
        return true;
    }
    return false;
}

bool map::addIdol(int r, int c){
    if (getChar(r, c) == space){
        grid[r][c] = gold_idol;
        return true;
    }
    return false;
}

bool map::newPosition(int& r, int& c, char dir) const {
    switch (dir)
    {
      case ARROW_UP:     if (r <= 1)      return false; else r--; break;
      case ARROW_DOWN:   if (r >= MAXROW) return false; else r++; break;
      case ARROW_LEFT:   if (c <= 1)      return false; else c--; break;
      case ARROW_RIGHT:  if (c >= MAXCOL) return false; else c++; break;
      default:     return false;
    }
    return true;
}

bool map::monstersAt(int r, int c) const{
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if (m_monsters[i]->monsterRow() == r && m_monsters[i]->monsterCol() == c)
        {
            return true;
        }
    }
    return false;
}

void map::dropObject(int r, int c, char monster){
    int n_scroll = randInt(4);
    int n_weapon = randInt(2);
    switch (monster)
    {
        case 'B':
            m_objects.push_back(new MagicAxe(r,c));
            break;
        case 'S':
            m_objects.push_back(new MagicFang(r,c));
            break;
        case 'D':
            switch (n_scroll)
        {
            case 0:
                m_objects.push_back(new Teleport(r,c));
                break;
            case 1:
                m_objects.push_back(new Strength(r,c));
                break;
            case 2:
                m_objects.push_back(new Health(r,c));
                break;
            case 3:
                m_objects.push_back(new Armor(r,c));
                break;
            case 4:
                m_objects.push_back(new Dexterity(r,c));
                break;
            default:
                break;
        }
            break;
        case 'G':
            switch (n_weapon)
            {
                case 0:
                    m_objects.push_back(new MagicAxe(r,c));
                    break;
                case 1:
                    m_objects.push_back(new MagicFang(r,c));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
    

object* map::pickObject(int r, int c){
    for (int i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i]->objRow() == r && m_objects[i]->objCol() == c)
        {
            return m_objects[i];
        }
    }
    return nullptr;
}

void map::removeMonsters(monsters* dead_monster){
    for (vector<monsters*>::iterator p = m_monsters.begin(); p != m_monsters.end();)
    {
        if (*p == dead_monster)
        {
            delete *p;
            p = m_monsters.erase(p);
        }
        else
            p++;
    }
}

void map::removeObject(int r, int c){
    if (m_player->getPickedUp())
    {
        for (vector<object*>::iterator p = m_objects.begin(); p != m_objects.end();)
        {
            if (*p == pickObject(r, c))
            {
                p = m_objects.erase(p);
            }
            else
            {
                p++;
            }
        }
    }
}

bool map::ObjectAt(int r, int c) const{
    if (grid[r][c] == '?' || grid[r][c] == ')'){
        return true;
    }
    else{
        return false;
    }
}

bool map::isPlayerAt(int r, int c) const{
    return (m_player!=nullptr && m_player->row() == r && m_player->col() == c);
}
