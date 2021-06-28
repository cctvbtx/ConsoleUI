#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>

typedef unsigned short WORD;

using namespace std;

class Table {

    vector<vector<string>> values;

    vector<string> rows;        //For storing the row and column labels
    vector<string> columns;

    string _title;

    bool showRowLabels = false;

    int calculateWidth();
    int getColumnWidth(int index);
    int getLabelsWidth();
    void drawHorLine();
    void drawRow(const vector<string> &vec, int idx);
    void drawTitle();


    public:

    Table() = default;
    Table(string title);

    static string repeatStr(string text, int n);

    void addColumn(string _text);
    void removeColumn(int _index);
    void insertColumn(string _text, int _index);

    void addRow(const vector<string> &_values, string label = "");
    void removeRow(int _index);
    void insertRow(const vector<string> &_values, int _index, string label = "");

    void setValue(int row, int col, string _text);

    void setTitle(string _text);

    void draw();

};
#endif
