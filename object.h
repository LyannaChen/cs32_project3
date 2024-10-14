//
//  object.h
//  project 3
//
//  Created by Lyanna Chen on 5/16/24.
//

#ifndef object_h
#define object_h

#include <iostream>
#include <string>
using namespace std;

class Player;
class monsters;

class object{
public:
    // initial weapon
    object(string name, char symbol, string action, int dexterity, int damage) :
    m_name(name), m_symbol(symbol), m_action(action), m_dexterity(dexterity), m_damage(damage){};
    // pick up weapon
    object(string name, char symbol, string action, int dexterity, int damage, int r, int c) :
    m_name(name), m_symbol(symbol), m_action(action), m_dexterity(dexterity), m_damage(damage), m_row(r), m_col(c){};
    // pick up scroll
    object(string name, char symbol, string action, char type, int r, int c) :
    m_name(name), m_symbol(symbol), m_action(action), m_type(type), m_row(r), m_col(c){};
    // gold idol
    object(string name, char symbol, int r, int c) :
    m_name(name), m_symbol(symbol), m_row(r), m_col(c){};
    
    virtual ~object() = 0;
    
    string display_name() { return m_name; }
    string display_action() { return m_action; }
    int objRow() { return m_row; }
    int objCol() { return m_col; }
    char objSymbol() { return m_symbol; }
    char scrollType() { return m_type; }
//    bool isScroll(){ return (m_symbol == '?'); }
    int weaponDexterity() { return m_dexterity; }
    int weaponDamage() { return m_damage; }
    
private:
    string m_name = "", m_action = "";
    char m_symbol, m_type;
    int m_dexterity, m_damage;
    int m_row, m_col;
};



class Mace : public object {
public:
    Mace(int r, int c) : object("mace", ')', "swings", 0, 2, r, c) {};
    ~Mace() {};
};

class ShortSword : public object{
public:
    ShortSword() : object("short sword", ')', "slashes", 0, 2) {};
    ShortSword(int r, int c) : object("short sword", ')', "slashes", 0, 2, r, c) {};
    ~ShortSword() {};
};

class LongSword : public object{
public:
    LongSword() : object("long sword", ')', "swings", 2, 4) {};
    LongSword(int r, int c) : object("long sword", ')', "swings", 2, 4, r, c) {};
    ~LongSword() {};
};

class MagicFang : public object{
public:
    MagicFang() : object("magic fangs of sleep", ')', "strikes", 3, 2) {};
    MagicFang(int r, int c) : object("magic fangs of sleep", ')', "strikes", 3, 2, r, c) {};
    ~MagicFang() {};
};

class MagicAxe : public object{
public: 
    MagicAxe() : object("magic axe", ')', "chops", 5, 5) {};
    MagicAxe(int r, int c) : object("magic axe", ')', "chops", 5, 5, r, c) {};
    ~MagicAxe() {};
};

class Teleport : public object{
public:
    Teleport(int r, int c) : object("scroll of teleportation", '?', "You feel your body wrenched in space and time.", 'T', r, c) {};
    ~Teleport() {};
};

class Strength : public object{
public:
    Strength(int r, int c) : object("scroll of strength", '?', "Your muscles bulge.", 'S', r, c) {};
    ~Strength() {};
};

class Dexterity : public object{
public:
    Dexterity(int r, int c) : object("scroll of enhance dexterity", '?', "You feel like less of a klutz.", 'D', r, c) {};
    ~Dexterity() {};
};

class Health : public object{
public:
    Health(int r, int c) : object("scroll of enhance health", '?', "You feel your heart beating stronger.", 'H', r, c) {};
    ~Health() {};
};

class Armor : public object{
public:
    Armor(int r, int c) : object("scroll of enhance armor", '?', "Your armor glows blue.", 'A', r, c) {};
    ~Armor() {};
};

class Idol : public object{
public:
    Idol(int r, int c) : object("the gold idol", '&', r, c) {};
    ~Idol() {};
};

#endif /* object_h */
