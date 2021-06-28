#include "conui.h"
#include "menu.h"
#include "windows.h"
#include "wincon.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

typedef unsigned short WORD;

const WORD def_bg = 0;
const WORD def_fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;


MenuItem::MenuItem(string text, WORD _background, WORD _foreground, WORD _checkcol, bool align_right) {

    foreground = _foreground;
    background = _background;
    check_color = _checkcol;

    this -> align_right = align_right;
    this -> text = text;

}

int ConMenu::show_menu() {

    update();

    while(1) {

        KEY_EVENT_RECORD input_event = ui.waitForKbEvent();

        switch(input_event.wVirtualKeyCode) {

        case VK_UP:

            if(selected_index != 0)
                selected_index--;

            break;

        case VK_DOWN:

            if(selected_index != _items.size() - 1)
                selected_index++;

            break;

        case VK_ESCAPE:
            return -1;

        case VK_RETURN:
            return selected_index;


        }
        update();
    }
}

void ConMenu::update() {

    system("cls");  //Not elegant but way faster than using WinApi

    for(int i = 0; i < _items.size(); i++) {

        MenuItem current_item = _items[i];

        if(current_item.background != ui.current_bg)
            ui.setbc(current_item.background);

        if(current_item.foreground != ui.current_fg)
            ui.setfc(current_item.foreground);

        cout << current_item.text << ' ';

        cout << "[ ";

        if(selected_index == i)
            ui.printcol(" X ", current_item.check_color, 0x00);

        cout << " ]" << '\n';

        //ui.resetColors();

    }
}

void ConMenu::add_item(MenuItem *itm) {
    _items.push_back(*itm);
}

void ConMenu::remove_item(int _index) {
    if((size_t)_index < _items.size() - 1)
        _items.erase(_items.begin() + _index);
}

ConMenu::ConMenu(const conUI &output) { ui = output; };

string ConMenu::get_text(int index) {
    return _items[index].text;
}
