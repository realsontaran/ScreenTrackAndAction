#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QImage>
#include <QWidget>
#include <qnamespace.h>

class ScribbleArea : public QWidget {
  Q_OBJECT
public:
  ScribbleArea(QWidget *parent = nullptr);
  ~ScribbleArea();
  bool isModified() const { return modified; }
  QColor penColor() const { return myPenColor; }
  int penWidth() const { return myPenWidth; }
  QImage getImage() const { return this->image; }
public slots:
  void clearImage();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  void drawLineTo(const QPoint &endPoint);
  void resizeImage(QImage *image, const QSize &newSize);

  bool modified = false;
  bool scribbling = false;
  int myPenWidth = 20;
  QColor myPenColor = Qt::black;
  QImage image;
  QPoint lastPoint;
};

#endif // SCRIBBLEAREA_H
