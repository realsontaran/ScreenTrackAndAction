#ifndef CAPTUREMOUSE_H_
#define CAPTUREMOUSE_H_
#include <QCursor>
#include <QRect>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <QImage>
class CaptureMouse {
public:
  CaptureMouse();
  ~CaptureMouse();
  bool captureMouse(QImage &image,QRect rect);

private:
  Display *dis;
  Window win;
};

#endif // CAPTUREMOUSE_H_
