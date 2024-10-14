//
//  monsters.cpp
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#include "monsters.h"
#include <iostream>
#include "utilities.h"
#include "player.h"
#include <cstdlib>
#include "level.h"
#include <algorithm>
using namespace std;

class map;

void monsters::attack(Player* player){
    m_move = true;
    m_attack = "";
    bool hit = false;
    int attackerPoints = m_dexterity + m_weapon->weaponDexterity();
    int defenderPoints = player->playerDexterity() + player->playerArmor();
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)){
        hit = true;
    }
    // successful hits
    if (hit){
        int damagePoints = randInt(0, m_strength+m_weapon->weaponDamage()-1);
        player->playerHurt(damagePoints);
        // kill player
        if (player->playerDead()){
            if (m_weapon->display_name() == "magic fangs of sleep"){
                m_attack = "the Snakewoman strikes magic fangs at Player dealing a final blow. \n";
            }
            else {
                m_attack = "the " + this->monsterName() + " " + m_weapon->display_action() + " " + m_weapon->display_name() + " at Player dealing a final blow. \n";
            }
        }
        // didn't kill
        else {
            if (m_weapon->display_name() == "magic fangs of sleep"){
                bool successSleep = trueWithProbability(0.2);
                if (successSleep){
                    int sleepTime = randInt(2, 6);
                    if (player->playerAsleep()){
                        if (player->playerSleepTime() < sleepTime){
                            player->setSleepTime(sleepTime);
                        }
                    }
                    else {
                        player->setSleepTime(sleepTime);
                    }
                    m_attack = "the Snakewoman strikes magic fangs at Player and hits, putting Player to sleep. \n";
                }
                else {
                    m_attack = "the Snakewoman strikes magic fangs at Player and hits. \n";
                }
            }
            else {
                m_attack = "the " + this->monsterName() + " " + m_weapon->display_action() + " " + m_weapon->display_name() + " at Player and hits. \n";
            }
        }
    }
    // unsuccessful hits
    else {
        if (m_weapon->display_name() == "magic fangs of sleep"){
            m_attack = "the Snakewoman strikes magic fangs at Player and misses. \n";
        }
        else{
            m_attack = "the " + this->monsterName() + " " + m_weapon->display_action() + " " + m_weapon->display_name() + " at Player and misses. \n";
        }
    }
}

bool monsters::move(int smell){
    m_move = false;
    if (Asleep()){
        reduceMonsterSleepTime();
        return false;
    }
    
    int p_row = m_map->player()->row();
    int p_col = m_map->player()->col();
    int distance = abs(monsterCol() - p_col) + abs(monsterRow() - p_row);
    int r = monsterRow();
    int c = monsterCol();
    int row_diff = r - p_row;
    int col_diff = c - p_col;
    // next to the player -> attack the player
    if ((row_diff == 0 && (col_diff == -1 || col_diff == 1)) || (col_diff == 0 && (row_diff == -1 || row_diff == 1))){
        attack(m_map->player());
        return true;
    }
     
    // not next to the player -> move to the player
    if (distance <= smell) {
        // Check movement in each direction and move if valid
        if (abs(c - p_col) + abs(r - 1 - p_row) < distance) {
            if (m_map->newPosition(r, c, ARROW_UP) && !m_map->monstersAt(r, c) && m_map->getChar(r, c) != walls && !m_map->isPlayerAt(r, c)) {
                m_row = r;
                return true;
            }
        }
        if (abs(c - p_col) + abs(r + 1 - p_row) < distance) {
            if (m_map->newPosition(r, c, ARROW_DOWN) && !m_map->monstersAt(r, c) && m_map->getChar(r, c) != walls && !m_map->isPlayerAt(r, c)) {
                m_row = r;
                return true;
            }
        }
        if (abs(c + 1 - p_col) + abs(r - p_row) < distance) {
            if (m_map->newPosition(r, c, ARROW_RIGHT) && !m_map->monstersAt(r, c) && m_map->getChar(r, c) != walls && !m_map->isPlayerAt(r, c)) {
                m_col = c;
                return true;
            }
        }
        if (abs(c - 1 - p_col) + abs(r - p_row) < distance) {
            if (m_map->newPosition(r, c, ARROW_LEFT) && !m_map->monstersAt(r, c) && m_map->getChar(r, c) != walls && !m_map->isPlayerAt(r, c)) {
                m_col = c;
                return true;
            }
        }
        else {
            return  false;
        }
    }
    else {
        return false;
    }
    return false;
}

//Bogeymen-------------------------------------
void Bogeymen::deathDrop(){
    if (monsterMap()->pickObject(monsterRow(), monsterCol()) == nullptr && monsterMap()->getChar(monsterRow(), monsterCol()) != gold_idol) //If there isnt an object already there
    {
        if (trueWithProbability(0.1)){
            monsterMap()->dropObject(monsterRow(), monsterCol(), 'B');
        }
    }
}

//Snakewomen-----------------------------------
void SnakeWomen::deathDrop(){
    if (monsterMap()->pickObject(monsterRow(), monsterCol()) == nullptr && monsterMap()->getChar(monsterRow(), monsterCol()) != gold_idol) //If there isnt an object already there
    {
        if (trueWithProbability(0.3)){
            monsterMap()->dropObject(monsterRow(), monsterCol(), 'S');
        }
    }
}

//Dragons--------------------------------------
void Dragons::deathDrop(){
    if (monsterMap()->pickObject(monsterRow(), monsterCol()) == nullptr && monsterMap()->getChar(monsterRow(), monsterCol()) != gold_idol) //If there isnt an object already there
    {
        monsterMap()->dropObject(monsterRow(), monsterCol(), 'D');
    }
}

bool Dragons::move(){
    clearMoved();
    if (Asleep()){
        reduceMonsterSleepTime();
        return false;
    }
    
    int p_row = monsterMap()->player()->row();
    int p_col = monsterMap()->player()->col();
    if (trueWithProbability(0.1)){ regain_hit(); }
    
    int r = monsterRow();
    int c = monsterCol();
    int row_diff = r - p_row;
    int col_diff = c - p_col;
    
    // next to the player -> attack the player
    if ((row_diff == 0 && (col_diff == -1 || col_diff == 1)) || (col_diff == 0 && (row_diff == -1 || row_diff == 1))){
        attack(monsterMap()->player());
        return true;
    }
    return false;
}

//Goblins--------------------------------------
void Goblins::deathDrop(){
    if (monsterMap()->pickObject(monsterRow(), monsterCol()) == nullptr && monsterMap()->getChar(monsterRow(), monsterCol()) != gold_idol) //If there isnt an object already there
    {
        if (trueWithProbability(0.3)) {
            monsterMap()->dropObject(monsterRow(), monsterCol(), 'G');
        }
    }
}

bool Goblins::move(int distance){
    clearMoved();
    if (Asleep()){
        reduceMonsterSleepTime();
        return false;
    }
    
    // next to player -> attack
    int p_row = monsterMap()->player()->row();
    int p_col = monsterMap()->player()->col();
    int r = monsterRow();
    int c = monsterCol();
    int row_diff = r - p_row;
    int col_diff = c - p_col;
    if ((row_diff == 0 && (col_diff == -1 || col_diff == 1)) || (col_diff == 0 && (row_diff == -1 || row_diff == 1))){
        attack(monsterMap()->player());
        
        return true;
    }
    
    int path[18][70] = {0};
   
    int gRows = monsterRow()-1;
    int gCols = monsterCol()-1;
    
    queue<int> rows;
    queue<int> cols;
    rows.push(p_row);
    cols.push(p_col);
    path[p_row][p_col] = 1;
    smartMove(path, p_row, p_col, rows, cols);
    
//    for (int r = 0; r < 18; r++){
//        for (int c = 0; c < 70; c++){
//            cout<<path[r][c];
//        }
//        cout<<endl;
//    }
//    
//    path[gRows][gCols]
    if (path[gRows][gCols] <= distance){
        if (path[gRows-1][gCols] <= path[gRows][gCols] && monsterMap()->getChar(monsterRow()-1, monsterCol()) != walls && !monsterMap()->monstersAt(monsterRow()-1, monsterCol())){
            setRow(monsterRow()-1);
            return true;;
        }
        if (path[gRows+1][gCols] <= path[gRows][gCols] && monsterMap()->getChar(monsterRow()+1, monsterCol()) != walls && !monsterMap()->monstersAt(monsterRow()+1, monsterCol())){
            setRow(monsterRow()+1);
            return true;
        }
        if (path[gRows][gCols-1] <= path[gRows][gCols] && monsterMap()->getChar(monsterRow(), monsterCol()-1) != walls && !monsterMap()->monstersAt(monsterRow(), monsterCol()-1)){
            setCol(monsterCol()-1);
            return true;
        }
        if (path[gRows][gCols+1] <= path[gRows][gCols] && monsterMap()->getChar(monsterRow(), monsterCol()+1) != walls && !monsterMap()->monstersAt(monsterRow(), monsterCol()+1)){
            setCol(monsterCol()+1);
            return true;
        }
    }

    return false;
}


// prow = 2, pcol = 2  -> prow = 3, pcol = 2
// rows: 3, 2, 1, 2           cols: 2, 3, 2, 1

// 0100
// 1010
// 01x0
// 0000

// label the grid around the player with the number of steps it take to reach a goblin
void Goblins::smartMove(int path[18][70], int p_r, int p_c, queue<int> rows, queue<int> cols){
    if (monsterMap()->getChar(p_r+1, p_c) != walls && path[p_r][p_c-1] == 0 ){
        rows.push(p_r+1);
        cols.push(p_c);
        path[p_r][p_c-1] = path[p_r-1][p_c-1] + 1;
    }
    if (monsterMap()->getChar(p_r, p_c+1) != walls && path[p_r-1][p_c] == 0){
        rows.push(p_r);
        cols.push(p_c+1);
        path[p_r-1][p_c] = path[p_r-1][p_c-1] + 1;
    }
    if (monsterMap()->getChar(p_r-1, p_c) != walls && path[p_r-2][p_c-1] == 0){
        rows.push(p_r-1);
        cols.push(p_c);
        path[p_r-2][p_c-1] = path[p_r-1][p_c-1] + 1;
    }
    if (monsterMap()->getChar(p_r, p_c-1) != walls && path[p_r-1][p_c-2] == 0){
        rows.push(p_r);
        cols.push(p_c-1);
        path[p_r-1][p_c-2] = path[p_r-1][p_c-1] + 1;
    }
    if (!rows.empty() && !cols.empty()){
        p_r = rows.front();
        p_c = cols.front();
        rows.pop();
        cols.pop();
        smartMove(path, p_r, p_c, rows, cols);
    }
    
//    if (monsterMap()->getChar(p_r+1, p_c) != walls && path[p_r+1][p_c] == 0 ){
//        rows.push(p_r+1);
//        cols.push(p_c);
//        path[p_r+1][p_c] = path[p_r-1][p_c-1] + 1;
//    }
//    if (monsterMap()->getChar(p_r, p_c+1) != walls && path[p_r][p_c+1] == 0){
//        rows.push(p_r);
//        cols.push(p_c+1);
//        path[p_r][p_c+1] = path[p_r-1][p_c-1] + 1;
//    }
//    if (monsterMap()->getChar(p_r-1, p_c) != walls && path[p_r-1][p_c] == 0){
//        rows.push(p_r-1);
//        cols.push(p_c);
//        path[p_r-1][p_c] = path[p_r-1][p_c-1] + 1;
//    }
//    if (monsterMap()->getChar(p_r, p_c-1) != walls && path[p_r][p_c-1] == 0){
//        rows.push(p_r);
//        cols.push(p_c-1);
//        path[p_r][p_c-1] = path[p_r-1][p_c-1] + 1;
//    }
//    if (!rows.empty() && !cols.empty()){
//        p_r = rows.front();
//        p_c = cols.front();
//        rows.pop();
//        cols.pop();
//        smartMove(path, p_r, p_c, rows, cols);
//    }
//    rows.push(p_r);
//    cols.push(p_c);
//    path[p_r][p_c] = 1;
//    
//    while (!rows.empty() && !cols.empty()) {
//           int curr_r = rows.front();
//           int curr_c = cols.front();
//           rows.pop();
//           cols.pop();
//           
//           // Check and enqueue each possible move
//           if (monsterMap()->getChar(curr_r + 1, curr_c) != walls && path[curr_r + 1][curr_c] == 0) {
//               rows.push(curr_r + 1);
//               cols.push(curr_c);
//               path[curr_r + 1][curr_c] = path[curr_r][curr_c] + 1;
//           }
//           if (monsterMap()->getChar(curr_r, curr_c + 1) != walls && path[curr_r][curr_c + 1] == 0) {
//               rows.push(curr_r);
//               cols.push(curr_c + 1);
//               path[curr_r][curr_c + 1] = path[curr_r][curr_c] + 1;
//           }
//           if (monsterMap()->getChar(curr_r - 1, curr_c) != walls&& path[curr_r - 1][curr_c] == 0) {
//               rows.push(curr_r - 1);
//               cols.push(curr_c);
//               path[curr_r - 1][curr_c] = path[curr_r][curr_c] + 1;
//           }
//           if (monsterMap()->getChar(curr_r, curr_c - 1) != walls && path[curr_r][curr_c - 1] == 0) {
//               rows.push(curr_r);
//               cols.push(curr_c - 1);
//               path[curr_r][curr_c - 1] = path[curr_r][curr_c] + 1;
//           }
//       }
}



// Try 1
//int Goblins::findShortestPathToPlayer(int r, int c, int p_row, int p_col, int steps = 0) {
//    if (monsterMap()->getChar(r, c) == walls) {
//        return -1;  // Wall encountered, path not traversable
//    }
//
//    // Next to the player -> return current steps
//    int row_diff = r - p_row;
//    int col_diff = c - p_col;
//    if ((row_diff == 0 && (col_diff == -1 || col_diff == 1)) ||
//        (col_diff == 0 && (row_diff == -1 || row_diff == 1))) {
//        return steps;
//    }
////    monsterMap()->setChar(r, c, '.');
//    
//    // Check if the maximum step count is exceeded
//    if (steps >= 5) {
//        return -1;  // Player not reachable within 15 steps
//    }
//
//    // Recursively explore all possible directions
//    int down = findShortestPathToPlayer(r + 1, c, p_row, p_col, steps + 1);
//    int up = findShortestPathToPlayer(r - 1, c, p_row, p_col, steps + 1);
//    int left = findShortestPathToPlayer(r, c - 1, p_row, p_col, steps + 1);
//    int right = findShortestPathToPlayer(r, c + 1, p_row, p_col, steps + 1);
//
////    monsterMap()->setChar(r, c, space);
//    // Return the minimum non-negative value, or -1 if no path found
//    int minSteps = min({down, up, left, right});
//    minSteps = min(minSteps, -1);
//    if (minSteps == -1 || minSteps > 5){
//        return -1;
//    }
//    return minSteps;
//}
//
//bool Goblins::smartMove(int r, int c, int p_row, int p_col) {
//    int minSteps = findShortestPathToPlayer(r, c, p_row, p_col);
//
//    // Try moving up
//    if (monsterMap()->newPosition(r, c, ARROW_UP) &&
//        !monsterMap()->monstersAt(r - 1, c) &&
//        !monsterMap()->isPlayerAt(r - 1, c)) {
//        int newSteps = findShortestPathToPlayer(r - 1, c, p_row, p_col);
//        if (newSteps != -1 && newSteps < minSteps) {
//            setRow(r - 1);  // Move up
//            return true;
//        }
//    }
//
//    // Try moving down
//    if (monsterMap()->newPosition(r, c, ARROW_DOWN) &&
//        !monsterMap()->monstersAt(r + 1, c) &&
//        !monsterMap()->isPlayerAt(r + 1, c)) {
//        int newSteps = findShortestPathToPlayer(r + 1, c, p_row, p_col);
//        if (newSteps != -1 && newSteps < minSteps) {
//            setRow(r + 1);  // Move down
//            return true;
//        }
//    }
//
//    // Try moving left
//    if (monsterMap()->newPosition(r, c, ARROW_LEFT) &&
//        !monsterMap()->monstersAt(r, c - 1) &&
//        !monsterMap()->isPlayerAt(r, c - 1)) {
//        int newSteps = findShortestPathToPlayer(r, c - 1, p_row, p_col);
//        if (newSteps != -1 && newSteps < minSteps) {
//            setCol(c - 1);  // Move left
//            return true;
//        }
//    }
//
//    // Try moving right
//    if (monsterMap()->newPosition(r, c, ARROW_RIGHT) &&
//        !monsterMap()->monstersAt(r, c + 1) &&
//        !monsterMap()->isPlayerAt(r, c + 1)) {
//        int newSteps = findShortestPathToPlayer(r, c + 1, p_row, p_col);
//        if (newSteps != -1 && newSteps < minSteps) {
//            setCol(c + 1);  // Move right
//            return true;
//        }
//    }
//
//    return false;
//}

// Try 2
//int Goblins::countStepsHelper(map* g_map, int sr, int sc, int er, int ec, int steps){
//    if (g_map->getChar(sr, sc) == walls){ return -1; }
//    
//    // next to the player -> return result
//    int row_diff = sr - er;
//    int col_diff = sc - ec;
//    if ((row_diff == 0 && (col_diff == -1 || col_diff == 1)) || (col_diff == 0 && (row_diff == -1 || row_diff == 1))){ return steps; }
//    if (sr == er && sc == ec){ return steps; }
//
//    int down = countStepsHelper(g_map, sr+1, sc, er, ec, steps+1);
//    int up = countStepsHelper(g_map, sr-1, sc, er, ec, steps+1);
//    int left = countStepsHelper(g_map, sr, sc-1, er, ec, steps+1);
//    int right = countStepsHelper(g_map, sr, sc+1, er, ec, steps+1);
//    
//    g_map[sr][sc] = space;
//    
//    return max({down, up, left, right});
//}
//
//int Goblins::countSteps(char (*maze)[70], int sr, int sc, int er, int ec){
//    return countStepsHelper(maze, sr, sc, er, ec, 0);
//}
//int Goblins::countSteps(map* g_map, int sr, int sc, int er, int ec){
//    return countStepsHelper(g_map, sr, sc, er, ec, 0);
//}
//void Goblins::setCopy(){
//    // own current position
//    int cur_row = monsterRow();
//    int cur_col = monsterCol();
//    for (int r = 0; r < MAXROW; r++){
//        for (int c = 0; c < MAXCOL; c++){
//            if (r == cur_row && c == cur_col){
//                copy[r][c] = space;
//            }
//            else if (monsterMap()->getChar(r,c) != walls && (!monsterMap()->monstersAt(r, c))){
//                copy[r][c] = space;
//            }
//            else {
//                copy[r][c] = walls;
//            }
//        }
//    }
//}
//
//countSteps(copy, r-1, c, p_row, p_col) < 4 &&
//bool Goblins::smartMove(int sr, int sc, int er, int ec, int smellrange) {
//    // Base case: reached destination
//    if (smellrange == 0)
//        return false;
//
//    // Check each direction
//    else {
//        if ((!monsterMap()->monstersAt(sr - 1, sc)) && (!monsterMap()->isPlayerAt(sr - 1, sc)) && monsterMap()->newPosition(sr, sc, ARROW_UP)) {
//            if (smartMove(sr, sc, er, ec, smellrange - 1)) {
//                return true;
//            }
//        }
//        if ((!monsterMap()->monstersAt(sr + 1, sc)) && (!monsterMap()->isPlayerAt(sr + 1, sc)) && monsterMap()->newPosition(sr, sc, ARROW_DOWN)) {
//            if (smartMove(sr, sc, er, ec, smellrange - 1)) {
//                return true;
//            }
//        }
//        if ((!monsterMap()->monstersAt(sr, sc - 1)) && (!monsterMap()->isPlayerAt(sr, sc - 1)) && monsterMap()->newPosition(sr, sc, ARROW_LEFT)) {
//            if (smartMove(sr, sc, er, ec, smellrange - 1)) {
//                return true;
//            }
//        }
//        if ((!monsterMap()->monstersAt(sr, sc + 1)) && (!monsterMap()->isPlayerAt(sr, sc + 1)) && monsterMap()->newPosition(sr, sc, ARROW_RIGHT)) {
//            if (smartMove(sr, sc, er, ec, smellrange - 1)) {
//                return true;
//            }
//        }
//        return  false;// Default case: no valid move found
//    }
//}

