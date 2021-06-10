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
    int _speed;

signals:
    void updateUI(const QByteArray Data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();
    void Avant(short speed1, short speed2);
    void Arriere(short speed1, short speed2);
    void Gauche(short speed1, short speed2);
    void Droite(short speed1, short speed2);
    void Stop();
    short Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max);
    bool slowSpeedChange(int endValue, float pas);

private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
};

#endif // ROBOT_H
