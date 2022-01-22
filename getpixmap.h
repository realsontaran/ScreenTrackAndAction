#ifndef GETPIXMAP_H
#define GETPIXMAP_H

#include <QWidget>
#include <QRect>
#include <QGuiApplication>
#include <QWindow>
#include <QPainter>
#include <QScreen>

class GetPixmap
{
public:
    GetPixmap();
    ~GetPixmap();
    QPixmap get_region(QRect rect);
};

#endif // GETPIXMAP_H
