#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include "conui.h"

typedef unsigned short WORD;

struct MenuItem {
    std::string text;
    WORD foreground, background, check_color;
    bool align_right;
    MenuItem(std::string text, WORD _background = 0x0, WORD _foreground = 0x7, WORD _checkcol = 0x7, bool align_right = false);
};

class ConMenu{
private:
    conUI ui;
    int selected_index = 0;
    std::vector<MenuItem> _items;
    void update();


public:
    void add_item(MenuItem *itm);
    void remove_item(int _index);
    int show_menu();
    std::string get_text(int index);
    ConMenu(const conUI &output);
};

#endif // MENU_H
