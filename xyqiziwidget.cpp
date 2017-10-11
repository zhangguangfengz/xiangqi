﻿#include "xyqiziwidget.h"
#include "xyqipanwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>
#include <QDebug>

XYQiziWidget::XYQiziWidget(TYPE type, int times, QWidget *parent)
    : QWidget(parent)
{
    setType(type, times);
    setVisible(false);
}

XYQiziWidget::~XYQiziWidget()
{

}

QPoint XYQiziWidget::getQiziDefaultPos(bool up)
{
    int row = 0;
    int column = 0;
    switch (type)
    {
    case HONG_ZU:
    case HEI_ZU:
        row = up ? 3 : 6;
        column = 0 + 2 * times;
        break;
    case HONG_PAO:
    case HEI_PAO:
        row = up ? 2 : 7;
        column = 1 + 6 * times;
        break;
    case HONG_CHE:
    case HEI_CHE:
        row = up ? 0 : 9;
        column = 0 + 8 * times;
        break;
    case HONG_MA:
    case HEI_MA:
        row = up ? 0 : 9;
        column = 1 + 6 * times;
        break;
    case HONG_XIANG:
    case HEI_XIANG:
        row = up ? 0 : 9;
        column = 2 + 4 * times;
        break;
    case HONG_SI:
    case HEI_SI:
        row = up ? 0 : 9;
        column = 3 + 2 * times;
        break;
    case HONG_JIANG:
    case HEI_JIANG:
        row = up ? 0 : 9;
        column = 4;
        break;
    default:
        break;
    }
    defaultPos = QPoint(row, column);
    return QPoint(row, column);
}

bool XYQiziWidget::isMovable(int row, int column)
{
    bool yes = false;
    switch (type)
    {
    case HONG_ZU:
    case HEI_ZU:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 1
                && (defaultPos.x() <= 4 && row >= curPos.x()
                    || defaultPos.x() >= 5 && row <= curPos.x()) )
        {
            yes = true;
        }
        break;
    case HONG_PAO:
    case HEI_PAO:
        if (row == curPos.x())
        {
            int fuhao = column > curPos.y() ? 1 : -1;
            int count = qAbs(column - curPos.y());
            int obstacleTimes = 0;
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x(), curPos.y() + fuhao * i);
                if (obstacleQizi != NULL)
                {
                    obstacleTimes++;
                }
            }
            XYQiziWidget *lastQizi = XYQipanWidget::getInstance()
                    ->getPositionQizi(row, column);
            if ((obstacleTimes == 1 && lastQizi != NULL)
                    || (obstacleTimes == 0 && lastQizi == NULL))
            {
                yes = true;
            }
        }
        else if (column == curPos.y())
        {
            int fuhao = row > curPos.x() ? 1 : -1;
            int count = qAbs(row - curPos.x());
            int obstacleTimes = 0;
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x() + fuhao * i, curPos.y());
                if (obstacleQizi != NULL)
                {
                    obstacleTimes++;
                }
            }
            XYQiziWidget *lastQizi = XYQipanWidget::getInstance()
                    ->getPositionQizi(row, column);
            if ((obstacleTimes == 1 && lastQizi != NULL)
                    || (obstacleTimes == 0 && lastQizi == NULL))
            {
                yes = true;
            }
        }
        break;
    case HONG_CHE:
    case HEI_CHE:
        if (row == curPos.x())
        {
            int fuhao = column > curPos.y() ? 1 : -1;
            int count = qAbs(column - curPos.y());
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x(), curPos.y() + fuhao * i);
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        else if (column == curPos.y())
        {
            int fuhao = row > curPos.x() ? 1 : -1;
            int count = qAbs(row - curPos.x());
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x() + fuhao * i, curPos.y());
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        break;
    case HONG_MA:
    case HEI_MA:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 3)
        {
            // 判断是否有挡路棋子
            if (qAbs(row - curPos.x()) == 2)
            {
                int fuhao = row > curPos.x() ? 1 : -1;
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x() + fuhao, curPos.y());
                if (obstacleQizi != NULL)
                {
                    return false;
                }

            }
            else if (qAbs(column - curPos.y()) == 2)
            {
                int fuhao = column > curPos.y() ? 1 : -1;
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x(), curPos.y() + fuhao);
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        break;
    case HONG_XIANG:
    case HEI_XIANG:
        if (qAbs(row - curPos.x()) == 2
                && qAbs(column - curPos.y()) == 2
                && (row <= 4 && curPos.x() <= 4
                    || row >= 5 && curPos.x() >= 5) )
        {
            // 判断是否有挡路棋子
            XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                    ->getPositionQizi((curPos.x() + row)/2, (curPos.y() + column)/2);
            if (obstacleQizi != NULL)
            {
                return false;
            }
            yes = true;
        }
        break;
    case HONG_SI:
    case HEI_SI:
        if (qAbs(row - curPos.x()) == 1
                && qAbs(column - curPos.y()) == 1
                && (column >= 3 && column <= 5)
                && (row >= 7 || row <= 2) )
        {
            yes = true;
        }
        break;
    case HONG_JIANG:
    case HEI_JIANG:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 1
                && (column >= 3 && column <= 5)
                && (row >= 7 || row <= 2))
        {
            yes = true;
        }
        else
        {
            int fuhao = defaultPos.x() > 5 ? -1 : 1;
            for (int i = 1; i <= 9
                 && (curPos.x() + fuhao * i >= 0
                     && curPos.x() + fuhao * i <= 9); ++i)
            {
                XYQiziWidget *obstacleQizi = XYQipanWidget::getInstance()
                        ->getPositionQizi(curPos.x() + fuhao * i, curPos.y());
                if (obstacleQizi != NULL)
                {
                    if (obstacleQizi->getType() == HEI_JIANG
                            || obstacleQizi->getType() == HONG_JIANG)
                    {
                        yes = true;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    // 同一方的棋子不能覆盖
    XYQiziWidget *lastQizi = XYQipanWidget::getInstance()->getPositionQizi(row, column);

    if (lastQizi != NULL && (lastQizi->getType() <= HONG_JIANG && type <= HONG_JIANG
            || lastQizi->getType() > HONG_JIANG && type > HONG_JIANG))
    {
        yes = false;
    }
    return yes;
}

void XYQiziWidget::setType(XYQiziWidget::TYPE type, int times)
{
    this->type = type;
    this->times = times;
    getPixMapByType(type, true);
}

XYQiziWidget::TYPE XYQiziWidget::getType() const
{
    return this->type;
}

void XYQiziWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawPixmap(rect(), getPixMapByType(type));
}

void XYQiziWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
    raise();
}

void XYQiziWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
        XYQipanWidget::getInstance()->moveToNearestPos(this);
    }
}

void XYQiziWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftMousePressed)
    {
        QPoint lastpos(x(), y());
        lastpos.setX( lastpos.x() + event->globalX() - moLastPos.x());
        lastpos.setY( lastpos.y() + event->globalY() - moLastPos.y());
        move(lastpos);
        moLastPos = event->globalPos();
    }
}

QPixmap XYQiziWidget::getPixMapByType(XYQiziWidget::TYPE type, bool force)
{
    if (force || this->pixmap.isNull())
    {
        QPixmap pixmap;
        switch (type)
        {
        case HONG_ZU:
            pixmap.load(":/xiangqi/hong_zu.png");
            break;
        case HONG_PAO:
            pixmap.load(":/xiangqi/hong_pao.png");
            break;
        case HONG_CHE:
            pixmap.load(":/xiangqi/hong_che.png");
            break;
        case HONG_MA:
            pixmap.load(":/xiangqi/hong_ma.png");
            break;
        case HONG_XIANG:
            pixmap.load(":/xiangqi/hong_xiang.png");
            break;
        case HONG_SI:
            pixmap.load(":/xiangqi/hong_si.png");
            break;
        case HONG_JIANG:
            pixmap.load(":/xiangqi/hong_jiang.png");
            break;
        case HEI_ZU:
            pixmap.load(":/xiangqi/hei_zu.png");
            break;
        case HEI_PAO:
            pixmap.load(":/xiangqi/hei_pao.png");
            break;
        case HEI_CHE:
            pixmap.load(":/xiangqi/hei_che.png");
            break;
        case HEI_MA:
            pixmap.load(":/xiangqi/hei_ma.png");
            break;
        case HEI_XIANG:
            pixmap.load(":/xiangqi/hei_xiang.png");
            break;
        case HEI_SI:
            pixmap.load(":/xiangqi/hei_si.png");
            break;
        case HEI_JIANG:
            pixmap.load(":/xiangqi/hei_jiang.png");
            break;
        default:
            break;
        }
        this->pixmap = pixmap;
    }

    return this->pixmap;
}
QPoint XYQiziWidget::getCurPos() const
{
    return curPos;
}

void XYQiziWidget::setCurPos(const QPoint &value)
{
    curPos = value;
}

bool XYQiziWidget::getBeEaten() const
{
    return beEaten;
}

void XYQiziWidget::setBeEaten(bool beEaten)
{
    if (beEaten)
    {
        setVisible(false);
    }
    this->beEaten = beEaten;
}

void XYQiziWidget::resizeQizi(const QSize &size)
{
    static QPixmap qipan(":/xiangqi/qipan.png");
    qreal ratio = 0.9 * size.width() / qipan.size().width();
    QPixmap qizi = getPixMapByType(type);
    resize(qizi.size() * ratio);

    XYQipanWidget::getInstance()->putQizi(this, curPos.x(), curPos.y());
}
