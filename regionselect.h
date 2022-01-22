#ifndef SCREENSELECT_H
#define SCREENSELECT_H

#include <QLabel>
#include <QMenu>
#include <QMouseEvent>

class RegionSelect : public QLabel {
  Q_OBJECT
public:
  RegionSelect(QWidget *parent = nullptr);
  ~RegionSelect();
  QRect getRect() { return rect; };

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  bool selStart;
  QRect rect;
};

#endif // SCREENSELECT_H
