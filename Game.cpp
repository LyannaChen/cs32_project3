//
//  Game.cpp
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#include "Game.h"
#include "level.h"
#include "utilities.h"
#include "monsters.h"
#include <iostream>

using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance) {
    // initialize game
    currentlevel = 0;
    m_map = new map(0);
    SmellDistance = goblinSmellDistance;
}
    

void Game::play() {
    clearScreen();
    m_map->createLevel(0);
    m_map->display();
    
    cout<<endl;
    
    Player* p = m_map->player();
    if (p == nullptr)
        return;
    

    while (!p->playerDead() && !p->playerHasWin()){
        char move = getCharacter();
        char choice;
        switch (move){
            case 'i':  // inventory related-actions - DONE
                p->check_invent();
                choice = getCharacter();
                m_map->display();
                break;
            case 'w':
                p->check_invent();
                choice = getCharacter();
                p->wield(choice);
                break;
            case 'r':
                p->check_invent();
                choice = getCharacter();
                p->read(choice);
                break;
            case ARROW_UP:   // move (DONE) + attack (DONE)
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
                p->move(move);
                p->regain_hit();
                monsterMove();
                m_map->display();
                break;
            case 'g': // pick up item - DONE pickup gold idol + winning (DONE)
                if (p->pick(p->row(), p->col())){
                    if (p->playerHasWin()){
                        m_map->display();
                        cout<<"You pick up the golden idol."<<endl;
                        cout<<"Congratulations, you won!"<<endl;
                        break;
                    }
                    
                    string picked = "You pick up a " + m_map->pickObject(p->row(), p->col())->display_name();
                    if (m_map->pickObject(p->row(), p->col())->objSymbol() == '?'){
                        string picked = "You picked up a scroll called " + m_map->pickObject(p->row(), p->col())->display_name();
                    }
                    m_map->removeObject(p->row(), p->col());
                    m_map->display();
                    cout<<picked<<endl;
                    break;
                }
                else
                    break;
            case '>': // decend Stairs - DONE
                if (m_map->getChar(p->row(), p->col()) == stairs){
                    m_map->increaseLevel();
                    m_map->createLevel(m_map->level());
                    m_map->display();
                }
                break;
            case 'c': // cheat - DONE
                p->cheat();
                monsterMove();
                m_map->display();
                break;
            case 'q': // quit game - DONE
                return;
        }
    }
    
    if (p->die()){
        cout<<"Press q to exit the game."<<endl;
    }
    if (p->playerHasWin()){
        cout<<"Press q to exit the game."<<endl;
    }
    while (getCharacter() != 'q')
        ;
}

void Game::monsterMove() {  // move monster DEBUGGING
    for (monsters* monster_moving : m_map->getMonsters()){
        if (monster_moving->monsterSymbol() == 'B'){
            monster_moving->move(5);
        }
        else if (monster_moving->monsterSymbol() == 'S'){
            monster_moving->move(3);
        }
        else if (monster_moving->monsterSymbol() == 'D'){
            dynamic_cast<Dragons*>(monster_moving)->move();
        }
        else if (monster_moving->monsterSymbol() == 'G'){
            dynamic_cast<Goblins*>(monster_moving)->move(SmellDistance);
//            monster_moving->move(SmellDistance);
        }
    }
}
// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons,
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
