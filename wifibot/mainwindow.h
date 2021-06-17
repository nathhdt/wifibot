#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "robot.h"

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void insertConsole(QString text);
    void showCamera();
    void hideCamera();
    Ui::MainWindow *ui;
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_pushButton_Connexion_clicked();
    void on_pushButton_Deconnexion_clicked();
    void on_pushButton_Screenshot_clicked();
    void on_pushButton_avancer_pressed();
    void on_pushButton_avancer_released();
    void on_pushButton_reculer_pressed();
    void on_pushButton_reculer_released();
    void on_pushButton_gauche_pressed();
    void on_pushButton_gauche_released();
    void on_pushButton_droite_pressed();
    void on_pushButton_droite_released();
    void update_Robot_Informations();

    void on_pushButton_Camera_Bas_clicked();
    void on_pushButton_Camera_Haut_clicked();
    void on_pushButton_Camera_Gauche_clicked();
    void on_pushButton_Camera_Droite_clicked();

private:
    Robot wifibotv3;
    QWebEngineView *view;

};
#endif // MAINWINDOW_H
