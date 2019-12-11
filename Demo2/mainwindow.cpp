#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
//#include <QTextCursor>
#include <QNetworkInterface>
#include <QtSerialPort/QSerialPortInfo>

#include "mylabel.h"

static int row;       //用于显示测试信息的行号变量
static QAbstractSocket::SocketError error;
static QByteArray g_qba;                //存储网卡信息
QList<QString> shareList;              //全局变量，存储网卡信息，气泡显示使用
static int count = 0;                   //点击label次数
QList<QString> nameList;                //存储网卡状态与label名

/*
*消息结构体
*/
struct header
{
    short type;
    char ip[16];
    short length;
    char tdata[0];
};

//定义接收结构体
struct rcvhead
{
    short type;
    short length;
    char rdata[1024];
};

/*
*网卡信息结构体
*/
struct mac
{
    char name[10];
    char ipaddr[16];
    short status;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(),this->height());

    ui->c1->setAutoFillBackground(true);
    ui->c2->setAutoFillBackground(true);
    ui->c3->setAutoFillBackground(true);
    ui->c4->setAutoFillBackground(true);
    ui->c5->setAutoFillBackground(true);
    ui->c6->setAutoFillBackground(true);

    ui->c1->setStyleSheet("QLabel{background:rgb(0,0,0);}");
    ui->c2->setStyleSheet("QLabel{background:rgb(0,0,0);}");
    ui->c3->setStyleSheet("QLabel{background:rgb(0,0,0);}");
    ui->c4->setStyleSheet("QLabel{background:rgb(0,0,0);}");
    ui->c5->setStyleSheet("QLabel{background:rgb(0,0,0);}");
    ui->c6->setStyleSheet("QLabel{background:rgb(0,0,0);}");

    ui->l1->setAutoFillBackground(true);
    ui->l2->setAutoFillBackground(true);
    ui->l3->setAutoFillBackground(true);
    ui->l4->setAutoFillBackground(true);
    ui->l5->setAutoFillBackground(true);
    ui->l6->setAutoFillBackground(true);

    ui->l1->setStyleSheet("QLabel{background:rgb(179,169,178);}");
    ui->l2->setStyleSheet("QLabel{background:rgb(179,169,178);}");
    ui->l3->setStyleSheet("QLabel{background:rgb(179,169,178);}");
    ui->l4->setStyleSheet("QLabel{background:rgb(179,169,178);}");
    ui->l5->setStyleSheet("QLabel{background:rgb(179,169,178);}");
    ui->l6->setStyleSheet("QLabel{background:rgb(179,169,178);}");

    QFont ft;
    ft.setPointSize(12);
    ft.setWeight(50);

    ui->l1->setWordWrap(true);
    ui->l2->setWordWrap(true);
    ui->l3->setWordWrap(true);
    ui->l4->setWordWrap(true);
    ui->l5->setWordWrap(true);
    ui->l6->setWordWrap(true);

    ui->l1->setText("未检测到网卡");
    ui->l2->setText("未检测到网卡");
    ui->l3->setText("未检测到网卡");
    ui->l4->setText("未检测到网卡");
    ui->l5->setText("未检测到网卡");
    ui->l6->setText("未检测到网卡");

    ui->l1->setAlignment(Qt::AlignCenter);
    ui->l2->setAlignment(Qt::AlignCenter);
    ui->l3->setAlignment(Qt::AlignCenter);
    ui->l4->setAlignment(Qt::AlignCenter);
    ui->l5->setAlignment(Qt::AlignCenter);
    ui->l6->setAlignment(Qt::AlignCenter);



    connect(setipw,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString))); //连接信号函数和槽函数，为了传递变量IP
    tcpSocket = new QTcpSocket(this);                        //创建tcpSocket对象
    connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::read_data);          //将读取数据的槽函数绑定信号量，用来接收服务端发来的信息。
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(ui->l1,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    connect(ui->l2,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    connect(ui->l3,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    connect(ui->l4,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    connect(ui->l5,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    connect(ui->l6,SIGNAL(mousePress(QString)),this,SLOT(respose(QString)));                //连接label点击事件信号与处理槽函数
    ui->tabWidget->setCurrentIndex(0);                                  //初始显示测试协议类型选择一行
    InitArray();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
*向QbyteArray写入多个结构体
*/
void MainWindow::InitArray()
{
    g_qba.resize(6*sizeof(struct mac));

    //初始化
    for(int i = 0;i<6;i++)
    {
        mac stumac1;
        stumac1.status = 4;
        strcpy(stumac1.name,"1");
        strcpy(stumac1.ipaddr,"1");
        memcpy(g_qba.data()+i*sizeof(mac),&stumac1,sizeof(mac));            //移动指针，写入多个数据
    }
}


/*
*根据数据获取类型
*/
short MainWindow::getType(QString data)
{
    if(data.compare("G") == 0)
    {
        return 1;
    }
    else if(data.compare("P") == 0)
        return 4;
    else if(data.compare("E") == 0)
        return 5;
    else if(data.compare("C") == 0)
    {
        return 3;
    }
    else {
        return 2;
    }
}

/*
*打开输入输入ip与端口窗体
*/
void MainWindow::on_actionConnect_triggered()
{
   //setipw = new setIP;
   setipw->show();       //打开设置ip与端口的窗口
}

/*
 * 接收setip（设置要连接服务器ip和端口号）窗口中填写的内容
 */
void MainWindow::receiveData(QString data)
{
    ui->info_1->setText(data);      //显示用户输入的IP和端口号
}

/*
 * 连接服务器函数
 */
void MainWindow::connectServer()
{  
    tcpSocket->abort();                 //取消所有连接
    tcpSocket->connectToHost(testip,testport.toInt());    //根据ip和端口连接服务端
    //qDebug() <<QString(testip);
    if(!tcpSocket->waitForConnected(30000))            //如果连接等待时间过长,显示失败
    {
        ui->info_1->setText("IP: "+testip+"\nport: "+testport+"\n"+"连接错误，请重试");
        return;
    }
    else
    {
        ui->info_1->setText("IP: "+testip+"\nport: "+testport+"\n已连接");
    }
}

/*
 * 读取服务器发来的数据写到textedit，暂时只接收协议栈的返回数据
 */
void MainWindow::read_data()
{
    QFont font;
    QByteArray msg = tcpSocket->readAll();                              //接收服务器发送过来的数据
    rcvhead *rh = (rcvhead *)msg.data();
    //qDebug() << rh->type;
    if(rh->type == 1)                //消息类型1，网卡消息
    {
        memcpy(g_qba.data(),rh->rdata,rh->length);                //拷贝tcp接收的内容
        mac *pm = (mac*)g_qba.data();
        for(int i=1;i<7;i++)
        {
            if(strcmp(pm->name,"1") != 0)                        //判断是否有信息
            {
                QLabel *label = findChild<QLabel*>("l" + QString::number(i));
                QLabel *count = findChild<QLabel*>("c" + QString::number(i));
                if(label != NULL)
                {
                    font.setPointSize(12);
                    font.setWeight(60);
                    label->setText(pm->name);
                    label->setFont(font);
                    if(pm->status == 1)
                    {
                        label->setStyleSheet("QLabel{background:rgb(46,117,181);}");
                        count->setStyleSheet("QLabel{background:rgb(112,173,71);}");
                    }
                    else if(pm->status == 2)
                    {
                        label->setStyleSheet("QLabel{background:rgb(46,117,181);}");
                        count->setStyleSheet("QLabel{background:rgb(184,20,21);}");
                    }
                    else if(pm->status == 3)
                    {
                        label->setStyleSheet("QLabel{background:rgb(255,165,0);}");
                        count->setStyleSheet("QLabel{background:rgb(184,20,21);}");
                    }

                    shareList.append(pm->name);
                    shareList.append(pm->ipaddr);
                    nameList.append(label->objectName());
                    nameList.append(QString::number(pm->status));                 //将label名字与网卡状态对应
                }
                //qDebug() << pm->name << "--" << pm->ipaddr;                //显示网卡信息
            }
            if(i != 5)
                pm++;
        }
    }
    else if(rh->type == 2)            //消息类型2，协议栈消息
    {
        QByteArray rcv;
        rcv.resize(rh->length);
        memcpy(rcv.data(),rh->rdata,rh->length);
        ui->text_info->insertPlainText(QString(QString::number(row,10)+":"+rcv+"\n").arg(row++));       //每次开启程序row都为0，在row行添加文字
        ui->text_info->moveCursor(QTextCursor::End);                        //设置光标位末尾行
    }

}

/*
 * tcp连接错误显示
 */
void MainWindow::displayError(QAbstractSocket::SocketError)
{
   ui->info_1->clear();                     //将显示内容清空
   error = tcpSocket->error();               //获取错误信息
   switch (error) {                        //判断错误信息
   case QAbstractSocket::RemoteHostClosedError:
   {
       QString hostAddress = tcpSocket->QAbstractSocket::peerAddress().toString();
       ui->info_1->insertPlainText(tr("[%1]已断开连接\r\n").arg(hostAddress));
       break;
   }
   default:
   {
       QMessageBox::information(this,"show",tcpSocket->errorString());
       break;
   }
   }
}

/*
 * 向服务器发送数据，只发送了一个结构体
 */
void MainWindow::sendDatatoServer(QString data)
{
    g_sip = tcpSocket->QAbstractSocket::localAddress().toString();
    struct header head;          //初始化结构体
    QByteArray strdata;
    QByteArray strbyte = g_sip.toLatin1();
    QByteArray ba = data.toLatin1();         //将转换编码

    head.length = data.length();            //获取数据长度
    struct header *h = (struct header *)malloc(sizeof(struct header)+head.length);                       //为结构体分配内存

    h->type = getType(data);
    strcpy(h->ip,strbyte.data());
    h->length = head.length;
    strcpy(h->tdata,ba.data());               //结构体内容赋值

    //qDebug() << ("%s",head.tdata);                   //写入结构体能正确输出
    //qDebug() << ("%s\n",head.ip);


    strdata.resize((sizeof(struct header)+head.length));
    memcpy(strdata.data(),h,(sizeof(struct header)+head.length));
    tcpSocket->write(strdata);
    tcpSocket->flush();     //通过tcp向服务器发送消息

}

/*
 * 点击测试执行按钮进行发送结构体
 */
void MainWindow::on_btn_Connect_clicked()
{
    connectServer();              //点击连接测试仪按钮建立tcp连接

}

/*
 * 断开tcp连接
 */
void MainWindow::on_actionDisconnect_triggered()
{
    tcpSocket->disconnectFromHost();           //断开与服务器的连接
    ui->info_1->setText("已断开连接");           //信息框显示断开信息
}

/*
 * 点击按键获取网卡列表
 */
void MainWindow::on_btn_ok_clicked()           //点击确定按钮将信息发给服务端
{
    sendDatatoServer("G");

}

/*
* 添加对应测试项
*/
void MainWindow::addItem_2()
{
    //第二个Listwidget添加测试项
    QListWidgetItem* it1 = new QListWidgetItem("功能码 03");
    QListWidgetItem* it2 = new QListWidgetItem("功能码 04");
    QListWidgetItem* it3 = new QListWidgetItem("功能码 16");
    ui->listWidget_2->addItem(it1);
    ui->listWidget_2->addItem(it2);
    ui->listWidget_2->addItem(it3);
}

/**
 * @brief MainWindow::on_listWidget_itemClicked
 * @param item
 * 协议类型选择函数
 */
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->listWidget->blockSignals(true);    //关闭信号
    g_stestmsg = "";                           //将要传送的数据清空
    QString val = item->text();                //获取选中item的值
    QStringList list2 = val.split(" ");        //以空格分隔获取的Item的值，获取协议名前的数字
    //qDebug() << QString(list2[0].mid(1,2)+" ");
    g_stestmsg = list2[0].mid(1,2)+" ";            //将两位的数字赋值给要传输的信息
    ui->listWidget_2->clear();                  //清空listwidget的所有Item显示
    ui->typeInfo->clear();                     //清空文本浏览框显示
    switch (list2[0].mid(1,2).toInt())         //1-5分别表示协议类型，暂时只有modbus master类型
    {
    case 1:
        break;
    case 2:
        addItem_2();
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
    int index = ui->tabWidget->currentIndex();            //获取当前tabwidget的页面index
    if(index == 0)                                         //判断是否为第一个页面
        index++;                                           //第一个页面跳转到第二页
    ui->tabWidget->setCurrentIndex(index);                //设置tabwidget显示的页面
    ui->listWidget->blockSignals(false);    //开启信号
    qDebug() << QString(g_stestmsg);
}


/*
 * 点击测试加载的函数，函数中暂时写的是获取mac和串口
 */
void MainWindow::on_btn_loadtest_clicked()
{

}

/*
 *测试项点击函数
 */
void MainWindow::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_2->blockSignals(true);
    QString fun = item->text();
    QStringList list = fun.split(" ");
    QString temp = g_stestmsg.mid(0,3);
    //qDebug() << QString(temp);
    switch (list[1].toInt())                       //根据不同测试例显示要测试的内容
    {
        case 3:
            ui->typeInfo->setText("测试功能码03，读保持寄存器");                  //协议代号+两位功能码
        break;
        case 4:
            ui->typeInfo->setText("测试功能码04，读输入寄存器");
        break;
        case 16:
            ui->typeInfo->setText("测试功能码16，写多个寄存器");
        break;
        default:
            ui->typeInfo->setText("无效测试项，请重新选择");
        break;
    }
    temp = temp + list[1];                  //将功能码添加到信息末尾
    g_stestmsg = temp;                         //将中间值替换
    qDebug() << QString(g_stestmsg);
    ui->listWidget_2->blockSignals(false);
}

/*
 *测试执行按钮点击
 */
void MainWindow::on_btn_starttest_clicked()
{
    if(g_stestmsg.length()<5)                    //检查协议类型和测试项是否选择完整
       QMessageBox::warning(NULL,"warning","请完整选择协议类型和测试项",QMessageBox::Yes);           //选择不完整弹出警告框
    else
    {
        sendDatatoServer(g_stestmsg);              //选择完整就发送给主程序
    }
}

/*
 * 测试结束函数
 */
void MainWindow::on_btn_endtest_clicked()
{
    sendDatatoServer("E");
}

/**
 * @brief MainWindow::on_btn_pausetest_clicked
 * 暂停测试
 */
void MainWindow::on_btn_pausetest_clicked()
{
    sendDatatoServer("P");
}

/**
 * @brief MainWindow::respose
 * 响应label的点击事件
 */
void MainWindow::respose(QString Objectname)
{
    qDebug() << Objectname;
    if(nameList.length() == 0)
    {
        QMessageBox::warning(NULL,"warning","插槽未检测到插卡",QMessageBox::Yes);
    }
    else {
        if(count % 2 == 0)
        {
            //遍历nameList，判断点击的lable显示的网卡状态
            for(int i=0;i<nameList.length();i++)
            {

                if(nameList[i].compare(Objectname) == 0)
                {

                    if((i+1) != nameList.length())
                    {
                        if(nameList[i+1].compare("1") == 0)
                        {

                            QMessageBox::warning(NULL,"warning","此卡已被占用",QMessageBox::Yes);
                        }
                        else if(nameList[i+1].compare("2") == 0)
                        {

                            QMessageBox::warning(NULL,"warning","此卡已被预约",QMessageBox::Yes);
                        }
                        else if(nameList[i+1].compare("3") == 0)
                        {
                            QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect;
                            shadow_effect->setOffset(0,0);
                            shadow_effect->setColor(QColor(0,0,255));
                            shadow_effect->setBlurRadius(10);
                            this->setGraphicsEffect(shadow_effect);
                            QMessageBox::warning(NULL,"OK","此卡可以被预约选择",QMessageBox::Yes);
                            count++;
                        }
                        else {
                            QMessageBox::warning(NULL,"warning","插槽未检测到插卡",QMessageBox::Yes);
                        }
                    }

                }
                else {
                    QMessageBox::warning(NULL,"warning","插槽未检测到插卡",QMessageBox::Yes);
                }
            }
        }
        else {
            int nRet = QMessageBox::question(NULL,"提示","已预约此网卡，是否取消预约",QMessageBox::Yes,QMessageBox::No);
            if(QMessageBox::Yes == nRet)       //选择是，取消选中
            {
                QGraphicsOpacityEffect *gra = new QGraphicsOpacityEffect;
                gra->setOpacity(1);
                this->setGraphicsEffect(gra);
                count++;
            }
            else {

            }

        }
    }

}
