//
//  Game.hpp
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <iostream>
#include <string>
#include "object.h"
#include "level.h"
using namespace std;
// You may add data members and other member functions to this class.

class map;

class Game
{
public:
    Game(int goblinSmellDistance);
    void display();
    void play();
    void monsterMove();
    ~Game() { delete m_map; }
private:
    int currentlevel;
    int SmellDistance;
    map* m_map;
    
};

#endif // GAME_INCLUDED
