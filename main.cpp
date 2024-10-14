//
//  main.cpp
//  project 3
//
//  Created by Lyanna Chen on 5/10/24.
//

#include "Game.h"
#include "level.h"
#include "player.h"
#include "monsters.h"
#include "object.h"
#include <iostream>
#include <algorithm>

int main()
{
      // Your program must use whatever value is passed to the Game constructor
      // as the distance a goblin can smell.  The spec stated this is 15.  Some
      // people might implement the recursive goblin algorithm for determining
      // the shortest path to the player in a way that takes an uncomfortably
      // long time for that distance.  If you're one of them, then until you
      // improve your algorithm, pass a value smaller than 15 to the game
      // constructor so your game is comfortable to play.

    Game g(15);
    g.play();
    
//    map* test = new map(0);
//    test->createLevel(0);
//    test.display();
//    test.createLevel(4);
//    test.display();
//    cout<<test.getObject()[0]->display_name()<<endl;
//    cout<<test.getObject()[1]->display_name()<<endl;
//    test.createLevel(3);
//    test.display();
//    int p_rol = randInt(2, ROW);
//    int p_col = randInt(2, COL);q
//    test.place_player(p_rol, p_col);
//    cout<<boolalpha<<test.place_player(3, 3)<<endl;
//    test.display();
//    Player p1(test, 10, 10);
//    p1.check_invent();
//    p1.getInventory().push_back(new LongSword());
//    p1.check_invent();
//    cout<<p1.getInventory().size()<<endl;
//    map* test = new map(0);
//    test->createLevel(0);
//    test->display();
//    test->createLevel(4);
//    monsters* monster[2];
//    monster[1] = new Dragons(test, 10, 25);
//    test->display();
//    test->removeMonsters(monster[1]);
//    test->display();
    return 0;
}
