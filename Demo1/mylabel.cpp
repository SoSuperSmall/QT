
#include "mylabel.h"
#include <QDebug>

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
    aw = new ArrowWidget(mw);
    QPoint loa = this->pos();
    aw->setDerection(ArrowWidget::left);
    aw->setGeometry(loa.x(),loa.y(),130,80);
    aw->setStartPos(60);
    aw->setTriangleInfo(20,12);
    aw->setText("Not Found");
    for(int i=0;i<shareList.length();i++)
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


