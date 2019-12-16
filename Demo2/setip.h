#ifndef SETIP_H
#define SETIP_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QRegExpValidator>
#include <QLineEdit>

namespace Ui {
class setIP;
}

class setIP : public QDialog
{
    Q_OBJECT

public:
    explicit setIP(QWidget *parent = nullptr);
    ~setIP();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancle_clicked();


private:
    QString ip;
    void ReadHistory();
    void WriteHistory(QString ip);
    QRegExpValidator *ip_Val;
    QLineEdit *le;
    bool check(QString ip);
    bool isIpAddr(QString ip);

signals:
    void sendData(QString);

private:
    Ui::setIP *ui;

};

extern QString testip,testport;

#endif // SETIP_H
