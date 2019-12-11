#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include "mainwindow.h"

#include "arrowwidget.h"

class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget *parent = nullptr);

private:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *event);
    ArrowWidget *aw;
    MainWindow *mw = (MainWindow*)parentWidget();
private slots:

signals:
    void mousePress(QString);

public slots:
};

#endif // MYLABEL_H
