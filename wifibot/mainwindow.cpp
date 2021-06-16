#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
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

    //Connect Signal
    connect(&wifibotv3, SIGNAL(updateUI(QByteArray)), this, SLOT(update_Robot_Informations()));
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

        qDebug() << tryConn;

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


// // // // // // // // // // //
// RECUPERATION INFOS CAPTEURS//
// // // // // // // // // // //

void MainWindow::update_Robot_Informations(){

    //on stocke l'ancienne valeur d'odométrie
    wifibotv3.odometryGPrevious=wifibotv3.odometryGNow;
    wifibotv3.odometryDPrevious=wifibotv3.odometryDNow;

//RECUPERATION DES VALEURS DU SOCKET

    //Niveau de Batterie (et conversion)
    wifibotv3.batteryLvl= ((unsigned char) wifibotv3.DataReceived[2])*100/185;//Le 100/185 sert à ramener la batterie entre 0 et 100

    //Capteurs IR
      wifibotv3.IRG_AV= (unsigned char) wifibotv3.DataReceived[3];
      wifibotv3.IRG_AR= (unsigned char) wifibotv3.DataReceived[4];
      wifibotv3.IRD_AV= (unsigned char) wifibotv3.DataReceived[11];
      wifibotv3.IRD_AR= (unsigned char) wifibotv3.DataReceived[12];

    //Récupération de l'odométrie sur 4 octets puis somme (on prend des double car un unsigned char , sur 24 bits, est trop court)
    double odometryG1= (unsigned char) wifibotv3.DataReceived[5];
    double odometryG2= ((unsigned char) wifibotv3.DataReceived[6]<<8);
    double odometryG3= ((unsigned char) wifibotv3.DataReceived[7]<<16);
    double odometryG4= ((unsigned char) wifibotv3.DataReceived[8]<<24);

    double odometryD1= (unsigned char) wifibotv3.DataReceived[13];
    double odometryD2= ((unsigned char) wifibotv3.DataReceived[14]<<8);
    double odometryD3= ((unsigned char) wifibotv3.DataReceived[15]<<16);
    double odometryD4= ((unsigned char) wifibotv3.DataReceived[16]<<24);


//CONVERSION DES VALEURS RECUPEREES
    //on stocke la nouvelle valeur d'odométrie
    wifibotv3.odometryGNow= (odometryG1+ odometryG2+ odometryG3+ odometryG4)/2248;
    wifibotv3.odometryDNow= (odometryD1+ odometryD2+ odometryD3+ odometryD4)/2248;

    //Conversion odométrie en vitesse : m/s
    wifibotv3.SpeedG= abs(((wifibotv3.odometryGNow-wifibotv3.odometryGPrevious)/75)*(10)*(2*3.14*14)); //delta odométrie divisé par temps entre deux mesures consécutives multiplié par rpérimètre de la roue ==> vitesse de la roue par rapport au sol
    wifibotv3.SpeedD= abs(((wifibotv3.odometryDNow-wifibotv3.odometryDPrevious)/75)*(10)*(2*3.14*14));
    wifibotv3.mediumSpeed=abs((wifibotv3.SpeedG+wifibotv3.SpeedD)/2);


    //Conversion des valeurs des capteurs en distance de 10 à 130 cm
    unsigned char DistG_AV=10+((255-wifibotv3.IRG_AV)*130/255);
    unsigned char DistG_AR=10+((255-wifibotv3.IRG_AR)*130/255);
    unsigned char DistD_AV=10+((255-wifibotv3.IRD_AV)*130/255);
    unsigned char DistD_AR=10+((255-wifibotv3.IRD_AR)*130/255);

    qDebug()<< wifibotv3.batteryLvl;
    ui->lcdNumber_Batterie->setProperty("value",QVariant((QVariant) ((wifibotv3.batteryLvl))));
}

void MainWindow::on_pushButton_stop_clicked()
{
    update_Robot_Informations();
}

