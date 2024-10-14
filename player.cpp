//
//  player.cpp
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#include "player.h"
#include "level.h"

// actions
//void descend();


void Player::check_invent(){
    if (playerAsleep()){
        reduceSleepTime();
        return;
    }
    
    clearScreen();
    cout<<"Inventory:"<<endl;
    for (int i = 0; i < inventory.size(); i++){
        char ch = 'a' + i;
        if (inventory[i]->objSymbol() == ')'){
            cout<<ch<<". "<<inventory[i]->display_name()<<endl;
        }
        if (inventory[i]->objSymbol() == '?'){
            cout<<ch<<". a scroll called "<<inventory[i]->display_name()<<endl;
        }
    }
    cout<<endl;
}

void Player::wield(char ch){
    int index = ch - 'a';
    if (index >= 0 && index < inventory.size()){
        if (inventory[index]->objSymbol() == ')'){
            playerMap()->display();
            curr_weapon = inventory[index];
            cout<<"You are wielding "<<curr_weapon->display_name()<<endl;
        }
        else {
            playerMap()->display();
            cout<<"You can't wield "<<inventory[index]->display_name()<<endl;
        }
    }
    else {
        playerMap()->display();
    }
}

void Player::read(char ch){
//    check_invent();
    int index = ch - 'a';
    if (index >= 0 && index < inventory.size()){
        if (inventory[index]->objSymbol() == '?'){
            playerMap()->display();
            cout<<"You read the scroll called "<<inventory[index]->display_name()<<endl;
            cout<<inventory[index]->display_action()<<endl;
            if (inventory[index]->scrollType() == 'S')
                p_strength += randInt(1, 3);
            else if (inventory[index]->scrollType() == 'A')
                p_armor += randInt(1, 3);
            else if (inventory[index]->scrollType() == 'H')
                p_hit += randInt(3, 8);
            else if (inventory[index]->scrollType() == 'D')
                p_dexterity++;
            else if (inventory[index]->scrollType() == 'T'){
                int tr = randInt(1, MAXROW-1);
                int tc = randInt(1, MAXCOL-1);
                while (!teleport(tr, tc)){
                     tr = randInt(1, MAXROW-1);
                     tc = randInt(1, MAXCOL-1);
                }
                
            }
            inventory.erase(begin(inventory)+index);
        }
        else {
            playerMap()->display();
            cout<<"You can't read a "<<inventory[index]->display_name()<<endl;
        }
    }
    else {
        playerMap()->display();
    }
}

bool Player::teleport(int r, int c){
    if (p_map->getChar(r, c) != walls && !p_map->monstersAt(r, c)){
        p_r = r;
        p_c = c;
        return true;
    }
    return false;
}

bool Player::move(char dir){
    moved = false;
    // if asleep
    if (playerAsleep()){
        reduceSleepTime();
        return false;
    }
    
    // move to blank space
    int r = p_r;
    int c = p_c;
    if (p_map->newPosition(r, c, dir)){
        // If there's no monster at position && the position is not wall
        if ((!p_map->monstersAt(r, c)) && p_map->getChar(r, c)!= walls) {
            p_r = r;
            p_c = c;
//            moved = true;
            return true;
        }
        else {
            monsters* opponent = nullptr;
            for (int i = 0; i < p_map->getMonsters().size(); i++){
                if (p_map->getMonsters()[i]->monsterRow() == r && p_map->getMonsters()[i]->monsterCol() == c){
                    opponent = p_map->getMonsters()[i];
                    break;
                }
            }
            if (opponent != nullptr){
                attack(opponent);
                return true;
            }
        }
    }
        return false;
}

void Player::attack(monsters* monster){
    p_attack = "";
    bool hit = false;
    moved = true;
    int attackerPoints = p_dexterity + curr_weapon->weaponDexterity();
    int defenderPoints = monster->monsterDexterity() + monster->monsterArmor();
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)){
        hit = true;
    }
    // successful hits
    if (hit){
        int damagePoints = randInt(0, p_strength+curr_weapon->weaponDamage()-1);
        monster->monsterHurt(damagePoints);
        // kill monsters
        if (monster->monsterDead()){
            if (curr_weapon->display_name() == "magic fangs of sleep"){
                p_attack = "Player strikes magic fangs at " + monster->monsterName() + " dealing a final blow. \n";
                p_map->removeMonsters(monster);
            }
            else {
                p_attack = "Player " + curr_weapon->display_action()+ " " + curr_weapon->display_name() + " at the " + monster->monsterName() + " dealing a final blow. \n";
                p_map->removeMonsters(monster);
            }
        }
        // didn't kill
        else {
            if (curr_weapon->display_name() == "magic fangs of sleep"){
                bool successSleep = trueWithProbability(0.2);
                if (successSleep){
                    int sleepTime = randInt(2, 6);
                    if (monster->Asleep()){
                        if (monster->monsterSleep() < sleepTime){
                            monster->monsterAsleep(sleepTime);
                        }
                    }
                    else {
                        monster->monsterAsleep(sleepTime);
                    }
                    p_attack = "Player strikes magic fangs at " + monster->monsterName() + " and hits, putting " + monster->monsterName() + " to sleep. \n";
                }
                else {
                    p_attack = "Player strikes magic fangs at " + monster->monsterName() + " and hits. \n";
                }
            }
            else {
                p_attack = "Player " + curr_weapon->display_action()+ " " + curr_weapon->display_name() + " at the " + monster->monsterName() + " and hits. \n";
            }
        }
    }
    // unsuccessful hits
    else {
        if (curr_weapon->display_name() == "magic fangs of sleep"){
            p_attack = "Player strikes magic fangs at " + monster->monsterName() + " and misses. \n";
        }
        else{
            p_attack = "Player " + curr_weapon->display_action() + " " + curr_weapon->display_name() + " at the " + monster->monsterName() + " and misses. \n";
        }
    }
}

bool Player::pick(int r, int c){
    if (playerAsleep()){
        reduceSleepTime();
        return false;
    }
    // pick up gold idol
    if (playerMap()->getChar(r, c)==gold_idol){
        playerWin = true;
        return true;;
    }
    // pick others
    // full inventory
    if (inventory.size() >= 25){
        cout<<"Your knapsack is full; you can't pick that up."<<endl;
        return false;
    }
    else {
        object* just_picked = playerMap()->pickObject(row(), col());
        if (just_picked != nullptr){
            inventory.push_back(just_picked);
            pickup = true;
            return true;
        }
        else {
            return false;
        }
    }
}

void Player::cheat(){
    if (playerAsleep()){
        reduceSleepTime();
        return;
    }
    p_strength = 9;
    cur_hit = 50;
}

void Player::regain_hit(){
    bool heal = trueWithProbability(0.1);
    if (heal){
        if (cur_hit < p_hit){
            cur_hit++;
        }
    }
}
