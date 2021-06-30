#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "table.h"

using std::cin, std::cout, std::string, std::vector;

bool emptyOrWhitespace(const string &s) {
    return s.find_first_not_of(" \t\n\v\f\r") == string::npos;
}

vector<string> processRow(const string &buffer) {
    vector<string> cols;
    string token;

    stringstream strs(buffer);

    while(getline(strs, token, ';')) {
        cols.push_back(token);
    }

    return cols;
}

bool allEmpty(const vector<string> &vec) {
    bool empty = true;
    for(const string &s : vec) {
        if(!emptyOrWhitespace(s)) {
            empty = false;
            break;
        }
    }
    return empty;
}

void deleteEmpty(vector<string> &v) {
    for(int b = 0; b < v.size();b++) {        //Remove all empty columns
        if(emptyOrWhitespace(v[b]))
            v.erase(v.begin() + b--);
    }
}

int main(int argc, char* argv[]) {
    ifstream fin;
    string fname;
    char linebuf[1024];
    Table tab;

    if(argc < 2) {
        cout << "Enter a file name: ";
        cin >> fname;
    } else if(argc == 2) {
        fname = argv[1];
    } else {
        cout << "USAGE: CSView.exe [filename]" << endl;
        return 0;
    }

    fin.open(fname);
    if(!fin.good()) {
        cerr << "Error opening file" << '\n';
        return -1;
    }

    tab.setTitle(fname);
    fill(begin(linebuf), end(linebuf), 0);

    for(int i = 0; fin.getline(linebuf, 1024); i++) {
        vector<string> cols = processRow(linebuf);

        if(allEmpty(cols))  //Skip empty rows
            break;

        string label = cols[0];

        if(i == 0) {
            cols.erase(cols.begin());
            deleteEmpty(cols);

            for(const string &a : cols) {
                tab.addColumn(a);
            }
            continue;
        }

        deleteEmpty(cols);

        cols.erase(cols.begin());
        tab.addRow(cols, label);
    }

    tab.draw();
}
