#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QRegExpValidator>
#include <QKeyEvent>
#include <algorithm>
#include <QtGui>


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
            ui->pushButton_Screenshot->setEnabled(true);
            ui->pushButton_Screenshot->repaint();
            ui->pushButton_Camera_Haut->setEnabled(true);
            ui->pushButton_Camera_Haut->repaint();
            ui->pushButton_Camera_Bas->setEnabled(true);
            ui->pushButton_Camera_Bas->repaint();
            ui->pushButton_Camera_Droite->setEnabled(true);
            ui->pushButton_Camera_Droite->repaint();
            ui->pushButton_Camera_Gauche->setEnabled(true);
            ui->pushButton_Camera_Gauche->repaint();
            ui->horizontalSlider_Vitesse->setEnabled(true);
            ui->horizontalSlider_Vitesse->repaint();
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
    ui->pushButton_Screenshot->setEnabled(false);
    ui->pushButton_Screenshot->repaint();
    ui->pushButton_Camera_Haut->setEnabled(false);
    ui->pushButton_Camera_Haut->repaint();
    ui->pushButton_Camera_Bas->setEnabled(false);
    ui->pushButton_Camera_Bas->repaint();
    ui->pushButton_Camera_Droite->setEnabled(false);
    ui->pushButton_Camera_Droite->repaint();
    ui->pushButton_Camera_Gauche->setEnabled(false);
    ui->pushButton_Camera_Gauche->repaint();
    ui->horizontalSlider_Vitesse->setEnabled(false);
    ui->horizontalSlider_Vitesse->repaint();

    ui->lcdNumber_Batterie->setProperty("value",QVariant((QVariant) 0));
    ui->lcdNumber_KMH->setProperty("value",QVariant((QVariant) 0));
    ui->lcdNumber_InfrarougeAR->setProperty("value",QVariant((QVariant) 0));
    ui->lcdNumber_InfrarougeAV->setProperty("value",QVariant((QVariant) 0));

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

void MainWindow::on_pushButton_Screenshot_clicked()
{
    QSize size(512,385);
    QImage img(size, QImage::Format_ARGB32);
    QPainter paintView(&img);
    view->render(&paintView);
    paintView.end();

    QString imgFileName = QDir::homePath() + "/Desktop/Wifibot_" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh'h'mm'm'ss's'") + ".jpg";
    img.save(imgFileName);

    ui->plainTextEdit_console->appendPlainText("[Caméra] Capture d'écran enregistrée sous " + imgFileName);
}

void MainWindow::on_pushButton_avancer_pressed()
{
    ui->plainTextEdit_console->appendPlainText("[Direction] Avant (Z)");
    wifibotv3.Avant(ui->horizontalSlider_Vitesse->value(), ui->horizontalSlider_Vitesse->value());
}

void MainWindow::on_pushButton_avancer_released()
{
    wifibotv3.Avant(0, 0);
}

void MainWindow::on_pushButton_reculer_pressed()
{
    ui->plainTextEdit_console->appendPlainText("[Direction] Arrière (S)");
    wifibotv3.Arriere(ui->horizontalSlider_Vitesse->value(), ui->horizontalSlider_Vitesse->value());
}

void MainWindow::on_pushButton_reculer_released()
{
    wifibotv3.Arriere(0, 0);
}

void MainWindow::on_pushButton_gauche_pressed()
{
    ui->plainTextEdit_console->appendPlainText("[Direction] Gauche (Q)");
    wifibotv3.Gauche(ui->horizontalSlider_Vitesse->value(), ui->horizontalSlider_Vitesse->value());
}

void MainWindow::on_pushButton_gauche_released()
{
    wifibotv3.Gauche(0, 0);
}

void MainWindow::on_pushButton_droite_pressed()
{
    ui->plainTextEdit_console->appendPlainText("[Direction] Droite (Q)");
    wifibotv3.Droite(ui->horizontalSlider_Vitesse->value(), ui->horizontalSlider_Vitesse->value());
}

void MainWindow::on_pushButton_droite_released()
{
    wifibotv3.Droite(0, 0);
}

// // // // // // // // // // //
// RECUPERATION INFOS CAPTEURS//
// // // // // // // // // // //

void MainWindow::update_Robot_Informations(){

    // On stocke l'ancienne valeur d'odométrie
    wifibotv3.setOdometryGPrevious(wifibotv3.getOdometryGNow());
    wifibotv3.setOdometryDPrevious(wifibotv3.getOdometryDNow());

// Récupération des valeurs depuis le socket

    // Niveau de Batterie (et conversion)
    wifibotv3.setBatteryLevel(((unsigned char) wifibotv3.DataReceived[2])*100/185);//Le 100/185 sert à ramener la batterie entre 0 et 100

    // Capteurs IR
      wifibotv3.setIRG_AV((unsigned char) wifibotv3.DataReceived[3]);
      wifibotv3.setIRG_AR((unsigned char) wifibotv3.DataReceived[4]);
      wifibotv3.setIRD_AV((unsigned char) wifibotv3.DataReceived[11]);
      wifibotv3.setIRD_AR((unsigned char) wifibotv3.DataReceived[12]);

    // Récupération de l'odométrie sur 4 octets puis somme (on prend des double car un unsigned char , sur 24 bits, est trop court)
    double odometryG1= (unsigned char) wifibotv3.DataReceived[5];
    double odometryG2= ((unsigned char) wifibotv3.DataReceived[6]<<8);
    double odometryG3= ((unsigned char) wifibotv3.DataReceived[7]<<16);
    double odometryG4= ((unsigned char) wifibotv3.DataReceived[8]<<24);

    double odometryD1= (unsigned char) wifibotv3.DataReceived[13];
    double odometryD2= ((unsigned char) wifibotv3.DataReceived[14]<<8);
    double odometryD3= ((unsigned char) wifibotv3.DataReceived[15]<<16);
    double odometryD4= ((unsigned char) wifibotv3.DataReceived[16]<<24);


    // Conversion des valeurs récupérées

    // on stocke la nouvelle valeur d'odométrie
    wifibotv3.setOdometryGNow((odometryG1+ odometryG2+ odometryG3+ odometryG4)/2248);
    wifibotv3.setOdometryDNow((odometryD1+ odometryD2+ odometryD3+ odometryD4)/2248);

    // Conversion odométrie en vitesse : m/s
    wifibotv3.setSpeedG(abs(((wifibotv3.getOdometryGNow() - wifibotv3.getOdometryGPrevious())/75)*(10)*(3.14*14))); //delta odométrie divisé par temps entre deux mesures consécutives multiplié par rpérimètre de la roue ==> vitesse de la roue par rapport au sol
    wifibotv3.setSpeedD(abs(((wifibotv3.getOdometryDNow() - wifibotv3.getOdometryDPrevious())/75)*(10)*(3.14*14)));
    wifibotv3.setMediumSpeed(abs((wifibotv3.getSpeedG() + wifibotv3.getSpeedD())/2));


    // Conversion des valeurs des capteurs en distance de 10 à 130 cm
    unsigned char DistG_AV=10+((255-wifibotv3.getIRG_AV())*130/255);
    unsigned char DistG_AR=10+((255-wifibotv3.getIRG_AR())*130/255);
    unsigned char DistD_AV=10+((255-wifibotv3.getIRD_AV())*130/255);
    unsigned char DistD_AR=10+((255-wifibotv3.getIRD_AR())*130/255);

    unsigned char DistAV= std::min((int)DistG_AV, (int)DistD_AV);
    unsigned char DistAR= std::min((int)DistG_AR,(int) DistD_AR);
    // Update UI
    qDebug()<< wifibotv3.getBatteryLvl();
    ui->lcdNumber_Batterie->setProperty("value",QVariant((QVariant) ((wifibotv3.getBatteryLvl()))));
    ui->lcdNumber_KMH->setProperty("value",QVariant((QVariant) (((int)(wifibotv3.getMediumSpeed()*3.6)))));
    ui->lcdNumber_InfrarougeAR->setProperty("value",QVariant((QVariant) (((int)(DistAR)))));
    ui->lcdNumber_InfrarougeAV->setProperty("value",QVariant((QVariant) (((int)(DistAV)))));

}

void MainWindow::on_pushButton_Camera_Bas_clicked()
{
    QNetworkRequest request;
    QNetworkAccessManager* man = new QNetworkAccessManager();
    request.setUrl(QUrl("http://" + ui->lineEdit_IP->text() + ":8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"));
    man->get(request);
    man=NULL;
    free(man);
}

void MainWindow::on_pushButton_Camera_Haut_clicked()
{
    QNetworkRequest request;
    QNetworkAccessManager* man = new QNetworkAccessManager();
    request.setUrl(QUrl("http://" + ui->lineEdit_IP->text() + ":8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"));
    man->get(request);
    man=NULL;
    free(man);
}

void MainWindow::on_pushButton_Camera_Gauche_clicked()
{
    QNetworkRequest request;
    QNetworkAccessManager* man = new QNetworkAccessManager();
    request.setUrl(QUrl("http://" + ui->lineEdit_IP->text() + ":8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"));
    man->get(request);
    man=NULL;
    free(man);
}

void MainWindow::on_pushButton_Camera_Droite_clicked()
{
    QNetworkRequest request;
    QNetworkAccessManager* man = new QNetworkAccessManager();
    request.setUrl(QUrl("http://" + ui->lineEdit_IP->text() + ":8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"));
    man->get(request);
    man=NULL;
    free(man);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z)
    {
        on_pushButton_avancer_pressed();
    }

    if(event->key() == Qt::Key_Q)
    {
        on_pushButton_gauche_pressed();
    }

    if(event->key() == Qt::Key_S)
    {
        on_pushButton_reculer_pressed();
    }

    if(event->key() == Qt::Key_D)
    {
        on_pushButton_droite_pressed();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z)
    {
        on_pushButton_avancer_released();
    }

    if(event->key() == Qt::Key_Q)
    {
        on_pushButton_gauche_released();
    }

    if(event->key() == Qt::Key_S)
    {
        on_pushButton_reculer_released();
    }

    if(event->key() == Qt::Key_D)
    {
        on_pushButton_droite_released();
    }
}
