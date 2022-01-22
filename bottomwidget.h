#ifndef BOTTOMWIDGET_H_
#define BOTTOMWIDGET_H_
#include <QColor>
#include <QColorDialog>
#include <QWidget>
#include <qboxlayout.h>
#include <qimage.h>
#include "scribblearea.h"
class BottomWidget : public QWidget {
  Q_OBJECT
public:
  BottomWidget(bool left = false, QWidget *parent = nullptr);
  ~BottomWidget();
  void init(int index);
  QHBoxLayout *layout;
  QString get_str() { return str; }
  QImage get_gesture() { return gestureImage; }
  void update_data(int index);

private:
  QWidget *wid;
  bool isLeft;
  QString str;
  ScribbleArea *scribble;
  QImage gestureImage;
};

#endif // BOTTOMWIDGET_H_
