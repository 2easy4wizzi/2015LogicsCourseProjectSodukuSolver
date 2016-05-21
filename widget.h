#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>

class Globals;
namespace Ui {
class Widget;
//class Globals;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void build81x81board(QGridLayout* mainLayout,int QMap);
    void build3x3frame(QFrame* box3x3frame,int rowStart, int colStart,int QMap);
    void loadData();
    int mainEngine();
    void setTitle(QString newTitle);
    void appendRowToLog(QString newRow);
    QGridLayout* getOriginBoard();
    QGridLayout* getResultBoard();

    QMap<QPair<int, int>, QLabel *> getOriginPointToCellMap() ;

    QMap<QPair<int, int>, QLabel *> getResultPointToCellMap() ;
    void case1(bool& solved);
    void case2(bool& solved);
    void case3(bool& solved);
    void case4();
    void case5(bool &solved);
    void case6();
    void printOptionsToLog();

public slots:
    void solveWasPressed();

private:
    Ui::Widget *ui;
    QGridLayout* mainOriginLayout;
    QGridLayout* mainResultLayout;
    QLabel* title;
    QTextEdit* log;
    QLineEdit* q;
    QPushButton* runSolverButton;
    Globals *g;
    QMap<QPair<int,int>,QLabel*> originPointToCellMap;
    QMap<QPair<int,int>,QLabel*> resultPointToCellMap;
    int op;
    bool solved;

};

#ifndef GLOBALS_H
#define GLOBALS_H

#include "widget.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>

class Widget;
using namespace std;
using std::vector;
using std::string;

#define CLS_NO 11745    // Clauses maximal number
#define N 9
#define CELLS_NO N*N
#define VAR_NO N*N*N      // Variable number
#define DEBUG false

class Clause {
private:
    vector<int> arr;
public:
    int size;
    Clause(int x) :arr(x) { size = x; }
    vector<int> getClauseArray() { return arr; }
    void setClauseIthElemnet(int index, int value) { arr[index] = value; }
};

class Threesome
{
private:
    int x,y,d;
public:
    Threesome() :x(0), y(0), d(0) {}
    void setTrio(int a, int b, int c) { x = a; y = b; d = c; }
    void print() {  }
    int getx() { return x; }
    int gety() { return y; }
    int getd() { return d; }
    void setTruthValue(bool val) { d = (val ? 1 : 0); }
};

class Globals
{
    //Q_OBJECT
public:

    int board[9][9];  // Sudoku board
    int boardOrigin[9][9];  // Sudoku board
    int boardValidated[9][9];  // to comapare between programs outputs and valid solution
    size_t formersize; //for debuging
    Threesome variableTable[VAR_NO+1]; // Variable table
    int variableCounter;
    vector<Clause> myClauses;
    bool pause;
    string sat_result_filename;
    pair<int,int> indexToMarix[CELLS_NO];
    string output_clauses_filename;
    bool debugMode;
    string m_board_filename;

    Globals(bool debug,Widget* guiHost);
    void init_board(string board_filename);						// Sudoku board initialization
    void print_board(string msg, bool origin);											// Print a Sudoku board (with no 0's)
    void fill_var_table_and_givens();
    void print_var_table();

    string output_clauses_file();    // Creation of output_clauses file (for the SAT solver)
    string run_sat_solver(string clauses_filename);             // Running SAT solver program on the output_clauses file
    void read_sat_result_file(string filename);                 // Reading the result output file of the SAT solver program and translating it to the Sudoku board

    bool isCellGiven(int i);
    int getCellGiven(int i);
    void uniqueness();
    void validClauses();
    void defenitionClauses();

    void switchBoard();
    void debug(string msg, string msg2);

    void runProgram(int op);

    void add9Choose2NegativeClauses(vector<int> vec);

    void compareResultsOfProgramWithValidatedResults();

    Widget* gui;

};


inline void Globals::print_board(string msg, bool origin)
{

//    qDebug()<< "1" << parent->objectName();
//    qDebug()<< "2" << parent->getOriginBoard()->objectName();
//    qDebug()<< "3" << parent->getOriginBoard()->count();
//    qDebug()<< "4" << parent->getOriginBoard()->itemAt(7);
//    QLayoutItem* item3x3 =parent->getOriginBoard()->itemAt(7);
//    qDebug()<< "4" << item3x3;
//    QFrame * box3x3frame = dynamic_cast<QFrame *>(item3x3->widget());
//    qDebug()<< "4" << box3x3frame;
//    QObjectList box3x3list = box3x3frame->children();
//    qDebug()<< "5" << box3x3list.size();
//    QGridLayout* box3x3 = dynamic_cast<QGridLayout*>(box3x3frame->layout());
//    qDebug()<< "6" << box3x3 << box3x3->count() << box3x3->itemAt(2);
//    QLayoutItem* itemCell =box3x3->itemAt(2);
//    qDebug()<< "6" <<itemCell;
//    QLabel* a = dynamic_cast<QLabel*>(itemCell->widget());
//    qDebug()<< "6" << a->text();
//    a->setText("asd");

    gui->setTitle(QString(msg.c_str()));



    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (origin)
            {
                QString content = boardOrigin[i][j] ? QString::number(boardOrigin[i][j]) : " ";
                gui->getOriginPointToCellMap()[QPair<int,int>(i,j)]->setText(content);
            }
            else
            {
                QString content = board[i][j] ? QString::number(board[i][j]) : " ";
                gui->getResultPointToCellMap()[QPair<int,int>(i,j)]->setText(content);
            }
        }
    }
}



//filling var table: 729 vars. variableTable[k] = threesome(i,j,d) , where i,j are cordinates in the board and d is a boolean which states if the k-th variable is in the i,j cell(k mod 9)
inline void Globals::fill_var_table_and_givens()
{
    formersize = myClauses.size();
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            if (board[i - 1][j - 1] == 0) //not a given
            {
                for (int k = 1; k <= 9; k++) {
                    variableTable[++variableCounter].setTrio(i, j, -1);     // Variable number myVtCtr means if d==true => in the soduku board in the i,j place there is the digit: myVtCtr mod 9
                }
            }
            else { //is given
                int val = board[i - 1][j - 1];
                for (int k = 1; k <= 9; k++) {
                    int y = val == k ? 1 : 0;
                    variableTable[++variableCounter].setTrio(i, j, y);     // Variable number myVtCtr means if d==true => in the soduku board in the i,j place there is the digit: myVtCtr mod 9
                    if (val == k)
                    {
                        Clause tempClause(1);
                        int clauseSign = y == 1 ? variableCounter : -1 * variableCounter;
                        tempClause.setClauseIthElemnet(0, clauseSign);
                        myClauses.push_back(tempClause);
                    }
                }
            }
        }
    }
    debug("givens","for each given, we add a clause of size 1");
}

inline void Globals::print_var_table()
{
    for (int i = 1; i <= variableCounter; i++) {
        gui->appendRowToLog( "x" + QString::number(i) + " | ");
        //variableTable[i].print();
        gui->appendRowToLog("((" + QString::number(variableTable[i].getx()) + "," + QString::number(variableTable[i].gety()) + ") == " + QString::number(variableTable[i].getd()) + ")");
        int x = (i % 9 == 0) ? 9 : 0;
        if (variableTable[i].getd() ==1) gui->appendRowToLog("    " + QString::number(i) + "mod9=" + QString::number(i % 9 + x) );
        else gui->appendRowToLog("\n");
//        if ((i % 100) == 0) {    // Every 100 lines wait for input (just to be able to see the output)
//            system("pause");
//        }
    }
}

inline string Globals::output_clauses_file()
{
    string clauses_filename = "clauses.cnf";
    ofstream clausefile(clauses_filename.c_str());      // open clausefile with clauses_filename for writing

    clausefile << "p cnf " << VAR_NO << " " << CLS_NO << endl;      // first line in the file
    for (Clause cl : myClauses) //iterating through all the clauses
    {
        vector<int> pvec(cl.getClauseArray());
        clausefile << " ";
        for (vector<int>::iterator it = pvec.begin(); it != pvec.end(); it++) {      // run trough pvec and insert its values into the clausefile
            clausefile << *(it) << " ";
        }
        clausefile << "0" << endl;      // every line ends with 0
    }
    clausefile.close();

    return clauses_filename;
}

inline string Globals::run_sat_solver(string clauses_filename)
{
    string sat_command = "java -jar org.sat4j.core.jar ";     // the command to run the SAT solver
    string redirect = " > ";                                  // output redirection command
    string sat_result_filename = "sat_result.txt";            // target filename for SAT solver result

    sat_command.append(clauses_filename);
    sat_command.append(redirect);
    sat_command.append(sat_result_filename);

    system(sat_command.c_str());        // execute the combined command - SAT solver runs and outputs result to sat_result.txt

    return sat_result_filename;
}

inline void Globals::read_sat_result_file(string filename)
{
    fstream parsefile(sat_result_filename.c_str(), std::ios_base::in);      // open parsefile file named sat_result_filename for reading
    string input;
    string satisfiable = "SATISFIABLE";
    string unsatisfiable = "UNSATISFIABLE";

    parsefile >> input;     // first input from parsefile
    while (input.compare(satisfiable) != 0 && input.compare(unsatisfiable) != 0) {   // the result file contains unneeded data, this loop runs trough it...
        parsefile >> input;                                                    //...until it reaches SATISFIABLE
    }
    parsefile >> input;     // last input ("v") from parsefile before the actual result
    int index;
    bool value;
    for (int i = 1; i < VAR_NO+1; i++) {
        parsefile >> index;
        value = (index > 0) ? true : false; //checking if the assignment is true or false
        if (value == false) index *= -1; // cant have negative index
        variableTable[index].setTruthValue(value); //mark at the variable table the value of this var.
    }

    //filling the board with the results
    for (int i = 1; i < VAR_NO+1; i++) {
        if (variableTable[i].getd() == 1) { //no need for vars that are 0
            if (board[variableTable[i].getx() - 1][variableTable[i].gety() - 1] == 0) //check that its not already on the board (given)
            {
                board[variableTable[i].getx() - 1][variableTable[i].gety() - 1] = i % 9; //gives the board the digit i mod 9. for ex: variableTable[11] == (1,1,1) => board[1][1] = 11%9 which is 2.
                if (i % 9 == 0) board[variableTable[i].getx() - 1][variableTable[i].gety() - 1] = 9; // if i%9 is 0, change it to 9
                if (pause) {//if user asked for move by move fillins
                    print_board("step by step", false);
                    //system("pause");
                }
            }
        }
    }

}



inline Globals::Globals(bool debug, Widget *guiHost) //ctor
{
    gui=guiHost;
    pause = false; formersize = 0; variableCounter = 0; debugMode = debug;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            indexToMarix[i*9+j] = pair<int, int>(i, j); //will be used to map number from 1-81 to i,j cordinates
        }
    }
}

inline void Globals::init_board(string board_filename)
{
    fstream boardfile(board_filename.c_str(), std::ios_base::in);      // initializing board and keeping the origin
    m_board_filename = board_filename.substr(0,board_filename.size()-4); //drop .txt
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            boardfile >> board[i][j];
            boardOrigin[i][j] = board[i][j];
        }
    }
    boardfile.close();
}


inline bool Globals::isCellGiven(int i)
{
    return board[indexToMarix[i].first][indexToMarix[i].second] != 0; //checks if the given cell is occupied
}

inline int Globals::getCellGiven(int i)
{
    return board[indexToMarix[i].first][indexToMarix[i].second] ; //gets a given's value
}



inline void Globals::switchBoard()//allowing user to choose boards from our "data base"
{
    char confirm = NULL;
    int i = 1,j = 0,k = 0;
    while (1) {
        string board_filename = "board" + std::to_string((j++)) + ".txt";
        fstream boardfile(board_filename.c_str(), std::ios_base::in);      // initializing board and keeping the origin
        if (boardfile.is_open()) k++;
        else break;
    }



    gui->appendRowToLog("we have " + QString::number(k) + " diffrent boards in the DB");
    while (confirm != 'c')
    {
        string board_filename = "board" + std::to_string((i++)%k) + ".txt";
        init_board(board_filename);
        m_board_filename = board_filename.substr(0, board_filename.size() - 4); //drop .txt
        print_board(board_filename + " -to confirm board, type 'c':", true);
        cin >> confirm;
    }
}

inline void Globals::debug(string msg, string msg2)
{
    if (debugMode)
    {
        gui->appendRowToLog( "\nin " + QString(msg.c_str()) + " function, there were " + QString::number( myClauses.size() - formersize )+ " clauses created.\n" + QString(msg2.c_str()) );
        gui->appendRowToLog( "total clauses now is " + QString::number(myClauses.size()) + ". the maximum is 11745.");
    }
}

inline void Globals::runProgram(int op)
{
    if (op == 3) pause = true;
    fill_var_table_and_givens();
    defenitionClauses();
    uniqueness();
    validClauses();
    output_clauses_filename = output_clauses_file();
    sat_result_filename = run_sat_solver(output_clauses_filename);
    read_sat_result_file(sat_result_filename);
    gui->appendRowToLog("\n\nsummary:");
    gui->appendRowToLog("number of clauses for this board are: " + QString::number(myClauses.size()) + " from a maximum of 11745 clauses");
    print_board("origin", true);
    print_board("solved", false);
    if (debugMode) compareResultsOfProgramWithValidatedResults();
    //QMessageBox msgBox;msgBox.setText(QString::number(debugMode));msgBox.exec();
    //system("pause");
}

inline void Globals::add9Choose2NegativeClauses(vector<int> vec)
{
    for (vector<int>::iterator it1 = vec.begin(); it1 < vec.end(); ++it1) //9 choose 2
    {
        for (vector<int>::iterator it2 = it1; it2 < vec.end(); ++it2)
        {
            if (*it1 != *it2) {
                Clause myClause(2);
                myClause.setClauseIthElemnet(0, -1 * (*it1));
                myClause.setClauseIthElemnet(1, -1 * (*it2));
                myClauses.push_back(myClause);
            }
        }
    }


}

inline void Globals::compareResultsOfProgramWithValidatedResults()
{
    string validatedResult = m_board_filename + "valid.txt";
    fstream boardfile(validatedResult.c_str(), std::ios_base::in);      // initializing board and keeping the origin

    if (boardfile.is_open() == 0) {
        gui->appendRowToLog("cannot open validation file");
        return;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            boardfile >> boardValidated[i][j];
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (boardValidated[i][j] != board[i][j])
            {
                gui->appendRowToLog("there is a mismatch");
                return;
            }
        }
    }
    gui->appendRowToLog("PERFECT MATCH");
    boardfile.close();
}

inline void Globals::uniqueness()//creating the uniqueness clauses. maximum 2916 clauses
{
    formersize = myClauses.size(); //for debuging

    for (int i = 0; i < (CELLS_NO); ++i) //maximum 1 digit per cell
    {
        if (!isCellGiven(i)) // if a cell contains a digit, skip it.
        {

            vector<int> temp;
            for (int j = i * 9; j < (i * 9) + 9; j++)
            {
                temp.push_back(j + 1);
            }

            add9Choose2NegativeClauses(temp);


        }
    }
    debug("uniqueness", "the maximum clauses are 2916");//for debugging
}

inline void Globals::validClauses() //creating the uniqueness clauses. maximum 8748 clauses
{
    formersize = myClauses.size();	//for debuging
                                    /*
                                    j - represents which set is in progress. ex: j==3 => set 4 in progress. making xors for the 4th row,column and grid(for all digits)
                                    h - represents which digit in progress . ex: h==2 => creating xors for the digit '2'.
                                    k - represents which element in progress. ex: k==3 => inside the xor being made now from 9 booleans representing a digit, we are on the 3rd.
                                    */
    int dropGridOffset = -162;
    for (int j = 0; j < N; j++)
    {
        if (j % 3 == 0) dropGridOffset += 162;
        for (int h = 1; h < 10; h++) //this loop does 1 column + 1 row + 1 grid (for all 9 digits)
        {
            vector<int> row;
            vector<int> col;
            vector<int> grid;

            for (int k = 0; k < 9; k++) //prepering the 9 elements needed in a row
            {
                row.push_back(k * 9 + h + j * 81);
            }
            add9Choose2NegativeClauses(row);


            for (int k = 0; k < 9; k++) //prepering the 9 elements needed in a column
            {
                col.push_back(k * 81 + h + j * 9);
            }
            add9Choose2NegativeClauses(col);

            int dropLineOffset = -1;
            for (int k = 0; k < 9; k++) //prepering the 9 elements needed in a grid
            {
                if (k % 3 == 0) dropLineOffset++;
                grid.push_back(k * 9 + dropLineOffset * 54 + h + dropGridOffset + 27 * j);
            }
            add9Choose2NegativeClauses(grid);
        }
    }
    debug("validClauses", "the maximum clauses are 8748");//for debuging
}

inline void Globals::defenitionClauses() //creating the uniqueness clauses. maximum 81 clauses
{
    formersize = myClauses.size(); //for debugging
    for (int i = 0; i < (CELLS_NO); ++i) //81 clauses that make sure each cell got at least 1 digit in him
    {
        if (!isCellGiven(i)) //is cell already contains a digit, skip it.
        {
            Clause myClause(9);
            for (int j = i * 9; j < (i * 9) + 9; j++)
            {
                myClause.setClauseIthElemnet(j % 9, j + 1);
            }
            myClauses.push_back(myClause);
        }
    }
    debug("defenitionClauses", "the maximum clauses are 81"); //for debugging
}



#endif // GLOBALS_H



#endif // WIDGET_H
