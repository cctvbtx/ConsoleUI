#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"    //size_t to int conversions, please don't add more than INT_MAX rows or columns, thank you. TODO: fix this
#include <iostream>
#include <sstream>

#include "windows.h"
#include "wincon.h"

#include "table.h"

using namespace std;

//Helper functions

string repeatStr(const string &text, int n) {
    string txt;
    for(int i = 0; i < n; i++) {
        txt.append(text);
    }
    return txt;
}

vector<string> split(const string &s, const char f) {

    vector<string> st;
    istringstream is(s);
    string tok;

    while(getline(is, tok, f)) {
        st.push_back(tok);
    }

    return st;
}

//Table class

Table::Table(const string &title) {
    _title = title;
}

void Table::addColumn(const string &text) {
    columns.push_back(text);
}

void Table::insertColumn(const string &text, int index) {

    if(columns.begin() + index > columns.end()) {
        throw range_error("Index (" + to_string(index) + ") out of bounds for table (" + _title + ").");
    }

    columns.insert(columns.begin() + index, text);
    for(auto &row : values) {
        row.insert(row.begin() + index, "");
    }
}

void Table::removeColumn(int index) {
    if(index > columns.size() - 1) {
        throw range_error("Index (" + to_string(index) + ") out of bounds for table (" + _title + ").");
    }

    columns.erase(columns.begin() + index);
    for(auto &v : values) {
        v.erase(v.begin() + index);
    }
}

void Table::removeRow(int index) {
    if(index > values.size() - 1) {
        throw range_error("Index (" + to_string(index) + ") out of bounds for table (" + _title + ").");
    }
    rows.erase(rows.begin() + index);
    values.erase(values.begin() + index);
}

void Table::insertRow(const vector<string> &row, int index, const string &label) { //TODO: Newlines still break the table when using this

    if(row.size() != columns.size())
        throw invalid_argument("Passed arguments do not match table size");

    if(values.begin() + index > values.end())
        throw range_error("Index (" + to_string(index) + ") out of bounds for table (" + _title + ").");

    values.insert(values.begin() + index, row);
    rows.insert(rows.begin() + index, label);

    if(!label.empty())
        showRowLabels = true;
}

void Table::addRow(vector<string> row, const string &label) {

    bool has_newlines = false;

    rows.push_back(label);

    if(!label.empty())
        showRowLabels = true;

    if(row.size() > columns.size()){

        throw invalid_argument("Passed arguments do not match table size");

    } else if(row.size() < columns.size()) {

        for(int i = row.size(); i < columns.size(); i++)
            row.emplace_back("");
    }

   for(const string &s : row) {
        if(s.find('\n') != string::npos) {
            has_newlines = true;
            break;
        }
    }


    if(has_newlines) {

        vector<vector<string>> newrows;
        int maxlines = 1;
        for(const string &s : row) {

            vector<string> lines = split(s, '\n');

            if(lines.size() > maxlines)
                maxlines = lines.size();

            newrows.push_back(lines);
        }

        for(int i = 0; i < maxlines; i++) {
            vector<string> a;
            for(const vector<string> &v : newrows) {

                if(v.size() - 1 < i) {
                    a.emplace_back("");
                } else {
                    a.push_back(v[i]);
                }
            }
            values.push_back(a);
            if(i != maxlines - 1) {
                rows.emplace_back("");
            }
        }
        return;
    }

    values.push_back(row);

}

int Table::getLabelsWidth() {
    int _max = 0;

    for(const string &s : rows)
        if(s.length() > _max)
            _max = s.length();

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

void Table::drawLine() {

    cout << repeatStr("-", calculateWidth()) << '\n';

}

void Table::drawRow(const vector<string> &vec, int index) {

    if(showRowLabels) {

        if(index == -1) {
            cout << repeatStr(" ", getLabelsWidth());
        } else {
            cout << rows[index] << repeatStr(" ", getLabelsWidth() - rows[index].length());
        }
    }

    cout << (showRowLabels ? "||" : "|");

        for(int i = 0; i < vec.size(); i++) {

            int padding = (getColumnWidth(i) - vec[i].size()) / 2;

            bool isOdd = (getColumnWidth(i) - vec[i].size()) % 2;

            cout << repeatStr(" ", padding) << vec[i] << repeatStr(" ", isOdd ? padding + 1 : padding) << '|';

        }
        cout << '\n';
}

void Table::drawTitle() {

    if(_title.length() > calculateWidth())
        _title = _title.substr(0, calculateWidth());

    int padding = (calculateWidth() - _title.length()) / 2;
    bool isOdd = (calculateWidth() - _title.length()) % 2;

    cout << repeatStr(" ", padding) << _title << repeatStr(" ", isOdd ? padding + 1 : padding) << '\n';

}

void Table::draw() {
    drawTitle();
    drawLine();
    drawRow(columns, -1);
    drawLine();

    for(int i = 0; i < values.size(); i++)
        drawRow(values[i], i);

    drawLine();
}

//SelectionTable class

//TODO: SelectionTable still doesn't handle newlines well

SelectionTable::SelectionTable(const conUI &ui, const string &title) {
    _ui = ui;
    _title = title;
}

void SelectionTable::drawRow(const vector<string> &vec, int index) {

    if(showRowLabels) {

        if(index == -1) {
            cout << repeatStr(" ", getLabelsWidth());
        } else {
            cout << rows[index] << repeatStr(" ", getLabelsWidth() - rows[index].length());
        }
    }

    cout << (showRowLabels ? "||" : "|");

    for(int i = 0; i < vec.size(); i++) {

        int padding = (getColumnWidth(i) - vec[i].size()) / 2;

        bool isOdd = (getColumnWidth(i) - vec[i].size()) % 2;

        cout << repeatStr(" ", padding) << vec[i] << repeatStr(" ", isOdd ? padding + 1 : padding);

        if(i != vec.size() - 1)
            cout << '|';

    }

    if(index == start_at && start_at != 0) {
        cout << '-';
    } else if(index == (start_at + max_length - 1) && index != rows.size() - 1) {
        cout << '+';
    } else {
        cout << '|';
    }

    cout << '\n';
}

void SelectionTable::draw() {
    drawTitle();
    drawLine();
    drawRow(columns, -1);
    drawLine();

    for(int i = start_at; i < (max_length == -1 ? values.size() : start_at + max_length); i++){
        if(i == selected_idx) {
            _ui.invertColors();
            drawRow(values[i], i);
            _ui.invertColors();
        } else {
            drawRow(values[i], i);
        }
    }

    drawLine();
}

int SelectionTable::show() {

    draw();

    while(true) {
        bool index_changed = false;
        KEY_EVENT_RECORD input_event = _ui.waitForKbEvent();

        switch(input_event.wVirtualKeyCode) {
            case VK_UP:

                if(selected_idx != 0) {
                    index_changed = true;
                    selected_idx--;

                    if(selected_idx < start_at) {
                        start_at--;
                    }
                }

                break;

            case VK_DOWN:

                if(selected_idx != values.size() - 1) {
                    index_changed = true;
                    selected_idx++;

                    if (selected_idx >= start_at + max_length) {
                        start_at++;
                    }
                }

                break;

            case VK_ESCAPE:
                return -1;

            case VK_RETURN:
                return selected_idx;
        }

        if(index_changed) {
            system("cls");
            draw();
        }
    }
}
