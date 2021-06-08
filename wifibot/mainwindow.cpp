#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegExpValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup UI
    ui->plainTextEdit_console->setStyleSheet("QPlainTextEdit{color: #eeeeee; background-color: #303030; selection-background-color: #606060; selection-color: #ffffff;}");

    // RegEx port
    ui->lineEdit_Port->setValidator(new QRegExpValidator(QRegExp("[0-9]{5}")));


    // Setup camera
    view = new QWebEngineView(parent);


    ui->gridLayout->addWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertConsole(QString text)
{
    ui->plainTextEdit_console->appendPlainText(text);
}


void MainWindow::on_pushButton_Connexion_clicked()
{
    if (ui->lineEdit_IP->text() == "..." || ui->lineEdit_Port->text() == "")
    {
        ui->plainTextEdit_console->appendPlainText("[Connexion] Veuillez entrer une IP et un port valides");
    }
    else
    {
        // Désactive le bouton de connexion
        ui->pushButton_Connexion->setEnabled(false);
        ui->pushButton_Connexion->repaint();

        // Console
        ui->plainTextEdit_console->insertPlainText("\n[Connexion] Connexion sur ");
        ui->plainTextEdit_console->insertPlainText(ui->lineEdit_IP->text());
        ui->plainTextEdit_console->insertPlainText(":");
        ui->plainTextEdit_console->insertPlainText(ui->lineEdit_Port->text());
        ui->plainTextEdit_console->insertPlainText("...");

        ui->plainTextEdit_console->repaint();

        // Connexion au robot
        bool tryConn = wifibotv3.doConnect(ui->lineEdit_IP->text(), ui->lineEdit_Port->text().toInt());

        if (tryConn == true)
        {
            // Affichage de la caméra
            showCamera();

            // Active les boutons nécessaires
            ui->pushButton_Deconnexion->setEnabled(true);
            ui->pushButton_Deconnexion->repaint();
            ui->pushButton_droite->setEnabled(true);
            ui->pushButton_droite->repaint();
            ui->pushButton_gauche->setEnabled(true);
            ui->pushButton_gauche->repaint();
            ui->pushButton_avancer->setEnabled(true);
            ui->pushButton_avancer->repaint();
            ui->pushButton_reculer->setEnabled(true);
            ui->pushButton_reculer->repaint();
            ui->pushButton_stop->setEnabled(true);
            ui->pushButton_stop->repaint();
            ui->pushButton_Screenshot->setEnabled(true);
            ui->pushButton_Screenshot->repaint();
        } else if (tryConn == false) {
            ui->plainTextEdit_console->appendPlainText("[Connexion] Impossible de se connecter au robot");
            // Désactive le bouton de connexion
            ui->pushButton_Connexion->setEnabled(true);
            ui->pushButton_Connexion->repaint();
        }


    }
}

void MainWindow::on_pushButton_Deconnexion_clicked()
{
    // Désactive le bouton de déconnexion
    ui->pushButton_Deconnexion->setEnabled(false);
    ui->pushButton_Deconnexion->repaint();
    ui->pushButton_droite->setEnabled(false);
    ui->pushButton_droite->repaint();
    ui->pushButton_gauche->setEnabled(false);
    ui->pushButton_gauche->repaint();
    ui->pushButton_avancer->setEnabled(false);
    ui->pushButton_avancer->repaint();
    ui->pushButton_reculer->setEnabled(false);
    ui->pushButton_reculer->repaint();
    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_stop->repaint();
    ui->pushButton_Screenshot->setEnabled(false);
    ui->pushButton_Screenshot->repaint();

    // Déconnexion du robot
    wifibotv3.disConnect();

    // Reset du WebEngine
    hideCamera();

    // Active le bouton de connexion
    ui->pushButton_Connexion->setEnabled(true);
    ui->pushButton_Connexion->repaint();

    ui->plainTextEdit_console->appendPlainText("[Connexion] Robot déconnecté");
}

void MainWindow::showCamera()
{
    ui->plainTextEdit_console->appendPlainText("[Caméra] Connexion à la caméra");
    QString url = "http://" + ui->lineEdit_IP->text() + ":8080/?action=stream.html";
    view->load(QUrl(url));
    view->setZoomFactor(view->zoomFactor()*1.66);
    view->show();
}

void MainWindow::hideCamera()
{
    QString deconnecte = "<html>"
            "<body style='background-color:#303030;font-family:Helvetica,sans-serif;'>"
                "<h5 style='color:#eeeeee;text-align:center;line-height:150px;'>Le robot est déconnecté</h5>"
            "</body>"
        "</html>";
    view->setHtml(deconnecte);
    view->setZoomFactor(1);
}
