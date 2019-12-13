#include "setip.h"
#include "ui_setip.h"
#include <QDebug>
#include <QMessageBox>

QString testip;
QString testport;

setIP::setIP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setIP)
{
    ui->setupUi(this);

    ui->test_ip->setEditable(true);
    le =  ui->test_ip->lineEdit();
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");            //ip地址正则判断
    ip_Val = new QRegExpValidator(rx,this);
    le->setValidator(ip_Val);
    ui->test_ip->clear();
    ReadHistory();
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
    QString ip = le->text();
    testip = ip;
    QString port = ui->test_port->text();
    testport = port;
    //emit sendData(ui->test_ip->text());
    if(ip.compare(""))
    {
        QMessageBox::warning(this,"sdf","IP地址不能为空",QMessageBox::Yes);
    }
    else {
        emit sendData("IP: "+ip+"\n"+"port: "+port+"\n未连接");
        WriteHistory(ip);
        this->close();
    }
}


/*
*点击取消按钮
*/
void setIP::on_btn_cancle_clicked()
{
    this->close();
}

/**
 * @brief setIP::ReadHistory
 * 读取文件中存储的ip地址并显示
 */
void setIP::ReadHistory()
{
    QFile file("./DataFile/history.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file" <<endl;
        return;
    }
    QTextStream in(&file);
    //QString line = in.readLine();      //读取一行
    //QString text = in.readAll();     //读取所有
    QString line;
    do{
        line = in.readLine();
        if(!line.isNull())
        {
            ui->test_ip->addItem(line);
        }
    }while(!line.isNull());
    file.close();
}


/**
 * @brief setIP::WriteHistory
 * @param ip
 *历史输入ip保存进文件
 */
void setIP::WriteHistory(QString ip)
{
    QDir *DataFile = new QDir;
    bool exist = DataFile->exists("./DataFile");
    if(!exist)
    {
        bool isok = DataFile->mkdir("./DataFile");
        if(!isok)
            QMessageBox::warning(this,"sdf","can't mkdir",QMessageBox::Yes);
    }
    QString fileName = "./DataFile/history.txt";
    QFile file(fileName);

    if(!file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Can't open the file" <<endl;
    }
    QTextStream stream(&file);
    if(check(ip))
        stream << ip << "\n";
    file.close();
}

bool setIP::check(QString ip)
{
     QList<QString> tempList;
     QComboBox *his_ip = ui->test_ip;
     bool flag = true;
     for(int i=0 ;i < his_ip->count();i++)
     {
         QString val = his_ip->itemText(i);
         if(val.compare(ip) == 0)
         {
             flag = false;
             return flag;
         }
         else {
             flag = true;
         }
     }
     return flag;
}

