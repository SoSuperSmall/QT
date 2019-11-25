#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
//#include <QTextCursor>
#include <QNetworkInterface>
#include <QtSerialPort/QSerialPortInfo>

static int row;       //用于显示测试信息的行号变量
static QAbstractSocket::SocketError error;
static QByteArray g_qba;                //存储网卡信息

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

/*
*网卡信息结构体
*/
struct mac
{
    char name[10];
    char ipaddr[16];
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(setipw,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString))); //连接信号函数和槽函数，为了传递变量IP
    tcpSocket = new QTcpSocket(this);                        //创建tcpSocket对象
    connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::read_data);          //将读取数据的槽函数绑定信号量，用来接收服务端发来的信息。
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
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
    int t=0;
    if(data.compare("G") == 0)
    {
        t = 1;
    }
    else if(data.compare("P") == 0)
        t = 4;
    else if(data.compare("E") == 0)
        t = 5;
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
    QByteArray msg = tcpSocket->readAll();                              //接收服务器发送过来的数据
    g_qba = msg;                //拷贝tcp接收的内容
    mac *pm = (mac*)g_qba.data();
    for(int i=0;i<6;i++)
    {
        if(strcmp(pm->name,"1") != 0)                        //判断是否有信息
        {
            qDebug() << pm->name << "--" << pm->ipaddr;                //显示网卡信息
        }
        if(i != 5)
            pm++;
    }
    //qDebug() << pm->name<<"---"<<pm->ipaddr;                       //显示一条网卡消息

    //ui->text_info->insertPlainText(QString(QString::number(row,10)+":"+msg+"\n").arg(row++));       //每次开启程序row都为0，在row行添加文字
    //ui->text_info->moveCursor(QTextCursor::End);                        //设置光标位末尾行
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
    /*ui->comboBox->clear();
    //获取mac地址和串口
    QList<QNetworkInterface> ifList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface i,ifList)
    {
        if(i.hardwareAddress()!="") //屏蔽掉空MAC地址
        {
            //ui->text_info->insertPlainText(QString(QString::number(row,10)+":"+i.hardwareAddress()+"\n").arg(row++));       //每次开启程序row都为0，在row行添加文字
            //ui->text_info->moveCursor(QTextCursor::End);
            ui->comboBox->addItem(i.hardwareAddress());         //将mac地址添加到下拉选项框中
            QList<QNetworkAddressEntry> entryList = i.addressEntries();
            foreach(QNetworkAddressEntry entry,entryList)
            {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    ip = entry.ip().toString();
                }
            }
            macList.push_back(i.hardwareAddress()+","+ip);              //将MAC地址和ip地址放入list
        }

    }
    QList<QString> comList;
    QList<QSerialPortInfo> serList = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo j,serList)
    {
        //ui->text_info->insertPlainText(QString(QString::number(row,10)+":"+j.portName()+"\n").arg(row++));       //每次开启程序row都为0，在row行添加文字
        //ui->text_info->moveCursor(QTextCursor::End);
        ui->comboBox->addItem(j.portName());
        comList.push_back(j.portName());
    }*/
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

}


/*
 * 下拉选项改变事件函数
 */
void MainWindow::on_comboBox_activated(const QString &arg1)
{
    /*ui->comboBox->blockSignals(true);
    foreach(QString str,macList)
    {
        QList<QString> strList = str.split(",");
        if(arg1 == strList[0])                 //mac地址匹配
        {
            ip = strList[1];
            ui->info_2->setText("IP地址："+strList[1]);     //显示IP地址
            //qApp->processEvents();
            break;                                         //不写break造成ip不显示
        }
        else
        {
            ui->info_2->clear();        //清空显示数据
            ip = arg1;
        }

    }
    ui->comboBox->blockSignals(false);*/
}
