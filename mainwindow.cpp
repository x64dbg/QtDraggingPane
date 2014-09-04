#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PaneWidget.h"
#include "PaneSubWidget.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "hello world";
    ui->setupUi(this);
    // the central widget is just a widget = container of panes
    setCentralWidget(ui->widgetpane);
    // we want to nest the panes
    PaneSubWidget *mainPane = new PaneSubWidget(this);
    mainPane->setIdName("mainpane");
    mainPane->setObjectName("mainpane");
    // place the topmost pane
    ui->widgetpane->setPane(mainPane);

    PaneSubWidget *leftPane = new PaneSubWidget(mainPane);
    leftPane->setIdName("leftpane");
    leftPane->setObjectName("leftpane");
    PaneSubWidget *rightPane = new PaneSubWidget(mainPane);
    rightPane->setIdName("rightpane");
    rightPane->setObjectName("rightpane");

    // create dummy elements
    for(int i = 0; i < 5; i++)
    {
        QPushButton* b1 = new QPushButton(QString("leftpane %1").arg(i + 1));
        b1->setWindowTitle(b1->text());
        b1->setObjectName(b1->text());
        leftPane->addWidget(b1);
    }
    for(int i = 0; i < 5; i++)
    {
        QPushButton* b1 = new QPushButton(QString("rightpane %1").arg(i + 1));
        b1->setWindowTitle(b1->text());
        b1->setObjectName(b1->text());
        rightPane->addWidget(b1);
    }

    // add the panes as tabs
    mainPane->addWidget(leftPane,TabIn);
    mainPane->addWidget(rightPane,TabIn);



}

MainWindow::~MainWindow()
{
    delete ui;
}
