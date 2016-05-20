#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    loadData();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    //g = new Globals(DEBUG);
    this->setWindowTitle("window title here");
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hl1 = new QHBoxLayout;
    this->setLayout(layout);
    mainOriginLayout = new QGridLayout();
    mainResultLayout = new QGridLayout();
    build81x81board(mainOriginLayout);
    build81x81board(mainResultLayout);

    title = new QLabel("title here");
    log = new QTextEdit("log");

    hl1->addLayout(mainOriginLayout);
    hl1->addLayout(mainResultLayout);

    layout->addWidget(title);
    layout->addLayout(hl1);
    layout->addWidget(log);


}

void Widget::build81x81board(QGridLayout *mainLayout)
{
     mainLayout->setSpacing(0);
     mainLayout->setMargin(0);
     for (int i = 0; i < 3; ++i)
     {
         for (int j = 0; j < 3; ++j)
         {
             QFrame* box3x3frame = new QFrame;
             build3x3frame(box3x3frame);
             mainLayout->addWidget(box3x3frame,i,j,1,1);
         }
     }
}

void Widget::build3x3frame(QFrame *box3x3frame)
{
    box3x3frame->setFrameStyle(QFrame::Plain);
    box3x3frame->setFrameShape(QFrame::Box);
    QGridLayout* box3x3 = new QGridLayout(box3x3frame);
    box3x3->setSpacing(0);
    box3x3->setMargin(0);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            QLabel* cell = new QLabel(QString::number(i*3+j+1));
            cell->setFrameStyle(QFrame::Plain);
            cell->setFrameShape(QFrame::Box);
            cell->setMargin(5);
            box3x3->addWidget(cell,i,j,1,1);
        }
    }
}

void Widget::loadData()
{
    //mainEngine();
}

int Widget::mainEngine()
{
    int op=0;                   // operation number
    string board_filename = "board0.txt";
    g = new Globals(DEBUG);
    bool solved = false;

    g->init_board(board_filename);
    g->print_board(board_filename + " -default board", true);
    cout << "Welcome to sudoku solver" << endl;

    while (true) {
        cout << "\n\nplease enter the number of the desired operation :" << endl;
        cout << "1. choose a sudoku board from DataBase?" << endl;
        cout << "2. Solve the sudoku board using sat solver" << endl;
        cout << "3. Solve the sudoku board using sat solver with break each digit" << endl;
        cout << "4. Print board" << endl;
        cout << "5. Print var table" << endl;
        cout << "6. exit" << endl;
        cin >> op;
        switch (op) {
        case 1:
            delete g;
            g = new Globals(DEBUG);
            g->switchBoard();
            solved = false;
            break;
        case 2:
        case 3:
            if (!solved) {
                g->runProgram(op);
                solved = true;
            }
            else
            {
                string currentBoard = g->m_board_filename + ".txt";
                delete g;
                g = new Globals(DEBUG);
                g->init_board(currentBoard);
                g->runProgram(op);
            }
            break;
        case 4:
            g->print_board("current board" , false);
            break;
        case 5:
            if (!solved) {
                cout << "you have to solve the board first.only than you can print the var table"<<endl;
            }
            else
            {
                g->print_var_table();
            }
            break;
        case 6:
            delete g;
            return 0;
        default:
            cout << "Unrecognized operation number, please try again" << endl;
        }
    }
    return 0;

}


