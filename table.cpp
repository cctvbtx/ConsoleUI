#include <iostream>

#include "table.h"

//TODO: Make an extended class with cOlOrS

Table::Table(string title) {
    _title = title;
}

void Table::addColumn(string _text) {
    columns.push_back(_text);
}

void Table::removeColumn(int _index) {
    columns.erase(begin(columns) + _index);
}

void Table::addRow(vector<string> row) {    //TODO: Add labels for rows
    if(row.size() != columns.size()){
        throw invalid_argument("Passed arguments do not match table size");
        return;
    }
    values.push_back(row);

}

void Table::setTitle(string title) {
    _title = title;
}

int Table::calculateWidth() {
    int len = 0;

    for(int i = 0; i < columns.size(); i++) {
        len += getColumnWidth(i);
    }

    len += columns.size() + 1;

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

    cout << endl;

    return;
}

void Table::drawRow(vector<string> vec) {
    cout << '|';

        for(int i = 0; i < vec.size(); i++) {

            int padding = (getColumnWidth(i) - vec[i].size()) / 2;

            bool isOdd = (getColumnWidth(i) - vec[i].size()) % 2;

            for(int ii = 0; ii < padding; ii++)
                cout << ' ';

            cout << vec[i];

            for(int ii = 0; ii < (isOdd ? padding + 1 : padding); ii++)
                cout << ' ';

            cout << '|';

        }
        cout << endl;
}

void Table::drawTitle() {

    if(_title.length() > calculateWidth()) {
        throw length_error("Title cannot be longer than the size of the table"); //TODO: Maybe trim the title if it's too long instead?
    }

    int padding = (calculateWidth() - _title.length()) / 2;
    bool isOdd = (calculateWidth() - _title.length()) % 2;

    for(int ii = 0; ii < padding; ii++)
        cout << ' ';

    cout << _title;

    for(int ii = 0; ii < (isOdd ? padding + 1 : padding); ii++)
        cout << ' ';

    cout << endl;
}

void Table::draw() {
    drawTitle();
    drawHorLine();
    drawRow(columns);
    drawHorLine();

    for(vector<string> col : values)
        drawRow(col);

    drawHorLine();
}
