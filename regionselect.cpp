#include "regionselect.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>

RegionSelect::RegionSelect(QWidget *parent) : QLabel(parent)
{
    selStart = false;
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAlignment(Qt::AlignCenter);
    setParent(0);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

RegionSelect::~RegionSelect() {}

void RegionSelect::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0, 180)), 3, Qt::SolidLine));
    painter.setBrush(QBrush(QColor(255, 255, 255, 120)));
    painter.drawRect(rect);
}

void RegionSelect::mousePressEvent(QMouseEvent *event)
{
    selStart = true;
    rect.setTopLeft(event->pos());
    rect.setBottomRight(event->pos());
}

void RegionSelect::mouseMoveEvent(QMouseEvent *event)
{
    if (selStart)
    {
        rect.setBottomRight(event->pos());
        repaint();
    }
}

void RegionSelect::mouseReleaseEvent(QMouseEvent *event)
{
    selStart = false;
    this->hide();
}
