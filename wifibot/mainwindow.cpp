#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegExpValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // RegEx port
    ui->lineEdit_Port->setValidator(new QRegExpValidator(QRegExp("[0-9]{5}")));

    // RegEx IP
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    ui->lineEdit_IP->setValidator(ipValidator);









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

