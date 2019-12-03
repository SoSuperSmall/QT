#ifndef SETIP_H
#define SETIP_H

#include <QDialog>

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

signals:
    void sendData(QString);

private:
    Ui::setIP *ui;

};

extern QString testip,testport;

#endif // SETIP_H
