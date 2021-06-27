#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>

typedef unsigned short WORD;

using namespace std;

class Table {

    vector<vector<string>> values;
    vector<string> rows;
    vector<string> columns;

    string _title = "";

    int calculateWidth();
    int getColumnWidth();
    void drawHorLine();
    void drawColumnNames();
    void drawRow(vector<string> vec);
    void drawTitle();
    int getColumnWidth(int index);

    public:

    Table() = default;
    Table(string title);

    void addColumn(string _text);
    void removeColumn(int _index);
    void insertColumn(string _text, int _index);

    void addRow(vector<string> _text);
    void removeRow(int _index);
    void insertRow(vector<string> _text, int _index);

    void setValue(int row, int col, string _text);

    void setTitle(string _text);

    void draw();

};
#endif
