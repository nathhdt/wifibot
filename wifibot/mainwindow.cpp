#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //wifibotv3.doConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    wifibotv3.Stop();
}

void MainWindow::on_pushButton_2_clicked()
{
    wifibotv3.Avant(150, 150);
}

