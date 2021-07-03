#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
#include "conui.h"

typedef unsigned short WORD;

using std::vector, std::string;

class Table {

protected:
    vector<vector<string>> values;

    vector<string> rows;        //For storing the row and column labels
    vector<string> columns;

    string _title;

    bool showRowLabels = false;
    bool separateRows = false;

    unsigned int calculateWidth();
    unsigned int getColumnWidth(int index);
    int getLabelsWidth();
    void drawLine();
    virtual void drawRow(const vector<string> &vec, int index);
    void drawTitle();


public:

    Table() = default;
    explicit Table(const string &title);

    void addColumn(const string &text);
    void removeColumn(int index);
    void insertColumn(const string &text, int index);

    void addRow(vector<string> row, const string &label = "");
    void removeRow(int index);
    void insertRow(const vector<string> &row, int index, const string &label = "");

    void setValue(int row, int col, const string &_text);

    void setTitle(const string &_text);

    virtual void draw();

};

class SelectionTable : public Table {
protected:

    int selected_idx = 0;
    conUI _ui;
    void draw() override;

public:

    explicit SelectionTable(const conUI &ui, const string &title = "");
    int show();
};
#endif
