#include <iostream>

#include "windows.h"
#include "wincon.h"

#include "table.h"


//TODO: Make an extended class with cOlOrS

//TODO: Newlines break the table

Table::Table(const string &title) {
    _title = title;
}

string Table::repeatStr(const string &text, int n) {
    string txt;
    for(int i = 0; i < n; i++) {
        txt.append(text);
    }
    return txt;
}

void Table::addColumn(const string &_text) {
    columns.push_back(_text);
}

void Table::insertColumn(const string &_text, int _index) {

    if(columns.begin() + _index > columns.end()) {
        throw range_error("Index (" + to_string(_index) + ") out of bounds for table (" + _title + ").");
    }

    columns.insert(columns.begin() + _index, _text);
    for(auto &row : values) {
        row.insert(row.begin() + _index, "");
    }
}

void Table::removeColumn(int _index) {
    if(_index > columns.size() - 1) {
        throw range_error("Index (" + to_string(_index) + ") out of bounds for table (" + _title + ").");
    }

    columns.erase(columns.begin() + _index);
    for(auto &v : values) {
        v.erase(v.begin() + _index);
    }
}

void Table::removeRow(int _index) {
    if(_index > values.size() - 1) {
        throw range_error("Index (" + to_string(_index) + ") out of bounds for table (" + _title + ").");
    }
    rows.erase(rows.begin() + _index);
    values.erase(values.begin() + _index);
}

void Table::insertRow(const vector<string> &row, int _index, const string &label) {
    if(row.size() != columns.size()){
        throw invalid_argument("Passed arguments do not match table size");
    }

    if(values.begin() + _index > values.end()) {
        throw range_error("Index (" + to_string(_index) + ") out of bounds for table (" + _title + ").");
    }

    values.insert(values.begin() + _index, row);
    rows.insert(rows.begin() + _index, label);

    if(label != "") {
        showRowLabels = true;
    }
}

void Table::addRow(const vector<string> &row, const string &label) {
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
    for(const string &s : rows) {
        if(s.length() > _max)
            _max = s.length();
    }

    return _max > 0 ? _max + 2 : -1;
}

void Table::setTitle(const string &title) {
    _title = title;
}

void Table::setValue(int row, int col, const string &_text) {
    if(row < values.size() && col < values[0].size()) {
        values[row][col] = _text;
    } else {
        throw range_error("Index out of bounds for table (" + _title + ").");
    }
}

unsigned int Table::calculateWidth() {
    int len = 0;

    for(int i = 0; i < columns.size(); i++) {
        len += getColumnWidth(i);
    }

    len += columns.size() + 1;

    len += getLabelsWidth() + 1;

    return len;
}

unsigned int Table::getColumnWidth(int index) {
    unsigned int _max = columns[index].size();

    for(auto &row : values) {
        if(row[index].size() > _max)
            _max = row[index].size();
    }

    return _max + 4; //Four characters for two padding spaces on each side
}

void Table::drawHorLine() {
    unsigned int length = calculateWidth();

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

SelectionTable::SelectionTable(const conUI &ui, const string &title) {
    _ui = ui;
    _title = title;
}

void SelectionTable::draw() {
    drawTitle();
    drawHorLine();
    drawRow(columns, -1);
    drawHorLine();

    for(int i = 0; i < values.size(); i++){
        if(i == selected_idx) {
            _ui.invertColors();
            drawRow(values[i], i);
            _ui.invertColors();
        } else {
            drawRow(values[i], i);
        }
    }

    drawHorLine();
}

int SelectionTable::show() {

    draw();

    while(true) {
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