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

#include "globals.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void build81x81board(QGridLayout* mainLayout);
    void build3x3frame(QFrame* box3x3frame);
    void loadData();
    int mainEngine();

private:
    Ui::Widget *ui;
    QGridLayout* mainOriginLayout;
    QGridLayout* mainResultLayout;
    QLabel* title;
    QTextEdit* log;
    Globals *g;
};

#endif // WIDGET_H
