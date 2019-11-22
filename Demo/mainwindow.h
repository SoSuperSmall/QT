#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QListWidgetItem>
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
    QString ip;
    QTcpSocket *tcpSocket;
    void connectServer();
    void displayError(QAbstractSocket::SocketError);
    void sendDatatoServer(QString data);
    void addItem_2();
    struct header
    {
        short type;
        char ip[19];
        short length;
        char tdata[0];
    };
    struct header *h;
    void InitArray(header head);
    short getType(QString data);
    QString testmsg;
    void getIp();

private slots:
    void on_actionConnect_triggered();
    void on_btn_ok_clicked();
    void read_data();

private slots:
    void receiveData(QString data);

    void on_btn_Connect_clicked();

    void on_actionDisconnect_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_btn_loadtest_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_btn_starttest_clicked();

    void on_btn_endtest_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
