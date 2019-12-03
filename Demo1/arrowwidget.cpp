#include "arrowwidget.h"
#include <QPainter>
#include <QRect>

ArrowWidget::ArrowWidget(QWidget *parent) : QWidget(parent),
    m_startX(50),
    m_triangleWidth(TRIANGLE_WIDTH),
    m_triangleHeight(TRIANGLE_HEIGHT)

{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置阴影边框;
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(SHADOW_WIDTH);
    this->setGraphicsEffect(shadowEffect);
    lb_text = new QLabel();
    QHBoxLayout* hMainLayout = new QHBoxLayout(this);
    hMainLayout->addWidget(lb_text);
    hMainLayout->setSpacing(0);
    hMainLayout->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH + TRIANGLE_HEIGHT, SHADOW_WIDTH, SHADOW_WIDTH);
}
// 设置小三角显示的起始位置;
void ArrowWidget::setStartPos(int startX)
{
    m_startX = startX;
}

void ArrowWidget::setTriangleInfo(int width, int height)
{
    m_triangleWidth = width;
    m_triangleHeight = height;
}

void ArrowWidget::setText(QString s)
{
    lb_text->setText (s);
}

void ArrowWidget::setDerection(ArrowWidget::Derection d)
{
    derect = d;
}

QString ArrowWidget::text()
{
    return lb_text->text ();
}

void ArrowWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 200));
    // 小三角区域;
    QPainterPath drawPath;
    QPolygon trianglePolygon;
    switch (derect)
    {
        case up:
        trianglePolygon << QPoint(m_startX, m_triangleHeight + SHADOW_WIDTH);
        trianglePolygon << QPoint(m_startX + m_triangleWidth / 2, SHADOW_WIDTH);
        trianglePolygon << QPoint(m_startX + m_triangleWidth, m_triangleHeight + SHADOW_WIDTH);
        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, m_triangleHeight + SHADOW_WIDTH,\
                                    width() - SHADOW_WIDTH * 2, height() - SHADOW_WIDTH * 2 - m_triangleHeight),\
                                    BORDER_RADIUS, BORDER_RADIUS);
        break;
        case left:
        trianglePolygon << QPoint(3 + m_triangleHeight, 43);
        trianglePolygon << QPoint(8, 48 + m_triangleWidth / 2);
        trianglePolygon << QPoint(3 + m_triangleHeight, 39 + m_triangleWidth);
        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, m_triangleHeight + SHADOW_WIDTH,\
                                    width() - SHADOW_WIDTH * 2, height() - SHADOW_WIDTH * 2 - m_triangleHeight),\
                                    BORDER_RADIUS, BORDER_RADIUS);
        break;
        case right:
        trianglePolygon << QPoint(115, 40);
        trianglePolygon << QPoint(116 + m_triangleHeight, 48 + m_triangleWidth / 2);
        trianglePolygon << QPoint(115, 39 + m_triangleWidth);
        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, m_triangleHeight + SHADOW_WIDTH,\
                                                                    width() - SHADOW_WIDTH * 2, height() - SHADOW_WIDTH * 2 - m_triangleHeight),\
                                    BORDER_RADIUS, BORDER_RADIUS);

        break;
        case down:
        trianglePolygon << QPoint(m_startX, 65);
        trianglePolygon << QPoint(m_startX + m_triangleWidth / 2, 65 + m_triangleHeight);
        trianglePolygon << QPoint(m_startX + m_triangleWidth, 65);
        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, m_triangleHeight + SHADOW_WIDTH,\
                                    width() - SHADOW_WIDTH * 2, height() - SHADOW_WIDTH * 2 - m_triangleHeight),\
                                    BORDER_RADIUS, BORDER_RADIUS);
        break;
    default: break;
    }
    drawPath.addPolygon(trianglePolygon);
    painter.drawPath(drawPath);
}

void ArrowWidget::mousePressEvent(QMouseEvent *)
{
    this->close ();
}
