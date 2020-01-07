#include "paramter.h"
#include "ui_paramter.h"

#include "mainwindow.h"

paramter::paramter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::paramter)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    this->setWindowTitle("协议-属性");

    ui->tableView->resize(ui->tabWidget->geometry().width(),ui->tabWidget->geometry().height());

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ui->lineEdit_ip->setValidator(new QRegExpValidator(rx,this));

    sp1 = new QSpinBox;
    sp2 = new QSpinBox;
    sp3 = new QSpinBox;
    sp4 = new QSpinBox;
    sp5 = new QSpinBox;
    sp6 = new QSpinBox;
    sp7 = new QSpinBox;         //定义插入表格需要的spinbox

    line1 = new QLineEdit;
    line2 = new QLineEdit;
    line3 = new QLineEdit;
    line4 = new QLineEdit;
    line5 = new QLineEdit;
    line6 = new QLineEdit;
    line7 = new QLineEdit;           //定义插入表格需要的LineEdit以进行编辑

    QRegExp regExp("[a-fA-F0-9]{4}");
    line1->setValidator(new QRegExpValidator(regExp,this));
    line2->setValidator(new QRegExpValidator(regExp,this));
    line3->setValidator(new QRegExpValidator(regExp,this));
    line4->setValidator(new QRegExpValidator(regExp,this));
    line5->setValidator(new QRegExpValidator(regExp,this));
    line6->setValidator(new QRegExpValidator(regExp,this));
    line7->setValidator(new QRegExpValidator(regExp,this));

    line1->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line2->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line3->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line4->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line5->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line6->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    line7->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");


    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(),this->height());                 //设置窗口不能调整大小和最大化

    QList<QString> tempList;
    tempList = g_stestmsg.split(" ");
    switch (tempList[0].toInt())
    {
        case 02:
        {
            ui->lineEdit->setText("MODBUS Slvae协议");
            break;
        }
        //添加协议类型显示
    }


    ui->tabWidget->setTabText(0,"请求帧");
    ui->tabWidget->setTabText(1,"响应帧");

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);          //设置表头填满
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);          //设置表头均分宽度



    m_model = new QStandardItemModel;
    strHeader<<QString::fromUtf8("序号")<<QString::fromUtf8("名称")<<QString::fromUtf8("数据长度(位)")<<QString::fromUtf8("数据")<<QString::fromUtf8("属性");
    m_model->setHorizontalHeaderLabels(strHeader);
    ui->tableView->setModel(m_model);                          //先设置model再用setIndexWidget添加组件

    ui->tableView->setColumnWidth(0,30);
    ui->tableView->setColumnWidth(1,90);
    ui->tableView->setColumnWidth(2,80);
    ui->tableView->setColumnWidth(3,80);                   //设置各列列宽

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);        //设置列宽不可拖动


    switch (tempList[1].toInt())
    {
        case 1:
        {
            ui->lineEdit_2->setText("(0x01)读线圈");
            break;
        }
        case 2:
        {
            ui->lineEdit_2->setText("(0x02)读离散输入");
            break;
        }
        case 3:
        {
            ui->lineEdit_2->setText("(0x03)读保持寄存器");
            break;
        }
        case 4:
        {
            ui->lineEdit_2->setText("(0x04)读输入寄存器");
            break;
        }
        case 5:
        {
            ui->lineEdit_2->setText("(0x05)写单个线圈");
            break;
        }
        case 6:
        {
            ui->lineEdit_2->setText("(0x06)写单个寄存器");
            break;
        }
        case 15:
        {
            ui->lineEdit_2->setText("(0x0F)写多个线圈");
            break;
        }
        case 16:
        {
            ui->lineEdit_2->setText("(0x10)写多个寄存器");
            break;
        }
        case 20:
        {
            ui->lineEdit_2->setText("(0x14)读文件记录");
            break;
        }
        case 21:
        {
            ui->lineEdit_2->setText("(0x15)写文件记录");
            break;
        }
        case 22:
        {
            ui->lineEdit_2->setText("(0x16)屏蔽写寄存器");
            break;
        }
        default:
        break;
    }
    for(int i=0;i<7;i++)
    {
        m_model->setItem(i,0,new QStandardItem(QString::number(i+1)));
        m_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        switch (i)
        {
            case 0:
            {
                m_model->setItem(i,1,new QStandardItem("事务处理标识符"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp1);           //插入spinbox
                ui->tableView->setIndexWidget(m_model->index(i,3),line1);
                break;
            }
            case 1:
            {
                m_model->setItem(i,1,new QStandardItem("协议标识符"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp2);
                break;
            }
            case 2:
            {
                m_model->setItem(i,1,new QStandardItem("长度"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp3);
                break;
            }
            case 3:
            {
                m_model->setItem(i,1,new QStandardItem("单元识别符"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp4);
                break;
            }
            case 4:
            {
                m_model->setItem(i,1,new QStandardItem("功能码"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp5);
                break;
            }
            case 5:
            {
                m_model->setItem(i,1,new QStandardItem("起始地址"));               
                ui->tableView->setIndexWidget(m_model->index(i,2),sp6);
                break;
            }
            case 6:
            {
                m_model->setItem(i,1,new QStandardItem("长度/数量"));
                ui->tableView->setIndexWidget(m_model->index(i,2),sp7);
                break;
            }
        default:
            break;
        }
    }



    _headView = ui->tableView->horizontalHeader();


    ui->tableView->setShowGrid(false);           //去掉网格线
    ui->tableView->verticalHeader()->hide();         //取消默认行号显示
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);            //选中整行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);              //设置不可编辑

}

paramter::~paramter()
{
    delete ui;
}

void paramter::on_pushButton_clicked()
{
    this->close();
}
