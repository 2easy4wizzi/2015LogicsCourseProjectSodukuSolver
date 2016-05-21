#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    setObjectName("main gui");
    init();
    //guiHostt = this;

}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    this->setWindowTitle("window title here");
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hl1 = new QHBoxLayout;
    this->setLayout(layout);
    mainOriginLayout = new QGridLayout();
    mainResultLayout = new QGridLayout();
    mainOriginLayout->setObjectName("origin board");
    mainResultLayout->setObjectName("result board");
    build81x81board(mainOriginLayout,1);
    build81x81board(mainResultLayout,0);

    title = new QLabel("title here");
    log = new QTextEdit("log");
    q = new QLineEdit("type your query here...");
    runSolverButton = new QPushButton("solve");
    connect(runSolverButton,SIGNAL(clicked(bool)),this,SLOT(solveWasPressed()));
    hl1->addLayout(mainOriginLayout);
    hl1->addLayout(mainResultLayout);

    layout->addWidget(title);
    layout->addLayout(hl1);
    layout->addWidget(log);
    layout->addWidget(q);
    layout->addWidget(runSolverButton);


}

void Widget::build81x81board(QGridLayout *mainLayout, int QMap)
{
     int rowStart = 0;
     int colStart = 0;
     mainLayout->setSpacing(0);
     mainLayout->setMargin(0);
     for (int i = 0; i < 3; ++i)
     {
         for (int j = 0; j < 3; ++j)
         {
             QFrame* box3x3frame = new QFrame;
             box3x3frame->setObjectName(QString::number(i*3+j+1));
             build3x3frame(box3x3frame,rowStart,colStart,QMap);
             mainLayout->addWidget(box3x3frame,i,j,1,1);
             colStart+=3;
         }
         rowStart+=3;
     }
}




void Widget::build3x3frame(QFrame *box3x3frame, int rowStart, int colStart, int QMap)
{
    box3x3frame->setFrameStyle(QFrame::Plain);
    box3x3frame->setFrameShape(QFrame::Box);
    QGridLayout* box3x3 = new QGridLayout(box3x3frame);
    box3x3->setSpacing(0);
    box3x3->setMargin(0);
    int x = rowStart%9;
    for (int i = 0; i < 3; ++i)
    {
        int y = colStart%9;
        for (int j = 0; j < 3; ++j)
        {
            QLabel* cell = new QLabel("");
            if(QMap) originPointToCellMap.insert(QPair<int,int>(x,y++),cell);
            else     resultPointToCellMap.insert(QPair<int,int>(x,y++),cell);
            cell->setFrameStyle(QFrame::Plain);
            cell->setFrameShape(QFrame::Box);
            cell->setMargin(5);
            box3x3->addWidget(cell,i,j,1,1);
        }
        x++;
    }
}

void Widget::loadData()
{
    mainEngine();

}

int Widget::mainEngine()
{
    setTitle("asdasdqweqweqweqweqweqwe");
    op=0;                   // operation number
    string board_filename = "board0.txt";
    g = new Globals(DEBUG,this);
    solved = false;
    g->init_board(board_filename);
    g->print_board(board_filename + " -default board", true);
    appendRowToLog("Welcome to sudoku solver");
    printOptionsToLog();
    return 0;

}

void Widget::setTitle(QString newTitle)
{
    title->setText(newTitle);
}

void Widget::appendRowToLog(QString newRow)
{
    log->append(newRow);
}


QGridLayout *Widget::getOriginBoard()
{
    return mainOriginLayout;
}

QGridLayout *Widget::getResultBoard()
{
    return mainResultLayout;
}

QMap<QPair<int, int>, QLabel *> Widget::getOriginPointToCellMap()
{
    return originPointToCellMap;
}

QMap<QPair<int, int>, QLabel *> Widget::getResultPointToCellMap()
{
    return resultPointToCellMap;
}

void Widget::case1(bool &solved)
{
    delete g;
    g = new Globals(DEBUG,this);
    g->switchBoard();
    solved = false;
}
void Widget::case2(bool &solved)
{
    if (!solved) {
        g->runProgram(op);
        solved = true;
    }
    else
    {
        string currentBoard = g->m_board_filename + ".txt";
        delete g;
        g = new Globals(DEBUG,this);
        g->init_board(currentBoard);
        g->runProgram(op);
    }
}
void Widget::case3(bool &solved)
{
    case2(solved);
}
void Widget::case4()
{
    g->print_board("current board" , false);
}
void Widget::case5(bool &solved)
{
    if (!solved) {
        appendRowToLog("you have to solve the board first.only than you can print the var table");
    }
    else
    {
        g->print_var_table();
    }
}
void Widget::case6()
{
    delete g;
    exit (0);
}

void Widget::printOptionsToLog()
{
    appendRowToLog("\n\nplease enter the number of the desired operation :");
    appendRowToLog("1. choose a sudoku board from DataBase?");
    appendRowToLog("2. Solve the sudoku board using sat solver");
    appendRowToLog("3. Solve the sudoku board using sat solver with break each digit");
    appendRowToLog("4. Print board");
    appendRowToLog("5. Print var table");
    appendRowToLog("6. exit");
}


void Widget::solveWasPressed()
{
    case2(solved);
}



