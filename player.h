//
//  player.h
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#ifndef player_h
#define player_h

#include <iostream>
#include <string>
#include <vector>
#include "utilities.h"
#include "object.h"
#include "level.h"
using namespace std;

class map;
class object;
class monsters;

class Player{
public:
    Player(map* map, int r, int c) :
    p_map(map), p_r(r), p_c(c), p_hit(20), cur_hit(20), p_strength(2), p_armor(2), p_dexterity(2), playerWin(false), symbol('@'), curr_weapon(new ShortSword()), p_sleep(0) {
        inventory.push_back(curr_weapon);
    };
    ~Player() { delete curr_weapon; }
    // actions
    bool move(char dir);
    void attack(monsters* monster);
    bool pick(int r, int c);
    void wield(char ch);
    void read(char ch);
    void check_invent();
    void cheat();
    void regain_hit();
    void reduceSleepTime() { if (p_sleep > 0) p_sleep--; }
    void setSleepTime(int sleepTime) { p_sleep = sleepTime; }
    bool getPickedUp() const { return pickup; }
    
    // Player info
    int playerHit() { return cur_hit; }
    int playerArmor() { return p_armor; }
    int playerStrength() { return p_strength; }
    int playerDexterity() { return p_dexterity; }
    bool playerDead() { return (cur_hit <= 0); }
    bool playerAsleep() { return (p_sleep > 0); }
    int playerSleepTime() { return p_sleep; }
    object* playerWeapon() { return curr_weapon; }
    map* playerMap() { return p_map; }
    void playerHurt(int damagePoints) { cur_hit -= damagePoints; }
    bool playerHasWin() { return playerWin; }
    vector<object*> getInventory(){ return inventory; }
    string attacked() { return p_attack; }
    bool die() const { return cur_hit <= 0; };
    bool playerMoved() const { return moved; }

    // Player position
    int row() { return p_r; }
    int col() { return p_c; }
    void set_row(int row) { p_r = row; }
    void set_col(int col) { p_c = col; }
    bool teleport(int r, int c);
    
    
private:
    int p_hit, p_armor, p_strength, p_dexterity, cur_hit, p_sleep;
    int p_r, p_c;
    char symbol;
    string p_attack = "";
    vector<object*> inventory;
    bool playerWin = false, dead = false, pickup, moved = false;
    map* p_map;
    object* curr_weapon;
};

#endif /* player_h */
//
