#include "robot.h"

Robot::Robot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;
    DataToSend[8] = 0x0;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
    socket = new QTcpSocket(this); // Socket creation
}

bool Robot::doConnect(QString ip, int port) {
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "Connecting...";

    socket->connectToHost(ip, (quint16)port); // Connexion au Wifibot

    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }
    TimerEnvoi->start(75);
    return true;
}

void Robot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

void Robot::connected() {
    qDebug() << "Connected!";

}

void Robot::disconnected() {
    qDebug() << "Disconnected!";
}

void Robot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void Robot::readyRead() {
    qDebug() << "Reading from socket...";
    qDebug() << "Reading from socket..."; // read the data from the socket
    DataReceived = socket->readAll();
    if (DataReceived.size()>0){
        emit updateUI(DataReceived);
        qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
    }
}

void Robot::MyTimerSlot() {
    qDebug() << "[Timer] Clock";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

// // // GETTERS // // //
QTcpSocket* Robot::getSocket(){
    return socket;
}

short Robot::Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max){
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity = 0;

    Crc = 0xFFFF;
    Polynome = 0xA001;
    for (CptOctet=0; CptOctet<Taille_Max;CptOctet++){
        Crc ^= *(_Adresse_tab + CptOctet);

        for(CptBit=0;CptBit<=7;CptBit++){
            Parity = Crc;
            Crc >>=1;
            if(Parity%2 == true) Crc ^= Polynome;
        }
    }
    return (Crc);
}


void Robot::Avant(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(80+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Arriere(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(0+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Droite(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(64+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Gauche(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(16+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Stop(){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x00;
    DataToSend[3] = 0x00;
    DataToSend[4] = 0x00;
    DataToSend[5] = 0x00;
    DataToSend[6] = 0x00;
    DataToSend[7] = 0x00;
    DataToSend[8] = 0x00;

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

double Robot::getOdometryGPrevious() {
    return odometryGPrevious;
}

double Robot::getOdometryGNow() {
    return odometryGNow;
}

double Robot::getOdometryDPrevious() {
    return odometryDPrevious;
}

double Robot::getOdometryDNow() {
    return odometryDNow;
}

float Robot::getSpeedG() {
    return SpeedG;
}

float Robot::getSpeedD() {
    return SpeedD;
}

float Robot::getMediumSpeed() {
    return mediumSpeed;
}

unsigned char Robot::getIRG_AV() {
    return IRG_AV;
}

unsigned char Robot::getIRG_AR() {
    return IRG_AR;
}

unsigned char Robot::getIRD_AV() {
    return IRD_AV;
}

unsigned char Robot::getIRD_AR() {
    return IRD_AR;
}

unsigned char Robot::getBatteryLvl() {
    return batteryLvl;
}

void Robot::setOdometryGPrevious(double _odometryGPrevious) {
    odometryGPrevious = _odometryGPrevious;
}

void Robot::setOdometryGNow(double _odometryGNow) {
    odometryGNow = _odometryGNow;
}

void Robot::setOdometryDPrevious(double _odometryDPrevious) {
odometryDPrevious = _odometryDPrevious;
}

void Robot::setOdometryDNow(double _odometryDNow) {
 odometryDNow = _odometryDNow;
}

void Robot::setSpeedG(float _speedG) {
    SpeedG = _speedG;
}

void Robot::setSpeedD(float _speedD) {
    SpeedD = _speedD;
}

void Robot::setMediumSpeed(float _mediumSpeed) {
    mediumSpeed = _mediumSpeed;
}

void Robot::setIRG_AV(unsigned char _IRG_AV) {
    IRG_AV = _IRG_AV;
}

void Robot::setIRG_AR(unsigned char _IRG_AR) {
    IRG_AR = _IRG_AR;
}

void Robot::setIRD_AV(unsigned char _IRD_AV) {
    IRD_AV = _IRD_AV;
}

void Robot::setIRD_AR(unsigned char _IRD_AR) {
    IRD_AR = _IRD_AR;
}

void Robot::setBatteryLevel(unsigned char _batteryLevel) {
    batteryLvl = _batteryLevel;
}
