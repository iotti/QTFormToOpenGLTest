#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
MainWidget *w = NULL;
void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"PUSHED!";
    if(w)
        delete w;
    w = new MainWidget();
    this->setCentralWidget(w);
}
