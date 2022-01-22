#include "scribblearea.h"
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_StaticContents);
  image.fill(Qt::white);
}

ScribbleArea::~ScribbleArea() {}

void ScribbleArea::clearImage() {
  image.fill(Qt::white);
  modified = true;
  update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    lastPoint = event->pos();
    scribbling = true;
  }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton) && scribbling)
    drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && scribbling) {
    drawLineTo(event->pos());
    scribbling = false;
  }
  this->hide();
}

void ScribbleArea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event) {
  if (width() > image.width() || height() > image.height()) {
    int newWidth = qMax(width() + 128, image.width());
    int newHeight = qMax(height() + 128, image.height());
    resizeImage(&image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint) {
  QPainter painter(&image);
  painter.setPen(
      QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(lastPoint, endPoint);
  modified = true;

  int rad = (myPenWidth / 2) + 2;
  update(
      QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
  lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize) {
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}
