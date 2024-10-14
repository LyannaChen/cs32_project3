//
//  level.hpp
//  project 3
//
//  Created by Lyanna Chen on 5/18/24.
//

#ifndef level_h
#define level_h

#include "utilities.h"
#include <iostream>
#include <string>
#include <vector>
#include "player.h"
#include "monsters.h"
#include "object.h"
#include <algorithm>
using namespace std;

const int MAXROW = 18;
const int MAXCOL = 70;

const char walls = '#';
const char stairs = '>';
const char gold_idol = '&';
const char weapon = ')';
const char scrolls = '?';
const char space = ' ';
//const char player = '@';

class room{
public:
//    room(int left, int right, int top, int bottom) : left(left), right(right), top(top), bottom(bottom) {};
    room(int row, int col, int x, int y) : row(row), col(col), x(x), y(y) {};
    
    int centerCol() const;
    int centerRow() const;
    bool overlap(room other);
    bool operator<(room other) const { return this->centerCol() < other.centerCol(); }
    
private:
    int row, y, col, x;
};

class map{
public:
    map(int level);
    ~map();
    
    // Basic Map
    void display();
    void createRoom(int r_row, int r_col, int r_x, int r_y);
    void corridor(room r1, room r2);
    void createMap(int n_rooms);
    void increaseLevel() { if (m_level < 4) m_level++; }
    void createLevel(int level);
    
    // Accessors
    int row() const { return m_row; }
    int col() const { return m_col; }
    int level() const { return m_level; }
    char getChar(int r, int c) const { return grid[r][c]; }
    Player* player() const { return m_player; }
    vector<monsters*> getMonsters() const { return m_monsters; }
    vector<object*> getObject() const { return m_objects; }
    bool ObjectAt(int r, int c) const;
    bool hasAttacked() const { return attacked; }
    bool monstersAt(int r, int c) const;
    void setChar (int r, int c, char temp) { grid[r][c] = temp; }
    
    // Game Objects
    bool addPlayer(int r, int c);
    bool addStairs(int r, int c);
    bool addIdol(int r, int c);
    bool addMonsters(int r, int c, char monster);
    void removeMonsters(monsters* dead_monster);
    bool addObjects(int r, int c, char objects);
    void removeObject(int r, int c);
    object* pickObject(int r, int c);
    void dropObject(int r, int c, char monster);
    bool newPosition(int& r, int& c, char dir) const;
    bool isPlayerAt(int r, int c) const;
private:
    int m_row, m_col, m_level = 0;
    char grid[MAXROW][MAXCOL];
    Player* m_player;
    vector<object*> m_objects;
    vector<monsters*> m_monsters;
    vector<room> m_room;
    bool attacked;
};


#endif /* level_h */
