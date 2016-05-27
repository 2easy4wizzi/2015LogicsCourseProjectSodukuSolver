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
    delete g;
    delete ui;
}

void Widget::init()
{

    setCentralWidgetPropreties();
    setSoDukoBoardsPropreties();
    setsolvePropreties();
    setDBPropreties();
    setlogPropreties();
}

void Widget::setCentralWidgetPropreties()
{
    this->setWindowTitle("Vadim Khakham & Gilad Eini Soduku Solver using sat4j");
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);
}

void Widget::setSoDukoBoardsPropreties()
{
    QVBoxLayout* originBlockLayout = new QVBoxLayout;
    titleOrigin = new QLabel("origin");
    titleOrigin->setStyleSheet("font-family: \"Times New Roman\", Times, serif; font-size: 20px; background-color: yellow");
    titleOrigin->setAlignment(Qt::AlignCenter);
    originBoardLayout = new QGridLayout();
    originBoardLayout->setObjectName("origin board");
    build81x81board(originBoardLayout,1);
    originBlockLayout->addWidget(titleOrigin);
    originBlockLayout->addLayout(originBoardLayout);
    originBlockLayout->setSpacing(2);



    QVBoxLayout* resultBlockLayout = new QVBoxLayout;
    titleResult = new QLabel("result");
    titleResult->setStyleSheet("font-family: \"Times New Roman\", Times, serif; font-size: 20px;");
    titleResult->setAlignment(Qt::AlignCenter);
    setTitleResultColor(1);
    resultBoardLayout = new QGridLayout();
    resultBoardLayout->setObjectName("result board");
    build81x81board(resultBoardLayout,0);
    resultBlockLayout->addWidget(titleResult);
    resultBlockLayout->addLayout(resultBoardLayout);
    resultBlockLayout->setSpacing(2);

    QHBoxLayout* originAndResultBlocksLayouts = new QHBoxLayout;
    originAndResultBlocksLayouts->addLayout(originBlockLayout);
    originAndResultBlocksLayouts->addLayout(resultBlockLayout);
    //hl1->addWidget(changeBoard);
    originAndResultBlocksLayouts->setSpacing(50);

    QWidget* nonResizeBlocks = new QWidget(this);
    nonResizeBlocks->setFixedSize(QSize(500,300));
    nonResizeBlocks->setLayout(originAndResultBlocksLayouts);

    mainLayout->addWidget(nonResizeBlocks);
    mainLayout->setAlignment(nonResizeBlocks, Qt::AlignCenter);


}

void Widget::setlogPropreties()
{
    log = new QTextEdit();
    log->setStyleSheet("font-family: \"Times New Roman\", Times, serif; font-size: 20px;");
    log->setReadOnly(true);
    mainLayout->addWidget(log);
}

void Widget::setDBPropreties()
{
    changeBoard = new QGroupBox;
    changeBoard->setTitle("DB Options");
    //changeBoard->setStyleSheet("border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; border-color: red");

    QHBoxLayout* nextAndPrevButtons = new QHBoxLayout;
    QPushButton* nextBoard = new QPushButton("next");
    nextBoard->setObjectName("next");
    connect(nextBoard,SIGNAL(clicked(bool)),this,SLOT(changeBoards()));
    QPushButton* prevBoard = new QPushButton("prev");
    prevBoard->setObjectName("prev");
    connect(prevBoard,SIGNAL(clicked(bool)),this,SLOT(changeBoards()));
    nextAndPrevButtons->addWidget(prevBoard);
    nextAndPrevButtons->addWidget(nextBoard);


    QHBoxLayout* customPathLayout = new QHBoxLayout;
    QLabel* customPath = new QLabel("custom");
    customPath->setToolTip("enter path to board file. ex: <name>.txt");
    newBoardPath = new QLineEdit("");
    newBoardPath->setToolTip("enter path to board file. ex: <name>.txt");
    QPushButton* newBoardPathLoad = new QPushButton("load");
    newBoardPathLoad->setObjectName("load");
    connect(newBoardPathLoad,SIGNAL(clicked(bool)),this,SLOT(changeBoards()));
    compareBoardPath = new QLineEdit("");
    compareBoardPath->setReadOnly(true);
    compareBoardPath->setToolTip("path to solved board.after solving the board, comparing result with validated solution");

    customPathLayout->addWidget(customPath);
    customPathLayout->addWidget(newBoardPath);
    customPathLayout->addWidget(newBoardPathLoad);
    customPathLayout->addWidget(compareBoardPath);

    QVBoxLayout* changeBoardLayout = new QVBoxLayout;
    changeBoardLayout->addLayout(nextAndPrevButtons);
    changeBoardLayout->addLayout(customPathLayout);

    changeBoard->setLayout(changeBoardLayout);
    QHBoxLayout* spacersLayout = new QHBoxLayout();

    spacersLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    spacersLayout->addWidget(changeBoard);
    spacersLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    mainLayout->addLayout(spacersLayout);
}

void Widget::setsolvePropreties()
{
    runSolverButton = new QPushButton("solve");
    runSolverButton->setToolTip("run sat solver on chosen board");
    connect(runSolverButton,SIGNAL(clicked(bool)),this,SLOT(solveWasPressed()));

    printVtButton = new QPushButton("print VT");
    printVtButton->setToolTip("print 729 cells of the Variable Table");
    connect(printVtButton,SIGNAL(clicked(bool)),this,SLOT(printVt()));

    animationButton = new QPushButton("turn animation on");
    animationButton->setToolTip("printing with animation");
    connect(animationButton,SIGNAL(clicked(bool)),this,SLOT(animationButtonPressed()));

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    buttonLayout->addWidget(runSolverButton);
    buttonLayout->addWidget(printVtButton);
    buttonLayout->addWidget(animationButton);
    buttonLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    mainLayout->addLayout(buttonLayout);
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
            cell->setAlignment(Qt::AlignCenter);
            //cell->setAlignment(Qt::AlignVCenter);
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

int Widget::mainEngine()
{
    animation = false;
    string board_filename = "board0.txt";
    currnetBoardNumber = 0;
    g = new Globals(DEBUG,this);
    solved = false;
    g->init_board(board_filename);
    g->print_board(board_filename + " -default board", true);
    appendRowToLog("Welcome to sudoku solver");
    printOptionsToLog();

    return 0;

}

void Widget::setTitleResultColor(bool start)
{
    QString color = start ? "font-family: \"Times New Roman\", Times, serif; font-size: 20px; background-color: red;" : "font-family: \"Times New Roman\", Times, serif; font-size: 20px; background-color: green;";
    titleResult->setStyleSheet(color);
}

void Widget::setTitleOrigin(QString newTitle)
{
    titleOrigin->setText(newTitle);
}
void Widget::setTitleResult(QString newTitle)
{
    titleResult->setText(newTitle);
}

void Widget::appendRowToLog(QString newRow)
{
    log->append(newRow);
}


QGridLayout *Widget::getOriginBoard()
{
    return originBoardLayout;
}

QGridLayout *Widget::getResultBoard()
{
    return resultBoardLayout;
}

QMap<QPair<int, int>, QLabel *> Widget::getOriginPointToCellMap()
{
    return originPointToCellMap;
}

QMap<QPair<int, int>, QLabel *> Widget::getResultPointToCellMap()
{
    return resultPointToCellMap;
}

void Widget::changeBoards()
{
    setTitleResultColor(1);
    delete g;
    g = new Globals(DEBUG,this);
    QString senderName = QObject::sender()->objectName();
    log->clear();
    if(senderName == "load")
    {
        QString path = newBoardPath->text();
        currnetBoardNumber = 0;
        int result = g->chooseBoard(path,compareBoardPath);
        if(result)
        {
            titleOrigin->setText(path);
            appendRowToLog("success loading " + path);
        }
        else
        {
            newBoardPath->setText("");
            //QMessageBox msgBox;msgBox.setText("can't find board file with the name: " + path);msgBox.exec();
            newBoardPath->setText("board0.txt");
            g->chooseBoard("board0.txt",compareBoardPath);
            titleOrigin->setText(QString(g->m_board_filename.c_str()) +" -default board");
            appendRowToLog("failue loading " + path + "- loaded default board instead: board0.txt");
        }
    }
    else
    {
        cleanHighlights();
        g->switchBoard(senderName,currnetBoardNumber);
    }
    solved = false;
}



void Widget::printVt()
{
    log->clear();
    if (!solved) {
        appendRowToLog("you have to solve the board first.only than you can print the var table");
    }
    else
    {
        g->print_var_table();
    }

}

void Widget::animationButtonPressed()
{
    animation = !animation;
    animation ? animationButton->setText("turn animation off") : animationButton->setText("turn animation on") ;
}


void Widget::printOptionsToLog()
{
    g->countDB();

}

void Widget::cleanHighlights()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QLabel* cellOrigin = getOriginPointToCellMap()[QPair<int,int>(i,j)];
            QLabel* cellResult = getResultPointToCellMap()[QPair<int,int>(i,j)];;
            cellResult->setText(" ");
            cellResult->setStyleSheet(" ");
            cellOrigin->setStyleSheet(" ");
        }
    }
}

void Widget::wait()
{

    msgBox = new QMessageBox();
    msgBox->setGeometry(5000,5000,5000,5000);
    QTimer::singleShot(0, this, SLOT(closeMessageBox()));
    msgBox->exec();


}


void Widget::solveWasPressed()
{
    log->clear();
    if (!solved) {
        g->runProgram(animation);
        solved = true;
    }
    else
    {
        string currentBoard = g->m_board_filename + ".txt";
        delete g;
        g = new Globals(DEBUG,this);
        g->init_board(currentBoard);
        g->runProgram(animation);
    }
}

void Widget::closeMessageBox()
{
    msgBox->close();
}

int Widget::Animation() const
{
    return animation;
}

void Widget::setAnimation(bool value)
{
    animation = value;
}

QLabel *Widget::getLastCell() const
{
    return lastCell;
}

void Widget::setLastCell(QLabel *value)
{
    lastCell = value;
}

QPair<int, int> *Widget::getLastPair() const
{
    return lastPair;
}

void Widget::setLastPair(QPair<int,int> *value)
{
    lastPair = value;
}





