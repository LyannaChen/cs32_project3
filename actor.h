//
//  actor.hpp
//  project 3
//
//  Created by Lyanna Chen on 5/16/24.
//

#ifndef actor_h
#define actor_h

#include <iostream>
#include "utilities.h"
#include <string>
using namespace std;

class actor{
public:
    actor(){};
    virtual void move();
    virtual void attack();
    virtual ~actor();
private:
    
};


#endif /* actor_h */
