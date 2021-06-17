#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>


class Robot : public QObject {
    Q_OBJECT
public:

    explicit Robot(QObject *parent = 0);
    bool doConnect(QString ip, int port);
    void disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;// [2]->batteryLvl /;/ [3]->IRG_AV; [4]->IRG_AR; [11]->IRD_AV; [12]IRD_AR /;/ [8][7][6][5]->odométrie => is a float
    QMutex Mutex;

    double getOdometryGPrevious();
    double getOdometryGNow();
    double getOdometryDPrevious();
    double getOdometryDNow();
    float getSpeedG();
    float getSpeedD();
    float getMediumSpeed();
    unsigned char getIRG_AV();
    unsigned char getIRG_AR();
    unsigned char getIRD_AV();
    unsigned char getIRD_AR();
    unsigned char getBatteryLvl();

    void setOdometryGPrevious(double _odometryGPrevious);
    void setOdometryGNow(double _odometryGNow);
    void setOdometryDPrevious(double _odometryDPrevious);
    void setOdometryDNow(double _odometryDNow);
    void setSpeedG(float _speedG);
    void setSpeedD(float _speedD);
    void setMediumSpeed(float _mediumSpeed);
    void setIRG_AV(unsigned char _IRG_AV);
    void setIRG_AR(unsigned char _IRG_AR);
    void setIRD_AV(unsigned char _IRD_AV);
    void setIRD_AR(unsigned char _IRD_AR);
    void setBatteryLevel(unsigned char _batteryLevel);

signals:
    void updateUI(const QByteArray Data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();
    QTcpSocket* getSocket();

    void Avant(short speed1, short speed2);
    void Arriere(short speed1, short speed2);
    void Gauche(short speed1, short speed2);
    void Droite(short speed1, short speed2);
    void Stop();
    short Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max);

private:
    //Variables des capteurs
    //Odométrie
    double odometryGPrevious=0;
    double odometryGNow=0;
    double odometryDPrevious=0;
    double odometryDNow=0;
    //Vitesses en m/s
    float SpeedG=0;
    float SpeedD=0;
    float mediumSpeed=0;
    //Batterie
    unsigned char batteryLvl=0;
    //IR
    unsigned char IRG_AV= 0;
    unsigned char IRG_AR= 0;
    unsigned char IRD_AV= 0;
    unsigned char IRD_AR= 0;
    //FIN
    QTcpSocket *socket;
    QTimer *TimerEnvoi;

};

#endif // ROBOT_H
