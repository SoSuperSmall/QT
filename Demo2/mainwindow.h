#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QListWidgetItem>
#include <QPalette>
#include <QFont>
#include "setip.h"

class QTcpSocket;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    setIP *setipw = new setIP;          //定义全局变量setipw，用于打开新窗口和绑定信号函数与槽函数
    ~MainWindow();

private:
    QString g_sip;
    QTcpSocket *tcpSocket;
    void connectServer();
    void sendDatatoServer(QString data);
    void addItem_2();
    short getType(QString data);
    QString g_stestmsg;
    void InitArray();

signals:


private slots:
    void on_actionConnect_triggered();
    void on_btn_ok_clicked();
    void read_data();
    void displayError(QAbstractSocket::SocketError);
    void respose(QString Objectname);

private slots:
    void receiveData(QString data);

    void on_btn_Connect_clicked();

    void on_actionDisconnect_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_btn_loadtest_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_btn_starttest_clicked();

    void on_btn_endtest_clicked();

    void on_btn_pausetest_clicked();

private:
    Ui::MainWindow *ui;
};
extern QList<QString> shareList;



#endif // MAINWINDOW_H
