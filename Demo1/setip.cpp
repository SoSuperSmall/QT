#include "setip.h"
#include "ui_setip.h"
#include <QDebug>

QString testip;
QString testport;

setIP::setIP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setIP)
{
    ui->setupUi(this);
}


setIP::~setIP()
{
    delete ui;
}

/*
*点击确定按钮
*/
void setIP::on_btn_ok_clicked()
{
    QString ip = ui->test_ip->text();
    testip = ip;
    QString port = ui->test_port->text();
    testport = port;
    //emit sendData(ui->test_ip->text());
    emit sendData("IP: "+ip+"\n"+"port: "+port+"\n未连接");
    this->close();
    //qDebug()<<QString(ip);
}


/*
*点击取消按钮
*/
void setIP::on_btn_cancle_clicked()
{
    this->close();
}
