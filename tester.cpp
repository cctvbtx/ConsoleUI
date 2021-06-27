#include <iostream>
#include <vector>
#include "conui.h"
#include "menu.h"
#include "windows.h"
#include "WinDef.h"
#include "WinCon.h"
#include "table.h"

using namespace std;

int main() {

    /*conUI c{};

    ConMenu m = *new ConMenu(c);

    m.add_item(new MenuItem("uwu", 0, FOREGROUND_BLUE | FOREGROUND_INTENSITY, 7));
    m.add_item(new MenuItem("OwO", 0, FOREGROUND_BLUE | FOREGROUND_INTENSITY, 7));
    m.add_item(new MenuItem("TwT", 0, FOREGROUND_GREEN | FOREGROUND_INTENSITY, 7));
    m.add_item(new MenuItem("OnO", 0, FOREGROUND_RED | FOREGROUND_INTENSITY, 7));

    cout << "User selected " + m.get_text(m.show_menu()) << endl;*/

    Table t = *new Table("Tabulka rozlozeni smetan na balkonech VOLE PICO PICO ZIJU VE TVYCH ZDECH AMOGUSUS");

    t.addColumn("penis");
    t.addColumn("uwu");
    t.addColumn("ahoj brasko");
    t.addColumn("smetana na balkone");

    t.addRow(*new vector<string> {"1", "2", "33333333333333333333333333333333", "4"});
    t.addRow(*new vector<string> {"dbeil", "voel voel voel voel voel", "sere fok", "wtf about literatura delam podel kradu kone"});

    /*t.drawHorLine();
    t.drawColumnNames();*/

    t.draw();
}
