#include "windows.h"
#include "winbase.h"
#include "WinDef.h"
#include "WinCon.h"
#include "conui.h"

#include <iostream>
#include <utility>

using namespace std;

const WORD def_bg = 0;
const WORD def_fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

WORD current_bg, current_fg;

/*enum bgColors: WORD {
        Red = BACKGROUND_RED,
        Blue = BACKGROUND_BLUE,
        Green = BACKGROUND_GREEN,
        White = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN
};*/


conUI::conUI(HANDLE h_out, HANDLE h_in) {

        output_handle = h_out;
        input_handle = h_in;
        SetConsoleTextAttribute(output_handle, def_bg | def_fg);
        current_bg = def_bg;
        current_fg = def_fg;

}

conUI::conUI() {

        output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        input_handle = GetStdHandle(STD_INPUT_HANDLE);

        if(output_handle == INVALID_HANDLE_VALUE) {
            std::cout << "Exception: Unable to get output handle" << std::endl;
            exit(-1);
        }

        if(input_handle == INVALID_HANDLE_VALUE) {
            std::cout << "Exception: Unable to get input handle" << std::endl;
            exit(-1);
        }

        SetConsoleTextAttribute(output_handle, def_bg | def_fg);

        current_bg = def_bg;
        current_fg = def_fg;
}

void conUI::printcol(const char* _string, WORD foreground, WORD background) {

        WORD prev_bg = current_bg;
        WORD prev_fg = current_fg;

        SetConsoleTextAttribute(output_handle, foreground | background);
        std::cout << _string;
        SetConsoleTextAttribute(output_handle, prev_fg | prev_bg);

}

void conUI::setbc(WORD _background) {

    SetConsoleTextAttribute(output_handle, current_fg | _background);
    current_bg = _background;
}

void conUI::setfc(WORD _foreground) {

    SetConsoleTextAttribute(output_handle, _foreground | current_bg);
    current_fg = _foreground;

}

void conUI::resetColors() {

    if(output_handle != NULL) {

        SetConsoleTextAttribute(output_handle, def_bg | def_fg);
        current_bg = def_bg;
        current_fg = def_fg;

    }

}

void conUI::debug() {

    cout << "Current set bg is: " << current_bg << endl;
    cout << "Current set fg is: " << current_fg << endl;
    cout << "Console Text Attributes will be set to: 0x" << (current_bg | current_fg) << endl;
    cout << "Output handle is: " << (output_handle == INVALID_HANDLE_VALUE ? "invalid" : "valid") << endl;
}

KEY_EVENT_RECORD conUI::waitForKbEvent() {

    INPUT_RECORD inBuffer[64];
    DWORD eventNumber = 0;

    while(true) {

    while(!ReadConsoleInput(input_handle, inBuffer, 64, &eventNumber)) {    //Wait for an input event
    }

    for(int i = 0; i < eventNumber; i++) {

        if(inBuffer[i].EventType != KEY_EVENT) {
            continue;
        } else {
            if(inBuffer[i].Event.KeyEvent.bKeyDown) { //Check if the key is being pressed down so the event doesn't return twice for each key
                return inBuffer[i].Event.KeyEvent;
            }
        }


    }

    }
}

void conUI::clearScreen() {
    COORD tl = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(output_handle, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(output_handle, ' ', cells, tl, &written);
    FillConsoleOutputAttribute(output_handle, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(output_handle, tl);
}
