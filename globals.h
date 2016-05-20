#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>


using namespace std;
using std::vector;
using std::string;

#define CLS_NO 11745    // Clauses maximal number
#define N 9
#define CELLS_NO N*N
#define VAR_NO N*N*N      // Variable number
#define DEBUG true

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
    void print() { std::cout << "((" << x << "," << y << ") == " << d << ")"; }
    int getx() { return x; }
    int gety() { return y; }
    int getd() { return d; }
    void setTruthValue(bool val) { d = (val ? 1 : 0); }
};

class Globals
{
public:

    int board[9][9];  // Sudoku board
    int boardOrigin[9][9];  // Sudoku board
    int boardValidated[9][9];  // to comapare between programs outputs and valid solution
    int formersize; //for debuging
    Threesome variableTable[VAR_NO+1]; // Variable table
    int variableCounter;
    vector<Clause> myClauses;
    bool pause;
    string sat_result_filename;
    pair<int,int> indexToMarix[CELLS_NO];
    string output_clauses_filename;
    bool debugMode;
    string m_board_filename;

    Globals(bool debug);
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

};



inline void Globals::print_board(string msg, bool origin)
{
    cout << msg << endl;
    for (int i = 0; i < 9; i++) {
        if (i == 0 || (i % 3) == 0)
            cout << "-------------------------" << endl;
        for (int j = 0; j < 9; j++) {
            if (j == 0 || (j % 3) == 0)
                cout << "| ";
            if (origin)
            {
                boardOrigin[i][j] ? cout << board[i][j] : cout << " "; cout << " ";
            }
            else
            {
                board[i][j] ? cout << board[i][j] : cout << " "; cout << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "-------------------------" << endl;
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
        cout << "x" << i << " | ";
        variableTable[i].print();
        int x = (i % 9 == 0) ? 9 : 0;
        if (variableTable[i].getd() ==1) cout <<"    " <<i << "mod9=" << i % 9 + x << endl;
        else cout << endl;
        if ((i % 100) == 0) {    // Every 100 lines wait for input (just to be able to see the output)
            system("pause");
        }
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
                    system("pause");
                }
            }
        }
    }

}



inline Globals::Globals(bool debug) //ctor
{
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

    cout << "we have " << k << " diffrent boards in the DB"<<endl;
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
        cout << "\nin " << msg << " function, there were " << myClauses.size() - formersize << " clauses created.\n"<<msg2 <<endl;
        cout << "total clauses now is " << myClauses.size() << ". the maximum is 11745."<<endl;
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
    cout << "\n\nsummary:" << endl;
    cout << "number of clauses for this board are: " << myClauses.size() << " from a maximum of 11745 clauses" << endl;
    print_board("origin", true);
    print_board("solved", false);
    if (debugMode) compareResultsOfProgramWithValidatedResults();
    system("pause");
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
        cout << "cannot open validation file" << endl;
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
                cout << "there is a mismatch" << endl;
                return;
            }
        }
    }
    cout << "PERFECT MATCH" << endl;
    boardfile.close();
}





#endif // GLOBALS_H
