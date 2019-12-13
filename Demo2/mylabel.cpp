
#include "mylabel.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}
/**
 * @brief myLabel::enterEvent
 * @param e
 * 自定义label的鼠标移入事件
 */
void myLabel::enterEvent(QEvent *e)
{
    aw = new ArrowWidget(mw);                 //创建自定义Widget对象
    QPoint loa = this->pos();                   //获取当前mylabel的坐标
    aw->setDerection(ArrowWidget::left);        //设置气泡朝向左边
    aw->setGeometry(loa.x(),loa.y(),130,80);     //设置气泡显示的位置
    aw->setStartPos(60);                          //起始位置
    aw->setTriangleInfo(20,12);
    aw->setText("Not Found");
    for(int i=0;i<shareList.length();i+=2)
    {
        if(shareList[i].compare(this->text()) == 0)
            aw->setText(shareList[i+1]);
    }
    aw->show();
}
/**
 * @brief myLabel::leaveEvent
 * @param e
 * 鼠标移出事件
 */
void myLabel::leaveEvent(QEvent *e)
{
    aw->close();
}

//Label点击事件
void myLabel::mousePressEvent(QMouseEvent *event)
{
    /*if(count % 2 == 0)
    {
        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect;
        shadow_effect->setOffset(0,0);
        shadow_effect->setColor(QColor(0,0,255));
        shadow_effect->setBlurRadius(10);
        this->setGraphicsEffect(shadow_effect);
    }
    else {
        QGraphicsOpacityEffect *gra = new QGraphicsOpacityEffect;
        gra->setOpacity(1);
        this->setGraphicsEffect(gra);
    }
    count++;*/
    emit mousePress(this->objectName());
}


