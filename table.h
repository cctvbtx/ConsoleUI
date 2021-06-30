#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
#include "conui.h"

typedef unsigned short WORD;

using namespace std;

class Table {

protected:
    vector<vector<string>> values;

    vector<string> rows;        //For storing the row and column labels
    vector<string> columns;

    string _title;

    vector<int> extra_rows;

    bool showRowLabels = false;

    int calculateWidth();
    int getColumnWidth(int index);
    int getLabelsWidth();
    void drawHorLine();
    virtual void drawRow(const vector<string> &vec, int idx);
    void drawTitle();


public:

    Table() = default;
    Table(const string &title);

    static string repeatStr(const string &text, int n);

    void addColumn(const string &_text);
    void removeColumn(int _index);
    void insertColumn(const string &_text, int _index);

    void addRow(const vector<string> &_values, string label = "");
    void removeRow(int _index);
    void insertRow(const vector<string> &_values, int _index, string label = "");

    void setValue(int row, int col, string _text);

    void setTitle(const string &_text);

    void draw();

};

class SelectionTable : public Table {

    int selected_idx = 0;
    conUI _ui;

    void draw();

    public:

    SelectionTable(const conUI &ui, string title = "");
    int show();
};
#endif
