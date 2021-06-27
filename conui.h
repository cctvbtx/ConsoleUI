#ifndef CONUI_H
#define CONUI_H

#include <windows.h>
#include <wincon.h>
#include <utility>

const WORD BG_BLACK = 0;

//enum bgColors: WORD;

class conUI {

private:
void *output_handle, *input_handle;

public:
WORD current_bg, current_fg;
void printcol(const char* _string, WORD foreground = 0x7, WORD background = 0x0); //0x7 - White foreground, 0x0 - Black background
void setbc(WORD _background);
void setfc(WORD _foreground);
void resetColors();
void debug();
void clearScreen();
KEY_EVENT_RECORD waitForKbEvent();
conUI(HANDLE h_out, HANDLE h_in);
conUI();

};
#endif // CONUI_H
