#include "capturemouse.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <QColor>
#include <QPainter>

CaptureMouse::CaptureMouse() {
  dis = XOpenDisplay(nullptr);
  if (!dis) {
    perror("Open Display: ");
    exit(EXIT_FAILURE);
  }
  win = XDefaultRootWindow(dis);
}

CaptureMouse::~CaptureMouse() { free(dis); }

bool CaptureMouse::captureMouse(QImage &im,QRect rect) {
  XEvent event;
  Status status;
  bool correct = false;
  im = QImage(rect.width(),rect.height(),QImage::Format_RGB32);
  im.fill(Qt::white);
  QPainter painter(&im);
  painter.setPen(QPen(Qt::black,10,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
  painter.setRenderHint(QPainter::Antialiasing);

  Cursor cursor = XCreateFontCursor(dis, XC_crosshair);
  status =
      XGrabPointer(dis, win, True,
                   Button1MotionMask| ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                       FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                   GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
  if (status != GrabSuccess) {
    perror("Grab Pointer: ");
    XFlush(dis);
    XUngrabPointer(dis, CurrentTime);
    XFreeCursor(dis, cursor);
    XSync(dis, 1);
    return false;
  }
  bool cnt = true;
  bool btn_hold = false;
  while (cnt ) {
    XNextEvent(dis, &event);
    switch (event.type) {
    case ButtonPress:
      btn_hold = true;
      break;
    case MotionNotify:
      if (btn_hold) {
        painter.drawPoint(event.xbutton.x_root - rect.x(),event.xbutton.y_root - rect.y());
      }
      break;
    case ButtonRelease:
      painter.end();
      correct = true;
      cnt = false;
    }
    if(!rect.contains(QCursor::pos()))
        cnt=false;
  }
  XFlush(dis);
  XUngrabPointer(dis, CurrentTime);
  XFreeCursor(dis, cursor);
  XSync(dis, 1);
  return correct;
}
