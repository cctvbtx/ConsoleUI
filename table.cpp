#include <iostream>

#include "windows.h"
#include "wincon.h"

#include "table.h"


//TODO: Make an extended class with cOlOrS

Table::Table(string title) {
    _title = title;
}

string Table::repeatStr(string text, int n) {
    string txt;
    for(int i = 0; i < n; i++) {
        txt.append(text);
    }
    return txt;
}

void Table::addColumn(string _text) {
    columns.push_back(_text);
}

void Table::removeColumn(int _index) {
    columns.erase(begin(columns) + _index);
}

void Table::addRow(const vector<string> &row, string label) {
    if(row.size() != columns.size()){
        throw invalid_argument("Passed arguments do not match table size");
    }
    values.push_back(row);
    rows.push_back(label);

    if(label != "") {
        showRowLabels = true;
    }

}

int Table::getLabelsWidth() {
    int _max = 0;
    for(string s : rows) {
        if(s.length() > _max)
            _max = s.length();
    }

    return _max > 0 ? _max + 2 : -1;
}

void Table::setTitle(string title) {
    _title = title;
}

void Table::setValue(int row, int col, string _text) {
    if(row < values.size() && col < values[0].size()) {
        values[row][col] = _text;
    }
}

int Table::calculateWidth() {
    int len = 0;

    for(int i = 0; i < columns.size(); i++) {
        len += getColumnWidth(i);
    }

    len += columns.size() + 1;

    len += getLabelsWidth() + 1;

    return len;
}

int Table::getColumnWidth(int index) {
    int _max = columns[index].size();

    for(vector<string> row : values) {
        if((int)row[index].size() > _max)
            _max = row[index].size();
    }

    return _max + 4; //Four characters for two padding spaces on each side
}

void Table::drawHorLine() {
    int length = calculateWidth();

    for(int i = 0; i < length; i++) {
        cout << '-';
    }

    cout << '\n';

}

void Table::drawRow(const vector<string> &vec, int idx) {

    if(showRowLabels) {

        if(idx == -1) {
            cout << repeatStr(" ", getLabelsWidth());
        } else {
            cout << rows[idx] << repeatStr(" ", getLabelsWidth() - rows[idx].length());
        }
    }

    cout << (showRowLabels ? "||" : "|");

        for(int i = 0; i < vec.size(); i++) {

            int padding = (getColumnWidth(i) - vec[i].size()) / 2;

            bool isOdd = (getColumnWidth(i) - vec[i].size()) % 2;

            cout << repeatStr(" ", padding) << vec[i] << repeatStr(" ", isOdd ? padding + 1 : padding);

            cout << '|';

        }
        cout << '\n';
}

void Table::drawTitle() {

    if(_title.length() > calculateWidth()) {
        _title = _title.substr(0, calculateWidth());
    }

    int padding = (calculateWidth() - _title.length()) / 2;
    bool isOdd = (calculateWidth() - _title.length()) % 2;

    for(int ii = 0; ii < padding; ii++)
        cout << ' ';

    cout << _title;

    for(int ii = 0; ii < (isOdd ? padding + 1 : padding); ii++)
        cout << ' ';

    cout << '\n';
}

void Table::draw() {
    drawTitle();
    drawHorLine();
    drawRow(columns, -1);
    drawHorLine();

    for(int i = 0; i < values.size(); i++){
        drawRow(values[i], i);
    }

    drawHorLine();
}

//SelectionTable

SelectionTable::SelectionTable(const conUI &ui, string title) {
    _ui = ui;
    _title = title;
}

void SelectionTable::drawRow(const vector<string> &vec, int idx) {

    if(idx == selected_idx) {
        _ui.invertColors();
    }

    if(showRowLabels) {

        if(idx == -1) {
            cout << repeatStr(" ", getLabelsWidth());
        } else {
            cout << rows[idx] << repeatStr(" ", getLabelsWidth() - rows[idx].length());
        }
    }

    cout << (showRowLabels ? "||" : "|");

    for(int i = 0; i < vec.size(); i++) {

        int padding = (getColumnWidth(i) - vec[i].size()) / 2;

        bool isOdd = (getColumnWidth(i) - vec[i].size()) % 2;

        cout << repeatStr(" ", padding) << vec[i] << repeatStr(" ", isOdd ? padding + 1 : padding);

        cout << '|';

    }

    cout << '\n';

    if(idx == selected_idx) {
        _ui.invertColors();
    }

}

int SelectionTable::show() {

    draw();

    while(1) {
        KEY_EVENT_RECORD input_event = _ui.waitForKbEvent();

        switch(input_event.wVirtualKeyCode) {
            case VK_UP:

                if(selected_idx != 0)
                    selected_idx--;

                break;

            case VK_DOWN:

                if(selected_idx != values.size() - 1)
                    selected_idx++;

                break;

            case VK_ESCAPE:
                return -1;

            case VK_RETURN:
                return selected_idx;
        }
        system("cls");
        draw();
    }
}