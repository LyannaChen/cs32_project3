//
//  monsters.h
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#ifndef monsters_h
#define monsters_h

#include <iostream>
#include "utilities.h"
#include <string>
#include "object.h"
#include "level.h"
#include <algorithm>
#include <queue>
using namespace std;

const int MAXPOINTS = 99;
const int MINPOINTS = 0;

class map;
class Player;
class object;

class monsters{
public:
    monsters(map* map, int r, int c, int hit, int strength, int armor, int dexterity, int sleep, char symbol, string name) :
    m_map(map), m_row(r), m_col(c), m_hit(hit), m_strength(strength), m_armor(armor), m_dexterity(dexterity), m_sleep(sleep), m_symbol(symbol), m_name(name), m_weapon(nullptr) {};
    
    // information
    int monsterHit() { return m_hit; }
    int monsterStrength() { return m_strength; }
    int monsterArmor() { return m_armor; }
    int monsterDexterity() { return m_dexterity; }
    int monsterSleep() { return m_sleep; }
    char monsterSymbol() { return m_symbol; }
    int monsterRow() { return m_row; }
    int monsterCol() { return m_col; }
    string monsterName() { return m_name; }
    object* monsterWeapon() { return m_weapon; }
    map* monsterMap() { return m_map; }
    bool monsterDead() { return (m_hit <= 0); }
    bool Asleep() { return (m_sleep > 0); }
    string attacked() { return m_attack; }
    void setWeapon(object* newWeapon) { m_weapon = newWeapon; }
    void monsterHurt(int damagePoints) { m_hit -= damagePoints; }
    void monsterAsleep(int sleepTime) { m_sleep = sleepTime; }
    void reduceMonsterSleepTime(){ m_sleep--; }
    void attack(Player* player);
    void regain_hit() { m_hit++; }
    void setRow(int r) { m_row = r; }
    void setCol(int c) { m_col = c; }
    bool monsterMoved() { return m_move; }
    void clearMoved() { m_move = false; }

    
    virtual bool move(int smell); // for bogeymen and snakewomen
    virtual void deathDrop() = 0;
    virtual ~monsters() { delete m_weapon; };
    
private:
    int m_hit, m_strength, m_dexterity, m_armor, m_sleep;
    int m_row, m_col;
    char m_symbol;
    string m_name = "", m_attack = "";
    bool m_dead = false, m_move = false;
    object* m_weapon;
    map* m_map;
};

class Bogeymen : public monsters {
public:
    Bogeymen(map* map, int r, int c) :
    monsters(map, r, c, randInt(5, 10), randInt(2, 3), 2, randInt(2, 3), 0, 'B', "Bogeyman") {
        setWeapon(new ShortSword());
    };
    virtual ~Bogeymen() { deathDrop(); }
    virtual void deathDrop();
private:
};

class SnakeWomen : public monsters{
public:
    SnakeWomen(map* map, int r, int c) : 
    monsters(map, r, c, randInt(3, 6), 2, 3, 3, 0, 'S', "Snakewoman") {
        setWeapon(new MagicFang());
    };
    virtual ~SnakeWomen() {  deathDrop(); }
    virtual void deathDrop();
private:
};

class Dragons : public monsters{
public:
    Dragons(map* map, int r, int c) :
    monsters(map, r, c, randInt(20, 25), 4, 4, 4, 0, 'D', "Dragon") {
        setWeapon(new LongSword());
    };
    virtual ~Dragons() { deathDrop(); }
    virtual bool move();
    virtual void deathDrop();
private:
};

class Goblins : public monsters{
public:
    Goblins(map* map, int r, int c) :
    monsters(map, r, c, randInt(15, 20), 3, 1, 1, 0, 'G', "Goblin") {
        setWeapon(new ShortSword());
    };
    virtual ~Goblins() { deathDrop(); }
    virtual void deathDrop();
    virtual bool move(int distance);
//    bool smartMove(int sr, int sc, int er, int ec, int smellrange);
    void smartMove(int path[18][70], int r, int c, queue<int> rows, queue<int> cols);
private:
    int smellDistance;
};

#endif /* monsters_h */
