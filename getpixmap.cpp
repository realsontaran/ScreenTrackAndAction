#include "getpixmap.h"
#include <QDebug>
#include <QFile>

GetPixmap::GetPixmap() {}
GetPixmap::~GetPixmap() {}
QPixmap GetPixmap::get_region(QRect rect) {
  auto screens = QGuiApplication::screens();
  QList<QPixmap> scrs;
  int w = 0, h = 0, p = 0;
  foreach (auto scr, screens) {
    QPixmap pix = scr->grabWindow(0);
    w += pix.width();
    if (h < pix.height())
      h = pix.height();

    scrs << pix;
  }
  QPixmap screenPx = QPixmap(w, h);
  QPainter painter(&screenPx);
  screenPx.fill(Qt::black);
  foreach (auto scr, scrs) {
    painter.drawPixmap(QPoint(p, 0), scr);
    p += scr.width();
  }
  return screenPx.copy(rect);
}
